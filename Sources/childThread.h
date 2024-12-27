//
// Created by Senpouuu on 2023/11/24.
//

#ifndef QT_TEMPLATE_CHILDTHREAD_H
#define QT_TEMPLATE_CHILDTHREAD_H
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

extern QMutex mutexRefresh;
extern QWaitCondition dataIsChanged;

class childThread : public QThread
{
    Q_OBJECT
protected:
    void run() override;

signals:
    void sig_dataChanged();
};


#endif //QT_TEMPLATE_CHILDTHREAD_H
