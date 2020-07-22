#include "RBasicComboPopup.h"

#include <QButtonGroup>
#include <QDebug>

RBasicComboPopup::RBasicComboPopup(QWidget *parent)
    : QWidget(parent)
{
    //背景透明FramelessWindowHint+WA_TranslucentBackground
    //这样才能给上面的组件设置透明色
    setWindowFlags(Qt::Popup|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_WindowPropagation);
    //setAttribute(Qt::WA_StyledBackground);

    animation=new QPropertyAnimation(this,"size",this);
    connect(animation,&QPropertyAnimation::finished,[this]{
        if(container)
            container->setFocus(); //弹出后获得焦点
    });
}

void RBasicComboPopup::showPopup()
{
    //没有设置container，已显示，动画正在运行？
    if(!container||isVisible()||animation->state()==QAbstractAnimation::Running)
        return;
    if(targetWidget){
        //设置为目标widget的宽度
        setPopSize(targetWidget->width(),1);
        //先show再move位置才是正确的
        show();
        //show之后取到的大小才是计算好的
        const int view_height=container->getContentsHeight();
        setPopSize(targetWidget->width(),view_height>150?150:view_height);
        //wrapper复位
        container->move(0,0);
        //移动到目标widget上方或下方
        move(targetWidget->mapToGlobal(targetWidget->rect().bottomLeft()));
    }
    //弹出如果从0开始，点击窗口标题栏不会隐藏
    animation->setStartValue(QSize(targetWidget->width(),1));
    animation->setEndValue(QSize(targetWidget->width(),container->height()));
    animation->setDuration(250);

    animation->start();
}

void RBasicComboPopup::hidePopup()
{
    hide();
}

void RBasicComboPopup::attachTarget(QWidget *widget)
{
    targetWidget=widget;
}

RBasicComboContainer *RBasicComboPopup::getContainer()
{
    return container;
}

void RBasicComboPopup::setContainer(RBasicComboContainer *container)
{
    if(this->container!=container){
        if(this->container){
            this->container->disconnect();
            this->container->deleteLater();
        }
        //
        this->container=container;
        this->container->setParent(this);
        connect(this->container,&RBasicComboContainer::updateData,this,&RBasicComboPopup::hidePopup);
        emit containerChanged();
    }
}

void RBasicComboPopup::setPopSize(int width, int height)
{
    //如果会被内容撑开，可以用fixed size
    resize(width,1);
    if(container)
        container->resize(width,height);
}
