#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDialog>



namespace Ui {
    class MainWindow;
}


class registration;

class MainWindow : public QMainWindow
{

    Q_OBJECT

    friend class registration;
    public:
        void setStartActions();
        void fixBugs();
        void correctPassEdit();        
        void fixLineEdits();
        bool isConnectedToDB();
        void closeDBConnection();
        void clearSignInStatus();

        static MainWindow * getInstance() {
            static MainWindow * inst_ptr;
            static MainWindow instance_;
            if ( inst_ptr == 0 ) {
                inst_ptr = &instance_;
            }
            return inst_ptr;
        }


        ~MainWindow();



    private:
         Ui::MainWindow * ui;
         registration   * regWin;
         QSqlDatabase     internsDBLink;
         MainWindow (MainWindow const &);
         void operator=(MainWindow const &);
         explicit MainWindow(QWidget *parent = 0);

    signals:

    private slots:
         void signInBtnClicked();
         void swictToRegisterWin();
         void exitApp();
};




namespace Ui {
    class registration;
}


class registration : public QDialog
{
    Q_OBJECT

    public:
        explicit registration(QWidget *parent = 0);
        ~registration();
        void setActions();
        void fixBugs();

    private:
        Ui::registration *ui;

    private slots:
        void swicthToMainWindow();
        bool registerUser();

};



#endif
