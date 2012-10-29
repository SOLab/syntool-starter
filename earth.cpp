#include "earth.h"
#include "qgltexture2d.h"
#include "qglmaterialcollection.h"

#include <QImage>
#include <QPainter>

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);

    QGLBuilder builder;
    builder.newSection();
    QGLSceneNode *root = builder.currentNode();//  sceneNode();
    QUrl url;

    //set up our materials palette - this describes all
    //of the materials we will use for this scene.

    //solar surface
    QGLMaterial *mat1 = new QGLMaterial;
    url.setPath(QLatin1String(":/earth_6_3.jpg"));
    url.setScheme(QLatin1String("file"));
    mat1->setTextureUrl(url,0);

    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = root->palette()->addMaterial(mat1);

    // create sphere
    builder.pushNode()->setObjectName(QLatin1String("Earth"));
    builder<<QGLSphere(1, 10);

    builder.currentNode()->setMaterialIndex(earthMat);
//    builder.currentNode()->setEffect(QGL::LitModulateTexture2D);
    builder.currentNode()->setEffect(QGL::FlatReplaceTexture2D);

    //create and add rotations for axial tilt and rotation
    earthRotation = new QGraphicsRotation3D();
    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));
    earthRotation->setAngle(0.0f);
    builder.currentNode()->addTransform(earthRotation);
    builder.currentNode()->addTransform(axialTilt1);

     //completed building, so finalise
    QGLSceneNode *n = builder.finalizedSceneNode();
    addNode(n);
}

Earth::~Earth()
{
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
    //    m_texture->cleanupResources();
}

void Earth::drawImage(QGLPainter *glpainter, QSharedPointer<QGLMaterialCollection> materials)
{
    Q_UNUSED(glpainter);
    setPalette(materials);

    QGLBuilder builder;
    builder.newSection();
    QGLSceneNode *root = builder.currentNode();
    QUrl url;

    //set up our materials palette - this describes all
    //of the materials we will use for this scene.

    //solar surface
    QGLMaterial *mat1 = new QGLMaterial;
//    url.setPath(QLatin1String(":/earth.jpg"));
    url.setPath(QLatin1String(":/6.png"));
    url.setScheme(QLatin1String("file"));
    mat1->setTextureUrl(url,0);

    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = root->palette()->addMaterial(mat1);

    // create sphere
    builder.pushNode()->setObjectName(QLatin1String("Earth"));

/////////////////////////
    const int imgSize = 1024;
    const int txtSize = 36;

    QImage uv(imgSize, imgSize, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&uv);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen = painter.pen();
    pen.setWidth(2.0);

    painter.setPen(pen);
    QFont font = painter.font();
    font.setPointSize(txtSize);
    painter.setFont(font);
//////////////////////////

    QGeometryData op;

    QVector3D v0(0.0f, 1.0f, 1.0f);
    QVector3D v1(0.0f, 0.0f, 1.0f);
    QVector3D v2(1.0f, 0.0f, 1.0f);
    QVector3D v3(1.0f, 1.0f, 1.0f);

    QVector2D t0(0.0, 1.0);
    QVector2D t1(0.0, 0.0);
    QVector2D t2(1.0, 0.0);
    QVector2D t3(1.0, 1.0);

    QVector2D tv0 = t0 * static_cast<float>(imgSize);
    QVector2D tv1 = t1 * static_cast<float>(imgSize);
    QVector2D tv2 = t2 * static_cast<float>(imgSize);
    QVector2D tv3 = t3 * static_cast<float>(imgSize);


    painter.setPen(QColor("darkblue"));
    painter.drawLine(tv0.toPointF(), tv1.toPointF());
    painter.drawLine(tv1.toPointF(), tv2.toPointF());
    painter.drawLine(tv2.toPointF(), tv3.toPointF());
    painter.drawLine(tv3.toPointF(), tv0.toPointF());

    op.appendVertex(v0, v1, v2, v3);
    op.appendTexCoord(t0, t1, t2, t3);

    painter.end();
    builder.addQuads(op);
///////////////////////////////////////////
    builder.currentNode()->setMaterialIndex(earthMat);
    builder.currentNode()->setEffect(QGL::LitModulateTexture2D);

//    //create and add rotations for axial tilt and rotation
//    earthRotation = new QGraphicsRotation3D();
//    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
//    axialTilt1->setAngle(270.0f);
//    axialTilt1->setAxis(QVector3D(1,0,0));
//    earthRotation->setAngle(0.0f);
//    builder.currentNode()->addTransform(earthRotation);
//    builder.currentNode()->addTransform(axialTilt1);

     //completed building, so finalise
    QGLSceneNode *n = builder.finalizedSceneNode();
    addNode(n);
}
