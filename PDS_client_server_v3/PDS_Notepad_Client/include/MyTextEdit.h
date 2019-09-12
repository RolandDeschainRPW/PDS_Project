//
// Created by raffa on 11/09/2019.
//

#ifndef NOTEPAD_MYTEXTEDIT_H
#define NOTEPAD_MYTEXTEDIT_H

#include <QTextEdit>

class MyTextEdit : public QTextEdit {
    Q_OBJECT

public:
    MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

private:
    qint32 cur_width;

    void paintEvent(QPaintEvent* event) override;
};


#endif //NOTEPAD_MYTEXTEDIT_H
