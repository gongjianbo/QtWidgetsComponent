#pragma once
#include <QDialog>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include "CuteComponentExport.h"
#include "CuteDesktopTipForm.h"

/**
 * @brief 桌面弹框
 * @author 龚建波
 * @date 2020-6-28
 * @note
 * 可以增加 show 的 parent 参数，使模态有父子关系
 * 目前没有样式设置接口，可以增加一个 setForm 接口设置内部窗口
 * @details
 * show 时创建，close 时释放
 * 分为外层透明区域限定位置，和内层窗口样式
 */
class Cute_API CuteDesktopTip : public QDialog
{
    Q_OBJECT
public:
    // 动画模式枚举
    enum AnimationMode
    {
        // 无动画
        NoAnimation = 0x00,
        // 仅透明度动画
        OpacityAnimation = 0x01,
        // 仅位置动画
        PosAnimation = 0x02,
        // 全部动画
        AllAnimation = 0xFF
    };
    // 显示位置枚举
    enum DisplayArea
    {
        // 当前窗口所在屏幕居中
        CenterArea = 0x00,
        // 主屏幕右下角
        RightBottomArea = 0x01
    };
private:
    explicit CuteDesktopTip(QWidget *parent = nullptr);
public:
    ~CuteDesktopTip();

    // 设置动画模式
    static CuteDesktopTip::AnimationMode getMode();
    static void setMode(CuteDesktopTip::AnimationMode mode);

    // 设置显示位置
    static CuteDesktopTip::DisplayArea getArea();
    static void setArea(CuteDesktopTip::DisplayArea area);

    // 显示弹框 -已显示动画重新开始, timeout <= 0 不会定时消失
    static void showTip(const QString &title, const QString &text, int timeout = 0);
    // 显示弹框 -已显示不重复动画
    static void keepTip(const QString &title, const QString &text);
    // 隐藏弹框
    static void hideTip();

private:
    // 初始化动画设置
    void initAnimation();
    // 初始化定时器设置
    void initTimer();
    // 准备定时器
    void readyTimer(int timeout);
    // 启动显示动画-已显示动画重新开始
    void showAnimation();
    // 启动显示动画-已显示不重复动画
    void keepAnimation();
    // 启动隐藏动画
    void hideAnimation();
    // 显示的标题和文本
    void setText(const QString &title, const QString &text);

private:
    // form 为独立的 widget，封装 ui 相关接口
    CuteDesktopTipForm *form{ nullptr };

    // 单例
    static CuteDesktopTip *instance;
    // 动画设置
    static AnimationMode aniMode;
    // 显示位置
    static DisplayArea aniArea;

    // 动画组
    QParallelAnimationGroup *aniGroup{ nullptr };
    // 透明度属性动画
    QPropertyAnimation *aniOpacity{ nullptr };
    // 位置属性动画
    QPropertyAnimation *aniPos{ nullptr };
    // 动画结束标志，因为每次都会创建新的弹框，所以默认 false 表示正在动画
    bool showAniEnd{ false };

    // 定时关闭
    QTimer *hideTimer{ nullptr };
    // 定时计数
    int hideCount{ 0 };
};
