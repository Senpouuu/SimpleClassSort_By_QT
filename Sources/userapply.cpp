//
// Created by Senpouuu on 2023/12/8.
//

// You may need to build the project (run Qt uic code generator) to get "ui_userapply.h" resolved

#include "userapply.h"
#include "ui_userapply.h"


userapply::userapply(Apply* apply,QWidget *parent) :
        QDialog(parent), ui(new Ui::userapply) {
    ui->setupUi(this);
    this->pApply = apply;
    setWindowTitle("申请:"+pApply->getApplyID());
    ui->textBrowser_2->setText(pApply->getInfo());
    QString text;
    switch (pApply->getState()) {
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
    ui->label_3->setText("申请状态:" + text);
}

userapply::~userapply() {
    delete ui;
}
