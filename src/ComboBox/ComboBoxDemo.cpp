#include "ComboBoxDemo.h"
#include "ui_ComboBoxDemo.h"

#include <QDebug>

ComboBoxDemo::ComboBoxDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboBoxDemo)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    initBasicComboBox();
}

ComboBoxDemo::~ComboBoxDemo()
{
    delete ui;
}

void ComboBoxDemo::initBasicComboBox()
{
    ui->basicComboBoxA->setItems({"aaa","bbb","ccc","ddd","eee","fff","ggg",
                                 "fff","iii","jjj","kkk","lll","mmm","nnn"});
    connect(ui->basicComboBoxA,&RBasicComboBox::currentIndexChanged,[this](int index){
        qDebug()<<"RBasicComboBox::currentIndexChanged"<<index;
    });
    connect(ui->basicComboBoxA,&RBasicComboBox::currentTextChanged,[this](const QString &text){
        qDebug()<<"RBasicComboBox::currentTextChanged"<<text;
    });

    ui->basicComboBoxB->setItems({"aaa","bbb","ccc"});
}
