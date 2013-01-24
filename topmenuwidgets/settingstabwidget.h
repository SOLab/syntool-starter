#ifndef SETTINGSTABWIDGET_H
#define SETTINGSTABWIDGET_H

#include <QTabWidget>
#include <topmenuwidgets/cachetabwidget.h>
#include <topmenuwidgets/commontabwidget.h>

class SettingsTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SettingsTabWidget(QWidget *parent = 0);

protected:
    QWidget* cacheWidget;
    QWidget* commonWidget;

signals:

public slots:

};

#endif // SETTINGSTABWIDGET_H
