//
// Created by Senpouuu on 2023/11/25.
//

#include "a_childThread.h"
QMutex a_mutexRefresh;
QWaitCondition a_dataIsChanged;

void a_childThread::run()
{
    while (1)
    {
        a_mutexRefresh.lock();
        a_dataIsChanged.wait(&a_mutexRefresh);
        emit sig_a_dataChanged();
        a_mutexRefresh.unlock();
    }
}
