/****************************************************************************
** Meta object code from reading C++ file 'Notepad.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/Notepad.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Notepad.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Notepad_t {
    QByteArrayData data[27];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Notepad_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Notepad_t qt_meta_stringdata_Notepad = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Notepad"
QT_MOC_LITERAL(1, 8, 11), // "newDocument"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 4), // "open"
QT_MOC_LITERAL(4, 26, 4), // "save"
QT_MOC_LITERAL(5, 31, 6), // "saveAs"
QT_MOC_LITERAL(6, 38, 5), // "print"
QT_MOC_LITERAL(7, 44, 4), // "exit"
QT_MOC_LITERAL(8, 49, 4), // "copy"
QT_MOC_LITERAL(9, 54, 3), // "cut"
QT_MOC_LITERAL(10, 58, 5), // "paste"
QT_MOC_LITERAL(11, 64, 4), // "undo"
QT_MOC_LITERAL(12, 69, 4), // "redo"
QT_MOC_LITERAL(13, 74, 10), // "selectFont"
QT_MOC_LITERAL(14, 85, 11), // "setFontBold"
QT_MOC_LITERAL(15, 97, 4), // "bold"
QT_MOC_LITERAL(16, 102, 16), // "setFontUnderline"
QT_MOC_LITERAL(17, 119, 9), // "underline"
QT_MOC_LITERAL(18, 129, 13), // "setFontItalic"
QT_MOC_LITERAL(19, 143, 6), // "italic"
QT_MOC_LITERAL(20, 150, 18), // "interceptUserInput"
QT_MOC_LITERAL(21, 169, 3), // "pos"
QT_MOC_LITERAL(22, 173, 3), // "del"
QT_MOC_LITERAL(23, 177, 3), // "add"
QT_MOC_LITERAL(24, 181, 12), // "displayError"
QT_MOC_LITERAL(25, 194, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(26, 223, 11) // "socketError"

    },
    "Notepad\0newDocument\0\0open\0save\0saveAs\0"
    "print\0exit\0copy\0cut\0paste\0undo\0redo\0"
    "selectFont\0setFontBold\0bold\0"
    "setFontUnderline\0underline\0setFontItalic\0"
    "italic\0interceptUserInput\0pos\0del\0add\0"
    "displayError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Notepad[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    0,  100,    2, 0x08 /* Private */,
       4,    0,  101,    2, 0x08 /* Private */,
       5,    0,  102,    2, 0x08 /* Private */,
       6,    0,  103,    2, 0x08 /* Private */,
       7,    0,  104,    2, 0x08 /* Private */,
       8,    0,  105,    2, 0x08 /* Private */,
       9,    0,  106,    2, 0x08 /* Private */,
      10,    0,  107,    2, 0x08 /* Private */,
      11,    0,  108,    2, 0x08 /* Private */,
      12,    0,  109,    2, 0x08 /* Private */,
      13,    0,  110,    2, 0x08 /* Private */,
      14,    1,  111,    2, 0x08 /* Private */,
      16,    1,  114,    2, 0x08 /* Private */,
      18,    1,  117,    2, 0x08 /* Private */,
      20,    3,  120,    2, 0x08 /* Private */,
      24,    1,  127,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   21,   22,   23,
    QMetaType::Void, 0x80000000 | 25,   26,

       0        // eod
};

void Notepad::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Notepad *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newDocument(); break;
        case 1: _t->open(); break;
        case 2: _t->save(); break;
        case 3: _t->saveAs(); break;
        case 4: _t->print(); break;
        case 5: _t->exit(); break;
        case 6: _t->copy(); break;
        case 7: _t->cut(); break;
        case 8: _t->paste(); break;
        case 9: _t->undo(); break;
        case 10: _t->redo(); break;
        case 11: _t->selectFont(); break;
        case 12: _t->setFontBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->setFontUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setFontItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->interceptUserInput((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Notepad::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_Notepad.data,
    qt_meta_data_Notepad,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Notepad::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Notepad::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Notepad.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Notepad::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
