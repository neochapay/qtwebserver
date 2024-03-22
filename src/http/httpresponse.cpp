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

// Qt includes
#include <QStringBuilder>

// Own includes
#include "httpresponse.h"

namespace QtWebServer {

namespace Http {

    Response::Response()
        : Logger("WebServer::Http::Response")
    {
        m_statusCode = Http::Ok;
        m_body = "";
    }

    QByteArray Response::toByteArray()
    {
        QByteArray response = "";

        // HTTP response header line.
        response += QString("HTTP/1.1 %1 %2\r\n")
                        .arg(m_statusCode)
                        .arg(Http::reasonPhrase(m_statusCode))
                        .toUtf8();

        // Append HTTP headers.
        QStringList headerNames = m_headers.keys();
        foreach (QString headerName, headerNames) {
            response += QString("%1: %2\r\n")
                            .arg(headerName)
                            .arg(m_headers.value(headerName))
                            .toUtf8();
        }

        // Add empty line to mark the end of the header.
        response += "\r\n";

        // Append the response body.
        response += m_body;

        return response;
    }

    Http::StatusCode Response::statusCode()
    {
        return m_statusCode;
    }

    void Response::setStatusCode(Http::StatusCode statusCode)
    {
        m_statusCode = statusCode;
    }

    QByteArray Response::body()
    {
        return m_body;
    }

    void Response::setBody(QByteArray body)
    {
        m_body = body;
    }

    void Response::setHeader(Header header, QString headerValue)
    {
        setHeader(headerName(header), headerValue);
    }

    void Response::setHeader(QString headerName, QString headerValue)
    {
        m_headers.insert(headerName, headerValue);
    }

    QString Response::header(Header header) const
    {
        return this->header(headerName(header));
    }

    QString Response::header(QString headerName) const
    {
        return m_headers.value(headerName);
    }

} // namespace Http

} // namespace QtWebServer
