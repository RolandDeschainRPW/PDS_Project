//
// Created by raffa on 29/08/2019.
//

#ifndef PDS_SERVER_RESPONSE_H
#define PDS_SERVER_RESPONSE_H

#include <QtCore>

class Response {
public:
    Response();
    Response(qint32 result, QUrl user_path_url);
    Response(const Response& res);
    ~Response();

    qint32 getResult() const;
    QUrl getUrl() const;

    static const qint32 WRONG_CREDENTIALS = -5;
    static const qint32 SUCCESSFUL_LOGIN = -6;
    static const qint32 USERNAME_ALREADY_IN_USE = -7;
    static const qint32 USERNAME_ACCEPTED = -8;

private:
    qint32 result;
    QUrl user_path_url;
};

QDataStream &operator<<(QDataStream &out, const Response& res);
QDataStream &operator>>(QDataStream &in, Response& res);

#endif //PDS_SERVER_RESPONSE_H
