#include "earth.h"
#include "qgltexture2d.h"
#include "qglmaterialcollection.h"
#include "qmath.h"
#include <QGLDome>

#include <QImage>
#include <QPainter>
#include <qgl.h>
#include "lightmaps.h"
#include <QThread>
#include <QTimer>

struct Sleeper : private QThread
{
    static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
};

inline double mercator(double x) {
    return 0.5*log((1.0+sin(x))/(1.0-sin(x)));
}

const double maxAbsLon = 180.0;
const double maxAbsLat = 85.051128;
#define maxAbsLatMer 85.051128
const double defMercAngle = maxAbsLat * M_PI / 180.0;
const double defMercScale = M_PI_2 / mercator(defMercAngle);

//const double a = 6378.137;
const double a = 0.5;

//double Mercator2SphereAnalytic2(double iTY, const double scale = defMercScale,
//                               const double maxAng = defMercAngle)
//{
//    return iTY;
//}

double Mercator2SphereAnalytic(double iTY, const double scale = defMercScale,
                               const double maxAng = defMercAngle)
{
    double angle = (iTY * 2 - 1) * M_PI_2;		// texture V to angle
    double angle2 = fabs(angle);
    double val = (angle2 > maxAng) ? M_PI_2 : (mercator(angle2) * scale);
    if (angle < 0.0) val = -val;
    return (1 + val / M_PI_2) * 0.5;	// angle to texture V
}

#define RADIANS_PER_DEGREE M_PI/180
double Lat2MercatorLatitude(double latitude)
{
    latitude = latitude*180.0/M_PI;
    double sign = (latitude < 0) ? -1 : 1;
    double sin = qSin(latitude * RADIANS_PER_DEGREE * sign);
    return sign * (qLn((1.0 + sin) / (1.0 - sin)) / 2.0);
}

QVector3D llh2xyz(qreal _lat, qreal _lon, qreal h = 0)
{
    qreal lat = _lat;//*M_PI/180.0;
    qreal lon = _lon;//*M_PI/180.0;
    qreal e2 = 0.00669436619;
    qreal N = a/qSqrt(1 - e2 * qPow(qSin(lat),2));

    QVector3D decartCoordPoint;

    decartCoordPoint.setX( (N + h)*qCos(lat)*qCos(lon) );
    decartCoordPoint.setY( (N + h)*qCos(lat)*qSin(lon) );
    decartCoordPoint.setZ( (N*(1-e2)+h)*qSin(lat) );

    return decartCoordPoint;
}

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);
    cacheDir = configData.cacheDir;

    sphere = buildEarthNode(0.5, 10, 0);
    sphere->setObjectName("Earth");

    QGLTexture2D * tex;
    tex = new QGLTexture2D();
    tex->setSize(QSize(512, 256));

    QUrl url;
    url.setPath(QLatin1String(":/zoom0.png"));
    url.setScheme(QLatin1String("file"));
    tex->setUrl(url);

    QGLMaterial *mat1 = new QGLMaterial;
    mat1->setTexture(tex, 0);

    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = sphere->palette()->addMaterial(mat1);

    sphere->setMaterialIndex(earthMat);
    sphere->setEffect(QGL::LitModulateTexture2D);

    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D();
    rotateY->setAngle(-90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(axialTilt1);
    addTransform(rotateY);
    addNode(sphere);

    zoom = 0;

//    tileDownloader = new TileDownloader();
}

QGLSceneNode *Earth::buildEarthNode(qreal radius, int divisions, int cur_zoom)
{
    qreal separation = qPow(2, cur_zoom);

    QGLBuilder builder;
    for (int lonTileNum = 0; lonTileNum < separation; lonTileNum++)
    {
        for (int latTileNum = 0; latTileNum < separation; latTileNum++)
        {
//            if ((lonTileNum == 1 && latTileNum == 0) || (lonTileNum == 2 && latTileNum == 1))
//            if ((lonTileNum == 1 || latTileNum == separation/2-1))
//            if (latTileNum == 1)
            addTileNode(&builder, radius, divisions, cur_zoom, lonTileNum, latTileNum);
        }
    }
    return builder.finalizedSceneNode();
}

qreal tiley2lat(int y, int separation)
{
    double n = M_PI - 2.0 * M_PI * y / qreal(separation);
    return 180.0 / M_PI * qAtan(0.5 * (qExp(n) - qExp(-n)));
}

void Earth::addTileNode(QGLBuilder* builder, qreal radius, int divisions, int cur_zoom,
                        qint32 lonTileNum, qint32 latTileNum)
{
    int separation = qPow(2, cur_zoom);
    qreal NTLon = 2*M_PI / separation;

    qreal minSphereLat = -tiley2lat(latTileNum, separation)/180.0*M_PI;
    qreal maxSphereLat = -tiley2lat(latTileNum+1, separation)/180.0*M_PI;

    qreal minSphereLon = ((lonTileNum) * NTLon - M_PI);
    qreal maxSphereLon = ((lonTileNum+1) * NTLon - M_PI);

    QGLSceneNode* tempNode = BuildSpherePart(separation, minSphereLat, maxSphereLat, minSphereLon, maxSphereLon);

    addTextureToTile(tempNode, separation, lonTileNum, latTileNum, cur_zoom);

    builder->sceneNode()->addNode(tempNode);
}

