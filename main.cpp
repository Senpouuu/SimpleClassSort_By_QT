#include <QApplication>
#include "Sources/Login.h"
#include "Sources/mainwindow.h"
#include "Sources/a_mainwindow.h"
#include "Sources/utils.h"
#include "Sources/courseTable.h"
QList<Room*> roomList;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Data(qApp->applicationDirPath() + "/Class/Classes.ini",qApp->applicationDirPath() + "/Class/RaiseObs.ini");
    roomList = Room::roomsInit();
    auto w = new widget;
    auto s = new a_mainwindow;

    w->show();
    s->show();
    QObject::connect(w,&widget::sig_loginSucc,[=](QString ID){
        auto m = new mainwindow(nullptr,ID);
        m->show();
    });



    return QApplication::exec();

}
