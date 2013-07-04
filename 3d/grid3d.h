#ifndef GRID3D_H
#define GRID3D_H

#include <QObject>
#include <QColor>
#include "line3dnode.h"

class Grid3D : public QObject
{
    Q_OBJECT
public:
    explicit Grid3D(QObject *parent = 0);
    ~Grid3D();
    void createGrid();
    void draw(QGLPainter *painter);
    
private:
    QColor _color;

    QList<Line3DNode*>* lines;

signals:
    
public slots:
    
};

#endif // GRID3D_H
