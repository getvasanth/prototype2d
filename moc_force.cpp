/****************************************************************************
** Meta object code from reading C++ file 'force.h'
**
** Created: Sun Jul 19 08:32:52 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "games/force/force.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'force.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Force__Game[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      20,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Force__Game[] = {
    "Force::Game\0\0pPoint\0"
    "handleCollision(const ContactPoint*)\0"
};

const QMetaObject Force::Game::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Force__Game,
      qt_meta_data_Force__Game, 0 }
};

const QMetaObject *Force::Game::metaObject() const
{
    return &staticMetaObject;
}

void *Force::Game::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Force__Game))
        return static_cast<void*>(const_cast< Game*>(this));
    if (!strcmp(_clname, "Interface::IGame"))
        return static_cast< Interface::IGame*>(const_cast< Game*>(this));
    return QObject::qt_metacast(_clname);
}

int Force::Game::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: handleCollision((*reinterpret_cast< const ContactPoint*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
