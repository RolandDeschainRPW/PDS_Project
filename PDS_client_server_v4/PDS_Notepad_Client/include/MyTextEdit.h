//
// Created by raffa on 11/09/2019.
//

#ifndef NOTEPAD_MYTEXTEDIT_H
#define NOTEPAD_MYTEXTEDIT_H

#include <QColor>
#include <QTextEdit>
#include <QTextCursor>

class MyTextEdit : public QTextEdit {
    Q_OBJECT

public:
    MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

    void setSiteId(qint32 site_id);
    void addCursor(qint32 site_id);
    void removeCursor(qint32 site_id);
    void setCursorPosition(qint32 site_id, qint32 pos);

private:
    qint32 siteId;
    qint32 cur_width;
    QVector<QColor> colors;
    QMap<qint32, QTextCursor> cursors;

    void paintEvent(QPaintEvent* event) override;
};


#endif //NOTEPAD_MYTEXTEDIT_H
