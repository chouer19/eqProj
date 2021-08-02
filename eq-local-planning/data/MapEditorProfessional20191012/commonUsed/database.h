#ifndef DATABASE_H
#define DATABASE_H

#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlRecord>
#include"typedefs.h"


class database
{
public:
    database();

    void creatConnection(QString dbname);
    void creatSaveConnection(QString dbname);
    void addData(QList<MAP_POINT> data);
    QList<MAP_POINT> getData();
    void close();

    QSqlDatabase db;
    bool IsOpened;
    bool isAddComplete;
    bool isReadComlete;
    int num_frame;
private:


    QString table_name;
};

#endif // DATABASE_H
