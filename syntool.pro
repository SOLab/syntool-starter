TEMPLATE = app
TARGET = syntool
CONFIG += qt warn_on

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += 3d

include(./pkg.pri)

SOURCES = \
    main.cpp \
    mainwidget.cpp \
    windowwidget.cpp \
    topmenu.cpp \
    aboutwidget.cpp \
    earthview.cpp \
    customproxy.cpp \
    timeline.cpp \
    earth.cpp
HEADERS = \
    mainwidget.h \
    windowwidget.h \
    topmenu.h \
    aboutwidget.h \
    earthview.h \
    customproxy.h \
    timeline.h \
    myapplication.h \
    earth.h
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


#unix:PYTHON_VERSION=2.7
#unix:LIBS += $$system(python$${PYTHON_VERSION}-config --libs)
#unix:QMAKE_CXXFLAGS += $$system(python$${PYTHON_VERSION}-config --includes)

#CONFIG(debug, debug|release) {
#  DEBUG_EXT = _d
#} else {
#  DEBUG_EXT =
#}

#win32::LIBS += $$PWD/../lib/PythonQt$${DEBUG_EXT}.lib
#unix::LIBS += -L$$PWD/../lib -lPythonQt$${DEBUG_EXT}

#INCLUDEPATH += $$PWD/../extensions/PythonQt_QtAll

# check if debug or release
#CONFIG(debug, debug|release) {
#  DEBUG_EXT = _d
#} else {
#  DEBUG_EXT =
#}

#win32::LIBS += $$PWD/../lib/PythonQt_QtAll$${DEBUG_EXT}.lib
#unix::LIBS += -L$$PWD/../lib -lPythonQt_QtAll$${DEBUG_EXT}
