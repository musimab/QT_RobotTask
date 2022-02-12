#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QComboBox>
#include <QAbstractItemView>
#include <QSqlTableModel>
#include <QTableView>
#include "combodelegate.h"
#include "database.h"
#include <QToolBar>
#include <QTableWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    DataBase* sqlDb = new DataBase(this);

    checkDatabaseConnection();

    sqlDb->createDataBaseTable();


    connect(ui->robotNamelineEdit_3, &QLineEdit::textEdited, this, &Widget::autoFillRobotNames);

    createMaintanenceTable();

    connect(ui->saveButton, &QPushButton::clicked, this, &Widget::saveMaintanenceTabletoDb);

    connect(ui->scrollToBottomButton, &QPushButton::clicked, [=] {ui->tableWidget->scrollToBottom();});

    connect(ui->scrollToTopButton, &QPushButton::clicked, [=] {ui->tableWidget->scrollToTop();});


}
Widget::~Widget()
{
    delete ui;

}

void Widget::createMaintanenceTable() {

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(0);
    QStringList labels;
    labels  <<"NO" <<"BAKIM TÜRÜ" <<"PROSEDÜR" <<"PERIOD" <<"AÇIKLAMA" ;
    ui->tableWidget->setHorizontalHeaderLabels(labels);



}

void Widget::on_addRowButton_clicked()
{

    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    periodCombo = new QComboBox;
    periodCombo->insertItem(0,"Select");
    periodCombo->insertItem(1,"Ayda bir");
    periodCombo->insertItem(2,"3 ayda bir");
    periodCombo->insertItem(3,"6 ayda bir");
    periodCombo->insertItem(4,"Yılda bir");
    ui->tableWidget->setCellWidget(rowCount, 3, periodCombo);


}

void Widget::saveMaintanenceTabletoDb() {

    DataBase db;
    db.openDataBase();
    int rowCount = ui->tableWidget->rowCount();

    for(int i= 0; i<rowCount; i++) {

        No.append(ui->tableWidget->item(i,0)->text());

        maintenanceType.append(ui->tableWidget->item(i,1)->text());

        procedure.append(ui->tableWidget->item(i,2)->text());

        period.append(periodCombo->currentText());

        description.append(ui->tableWidget->item(i,4)->text());

    }

    QString kod = ui->kodLineEdit->text();
    for(int i= 0; i< rowCount; i++) {

        QSqlQuery qry;
        qry.prepare("INSERT INTO Maintenance (kod,No,MaintenanceType,Procedure,Period , Description) VALUES(:kod,:No,:MaintenanceType,:Procedure,:Period , :Description)");
        qry.bindValue(":kod", kod);
        qry.bindValue(":No", No[i]);
        qry.bindValue(":MaintenanceType", maintenanceType[i]);
        qry.bindValue(":Procedure", procedure[i]);
        qry.bindValue(":Period", period[i]);
        qry.bindValue(":Description", description[i]);
        if(qry.exec())
                qDebug() <<"Success to saved";
        else {
               QMessageBox::critical(this, tr("error"), qry.lastError().text());
                }

    }

    db.closeDataBase();
}



void Widget::checkDatabaseConnection()
{
    DataBase db;
    if(!db.openDataBase()) {
        ui->connectionLabel->setText(" Failed to open the database");
        return;
    }
    else{

        ui->connectionLabel->setText("Connected ...");
    }

}


void Widget::autoFillRobotNames()
{
    ui->robotNamelineEdit_2->setText(ui->robotNamelineEdit_3->text());
    ui->robotNamelineEdit->setText(ui->robotNamelineEdit_3->text());
}


