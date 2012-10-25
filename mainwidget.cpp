#include "mainwidget.h"
#include <QDebug>


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

//    create right toolBox
    rightWgt = new QToolBox;
    rightWgt->setMinimumWidth(100);

    QWidget* NavigationWgt = new QWidget(centralwgt);
    QWidget* MapsWgt = new QWidget(centralwgt);
    QWidget* LayersWgt = new QWidget(centralwgt);

    rightWgt->addItem(NavigationWgt, "Navigation");
    rightWgt->addItem(MapsWgt, "Maps");
    rightWgt->addItem(LayersWgt, "Layers");

//    add all main widget
    splitter->addWidget(glwgt);
    splitter->addWidget(rightWgt);
    splitter->setStretchFactor(0, 4);
    splitter->setStretchFactor(1, 1);


    timeLine = new TimeLine(this);
    timeLine->setFixedHeight(70);
    timeLine->hide();

    vlayout->addWidget(topMenu);
    vlayout->addWidget(splitter);
    vlayout->addWidget(timeLine);

    createMenuBar();
    createPythonConsole();
    setFocusPolicy(Qt::StrongFocus);

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
    QAction* openAction = new QAction(QIcon().fromTheme("document-open"), tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    QAction* saveAction = new QAction(QIcon().fromTheme("document-save"), tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    QAction* exitAction = new QAction(QIcon().fromTheme("application-exit"), tr("E&xit"), this);
//    exitAction->setShortcut(QKeySequence( Qt::CTRL + Qt::Key_Q ));
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setShortcutContext(Qt::ApplicationShortcut);

//    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
//    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&View"));
    menuBar()->addMenu(tr("&Setting"));

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* aboutAction = new QAction(QIcon().fromTheme("help-about"), tr("&About"), this);
    aboutAction->setShortcut(QKeySequence::HelpContents);
    aboutAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutProgram()));
    helpMenu->addAction(aboutAction);
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
    qDebug() << 5666565;
    this->keyPressEvent(e);
}
