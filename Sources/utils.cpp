//
// Created by Senpouuu on 2023/11/28.
//
#include <QSettings>
#include <QMessageBox>
#include "raisePlan.h"
#include "utils.h"
#include "course.h"
Data::Data(QString Class_Path, QString Obs_Path) {
    this->Class_Path = Class_Path;
    this->Obs_Path = Obs_Path;
    raisePlanSub = getRaisePlanSubject();
    readData();
}

raisePlanSubject *Data::getRaisePlanSubject() {
    if(raisePlanSub == nullptr)
        raisePlanSub = new raisePlanSubject;
    return raisePlanSub;

}

void Data::readData() {
    /*读取课表*/
    QSettings Classes(Class_Path,QSettings::IniFormat);
    QStringList allKeys = Classes.allKeys();
    int i = 0;
    while (i != allKeys.length())
    {
        int Credits = Classes.value(allKeys[i]).toInt();
        int Hour = Classes.value(allKeys[++i]).toInt();
        int ID = Classes.value(allKeys[++i]).toInt();
        QString Name = Classes.value(allKeys[++i]).toString();
        QString Teacher = Classes.value(allKeys[++i]).toString();
        int Core = Classes.value(allKeys[++i]).toInt();
        i++;
        Courses::appendCourse(Course(ID,Name,Credits,Core,Hour,Teacher));
    }


    QSettings settings(Obs_Path,QSettings::IniFormat);
    auto alluser = settings.childGroups();
    for (auto user : alluser)
    {
        auto obs = new raisePlanObserver;
        auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
        /*有数据取消读取*/
        auto flag = raisePlanSub->getObserverByID(user);
        if (flag != nullptr)
            return;

        auto ID = settings.value("/" + user + "/courseID").toString().split(',');
        auto Name = settings.value('/' + user + "/courseName").toString().split(',');
        auto Credit= settings.value('/' + user + "/courseCredits").toString().split(',');
        auto Core = settings.value('/' + user + "/isCoreCourse").toString().split(',');
        auto Hour = settings.value('/' + user + "/courseHours").toString().split(',');
        auto Teacher = settings.value('/' + user + "/courseTeacher").toString().split(',');

        Cobs->ID = user;
        int i = 0;
        while (i != ID.length())
        {
            Cobs->append_raisePlan(Course(ID[i].toInt(),Name[i],Credit[i].toInt(),Core[i].toInt(),Hour[i].toInt(),Teacher[i]));
            i++;
        }
        Cobs->examineState = settings.value("/" + user + "/examineState").toInt();
        raisePlanSub->registerObserver(obs);

        // 读取课程表
        auto rows = settings.value('/' + user + "/tableRow").toString().split(',');
        auto cols = settings.value('/' + user + "/tableCol").toString().split(',');
        auto names = settings.value('/' + user + "/tableName").toString().split(',');
        auto rooms = settings.value('/' + user + "/tableRoom").toString().split(',');

        if (rows.length()>1 && cols.length()>1 && names.length()>1)
        {
            int j = 0;
            while (j!=rows.length())
            {
//                qDebug() <<Cobs->ID<<"ReadingData:"<<"table"<<rows[j]<<cols[j] <<':'<< Courses::getCourseByName(names[j]).getName();
                auto course = Courses::getCourseByName(names[j]);
                course.setRoom(rooms[j]);
                Cobs->table[rows[j].toInt()][cols[j].toInt()] = course;
                j++;
            }
        }

        //读取申请表
        auto applyTime = settings.value('/' + user + "/applyTime").toString().split(',');
        auto applyInfo = settings.value('/' + user + "/applyInfo").toString().split(',');
        auto applyState = settings.value('/' + user + "/applyState").toString().split(',');
        auto applyUser = settings.value('/' + user + "/applyUser").toString().split(',');

        if (applyTime[0] != "")
        {
            int l = 0;
            while (l!=applyTime.length())
            {
                Cobs->applyList.append(Apply(applyTime[l],applyInfo[l],applyUser[l],applyState[l].toInt()));
                l++;
            }
        }
    }


}

