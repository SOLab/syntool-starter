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
#include "qglscenenode.h"

#include <qmath.h>
#include <QTimer>
#include <QApplication>
#include "earthscene.h"
#include "skybox.h"
#include "navigatebutton.h"

/* Constants (WGS ellipsoid) */
//Средний радиус 6371,0 км

const double a = 6378137.0;
const double e = 8.1819190842622e-2;
const double pi = 3.1415926535897932384626433832795;

struct geoDetic
{
  double alt, lat, lon;
};

geoDetic convert_ecef_to_wgs84(double x, double y, double z)
{
    geoDetic pos;
    pos.lon = 0;
    pos.lat = 0;
    pos.alt = 0;

    double deg = 0.01745329252;
    double r = qSqrt(x * x + y * y);
    pos.lon = qAsin(y / r) / deg;
    if (pos.lon > 180)
        pos.lon = 180 - pos.lon;
    pos.lat = qAtan(z / r) / deg;
    pos.lon = 90 - pos.lon;
    //определять долготу по координате х камеры
//    double b = qSqrt(qPow(a, 2) * (1 - qPow(e, 2)));
//    double ep = qSqrt((qPow(a, 2) - qPow(b, 2)) / qPow(b, 2));
//    double p = qSqrt(qPow(x, 2) + qPow(y, 2));
//    double th = qAtan2(a * z, b * p);
//    pos.lon = qAtan2(y, x);
//    pos.lat = qAtan2((z + qPow(ep, 2) * b * qPow(qSin(th), 3)), (p - qPow(e, 2) * a * qPow(qCos(th), 3)));
//    double N = a / (qSqrt(1 - qPow(e, 2) * qPow(qSin(pos.lat), 2)));
//    pos.alt = p / qCos(pos.lat) - N;

//    pos.lon = pos.lon / (2 * pi);

//    bool k = qAbs(x) < 1.0 && qAbs(y) < 1.0;
//    if (k)
//        pos.alt = qAbs(z) - b;

//    pos.lon = pos.lon * 180 / pi;
//    pos.lat = pos.lat * 180 / pi;

    return pos;
}

EarthView::EarthView(QWindow *parent)
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
    scale2F = QSizeF(1/scale,1/scale);
    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale);
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

    earth = new Earth(this, m_palette);
    m_scene->mainNode()->addNode(earth);

    QString path = ":/skybox";
    m_skybox = new SkyBox(this, path);
}

EarthView::~EarthView()
{
    delete earth;
//    delete sunEffect;
}

void EarthView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);
}

void EarthView::paintGL(QGLPainter *painter)
{
    glEnable(GL_BLEND);
    m_skybox->draw(painter);
    earth->draw(painter);
    navigateButton->draw(painter);

    if (navigateButtonPressed)
    {
        navigateButton->drawSector(navigateVector, painter);
//        earth->drawImage(painter);
//        float zoom = log10(scale)/log10(2);
//        earth->changeTexture(painter, zoom);
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

    qDebug() << "camera: " << camera()->eye();
    geoDetic pos = convert_ecef_to_wgs84(camera()->eye().x()*a/10.0,
                                         camera()->eye().z()*a/10.0,
                                         camera()->eye().y()*a/10.0);

    qDebug() << "long" << pos.lon;
    qDebug() << "lat" << pos.lat;
    qDebug() << "alt" << pos.alt;

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
    if (e->delta() > 0)
    {
        scalePlus();
//        QKeyEvent key(QKeyEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier, "Plus", false, 0 );
//        EarthView::keyPressEvent(&key);
    }
    else {
        scaleMinus();
//        QKeyEvent key(QKeyEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier, "Minus", false, 0 );
//        EarthView::keyPressEvent(&key);
    }
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
}

void EarthView::scalePlus()
{
    if (scale < 2000)
    {
        for (int i = 0; i < 5; i++)
        {
            QTimer::singleShot(i*50, this, SLOT(scalePlus_slot()));
        }
    }
//    qDebug() << "scale: " << scale;
}

void EarthView::scalePlus_slot()
{
    float dscale = scale/20.0;
    scale += dscale;
    scale2F = QSizeF(1/scale,1/scale);

    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale);
    else
        camera()->setViewSize(scale2F);
    qDebug() << "scale: " << scale;
    qDebug() << log10(scale)/log10(2);
}

void EarthView::scaleMinus()
{
    if (scale > 0.8)
    {
        for (int i = 0; i < 5; i++)
        {
            QTimer::singleShot(i*50, this, SLOT(scaleMinus_slot()));
        }
    }
//    qDebug() << "scale: " << scale;
}

void EarthView::scaleMinus_slot()
{
    float dscale = scale/20.0;
    scale -= dscale;
    scale2F = QSizeF(1/scale,1/scale);

    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale);
    else
        camera()->setViewSize(scale2F);
    qDebug() << "scale: " << scale;
    qDebug() << log10(scale)/log10(2);
}

void EarthView::leftSlot()
{
    qDebug() << "left";
}


void EarthView::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed)
    {
        QPoint delta = e->pos() - startPan;
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
        lastPan = e->pos();
//        if ((e->modifiers() & Qt::ControlModifier) != 0)
//            wheel(delta.y() * -60);
//        else if ((e->modifiers() & Qt::ShiftModifier) != 0)
//            pan(delta.x(), delta.y());
//        else
        rotate(delta.x(), delta.y());
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
}

void EarthView::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    mousePressed = false;
    navigateButtonPressed = false;
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
