//
// Created by Senpouuu on 2023/12/7.
//

// You may need to build the project (run Qt uic code generator) to get "ui_editapply.h" resolved

#include "editapply.h"
#include "ui_editapply.h"
#include "utils.h"
#include "childThread.h"
#include "a_childThread.h"
editapply::editapply(Apply* apply,QWidget *parent) :
        QDialog(parent), ui(new Ui::editapply) {
    ui->setupUi(this);
    this->pApply = apply;
    setWindowTitle("申请:" + apply->getApplyID());
    connect(ui->pushButton,&QPushButton::clicked, [=]{
        if(ui->lineEdit->text().isEmpty())
        {
            utilCreateInfoBox("请填写申请内容!");
            return;
        }
        apply->setInfo(ui->lineEdit->text());
        utilCreateInfoBox("修改成功!");
        dataIsChanged.wakeAll();
        a_dataIsChanged.wakeAll();
        close();
    });

}

editapply::~editapply() {
    delete ui;
}
