#include "earth.h"
#include "qgltexture2d.h"
#include "qglmaterialcollection.h"
#include "qmath.h"
#include <QGLDome>

#include <QImage>
#include <QPainter>
#include <qgl.h>

inline double mercator(double x) {
//    x = (M_PI / (double)(180 * x));
    return 0.5*log((1.0+sin(x))/(1.0-sin(x)));
}

double Mercator2Sphere( double iTY, const double maxAng = 85.0 )
{
    static std::vector <double> vec;
    if (!vec.size()) {
        vec.push_back(0.0);
        double unit = M_PI / 180;
        for (double i = 1; i < maxAng; ++i)
            vec.push_back(vec[i - 1] + unit / cos(i * unit));

        // normalize
        double scale = M_PI_2 / vec.back();
        for (size_t i = 0; i < vec.size(); ++i)
            vec[i] *= scale;
    }

    double angle = (iTY * 2 - 1) * M_PI_2;		// texture V to angle
    double angle_degree = fabs(angle) * 180 / M_PI;
    int beg = (int) angle_degree;
    double val;
    if (beg >= (int) vec.size() - 1)
        val = M_PI_2;
    else {
        double w = angle_degree - beg;
        val = vec[beg] * (1.0 - w) + vec[beg + 1] * w;
    }
    if (angle < 0.0) val = -val;
    return (1 + val / M_PI_2) * 0.5;	// angle to texture V
}

Earth::Earth(QObject *parent, QSharedPointer<QGLMaterialCollection> materials)
    : QGLSceneNode(parent)
    , m_texture(0)
{
    setPalette(materials);

    sphere = buildGeometry(0.5, 10);
    sphere->setObjectName("Earth");


    QGLTexture2D * tex;
    tex = new QGLTexture2D();
    tex->setSize(QSize(512, 256));

    // Equality test of images can be very expensive, so always assign the
    // value and emit effect changed
    QUrl url;
//    url.setPath(QLatin1String(":/earth_6_3.jpg"));
    url.setPath(QLatin1String(":/zoom0.png"));
    url.setScheme(QLatin1String("file"));
    tex->setUrl(url);

    // prevents artifacts due to texture smoothing wrapping around edges of texture
//    tex->setHorizontalWrap(QGL::ClampToEdge);
//    tex->setVerticalWrap(QGL::ClampToEdge);

    QGLMaterial *mat1 = new QGLMaterial;
    mat1->setTexture(tex, 0);

//    QString searchPath("Earth::part1");
//    QGLSceneNode* temp = findSceneNode(searchPath);

//    qDebug() << sn1;
//    qDebug() << this;

    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = sphere->palette()->addMaterial(mat1);

    sphere->setMaterialIndex(earthMat);
    sphere->setEffect(QGL::LitModulateTexture2D);


//////////////////////////////////////////////////////////

//    QGLTexture2D * tex2;
//    tex2 = new QGLTexture2D();

//    QUrl url2;
//    url2.setPath(QLatin1String(":/0-0.png"));
//    url2.setScheme(QLatin1String("file"));
//    tex2->setUrl(url2);

//    tex2->setSize(QSize(128,256));

//    QGLMaterial *mat2 = new QGLMaterial;
//    mat2->setTexture(tex2, 0);

////    QString searchPath2("Earth::part2");
////    QGLSceneNode* temp2 = findSceneNode(searchPath2);

////    qDebug() << temp2;
////    qDebug() << this;

//    m_LoadedTextures.push_back(mat2->texture(0));
//    int earthMat2 = sn2->palette()->addMaterial(mat2);

//    sn2->setMaterialIndex(earthMat2);
//    sn2->setEffect(QGL::LitModulateTexture2D);
//////////////////////////////////////////////////////////

//    sphere->setMaterialIndex(earthMat);
//    sphere->setEffect(QGL::LitModulateTexture2D);

    earthRotation = new QGraphicsRotation3D();
    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));
    earthRotation->setAngle(0.0f);

    addTransform(earthRotation);
    addTransform(axialTilt1);
