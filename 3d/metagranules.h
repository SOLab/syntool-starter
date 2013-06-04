#ifndef METAGRANULES_H
#define METAGRANULES_H

#include <QObject>
#include <QGLSceneNode>
#include <QMap>

#include "more/structure.h"
#include "more/ProductStructures.h"
#include "tilecachenumbersclass.h"
#include "simplegranulesnode.h"

class EarthView;

class MetaGranules : public QGLSceneNode
{
    Q_OBJECT
public:
    MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData *configData);
    void drawSimpleGranules(QGLPainter *painter);
    void drawTiledGranules(QGLPainter *painter);
    void setParent(EarthView *parentView){m_parentView = parentView;}
    void addGranuleNode(qint32 granuleId, qint32 productId);

private:
    // int idGranule
    QCache<qint32, SimpleGranulesNode>*      simpleGranuleCache;
    QCache<qint32, SimpleGranuleCacheNumber> simpleGranuleNumberCache;
    // <height, granuleId> - QMap items are always sorted by key.
    QMap<qint32, qint32> heightGranuleMap;

    QCache<TileCacheNumber, QGLSceneNode> tiledGranuleCache;
    QSharedPointer<QGLMaterialCollection> m_palette;

    EarthView*  m_parentView;
    ConfigData* m_configData;
    qint32      currentHeight;
    qint32      maxHeight;

    // <NaiadProductId, Product>
    QHash<QString, Product>* m_productsHash;
    // <NaiadId, NaiadProductId>
    QHash<qint32, QString>*  m_productsIdName;

signals:
    void displayed();
    
public slots:
    void addSimpleGranuleNode(qint32 granuleId, qint32 productId);
    void removeSimpleGranuleNode(qint32 granuleId, qint32 productId);
    void setGranuleTransparency(qint32 granuleId, qint32 productId, qint32 transparency);
    void setProductsHashSlot(QHash<QString, Product>* productsHash, QHash<qint32, QString>* productsIdName);
    
};

#endif // METAGRANULES_H
