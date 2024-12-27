//
// Created by a5192 on 2023/12/4.
//

#ifndef QT_TEMPLATE_APPLY_H
#define QT_TEMPLATE_APPLY_H
#include "QString"
#include "QDateTime"
class Apply
{
public:
    Apply(QString info,QString id):
    time(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")),
    applyInfo(info),
    examineState(0),
    ID(id){}

    Apply(QString Time,QString Info,QString ID, int State):
    time(Time),
    applyInfo(Info),
    ID(ID),
    examineState(State){}
    QString getApplyID(){return time;}
    int getState() {return examineState;}
    QString getInfo(){return applyInfo;}
    QString getUserID(){return ID;}
    void setState(int state){ this->examineState = state;}
    void setInfo(QString Info){ this->applyInfo = Info;}

private:
    QString time;
    QString applyInfo;
    QString ID;
    int examineState;

};


#endif //QT_TEMPLATE_APPLY_H
