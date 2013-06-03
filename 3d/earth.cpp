#include "earth.h"

const double a = 6378137.0;

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData *configData)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);
    cacheDir = configData->cacheDir;

    // set the maximum number of threads to download images tiles
    downloadQueue = new QCache<QString, TileDownloader>;
    downloadQueue->setMaxCost(25);

    // set maximum cost for cache
    tileNodeCache.setMaxCost(configData->numberCachedTiles);
//    m_LoadedTextures.setMaxCost(50);

    buildEarthNode(a, 10, 0);

    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D(this);
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D(this);
    rotateY->setAngle(-90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(axialTilt1);
    addTransform(rotateY);

//    addNode(sphere);

    zoom = -1;
    zoom_old = 0;

    connect(this, &Earth::textureDownloadedSignal, this, &Earth::textureDownloaded);

//    tileDownloader = new TileDownloader();
}

/*!
 * Check availability QGLSceneNode* in tileNodeCache
 * and the addition of node if contains
 */
bool Earth::checkNodeInCache(int zoom, int x, int y)
{
    TileCacheNumber currentCacheNumber = TileCacheNumber(zoom, x, y);
    if (tileNodeCache.contains(currentCacheNumber))
    {
        QGLSceneNode* sceneNode = tileNodeCache.object(currentCacheNumber)->glSceneNodeObject();
        if (sceneNode->options().testFlag(QGLSceneNode::HideNode))
        {
            sceneNode->setOptions(QGLSceneNode::CullBoundingBox);
            addNode(sceneNode);
        }

        emit displayed();
        return true;
    }
    return false;
}

/*!
    Add all tiles node for current zoom (considering bbox).
*/
void Earth::buildEarthNode(qreal radius, int divisions, int cur_zoom)
{
    Q_UNUSED(radius);
    Q_UNUSED(divisions);
    qreal separation = qPow(2, cur_zoom);

    if (cur_zoom > 2){
        int numberTiles = qCeil(getNumberTiles(cur_zoom, 2*40000/curScale));

        TileNumber tileNumber = deg2TileNum(curGeoCoords, cur_zoom);

        TileRange* tileRange = getTileRange(tileNumber, numberTiles, cur_zoom);
//        qCritical() << "=========>";
//        qCritical() << tileRange[0].startX << tileRange[0].endX << tileRange[0].startY << tileRange[0].endY << tileRange[0].end;
//        qCritical() << tileRange[1].startX << tileRange[1].endX << tileRange[1].startY << tileRange[1].endY;

        for (int tileRangeNumber = 0; tileRangeNumber <= 1; tileRangeNumber++)
        {
            for (int lonTileNum = tileRange[tileRangeNumber].startX; lonTileNum <= tileRange[tileRangeNumber].endX; lonTileNum++)
            {
                for (int latTileNum = tileRange[tileRangeNumber].startY; latTileNum <= tileRange[tileRangeNumber].endY; latTileNum++)
                {
                    if (!checkNodeInCache(cur_zoom, lonTileNum, latTileNum))
                    {
                        addTileNode(cur_zoom, lonTileNum, separation-1-latTileNum);
                    }
                }
            }
            if (tileRange[tileRangeNumber].end)
                break;
        }
        delete tileRange;
    }
    else
    {
        for (int lonTileNum = 0; lonTileNum < separation; lonTileNum++)
        {
            for (int latTileNum = 0; latTileNum < separation; latTileNum++)
            {
                if (!checkNodeInCache(cur_zoom, lonTileNum, separation-1-latTileNum))
                {
                    addTileNode(cur_zoom, lonTileNum, latTileNum);
                }
            }
        }
    }
}

/*!
    Wrapper to add a one tile to the sphere.
    If texture file not exist then run new thread (from method tileDownload).
*/
void Earth::addTileNode(int cur_zoom, qint32 lonTileNum, qint32 latTileNum)
{
    int separation = qPow(2, cur_zoom);

    qWarning() << cur_zoom;
    if (!checkTextureFile(separation, lonTileNum, latTileNum, cur_zoom))
    {
        tileDownload(cur_zoom, separation, lonTileNum, latTileNum);
        return;
    }
//    emit textureDownloadedSignal(cur_zoom, lonTileNum, latTileNum);
    textureDownloaded(cur_zoom, lonTileNum, latTileNum);
}

