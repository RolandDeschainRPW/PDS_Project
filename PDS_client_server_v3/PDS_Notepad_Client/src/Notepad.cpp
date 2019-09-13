#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QSignalBlocker>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB

#include <QFont>
#include <QFontDialog>
#include <QDebug>
#include <QInputDialog>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>

#include "../include/Notepad.h"
#include "../include/Symbol.h"
#include "../include/Message.h"
#include "../include/Request.h"
#include "../include/SerialSize.h"
#include "ui_notepad.h"

Notepad::Notepad(NetworkingData* net_data,
        QImage profile_pic,
        QString nickname,
        QVector<Collaborator> connected_editors,
        QWidget *parent,
        qint32 base,
        qint32 boundary,
        qint32 strategy) : QMainWindow(parent),
                        ui(new Ui::Notepad),
                        base(base),
                        boundary(boundary),
                        strategy(strategy),
                        net_data(net_data) {
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose, true);

    this->net_data->setParent(this); // To avoid Memory Leakage!
    this->setWindowTitle(tr("Notepad, Site ID -> %1").arg(net_data->getSiteId()));
    this->setCentralWidget(ui->centralWidget);

    in.setDevice(net_data->getTcpSocket());
    in.setVersion(QDataStream::Qt_5_13);

    connect(ui->actionPrint, &QAction::triggered, this, &Notepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &Notepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &Notepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notepad::redo);

    // Updating the document with symbols received from Server.
    ui->textEdit->document()->setPlainText(this->symbols_to_string());

    // Listen on text changed signals.
    connect(ui->textEdit->document(), &QTextDocument::contentsChange, this, &Notepad::interceptUserInput);

    // Updating profile pics on UI.
    this->updateConnectedCollaborators(net_data->getSiteId(), Message::NOTIFY_CONNECTION, nickname, profile_pic);

    foreach (Collaborator col, connected_editors)
        this->updateConnectedCollaborators(col.getSiteId(),
                                           Message::NOTIFY_CONNECTION,
                                           col.getNickname(),
                                           col.getProfilePic());

    // Connecting signals for networking.
    connect(net_data->getTcpSocket(), &QIODevice::readyRead, this, [this, net_data] {
        while (net_data->getTcpSocket()->bytesAvailable()) {
            in.startTransaction();

            qint64 size;
            in >> size;

            if(size > net_data->getTcpSocket()->bytesAvailable()) {
                in.rollbackTransaction();
                in.abortTransaction();
                return;
            }

            Message next_msg;
            in >> next_msg;

            if (!in.commitTransaction()) {
                qDebug() << "Something went wrong!\n\t-> I couldn't read the incoming message!";
                return;
            }

            this->readMessage(next_msg);
        }
    });
    connect(net_data->getTcpSocket(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Notepad::displayError);

    // Disable menu actions for unavailable features
    #if !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
    #endif

    #if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    #endif
}

Notepad::~Notepad() {
    delete ui;
}

void Notepad::print() {
    #if QT_CONFIG(printer)
    QPrinter printDev;
    #if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
    #endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
    #endif // QT_CONFIG(printer)
}

void Notepad::exit() {
    QCoreApplication::quit();
}

void Notepad::copy() {
    #if QT_CONFIG(clipboard)
    ui->textEdit->copy();
    #endif
}

void Notepad::cut() {
    #if QT_CONFIG(clipboard)
    ui->textEdit->cut();
    #endif
}

void Notepad::paste() {
    #if QT_CONFIG(clipboard)
    ui->textEdit->paste();
    #endif
}

void Notepad::undo() {
    ui->textEdit->undo();
}

void Notepad::redo() {
    ui->textEdit->redo();
}

void Notepad::interceptUserInput(int pos, int del, int add) {
    if(del > 0) {
        undo();
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + del, QTextCursor::KeepAnchor);
        for (int i = c.selectedText().size() - 1; i >= 0; i--)
            this->localErase(pos + i);
        redo();
    }

    if(add > 0) {
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + add, QTextCursor::KeepAnchor);
        for (int i = 0; i < c.selectedText().size(); i++)
            this->localInsert(pos + i, c.selectedText()[i]);
    }
}

