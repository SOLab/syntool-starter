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
#include "earthview.h"

/* Constants (WGS ellipsoid) */
//Средний радиус 6371,0 км

// длина окружности Земли (экватор) 40 075,017
//const qint32 l = 40 075;
//const double a = 6378137.0;
//const double e = 8.1819190842622e-2;
//const double pi = 3.1415926535897932384626433832795;

GeoCoords getGeoCoordsPos(QVector3D eye)
{
    return ecef2wgs84Deg(eye.z(), eye.x(), eye.y());
}

EarthView::EarthView(ConfigData *configData, QSurfaceFormat sf, QWindow *parent)
    : QGLView(sf, parent)
    , m_scene(0)
    , m_palette(new QGLMaterialCollection())
{
    //Generate geometry for the scene
//    spaceScene = createScene();
    setOptions(QGLView::FOVZoom | QGLView::CameraNavigation | QGLView::ObjectPicking);
    //Set up the camera
    camera()->setEye(QVector3D(0, 0, 10));

    hideAllFlag = false;
    showCoordsFlag = false;
    showGridFlag = false;

    scale = 1;
    maxScale = 20480;
    scale2F = QSizeF(2/scale,2/scale);
    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale*2);
    else
        camera()->setViewSize(scale2F);
//    camera()->setFarPlane(1000);
    mousePressed = false;
    navigateButtonPressed = false;
    zoomInButtonPressed = false;
    zoomOutButtonPressed = false;
    navigateValueInit = false;

    startPan = QPoint(-1, -1);
    lastPan = QPoint(-1, -1);
    panModifiers = Qt::NoModifier;
    currentCursorMode = CursorMode::Move;
    currentMouseLat = -1000;
    currentMouseLon = -1000;

    navigateButton = new NavigateButton(this, m_palette, maxScale);

    m_scene = new EarthScene(this);
    m_scene->setPickable(true);
//    m_scene->mainNode()->addNode(navigateButton);

    earth = new Earth(this, m_palette, configData);
    earth->setObjectName("Earth");

    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
    connect (earth, &Earth::displayed, this, &EarthView::update);

    m_scene->mainNode()->addNode(earth);

    metaGranulesNode = new MetaGranules(this, m_palette, configData);
    connect (metaGranulesNode, &MetaGranules::displayed, this, &EarthView::update);
    connect (earth, &Earth::updatedTilesRangeSignal, metaGranulesNode, &MetaGranules::updatedTilesRangeSignal);
    connect (earth, &Earth::updatedAllTilesSignal, metaGranulesNode, &MetaGranules::updatedAllTilesSignal);

    connect (this, &EarthView::updatedTilesSignal, metaGranulesNode, &MetaGranules::updateTilesSignal);
    connect (metaGranulesNode, &MetaGranules::displayed, this, &EarthView::update);

    metaGLInfoNode = new MetaGLInfoClass(this, earth, m_palette, configData);
    connect (metaGLInfoNode, &MetaGLInfoClass::displayed, this, &EarthView::update);

//    m_scene->mainNode()->addNode(granulesNode);

    QString path = ":/skybox";
    m_skybox = new SkyBox(this, path);

    lastMouseMoveTime = QTime::currentTime();
    delta = QPoint(0,0);

    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);

    test = false;
    firstPointFlag = true;
}

EarthView::~EarthView()
{
}

void EarthView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);
}

