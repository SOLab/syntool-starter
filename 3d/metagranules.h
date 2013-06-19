#ifndef METAGRANULES_H
#define METAGRANULES_H

#include <QObject>
#include <QGLSceneNode>
#include <QGLPickNode>
#include <QGLView>
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
    void setParent(EarthView *parentView);
    void addGranuleNode(qint32 granuleId, qint32 productId, qint32 height, GranuleType::Type type);

private:
    // int idGranule
    QCache<qint32, SimpleGranulesNode>*      simpleGranuleCache;
    QCache<qint32, SimpleGranuleCacheNumber> simpleGranuleNumberCache;
    // <height, granuleId> - QMap items are always sorted by key.
    QMap<qint32, qint32> heightGranuleMap;

    QCache<TileCacheNumber, QGLSceneNode> tiledGranuleCache;
    QSharedPointer<QGLMaterialCollection> m_palette;

    QGLView*  m_parentView;
    ConfigData* m_configData;

    // <NaiadProductId, Product>
    QHash<QString, Product>* m_productsHash;
    // <NaiadId, NaiadProductId>
    QHash<qint32, QString>*  m_productsIdName;

signals:
    void displayed();
    
public slots:
    void addSimpleGranuleNode(qint32 granuleId, qint32 productId, qint32 height);
    void removeSimpleGranuleNode(qint32 granuleId, qint32 productId);
    void setGranuleTransparency(qint32 granuleId, qint32 productId, qint32 transparency);
    void setProductsHashSlot(QHash<QString, Product>* productsHash, QHash<qint32, QString>* productsIdName);

    void changedGranuleHeight(qint32 granuleId, qint32 height);
    void changedGranulesHeight(qint32 granuleId1, qint32 height1, qint32 granuleId2, qint32 height2);

    void registerPicking(SimpleGranulesNode *granulesNode);
    void unregisterPicking();
    void objectPicked();
};

#endif // METAGRANULES_H
