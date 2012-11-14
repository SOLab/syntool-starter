#include "earth.h"
#include "qgltexture2d.h"
#include "qglmaterialcollection.h"
#include "qmath.h"
#include <QGLDome>

#include <QImage>
#include <QPainter>
#include <qgl.h>
#include "lightmaps.h"

inline double mercator(double x) {
    return 0.5*log((1.0+sin(x))/(1.0-sin(x)));
}

const double defMercAngle = 85 * M_PI / 180;
const double defMercScale = M_PI_2 / mercator(defMercAngle);

double Mercator2SphereAnalytic(double iTY, const double scale = defMercScale,
                               const double maxAng = defMercAngle)
{
    double angle = (iTY * 2 - 1) * M_PI_2;		// texture V to angle
        double angle2 = fabs(angle);
    double val = (angle2 > maxAng) ? M_PI_2 : (mercator(angle2) * scale);
    if (angle < 0.0) val = -val;
    return (1 + val / M_PI_2) * 0.5;	// angle to texture V
}

double Mercator2Sphere( double iTY, const double maxAng = 85.0 )
{
    static std::vector <double> vec;
    if (!vec.size()) {
        vec.push_back(0.0);
        double unit = M_PI / 180;
        for (double i = 1; i < maxAng; ++i)
//            vec.push_back(vec[i - 1] + unit / cos(i * unit));
            vec.push_back(mercator(i * unit));

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

    sphere = buildEarthNode(0.5, 10, 0);
    sphere->setObjectName("Earth");

    QGLTexture2D * tex;
    tex = new QGLTexture2D();
    tex->setSize(QSize(512, 256));

    QUrl url;
    url.setPath(QLatin1String(":/zoom0.png"));
    url.setScheme(QLatin1String("file"));
    tex->setUrl(url);

    QGLMaterial *mat1 = new QGLMaterial;
    mat1->setTexture(tex, 0);

    m_LoadedTextures.push_back(mat1->texture(0));
    int earthMat = sphere->palette()->addMaterial(mat1);

    sphere->setMaterialIndex(earthMat);
    sphere->setEffect(QGL::LitModulateTexture2D);

    earthRotation = new QGraphicsRotation3D();
    QGraphicsRotation3D *axialTilt1 = new QGraphicsRotation3D();
    axialTilt1->setAngle(270.0f);
    axialTilt1->setAxis(QVector3D(1,0,0));
    earthRotation->setAngle(0.0f);

    addTransform(earthRotation);
    addTransform(axialTilt1);
    addNode(sphere);

    zoom = 1;
}

QGLSceneNode *Earth::buildEarthNode(qreal radius, int divisions, int cur_zoom)
{
    Q_UNUSED(cur_zoom);
//    divisions = 1;

    int separation = qPow(2, cur_zoom);
    qDebug() << "=====>" << separation;
    if (separation > 2)
        separation = 2;

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

    int stacks_part = stacks / separation;
    int cur_stacks_part = stacks_part;

    int stack_num, slice_num;

    for (; cur_stacks_part <= stacks; cur_stacks_part += stacks_part)
    {
        qDebug() << "=>" << cur_stacks_part;
        qDebug() << "stacks" << stacks;
        qDebug() << "slices" << slices;
        // Create the stacks.
        int slices_part = slices / separation;
        int cur_slices_part = slices_part;

        for (; cur_slices_part <= slices; cur_slices_part += slices_part)
        {
            QGLBuilder tempBuilder;
            for (int stack = cur_stacks_part - stacks_part; stack < cur_stacks_part; ++stack) {
//                qDebug() << "stack === "<<stack;
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
                double xTexCoord;
                double xTexCoordNext;

                prim.clear();
                for (int slice = cur_slices_part - slices_part; slice <= cur_slices_part; ++slice) {
//                    qDebug() << "slice"<<slice;
                    prim.appendVertex
                        (QVector3D(nextr * sliceSin[slice],
                                   nextr * sliceCos[slice], nextz));
                    prim.appendNormal
                        (QVector3D(sliceSin[slice] * nexts,
                                   sliceCos[slice] * nexts, nextc));

                    yTexCoordNext = 1.0f - qreal(stack + 1) / stacks;
                    yTexCoordNext = Mercator2SphereAnalytic(yTexCoordNext)*separation;
                    xTexCoordNext = (1.0f - qreal(slice) / slices)*separation;

                    prim.appendTexCoord
                        (QVector2D(xTexCoordNext,
                                   yTexCoordNext));

                    prim.appendVertex
                        (QVector3D(r * sliceSin[slice],
                                   r * sliceCos[slice], z));
                    prim.appendNormal
                        (QVector3D(sliceSin[slice] * s,
                                   sliceCos[slice] * s, c));

                    yTexCoord = 1.0f - qreal(stack) / stacks;
                    yTexCoord = Mercator2SphereAnalytic(yTexCoord)*separation;
                    xTexCoord = (1.0f - qreal(slice) / slices)*separation;

                    prim.appendTexCoord
                        (QVector2D(xTexCoord,
                                   yTexCoord));
                }

                tempBuilder.addQuadStrip(prim);
            }
            stack_num = cur_stacks_part / stacks_part - 1;
            slice_num = cur_slices_part / slices_part - 1;
            qDebug() << "y" << stack_num << "x" << slice_num;

            QGLSceneNode* tempNode = tempBuilder.finalizedSceneNode();
            if (separation > 1){
            QGLTexture2D* tex;
            tex = new QGLTexture2D();
            tex->setSize(QSize(512, 256));

            QUrl url;
            QString path = ":"+QString::number(separation-slice_num-1)+"-"+QString::number(qAbs(stack_num))+".png";
            url.setPath(path);
            url.setScheme(QLatin1String("file"));
            tex->setUrl(url);

            QGLMaterial *mat1 = new QGLMaterial;
            mat1->setTexture(tex, 0);

            m_LoadedTextures.push_back(mat1->texture(0));
            int earthMat = tempNode->palette()->addMaterial(mat1);

            tempNode->setMaterialIndex(earthMat);
            tempNode->setEffect(QGL::LitModulateTexture2D);
            }

            builder.sceneNode()->addNode(tempNode);
        }
    }
    return builder.finalizedSceneNode();
}

Earth::~Earth()
{
    for (int i=0; i<m_LoadedTextures.count(); ++i) {
        m_LoadedTextures.at(i)->cleanupResources();
    }
        m_texture->cleanupResources();
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

void Earth::changeTexture(qreal cur_zoom)
{
//    qDebug() << "changeTexture, zoom = " << cur_zoom;
    if (zoom != (int) floor(cur_zoom + 0.5))
    {
        zoom = (int) floor(cur_zoom + 0.5);

//        qDebug() << "zoom changed!!! zoom = " << cur_zoom;
        QString search_path = QString("Earth");
        if (this->findSceneNode(search_path))
        {
            removeNode(sphere);
//            delete sphere;

            sphere = buildEarthNode(0.5, 10, cur_zoom);
            sphere->setObjectName("Earth");

            QGLTexture2D * tex;
            tex = new QGLTexture2D();
            tex->setSize(QSize(512, 256));

            QUrl url;
            url.setPath(QLatin1String(":/zoom0.png"));
            url.setScheme(QLatin1String("file"));
            tex->setUrl(url);

            QGLMaterial *mat1 = new QGLMaterial;
            mat1->setTexture(tex, 0);

            m_LoadedTextures.push_back(mat1->texture(0));
            int earthMat = sphere->palette()->addMaterial(mat1);

            sphere->setMaterialIndex(earthMat);
            sphere->setEffect(QGL::LitModulateTexture2D);

//            QGraphicsRotation3D *circular_rotate = new QGraphicsRotation3D();
//            circular_rotate->setAngle(180.0f);
//            circular_rotate->setAxis(QVector3D(0,1,0));

//            sphere->addTransform(circular_rotate);

            addNode(sphere);
        }

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
}