void EarthView::paintGL(QGLPainter *painter)
{
    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    GLfloat color[4] = { 1, 1, 1, 0.2};
//    glColor4fv(color);
//    glDisable(GL_CULL_FACE);
    m_skybox->draw(painter);
    earth->draw(painter);

    metaGLInfoNode->drawAll(painter, showGridFlag);

    if (!hideAllFlag)
    {
        metaGranulesNode->drawSimpleGranules(painter);
        metaGranulesNode->drawTiledGranules(painter);
    }

    if (showCoordsFlag)
        navigateButton->draw(painter, scale, showCoordsFlag, currentMouseLat, currentMouseLon);
    else
        navigateButton->draw(painter, scale, showCoordsFlag);

    if (navigateButtonPressed)
    {
        navigateButton->drawSector(navigateVector, painter);
    }

    // calculate position 2D buttons
    if (!navigateValueInit)
    {
        centerNavigateButton = navigateButton->navButton->boundingBox().center();
        radiusNavigateButton =  (navigateButton->navButton->boundingBox().maximum().x() -
                      navigateButton->navButton->boundingBox().minimum().x()) / 2;

        centerZoomInButton = navigateButton->zoomInButton->boundingBox().center();
        radiusZoomInButton =  (navigateButton->zoomInButton->boundingBox().maximum().x() -
                               navigateButton->zoomInButton->boundingBox().minimum().x()) / 2;
        centerZoomOutButton = navigateButton->zoomOutButton->boundingBox().center();
        radiusZoomOutButton =  (navigateButton->zoomOutButton->boundingBox().maximum().x() -
                                navigateButton->zoomOutButton->boundingBox().minimum().x()) / 2;
        navigateValueInit = true;
    }

    glDisable(GL_BLEND);
}

void EarthView::keyPress(QKeyEvent *e)
{
    this->keyPressEvent(e);
}

void EarthView::keyPressEvent(QKeyEvent *e)
{
    //QGLView keys
    //case Qt::Key_Escape:
    //case Qt::Key_Q:
    //case Qt::Key_Left: + control and shift modifiers
    //case Qt::Key_Right: + control and shift modifiers
    //case Qt::Key_Up: + control and shift modifiers
    //case Qt::Key_Down: + control and shift modifiers

    //Инерциальная геоцентрическая система координат

    switch (e->key())
    {
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
        {
//            camera()->setCenter(QVector3D(1,0,0));
            QGLView::keyPressEvent(e);
            GeoCoords pos = getGeoCoordsPos(camera()->eye());
            emit updatedTilesSignal(scale, pos);
        }
        break;

//        case Qt::Key_Left:
//        {
//            camera()->setEye(QVector3D(-15,0,15));
//        }
//        break;

        case Qt::Key_Plus:
        {
            scalePlus();
        }
        break;

        case Qt::Key_Minus:
        {
            scaleMinus();
        }
        break;

    }
}

void EarthView::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
//    m_buttons->clearPositions();
}

void EarthView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //    update();
}

void EarthView::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0){
        scalePlus();
    }
    else {
        scaleMinus();
    }
}

// for calling from wheelEvent on press button
void EarthView::scalePlus()
{
    // for OSM maximum 18 zoom - 458752
    // 28672 - zoom 14
    // 1792 - zoom 10
    // 224 - zoom 7 maximum
    if (scale < maxScale)
    {
        scalePlusMinusSlot(true);
    }
    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);
}

void EarthView::scaleMinus()
{
    if (scale > 0.8)
    {
        scalePlusMinusSlot(false);
    }
    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);
}

// for changed scale
float flog(float zoom, bool plus){
    if(!plus && (zoom - qFloor(zoom) < 0.001))
        zoom -= 1;
    zoom = qPow(2,qFloor(qLn(zoom)/qLn(2)));

    return qFloor(zoom)/4.0;
}

void EarthView::scalePlusMinusSlot(bool plus)
{
    float dscale = flog(scale, plus);
    if (plus)
        scale += dscale;
    else
        scale -= dscale;
    scale2F = QSizeF(2/scale,2/scale);

    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(2*scale);
    else
        camera()->setViewSize(scale2F);
    qDebug() << "scale: " << scale;
    qDebug() << "zoom =" << log10(scale)/log10(2.0);
}

void EarthView::rotate(int deltax, int deltay)
{
    float temp_scale = scale > 1.2 ? scale : 1.2;
    int rotation = camera()->screenRotation();
    if (rotation == 90 || rotation == 270) {
        qSwap(deltax, deltay);
    }
    if (rotation == 90 || rotation == 180) {
        deltax = -deltax;
    }
    if (rotation == 180 || rotation == 270) {
        deltay = -deltay;
    }
    float anglex = 5*deltax * 90.0f / (width() * temp_scale);
    float angley = 5*deltay * 90.0f / (height() * temp_scale);
    QQuaternion q = camera()->pan(-anglex);

    q *= camera()->tilt(-angley);
    camera()->rotateCenter(q);

    // emit signal about changed center
    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);
}

