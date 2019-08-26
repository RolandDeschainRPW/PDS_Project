//
// Created by raffa on 24/08/2019.
//

#ifndef NOTEPAD_REQUEST_H
#define NOTEPAD_REQUEST_H

#include <optional>

#include <QtCore>

#include "../include/Message.h"

class Request {
public:
    Request();
    Request(const Request& req);
    Request(qint32 siteId, qint32 requestType, std::optional<Message> opt_msg);
    ~Request();

    static const qint32 MESSAGE_TYPE = -1;
    static const qint32 DISCONNECT_TYPE = -2;

    qint32 getSiteId() const;
    qint32 getRequestType() const;
    Message getMessage() const;

private:
    qint32 siteId;
    qint32 requestType;
    Message msg;
};

QDataStream &operator<<(QDataStream &out, const Request& req);
QDataStream &operator>>(QDataStream &in, Request& req);

#endif //NOTEPAD_REQUEST_H
