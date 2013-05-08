#ifndef GRANULESNODE_H
#define GRANULESNODE_H


#include "qglbuilder.h"
#include "qgraphicsrotation3d.h"
#include <QGLPainter>

#include "qgltexture2d.h"
#include <QImage>
#include <QFile>
//#include <QTimer>
//#include <QObject>
//#include <typeinfo>

#include "more/structure.h"
#include "more/geofunctions.h"

class SimpleGranulesNode : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit SimpleGranulesNode(QObject *parent, QSharedPointer<QGLMaterialCollection> materials, ConfigData configData);

private:
    QGLSceneNode *BuildGranuleMerNode(int separation, qreal minSphereLat,
                                      qreal maxSphereLat, qreal minSphereLon, qreal maxSphereLon);
    bool addTextureToGranuleNode(QGLSceneNode *tempNode, QString filepath);
    void addGranuleNodes();

signals:
    
public slots:
    
};

#endif // GRANULESNODE_H
