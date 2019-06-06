#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T12:38:19
#
#-------------------------------------------------


CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Bomberman
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    g_homescreen.cpp \
    bomb.cpp \
    mapbloc.cpp \
    map.cpp \
    g_game.cpp \
    game.cpp \
    player.cpp \
    g_mapchooser.cpp \
    g_help.cpp \
    g_mainwidget.cpp \
    ai_player.cpp

HEADERS += \
    stable.h \
    g_homescreen.h \
    bomb.h \
    mapbloc.h \
    map.h \
    g_game.h \
    game.h \
    player.h \
    g_mapchooser.h \
    g_help.h \
    g_mainwidget.h \
    priorityqueue.h \
    ai_player.h

RESOURCES += \
    ressources.qrc
