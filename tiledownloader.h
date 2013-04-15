#ifndef TILEDOWNLOADER_H
#define TILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>

struct Sleeper : private QThread
{
    static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
};

class TileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit TileDownloader(QUrl imageUrl, QString imagePath, QObject *parent = 0);

    virtual ~TileDownloader();

    QByteArray downloadedData() const;

signals:
        void resultReady(QString textureStorePath);

private slots:

    void fileDownloaded(QNetworkReply* pReply);

private:
    QString textureStorePath;
    QUrl textureDownloadUrl;

    QNetworkAccessManager m_WebCtrl;

    QByteArray m_DownloadedData;

};

#endif // TILEDOWNLOADER_H
