#include "logindlg.h"
#include "ui_logindlg.h"
#include <QMessageBox>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if (username == "test" && password == "123") {
        accept();
    } else {
        if (username == "" && password == "") {
            QMessageBox::warning(NULL, tr("提示"), tr("用户名或密码不能为空"), QMessageBox::Yes);
        } else {
            QMessageBox::warning(NULL, tr("提示"), tr("用户名或密码错误"), QMessageBox::Yes);
        }
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_username->setFocus();
    }
}

void LoginDlg::on_pushButton_exit_clicked()
{
    close();
}

