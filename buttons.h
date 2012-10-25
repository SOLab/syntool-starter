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

#ifndef BUTTONS_H
#define BUTTONS_H

#include "qglscenenode.h"
#include <QMouseEvent>
#include "qglpainter.h"

#include <Qt>

class SubButton :public QGLSceneNode
{
    Q_OBJECT
public:
    explicit SubButton(QObject *parent, QSharedPointer<QGLMaterialCollection> palette)
        : QGLSceneNode(parent)
    {
        Q_UNUSED(palette);
    }
    ~SubButton(){}

protected:
    bool event(QEvent *e)
    {
        // Convert the raw event into a signal representing the user's action.
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *me = (QMouseEvent *)e;
            if (me->button() == Qt::LeftButton)
                emit pressed();
        } else if (e->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *me = (QMouseEvent *)e;
            if (me->button() == Qt::LeftButton) {
                emit released();
                if (me->x() >= 0)   // Positive: inside object, Negative: outside.
                    emit clicked();
            }
        } else if (e->type() == QEvent::MouseButtonDblClick) {
            emit doubleClicked();
        } else if (e->type() == QEvent::Enter) {
            m_hovering = true;
            emit hoverChanged();
        } else if (e->type() == QEvent::Leave) {
            m_hovering = false;
            emit hoverChanged();
        }
        return QObject::event(e);
    }
private:
    bool m_hovering;

signals:
    void pressed();
    void released();
    void clicked();
    void doubleClicked();
    void hoverChanged();
};

class Buttons : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit Buttons(QObject *parent,
                     QSharedPointer<QGLMaterialCollection> palette);
    ~Buttons();
    void draw(QGLPainter *painter);
    void clearPositions();
    void createButton();
    SubButton *m_left;
    SubButton *m_right;
    SubButton *m_up;
    SubButton *m_down;

    void createButton(QSharedPointer<QGLMaterialCollection> palette);
private:
    QSize m_size;
    QList<QGLTexture2D*> m_LoadedTextures;
    QList<QGLTexture2D*> m_LoadedTextures_up;
    bool m_hovering;

signals:
    void pressed();
    void released();
    void clicked();
    void doubleClicked();
    void hoverChanged();
};

#endif // BUTTONS_H
