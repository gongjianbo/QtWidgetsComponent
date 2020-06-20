#include "ToolTipDemo.h"
#include "ui_ToolTipDemo.h"

#include "RToolTip.h"

ToolTipDemo::ToolTipDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolTipDemo)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    //对于没有parent的widget，可以给qApp设置全局样式
    RToolTip *tipA=new RToolTip("tipA","第1个tip");
    tipA->anchorTarget(ui->btnToolTipA);
    /*connect(ui->btnToolTipA,&QPushButton::clicked,[=]{
        //测试RToolTip析构是否调用，需打开析构的qDebug输出
        if(ui->btnToolTipB){
            ui->btnToolTipB->deleteLater();
            ui->btnToolTipB=nullptr;
        }
    });*/
    tipA->setStyleSheet(R"(
                        .RToolTip#tipA{
                        min-width:100px;
                        max-width:100px;
                        min-height:30px;
                        max-height:30px;
                        }
                        .RToolTip#tipA QLabel{
                        color:white;
                        border: 1px solid white;
                        padding:0;
                        border-radius:5px;
                        background-color:rgba(250,170,0,150);
                        }
                        )");

    RToolTip *tipB=new RToolTip("tipB","第2个tip");
    tipB->anchorTarget(ui->btnToolTipB);
    tipB->setStyleSheet(R"(
                        .RToolTip#tipB{
                        qproperty-rightOffset:"20";
                        qproperty-bottomOffset:"3";
                        }
                        .RToolTip#tipB QLabel{
                        padding:10px 50px;
                        color:white;
                        border: 1px solid white;
                        background-color:rgb(20,50,90);
                        }
                        )");
}

ToolTipDemo::~ToolTipDemo()
{
    delete ui;
}
