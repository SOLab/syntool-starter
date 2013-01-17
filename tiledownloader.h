#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>
#include <QObject>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

QT_USE_NAMESPACE

class Downloader: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;

public:
    Downloader();
    void download();
    QUrl m_url;
    QString m_filename;
    void setUrl(QUrl url){m_url = url;}
    void setFilename(QString filename){m_filename = filename;}

public slots:
};

#endif // DOWNLOADER_H
