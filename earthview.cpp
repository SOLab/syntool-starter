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
#include "qglbuilder.h"
#include "qglmaterialcollection.h"
#include "qgltexture2d.h"
#include "qglmaterial.h"
#include "qglscenenode.h"
#include "qgllightmodel.h"
#include "qglsphere.h"
#include "qgraphicsrotation3d.h"
#include "qgraphicstranslation3d.h"
#include "qglshaderprogrameffect.h"

#include <QPropertyAnimation>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <qmath.h>
#include <QDebug>
#include <QSphere3D>
#include <QKeyEvent>
#include <GL/freeglut.h>
#include <QMatrix4x4>

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
    , spaceScene(new QGLSceneNode(this))
//    , sunEffect(0)
{
    //Generate geometry for the scene
    spaceScene = createScene();
    setOptions(QGLView::FOVZoom | QGLView::CameraNavigation | QGLView::ObjectPicking);
    //Set up the camera
    camera()->setEye(QVector3D(0, 0, 10));
//    qDebug() << camera()->screenRotation();
//    camera()->setScreenRotation(90);
//    qDebug() << camera()->screenRotation();

    scale = 1;
    scale2F = QSizeF(1/scale,1/scale);
    float fov = camera()->fieldOfView();
    if (fov != 0.0f)
        camera()->setFieldOfView(scale);
    else
        camera()->setViewSize(scale2F);
//    camera()->setFarPlane(1000);
    mousePressed = false;
    startPan = QPoint(-1, -1);
    lastPan = QPoint(-1, -1);
    panModifiers = Qt::NoModifier;

//    //Create the animation for the rotation of the Sun
//    QPropertyAnimation *animation;
//    animation = new QPropertyAnimation(this, "angle1", this);
//    animation->setStartValue(0.0f);
//    animation->setEndValue(360.0f);
//    animation->setDuration(7000);
//    animation->setLoopCount(-1);
//    animation->start();
//    animation->stop();

//    //Create the animation for the rotation of the Planet
//    QPropertyAnimation *animation2;
//    animation2 = new QPropertyAnimation(this, "angle2", this);
//    animation2->setStartValue(0.0f);
//    animation2->setEndValue(360.0f);
//    animation2->setDuration(3000);
//    animation2->setLoopCount(-1);
//    animation2->start();
//    animation2->stop();

//    //Create the animation for the rotation of the Solar-System
//    QPropertyAnimation *animation3;
//    animation3 = new QPropertyAnimation(this, "angle3", this);
//    animation3->setStartValue(0.0f);
//    animation3->setEndValue(360.0f);
//    animation3->setDuration(30000);
//    animation3->setLoopCount(-1);
//    animation3->start();
//    animation3->stop();

//    //Create the animation for the glow effect
//    QPropertyAnimation *glowAnimation;
//    glowAnimation = new QPropertyAnimation(this, "glowFactor", this);
//    glowAnimation->setStartValue(0.0f);
//    glowAnimation->setEndValue(0.0f);
//    glowAnimation->setKeyValueAt(0.5, 1.0f);
//    glowAnimation->setDuration(3000);
//    glowAnimation->setLoopCount(-1);
//    glowAnimation->start();
//    glowAnimation->stop();
}

EarthView::~EarthView()
{    
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
    delete spaceScene;
//    delete sunEffect;
}

void EarthView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);
}

void EarthView::paintGL(QGLPainter *painter)
{
//    sunRotation->setAngle(-m_angle1);
//    planetRotation->setAngle(m_angle2);
//    systemRotation->setAngle(m_angle3);
    // Set the effect active to make sure that the program is created
    // and bound so that we can update our uniform
//    sunEffect->setActive(painter,true);
//    sunEffect->program()->setUniformValue("glowFactor", m_glowFactor);
//    painter->modelViewMatrix().rotate(45.0f, 1.0f, 1.0f, 1.0f);
//    painter->modelViewMatrix().scale(3);

    spaceScene->draw(painter);
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
            if (scale < 2000)
            {
                scale += scale*0.1;
                scale2F = QSizeF(1/scale,1/scale);
                qDebug() << "scale: " << scale;

                float fov = camera()->fieldOfView();
                if (fov != 0.0f)
    //                camera()->setFieldOfView(camera()->fieldOfView() / scale_plus);
                    camera()->setFieldOfView(scale);
                else
    //                camera()->setViewSize(camera()->viewSize() / scale_plus);
                    camera()->setViewSize(scale2F);
            }
        }
        break;

        case Qt::Key_Minus:
        {
            if (scale > 1)
            {
                scale -= scale*0.1;
                scale2F = QSizeF(1/scale,1/scale);
                qDebug() << "scale: " << scale;

                float fov = camera()->fieldOfView();
                if (fov != 0.0f)
                    camera()->setFieldOfView(scale);
                else
                    camera()->setViewSize(scale2F);
            }

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

void EarthView::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    //    update();
}

void EarthView::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0)
    {
        QKeyEvent key(QKeyEvent::KeyPress, Qt::Key_Plus, Qt::NoModifier, "Plus", false, 0 );
        EarthView::keyPressEvent(&key);
    }
    else {
        QKeyEvent key(QKeyEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier, "Minus", false, 0 );
        EarthView::keyPressEvent(&key);
    }
}

