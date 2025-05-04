/****************************************************************************
** Meta object code from reading C++ file 'NetworkThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../client/NetworkThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkThread_t {
    QByteArrayData data[24];
    char stringdata0[319];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkThread_t qt_meta_stringdata_NetworkThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "NetworkThread"
QT_MOC_LITERAL(1, 14, 9), // "connected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "disconnected"
QT_MOC_LITERAL(4, 38, 16), // "responseReceived"
QT_MOC_LITERAL(5, 55, 8), // "response"
QT_MOC_LITERAL(6, 64, 13), // "errorOccurred"
QT_MOC_LITERAL(7, 78, 9), // "errorCode"
QT_MOC_LITERAL(8, 88, 11), // "errorString"
QT_MOC_LITERAL(9, 100, 16), // "connectionFailed"
QT_MOC_LITERAL(10, 117, 11), // "commandSent"
QT_MOC_LITERAL(11, 129, 7), // "success"
QT_MOC_LITERAL(12, 137, 15), // "connectToServer"
QT_MOC_LITERAL(13, 153, 7), // "address"
QT_MOC_LITERAL(14, 161, 4), // "port"
QT_MOC_LITERAL(15, 166, 20), // "disconnectFromServer"
QT_MOC_LITERAL(16, 187, 11), // "sendCommand"
QT_MOC_LITERAL(17, 199, 7), // "command"
QT_MOC_LITERAL(18, 207, 17), // "onSocketConnected"
QT_MOC_LITERAL(19, 225, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(20, 246, 17), // "onSocketReadyRead"
QT_MOC_LITERAL(21, 264, 13), // "onSocketError"
QT_MOC_LITERAL(22, 278, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(23, 307, 11) // "socketError"

    },
    "NetworkThread\0connected\0\0disconnected\0"
    "responseReceived\0response\0errorOccurred\0"
    "errorCode\0errorString\0connectionFailed\0"
    "commandSent\0success\0connectToServer\0"
    "address\0port\0disconnectFromServer\0"
    "sendCommand\0command\0onSocketConnected\0"
    "onSocketDisconnected\0onSocketReadyRead\0"
    "onSocketError\0QAbstractSocket::SocketError\0"
    "socketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    1,   81,    2, 0x06 /* Public */,
       6,    2,   84,    2, 0x06 /* Public */,
       9,    1,   89,    2, 0x06 /* Public */,
      10,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    2,   95,    2, 0x0a /* Public */,
      15,    0,  100,    2, 0x0a /* Public */,
      16,    1,  101,    2, 0x0a /* Public */,
      18,    0,  104,    2, 0x08 /* Private */,
      19,    0,  105,    2, 0x08 /* Private */,
      20,    0,  106,    2, 0x08 /* Private */,
      21,    1,  107,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,

       0        // eod
};

void NetworkThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->responseReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->connectionFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->commandSent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->connectToServer((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 7: _t->disconnectFromServer(); break;
        case 8: _t->sendCommand((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onSocketConnected(); break;
        case 10: _t->onSocketDisconnected(); break;
        case 11: _t->onSocketReadyRead(); break;
        case 12: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::responseReceived)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkThread::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::errorOccurred)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::connectionFailed)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkThread::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkThread::commandSent)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_NetworkThread.data,
    qt_meta_data_NetworkThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int NetworkThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void NetworkThread::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkThread::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NetworkThread::responseReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkThread::errorOccurred(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkThread::connectionFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkThread::commandSent(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
