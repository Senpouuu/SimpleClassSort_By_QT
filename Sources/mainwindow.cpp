//
// Created by Senpouuu on 2023/11/23.
//
// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include <QMessageBox>
#include "mainwindow.h"
#include "a_mainwindow.h"
#include "ui_mainwindow.h"
#include "QFont"
#include "QTabBar"
#include "course.h"
#include "QTableWidgetItem"
#include "QSettings"
#include "raisePlan.h"
#include "childThread.h"
#include "utils.h"
#include "QLineEdit"
#include "editapply.h"
#include "userapply.h"
#include "apply.h"

mainwindow::mainwindow(QWidget *parent,QString ID) :
    QDialog(parent), ui(new Ui::mainwindow) {
    // 培养方案
    /*初始化变量*/
    ui->setupUi(this);
    currentUser = ID;
    pSub = Data::getRaisePlanSubject();
    RaiseFacotry = new raisePlanFactory;
    RaiseObs = pSub->getObserverByID(ID);
    if (RaiseObs == nullptr)
        RaiseObs = RaiseFacotry->createItem();
    Cobs = dynamic_cast<raisePlanObserver*>(RaiseObs);
    refreshThread = new childThread();
    raiseTable = new myTableWidget(ui->RaiseTable,Cobs->getRaisePlan());
    courseTable = new myTableWidget(ui->CoursesTable,Courses::courses);
    refreshThread->start();
    setWindowTitle("当前用户:"+currentUser);

    QStringList strs = {"课程号", "课程名", "学分","课程类型","上课时间"};
    ui->CoursesTable->setColumnCount(strs.length());
    ui->CoursesTable->setHorizontalHeaderLabels(strs);
    ui->CoursesTable->setRowCount(Courses::courses.length());
    ui->CoursesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->CoursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->CoursesTable->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择
    courseTable->raisePlanTableRefresh();

    ui->RaiseTable->setColumnCount(strs.length());
    ui->RaiseTable->setHorizontalHeaderLabels(strs);
    ui->RaiseTable->setRowCount(0);
    ui->RaiseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->RaiseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->RaiseTable->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择
    raiseTable->raisePlanTableRefresh();
    stateRefresh();

    if(Cobs->hasTable())
    {
        qDebug() <<Cobs->ID <<"HasTable";
        tableTab = new QWidget();
        ui->courseTableWidget->addTab(tableTab, "我的课表");
        auto widgetTable = new QTableWidget(tableTab);
        auto pLay = new QVBoxLayout(tableTab);
        pLay->addWidget(widgetTable);

        QStringList columnStrs = {"周一", "周二", "周三","周四","周五","周六","周日"};
        widgetTable->setColumnCount(columnStrs.length());
        widgetTable->setHorizontalHeaderLabels(columnStrs);
        widgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QStringList rowStrs = {"上午", "上午", "上午","上午","下午","下午","下午","下午","晚上","晚上"};
        widgetTable->setRowCount(rowStrs.length());
        widgetTable->setVerticalHeaderLabels(rowStrs);
        widgetTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        courseTable::setTableToWidget(Cobs->table,widgetTable);
    }


    connect(ui->CoursesTable,&QTableWidget::itemClicked,[=](QTableWidgetItem* item)mutable {
        selected_CourseID = item->row();
    });

    connect(ui->RaiseTable,&QTableWidget::itemClicked,[=](QTableWidgetItem* item)mutable {
        selected_RaiseID = item->row();
    });

    connect(refreshThread,&childThread::sig_dataChanged,[=]{
        stateRefresh();
        if (Cobs->examineState < 2)
            setApplyBtn(false);
        else
            setApplyBtn(true);

    });

    connect(refreshThread,&childThread::sig_dataChanged, this,&mainwindow::data_update);

    // 申请表
    ui->plainTextEdit->setPlainText("在这里填写你的申请");
    applyTable = new myTableWidget(ui->applyTable);
    QStringList columnStrs = {"申请编号","申请用户","申请内容","申请状态"};
    ui->applyTable->setColumnCount(columnStrs.length());
    ui->applyTable->setHorizontalHeaderLabels(columnStrs);
    ui->applyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->applyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->applyTable->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择
    if(Cobs->raisePlan.isEmpty() || Cobs->examineState < 2)
        setApplyBtn(false);


    if(!Cobs->applyList.isEmpty())
        applyTable->applyTableRefresh(currentUser);


    connect(ui->applyTable,&QTableWidget::itemClicked,[=](QTableWidgetItem* item)mutable {
        selected_applyID = item->row();
    });

    // 双击查看详细信息
    connect(ui->applyTable,&QTableWidget::cellDoubleClicked,[=](int column,int row){
        auto page = new userapply(&Cobs->applyList[column]);
        page->show();
    });

}

