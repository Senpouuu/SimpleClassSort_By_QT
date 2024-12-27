//
// Created by a5192 on 2023/12/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_examineapply.h" resolved

#include "examineapply.h"
#include "ui_examineapply.h"
#include "utils.h"
#include "childThread.h"
#include "a_childThread.h"

examineapply::examineapply(QString UserID,QString applyID,QWidget *parent) :
        QDialog(parent), ui(new Ui::examineapply) {
    ui->setupUi(this);
    this->UserID = UserID;
    this->applyID = applyID;
    pSub = Data::getRaisePlanSubject();
    auto obs = pSub->getObserverByID(UserID);
    pObs = dynamic_cast<raisePlanObserver*>(obs);
    pApply = pObs->getApplyByID(applyID);
    if (pApply->getInfo().contains("审核意见:"))
    {
        ui->lineEdit->setEnabled(false);
        ui->lineEdit->setText(pApply->getInfo().split("审核意见:")[1]);
    }

    ui->label_2->setText("用户:" + UserID + "的申请");
    setWindowTitle("申请:" + applyID);
    ui->textBrowser->setText(pApply->getInfo());
    if(pApply->getState() != 0)
    {
        ui->btnDeny->setEnabled(false);
        ui->btnPass->setEnabled(false);
    }
}

examineapply::~examineapply() {
    delete ui;
}

void examineapply::on_btnPass_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        utilCreateInfoBox("请填写审核意见");
        return;
    }
    pObs->update(applyID,ui->lineEdit->text(),1);
    utilCreateInfoBox("审核成功!");
    Data::saveData(UserID);
    dataIsChanged.wakeAll();
    a_dataIsChanged.wakeAll();
    close();
}

void examineapply::on_btnDeny_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        utilCreateInfoBox("请填写审核意见");
        return;
    }
    pObs->update(applyID,ui->lineEdit->text(),2);
    utilCreateInfoBox("驳回成功!");
    Data::saveData(UserID);
    dataIsChanged.wakeAll();
    a_dataIsChanged.wakeAll();
    close();
}
