#include "borrow_return.h"

//借书界面
Borrow_Return::Borrow_Return(QWidget *parent) : QWidget(parent)
{
    QString LabelNameUser[] = {"卡号：", "姓名：", "性别：", "年龄：", "手机："}; //标签文本

    //布局
    QGridLayout *MainLayout = new QGridLayout();//主布局
    QVBoxLayout *UserLayout = new QVBoxLayout();//用户区域布局
    QVBoxLayout *RightLayout = new QVBoxLayout();//右侧布局
    QHBoxLayout *ButtonLayout = new QHBoxLayout();//右侧布局
    Status = new QLabel();  // 状态栏

    //组合框
    QGroupBox *BooksGroupBox = new QGroupBox();
    QGroupBox *UserGroupBox = new QGroupBox();

    sql = new Sqlite();

    //初始化文本框和标签 将文本框和标签添加到布局中
    for(int i = 0; i < Edit_Count_BORROW_RETURN; i++)
    {
        QHBoxLayout *Layout = new QHBoxLayout();
        Edit_User[i] = new QLineEdit();
        Label_User[i] = new QLabel(LabelNameUser[i]);
        Edit_User[i]->setFocusPolicy(Qt::NoFocus); //设置为禁止编辑
        Layout->addWidget(Label_User[i]);
        Layout->addWidget(Edit_User[i]);
        UserLayout->addLayout(Layout);
    }

    //借还书单选按钮
    Borrow = new QRadioButton("借书");
    Return = new QRadioButton("还书");
    Borrow->setChecked(true);
    Function = new QButtonGroup();
    Function->addButton(Borrow);//单选按钮加入按钮组
    Function->addButton(Return);

    ButtonLayout->addWidget(Borrow);
    ButtonLayout->addWidget(Return);
    UserLayout->addLayout(ButtonLayout);
    UserGroupBox->setTitle("用户信息");//设置标题
    UserGroupBox->setLayout(UserLayout);
    UserGroupBox->setFixedSize(200,300);//设置大小

    Table = new QTableWidget();//表格
    Table->setColumnCount(Table_Column_BORROW_RETURN);//设置列数
    Table->setSelectionBehavior ( QAbstractItemView::SelectRows);//选择方式为选中整行
    Table->setEditTriggers ( QAbstractItemView::NoEditTriggers );//不可编辑
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽度自适应

    RightLayout->addWidget(Table);

    BooksGroupBox->setTitle("借书列表");//设置组合框标题
    BooksGroupBox->setLayout(RightLayout);

    /*设置图片*/
    QLabel *Picture = new QLabel();
    QImage *jpg = new QImage(":/img/img/book.jpg");
    Picture->setPixmap(QPixmap::fromImage(*jpg));



    MainLayout->addWidget(UserGroupBox,0,0,1,1);
    MainLayout->addWidget(BooksGroupBox,0,1,2,1);
    MainLayout->addWidget(Picture,1,0,1,1);
    MainLayout->addWidget(Status,2,0,1,2);
    MainLayout->setSpacing(20);
    this->setLayout(MainLayout);

    // 先把表格静止
    Clear();

}
//表格显示
void Borrow_Return::ShowTable(QSqlQuery query)
{
    //设置表格表头
    Table->setHorizontalHeaderLabels(QStringList()<<"用户卡号"<<"书名"<<"作者"<<"出版社"<<"总数（本）"<<"剩余（本）");
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

//设置用户信息(卡ID)
void Borrow_Return::SetInfo(QString cardID)
{
    //将用户信息显示到文本框中
    QSqlQuery query = sql->SelectUser(cardID);
    if(query.next())//如果是用户
    {
        // 判断是否被挂失
        if(query.value(5).toString() == "true")
        {
            // 如果被挂失 弹出对话框
            QMessageBox::warning(parentWidget(), tr("警告！"), tr("该用户已经被挂失！\n该卡无法使用！！"), QMessageBox::Ok, QMessageBox::NoButton);
            Status->setText("登陆失败！   该卡已经被挂失！");
            return;
        }
        for(int i=0; i < Edit_Count_BORROW_RETURN; i++)
        {
            Edit_User[i]->setText(query.value(i).toString());
        }
        //将书信息显示到表格中
        ShowTable(sql->SelectBooksOfBorrow(cardID));//显示表格内容
        // 显示表格
        Table->setEnabled(true);
        // 更新状态栏
        Status->setText("登录成功！   请刷书籍卡借书或还书。");
        return;
    }
    query = sql->SelectBooks(cardID);
    if(query.next())//如果是书
    {
        // 如果未登录
        if(Edit_User[CardId_User_Borrow]->text().isEmpty())
        {
            Status->setText("该卡是书！  请先刷用户卡登陆！");
            return;
        }
        // 借书
        if(Borrow->isChecked())
        {
            // TODO 该书籍已被自己借出
            if(sql->SelectRecord(Edit_User[CardId_User_Borrow]->text(), query.value(0).toString()).next())
            {
                QMessageBox::information(parentWidget(), tr("提示！"), tr("该书籍已经被您借出\n若需要归还请点击还书按钮！！"),
                                     QMessageBox::Ok, QMessageBox::NoButton);
                return;
            }
            // TODO 已经被别人借出
            if(query.value(8).toString() == "是")
            {
                QMessageBox::information(parentWidget(), tr("提示！"), tr("该书籍已经被其他人借出\n借书失败！"),
                                          QMessageBox::Ok, QMessageBox::NoButton);
                return;
            }
            // 借书确认弹窗
            QMessageBox msg;
            QString bookinfo;
            bookinfo = "书名   ：  "+query.value(2).toString()+"\n"+
                       "作者   ：  "+query.value(3).toString()+"\n"+
                       "出版社 ：  "+query.value(4).toString()+"\n"+
                       "可借天数：  "+query.value(6).toString()+" 天\n"+
                    "请在到期前归还！\n\n是否借出？";
            int ret = msg.question(parentWidget(),tr("借书"), bookinfo, QMessageBox::Ok, QMessageBox::Cancel);
            // 确定
            if(ret == QMessageBox::Ok)
            {
                // 加入记录表
                if(sql->InsertRecord(query.value(0).toString(), Edit_User[CardId_User_Borrow]->text(), "sdf" ))
                //书籍的状态改为已借出
                sql->UpdataBooks(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                                 query.value(4).toString(),query.value(5).toString(),query.value(6).toString(), query.value(7).toString(), "是" );

            }

        }
        // 还书
        else
        {
            // TODO 该书籍没有被该用户借出
//            if(!sql->SelectRecord(Edit_User[CardId_User_Borrow]->text(), query.value(0).toString()).next())
//            {
//                QMessageBox::information(parentWidget(), tr("提示！"), tr("该书没有被您借出！\n还书失败！"),
//                                          QMessageBox::Ok, QMessageBox::NoButton);
//                return;
//            }
//                书籍状态改为已还书
            sql->UpdataBooks(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                             query.value(4).toString(),query.value(5).toString(),query.value(6).toString(), query.value(7).toString(), "否" );
            // TODO 更新表格
//            if(sql->DeleteRecord(Edit_User[CardId_User_Borrow]->text(), query.value(0).toString()))//将用户ID和书籍编号添加到数据表中
//            {
//            }
        }
        ShowTable(sql->SelectBooksOfBorrow(Edit_User[0]->text()));//显示表格内容
        return;
    }
    Status->setText("该卡没有在本系统中注册！");
}

//清空文本框和刷新表格
void Borrow_Return::Clear()
{
    // 进入未登录的状态
    for(int i = 0; i < Edit_Count_BORROW_RETURN; i++)
    {
        Edit_User[i]->clear();
    }
    ShowTable(sql->SelectBooksOfBorrow(Edit_User[0]->text()));//显示表格内容
    Table->setDisabled(true);

    // 设置状态栏
    Status->setText("请先刷卡登录！");

}