/*!
    starts then texture exists or after download texture.
    This method calling all methods for create tile, overlay texture and
         add QGLSceneNode to scene (BuildSpherePart, addTextureToTile, addNode)
*/
void Earth::textureDownloaded(qint32 cur_zoom, qint32 lonTileNum, qint32 latTileNum)
{
    if(cur_zoom != zoom)
        return;

    int separation = qPow(2, cur_zoom);

    qreal NTLon = 2*M_PI / separation;

    qreal minSphereLat = -tiley2lat(latTileNum, separation)/180.0*M_PI;
    qreal maxSphereLat = -tiley2lat(latTileNum+1, separation)/180.0*M_PI;

    qreal minSphereLon = ((lonTileNum) * NTLon - M_PI);
    qreal maxSphereLon = ((lonTileNum+1) * NTLon - M_PI);

    QGLSceneNode* sceneNode = BuildSpherePart(separation, minSphereLat, maxSphereLat,
                                             minSphereLon, maxSphereLon);

    QString nodeObjectName = QString("tile-%1-%2-%3").arg(cur_zoom).arg(lonTileNum)
                                                     .arg(separation-1-latTileNum);

    if (addTextureToTile(sceneNode, separation, lonTileNum, latTileNum, cur_zoom))
    {
        if(cur_zoom != zoom)
        {
            delete sceneNode;
            return;
        }
        sceneNode->setOptions(QGLSceneNode::CullBoundingBox);

        // add SceneNode to cache
        TileCacheNumber tileNumber = TileCacheNumber(cur_zoom, lonTileNum,separation-1-latTileNum);

        GLSceneNodeWrapper* sceneNodeWrapper = new GLSceneNodeWrapper;
        sceneNodeWrapper->setGLSceneNodeObject(sceneNode);

        tileNodeCache.insert(tileNumber, sceneNodeWrapper);
        sceneNode->setObjectName(nodeObjectName);

        addNode(sceneNode);
        qDebug() << tr("Add GLSceneNone object") << nodeObjectName;
    }
    else
    {
        qWarning() << tr("Texture not added");
    }
    emit displayed();
}

