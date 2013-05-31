#ifndef CONFIGFUNCTIONS_H
#define CONFIGFUNCTIONS_H
#include "structure.h"
#include <QMessageBox>
#include <QObject>

inline bool createConfigFile(ConfigData *configData)
{
    QFile configFile(configData->configFile);
    if (!configFile.exists())
    {
        QMessageBox::warning(NULL, QObject::tr("Config file not exists"),
                             QObject::tr("Unable to open: ") + configData->configFile + "\n" +
                             QObject::tr("Will try to create a new file"), "OK");

        QSettings *settings = new QSettings(configData->configFile, QSettings::IniFormat);
        settings->setValue("new/file","");
        settings->sync();
        settings->clear();
        settings->sync();

        if (settings->status() != QSettings::NoError)
        {
            QString errorStr;
            if (settings->status() == QSettings::AccessError)
                errorStr = QObject::tr("Access error");
            else
                errorStr = QObject::tr("Format error");
            QMessageBox::critical(NULL, QObject::tr("Config file error"),
                                 QObject::tr("Cannot create file: ") + settings->fileName() + "\n" +
                                 errorStr + ".", "OK");
            delete settings;
            return false;
        }
        delete settings;
    }
    return true;
}

inline ConfigData* readConfigFile(ConfigData *configData)
{
    QSettings *settings = new QSettings(configData->configFile, QSettings::IniFormat);
    configData->serverName = settings->value("common/server_name", "http://staging.satin.rshu.ru").toString();
    configData->logLevel = (LogLevel::LogLevelValue)settings->value("common/logging_level", LogLevel::DebugOnly).toInt();
    configData->lang = settings->value("common/lang", QLocale::system().name()).toString();

    configData->cacheDir = settings->value("cache/dir", "/tmp/syntool").toString();
    configData->numberCachedTiles = settings->value("cache/number_tiles", 200).toInt();
    configData->numberCachedSimpleGranules = settings->value("cache/number_simple_granules", 50).toInt();
    configData->numberCachedTiledGranules = settings->value("cache/number_tiled_granules", 100).toInt();
    delete settings;
    return configData;
}

#endif // CONFIGFUNCTIONS_H
