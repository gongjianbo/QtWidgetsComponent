#include "RBasicDialog.h"
#include "ui_RBasicDialog.h"

#include <functional>

#include <QPushButton>
#include <QMouseEvent>
#include <QShowEvent>
#include <QKeyEvent>
#include <QChildEvent>
#include <QDebug>

RBasicDialog::RBasicDialog(QWidget *parent) :
    QDialog(parent),
    basicui(new Ui::RBasicDialog)
{
    basicui->setupUi(this);

    //如果被VC HOOK影响修改Qt::Dialog|Qt::Popup-->Qt::ToolTip
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);//无边框
    setAttribute(Qt::WA_TranslucentBackground); //透明
    //setFocusPolicy(Qt::StrongFocus);
    //basicui->btnClose->setFocusPolicy(Qt::NoFocus);
    //setAttribute(Qt::WA_DeleteOnClose); //close时释放
    //子类可调用 ui->setupUi(contentDialog);
    contentDialog = basicui->contentDialog;
    contentDialog->setWindowFlags(Qt::Widget);
    contentDialog->setAttribute(Qt::WA_StyledBackground);

    //点击关闭按钮
    //操作后setResult(int)，外部通过exec()获取结果
    //防止和旧的closeBtn样式冲突，改了下名
    basicui->btnClose->setShortcut(Qt::Key_Escape);
    connect(basicui->btnClose, &QPushButton::clicked, this, &RBasicDialog::closeButtonClick);

    //按钮区域默认隐藏，加了按钮才显示
    basicui->buttonArea->setVisible(false);
    //qDebug() << "create BasicDialog";
}

RBasicDialog::RBasicDialog(int buttons, QWidget *parent)
    : RBasicDialog(parent)
{
    int temp = 1;

    while (buttons)
    {
        if (buttons&temp) {
            addDefaultButton(buttons&temp);
        }
        buttons &= ~temp;
        temp <<= 1;
    }
}

RBasicDialog::~RBasicDialog()
{
    //qDebug() << "delete BasicDialog";
    delete basicui;
}

void RBasicDialog::setDialogTitle(const QString & text)
{
    basicui->titleLabel->setText(text);
}

void RBasicDialog::addDefaultButton(int type, const QString & text)
{
    //还没有进行重复性判断
    if (type == ButtonType::NoButton)
        return;

    QPushButton *button = new QPushButton(text, this);
    basicui->buttonLayout->addWidget(button);
    //第一个作为默认
    if (basicui->buttonLayout->count() == 1) {
        button->setDefault(true);
        //按钮区域默认隐藏的，加了按钮后显示出来
        basicui->buttonArea->setVisible(true);
    }
    connect(button, &QPushButton::clicked, std::bind(&RBasicDialog::buttonClick, this, type));

    switch (type)
    {
    case Ok: if (text.isEmpty()) button->setText("确认"); break;
    case Cancel: if (text.isEmpty()) button->setText("取消"); break;
    case Yes: if (text.isEmpty()) button->setText("是"); break;
    case No: if (text.isEmpty()) button->setText("否"); break;
    default: break;
    }
}

void RBasicDialog::addButton(QPushButton * button, int type)
{
    if (!button)
        return;
    button->setParent(this); //多余的，布局会做
    basicui->buttonLayout->addWidget(button);
    //第一个作为默认
    if (basicui->buttonLayout->count() == 1) {
        button->setDefault(true);
        //按钮区域默认隐藏的，加了按钮后显示出来
        basicui->buttonArea->setVisible(true);
    }
    connect(button, &QPushButton::clicked, std::bind(&RBasicDialog::buttonClick, this, type));
}

void RBasicDialog::mousePressEvent(QMouseEvent * event)
{
    //在标题栏按下启用拖动
    posTemp = event->pos();
    if (posTemp.y() < basicui->titleArea->height()) {
        onMoving = true;
    }
    QDialog::mousePressEvent(event);
}

void RBasicDialog::mouseMoveEvent(QMouseEvent * event)
{
    //拖动标题栏
    if (onMoving) {
        event->accept();
        move(event->globalPos() - posTemp);
        update();
    }
    else {
        QDialog::mouseMoveEvent(event);
    }
}

void RBasicDialog::mouseReleaseEvent(QMouseEvent * event)
{
    onMoving = false;
    QDialog::mouseReleaseEvent(event);
}

void RBasicDialog::showEvent(QShowEvent * event)
{
    activateWindow();
    setFocus();
    QDialog::showEvent(event);
}

void RBasicDialog::closeButtonClick()
{
    done(ButtonType::Cancel);
}

void RBasicDialog::buttonClick(int type)
{
    done(type);
}
