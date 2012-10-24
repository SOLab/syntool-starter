#ifndef EARTHSCENE_H
#define EARTHSCENE_H
#include "qglabstractscene.h"

QT_BEGIN_NAMESPACE
class QGLSceneNode;
QT_END_NAMESPACE

class EarthScene : public QGLAbstractScene
{
    Q_OBJECT
public:
    explicit EarthScene(QObject *parent = 0);
    virtual QList<QObject *> objects() const;
    QGLSceneNode *mainNode() const { return m_rootNode; }
private:
    QGLSceneNode *m_rootNode;
};
#endif // EARTHSCENE_H
