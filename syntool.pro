TEMPLATE = app
TARGET = syntool
CONFIG += qt warn_on

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += 3d network xml

include(./pkg.pri)

SOURCES = \
    main.cpp \
    mainwidget.cpp \
    windowwidget.cpp \
    topmenu.cpp \
    aboutwidget.cpp \
    earthview.cpp \
    earth.cpp \
    buttons.cpp \
    earthscene.cpp \
    skybox.cpp \
    navigatebutton.cpp \
    timeline.cpp \
    toolboxwidgets/layerswidget.cpp \
    toolboxwidgets/mapswidget.cpp \
    toolboxwidgets/placewidget.cpp \
    toolboxwidgets/productswidget.cpp \
    tiledownloader.cpp \
    rightsidebar.cpp \
    additionalwidgets/calendar.cpp \
    additionalwidgets/inputbox.cpp \
    additionalwidgets/imagebutton.cpp \
    topmenuwidgets/settingswidget.cpp \
    topmenuwidgets/settingstabwidget.cpp \
    topmenuwidgets/cachetabwidget.cpp \
    topmenuwidgets/commontabwidget.cpp \
    network/downloadimage.cpp \
    additionalwidgets/productinfowidget.cpp \
    network/getgranules.cpp \
    additionalwidgets/layerboxwidget.cpp \
    toolboxwidgets/datasetswidget.cpp \
    additionalwidgets/granuleinfowidget.cpp \
    network/getgranulecoords.cpp \
    additionalwidgets/datasetboxwidget.cpp
HEADERS = \
    mainwidget.h \
    windowwidget.h \
    topmenu.h \
    aboutwidget.h \
    earthview.h \
    timeline.h \
    myapplication.h \
    earth.h \
    buttons.h \
    earthscene.h \
    skybox.h \
    navigatebutton.h \
    toolboxwidgets/layerswidget.h \
    toolboxwidgets/mapswidget.h \
    toolboxwidgets/placewidget.h \
    toolboxwidgets/productswidget.h \
    tiledownloader.h \
    rightsidebar.h \
    additionalwidgets/calendar.h \
    additionalwidgets/inputbox.h \
    additionalwidgets/imagebutton.h \
    topmenuwidgets/settingswidget.h \
    topmenuwidgets/settingstabwidget.h \
    topmenuwidgets/cachetabwidget.h \
    topmenuwidgets/commontabwidget.h \
    network/downloadimage.h \
    additionalwidgets/productinfowidget.h \
    more/ProductStructures.h \
    network/getgranules.h \
    additionalwidgets/layerboxwidget.h \
    toolboxwidgets/datasetswidget.h \
    additionalwidgets/granuleinfowidget.h \
    network/getgranulecoords.h \
    additionalwidgets/datasetboxwidget.h \
    more/granuleactions.h \
    more/structure.h \
    more/geofunctions.h
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

QTPLUGIN += xcb

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
