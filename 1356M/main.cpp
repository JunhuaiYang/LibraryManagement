#include "widget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("图书管理系统");
    w.setWindowIcon(QIcon(":/img/img/book.jpg"));
    w.show();

    return a.exec();
}
