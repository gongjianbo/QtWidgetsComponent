#include "ToolTipDemo.h"
#include "ui_ToolTipDemo.h"
#include "ToolTip/CuteToolTip.h"

ToolTipDemo::ToolTipDemo(QWidget *parent)
    : QWidget{parent},
    ui{new Ui::ToolTipDemo}
{
    ui->setupUi(this);
    initToolTip();
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
