//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_REQUEST_H
#define PDS_SERVER_REQUEST_H

#include <optional>

#include <QtCore>
#include <QImage>

#include "../include/Message.h"

class Request {
public:
    Request();
    Request(const Request& req);
    Request(qint32 siteId,
            qint32 requestType,
            std::optional<Message> opt_msg = std::nullopt,
            QString username = "",
            QString password = "",
            QString filename = "",
            quint32 counter = 0,
            QString nickname = "",
            std::optional<QImage> opt_img = std::nullopt,
            QString image_format = "");
    ~Request();

    static const qint32 MESSAGE_TYPE = -1;
    static const qint32 DISCONNECT_TYPE = -2;
    static const qint32 CONNECT_TYPE = -3;
    static const qint32 SIGN_UP_TYPE = -4;
    static const qint32 SITE_ID_UNASSIGNED = -9;
    static const qint32 OPEN_DOCUMENT_TYPE = -20;
    static const qint32 NEW_DOCUMENT_TYPE = -21;
    static const qint32 ADD_COLLABORATOR_TYPE = -30;

    qint32 getSiteId() const;
    qint32 getRequestType() const;
    Message getMessage() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getFilename() const;
    quint32 getCounter() const;
    QString getNickname() const;
    QImage getProfilePic() const;
    QString getImageFormat() const;

private:
    qint32 siteId;
    quint32 counter;
    qint32 requestType;
    Message msg;
    QString username;
    QString password;
    QString filename;
    QString nickname;
    QImage profile_pic;
    QString image_format;
};

QDataStream &operator<<(QDataStream &out, const Request& req);
QDataStream &operator>>(QDataStream &in, Request& req);

#endif //PDS_SERVER_REQUEST_H