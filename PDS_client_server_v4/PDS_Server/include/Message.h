//
// Created by raffa on 21/08/2019.
//

#ifndef PDS_SERVER_MESSAGE_H
#define PDS_SERVER_MESSAGE_H

#include <optional>

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <QDataStream>
#include <QImage>

#include "../include/Symbol.h"

class Message {
private:
    qint32 _siteId;
    qint32 type;
    Symbol s;
    QString incoming_nickname;
    QImage incoming_user_pic;
    qint32 cursor_position;

public:
    static const int INSERT_TYPE = -1;
    static const int ERASE_TYPE = -2;
    static const int NOTIFY_CONNECTION = -3;
    static const int NOTIFY_DISCONNECTION = -4;
    static const int CURSOR_POS_CHANGED_TYPE = -5;

    Message();
    Message(qint32 _siteId,
            qint32 type,
            std::optional<Symbol> opt_s = std::nullopt,
            std::optional<QImage> opt_incoming_user_pic = std::nullopt,
            QString incoming_nickname = "",
            qint32 cursor_position = -1);
    Message(const Message& m);
    ~Message();

    Message& operator=(const Message& m);
    qint32 getSiteId() const;
    qint32 getType() const;
    Symbol getSymbol() const;
    QString getIncomingNickname() const;
    QImage getIncomingUserPic() const;
    qint32 getCursorPosition() const;
};

QDataStream &operator<<(QDataStream &out, const Message& msg);
QDataStream &operator>>(QDataStream &in, Message& msg);

#endif //PDS_SERVER_MESSAGE_H
