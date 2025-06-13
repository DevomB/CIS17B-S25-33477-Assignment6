#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Add Item Section
    QHBoxLayout *addLayout = new QHBoxLayout();
    idInput = new QLineEdit();
    descInput = new QLineEdit();
    locInput = new QLineEdit();
    QPushButton *addBtn = new QPushButton("Add Item");

    addLayout->addWidget(new QLabel("ID:"));
    addLayout->addWidget(idInput);
    addLayout->addWidget(new QLabel("Description:"));
    addLayout->addWidget(descInput);
    addLayout->addWidget(new QLabel("Location:"));
    addLayout->addWidget(locInput);
    addLayout->addWidget(addBtn);

    // Find Item Section
    QHBoxLayout *findLayout = new QHBoxLayout();
    findIdInput = new QLineEdit();
    QPushButton *findBtn = new QPushButton("Find Item");
    findLayout->addWidget(new QLabel("Find by ID:"));
    findLayout->addWidget(findIdInput);
    findLayout->addWidget(findBtn);

    // Remove Item Section
    QHBoxLayout *removeLayout = new QHBoxLayout();
    removeIdInput = new QLineEdit();
    QPushButton *removeBtn = new QPushButton("Remove Item");
    removeLayout->addWidget(new QLabel("Remove by ID:"));
    removeLayout->addWidget(removeIdInput);
    removeLayout->addWidget(removeBtn);

    // Item List
    itemList = new QListWidget();

    mainLayout->addLayout(addLayout);
    mainLayout->addLayout(findLayout);
    mainLayout->addLayout(removeLayout);
    mainLayout->addWidget(itemList);

    setCentralWidget(central);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);
    connect(findBtn, &QPushButton::clicked, this, &MainWindow::onFindItemClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveItemClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddItemClicked() {
    QString id = idInput->text();
    QString desc = descInput->text();
    QString loc = locInput->text();

    try {
        auto item = std::make_shared<StoredItem>(id.toStdString(), desc.toStdString(), loc.toStdString());
        manager.addItem(item);
        refreshList();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
    idInput->clear();
    descInput->clear();
    locInput->clear();
    idInput->setFocus();
}

void MainWindow::onFindItemClicked() {
    QString id = findIdInput->text();
    try {
        auto item = manager.findById(id.toStdString());
        QString msg = QString::fromStdString("Found: " + item->getDescription() + " at " + item->getLocation());
        QMessageBox::information(this, "Item Found", msg);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::onRemoveItemClicked() {
    QString id = removeIdInput->text();
    try {
        manager.removeItem(id.toStdString());
        refreshList();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::refreshList() {
    itemList->clear();
    for (auto &item : manager.listItemsByDescription()) {
        QString display = QString::fromStdString(item->getId() + " - " + item->getDescription() + " - " + item->getLocation());
        itemList->addItem(display);
    }
}
