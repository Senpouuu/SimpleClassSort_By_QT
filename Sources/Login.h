//
// Created by Senpouuu on 2023/11/11.
//

#ifndef QT_TEMPLATE_LOGIN_H
#define QT_TEMPLATE_LOGIN_H

#include <QWidget>
#include "QLabel"
#include "mainwindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class widget; }
QT_END_NAMESPACE

class widget : public QWidget {
Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);

    ~widget() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_btn_login_clicked();
signals:
    void sig_loginSucc(QString ID);

private:
    Ui::widget *ui;
    QString user_path;

};


#endif //QT_TEMPLATE_LOGIN_H
