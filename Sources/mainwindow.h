//
// Created by Senpouuu on 2023/11/23.
//

#ifndef QT_CLASSSORT_MAINWINDOW_H
#define QT_CLASSSORT_MAINWINDOW_H

#include <QDialog>
#include <QApplication>
#include <QTableWidget>
#include "course.h"
#include "raisePlan.h"
#include "childThread.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QDialog {
    Q_OBJECT



public:
    explicit mainwindow(QWidget *parent = nullptr,QString ID = "");

    ~mainwindow() override;

private slots:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void on_btnSave_clicked();
    void on_btnSub_clicked();
    void on_btnApplySub_clicked();
    void on_btnApplyEdit_clicked();
    void on_btnApplyDel_clicked();
    void data_update();
public slots:


public:
    static void CreateInfoBox(QString Message);

private:
    childThread* refreshThread;
    int selected_CourseID = -1;
    int selected_RaiseID = -1;
    int selected_applyID = -1;
    QString currentUser;
    Factory* RaiseFacotry;
    myTableWidget * raiseTable;
    myTableWidget * courseTable;
    myTableWidget * applyTable;
    Observer* RaiseObs;
    raisePlanObserver* Cobs;
    raisePlanSubject* pSub;
    static inline QWidget* tableTab = nullptr;

    int stateRefresh();
    void setBtn(bool state);
    void setApplyBtn(bool state);
    void testTable(Room* pRoom);

protected:
    Ui::mainwindow *ui;

};


#endif //QT_CLASSSORT_MAINWINDOW_H
