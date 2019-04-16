/****************************************************************************
** Meta object code from reading C++ file 'recvdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "recvdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recvdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GETDT_t {
    QByteArrayData data[8];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GETDT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GETDT_t qt_meta_stringdata_GETDT = {
    {
QT_MOC_LITERAL(0, 0, 5), // "GETDT"
QT_MOC_LITERAL(1, 6, 6), // "prompt"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 5), // "title"
QT_MOC_LITERAL(4, 20, 5), // "state"
QT_MOC_LITERAL(5, 26, 6), // "status"
QT_MOC_LITERAL(6, 33, 8), // "eventnum"
QT_MOC_LITERAL(7, 42, 8) // "eventnow"

    },
    "GETDT\0prompt\0\0title\0state\0status\0"
    "eventnum\0eventnow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GETDT[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       6,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::ULong,    7,

       0        // eod
};

void GETDT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GETDT *_t = static_cast<GETDT *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->prompt((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->state((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->eventnum((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (GETDT::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GETDT::prompt)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GETDT::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GETDT::state)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (GETDT::*_t)(unsigned long );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GETDT::eventnum)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GETDT::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GETDT.data,
      qt_meta_data_GETDT,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GETDT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GETDT::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GETDT.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GETDT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GETDT::prompt(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GETDT::state(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GETDT::eventnum(unsigned long _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
