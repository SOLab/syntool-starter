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
                                ConfigData *configData, qint32 granuleId, qint32 productId, bool IsGlobalCoverage);
    ~SimpleGranulesNode();

    qint32  height(){return m_height;}
    void    setHeight(qint32 height){m_height = height;}
    qint32  granuleId(){return m_granuleId;}
    void    setGranuleId(qint32 granuleId){m_granuleId = granuleId;}
    qint32  productId(){return m_productId;}
    void    setProductId(qint32 productId){m_productId = productId;}
    qint32  transparency(){return m_transparency;}

    void getGranuleImageUrl();
    void show();
    void hide();
    bool isVisible();
    QString     imagePath;
    void rebuildGranuleNode();

protected:
    QByteArray  currentRequest;
    bool        isGlobalCoverage;

private:
    QGLSceneNode *BuildGranuleMerNode(int separation, qreal minSphereLat,
                                      qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToGranuleNode(QGLSceneNode *tempNode, QString filepath);

    void addGranuleNodeStart();
    void downloadGranuleImage();
    void addGranuleNode(QString image_path);

    qint32  m_height;
    qint32  m_transparency;
    qint32  m_granuleId;
    qint32  m_productId;
    QString cacheDir;
    QString serverName;
    QUrl    urlGranules;
    QUrl    currentGranulesUrl;
    bool    visible;
    bool    rebuild;

    QNetworkAccessManager*  networkManager;
    QGLSceneNode*           mainNode;

signals:
    void granuleImageUrlReceived();
    void granuleImageDownloaded();
    
public slots:
    void setTransparency(qint32 granuleId, qint32 transparency);
    void getErrorImageUrl(QNetworkReply::NetworkError);
    void slotReadyReadImageUrl();
};

#endif // GRANULESNODE_H
