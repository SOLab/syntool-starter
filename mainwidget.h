#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
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

class TimeLine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ConfigData _configData, QWidget *parent = 0);
    ~MainWindow();
    void setHostedWindow(QWindow *window);
    void keyPress(QKeyEvent* e);
//    void resizeEvent(QResizeEvent *e);

    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;
    ConfigData configData;

    void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static MainWindow *self;
protected:
    QSplitter *splitter;
    WindowWidget* glwgt;
    TopMenu* topMenu;
    TimeLine* timeLine;
    RightSidebar* rightSidebar;

    PlaceWidget* PlaceWgt;
    MapsWidget* MapsWgt;
    DatasetsWidget* DatasetsWgt;
    ProductsWidget* ProductsWgt;
    LayersWidget* LayersWgt;

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QWidget* centralwgt;

    AboutWidget* aboutWgt;
    SettingsWidget* settingsWidget;

    void createMenuBar();
    void createPythonConsole();
//    void keyPressEvent(QKeyEvent *e);

private:

public slots:
    void aboutProgram();
    void showTimeLine();

    void showSettings();
};

#endif // MAINWIDGET_H
