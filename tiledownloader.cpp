#include "tiledownloader.h"

Downloader::Downloader()
{
}

void Downloader::download()
{
    QNetworkAccessManager m;
    QNetworkReply * reply = m.get( QNetworkRequest( m_url ) );
    QEventLoop loop;
    connect( reply, SIGNAL(finished()), &loop, SLOT(quit()) );
    loop.exec();

    if (QFile::exists(m_filename))
    {
        return;
    }

//    qDebug() << "filepath" << m_filename;
    if ( reply->error() == QNetworkReply::NoError )
    {
      QFile image( m_filename );
      image.open(QIODevice::WriteOnly);
      image.write( reply->readAll() );
      image.close();
    }
}