void EarthView::rotateToCoords(GeoCoords pos)
{
    pos.lat = pos.lat*180/M_PI;
    pos.lon = pos.lon*180/M_PI;

    float anglex = 10;
    float angley = 10;
    QQuaternion q;
    GeoCoords cameraPos;

    while (qAbs(anglex) > 0.01 || qAbs(angley) > 0.01)
    {
        cameraPos = getGeoCoordsPos(camera()->eye());

        if (pos.lon < cameraPos.lon)
            anglex = float(qAbs(cameraPos.lon - pos.lon)*-1);
        else
            anglex = float(qAbs(cameraPos.lon - pos.lon));

        if (pos.lat > cameraPos.lat)
            angley = float(qAbs(cameraPos.lat - pos.lat)*-1);
        else
            angley = float(qAbs(cameraPos.lat - pos.lat));

        q = camera()->pan(anglex);
        q *= camera()->tilt(angley);

        camera()->rotateCenter(q);
    }

    cameraPos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, cameraPos);
}

void EarthView::leftSlot()
{
}

void EarthView::mouseMoveEvent(QMouseEvent *e)
{
    if (currentCursorMode == CursorMode::GetAreaCoords && !firstPointFlag)
    {
        GeoCoords pos = mousePos2coords(e->pos());

        if (pos.lat > -100)
        {
            metaGLInfoNode->moveIteractionRect(GeoCoordsDeg2Rad(pos));
            update();
        }
    }

    if (mousePressed)
    {
        delta = e->pos() - startPan;
        if (e->modifiers() == panModifiers) {
            camera()->setEye(startEye);
            camera()->setCenter(startCenter);
            camera()->setUpVector(startUpVector);
        } else {
//            d->startPan = d->lastPan;
//            delta = e->pos() - d->startPan;
//            d->startEye = d->camera->eye();
//            d->startCenter = d->camera->center();
//            d->startUpVector = d->camera->upVector();
//            d->panModifiers = e->modifiers();
        }
        lastPan = e->pos();// not using
//        if ((e->modifiers() & Qt::ControlModifier) != 0)
//            wheel(delta.y() * -60);
//        else if ((e->modifiers() & Qt::ShiftModifier) != 0)
//            pan(delta.x(), delta.y());
//        else
        rotate(delta.x(), delta.y());


//        GeoCoords pos = getGeoCoordsPos(camera()->eye());
    }
//    if (d->panning) {
//        QPoint delta = e->pos() - d->startPan;
//        if (e->modifiers() == d->panModifiers) {
//            d->camera->setEye(d->startEye);
//            d->camera->setCenter(d->startCenter);
//            d->camera->setUpVector(d->startUpVector);
//        } else {
//            d->startPan = d->lastPan;
//            delta = e->pos() - d->startPan;
//            d->startEye = d->camera->eye();
//            d->startCenter = d->camera->center();
//            d->startUpVector = d->camera->upVector();
//            d->panModifiers = e->modifiers();
//        }
//        d->lastPan = e->pos();
//        if ((e->modifiers() & Qt::ControlModifier) != 0)
//            wheel(delta.y() * -60);
//        else if ((e->modifiers() & Qt::ShiftModifier) != 0)
//            pan(delta.x(), delta.y());
//        else
//            rotate(delta.x(), delta.y());
//    } else if ((d->options & QGLView::ObjectPicking) != 0) {
//        QObject *object = objectForPoint(e->pos());
//        if (d->pressedObject) {
//            // Send the move event to the pressed object.  Use a position
//            // of (0, 0) if the mouse is still within the pressed object,
//            // or (-1, -1) if the mouse is no longer within the pressed object.
//            QMouseEvent event
//                (QEvent::MouseMove,
//                 (d->pressedObject == object) ? QPoint(0, 0) : QPoint(-1, -1),
//                 e->globalPos(), e->button(), e->buttons(), e->modifiers());
//            QCoreApplication::sendEvent(d->pressedObject, &event);
//        } else if (object) {
//            if (object != d->enteredObject) {
//                if (d->enteredObject)
//                    sendLeaveEvent(d->enteredObject);
//                d->enteredObject = object;
//                sendEnterEvent(d->enteredObject);
//            }
//            QMouseEvent event
//                (QEvent::MouseMove, QPoint(0, 0),
//                 e->globalPos(), e->button(), e->buttons(), e->modifiers());
//            QCoreApplication::sendEvent(object, &event);
//        } else if (d->enteredObject) {
//            sendLeaveEvent(d->enteredObject);
//            d->enteredObject = 0;
//        }
//    }


    if (showCoordsFlag)
    {
        GeoCoords pos = mousePos2coords(e->pos());
        currentMouseLat = pos.lat;
        currentMouseLon = pos.lon;
        update();
    }

    lastMouseMoveTime = QTime::currentTime();
    QWindow::mouseMoveEvent(e);
}

