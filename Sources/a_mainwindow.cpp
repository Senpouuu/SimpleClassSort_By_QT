//
// Created by Senpouuu on 2023/11/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_a_mainwindow.h" resolved

#include <QSettings>
#include <QListWidget>
#include <QMenu>
#include <QLineEdit>
#include "a_mainwindow.h"
#include "ui_a_mainwindow.h"
#include "mainwindow.h"
#include "raisePlan.h"
#include "examineraiseplan.h"
#include "utils.h"
#include "courseTable.h"
#include "examineapply.h"

a_mainwindow::a_mainwindow(QWidget *parent):
        QDialog(parent), ui(new Ui::a_mainwindow) {
    ui->setupUi(this);
    QStringList strs = {"用户名", "审核状态"};
    examineTable = new myTableWidget(ui->tableWidget);
    applyTable = new myTableWidget(ui->tableWidget_2);
    pSub = Data::getRaisePlanSubject();
    a_refreshThread = new a_childThread();
    setWindowTitle("排课系统管理员");

    // 菜单
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    m_switchMenu = new QMenu(ui->tab);
    QAction *pAct1 = new QAction("刷新");
    m_switchMenu->addAction(pAct1);
    ui->tableWidget->setColumnCount(strs.length());
    ui->tableWidget->setHorizontalHeaderLabels(strs);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择'
    examineTable->examineTableRefresh();
    m_switchMenu->show();
    a_refreshThread->start();
    // 右键刷新
    connect(pAct1,&QAction::triggered,[=]{
        examineTable->examineTableRefresh();

    });


    // 双击打开
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,[=](int column,int row){
        auto name = ui->tableWidget->item(column,0);
        auto page = new examineRaisePlan(this);
        page->readData(pSub,name->text());
        page->show();
//        disconnect(page, nullptr, nullptr, nullptr);
        connect(page,&examineRaisePlan::sig_btnRejc_clicked,this,&a_mainwindow::btn_RejcClicked);
        connect(page,&examineRaisePlan::sig_btnAccept_clicked,this,&a_mainwindow::btn_AccpetClicked);
    });

    connect(a_refreshThread,&a_childThread::sig_a_dataChanged,[=]{
        examineTable->examineTableRefresh();
        applyTable->examineApplyRefresh(pSub);
        
    });

    // 申请从这里开始
    // 申请表
    applyTable = new myTableWidget(ui->tableWidget_2);
    QStringList columnStrs = {"申请编号","申请用户","申请内容","申请状态"};
    ui->tableWidget_2->setColumnCount(columnStrs.length());
    ui->tableWidget_2->setHorizontalHeaderLabels(columnStrs);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择模式为单行选择
    applyTable->examineApplyRefresh(pSub);


    // 双击打开
    connect(ui->tableWidget_2,&QTableWidget::cellDoubleClicked,[=](int column,int row){
        auto m = new examineapply(ui->tableWidget_2->item(column,1)->text(),ui->tableWidget_2->item(column,0)->text());
        m->show();
    });



}

a_mainwindow::~a_mainwindow() {
    delete ui;
}


void a_mainwindow::btn_RejcClicked(QString UserID)
{
    auto obs = pSub->getObserverByID(UserID);
    obs->update(0);
    Data::saveData(UserID);
    mainwindow::CreateInfoBox("驳回成功!");
    
    dataIsChanged.wakeAll();
    examineTable->examineTableRefresh();
}

void a_mainwindow::btn_AccpetClicked(QString UserID)
{
    auto obs = pSub->getObserverByID(UserID);
    auto Cobs = dynamic_cast<raisePlanObserver*>(obs);
    auto pageTable = new classTable(UserID,Cobs->raisePlan);
    pageTable->setUser(UserID);
    pageTable->show();
    connect(this,&a_mainwindow::sig_tableObs,pageTable,&classTable::passRaisplanObs);
    examineTable->examineTableRefresh();
    emit sig_tableObs(Cobs);
    dataIsChanged.wakeAll();
}

void a_mainwindow::contextMenuEvent(QContextMenuEvent *event)
{
    m_switchMenu->exec(QCursor::pos());
}


