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

#include "../include/Notepad.h"
#include "../include/Symbol.h"
#include "../include/Message.h"
#include "../include/Request.h"
#include "ui_notepad.h"

Notepad::Notepad(NetworkingData* net_data,
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
    this->net_data->setParent(this); // To avoid Memory Leakage!
    this->setWindowTitle(tr("Notepad, Site ID -> %1").arg(net_data->getSiteId()));
    this->setCentralWidget(ui->textEdit);

    // DA RIVEDERE!!
    //connect(this, &QDialog::finished, this, &QObject::deleteLater);

    in.setDevice(net_data->getTcpSocket());
    in.setVersion(QDataStream::Qt_5_13);

    connect(ui->actionNew, &QAction::triggered, this, &Notepad::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &Notepad::open);
    connect(ui->actionSave, &QAction::triggered, this, &Notepad::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Notepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &Notepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &Notepad::exit);
    connect(ui->actionCopy, &QAction::triggered, this, &Notepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &Notepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &Notepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &Notepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &Notepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &Notepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &Notepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &Notepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &Notepad::setFontItalic);

    // Updating the document with symbols received from Server.
    ui->textEdit->document()->setPlainText(this->symbols_to_string());

    // Listen on text changed signals.
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Notepad::onTextChanged);
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &Notepad::onCursorPositionChanged);
    connect(ui->textEdit->document(), &QTextDocument::contentsChange, this, &Notepad::interceptUserInput);

    // Connecting signals for networking.
    connect(net_data->getTcpSocket(), &QIODevice::readyRead, this, [this, net_data] {
        while (net_data->getTcpSocket()->bytesAvailable()) this->readMessage();
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

void Notepad::newDocument() {
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void Notepad::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::save() {
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void Notepad::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
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

void Notepad::selectFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::setFontUnderline(bool underline) {
    ui->textEdit->setFontUnderline(underline);
}

void Notepad::setFontItalic(bool italic) {
    ui->textEdit->setFontItalic(italic);
}

void Notepad::setFontBold(bool bold) {
    bold ? ui->textEdit->setFontWeight(QFont::Bold) : ui->textEdit->setFontWeight(QFont::Normal);
}

void Notepad::onTextChanged() {
    qDebug() << "onTextChanged() WORKS! :O";
    //Notepad::save();
}

void Notepad::onCursorPositionChanged() {
    qDebug() << "onCursorPositionChanged() WORKS! :O";
    qDebug() << "Position in block: " << ui->textEdit->textCursor().positionInBlock();
    qDebug() << "Absolute position: " << ui->textEdit->textCursor().position();
}

void Notepad::interceptUserInput(int pos, int del, int add) {
    if(del > 0) {
        undo();
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + del, QTextCursor::KeepAnchor);
        qDebug() << "Removed: " << del << " (" << c.selectedText() << ")";
        for (int i = c.selectedText().size() - 1; i >= 0; i--)
            this->localErase(pos + i);
        redo();
    }

    if(add > 0) {
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + add, QTextCursor::KeepAnchor);
        qDebug() << "Added: " << add << " (" << c.selectedText() << ")";
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
        net_data->getTcpSocket()->abort();
        qDebug() << "DISCONNECTED!";
        QMainWindow::closeEvent(event);
    });

    // I am going to warn the Server that I want to disconnect.
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->net_data->getSiteId(), Request::DISCONNECT_TYPE, std::nullopt);
    out << req_to_send;
    net_data->getTcpSocket()->write(block);
    qDebug() << "I am going to DISCONNECT!";
}

void Notepad::readMessage() {
    qDebug() << "There is a message to read!";
    in.startTransaction();

    Message next_msg;
    in >> next_msg;

    if (!in.commitTransaction()) {
        qDebug() << "Something went wrong!\n\t-> I couldn't read the incoming message!";
        return;
    }

    qDebug() << "I am going to process the message!";
    this->processSymbol(next_msg);

    // Showing the current symbols on standard output.
    qDebug() << "Current symbols on this Client with Site Id " << this->net_data->getSiteId() << ": " << this->symbols_to_string();
}

qint32 Notepad::getSiteId() {
    return net_data->getSiteId();
}

qint32 Notepad::getCounter() {
    return _counter;
}

void Notepad::localInsert(qint32 index, QChar value) {
    Symbol s = this->generateSymbol(value, index);

    if (net_data->getSymbols().empty())  net_data->getSymbols().push_back(s);
    else if (index > static_cast<qint32>(net_data->getSymbols().size() - 1)) net_data->getSymbols().push_back(s);
    else if (index == 0) net_data->getSymbols().insert(net_data->getSymbols().begin(), s);
    else net_data->getSymbols().insert(net_data->getSymbols().begin() + index, s);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    Request req_to_send = Request(this->net_data->getSiteId(), Request::MESSAGE_TYPE, Message(this->net_data->getSiteId(), Message::INSERT_TYPE, s));
    out << req_to_send;
    net_data->getTcpSocket()->write(block);
    qDebug() << "I have sent an INSERT MESSAGE!";
}

void Notepad::localErase(qint32 index) {
    if (index >= static_cast<qint32>(net_data->getSymbols().size())) return;

    Symbol tmp = net_data->getSymbols().at(index);
    net_data->getSymbols().erase(net_data->getSymbols().begin() + index);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << Request(this->net_data->getSiteId(), Request::MESSAGE_TYPE, Message(this->net_data->getSiteId(), Message::ERASE_TYPE, tmp));
    net_data->getTcpSocket()->write(block);
    qDebug() << "I have sent an ERASE MESSAGE!";
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
            if (s.getChar() == m.getSymbol().getChar() && s.getId() == m.getSymbol().getId()) {
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
    if (!net_data->getSymbols().empty() && index >= 0 && index <= static_cast<qint32>(net_data->getSymbols().size() - 1)) return net_data->getSymbols().at(index);
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
    return Symbol(value, this->getSiteId(), this->getCounter(), newPos);
}

QVector<qint32> Notepad::generatePosBetween(qint32 siteId1,
                                                 qint32 siteId2,
                                                 QVector<qint32> before,
                                                 std::optional<QVector<qint32>> after_opt,
                                                 std::optional<QVector<qint32>> newPos_opt,
                                                 qint32 level) {
    qint32 myBase = static_cast<qint32>(pow(2, level) * this->base);
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
    qreal tmp = static_cast<qreal>(QRandomGenerator::global()->bounded(1.0));
    return static_cast<qint32>(qFloor(tmp * (max - min)) + min);
}

qint32 Notepad::retrieveStrategy(qint32 level) {
    if (!strategyCache.empty() && level <= static_cast<qint32>(strategyCache.size() - 1)) return strategyCache[level];

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