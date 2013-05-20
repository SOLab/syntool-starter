#include "simplegranulesnode.h"

SimpleGranulesNode::SimpleGranulesNode(QObject *parent, QSharedPointer<QGLMaterialCollection> materials,
                                       ConfigData configData) :
    QGLSceneNode(parent)
{
    Q_UNUSED(materials);
    Q_UNUSED(configData);
    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D(this);
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D(this);
    rotateY->setAngle(-90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(axialTilt1);
    addTransform(rotateY);

    addGranuleNodes();
}

void SimpleGranulesNode::addGranuleNodes()
{
    QString filename = "/mnt/d/OISST-AVHRR-AMSR-V2.png";
//    QString filename = "/mnt/d/ascat_20120704_003001.png";
    qCritical() << 11111111111111111;
    QGLSceneNode* testNode = findSceneNode(filename);
    if (!testNode)// || newZoomFlag)
    {
        qCritical() << 2222222222222;
        delete testNode;
        qreal minSphereLat = -M_PI_2;
        qreal maxSphereLat = M_PI_2;
        QGLSceneNode* testNode = BuildGranuleMerNode(1, minSphereLat, maxSphereLat, -M_PI, M_PI);
        qCritical() << "11111" << addTextureToGranuleNode(testNode, filename);

    //    testNone->setOptions(QGLSceneNode::NoOptions);
        QGLBuilder builder;
        builder.sceneNode()->addNode(testNode);

        // add SceneNode to cache
    //    TileCacheNumber tileNumber = TileCacheNumber(cur_zoom, lonTileNum,separation-1-latTileNum);
    //    tileNodeCache.insert(tileNumber, tempNode);

        QGLSceneNode* temp2 = builder.finalizedSceneNode();
        temp2->setObjectName(filename);
        addNode(temp2);
    }
    else
    {
        emit updated();
    }
}

QGLSceneNode* SimpleGranulesNode::BuildGranuleMerNode(int separation, qreal minSphereLat, qreal maxSphereLat,
                                         qreal minSphereLon, qreal maxSphereLon)
{
    // calculate spherical min and max lon and lat
//    qreal minMerLat = Lat2MercatorLatitude(minSphereLat);
//    qreal maxMerLat = Lat2MercatorLatitude(maxSphereLat);
    qreal minMerLat = (minSphereLat);
    qreal maxMerLat = (maxSphereLat);

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
//        curMerLat = Lat2MercatorLatitude(curSphereLat);
//        curMerLatNext = Lat2MercatorLatitude(curSphereLatNext);
        curMerLat = (curSphereLat);
        curMerLatNext = (curSphereLatNext);

        prim.clear();
        for (qreal curSphereLon = minSphereLon; curSphereLon < maxSphereLonApprox;
                                                curSphereLon += oneSphereSliceDegrees)
        {
            // calculate decart coordinates (x,y,z) for Vertex and Normal
            curDecart = llh2xyz(curSphereLat, curSphereLon, 0.001);

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
            curDecartNext = llh2xyz(curSphereLatNext, curSphereLon, 0.001);
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

bool SimpleGranulesNode::addTextureToGranuleNode(QGLSceneNode* tempNode, QString filepath)
{
//    if (separation > 1){
    QGLTexture2D* tex = new QGLTexture2D();
//    tex->setSize(QSize(512, 256));

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
