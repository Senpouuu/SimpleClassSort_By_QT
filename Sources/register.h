//
// Created by a5192 on 2023/11/22.
//

#ifndef QT_TEMPLATE_REGISTER_H
#define QT_TEMPLATE_REGISTER_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QDialog {
Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);

    ~Register() override;
private slots:
    void on_Regbtn_reg_clicked();
private:
    Ui::Register *ui;
    QString user_path;
    void saveUserInfo(const QString &username, const QString &password);
};


#endif //QT_TEMPLATE_REGISTER_H