void Notepad::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("SharedEditor"),
                                     tr("The following error occurred: %1.").arg(net_data->getTcpSocket()->errorString()));
    }
}

// Overriding the closeEvent() function to correctly disconnect from the Server.
void Notepad::closeEvent(QCloseEvent* event) {
    // Knowing how to handle the disconnection from Server.
    connect(net_data->getTcpSocket(), &QAbstractSocket::disconnected, net_data->getTcpSocket(), [this, event] {
        qDebug() << "DISCONNECTED!";
        QMainWindow::closeEvent(event);
    });

    // I am going to warn the Server that I want to disconnect.
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->net_data->getSiteId(), Request::DISCONNECT_TYPE, std::nullopt, "", "", net_data->getRemoteFilePath(), net_data->getCounter());

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    net_data->getTcpSocket()->write(block);
    qDebug() << "I am going to DISCONNECT!";
}

void Notepad::readMessage(Message& msg) {
    if (msg.getType() == Message::INSERT_TYPE || msg.getType() == Message::ERASE_TYPE)
        this->processSymbol(msg);
    else if (msg.getType() == Message::NOTIFY_CONNECTION) {
        this->updateConnectedCollaborators(msg.getSiteId(),
                                           msg.getType(),
                                           msg.getIncomingNickname(),
                                           msg.getIncomingUserPic());
    } else /* Message::NOTIFY_DISCONNECTION */ {
        this->updateConnectedCollaborators(msg.getSiteId(),
                                           msg.getType());
    }
}

void Notepad::updateConnectedCollaborators(qint32 site_id, qint32 update_type, QString nickname, std::optional<QImage> opt_profile_pic) {
    if (update_type == Message::NOTIFY_DISCONNECTION) {
        QImage default_pic = QImage(QDir::toNativeSeparators(":/images/default_profile_pic.png"));
        QPixmap pixmap = QPixmap::fromImage(default_pic);
        if (site_id == 1) {
            // Freeing spot 1.
            ui->collaboratorPicLabel1->setPixmap(pixmap);
            ui->nicknameLabel1->setText(tr("Unconnected"));
        } else if (site_id == 2) {
            // Freeing spot 2.
            ui->collaboratorPicLabel2->setPixmap(pixmap);
            ui->nicknameLabel2->setText(tr("Unconnected"));
        } else if (site_id == 3) {
            // Freeing spot 3.
            ui->collaboratorPicLabel3->setPixmap(pixmap);
            ui->nicknameLabel3->setText(tr("Unconnected"));
        } else if (site_id == 4) {
            // Freeing spot 4.
            ui->collaboratorPicLabel4->setPixmap(pixmap);
            ui->nicknameLabel4->setText(tr("Unconnected"));
        }
    } else /* Message::NOTIFY_CONNECTION */ {
        QImage profile_pic = opt_profile_pic.value();
        QPixmap pixmap = QPixmap::fromImage(profile_pic);
        if (site_id == 1) {
            // Filling spot 1.
            ui->collaboratorPicLabel1->setPixmap(pixmap);
            ui->nicknameLabel1->setText(nickname);
        } else if (site_id == 2) {
            // Filling spot 2.
            ui->collaboratorPicLabel2->setPixmap(pixmap);
            ui->nicknameLabel2->setText(nickname);
        } else if (site_id == 3) {
            // Filling spot 3.
            ui->collaboratorPicLabel3->setPixmap(pixmap);
            ui->nicknameLabel3->setText(nickname);
        } else if (site_id == 4) {
            // Filling spot 4.
            ui->collaboratorPicLabel4->setPixmap(pixmap);
            ui->nicknameLabel4->setText(nickname);
        }
    }
}

qint32 Notepad::getSiteId() {
    return net_data->getSiteId();
}

