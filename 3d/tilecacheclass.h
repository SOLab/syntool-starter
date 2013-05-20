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
        _tileNumber.zoom = zoom;
        _tileNumber.x = x;
        _tileNumber.y = y;
    }

    TileNumber tileNumber() const {return _tileNumber;}
//private:
    TileNumber _tileNumber;
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

    qint32 granuleId(){return _granuleId;}
    void setGranuleId(qint32 granuleId){_granuleId = granuleId;}
    qint32 productId(){return _productId;}
    void setProductId(qint32 productId){_productId = productId;}

private:
    qint32 _granuleId;
    qint32 _productId;

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

    qint32 granuleId(){return _granuleId;}
    void setGranuleId(qint32 granuleId){_granuleId = granuleId;}
    qint32 productId(){return _productId;}
    void setProductId(qint32 productId){_productId = productId;}
    qint32 height(){return _height;}
    void setHeight(qint32 height){_height = height;}
    qint32 transparency(){return _transparency;}

    TileNumber tileNumber(){return _tileNumber;}
    void setTileNumber(TileNumber tileNumber){_tileNumber = tileNumber;}

//    bool operator==(TiledGranuleCacheNumber &tgcn);

private:
    qint32 _granuleId;
    qint32 _productId;
    qint32 _height;
    qreal _transparency;

    TileNumber _tileNumber;

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
