#ifndef TIMELINEPLAYER_H
#define TIMELINEPLAYER_H

#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

class TimeLinePlayer : public QWidget
{
    Q_OBJECT
public:
    explicit TimeLinePlayer(QWidget *parent = 0);
    QSpinBox* playSpeed;
    QPushButton* backPlayButton;
    QPushButton* playButton;
    QPushButton* stopButton;

    bool isPlaying();
    qint32 currentSpeed();
    void setCurrentSpeed(qint32 speedValue);

private:
    QHBoxLayout* hLayout;
    bool m_isPlaying;
    qint32 m_currentSpeed;
    QTimer *timerPlay;
    QTimer *timerBackPlay;
    
signals:
    void addTimeSignal(qint32 minutes);
    void deductTimeSignal(qint32 minutes);
    
public slots:
    void emitAddTime();
    void emitDeductTime();
    void play();
    void backPlay();
    void stop();
    
};

#endif // TIMELINEPLAYER_H
