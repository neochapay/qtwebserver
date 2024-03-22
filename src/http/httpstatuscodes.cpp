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
#include "httpstatuscodes.h"

namespace QtWebServer {

namespace Http {

    ReasonPhrasePair reasonPhrasePairMap[STATUS_CODE_COUNT] = {
        { Continue, "Continue" },
        { SwitchingProtocols, "Switching Protocols" },

        { Ok, "Ok" },
        { Created, "Created" },
        { Accepted, "Accepted" },
        { NonAuthoritativeInformation, "Non-Authoritative Information" },
        { NoContent, "No Content" },
        { ResetContent, "Reset Content" },
        { PartialContent, "Partial Content" },

        { MultipleChoices, "Multiple Choices" },
        { MovedPermanently, "Moved Permanently" },
        { Found, "Found" },
        { SeeOther, "See Other" },
        { NotModified, "Not Modified" },
        { UseProxy, "Use Proxy" },
        { Unused, "Unused" },
        { TemporaryRedirect, "Temporary Redirect" },

        { BadRequest, "Bad Request" },
        { Unauthorized, "Unauthorized" },
        { PaymentRequired, "Payment Required" },
        { Forbidden, "Forbidden" },
        { NotFound, "Not Found" },
        { MethodNotAllowed, "Method Not Allowed" },
        { NotAcceptable, "Not Acceptable" },
        { ProxyAuthenticationRequired, "Proxy Authentication Required" },
        { RequestTimeout, "Request Time-out" },
        { Conflict, "Conflict" },
        { Gone, "Gone" },
        { LengthRequired, "Length Required" },
        { PreconditionFailed, "Precondition Failed" },
        { RequestEntityTooLarge, "Request Entity Too Large" },
        { RequestURITooLong, "Request-URI Too Long" },
        { UnsupportedMediaType, "Unsupported Media Type" },
        { RequestedRangeNotSatisfiable, "Requested range not satisfiable" },
        { ExpectationFailed, "Expectation Failed" },

        { InternalServerError, "Internal Server Error" },
        { NotImplemented, "Not Implemented" },
        { BadGateway, "Bad Gateway" },
        { ServiceUnavailable, "Service Unavailable" },
        { GatewayTimeout, "Gateway Time-out" },
        { HTTPVersionNotSupported, "HTTP Version not supported" }
    };

    QString reasonPhrase(Http::StatusCode statusCode)
    {
        for (int i = 0; i < STATUS_CODE_COUNT; i++) {
            if (reasonPhrasePairMap[i].statusCode == statusCode) {
                return reasonPhrasePairMap[i].reasonPhrase;
            }
        }
        return "";
    }

} // namespace Http

} // namespace QtWebServer
