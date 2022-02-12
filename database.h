#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QObject>
#include <QSqlDatabase>

class DataBase : public QObject
{
    Q_OBJECT
public:

    explicit DataBase(QObject *parent = nullptr);
    bool createDataBaseTable();
    bool openDataBase();
    void closeDataBase();

    QSqlDatabase robotdb;
    //QSqlTableModel *tableModel;
    //QByteArray robotImage;
    //QByteArray robotTechnicDoc;

signals:

public slots:
    //void saveIntoTable();
private:


};

#endif // DATABASE_H
