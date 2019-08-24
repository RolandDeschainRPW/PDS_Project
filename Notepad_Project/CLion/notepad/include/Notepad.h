/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <optional>

#include <QMainWindow>
#include <QTextEdit>
#include <QTextCursor>
#include <QVector>
#include <QtNetwork>

#include "../include/Symbol.h"
#include "../include/Message.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Notepad;
}
QT_END_NAMESPACE

class Notepad : public QMainWindow {
    Q_OBJECT

public:
    static const qint32 SITE_ID_UNASSIGNED = -1;
    static const qint32 BOUNDARY_PLUS = 0;
    static const qint32 BOUNDARY_MINUS = 1;
    static const qint32 RANDOM = 2;

    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

    qint32 getSiteId();
    qint32 getCounter();
    std::optional<Symbol> getSymbol(qint32 index);
    void localInsert(qint32 index, QChar value);
    void localErase(qint32 index);
    void processSymbol(const Message &m);
    QString symbols_to_string();

private slots:
    void newDocument();
    void open();
    void save();
    void saveAs();
    void print();
    void exit();
    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void selectFont();
    void setFontBold(bool bold);
    void setFontUnderline(bool underline);
    void setFontItalic(bool italic);

    // Slots for text changes.
    void onTextChanged();
    void onCursorPositionChanged();
    void change(int pos, int del, int add);

    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableConnectToServerButton();
    void sessionOpened();

private:
    Ui::Notepad *ui;
    QString currentFile;

    // Vars for text changes.
    QTextCursor cursor;
    QString pos_cursor;

    // Vars for symbols managing.
    qint32 _siteId = SITE_ID_UNASSIGNED;
    qint32 _counter = 0;
    qint32 boundary;
    qint32 base;
    qint32 strategy;
    QVector<qint32> strategyCache;
    QVector<Symbol> _symbols;

    // Vars for networking.
    QTcpSocket* tcpSocket = nullptr;
    QDataStream in;
    QNetworkSession* networkSession = nullptr;

    Symbol generateSymbol(QChar value, qint32 index);
    QVector<qint32> generatePosBetween(qint32 siteId1,
                                       qint32 siteId2,
                                       QVector<qint32> before,
                                       std::optional<QVector<qint32>> after_opt,
                                       std::optional<QVector<qint32>> newPos_opt,
                                       qint32 level = 0);
    qint32 generateIdBetween(qint32 min, qint32 max, qint32 myStrategy);
    qint32 retrieveStrategy(qint32 level);
    bool comparePositions(std::optional<QVector<qint32>> pos1_opt, std::optional<QVector<qint32>> pos2_opt);
    void readMessage();
    void readDataFromSocket();
    void closeEvent(QCloseEvent* event);
};

#endif // NOTEPAD_H
