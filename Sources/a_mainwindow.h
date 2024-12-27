//
// Created by Senpouuu on 2023/11/25.
//

#ifndef QT_TEMPLATE_A_MAINWINDOW_H
#define QT_TEMPLATE_A_MAINWINDOW_H

#include <QDialog>
#include "raisePlan.h"
#include "QTableWidget"
#include "utils.h"
#include "a_childThread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class a_mainwindow; }
QT_END_NAMESPACE

class a_mainwindow : public QDialog {
Q_OBJECT

public:
    explicit a_mainwindow(QWidget *parent = nullptr);

    ~a_mainwindow() override;
signals:
    void sig_tableObs(raisePlanObserver * obs);
public slots:
    void btn_RejcClicked(QString UserID);
    void btn_AccpetClicked(QString UserID);


private:
    Ui::a_mainwindow *ui;
    QMenu* m_switchMenu;
    raisePlanSubject* pSub;
    myTableWidget* examineTable;
    myTableWidget* applyTable;
    a_childThread* a_refreshThread;
    int selectApply = -1;
    void contextMenuEvent(QContextMenuEvent *event);


};


#endif //QT_TEMPLATE_A_MAINWINDOW_H
