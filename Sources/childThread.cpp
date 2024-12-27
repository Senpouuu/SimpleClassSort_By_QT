//
// Created by Senpouuu on 2023/11/24.
//
#include "childThread.h"
#include "QDebug"
QMutex mutexRefresh;
QWaitCondition dataIsChanged;

void childThread::run()
{
    while (1)
    {
        mutexRefresh.lock();
        dataIsChanged.wait(&mutexRefresh);
        emit sig_dataChanged();
        mutexRefresh.unlock();
    }

}