void Notepad::localInsert(qint32 index, QChar value) {
    Symbol s = this->generateSymbol(value, index);

    if (net_data->getSymbols().empty())  net_data->getSymbols().push_back(s);
    else if (index > net_data->getSymbols().size() - 1) net_data->getSymbols().push_back(s);
    else if (index == 0) net_data->getSymbols().insert(net_data->getSymbols().begin(), s);
    else net_data->getSymbols().insert(net_data->getSymbols().begin() + index, s);

    net_data->incrementCounter();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->net_data->getSiteId(), Request::MESSAGE_TYPE, Message(this->net_data->getSiteId(), Message::INSERT_TYPE, s), "", "", net_data->getRemoteFilePath());

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    net_data->getTcpSocket()->write(block);
}

void Notepad::localErase(qint32 index) {
    if (index >= net_data->getSymbols().size()) return;

    Symbol s = net_data->getSymbols().at(index);
    net_data->getSymbols().erase(net_data->getSymbols().begin() + index);

    net_data->incrementCounter();

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->net_data->getSiteId(), Request::MESSAGE_TYPE, Message(this->net_data->getSiteId(), Message::ERASE_TYPE, s), "", "", net_data->getRemoteFilePath());

    // Calculating the size of the request in bytes.
    SerialSize size;
    qint64 req_size = size(req_to_send);

    out << req_size << req_to_send;
    net_data->getTcpSocket()->write(block);
}

void Notepad::processSymbol(const Message &m) {
    qint32 index = 0;
    if (m.getType() == Message::INSERT_TYPE) {
        if (net_data->getSymbols().empty()) {
            qint32 index = net_data->getSymbols().size();
            net_data->getSymbols().push_back(m.getSymbol());
            updateDocument(index, Message::INSERT_TYPE, m.getSymbol().getChar());
        } else {
            foreach (Symbol s, net_data->getSymbols()) {
                if (this->comparePositions(s.getPos(), m.getSymbol().getPos())) {
                    net_data->getSymbols().insert(net_data->getSymbols().begin() + index, m.getSymbol());
                    updateDocument(index, Message::INSERT_TYPE, m.getSymbol().getChar());
                    return;
                }
                index++;
            }
            qint32 index = net_data->getSymbols().size();
            net_data->getSymbols().push_back(m.getSymbol());
            updateDocument(index, Message::INSERT_TYPE, m.getSymbol().getChar());
        }
    } else /* Message::ERASE_TYPE */ {
        foreach (Symbol s, net_data->getSymbols()){
            if (s.getChar() == m.getSymbol().getChar() && s.getSiteId() == m.getSymbol().getSiteId() && s.getCounter() == m.getSymbol().getCounter()) {
                net_data->getSymbols().erase(net_data->getSymbols().begin() + index);
                updateDocument(index, Message::ERASE_TYPE, "");
                return;
            }
            index++;
        }
    }
}

QString Notepad::symbols_to_string() {
    QString str = "";
    foreach (Symbol s, net_data->getSymbols()) {
        str += s.getChar();
    }
    return str;
}

std::optional<Symbol> Notepad::getSymbol(qint32 index) {
    if (!net_data->getSymbols().empty() && index >= 0 && index <= net_data->getSymbols().size() - 1) return net_data->getSymbols().at(index);
    else return std::nullopt;
}

// Returns true if pos1 > pos2
bool Notepad::comparePositions(std::optional<QVector<qint32>> pos1_opt,
                                    std::optional<QVector<qint32>> pos2_opt) {
    QVector<qint32> pos1 = (pos1_opt.has_value()) ? pos1_opt.value() : QVector<qint32>();
    QVector<qint32> pos2 = (pos2_opt.has_value()) ? pos2_opt.value() : QVector<qint32>();

    qint32 digit1 = (!pos1.empty()) ? pos1[0] : 0;
    qint32 digit2 = (!pos2.empty()) ? pos2[0] : 0;

    if (digit1 > digit2) return true;
    else if (digit1 < digit2) return false;
    else if (digit1 == digit2 && pos1.size() == 1 && pos2.size() == 1) return false;
    else {
        QVector<qint32> slice1 = (pos1.begin() + 1 < pos1.end()) ? pos1.mid(1) : QVector<qint32>();
        QVector<qint32> slice2 = (pos2.begin() + 1 < pos2.end()) ? pos2.mid(1) : QVector<qint32>();
        return this->comparePositions(slice1, slice2);
    }
}

