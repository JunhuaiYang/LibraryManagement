#include "booksmanage.h"

BooksManage::BooksManage(QWidget *parent) : QWidget(parent)
{
    QString LabelName[] = {"卡号：", "书籍编号：", "书名：", "作者：", "出版社：","图书类型：","可借天数：",  "出版时间:"};//标签文本
    QString ButtonName[] = {"添加", "删除", "修改", "搜索"};//按钮文本
    QVBoxLayout *MainLayout = new QVBoxLayout();//主布局
    QHBoxLayout *ButtonLayout = new QHBoxLayout();//按钮布局
    QHBoxLayout *EditLayout = new QHBoxLayout();//文本框布局
    QHBoxLayout *TableLayout = new QHBoxLayout();//表格布局
    QGroupBox *BookTable = new QGroupBox();//表格区域
    QGroupBox *BookInfo = new QGroupBox();//信息
    sql = new Sqlite();

    for(int i = 0; i < Edit_Count_BOOKS; i++) //初始化文本框和标签
    {
        Edit[i] = new QLineEdit();
        Label[i] = new QLabel(LabelName[i]);
        EditLayout->addWidget(Label[i]);//将文本框和标签添加到布局中
        EditLayout->addWidget(Edit[i]);
    }
    // 日期选择
    Label[Edit_Count_BOOKS] = new QLabel(LabelName[Edit_Count_BOOKS]);
    DateEdit = new QDateEdit();
    DateEdit->setDisplayFormat("yyyy-MM-dd");
    EditLayout->addWidget(Label[Edit_Count_BOOKS]);//将文本框和标签添加到布局中
    EditLayout->addWidget(DateEdit);

    // 正则表达式
    QString pattern("[A-Fa-f9-0]*");
    QRegExp regExp(pattern);
    Edit[ID_Books]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[9-0]{13}");
    Edit[GoodsID]->setValidator(new QRegExpValidator(regExp, this));

    regExp.setPattern("[9-0]{3}");
    Edit[RentDays]->setValidator(new QRegExpValidator(regExp, this));


    BookInfo->setLayout(EditLayout);//设置信息组合框的布局

    for(int i = 0; i < Button_Count_BOOKS; i++)//初始化按钮
    {
        Button[i] = new QPushButton();
        Button[i]->setText(ButtonName[i]);
        ButtonLayout->addWidget(Button[i]);//按钮添加到布局中
    }
    ButtonLayout->addStretch(0);
    ButtonLayout->setSpacing(20);

    Table = new QTableWidget();
    Table->setColumnCount(Table_Column_BOOKS);
    Table->setSelectionBehavior ( QAbstractItemView::SelectRows);//选中整行
    Table->setEditTriggers ( QAbstractItemView::NoEditTriggers );//不可编辑
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列宽度自适应
    TableLayout->addWidget(Table);

    BookTable->setLayout(TableLayout);
    BookTable->setTitle("图书列表");

    MainLayout->addWidget(BookInfo);
    MainLayout->addLayout(ButtonLayout);
    MainLayout->addWidget(BookTable);
    MainLayout->setSpacing(10);
    this->setLayout(MainLayout);
    SetSlot();
}

void BooksManage::SetSlot()//设置槽函数
{
    connect(Button[Add_Books],SIGNAL(clicked()),this,SLOT(add_books()));//添加按钮连接槽函数add_books()
    connect(Button[Delete_Books],SIGNAL(clicked()),this,SLOT(delete_books()));//删除按钮连接槽函数delete_books()
    connect(Button[Updata_Books],SIGNAL(clicked()),this,SLOT(updata_books()));//修改按钮连接槽函数updata_books()
    connect(Button[Select_Books],SIGNAL(clicked()),this,SLOT(select_books()));//查找按钮连接槽函数select_books()
    connect(Table,SIGNAL(cellClicked(int,int)),this,SLOT(get_table_line(int, int)));//表格单击事件连接槽函数get_table_line(int, int)
}

