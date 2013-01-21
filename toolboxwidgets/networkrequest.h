#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QObject>

class NetworkRequest : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequest(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // NETWORKREQUEST_H
