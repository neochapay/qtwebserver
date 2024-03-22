//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with QtWebServer.
// If not, see <http://www.gnu.org/licenses/>.
//
// It is possible to obtain a commercial license of QtWebServer.
// Please contact Jacob Dawid <jacob@omg-it.works>
//

// Own includes
#include "httprequest.h"
#include "util/utilformurlcodec.h"

// Qt includes
#include <QStringList>
#include <QRegularExpression>

namespace QtWebServer {

namespace Http {

Request::Request()
    : Logger("WebServer::Http::Request") {
    setDefaults();
}

Request::Request(const QByteArray &rawRequest)
    : Logger("WebServer::Http::Request") {
    setDefaults();
    deserialize(rawRequest);
}

bool Request::isValid() const {
    return _valid;
}

QString Request::method() const {
    return _method;
}

QString Request::uniqueResourceIdentifier() const {
    return _uniqueResourceIdentifier;
}

QString Request::version() const {
    return _version;
}

QMap<QString, QByteArray> Request::urlParameters() const {
    return _urlParameters;
}

QMap<QString, QString> Request::headers() const {
    return _headers;
}

QString Request::header(Header header) const {
    return this->header(headerName(header));
}

QString Request::header(QString headerName) const {
    return _headers.value(headerName);
}

QMap<QString, QString> Request::getParameters() const{
    return _getParameters;
}

QMap<QString, QString> Request::postParameters() const {
    return _postParameters;
}

QByteArray Request::body() const {
    return _body;
}

void Request::appendBodyData(QByteArray bodyData) {
    _body.append(bodyData);
}

bool Request::isComplete() const {
    if(_headers.contains(headerName(ContentLength))) {
        bool contentLengthValid = false;
        long long contentLength = _headers.value(headerName(ContentLength)).toLongLong(&contentLengthValid);
        if(contentLengthValid) {
            return _body.count() == contentLength;
        }
    }

    // TODO: Chunked transfer mode here.

    // If there is nothing indicating the content length
    // and no information about chunked transfer mode then
    // we have to assume the requesrespondt is complete
    return true;
}

QByteArray Request::takeLine(QByteArray& rawRequest) {
    // Lines in the HTTP protocol are defined to be separated by '\r\n'
    QByteArray line;
    int rawSize = rawRequest.size();
    int rawPosition;

    // Watch out for the first occurrence of '\r'
    for(rawPosition = 0; rawPosition < rawSize - 1; rawPosition++) {
        if(rawRequest[rawPosition] == '\r' && rawRequest[rawPosition + 1] == '\n') {
            break;
        }
    }

    // If there is just two characters left, this must be EOF
    if(rawPosition != rawSize - 2) {
        // Get the line contents
        line = rawRequest.left(rawPosition);

        // Get the remaining data, skipping '\r\n'
        rawRequest = rawRequest.right(rawSize - line.count() - 2);
    } else {
        // EOF
        rawRequest = "";
        line = rawRequest;
    }

    return line;
}

void Request::setDefaults() {
    _headers.clear();
    _getParameters.clear();
    _postParameters.clear();
    _urlParameters.clear();
    _valid = false;
    _method = "";
    _uniqueResourceIdentifier = "";
    _version = "";
    _body = "";
}

void Request::deserialize(QByteArray rawRequest) {
    // Read ahead the first line in the request
    QByteArray rawRequestLine = takeLine(rawRequest);
    QStringList requestLine = QString::fromUtf8(rawRequestLine)
                                .split(QRegularExpression("\\s+"));

    QStringList rawRequestLines = QString::fromUtf8(rawRequest).split("\n");

    if(requestLine.count() < 3) {
        // The request line has to contain three strings: The method
        // string, the request uri and the HTTP version. If we were
        // strict, we shouldn't even accept anything larger than four
        // strings, but we're permissive here.
        return;
    }

    _method = requestLine.at(0).toLower();

    //add post paremeters
    if(_method == "post")
    {
        log("We get POST!");
        QString postLine = rawRequestLines.at(rawRequestLines.count()-1);
        if(!postLine.isEmpty())
        {
            QStringList postData = postLine.split('&', Qt::SkipEmptyParts);
            for(int i=0; i < postData.count(); i++)
            {
                QStringList post = postData.at(i).split('=', Qt::SkipEmptyParts);
                if(post.count() == 2)
                {
                    _postParameters.insert(post.at(0),post.at(1));
                }
            }
        }
    }

    QStringList splittedURI = requestLine.at(1).split('?', Qt::SkipEmptyParts);
    if(splittedURI.count() > 1) {
        _urlParameters = Util::FormUrlCodec::decodeFormUrl(splittedURI.at(1).toUtf8());
        // add get parameters
        QStringList getData = splittedURI.at(1).split('&', Qt::SkipEmptyParts);
        for(int i=0; i < getData.count(); i++)
        {
            QStringList get = getData.at(i).split('=', Qt::SkipEmptyParts);
            if(get.count() == 2)
            {
                _getParameters.insert(get.at(0), get.at(1));
            }
        }
    }

    _uniqueResourceIdentifier = splittedURI.at(0);

    _version = requestLine.at(2);

    QByteArray nextLine;
    while(!(nextLine = takeLine(rawRequest)).isEmpty()) {
        deserializeHeader(nextLine);
    }

    // By definition, all that follows after a \r\n\r\n is the body of the request.
    _body = QByteArray(rawRequest);
    _valid = true;
}

void Request::deserializeHeader(const QByteArray& rawHeader) {
    QString headerLine = QString::fromUtf8(rawHeader);
    int colonPosition;
    int headerLineLength = headerLine.count();
    for(colonPosition = 0; colonPosition < headerLineLength; colonPosition++) {
        if(headerLine.at(colonPosition) == ':') {
            break;
        }
    }

    if(colonPosition == headerLineLength) {
        log(QString("Invalid header line found %1").arg(headerLine), Log::Warning);
        return;
    }

    QString headerName = headerLine.left(colonPosition);
    QString headerValue = headerLine.right(headerLineLength - colonPosition - 1).trimmed();
    _headers.insert(headerName, headerValue);
}

} // namespace Http

} // namespace QtWebServer
