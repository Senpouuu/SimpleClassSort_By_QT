//
// Created by Senpouuu on 2023/11/29.
//

// You may need to build the project (run Qt uic code generator) to get "ui_classTable.h" resolved

#include "courseTable.h"
#include "ui_classTable.h"
#include "course.h"
#include "childThread.h"
#include "a_childThread.h"
#include "utils.h"


classTable::classTable(QString ID,QList<Course> courseList,QWidget *parent) :
        QDialog(parent), ui(new Ui::classTable) {
    ui->setupUi(this);

    QStringList columnStrs = {"周一", "周二", "周三","周四","周五","周六","周日"};
    ui->tableWidget->setColumnCount(columnStrs.length());
    ui->tableWidget->setHorizontalHeaderLabels(columnStrs);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList rowStrs = {"上午", "上午", "上午","上午","下午","下午","下午","下午","晚上","晚上"};
    ui->tableWidget->setRowCount(rowStrs.length());
    ui->tableWidget->setVerticalHeaderLabels(rowStrs);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setWindowTitle("课程表");
    this->UserID = ID;
    auto pTable = new courseTable(courseList);
    auto flag = pTable->arrageCoursebyRoom(UserID);

    if (flag)
        utilCreateInfoBox("排课成功!");
    else
        utilCreateInfoBox("排课失败,请手动调整!");
    pTable->setTableToWidget(ui->tableWidget);
    connect(ui->btnClassTableAccept,&QPushButton::clicked,[=]{
        currenObs->update(pTable->table);
        currenObs->update(3);
        Data::saveData(currenObs->ID);
        dataIsChanged.wakeAll();
        a_dataIsChanged.wakeAll();
        utilCreateInfoBox("通过成功!");
        close();
    });
    connect(ui->btnClassTableEdit,&QPushButton::clicked,[=]{
        close();
    });

}

classTable::~classTable() {
    delete ui;
}

void classTable::closeEvent(QCloseEvent *event)
{
    delete this;
}

void classTable::setUser(QString userID)
{
    ui->label_2->setText(userID + "的课程表");
}

void classTable::passRaisplanObs(raisePlanObserver *obs)
{

    this->currenObs = obs;
}



