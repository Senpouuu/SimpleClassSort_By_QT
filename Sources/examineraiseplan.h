//
// Created by Senpouuu on 2023/11/25.
//

#ifndef QT_TEMPLATE_EXAMINERAISEPLAN_H
#define QT_TEMPLATE_EXAMINERAISEPLAN_H

#include <QDialog>
#include "a_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class examineRaisePlan; }
QT_END_NAMESPACE

class examineRaisePlan : public QDialog{
Q_OBJECT

public:
    explicit examineRaisePlan(QWidget *parent = nullptr);

    ~examineRaisePlan() override;

signals:
    void sig_btnRejc_clicked(QString UserID);
    void sig_btnAccept_clicked(QString UserID);

private:
    Ui::examineRaisePlan *ui;


public:
    void readData(raisePlanSubject* sub,QString UserID);

};


#endif //QT_TEMPLATE_EXAMINERAISEPLAN_H
