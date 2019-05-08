#include "sqlite.h"

Sqlite::Sqlite()
{
}
/*连接数据库*/
bool Sqlite::Connect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATABASE);
    if(!db.open()) return false;
    QSqlQuery query;
    query.exec("create table user_15693 (cardID vchar, name vchar,  gender vchar, age int, telphone vchar(12), isLocked bool, primary key (cardID))");
    query.exec("create table books_15693 (booksID vchar, goodsID vchar, name vchar, author vchar, publishing_house vchar, book_type vchar, rent_days int, publishing_time vchar, isRent vchar, primary key (booksID))");
    query.exec("create table record_15693 (recordID integer PRIMARY KEY autoincrement , cardID vchar, booksID vchar, lend_time vchar, return_time vchar,isRenting vchar, FOREIGN KEY (cardID ) REFERENCES user_15693(cardID), FOREIGN KEY (booksID ) REFERENCES books_15693(booksID))");
    return true;
}
//打印SQL语句
bool Sqlite::ExecSQL(QString cmd)
{
    QSqlQuery query;
    qDebug()<<cmd.toUtf8().data();
    return query.exec(cmd);
}
//添加语句
bool Sqlite::Insert(QString table, QString value)
{
    QString cmd = "insert into " + table + " values(" + value + ");";
    return ExecSQL(cmd);
}
//删除语句
bool Sqlite::Delete(QString table, QString where)
{
    QString cmd = "delete from " + table + " where " + where + ";";
    return ExecSQL(cmd);
}
//修改语句
bool Sqlite::Updata(QString table, QString value,QString where)
{
    QString cmd = "update " + table + " set " + value +" where " + where + ";";
    return ExecSQL(cmd);
}
//查询语句
QSqlQuery Sqlite::Select(QString table, QString value, QString where)
{
    QString cmd;
    if(where.isEmpty())
    {
        cmd = "select " + value + " from " + table + ";";
    }
    else
    {
        cmd = "select " + value + " from " + table + " where " + where + ";";
    }
    QSqlQuery query;
    qDebug()<<cmd.toUtf8();
    query.exec(cmd);
    return query;
}
//向user表中添加
bool Sqlite::InsertUser(QString cardID, QString name, QString gender, int age, QString tel)
{
    return Insert("user_15693", "'"+cardID+"', '"+name+"', '"+gender+"', "+QString::number(age)+", '"+tel+ "', "+"'false'");
}
//向books表中添加
bool Sqlite::InsertBooks(QString booksID, QString goodsID, QString name, QString author, QString publishing_house, QString book_type, QString rent_days, QString publishing_time)
{
    return Insert("books_15693", "'" + booksID+"', '" + goodsID+ "', '"+name+"', '"+author+"', '"+publishing_house+ "', '"+ book_type+"', " + rent_days +", '"+publishing_time+"'"+", '否'");
}
//向record表中添加
bool Sqlite::InsertRecord(QString booksID, QString cardID, QString lend_time)
{
    return Insert("record_15693", "NULL, '"+cardID+"', '"+booksID+"', '"+lend_time+"', NULL, '是'");
}

