#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void checkDatabaseConnection();
    void createMaintanenceTable();
    void saveMaintanenceTabletoDb();
    //bool is_MaintenanceCellItemEmpyt();
    QByteArray robotImage;
    QByteArray robotTechnicDoc;
    QStringList Kod, No,maintenanceType,procedure, period, description;
    QComboBox *periodCombo;
signals:


private slots:


    void autoFillRobotNames();

    void on_saveButton_clicked();

    void on_addImageButton_clicked();

    void on_addDocumentButton_clicked();

    void on_addRowButton_clicked();

    void on_removeRowButton_clicked();

    void on_tabWidget_currentChanged(int index);


    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
