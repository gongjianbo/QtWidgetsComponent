#include "RBasicComboContainer.h"

#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

RBasicComboContainer::RBasicComboContainer(QWidget *parent)
    : QWidget(parent)
{
    //背景透明FramelessWindowHint+WA_TranslucentBackground
    //这样才能给上面的组件设置透明色
    setWindowFlags(Qt::Popup|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_WindowPropagation);
    //setAttribute(Qt::WA_StyledBackground);
    initComponent();
}

void RBasicComboContainer::showPopup()
{
    if(isVisible()||animation->state()==QAbstractAnimation::Running)
        return;
    if(targetWidget){
        //设置为目标widget的宽度
        setPopSize(targetWidget->width(),1);
        //先show再move位置才是正确的
        show();
        //show之后取到的大小才是计算好的
        const int view_height=view->getContentsHeight()+2; //margin待设计
        setPopSize(targetWidget->width(),view_height>150?150:view_height);

        //wrapper复位
        wrapper->move(0,0);
        //移动到目标widget上方或下方
        move(targetWidget->mapToGlobal(targetWidget->rect().bottomLeft()));
    }
    //弹出如果从0开始，点击窗口标题栏不会隐藏
    animation->setStartValue(QSize(targetWidget->width(),1));
    animation->setEndValue(QSize(targetWidget->width(),wrapper->height()));
    animation->setDuration(250);

    animation->start();
}

void RBasicComboContainer::hidePopup()
{
    hide();
}

void RBasicComboContainer::attachTarget(QWidget *widget)
{
    targetWidget=widget;
}

RBasicComboView *RBasicComboContainer::getBasicView() const
{
    return view;
}

void RBasicComboContainer::initComponent()
{
    //底层
    wrapper->setObjectName("wrapper");
    //列表
    view->setObjectName("view");
    connect(view,&RBasicComboView::rowClicked,this,&RBasicComboContainer::hidePopup);
    //布局
    QVBoxLayout *layout=new QVBoxLayout(wrapper);
    layout->setMargin(1);
    layout->setSpacing(0);
    layout->addWidget(view);

    animation=new QPropertyAnimation(this,"size",this);
    connect(animation,&QPropertyAnimation::finished,[this]{
        view->setFocus(); //弹出后获得焦点
    });
}

void RBasicComboContainer::setPopSize(int width, int height)
{
    //如果会被内容撑开，可以用fixed size
    resize(width,1);
    wrapper->resize(width,height);
}
