TEMPLATE = app
TARGET = syntool
CONFIG += qt warn_on console
DEFINES -= QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += 3d network xml core gui opengl widgets

include(./pkg.pri)

SOURCES = \
    main.cpp \
    mainwidget.cpp \
    windowwidget.cpp \
    topmenu.cpp \
    aboutwidget.cpp \
    3d/earthview.cpp \
    3d/earth.cpp \
    3d/buttons.cpp \
    3d/earthscene.cpp \
    navigatebutton.cpp \
    timeline.cpp \
    toolboxwidgets/layerswidget.cpp \
    toolboxwidgets/mapswidget.cpp \
    toolboxwidgets/placewidget.cpp \
    toolboxwidgets/productswidget.cpp \
    tiledownloader.cpp \
    toolboxwidgets/rightsidebar.cpp \
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
    additionalwidgets/datasetboxwidget.cpp \
    3d/simplegranulesnode.cpp \
    3d/skybox.cpp \
    3d/metagranules.cpp
HEADERS = \
    mainwidget.h \
    windowwidget.h \
    topmenu.h \
    aboutwidget.h \
    3d/earthview.h \
    timeline.h \
    myapplication.h \
    3d/earth.h \
    3d/buttons.h \
    3d/earthscene.h \
    navigatebutton.h \
    toolboxwidgets/layerswidget.h \
    toolboxwidgets/mapswidget.h \
    toolboxwidgets/placewidget.h \
    toolboxwidgets/productswidget.h \
    tiledownloader.h \
    toolboxwidgets/rightsidebar.h \
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
    more/geofunctions.h \
    glclasses/tilecache.h \
    3d/simplegranulesnode.h \
    3d/skybox.h \
    3d/metagranules.h
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
