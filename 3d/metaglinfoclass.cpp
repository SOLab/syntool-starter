#include "metaglinfoclass.h"

MetaGLInfoClass::MetaGLInfoClass(EarthView *parentView, Earth *earth, QSharedPointer<QGLMaterialCollection> palette,
                                 ConfigData *configData) :
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

void MetaGLInfoClass::addPoint(GeoCoords pos, Geometry::Type objectType)
{
    Point3DNode* pointNode = new Point3DNode(this);
    pointNode->createPoint(objectType, pos);

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

    if (objectType == Geometry::Point)
        pointNode->setObjectName("Point"+QString::number(pointNumber));
    else if (objectType == Geometry::Tag)
        pointNode->setObjectName("Tag"+QString::number(pointNumber));
    else if (objectType == Geometry::Pin)
        pointNode->setObjectName("Pin"+QString::number(pointNumber));
    emit addPointSignal(pointNumber, pos, pointNode->objectName());
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

    lineNode->setObjectName("Line"+QString::number(lineNumber));
    emit addLineSignal(lineNumber, pos1, pos2, lineNode->objectName());
}

qint32 MetaGLInfoClass::addRect(GeoCoords pos1, GeoCoords pos2)
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

    rectNode->setObjectName("Rect"+QString::number(rectNumber));
    emit addRectSignal(rectNumber, pos1, pos2, rectNode->objectName());
    return rectNumber;
}

void MetaGLInfoClass::addIteractionRect(GeoCoords pos1, GeoCoords pos2)
{
    iteractionRectNumber = addRect(pos1, pos2);
}

void MetaGLInfoClass::moveIteractionRect(GeoCoords pos2)
{
    rectHash->value(iteractionRectNumber)->changedPoint(pos2);
}

void MetaGLInfoClass::removeIteractionRect()
{
    Rect3DNode* iterRect = rectHash->value(iteractionRectNumber);
    rectHash->remove(iteractionRectNumber);
    iterRect->deleteLater();
    emit removeObjectSignal(Geometry::Rect, iteractionRectNumber);
}

void MetaGLInfoClass::removeObjectSlot(Geometry::Type type, qint32 objectNumber)
{
    if (type == Geometry::Line)
    {
        Line3DNode* lineNode = lineHash->value(objectNumber);
        lineHash->remove(objectNumber);
        if (lineNode)
            lineNode->deleteLater();
    }
    else if (type == Geometry::Rect)
    {
        Rect3DNode* rectNode = rectHash->value(objectNumber);
        rectHash->remove(objectNumber);
        if (rectNode)
            rectNode->deleteLater();
    }
    else if (type == Geometry::Pin || type == Geometry::Tag || type == Geometry::Point)
    {
        Point3DNode* pointNode = pointHash->value(objectNumber);
        pointHash->remove(objectNumber);
        if (pointNode)
            pointNode->deleteLater();
    }
    displayed();
}