void Widget::on_saveButton_clicked()
{

    DataBase db;
    QString robotName,robotInfoImage,robotInfoTechnicDoc, robotKod;
    u_int robotAge {0};

    robotKod = ui->kodLineEdit->text();
    robotName = ui->robotNamelineEdit_3->text();
    robotAge = ui->omurGenellineEdit->text().toUInt();
    robotInfoImage = ui->aciklamaResimlineEdit->text();
    robotInfoTechnicDoc = ui->aciklamaTekniklineEdit->text();

    db.openDataBase();

    QSqlQuery qry;
    qry.prepare("INSERT INTO RobotInfo (kod,name,age,imageInfo,imageData, techInfo, techDocument) VALUES(:kod, :name, :age, :imageInfo, :imageData, :techInfo, :techDocument)");
    qry.bindValue(":kod", robotKod);
    qry.bindValue(":name", robotName);
    qry.bindValue(":age", robotAge);
    qry.bindValue(":imageInfo", robotInfoImage);
    qry.bindValue(":imageData", robotImage);
    qry.bindValue(":techInfo", robotInfoTechnicDoc);
    qry.bindValue(":techDocument", robotTechnicDoc);

    if(qry.exec()) {
        QMessageBox::information(this, tr("Save"), tr("Saved"));
        db.closeDataBase();
    }
    else {
        QMessageBox::critical(this, tr("error"), qry.lastError().text());
        db.closeDataBase();
    }


}


void Widget::on_addImageButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", "/home/mustafa/Downloads/data/");
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly)) return;
    robotImage = file.readAll();

}

void Widget::on_addDocumentButton_clicked()
{

    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", "/home/mustafa/Downloads/data/");

    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly |  QFile::Text)) {
        return;
    }
    robotTechnicDoc = file.readAll();
    file.close();


}


void Widget::on_removeRowButton_clicked()
{

    int curRow= ui->tableWidget->currentIndex().row();

    //It's just deleted from the model and not submitted to the database yet.
    auto reply = QMessageBox::warning(this,QStringLiteral("Delete the current line"),
                                  QStringLiteral("Are you sure?"),
                                  QMessageBox::Yes,QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        qDebug() <<"OK" << reply;
        ui->tableWidget->removeRow(curRow);
        ui->connectionLabel->setText("Row removed");
    }


}


void Widget::on_tabWidget_currentChanged(int index)
{

    if(index !=3) {
        ui->mainnameLineEdit->setVisible(false);
        ui->mainnameLabel->setVisible(false);
    }
    else {
        ui->mainnameLineEdit->setVisible(true);
        ui->mainnameLabel->setVisible(true);
        ui->mainnameLineEdit->setText(ui->robotNamelineEdit_3->text());
    }

}

/*
void Widget::loadFromDbToTableView()
{
    DataBase db;
    if(db.openDataBase()) {

        tableModel = new QSqlTableModel(this);

        tableModel->setTable("Maintenance");
        tableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tableView->setModel(tableModel);

        // Set the size of the columns by content
        ui->tableView->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->resizeRowsToContents();
        ui->tableView->setColumnHidden(0, true);
        ComboDelegate * comboItemDel = new ComboDelegate (ui->tableView);
        ui->tableView->setItemDelegateForColumn(4,comboItemDel);
        tableModel->select();

        connect(tableModel, &QSqlTableModel::rowsInserted,[=]() {
            qDebug() <<"Row inserted ";
            ui->connectionLabel->setText("Row inserted");
        });

        connect(tableModel, &QSqlTableModel::rowsRemoved,[=]() {
            qDebug() <<"Row Removed";
            ui->connectionLabel->setText("Row removed");
        });




    }
    else {
        QMessageBox::information(this,"Failed", "connection failed");
    }


}
*/




void Widget::on_tableWidget_cellChanged(int row, int column)
{
    bool flag = ui->tableWidget->item(row, column)->text().isEmpty();

    if (!flag) /* the cell is not empty */
    {
        qDebug() <<"Not empty";

    }
    else /* the cell is empty */
    {
        qDebug() <<" empty";
        QMessageBox::critical(this, tr("empty"), tr("Empty cell"));

    }
}

