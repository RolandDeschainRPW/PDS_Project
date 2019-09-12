//
// Created by raffa on 12/09/2019.
//

#include "../include/Collaborator.h"

Collaborator::Collaborator() { /* empty */ }

Collaborator::Collaborator(const Collaborator& collaborator) : nickname(collaborator.nickname),
                                                         profile_pic(collaborator.profile_pic),
                                                         siteId(collaborator.siteId) { /* empty */ }

Collaborator::Collaborator(QString nickname,
                           QImage profile_pic,
                           qint32 siteId) : nickname(nickname),
                                            profile_pic(profile_pic),
                                            siteId(siteId) { /* empty */ }

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

QDataStream &operator<<(QDataStream &out, const Collaborator& collaborator) {
    out << collaborator.getNickname() << collaborator.getProfilePic() << collaborator.getSiteId();
    return out;
}

QDataStream &operator>>(QDataStream &in, Collaborator& collaborator) {
    QString nickname;
    QImage profile_pic;
    qint32 siteId;

    in >> nickname >> profile_pic >> siteId;
    collaborator = Collaborator(nickname,profile_pic, siteId);

    return in;
}