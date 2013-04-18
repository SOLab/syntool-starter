#ifndef EARTH_H
#define EARTH_H

#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLSphere>
#include <QGLPainter>

#include <QImage>
#include "tiledownloader.h"
#include "more/structure.h"
#include "more/geofunctions.h"

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

    void tileDownload(qint32 cur_zoom, qint32 separation, qint32 lonTileNum, qint32 latTileNum);
private:
    int zoom;
    int zoom_old;
    QList<QGLTexture2D*> m_LoadedTextures;
    QGLTexture2D *m_texture;
    QGraphicsRotation3D *earthRotation;
    QGLSceneNode *sn1;
    QGLSceneNode *sn2;
    QGLSceneNode *sn3;
    QGLSceneNode *sn4;
    void buildEarthNode(qreal radius = 1.0, int divisions = 5, int separation = 1);
    QGLSceneNode* BuildSpherePart(int separation, qreal minSphereLat, qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToTile(QGLSceneNode *tempNode, int separation, int lonTileNum, int latTileNum, int cur_zoom);
    bool checkTextureFile(int separation, int lonTileNum, int latTileNum, int cur_zoom);
    QString cacheDir;

public slots:
    void changeTexture(qreal scale);
    void addTileNode(int cur_zoom, qint32 lonTileNum, qint32 latTileNum);
    void textureDownloaded(qint32 cur_zoom, qint32 lonTileNum, qint32 latTileNum);

};

#endif // EARTH_H
