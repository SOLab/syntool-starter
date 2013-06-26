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
                     QSharedPointer<QGLMaterialCollection> palette);
    ~NavigateButton();
    void draw(QGLPainter *painter, bool drawCoords, qreal lat=-1000, qreal lon=-1000);
    void clearPositions();
    void createButton();
    void drawSector(QVector2D navigateVector, QGLPainter *painter);
    QGLSceneNode* subButton;
    void drawText(QGLPainter *painter, const QString &str, const QPoint &posn);
private:
    QSize m_size;
    QList<QGLTexture2D*> m_LoadedTextures;
    bool m_hovering;

    QGLView* viewPointer;

signals:
    void pressed();
    void released();
    void clicked();
    void doubleClicked();
    void hoverChanged();
};

#endif // NAVIGATEBUTTON_H
