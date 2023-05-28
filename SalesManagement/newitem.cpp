#include "newitem.h"
#include "ui_newitem.h"

NewItem::NewItem(QSqlDatabase _db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewItem)
{
    ui->setupUi(this);
    currentTabName = "product";
    db = _db;
    qry = new QSqlQuery;
    setWindowTitle(currentTabName+" edit");
    categoryBoxFill();
    ui->inputDescription->setReadOnly(false);
    ui->spinBox->setMaximum(999999999);
    ui->newCategoryLine->hide();
    isAddCategory = false;
}

NewItem::~NewItem()
{
    delete ui;
}

void NewItem::closeEvent(QCloseEvent *e)
{
    emit addWindowClosed(0);
}

void NewItem::categoryBoxFill()
{
    ui->categoryBox->clear();
    qry->exec("SELECT category_id, category_name FROM category");
    while (qry->next()) {
        int id = qry->value(0).toInt();
        QString name = qry->value(1).toString();
        ui->categoryBox->addItem(name);
        idAndNameCategory.insert(name,id);
    }
}

void NewItem::on_saveProduct_clicked()
{
    QString description = ui->inputDescription->toPlainText();
    qry->prepare("INSERT INTO product (category_id, model, description, availability) "
                    "VALUES (?, ?, ?, ?)");
    qry->addBindValue(idAndNameCategory.find(ui->categoryBox->currentText()).value());
    qry->addBindValue(ui->inputModel->text());
    description==""?qry->addBindValue(QVariant()): qry->addBindValue(ui->inputDescription->toPlainText());
    qry->addBindValue(ui->spinBox->value());
    if(qry->exec()){
        ui->labelStatus->setText("Данные добавлены");
        ui->inputModel->clear();
        ui->inputDescription->clear();
        ui->spinBox->setValue(0);
        emit newItem();
    }
    else{
        ui->labelStatus->setText("Данные не добавлены.\nЗаполните поля");
    }
}

void NewItem::on_addCategory_clicked()
{
    QString category = ui->newCategoryLine->text();
    if(!isAddCategory){
        isAddCategory=true;
        ui->categoryBox->hide();
        ui->newCategoryLine->show();
        ui->addCategory->setText("ОК");
        ui->addCategory->setFont(QFont("Arial", 10));
    }
    else{
        isAddCategory=false;
        ui->categoryBox->show();
        ui->newCategoryLine->hide();
        ui->addCategory->setText("+");
        ui->addCategory->setFont(QFont("Arial", 16));
        qry->prepare("INSERT INTO category (category_name) VALUES (?)");
        qry->addBindValue(category);
        ui->newCategoryLine->clear();
        qry->exec();
        ui->labelStatus->setText("Добавлена категория\n" +category);
    }
    categoryBoxFill();
    ui->categoryBox->setCurrentText(category);
}

void NewItem::on_delCategory_clicked()
{
    QString category = ui->categoryBox->currentText();
    qry->prepare("DELETE FROM category WHERE category_name = ?");
    qry->addBindValue(category);
    if(qry->exec()){
        categoryBoxFill();
        ui->labelStatus->setText("Удалена категория\n" +category);
        emit newItem();
    }
    else{
        ui->labelStatus->setText("Категорию " + category +"\nнельзя удалить");
    }
}

