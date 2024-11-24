/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
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
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_play_btn_clicked",
    "",
    "on_end_btn_clicked",
    "on_listWidget_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_Refresh_clicked",
    "updateProgress",
    "seekMusic",
    "position",
    "listfile",
    "on_volumeslider_valueChanged",
    "value",
    "on_previous_clicked",
    "on_next_clicked",
    "on_addplaylist_btn_clicked",
    "showPlaylistDialog",
    "createplaylist",
    "listPlaylist",
    "getPlaylistNames",
    "on_playlistwidget_itemClicked",
    "on_addtoPlaylist_clicked",
    "on_browse_btn_clicked",
    "updateButtonVisibility",
    "updateDeletelistVisibility",
    "updateDeletesongVisibility",
    "toggle3DSound",
    "on_delSong_clicked",
    "on_delPlaylist_clicked",
    "on_modebtn_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  164,    2, 0x08,    1 /* Private */,
       3,    0,  165,    2, 0x08,    2 /* Private */,
       4,    1,  166,    2, 0x08,    3 /* Private */,
       7,    0,  169,    2, 0x08,    5 /* Private */,
       8,    0,  170,    2, 0x08,    6 /* Private */,
       9,    1,  171,    2, 0x08,    7 /* Private */,
      11,    0,  174,    2, 0x08,    9 /* Private */,
      12,    1,  175,    2, 0x08,   10 /* Private */,
      14,    0,  178,    2, 0x08,   12 /* Private */,
      15,    0,  179,    2, 0x08,   13 /* Private */,
      16,    0,  180,    2, 0x08,   14 /* Private */,
      17,    0,  181,    2, 0x08,   15 /* Private */,
      18,    1,  182,    2, 0x08,   16 /* Private */,
      19,    0,  185,    2, 0x08,   18 /* Private */,
      20,    0,  186,    2, 0x08,   19 /* Private */,
      21,    1,  187,    2, 0x08,   20 /* Private */,
      22,    0,  190,    2, 0x08,   22 /* Private */,
      23,    0,  191,    2, 0x08,   23 /* Private */,
      24,    0,  192,    2, 0x08,   24 /* Private */,
      25,    0,  193,    2, 0x08,   25 /* Private */,
      26,    0,  194,    2, 0x08,   26 /* Private */,
      27,    1,  195,    2, 0x08,   27 /* Private */,
      28,    0,  198,    2, 0x08,   29 /* Private */,
      29,    0,  199,    2, 0x08,   30 /* Private */,
      30,    0,  200,    2, 0x08,   31 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_play_btn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_end_btn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_listWidget_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_Refresh_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'seekMusic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'listfile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_volumeslider_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_previous_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_next_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addplaylist_btn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPlaylistDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createplaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'listPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getPlaylistNames'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'on_playlistwidget_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_addtoPlaylist_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_browse_btn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateButtonVisibility'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDeletelistVisibility'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDeletesongVisibility'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toggle3DSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_delSong_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_delPlaylist_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_modebtn_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_play_btn_clicked(); break;
        case 1: _t->on_end_btn_clicked(); break;
        case 2: _t->on_listWidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 3: _t->on_Refresh_clicked(); break;
        case 4: _t->updateProgress(); break;
        case 5: _t->seekMusic((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->listfile(); break;
        case 7: _t->on_volumeslider_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->on_previous_clicked(); break;
        case 9: _t->on_next_clicked(); break;
        case 10: _t->on_addplaylist_btn_clicked(); break;
        case 11: _t->showPlaylistDialog(); break;
        case 12: _t->createplaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->listPlaylist(); break;
        case 14: { QStringList _r = _t->getPlaylistNames();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 15: _t->on_playlistwidget_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 16: _t->on_addtoPlaylist_clicked(); break;
        case 17: _t->on_browse_btn_clicked(); break;
        case 18: _t->updateButtonVisibility(); break;
        case 19: _t->updateDeletelistVisibility(); break;
        case 20: _t->updateDeletesongVisibility(); break;
        case 21: _t->toggle3DSound((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->on_delSong_clicked(); break;
        case 23: _t->on_delPlaylist_clicked(); break;
        case 24: _t->on_modebtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}
QT_WARNING_POP
