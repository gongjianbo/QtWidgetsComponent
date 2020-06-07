#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "RToolTip.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initAppStyleSheet();
    /*配合样式表设置tab背景*/
    ui->tabWidget->setAttribute(Qt::WA_StyledBackground);

    RToolTip *tipA=new RToolTip("tipA","第1个tip");
    tipA->anchorTarget(ui->btnToolTipA);
    connect(ui->btnToolTipA,&QPushButton::clicked,[=]{
        if(ui->btnToolTipB){
            ui->btnToolTipB->deleteLater();
            ui->btnToolTipB=nullptr;
        }
    });

    RToolTip *tipB=new RToolTip("tipB","第2个tip");
    tipB->anchorTarget(ui->btnToolTipB);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initAppStyleSheet()
{
    qApp->setStyleSheet(R"(
                        .RToolTip#tipA{
                        min-width:100px;
                        max-width:100px;
                        min-height:30px;
                        max-height:30px;
                        }
                        .RToolTip#tipB{
                        qproperty-rightOffset:"20";
                        qproperty-bottomOffset:"3";
                        }
                        .RToolTip QLabel{
                        padding:10px 50px;
                        color:white;
                        border: 1px solid white;
                        background-color:rgb(20,50,90);
                        }
                        .RToolTip#tipA QLabel{
                        padding:0;
                        border-radius:5px;
                        background-color:rgba(250,170,0,150);
                        }
                        )");
}
