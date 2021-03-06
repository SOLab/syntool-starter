/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef BUILDER_H
#define BUILDER_H

#include <QKeyEvent>
#include <QProcess>
#include <QTime>
#include <qmath.h>
#include <QTimer>
#include <QApplication>
#include <GL/gl.h>
#include <QGLPickNode>

#include "qglview.h"
#include "earth.h"
#include "metagranules.h"
#include "metaglinfoclass.h"
#include "more/geofunctions.h"
#include "qglscenenode.h"
#include "earthscene.h"
#include "skybox.h"
#include "navigatebutton.h"
#include "more/structure.h"


QT_BEGIN_NAMESPACE
class QGLSceneNode;
class QGLBuilder;
class QGLShaderProgramEffect;
QT_END_NAMESPACE

class EarthScene;
class SkyBox;
class NavigateButton;

class EarthView : public QGLView
{
    Q_OBJECT
public:
    explicit EarthView(ConfigData *configData, QSurfaceFormat sf, QWindow *parent = 0);
    ~EarthView();

    void        setupViewport(int width, int height);
    void        keyPress(QKeyEvent* e);
    EarthScene* scene() { return m_scene; }
    float       getMemUsage();
    void        timeout();
    void        setScale(float _scale){scale = _scale;}
    float       getScale(){return scale;}
    void        setDefaultPosition();

    MetaGranules* metaGranulesNode;
    Earth *earth;
    MetaGLInfoClass* metaGLInfoNode;

    GeoCoords mousePos2coords(QPoint pos);
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    void keyPressEvent(QKeyEvent *e);
    void resizeGL(int w, int h);
//    void setupViewport(int width, int height);
    void resizeEvent(QResizeEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);

    QTime lastMouseMoveTime;
    QPoint delta;

    qreal currentMouseLat;
    qreal currentMouseLon;

//    QGLBuilder builder;
private:
    bool test;
    EarthScene* m_scene;
    float       scale;
    float       maxScale;
    bool        mousePressed;

    bool        navigateButtonPressed;
    bool        navigateValueInit;
    QVector3D   centerNavigateButton;
    QVector2D   navigateVector;
    qint32      radiusNavigateButton;

    bool        zoomInButtonPressed;
    QVector3D   centerZoomInButton;
    qint32      radiusZoomInButton;
    bool        zoomOutButtonPressed;
    QVector3D   centerZoomOutButton;
    qint32      radiusZoomOutButton;

    QPoint      startPan, lastPan;
    QSizeF      scale2F;
    QVector3D   startEye;
    QVector3D   startCenter;
    QVector3D   startUpVector;
    SkyBox*     m_skybox;

    NavigateButton*                       navigateButton;
    QSharedPointer<QGLMaterialCollection> m_palette;
    Qt::KeyboardModifiers                 panModifiers;

    CursorMode::Mode currentCursorMode;
    bool showCoordsFlag;
    bool showGridFlag;
    bool hideAllFlag;
//    QGLPickNode* pick_left;
//    float m_angle1;
//    float m_angle2;
//    float m_angle3;
//    float m_glowFactor;

    GeoCoords firstPoint;
    bool firstPointFlag;

    void scalePlus();
    void scaleMinus();
protected slots:
    void scalePlusMinusSlot(bool plus);
    void leftSlot();

    void navigateButtonPress();
    void rotateInertia();

public slots:
    void rotate(int deltax, int deltay);

    void registerPicking();
    void unregisterPicking();
    void objectPicked();

    void setCursorModeSlot(CursorMode::Mode value);
    void showCoordsSlot(bool value);
    void showGridSlot(bool value);
    void hideAllSlot(bool value);

    void rotateToCoords(GeoCoords pos);

signals:
    void updatedTilesSignal(qreal scale, GeoCoords geoCoords);
    void setCursorModeSignal(CursorMode::Mode value);
    void areaCoordsSignal(GeoCoords pos1, GeoCoords pos2);
//    void rightBottomCoordsSignal(qreal lat, qreal lon);

};

#endif
