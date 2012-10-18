TEMPLATE = app
TARGET = syntool
CONFIG += qt warn_on

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += 3d

include(./pkg.pri)

SOURCES = \
    main.cpp \
    mainwidget.cpp \
    windowwidget.cpp \
    topmenu.cpp \
    aboutwidget.cpp \
    earthview.cpp
HEADERS = \
    mainwidget.h \
    windowwidget.h \
    topmenu.h \
    aboutwidget.h \
    earthview.h
RESOURCES += \
    syntool.qrc
DEPENDPATH += resources

OTHER_FILES += \
    resources/moon-texture.jpg \
    resources/planet.jpg \
    resources/solar.jpg \
    resources/solar.fsh \
    resources/solar.vsh \
    syntool.rc

OTHER_FILES +=

RC_FILE = syntool.rc

LIBS += -lglut -lGL -lGLU
