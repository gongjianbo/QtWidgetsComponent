#pragma once
#include <QDialog>
#include <QLabel>
#include <QBasicTimer>
#include "CuteComponentExport.h"

/**
 * @brief 最简易的 ToolTip 提示框
 * @author 龚建波
 * @date 2020-6-7
 * @details
 * 外部为 QDialog，设置 parent 后也能独立窗口显示
 * 内部套一个 QLabel 显示文字
 */
class Cute_API CuteToolTip : public QDialog
{
    Q_OBJECT
    // 默认显示为 point 的左上角，通过属性设置偏移，左-右+，上-下+，同屏幕坐标系规则
    // qss 右移 1px: qproperty-xOffset:"1";
    Q_PROPERTY(int xOffset READ getXOffset WRITE setXOffset NOTIFY xOffsetChanged)
    // qss 上移 1px: qproperty-yOffset:"-1";
    Q_PROPERTY(int yOffset READ getYOffset WRITE setYOffset NOTIFY yOffsetChanged)
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(Qt::Alignment alignment READ getAlignment WRITE setAlignment NOTIFY alignmentChanged)
public:
    explicit CuteToolTip(const QString text = QString(), QWidget *parent = nullptr);
    ~CuteToolTip();

    // x 轴偏移
    int getXOffset() const;
    void setXOffset(int offset);
    // y 轴偏移
    int getYOffset() const;
    void setYOffset(int offset);

    // 文本
    QString getText() const;
    void setText(const QString &text);
    // 对齐方式
    Qt::Alignment getAlignment() const;
    void setAlignment(Qt::Alignment alignment);

    // 设置锚定窗口，鼠标放上去时显示 tooltip
    void anchorTarget(QWidget *target);
    // 获取内层 label 对象
    const QLabel *label() const;
    // 显示 tip 在 widget 的左上角
    void showTip(const QWidget *obj);
    // 显示 tip 在点的左上角
    void showTip(const QPoint &rightBottom);
    // 隐藏 tip
    void hideTip();

protected:
    // 过滤锚定窗口的 enter 和 leave 事件
    bool eventFilter(QObject *target, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void xOffsetChanged();
    void yOffsetChanged();
    void textChanged();
    void alignmentChanged();

private:
    // 内部套一个 QLabel 显示文字
    QLabel *contentLabel{ nullptr };
    // 默认显示为 point 的左上角，通过属性设置偏移
    // 左-右+，上-下+，同屏幕坐标系规则
    int xOffset{ 0 };
    int yOffset{ 0 };

    // 锚定的窗口
    QWidget *targetWidget{ nullptr };
    QPoint targetPoint;
    // show 和 hide 延迟
    QBasicTimer showTimer;
    QBasicTimer hideTimer;
};
