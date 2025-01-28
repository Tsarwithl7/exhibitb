/****************************************************************************
** Meta object code from reading C++ file 'SideWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SideWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SideWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMeasurementToolENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMeasurementToolENDCLASS = QtMocHelpers::stringData(
    "MeasurementTool",
    "updateContrast",
    "",
    "value",
    "updateBrightness",
    "mouseReleaseEvent",
    "QMouseEvent*",
    "event",
    "clearMeasurements",
    "setButtonValue",
    "clearRects"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMeasurementToolENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[16];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[17];
    char stringdata5[18];
    char stringdata6[13];
    char stringdata7[6];
    char stringdata8[18];
    char stringdata9[15];
    char stringdata10[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMeasurementToolENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMeasurementToolENDCLASS_t qt_meta_stringdata_CLASSMeasurementToolENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "MeasurementTool"
        QT_MOC_LITERAL(16, 14),  // "updateContrast"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 5),  // "value"
        QT_MOC_LITERAL(38, 16),  // "updateBrightness"
        QT_MOC_LITERAL(55, 17),  // "mouseReleaseEvent"
        QT_MOC_LITERAL(73, 12),  // "QMouseEvent*"
        QT_MOC_LITERAL(86, 5),  // "event"
        QT_MOC_LITERAL(92, 17),  // "clearMeasurements"
        QT_MOC_LITERAL(110, 14),  // "setButtonValue"
        QT_MOC_LITERAL(125, 10)   // "clearRects"
    },
    "MeasurementTool",
    "updateContrast",
    "",
    "value",
    "updateBrightness",
    "mouseReleaseEvent",
    "QMouseEvent*",
    "event",
    "clearMeasurements",
    "setButtonValue",
    "clearRects"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMeasurementToolENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x0a,    1 /* Public */,
       4,    1,   53,    2, 0x0a,    3 /* Public */,
       5,    1,   56,    2, 0x0a,    5 /* Public */,
       8,    0,   59,    2, 0x0a,    7 /* Public */,
       9,    1,   60,    2, 0x0a,    8 /* Public */,
      10,    0,   63,    2, 0x0a,   10 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MeasurementTool::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_CLASSMeasurementToolENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMeasurementToolENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMeasurementToolENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MeasurementTool, std::true_type>,
        // method 'updateContrast'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateBrightness'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'mouseReleaseEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'clearMeasurements'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setButtonValue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'clearRects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MeasurementTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeasurementTool *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateContrast((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->updateBrightness((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->mouseReleaseEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 3: _t->clearMeasurements(); break;
        case 4: _t->setButtonValue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->clearRects(); break;
        default: ;
        }
    }
}

const QMetaObject *MeasurementTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeasurementTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMeasurementToolENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MeasurementTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSideWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSSideWidgetENDCLASS = QtMocHelpers::stringData(
    "SideWidget",
    "openImage",
    "",
    "clearDistance",
    "clearRects",
    "onUnitButtonClicked",
    "onkeyPointsButtonClicked",
    "saveCurrentFrame"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSideWidgetENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[11];
    char stringdata5[20];
    char stringdata6[25];
    char stringdata7[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSideWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSideWidgetENDCLASS_t qt_meta_stringdata_CLASSSideWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "SideWidget"
        QT_MOC_LITERAL(11, 9),  // "openImage"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 13),  // "clearDistance"
        QT_MOC_LITERAL(36, 10),  // "clearRects"
        QT_MOC_LITERAL(47, 19),  // "onUnitButtonClicked"
        QT_MOC_LITERAL(67, 24),  // "onkeyPointsButtonClicked"
        QT_MOC_LITERAL(92, 16)   // "saveCurrentFrame"
    },
    "SideWidget",
    "openImage",
    "",
    "clearDistance",
    "clearRects",
    "onUnitButtonClicked",
    "onkeyPointsButtonClicked",
    "saveCurrentFrame"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSideWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    0,   51,    2, 0x08,    2 /* Private */,
       4,    0,   52,    2, 0x08,    3 /* Private */,
       5,    0,   53,    2, 0x08,    4 /* Private */,
       6,    0,   54,    2, 0x08,    5 /* Private */,
       7,    0,   55,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SideWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSSideWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSideWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSideWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SideWidget, std::true_type>,
        // method 'openImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearDistance'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearRects'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUnitButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onkeyPointsButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveCurrentFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SideWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SideWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->openImage(); break;
        case 1: _t->clearDistance(); break;
        case 2: _t->clearRects(); break;
        case 3: _t->onUnitButtonClicked(); break;
        case 4: _t->onkeyPointsButtonClicked(); break;
        case 5: _t->saveCurrentFrame(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *SideWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SideWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSideWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int SideWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
