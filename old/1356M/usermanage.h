#ifndef USERMANAGE_H
#define USERMANAGE_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include "sqlite.h"

#define Button_Count_USER 4//按钮个数
#define Button_Count_Edit_USER 3//下面按钮个数
#define Edit_Count_USER 5//文本框个数
#define Label_Count_USER 4//标签卡个数
#define Table_Column_USER 6//表格列数

enum Edit_Index_User{ID_User = 0, Name_User, Gender_User, Age_User, Tel_User};//卡号 姓名 性别 年龄
enum Button_Index_User{Add_User = 0, Delete_User, Updata_User, Select_User};// 添加按钮 删除按钮 修改按钮 搜索按钮
enum Button_Edit{Lost_User = 0, Find_User, Logout_User};

class UserManage : public QWidget//用户管理界面
{
    Q_OBJECT
public:
    explicit UserManage(QWidget *parent = 0);
    void SetSlot();//设置槽函数
    void ShowTable(QSqlQuery query);//显示表格
    void ClearEdit();//清空文本框
    void Clear();//清空文本框和表格中的数据
    void SetCard(QString cardID);//设置卡号

public slots:
    void add_user();//添加用户
    void delete_user();//删除用户
    void updata_user();//修改用户
    void select_user();//搜索用户
    void get_table_line(int row, int col);//表格单击事件 获取某行值
    void clickedLostUser();
    void clickedFindUser();
    void clickedLogoutUser();

private:
    QPushButton *Button[Button_Count_USER];//按钮
    QPushButton *Edit_Button[Button_Count_Edit_USER];//按钮
    QLineEdit *Edit[Edit_Count_USER];//文本框
    QLabel *Label[Label_Count_USER];//标签
    QTableWidget *Table;//表格
    Sqlite *sql;//数据库相关操作的类对象
};

#endif // USERMANAGE_H
