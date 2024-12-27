//
// Created by Senpouuu on 2023/11/24.
//

#include <QVBoxLayout>
#include "raisePlan.h"
#include "QDebug"
#include "QTableWidget"
void raisePlanSubject::removeObserver()
{

}

//0保存 1已提交 2已审核
void raisePlanSubject::registerObserver(Observer* obs)
{
    this->ObserverList.append(obs);
}

Observer *raisePlanSubject::getObserverByID(QString ID)
{
    Observer * ret;
    for (auto obs : ObserverList)
    {
        auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
        if(Cobs->ID == ID)
        {
            ret = obs;
            return ret;
        }
    }
    return nullptr;
}

/*方案状态更新*/
void raisePlanObserver::update(int state)
{
    this->examineState = state;
}


/*方案课程表更新*/
void raisePlanObserver::update(Course (*courseArray)[7])
{
    // 遍历类中的table，将每个元素复制到目标table中
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 7; j++)
            table[i][j] = courseArray[i][j];
}

/*申请更新*/
void raisePlanObserver::update(QString ID,QString info,int state)
{
    auto apply = getApplyByID(ID);
    QString m = apply->getInfo() + "\r\n\r\n审核意见:"  + info;
    apply->setInfo(m);
    apply->setState(state);
}


void raisePlanObserver::append_raisePlan(Course c)
{
    raisePlan.append(c);
}


bool raisePlanObserver::hasTable()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 7; j++)
            if(table[i][j].getID() != -1)
                return true;
    return false;
}

Apply* raisePlanObserver::getApplyByID(QString ID)
{
    for (auto & i : applyList)
        if (i.getApplyID() == ID)
        {
            Apply* ptr = &i;
            return ptr;
        }
    return nullptr ;
}


Observer *raisePlanFactory::createItem()
{
    auto Obs = new raisePlanObserver();
    return Obs;
}


