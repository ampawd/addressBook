#include "mainwindow.h"
#include "profilewindow.h"
#include "ui_mainwindow.h"
#include "ui_registration.h"
#include <QMessageBox>
#include <QFileSystemModel>

//#include <QRegExpValidator>
//#include <QRegExp>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
    internsDBLink = QSqlDatabase::addDatabase("QSQLITE");
    internsDBLink.setDatabaseName("C:/sqllite/internship6.db");

    if ( !internsDBLink.open() ) {
        ui -> signInStatus -> setText("connection to database was not estabslihed!");
        return;
    } else {
        //ui -> signInStatus -> setText("Connected!");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    //if ( internsDBLink.isOpen() ) internsDBLink.close();
    internsDBLink.removeDatabase(QSqlDatabase::defaultConnection);    
}


void MainWindow::fixBugs() {
    this -> correctPassEdit();
    this -> fixLineEdits();
    this -> clearSignInStatus();
}


void MainWindow::correctPassEdit()
{
    ui -> pass_sign_in -> setEchoMode(QLineEdit::Password);
}


void MainWindow::fixLineEdits() {
    ui -> email_sign_in -> clear();
    ui -> pass_sign_in  -> clear();
    ui -> email_sign_in -> setFocus();
}


void MainWindow::clearSignInStatus() {
    ui -> signInStatus -> clear();
}


bool MainWindow::isConnectedToDB() {
    return this -> internsDBLink.open();
}


void MainWindow::closeDBConnection() {
    this -> internsDBLink.close();
}


void MainWindow::signInBtnClicked()
{
    if ( !internsDBLink.open() ) { return; }

    QString email, pass;
    QSqlQuery query;
    QByteArray qbapass;
    QRegExp mailRegExp = QRegExp("^[-\\w.]+@([A-z0-9][-A-z0-9]+\\.)+[A-z]{2,4}$");
    //QRegExp passRegExp = QRegExp("(?=^.{4,}$)((?=.*\\d)|(?=.*\\W+))(?![.\\n])(?=.*[A-Z])(?=.*[a-z]).*$");

    int rowsCount = 0;

    email = ui -> email_sign_in -> text();
    pass  = ui -> pass_sign_in  -> text();

    if ( email.isEmpty() || pass.isEmpty() ||!email.contains(mailRegExp) ) {
        QMessageBox::critical( this, tr("Sign in error:"), "Incorrrect email or password!" );
        return;
    }

    qbapass.append(pass);
    QString HashedPass = QString( QCryptographicHash::hash((qbapass), QCryptographicHash::Md5).toHex() );

    query.prepare("SELECT f_name, l_name, address, phone_number, fb_link FROM interns_main WHERE email=:email AND pass=:pass");

    query.bindValue(":email", email);
    query.bindValue(":pass",  HashedPass);

    if ( query.exec() ) {
        while( query.next() ) { rowsCount++; }
    } else {
        ui -> signInStatus -> setText( query.lastError().text() );
        return;
    }


    // if user email and pass are correct hiding sign in form and loading data
    if ( rowsCount == 1 ) {
        query.first();

        QSqlQueryModel * peopleListModel = new QSqlQueryModel;

        peopleListModel -> setQuery( query );

        query.prepare("SELECT id, avatar_path FROM interns_main WHERE email = :email");
        query.bindValue(":email", email);

        QString avatar_path;
        int user_id;
        if ( !query.exec() ) { return; }
        while ( query.next() ) {
            avatar_path = query.value(1).toString();
            user_id = query.value(0).toInt();
        }

        Profilewindow * profileWin = new Profilewindow;
        profileWin -> show();
        profileWin -> setActions();
        profileWin -> setPersonalData(peopleListModel, email, avatar_path, user_id);
        profileWin -> addFriendsToUserFriendList();
        profileWin -> fixFriendList();
        profileWin -> setAttribute(Qt::WA_DeleteOnClose);
        this -> hide();
        return;
    } else {
        //ui -> signInStatus -> setText("wroung email or password!");
        QMessageBox::critical( this, tr("Sign in error:"), "Incorrrect email or password!" );
        return;
    }
}


void MainWindow::swictToRegisterWin()
{
    regWin = new registration;
    regWin -> setModal(true);
    regWin -> setActions();
    regWin -> fixBugs();
    regWin -> exec();
    delete regWin;
}


