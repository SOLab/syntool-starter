#include "navigatebutton.h"
#include "QVector3DArray"
#include "QGLVertexBundle"
#include <QArray>
#include <qmath.h>

NavigateButton::NavigateButton(QObject *parent, QSharedPointer<QGLMaterialCollection> palette)
    : QGLSceneNode(parent)
{
    setObjectName("Buttons");
    setPalette(palette);
    setOption(QGLSceneNode::CullBoundingBox, false);
    createButton();
}

NavigateButton::~NavigateButton()
{
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
}

void NavigateButton::draw(QGLPainter *painter)
{
    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();

    QRect rect = painter->currentSurface()->viewportRect();
    QMatrix4x4 projm;
    projm.ortho(rect);
    painter->projectionMatrix() = projm;
    painter->modelViewMatrix().setToIdentity();

    if (subButton->position().isNull())
    {
        QVector2D pos(m_size.width() - 10, m_size.height() - 10);
        subButton->setPosition(pos);
    }

    glDisable(GL_DEPTH_TEST);

    QGLSceneNode::draw(painter);

///////////////////////////////////////////////////
//    painter->setStandardEffect(QGL::FlatColor);
//    painter->setColor(QColor(88, 131, 190, 255));

//    QGLVertexBundle vertices1;
//    QVector3DArray positions;
//    positions.append(QVector3D(boundingBox().center().x(),  boundingBox().center().y()-20,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()+8,  boundingBox().center().y()-12,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()-8,  boundingBox().center().y()-12,  0.0f));
//    vertices1.addAttribute(QGL::Position, positions);
//    vertices1.upload();

//    QGLVertexBundle vertices2;
//    positions.clear();
//    positions.append(QVector3D(boundingBox().center().x(),  boundingBox().center().y()+20,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()+8,  boundingBox().center().y()+12,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()-8,  boundingBox().center().y()+12,  0.0f));
//    vertices2.addAttribute(QGL::Position, positions);
//    vertices2.upload();

//    QGLVertexBundle vertices3;
//    positions.clear();
//    positions.append(QVector3D(boundingBox().center().x()+20,  boundingBox().center().y(),  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()+12,  boundingBox().center().y()-8,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()+12,  boundingBox().center().y()+8,  0.0f));
//    vertices3.addAttribute(QGL::Position, positions);
//    vertices3.upload();

//    QGLVertexBundle vertices4;
//    positions.clear();
//    positions.append(QVector3D(boundingBox().center().x()-20,  boundingBox().center().y(),  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()-12,  boundingBox().center().y()-8,  0.0f));
//    positions.append(QVector3D(boundingBox().center().x()-12,  boundingBox().center().y()+8,  0.0f));
//    vertices4.addAttribute(QGL::Position, positions);
//    vertices4.upload();


//    painter->setVertexBundle(vertices1);
//    painter->draw(QGL::Triangles, 3);
//    painter->setVertexBundle(vertices2);
//    painter->draw(QGL::Triangles, 3);
//    painter->setVertexBundle(vertices3);
//    painter->draw(QGL::Triangles, 3);
//    painter->setVertexBundle(vertices4);
//    painter->draw(QGL::Triangles, 3);
////////////////////////////////////////////////////

    glEnable(GL_DEPTH_TEST);

    painter->projectionMatrix().pop();
    painter->modelViewMatrix().pop();
}

void NavigateButton::clearPositions()
{
    subButton->setPosition(QVector3D());
}

void NavigateButton::createButton()
{
    subButton = new QGLSceneNode(this);
    subButton->setObjectName("Left Button");

    QGLMaterial *mat = new QGLMaterial;
    QImage im(":/navigate.png");
    m_size = im.size();
    QGLTexture2D *tex = new QGLTexture2D(mat);
    m_LoadedTextures.push_back(tex);
    tex->setImage(im);
    mat->setTexture(tex);

    setMaterial(mat);
    setEffect(QGL::FlatReplaceTexture2D);

    QGeometryData data;
    QSize f = im.size() / 2;
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

    subButton->setGeometry(data);
    subButton->setCount(6);
    subButton->setOption(QGLSceneNode::CullBoundingBox, false);
}

void NavigateButton::drawSector(QVector2D navigateVector, QGLPainter *painter)
{
    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();

    QRect rect = painter->currentSurface()->viewportRect();
    QMatrix4x4 projm;
    projm.ortho(rect);
    painter->projectionMatrix() = projm;
    painter->modelViewMatrix().setToIdentity();

    glDisable(GL_DEPTH_TEST);

//    QGLSceneNode::draw(painter);

///////////////////////////////////////////////////
    painter->setStandardEffect(QGL::FlatColor);
    int alpha = qSqrt(qPow(navigateVector.x(), 2)+qPow(navigateVector.y(), 2));
//    painter->setColor(QColor(88, 131, 190, alpha*4));
    painter->setColor(QColor(0, 192, 0, alpha*4));

    QVector2DArray vertices;
    QVector3DArray normals;

    QVector3D pos = subButton->position();
    int radius = subButton->boundingBox().maximum().x() - pos.x();

    float a=qAtan(navigateVector.y()/navigateVector.x());

    vertices.append(pos.x(), pos.y());
    int x[5] = {};
    int y[5] = {};
    for (int i= -2; i<3; i++)
    {
        x[i]= (navigateVector.x() >= 0) ? pos.x()-radius*qCos(a+i/6.0) : pos.x()+radius*qCos(a+i/6.0);
        y[i]= (navigateVector.x() >= 0) ? pos.y()-radius*qSin(a+i/6.0) : pos.y()+radius*qSin(a+i/6.0);
        vertices.append(x[i], y[i]);
    }

    normals.append(0.0f, 0.0f, 1.0f);
    normals.append(0.0f, 0.0f, 1.0f);
    normals.append(0.0f, 0.0f, 1.0f);
    normals.append(0.0f, 0.0f, 1.0f);
    normals.append(0.0f, 0.0f, 1.0f);
    normals.append(0.0f, 0.0f, 1.0f);

    painter->clearAttributes();
    painter->setVertexAttribute(QGL::Position, vertices);
    painter->setVertexAttribute(QGL::Normal, normals);
    painter->draw(QGL::TriangleFan, 6);

////////////////////////////////////////////////////

    glEnable(GL_DEPTH_TEST);

    painter->projectionMatrix().pop();
    painter->modelViewMatrix().pop();
}