void Data::saveData(QString UserID) {
    QSettings settings(Obs_Path, QSettings::IniFormat);
    auto obs = raisePlanSub->getObserverByID(UserID);
    auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
    QString IDs,Credits,Names,Cores,Hours,Teachers;
    for (auto raise : Cobs->raisePlan)
    {
        IDs = IDs + QString::number(raise.getID()) + ',';
        Credits = Credits + QString::number(raise.getCredits()) + ',';
        Names = Names + raise.getName() + ',';
        Cores = Cores + QString::number(raise.isCore()) + ',';
        Hours = Hours + QString::number(raise.getHouse()) + ',';
        Teachers = Teachers + raise.getTeacher() + ',';
    }
    IDs.chop(1);
    Credits.chop(1);
    Names.chop(1);
    Cores.chop(1);
    Hours.chop(1);
    Teachers.chop(1);

    settings.setValue("/"+ UserID +"/examineState",Cobs->examineState);
    settings.setValue("/"+ UserID +"/courseID",IDs);
    settings.setValue("/"+ UserID +"/courseName",Names);
    settings.setValue("/"+ UserID +"/courseCredits",Credits);
    settings.setValue("/"+ UserID +"/isCoreCourse",Cores);
    settings.setValue("/"+ UserID +"/courseHours",Hours);
    settings.setValue("/"+ UserID +"/courseTeacher",Teachers);



    if (Cobs->hasTable())
    {
        QString rows,cols,names,rooms;
        for(int i = 0;i<10;i++)
            for (int j = 0;j<7;j++)
            {
                rows = rows + QString::number(i) + ',';
                cols = cols + QString::number(j) + ',';
                names = names + Cobs->table[i][j].getName() + ',';
                rooms = rooms + Cobs->table[i][j].getRoom() + ',';
            }
        rows.chop(1);cols.chop(1);names.chop(1);
        settings.setValue("/"+ UserID +"/tableRow",rows);
        settings.setValue("/"+ UserID +"/tableCol",cols);
        settings.setValue("/"+ UserID +"/tableName",names);
        settings.setValue("/"+ UserID +"/tableRoom",rooms);
    }

    if(!Cobs->applyList.isEmpty())
    {
        QString times,infos,states,users;

        for (auto l : Cobs->applyList)
        {
            times = times + l.getApplyID() + ',';
            infos = infos + l.getInfo() + ',';
            states = states + QString::number(l.getState()) + ',';
            users = users + l.getUserID() + ',';
        }

        times.chop(1);infos.chop(1),states.chop(1);users.chop(1);
        settings.setValue("/"+ UserID +"/applyTime",times);
        settings.setValue("/"+ UserID +"/applyInfo",infos);
        settings.setValue("/"+ UserID +"/applyState",states);
        settings.setValue("/"+ UserID +"/applyUser",users);
    }
    settings.sync();
}

