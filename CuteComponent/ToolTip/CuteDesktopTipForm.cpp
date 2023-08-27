#include "CuteDesktopTipForm.h"
#include "ui_CuteDesktopTipForm.h"
#include <QStringList>

CuteDesktopTipForm::CuteDesktopTipForm(QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::CuteDesktopTipForm}
{
    ui->setupUi(this);
}

CuteDesktopTipForm::~CuteDesktopTipForm()
{
    delete ui;
}
