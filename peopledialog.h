#ifndef PEOPLEDIALOG_H
#define PEOPLEDIALOG_H

#include <QDialog>
#include "profilewindow.h"


namespace Ui {
    class PeopleDialog;
}


class PeopleDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PeopleDialog(QWidget *parent = 0);
        ~PeopleDialog();
        void loadPeopleList(QHash<QString, QString> &);
        void setActions();
        void recievedData(int);

    private:
        Ui::PeopleDialog *ui;
        int user_id;


    private slots:
        bool addFriends();
};


#endif // PEOPLEDIALOG_H
