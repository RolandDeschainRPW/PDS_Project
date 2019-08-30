//
// Created by raffa on 24/08/2019.
//

#include "../include/Request.h"

Request::Request() { /* empty */ }

Request::Request(const Request& req) : siteId(req.siteId),
                                       requestType(req.requestType),
                                       username(req.username),
                                       password(req.password),
                                       filename(req.filename) {
    if (req.requestType == MESSAGE_TYPE) this->msg = req.msg;
}

Request::Request(qint32 siteId,
                 qint32 requestType,
                 std::optional<Message> opt_msg,
                 QString username,
                 QString password,
                 QString filename) : siteId(siteId),
                                     requestType(requestType),
                                     username(username),
                                     password(password),
                                     filename(filename){
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

QString Request::getUsername() const {
    return username;
}

QString Request::getPassword() const {
    return password;
}

QString Request::getFilename() const {
    return filename;
}

QDataStream &operator<<(QDataStream &out, const Request& req) {
    out << req.getSiteId() << req.getRequestType() << req.getUsername() << req.getPassword() << req.getFilename();
    if (req.getRequestType() == Request::MESSAGE_TYPE)
        out << req.getMessage();
    return out;
}

QDataStream &operator>>(QDataStream &in, Request& req) {
    qint32 siteId;
    qint32 request_type;
    QString username;
    QString password;
    QString filename;
    Message msg;

    in >> siteId >> request_type >> username >> password >> filename;
    if (request_type == Request::MESSAGE_TYPE) {
        in >> msg;
        req = Request(siteId, request_type, msg, username, password);
        return in;
    }
    req = Request(siteId, request_type, std::nullopt, username, password, filename);
    return in;
}

