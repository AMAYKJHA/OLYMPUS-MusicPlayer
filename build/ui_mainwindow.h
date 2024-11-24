/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *play_btn;
    QPushButton *end_btn;
    QListWidget *listWidget;
    QPushButton *Refresh;
    QSlider *Progressbar;
    QSlider *volumeslider;
    QLabel *timelabel;
    QPushButton *previous;
    QPushButton *next;
    QLabel *playing_song;
    QLabel *volume_label;
    QLabel *end_label;
    QPushButton *modebtn;
    QLabel *modelabel;
    QLineEdit *SearchLine;
    QListWidget *playlistwidget;
    QPushButton *addplaylist_btn;
    QLabel *playlist_label;
    QPushButton *addtoPlaylist;
    QLabel *music_label;
    QLabel *sound_label;
    QPushButton *browse_btn;
    QCheckBox *sound3d;
    QPushButton *delSong;
    QPushButton *delPlaylist;
    QLabel *searchicon;
    QLabel *sound3dlabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        play_btn = new QPushButton(centralwidget);
        play_btn->setObjectName("play_btn");
        play_btn->setGeometry(QRect(250, 80, 31, 30));
        end_btn = new QPushButton(centralwidget);
        end_btn->setObjectName("end_btn");
        end_btn->setGeometry(QRect(580, 20, 35, 31));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(100, 270, 431, 281));
        Refresh = new QPushButton(centralwidget);
        Refresh->setObjectName("Refresh");
        Refresh->setGeometry(QRect(440, 170, 91, 41));
        Progressbar = new QSlider(centralwidget);
        Progressbar->setObjectName("Progressbar");
        Progressbar->setGeometry(QRect(50, 30, 501, 20));
        Progressbar->setAutoFillBackground(false);
        Progressbar->setOrientation(Qt::Orientation::Horizontal);
        volumeslider = new QSlider(centralwidget);
        volumeslider->setObjectName("volumeslider");
        volumeslider->setGeometry(QRect(700, 100, 16, 160));
        volumeslider->setStyleSheet(QString::fromUtf8(""));
        volumeslider->setMaximum(100);
        volumeslider->setOrientation(Qt::Orientation::Vertical);
        timelabel = new QLabel(centralwidget);
        timelabel->setObjectName("timelabel");
        timelabel->setGeometry(QRect(50, 50, 31, 21));
        previous = new QPushButton(centralwidget);
        previous->setObjectName("previous");
        previous->setGeometry(QRect(190, 80, 41, 31));
        next = new QPushButton(centralwidget);
        next->setObjectName("next");
        next->setGeometry(QRect(310, 80, 41, 31));
        playing_song = new QLabel(centralwidget);
        playing_song->setObjectName("playing_song");
        playing_song->setGeometry(QRect(110, 10, 361, 21));
        volume_label = new QLabel(centralwidget);
        volume_label->setObjectName("volume_label");
        volume_label->setGeometry(QRect(720, 240, 31, 16));
        end_label = new QLabel(centralwidget);
        end_label->setObjectName("end_label");
        end_label->setGeometry(QRect(500, 50, 51, 16));
        modebtn = new QPushButton(centralwidget);
        modebtn->setObjectName("modebtn");
        modebtn->setGeometry(QRect(31, 90, 45, 23));
        modelabel = new QLabel(centralwidget);
        modelabel->setObjectName("modelabel");
        modelabel->setGeometry(QRect(80, 91, 41, 21));
        SearchLine = new QLineEdit(centralwidget);
        SearchLine->setObjectName("SearchLine");
        SearchLine->setGeometry(QRect(100, 220, 431, 31));
        playlistwidget = new QListWidget(centralwidget);
        playlistwidget->setObjectName("playlistwidget");
        playlistwidget->setGeometry(QRect(600, 330, 141, 131));
        addplaylist_btn = new QPushButton(centralwidget);
        addplaylist_btn->setObjectName("addplaylist_btn");
        addplaylist_btn->setGeometry(QRect(710, 300, 25, 25));
        playlist_label = new QLabel(centralwidget);
        playlist_label->setObjectName("playlist_label");
        playlist_label->setGeometry(QRect(600, 300, 91, 21));
        QFont font;
        font.setPointSize(12);
        playlist_label->setFont(font);
        playlist_label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        addtoPlaylist = new QPushButton(centralwidget);
        addtoPlaylist->setObjectName("addtoPlaylist");
        addtoPlaylist->setGeometry(QRect(540, 270, 31, 31));
        music_label = new QLabel(centralwidget);
        music_label->setObjectName("music_label");
        music_label->setGeometry(QRect(63, 2, 31, 31));
        sound_label = new QLabel(centralwidget);
        sound_label->setObjectName("sound_label");
        sound_label->setGeometry(QRect(660, 240, 37, 21));
        browse_btn = new QPushButton(centralwidget);
        browse_btn->setObjectName("browse_btn");
        browse_btn->setGeometry(QRect(620, 20, 41, 31));
        sound3d = new QCheckBox(centralwidget);
        sound3d->setObjectName("sound3d");
        sound3d->setGeometry(QRect(571, 141, 41, 18));
        delSong = new QPushButton(centralwidget);
        delSong->setObjectName("delSong");
        delSong->setGeometry(QRect(540, 310, 25, 25));
        delPlaylist = new QPushButton(centralwidget);
        delPlaylist->setObjectName("delPlaylist");
        delPlaylist->setGeometry(QRect(750, 330, 25, 25));
        searchicon = new QLabel(centralwidget);
        searchicon->setObjectName("searchicon");
        searchicon->setGeometry(QRect(70, 225, 22, 22));
        sound3dlabel = new QLabel(centralwidget);
        sound3dlabel->setObjectName("sound3dlabel");
        sound3dlabel->setGeometry(QRect(610, 130, 37, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        play_btn->setText(QString());
        end_btn->setText(QString());
        Refresh->setText(QCoreApplication::translate("MainWindow", "All Songs", nullptr));
        timelabel->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        previous->setText(QString());
        next->setText(QString());
        playing_song->setText(QCoreApplication::translate("MainWindow", "__-__-__-__-__", nullptr));
        volume_label->setText(QCoreApplication::translate("MainWindow", "00", nullptr));
        end_label->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        modebtn->setText(QCoreApplication::translate("MainWindow", "MODE", nullptr));
        modelabel->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        addplaylist_btn->setText(QString());
        playlist_label->setText(QCoreApplication::translate("MainWindow", "Playlists", nullptr));
        addtoPlaylist->setText(QString());
        music_label->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        sound_label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        browse_btn->setText(QString());
        sound3d->setText(QCoreApplication::translate("MainWindow", "3D", nullptr));
        delSong->setText(QString());
        delPlaylist->setText(QString());
        searchicon->setText(QCoreApplication::translate("MainWindow", ">", nullptr));
        sound3dlabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
