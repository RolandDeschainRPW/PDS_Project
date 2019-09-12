//
// Created by raffa on 12/09/2019.
//

#ifndef PDS_SERVER_COLLABORATOR_H
#define PDS_SERVER_COLLABORATOR_H

#include <QtCore>
#include <QImage>

class Collaborator {
public:
    Collaborator();
    Collaborator(const Collaborator& collaborator);
    Collaborator(QString nickname,
                 QImage profile_pic,
                 qint32 site_id);
    ~Collaborator();

    QString getNickname() const;
    QImage getProfilePic() const;
    qint32 getSiteId() const;

private:
    QString nickname;
    QImage profile_pic;
    qint32 siteId;
};

QDataStream &operator<<(QDataStream &out, const Collaborator& collaborator);
QDataStream &operator>>(QDataStream &in, Collaborator& collaborator);

#endif //PDS_SERVER_COLLABORATOR_H
