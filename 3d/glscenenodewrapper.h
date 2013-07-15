#ifndef GLSCENENODEWRAPPER_H
#define GLSCENENODEWRAPPER_H

#include <QObject>
#include <QGLSceneNode>
#include "simplegranulesnode.h"

class GLSceneNodeWrapper : public QObject
{
    Q_OBJECT
public:
    explicit GLSceneNodeWrapper(QObject *parent = 0);
    ~GLSceneNodeWrapper();
    void setGLSceneNodeObject(QGLSceneNode *glSceneNode, QGLSceneNode *_parent);
    QGLSceneNode* glSceneNodeObject(){return m_glSceneNode;}
    
private:
    QGLSceneNode* m_glSceneNode;
    QGLSceneNode* m_parent;

signals:
    
public slots:
    
};

#endif // GLSCENENODEWRAPPER_H
