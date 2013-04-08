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
const double maxAbsLat = 85.0511;
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

double Mercator2SphereAnalyticDegrees(double iTY, const double scale = defMercScale,
                               const double maxAng = defMercAngle)
{
    iTY = iTY/90.0;
    return Mercator2SphereAnalytic(iTY, scale, maxAng);
}

//struct decartCoord
//{
//    qreal x;
//    qreal y;
//    qreal z;
//};

QVector3D llh2xyz(qreal _lat, qreal _lon, qreal h = 0)
{
    qreal lat = _lat*M_PI/180.0;
    qreal lon = _lon*M_PI/180.0;
    qreal e2 = 0.00669436619;
    qreal N = a/qSqrt(1 - e2 * qPow(qSin(lat),2));

    QVector3D decartCoordPoint;

    decartCoordPoint.setX( (N + h)*qCos(lat)*qCos(lon) );
    decartCoordPoint.setY( (N + h)*qCos(lat)*qSin(lon) );
    decartCoordPoint.setZ( (N*(1-e2)+h)*qSin(lat) );

    return decartCoordPoint;
}

//                                        1
//        C =  ---------------------------------------------------
//             sqrt( cos^2(latitude) + (1-f)^2 * sin^2(latitude) )

//        (1/f) = 298.257224

//        S = (1-f)^2 * C

//decartCoord llh2xyz_var2(qreal _lat, qreal _lon, qreal h)
//{
//    qreal lat = _lat*M_PI/180.0;
//    qreal lon = _lon*M_PI/180.0;
//    qreal f = 1.0/298.257224;
//    qreal C = 1/qSqrt(qPow(qCos(lat),2) + qPow(1-f,2) * qPow(qSin(lat), 2));
//    qreal S = qPow(1-f,2) * C;

//    decartCoord decartCoordPoint;
//    decartCoordPoint.x = (a*C+h)*qCos(lat)*qCos(lon);
//    decartCoordPoint.y = (a*C+h)*qCos(lat)*qSin(lon);
//    decartCoordPoint.z = (a*S+h)*qSin(lat);

//    return decartCoordPoint;
//}

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);

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

    cacheDir = configData.cacheDir;

//    tileDownloader = new TileDownloader();
}

QGLSceneNode *Earth::buildEarthNode(qreal radius, int divisions, int cur_zoom)
{
//    Q_UNUSED(cur_zoom);
//    divisions = 1;

    qreal separation = qPow(2, cur_zoom);
    qDebug() << "separation = " << separation;
//    if (separation > 2)
//        separation = 2;

    QGLBuilder builder;
    // Determine the number of slices and stacks to generate.

//    int lonTileNum;
//    int latTileNum;

    for (int lonTileNum = 0; lonTileNum < separation; lonTileNum++)
    {
        for (int latTileNum = 0; latTileNum < separation; latTileNum++)
        {
//            qDebug() << latTileNum << lonTileNum;
//            if (lonTileNum == 1 && latTileNum == 1)
//            if (latTileNum == 0)
            addTileNode(&builder, radius, divisions, cur_zoom, lonTileNum, latTileNum);
        }
    }
    return builder.finalizedSceneNode();
}

struct UV {
 qreal u, v;
};

UV GetUV( qreal angleU, qreal angleV )
{
  UV uv;
  uv.u = angleU / (M_PI * 2);
  uv.v = angleV / (M_PI) + 0.5;
  return uv;
}

