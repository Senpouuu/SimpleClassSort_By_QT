//
// Created by Senpouuu on 2023/11/29.
//

#ifndef QT_TEMPLATE_COURSETABLE_H
#define QT_TEMPLATE_COURSETABLE_H

#include <QDialog>
#include <utility>
#include "course.h"
#include "QCloseEvent"
#include "raisePlan.h"

QT_BEGIN_NAMESPACE
namespace Ui { class classTable; }
QT_END_NAMESPACE

class classTable : public QDialog {
Q_OBJECT

public:
    explicit classTable(QString ID,QList<Course> courseList,QWidget *parent = nullptr);
    ~classTable() override;
    void setUser(QString userID);

private:
    courseTable* pTable;
    raisePlanObserver* currenObs = nullptr;
    QString UserID;
    Ui::classTable *ui;


signals:

public slots:
    void passRaisplanObs(raisePlanObserver * obs);

protected:
    // 重写关闭事件
    void closeEvent(QCloseEvent *event) override;

};


#endif //QT_TEMPLATE_COURSETABLE_H
