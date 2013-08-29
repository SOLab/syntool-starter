#include "sarimagenode.h"

SarImageNode::SarImageNode(ConfigData *configData, QString granuleName, QObject *parent) :
    QGLSceneNode(parent)
{
    _configData = configData;
    fileError = false;
    visible = true;
    m_curZoom = -1;
    m_granuleName = granuleName;

    QDir granulePath(_configData->cacheDir);
    if (!QDir(granulePath.path()+"/"+granuleName).exists())
    {
        fileError = true;
        return;
    }
    granulePath.cd(granuleName);
    if (!QDir(granulePath.path()+"/tiles.zxy").exists())
    {
        fileError = true;
        return;
    }

    granulePath.cd("tiles.zxy");

    granuleTilePath = granulePath.path() + "/%1/%2/%3.png";

    tileNodeCache = new QCache<TileCacheNumber, GLSceneNodeWrapper>;
    // set maximum cost for cache
    tileNodeCache->setMaxCost(configData->numberCachedTiles);

    rotateX = new QGraphicsRotation3D(this);
    rotateX->setAngle(180.0f);
    rotateX->setAxis(QVector3D(1,0,0));

    rotateY = new QGraphicsRotation3D(this);
    rotateY->setAngle(90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    qCritical() << "1111111111111111";
//    addTransform(rotateX);
    //    addTransform(rotateY);
}

void SarImageNode::updateTilesSlot(qreal scale, GeoCoords geoCoords)
{
    qCritical() << "update tiles slot!";
}
