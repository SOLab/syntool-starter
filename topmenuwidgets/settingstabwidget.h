#ifndef SETTINGSTABWIDGET_H
#define SETTINGSTABWIDGET_H

#include <QTabWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>
#include <QSettings>

#include "topmenuwidgets/cachetabwidget.h"
#include "more/structure.h"
#include "topmenuwidgets/commontabwidget.h"
#include "fileopenwidget.h"

class SettingsTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SettingsTabWidget(ConfigData *configValue, QWidget *parent = 0);
    QComboBox*      langCombo;
    QLineEdit*      serverNameEdit;
    QComboBox*      logLevelCombo;
    FileOpenWidget* cacheDirEdit;
    QSpinBox*       numberCachedTilesEdit;
    QSpinBox*       numberCachedSimpleGranulesEdit;
    QSpinBox*       numberCachedTiledGranulesEdit;

protected:
    ConfigData* configData;
    QWidget*    cacheWidget;
    QWidget*    commonWidget;
    void createCacheWidget();
    void createCommonWidget();

signals:

public slots:

};

#endif // SETTINGSTABWIDGET_H
