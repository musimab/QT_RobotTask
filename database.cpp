#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}


bool DataBase::createDataBaseTable()
{

    if(openDataBase()) {
       QSqlQuery query = QSqlQuery( this->robotdb );
       query.exec( "CREATE TABLE IF NOT EXISTS RobotInfo (kod TEXT ,name TEXT,age INTEGER,imageInfo TEXT,imageData BLOB, techInfo TEXT, techDocument BLOB )" );
       //query.exec( "CREATE TABLE IF NOT EXISTS Maintenance (kod TEXT, No INTEGER, MaintenanceType TEXT,Procedure TEXT,Period TEXT,Description TEXT)" );
       query.exec( "CREATE TABLE IF NOT EXISTS Maintenance (kod TEXT, No INTEGER, MaintenanceType TEXT,Procedure TEXT,Period TEXT,Description TEXT)" );

       return true;
    }
    else {
        qDebug() <<" Failed to open database";

    }
    return false;


}



bool DataBase::openDataBase()
{
    robotdb = QSqlDatabase::addDatabase("QSQLITE");
    robotdb.setDatabaseName("/home/mustafa/Desktop/altınay/DB/myDatabase.db3");
    if(!robotdb.open()) {
        qDebug() << ("Failed to open the database");
        return false;
    }
    else {
        qDebug() << "Connected...";
        return true;
    }
}

void DataBase::closeDataBase()
{
    robotdb.close();
    robotdb.removeDatabase(QSqlDatabase::defaultConnection);
}