//    sphere->addTransform(earthRotation);
//    sphere->addTransform(axialTilt1);
    addNode(sphere);

//    this->removeNode(sphere);
//    delete sphere;

}

QGLSceneNode *Earth::buildGeometry(qreal radius, int divisions)
{
    QGLBuilder builder;
    // Determine the number of slices and stacks to generate.
    static int const slicesAndStacks[] = {
        8, 4,
        8, 8,
        16, 8,
        16, 16,
        32, 16,
        32, 32,
        64, 32,
        64, 64,
        128, 64,
        128, 128
    };

    if (divisions < 1)
        divisions = 1;
    else if (divisions > 10)
        divisions = 10;
    int stacks = slicesAndStacks[divisions * 2 - 1];
    int slices = slicesAndStacks[divisions * 2 - 2];

    // Precompute sin/cos values for the slices and stacks.
    const int maxSlices = 128 + 1;
    const int maxStacks = 128 + 1;
    qreal sliceSin[maxSlices];
    qreal sliceCos[maxSlices];
    qreal stackSin[maxStacks];
    qreal stackCos[maxStacks];
    for (int slice = 0; slice < slices; ++slice) {
        qreal angle = 2 * M_PI * slice / slices;
        sliceSin[slice] = qFastSin(angle);
        sliceCos[slice] = qFastCos(angle);
    }
    sliceSin[slices] = sliceSin[0]; // Join first and last slice.
    sliceCos[slices] = sliceCos[0];
    for (int stack = 0; stack <= stacks; ++stack) {
        qreal angle = M_PI * stack / stacks;
        stackSin[stack] = qFastSin(angle);
        stackCos[stack] = qFastCos(angle);
    }
    stackSin[0] = 0.0f;             // Come to a point at the poles.
    stackSin[stacks] = 0.0f;

    qDebug() << stacks;
//    stacks = 8;
    // Create the stacks.
    for (int stack = 0; stack < stacks; ++stack) {
        QGeometryData prim;
        qreal z = radius * stackCos[stack];
        qreal nextz = radius * stackCos[stack + 1];
        qreal s = stackSin[stack];
        qreal nexts = stackSin[stack + 1];
        qreal c = stackCos[stack];
        qreal nextc = stackCos[stack + 1];
        qreal r = radius * s;
        qreal nextr = radius * nexts;

        double yTexCoord;
        double yTexCoordNext;
        for (int slice = 0; slice <= slices; ++slice) {
            prim.appendVertex
                (QVector3D(nextr * sliceSin[slice],
                           nextr * sliceCos[slice], nextz));
            prim.appendNormal
                (QVector3D(sliceSin[slice] * nexts,
                           sliceCos[slice] * nexts, nextc));
            yTexCoordNext = 1.0f - qreal(stack + 1) / stacks;

            if (slice == 32)
                qDebug() << yTexCoordNext;
            yTexCoordNext = Mercator2Sphere(yTexCoordNext);
//            yTexCoordNext = mercator((yTexCoordNext - 0.5) * 2)/2 + 0.5;
            if (slice == 32)
                qDebug() << "==>" << yTexCoordNext;

//            yTexCoordNext = mercator((yTexCoordNext - 0.5)*M_PI);
//            if (slice == 32)
//                qDebug() << "==>" << yTexCoordNext;

            prim.appendTexCoord
                (QVector2D(1.0f - qreal(slice) / slices,
                           yTexCoordNext));

            prim.appendVertex
                (QVector3D(r * sliceSin[slice],
                           r * sliceCos[slice], z));
            prim.appendNormal
                (QVector3D(sliceSin[slice] * s,
                           sliceCos[slice] * s, c));
            yTexCoord = 1.0f - qreal(stack) / stacks;
            yTexCoord = Mercator2Sphere(yTexCoord);
            prim.appendTexCoord
                (QVector2D(1.0f - qreal(slice) / slices,
                           yTexCoord));
        }
        builder.addQuadStrip(prim);
    }
    return builder.finalizedSceneNode();
}

