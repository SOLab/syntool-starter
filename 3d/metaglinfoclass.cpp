#include "metaglinfoclass.h"

MetaGLInfoClass::MetaGLInfoClass(EarthView *parentView, Earth *earth, QSharedPointer<QGLMaterialCollection> palette, ConfigData *configData) :
    QGLSceneNode(qobject_cast<QGLSceneNode*>(earth))
{
    m_palette = palette;
    m_configData = configData;
    lineHash = new QMap<qint32, Line3DNode*>();
    pointHash = new QMap<qint32, Point3DNode*>();
    rectHash = new QMap<qint32, Rect3DNode*>();

    //    setParent(parentView);
}

void MetaGLInfoClass::drawAll(QGLPainter *painter)
{
    QMap<qint32, Line3DNode*>::const_iterator iLine = lineHash->constBegin();
    while (iLine != lineHash->constEnd()) {
        iLine.value()->draw(painter);
        ++iLine;
    }

    QMap<qint32, Point3DNode*>::const_iterator iPoint = pointHash->constBegin();
    while (iPoint != pointHash->constEnd()) {
        iPoint.value()->draw(painter);
        ++iPoint;
    }

    QMap<qint32, Rect3DNode*>::const_iterator iRect = rectHash->constBegin();
    while (iRect != rectHash->constEnd()) {
        iRect.value()->draw(painter);
        ++iRect;
    }
}

void MetaGLInfoClass::addPoint(GeoCoords pos)
{
    Point3DNode* pointNode = new Point3DNode(this);
    pointNode->createPoint(pos);

    qint32 pointNumber;
    if (pointHash->size() == 0)
        pointNumber = 1;
    else
    {
        pointNumber = pointHash->end().key() + 1;
        if (pointNumber > 10000)
            pointNumber = 1;
        while (pointHash->contains(pointNumber))
            pointNumber += 1;
    }
    pointHash->insert(pointNumber, pointNode);
}

void MetaGLInfoClass::addLine(GeoCoords pos1, GeoCoords pos2)
{
    Line3DNode* lineNode = new Line3DNode(this);
    lineNode->createLine(pos1, pos2);

    qint32 lineNumber;
    if (lineHash->size() == 0)
        lineNumber = 1;
    else
    {
        lineNumber = lineHash->end().key() + 1;
        if (lineNumber > 10000)
            lineNumber = 1;
        while (lineHash->contains(lineNumber))
            lineNumber += 1;
    }
    lineHash->insert(lineNumber, lineNode);
}

void MetaGLInfoClass::addRect(GeoCoords pos1, GeoCoords pos2)
{
    Rect3DNode* rectNode = new Rect3DNode(this);
    rectNode->createRect(pos1, pos2);

    qint32 rectNumber;
    if (rectHash->size() == 0)
        rectNumber = 1;
    else
    {
        rectNumber = rectHash->end().key() + 1;
        if (rectNumber > 10000)
            rectNumber = 1;
        while (rectHash->contains(rectNumber))
            rectNumber += 1;
    }
    rectHash->insert(rectNumber, rectNode);
}
