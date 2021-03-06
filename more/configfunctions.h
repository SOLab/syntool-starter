#ifndef CONFIGFUNCTIONS_H
#define CONFIGFUNCTIONS_H
#include "structure.h"
#include <QMessageBox>
#include <QObject>
#include <QSettings>

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
    configData->logLevel = (LogLevel::LogLevelValue)settings->value("common/logging_level", LogLevel::ErrorOnly).toInt();
    configData->lang = settings->value("common/lang", QLocale::system().name()).toString();
    configData->mapThemeName = settings->value("common/map_theme", "googleSatellite").toString();

    configData->cacheDir = settings->value("cache/dir", "/tmp/syntool").toString();
    configData->numberCachedTiles = settings->value("cache/number_tiles", 200).toInt();
    configData->numberCachedSimpleGranules = settings->value("cache/number_simple_granules", 50).toInt();
    configData->numberCachedTiledGranules = settings->value("cache/number_tiled_granules", 25).toInt();

    configData->timeLineMoveFrequency = settings->value("other/timeline_move_frequency", 500).toInt();
    configData->numberDisplayedGranules = settings->value("other/number_displayed_granules", 20).toInt();

    configData->datetime = settings->value("other/current_datetime",
                                           QDateTime::currentDateTime().toString(Qt::ISODate)).toDateTime();

    configData->hideStartWidget = settings->value("other/hide_start_widget", 0).toInt();

    configData->favoriteProducts->clear();
    QStringList favoriteProductsStringList = settings->value("other/favorite_products").toStringList();
    foreach(QString n, favoriteProductsStringList) configData->favoriteProducts->append(n.toInt());

    configData->selectedProducts->clear();
    QStringList addedProductsStringList = settings->value("other/selected_products").toStringList();
    foreach(QString n, addedProductsStringList) configData->selectedProducts->append(n.toInt());

    delete settings;
    return configData;
}

inline void writeTimeProductsToConfig(ConfigData* configData)
{
    QSettings *settings = new QSettings(configData->configFile, QSettings::IniFormat);

    settings->setValue("other/current_datetime", configData->datetime);

    QStringList favoriteProductsStringList;
    foreach(int n, *configData->favoriteProducts) favoriteProductsStringList << QString::number(n);
    settings->setValue("other/favorite_products", favoriteProductsStringList);

    QStringList addedProductsStringList;
    foreach(int n, *configData->selectedProducts) addedProductsStringList << QString::number(n);
    settings->setValue("other/selected_products", addedProductsStringList);

    settings->sync();
    delete settings;

//    configData = readConfigFile(configData);
}

// remove all granules and tiles from cache
inline void clearCacheDir(QString cacheDir)
{
    // list files from cache dir
    QDir dir(cacheDir);
    QStringList lstFiles = dir.entryList(QDir::Files);
    QRegExp rxPNG(dir.absolutePath() + "/"+"(\\d+)-(\\d+)-(\\d+).png");
    QRegExp rxJPG(dir.absolutePath() + "/"+"(\\d+)-(\\d+)-(\\d+).jpg");

    QRegExp rxGranule(dir.absolutePath() + "/"+"granule_(\\d+).png");

    //remove files (earth tile e.g. 0-0-0.png)
    foreach (QString entry, lstFiles)
    {
        QString entryAbsPath = dir.absolutePath() + "/" + entry;
        if (rxPNG.exactMatch(entryAbsPath))
            QFile::remove(entryAbsPath);
        else if (rxJPG.exactMatch(entryAbsPath))
            QFile::remove(entryAbsPath);
        else if (rxGranule.exactMatch(entryAbsPath))
            QFile::remove(entryAbsPath);
    }
}

#endif // CONFIGFUNCTIONS_H
