#include "usermanage.h"

UserManage::UserManage(QWidget *parent) : QWidget(parent)
{
    QString LabelName[] = {"卡号：", "姓名：", "性别：", "年龄：","手机号："}; //标签文本
    QString ButtonName[] = {"添加", "删除", "修改", "搜索"}; //按钮文本
    QString ButtonEdit[] = {"挂失", "取消挂失", "注销用户"};  // 下面的按钮

    //布局
    QVBoxLayout *MainLayout = new QVBoxLayout();//主布局
    QHBoxLayout *ButtonLayout = new QHBoxLayout();//按钮布局
    QHBoxLayout *EditLayout = new QHBoxLayout();//文本框布局
    QHBoxLayout *TableLayout = new QHBoxLayout();//表格布局
    QHBoxLayout *EditUserButton = new QHBoxLayout(); // 下面的按钮

    QGroupBox *UserTable = new QGroupBox();//用户表格组合框
    QGroupBox *UserInfo = new QGroupBox();//用户信息组合框
    

    sql = new Sqlite();//数据库操作相关的对象

    //初始化文本框和标签
    for(int i = 0; i < Edit_Count_USER; i++)
    {
        Edit[i] = new QLineEdit();
        Label[i] = new QLabel(LabelName[i]);
        EditLayout->addWidget(Label[i]);//标签添加到布局中
        EditLayout->addWidget(Edit[i]);//文本框添加到布局中
    }

    // 正则表达式
    QRegExp regExp("[A-Fa-f9-0]*");
    Edit[ID_User]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[\u4e00-\u9fa5]*");
    Edit[Name_User]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[男女]");
    Edit[Gender_User]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[9-0]{2}");
    Edit[Age_User]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[9-0]{11}");
    Edit[Tel_User]->setValidator(new QRegExpValidator(regExp, this));

    UserInfo->setLayout(EditLayout);//用户信息组合框设置布局

    //初始化按钮
    for(int i = 0; i < Button_Count_USER; i++)
    {
        Button[i] = new QPushButton();
        Button[i]->setText(ButtonName[i]);
        ButtonLayout->addWidget(Button[i]);//按钮添加到布局中
    }
    ButtonLayout->addStretch(0);
    ButtonLayout->setSpacing(20);

    //初始化用户按钮
    for(int i = 0; i < Button_Count_Edit_USER; i++)
    {
        Edit_Button[i] = new QPushButton();
        Edit_Button[i]->setText(ButtonEdit[i]);
        EditUserButton->addWidget(Edit_Button[i]);//按钮添加到布局中
    }
    EditUserButton->addStretch(0);
    EditUserButton->setSpacing(20);

    Table = new QTableWidget();
    Table->setColumnCount(Table_Column_USER);//设置表格列
    Table->setSelectionBehavior ( QAbstractItemView::SelectRows);//选中整行
    Table->setEditTriggers ( QAbstractItemView::NoEditTriggers );//不可编辑
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽度自适应
    TableLayout->addWidget(Table);
    UserTable->setTitle("用户列表");
    UserTable->setLayout(TableLayout);

    MainLayout->addWidget(UserInfo);
    MainLayout->addLayout(ButtonLayout);
    MainLayout->addWidget(UserTable);
    MainLayout->addLayout(EditUserButton);
    MainLayout->setSpacing(10);
    this->setLayout(MainLayout);
    SetSlot();//设置槽函数
}

void UserManage::SetSlot()
{
    connect(Button[Add_User],SIGNAL(clicked()),this,SLOT(add_user()));//添加按钮连接槽函数
    connect(Button[Delete_User],SIGNAL(clicked()),this,SLOT(delete_user()));//删除按钮连接槽函数
    connect(Button[Updata_User],SIGNAL(clicked()),this,SLOT(updata_user()));//修改按钮连接槽函数
    connect(Button[Select_User],SIGNAL(clicked()),this,SLOT(select_user()));//搜索按钮连接槽函数
    connect(Edit_Button[Lost_User],SIGNAL(clicked()),this,SLOT(clickedLostUser()));//挂失按钮连接槽函数
    connect(Edit_Button[Find_User],SIGNAL(clicked()),this,SLOT(clickedFindUser()));//取消挂失按钮连接槽函数
    connect(Edit_Button[Logout_User],SIGNAL(clicked()),this,SLOT(delete_user()));//注销按钮连接槽函数
    connect(Table,SIGNAL(cellClicked(int,int)),this,SLOT(get_table_line(int, int)));//表格点击连接槽函数
}

