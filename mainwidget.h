#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBox>
#include <QSplitter>
#include <QMenuBar>
#include <QHash>
#include <QApplication>

#include "windowwidget.h"
#include "topmenu.h"
#include "aboutwidget.h"
#include "timeline.h"
#include "toolboxwidgets/rightsidebar.h"
#include "network/getgranules.h"
#include "more/structure.h"

#include "toolboxwidgets/layerswidget.h"
#include "toolboxwidgets/placewidget.h"
#include "toolboxwidgets/mapswidget.h"
#include "toolboxwidgets/productswidget.h"
#include "toolboxwidgets/datasetswidget.h"
#include "topmenuwidgets/settingswidget.h"
#include "3d/earthview.h"
#include "3d/metagranules.h"

class TimeLine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ConfigData *configValue, QWidget *parent = 0);
    ~MainWindow();
    void setHostedWindow(EarthView *window);
    void keyPress(QKeyEvent* e);
//    void resizeEvent(QResizeEvent *e);

    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;
    ConfigData *configData;

    void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static MainWindow *self;
protected:
    QSplitter*    splitter;
    WindowWidget* glwgt;
    TopMenu*      topMenu;
    TimeLine*     timeLine;
    RightSidebar* rightSidebar;

    PlaceWidget*    placeWgt;
    MapsWidget*     mapsWgt;
    DatasetsWidget* datasetsWgt;
    ProductsWidget* productsWgt;
    LayersWidget*   layersWgt;

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QWidget*     centralwgt;

    AboutWidget*    aboutWgt;
    SettingsWidget* settingsWidget;
    EarthView*      earthViewPointer;
    MetaGranules*   metaGranulesPointer;

    void createMenuBar();
    void createPythonConsole();

    QAction* fullScreenAction;

private:

public slots:
    void fileOpen();
    void fileSave();

    void aboutProgram();
    void openHandBook();
    void showTimeLine();

    void showSettings();
    void setFullScreen(bool value=false);
};

#endif // MAINWIDGET_H
