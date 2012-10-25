#include "navigatebutton.h"

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
    QImage im(":/circle.png");
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
