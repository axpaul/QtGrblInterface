#ifndef MOTOR_H
#define MOTOR_H

#define Cmd_end 0x0d // \r in text
#define Cmd_start 0x24 // & in text
#define Cmd_home 0x48 // H in text

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QtDebug>
#include <QDateTime>
#include <QSemaphore>
#include <QMutex>



class Motor : public QThread
{
    Q_OBJECT
public:
    Motor();
    ~Motor();

    void run();
    void movingPosition();
    void movingHome();

public slots:

    void initMotor();
    void closeSerial();

    void setHome();

    void setPosition(const double position);
    void errorSerial();

signals:

    void sendToCmd(QByteArray cmd);
    void motorState(const bool state, const double position);

private:

    int m_positionActu;
    int m_positionAsk;

    bool m_motorRun;
    bool m_errorSerial;
    bool m_homeApply;
    bool m_positionApply;

    QSemaphore *m_semStack;
    QMutex mut;

};

#endif // MOTOR_H
