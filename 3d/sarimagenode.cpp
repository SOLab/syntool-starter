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

    QGraphicsRotation3D *rotateX = new QGraphicsRotation3D(this);
    rotateX->setAngle(180.0f);
    rotateX->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D(this);
    rotateY->setAngle(90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(rotateX);
    addTransform(rotateY);
}

void SarImageNode::updateTileRangeSlot(qint32 curZoom, TileRange tileRange1, TileRange tileRange2)
{
    qCritical() << "updateTileRangeSlot" << tileRange1.startX << tileRange1.endX << tileRange1.startY << tileRange1.endY;
    qCritical() << "curZoom" << curZoom << m_curZoom;
//    if (curZoom != m_curZoom)
//    {
//        qCritical() << "!!!!!!!!!!!!!";
//        int separation_old = qPow(2, m_curZoom);
//        for (int y = 0; y < separation_old; y++)
//        {
//            for (int x = 0; x < separation_old; x++)
//            {
//                TileCacheNumber currentCacheNumber = TileCacheNumber(m_curZoom, x, y);

//                if (tileNodeCache->contains(currentCacheNumber))
//                {
//                    tileNodeCache->object(currentCacheNumber)->glSceneNodeObject()->setOptions(QGLSceneNode::HideNode);
//                    removeNode(tileNodeCache->object(currentCacheNumber)->glSceneNodeObject());
//                }
//            }
//        }
//    }

    if (curZoom != m_curZoom)
    {
        qCritical() << "!!!!!!!!!!!!!";
        QList<QGLSceneNode *> childrens = allChildren();
        for (int i = 0; i < childrens.size(); ++i)
        {
            removeNode(childrens.at(i));
        }
        tileNodeCache->clear();
    }


    m_curZoom = curZoom;
    qint32 separation = qPow(2, curZoom);

    TileRange *tileRange=new TileRange[2];
    tileRange[0] = tileRange1;
    tileRange[1] = tileRange2;

//    qCritical() << "wwwwwwwwww" << tileRange1.startX << tileRange1.endX << tileRange1.startY << tileRange1.endY;
    for (int tileRangeNumber = 0; tileRangeNumber <= 1; tileRangeNumber++)
    {
        for (int lonTileNum = tileRange[tileRangeNumber].startX; lonTileNum <= tileRange[tileRangeNumber].endX; lonTileNum++)
        {
            for (int latTileNum = tileRange[tileRangeNumber].startY; latTileNum <= tileRange[tileRangeNumber].endY; latTileNum++)
            {
                if (lonTileNum == 2 && latTileNum == 4)
                    qCritical() << "444444444"<< curZoom << lonTileNum << latTileNum;
                if (!checkNodeInCache(curZoom, lonTileNum, latTileNum))
                {
                    if (lonTileNum == 2 && latTileNum == 4)
                        qCritical() << "55555555";
                    addTileNode(curZoom, lonTileNum, separation-1-latTileNum);
                }
            }
        }
        if (tileRange[tileRangeNumber].end)
            break;
    }
}

void SarImageNode::updateAllTileSlot(qint32 curZoom)
{
    qCritical() << "updateAllTileSlot";

    if (curZoom != m_curZoom)
    {
        int separation_old = qPow(2, m_curZoom);
        for (int y = 0; y < separation_old; y++)
        {
            for (int x = 0; x < separation_old; x++)
            {
                TileCacheNumber currentCacheNumber = TileCacheNumber(m_curZoom, x, y);

                if (tileNodeCache->contains(currentCacheNumber))
                {
                    tileNodeCache->object(currentCacheNumber)->glSceneNodeObject()->setOptions(QGLSceneNode::HideNode);
                    removeNode(tileNodeCache->object(currentCacheNumber)->glSceneNodeObject());
                }
            }
        }
    }
//    QList<QGLSceneNode *> childrens = allChildren();
//    for (int i = 0; i < childrens.size(); ++i)
//    {
//        removeNode(childrens.at(i));
//    }
//    tileNodeCache->clear();

    m_curZoom = curZoom;
    qint32 separation = qPow(2, curZoom);

    for (int lonTileNum = 0; lonTileNum < separation; lonTileNum++)
    {
        for (int latTileNum = 0; latTileNum < separation; latTileNum++)
        {
//            qCritical() << "11111111111";
            if (!checkNodeInCache(curZoom, lonTileNum, separation-1-latTileNum))
            {
//                qCritical() << "22222222222";
                addTileNode(curZoom, lonTileNum, latTileNum);
            }
        }
    }
}