QGLSceneNode* Earth::BuildSpherePart(qreal startU, qreal stepU, int numU,   // широта (0..2*PI)
                     qreal startV, qreal stepV, int numV   // долгота (-PI/2 .. +PI/2)
                     // ... др параметры
                    )
{
// находим диапазон
    UV minUV = GetUV(startU, startV);
    UV maxUV = GetUV(startU + stepU * (numU - 1), startV + stepV * (numV - 1));
    minUV.v = Mercator2SphereAnalytic(minUV.v);
    maxUV.v = Mercator2SphereAnalytic(maxUV.v);

    QGLBuilder tempBuilder;
    QGeometryData prim;
    qDebug() << numU;
// вычисляем UV координаты для сферы
    for (int i =  0; i < numU; ++i)
    {

        qDebug() << "i = " << i;
        for (int j =  0; j < numV; ++j)
        {
//            qDebug() << "j = " << j;

            qreal angleU = startU + stepU * i;
            qreal angleV = startV + stepV * j;

            // добавляем вертекс
            qreal y = sin(angleV);
            qreal len = sqrt(1 - y * y);
            qreal x = cos(angleU) * len;
            qreal z = sin(angleU) * len;
            prim.appendVertex(QVector3D(x, y, z) * a);
            prim.appendNormal(QVector3D(x, y, z) * a);

            // получаем сферические UV
            UV uv = GetUV(angleU, angleV);

            // получаем V меркатора
            uv.v = Mercator2SphereAnalytic(maxUV.v);

            // нормируем UV
            uv.u = (uv.u - minUV.u) / (maxUV.u - minUV.u);
            uv.v = (uv.v - minUV.v) / (maxUV.v - minUV.v);

            prim.appendTexCoord(QVector2D(uv.u, uv.v));
        }

//        if (!(i%8))
//        {
//            prim.clear();
//        }
    }
    tempBuilder.addQuadStrip(prim);
    return tempBuilder.finalizedSceneNode();
}

