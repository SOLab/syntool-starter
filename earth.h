#ifndef EARTH_H
#define EARTH_H

#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLSphere>
#include <QGLPainter>

#include "qgltexture2d.h"
#include <QImage>
#include <QTimer>
#include <QObject>
#include <typeinfo>

#include "tiledownloader.h"
#include "more/structure.h"
#include "more/geofunctions.h"
#include "glclasses/tilecache.h"

QT_BEGIN_NAMESPACE
class QGLTexture2D;
QT_END_NAMESPACE

class Earth : public QGLSceneNode
{
    Q_OBJECT
public:
    Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData);
    ~Earth();

//    void drawImage(QGLPainter *painter);
    QGLSceneNode *sphere;

    void tileDownload(qint32 cur_zoom, qint32 separation, qint32 lonTileNum, qint32 latTileNum);

private:
    QCache<QString, TileDownloader> downloadQueue;
    TileCache<TileCacheNumber, QGLSceneNode> tileNodeCache;

    int zoom;
    int zoom_old;

    qreal curScale;
    qreal curZoom;
    GeoCoords curGeoCoords;
    bool newZoomFlag;

//    QList<QGLTexture2D*> m_LoadedTextures;
    QGLTexture2D *m_texture;
    QGraphicsRotation3D *earthRotation;
    QGLSceneNode *sn1;
    QGLSceneNode *sn2;
    QGLSceneNode *sn3;
    QGLSceneNode *sn4;
    void buildEarthNode(qreal radius = 1.0, int divisions = 5, int separation = 1);
    QGLSceneNode* BuildSpherePart(int separation, qreal minSphereLat, qreal maxSphereLat,
                                  qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToTile(QGLSceneNode *tempNode, int separation, int lonTileNum, int latTileNum, int cur_zoom);
    bool checkTextureFile(int separation, int lonTileNum, int latTileNum, int cur_zoom);
    QString cacheDir;

    bool checkNodeInCache(int zoom, int x, int y);
    void cleanupResources();

    QGLSceneNode *BuildGranuleMerNode(int separation, qreal minSphereLat,
                                      qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToGranuleNode(QGLSceneNode *tempNode, QString filepath);

signals:
    void textureDownloadedSignal(qint32 cur_zoom, qint32 lonTileNum, qint32 latTileNum);

public slots:
    void updateTilesSlot(qreal scale, GeoCoords geoCoords);
    void addTileNode(int cur_zoom, qint32 lonTileNum, qint32 latTileNum);
    void addGranuleNodes();
    void textureDownloaded(qint32 cur_zoom, qint32 lonTileNum, qint32 latTileNum);

};

#endif // EARTH_H
