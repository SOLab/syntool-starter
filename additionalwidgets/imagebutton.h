#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QImageReader>
#include <QIcon>

class ImageButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ImageButton(QString image_name, QString label, QWidget *parent = 0);

protected:
    QVBoxLayout* m_layout;
    QLabel*      image_lbl;
    QString      m_themeName;
    QString      styleSheetStr;

    void setSelected();
    void setUnSelected();

signals:
    
public slots:
    void setCurrentName(QString themeName);
    
};

#endif // IMAGEBUTTON_H
