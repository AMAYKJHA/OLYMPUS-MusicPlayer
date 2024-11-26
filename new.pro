QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    playlist.h

FORMS += \
    mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += concurrent


INCLUDEPATH += D:/Qtproject/SDL2/include  #sdl library
LIBS += -LD:/Qtproject/SDL2/lib -lSDL2


INCLUDEPATH += D:/Qtproject/SDL2_mixer/include   #sdl_mixer library
LIBS += -LD:/Qtproject/SDL2_mixer/lib -lSDL2_mixer

RESOURCES += \
    app_resources.qrc


RC_FILE = resource.rc
RC_ICONS = music.ico




