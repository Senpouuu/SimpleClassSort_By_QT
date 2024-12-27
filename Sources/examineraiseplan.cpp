//
// Created by Senpouuu on 2023/11/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_examineRaisePlan.h" resolved

#include <QSettings>
#include "examineraiseplan.h"
#include "ui_examineRaisePlan.h"
#include "childThread.h"

examineRaisePlan::examineRaisePlan(QWidget *parent) :
        QDialog(parent), ui(new Ui::examineRaisePlan) {
    ui->setupUi(this);

    QStringList strs = {"课程号", "课程名", "学分","课程类型","上课时间"};
    ui->tableWidget->setColumnCount(strs.length());
    ui->tableWidget->setHorizontalHeaderLabels(strs);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择
    setWindowTitle("培养方案");


    connect(ui->btnRejc,&QPushButton::clicked,[=]{
        ui->btnAccept->setEnabled(false);
        ui->btnRejc->setEnabled(false);
        emit sig_btnRejc_clicked(ui->label->text().split("的培养方案")[0]);
    });

    connect(ui->btnAccept,&QPushButton::clicked,[=]{
        emit sig_btnAccept_clicked(ui->label->text().split("的培养方案")[0]);
    });


}

examineRaisePlan::~examineRaisePlan() {
    delete ui;
}

void examineRaisePlan::readData(raisePlanSubject* sub,QString UserID)
{
    auto obs = sub->getObserverByID(UserID);
    auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
    switch (Cobs->examineState)
    {
        case 0:
            ui->btnRejc->setEnabled(false);
            ui->btnAccept->setEnabled(false);
            break;
        case 1:
            ui->btnRejc->setEnabled(false);
            ui->btnAccept->setEnabled(false);
            break;
        case 2:
            ui->btnRejc->setEnabled(true);
            ui->btnAccept->setEnabled(true);
            break;
        case 3:
            ui->btnRejc->setEnabled(true);
            ui->btnAccept->setEnabled(false);
            break;

    }
    // 管理员随时改状态
    ui->label->setText(UserID + "的培养方案");
    ui->tableWidget->setRowCount(Cobs->raisePlan.length());
    int idx = 0;
    for (auto course : Cobs->raisePlan)
    {
        QTableWidgetItem *ID = new QTableWidgetItem(QString::number(course.getID()),QTableWidgetItem::Type);
        ID->setToolTip(QString::number(course.getID()));
        ui->tableWidget->setItem(idx,0, ID);  // 设置单元格内容

        QTableWidgetItem * Name = new QTableWidgetItem(course.getName(),QTableWidgetItem::Type);
        Name->setToolTip(course.getName());
        Name->setSizeHint(QSize(110,30));
        ui->tableWidget->setItem(idx,1, Name);  // 设置单元格内容

        QTableWidgetItem * Crides = new QTableWidgetItem(QString::number(course.getCredits()),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        ui->tableWidget->setItem(idx,2, Crides);  // 设置单元格内容

        QTableWidgetItem * Marjor = new QTableWidgetItem(QString(course.isCore()? "必修课" : "选修课"),QTableWidgetItem::Type);
        Marjor->setToolTip(QString(course.isCore()? "必修课" : "选修课"));
        ui->tableWidget->setItem(idx,3, Marjor);  // 设置单元格内容

        QTableWidgetItem * Hours = new QTableWidgetItem(QString::number(course.getHouse()),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        ui->tableWidget->setItem(idx,4, Hours);  // 设置单元格内容

        QTableWidgetItem * Teacher = new QTableWidgetItem(course.getTeacher(),QTableWidgetItem::Type);
        Crides->setToolTip(QString::number(course.getCredits()));
        ui->tableWidget->setItem(idx,5, Teacher);  // 设置单元格内容
        idx++;
    }
}


