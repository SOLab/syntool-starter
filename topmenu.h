#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QDebug>

#include "more/structure.h"

class TopMenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TopMenuButton(QPushButton *parent = 0) : QPushButton(parent)
    {
        setContentsMargins(0,0,0,0);
        setFixedSize(24,24);

        int size = 16;//style()->pixelMetric(QStyle::PM_ToolBarIconSize);
        QSize iconSize(size, size);
        setIconSize(iconSize);
        setFocusPolicy(Qt::NoFocus);
    }
};

class TopMenu : public QWidget
{
    Q_OBJECT
public:
    explicit TopMenu(QWidget *parent = 0);

protected:
    TopMenuButton* moveButton;
    TopMenuButton* addLineButton;
    TopMenuButton* addRectButton;
    TopMenuButton* addTagButton;
    TopMenuButton* addPinButton;
    TopMenuButton* addShowButton;
    TopMenuButton* showCoordsButton;
    TopMenuButton* addPictureButton;
    TopMenuButton* addGridButton;
    TopMenuButton* TimeLineButton;
    TopMenuButton* PythonScriptButton;
    QHBoxLayout*   topLayout;
    QHBoxLayout*   leftLayout;
    QHBoxLayout*   rightLayout;

private:
    void unSelectAll();

signals:
    void setCursorMode(CursorMode::Mode value);

    void showCoordsAction(bool value);
    void showGridAction(bool value);
    void hideAllAction(bool value);
    void addPictureLayerAction();

public slots:
    void setCheckedButton(bool value);
    void setCursorModeSlot(CursorMode::Mode value);
};

#endif // TOPMENU_H
