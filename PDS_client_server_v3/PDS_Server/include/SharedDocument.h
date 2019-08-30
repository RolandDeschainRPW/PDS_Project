//
// Created by raffa on 28/08/2019.
//

#ifndef PDS_SERVER_SHAREDDOCUMENT_H
#define PDS_SERVER_SHAREDDOCUMENT_H

#include <QTextDocument>

#include "../include/Symbol.h"

class SharedDocument : public QObject {
    Q_OBJECT

public:
    SharedDocument(QObject* parent = 0);

private:
    QVector<Symbol> _symbols;
    QTextDocument* document = nullptr;
    QTimer* timer = nullptr;
};


#endif //PDS_SERVER_SHAREDDOCUMENT_H
