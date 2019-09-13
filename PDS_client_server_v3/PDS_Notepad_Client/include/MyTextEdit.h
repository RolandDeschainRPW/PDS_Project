//
// Created by raffa on 11/09/2019.
//

#ifndef NOTEPAD_MYTEXTEDIT_H
#define NOTEPAD_MYTEXTEDIT_H

#include <QColor>
#include <QTextEdit>

class MyTextEdit : public QTextEdit {
    Q_OBJECT

public:
    MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

    void setSiteId(qint32 site_id);

private:
    qint32 siteId;
    qint32 cur_width;
    QVector<QColor> colors;

    void paintEvent(QPaintEvent* event) override;
};


#endif //NOTEPAD_MYTEXTEDIT_H
