#include "timelineplayer.h"

TimeLinePlayer::TimeLinePlayer(QWidget *parent) :
    QWidget(parent)
{
    hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(2);
    hLayout->setAlignment(Qt::AlignRight);

    playSpeed = new QSpinBox(this);
    playSpeed->setRange(0, 1440);
    playSpeed->setValue(720);
    playSpeed->setSingleStep(60);

    backPlayButton = new QPushButton(QIcon(":/icons/backplay_gray.png"),"", this);
    backPlayButton->setFlat(true);
    backPlayButton->setFocusPolicy(Qt::NoFocus);
    backPlayButton->setFixedSize(18,18);
    backPlayButton->setStyleSheet("QPushButton:pressed {\
                                  background-color: #EEEEEE;\
                                  border: 0;\
                                  }");
    connect(backPlayButton, &QPushButton::clicked, this, &TimeLinePlayer::backPlay);

    playButton = new QPushButton(QIcon(":/icons/play_gray.png"),"", this);
    playButton->setFlat(true);
    playButton->setFocusPolicy(Qt::NoFocus);
    playButton->setFixedSize(18,18);
    playButton->setStyleSheet("QPushButton:pressed {\
                                  background-color: #EEEEEE;\
                                  border: 0;\
                                  }");
    connect(playButton, &QPushButton::clicked, this, &TimeLinePlayer::play);

    stopButton = new QPushButton(QIcon(":/icons/stop_gray.png"),"", this);
    stopButton->setFlat(true);
    stopButton->setFocusPolicy(Qt::NoFocus);
    stopButton->setFixedSize(18,18);
    stopButton->setStyleSheet("QPushButton:pressed {\
                                  background-color: #EEEEEE;\
                                  border: 0;\
                                  }");
    connect(stopButton, &QPushButton::clicked, this, &TimeLinePlayer::stop);

    hLayout->addWidget(playSpeed, 10);
    hLayout->addWidget(backPlayButton, 1);
    hLayout->addWidget(playButton, 1);
    hLayout->addWidget(stopButton, 1);

    timerPlay = new QTimer(this);
    connect(timerPlay, &QTimer::timeout, this, &TimeLinePlayer::emitAddTime);

    timerBackPlay = new QTimer(this);
    connect(timerBackPlay, &QTimer::timeout, this, &TimeLinePlayer::emitDeductTime);
}

bool TimeLinePlayer::isPlaying()
{
    return m_isPlaying;
}

void TimeLinePlayer::play()
{
    stop();
    timerPlay->start(500);
}

void TimeLinePlayer::backPlay()
{
    stop();
    timerBackPlay->start(500);
}

void TimeLinePlayer::stop()
{
    timerPlay->stop();
    timerBackPlay->stop();
}

qint32 TimeLinePlayer::currentSpeed()
{
    return m_currentSpeed;
}

void TimeLinePlayer::setCurrentSpeed(qint32 speedValue)
{
    m_currentSpeed = speedValue;
}

void TimeLinePlayer::emitAddTime()
{
    emit addTimeSignal(playSpeed->value());
}

void TimeLinePlayer::emitDeductTime()
{
    emit deductTimeSignal(playSpeed->value());
}
