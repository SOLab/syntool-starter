#include "mainwidget.h"


//#include "PythonQt.h"
//#include "gui/PythonQtScriptingConsole.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    vlayout = new QVBoxLayout;
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(5,0,5,2);

    centralwgt = new QWidget;
    centralwgt->setLayout(vlayout);
    setCentralWidget(centralwgt);

    splitter = new QSplitter(parent);

    topMenu = new TopMenu(this);
    topMenu->setFixedHeight(28);

    glwgt = new WindowWidget(this);
    glwgt->setMinimumSize(200, 200);

    QString serverName = "http://staging.satin.rshu.ru";
//    create right toolBox
    rightSidebar = new RightSidebar;

    MapsWgt = new MapsWidget(centralwgt);
    ProductsWgt = new ProductsWidget(serverName, centralwgt);
    DatasetsWgt = new DatasetsWidget(centralwgt);
    PlaceWgt = new PlaceWidget(centralwgt);
    LayersWgt = new LayersWidget(centralwgt);

    rightSidebar->addItem(MapsWgt, "Maps");
    rightSidebar->addItem(ProductsWgt, "Products");
    rightSidebar->addItem(LayersWgt, "Layers");
    rightSidebar->addItem(DatasetsWgt, "Datasets");
    rightSidebar->addItem(PlaceWgt, "Places");

//    add all main widget
    splitter->addWidget(glwgt);
    splitter->addWidget(rightSidebar);
    splitter->setStretchFactor(0, 4);
    splitter->setStretchFactor(1, 1);


    timeLine = new TimeLine(serverName, this);
    timeLine->setFixedHeight(100);
    timeLine->hide();

    vlayout->addWidget(topMenu);
    vlayout->addWidget(splitter);
    vlayout->addWidget(timeLine);

    createMenuBar();
    createPythonConsole();
    setFocusPolicy(Qt::StrongFocus);

    selectedProducts = new QHash<QString, selectedProduct>;
    granulesHash = new QHash<QString, Granule>;

    ProductsWgt->setSelectedProducts(selectedProducts, granulesHash);
    timeLine->setSelectedProducts(selectedProducts, granulesHash);

    connect(timeLine, SIGNAL(getNewAllGranules()), ProductsWgt, SLOT(getNewGranules()));

    connect(ProductsWgt, &ProductsWidget::productAdded, LayersWgt, &LayersWidget::addProduct);

    connect(LayersWgt, &LayersWidget::removeLayer, ProductsWgt, &ProductsWidget::removeProduct);

    ProductsWgt->setObjectsPointer(timeLine);

    rightSidebar->setMinimumWidth(120);

    setWindowTitle("Syntool");
}

void MainWindow::setHostedWindow(QWindow *window)
{
    glwgt->setHostedWindow(window);
}

void MainWindow::aboutProgram()
{
    aboutWgt = new AboutWidget;
}

void MainWindow::showTimeLine()
{
    if (timeLine->isVisible())
        timeLine->hide();
    else
        timeLine->show();
}

void MainWindow::createMenuBar()
{
    menuBar()->setContentsMargins(0,0,0,0);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QIcon::setThemeName("oxygen");

    QAction* openAction = new QAction(QIcon(":/icons/open.png"), tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);


    QAction* saveAction = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);


    QAction* exitAction = new QAction(QIcon(":/icons/exit.png"), tr("E&xit"), this);
    exitAction->setShortcut(Qt::Key_Q+Qt::CTRL);
//    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setShortcutContext(Qt::ApplicationShortcut);

//    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
//    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

/////////////// Edit tab
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));

    QAction* findAction = new QAction(QIcon(":/icons/find.png"), tr("Find"), this);
    // properties current object
    QAction* propertiesAction = new QAction(QIcon(":/icons/properties.png"), tr("Properties"), this);

    editMenu->addAction(findAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);

/////////////// View tab
    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

    QAction* showToolbarAction = new QAction(tr("Toolbar"), this);
    showToolbarAction->setCheckable(true);
    showToolbarAction->setChecked(true);
    connect(showToolbarAction, SIGNAL(triggered(bool)), topMenu, SLOT(setVisible(bool)));

    QAction* showSidebarAction = new QAction(tr("Sidebar"), this);
    showSidebarAction->setCheckable(true);
    showSidebarAction->setChecked(true);
    connect(showSidebarAction, SIGNAL(triggered(bool)), rightSidebar, SLOT(setVisible(bool)));

    viewMenu->addAction(showToolbarAction);
    viewMenu->addAction(showSidebarAction);

/////////////// Settings tab
    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction* settingAction = new QAction(QIcon(":/icons/settings.png"), tr("Settings"), this);
    connect (settingAction, SIGNAL(triggered()), this, SLOT(showSettings()));

    QAction* rulerAction = new QAction(QIcon(":/icons/ruler.png"), tr("Ruler"), this);

    toolsMenu->addAction(rulerAction);
    toolsMenu->addAction(settingAction);

/////////////// Help tab
    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction* aboutAction = new QAction(QIcon(":/icons/help.png"), tr("&About"), this);
//    aboutAction->setShortcut(QKeySequence::HelpContents);
//    aboutAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutProgram()));

    QAction* bugsAction = new QAction(QIcon(":/icons/e-mail.png"), tr("Send &bugs"), this);
    bugsAction->setShortcut(Qt::Key_B);

    QAction* handbookAction = new QAction(QIcon(":/icons/help_book.png"), tr("&Handbook"), this);
    handbookAction->setShortcut(QKeySequence::HelpContents);

    QAction* aboutQtAction = new QAction(QIcon(":/icons/qt.png"), tr("&About Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(handbookAction);
    helpMenu->addAction(bugsAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::showSettings()
{
    settingsWidget = new SettingsWidget();
    settingsWidget->show();
}

void MainWindow::createPythonConsole()
{
//    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
//    PythonQt_QtAll::init();

//    PythonQtObjectPtr  mainContext = PythonQt::self()->getMainModule();
//    PythonQtScriptingConsole console(NULL, mainContext);

//    mainContext.evalFile(":example.py");

//    QWidget *window = new QWidget;
//    QPushButton *button1 = new QPushButton("One");
//    QPushButton *button2 = new QPushButton("Two");

//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->addWidget(button1);
//    layout->addWidget(button2);
//    vlayout->addWidget(&console);

//    window->setLayout(layout);
//    window->show();
//    mainContext.addObject("window", window);
//    mainContext.addObject("button1", button1);
//    mainContext.addObject("button2", button2);
}


void MainWindow::keyPress(QKeyEvent *e)
{
    keyPressEvent(e);
}
