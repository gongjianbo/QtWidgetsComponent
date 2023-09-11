#include "PopupDemo.h"
#include "ui_PopupDemo.h"
#include "Popup/CuteBasicComboBox.h"
#include "Popup/CuteBasicComboContainer.h"
#include <QDebug>

PopupDemo::PopupDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupDemo)
{
    ui->setupUi(this);
    initComboBox();
}

PopupDemo::~PopupDemo()
{
    delete ui;
}

void PopupDemo::initComboBox()
{
    ui->comboBoxA->getPopup()->setContainer(new CuteBasicComboContainer(this));
    ui->comboBoxB->getPopup()->setContainer(new CuteBasicComboContainer(this));

    ui->comboBoxA->setItems({"aaa","bbb","ccc","ddd","eee","fff","ggg",
                             "fff","iii","jjj","kkk","lll","mmm","nnn"});
    connect(ui->comboBoxA, &CuteBasicComboBox::currentIndexChanged, [this](int index) {
        qDebug() << "CuteBasicComboBox::currentIndexChanged" << index;
    });
    connect(ui->comboBoxA, &CuteBasicComboBox::currentTextChanged, [this](const QString &text) {
        qDebug() << "CuteBasicComboBox::currentTextChanged" << text;
    });

    ui->comboBoxB->setItems({"aaa","bbb","ccc"});

    const char *qss_str = R"(
CuteBasicComboBox{
qproperty-borderWidth:"1";
border:1px solid white;
background:rgb(20,50,70);
}
CuteBasicComboBox:disable{
border:1px solid gray;
}
CuteBasicComboBox #edit{
border:0;
margin:0;
padding:0px 3px;
color:white;
background:rgb(20,50,70);
}
CuteBasicComboBox #down{
border:0;
padding:0;
margin:0;
min-width:20px;
max-width:20px;
min-height:20px;
max-height:20px;
background-color:rgb(20,50,70);
subcontrol-position:center;
border-image:url(:/Image/down_white.png);
}
CuteBasicComboBox[popupVisible="1"] #down{
border-image:url(:/Image/up_white.png);
}
CuteBasicComboContainer{
border:0;
background:red;
}
CuteBasicComboView{
border:1px solid white;
font:14px "宋体";
background:rgb(20,50,70);
}
CuteBasicComboView::item{
height:24px;
color:white;
background:rgb(20,50,70);
}
CuteBasicComboView::item:selected,
CuteBasicComboView::item:hover{
color:white;
background-color:rgb(255,170,0);
}
)";
    ui->comboBoxA->setStyleSheet(qss_str);
    ui->comboBoxB->setStyleSheet(qss_str);
}