void EarthView::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if(currentCursorMode != CursorMode::Move && e->button() == Qt::RightButton)
    {
        if (currentCursorMode == CursorMode::GetAreaCoords && !firstPointFlag)
        {
            metaGLInfoNode->removeIteractionRect();
        }
        currentCursorMode = CursorMode::Move;
        setCursor(Qt::ArrowCursor);
        emit setCursorModeSignal(currentCursorMode);
        update();
    }

    if (currentCursorMode == CursorMode::GetAreaCoords)
    {
        GeoCoords pos = mousePos2coords(e->pos());
        if (pos.lat > -100)
        {
            if (firstPointFlag)
            {
                metaGLInfoNode->removeIteractionRect();
                firstPointFlag = false;
                firstPoint = GeoCoordsDeg2Rad(pos);
                metaGLInfoNode->addIteractionRect(firstPoint, firstPoint);
            }
            else
            {
                emit areaCoordsSignal(firstPoint, GeoCoordsDeg2Rad(pos));
                currentCursorMode = CursorMode::Move;
                setCursor(Qt::ArrowCursor);
                emit setCursorModeSignal(currentCursorMode);
                update();
            }
//            if (currentCursorMode == CursorMode::GetAreaCoords)
//                emit areaCoordsSignal(pos1, pos2);
//            else if (currentCursorMode == CursorMode::RightBottomCoords)
//                emit rightBottomCoordsSignal(pos.lat, pos.lon);

            return;
        }
    }

    if (currentCursorMode == CursorMode::AddPin ||
        currentCursorMode == CursorMode::AddTag ||
        currentCursorMode == CursorMode::AddPoint)
    {
        GeoCoords pos = mousePos2coords(e->pos());
        if (pos.lat > -100)
        {
            if (currentCursorMode == CursorMode::AddPin)
                metaGLInfoNode->addPoint(GeoCoordsDeg2Rad(pos), Geometry::Pin);
            else if (currentCursorMode == CursorMode::AddTag)
                metaGLInfoNode->addPoint(GeoCoordsDeg2Rad(pos), Geometry::Tag);
            else if (currentCursorMode == CursorMode::AddPoint)
                metaGLInfoNode->addPoint(GeoCoordsDeg2Rad(pos), Geometry::Point);

            currentCursorMode = CursorMode::Move;
            setCursor(Qt::ArrowCursor);
            emit setCursorModeSignal(currentCursorMode);
            update();
            return;
        }
    }

    if (currentCursorMode == CursorMode::AddLine || currentCursorMode == CursorMode::AddRect)
    {
        GeoCoords pos = mousePos2coords(e->pos());
        if (pos.lat > -100)
        {
            if (!firstPointFlag)
            {
                if (currentCursorMode == CursorMode::AddLine)
                    metaGLInfoNode->addLine(firstPoint, GeoCoordsDeg2Rad(pos));
                else if (currentCursorMode == CursorMode::AddRect)
                    metaGLInfoNode->addRect(firstPoint, GeoCoordsDeg2Rad(pos));

                currentCursorMode = CursorMode::Move;
                setCursor(Qt::ArrowCursor);
                emit setCursorModeSignal(currentCursorMode);
                firstPointFlag = true;
                update();
            }
            else
            {
                firstPointFlag = false;
                firstPoint = GeoCoordsDeg2Rad(pos);
            }
            return;
        }
    }

