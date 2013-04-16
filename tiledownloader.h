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
    explicit TileDownloader(qint32 _separation, qint32 _lonTileNum, qint32 _latTileNum,
                            qint32 _cur_zoom, QString texStorePath, QObject *parent = 0);

    virtual ~TileDownloader();

    QByteArray downloadedData() const;

signals:
        void resultReady(qint32 cur_zoom, qint32 lonTileNum, qint32 latTileNum);

private slots:

    void fileDownloaded(QNetworkReply* pReply);

private:
    QString textureStorePath;
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;
    qint32 cur_zoom;
    qint32 separation;
    qint32 latTileNum;
    qint32 lonTileNum;
};

#endif // TILEDOWNLOADER_H
