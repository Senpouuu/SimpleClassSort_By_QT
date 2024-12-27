//
// Created by Senpouuu on 2023/12/8.
//

#ifndef QT_TEMPLATE_USERAPPLY_H
#define QT_TEMPLATE_USERAPPLY_H

#include <QDialog>
#include "apply.h"


QT_BEGIN_NAMESPACE
namespace Ui { class userapply; }
QT_END_NAMESPACE

class userapply : public QDialog {
Q_OBJECT

public:
    explicit userapply(Apply* apply,QWidget *parent = nullptr);

    ~userapply() override;

private:
    Ui::userapply *ui;
    Apply* pApply;
};


#endif //QT_TEMPLATE_USERAPPLY_H
