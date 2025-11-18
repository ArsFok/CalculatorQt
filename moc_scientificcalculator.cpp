/****************************************************************************
** Meta object code from reading C++ file 'scientificcalculator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../OneDrive/Desktop/Misp/prakt/My_Calculator/scientificcalculator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scientificcalculator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ScientificCalculator_t {
    QByteArrayData data[17];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScientificCalculator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScientificCalculator_t qt_meta_stringdata_ScientificCalculator = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ScientificCalculator"
QT_MOC_LITERAL(1, 21, 10), // "sinClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "cosClicked"
QT_MOC_LITERAL(4, 44, 10), // "tanClicked"
QT_MOC_LITERAL(5, 55, 11), // "ctanClicked"
QT_MOC_LITERAL(6, 67, 10), // "logClicked"
QT_MOC_LITERAL(7, 78, 9), // "lnClicked"
QT_MOC_LITERAL(8, 88, 16), // "factorialClicked"
QT_MOC_LITERAL(9, 105, 12), // "powerClicked"
QT_MOC_LITERAL(10, 118, 9), // "piClicked"
QT_MOC_LITERAL(11, 128, 8), // "eClicked"
QT_MOC_LITERAL(12, 137, 13), // "TenInXClicked"
QT_MOC_LITERAL(13, 151, 12), // "modulClicked"
QT_MOC_LITERAL(14, 164, 10), // "modClicked"
QT_MOC_LITERAL(15, 175, 16), // "leftParenClicked"
QT_MOC_LITERAL(16, 192, 17) // "rightParenClicked"

    },
    "ScientificCalculator\0sinClicked\0\0"
    "cosClicked\0tanClicked\0ctanClicked\0"
    "logClicked\0lnClicked\0factorialClicked\0"
    "powerClicked\0piClicked\0eClicked\0"
    "TenInXClicked\0modulClicked\0modClicked\0"
    "leftParenClicked\0rightParenClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScientificCalculator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x0a /* Public */,
       3,    0,   90,    2, 0x0a /* Public */,
       4,    0,   91,    2, 0x0a /* Public */,
       5,    0,   92,    2, 0x0a /* Public */,
       6,    0,   93,    2, 0x0a /* Public */,
       7,    0,   94,    2, 0x0a /* Public */,
       8,    0,   95,    2, 0x0a /* Public */,
       9,    0,   96,    2, 0x0a /* Public */,
      10,    0,   97,    2, 0x0a /* Public */,
      11,    0,   98,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    0,  101,    2, 0x0a /* Public */,
      15,    0,  102,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScientificCalculator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScientificCalculator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sinClicked(); break;
        case 1: _t->cosClicked(); break;
        case 2: _t->tanClicked(); break;
        case 3: _t->ctanClicked(); break;
        case 4: _t->logClicked(); break;
        case 5: _t->lnClicked(); break;
        case 6: _t->factorialClicked(); break;
        case 7: _t->powerClicked(); break;
        case 8: _t->piClicked(); break;
        case 9: _t->eClicked(); break;
        case 10: _t->TenInXClicked(); break;
        case 11: _t->modulClicked(); break;
        case 12: _t->modClicked(); break;
        case 13: _t->leftParenClicked(); break;
        case 14: _t->rightParenClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ScientificCalculator::staticMetaObject = { {
    &CalculatorBase::staticMetaObject,
    qt_meta_stringdata_ScientificCalculator.data,
    qt_meta_data_ScientificCalculator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ScientificCalculator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScientificCalculator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScientificCalculator.stringdata0))
        return static_cast<void*>(this);
    return CalculatorBase::qt_metacast(_clname);
}

int ScientificCalculator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CalculatorBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
