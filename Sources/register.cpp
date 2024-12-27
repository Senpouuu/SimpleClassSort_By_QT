//
// Created by a5192 on 2023/11/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Register.h" resolved

#include <QSettings>
#include <QCryptographicHash>
#include "register.h"
#include "ui_Register.h"
#include "QLineEdit"
#include "string"
#include "QFile"
#include "QMessageBox"
struct Account
{
    QString ID;
    QString PassWord;
    QString PassWord1;
}account;

Register::Register(QWidget *parent) :
        QDialog(parent), ui(new Ui::Register) {
    ui->setupUi(this);
    ui->verticalLayout->setContentsMargins(0,10,0,10);
    ui->RegPassLine->setEchoMode(QLineEdit::Password);
    ui->RegPass2->setEchoMode(QLineEdit::Password);
    user_path = qApp->applicationDirPath() + "/Users/User.ini";
    setWindowTitle("注册");

}

Register::~Register() {
    delete ui;
}

void Register::saveUserInfo(const QString &username, const QString &password)
{
    QSettings settings(user_path, QSettings::IniFormat);

    // 使用SHA-256哈希算法对密码进行哈希
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    // 以用户名为键，将用户名和哈希后的密码写入INI文件
    auto flag = settings.value(username + "/password").toString().isEmpty();
    if(flag)
    {
        settings.setValue(username + "/password", hashedPassword.toHex());
        settings.sync();
        QMessageBox* Succ = new QMessageBox();
        Succ->setText("注册成功!");
        int res = Succ->exec();
        if(res == QMessageBox::Ok)
            delete Succ;
        close();
    }
    else
    {
        QMessageBox* Fail = new QMessageBox();
        Fail->setText("注册失败\r\n用户:" + username + "已存在!");
        int res = Fail->exec();
        if(res == QMessageBox::Ok)
            delete Fail;
    }


}


void Register::on_Regbtn_reg_clicked()
{
    account.ID = ui->RegAccLine->text();
    account.PassWord = ui->RegPassLine->text();
    account.PassWord1 = ui->RegPass2->text();

    if (account.PassWord1 == account.PassWord && account.PassWord1.isEmpty() == 0 && account.ID.isEmpty() == 0 && account.PassWord.isEmpty()==0)
        saveUserInfo(account.ID,account.PassWord);
    else
    {
        QMessageBox* Same = new QMessageBox();
        Same->setText("请检查未填项目\r\n或两次输入密码不一致");
        Same->exec();
    }

}