//    registerPicking();
    if (objectForPoint(e->pos()))
    {
        QGLPickNode* node = qobject_cast<QGLPickNode*>(objectForPoint(e->pos()));
//        qCritical() << node->target();
//        qCritical() << earth;
        QString search_path = QString("tile-0-0-0");
//        qCritical() << earth->findSceneNode(search_path);
        if (node->target() == qobject_cast<QGLSceneNode*>(earth))
            qDebug() << "111111111";
    }

    int centerX = qRound(centerNavigateButton.x());
    int centerY = qRound(centerNavigateButton.y());
    int mouseX = e->pos().x();
    int mouseY = e->pos().y();
    int dX = mouseX - centerX;
    int dY = mouseY - centerY;

    if (qSqrt(qPow(dX, 2)+qPow(dY, 2)) < radiusNavigateButton)
    {
        navigateButtonPressed = true;
        navigateVector = QVector2D(dX, dY);
        navigateButtonPress();
        return;
    }

    dX = mouseX - centerZoomInButton.x();
    dY = mouseY - centerZoomInButton.y();
    if (qSqrt(qPow(dX, 2)+qPow(dY, 2)) < radiusZoomInButton)
    {
        zoomInButtonPressed = true;
        navigateButtonPress();
        return;
    }

    dX = mouseX - centerZoomOutButton.x();
    dY = mouseY - centerZoomOutButton.y();
    if (qSqrt(qPow(dX, 2)+qPow(dY, 2)) < radiusZoomOutButton)
    {
        zoomOutButtonPressed = true;
        navigateButtonPress();
        return;
    }

    mousePressed = true;
    startPan = e->pos();
    startEye = camera()->eye();
    startCenter = camera()->center();
    startUpVector = camera()->upVector();

    lastMouseMoveTime = QTime::currentTime();
}

void EarthView::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if (lastMouseMoveTime.msecsTo(QTime::currentTime()) > 50)
        qDebug() << "> 50!";
    if (lastMouseMoveTime.msecsTo(QTime::currentTime()) < 20)
    {
        mousePressed = false;
//        timeout();
    }

    mousePressed = false;
    navigateButtonPressed = false;
    zoomInButtonPressed = false;
    zoomOutButtonPressed = false;
}

void EarthView::timeout()
{
    if (!mousePressed)
    {
        int timeDelta = lastMouseMoveTime.msecsTo(QTime::currentTime());
        if (!timeDelta)
            timeDelta = 1;


        for (int i = 0; i < 30; i++)
        {
            QTimer::singleShot(i*20, this, SLOT(rotateInertia()));
        }

        lastMouseMoveTime = QTime::currentTime();
    }
}

void EarthView::setDefaultPosition()
{
    // set default position
    rotate(-60, 119);
    setScale(64);
    update();
    scalePlus();
}

void EarthView::rotateInertia()
{
    int timeDelta = lastMouseMoveTime.msecsTo(QTime::currentTime());
    lastMouseMoveTime = QTime::currentTime();
    if (!timeDelta)
        timeDelta = 1;

    if (qAbs(delta.x()) > 50)
        delta.setX((delta.x() > 0) ? 50 : -50);
    if (qAbs(delta.y()) > 50)
        delta.setY((delta.y() > 0) ? 50 : -50);

    if (!mousePressed)
        rotate(delta.x(), delta.y());

    delta.setX(delta.x());
    delta.setY(delta.y());
//    delta.setX(delta.x() / (float)(1.05));
//    delta.setY(delta.y() / (float)(1.05));
    update();
}

