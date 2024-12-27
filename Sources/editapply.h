//
// Created by Senpouuu on 2023/12/7.
//

#ifndef QT_TEMPLATE_EDITAPPLY_H
#define QT_TEMPLATE_EDITAPPLY_H

#include <QDialog>
#include "apply.h"


QT_BEGIN_NAMESPACE
namespace Ui { class editapply; }
QT_END_NAMESPACE

class editapply : public QDialog {
Q_OBJECT

public:
    explicit editapply(Apply* apply,QWidget *parent = nullptr);

    ~editapply() override;

private:
    Ui::editapply *ui;
    Apply* pApply;
};


#endif //QT_TEMPLATE_EDITAPPLY_H
