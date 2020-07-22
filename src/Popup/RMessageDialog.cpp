#include "RMessageDialog.h"
#include "ui_RMessageDialog.h"

RMessageDialog::RMessageDialog(QWidget *parent) :
    RBasicDialog(parent),
    ui(new Ui::RMessageDialog)
{
    ui->setupUi(contentDialog);

    addDefaultButton(ButtonType::Ok,"确认");
    addDefaultButton(ButtonType::Cancel,"取消");

    resize(300,200);
}

RMessageDialog::~RMessageDialog()
{
    delete ui;
}

void RMessageDialog::setMessage(const QString &title, const QString &msg)
{
    setDialogTitle(title);
    ui->label->setText(msg);
}

int RMessageDialog::showMessage(const QString &title, const QString &msg)
{
    RMessageDialog dialog;
    dialog.setMessage(title,msg);
    return dialog.exec();
}
