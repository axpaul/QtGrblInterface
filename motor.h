#ifndef MOTOR_H
#define MOTOR_H

#include <QThread>
#include <QObject>
#include <QWidget>

class Motor : public QThread
{
    Q_OBJECT
public:
    Motor();
    ~Motor();

    void run();

public slots:

    void initMotor();
    void closeSerial();

    void setHome();
    void homeCheck();

    void setPosition(const int position);
    void errorSerial();

signals:

    void sendToCmd(QByteArray cmd);
    void motorState(const bool state, int position);

private:

    int m_positionActu;
    int m_positionAsk;
    bool m_motorRun;

};

#endif // MOTOR_H
