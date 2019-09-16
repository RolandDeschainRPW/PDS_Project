//
// Created by raffa on 11/09/2019.
//

#include <QPainter>

#include "../include/MyTextEdit.h"

MyTextEdit::MyTextEdit(QWidget* parent) : QTextEdit(parent) {
    // Setting predefined font.
    this->setFont(QFont("Times New Roman", 12, QFont::Medium));

    QTextCursor cursor = this->textCursor();
    QPoint start_cur = this->viewport()->mapToGlobal(this->cursorRect(cursor).bottomLeft());
    QPoint end_cur = this->viewport()->mapToGlobal(this->cursorRect(cursor).topLeft());

    qint32 start_y = start_cur.y();
    qint32 end_y = end_cur.y();

    qint32 delta = start_y - end_y;
    cur_width = qAbs(delta);

    colors.push_back(Qt::black); // Black at index 0 - DO NOT USE!
    colors.push_back(Qt::red); // Red at index 1
    colors.push_back(Qt::green); // Green at index 2
    colors.push_back(Qt::blue); // Blue at index 3
    colors.push_back(QColor(255, 85, 0)); // Orange at index 4

    this->setCursorWidth(0);
}

MyTextEdit::~MyTextEdit() { /* empty */ }

void MyTextEdit::setSiteId(qint32 site_id) {
    this->siteId = site_id;
}

void MyTextEdit::addCursor(qint32 site_id) {
    cursors.insert(site_id, QTextCursor(this->document()));
}

void MyTextEdit::removeCursor(qint32 site_id) {
    cursors.erase(cursors.find(site_id));
}

void MyTextEdit::setCursorPosition(qint32 site_id, qint32 pos) {
    cursors.find(site_id).value().setPosition(pos);
    this->update();
}

QColor MyTextEdit::getColor(qint32 site_id) {
    return colors[site_id];
}

void MyTextEdit::paintEvent(QPaintEvent* event) {
    QTextEdit::paintEvent(event);

    QPainter painter(this->viewport());

    // Updating this Site Id cursor.
    {
        QPainterPath path;
        QTextCursor cursor = this->textCursor();
        QPoint start_cur = this->cursorRect(cursor).bottomLeft();
        QPoint end_cur = QPoint(start_cur);
        end_cur.setY(end_cur.y() - cur_width * 1.15);

        path.moveTo(QPointF(start_cur));
        path.lineTo(QPointF(end_cur));

        QBrush my_brush(colors[siteId]);
        QPen myPen(my_brush, 2, Qt::SolidLine);
        painter.setPen(myPen);
        painter.drawPath(path);
    }



    // Updating the other cursors.
    foreach (qint32 site_id, cursors.keys()) {
        QPainterPath path;
        QTextCursor cursor = cursors.find(site_id).value();
        QPoint start_cur = this->cursorRect(cursor).bottomLeft();
        QPoint end_cur = QPoint(start_cur);
        end_cur.setY(end_cur.y() - cur_width * 1.15);

        path.moveTo(QPointF(start_cur));
        path.lineTo(QPointF(end_cur));

        QBrush my_brush(colors[site_id]);
        QPen myPen(my_brush, 2, Qt::SolidLine);
        painter.setPen(myPen);
        painter.drawPath(path);
    }
}