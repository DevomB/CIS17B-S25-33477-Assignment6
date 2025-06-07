#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->addItemButton, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);
    connect(ui->findItemButton, &QPushButton::clicked, this, &MainWindow::onFindItemClicked);
    connect(ui->removeItemButton, &QPushButton::clicked, this, &MainWindow::onRemoveItemClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddItemClicked() {
    QString id = ui->idInput->text();
    QString desc = ui->descInput->text();
    QString loc = ui->locInput->text();

    try {
        auto item = std::make_shared<StoredItem>(id.toStdString(), desc.toStdString(), loc.toStdString());
        manager.addItem(item);
        refreshList();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
    ui->idInput->clear();
    ui->descInput->clear();
    ui->locInput->clear();
    ui->idInput->setFocus();
}

void MainWindow::onFindItemClicked() {
    QString id = ui->findIdInput->text();
    try {
        auto item = manager.findById(id.toStdString());
        QString msg = QString::fromStdString("Found: " + item->getDescription() + " at " + item->getLocation());
        QMessageBox::information(this, "Item Found", msg);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::onRemoveItemClicked() {
    QString id = ui->removeIdInput->text();
    try {
        manager.removeItem(id.toStdString());
        refreshList();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::refreshList() {
    ui->itemList->clear();
    for (auto &item : manager.listItemsByDescription()) {
        QString display = QString::fromStdString(item->getId() + " - " + item->getDescription() + " - " + item->getLocation());
        ui->itemList->addItem(display);
    }
}
