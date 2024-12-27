//
// Created by Senpouuu on 2023/11/24.
//

#ifndef QT_TEMPLATE_RAISEPLAN_H
#define QT_TEMPLATE_RAISEPLAN_H

#include "Interface.h"
#include "course.h"
#include "QTableWidget"
#include "apply.h"

class raisePlanSubject:public Subject
{
public:
    void registerObserver(Observer* obs) override;
    void removeObserver() override;
public:
    QList<Observer*> ObserverList;
    Observer* getObserverByID(QString ID);
};

class raisePlanObserver: public Observer
{

public:
    //0未提交 1已提交 2未审核 3已审核
    int examineState = 0;
    QString ID;
    QList<Course> raisePlan;
    QList<Apply> applyList;
    Course table[10][7];
    void append_raisePlan(Course c);
    auto getRaisePlan(){return raisePlan;}
    Apply* getApplyByID(QString ID);
    void update(int state) override;
    void update(Course (*courseArray)[7]) override;
    void update(QString ID,QString info,int state) override;
    bool hasTable();
private:
    QString userName;


};

class raisePlanFactory:public Factory
{
public:
    Observer* createItem() override;
};



#endif //QT_TEMPLATE_RAISEPLAN_H