//QGLSceneNode *Earth::buildGeometry2(qreal radius, int divisions)
//{
//    QGLBuilder builder;
//    // Determine the number of slices and stacks to generate.
//    static int const slicesAndStacks[] = {
//        8, 4,
//        8, 8,
//        16, 8,
//        16, 16,
//        32, 16,
//        32, 32,
//        64, 32,
//        64, 64,
//        128, 64,
//        128, 128
//    };

//    if (divisions < 1)
//        divisions = 1;
//    else if (divisions > 10)
//        divisions = 10;
//    int stacks = slicesAndStacks[divisions * 2 - 1];
//    int slices = slicesAndStacks[divisions * 2 - 2];

//    // Precompute sin/cos values for the slices and stacks.
//    const int maxSlices = 128 + 1;
//    const int maxStacks = 128 + 1;
//    qreal sliceSin[maxSlices];
//    qreal sliceCos[maxSlices];
//    qreal stackSin[maxStacks];
//    qreal stackCos[maxStacks];
//    for (int slice = 0; slice < slices; ++slice) {
//        qreal angle = 2 * M_PI * slice / slices;
//        sliceSin[slice] = qFastSin(angle);
//        sliceCos[slice] = qFastCos(angle);
//    }
//    sliceSin[slices] = sliceSin[0]; // Join first and last slice.
//    sliceCos[slices] = sliceCos[0];
//    for (int stack = 0; stack <= stacks; ++stack) {
//        qreal angle = M_PI * stack / stacks;
//        stackSin[stack] = qFastSin(angle);
//        stackCos[stack] = qFastCos(angle);
//    }
//    stackSin[0] = 0.0f;             // Come to a point at the poles.
//    stackSin[stacks] = 0.0f;

//    qDebug() << stacks;
////    stacks = 8;
//    // Create the stacks.
//    for (int stack = 0; stack < stacks; ++stack) {
//        QGeometryData prim;
//        qreal z = radius * stackCos[stack];
//        qreal nextz = radius * stackCos[stack + 1];
//        qreal s = stackSin[stack];
//        qreal nexts = stackSin[stack + 1];
//        qreal c = stackCos[stack];
//        qreal nextc = stackCos[stack + 1];
//        qreal r = radius * s;
//        qreal nextr = radius * nexts;
//        for (int slice = 0; slice <= slices; ++slice) {
//            prim.appendVertex
//                (QVector3D(nextr * sliceSin[slice],
//                           nextr * sliceCos[slice], nextz));
//            prim.appendNormal
//                (QVector3D(sliceSin[slice] * nexts,
//                           sliceCos[slice] * nexts, nextc));
//            prim.appendTexCoord
//                (QVector2D(1.0f - qreal(slice) / slices,
//                           1.0f - qreal(stack + 1) / stacks));

//            prim.appendVertex
//                (QVector3D(r * sliceSin[slice],
//                           r * sliceCos[slice], z));
//            prim.appendNormal
//                (QVector3D(sliceSin[slice] * s,
//                           sliceCos[slice] * s, c));
//            prim.appendTexCoord
//                (QVector2D(1.0f - qreal(slice) / slices,
//                           1.0f - qreal(stack) / stacks));
//        }
//        builder.addQuadStrip(prim);
//    }
//    return builder.finalizedSceneNode();
//}

//QGLSceneNode *Earth::buildGeometry2(qreal radius, int divisions)
//{
////    QGLBuilder builder;
//    QGLBuilder builder1;
//    QGLBuilder builder2;
//    QGLBuilder builder3;
//    QGLBuilder builder4;

////    builder.newNode();
//    //QGLSceneNode* all = builder.currentNode();
//    // Determine the number of slices and stacks to generate.
//    static int const slicesAndStacks[] = {
//        8, 4,
//        8, 8,
//        16, 8,
//        16, 16,
//        32, 16,
//        32, 32,
//        64, 32,
//        64, 64,
//        128, 64,
//        128, 128
//    };

