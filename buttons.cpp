/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "buttons.h"
#include "qglpainter.h"
#include "qdebug.h"

Buttons::Buttons(QObject *parent, QSharedPointer<QGLMaterialCollection> palette)
    : QGLSceneNode(parent)
{
    setObjectName("Buttons");
    setPalette(palette);
    setOption(QGLSceneNode::CullBoundingBox, false);
    createButton(palette);
}

Buttons::~Buttons()
{
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
    for (int i=0; i<m_LoadedTextures_up.count(); ++i) {
        m_LoadedTextures_up.at(i)->cleanupResources();
    }
}

void Buttons::draw(QGLPainter *painter)
{
    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();

    QRect rect = painter->currentSurface()->viewportRect();
    QMatrix4x4 projm;
    projm.ortho(rect);
    painter->projectionMatrix() = projm;
    painter->modelViewMatrix().setToIdentity();

    if (m_left->position().isNull())
    {
        QVector2D pos(m_size.width() / 2, m_size.height() + m_size.height() / 2);
        m_left->setPosition(pos);
        pos.setX(5 * m_size.width()/2);
        m_right->setPosition(pos);

        QVector2D pos2(3*m_size.width()/2, m_size.height() / 2);
        m_up->setPosition(pos2);
        pos2.setY(5 * m_size.height() / 2);
        m_down->setPosition(pos2);
    }

    glDisable(GL_DEPTH_TEST);

    QGLSceneNode::draw(painter);

    glEnable(GL_DEPTH_TEST);

    painter->projectionMatrix().pop();
    painter->modelViewMatrix().pop();
}

void Buttons::clearPositions()
{
    m_left->setPosition(QVector3D());
    m_right->setPosition(QVector3D());
}

void Buttons::createButton(QSharedPointer<QGLMaterialCollection> palette)
{
    m_left = new SubButton(this, palette);
    m_left->setObjectName("Left Button");
    m_right = new SubButton(this, palette);
    m_right->setObjectName("Right Button");

    m_up = new SubButton(this, palette);
    m_up->setObjectName("Up Button");
    m_down = new SubButton(this, palette);
    m_down->setObjectName("Down Button");

    QGLMaterial *mat = new QGLMaterial;
    QImage im(":/arrow.png");
    m_size = im.size() / 2;
    QGLTexture2D *tex = new QGLTexture2D(mat);
    m_LoadedTextures.push_back(tex);
    tex->setImage(im);
    mat->setTexture(tex);

    setMaterial(mat);
    setEffect(QGL::FlatReplaceTexture2D);

    QGeometryData data;
    QSize f = im.size() / 4;
    QVector2D a(-f.width(), -f.height());
    QVector2D b(f.width(), -f.height());
    QVector2D c(f.width(), f.height());
    QVector2D d(-f.width(), f.height());
    QVector2D ta(0, 1);
    QVector2D tb(1, 1);
    QVector2D tc(1, 0);
    QVector2D td(0, 0);
    data.appendVertex(a, b, c, d);
    data.appendTexCoord(ta, tb, tc, td);
    data.appendIndices(0, 1, 2);
    data.appendIndices(0, 2, 3);

    // the right hand arrow geometry is same as above, flipped X <-> -X
    data.appendGeometry(data);
    data.texCoord(4).setX(1);
    data.texCoord(5).setX(0);
    data.texCoord(6).setX(0);
    data.texCoord(7).setX(1);
    data.appendIndices(4, 5, 6);
    data.appendIndices(4, 6, 7);

    m_left->setGeometry(data);
    m_left->setCount(6);
    m_left->setOption(QGLSceneNode::CullBoundingBox, false);

    m_right->setGeometry(data);
    m_right->setStart(6);
    m_right->setCount(6);
    m_left->setOption(QGLSceneNode::CullBoundingBox, false);


    QGeometryData data_up;
    QSize f_up = im.size() / 4;
    QVector2D a_up(-f_up.width(), f_up.height());
    QVector2D b_up(-f_up.width(), -f_up.height());
    QVector2D c_up(f_up.width(), -f_up.height());
    QVector2D d_up(f_up.width(), f_up.height());
    QVector2D ta_up(0, 1);
    QVector2D tb_up(1, 1);
    QVector2D tc_up(1, 0);
    QVector2D td_up(0, 0);
    data_up.appendVertex(a_up, b_up, c_up, d_up);
    data_up.appendTexCoord(ta_up, tb_up, tc_up, td_up);
    data_up.appendIndices(0, 1, 2);
    data_up.appendIndices(0, 2, 3);

    // the right hand arrow geometry is same as above, flipped X <-> -X
    data_up.appendGeometry(data_up);
    data_up.texCoord(4).setX(1);
    data_up.texCoord(5).setX(0);
    data_up.texCoord(6).setX(0);
    data_up.texCoord(7).setX(1);
    data_up.appendIndices(4, 5, 6);
    data_up.appendIndices(4, 6, 7);

    m_up->setGeometry(data_up);
    m_up->setStart(6);
    m_up->setCount(6);
    m_up->setOption(QGLSceneNode::CullBoundingBox, false);

    m_down->setGeometry(data_up);
    m_down->setCount(6);
    m_down->setOption(QGLSceneNode::CullBoundingBox, false);

}
