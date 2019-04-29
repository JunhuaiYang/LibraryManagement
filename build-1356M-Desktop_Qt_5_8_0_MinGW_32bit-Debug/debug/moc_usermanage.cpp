/****************************************************************************
** Meta object code from reading C++ file 'usermanage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../1356M/usermanage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usermanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserManage_t {
    QByteArrayData data[9];
    char stringdata0[80];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserManage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserManage_t qt_meta_stringdata_UserManage = {
    {
QT_MOC_LITERAL(0, 0, 10), // "UserManage"
QT_MOC_LITERAL(1, 11, 8), // "add_user"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "delete_user"
QT_MOC_LITERAL(4, 33, 11), // "updata_user"
QT_MOC_LITERAL(5, 45, 11), // "select_user"
QT_MOC_LITERAL(6, 57, 14), // "get_table_line"
QT_MOC_LITERAL(7, 72, 3), // "row"
QT_MOC_LITERAL(8, 76, 3) // "col"

    },
    "UserManage\0add_user\0\0delete_user\0"
    "updata_user\0select_user\0get_table_line\0"
    "row\0col"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserManage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    2,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,

       0        // eod
};

void UserManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UserManage *_t = static_cast<UserManage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->add_user(); break;
        case 1: _t->delete_user(); break;
        case 2: _t->updata_user(); break;
        case 3: _t->select_user(); break;
        case 4: _t->get_table_line((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject UserManage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UserManage.data,
      qt_meta_data_UserManage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UserManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserManage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UserManage.stringdata0))
        return static_cast<void*>(const_cast< UserManage*>(this));
    return QWidget::qt_metacast(_clname);
}

int UserManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
