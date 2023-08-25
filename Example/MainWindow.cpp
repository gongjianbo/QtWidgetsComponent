#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QtComponent.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QtComponent::hello();
}

MainWindow::~MainWindow()
{
    delete ui;
}