void BooksManage::add_books()//添加按钮槽函数
{
    /*文本框为空时显示错误提示*/
    QString LabelName[] = {"卡号：", "书籍编号：", "书名：", "作者：", "出版社：","图书类型：","可借天数：",  "出版时间:"};//标签文本
    for(int i = 0; i < Edit_Count_BOOKS-1; i++)
    {
        if(Edit[i]->text().isEmpty())
        {
            QMessageBox::warning(NULL, "warning", LabelName[i]+"不能为空！", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
    }
    if (sql->SelectUser(Edit[ID_Books]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号已经注册为用户！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }



    //向数据库中添加书籍
    bool ret = sql->InsertBooks(Edit[ID_Books]->text(), Edit[GoodsID]->text(),Edit[Name_Books]->text(),Edit[Author_Books]->text(),Edit[PublishingHouse_Books]->text(), Edit[Type_Books]->text(),Edit[RentDays]->text(), DateEdit->text());
    if(!ret)
    {
        QMessageBox::warning(NULL, "warning", "添加失败，卡号已存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(NULL, "warning", "添加成功！", QMessageBox::Yes, QMessageBox::Yes);
    ClearEdit();    //清空文本框
    ShowTable(sql->SelectBooks());//更新表格
}

//删除按钮槽函数
void BooksManage::delete_books()
{    
    if (!Edit[ID_Books]->text().isEmpty() && sql->SelectUser(Edit[ID_Books]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号已经注册为用户！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!Edit[ID_Books]->text().isEmpty() && !sql->SelectBooks(Edit[ID_Books]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号不存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    // 先删除记录表中的，解除依赖关系
    sql->DeleteRecordBook(Edit[ID_Books]->text());
    //删除书籍
    bool ret = sql->DeleteBooks(Edit[ID_Books]->text());
    if(!ret)
    {
        QMessageBox::warning(NULL, "warning", "删除失败！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(NULL, "warning", "删除成功！", QMessageBox::Yes, QMessageBox::Yes);
    ClearEdit();//清空文本框
    ShowTable(sql->SelectBooks());//更新表格
}

//修改按钮单击事件
void BooksManage::updata_books()
{    
    if (!Edit[ID_Books]->text().isEmpty() && sql->SelectUser(Edit[ID_Books]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号已经注册为用户！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if (!Edit[ID_Books]->text().isEmpty() && !sql->SelectBooks(Edit[ID_Books]->text()).next())
    {
        QMessageBox::warning(NULL, "warning", "卡号不存在！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //修改书籍信息
    bool ret = sql->UpdataBooks(Edit[ID_Books]->text(),Edit[GoodsID]->text(),Edit[Name_Books]->text(),Edit[Author_Books]->text(),Edit[PublishingHouse_Books]->text(),Edit[Type_Books]->text(),DateEdit->text(), Edit[RentDays]->text());
    if(!ret)
    {
        QMessageBox::warning(NULL, "warning", "修改失败！", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QMessageBox::warning(NULL, "warning", "修改成功！", QMessageBox::Yes, QMessageBox::Yes);
    ClearEdit();//清空文本框
    ShowTable(sql->SelectBooks());//更新表格
}

//搜索按钮单击事件
void BooksManage::select_books()
{
    QSqlQuery query;
    query = sql->SelectBooks(Edit[ID_Books]->text(),Edit[Name_Books]->text(),Edit[Author_Books]->text(),Edit[PublishingHouse_Books]->text(), Edit[Type_Books]->text());
    ShowTable(query);//更新表格
    ClearEdit();//清空文本框
}

//显示表格
void BooksManage::ShowTable(QSqlQuery query)
{
    Table->setHorizontalHeaderLabels(QStringList()<<"卡号"<<"书籍ISBN号"<<"书名"<<"作者"<<"出版社"<<"书籍类型"<<"可借时间(天)"<<"出版日期"<<"是否借出");
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
        for (int col = 0; col<Table->columnCount(); col++)//按字段添加数据
        {
            //表格中添加数据库中的数据
            Table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;//行数增加
    }while(query.next());
}

//清空文本框
void BooksManage::ClearEdit()
{
    for(int i = 0; i < Edit_Count_BOOKS; i++)
    {
        Edit[i]->clear();
    }
}

//单击表格 在文本框中显示表格点击的行的数据
void BooksManage::get_table_line(int row, int col)
{
    for(int i = 0; i < Edit_Count_BOOKS; i++)
    {
        Edit[i]->setText(Table->item(row,i)->text());
    }
    // 日期更新
//    qDebug()<< Table->item(row, Edit_Count_BOOKS)->text() << endl;
//    qDebug()<< QDate::fromString("2000/01/10", "yyyy/MM/dd").toString() << endl;
    DateEdit->setDate( QDate::fromString(Table->item(row, Edit_Count_BOOKS)->text(), "yyyy-MM-dd") );

}

void BooksManage::SetCard(QString cardID)
{
    Edit[ID_Books]->setText(cardID);
}

//清空文本框和更新表格
void BooksManage::Clear()
{
    ClearEdit();
    ShowTable(sql->SelectBooks());
}
