#ifndef GRANULESNODE_H
#define GRANULESNODE_H


#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLPainter>

#include "qgltexture2d.h"
#include <QImage>
#include <QFile>

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
//#include <QTimer>
//#include <QObject>
//#include <typeinfo>

#include "more/structure.h"
#include "more/geofunctions.h"
#include "network/downloadimage.h"

class SimpleGranulesNode : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit SimpleGranulesNode(QObject *parent, QSharedPointer<QGLMaterialCollection> materials,
                                ConfigData configData, qint32 granuleId, qint32 productId);
    ~SimpleGranulesNode();

    qint32 height(){return _height;}
    void setHeight(qint32 height){_height = height;}
    qint32 granuleId(){return _granuleId;}
    void setGranuleId(qint32 granuleId){_granuleId = granuleId;}
    qint32 productId(){return _productId;}
    void setProductId(qint32 productId){_productId = productId;}
    qint32 transparency(){return _transparency;}

    void getGranuleImageUrl();

protected:
    QByteArray currentRequest;
    QString imagePath;

private:
    QGLSceneNode *BuildGranuleMerNode(int separation, qreal minSphereLat,
                                      qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToGranuleNode(QGLSceneNode *tempNode, QString filepath);

    void addGranuleNodeStart();
    void downloadGranuleImage();
    void addGranuleNode(QString _image_path);

    qint32 _height;
    qint32 _transparency;
    qint32 _granuleId;
    qint32 _productId;
    QString cacheDir;
    QString serverName;
    QUrl urlGranules;
    QUrl currentGranulesUrl;

    QNetworkAccessManager* networkManager;

signals:
    void granuleImageUrlReceived();
    void granuleImageDownloaded();
    
public slots:
    void setTransparency(qint32 granuleId, qint32 transparency);
    void getErrorImageUrl(QNetworkReply::NetworkError);
    void slotReadyReadImageUrl();
};

#endif // GRANULESNODE_H
