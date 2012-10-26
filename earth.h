#ifndef EARTH_H
#define EARTH_H

#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLSphere>

#include <QImage>

QT_BEGIN_NAMESPACE
class QGLTexture2D;
QT_END_NAMESPACE

class Earth : public QGLSceneNode
{
public:
    Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials);
    ~Earth();
private:
    QList<QGLTexture2D*> m_LoadedTextures;
    QGLTexture2D *m_texture;
    QGraphicsRotation3D *earthRotation;
};

#endif // EARTH_H
