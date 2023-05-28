#ifndef NEWITEM_H
#define NEWITEM_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>

namespace Ui {
class NewItem;
}

class NewItem : public QWidget
{
    Q_OBJECT

public:
    explicit NewItem(QSqlDatabase _db, QWidget *parent = nullptr);
    ~NewItem();
    void closeEvent(QCloseEvent *e)override;

private:
    Ui::NewItem *ui;
    QString currentTabName;
    QSqlDatabase db;
    QSqlQuery *qry;
    bool isAddCategory;

    QMap<QString,int>idAndNameCategory;
    void categoryBoxFill();
signals:
    void addWindowClosed(int id);
    void newItem();
private slots:
    void on_saveProduct_clicked();
    void on_addCategory_clicked();
    void on_delCategory_clicked();
};

#endif // NEWITEM_H
