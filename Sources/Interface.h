//
// Created by a5192 on 2023/11/23.
//

#ifndef QT_TEMPLATE_INTERFACE_H
#define QT_TEMPLATE_INTERFACE_H
#include "QString"
#include "course.h"

class Observer
{
public:
    virtual void update(int state){};
    virtual void update(Course (*courseArray)[7]){};
    virtual void update(QString ID,QString info,int state){};
};

class Subject
{
public:
    virtual void removeObserver() = 0;
    virtual void registerObserver(Observer* obs) = 0;
};


class Factory
{
public:
    virtual Observer* createItem() = 0;
};



#endif //QT_TEMPLATE_INTERFACE_H
