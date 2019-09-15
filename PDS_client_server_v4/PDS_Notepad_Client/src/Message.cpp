//
// Created by raffa on 24/08/2019.
//

#include "../include/Message.h"

Message::Message() { /* empty */ }

Message::Message(qint32 _siteId,
                 qint32 type,
                 std::optional<Symbol> opt_s,
                 std::optional<QImage> opt_incoming_user_pic,
                 QString incoming_nickname,
                 qint32 cursor_position) : _siteId(_siteId),
                                              type(type),
                                              incoming_nickname(incoming_nickname),
                                              cursor_position(cursor_position){
    if (opt_s.has_value()) s = opt_s.value();
    if (opt_incoming_user_pic.has_value()) incoming_user_pic = opt_incoming_user_pic.value();
}

Message::Message(const Message& m) : _siteId(m._siteId),
                                     type(m.type),
                                     incoming_nickname(m.incoming_nickname),
                                     cursor_position(m.cursor_position) {
    if (m.type == INSERT_TYPE || m.type == ERASE_TYPE) this->s = m.s;
    if (m.type == NOTIFY_CONNECTION) this->incoming_user_pic = m.incoming_user_pic;
}

Message::~Message() { /* empty */ }

qint32 Message::getSiteId() const {
    return _siteId;
}

qint32 Message::getType() const {
    return type;
}

Symbol Message::getSymbol() const {
    return s;
}

QString Message::getIncomingNickname() const {
    return incoming_nickname;
}

QImage Message::getIncomingUserPic() const {
    return incoming_user_pic;
}

qint32 Message::getCursorPosition() const {
    return cursor_position;
}

Message& Message::operator=(const Message& m) {
    if (this != &m) {
        this->_siteId = m._siteId;
        this->type = m.type;
        this->incoming_nickname = m.incoming_nickname;
        this->s = m.s;
        this->incoming_user_pic = m.incoming_user_pic;
        this->cursor_position = m.cursor_position;
    }
    return *this;
}

QDataStream &operator<<(QDataStream &out, const Message& msg) {
    out << msg.getType() << msg.getSiteId() << msg.getSymbol() << msg.getIncomingNickname() << msg.getIncomingUserPic() << msg.getCursorPosition();
    return out;
}

QDataStream &operator>>(QDataStream &in, Message& msg) {
    qint32 type;
    qint32 _siteId;
    Symbol s;
    QString nickname;
    QImage image;
    qint32 cursor_position;

    in >> type >> _siteId >> s >> nickname >> image >> cursor_position;
    msg = Message(_siteId, type, s, image, nickname, cursor_position);
    return in;
}