//    if (divisions < 1)
//        divisions = 1;
//    else if (divisions > 10)
//        divisions = 10;
//    int stacks = slicesAndStacks[divisions * 2 - 1];
//    int slices = slicesAndStacks[divisions * 2 - 2];

//    // Precompute sin/cos values for the slices and stacks.
//    const int maxSlices = 128 + 1;
//    const int maxStacks = 128 + 1;
//    qreal sliceSin[maxSlices];
//    qreal sliceCos[maxSlices];
//    qreal stackSin[maxStacks];
//    qreal stackCos[maxStacks];
//    for (int slice = 0; slice < slices; ++slice) {
//        qreal angle = 2 * M_PI * slice / slices;
//        sliceSin[slice] = qFastSin(angle);
//        sliceCos[slice] = qFastCos(angle);
//    }
//    sliceSin[slices] = sliceSin[0]; // Join first and last slice.
//    sliceCos[slices] = sliceCos[0];
//    for (int stack = 0; stack <= stacks; ++stack) {
//        qreal angle = M_PI * stack / stacks;
//        stackSin[stack] = qFastSin(angle);
//        stackCos[stack] = qFastCos(angle);
//    }
//    stackSin[0] = 0.0f;             // Come to a point at the poles.
//    stackSin[stacks] = 0.0f;

//    qDebug() << stacks;
////    stacks = 8;
//    // Create the stacks.
//    for (int stack = 0; stack < stacks/2; ++stack) {
//        QGeometryData prim;
//        qreal z = radius * stackCos[stack];
//        qreal nextz = radius * stackCos[stack + 1];
//        qreal s = stackSin[stack];
//        qreal nexts = stackSin[stack + 1];
//        qreal c = stackCos[stack];
//        qreal nextc = stackCos[stack + 1];
//        qreal r = radius * s;
//        qreal nextr = radius * nexts;
//        for (int slice = 0; slice <= slices/2; ++slice) {
//            prim.appendVertex
//                (QVector3D(nextr * sliceSin[slice],
//                           nextr * sliceCos[slice], nextz));
//            prim.appendNormal
//                (QVector3D(2* (sliceSin[slice] * nexts),
//                           2* (sliceCos[slice] * nexts), 2* nextc));
//            prim.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack + 1) / stacks)));

//            prim.appendVertex
//                (QVector3D(r * sliceSin[slice],
//                           r * sliceCos[slice], z));
//            prim.appendNormal
//                (QVector3D(2* (sliceSin[slice] * s),
//                           2* (sliceCos[slice] * s), 2* c));
//            prim.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack) / stacks)));
//        }

//        QGeometryData prim2;
//        for (int slice = slices/2; slice <= slices; ++slice) {
//            prim2.appendVertex
//                (QVector3D(nextr * sliceSin[slice],
//                           nextr * sliceCos[slice], nextz));
//            prim2.appendNormal
//                (QVector3D(2* (sliceSin[slice] * nexts),
//                           2* (sliceCos[slice] * nexts), 2* nextc));
//            prim2.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack + 1) / stacks)));

//            prim2.appendVertex
//                (QVector3D(r * sliceSin[slice],
//                           r * sliceCos[slice], z));
//            prim2.appendNormal
//                (QVector3D(2* (sliceSin[slice] * s),
//                           2* (sliceCos[slice] * s), 2* c));
//            prim2.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack) / stacks)));
//        }

////        builder1.newNode();
//        builder1.addQuadStrip(prim);
////        all->addNode(one);

////        builder2.newNode();
//        builder2.addQuadStrip(prim2);
////        all->addNode(two);
//    }
//    sn1 = builder1.finalizedSceneNode();
//    sn1->setObjectName("part1");
//    addNode(sn1);

//    sn2 = builder2.finalizedSceneNode();
//    sn2->setObjectName("part2");
//    addNode(sn2);

//    qDebug() << 2222222222;