bool SarImageNode::addTextureToTile(QGLSceneNode *tempNode, qint32 separation,
                                    qint32 lonTileNum, qint32 latTileNum, qint32 curZoom)
{
    QGLTexture2D* tex;
    tex = new QGLTexture2D();

    QString filepath = QString(granuleTilePath).arg(curZoom).arg(lonTileNum).arg(separation-1-latTileNum);

    if (!QFile::exists(filepath))
    {
        return false;
    }

    QUrl url;
    url.setPath(filepath);
    url.setScheme(QLatin1String("file"));
    tex->setUrl(url);

    QGLMaterial *mat1 = new QGLMaterial;
    mat1->setTexture(tex, 0);

    int earthMat = tempNode->palette()->addMaterial(mat1);

    tempNode->setMaterialIndex(earthMat);
    tempNode->setEffect(QGL::LitModulateTexture2D);
    return true;
}

bool SarImageNode::checkNodeInCache(qint32 zoom, qint32 x, qint32 y)
{
    TileCacheNumber currentCacheNumber = TileCacheNumber(zoom, x, y);
    if (tileNodeCache->contains(currentCacheNumber))
    {
        QGLSceneNode* sceneNode = tileNodeCache->object(currentCacheNumber)->glSceneNodeObject();
        qCritical() << "aaaaaaaaa";
        if (sceneNode->options().testFlag(QGLSceneNode::HideNode))
        {
            qCritical() << "bbbbbbbbbbb";
            sceneNode->setOptions(QGLSceneNode::CullBoundingBox);
            addNode(sceneNode);
        }

        qCritical() << "cccccccccc";

//        emit displayed();
        return true;
    }
    return false;
}

void SarImageNode::addTileNode(int curZoom, qint32 lonTileNum, qint32 latTileNum)
{
    int separation = qPow(2, curZoom);
    QString texFilePath = QString(granuleTilePath).arg(curZoom).
                          arg(lonTileNum).arg(separation-1-latTileNum);
    if (!QFile::exists(texFilePath))
        return;

    if(curZoom != m_curZoom)
        return;
    qCritical() << "=====>" << lonTileNum << latTileNum;

    qreal NTLon = 2*M_PI / qreal(separation);

    qreal minSphereLat = tiley2lat(latTileNum+1, separation)/180.0*M_PI;
    qreal maxSphereLat = tiley2lat(latTileNum, separation)/180.0*M_PI;

    qreal minSphereLon = ((lonTileNum) * NTLon - M_PI);
    qreal maxSphereLon = ((lonTileNum+1) * NTLon - M_PI);

    qCritical() << minSphereLat << maxSphereLat << minSphereLon << maxSphereLon;
    QGLSceneNode* sceneNode = BuildSpherePart(separation, minSphereLat, maxSphereLat,
                                             minSphereLon, maxSphereLon);

    QString nodeObjectName = QString(m_granuleName+"-%1-%2-%3").arg(curZoom).arg(lonTileNum)
                                                     .arg(separation-1-latTileNum);

    if (addTextureToTile(sceneNode, separation, lonTileNum, latTileNum, curZoom))
    {
        if(curZoom != m_curZoom)
        {
            delete sceneNode;
            return;
        }
        sceneNode->setOptions(QGLSceneNode::CullBoundingBox);

        // add SceneNode to cache
        TileCacheNumber tileNumber = TileCacheNumber(curZoom, lonTileNum,separation-1-latTileNum);

        GLSceneNodeWrapper* sceneNodeWrapper = new GLSceneNodeWrapper;
        sceneNodeWrapper->setGLSceneNodeObject(sceneNode, this);

        tileNodeCache->insert(tileNumber, sceneNodeWrapper);
        sceneNode->setObjectName(nodeObjectName);

        addNode(sceneNode);
        qCritical() << tr("Add GLSceneNone object") << nodeObjectName;
    }
    else
    {
        delete sceneNode;
        qCritical() << tr("Texture not added");
    }
    emit displayed();
}

