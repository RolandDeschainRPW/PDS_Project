//
// Created by raffa on 12/09/2019.
//

#include "../include/Collaborator.h"

Collaborator::Collaborator() { /* empty */ }

Collaborator::Collaborator(const Collaborator& collaborator) : nickname(collaborator.nickname),
                                                         profile_pic(collaborator.profile_pic),
                                                         siteId(collaborator.siteId),
                                                         cursor_pos(collaborator.cursor_pos) { /* empty */ }

Collaborator::Collaborator(QString nickname,
                           QImage profile_pic,
                           qint32 siteId,
                           qint32 cursor_pos) : nickname(nickname),
                                            profile_pic(profile_pic),
                                            siteId(siteId),
                                            cursor_pos(cursor_pos) { /* empty */ }

Collaborator::~Collaborator() { /* empty */ }

QString Collaborator::getNickname() const {
    return nickname;
}

QImage Collaborator::getProfilePic() const {
    return profile_pic;
}

qint32 Collaborator::getSiteId() const {
    return siteId;
}

qint32 Collaborator::getCursorPos() const {
    return cursor_pos;
}

QDataStream &operator<<(QDataStream &out, const Collaborator& collaborator) {
    out << collaborator.getNickname() << collaborator.getProfilePic() << collaborator.getSiteId() << collaborator.getCursorPos();
    return out;
}

QDataStream &operator>>(QDataStream &in, Collaborator& collaborator) {
    QString nickname;
    QImage profile_pic;
    qint32 siteId;
    qint32 cursor_pos;

    in >> nickname >> profile_pic >> siteId >> cursor_pos;
    collaborator = Collaborator(nickname, profile_pic, siteId, cursor_pos);

    return in;
}
