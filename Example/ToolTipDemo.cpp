#include "ToolTipDemo.h"
#include "ui_ToolTipDemo.h"
#include <QListView>
#include "ToolTip/CuteToolTip.h"
#include "ToolTip/CuteDesktopTip.h"

ToolTipDemo::ToolTipDemo(QWidget *parent)
    : QWidget{parent},
    ui{new Ui::ToolTipDemo}
{
    ui->setupUi(this);
    initToolTip();
    initDesktopTip();
}

ToolTipDemo::~ToolTipDemo()
{
    delete ui;
}

void ToolTipDemo::initToolTip()
{
    CuteToolTip *tip_a = new CuteToolTip("Tip A", ui->btnToolTipA);
    // 通过 objectName 区分样式表设置
    tip_a->setObjectName("tipA");
    tip_a->anchorTarget(ui->btnToolTipA);
    // 样式表设置为固定尺寸 tip
    tip_a->setStyleSheet(R"(
.CuteToolTip#tipA{
min-width:100px;
max-width:100px;
min-height:30px;
max-height:30px;
}
.CuteToolTip#tipA QLabel{
color:white;
border:1px solid white;
padding:0;
border-radius:5px;
background-color:rgba(250,170,0,150);
}
)");

    CuteToolTip *tip_b = new CuteToolTip("Tip B", ui->btnToolTipB);
    tip_b->setObjectName("tipB");
    tip_b->anchorTarget(ui->btnToolTipB);
    // 自适应的尺寸
    tip_b->setStyleSheet(R"(
.CuteToolTip#tipB{
qproperty-xOffset:"20";
qproperty-yOffset:"3";
}
.CuteToolTip#tipB QLabel{
padding:10px 30px;
color:white;
border:1px solid white;
background-color:rgb(20,50,90);
}
)");
}

void ToolTipDemo::initDesktopTip()
{
    // 选择启用的动画
    ui->boxDesktopTipAni->setView(new QListView(this));
    // 第一项用默认值
    ui->boxDesktopTipAni->addItems({"All", "No", "Opacity", "Pos"});
    connect(ui->boxDesktopTipAni, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [](int index){
                switch (index) {
                case 0: CuteDesktopTip::setMode(CuteDesktopTip::AllAnimation); break;
                case 1: CuteDesktopTip::setMode(CuteDesktopTip::NoAnimation); break;
                case 2: CuteDesktopTip::setMode(CuteDesktopTip::OpacityAnimation); break;
                case 3: CuteDesktopTip::setMode(CuteDesktopTip::PosAnimation); break;
                default: break;
                }
            });
    // 选择弹出位置
    ui->boxDesktopTipArea->setView(new QListView(this));
    // 第一项用默认值
    ui->boxDesktopTipArea->addItems({"RightBottom", "Center"});
    connect(ui->boxDesktopTipArea, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [](int index){
                switch (index) {
                case 0: CuteDesktopTip::setArea(CuteDesktopTip::RightBottomArea); break;
                case 1: CuteDesktopTip::setArea(CuteDesktopTip::CenterArea); break;
                default: break;
                }
            });

    connect(ui->btnDesktopTipShow, &QPushButton::clicked, [=]{
        //只显示 n 秒就消失
        CuteDesktopTip::showTip("测试 showTip"
                               , "这是 1 条信息\n这是 1 条信息\n这是 1 条信息\n这是 1 条信息"
                               , 5);
    });
    connect(ui->btnDesktopTipKeep, &QPushButton::clicked, [=]{
        // 一直显示，直到点关闭
        CuteDesktopTip::keepTip("测试 keepTip"
                                , "这是 1 条信息\n这是 1 条信息\n这是 1 条信息\n这是 1 条信息");
    });
    connect(ui->btnDesktopTipHide, &QPushButton::clicked, [=]{
        CuteDesktopTip::hideTip();
    });
}
