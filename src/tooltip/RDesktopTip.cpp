#include "RDesktopTip.h"
#include "ui_RDesktopTip.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>

RDesktopTip* RDesktopTip::instance=nullptr;
RDesktopTip::AnimationMode RDesktopTip::mode=RDesktopTip::AllAnimation;

RDesktopTip::RDesktopTip()
    : QWidget(nullptr),
      ui(new Ui::RDesktopTip),
      showGroup(new QParallelAnimationGroup(this))
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    //setWindowModality(Qt::WindowModal);

    //resize会被label撑开
    setFixedSize(310,210);

    //关闭
    connect(ui->btnClose,&QPushButton::clicked,this,&RDesktopTip::hideTip);
    //程序退出时释放
    connect(qApp,&QApplication::aboutToQuit,this,&RDesktopTip::close);
    //动画设置
    initAnimation();
    //定时器设置
    initTimer();
}

RDesktopTip::~RDesktopTip()
{
    qDebug()<<"delete DesktopTip";
    delete ui;
}

void RDesktopTip::showTip(const QStringList &texts, int timeout)
{
    if(!instance){
        //仅在ui线程
        instance=new RDesktopTip;
    }
    instance->readyTimer(timeout);
    //模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setTextList(texts);
    instance->showAnimation();
}

void RDesktopTip::keepTip(const QStringList &texts)
{
    if(!instance){
        //仅在ui线程
        instance=new RDesktopTip;
    }
    instance->readyTimer(0);
    //模态框
    instance->setWindowModality(Qt::WindowModal);
    instance->setTextList(texts);
    instance->keepAnimation();
}

void RDesktopTip::hideTip()
{
    if(!instance){
        return;
    }
    instance->hideAnimation();
}

RDesktopTip::AnimationMode RDesktopTip::getMode()
{
    return mode;
}

void RDesktopTip::setMode(RDesktopTip::AnimationMode newMode)
{
    if(mode!=newMode){
        mode=newMode;
    }
}

void RDesktopTip::initAnimation()
{
    //透明度动画
    showOpacity=new QPropertyAnimation(this,"windowOpacity");
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
    showPos=new QPropertyAnimation(this,"pos");
    QScreen * screen = QGuiApplication::primaryScreen();
    if (screen) {
        const QRect desk_rect = screen->availableGeometry();
        const QPoint hide_pos{desk_rect.width()-this->width(),
                    desk_rect.height()};
        const QPoint show_pos{desk_rect.width()-this->width(),
                    desk_rect.height()-this->height()};
        //判断是否设置了此模式的动画
        if(mode&AnimationMode::PosAnimation){
            showPos->setDuration(1500);
            showPos->setStartValue(hide_pos);
        }else{
            showPos->setDuration(0);
            showPos->setStartValue(show_pos);
        }
        showPos->setEndValue(show_pos);
    }
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

void RDesktopTip::initTimer()
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

void RDesktopTip::readyTimer(int timeout)
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

void RDesktopTip::showAnimation()
{
    showGroup->setDirection(QAbstractAnimation::Forward);
    //停止正在进行的动画重新
    if(showGroup->state()==QAbstractAnimation::Running){
        showGroup->stop();
    }
    showGroup->start();
    show();
}

void RDesktopTip::keepAnimation()
{
    //show没有完成，或者正在动画中才进入
    if(!showAnimEnd||showGroup->state()!=QAbstractAnimation::Stopped){
        showGroup->setDirection(QAbstractAnimation::Forward);
        showGroup->start();
        show();
    }
}

void RDesktopTip::hideAnimation()
{
    //Backward反向执行动画
    showGroup->setDirection(QAbstractAnimation::Backward);
    showGroup->start();
}

void RDesktopTip::setTextList(const QStringList &texts)
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
