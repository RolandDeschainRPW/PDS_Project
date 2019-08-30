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

    static const qint32 WRONG_CREDENTIALS = -5;
    static const qint32 SUCCESSFUL_LOGIN = -6;
    static const qint32 USERNAME_ALREADY_IN_USE = -7;
    static const qint32 USERNAME_ACCEPTED = -8;
    static const qint32 USERNAME_ACTIVE = -22;

private:
    qint32 result;
    QMap<QString, QString> docs;
    QString username;
};

QDataStream &operator<<(QDataStream &out, const Response& res);
QDataStream &operator>>(QDataStream &in, Response& res);

#endif //PDS_SERVER_RESPONSE_H
