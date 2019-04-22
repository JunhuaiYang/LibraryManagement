#include "user_login.h"
#include "ui_user_login.h"

user_login::user_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user_login)
{
    ui->setupUi(this);
}

user_login::~user_login()
{
    delete ui;
}
