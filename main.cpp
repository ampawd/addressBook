#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication AddressBook(argc, argv);
    MainWindow * myMainWindow = MainWindow::getInstance();

    myMainWindow -> fixBugs();
    myMainWindow -> setStartActions();
    myMainWindow -> show();
    myMainWindow -> setAttribute(Qt::WA_QuitOnClose);

    return AddressBook.exec();
}