//删除user表中数据
bool Sqlite::DeleteUser(QString cardID, QString name, QString gender, int age)
{    
    QString where;
    if( !cardID.isEmpty() )
        where += ("cardID = '" + cardID +"' ");
    return Delete("user_15693", where);
}
//删除books表中数据
bool Sqlite::DeleteBooks(QString booksID)
{
    QString where = "";
    if( !booksID.isEmpty() )
        where += ("booksID = '" + booksID +"' ");
    return Delete("books_15693", where);
}
//删除record表中数据
bool Sqlite::DeleteRecord(QString cardID, QString booksID)
{
    QString where;
    if( !cardID.isEmpty() )
        where += ("cardID = '" + cardID +"' ");
    if( !booksID.isEmpty() )
    {
        if(where.isEmpty())
            where += ("booksID = '" + booksID+"' ");
        else
            where += ("and booksID = '" + booksID+"' ");
    }
    return Delete("record_15693", where);
}
//修改user表中数据
bool Sqlite::UpdataUser(QString cardID, QString name, QString gender, int age, QString tel)
{
    return Updata("user_15693","cardID = '"+cardID+"', name = '"+name+"', gender = '"+gender+"', age = "+QString::number(age)+", telphone = '"+tel+"'", "cardID = '"+cardID+"'");
}
//修改books表中数据
bool Sqlite::UpdataBooks(QString booksID, QString goodsID, QString name, QString author, QString publishing_house, QString book_type, QString publishing_time, QString rent_days, QString isRent)
{
    if( isRent.isEmpty() )
        return Updata("books_15693","booksID = '"+booksID+"', name = '"+name+"', author = '"+author+"', publishing_house = '"+publishing_house+ "', goodsID = '"+goodsID+ "', book_type = '"+book_type+ "', rent_days = "+ rent_days+ ", publishing_time = '"+publishing_time+"'", "booksID = '"+booksID+"'");
    else
        return Updata("books_15693","booksID = '"+booksID+"', name = '"+name+"', author = '"+author+"', publishing_house = '"+publishing_house+ "', goodsID = '"+goodsID+ "', isRent = '"+isRent+"', book_type = '"+book_type+ "', publishing_time = '"+publishing_time+"'", "booksID = '"+booksID+"'");
}
//查询user表中数据
QSqlQuery Sqlite::SelectUser(QString cardID, QString name, QString gender, int age, QString telphone)
{
    QString where;
    if( !cardID.isEmpty() )
        where += ("cardID = '" + cardID +"' ");
    if( !name.isEmpty() )
    {
        if(where.isEmpty())
            where += ("name = '" + name+"' ");
        else
            where += ("and name = '" + name+"' ");
    }
    if( !gender.isEmpty() )
    {
        if(where.isEmpty())
            where += ("gender = '" + gender+"' ");
        else
            where += ("and gender = '" + gender+"' ");
    }
    if( !telphone.isEmpty() )
    {
        if(where.isEmpty())
            where += ("telphone = '" + telphone+"' ");
        else
            where += ("and telphone = '" + telphone+"' ");
    }
    if( age != -1 )
    {
        if(where.isEmpty())
            where += ("age = " + QString::number(age));
        else
            where += ("and age = " + QString::number(age));
    }

    return Select("user_15693", "*", where);
}
//查询books表中数据
QSqlQuery Sqlite::SelectBooks(QString booksID, QString name, QString author, QString publishing_house, QString book_type)
{
    QString where;
    if( !booksID.isEmpty() )
        where += ("booksID = '" + booksID +"' ");
    if( !name.isEmpty() )
    {
        if(where.isEmpty())
            where += ("name = '" + name+"' ");
        else
            where += ("and name = '" + name+"' ");
    }
    if( !author.isEmpty() )
    {
        if(where.isEmpty())
            where += ("author = '" + author+"' ");
        else
            where += ("and author = '" + author+"' ");
    }
    if( !publishing_house.isEmpty() )
    {
        if(where.isEmpty())
            where += ("publishing_house = '" + publishing_house+"' ");
        else
            where += ("and publishing_house = '" + publishing_house+"' ");
    }
    if( !book_type.isEmpty() )
    {
        if(where.isEmpty())
            where += ("book_type = '" + book_type+"' ");
        else
            where += ("and book_type = '" + book_type+"' ");
    }


    return Select("books_15693", "*", where);
}

// 查找用户借书
QSqlQuery Sqlite::SelectRecord(QString cardID, QString booksID, bool isRenting)
{
    QString where;
    if( !cardID.isEmpty() )
        where += ("cardID = '" + cardID +"' ");
    if( !booksID.isEmpty() )
    {
        if(where.isEmpty())
            where += ("booksID = '" + booksID+"' ");
        else
            where += ("and booksID = '" + booksID+"' ");
    }
    // 是否正在借阅
    if(isRenting)
        where += ("and isRenting = '是' ");
    return Select("record_15693", "*", where);
}

//查找借的书
QSqlQuery Sqlite::SelectBooksOfBorrow(QString cardID)
{
    return Select("books_15693", "*", "booksID in (select booksID from record_15693 where cardID = '"+cardID+"')");
}
