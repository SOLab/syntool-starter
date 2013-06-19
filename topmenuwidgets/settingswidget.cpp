#include "settingswidget.h"

SettingsWidget::SettingsWidget(ConfigData *configValue, QWidget *parent) :
    QWidget(parent)
{
    configData = configValue;
    vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    setFixedSize(450, 260);
    setWindowTitle(tr("Tools"));

    setWindowIcon(QIcon(":/icon/settings.png"));

    // for clear memory after closed this window
    setAttribute(Qt::WA_ShowModal);
    setAttribute(Qt::WA_DeleteOnClose);

    createForm();
    //    gui_signal = QtCore.Signal()
    //    other_signal = QtCore.Signal()
}

void SettingsWidget::createForm()
{
//    if self.layout():
//        for i in range(self.layout().count()):
//            child = self.layout().takeAt(0)
//            child.widget().hide()
//            child.widget().destroy()
//            del (child)

    // Add clear config button
    clearConfigButton = new QPushButton(QIcon(":/icons/clear.png"),
                                        tr("Clear the configuration file") + ": " + configData->configFile, this);
//    clearConfigButton->setMaximumWidth(380);
    connect(clearConfigButton, &QPushButton::clicked, this, &SettingsWidget::clearConfig);

    vLayout->addWidget(clearConfigButton);

    mainTabWidget = new SettingsTabWidget(configData, this);
    vLayout->addWidget(mainTabWidget);

    // add ok, apply and cancel button
    buttonsWidget = new QWidget(this);
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->setAlignment(Qt::AlignRight);

    okButton = new QPushButton(tr("Ok"), this);
    okButton->setFixedWidth(50);
    okButton->setShortcut(QKeySequence(Qt::Key_Return));
    okButton->setToolTip(tr("Save changes to the current tab and exit"));
    connect(okButton, &QPushButton::clicked, this, &SettingsWidget::saveChanges);
    connect(okButton, &QPushButton::clicked, this, &SettingsWidget::close);
    buttonsLayout->addWidget(okButton);

    applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setFixedWidth(100);
    applyButton->setToolTip(tr("Save changes to the current tab"));
    connect(applyButton, &QPushButton::clicked, this, &SettingsWidget::saveChanges);
    buttonsLayout->addWidget(applyButton);

    cancelButton = new QPushButton(tr("Cancel"), this);
    cancelButton->setFixedWidth(100);
    cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWidget::close);
    buttonsLayout->addWidget(cancelButton);

    buttonsWidget->setLayout(buttonsLayout);

//    self.gui_signal.connect(tab.GuiWidget.save_changes(ClientObj))
//    self.other_signal.connect(tab.OtherWidget.save_changes(ClientObj))

    vLayout->addWidget(buttonsWidget);
}

void SettingsWidget::saveChanges()
{
    QSettings *settings = new QSettings(configData->configFile, QSettings::IniFormat);
    // if common
    if (mainTabWidget->currentIndex() == 0)
    {
        settings->setValue("common/server_name",mainTabWidget->serverNameEdit->text());
        settings->setValue("common/logging_level",
                           mainTabWidget->logLevelCombo->itemData(mainTabWidget->logLevelCombo->currentIndex()).toInt());
        settings->setValue("common/lang",
                           mainTabWidget->langCombo->itemData(mainTabWidget->langCombo->currentIndex()).toString());
    }
    // if cache
    else if (mainTabWidget->currentIndex() == 1)
    {
        settings->setValue("cache/dir",mainTabWidget->cacheDirEdit->value());
        settings->setValue("cache/number_tiles",mainTabWidget->numberCachedTilesEdit->value());
        settings->setValue("cache/number_simple_granules",mainTabWidget->numberCachedSimpleGranulesEdit->value());
        settings->setValue("cache/number_tiled_granules",mainTabWidget->numberCachedTiledGranulesEdit->value());
    }
    // if other
    else if (mainTabWidget->currentIndex() == 2)
    {
        settings->setValue("other/timeline_move_frequency",mainTabWidget->timelineFrequencyEdit->value());
    }
    settings->sync();
    delete settings;

    configData = readConfigFile(configData);
}

void SettingsWidget::clearConfig()
{
    QSettings *settings = new QSettings(configData->configFile, QSettings::IniFormat);
    settings->clear();
    settings->sync();
    delete settings;
    configData = readConfigFile(configData);

    vLayout->removeWidget(clearConfigButton);
    clearConfigButton->close();
    vLayout->removeWidget(mainTabWidget);
    mainTabWidget->close();
    vLayout->addWidget(buttonsWidget);
    buttonsWidget->close();

    createForm();
}