void EarthView::rotate(int deltax, int deltay)
{
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
    float anglex = deltax * 90.0f / width();
    float angley = deltay * 90.0f / height();
    QQuaternion q = camera()->pan(-anglex);
    q *= camera()->tilt(-angley);
    camera()->rotateCenter(q);
}

void EarthView::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed)
    {
        QPoint delta = e->pos();
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
    //    QWindow::mouseMoveEvent(e);
}

void EarthView::mousePressEvent(QMouseEvent *e)
{
    mousePressed = true;
}

void EarthView::mouseReleaseEvent(QMouseEvent *e)
{
    mousePressed = false;
}

QGLSceneNode *EarthView::createScene()
{ 
    QGLBuilder builder;
    QGLSceneNode *root = builder.sceneNode();
    QUrl url;

    //set up our materials palette - this describes all
    //of the materials we will use for this scene.

    //solar surface
    QGLMaterial *mat1 = new QGLMaterial;
    url.setPath(QLatin1String(":/earth_6_3.jpg"));
    url.setScheme(QLatin1String("file"));
    mat1->setTextureUrl(url,0);
    m_LoadedTextures.push_back(mat1->texture(0));
    int sunMat = root->palette()->addMaterial(mat1);


    // create the sun for our solar system
    builder.pushNode()->setObjectName(QLatin1String("Solar"));
    builder<<QGLSphere(1);

    builder.currentNode()->setMaterialIndex(sunMat);

//    sunEffect = new QGLShaderProgramEffect();
//    sunEffect->setVertexShaderFromFile(":/solar.vsh");
//    sunEffect->setFragmentShaderFromFile(":/solar.fsh");
    builder.currentNode()->setEffect(QGL::LitModulateTexture2D);
//    builder.currentNode()->setUserEffect(sunEffect);

//    //create and add rotations for axial tilt and rotation
    sunRotation = new QGraphicsRotation3D();
    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));
    sunRotation->setAngle(0.0f);
    builder.currentNode()->addTransform(sunRotation);
    builder.currentNode()->addTransform(axialTilt1);

     //completed building, so finalise
    return builder.finalizedSceneNode();
}


//void SolarSystemView::drawText(QGLPainter *painter)
//{
//    QString _str = "ggggggggggggggggggggrrrrrrrrrrrrrrrrrrrrrrrrrreeeeeeeeeeeeeeeeeeeeeeeeeeee";

//qreal _red = 1;
//qreal _green = 0;
//qreal _blue = 0;
//qreal _alpha = 1;

//int _x, _y, _z;

//QFont _font = QFont();
//QFontMetrics fm(_font);
//QRect rect = fm.boundingRect(_str); // text bounding box rect.adjust(0, 0, 1, 1);
//QImage image(rect.size(), QImage::Format_ARGB32);
//image.fill(0); // set to transparent

//// draw the text on an image
//QPainter p2d(&image);
//p2d.setFont(_font);
//p2d.setPen(QColor(_red * 255, _green * 255, _blue * 255, _alpha * 255));
//p2d.drawText(0, 0, rect.width(), rect.height(), Qt::AlignCenter, _str);
//p2d.end();

//// convert the object coordinate to screen coordinate
//GLdouble winx, winy, winz;
//QMatrix4x4 model = painter->modelViewMatrix().top();
//QMatrix4x4 proj = painter->projectionMatrix().top();


//QGLUtils::objectToWindowCoord(_x, _y, _z, model.data(), proj.data(), &winx, &winy, &winz);
//winy -= rect.height()/2.0; // align center of height
//int x = (int)winx, y = (int)winy;
//QVector2DArray vertices;
//vertices.append(x, y + rect.height());
//vertices.append(x, y);
//vertices.append(x + rect.width(), y);
//vertices.append(x + rect.width(), y + rect.height());

//// texture coordinates
//QVector2DArray texCoord;
//texCoord.append(0.0f, 0.0f);
//texCoord.append(0.0f, 1.0f);
//texCoord.append(1.0f, 1.0f);
//texCoord.append(1.0f, 0.0f);

//// map the image to texture
//QGLTexture2D texture;
//texture.setImage(image);

//// get viewport
//GLint view4;
//glGetIntegerv(GL_VIEWPORT, &view0);
//painter->modelViewMatrix().push();
//painter->modelViewMatrix().setToIdentity();
//QMatrix4x4 projm;
//projm.ortho(view0, view2, view3, view1, 0, 1);
//painter->projectionMatrix().push();
//painter->projectionMatrix() = projm;

//// move to the actual position from the screen origin
//painter->modelViewMatrix().translate(0, 0, -winz);

//// enable blend to make the background transaprecy of the text
//glEnable(GL_BLEND);
//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//painter->clearAttributes();
//painter->setStandardEffect(QGL::FlatReplaceTexture2D);
//texture.bind();
//painter->setVertexAttribute(QGL::Position, vertices);
//painter->setVertexAttribute(QGL::TextureCoord0, texCoord);
//painter->draw(QGL::TriangleFan, 4);
//painter->setStandardEffect(QGL::FlatColor);
//glBindTexture(GL_TEXTURE_2D, 0);
//glDisable(GL_BLEND);
//painter->projectionMatrix().pop();
//painter->modelViewMatrix().pop();
//}