//挂失
void UserManage::clickedLostUser()
{
    // 如果没有选中卡
    if(Edit[ID_User]->text().isEmpty())
    {
        QMessageBox::warning(NULL, "error", "没有卡片或没有选中数据", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //如果已经挂失
    if(isLast == "true")
    {
        QMessageBox::warning(NULL, "error", "该账户已经被挂失！", QMessageBox::Yes, QMessageBox::Yes);
    }
    else {
        if(sql->Updata("user_15693","isLocked = 'true'", "cardID = '"+Edit[ID_User]->text()+"'"))
            QMessageBox::information(NULL, "information", "账户挂失成功！", QMessageBox::Yes, QMessageBox::Yes);
        else {
            QMessageBox::warning(NULL, "error", "账户挂失失败", QMessageBox::Yes, QMessageBox::Yes);
        }
    }
    Clear();
    return;
}

//取消挂失
void UserManage::clickedFindUser()
{
    // 如果没有选中卡
    if(Edit[ID_User]->text().isEmpty())
    {
        QMessageBox::warning(NULL, "error", "没有卡片或没有选中数据", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //如果没有挂失
    if(isLast == "false")
    {
        QMessageBox::warning(NULL, "error", "该账户没有被挂失！", QMessageBox::Yes, QMessageBox::Yes);
    }
    else {
        if(sql->Updata("user_15693","isLocked = 'false'", "cardID = '"+Edit[ID_User]->text()+"'"))
            QMessageBox::information(NULL, "information", "账户取消挂失成功！", QMessageBox::Yes, QMessageBox::Yes);
        else {
            QMessageBox::warning(NULL, "error", "账户取消挂失失败", QMessageBox::Yes, QMessageBox::Yes);
        }
    }
    Clear();
    return;
}

//注销
void UserManage::clickedLogoutUser()
{
    // 如果没有选中卡
    if(Edit[ID_User]->text().isEmpty())
    {
        QMessageBox::warning(NULL, "error", "没有卡片或没有选中数据", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    // TODO 后面的
    QMessageBox::warning(NULL, "warning", "不能为空！", QMessageBox::Yes, QMessageBox::Yes);
    return;
}

//添加用户槽函数
void UserManage::add_user()
{
    QString LabelName[] = {"卡号：", "姓名：", "性别：", "年龄：","手机号："}; //标签文本
     for(int i = 0; i < Edit_Count_USER; i++)
     {
         if(Edit[i]->text().isEmpty())
         {
             QMessageBox::warning(NULL, "warning", LabelName[i]+"不能为空！", QMessageBox::Yes, QMessageBox::Yes);
             return;
         }
     }

     if (sql->SelectBooks(Edit[ID_User]->text()).next())
     {
         QMessageBox::warning(NULL, "warning", "卡号已经注册为书籍！", QMessageBox::Yes, QMessageBox::Yes);
         return;
     }
    int ret = sql->InsertUser(Edit[ID_User]->text(),Edit[Name_User]->text(),Edit[Gender_User]->text(),Edit[Age_User]->text().toInt(),Edit[Tel_User]->text());
    if(!ret)
    {
        QMessageBox::warning(NULL, "warning", "添加失败，编号已存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(NULL, "warning", "添加成功！", QMessageBox::Yes, QMessageBox::Yes);
    ClearEdit();
    ShowTable(sql->SelectUser());
}

//删除用户槽函数
void UserManage::delete_user()
{
    if (!Edit[ID_User]->text().isEmpty() && sql->SelectBooks(Edit[ID_User]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号已经注册为书籍！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!Edit[ID_User]->text().isEmpty() && !sql->SelectUser(Edit[ID_User]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号不存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    int Age;
    if(Edit[Age_User]->text().isEmpty())
        Age = -1;
    else
        Age = Edit[Age_User]->text().toInt();

    // 弹出提示框
    QMessageBox msg;
    int rett = msg.warning(parentWidget(),tr("警告！"), "是否要将用户注销？", QMessageBox::Ok, QMessageBox::Cancel);
    // 确定
    if(rett == QMessageBox::Ok)
    {
        // 先删除记录表中的，解除依赖关系
        sql->DeleteRecordUser(Edit[ID_User]->text());
        int ret = sql->DeleteUser(Edit[ID_User]->text(),Edit[Name_User]->text(),Edit[Gender_User]->text(),Age);
        if(!ret)
        {
            QMessageBox::warning(NULL, "warning", "删除失败！", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        QMessageBox::warning(NULL, "warning", "删除成功！", QMessageBox::Yes, QMessageBox::Yes);
    }

    ClearEdit();
    ShowTable(sql->SelectUser());
}

//修改用户信息槽函数
void UserManage::updata_user()
{
    if (!Edit[ID_User]->text().isEmpty() && sql->SelectBooks(Edit[ID_User]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号已经注册为书籍！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!Edit[ID_User]->text().isEmpty() && !sql->SelectUser(Edit[ID_User]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号不存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    int ret = sql->UpdataUser(Edit[ID_User]->text(),Edit[Name_User]->text(),Edit[Gender_User]->text(),Edit[Age_User]->text().toInt(),Edit[Tel_User]->text() );
    if(!ret)
    {
        QMessageBox::warning(NULL, "warning", "修改失败！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(NULL, "warning", "修改成功！", QMessageBox::Yes, QMessageBox::Yes);

    ClearEdit();
    ShowTable(sql->SelectUser());
}

//搜索用户槽函数
void UserManage::select_user()
{
    QSqlQuery query;
    if(Edit[Age_User]->text().isEmpty())//如果年龄为空 调用SelectUser时 不传入年龄  默认年龄为-1
        query = sql->SelectUser(Edit[ID_User]->text(),Edit[Name_User]->text(),Edit[Gender_User]->text(),-1, Edit[Tel_User]->text());
    else
        query = sql->SelectUser(Edit[ID_User]->text(),Edit[Name_User]->text(),Edit[Gender_User]->text(),Edit[Age_User]->text().toInt(), Edit[Tel_User]->text());
    ShowTable(query);
}

//表格显示数据
void UserManage::ShowTable(QSqlQuery query)
{
    //设置表头
    Table->setHorizontalHeaderLabels(QStringList()<<"卡号"<<"姓名"<<"性别"<<"年龄"<<"手机号"<<"是否挂失");
    if(!query.next())
    {
        Table->setRowCount(0);//表格设置行数
        return;
    }
    /*计算record表中数据行数*/
    query.last();//跳转到最后一条数据
    int nRow = query.at() + 1;//取所在行数
    Table->setRowCount(nRow);//表格设置行数
    int row = 0;
    query.first();//返回第一条数据
    do
    {
        for (int col = 0; col<7; col++)//按字段添加数据
        {
            //表格中添加数据库中的数据
            Table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;//行数增加
    }while(query.next());
}

//获取表格中某一行数据 显示在文本框内
void UserManage::get_table_line(int row, int col)
{
    for(int i = 0; i < Edit_Count_USER; i++)
    {
        Edit[i]->setText(Table->item(row,i)->text());
    }
    isLast = Table->item(row,Edit_Count_USER)->text();
}
//这是卡号
void UserManage::SetCard(QString cardID)
{
    Edit[ID_User]->setText(cardID);
}
//清空文本框
void UserManage::ClearEdit()
{
    for(int i = 0; i < Edit_Count_USER; i++)
    {
        Edit[i]->clear();
    }
}

//清空文本框和刷新表格
void UserManage::Clear()
{
    ClearEdit();
    ShowTable(sql->SelectUser());
}