void MainWindow::exitApp() {
    qApp -> exit();
}


void MainWindow::setStartActions()
{
    QObject::connect( ui -> registerBtn, SIGNAL( clicked()   ), this, SLOT( swictToRegisterWin() ) );
    QObject::connect( ui -> sign_inBtn,  SIGNAL( clicked()   ), this, SLOT( signInBtnClicked()   ) );
    QObject::connect( ui -> actionExit,  SIGNAL( triggered() ), this, SLOT( exitApp()            ) );
}




registration::registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registration)
{
    ui -> setupUi(this);
}


registration::~registration()
{
    delete ui;
}


void registration::setActions()
{
    QObject::connect( ui -> sign_inWinBtn, SIGNAL( clicked() ), this, SLOT ( swicthToMainWindow()) );
    QObject::connect( ui -> registerBtn,   SIGNAL( clicked() ), this, SLOT ( registerUser()      ) );
}


void registration::fixBugs() {
    ui -> fname_register -> setFocus();
}


void registration::swicthToMainWindow()
{
    this -> hide();
    MainWindow * mw = MainWindow::getInstance();
    mw -> fixBugs();
}


bool registration::registerUser()
{
    MainWindow * mw = MainWindow::getInstance();
    if ( !mw -> internsDBLink.open() ) return false;

    QString f_name, l_name, email, pass;
    f_name       = ui -> fname_register -> text();
    l_name       = ui -> lname_register -> text();
    email        = ui -> email_register -> text();
    pass         = ui -> pass_register  -> text();

    if ( f_name.isEmpty() || l_name.isEmpty() || email.isEmpty() || pass.isEmpty() ) {
        QMessageBox::critical( this, tr("Validation error:"), "First name, last name email and pass should not be empty!" );
        return false;
    }

    QRegExp mailRegExp        = QRegExp("^[-\\w.]+@([A-z0-9][-A-z0-9]+\\.)+[A-z]{2,4}$");
    QRegExp flnames           = QRegExp("^[a-zA-Z][a-zA-Z0-9-_\\.]{1,20}$");
    QRegExp phoneNumberRegExp = QRegExp("^(0)$|^([1-9][0-9]*)$");
    //QRegExp passRegExp        = QRegExp("(?=^.{4,}$)((?=.*\\d)|(?=.*\W+))(?![.\\n])(?=.*[A-Z])(?=.*[a-z]).*$");
    QRegExp restRegExp        = QRegExp("^[a-zA-Z][a-zA-Z0-9-_\\.]{1,20}$");


    if (       !ui->email_register->text().contains(mailRegExp)
            || !ui->fname_register->text().contains(flnames)
            || !ui->lname_register->text().contains(flnames)
            || !ui->phone_register->text().contains(phoneNumberRegExp)
            || !ui->address_register->text().contains(restRegExp)
       )
    {
        QMessageBox::critical( this, tr("Incorrect data error:"), tr("You filled out incorrect data !") );
        return false;
    }


    QString phone_number, address, fb_link;
    QByteArray qbapass;
    QSqlQuery insertQuery;

    phone_number = ui -> phone_register   -> text();
    address      = ui -> address_register -> text();
    fb_link      = ui -> fb_link_register -> text();

    qbapass.append(pass);
    QString HashedPass = QString(QCryptographicHash::hash((qbapass),QCryptographicHash::Md5).toHex());

    insertQuery.prepare("INSERT INTO interns_main (f_name, l_name, email, pass, phone_number, address, fb_link, avatar_path) VALUES(:f_name, :l_name, :email, :pass, :phone_number, :address, :fb_link, :avatar_path)");

    insertQuery.bindValue(":f_name",       f_name);
    insertQuery.bindValue(":l_name",       l_name);
    insertQuery.bindValue(":email",        email);
    insertQuery.bindValue(":pass",         HashedPass);
    insertQuery.bindValue(":phone_number", phone_number);
    insertQuery.bindValue(":address",      address);
    insertQuery.bindValue(":fb_link",      fb_link);
    insertQuery.bindValue(":avatar_path",  "");

    if ( !insertQuery.exec() ) {
        QMessageBox::critical( this, tr("Registration error:"), "Same person cannot be registereded twice!" );
        return false;
    } else {
        QMessageBox::critical( this, tr("save"), tr("You have registered") );
        return true;
    }

    return false;
}
