//
// Created by a5192 on 2023/12/6.
//

#ifndef QT_TEMPLATE_EXAMINEAPPLY_H
#define QT_TEMPLATE_EXAMINEAPPLY_H

#include <QDialog>
#include "raisePlan.h"


QT_BEGIN_NAMESPACE
namespace Ui { class examineapply; }
QT_END_NAMESPACE

class examineapply : public QDialog {
    Q_OBJECT

public:
    explicit examineapply(QString UserID,QString applyID,QWidget *parent = nullptr);

    ~examineapply() override;

private slots:
    void on_btnPass_clicked();
    void on_btnDeny_clicked();
private:
    Ui::examineapply *ui;
    QString UserID;
    QString applyID;
    raisePlanSubject* pSub;
    raisePlanObserver* pObs;
    Apply* pApply;

};


#endif //QT_TEMPLATE_EXAMINEAPPLY_H
