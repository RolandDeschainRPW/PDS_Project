//
// Created by raffa on 29/08/2019.
//

#ifndef PDS_SERVER_RESPONSE_H
#define PDS_SERVER_RESPONSE_H

#include <QtCore>

class Response {
public:
    Response();
    Response(qint32 result, QMap<QString, QString> docs, QString username = "");
    Response(const Response& res);
    ~Response();

    qint32 getResult() const;
    QMap<QString, QString> getDocsList() const;
    QString getUsername() const;

    // For Login purposes.
    static const qint32 WRONG_CREDENTIALS = -5;
    static const qint32 SUCCESSFUL_LOGIN = -6;
    static const qint32 USERNAME_ACTIVE = -22;

    // For SignUp purposes.
    static const qint32 USERNAME_ALREADY_IN_USE = -7;
    static const qint32 USERNAME_ACCEPTED = -8;
    static const qint32 NICKNAME_ALREADY_IN_USE = -18;

    // For adding collaborators.
    static const qint32 NICKNAME_NON_EXISTENT = -50;
    static const qint32 NICKNAME_ACTIVE = -60;

    // For profile modifications.
    static const qint32 PROFILE_MODIFIED = -80;
    static const qint32 PROFILE_PIC_NOT_STORED = -90;
    static const qint32 PASSWORD_NOT_STORED = -100;

private:
    qint32 result;
    QMap<QString, QString> docs;
    QString username;
};

QDataStream &operator<<(QDataStream &out, const Response& res);
QDataStream &operator>>(QDataStream &in, Response& res);

#endif //PDS_SERVER_RESPONSE_H
