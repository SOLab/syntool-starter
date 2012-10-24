#include "earthscene.h"
#include "qglscenenode.h"

EarthScene::EarthScene(QObject *parent)
    : QGLAbstractScene(parent)
    , m_rootNode(new QGLSceneNode(this))
{
}

QList<QObject *> EarthScene::objects() const
{
    QList<QGLSceneNode *> children = m_rootNode->allChildren();
    QList<QObject *> objects;
    for (int index = 0; index < children.size(); ++index)
        objects.append(children.at(index));
    return objects;
}
