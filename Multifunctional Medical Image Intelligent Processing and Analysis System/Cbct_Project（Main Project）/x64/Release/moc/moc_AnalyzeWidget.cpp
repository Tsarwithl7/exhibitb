/****************************************************************************
** Meta object code from reading C++ file 'AnalyzeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../AnalyzeWidget.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnalyzeWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS = QtMocHelpers::stringData(
    "AnalyzeWidget",
    "onExportClicked",
    "",
    "onPrintClicked",
    "onOpenImage1Clicked",
    "onOpenImage2Clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[14];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[20];
    char stringdata5[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS_t qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "AnalyzeWidget"
        QT_MOC_LITERAL(14, 15),  // "onExportClicked"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 14),  // "onPrintClicked"
        QT_MOC_LITERAL(46, 19),  // "onOpenImage1Clicked"
        QT_MOC_LITERAL(66, 19)   // "onOpenImage2Clicked"
    },
    "AnalyzeWidget",
    "onExportClicked",
    "",
    "onPrintClicked",
    "onOpenImage1Clicked",
    "onOpenImage2Clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAnalyzeWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x08,    1 /* Private */,
       3,    0,   39,    2, 0x08,    2 /* Private */,
       4,    0,   40,    2, 0x08,    3 /* Private */,
       5,    0,   41,    2, 0x08,    4 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AnalyzeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAnalyzeWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AnalyzeWidget, std::true_type>,
        // method 'onExportClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPrintClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onOpenImage1Clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onOpenImage2Clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AnalyzeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AnalyzeWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onExportClicked(); break;
        case 1: _t->onPrintClicked(); break;
        case 2: _t->onOpenImage1Clicked(); break;
        case 3: _t->onOpenImage2Clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *AnalyzeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnalyzeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAnalyzeWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AnalyzeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