//    for (int stack = stacks/2; stack < stacks; ++stack) {
//        QGeometryData prim3;
//        qreal z = radius * stackCos[stack];
//        qreal nextz = radius * stackCos[stack + 1];
//        qreal s = stackSin[stack];
//        qreal nexts = stackSin[stack + 1];
//        qreal c = stackCos[stack];
//        qreal nextc = stackCos[stack + 1];
//        qreal r = radius * s;
//        qreal nextr = radius * nexts;
//        for (int slice = 0; slice <= slices/2; ++slice) {
//            prim3.appendVertex
//                (QVector3D(nextr * sliceSin[slice],
//                           nextr * sliceCos[slice], nextz));
//            prim3.appendNormal
//                (QVector3D(2* (sliceSin[slice] * nexts),
//                           2* (sliceCos[slice] * nexts), 2* nextc));
//            prim3.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack + 1) / stacks)));

//            prim3.appendVertex
//                (QVector3D(r * sliceSin[slice],
//                           r * sliceCos[slice], z));
//            prim3.appendNormal
//                (QVector3D(2* (sliceSin[slice] * s),
//                           2* (sliceCos[slice] * s), 2* c));
//            prim3.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack) / stacks)));
//        }

//        QGeometryData prim4;
//        for (int slice = slices/2; slice <= slices; ++slice) {
//            prim4.appendVertex
//                (QVector3D(nextr * sliceSin[slice],
//                           nextr * sliceCos[slice], nextz));
//            prim4.appendNormal
//                (QVector3D(2* (sliceSin[slice] * nexts),
//                           2* (sliceCos[slice] * nexts), 2* nextc));
//            prim4.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack + 1) / stacks)));

//            prim4.appendVertex
//                (QVector3D(r * sliceSin[slice],
//                           r * sliceCos[slice], z));
//            prim4.appendNormal
//                (QVector3D(2* (sliceSin[slice] * s),
//                           2* (sliceCos[slice] * s), 2* c));
//            prim4.appendTexCoord
//                (QVector2D(2* (1.0f - qreal(slice) / slices),
//                           2* (1.0f - qreal(stack) / stacks)));
//        }
//        builder3.addQuadStrip(prim3);

//        builder4.addQuadStrip(prim4);
//    }

//    sn3 = builder3.finalizedSceneNode();
//    sn3->setObjectName("part3");
//    addNode(sn3);

//    sn4 = builder4.finalizedSceneNode();
//    sn4->setObjectName("part4");
//    addNode(sn4);

////    builder.finalizedSceneNode();
//    qDebug() << 3333333333;
//    return sn4;
////    return builder.finalizedSceneNode();
////    return builder.finalizedSceneNode();
//}

Earth::~Earth()
{
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
    //    m_texture->cleanupResources();
}

//void Earth::drawImage(QGLPainter *glpainter)
//{
//    Q_UNUSED(glpainter);
////    setPalette(materials);

//    QGLBuilder builder;
//    builder.newSection();
//    QGLSceneNode *root = builder.currentNode();
//    QUrl url;

//    //set up our materials palette - this describes all
//    //of the materials we will use for this scene.

//    //solar surface
//    QGLMaterial *mat1 = new QGLMaterial;
////    url.setPath(QLatin1String(":/earth.jpg"));
//    url.setPath(QLatin1String(":/6.png"));
//    url.setScheme(QLatin1String("file"));
//    mat1->setTextureUrl(url,0);

//    m_LoadedTextures.push_back(mat1->texture(0));
//    int earthMat = root->palette()->addMaterial(mat1);

//    // create sphere
//    builder.pushNode()->setObjectName(QLatin1String("Earth"));

///////////////////////////
//    const int imgSize = 1024;
//    const int txtSize = 36;

//    QImage uv(imgSize, imgSize, QImage::Format_ARGB32);
//    QPainter painter;
//    painter.begin(&uv);
//    painter.setRenderHint(QPainter::Antialiasing);
//    QPen pen = painter.pen();
//    pen.setWidth(2.0);

