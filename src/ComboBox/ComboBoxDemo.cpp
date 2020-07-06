#include "ComboBoxDemo.h"
#include "ui_ComboBoxDemo.h"

ComboBoxDemo::ComboBoxDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboBoxDemo)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
}

ComboBoxDemo::~ComboBoxDemo()
{
    delete ui;
}
