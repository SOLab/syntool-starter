#ifndef DOWNLOADIMAGE_H
#define DOWNLOADIMAGE_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QPixmap>

#include <QDebug>

class DownloadImage : public QThread
{
    Q_OBJECT
public:
    explicit DownloadImage(QObject *parent = 0);
    void run();
    void setImageUrl(QString imageUrl, QString imagePath);

protected:
    QUrl    m_imageUrl;
    QString m_imagePath;

signals:
    void downloaded(QString);
    
public slots:
    void slotReadyRead(QNetworkReply *reply);
    void getError(QNetworkReply::NetworkError);
};

#endif // DOWNLOADIMAGE_H
