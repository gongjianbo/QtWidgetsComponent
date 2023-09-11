#include "CuteDesktopTip.h"
#include <QApplication>
#include <QScreen>
//#include <QDebug>

CuteDesktopTip *CuteDesktopTip::instance = nullptr;
CuteDesktopTip::AnimationMode CuteDesktopTip::aniMode = CuteDesktopTip::AllAnimation;
CuteDesktopTip::DisplayArea CuteDesktopTip::aniArea = CuteDesktopTip::RightBottomArea;

CuteDesktopTip::CuteDesktopTip(QWidget *parent)
    : QDialog{parent}
    , form{new CuteDesktopTipForm(this)}
    , aniGroup{new QParallelAnimationGroup(this)}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    // setWindowModality(Qt::WindowModal);

    // resize 会被 label 撑开
    this->setFixedSize(310,210);
    form->setFixedSize(310,210);

    // 关闭
    QPushButton *btn_close = form->findChild<QPushButton *>("close");
    if (btn_close) {
        connect(btn_close, &QPushButton::clicked, this, &CuteDesktopTip::hideTip);
    }
    // 程序退出时释放
    connect(qApp, &QApplication::aboutToQuit, this, &CuteDesktopTip::close);
    // 动画设置
    initAnimation();
    // 定时器设置
    initTimer();
    // qDebug() << __FUNCTION__;
}

CuteDesktopTip::~CuteDesktopTip()
{
    // qDebug() << __FUNCTION__;
}

CuteDesktopTip::AnimationMode CuteDesktopTip::getMode()
{
    return aniMode;
}

void CuteDesktopTip::setMode(AnimationMode mode)
{
    if (aniMode != mode) {
        aniMode = mode;
    }
}

CuteDesktopTip::DisplayArea CuteDesktopTip::getArea()
{
    return aniArea;
}

void CuteDesktopTip::setArea(DisplayArea area)
{
    if (aniArea != area) {
        aniArea = area;
    }
}

void CuteDesktopTip::showTip(const QString &title, const QString &text, int timeout)
{
    if (!instance) {
        // 仅在ui线程
        instance = new CuteDesktopTip;
    }
    instance->readyTimer(timeout);
    // 模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setText(title, text);
    instance->showAnimation();
}

void CuteDesktopTip::keepTip(const QString &title, const QString &text)
{
    if (!instance) {
        // 仅在ui线程
        instance = new CuteDesktopTip;
    }
    instance->readyTimer(0);
    // 模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setText(title, text);
    instance->keepAnimation();
}

void CuteDesktopTip::hideTip()
{
    if (!instance) {
        return;
    }
    instance->hideAnimation();
}

void CuteDesktopTip::initAnimation()
{
    // 透明度动画，窗口才能设置透明度
    aniOpacity = new QPropertyAnimation(this, "windowOpacity");
    // 判断是否设置了此模式的动画
    if (aniMode & AnimationMode::OpacityAnimation) {
        aniOpacity->setDuration(1500);
        aniOpacity->setStartValue(0);
    } else {
        aniOpacity->setDuration(0);
        aniOpacity->setStartValue(1);
    }
    aniOpacity->setEndValue(1);
    aniGroup->addAnimation(aniOpacity);

    // 位置动画
    aniPos = new QPropertyAnimation(form, "pos");
    // 右下角放主屏，居中默认在当前窗口所在屏幕
    if (aniArea == DisplayArea::RightBottomArea) {
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect rect = screen->availableGeometry();
            move(rect.right() - width(), rect.bottom() - height());
        }
    }
    const QPoint hide_pos{0, form->height()};
    const QPoint show_pos{0, 0};
    // 判断是否设置了此模式的动画
    if (aniMode & AnimationMode::PosAnimation){
        aniPos->setDuration(1500);
        aniPos->setStartValue(hide_pos);
    } else {
        aniPos->setDuration(0);
        aniPos->setStartValue(show_pos);
    }
    aniPos->setEndValue(show_pos);
    aniGroup->addAnimation(aniPos);
    //
    connect(aniGroup, &QParallelAnimationGroup::finished, [this]{
        // back 消失动画结束关闭窗口
        if (aniGroup->direction() == QAbstractAnimation::Backward) {
            // Qt::WA_DeleteOnClose 后手动设置为 nullptr
            instance = nullptr;
            qApp->disconnect(this);
            // 关闭时设置为非模态，方式主窗口被遮挡，待测试
            this->setWindowModality(Qt::NonModal);
            this->close();
        } else {
            // 配合 keepAnimation
            showAniEnd = true;
            //配合定时关闭
            if (hideCount > 0) {
                hideTimer->start();
            }
        }
    });
}

void CuteDesktopTip::initTimer()
{
    hideTimer = new QTimer(this);
    // 1s 间隔
    hideTimer->setInterval(1000);
    connect(hideTimer, &QTimer::timeout, [this]{
        QPushButton *btn_close = form->findChild<QPushButton *>("close");
        if (hideCount > 1) {
            hideCount--;
            if (btn_close) {
                btn_close->setText(QString("%1 S").arg(hideCount));
            }
        } else {
            if (btn_close) {
                btn_close->setText("Close");
            }
            hideTimer->stop();
            hideTip();
        }
    });
}

void CuteDesktopTip::readyTimer(int timeout)
{
    // 先设置，在显示动画结束再 start 开始计时器
    hideCount = timeout;
    hideTimer->stop();

    QPushButton *btn_close = form->findChild<QPushButton *>("close");
    if (hideCount > 0) {
        if (btn_close) {
            btn_close->setText(QString("%1 S").arg(hideCount));
        }
    } else {
        if (btn_close) {
            btn_close->setText("Close");
        }
    }
}

void CuteDesktopTip::showAnimation()
{
    aniGroup->setDirection(QAbstractAnimation::Forward);
    // 停止正在进行的动画重新
    if (aniGroup->state() == QAbstractAnimation::Running) {
        aniGroup->stop();
    }
    aniGroup->start();
    show();
}

void CuteDesktopTip::keepAnimation()
{
    // show 没有完成，或者正在动画中才进入
    if (!showAniEnd || aniGroup->state()!=QAbstractAnimation::Stopped) {
        aniGroup->setDirection(QAbstractAnimation::Forward);
        aniGroup->start();
        show();
    }
}

void CuteDesktopTip::hideAnimation()
{
    // Backward 反向执行动画
    aniGroup->setDirection(QAbstractAnimation::Backward);
    aniGroup->start();
}

void CuteDesktopTip::setText(const QString &title, const QString &text)
{
    QLabel *title_label = form->findChild<QLabel *>("title");
    if (title_label) {
        title_label->setText(title);
    }
    QLabel *content_label = form->findChild<QLabel *>("content");
    if (content_label) {
        QString tip_text("<p style='line-height:120%'>");
        QStringList tip_list = text.split("\n");
        for (const QString &line : tip_list)
        {
            if (line.isEmpty())
                continue;
            tip_text += line + "<br>";
        }
        tip_text += "</p>";
        content_label->setText(tip_text);
    }
}
