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
    Info = new QLabel("欢迎：");
    Info->setMinimumHeight(200);
    Status->setMaximumHeight(30);

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
    Table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
    Table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列

    RightLayout->addWidget(Table);

    BooksGroupBox->setTitle("借书列表");//设置组合框标题
    BooksGroupBox->setLayout(RightLayout);

    MainLayout->addWidget(UserGroupBox,0,0,1,1);
    MainLayout->addWidget(BooksGroupBox,0,1,2,1);
    MainLayout->addWidget(Info,1,0,1,1);
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
    Table->setHorizontalHeaderLabels(QStringList()<<"是否在借"<<"图书卡号"<<"书名"<<"作者"<<"出版社"<<"借书时间"<<"还书时间"<<"已借时间");
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
    QString username = query.value(3).toString();
    QDateTime lendtime;
    QDateTime returntime;
    QString days;
    QString renting = "当前借书列表：\n";
    int count_rent = 0, overtime_count = 0;
    do
    {
        lendtime  = QDateTime::fromString(query.value(9).toString(), FORMAT);
        // 看是否已还
        if(query.value(1) == "否")
        {
            returntime = QDateTime::fromString(query.value(10).toString(), FORMAT);
            days = QString::asprintf("%-3d天(已完成)", GetHowManyDays(lendtime, returntime));
        }
        else {
            count_rent++;
            returntime = QDateTime::currentDateTime();
            int day = GetHowManyDays(lendtime, returntime);
//            qDebug() << days << "  " << day << returntime.toString()<< " lt" << lendtime.toString() ;
            // 超时
            if(day > query.value(11).toInt())
            {
                overtime_count++;
                days = QString::asprintf("%-3d天(已超时)", day);
                renting += ( query.value(6).toString() + "  已超时！超时 "+ QString::number(day - query.value(11).toInt())+ " 天\n");
            }
            else {
                days = QString::asprintf("%-3d天(正在借)", day);
                renting += ( "《"+ query.value(6).toString() + "》  还剩" + QString::number(query.value(11).toInt() - day)+ " 天\n" );
            }

        }
//        qDebug() << days;
        //表格中添加数据库中的数据
        Table->setItem(row, 1-1, new QTableWidgetItem(query.value(1).toString()));
        Table->setItem(row, 2-1, new QTableWidgetItem(query.value(5).toString()));
        Table->setItem(row, 3-1, new QTableWidgetItem(query.value(6).toString()));
        Table->setItem(row, 4-1, new QTableWidgetItem(query.value(7).toString()));
        Table->setItem(row, 5-1, new QTableWidgetItem(query.value(8).toString()));
        Table->setItem(row, 6-1, new QTableWidgetItem(query.value(9).toString()));
        Table->setItem(row, 7-1, new QTableWidgetItem(query.value(10).toString()));
        Table->setItem(row, 8-1, new QTableWidgetItem(days));  // 已借时间
        row++;//行数增加
    }while(query.next());
    // 更新 Info
    QString Infoma = QString::asprintf("欢迎：%s\n当前借书：%-2d本\n当前已超时:%-2d本\n\n", username.toStdString().c_str(), count_rent, overtime_count );
    if(count_rent){
        Infoma += renting;
    }
    Info->setText(Infoma);

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
        ShowTable(sql->SelectRecord(cardID));//显示表格内容
        // 显示表格
        Table->setEnabled(true);
        // 更新状态栏
        Status->setText("登录成功！   请刷书籍卡借书或还书。");
        return;
    }


    //如果是书
    query = sql->SelectBooks(cardID);
    if(query.next())
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
            if(sql->SelectRecord(Edit_User[CardId_User_Borrow]->text(), query.value(0).toString()).next(),true)
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
                if(!sql->InsertRecord(query.value(0).toString(), Edit_User[CardId_User_Borrow]->text(),
                                      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") ))
                    QMessageBox::critical(parentWidget(), tr("错误！"), tr("数据库操作错误！"), QMessageBox::Ok, QMessageBox::NoButton);
                //书籍的状态改为已借出
                if(!sql->UpdataBooks(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                                     query.value(4).toString(),query.value(5).toString(),query.value(6).toString(), query.value(7).toString(), "是" ))
                    QMessageBox::critical(parentWidget(), tr("错误！"), tr("数据库操作错误！"), QMessageBox::Ok, QMessageBox::NoButton);

                // TODO 写卡


            }

        }
        // 还书
        else
        {
            QSqlQuery record_query = sql->SelectRecord(Edit_User[CardId_User_Borrow]->text(), query.value(0).toString(), true);
            // TODO 该书籍没有被该用户借出
            if(!record_query.next())
            {
                QMessageBox::information(parentWidget(), tr("提示！"), tr("该书没有被您借出！\n还书失败！"),
                                          QMessageBox::Ok, QMessageBox::NoButton);
                return;
            }
            // 获取借书时间
            QDateTime lendtime = QDateTime::fromString(record_query.value(4).toString(), "yyyy-MM-dd hh:mm:ss");
            QDateTime currenttime = QDateTime::currentDateTime();

            //  弹窗确认还书信息
            QMessageBox msg;
            QString return_info;
            return_info = "书名   ：  "+query.value(2).toString()+"\n"+
                    "作者   ：  "+query.value(3).toString()+"\n"+
                    "出版社 ：  "+query.value(4).toString()+"\n"+
                    "可借天数：  "+query.value(6).toString()+" 天\n\n"+
                    "借书时间：  "+record_query.value(4).toString()+"\n"+
                    "已借时间：  "+ QString::number( GetHowManyDays(lendtime, currenttime) ) + " 天\n";
            if(GetHowManyDays(lendtime, currenttime) > query.value(6).toInt())
            {
                return_info += "状态    ：  已超时! \n\n";
            }
            else {
                return_info += "状态    ：  未超时 \n\n";
            }
            return_info += "请确认是否还书？";
            int ret = msg.question(parentWidget(),tr("还书"),return_info , QMessageBox::Ok, QMessageBox::Cancel);
            // 确定
            if(ret == QMessageBox::Ok)
            {
                // 更新表格
                if(!sql->UpdateRecord(record_query.value(0).toInt(), currenttime.toString("yyyy-MM-dd hh:mm:ss")))//更新还书时间
                {
                    QMessageBox::critical(parentWidget(), tr("错误！"), tr("数据库操作错误！"), QMessageBox::Ok, QMessageBox::NoButton);
                }

                //    书籍状态改为已还书
                if(!sql->UpdataBooks(query.value(0).toString(),query.value(1).toString(),query.value(2).toString(),query.value(3).toString(),
                                     query.value(4).toString(),query.value(5).toString(),query.value(6).toString(), query.value(7).toString(), "否" ))
                    QMessageBox::critical(parentWidget(), tr("错误！"), tr("数据库操作错误！"), QMessageBox::Ok, QMessageBox::NoButton);

            }
            // TODO 写卡


        }
        ShowTable(sql->SelectRecord(Edit_User[0]->text()));//显示表格内容
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
//    Edit_User[0]->setText("123");
    QSqlQuery empty;
    if(Edit_User[0]->text().isEmpty())
        ShowTable(empty);
    else
        ShowTable(sql->SelectRecord(Edit_User[0]->text()));//显示表格内容
    Table->setDisabled(true);

    // 设置状态栏
    Status->setText("请先刷卡登录！");

}

// 返回相差多少天
int Borrow_Return::GetHowManyDays(QDateTime start, QDateTime end)
{
    const int ndaysec = 24*60*60;
    uint stime = start.toTime_t();
    uint etime = end.toTime_t();
    return (etime - stime)/(ndaysec);
}
