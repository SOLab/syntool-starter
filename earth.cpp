#include "earth.h"

const double a = 6378137.0;

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);
    cacheDir = configData.cacheDir;

    buildEarthNode(a, 10, 0);
//    sphere->setObjectName("Earth");

//    QGLTexture2D * tex;
//    tex = new QGLTexture2D();
//    tex->setSize(QSize(512, 256));

//    QUrl url;
//    url.setPath(QLatin1String(":/zoom0.png"));
//    url.setScheme(QLatin1String("file"));
//    tex->setUrl(url);

//    QGLMaterial *mat1 = new QGLMaterial;
//    mat1->setTexture(tex, 0);

//    m_LoadedTextures.push_back(mat1->texture(0));
//    int earthMat = sphere->palette()->addMaterial(mat1);

//    sphere->setMaterialIndex(earthMat);
//    sphere->setEffect(QGL::LitModulateTexture2D);

    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D();
    rotateY->setAngle(-90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(axialTilt1);
    addTransform(rotateY);
//    addNode(sphere);

    zoom = -1;
    zoom_old = 0;

    connect(this, SIGNAL(textureDownloadedSignal(qint32,qint32,qint32)), this, SLOT(textureDownloaded(qint32,qint32,qint32)));

//    tileDownloader = new TileDownloader();
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

        qDebug() << "km in each tile = " << 40075.017/qPow(2, cur_zoom);
        qDebug() << "km = " << 2*40000/curScale;
        int numberTiles = qCeil(getNumberTiles(cur_zoom, 2*40000/curScale));
        qDebug() << "number of tiles = " << numberTiles;

        TileNumber tileNumber = deg2TileNum(curGeoCoords, cur_zoom);
        qDebug() << "curGeoCoords = " << curGeoCoords.lat << curGeoCoords.lon;
        qDebug() << "tileNumber = " << tileNumber.x << ", " << tileNumber.y;

        TileRange* aaa = getTileRange(tileNumber, numberTiles, cur_zoom);

        for (int tileRangeNumber = 0; tileRangeNumber <= 1; tileRangeNumber++)
        {
            for (int lonTileNum = aaa[tileRangeNumber].startX; lonTileNum <= aaa[tileRangeNumber].endX; lonTileNum++)
            {
                for (int latTileNum = aaa[tileRangeNumber].startY; latTileNum <= aaa[tileRangeNumber].endY; latTileNum++)
                {
                    qDebug() << "lon" << lonTileNum<< "lat" << latTileNum;
                    addTileNode(cur_zoom, lonTileNum, separation-1-latTileNum);
                }
            }
        }
    }
    else
    {
        for (int lonTileNum = 0; lonTileNum < separation; lonTileNum++)
        {
            for (int latTileNum = 0; latTileNum < separation; latTileNum++)
            {
                addTileNode(cur_zoom, lonTileNum, latTileNum);
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

    if (!checkTextureFile(separation, lonTileNum, latTileNum, cur_zoom))
    {
        tileDownload(cur_zoom, separation, lonTileNum, latTileNum);
        return;
    }

//    emit textureDownloadedSignal(cur_zoom, lonTileNum, latTileNum);
    textureDownloaded(cur_zoom, lonTileNum, latTileNum);
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
    int stacks = 32;
    int slices = 32;
//    if (cur_zoom > 4)
//    {
//        stacks *= qPow(2, cur_zoom);
//        slices *= qPow(2, cur_zoom);
//    }

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

    for (qreal curSphereLat = minSphereLat; curSphereLat < maxSphereLat; curSphereLat+=oneSphereStackDegrees) {
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
        for (qreal curSphereLon = minSphereLon; curSphereLon < maxSphereLonApprox; curSphereLon += oneSphereSliceDegrees)
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
    QString texFilePath = QString(cacheDir+"/%1-%2-%3.png").arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum);
    return QFile::exists(texFilePath);
}

/*!
    add texture to current QGLSceneNode object.
*/
bool Earth::addTextureToTile(QGLSceneNode* tempNode, int separation, int lonTileNum, int latTileNum, int cur_zoom)
{
//    if (separation > 1){
    QGLTexture2D* tex;
    tex = new QGLTexture2D();
//    tex->setSize(QSize(512, 256));

    QString _filepath = cacheDir+"/%1-%2-%3.png";
    QString filepath(_filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

    if (!QFile::exists(filepath))
    {
        return false;
    }

    QUrl url;
    url.setPath(filepath);
    QFile image;
    image.setFileName(filepath);
    url.setScheme(QLatin1String("file"));
    tex->setUrl(url);

    QGLMaterial *mat1 = new QGLMaterial;
    mat1->setTexture(tex, 0);

    m_LoadedTextures.push_back(mat1->texture(0));
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
    QString _filepath = cacheDir+"/%1-%2-%3.png";
    QString textureStorePath = (_filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

    TileDownloader *tileDownloader = new TileDownloader(separation, lonTileNum, latTileNum, cur_zoom, textureStorePath);

    QObject::connect(tileDownloader, &TileDownloader::resultReady,
                     this, &Earth::textureDownloaded);
//    QObject::connect(tileDownloader, &TileDownloader::finished,
//                     tileDownloader, &TileDownloader::deleteLater);

    // Starts an event loop, and emits workerThread->started()
    tileDownloader->downloadedData();
    if (zoom == 0)
    {
        if (!QFile::exists(textureStorePath))
            Sleeper::msleep(50);
    }
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

//    QString _filepath = cacheDir+"/%1-%2-%3.png";
//    QString texStorePath = _filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum);

    qreal NTLon = 2*M_PI / separation;

    qreal minSphereLat = -tiley2lat(latTileNum, separation)/180.0*M_PI;
    qreal maxSphereLat = -tiley2lat(latTileNum+1, separation)/180.0*M_PI;

    qreal minSphereLon = ((lonTileNum) * NTLon - M_PI);
    qreal maxSphereLon = ((lonTileNum+1) * NTLon - M_PI);

    QGLSceneNode* tempNode = BuildSpherePart(separation, minSphereLat, maxSphereLat, minSphereLon, maxSphereLon);

    QString nodeObjectName = QString("tile-%1-%2-%3").arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum);
//    qDebug() << "nodeObjectName = " << nodeObjectName;
    tempNode->setObjectName(nodeObjectName);

    if (addTextureToTile(tempNode, separation, lonTileNum, latTileNum, cur_zoom))
    {
        QGLBuilder builder;
        builder.sceneNode()->addNode(tempNode);

        // add SceneNode to cache
        tileNodeCache.insert(TileCacheNumber(cur_zoom, lonTileNum,separation-1-latTileNum), tempNode);

        addNode(builder.finalizedSceneNode());
        qDebug() << "TEXTURE ADDED!!!";
    }
    else
    {
        qDebug() << "TEXTURE NOT ADDED!!!";
    }
    emit displayed();
}

/*!
    removal of old tiles and create new when changed zoom
*/
void Earth::updateTilesSlot(qreal scale, GeoCoords geoCoords)
{
    qreal cur_zoom = log10(scale)/log10(2);
    qDebug() << "cur_zoom = " << qFloor(cur_zoom);
//    qDebug() << "scale" << scale;
    curScale = scale;
    // save current coordinates
    curGeoCoords = geoCoords;

//    cur_zoom = 0;
    if (zoom != qFloor(cur_zoom))
    {
        zoom_old = zoom;
        zoom = qFloor(cur_zoom);

        int separation_old = qPow(2, zoom_old);
        for (int y = 0; y < separation_old; y++)
        {
            for (int x = 0; x < separation_old; x++)
            {
                QString search_path = QString("tile-%1-%2-%3").arg(zoom_old).arg(x).arg(y);
                QGLSceneNode* tempNode = this->findSceneNode(search_path);
                if (tempNode){
                    removeNode(this->findSceneNode(search_path));
                    qDebug() << "removeNode " << search_path;
                }
                delete tempNode;
                qDebug() << "delete Node " << search_path;
            }
        }
        buildEarthNode(a, 10, cur_zoom);
    }
    else
    {
        // only move camera
    }
}

Earth::~Earth()
{
    // clean textures
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
        m_texture->cleanupResources();
}

//void Earth::drawImage(QGLPainter *glpainter)
//{
//    Q_UNUSED(glpainter);
////    setPalette(materials);

//    QGLBuilder builder;
//    builder.newSection();
//    QGLSceneNode *root = builder.currentNode();
//    QUrl url;

//    //set up our materials palette - this describes all
//    //of the materials we will use for this scene.

//    //solar surface
//    QGLMaterial *mat1 = new QGLMaterial;
////    url.setPath(QLatin1String(":/earth.jpg"));
//    url.setPath(QLatin1String(":/6.png"));
//    url.setScheme(QLatin1String("file"));
//    mat1->setTextureUrl(url,0);

//    m_LoadedTextures.push_back(mat1->texture(0));
//    int earthMat = root->palette()->addMaterial(mat1);

//    // create sphere
//    builder.pushNode()->setObjectName(QLatin1String("Earth"));

///////////////////////////
//    const int imgSize = 1024;
//    const int txtSize = 36;

//    QImage uv(imgSize, imgSize, QImage::Format_ARGB32);
//    QPainter painter;
//    painter.begin(&uv);
//    painter.setRenderHint(QPainter::Antialiasing);
//    QPen pen = painter.pen();
//    pen.setWidth(2.0);

//    painter.setPen(pen);
//    QFont font = painter.font();
//    font.setPointSize(txtSize);
//    painter.setFont(font);
////////////////////////////

//    QGeometryData op;

//    QVector3D v0(0.0f, 1.0f, 1.0f);
//    QVector3D v1(0.0f, 0.0f, 1.0f);
//    QVector3D v2(1.0f, 0.0f, 1.0f);
//    QVector3D v3(1.0f, 1.0f, 1.0f);

//    QVector2D t0(0.0, 1.0);
//    QVector2D t1(0.0, 0.0);
//    QVector2D t2(1.0, 0.0);
//    QVector2D t3(1.0, 1.0);
////    float dx = 0.2f;
////    float dy = 0.2f;
////    float dz1 = -0.4f;
////    float dz2 = -0.9f;

////    QVector3D v0(0.0f+dx, 0.0f+dy, 1.0f+dz1+0.1);
////    QVector3D v1(0.3f+dx, 0.0f+dy, 1.0f+dz1);
////    QVector3D v2(0.3f+dx, 0.3f+dy, 1.0f+dz2);
////    QVector3D v3(0.0f+dx, 0.3f+dy, 1.0f+dz2);

//    QVector2D tv0 = t0 * static_cast<float>(imgSize);
//    QVector2D tv1 = t1 * static_cast<float>(imgSize);
//    QVector2D tv2 = t2 * static_cast<float>(imgSize);
//    QVector2D tv3 = t3 * static_cast<float>(imgSize);


//    painter.setPen(QColor("darkblue"));
//    painter.drawLine(tv0.toPointF(), tv1.toPointF());
//    painter.drawLine(tv1.toPointF(), tv2.toPointF());
//    painter.drawLine(tv2.toPointF(), tv3.toPointF());
//    painter.drawLine(tv3.toPointF(), tv0.toPointF());

//    op.appendVertex(v0, v1, v2, v3);
//    op.appendTexCoord(t0, t1, t2, t3);

//    painter.end();
//    builder.addQuads(op);
/////////////////////////////////////////////
//    builder.currentNode()->setMaterialIndex(earthMat);
//    builder.currentNode()->setEffect(QGL::LitModulateTexture2D);

////    //create and add rotations for axial tilt and rotation
////    earthRotation = new QGraphicsRotation3D();
////    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
////    axialTilt1->setAngle(270.0f);
////    axialTilt1->setAxis(QVector3D(1,0,0));
////    earthRotation->setAngle(0.0f);
////    builder.currentNode()->addTransform(earthRotation);
////    builder.currentNode()->addTransform(axialTilt1);

//     //completed building, so finalise
//    QGLSceneNode *n = builder.finalizedSceneNode();
//    addNode(n);
//}
