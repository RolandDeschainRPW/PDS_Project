//
// Created by raffa on 10/09/2019.
//

#ifndef PDS_SERVER_SERIALSIZE_H
#define PDS_SERVER_SERIALSIZE_H

#include <QtCore>

class SerialSize {
public:
    SerialSize() : stream(&data) {
        data.open(QIODevice::WriteOnly);
    }

    template <typename T>
    quint64 operator()(const T & t) {
        data.seek(0);
        stream << t;
        return data.pos();
    }

private:
    QBuffer data;
    QDataStream stream;
};

#endif //PDS_SERVER_SERIALSIZE_H
