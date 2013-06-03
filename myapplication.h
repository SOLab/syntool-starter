#ifndef KEYSINTERCEPTION_H
#define KEYSINTERCEPTION_H

#include <QtGui>
#include <QtCore>
#include "3d/earthview.h"
#include "mainwidget.h"
#include <QKeyEvent>
#include <QApplication>

class myApplication : public QApplication
{
public:
    myApplication(int argc ,char** argv):
    QApplication( argc, argv)
    {
        m_view = NULL;
    }
    ~myApplication()
    {}

    bool notify(QObject* object,QEvent* event)
    {

      //If this is keypress event , do not send it to the receiver
      if(event->type() == QEvent::KeyPress)
      {
          QKeyEvent * key = (QKeyEvent *) event;
          if (m_view)
          {
              switch (key->key())
              {
                  case Qt::Key_Left:
                  case Qt::Key_Right:
                  case Qt::Key_Up:
                  case Qt::Key_Down:
                  case Qt::Key_Plus:
                  case Qt::Key_Minus:
                  {
                      m_view->keyPress(key);
                  }
                  break;
              }
          }

        QApplication::notify(object, event);
        return true;
      }
      //else do the regular processing
      else
      {
          return QApplication::notify(object, event);
      }

    }

void set_view(EarthView *view)
    {
        m_view = view;
    }

protected:
    EarthView* m_view;
};

#endif // KEYSINTERCEPTION_H
