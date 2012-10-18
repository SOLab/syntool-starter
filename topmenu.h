#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>

class TopMenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TopMenuButton(QPushButton *parent = 0) : QPushButton(parent)
    {
        setContentsMargins(0,0,0,0);
        setFixedSize(24,24);

        int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
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
    QHBoxLayout* topLayout;

signals:
    
public slots:
    
};

#endif // TOPMENU_H
