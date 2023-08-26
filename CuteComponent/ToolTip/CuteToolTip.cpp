#include "CuteToolTip.h"
#include <QGuiApplication>
#include <QScreen>
#include <QEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QHBoxLayout>
//#include <QDebug>

CuteToolTip::CuteToolTip(const QString text, QWidget *parent)
    : QDialog{parent}
    , contentLabel{new QLabel(this)}
{
    // 把内层 label 添加到透明 dialog
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(contentLabel);

    contentLabel->setAlignment(Qt::AlignCenter);
    contentLabel->setText(text);

    // 把 dialog 设置为透明，样式表设置给 label
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);
    // 默认 hide
    setVisible(false);
    // qDebug() << __FUNCTION__;
}

CuteToolTip::~CuteToolTip()
{
    // qDebug() << __FUNCTION__;
}

int CuteToolTip::getXOffset() const
{
    return xOffset;
}

void CuteToolTip::setXOffset(int offset)
{
    if (xOffset != offset){
        xOffset = offset;
        // 没有动态样式的处理，可自行添加
        // style()->unpolish(this);
        // style()->polish(this);
        emit xOffsetChanged();
    }
}

int CuteToolTip::getYOffset() const
{
    return yOffset;
}

void CuteToolTip::setYOffset(int offset)
{
    if (yOffset != offset){
        yOffset = offset;
        emit yOffsetChanged();
    }
}

QString CuteToolTip::getText() const
{
    return contentLabel->text();
}

void CuteToolTip::setText(const QString &text)
{
    if (getText() != text) {
        contentLabel->setText(text);
        emit textChanged();
    }
}

Qt::Alignment CuteToolTip::getAlignment() const
{
    return contentLabel->alignment();
}

void CuteToolTip::setAlignment(Qt::Alignment alignment)
{
    if (getAlignment() != alignment) {
        contentLabel->setAlignment(alignment);
        emit alignmentChanged();
    }
}

void CuteToolTip::anchorTarget(QWidget *target)
{
    if (!target) return;
    if (target != targetWidget) {
        if (targetWidget) {
            targetWidget->removeEventFilter(this);
        }
        targetWidget = target;
        targetWidget->installEventFilter(this);
        targetWidget->setMouseTracking(true);
    }
}

const QLabel *CuteToolTip::label() const
{
    return contentLabel;
}

void CuteToolTip::showTip(const QWidget *obj)
{
    if (!obj) return;
    showTip(obj->mapToGlobal(QPoint(0, 0)));
}

void CuteToolTip::showTip(const QPoint &rightBottom)
{
    targetPoint = rightBottom;
    // move(rightBottom.x() - width() + xOffset,
    //      rightBottom.y() - height() + yOffset);
    // 直接用size + point得到的位置可能显示不全，这里计算下

    int rect_left = rightBottom.x() - width() + xOffset;
    int rect_top = rightBottom.y() - height() + yOffset;
    // 根据当前所在屏幕尺寸计算，左上角不超过范围
    // 因为锚定的左上角显示，右下角暂不考虑
    // 老版本用 QDesktopWidget
    // QDesktopWidget *desktop = QApplication::desktop();
    // QRect desk_rect=desktop->screenGeometry(targetWidget);
    // Qt5.11 QDesktopWidget 部分接口被废弃，Qt6 该类被移除
    // 新版本用 QScreen
    // 直接用 widget 的 screen() 获取的是整个 window 所在 screen，而不是组件的
    // 跨屏时在哪个屏幕占的区域最大，就以该屏幕作为组件的所属 screen
    QScreen *screen = nullptr;
    if (targetWidget) {
        screen = QGuiApplication::screenAt(targetWidget->mapToGlobal(targetWidget->rect().center()));
    }
    if (screen) {
        QRect desk_rect = screen->geometry();
        if(rect_left < desk_rect.x())
            rect_left = desk_rect.x();
        if(rect_top < desk_rect.y())
            rect_top = desk_rect.y();
    } else {
        if (rect_left < 0)
            rect_left = 0;
        if (rect_top < 0)
            rect_top = 0;
    }

    move(rect_left, rect_top);
    if (!showTimer.isActive())
        showTimer.start(200, this);
}

void CuteToolTip::hideTip()
{
    if (!hideTimer.isActive())
        hideTimer.start(300, this);
}

bool CuteToolTip::eventFilter(QObject *target, QEvent *event)
{
    if (target == targetWidget) {
        switch (event->type()) {
        case QEvent::Enter:
            // showTip(QCursor::pos());
            showTip(targetWidget);
            break;
        case QEvent::Leave:
            hideTip();
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(target, event);
}

void CuteToolTip::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == showTimer.timerId()) {
        showTimer.stop();
        // hideTimer.stop();
        if (!hideTimer.isActive() && isHidden()) {
            show();
        }
    } else if (event->timerId() == hideTimer.timerId()) {
        showTimer.stop();
        hideTimer.stop();
        if (!isHidden()) {
            hide();
        }
    } else {
        QWidget::timerEvent(event);
    }
}

void CuteToolTip::resizeEvent(QResizeEvent *event)
{
    // 初次 show 的时候可能 size 可能还没计算好
    showTip(targetPoint);
    QWidget::resizeEvent(event);
}
