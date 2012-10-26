#ifndef NAVIGATEBUTTON_H
#define NAVIGATEBUTTON_H

#include <qglscenenode.h>
#include <qglpainter.h>

class EarthView;

class NavigateButton : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit NavigateButton(QObject *parent,
                     QSharedPointer<QGLMaterialCollection> palette);
    ~NavigateButton();
    void draw(QGLPainter *painter);
    void clearPositions();
    void createButton();
    void drawSector(QVector2D navigateVector, QGLPainter *painter);
    QGLSceneNode* subButton;
private:
    QSize m_size;
    QList<QGLTexture2D*> m_LoadedTextures;
    bool m_hovering;

signals:
    void pressed();
    void released();
    void clicked();
    void doubleClicked();
    void hoverChanged();
};

#endif // NAVIGATEBUTTON_H
