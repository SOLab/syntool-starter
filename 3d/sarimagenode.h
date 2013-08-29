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

    QGraphicsRotation3D *rotateX;
    QGraphicsRotation3D *rotateY;
signals:
    
public slots:
    void updateTilesSlot(qreal scale, GeoCoords geoCoords);

};

#endif // SARIMAGENODE_H
