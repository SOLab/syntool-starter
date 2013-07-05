/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "3d/earthview.h"
#include "mainwidget.h"
#include "myapplication.h"
#include "more/structure.h"
#include "more/configfunctions.h"

#include <QGuiApplication>
#include <QDir>
#include <QProcess>
#include <QObject>
#include <QSettings>

int main(int argc, char *argv[])
{
    myApplication app(argc, argv);

    // new app font
    QFont newFont("Times New Roman", 11, QFont::Normal, false);
    // apply to all
    QApplication::setFont(newFont);

    ConfigData *configData = new ConfigData;
    configData->selectedProducts = new QList<qint32>;
    configData->favoriteProducts = new QList<qint32>;

    configData->configDir = QDir::homePath();
    configData->configFile = configData->configDir + "/syntool.cfg";

    if (!createConfigFile(configData))
    {
        app.exit(1);
        return 1;
    }
    configData = readConfigFile(configData);

    QTranslator myTranslator;
    myTranslator.load(configData->lang, "../");
    app.installTranslator(&myTranslator);

    if (configData->lang.startsWith("ru"))
        QLocale::setDefault(QLocale("ru_RU"));
    else
        QLocale::setDefault(QLocale("en_EN"));

    QDir cacheDir(configData->cacheDir);
    if (!cacheDir.exists()){
        cacheDir.mkdir(cacheDir.path());
    }

    clearCacheDir(configData->cacheDir);

    QWindow* win = new QWindow();
    QSurfaceFormat surfaceFormat = QSurfaceFormat();
    surfaceFormat.setStereo(false);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    surfaceFormat.setDepthBufferSize(24);
    EarthView view(configData, surfaceFormat, win);
    app.set_view(&view);

//    // process id
    qDebug() << QObject::tr("Process id:") << app.applicationPid();

//    view->resize(800, 600);

//    QStringList args = QCoreApplication::arguments();
//    int w_pos = args.indexOf("-width");
//    int h_pos = args.indexOf("-height");
//    if (w_pos >= 0 && h_pos >= 0)
//    {
//        bool ok = true;
//        int w = args.at(w_pos + 1).toInt(&ok);
//        if (!ok)
//        {
//            qWarning() << "Could not parse width argument:" << args;
//            return 1;
//        }
//        int h = args.at(h_pos + 1).toInt(&ok);
//        if (!ok)
//        {
//            qWarning() << "Could not parse height argument:" << args;
//            return 1;
//        }
//        view.resize(w, h);
//    }
//    else
//    {
//        view.resize(800, 600);
//    }

    MainWindow wgt(configData);
    wgt.setHostedWindow(&view);
    wgt.resize(900,900);

    app.setMainWidget(&wgt);

    app.setWindowIcon(QIcon(":/icons/layers.png"));
    wgt.show();

    return app.exec();
}
