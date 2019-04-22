#ifndef USER_LOGIN_H
#define USER_LOGIN_H

#include <QWidget>

namespace Ui {
class user_login;
}

class user_login : public QWidget
{
    Q_OBJECT

public:
    explicit user_login(QWidget *parent = 0);
    ~user_login();

private:
    Ui::user_login *ui;
};

#endif // USER_LOGIN_H
