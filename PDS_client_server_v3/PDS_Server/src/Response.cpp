//
// Created by raffa on 29/08/2019.
//

#include "../include/Response.h"

Response::Response() { /* empty */ }

Response::Response(const Response& res) : result(res.result),
                                          docs(res.docs),
                                          username(res.username) { /* empty */ }

Response::Response(qint32 result,
                   QMap<QString, QString> docs,
                   QString username) : result(result),
                                       docs(docs),
                                       username(username) { /* empty */ }

Response::~Response() { /* empty */ }

qint32 Response::getResult() const {
    return result;
}

QMap<QString, QString> Response::getDocsList() const {
    return docs;
}

QString Response::getUsername() const {
    return username;
}

QDataStream &operator<<(QDataStream &out, const Response& res) {
    out << res.getResult() << res.getDocsList() << res.getUsername();
    return out;
}

QDataStream &operator>>(QDataStream &in, Response& res) {
    qint32 result;
    QMap<QString, QString> docs;
    QString username;

    in >> result >> docs >> username;
    res = Response(result, docs, username);
    return in;
}