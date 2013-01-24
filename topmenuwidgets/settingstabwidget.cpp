#include "settingstabwidget.h"

SettingsTabWidget::SettingsTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    cacheWidget = new QWidget(this);
    commonWidget = new QWidget(this);

    addTab(cacheWidget, "Cache");
    addTab(commonWidget, "Common");

//    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
}
