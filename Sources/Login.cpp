//
// Created by Senpouuu on 2023/11/11.
//

// You may need to build the project (run Qt uic code generator) to get "ui_widget.h" resolved
#include <QSettings>
#include <QCryptographicHash>
#include "Login.h"
#include "ui_widget.h"
#include "QMenu"
#include "QGridLayout"
#include "register.h"
#include "QMessageBox"
#include "QKeyEvent"
#include "mainwindow.h"
QSize StdSize = QSize(300,250);


widget::widget(QWidget *parent) :
        QWidget(parent), ui(new Ui::widget) {
    ui->setupUi(this);
    ui->btn_forget->setCursor(Qt::PointingHandCursor);
    ui->btn_reg->setCursor(Qt::PointingHandCursor);
    ui->verticalLayout->setContentsMargins(10,20,10,10);
    this->setMinimumSize(300,250);
    this->setMaximumSize(300,250);
    ui->PasswordlineEdit->setEchoMode(QLineEdit::Password);
    user_path = qApp->applicationDirPath() + "/Users/User.ini";
    setWindowTitle("登录");
    connect(ui->btn_reg,&QPushButton::clicked, [=]{
        auto reg = new Register(this);
        reg->setMinimumSize(StdSize);
        reg->setMaximumSize(StdSize);
        reg->show();
    });


}


widget::~widget() {
    delete ui;
}


void widget::on_btn_login_clicked()
{
    QString ID = ui->AccountlineEdit->text();
    QString Password = ui->PasswordlineEdit->text();

    if (ID.isEmpty() || Password.isEmpty())
    {
        auto Fail = new QMessageBox ;
        Fail->setText("请输入账户名或密码");
        Fail->show();
        int res = Fail->exec();
        if(res == QMessageBox::Ok)
            delete Fail;
    }
    else
    {
        QSettings User(user_path, QSettings::IniFormat);
        QByteArray hashedPassword = QCryptographicHash::hash(Password.toUtf8(), QCryptographicHash::Sha256).toHex();
        QByteArray Account = User.value("/"+ ID + "/password").toByteArray();

        if(hashedPassword == Account)
        {
            auto Succ = new QMessageBox();
            Succ->setText("欢迎" + ID + "!");
            int res = Succ->exec();
            if(res == QMessageBox::Ok)
            {
                delete Succ;
                emit sig_loginSucc(ID);
                this->hide();
            }
        }
        else
        {
            auto fail = new QMessageBox();
            fail->setText("登录失败，请检查用户名或密码");
            int res = fail->exec();
            if(res == QMessageBox::Ok)
            {
                delete fail;
            }
        }

    }



}

void widget::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        on_btn_login_clicked();
    }

}






