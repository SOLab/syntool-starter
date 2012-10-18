#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include "windowwidget.h"
#include "topmenu.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QToolBox>
#include <QSplitter>
#include <QMenuBar>
#include "timeline.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow(){};
    void setHostedWindow(QWindow *window);
//    void resizeEvent(QResizeEvent *e);

protected:
    QSplitter *splitter;
    WindowWidget* glwgt;
    TopMenu* topMenu;
    TimeLine* timeLine;
    QToolBox* rightWgt;

    QVBoxLayout* vlayout;
    QHBoxLayout* hlayout;
    QWidget* centralwgt;

    void createMenuBar();

public slots:
    void aboutProgram();
};

#endif // MAINWIDGET_H
