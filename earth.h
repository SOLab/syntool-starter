#ifndef EARTH_H
#define EARTH_H

#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLSphere>
#include <QGLPainter>

#include <QImage>
#include "tiledownloader.h"
#include "more/structure.h"

QT_BEGIN_NAMESPACE
class QGLTexture2D;
QT_END_NAMESPACE

class Earth : public QGLSceneNode
{
public:
    Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData);
    ~Earth();

//    void drawImage(QGLPainter *painter);
    QGLSceneNode *sphere;

    QString tileDownload(int tx, int ty, int zoom);
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
    QGLSceneNode* BuildSpherePart(qreal startU, qreal stepU, int numU, qreal startV, qreal stepV, int numV);
    QString cacheDir;

public slots:
    void changeTexture(qreal zoom);
    void addTileNode(QGLBuilder *builder, qreal radius, int divisions, int cur_zoom, qint32 lonTileNum, qint32 latTileNum);

};

#endif // EARTH_H
