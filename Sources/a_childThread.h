//
// Created by Senpouuu on 2023/11/25.
//

#ifndef QT_TEMPLATE_A_CHILDTHREAD_H
#define QT_TEMPLATE_A_CHILDTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
extern QMutex a_mutexRefresh;
extern QWaitCondition a_dataIsChanged;

class a_childThread : public QThread
{
Q_OBJECT
protected:
    void run() override;

signals:
    void sig_a_dataChanged();
};






#endif //QT_TEMPLATE_A_CHILDTHREAD_H
