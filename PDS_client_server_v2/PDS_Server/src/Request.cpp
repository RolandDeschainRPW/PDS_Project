//
// Created by raffa on 21/08/2019.
//

#include "Request.h"

Request::Request() { /* empty */ }

Request::Request(const Request& req) {
    this->siteId = req.siteId;
    this->requestType = req.requestType;
    if (req.requestType == MESSAGE_TYPE)
        this->msg = req.msg;
}

Request::Request(qint32 siteId, qint32 requestType, std::optional<Message> opt_msg) {
    this->siteId = siteId;
    this->requestType = requestType;
    if (opt_msg.has_value()) msg = opt_msg.value();
}

Request::~Request() { /* empty */ }

qint32 Request::getSiteId() const {
    return siteId;
}

qint32 Request::getRequestType() const {
    return requestType;
}

// BE CAREFUL! "msg" could not be initialized if request_type == DISCONNECT_TYPE!
Message Request::getMessage() const {
    return msg;
}

QDataStream &operator<<(QDataStream &out, const Request& req) {
    out << req.getSiteId() << req.getRequestType();
    if (req.getRequestType() == Request::MESSAGE_TYPE)
        out << req.getMessage();
    return out;
}

QDataStream &operator>>(QDataStream &in, Request& req) {
    qint32 siteId;
    qint32 request_type;
    Message msg;

    in >> siteId >> request_type;
    if (request_type == Request::MESSAGE_TYPE) {
        in >> msg;
        req = Request(siteId, request_type, msg);
        return in;
    }
    req = Request(siteId, request_type, std::nullopt);
    return in;
}