QGLSceneNode* Earth::BuildSpherePart(int separation, qreal minSphereLat, qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon)
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

void Earth::addTextureToTile(QGLSceneNode* tempNode, int separation, int lonTileNum, int latTileNum, int cur_zoom)
{
//    if (separation > 1){
        QGLTexture2D* tex;
        tex = new QGLTexture2D();
        tex->setSize(QSize(512, 256));

        QString _filepath = cacheDir+"/%1-%2-%3.png";
        QString filepath(_filepath.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

        QUrl url;
        url.setPath(filepath);
        QFile image;

        if (!QFile::exists(filepath))
        {
            QString path = tileDownload(lonTileNum, separation-1-latTileNum, cur_zoom);

//            QUrl url;
//            url.setPath(path);
            image.setFileName(path);
            while ( ! QFile::exists(path) || !image.size()) {
                Sleeper::msleep(50);
            }
        }
        else
        {
            image.setFileName(filepath);
        }

        url.setScheme(QLatin1String("file"));
        tex->setUrl(url);

        QGLMaterial *mat1 = new QGLMaterial;
        mat1->setTexture(tex, 0);

        m_LoadedTextures.push_back(mat1->texture(0));
        int earthMat = tempNode->palette()->addMaterial(mat1);

        tempNode->setMaterialIndex(earthMat);
        tempNode->setEffect(QGL::LitModulateTexture2D);
//    }
}

QString Earth::tileDownload(int tx, int ty, int zoom)
{
    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    QString arg(path.arg(zoom).arg(tx).arg(ty));

    QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());

    QString _filepath = cacheDir+"/%1-%2-%3.png";
    QString filepath(_filepath.arg(zoom).arg(tx).arg(ty));


    QThread *workerThread = new QThread(this);

    Downloader *downloader = new Downloader;
    downloader->setUrl(url);
    downloader->setFilename(filepath);


    connect(workerThread, &QThread::started, downloader, &Downloader::download);
    connect(workerThread, &QThread::finished, downloader, &Downloader::deleteLater);
    downloader->moveToThread(workerThread);

    // Starts an event loop, and emits workerThread->started()
    workerThread->start();




//    QNetworkAccessManager m;
//    QNetworkReply * reply = m.get( QNetworkRequest( url ) );
//    QEventLoop loop;
//    connect( reply, SIGNAL(finished()), &loop, SLOT(quit()) );
//    loop.exec();

//    if (QFile::exists(filepath))
//    {
//        return filepath;
//    }

//    qDebug() << "filepath" << filepath;
//    if ( reply->error() == QNetworkReply::NoError )
//    {
//      QFile image( filepath );
//      image.open(QIODevice::WriteOnly);
//      image.write( reply->readAll() );
//      image.close();
//    }
//    else
//    {
//    }
    return filepath;
}

Earth::~Earth()
{
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

void Earth::changeTexture(qreal cur_zoom)
{
//    cur_zoom = 0;
//    qDebug() << "changeTexture, cur_zoom = " << cur_zoom;
//    qDebug() << "changeTexture, zoom = " << zoom;
    if (zoom != qFloor(cur_zoom))
    {
        zoom = qFloor(cur_zoom);

//        qDebug() << "zoom changed!!! zoom = " << cur_zoom;
        QString search_path = QString("Earth");
        if (this->findSceneNode(search_path))
        {
            removeNode(sphere);
//            delete sphere;

            sphere = buildEarthNode(0.5, 10, cur_zoom);
            sphere->setObjectName("Earth");

//            QGraphicsRotation3D *circular_rotate = new QGraphicsRotation3D();
//            circular_rotate->setAngle(180.0f);
//            circular_rotate->setAxis(QVector3D(0,0,1));

//            sphere->addTransform(circular_rotate);

            addNode(sphere);
            emit updated();
        }

//    if (zoom > 1)
//    {
//    QGLMaterial *mat1 = new QGLMaterial;
//    QUrl url;
//    url.setPath(QLatin1String(":/earth.jpg"));
//    url.setScheme(QLatin1String("file"));
//    mat1->setTextureUrl(url,0);

//    m_LoadedTextures.push_back(mat1->texture(0));
//    int earthMat = sphere->palette()->addMaterial(mat1);

//    sphere->setMaterialIndex(earthMat);
//    sphere->setEffect(QGL::LitModulateTexture2D);
//    }
//    else
//    {
//        QGLMaterial *mat1 = new QGLMaterial;
//        QUrl url;
//        url.setPath(QLatin1String(":/earth_6_3.jpg"));
//        url.setScheme(QLatin1String("file"));
//        mat1->setTextureUrl(url,0);

//        m_LoadedTextures.push_back(mat1->texture(0));
//        int earthMat = sphere->palette()->addMaterial(mat1);

//        sphere->setMaterialIndex(earthMat);
//        sphere->setEffect(QGL::LitModulateTexture2D);
//    }
    }
}
