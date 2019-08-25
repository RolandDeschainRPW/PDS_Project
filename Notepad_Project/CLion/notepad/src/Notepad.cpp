#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
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

Notepad::Notepad(QWidget *parent,
        qint32 base,
        qint32 boundary,
        qint32 strategy) : QMainWindow(parent),
                        ui(new Ui::Notepad),
                        base(base),
                        boundary(boundary),
                        strategy(strategy) {
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

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

    // Listen on text changed signals.
    connect(ui->textEdit, &QTextEdit::textChanged, this, &Notepad::onTextChanged);
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &Notepad::onCursorPositionChanged);
    connect(ui->textEdit->document(), &QTextDocument::contentsChange, this, &Notepad::interceptUserInput);

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
    qDebug() << "onTextChanged WORKS! :O";
    //Notepad::save();
}

void Notepad::onCursorPositionChanged() {
    // qui salvo posiz cursore: modo per differenziare cursori per utenti diversi?
    cursor = ui->textEdit->textCursor();
    pos_cursor = QString("Position: %1").arg(cursor.positionInBlock());
}

void Notepad::interceptUserInput(int pos, int del, int add) {
    if(del > 0) {
        undo();
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + del, QTextCursor::KeepAnchor);
        qDebug() << "Position in block: " << c.positionInBlock();
        qDebug() << "Absolute position: " << c.position();
        qDebug() << "Removed: " << del << " (" << c.selectedText() << ")";
        for (int i = 0; i < c.selectedText().size(); i++)
            this->localErase(c.position());
        redo();
    }

    if(add > 0) {
        QTextCursor c = QTextCursor(ui->textEdit->textCursor());
        c.setPosition(pos);
        c.setPosition(pos + add, QTextCursor::KeepAnchor);
        qDebug() << "Position in block: " << c.positionInBlock();
        qDebug() << "Absolute position: " << c.position();
        qDebug() << "Added: " << add << " (" << c.selectedText() << ")";
        for (int i = 0; i < c.selectedText().size(); i++)
            this->localInsert(c.position(), c.selectedText()[i]);
    }
}

void Notepad::readDataFromSocket() {
    if (_siteId != SITE_ID_UNASSIGNED) {
        std::cout << "There is a message to read!" << std::endl;
        this->readMessage();
    }
    else { // Here the client receives the Site Id from the Server!
        in.startTransaction();
        in >> this->_siteId >> this->_symbols;
        if (!in.commitTransaction()) {
            std::cout << "Something went wrong!\n\t-> I could not have Site Id and Symbols from Server!" << std::endl;
            return;
        }
        std::cout << "I received the following Site Id: " << this->_siteId << std::endl;
        // Showing the Site Id in the GUI.
        std::string site_id_str = "Site Id: " + std::to_string(this->_siteId);
        //this->statusLabel->setText(tr(site_id_str.c_str()));
        //this->statusLabel->setText(tr(this->symbols_to_string().toLocal8Bit().data()));

    }
}

// Overriding the closeEvent() function to correctly disconnect from the Server.
void Notepad::closeEvent(QCloseEvent* event) {
    // Knowing how to handle the disconnection from Server.
    connect(tcpSocket, &QAbstractSocket::disconnected, tcpSocket, [this, event] {
        tcpSocket->abort();
        std::cout << "DISCONNECTED!" << std::endl;
        //QDialog::closeEvent(event);
    });

    // I am going to warn the Server that I want to disconnect.
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    Request req_to_send = Request(this->_siteId, Request::DISCONNECT_TYPE, std::nullopt);
    out << req_to_send;
    tcpSocket->write(block);
    std::cout << "I am going to DISCONNECT!" << std::endl;
}

void Notepad::readMessage() {
    in.startTransaction();

    Message next_msg;
    in >> next_msg;

    if (!in.commitTransaction()) {
        std::cout << "Something went wrong!\n\t-> I couldn't read the incoming message!" << std::endl;
        return;
    }

    std::cout << "I am going to process the message!" << std::endl;
    this->processSymbol(next_msg);

    // Showing the current symbols on standard output.
    std::string str = this->symbols_to_string().toStdString();
    std::cout << "Current symbols on this Client with Site Id " << std::to_string(this->_siteId) << ": " << str << std::endl;
}

qint32 Notepad::getSiteId() {
    return _siteId;
}

qint32 Notepad::getCounter() {
    return _counter;
}

void Notepad::localInsert(qint32 index, QChar value) {
    Symbol s = this->generateSymbol(value, index);

    if (_symbols.empty())  _symbols.push_back(s);
    else if (index > static_cast<qint32>(_symbols.size() - 1)) _symbols.push_back(s);
    else if (index == 0) _symbols.insert(_symbols.begin(), s);
    else _symbols.insert(_symbols.begin() + index, s);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    Request req_to_send = Request(this->_siteId, Request::MESSAGE_TYPE, Message(this->_siteId, Message::INSERT_TYPE, s));
    out << req_to_send;
    tcpSocket->write(block);
    std::cout << "I have sent an INSERT MESSAGE!" << std::endl;
}

void Notepad::localErase(qint32 index) {
    if (index >= static_cast<qint32>(_symbols.size())) return;

    Symbol tmp = _symbols.at(index);
    _symbols.erase(_symbols.begin() + index);

    _counter++;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << Request(this->_siteId, Request::MESSAGE_TYPE, Message(this->_siteId, Message::ERASE_TYPE, tmp));
    tcpSocket->write(block);
    std::cout << "I have sent an ERASE MESSAGE!" << std::endl;
}

void Notepad::processSymbol(const Message &m) {
    qint32 index = 0;
    if (m.getType() == Message::INSERT_TYPE) {
        if (_symbols.empty()) _symbols.push_back(m.getSymbol());
        else {
            for (Symbol s : _symbols) {
                if (this->comparePositions(s.getPos(), m.getSymbol().getPos())) {
                    _symbols.insert(_symbols.begin() + index, m.getSymbol());
                    return;
                }
                index++;
            }
            _symbols.push_back(m.getSymbol());
        }
    } else /* Message::ERASE_TYPE */ {
        for (Symbol s : _symbols){
            if (s.getChar() == m.getSymbol().getChar() && s.getId() == m.getSymbol().getId()) {
                _symbols.erase(_symbols.begin() + index);
                return;
            }
            index++;
        }
    }
}

QString Notepad::symbols_to_string() {
    QString str;
    for (Symbol s : _symbols) {
        str += s.getChar();
    }
    return str;
}

std::optional<Symbol> Notepad::getSymbol(qint32 index) {
    if (!_symbols.empty() && index >= 0 && index <= static_cast<qint32>(_symbols.size() - 1)) return _symbols.at(index);
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
        if (siteId1 < siteId2) {
            newPos.push_back(id1);
            QVector<qint32> slice = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
            return this->generatePosBetween(siteId1, siteId2, slice, std::nullopt, newPos, level + 1);
        } else if (siteId1 == siteId2) {
            newPos.push_back(id1);
            QVector<qint32> slice1 = (before.begin() + 1 < before.end()) ? before.mid(1) : QVector<qint32>();
            QVector<qint32> slice2 = (after.begin() + 1 < after.end()) ? after.mid(1) : QVector<qint32>();
            return this->generatePosBetween(siteId1, siteId2, slice1, slice2, newPos, level + 1);
        } else {
            // Da rivedere!
            // "Fix Position Sorting"
            throw std::exception();
        }
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