void Earth::addTileNode(QGLBuilder* builder, qreal radius, int divisions, int cur_zoom,
                        qint32 lonTileNum, qint32 latTileNum)
{
    int separation = qPow(2, cur_zoom);
////////////////////////////////////////////
//    // degrees for one tile
//    qreal NTLat = 2*maxAbsLat / separation;
//    qreal NTLon = 2*maxAbsLon / separation;

//    // min and max tile degrees
//    qreal minMerLat = maxAbsLat - (latTileNum+1) * NTLat;
//    qreal maxMerLat = maxAbsLat - (latTileNum) * NTLat;
//    qreal minMerLon = maxAbsLon - (lonTileNum+1) * NTLon;
//    qreal maxMerLon = maxAbsLon - (lonTileNum) * NTLon;

//    // calculate spherical min and max lon and lat
//    int signMinLat = (minMerLat<0)?-1:1;
//    int signMaxLat = (maxMerLat<0)?-1:1;
//    qreal minSphereLat = Mercator2SphereAnalyticDegrees(qAbs(minMerLat))*signMinLat*90;
//    qreal maxSphereLat = Mercator2SphereAnalyticDegrees(qAbs(maxMerLat))*signMaxLat*90;

////    qDebug() << "minMerLat" << minMerLat << "minSphereLat" << minSphereLat;
//    qDebug() << "maxMerLat" << maxMerLat << "maxSphereLat" << maxSphereLat;

//    qreal minSphereLon = minMerLon;
//    qreal maxSphereLon = maxMerLon;


    // all stacks and slices
    int stacks = 32;
    int slices = 32;
    if (cur_zoom > 4)
    {
        stacks *= qPow(2, cur_zoom - 4);
        slices *= qPow(2, cur_zoom - 4);
    }
//    int stacks = stacksForOne * separation;
//    int slices = slicesForOne * separation;

//    // stacks and slices for one tile
    int stacksForOne = stacks/separation;
    int slicesForOne = slices/separation;


//    qreal oneMerStackDegrees = (maxMerLat - minMerLat)/(qreal)stacksForOne;
//    qreal oneMerSliceDegrees = (maxMerLon - minMerLon)/(qreal)slicesForOne;

//    qreal oneSphereStackDegrees = (maxSphereLat - minSphereLat)/(qreal)stacksForOne;
////    qreal oneSphereSliceDegrees = (maxSphereLon - minSphereLon)/(qreal)slicesForOne;
////    qDebug() << "oneStackDegrees = " << oneStackDegrees;
////    qDebug() << "oneSliceDegrees = " << oneSliceDegrees;

////    qreal curSphereLat, curSphereLatNext, curSphereLon;

//    QVector3D curDecart;
//    QVector3D curDecartNext;

//    int count_ = 0;

//    qreal curMerLat = minMerLat;

    // radian for one tile
    qreal NTLat = 2*M_PI / separation;
    qreal NTLon = M_PI / separation;

    // min and max tile degrees
    qreal minRadianLat = 2*M_PI - (latTileNum+1) * NTLat;
    qreal maxRadianLat = 2*M_PI - latTileNum * NTLat;
    qreal oneRadStack = (maxRadianLat - minRadianLat)/(qreal)stacksForOne;


    qreal minRadianLon = (lonTileNum+1) * NTLon - M_PI_2;
    qreal maxRadianLon = lonTileNum * NTLon - M_PI_2;
    qreal oneRadSice = (maxRadianLon - minRadianLon)/(qreal)slicesForOne;
//    QGLBuilder Earth::BuildSpherePart(qreal startU, qreal stepU, int numU,   // широта (0..2*PI)
//                         qreal startV, qreal stepV, int numV   // долгота (-PI/2 .. +PI/2)
//    QGLBuilder tempBuilder = BuildSpherePart(minRadianLat, oneRadStack, stacksForOne,
//                                             minRadianLon, oneRadSice, slicesForOne);

//    for (qreal curSphereLat = minSphereLat; curSphereLat <= (maxSphereLat-0.2);
//                                            curSphereLat+=oneSphereStackDegrees) {
//        count_ ++;
//        // calculate next point latitude
//        qreal curSphereLatNext = curSphereLat + oneSphereStackDegrees;
//        qreal curMerLatNext = curMerLat + oneMerStackDegrees;
//        QGeometryData prim;

////        qDebug() << "---------" << curSphereLat;

//        double yTexCoord;
//        double yTexCoordNext;
//        double xTexCoord;
//        double xTexCoordNext;

////        int signLat = (curMerLat >= 0)?1:-1;
////        int signLatNext = (curMerLatNext >= 0)?1:-1;

////        curSphereLat = Mercator2SphereAnalytic(qAbs(curMerLat)/90.0)*signLat*90;
////        curSphereLatNext = Mercator2SphereAnalytic(qAbs(curMerLatNext)/90.0)*signLatNext*90;

//        prim.clear();
//        for (qreal curMerLon = minMerLon; curMerLon <= maxMerLon; curMerLon += oneMerSliceDegrees)
//        {

//            qreal curSphereLon = curMerLon;

//            curDecart = llh2xyz((curSphereLat), curMerLon);
//            curDecartNext = llh2xyz((curSphereLatNext), curMerLon);

////            qDebug() << "====================================";
////            qDebug() << "curMerLat = " << curMerLat;
////            qDebug() << "curMerLon = " << curMerLon;
////            qDebug() << "minSphereLat = " << minSphereLat;
////            qDebug() << "maxSphereLat = " << maxSphereLat;
////            qDebug() << "minSphereLon = " << minSphereLon;
////            qDebug() << "maxSphereLon = " << maxSphereLon;
////            qDebug() << "curSphereLat = " << curSphereLat;
////            qDebug() << "curSphereLon = " << curSphereLon;
////            qDebug() << "curDecart = " << curDecart;
////            qDebug() << "minLat = " << minLat;
////            qDebug() << "maxLat = " << maxLat;
//            prim.appendVertex(curDecart);
//            prim.appendNormal(curDecart);
////            xTexCoord = ((curSphereLon) - (minSphereLon))/((maxSphereLon) - (minSphereLon));
////            yTexCoord = ((curSphereLat) - (minSphereLat))/((maxSphereLat) - (minSphereLat));
//            xTexCoord = ((curSphereLon) - (minSphereLon))/((maxSphereLon) - (minSphereLon));
//            yTexCoord = ((curMerLat) - (minMerLat))/((maxMerLat) - (minMerLat));
////            qDebug() << "xTexCoord = " << xTexCoord;
////            qDebug() << "yTexCoord = " << yTexCoord;
//            prim.appendTexCoord(QVector2D(xTexCoord, (yTexCoord)));

//            prim.appendVertex(curDecartNext);
//            prim.appendNormal(curDecartNext);
////            xTexCoordNext = ((curSphereLon) - (minSphereLon))/((maxSphereLon) - (minSphereLon));
////            yTexCoordNext = ((curSphereLatNext) - (minSphereLat))/((maxSphereLat) - (minSphereLat));
//            xTexCoordNext = ((curSphereLon) - (minSphereLon))/((maxSphereLon) - (minSphereLon));
//            yTexCoordNext = ((curMerLatNext) - (minMerLat))/((maxMerLat) - (minMerLat));
//            prim.appendTexCoord(QVector2D(xTexCoordNext, (yTexCoordNext)));
//        }
//        curMerLat += oneMerStackDegrees;
//        tempBuilder.addQuadStrip(prim);
//    }

    qDebug() << "www";
    QGLSceneNode* tempNode = BuildSpherePart(minRadianLat, oneRadStack, stacksForOne,
                                             minRadianLon, oneRadSice, slicesForOne);
    qDebug() << "WWW";
    if (separation > 1){
        QGLTexture2D* tex;
        tex = new QGLTexture2D();
        tex->setSize(QSize(512, 256));
//        qDebug() << "+++++++++++"<< tex->textureUpdated();

//            tileDownloader->setParameters(stack_num, separation-slice_num-1, 1);
//            QTimer::singleShot(0, &tileDownloader, SLOT(execute()));

        QString _filepath = cacheDir+"/%1-%2-%3.png";
        QString filepath(_filepath.arg(cur_zoom).arg(separation-1-lonTileNum).arg(latTileNum));

        QUrl url;
        url.setPath(filepath);
        QFile image;

        if (!QFile::exists(filepath))
        {
            QString path = tileDownload(separation-1-lonTileNum, latTileNum, cur_zoom);

//            QUrl url;
//            url.setPath(path);
            image.setFileName(path);
            while ( ! QFile::exists(path) || !image.size()) {
//                qDebug() << 1111111;
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
    }

    builder->sceneNode()->addNode(tempNode);
}


////////////////////////////////////////////
//    static int const slicesAndStacks[] = {
//        8, 4,
//        8, 8,
//        16, 8,
//        16, 16,
//        32, 16,
//        32, 32,
//        64, 32,
//        64, 64,
//        128, 64,
//        128, 128,
//        256, 256,
//        512, 512,
//        1024, 1024
//    };

//    if (divisions < 1)
//        divisions = 1;
//    else if (divisions > 13)
//        divisions = 13;
//    int stacks = slicesAndStacks[divisions * 2 - 1];
//    int slices = slicesAndStacks[divisions * 2 - 2];

//    // Calculate min and max lat and lon

//    int minSlice = (slices / separation) * lonTileNum;
//    int maxSlice = (slices / separation) * (lonTileNum + 1);
//    int minStack = (stacks / separation) * latTileNum;
//    int maxStack = (stacks / separation) * (latTileNum + 1) - 1;

    // Precompute sin/cos values for the slices and stacks.
//    const int maxSlices = 128 + 1;
//    const int maxStacks = 128 + 1;
//    qreal sliceSin[maxSlices];
//    qreal sliceCos[maxSlices];
//    qreal stackSin[maxStacks];
//    qreal stackCos[maxStacks];
//    for (int slice = 0; slice < slices; ++slice) {
//        qreal angle = 2 * M_PI * slice / slices;
//        sliceSin[slice] = qFastSin(angle);
//        sliceCos[slice] = qFastCos(angle);
//    }
//    sliceSin[slices] = sliceSin[0]; // Join first and last slice.
//    sliceCos[slices] = sliceCos[0];
//    for (int stack = 0; stack <= stacks; ++stack) {
//        qreal angle = M_PI * stack / stacks;
//        stackSin[stack] = qFastSin(angle);
//        stackCos[stack] = qFastCos(angle);
//    }
//    stackSin[0] = 0.0f;             // Come to a point at the poles.
//    stackSin[stacks] = 0.0f;

//    int stack_num = latTileNum;
//    int slice_num = lonTileNum;

//    int distal = 1;
//    if (separation > 2){
//        if (stack_num + 1 > separation / 2)
//            distal = (stack_num + 1) - (separation / 2);
//        else
//            distal = (separation / 2) - stack_num;
//    }
//    qDebug() << "distal" << distal;

//    QGLBuilder tempBuilder;
//    for (int stack = minStack; stack <= maxStack; ++stack) {
//        QGeometryData prim;
//        qreal z = radius * stackCos[stack];
//        qreal nextz = radius * stackCos[stack + 1];
//        qreal s = stackSin[stack];
//        qreal nexts = stackSin[stack + 1];
//        qreal c = stackCos[stack];
//        qreal nextc = stackCos[stack + 1];
//        qreal r = radius * s;
//        qreal nextr = radius * nexts;

//        double yTexCoord;
//        double yTexCoordNext;
//        double xTexCoord;
//        double xTexCoordNext;

//        int sliceToCoord;

//        prim.clear();
//        for (int slice = minSlice; slice <= maxSlice; ++slice) {

//            sliceToCoord = (maxSlice - slice) * separation;

//            prim.appendVertex
//                (QVector3D((nextr * sliceSin[slice]),
//                           (nextr * sliceCos[slice]), nextz));
//            prim.appendNormal
//                (QVector3D(sliceSin[slice] * nexts,
//                           sliceCos[slice] * nexts, nextc));

//            yTexCoordNext = 1.0f - qreal(stack + 1) / stacks;
//            yTexCoordNext = /*Mercator2SphereAnalytic*/(yTexCoordNext)*(separation);
//            xTexCoordNext = (qreal(sliceToCoord) / slices);//*(separation);

//            prim.appendTexCoord
//                (QVector2D(xTexCoordNext,
//                           yTexCoordNext));

//            prim.appendVertex
//                (QVector3D((r * sliceSin[slice]),
//                           (r * sliceCos[slice]), z));
//            prim.appendNormal
//                (QVector3D(sliceSin[slice] * s,
//                           sliceCos[slice] * s, c));

//            yTexCoord = 1.0f - qreal(stack) / stacks;
//            yTexCoord = /*Mercator2SphereAnalytic*/(yTexCoord)*(separation);
//            xTexCoord = (qreal(sliceToCoord) / slices);//*(separation);


//            prim.appendTexCoord
//                (QVector2D(xTexCoord,
//                           yTexCoord));
//        }

//        tempBuilder.addQuadStrip(prim);
//    }

//    QGLSceneNode* tempNode = tempBuilder.finalizedSceneNode();
//    if (separation > 1){
//        QGLTexture2D* tex;
//        tex = new QGLTexture2D();
//        tex->setSize(QSize(512, 256));
////        qDebug() << "+++++++++++"<< tex->textureUpdated();

////            tileDownloader->setParameters(stack_num, separation-slice_num-1, 1);
////            QTimer::singleShot(0, &tileDownloader, SLOT(execute()));

//        QString _filepath = cacheDir+"/%1-%2-%3.png";
//        QString filepath(_filepath.arg(cur_zoom).arg(separation-1-lonTileNum).arg(latTileNum));

//        QUrl url;
//        url.setPath(filepath);
//        QFile image;

//        if (!QFile::exists(filepath))
//        {
//            QString path = tileDownload(separation-1-slice_num, stack_num, cur_zoom);

////            QUrl url;
////            url.setPath(path);
//            image.setFileName(path);
//            while ( ! QFile::exists(path) || !image.size()) {
////                qDebug() << 1111111;
//                Sleeper::msleep(50);
//            }
//        }
//        else
//        {
//            image.setFileName(filepath);
//        }

//        url.setScheme(QLatin1String("file"));
//        tex->setUrl(url);

//        QGLMaterial *mat1 = new QGLMaterial;
//        mat1->setTexture(tex, 0);

//        m_LoadedTextures.push_back(mat1->texture(0));
//        int earthMat = tempNode->palette()->addMaterial(mat1);

//        tempNode->setMaterialIndex(earthMat);
//        tempNode->setEffect(QGL::LitModulateTexture2D);
//    }

//    builder->sceneNode()->addNode(tempNode);
//}

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

            if (zoom < 1)
            {
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
            }
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
