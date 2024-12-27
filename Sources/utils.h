//
// Created by Senpouuu on 2023/11/28.
//

#ifndef QT_TEMPLATE_UTILS_H
#define QT_TEMPLATE_UTILS_H
#include <QTableWidget>
#include "course.h"
#include "raisePlan.h"
#include "QMessageBox"

class myTableWidget
{
private:
    QTableWidget* table;
    QList<Course> courseList;
public:
    myTableWidget(QTableWidget* table,QList<Course> list)
    {
        this->table = table;
        this->courseList = list;
    }

    myTableWidget(QTableWidget* table)
    {
        this->table = table;
    }
    void listRefresh(QList<Course> list);
    void raisePlanTableRefresh();
    void examineTableRefresh();
    void applyTableRefresh(QString UserID);
    void examineApplyRefresh(raisePlanSubject* pSub);
};


class Data
{
private:
    static inline raisePlanSubject* raisePlanSub = nullptr;
public:
    static inline QString Class_Path = "";
    static inline QString Obs_Path = "";

    Data(QString Class_Path,QString Obs_Path);
    static raisePlanSubject* getRaisePlanSubject();
    static void readData();
    static void saveData(QString UserID);



};

static inline void utilCreateInfoBox(QString Message)
{
    auto Succ = new QMessageBox;
    Succ->setText(Message);
    Succ->setIcon(QMessageBox::Information);
    int res = Succ->exec();
    if(res == QMessageBox::Ok)
        delete Succ;
}

static inline int utilCreateSelectInfoBox(QString Message)
{
    auto msgBox = new QMessageBox;
    msgBox->setText(Message);
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int res = msgBox->exec();
    if(res == QMessageBox::Ok || res == QMessageBox::Cancel)
        delete msgBox;
    return res;
}



#endif //QT_TEMPLATE_UTILS_H
