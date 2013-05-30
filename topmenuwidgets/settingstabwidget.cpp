#include "settingstabwidget.h"

SettingsTabWidget::SettingsTabWidget(ConfigData *_configData, QWidget *parent) :
    QTabWidget(parent)
{
    configData = _configData;
    createCommonWidget();
    createCacheWidget();

//    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
}

void SettingsTabWidget::createCommonWidget()
{
    commonWidget = new QWidget(this);

    QGridLayout* commonGridLayout = new QGridLayout(commonWidget);

    QLabel* serverNameLabel = new QLabel("Server name", commonWidget);
    serverNameEdit = new QLineEdit("", commonWidget);
    serverNameEdit->setText(configData->serverName);

    commonGridLayout->addWidget(serverNameLabel, 0,0);
    commonGridLayout->addWidget(serverNameEdit, 0,1);

    qCritical() << "LOG_LEVEL" << configData->logLevel;

    QLabel* logLevelLabel = new QLabel("Logging level", commonWidget);
    logLevelCombo = new QComboBox(commonWidget);
    logLevelCombo->addItem("no message", 0);
    logLevelCombo->addItem("debug only", 1);
    logLevelCombo->addItem("warning only", 2);
    logLevelCombo->addItem("debug and warning", 3);
    logLevelCombo->addItem("error only", 4);
    logLevelCombo->addItem("debug and error", 5);
    logLevelCombo->addItem("warning and error", 6);
    logLevelCombo->addItem("debug, warning and error", 7);
    logLevelCombo->addItem("info only", 8);
    logLevelCombo->addItem("all message", 15);

    if (configData->logLevel == LogLevel::AllMessage)
        logLevelCombo->setCurrentIndex(logLevelCombo->count() - 1);
    else
        logLevelCombo->setCurrentIndex(configData->logLevel);

    commonGridLayout->addWidget(logLevelLabel, 2,0);
    commonGridLayout->addWidget(logLevelCombo, 2,1);

    commonGridLayout->setAlignment(Qt::AlignTop);

    addTab(commonWidget, "Common");
}

void SettingsTabWidget::createCacheWidget()
{
    cacheWidget = new QWidget(this);

    QGridLayout* cacheGridLayout = new QGridLayout(cacheWidget);

    QLabel* cacheDirLabel = new QLabel("Cache directory", cacheWidget);
    cacheDirEdit = new FileOpenWidget("dir", "Cache directory", configData->cacheDir, cacheWidget);
    cacheDirEdit->setText(configData->cacheDir);

    cacheGridLayout->addWidget(cacheDirLabel, 0,0);
    cacheGridLayout->addWidget(cacheDirEdit, 0,1);

    QLabel* numberCachedTilesLabel = new QLabel("Number of cache tiles", cacheWidget);
    numberCachedTilesEdit = new QSpinBox(cacheWidget);
    numberCachedTilesEdit->setMaximum(1000);
    numberCachedTilesEdit->setMinimum(10);
    numberCachedTilesEdit->setValue(configData->numberCachedTiles);

    cacheGridLayout->addWidget(numberCachedTilesLabel, 1,0);
    cacheGridLayout->addWidget(numberCachedTilesEdit, 1,1);

    QLabel* numberCachedSimpleGranulesLabel = new QLabel("Number of cache simple granules", cacheWidget);
    numberCachedSimpleGranulesEdit = new QSpinBox(cacheWidget);
    numberCachedSimpleGranulesEdit->setMaximum(500);
    numberCachedSimpleGranulesEdit->setMinimum(5);
    numberCachedSimpleGranulesEdit->setValue(configData->numberCachedSimpleGranules);

    cacheGridLayout->addWidget(numberCachedSimpleGranulesLabel, 2,0);
    cacheGridLayout->addWidget(numberCachedSimpleGranulesEdit, 2,1);

    QLabel* numberCachedTiledGranulesLabel = new QLabel("Number of cache tiled granules", cacheWidget);
    numberCachedTiledGranulesEdit = new QSpinBox(cacheWidget);
    numberCachedTiledGranulesEdit->setMaximum(1000);
    numberCachedTiledGranulesEdit->setMinimum(10);
    numberCachedTiledGranulesEdit->setValue(configData->numberCachedTiledGranules);

    cacheGridLayout->addWidget(numberCachedTiledGranulesLabel, 3,0);
    cacheGridLayout->addWidget(numberCachedTiledGranulesEdit, 3,1);

    cacheGridLayout->setAlignment(Qt::AlignTop);

    addTab(cacheWidget, "Cache");
}
