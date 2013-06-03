#include "tilecachenumbersclass.h"

SimpleGranuleCacheNumber::SimpleGranuleCacheNumber(const qint32 granuleId, const qint32 productId, QObject *parent) :
    QObject(parent)
{
    m_granuleId = granuleId;
    m_productId = productId;
}

TiledGranuleCacheNumber::TiledGranuleCacheNumber(const qint32 granuleId, const qint32 productId, QObject *parent) :
    QObject(parent)
{
    m_granuleId = granuleId;
    m_productId = productId;
    m_height = 0;
    m_transparency = 0;
}

void TiledGranuleCacheNumber::setTransparency(qint32 granuleId, qint32 transparency)
{
    if (granuleId == m_granuleId)
        m_transparency = transparency;
}
