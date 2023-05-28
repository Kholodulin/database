#include "addsale.h"
#include "ui_addsale.h"

AddSale::AddSale(QSqlDatabase _db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSale)
{
    ui->setupUi(this);
    currentTabName = "sale";
    db = _db;
    qry = new QSqlQuery;
    setWindowTitle(currentTabName+" edit");
    productBoxFill();
    customerBoxFill();
    ui->spinPrice->setMaximum(999999999);
    ui->spinAmount->setMaximum(999999999);
    ui->checkCustomer->setCheckState(Qt::Checked);
}

AddSale::~AddSale()
{
    delete ui;
}

void AddSale::closeEvent(QCloseEvent *e)
{
    emit addWindowClosed(1);
}

void AddSale::productBoxFill()
{
    ui->boxProduct->clear();
    qry->exec("SELECT product_id, model FROM product");
    while (qry->next()) {
        int id = qry->value(0).toInt();
        QString name = qry->value(1).toString();
        ui->boxProduct->addItem(name);
        idAndNameProduct.insert(name,id);
    }
}
void AddSale::customerBoxFill()
{
    ui->boxCustomer->clear();
    qry->exec("SELECT customer_id, customer_name FROM customer");
    while (qry->next()) {
        int id = qry->value(0).toInt();
        QString name = qry->value(1).toString();
        ui->boxCustomer->addItem(name);
        idAndNameCustomer.insert(name,id);
    }
}

void AddSale::on_save_clicked()
{
    qry->prepare("INSERT INTO sale (product_id, customer_id, unit_price,quantity, total_price,date) "
        "VALUES (?, ?, ?, ?, ?, ?)");
    qry->addBindValue(idAndNameProduct.find(ui->boxProduct->currentText()).value());
    !ui->checkCustomer->isChecked()?qry->addBindValue(QVariant()):
        qry->addBindValue(idAndNameCustomer.find(ui->boxCustomer->currentText()).value());
    qry->addBindValue(ui->spinPrice->value());
    qry->addBindValue(ui->spinAmount->value());
    qry->addBindValue((ui->spinPrice->value()) * (ui->spinAmount->value()));
    ui->checkCurrentDate->isChecked()?qry->addBindValue(QDate::currentDate())
                                    : qry->addBindValue(ui->date->date());
    if(qry->exec()){
        ui->labelStatus->setText("Данные сохранены");
        ui->spinPrice->setValue(0);
        ui->spinAmount->setValue(1);
        emit newItem();
    }
    else{
        ui->labelStatus->setText("Данные не сохранены");
    }
}

void AddSale::on_checkCustomer_clicked(bool checked)
{
    if(checked){
        ui->boxCustomer->setEnabled(true);
    }
    else{
        ui->boxCustomer->setEnabled(false);
    }
}


void AddSale::on_checkCurrentDate_clicked(bool checked)
{
    if(checked){
        ui->date->setEnabled(false);
    }
    else{
        ui->date->setEnabled(true);
    }
}

