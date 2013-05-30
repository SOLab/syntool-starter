#include "simplegranulesnode.h"

SimpleGranulesNode::SimpleGranulesNode(QObject *parent, QSharedPointer<QGLMaterialCollection> materials,
                                       ConfigData *configData, qint32 granuleId, qint32 productId) :
    QGLSceneNode(parent)
{
    Q_UNUSED(materials);
    Q_UNUSED(configData);

    setGranuleId(granuleId);
    setProductId(productId);

    serverName = configData->serverName;
    cacheDir = configData->cacheDir;

//    urlProducts = QUrl(serverName + "/api/products");
    urlGranules = QUrl(serverName + "/api/granules");

    networkManager = new QNetworkAccessManager(this);

    _height = 0;
    _transparency = 0;

    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D(this);
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));

    QGraphicsRotation3D *rotateY = new QGraphicsRotation3D(this);
    rotateY->setAngle(-90.0f);
    rotateY->setAxis(QVector3D(0,1,0));

    addTransform(axialTilt1);
    addTransform(rotateY);

    connect(this, &SimpleGranulesNode::granuleImageUrlReceived, this, &SimpleGranulesNode::downloadGranuleImage);

    addGranuleNodeStart();
}

SimpleGranulesNode::~SimpleGranulesNode()
{
    QGLSceneNode* nodeObject = findSceneNode(imagePath);
    if (nodeObject)
    {
        removeNode(nodeObject);
        nodeObject->deleteLater();
    }
}

void SimpleGranulesNode::getGranuleImageUrl()
{
    QNetworkRequest request;

    //create filter
    QString filter = "/";

    filter += QString::number(granuleId()) + "/quicklooks";

    request.setUrl(QUrl(urlGranules.scheme() + "://" + urlGranules.host() + urlGranules.path() + filter));
    request.setRawHeader("Content-Type", "text/xml");

//    qCritical() << request.url();

    QNetworkReply* reply = networkManager->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &SimpleGranulesNode::slotReadyReadImageUrl);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorImageUrl(QNetworkReply::NetworkError)));
}

void SimpleGranulesNode::slotReadyReadImageUrl()
{
    QNetworkReply *reply=qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << statusCode;
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "status code: " << statusCode;

        switch (statusCode)
        {
            case 200:
            {
                QByteArray bytes = reply->readAll();
//                QString string(bytes);
//                qDebug() << string;

                QDomDocument mDocument;
                QString errorMsg;
                int errorLine;
                int errorColumn;
                if (!mDocument.setContent(bytes, false, &errorMsg,
                                          &errorLine, &errorColumn))
                {
                    qWarning() << "Error parse XML";
//                    qDebug() << errorMsg;
//                    qDebug() << errorLine;
//                    qDebug() << errorColumn;
                    if (errorLine > 1)
                    {
                        currentRequest = bytes;
                        return;
                    }
                    else
                    {
                        currentRequest = currentRequest + bytes;
                        bytes = currentRequest;
                        if (!mDocument.setContent(bytes, false, &errorMsg,
                                                  &errorLine, &errorColumn))
                        {
                                qCritical() << "Error parse XML";
                                qCritical() << errorMsg;
                                qCritical() << errorLine;
                                qCritical() << errorColumn;
                                return;
                        }
                    }
                }

                currentRequest.clear();

                QDomElement  coordsElement = mDocument.documentElement();//.firstChildElement("Coordinates");


                currentGranulesUrl = QUrl(coordsElement.text());
            }
        }
    }
    emit granuleImageUrlReceived();
}

void SimpleGranulesNode::getErrorImageUrl(QNetworkReply::NetworkError)
{
}

void SimpleGranulesNode::setTransparency(qint32 granuleId, qint32 transparency)
{
    if (granuleId == _granuleId)
        _transparency = transparency;
}

void SimpleGranulesNode::addGranuleNodeStart()
{
    getGranuleImageUrl();
}

void SimpleGranulesNode::downloadGranuleImage()
{
    imagePath = QString(cacheDir+"/granule_%1.png").arg(granuleId());

    // download product image
    if (!currentGranulesUrl.isEmpty())
    {
        if (!QFile::exists(imagePath) ) {
            DownloadImage* downloadImage = new DownloadImage;

            qDebug() << "urlGranules = " << currentGranulesUrl.url();
            downloadImage->setImageUrl(currentGranulesUrl.url(), imagePath);

            connect(downloadImage, &DownloadImage::downloaded, this, &SimpleGranulesNode::addGranuleNode);

            downloadImage->run();
        }
        else
        {
            addGranuleNode(imagePath);
        }
    }
}

void SimpleGranulesNode::addGranuleNode(QString _image_path)
{
    Q_UNUSED(_image_path);

//    QString imagePath = "/mnt/d/OISST-AVHRR-AMSR-V2.png";
//    QString filename = "/mnt/d/ascat_20120704_003001.png";
    QGLSceneNode* testNode = findSceneNode(imagePath);
    if (!testNode)// || newZoomFlag)
    {
        delete testNode;
        qreal minSphereLat = -M_PI_2;
        qreal maxSphereLat = M_PI_2;
        QGLSceneNode* testNode = BuildGranuleMerNode(1, minSphereLat, maxSphereLat, -M_PI, M_PI);
        qDebug() << "addTextureToGranuleNode:" << addTextureToGranuleNode(testNode, imagePath);

    //    testNone->setOptions(QGLSceneNode::NoOptions);
        QGLBuilder builder;
        builder.sceneNode()->addNode(testNode);

        // add SceneNode to cache
    //    TileCacheNumber tileNumber = TileCacheNumber(cur_zoom, lonTileNum,separation-1-latTileNum);
    //    tileNodeCache.insert(tileNumber, tempNode);

        QGLSceneNode* temp2 = builder.finalizedSceneNode();
        temp2->setObjectName(imagePath);
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
    int stacks = 64;
    int slices = 64;
//    if (curZoom > 4)
//    {
//        stacks = qPow(2, curZoom);
//        slices = qPow(2, curZoom);
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
            curDecart = llh2xyz(curSphereLat, curSphereLon, 0.001 + height()/10000.0);

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
            curDecartNext = llh2xyz(curSphereLatNext, curSphereLon, 0.001 + height()/10000.0);
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

void SimpleGranulesNode::show()
{
    visible = true;
    setOptions(QGLSceneNode::CullBoundingBox);
}

void SimpleGranulesNode::hide()
{
    visible = false;
    setOptions(QGLSceneNode::HideNode);
}

bool SimpleGranulesNode::isVisible()
{
    return visible;
}
