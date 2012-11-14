#ifndef EARTH_H
#define EARTH_H

#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLSphere>
#include <QGLPainter>

#include <QImage>

QT_BEGIN_NAMESPACE
class QGLTexture2D;
QT_END_NAMESPACE

class Earth : public QGLSceneNode
{
public:
    Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials);
    ~Earth();

//    void drawImage(QGLPainter *painter);
    void changeTexture(qreal zoom);
    QGLSceneNode *sphere;

private:
    int zoom;
    QList<QGLTexture2D*> m_LoadedTextures;
    QGLTexture2D *m_texture;
    QGraphicsRotation3D *earthRotation;
    QGLSceneNode *sn1;
    QGLSceneNode *sn2;
    QGLSceneNode *sn3;
    QGLSceneNode *sn4;
    QGLSceneNode *buildEarthNode(qreal radius = 1.0, int divisions = 5, int separation = 1);
};

#endif // EARTH_H
