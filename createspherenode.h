#ifndef CREATESPHERENODE_H
#define CREATESPHERENODE_H

#include <QThread>

class createSphereNode : public QThread
{
    Q_OBJECT
public:
    explicit createSphereNode(QObject *parent = 0);
    void run();
signals:
    void resultReady(const QString &s);

public slots:
    
};

#endif // CREATESPHERENODE_H
