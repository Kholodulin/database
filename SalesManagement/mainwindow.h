#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "newitem.h"
#include "addsale.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlQuery>
#include <QCloseEvent>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e)override;

private slots:
    void on_select_table_currentIndexChanged(int index);
    void on_editMain_clicked();
    void on_delMain_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_refreshMain_clicked();

    void on_btnSearch_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel* model;
    QSqlQuery *qry;
    int currentRow;
    QString curTableName;

    NewItem *newItem;
    AddSale *addSale;
    bool isProductAddOpen;
    bool isSaleAddOpen;
    void setProductHeaderTitle();
    void setSaleHeaderTitle();
    void setCustomerHeaderTitle();
public slots:
    void addWindowClosedSlot(int id);
    void newItemSlot();
};
#endif // MAINWINDOW_H
