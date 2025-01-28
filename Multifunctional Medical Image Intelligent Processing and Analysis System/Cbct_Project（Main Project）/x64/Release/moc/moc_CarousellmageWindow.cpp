/****************************************************************************
** Meta object code from reading C++ file 'CarousellmageWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../CarousellmageWindow.h"
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
#error "The header file 'CarousellmageWindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS = QtMocHelpers::stringData(
    "CarouselImageWindow",
    "currentImageChanged",
    "",
    "currentIndex",
    "onImageChangeTimeout",
    "onImageSwitchButtonClicked",
    "buttonId",
    "getCurrentImageIndex"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[20];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[21];
    char stringdata5[27];
    char stringdata6[9];
    char stringdata7[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS_t qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 19),  // "CarouselImageWindow"
        QT_MOC_LITERAL(20, 19),  // "currentImageChanged"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 12),  // "currentIndex"
        QT_MOC_LITERAL(54, 20),  // "onImageChangeTimeout"
        QT_MOC_LITERAL(75, 26),  // "onImageSwitchButtonClicked"
        QT_MOC_LITERAL(102, 8),  // "buttonId"
        QT_MOC_LITERAL(111, 20)   // "getCurrentImageIndex"
    },
    "CarouselImageWindow",
    "currentImageChanged",
    "",
    "currentIndex",
    "onImageChangeTimeout",
    "onImageSwitchButtonClicked",
    "buttonId",
    "getCurrentImageIndex"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCarouselImageWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   41,    2, 0x0a,    3 /* Public */,
       5,    1,   42,    2, 0x0a,    4 /* Public */,
       7,    0,   45,    2, 0x0a,    6 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Int,

       0        // eod
};

Q_CONSTINIT const QMetaObject CarouselImageWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCarouselImageWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CarouselImageWindow, std::true_type>,
        // method 'currentImageChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onImageChangeTimeout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onImageSwitchButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getCurrentImageIndex'
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void CarouselImageWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CarouselImageWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->currentImageChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->onImageChangeTimeout(); break;
        case 2: _t->onImageSwitchButtonClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: { int _r = _t->getCurrentImageIndex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CarouselImageWindow::*)(int );
            if (_t _q_method = &CarouselImageWindow::currentImageChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *CarouselImageWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CarouselImageWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCarouselImageWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CarouselImageWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CarouselImageWindow::currentImageChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
