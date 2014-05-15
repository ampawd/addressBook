#include "peopledialog.h"
#include "ui_peopledialog.h"
#include <QPushButton>
#include <QVBoxLayout>


PeopleDialog::PeopleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeopleDialog)
{
    ui->setupUi(this);
}


PeopleDialog::~PeopleDialog()
{
    delete ui;
}


void PeopleDialog::setActions() {
    QObject::connect( ui -> addAsFriendBtn, SIGNAL( clicked() ), this, SLOT( addFriends()) );
}


void PeopleDialog::recievedData(int user_id) {
    this->user_id = user_id;
}


bool PeopleDialog::addFriends() {
    QList<QListWidgetItem *> selectedPeople = ui -> findedPeopleList -> selectedItems();
    int i = 0;

    QSqlQuery query;
    QStringList stringlist;

    if ( MainWindow::getInstance() -> isConnectedToDB() ) {

        for (; i < selectedPeople.count(); ++i) {
            stringlist = selectedPeople.at(i) -> text().split(" ");

            query.prepare("INSERT INTO user_friends(user_id, f_fname, f_lname) VALUES(:user_id, :f_fname, :f_lname)");
            query.bindValue(":user_id", this -> user_id );
            query.bindValue(":f_fname", stringlist.at(0));
            query.bindValue(":f_lname", stringlist.at(1));

            if ( !query.exec() ) {
                qDebug() << query.lastError().text();
            }
        }

        //MainWindow::getInstance() -> closeDBConnection();
        return true;
    }

    return false;
}



void PeopleDialog::loadPeopleList(QHash<QString, QString> & peopleList) {
    int j = 0;
    QHash<QString, QString>::iterator i;

    for (i = peopleList.begin(); i != peopleList.end(); ++i, j++) {
         ui -> findedPeopleList -> addItem( peopleList.value("FullFriendName"+j) );

    }

    ui -> findedPeopleList -> setSelectionMode(QAbstractItemView::ExtendedSelection);
}
