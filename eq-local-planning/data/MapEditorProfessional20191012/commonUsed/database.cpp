#include "database.h"

#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QtSql/QSqlRecord>
#include<QtSql/QSqlField>
#include<QtSql/QSqlDriver>
#include<QStringList>
#include<QDebug>
#include<QDateTime>
#include<QVector>
#include<QDateTime>

database::database()
{
    IsOpened=false;
}

void database::creatConnection(QString dbname)
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbname);
    }



    if (db.open())
    {

        QStringList tableList=db.tables();
        table_name=tableList[0];
        QSqlQuery query;
//        table_name="map"+QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

//        qDebug()<<table_name;
//        if(!tableList.contains(table_name))
//        {
//            QString create_sql = "create table "+table_name +" (roadIndex int,type int,x double,y double,heading double)";

//            query.prepare(create_sql);
//            if(!query.exec())
//            {
//                qDebug() << "Error: Fail to create table." << query.lastError();
//            }
//            else
//            {
//                qDebug() << "Table created!";
//            }
//        }
        IsOpened=true;
    }
}

void database::creatSaveConnection(QString dbname)
{
//    if (QSqlDatabase::contains("qt_sql_default_connection"))
//    {
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    }
//    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbname);
    }



    if (db.open())
    {

        QStringList tableList=db.tables();

        QSqlQuery query;
        table_name="map"+QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

        qDebug()<<table_name;
        if(!tableList.contains(table_name))
        {
            QString create_sql = "create table "+table_name +" (roadIndex int,type int,x double,y double,z double,heading double,roll double,pitch double)";

            query.prepare(create_sql);
            if(!query.exec())
            {
                qDebug() << "Error: Fail to create table." << query.lastError();
            }
            else
            {
                qDebug() << "Table created!";
            }
        }
        IsOpened=true;
    }
}

//void database::addData(QList<MAP_POINT> data)
//{
//    if(IsOpened)
//    {
//        QSqlDatabase::database().transaction();

//        for(int i=0;i<data.size();i++)
//        {
//            QSqlQuery sql_query;
//            QString insert_sql = "insert into "+table_name +" (roadIndex,type,x,y,heading) values (:roadIndex,:type,:x,:y,:heading)";

//            sql_query.prepare(insert_sql);
//            sql_query.bindValue(":roadIndex",data[i].roadIndex);
//            sql_query.bindValue(":type",data[i].type);
//            sql_query.bindValue(":x",data[i].point.x);
//            sql_query.bindValue(":y",data[i].point.y);
//            sql_query.bindValue(":heading",data[i].heading);

//            if(!sql_query.exec())
//            {
//                qDebug() << sql_query.lastError();
//            }
//            else
//            {
////                qDebug() << "inserted!";
//            }
//        }


//        isAddComplete=QSqlDatabase::database().commit();
//    }

//}

//QList<MAP_POINT> database::getData()
//{
//    QList<MAP_POINT> tempList;

//    if(IsOpened)
//    {
//         QSqlDatabase::database().transaction();

//        QSqlQuery sql_query;
//        sql_query.prepare("select * from "+table_name);
//        bool isSelect= sql_query.exec();
//        if(isSelect)
//        {
//            while(sql_query.next())
//            {
//                QSqlRecord record=sql_query.record();
//                MAP_POINT tempData;

//                tempData.roadIndex=record.value("roadIndex").toInt();
//                tempData.type=(TRACK_POINT_TYPE)(record.value("type").toInt());
//                tempData.point.x=record.value("x").toDouble();
//                tempData.point.y=record.value("y").toDouble();
//                tempData.heading=record.value("heading").toDouble();
//                tempList.append(tempData);
//            }
//        }
//        isReadComlete=QSqlDatabase::database().commit();
//    }
//    return tempList;

//}

void database::addData(QList<MAP_POINT> data)
{
    if(IsOpened)
    {
        QSqlDatabase::database().transaction();

        for(int i=0;i<data.size();i++)
        {
            QSqlQuery sql_query;
            QString insert_sql = "insert into "+table_name +" (roadIndex,type,x,y,z,heading,roll,pitch) values (:roadIndex,:type,:x,:y,:z,:heading,:roll,:pitch)";

            sql_query.prepare(insert_sql);
            sql_query.bindValue(":roadIndex",data[i].roadIndex);
            sql_query.bindValue(":type",data[i].type);
            sql_query.bindValue(":x",data[i].point.x);
            sql_query.bindValue(":y",data[i].point.y);
            sql_query.bindValue(":z",data[i].point.z);
            sql_query.bindValue(":heading",data[i].heading);
            sql_query.bindValue(":roll",data[i].roll);
            sql_query.bindValue(":pitch",data[i].pitch);

            if(!sql_query.exec())
            {
                qDebug() << sql_query.lastError();
            }
            else
            {
                qDebug() << "inserted!";
            }
        }


        isAddComplete=QSqlDatabase::database().commit();
    }

}

QList<MAP_POINT> database::getData()
{
    QList<MAP_POINT> tempList;

    if(IsOpened)
    {
         QSqlDatabase::database().transaction();

        QSqlQuery sql_query;
        sql_query.prepare("select * from "+table_name);
        bool isSelect= sql_query.exec();
        if(isSelect)
        {
            while(sql_query.next())
            {
                QSqlRecord record=sql_query.record();
                MAP_POINT tempData;

                tempData.roadIndex=record.value("roadIndex").toInt();
                tempData.type=(TRACK_POINT_TYPE)(record.value("type").toInt());
                tempData.point.x=record.value("x").toDouble();
                tempData.point.y=record.value("y").toDouble();
                tempData.point.z=record.value("z").toDouble();
                tempData.heading=record.value("heading").toDouble();
                tempData.roll=record.value("roll").toDouble();
                tempData.pitch=record.value("pitch").toDouble();
                tempData.curvature=0;
                tempList.append(tempData);
            }
        }
        isReadComlete=QSqlDatabase::database().commit();
    }
    return tempList;

}

void database::close()
{
    db.close();
}
