#include "RDesktopTip2.h"
#include "ui_RDesktopTip2.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>

RDesktopTip2* RDesktopTip2::instance=nullptr;
RDesktopTip2::AnimationMode RDesktopTip2::mode=RDesktopTip2::AllAnimation;

RDesktopTip2::RDesktopTip2()
    : QWidget(nullptr),
      ui(new Ui::RDesktopTip2),
      showGroup(new QParallelAnimationGroup(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    //setWindowModality(Qt::WindowModal);

    //resize会被label撑开
    this->setFixedSize(310,210);
    ui->tipArea->setFixedSize(310,210);

    //关闭
    connect(ui->btnClose,&QPushButton::clicked,this,&RDesktopTip2::hideTip);
    //程序退出时释放
    connect(qApp,&QApplication::aboutToQuit,this,&RDesktopTip2::close);
    //动画设置
    initAnimation();
    //定时器设置
    initTimer();
}

RDesktopTip2::~RDesktopTip2()
{
    qDebug()<<"delete DesktopTip2";
    delete ui;
}

void RDesktopTip2::showTip(const QStringList &texts, int timeout)
{
    if(!instance){
        //仅在ui线程
        instance=new RDesktopTip2;
    }
    instance->readyTimer(timeout);
    //模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setTextList(texts);
    instance->showAnimation();
}

void RDesktopTip2::keepTip(const QStringList &texts)
{
    if(!instance){
        //仅在ui线程
        instance=new RDesktopTip2;
    }
    instance->readyTimer(0);
    //模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setTextList(texts);
    instance->keepAnimation();
}

void RDesktopTip2::hideTip()
{
    if(!instance){
        return;
    }
    instance->hideAnimation();
}

RDesktopTip2::AnimationMode RDesktopTip2::getMode()
{
    return mode;
}

void RDesktopTip2::setMode(RDesktopTip2::AnimationMode newMode)
{
    if(mode!=newMode){
        mode=newMode;
    }
}

void RDesktopTip2::initAnimation()
{
    //透明度动画
    showOpacity=new QPropertyAnimation(ui->tipArea,"windowOpacity");
    //判断是否设置了此模式的动画
    if(mode&AnimationMode::OpacityAnimation){
        showOpacity->setDuration(1500);
        showOpacity->setStartValue(0);
    }else{
        showOpacity->setDuration(0);
        showOpacity->setStartValue(1);
    }
    showOpacity->setEndValue(1);
    showGroup->addAnimation(showOpacity);

    //位置动画
    showPos=new QPropertyAnimation(ui->tipArea,"pos");
    //QScreen * screen = QGuiApplication::primaryScreen();
    //if (screen) {
    //    const QRect desk_rect = screen->availableGeometry();
    //}
    //TODO 提供设置动画位置的接口
    //TODO 丰富入场动画选项
    const QPoint hide_pos{0,ui->tipArea->height()};
    const QPoint show_pos{0,0};
    //判断是否设置了此模式的动画
    if(mode&AnimationMode::PosAnimation){
        showPos->setDuration(1500);
        showPos->setStartValue(hide_pos);
    }else{
        showPos->setDuration(0);
        showPos->setStartValue(show_pos);
    }
    showPos->setEndValue(show_pos);
    showGroup->addAnimation(showPos);
    //
    connect(showGroup,&QParallelAnimationGroup::finished,[this]{
        //back消失动画结束关闭窗口
        if(showGroup->direction()==QAbstractAnimation::Backward){
            //Qt::WA_DeleteOnClose后手动设置为null
            instance=nullptr;
            qApp->disconnect(this);
            //关闭时设置为非模态，方式主窗口被遮挡，待测试
            this->setWindowModality(Qt::NonModal);
            this->close();
        }else{
            //配合keepAnimation
            showAnimEnd=true;
            //配合定时关闭
            if(hideCount>0)
                hideTimer->start();
        }
    });
}

void RDesktopTip2::initTimer()
{
    hideTimer=new QTimer(this);
    hideTimer->setInterval(1000); //1s间隔
    connect(hideTimer,&QTimer::timeout,[this]{
        if(hideCount>1){
            hideCount--;
            ui->btnClose->setText(QString("%1 S").arg(hideCount));
        }else{
            ui->btnClose->setText(QString("Close"));
            hideTimer->stop();
            hideTip();
        }
    });
}

void RDesktopTip2::readyTimer(int timeout)
{
    //先设置，在显示动画结束再start开始计时器
    hideCount=timeout;
    hideTimer->stop();

    if(hideCount>0){
        ui->btnClose->setText(QString("%1 S").arg(hideCount));
    }else{
        ui->btnClose->setText(QString("Close"));
    }
}

void RDesktopTip2::showAnimation()
{
    showGroup->setDirection(QAbstractAnimation::Forward);
    //停止正在进行的动画重新
    if(showGroup->state()==QAbstractAnimation::Running){
        showGroup->stop();
    }
    showGroup->start();
    show();
}

void RDesktopTip2::keepAnimation()
{
    //show没有完成，或者正在动画中才进入
    if(!showAnimEnd||showGroup->state()!=QAbstractAnimation::Stopped){
        showGroup->setDirection(QAbstractAnimation::Forward);
        showGroup->start();
        show();
    }
}

void RDesktopTip2::hideAnimation()
{
    //Backward反向执行动画
    showGroup->setDirection(QAbstractAnimation::Backward);
    showGroup->start();
}

void RDesktopTip2::setTextList(const QStringList &texts)
{
    QString tip_text("<p style='line-height:120%'>");
    for (const QString &text : texts)
    {
        if (text.isEmpty())
            continue;
        tip_text += text + "<br>";
    }
    tip_text += "</p>";
    ui->contentLabel->setText(tip_text);
}

