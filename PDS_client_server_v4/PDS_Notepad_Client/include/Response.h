//
// Created by raffa on 29/08/2019.
//

#ifndef NOTEPAD_RESPONSE_H
#define NOTEPAD_RESPONSE_H

#include <optional>

#include <QtCore>
#include <QImage>

#include "../include/Symbol.h"
#include "../include/Collaborator.h"

class Response {
public:
    Response();
    Response(qint32 result,
             std::optional<QMap<QString, QString>> opt_docs = std::nullopt,
             QString username = "",
             qint32 site_id = -1,
             quint32 counter = -1,
             QString file_path = "",
             std::optional<QVector<Symbol>> opt_symbols = std::nullopt,
             QString nickname = "",
             std::optional<QImage> opt_profile_pic = std::nullopt,
             std::optional<QVector<Collaborator>> opt_connected_editors = std::nullopt);
    Response(const Response& res);
    ~Response();

    qint32 getResult() const;
    QMap<QString, QString> getDocsList() const;
    QString getUsername() const;
    qint32 getSiteId() const;
    quint32 getCounter() const;
    QString getFilePath() const;
    QVector<Symbol> getSymbols() const;
    QString getNickname() const;
    QImage getProfilePic() const;
    QVector<Collaborator> getConnectedEditors() const;

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
    static const qint32 TOO_MANY_COLLABORATORS = -200;

    // For profile modifications.
    static const qint32 PROFILE_MODIFIED = -80;
    static const qint32 PROFILE_PIC_NOT_STORED = -90;
    static const qint32 PASSWORD_NOT_STORED = -100;

    // For opening/creating documents.
    static const qint32 START_DATA = -300;

private:
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
};

QDataStream &operator<<(QDataStream &out, const Response& res);
QDataStream &operator>>(QDataStream &in, Response& res);

#endif //NOTEPAD_RESPONSE_H
