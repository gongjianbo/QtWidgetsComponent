#include "CuteBasicComboPopup.h"
#include "CuteBasicComboContainer.h"
#include <QDebug>

CuteBasicComboPopup::CuteBasicComboPopup(QWidget *parent)
    : QWidget{parent}
{
    // 背景透明 FramelessWindowHint + WA_TranslucentBackground
    // 这样才能给上面的组件设置透明色
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_WindowPropagation);
    //setAttribute(Qt::WA_StyledBackground);

    animation = new QPropertyAnimation(this, "size", this);
    connect(animation, &QPropertyAnimation::finished, [this] {
        // 弹出后获得焦点
        if (container)
            container->setFocus();
    });
}

void CuteBasicComboPopup::showPopup()
{
    // 没有设置 container
    if (!container) {
        qDebug() << __FUNCTION__ << "please call attachTarget first";
        return;
    }
    if (!targetWidget) {
        qDebug() << __FUNCTION__ << "please call setContainer first";
        return;
    }
    // 已显示 or 动画正在运行？
    if (isVisible() || animation->state() == QAbstractAnimation::Running)
        return;

    // 设置为目标 widget 的宽度
    setPopSize(targetWidget->width(), 1);
    // 先 show 再 move 位置才是正确的
    show();
    // show 之后取到的大小才是计算好的
    const int view_height = container->getContentsHeight();
    setPopSize(targetWidget->width(), view_height>150?150:view_height);
    // wrapper 复位
    container->move(0, 0);
    // 移动到目标 widget 上方或下方
    move(targetWidget->mapToGlobal(targetWidget->rect().bottomLeft()));

    // 弹出如果从0开始，点击窗口标题栏不会隐藏？
    animation->setStartValue(QSize(targetWidget->width(), 1));
    animation->setEndValue(QSize(targetWidget->width(), container->height()));
    animation->setDuration(250);

    animation->start();
}

void CuteBasicComboPopup::hidePopup()
{
    hide();
}

void CuteBasicComboPopup::attachTarget(QWidget *widget)
{
    targetWidget = widget;
}

CuteBasicComboContainer *CuteBasicComboPopup::getContainer()
{
    return container;
}

void CuteBasicComboPopup::setContainer(CuteBasicComboContainer *widget)
{
    if (container == widget)
        return;

    if (container) {
        container->disconnect();
        container->deleteLater();
    }
    //
    container = widget;
    container->setParent(this);
    connect(container, &CuteBasicComboContainer::updateData, this, &CuteBasicComboPopup::hidePopup);
    emit containerChanged();
}

void CuteBasicComboPopup::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit visibleChanged(true);
}

void CuteBasicComboPopup::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    emit visibleChanged(false);
}

void CuteBasicComboPopup::setPopSize(int width, int height)
{
    // 如果会被内容撑开，可以用 fixed size
    resize(width, 1);
    if (container)
        container->resize(width, height);
}
