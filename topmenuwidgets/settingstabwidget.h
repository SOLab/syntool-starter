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
#include <QCheckBox>

#include "more/structure.h"
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

    QSpinBox*       timelineFrequencyEdit;
    QSpinBox*       numberDisplayedGranulesEdit;
    QCheckBox*      hideStartWidget;

protected:
    ConfigData* configData;
    QWidget*    commonWidget;
    QWidget*    cacheWidget;
    QWidget*    otherWidget;
    void createCacheWidget();
    void createCommonWidget();
    void createOtherWidget();

signals:

public slots:

};

#endif // SETTINGSTABWIDGET_H
