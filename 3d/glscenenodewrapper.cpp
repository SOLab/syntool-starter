#include "glscenenodewrapper.h"

GLSceneNodeWrapper::GLSceneNodeWrapper(QObject *parent) :
    QObject(parent)
{
    m_glSceneNode = NULL;
}

GLSceneNodeWrapper::~GLSceneNodeWrapper()
{
    if(m_glSceneNode)
    {
        m_glSceneNode->palette()->material(m_glSceneNode->materialIndex())->texture()->cleanupResources();
        delete m_glSceneNode->palette()->material(m_glSceneNode->materialIndex())->texture();
        m_glSceneNode->deleteLater();
    }
}

void GLSceneNodeWrapper::setGLSceneNodeObject(QGLSceneNode *glSceneNode)
{
    m_glSceneNode = glSceneNode;
}
