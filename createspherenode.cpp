#include "createspherenode.h"

createSphereNode::createSphereNode(QObject *parent) :
    QThread(parent)
{

}

void createSphereNode::run() Q_DECL_OVERRIDE {
    QString result = "Done!";
    emit resultReady(result);
}
