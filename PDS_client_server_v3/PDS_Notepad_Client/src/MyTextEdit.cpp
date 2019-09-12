//
// Created by raffa on 11/09/2019.
//

#include <QPainter>

#include "../include/MyTextEdit.h"

MyTextEdit::MyTextEdit(QWidget* parent) : QTextEdit(parent) {
    QTextCursor cursor = this->textCursor();
    QPoint start_cur = this->viewport()->mapToGlobal(this->cursorRect(cursor).bottomLeft());
    QPoint end_cur = this->viewport()->mapToGlobal(this->cursorRect(cursor).topLeft());

    qint32 start_y = start_cur.y();
    qint32 end_y = end_cur.y();

    qint32 delta = start_y - end_y;
    cur_width = qAbs(delta);

    this->setCursorWidth(0);
}

MyTextEdit::~MyTextEdit() { /* empty */ }

void MyTextEdit::paintEvent(QPaintEvent* event) {
    QTextEdit::paintEvent(event);

    QPainter painter(this->viewport());

    QTextCursor cursor = this->textCursor();
    QPoint start_cur = this->cursorRect(cursor).bottomLeft();
    QPoint end_cur = QPoint(start_cur);
    end_cur.setY(end_cur.y() - cur_width * 1.15);

    QPainterPath path;
    path.moveTo(QPointF(start_cur));
    path.lineTo(QPointF(end_cur));

    QBrush my_brush(Qt::red);
    QPen myPen(my_brush, 2, Qt::SolidLine);
    painter.setPen(myPen);
    painter.drawPath(path);
}