void myTableWidget::raisePlanTableRefresh()
{
    table->setRowCount(courseList.length());
    int idx = 0;
    for (auto course : courseList)
    {
        QTableWidgetItem *ID = new QTableWidgetItem(QString::number(course.getID()),QTableWidgetItem::Type);
        ID->setToolTip(QString::number(course.getID()));
        table->setItem(idx,0, ID);  // 设置单元格内容

        QTableWidgetItem * Name = new QTableWidgetItem(course.getName(),QTableWidgetItem::Type);
        Name->setToolTip(course.getName());
        table->setItem(idx,1, Name);  // 设置单元格内容

        QTableWidgetItem * Crides = new QTableWidgetItem(QString::number(course.getCredits()),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        table->setItem(idx,2, Crides);  // 设置单元格内容

        QTableWidgetItem * Marjor = new QTableWidgetItem(QString(course.isCore()? "必修课" : "选修课"),QTableWidgetItem::Type);
        Marjor->setToolTip(QString(course.isCore()? "必修课" : "选修课"));
        table->setItem(idx,3, Marjor);  // 设置单元格内容

        QTableWidgetItem * Hours = new QTableWidgetItem(QString::number(course.getHouse()),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        table->setItem(idx,4, Hours);  // 设置单元格内容

        QTableWidgetItem * Teacher = new QTableWidgetItem(course.getTeacher(),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        table->setItem(idx,5, Teacher);  // 设置单元格内容
        idx++;
        //qDebug() << ID->text() << Crides->text() << Marjor->text() << courses.length();
    }
}

void myTableWidget::examineTableRefresh()
{
    auto pSub = Data::getRaisePlanSubject();
    if(pSub == nullptr)
        return;
    table->setRowCount(pSub->ObserverList.length());
    int idx = 0;
    for (auto obs :pSub->ObserverList)
    {
        auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
        int state = Cobs->examineState;
        QString text;
        switch (state) {
            case 0:
                text = "未保存";
                break;
            case 1:
                text = "已保存";
                break;
            case 2:
                text = "已提交";
                break;
            case 3:
                text = "已审核";
                break;
            default:
                text = "";
        }
        QTableWidgetItem *State = new QTableWidgetItem(text,QTableWidgetItem::Type);
        table->setItem(idx,1, State);  // 设置单元格内容

        QTableWidgetItem *ID = new QTableWidgetItem(Cobs->ID,QTableWidgetItem::Type);
        table->setItem(idx,0,ID);
        idx++;

    }

}

void myTableWidget::listRefresh(QList<Course> list)
{
    this->courseList = list;
}

void myTableWidget::applyTableRefresh(QString UserID)
{
    auto pObs = Data::getRaisePlanSubject()->getObserverByID(UserID);
    auto Cobs = dynamic_cast<raisePlanObserver*>(pObs);
    int idx = 0;

    table->setRowCount(Cobs->applyList.length());
    for (auto l : Cobs->applyList)
    {
        auto Time = new QTableWidgetItem(l.getApplyID());
        Time->setToolTip(l.getApplyID());
        table->setItem(idx,0,Time);

        auto ID = new QTableWidgetItem(l.getUserID());
        ID->setToolTip(l.getUserID());
        table->setItem(idx,1,ID);

        auto Info = new QTableWidgetItem(l.getInfo());
        Info->setToolTip(l.getInfo());
        table->setItem(idx,2,Info);

        QString text;
        switch (l.getState()) {
            case 0:
                text = "已提交";
                break;
            case 1:
                text = "已受理";
                break;
            case 2:
                text = "已驳回";
                break;
            default:
                text = "";
        }
        auto State = new QTableWidgetItem(text);
        table->setItem(idx,3,State);

        idx++;
    }
}

void myTableWidget::examineApplyRefresh(raisePlanSubject* pSub)
{
    QList<Apply> ApplyList;
    for (auto user : pSub->ObserverList)
    {
        auto Cuser = dynamic_cast<raisePlanObserver*>(user);
        for (auto l: Cuser->applyList)
            ApplyList.append(l);
    }

    int idx = 0;
    table->setRowCount(ApplyList.length());
    for (auto l :ApplyList)
    {
        auto Time = new QTableWidgetItem(l.getApplyID());
        Time->setToolTip(l.getApplyID());
        table->setItem(idx,0,Time);

        auto ID = new QTableWidgetItem(l.getUserID());
        ID->setToolTip(l.getUserID());
        table->setItem(idx,1,ID);

        auto Info = new QTableWidgetItem(l.getInfo());
        Info->setToolTip(l.getInfo());
        table->setItem(idx,2,Info);

        QString text;
        switch (l.getState())
        {
            case 0:
                text = "已提交";
                break;
            case 1:
                text = "已受理";
                break;
            case 2:
                text = "已驳回";
                break;
            default:
                text = "";
        }
        auto State = new QTableWidgetItem(text);
        table->setItem(idx,3,State);

        idx++;
    }

}


