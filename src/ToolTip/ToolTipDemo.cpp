#include "ToolTipDemo.h"
#include "ui_ToolTipDemo.h"

#include <QListView>

#include "RToolTip.h"
#include "RDesktopTip.h"
#include "RDesktopTip2.h"

ToolTipDemo::ToolTipDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolTipDemo)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    initToolTip();
    initDesktopTip();
    initDesktopTip2();
}

ToolTipDemo::~ToolTipDemo()
{
    delete ui;
}

void ToolTipDemo::initToolTip()
{
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

void ToolTipDemo::initDesktopTip()
{
    //待修改样式
    ui->desktopTipBox->setView(new QListView(this));
    //选择启用的动画
    ui->desktopTipBox->addItems({"All","No","Opacity","Pos"});
    connect(ui->desktopTipBox,QOverload<int>::of(&QComboBox::currentIndexChanged),
            [](int index){
        switch (index) {
        case 0: RDesktopTip::setMode(RDesktopTip::AllAnimation); break;
        case 1: RDesktopTip::setMode(RDesktopTip::NoAnimation); break;
        case 2: RDesktopTip::setMode(RDesktopTip::OpacityAnimation); break;
        case 3: RDesktopTip::setMode(RDesktopTip::PosAnimation); break;
        default: break;
        }
    });
    connect(ui->btnDesktopTipShow,&QPushButton::clicked,[=]{
        RDesktopTip::showTip({
                                 "这是 1 条信息",
                                 "这是 1 条信息",
                                 "这是 1 条信息",
                                 "这是 1 条信息"
                             },5); //只显示5s就消失
    });
    connect(ui->btnDesktopTipKeep,&QPushButton::clicked,[=]{
        RDesktopTip::keepTip({
                                 "这是 2 条信息",
                                 "这是 2 条信息",
                                 "这是 2 条信息",
                                 "这是 2 条信息"
                             }); //一直显示，直到点关闭
    });
    connect(ui->btnDesktopTipHide,&QPushButton::clicked,[=]{
        RDesktopTip::hideTip();
    });
}

void ToolTipDemo::initDesktopTip2()
{
    //待修改样式
    ui->desktopTip2Box->setView(new QListView(this));
    //选择启用的动画
    ui->desktopTip2Box->addItems({"All","No","Opacity","Pos"});
    connect(ui->desktopTip2Box,QOverload<int>::of(&QComboBox::currentIndexChanged),
            [](int index){
        switch (index) {
        case 0: RDesktopTip2::setMode(RDesktopTip2::AllAnimation); break;
        case 1: RDesktopTip2::setMode(RDesktopTip2::NoAnimation); break;
        case 2: RDesktopTip2::setMode(RDesktopTip2::OpacityAnimation); break;
        case 3: RDesktopTip2::setMode(RDesktopTip2::PosAnimation); break;
        default: break;
        }
    });
    connect(ui->btnDesktopTip2Show,&QPushButton::clicked,[=]{
        RDesktopTip2::showTip({
                                  "这是 1 条信息",
                                  "这是 1 条信息",
                                  "这是 1 条信息",
                                  "这是 1 条信息"
                              },5); //只显示5s就消失
    });
    connect(ui->btnDesktopTip2Keep,&QPushButton::clicked,[=]{
        RDesktopTip2::keepTip({
                                  "这是 2 条信息",
                                  "这是 2 条信息",
                                  "这是 2 条信息",
                                  "这是 2 条信息"
                              }); //一直显示，直到点关闭
    });
    connect(ui->btnDesktopTip2Hide,&QPushButton::clicked,[=]{
        RDesktopTip2::hideTip();
    });

}
