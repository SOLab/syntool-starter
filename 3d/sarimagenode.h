#ifndef SARIMAGENODE_H
#define SARIMAGENODE_H

#include <QGLSceneNode>
#include <QDir>

#include "3d/tilecachenumbersclass.h"
#include "3d/glscenenodewrapper.h"

class SarImageNode : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit SarImageNode(ConfigData *configData, QString granuleName, QObject *parent = 0);

    qint32  height(){return m_height;}
    void    setHeight(qint32 height){m_height = height;}

    void show();
    void hide();
private:
    QCache<TileCacheNumber, GLSceneNodeWrapper>* tileNodeCache;

    qreal       curZoom;
    ConfigData* _configData;
    QString     granuleTilePath;

    qint32  m_curZoom;
    qint32  m_height;
    qreal   m_altitude;
    QString m_granuleName;
    bool    fileError;
    bool    visible;

    // private methods
    bool          addTextureToTile(QGLSceneNode *tempNode, qint32 separation,
                                   qint32 lonTileNum, qint32 latTileNum, qint32 cur_zoom);
    QGLSceneNode* BuildSpherePart(qint32 separation, qreal minSphereLat, qreal maxSphereLat,
                                  qreal minSphereLon, qreal maxSphereLon);
    bool          checkNodeInCache(qint32 zoom, qint32 x, qint32 y);
signals:
    
public slots:
    void addTileNode(int curZoom, qint32 lonTileNum, qint32 latTileNum);
    void updateTileRangeSlot(qint32 curZoom, TileRange tileRange1, TileRange tileRange2);
    void updateAllTileSlot(qint32 curZoom);
    
};

#endif // SARIMAGENODE_H
