#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "peopledialog.h"
#include <QTableWidgetItem>

namespace Ui {
    class Profilewindow;
}



class Profilewindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Profilewindow(QWidget *parent = 0);
        ~Profilewindow();
        void setActions();
        //void setPersonalData(QHash<QString, QString> &);
        void setPersonalData(QSqlQueryModel *, QString &, QString &, int);
        bool createUserAvatar(QString &);
        void fixFriendList();
        void initFriendsFilter();


    private:
        Ui::Profilewindow *ui;
        QString userEmail;
        QString oldavatar;
        QVector<QTableWidgetItem *>friendsTableVector;
        QVector<QTableWidgetItem *>::Iterator fit;
        int user_id;

    public slots:
        bool addFriendsToUserFriendList();
        bool removeFriends();
        void setFriendSearch();

    private slots:
        void signOut();
        bool loadUserAvatar();
        bool showPeople();
        bool updateFriendsList();
};


#endif // PROFILEWINDOW_H
