#include "settingstabwidget.h"

SettingsTabWidget::SettingsTabWidget(ConfigData *configValue, QWidget *parent) :
    QTabWidget(parent)
{
    configData = configValue;
    createCommonWidget();
    createCacheWidget();
    createOtherWidget();
}

void SettingsTabWidget::createCommonWidget()
{
    commonWidget = new QWidget(this);

    QGridLayout* commonGridLayout = new QGridLayout(commonWidget);

    QLabel* langLabel = new QLabel(tr("Language"), commonWidget);
    langCombo = new QComboBox(commonWidget);
    langCombo->addItem(tr("English"), "en");
    langCombo->addItem(tr("Russian"), "ru");

    if(configData->lang.left(2).toLower() == "en")
        langCombo->setCurrentIndex(0);
    else if (configData->lang.left(2).toLower() == "ru")
        langCombo->setCurrentIndex(1);

    commonGridLayout->addWidget(langLabel, 0,0);
    commonGridLayout->addWidget(langCombo, 0,1);

    QLabel* serverNameLabel = new QLabel(tr("Server name"), commonWidget);
    serverNameEdit = new QLineEdit(commonWidget);
    serverNameEdit->setText(configData->serverName);

    commonGridLayout->addWidget(serverNameLabel, 1,0);
    commonGridLayout->addWidget(serverNameEdit, 1,1);

    QLabel* logLevelLabel = new QLabel(tr("Logging level"), commonWidget);
    logLevelCombo = new QComboBox(commonWidget);
    logLevelCombo->addItem(tr("no message"), 0);
    logLevelCombo->addItem(tr("debug only"), 1);
    logLevelCombo->addItem(tr("warning only"), 2);
    logLevelCombo->addItem(tr("debug and warning"), 3);
    logLevelCombo->addItem(tr("error only"), 4);
    logLevelCombo->addItem(tr("debug and error"), 5);
    logLevelCombo->addItem(tr("warning and error"), 6);
    logLevelCombo->addItem(tr("debug, warning and error"), 7);
    logLevelCombo->addItem(tr("info only"), 8);
    logLevelCombo->addItem(tr("all message"), 15);

    if (configData->logLevel == LogLevel::AllMessage)
        logLevelCombo->setCurrentIndex(logLevelCombo->count() - 1);
    else
        logLevelCombo->setCurrentIndex(configData->logLevel);

    commonGridLayout->addWidget(logLevelLabel, 2,0);
    commonGridLayout->addWidget(logLevelCombo, 2,1);

    commonGridLayout->setAlignment(Qt::AlignTop);

    addTab(commonWidget, tr("Common"));
}

void SettingsTabWidget::createCacheWidget()
{
    cacheWidget = new QWidget(this);

    QGridLayout* cacheGridLayout = new QGridLayout(cacheWidget);

    QLabel* cacheDirLabel = new QLabel(tr("Cache directory"), cacheWidget);
    cacheDirEdit = new FileOpenWidget("dir", tr("Cache directory"), configData->cacheDir, cacheWidget);
    cacheDirEdit->setText(configData->cacheDir);

    cacheGridLayout->addWidget(cacheDirLabel, 0,0);
    cacheGridLayout->addWidget(cacheDirEdit, 0,1);

    QLabel* numberCachedTilesLabel = new QLabel(tr("Number of cache tiles"), cacheWidget);
    numberCachedTilesEdit = new QSpinBox(cacheWidget);
    numberCachedTilesEdit->setMaximum(1000);
    numberCachedTilesEdit->setMinimum(10);
    numberCachedTilesEdit->setValue(configData->numberCachedTiles);

    cacheGridLayout->addWidget(numberCachedTilesLabel, 1,0);
    cacheGridLayout->addWidget(numberCachedTilesEdit, 1,1);

    QLabel* numberCachedSimpleGranulesLabel = new QLabel(tr("Number of cache simple granules"), cacheWidget);
    numberCachedSimpleGranulesEdit = new QSpinBox(cacheWidget);
    numberCachedSimpleGranulesEdit->setMaximum(500);
    numberCachedSimpleGranulesEdit->setMinimum(5);
    numberCachedSimpleGranulesEdit->setValue(configData->numberCachedSimpleGranules);

    cacheGridLayout->addWidget(numberCachedSimpleGranulesLabel, 2,0);
    cacheGridLayout->addWidget(numberCachedSimpleGranulesEdit, 2,1);

    QLabel* numberCachedTiledGranulesLabel = new QLabel(tr("Number of cache tiled granules"), cacheWidget);
    numberCachedTiledGranulesEdit = new QSpinBox(cacheWidget);
    numberCachedTiledGranulesEdit->setMaximum(500);
    numberCachedTiledGranulesEdit->setMinimum(5);
    numberCachedTiledGranulesEdit->setValue(configData->numberCachedTiledGranules);

    cacheGridLayout->addWidget(numberCachedTiledGranulesLabel, 3,0);
    cacheGridLayout->addWidget(numberCachedTiledGranulesEdit, 3,1);

    cacheGridLayout->setAlignment(Qt::AlignTop);

    addTab(cacheWidget, tr("Cache"));
}

void SettingsTabWidget::createOtherWidget()
{
    otherWidget = new QWidget(this);

    QGridLayout* otherGridLayout = new QGridLayout(otherWidget);

    QLabel* TimelineFrequencyLabel = new QLabel(tr("Timeline move frequency"), otherWidget);
    timelineFrequencyEdit = new QSpinBox(otherWidget);
    timelineFrequencyEdit->setMaximum(2000);
    timelineFrequencyEdit->setMinimum(500);
    timelineFrequencyEdit->setSingleStep(100);
    timelineFrequencyEdit->setValue(configData->timeLineMoveFrequency);

    otherGridLayout->addWidget(TimelineFrequencyLabel, 0,0);
    otherGridLayout->addWidget(timelineFrequencyEdit, 0,1);

    QLabel* granuleDisplayNumberLabel = new QLabel(tr("The number of displayed granules"), otherWidget);
    numberDisplayedGranulesEdit = new QSpinBox(otherWidget);
    numberDisplayedGranulesEdit->setMaximum(50);
    numberDisplayedGranulesEdit->setMinimum(5);
    numberDisplayedGranulesEdit->setSingleStep(5);
    numberDisplayedGranulesEdit->setValue(configData->numberDisplayedGranules);

    otherGridLayout->addWidget(granuleDisplayNumberLabel, 1,0);
    otherGridLayout->addWidget(numberDisplayedGranulesEdit, 1,1);

    QLabel* hideStartWidgetLabel = new QLabel(tr("Hide StartWidget during next launch"), otherWidget);
    hideStartWidget = new QCheckBox(this);
    if(configData->hideStartWidget)
        hideStartWidget->setChecked(true);
    else
        hideStartWidget->setChecked(false);

    otherGridLayout->addWidget(hideStartWidgetLabel, 2,0);
    otherGridLayout->addWidget(hideStartWidget, 2,1);

    otherGridLayout->setAlignment(Qt::AlignTop);

    addTab(otherWidget, tr("Other"));
}
