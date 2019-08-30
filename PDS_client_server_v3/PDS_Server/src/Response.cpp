//
// Created by raffa on 29/08/2019.
//

#include "../include/Response.h"

Response::Response() { /* empty */ }

Response::Response(const Response& res) : result(res.result),
                                          user_path_url(res.user_path_url) { /* empty */ }

Response::Response(qint32 result, QUrl user_path_url) : result(result),
                                                        user_path_url(user_path_url) { /* empty */ }

Response::~Response() { /* empty */ }

qint32 Response::getResult() const {
    return result;
}

QUrl Response::getUrl() const {
    return user_path_url;
}

QDataStream &operator<<(QDataStream &out, const Response& res) {
    out << res.getResult() << res.getUrl();
    return out;
}

QDataStream &operator>>(QDataStream &in, Response& res) {
    qint32 result;
    QUrl user_path_url;

    in >> result >> user_path_url;
    res = Response(result, user_path_url);
    return in;
}
