#ifndef GLSCENENODEWRAPPER_H
#define GLSCENENODEWRAPPER_H

#include <QObject>
#include <QGLSceneNode>

class GLSceneNodeWrapper : public QObject
{
    Q_OBJECT
public:
    explicit GLSceneNodeWrapper(QObject *parent = 0);
    void setGLSceneNodeObject(QGLSceneNode* glSceneNodeObject){_glSceneNodeObject = glSceneNodeObject;}
    QGLSceneNode* getGLSceneNodeObject(){return _glSceneNodeObject;}
    
private:
    QGLSceneNode* _glSceneNodeObject;

signals:
    
public slots:
    
};

#endif // GLSCENENODEWRAPPER_H