//    painter.setPen(pen);
//    QFont font = painter.font();
//    font.setPointSize(txtSize);
//    painter.setFont(font);
////////////////////////////

//    QGeometryData op;

//    QVector3D v0(0.0f, 1.0f, 1.0f);
//    QVector3D v1(0.0f, 0.0f, 1.0f);
//    QVector3D v2(1.0f, 0.0f, 1.0f);
//    QVector3D v3(1.0f, 1.0f, 1.0f);

//    QVector2D t0(0.0, 1.0);
//    QVector2D t1(0.0, 0.0);
//    QVector2D t2(1.0, 0.0);
//    QVector2D t3(1.0, 1.0);
////    float dx = 0.2f;
////    float dy = 0.2f;
////    float dz1 = -0.4f;
////    float dz2 = -0.9f;

////    QVector3D v0(0.0f+dx, 0.0f+dy, 1.0f+dz1+0.1);
////    QVector3D v1(0.3f+dx, 0.0f+dy, 1.0f+dz1);
////    QVector3D v2(0.3f+dx, 0.3f+dy, 1.0f+dz2);
////    QVector3D v3(0.0f+dx, 0.3f+dy, 1.0f+dz2);

//    QVector2D tv0 = t0 * static_cast<float>(imgSize);
//    QVector2D tv1 = t1 * static_cast<float>(imgSize);
//    QVector2D tv2 = t2 * static_cast<float>(imgSize);
//    QVector2D tv3 = t3 * static_cast<float>(imgSize);


//    painter.setPen(QColor("darkblue"));
//    painter.drawLine(tv0.toPointF(), tv1.toPointF());
//    painter.drawLine(tv1.toPointF(), tv2.toPointF());
//    painter.drawLine(tv2.toPointF(), tv3.toPointF());
//    painter.drawLine(tv3.toPointF(), tv0.toPointF());

//    op.appendVertex(v0, v1, v2, v3);
//    op.appendTexCoord(t0, t1, t2, t3);

//    painter.end();
//    builder.addQuads(op);
/////////////////////////////////////////////
//    builder.currentNode()->setMaterialIndex(earthMat);
//    builder.currentNode()->setEffect(QGL::LitModulateTexture2D);

////    //create and add rotations for axial tilt and rotation
////    earthRotation = new QGraphicsRotation3D();
////    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
////    axialTilt1->setAngle(270.0f);
////    axialTilt1->setAxis(QVector3D(1,0,0));
////    earthRotation->setAngle(0.0f);
////    builder.currentNode()->addTransform(earthRotation);
////    builder.currentNode()->addTransform(axialTilt1);

//     //completed building, so finalise
//    QGLSceneNode *n = builder.finalizedSceneNode();
//    addNode(n);
//}

void Earth::changeTexture(QGLPainter *painter, qreal zoom)
{
//    if (zoom > 1)
//    {
//    QGLMaterial *mat1 = new QGLMaterial;
//    QUrl url;
//    url.setPath(QLatin1String(":/earth.jpg"));
//    url.setScheme(QLatin1String("file"));
//    mat1->setTextureUrl(url,0);

//    m_LoadedTextures.push_back(mat1->texture(0));
//    int earthMat = sphere->palette()->addMaterial(mat1);

//    sphere->setMaterialIndex(earthMat);
//    sphere->setEffect(QGL::LitModulateTexture2D);
//    }
//    else
//    {
//        QGLMaterial *mat1 = new QGLMaterial;
//        QUrl url;
//        url.setPath(QLatin1String(":/earth_6_3.jpg"));
//        url.setScheme(QLatin1String("file"));
//        mat1->setTextureUrl(url,0);

//        m_LoadedTextures.push_back(mat1->texture(0));
//        int earthMat = sphere->palette()->addMaterial(mat1);

//        sphere->setMaterialIndex(earthMat);
//        sphere->setEffect(QGL::LitModulateTexture2D);
//    }
}