void EarthView::navigateButtonPress()
{
    if (navigateButtonPressed)
    {
        QPoint pos = this->mapFromGlobal(QCursor::pos());

        qint32 dX = qRound(centerNavigateButton.x()) - pos.x();
        qint32 dY = qRound(centerNavigateButton.y()) - pos.y();

        if (dX > 40) dX = 40;
        if (dX < -40) dX = -40;
        if (dY > 40) dY = 40;
        if (dY < -40) dY = -40;

        navigateVector = QVector2D(dX, dY);
        rotate(dX, dY);

//        navigateButton->drawSector(3,3, this->context());
        QTimer::singleShot(60, this, SLOT(navigateButtonPress()));
    }
    else if (zoomInButtonPressed)
    {
        QPoint pos = this->mapFromGlobal(QCursor::pos());

        qint32 dX = pos.x() - centerZoomInButton.x();
        qint32 dY = pos.y() - centerZoomInButton.y();
        if (qSqrt(qPow(dX, 2)+qPow(dY, 2)) < radiusZoomInButton)
        {
            scalePlus();
            QTimer::singleShot(120, this, SLOT(navigateButtonPress()));
        }
    }
    else if (zoomOutButtonPressed)
    {
        QPoint pos = this->mapFromGlobal(QCursor::pos());

        qint32 dX = pos.x() - centerZoomOutButton.x();
        qint32 dY = pos.y() - centerZoomOutButton.y();
        if (qSqrt(qPow(dX, 2)+qPow(dY, 2)) < radiusZoomOutButton)
        {
            scaleMinus();
            QTimer::singleShot(120, this, SLOT(navigateButtonPress()));
        }
    }
    else
    {
        update();
    }
}

float EarthView::getMemUsage()
{
    QProcess *Process1 = new QProcess();
    QProcess *Process2 = new QProcess();
    Process1->setStandardOutputProcess(Process2);

    Process1->start(QString("cat /proc/%1/status").arg(QApplication::applicationPid()));
    Process2->start("awk \"/VmRSS:/ {print($2)}\"");

    Process1->waitForFinished();
    Process2->waitForFinished();
    QString mem_percent = Process2->readAll();
    qDebug() << tr("Usage") << ": " << mem_percent.toFloat() << "Kb";
    delete Process1;
    delete Process2;
    return mem_percent.toFloat();
}

void EarthView::registerPicking()
{
    if (test)
        return;

//    QString search_path = QString("tile-0-0-0");
//    m_scene->mainNode()->addNode(earth->findSceneNode(search_path));
//    QGLAbstractScene *manager = m_scene->manager();
    QList<QGLPickNode *>nodes = m_scene->pickNodes();
    QList<QGLPickNode *>::const_iterator it = nodes.constBegin();
    for ( ; it != nodes.constEnd(); ++it)
    {
        QGLPickNode *node = *it;
//        qCritical() << node->objectName();
//        qCritical() << "!!!!!!!!!!!!!" << node->target();
//        qCritical() << "55555555" << node->target()->objectName();

        if (node->target()->objectName().startsWith("tile") || node->target()->objectName().startsWith("E"))
            qCritical() << node->target()->geometry();

        {
            registerObject(node->id(), node);
            connect(node, SIGNAL(clicked()),
                    this, SLOT(objectPicked()));
        }

    }
    test = true;
}

void EarthView::objectPicked()
{
//    Q_ASSERT(m_treeView);
    QGLPickNode *node = qobject_cast<QGLPickNode*>(sender());
    Q_ASSERT(node);
    QGLSceneNode *target = qobject_cast<QGLSceneNode*>(node->target());
    Q_UNUSED(target);
//    QModelIndex ix = m_model->selectNode(target);
//    m_treeView->expand(ix);
//    m_treeView->selectionModel()->clearSelection();
//    m_treeView->selectionModel()->select(ix, QItemSelectionModel::Select);
//    qCritical() << "Picked:" << node->target();
//    qCritical() << "Picked:" << target->objectName();
}

