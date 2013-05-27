#include "glscenenodewrapper.h"

GLSceneNodeWrapper::GLSceneNodeWrapper(QObject *parent) :
    QObject(parent)
{
    _glSceneNode = NULL;
}

GLSceneNodeWrapper::~GLSceneNodeWrapper()
{
    if(_glSceneNode)
    {
        delete _glSceneNode->palette()->material(_glSceneNode->materialIndex())->texture();
        _glSceneNode->deleteLater();
    }
}

void GLSceneNodeWrapper::setGLSceneNodeObject(QGLSceneNode *glSceneNode)
{
    _glSceneNode = glSceneNode;
}
