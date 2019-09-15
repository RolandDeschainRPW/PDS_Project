//
// Created by raffa on 29/08/2019.
//

#include "../include/Response.h"

Response::Response() { /* empty */ }

Response::Response(const Response& res) : result(res.result),
                                          docs(res.docs),
                                          username(res.username),
                                          site_id(res.site_id),
                                          counter(res.counter),
                                          file_path(res.file_path),
                                          symbols(res.symbols),
                                          nickname(res.nickname),
                                          profile_pic(res.profile_pic),
                                          connected_editors(res.connected_editors) { /* empty */ }

Response::Response(qint32 result,
                   std::optional<QMap<QString, QString>> opt_docs,
                   QString username,
                   qint32 site_id,
                   quint32 counter,
                   QString file_path,
                   std::optional<QVector<Symbol>> opt_symbols,
                   QString nickname,
                   std::optional<QImage> opt_profile_pic,
                   std::optional<QVector<Collaborator>> opt_connected_editors) : result(result),
                                                            username(username),
                                                            site_id(site_id),
                                                            counter(counter),
                                                            file_path(file_path),
                                                            nickname(nickname) {
    if (opt_docs.has_value()) docs = opt_docs.value();
    if (opt_symbols.has_value()) symbols = opt_symbols.value();
    if (opt_profile_pic.has_value()) profile_pic = opt_profile_pic.value();
    if (opt_connected_editors.has_value()) connected_editors = opt_connected_editors.value();
}

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

qint32 Response::getSiteId() const {
    return site_id;
}

quint32 Response::getCounter() const {
    return counter;
}

QString Response::getFilePath() const {
    return file_path;
}

QVector<Symbol> Response::getSymbols() const {
    return symbols;
}

QString Response::getNickname() const {
    return nickname;
}

QImage Response::getProfilePic() const {
    return profile_pic;
}

QVector<Collaborator> Response::getConnectedEditors() const {
    return connected_editors;
}

QDataStream &operator<<(QDataStream &out, const Response& res) {
    out << res.getResult() << res.getUsername() << res.getSiteId() << res.getCounter() << res.getFilePath() << res.getNickname();
    if (res.getResult() == Response::SUCCESSFUL_LOGIN)
        out << res.getDocsList();
    if (res.getResult() == Response::START_DATA)
        out << res.getSymbols() << res.getProfilePic() << res.getConnectedEditors();
    return out;
}

QDataStream &operator>>(QDataStream &in, Response& res) {
    qint32 result;
    QMap<QString, QString> docs;
    QString username;
    qint32 site_id;
    quint32 counter;
    QString file_path;
    QVector<Symbol> symbols;
    QString nickname;
    QImage profile_pic;
    QVector<Collaborator> connected_editors;

    in >> result >> username >> site_id >> counter >> file_path >> nickname;

    if (result == Response::SUCCESSFUL_LOGIN) {
        in >> docs;
        res = Response(result, docs, username);
        return in;
    }

    if (result == Response::START_DATA) {
        in >> symbols >> profile_pic >> connected_editors;
        res = Response(result,
                       std::nullopt,
                       username,
                       site_id,
                       counter,
                       file_path,
                       symbols,
                       nickname,
                       profile_pic,
                       connected_editors);
        return in;
    }

    res = Response(result, std::nullopt, username);
    return in;
}