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

EarthView::EarthView(ConfigData configData, QWindow *parent)
    : QGLView(parent)
    , m_scene(0)
    , m_palette(new QGLMaterialCollection())
//    , sunEffect(0)
{
    //Generate geometry for the scene
//    spaceScene = createScene();
    setOptions(QGLView::FOVZoom | QGLView::CameraNavigation | QGLView::ObjectPicking);
    //Set up the camera
    camera()->setEye(QVector3D(0, 0, 10));

    scale = 1;
    scale2F = QSizeF(2/scale,2/scale);
    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale*2);
    else
        camera()->setViewSize(scale2F);
//    camera()->setFarPlane(1000);
    mousePressed = false;
    navigateButtonPressed = false;
    navigateValueInit = false;

    startPan = QPoint(-1, -1);
    lastPan = QPoint(-1, -1);
    panModifiers = Qt::NoModifier;

    m_scene = new EarthScene(this);
    navigateButton = new NavigateButton(this, m_palette);

    m_scene->setPickable(true);
    m_scene->mainNode()->addNode(navigateButton);

    earth = new Earth(this, m_palette, configData);
    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
    connect (earth, &Earth::displayed, this, &EarthView::update);

    m_scene->mainNode()->addNode(earth);

    metaGranulesNode = new MetaGranules(this, m_palette, configData);
//    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
    connect (metaGranulesNode, &MetaGranules::displayed, this, &EarthView::update);

//    m_scene->mainNode()->addNode(granulesNode);

    QString path = ":/skybox";
    m_skybox = new SkyBox(this, path);

    lastMouseMoveTime = QTime::currentTime();
    delta = QPoint(0,0);

    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);
}

EarthView::~EarthView()
{
    delete earth;
    delete m_scene;
    delete m_skybox;
    delete navigateButton;
}

void EarthView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);
}

void EarthView::paintGL(QGLPainter *painter)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat color[4] = { 1, 1, 1, 0.2};
    glColor4fv(color);
    glDisable(GL_CULL_FACE);
    m_skybox->draw(painter);
    earth->draw(painter);
    metaGranulesNode->draw(painter);
    navigateButton->draw(painter);

    glDisable(GL_BLEND);
//    getMemUsage();

    if (navigateButtonPressed)
    {
        navigateButton->drawSector(navigateVector, painter);
//        earth->drawImage(painter);
    }

    // calculate position 2D buttons
    if (!navigateValueInit)
    {
        centerNavigateButton = navigateButton->subButton->boundingBox().center();
        radiusNavigateButton =  (navigateButton->subButton->boundingBox().maximum().x() -
                      navigateButton->subButton->boundingBox().minimum().x()) / 2;
        navigateValueInit = true;
    }
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
    if (scale < 56)
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
    qDebug() << log10(scale)/log10(2);
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
    float anglex = 2*deltax * 90.0f / (width() * temp_scale);
    float angley = 2*deltay * 90.0f / (height() * temp_scale);
    QQuaternion q = camera()->pan(-anglex);

    q *= camera()->tilt(-angley);
    camera()->rotateCenter(q);

    // emit signal about changed center
    GeoCoords pos = getGeoCoordsPos(camera()->eye());
    emit updatedTilesSignal(scale, pos);

}

void EarthView::leftSlot()
{
}


void EarthView::mouseMoveEvent(QMouseEvent *e)
{
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


    lastMouseMoveTime = QTime::currentTime();
    QWindow::mouseMoveEvent(e);
}

void EarthView::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

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

        int dX = qRound(centerNavigateButton.x()) - pos.x();
        int dY = qRound(centerNavigateButton.y()) - pos.y();

        if (dX > 40) dX = 40;
        if (dX < -40) dX = -40;
        if (dY > 40) dY = 40;
        if (dY < -40) dY = -40;

        navigateVector = QVector2D(dX, dY);
        rotate(dX, dY);

//        navigateButton->drawSector(3,3, this->context());
        QTimer::singleShot(60, this, SLOT(navigateButtonPress()));
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
    qDebug() << "Usage " << mem_percent.toFloat() << "Kb";
    delete Process1;
    delete Process2;
    return mem_percent.toFloat();
}
