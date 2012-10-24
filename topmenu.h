#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QDebug>

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
    TopMenuButton* addLineButton;
    TopMenuButton* addLayerButton;
    TopMenuButton* TimeLineButton;
    TopMenuButton* PythonScriptButton;
    QHBoxLayout* topLayout;
    QHBoxLayout* leftLayout;
    QHBoxLayout* rightLayout;

signals:
    
public slots:
    
};

#endif // TOPMENU_H
