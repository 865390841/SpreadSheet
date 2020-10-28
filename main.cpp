#include "mainwindow.h"
#include <QApplication>
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include <QSplashScreen>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splash=new QSplashScreen();
    splash->setPixmap(QPixmap(":/images/start"));
    splash->show();
    MainWindow w;
    splash->finish(&w);
    w.showMaximized();
//    FindDialog *fd=new FindDialog;
//    fd->show();
//    GoToCellDialog gd;
//    gd.show();
//    SortDialog sd;
//    sd.show();
    return a.exec();
}