mainwindow::~mainwindow() {
    delete ui;
    delete RaiseFacotry;
}

void mainwindow::on_btnAdd_clicked()
{
    if (selected_CourseID != -1)
    {
        Cobs->append_raisePlan(Courses::courses[selected_CourseID]);
        raiseTable->listRefresh(Cobs->raisePlan);
        ui->RaiseTable->setRowCount(Cobs->raisePlan.length());
        raiseTable->raisePlanTableRefresh();
    }
    selected_CourseID = -1;
}

void mainwindow::on_btnDel_clicked()
{
    if (selected_RaiseID != -1)
    {
        Cobs->raisePlan.removeAt(selected_RaiseID);
        raiseTable->listRefresh(Cobs->raisePlan);
        ui->RaiseTable->setRowCount(Cobs->raisePlan.length());
        raiseTable->raisePlanTableRefresh();
    }
    selected_RaiseID = -1;

}

void mainwindow::on_btnSave_clicked()
{
    if(ui->RaiseTable->findItems("*", Qt::MatchWildcard).isEmpty())
    {
        CreateInfoBox("还未添加任何内容!");
        return;
    }
    auto flag = pSub->getObserverByID(currentUser);
    flag = dynamic_cast<raisePlanObserver*>(flag);
    if (flag == nullptr)
    {
        Cobs->examineState = 1;
        Cobs->ID = currentUser;
        pSub->registerObserver(RaiseObs);

    }
    else if(Cobs->examineState < 1)
        flag->update(1);


    Data::saveData(currentUser);
    dataIsChanged.wakeAll();
    a_dataIsChanged.wakeAll();
    CreateInfoBox("保存成功!");

}

void mainwindow::on_btnSub_clicked()
{
    if(ui->RaiseTable->findItems("*", Qt::MatchWildcard).isEmpty())
    {
        CreateInfoBox("还未添加任何内容!");
        return;
    }
    auto flag = pSub->getObserverByID(currentUser);
    flag = dynamic_cast<raisePlanObserver*>(flag);
    if (flag == nullptr)
    {
        Cobs->ID = currentUser;
        Cobs->examineState = 2;
        pSub->registerObserver(RaiseObs);
    }
    else if(Cobs->examineState < 2)
        flag->update(2);


    a_dataIsChanged.wakeAll();
    dataIsChanged.wakeAll();
    Data::saveData(currentUser);
    CreateInfoBox("提交成功!");

}



int mainwindow::stateRefresh()
{
    int flag = Cobs->examineState;
    switch (flag)
    {
        case 0:
            ui->labelState->setText("<未保存>");
            setBtn(true);
            break;
        case 1:
            ui->labelState->setText("<已保存>");
            setBtn(true);
            break;
        case 2:
            ui->labelState->setText("<已提交>");
            setBtn(false);
            break;
        case 3:
            ui->labelState->setText("<已审核>");
            setBtn(false);
            break;
        default:
            ui->labelState->hide();
    }

    return flag;
}

void mainwindow::CreateInfoBox(QString Message)
{
    auto Succ = new QMessageBox;
    Succ->setText(Message);
    Succ->setIcon(QMessageBox::Information);
    int res = Succ->exec();
    if(res == QMessageBox::Ok)
        delete Succ;
}