/*!
    Create QGLSceneNode for one tile.
    stacks - number pieces of tatitude for one tile,
    slices - number pieces of longitude for one tile,
*/
QGLSceneNode* Earth::BuildSpherePart(int separation, qreal minSphereLat, qreal maxSphereLat,
                                     qreal minSphereLon, qreal maxSphereLon)
{
    // calculate spherical min and max lon and lat
    qreal minMerLat = Lat2MercatorLatitude(minSphereLat);
    qreal maxMerLat = Lat2MercatorLatitude(maxSphereLat);

    // all stacks and slices
    int stacks = 128;
    int slices = 128;
    if (curZoom > 7)
    {
        stacks = qPow(2, curZoom);
        slices = qPow(2, curZoom);
    }

    // stacks and slices for one tile
//    qreal stacksForOne = stacks/(float)separation;
//    qreal slicesForOne = slices/(float)separation;

    qreal oneSphereStackDegrees = (maxSphereLat - minSphereLat) / (stacks/(float)separation);
    qreal oneSphereSliceDegrees = (maxSphereLon - minSphereLon) / (slices/(float)separation);

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
    qreal maxSphereLonApprox = maxSphereLon+0.0001;
    bool firstFlag = true;
    bool lastFlag = false;

    for (qreal curSphereLat = minSphereLat; curSphereLat < maxSphereLat;
                                            curSphereLat+=oneSphereStackDegrees) {
        // calculate next point latitude
        curSphereLatNext = curSphereLat + oneSphereStackDegrees;

        // if next latitude is max latitude
        if (qAbs(curSphereLatNext - maxSphereLat) < 0.001)
        {
            curSphereLatNext = maxSphereLat;
            curSphereLat = maxSphereLat - oneSphereStackDegrees;
            lastFlag = true;
        }

        // calculate mercator latitude
        curMerLat = Lat2MercatorLatitude(curSphereLat);
        curMerLatNext = Lat2MercatorLatitude(curSphereLatNext);

        prim.clear();
        for (qreal curSphereLon = minSphereLon; curSphereLon < maxSphereLonApprox;
                                                curSphereLon += oneSphereSliceDegrees)
        {
            // calculate decart coordinates (x,y,z) for Vertex and Normal
            curDecart = llh2xyz((curSphereLat), curSphereLon);

            // calculate texture coordinates
            xTexCoord = ((curSphereLon) - (minSphereLon))/qreal((maxSphereLon) - (minSphereLon));
            if (firstFlag)
                yTexCoord = 0;
            else
                yTexCoord = (curMerLat - minMerLat) / qreal(maxMerLat - minMerLat);

            prim.appendVertex(curDecart);
            prim.appendNormal(curDecart);
            prim.appendTexCoord(QVector2D(xTexCoord, (yTexCoord)));

            // all for next point
            curDecartNext = llh2xyz((curSphereLatNext), curSphereLon);
            xTexCoordNext = ((curSphereLon) - (minSphereLon)) / qreal((maxSphereLon) - (minSphereLon));
            if (lastFlag)
                yTexCoordNext = 1;
            else
                yTexCoordNext = (curMerLatNext - minMerLat) / qreal(maxMerLat - minMerLat);

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

/*!
    verification of the existence texture file in cache directory.
    return true if file exist.
*/
bool Earth::checkTextureFile(int separation, int lonTileNum, int latTileNum, int cur_zoom)
{
    QString texFilePath = QString(cacheDir+"/%1-%2-%3.png").arg(cur_zoom).
                                                arg(lonTileNum).arg(separation-1-latTileNum);
    return QFile::exists(texFilePath);
}

/*!
    add texture to current QGLSceneNode object.
*/
bool Earth::addTextureToTile(QGLSceneNode* tempNode, int separation, int lonTileNum,
                             int latTileNum, int cur_zoom)
{
//    if (separation > 1){
    QGLTexture2D* tex;
    tex = new QGLTexture2D();
//    tex->setSize(QSize(512, 256));

    QString t_filepath = cacheDir+"/%1-%2-%3.png";
    QString filepath(t_filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

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

//    m_LoadedTextures.insert(filepath, mat1->texture(0));
//    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = tempNode->palette()->addMaterial(mat1);

    tempNode->setMaterialIndex(earthMat);
    tempNode->setEffect(QGL::LitModulateTexture2D);
    return true;
}

/*!
    starts a thread for downloading tile texture.
*/
void Earth::tileDownload(qint32 cur_zoom, qint32 separation, qint32 lonTileNum, qint32 latTileNum)
{
    QString m_filepath = cacheDir+"/%1-%2-%3.png";
    QString textureStorePath = (m_filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

    TileDownloader *tileDownloader = new TileDownloader(separation, lonTileNum, latTileNum,
                                                        cur_zoom, textureStorePath);

    downloadQueue->insert(textureStorePath, tileDownloader);

    QObject::connect(tileDownloader, &TileDownloader::resultReady,
                     this, &Earth::textureDownloaded);
//    QObject::connect(tileDownloader, &TileDownloader::resultReady,
//                     tileDownloader, &TileDownloader::deleteLater);

    // Starts an event loop, and emits workerThread->started()
//    tileDownloader->downloadedData();
    if (zoom == 0)
    {
        if (!QFile::exists(textureStorePath))
            Sleeper::msleep(50);
    }
}

/*!
    removal of old tiles and create new when changed zoom
*/
void Earth::updateTilesSlot(qreal scale, GeoCoords geoCoords)
{
    curZoom = log10(scale)/log10(2.0);
    curScale = scale;
    // save current coordinates
    curGeoCoords = geoCoords;

//    curZoom+=1;
    newZoomFlag = false;

    if (zoom != qFloor(curZoom))
    {
        newZoomFlag = true;
        zoom_old = zoom;
        zoom = qFloor(curZoom);

        int separation_old = qPow(2, zoom_old);
        for (int y = 0; y < separation_old; y++)
        {
            for (int x = 0; x < separation_old; x++)
            {
                TileCacheNumber currentCacheNumber = TileCacheNumber(zoom_old, x, y);

                if (tileNodeCache.contains(currentCacheNumber))
                {
                    QGLSceneNode* sceneNode = tileNodeCache.object(currentCacheNumber)->glSceneNodeObject();

                    if (!sceneNode->options().testFlag(QGLSceneNode::HideNode))
                    {
                        sceneNode->setOptions(QGLSceneNode::HideNode);
                    }
                }

                QString search_path = QString("tile-%1-%2-%3").arg(zoom_old).arg(x).arg(y);
                QGLSceneNode* sceneNode = this->findSceneNode(search_path);

                if (sceneNode)
                {
                    removeNode(sceneNode);
//                    delete tempNode;
                }
            }
        }
        buildEarthNode(a, 10, curZoom);

        qDebug() << "current zoom = " << qFloor(curZoom);
        emit displayed();
    }
    else
    {
        // only move camera
        buildEarthNode(a, 10, curZoom);
    }
}

Earth::~Earth()
{
    // clean textures
//    for (int i=0; i<m_LoadedTextures.count(); ++i) {
//        m_LoadedTextures.at(i)->cleanupResources();
//    }
        m_texture->cleanupResources();
}

void Earth::cleanupResources()
{

}
