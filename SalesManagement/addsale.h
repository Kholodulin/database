#ifndef ADDSALE_H
#define ADDSALE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

namespace Ui {
class AddSale;
}

class AddSale : public QWidget
{
    Q_OBJECT

public:
    explicit AddSale(QSqlDatabase _db,QWidget *parent = nullptr);
    ~AddSale();
    void closeEvent(QCloseEvent *e)override;

private:
    Ui::AddSale *ui;
    QString currentTabName;
    QSqlDatabase db;
    QSqlQuery *qry;

    QMap<QString,int>idAndNameProduct;
    QMap<QString,int>idAndNameCustomer;

    void productBoxFill();
    void customerBoxFill();
signals:
    void addWindowClosed(int id);
    void newItem();
private slots:
    void on_save_clicked();
    void on_checkCustomer_clicked(bool checked);
    void on_checkCurrentDate_clicked(bool checked);
};

#endif // ADDSALE_H
