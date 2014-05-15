#include "profilewindow.h"
#include "ui_profilewindow.h"
#include <QFileDialog>
#include <QDebug>


Profilewindow::Profilewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Profilewindow)
{
    ui -> setupUi(this);
}


Profilewindow::~Profilewindow()
{
    delete ui;
}


void Profilewindow::setPersonalData(QSqlQueryModel * personalDataModel, QString & email, QString & avatar, int user_id) {
    ui -> personalDataTableView -> setModel(personalDataModel);
    this -> userEmail = email;
    this -> oldavatar = avatar;
    this -> createUserAvatar(avatar);
    this -> user_id = user_id;
}


void Profilewindow::setActions() {
    QObject::connect( ui -> actionSign_out,       SIGNAL( triggered()          ), this, SLOT( signOut()           ) );
    QObject::connect( ui -> chooseAvatarBtn,      SIGNAL( clicked()            ), this, SLOT( loadUserAvatar()    ) );
    QObject::connect( ui -> findFriendsBtn,       SIGNAL( clicked()            ), this, SLOT( showPeople()        ) );
    QObject::connect( ui -> updateFriendsListBtn, SIGNAL( clicked()            ), this, SLOT( updateFriendsList() ) );
    QObject::connect( ui -> RemoveFriendBtn,      SIGNAL( clicked()            ), this, SLOT( removeFriends()     ) );
    QObject::connect( ui -> searchLine,           SIGNAL( textChanged(QString) ), this, SLOT( setFriendSearch()   ) );
}


void Profilewindow::signOut() {
    this -> close();
    MainWindow * mw = MainWindow::getInstance();
    mw -> show();
    mw -> fixLineEdits();
    mw -> clearSignInStatus();
}


bool Profilewindow::createUserAvatar(QString &ffp) {
    if ( ffp.isEmpty() || ffp.isNull() )
        ffp = "D:/C++ PROGRAMMING STUFF/QT Projects/someGUITest/defaultAvatar.png";

    QPixmap pix(ffp);
    ui -> avatarPlace -> setScaledContents(true);
    ui -> avatarPlace -> setPixmap(pix);
    return true;
}


bool Profilewindow::loadUserAvatar() {

    QString filefullPath = QFileDialog::getOpenFileName(this, "Load avatar");

    if ( filefullPath != NULL && this -> userEmail != NULL ) {
        this -> createUserAvatar(filefullPath);

        if ( MainWindow::getInstance()-> isConnectedToDB() ) {
            QSqlQuery query;

            query.prepare("UPDATE interns_main SET avatar_path = :avatar_path WHERE email = :email");

            query.bindValue(":avatar_path", filefullPath);
            query.bindValue(":email",       this -> userEmail);

            if ( !query.exec() ) {
                return false;
            }

            MainWindow::getInstance() -> closeDBConnection();
            return true;
        }
    }

    return false;
}


bool Profilewindow::showPeople() {
    if ( MainWindow::getInstance()-> isConnectedToDB() ) {
        QSqlQuery query;

        query.prepare("SELECT f_name, l_name, fb_link FROM interns_main WHERE id !=:user_id");
        query.bindValue(":user_id", this->user_id);

        if ( !query.exec() ) {
            return false;
        }

        QHash<QString, QString> peopleList;
        int i = 0;
        //query.first();
        while ( query.next() ) {
            peopleList.insert("FullFriendName"+i, query.value(0).toString() + " " + query.value(1).toString() );
            //qDebug() << query.value(0).toString();
            i++;
        }

        PeopleDialog * pf = new PeopleDialog(this);

        pf -> setModal(true);
        pf -> loadPeopleList(peopleList);
        pf -> recievedData(this -> user_id);
        pf -> setActions();
        pf -> exec();
        pf -> setAttribute(Qt::WA_DeleteOnClose);

       // MainWindow::getInstance() -> closeDBConnection();
        return true;
    }
    return false;
}


bool Profilewindow::addFriendsToUserFriendList() {
    if ( MainWindow::getInstance()-> isConnectedToDB() ) {
        QSqlQuery query;

        query.prepare("SELECT id, f_fname, f_lname FROM user_friends WHERE user_id=:user_id AND f_fname !='' AND f_lname !=''");

        query.bindValue(":user_id", user_id);

        if ( ! query.exec() ) {
            ui -> profiletestlabel -> setText( query.lastError().text() );
             qDebug() << query.value(0);
            return false;
        }//query.value(1).toString() + " " + query.value(2).toString()
        int i = 0;
        while (query.next()) {
            //qDebug() << i;
            ui -> friendsTable -> insertRow(i);
            ui -> friendsTable -> setItem(i, 0, new QTableWidgetItem(query.value(1).toString() + " " + query.value(2).toString()));
            i++;
        }
        ui -> friendsTable ->sortItems(0);
        MainWindow::getInstance() -> closeDBConnection();
        return true;
    }
    return false;
}



bool Profilewindow::updateFriendsList() {
    ui -> friendsTable -> clear();

    while ( ui -> friendsTable->rowCount() > 0)
    {
         ui -> friendsTable->removeRow(0);
    }

    if ( this -> addFriendsToUserFriendList() ) return true;
    return false;
}


bool Profilewindow::removeFriends() {
    /*QList<QListWidgetItem *> selectedFriends = ui -> userfriendsList -> selectedItems();

    QSqlQuery query;
    QStringList stringlist;

    if ( MainWindow::getInstance() -> isConnectedToDB() ) {

        for ( int i = 0; i != selectedFriends.count(); ++i ) {
            query.prepare("DELETE FROM user_friends WHERE user_id = :user_id AND f_fname = :f_fname AND f_lname = :f_lname");
            stringlist = selectedFriends.at(i) -> text().split(" ");

            qDebug() << this -> user_id;
            query.bindValue(":user_id", this->user_id);
            qDebug() << stringlist.at(0);
            query.bindValue(":f_fname", stringlist.at(0));
            qDebug() << stringlist.at(1);
            query.bindValue(":f_lname", stringlist.at(1));

            if ( !query.exec() ) {
                qDebug() << "query error: "+query.lastError().text();
            }
            qDebug() << query.lastError();

        }
        MainWindow::getInstance() -> closeDBConnection();
        if ( this -> updateFriendsList() ) return true;
        return false;
    }

    return false;*/
    return false;
}


void Profilewindow::fixFriendList() {
    ui -> friendsTable -> setSelectionMode(QAbstractItemView::ExtendedSelection);
    this -> initFriendsFilter();
}


void Profilewindow::initFriendsFilter() {
    if (!friendsTableVector.isEmpty()) friendsTableVector.clear();

    for (int i = 0; i < ui->friendsTable->rowCount(); ++i) {
        friendsTableVector.insert(i, ui->friendsTable->item(i, 0));
    }
}



void Profilewindow::setFriendSearch() {
    /*static int strIndex = 0;
    //int rowIndex = 0;
    static QString userTyped;
    static QRegExp lettersFilter = QRegExp("^[a-zA-Z]+$");
    userTyped = ui->searchLine->text();
    int i1 = 0, i = 0;

    if ( userTyped.contains(lettersFilter) ) {

        while ( true  )
        {
            if ( userTyped.at(strIndex) == ui->friendsTable->item(i, 0)->text().at(strIndex) )
            {
                qDebug() << ui->friendsTable->item(i, 0)->text();
            } else {

            }
            i1++; i++;
            if ( i1 == ui -> friendsTable->rowCount() ) {
                //ui -> friendsTable -> sortItems(0);
                break;
            }
        }
        //++strIndex;

    }*/
}
