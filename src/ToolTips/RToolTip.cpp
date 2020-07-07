#include "RToolTip.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QTimerEvent>
#include <QResizeEvent>
//#include <QStyle>
#include <QHBoxLayout>
//#include <QDebug>

RToolTip::RToolTip(const QString &objectName, const QString text)
    : QWidget(nullptr)
    ,contentLabel(new QLabel(this))
{
    //把内层label添加到透明widget
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(contentLabel);

    contentLabel->setAlignment(Qt::AlignCenter);
    contentLabel->setText(text);

    setObjectName(objectName);

    //把widget设置为透明，样式表设置给label
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //默认hide
    setVisible(false);

    //qDebug()<<"new tooltip"<<objectName;
}

RToolTip::~RToolTip()
{
    //qDebug()<<"delete tooltip"<<objectName();
}

int RToolTip::getRightOffset()
{
    return rightOffset;
}

void RToolTip::setRightOffset(int offset)
{
    if(rightOffset!=offset){
        rightOffset=offset;
        //没有动态样式的处理，可自行添加
        //style()->unpolish(this);
        //style()->polish(this);
    }
}

int RToolTip::getBottomOffset()
{
    return bottomOffset;
}

void RToolTip::setBottomOffset(int offset)
{
    if(bottomOffset!=offset){
        bottomOffset=offset;
    }
}

QString RToolTip::getText() const
{
    return contentLabel->text();
}

void RToolTip::setText(const QString &text)
{
    contentLabel->setText(text);
}

Qt::Alignment RToolTip::getAlignment() const
{
    return contentLabel->alignment();
}

void RToolTip::setAlignment(Qt::Alignment alignment)
{
    contentLabel->setAlignment(alignment);
}

void RToolTip::anchorTarget(QWidget *target)
{
    if(target&&target!=targetWidget){
        if(targetWidget){
            targetWidget->removeEventFilter(this);
        }
        targetWidget=target;
        targetWidget->installEventFilter(this);
        targetWidget->setMouseTracking(true);
        //如果是随窗口关闭的，看不到析构的打印，难道此时事件循环已停止？
        connect(targetWidget,&QObject::destroyed,this,&QObject::deleteLater);
    }
}

const QLabel *RToolTip::label() const
{
    return contentLabel;
}

void RToolTip::showTip(const QWidget *obj)
{
    if(!obj)
        return;
    showTip(obj->mapToGlobal(QPoint(0, 0)));
}

void RToolTip::showTip(const QPoint &rightBottom)
{
    targetPoint=rightBottom;
    //move(rightBottom.x() - width() + rightOffset,
    //     rightBottom.y() - height() + bottomOffset);
    //直接用size+point得到的位置可能显示不全，这里计算下

    int rect_left=rightBottom.x()-width()+rightOffset;
    int rect_top=rightBottom.y()-height()+bottomOffset;
    if(rect_left<0)
        rect_left=0;
    if(rect_top<0)
        rect_top=0;
    //要考虑多个显示器情况，【待测试】
    //根据当前所在屏幕尺寸计算
    QDesktopWidget * desktop=QApplication::desktop();
    if(desktop){
        QRect desk_rect=desktop->screenGeometry(targetWidget?targetWidget:this);
        if(rect_left+width()>desk_rect.width())
            rect_left=desk_rect.width()-width();
        if(rect_top+height()>desk_rect.height())
            rect_top=desk_rect.height()-height();
    }

    move(rect_left,rect_top);
    if(!showTimer.isActive())
        showTimer.start(200,this);
}

void RToolTip::hideTip()
{
    if(!hideTimer.isActive())
        hideTimer.start(300,this);
}

bool RToolTip::eventFilter(QObject *target, QEvent *event)
{
    if(target==targetWidget){
        switch (event->type()) {
        case QEvent::Enter:
            //showTip(QCursor::pos());
            showTip(targetWidget);
            break;
        case QEvent::Leave:
            hideTip();
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(target,event);
}

void RToolTip::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==showTimer.timerId()) {
        showTimer.stop();
        //hideTimer.stop();
        if(!hideTimer.isActive()&&isHidden()){
            show();
        }
    }else if(event->timerId()==hideTimer.timerId()){
        showTimer.stop();
        hideTimer.stop();
        if(!isHidden()){
            hide();
        }
    }else{
        QWidget::timerEvent(event);
    }
}

void RToolTip::resizeEvent(QResizeEvent *event)
{
    //初次show的时候可能size可能还没计算好
    showTip(targetPoint);
    QWidget::resizeEvent(event);
}
