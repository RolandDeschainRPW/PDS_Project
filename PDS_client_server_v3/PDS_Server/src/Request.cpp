//
// Created by raffa on 21/08/2019.
//

#include "../include/Request.h"

Request::Request() { /* empty */ }

Request::Request(const Request& req) : siteId(req.siteId),
                                       requestType(req.requestType),
                                       username(req.username),
                                       password(req.password),
                                       filename(req.filename),
                                       counter(req.counter),
                                       nickname(req.nickname),
                                       image_format(req.image_format) {
    if (req.requestType == MESSAGE_TYPE) this->msg = req.msg;
    if (req.requestType == SIGN_UP_TYPE) this->profile_pic = req.profile_pic;
}

Request::Request(qint32 siteId,
                 qint32 requestType,
                 std::optional<Message> opt_msg,
                 QString username,
                 QString password,
                 QString filename,
                 quint32 counter,
                 QString nickname,
                 std::optional<QImage> opt_img,
                 QString image_format) : siteId(siteId),
                                     requestType(requestType),
                                     username(username),
                                     password(password),
                                     filename(filename),
                                     counter(counter),
                                     nickname(nickname),
                                     image_format(image_format) {
    if (opt_msg.has_value()) msg = opt_msg.value();
    if (opt_img.has_value()) profile_pic = opt_img.value();
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

quint32 Request::getCounter() const {
    return counter;
}

QString Request::getNickname() const {
    return nickname;
}

QImage Request::getProfilePic() const {
    return profile_pic;
}

QString Request::getImageFormat() const {
    return image_format;
}

QDataStream &operator<<(QDataStream &out, const Request& req) {
    out << req.getSiteId() << req.getRequestType() << req.getUsername() << req.getPassword() << req.getFilename() << req.getCounter() << req.getNickname() << req.getImageFormat();
    if (req.getRequestType() == Request::MESSAGE_TYPE)
        out << req.getMessage();
    if (req.getRequestType() == Request::SIGN_UP_TYPE)
        out << req.getProfilePic();
    return out;
}

QDataStream &operator>>(QDataStream &in, Request& req) {
    qint32 siteId;
    qint32 request_type;
    QString username;
    QString password;
    QString filename;
    quint32 counter;
    QString nickname;
    Message msg;
    QImage profile_pic;
    QString image_format;

    in >> siteId >> request_type >> username >> password >> filename >> counter >> nickname >> image_format;

    if (request_type == Request::MESSAGE_TYPE) {
        in >> msg;
        req = Request(siteId, request_type, msg, username, password, filename, counter, nickname);
        return in;
    }

    if (request_type == Request::SIGN_UP_TYPE) {
        in >> profile_pic;
        req = Request(siteId, request_type, msg, username, password, filename, counter, nickname, profile_pic, image_format);
        return in;
    }

    req = Request(siteId, request_type, std::nullopt, username, password, filename, counter, nickname);
    return in;
}