Symbol Notepad::generateSymbol(QChar value, qint32 index) {
    QVector<qint32> before = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getPos() : QVector<qint32>();
    qint32 siteIdBefore = (this->getSymbol(index - 1).has_value()) ? this->getSymbol(index - 1).value().getSiteId() : this->getSiteId();

    QVector<qint32> after = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getPos() : QVector<qint32>();
    qint32 siteIdAfter = (this->getSymbol(index).has_value()) ? this->getSymbol(index).value().getSiteId() : this->getSiteId();

    QVector<qint32> newPos = this->generatePosBetween(siteIdBefore, siteIdAfter, before, after, std::nullopt);
    return Symbol(value, this->getSiteId(), net_data->getCounter(), newPos);
}

QVector<qint32> Notepad::generatePosBetween(qint32 siteId1,
                                                 qint32 siteId2,
                                                 QVector<qint32> before,
                                                 std::optional<QVector<qint32>> after_opt,
                                                 std::optional<QVector<qint32>> newPos_opt,
                                                 qint32 level) {
    qint32 myBase = pow(2, level) * this->base;
    qint32 myStrategy = this->retrieveStrategy(level);

    QVector<qint32> after = (after_opt.has_value()) ? after_opt.value() : QVector<qint32>();
    QVector<qint32> newPos = (newPos_opt.has_value()) ? newPos_opt.value() : QVector<qint32>();

    qint32 id1 = (!before.empty()) ? before[0] : 0;
    qint32 id2 = (!after.empty()) ? after[0] : myBase;

    if (id2 - id1 > 1) {
        qint32 newDigit = this->generateIdBetween(id1, id2, myStrategy);
        newPos.push_back(newDigit);
        return newPos;
    } else if (id2 - id1 == 1) {
        newPos.push_back(id1);
        QVector<qint32> slice = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
        return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
    } else if (id1 == id2) {
        newPos.push_back(id1);
        QVector<qint32> slice1 = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
        QVector<qint32> slice2 = (after.begin() + 1 < after.end()) ? after.mid(1) : QVector<qint32>();
        return this->generatePosBetween(siteId1, siteId2, slice1, slice2, newPos, level + 1);
    }
}

qint32 Notepad::generateIdBetween(qint32 min, qint32 max, qint32 myStrategy) {
    if ((max - min) < boundary) {
        min++;
    } else {
        if (myStrategy == BOUNDARY_MINUS) {
            min = max - boundary;
        } else {
            min++;
            max = min + boundary;
        }
    }

    // Random number between 0.0 (inclusive) and 1.0 (exclusive).
    double tmp = QRandomGenerator::global()->bounded(1.0);
    return qFloor(tmp * (max - min)) + min;
}

qint32 Notepad::retrieveStrategy(qint32 level) {
    if (!strategyCache.empty() && level <= strategyCache.size() - 1) return strategyCache[level];

    qint32 myStrategy;

    switch (this->strategy) {
        case BOUNDARY_PLUS: {
            myStrategy = BOUNDARY_PLUS;
            break;
        }
        case BOUNDARY_MINUS: {
            myStrategy = BOUNDARY_MINUS;
            break;
        }
        case RANDOM: {
            // Random number between 0.0 (inclusive) and 1.0 (exclusive).
            double tmp = QRandomGenerator::global()->bounded(1.0);
            myStrategy = round(tmp) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
        default: {
            myStrategy = (level % 2) == 0 ? BOUNDARY_PLUS : BOUNDARY_MINUS;
            break;
        }
    }

    strategyCache.push_back(myStrategy);
    return myStrategy;
}

void Notepad::updateDocument(qint32 index, qint32 updateType, QString text) {
    // Temporarily blocking signals from QTextDocument
    // to avoid cyclic bug between the Server and the Clients!
    const QSignalBlocker blocker(ui->textEdit->document());
    QTextCursor tmp_cur = QTextCursor(ui->textEdit->document());
    tmp_cur.setPosition(index);
    if (updateType == Message::INSERT_TYPE) {
        tmp_cur.insertText(text);
    } else /* Message::ERASE_TYPE */ {
        tmp_cur.deleteChar();
    }
}