void mainwindow::setBtn(bool state)
{
    ui->btnAdd->setEnabled(state);
    ui->btnDel->setEnabled(state);
    ui->btnSave->setEnabled(state);
    ui->btnSub->setEnabled(state);
}

void mainwindow::setApplyBtn(bool state)
{
    ui->btnApplySub->setEnabled(state);
    ui->btnApplyEdit->setEnabled(state);
    ui->btnApplyDel->setEnabled(state);
}

void mainwindow::data_update()
{
    // 刷新课表
    if (Cobs->hasTable())
    {
        if(tableTab == nullptr)
        {
            tableTab = new QWidget();
            ui->courseTableWidget->addTab(tableTab, "我的课表");
            auto widgetTable = new QTableWidget(tableTab);
            auto pLay = new QVBoxLayout(tableTab);
            pLay->addWidget(widgetTable);

            QStringList columnStrs = {"周一", "周二", "周三","周四","周五","周六","周日"};
            widgetTable->setColumnCount(columnStrs.length());
            widgetTable->setHorizontalHeaderLabels(columnStrs);
            widgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

            QStringList rowStrs = {"上午", "上午", "上午","上午","下午","下午","下午","下午","晚上","晚上"};
            widgetTable->setRowCount(rowStrs.length());
            widgetTable->setVerticalHeaderLabels(rowStrs);
            widgetTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

            courseTable::setTableToWidget(Cobs->table,widgetTable);
        }
    }

    // 刷新申请表
    if(!Cobs->applyList.isEmpty())
        applyTable->applyTableRefresh(currentUser);




}

void mainwindow::on_btnApplySub_clicked()
{
    if(ui->plainTextEdit->toPlainText() == "在这里填写你的申请")
        CreateInfoBox("请填写申请");
    else
    {
        Cobs->applyList.append(Apply(ui->plainTextEdit->toPlainText(),currentUser));
        CreateInfoBox("提交成功!");
        Data::saveData(currentUser);
        dataIsChanged.wakeAll();
        a_dataIsChanged.wakeAll();
    }
}

void mainwindow::on_btnApplyEdit_clicked()
{
    if (selected_applyID == -1)
        return;
    if(Cobs->applyList[selected_applyID].getState()!=0)
    {
        CreateInfoBox("申请已受理，无法修改！");
        return;
    }
    auto page = new editapply(&Cobs->applyList[selected_applyID]);
    page->show();
    Data::saveData(currentUser);
}

void mainwindow::on_btnApplyDel_clicked()
{
    if (selected_applyID != -1)
    {
        auto flag = utilCreateSelectInfoBox("确定删除吗?");
        if (flag != 1024)
            return;
        Cobs->applyList.removeAt(selected_applyID);
        ui->applyTable->setRowCount(Cobs->applyList.length());
        applyTable->applyTableRefresh(currentUser);
    }
    Data::saveData(currentUser);
    dataIsChanged.wakeAll();
    a_dataIsChanged.wakeAll();
    selected_applyID = -1;
}



void mainwindow::testTable(Room* pRoom)
{
    tableTab = new QWidget();
    ui->courseTableWidget->addTab(tableTab,pRoom->getID());
    auto widgetTable = new QTableWidget(tableTab);
    auto pLay = new QVBoxLayout(tableTab);
    pLay->addWidget(widgetTable);

    QStringList testCol = {"0", "1", "2","3","4","5","6"};
    widgetTable->setColumnCount(testCol.length());
    widgetTable->setHorizontalHeaderLabels(testCol);
    widgetTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList testRow = {"0", "1", "2","3","4","5","6","7","8","9"};
    widgetTable->setRowCount(testRow.length());
    widgetTable->setVerticalHeaderLabels(testRow);
    widgetTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    widgetTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    courseTable::setTableToWidget(pRoom->table,widgetTable);
    widgetTable->show();
}