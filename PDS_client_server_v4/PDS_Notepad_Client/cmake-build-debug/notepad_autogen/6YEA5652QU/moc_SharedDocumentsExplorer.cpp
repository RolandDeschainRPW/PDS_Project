/****************************************************************************
** Meta object code from reading C++ file 'SharedDocumentsExplorer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/SharedDocumentsExplorer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SharedDocumentsExplorer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SharedDocumentsExplorer_t {
    QByteArrayData data[10];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SharedDocumentsExplorer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SharedDocumentsExplorer_t qt_meta_stringdata_SharedDocumentsExplorer = {
    {
QT_MOC_LITERAL(0, 0, 23), // "SharedDocumentsExplorer"
QT_MOC_LITERAL(1, 24, 12), // "displayError"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(4, 67, 11), // "socketError"
QT_MOC_LITERAL(5, 79, 22), // "askNewDocumentFilename"
QT_MOC_LITERAL(6, 102, 12), // "openDocument"
QT_MOC_LITERAL(7, 115, 23), // "readStartDataFromServer"
QT_MOC_LITERAL(8, 139, 15), // "addCollaborator"
QT_MOC_LITERAL(9, 155, 23) // "openModifyProfileDialog"

    },
    "SharedDocumentsExplorer\0displayError\0"
    "\0QAbstractSocket::SocketError\0socketError\0"
    "askNewDocumentFilename\0openDocument\0"
    "readStartDataFromServer\0addCollaborator\0"
    "openModifyProfileDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SharedDocumentsExplorer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,
       8,    0,   50,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SharedDocumentsExplorer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SharedDocumentsExplorer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: _t->askNewDocumentFilename(); break;
        case 2: _t->openDocument(); break;
        case 3: _t->readStartDataFromServer(); break;
        case 4: _t->addCollaborator(); break;
        case 5: _t->openModifyProfileDialog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SharedDocumentsExplorer::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SharedDocumentsExplorer.data,
    qt_meta_data_SharedDocumentsExplorer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SharedDocumentsExplorer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SharedDocumentsExplorer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SharedDocumentsExplorer.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SharedDocumentsExplorer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