void EarthView::setCursorModeSlot(CursorMode::Mode value)
{
    currentCursorMode = value;
    if (currentCursorMode == CursorMode::Move)
        setCursor(Qt::ArrowCursor);
    else if (currentCursorMode == CursorMode::AddLine ||
             currentCursorMode == CursorMode::AddRect ||
             currentCursorMode == CursorMode::GetAreaCoords)
        setCursor(Qt::CrossCursor);
    else if (currentCursorMode == CursorMode::AddPin ||
             currentCursorMode == CursorMode::AddTag)
        setCursor(Qt::PointingHandCursor);
    emit setCursorModeSignal(currentCursorMode);

    firstPointFlag = true;
}

void EarthView::showCoordsSlot(bool value)
{
    showCoordsFlag = value;
    update();
}

void EarthView::showGridSlot(bool value)
{
    showGridFlag = value;
    update();
}

void EarthView::hideAllSlot(bool value)
{
    hideAllFlag = value;
    update();
}

void EarthView::unregisterPicking()
{
//    qCritical() << "99999999";
    QGLPickNode* node = earth->pickNode();
    deregisterObject(node->id());
//    connect(node, &QGLPickNode::clicked, this, &EarthView::objectPicked);

//        QList<QGLPickNode *>nodes = earth->pickNode();
//        QList<QGLPickNode *>::const_iterator it = nodes.constBegin();
//        for ( ; it != nodes.constEnd(); ++it)
//        {
//            QGLPickNode *node = *it;
//            deregisterObject(node->id());
//        }
}

// length of the vector
qreal norm(QVector3D v)
{
  return sqrt(v.x()*v.x()+v.y()*v.y()+v.z()*v.z());
}
//---------------------------------------------------------------------------
// scalar multiplication of two vectors
qreal sprod(QVector3D v1, QVector3D v2)
{
  return v1.x()*v2.x()+v1.y()*v2.y()+v1.z()*v2.z();
}
//---------------------------------------------------------------------------
// Sign
// x>0 : 1;   x=0 : 0;   x<0 : -1
int Sign(float x)
{
  return (x>0)-(x<0);
}

/*
 *p - the starting point of the ray
 *l - direction vector of the ray
 *c - center of the sphere
 *r - radius of the sphere
 */
float Ray2Sphere(QVector3D p, QVector3D l, QVector3D c, float r)
{
    // square of the radius
    r*=r;

    // square of the distance from the start point to the center of the sphere
    float d=qPow(norm(c-p),2);

    // projection of the sphere center on ray
    float s=sprod(c-p,l)/norm(l);

    if(d>r && s<0) // point outside the sphere and the ray is directed from the sphere
        return -1; // is no intersection

    // square of the distance from the line to the center of the sphere (Pythagoras' theorem)
    float h=d-s*s;

    if(h>r) // ray does not intersect the sphere
        return -1;

    return s+sqrt(r-h)*Sign(r-d); // distance to the intersection
}

GeoCoords EarthView::mousePos2coords(QPoint pos)
{
    QVector3D p = QVector3D(0,0,0);
    QVector3D l = mapPoint(pos);
    QVector3D c = camera()->modelViewMatrix() * camera()->center();

    float r = 0.5;
    // distance to the point
    qreal R_p = Ray2Sphere(p,l,c,r);
    if (R_p == -1)
    {
        GeoCoords pos = {-1000, -1000};
        return pos;
    }

    // distance to the front
    qreal R_front = qSqrt(qPow(p.x() - l.x(),2) + qPow(p.y() - l.y(),2) + qPow(p.z() - l.z(),2));

    qreal x1 = R_p*l.x()/R_front;
    qreal y1 = R_p*l.y()/R_front;
    qreal z1 = R_p*l.z()/R_front;

    QVector3D pointVector = QVector3D(x1,y1,z1);

    // from the camera coordinate to world coordinates
    QVector3D worldPos = camera()->modelViewMatrix().inverted() * pointVector;

    return ecef2wgs84Deg(worldPos.z(), worldPos.x(), worldPos.y());
}

void EarthView::mouseDoubleClickEvent(QMouseEvent *e)
{
    GeoCoords pos = mousePos2coords(e->pos());
    if (pos.lat > -100)
        qCritical() << "lat:" << pos.lat << ", lon:" << pos.lon;
}
