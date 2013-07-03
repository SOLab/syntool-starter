#ifndef NAVIGATEBUTTON_H
#define NAVIGATEBUTTON_H

#include <qglscenenode.h>
#include <qglpainter.h>
#include <QGLView>

class EarthView;

class NavigateButton : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit NavigateButton(QObject *parent,
                     QSharedPointer<QGLMaterialCollection> palette, float maxScale);
    ~NavigateButton();
    void draw(QGLPainter *painter, float scale, bool drawCoords, qreal lat=-1000, qreal lon=-1000);

    void createButtons();
    void drawSector(QVector2D navigateVector, QGLPainter *painter);
    void drawText(QGLPainter *painter, const QString &str, const QPoint &posn);

    QGLSceneNode* navButton;
    QGLSceneNode* zoomInButton;
    QGLSceneNode* zoomOutButton;
    QGLSceneNode* scaleButton;

private:
    qint32 m_size;
    QList<QGLTexture2D*> m_LoadedTextures;
    bool m_hovering;
    qint32 numberDivisions;
    qint32 m_curDivisions;
    QVector2DArray verts;

    QGLView* viewPointer;
    QGLSceneNode* createButton(QString imageName);

signals:
    void pressed();
    void released();
    void clicked();
    void doubleClicked();
    void hoverChanged();
};

#endif // NAVIGATEBUTTON_H
