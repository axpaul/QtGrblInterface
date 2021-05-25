#include "motor.h"

Motor::Motor()
{

}

Motor::~Motor()
{
    delete m_semStack;
}

void Motor::run(){

    m_motorRun = false;
    m_homeApply = false;
    m_positionApply = false;
    m_errorSerial = false;

    m_semStack = new QSemaphore(0);

    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MOTOR] " << QThread::currentThread();

    while(1){
        if(m_motorRun)
        {
            while(m_motorRun && !m_errorSerial){

               if(m_semStack->tryAcquire(1)){

                   mut.lock();

                   if (m_homeApply){
                       movingHome();
                   }
                   else if (m_positionApply){
                       movingPosition();
                   }

                   mut.unlock();
               }
            }
        }
        else
            QThread::usleep(100);

   }
}

void Motor::initMotor(){
    m_positionActu = 0;
    m_positionApply = 0;  

}

void Motor::setHome()
{
    m_homeApply = true;
    m_semStack->available();
}

void Motor::setPosition(const double position)
{
    m_positionApply = true;
    m_positionAsk = position;
    m_semStack->available();
}

void Motor::errorSerial()
{
    m_errorSerial = true;
    m_motorRun = false;
    m_semStack->available();
}

void Motor::closeSerial()
{
    m_motorRun = false;
    m_semStack->available();
}

void Motor::movingPosition()
{
    QByteArray cmd;
    double move;

    move = m_positionActu;

    if (m_positionActu > m_positionAsk){
        cmd.resize(12);
        cmd[0] = Cmd_gcode;
        cmd[1] = Cmd_echelle_nb2;
        cmd[2] = Cmd_echelle_nb1;
        cmd[3] = Cmd_gcode;
        cmd[4] = Cmd_position_nb9;
        cmd[5] = Cmd_position_nb0;
        cmd[6] = Cmd_position_X;
        cmd[7] = Cmd_neg;
        cmd[8] = Cmd_position_nb0;
        cmd[9] = Cmd_point;
        cmd[10] = Cmd_position_nb1;
        cmd[11] = Cmd_end;

        do{
            move+= 0.1;
            emit sendToCmd(cmd);
            QThread::usleep(100);
            m_positionActu = move;
            emit motorState(true, m_positionActu);

        }while(m_positionAsk >= move);
     emit motorState(false, m_positionActu);

    }
    else if (m_positionActu < m_positionAsk){
        cmd.resize(11);
        cmd[0] = Cmd_gcode;
        cmd[1] = Cmd_echelle_nb2;
        cmd[2] = Cmd_echelle_nb1;
        cmd[3] = Cmd_gcode;
        cmd[4] = Cmd_position_nb9;
        cmd[5] = Cmd_position_nb0;
        cmd[6] = Cmd_position_X;
        cmd[7] = Cmd_position_nb0;
        cmd[8] = Cmd_point;
        cmd[9] = Cmd_position_nb1;
        cmd[10] = Cmd_end;

        do{
            move-= 0.1;
            emit sendToCmd(cmd);
            QThread::usleep(100);
            m_positionActu = move;
            emit motorState(true, m_positionActu);
        }while(m_positionAsk <= move);

        emit motorState(false, m_positionActu);
    }
    else{
        emit motorState(false, m_positionActu);
    }

}

void Motor::movingHome()
{

    QByteArray cmd;
        cmd.resize(3);
        cmd[0] = Cmd_start;
        cmd[1] = Cmd_home_H;
        cmd[2] = Cmd_end;
    emit sendToCmd(cmd);

}
