#ifndef TILECACHECLASS_H
#define TILECACHECLASS_H

#include <QObject>
#include "more/structure.h"

/*
 * class TileCacheNumber for caching OSM tiles
 */
class TileCacheNumber
{
public:
    TileCacheNumber(qint32 zoom, qint32 x, qint32 y)
    {
        m_tileNumber.zoom = zoom;
        m_tileNumber.x = x;
        m_tileNumber.y = y;
    }

    TileNumber tileNumber() const {return m_tileNumber;}
//private:
    TileNumber m_tileNumber;
};

inline bool operator==(const TileCacheNumber &tcn1, const TileCacheNumber &tcn2)
{
    return (tcn1.tileNumber() == tcn2.tileNumber());
}

inline uint qHash(const TileCacheNumber &key)
{
    return qHash(key.tileNumber().zoom) ^ qHash( key.tileNumber().x ) ^ qHash( key.tileNumber().y );
}

/*
 * class TileCacheNumber for caching simple granules (one tile)
 */
class SimpleGranuleCacheNumber : public QObject
{
    Q_OBJECT
public:
    explicit SimpleGranuleCacheNumber(const qint32 granuleId, const qint32 productId, QObject *parent = 0);

    qint32  granuleId(){return m_granuleId;}
    void    setGranuleId(qint32 granuleId){m_granuleId = granuleId;}
    qint32  productId(){return m_productId;}
    void    setProductId(qint32 productId){m_productId = productId;}

private:
    qint32 m_granuleId;
    qint32 m_productId;

signals:
    
public slots:
};

inline bool operator==(SimpleGranuleCacheNumber &sgcn1, SimpleGranuleCacheNumber &sgcn2)
{
    return (sgcn1.granuleId() == sgcn2.granuleId());
}

inline uint qHash(SimpleGranuleCacheNumber &key)
{
    return qHash(key.granuleId()) ^ qHash(key.productId());
}

/*
 * class TileCacheNumber for caching complex granules (many tile)
 */
class TiledGranuleCacheNumber : public QObject
{
    Q_OBJECT
public:
    explicit TiledGranuleCacheNumber(const qint32 granuleId, const qint32 productId, QObject *parent = 0);

    qint32      granuleId(){return m_granuleId;}
    void        setGranuleId(qint32 granuleId){m_granuleId = granuleId;}
    qint32      productId(){return m_productId;}
    void        setProductId(qint32 productId){m_productId = productId;}
    qint32      height(){return m_height;}
    void        setHeight(qint32 height){m_height = height;}
    qint32      transparency(){return m_transparency;}

    TileNumber  tileNumber(){return m_tileNumber;}
    void        setTileNumber(TileNumber tileNumber){m_tileNumber = tileNumber;}

//    bool operator==(TiledGranuleCacheNumber &tgcn);

private:
    qint32      m_granuleId;
    qint32      m_productId;
    qint32      m_height;
    qreal       m_transparency;
    TileNumber  m_tileNumber;

signals:

public slots:
    void setTransparency(qint32 granuleId, qint32 transparency);
};

inline bool operator==(TiledGranuleCacheNumber &tgcn1, TiledGranuleCacheNumber &tgcn2)
{
    return (tgcn1.granuleId() == tgcn2.granuleId() && tgcn1.tileNumber() == tgcn2.tileNumber());
}

inline uint qHash(TiledGranuleCacheNumber &key)
{
    return qHash(key.granuleId()) ^ qHash(key.productId());
}

#endif // TILECACHECLASS_H