QGLSceneNode *SarImageNode::BuildSpherePart(qint32 separation, qreal minSphereLat, qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon)
{
    m_altitude = 0.0001 + height()/10000.0;
    qCritical() << m_altitude;
    qreal minMerLat = Lat2MercatorLatitude(minSphereLat);
    qreal maxMerLat = Lat2MercatorLatitude(maxSphereLat);

    // all stacks and slices
    int stacks = 64;
    int slices = 64;
    if (curZoom > 5)
    {
        stacks = qPow(2, qFloor(curZoom));
        slices = qPow(2, qFloor(curZoom));
    }

    // stacks and slices for one tile
//    qreal stacksForOne = stacks/(float)separation;
//    qreal slicesForOne = slices/(float)separation;

    qreal oneSphereStackDegrees = (maxSphereLat - minSphereLat) / qreal(stacks/(qreal)separation);
    qreal oneSphereSliceDegrees = (maxSphereLon - minSphereLon) / qreal(slices/(qreal)separation);

    QVector3D curDecart;
    QVector3D curDecartNext;

    qreal curMerLat;
    qreal curMerLatNext;
    qreal curSphereLatNext;

    QGLBuilder tempBuilder;
    QGeometryData prim;

    double yTexCoord;
    double yTexCoordNext;
    double xTexCoord;
    double xTexCoordNext;

    // maxSphereLon with with an error of calculations (approximately)
    bool firstFlag = true;
    bool lastFlag = false;
    qreal curSphereLon, curSphereLat;

    for (int lat_iter = 0; lat_iter <= stacks/(qreal)separation; lat_iter++)
    {
        curSphereLat = minSphereLat + lat_iter * oneSphereStackDegrees;
        // calculate next point latitude
        curSphereLatNext = curSphereLat + oneSphereStackDegrees;

        // if next latitude is max latitude
        if (qAbs(curSphereLatNext - maxSphereLat) < 0.0001)
        {
            curSphereLatNext = maxSphereLat;
            curSphereLat = maxSphereLat - oneSphereStackDegrees;
            lastFlag = true;
        }

        // calculate mercator latitude
        curMerLat = Lat2MercatorLatitude(curSphereLat);
        curMerLatNext = Lat2MercatorLatitude(curSphereLatNext);

        // calculate xTexCoord for this lat and next lat
        yTexCoord = (curMerLat - minMerLat) / qreal(maxMerLat - minMerLat);
        yTexCoordNext = (curMerLatNext - minMerLat) / qreal(maxMerLat - minMerLat);

        prim.clear();

        for (int lon_iter = 0; lon_iter <= slices/(qreal)separation; lon_iter++)
        {
            curSphereLon = minSphereLon + lon_iter * oneSphereSliceDegrees;
            // calculate decart coordinates (x,y,z) for Vertex and Normal
            curDecart = llh2xyz((curSphereLat), curSphereLon, m_altitude);

            // calculate texture coordinates
            xTexCoord = ((curSphereLon) - (minSphereLon))/qreal((maxSphereLon) - (minSphereLon));
            if (firstFlag)
                yTexCoord = 0;

            prim.appendVertex(curDecart);
            prim.appendNormal(curDecart);
            prim.appendTexCoord(QVector2D(xTexCoord, (yTexCoord)));

            // all for next point
            curDecartNext = llh2xyz((curSphereLatNext), curSphereLon, m_altitude);
            xTexCoordNext = ((curSphereLon) - (minSphereLon)) / qreal((maxSphereLon) - (minSphereLon));
            if (lastFlag)
                yTexCoordNext = 1;

            prim.appendVertex(curDecartNext);
            prim.appendNormal(curDecartNext);
            prim.appendTexCoord(QVector2D(xTexCoordNext, (yTexCoordNext)));
        }

        // add QGeometryData to our builder
        tempBuilder.addQuadStrip(prim);
        firstFlag = false;
        if (lastFlag)
            break;
    }

    return tempBuilder.finalizedSceneNode();
}

void SarImageNode::show()
{
    visible = true;
    setOptions(QGLSceneNode::CullBoundingBox);
}

void SarImageNode::hide()
{
    qCritical() << "HIDEHIDEHIDEHIDE";
    visible = false;
    setOptions(QGLSceneNode::HideNode);
}
