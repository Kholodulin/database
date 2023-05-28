#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SRM");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./../Sales.db");
    if(db.open()){
        ui->statusbar->showMessage("Подключение к базе данных успешно");
        model = new QSqlTableModel(this,db);
        curTableName="product";
        model->setTable(curTableName);
        model->select();
        setProductHeaderTitle();

        ui->tableView->setModel(model);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSortingEnabled(true);
        ui->tableView->sortByColumn(0,Qt::AscendingOrder);
        ui->tableView->setColumnHidden(0,true);

        isProductAddOpen = false;
        isSaleAddOpen = false;
        qry = new QSqlQuery;
    }
    else{
        ui->statusbar->showMessage("Ошибка при подключении к базе данных "+ db.lastError().databaseText());
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    if(isProductAddOpen){delete newItem;}
    if(isSaleAddOpen){delete addSale;}
}

void MainWindow::on_select_table_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        curTableName="product";
        model->setTable(curTableName);
        setProductHeaderTitle();
        isProductAddOpen ? ui->editMain->setEnabled(false):ui->editMain->setEnabled(true);
        ui->tableView->sortByColumn(0,Qt::AscendingOrder);
        break;
    case 1:
        curTableName="sale";
        model->setTable(curTableName);
        setSaleHeaderTitle();
        isSaleAddOpen ? ui->editMain->setEnabled(false):ui->editMain->setEnabled(true);
        ui->tableView->sortByColumn(0,Qt::AscendingOrder);
        break;
    case 2:
        curTableName="customer";
        model->setTable(curTableName);
        setCustomerHeaderTitle();
        ui->editMain->setEnabled(true);
        ui->tableView->sortByColumn(0,Qt::AscendingOrder);
        break;
    }
    model->select();
    ui->tableView->setColumnHidden(0,true);
}

void MainWindow::on_editMain_clicked()
{
    int index = ui->select_table->currentIndex();
    switch (index) {
    case 0:
        newItem = new NewItem(db);
        newItem->show();
        isProductAddOpen = true;
        connect(newItem, &NewItem::addWindowClosed,this,&MainWindow::addWindowClosedSlot);
        connect(newItem, &NewItem::newItem,this,&MainWindow::newItemSlot);
        break;
    case 1:
        addSale = new AddSale(db);
        addSale->show();
        isSaleAddOpen = true;
        connect(addSale, &AddSale::addWindowClosed,this,&MainWindow::addWindowClosedSlot);
        connect(addSale, &AddSale::newItem,this,&MainWindow::newItemSlot);
        break;
    case 2:
        model->insertRow(model->rowCount());
        break;
    }
    if(index!=2){
        ui->editMain->setEnabled(false);
    }
}
void MainWindow::on_delMain_clicked()
{
    model->removeRow(currentRow);
    model->select();
}
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}
void MainWindow::on_refreshMain_clicked()
{
    model->select();
    ui->tableView->selectRow(currentRow);
}

void MainWindow::addWindowClosedSlot(int id)
{
    switch (id) {
    case 0:
        isProductAddOpen = false;
        delete newItem;
        break;
    case 1:
        isSaleAddOpen = false;
        delete addSale;
        break;
    }
    if(ui->select_table->currentIndex()==id){
        ui->editMain->setEnabled(true);
    }
}

void MainWindow::newItemSlot()
{
    model->select();
}

void MainWindow::setProductHeaderTitle()
{
    model->setHeaderData(1,Qt::Horizontal,"Категория");
    model->setHeaderData(2,Qt::Horizontal,"Модель");
    model->setHeaderData(3,Qt::Horizontal,"Описание");
    model->setHeaderData(4,Qt::Horizontal,"Наличие");
}
void MainWindow::setSaleHeaderTitle()
{
    model->setHeaderData(1,Qt::Horizontal,"Товар");
    model->setHeaderData(2,Qt::Horizontal,"Заказчик");
    model->setHeaderData(3,Qt::Horizontal,"Цена за единицу");
    model->setHeaderData(4,Qt::Horizontal,"Колличество");
    model->setHeaderData(5,Qt::Horizontal,"Итого");
    model->setHeaderData(6,Qt::Horizontal,"Дата");
}
void MainWindow::setCustomerHeaderTitle()
{
    model->setHeaderData(1,Qt::Horizontal,"Имя клиента");
    model->setHeaderData(3,Qt::Horizontal,"Номер телефона");
    model->setHeaderData(4,Qt::Horizontal,"Адрес");
}

void MainWindow::on_btnSearch_clicked()
{
    QString txt = ui->lineSearch->text();
    switch (ui->select_table->currentIndex()) {
    case 0:
        model->setFilter(QString("model = '%1'").arg(txt));
        break;
    case 1:
        model->setFilter(QString("product_id = '%1'").arg(txt));
        break;
    case 2:
        model->setFilter(QString("customer_name = '%1'").arg(txt));
        break;
    }
    if(txt==""){
        model->setFilter("");
    }
}

