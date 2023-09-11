#pragma once
#include <QWidget>
#include <QPropertyAnimation>
#include "CuteComponentExport.h"
class CuteBasicComboContainer;

/**
 * @brief BasicComboBox 的弹框
 * @author 龚建波
 * @date 2020-7-7
 */
class Cute_API CuteBasicComboPopup : public QWidget
{
    Q_OBJECT
public:
    explicit CuteBasicComboPopup(QWidget *parent = nullptr);

    // 弹框显示隐藏
    void showPopup();
    void hidePopup();

    // 绑定弹出的目标组件，在其上或其下弹出
    void attachTarget(QWidget *widget);
    // 最终弹出的组件
    CuteBasicComboContainer *getContainer();
    void setContainer(CuteBasicComboContainer *widget);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    // 设置弹出框大小
    void setPopSize(int width, int height);

signals:
    void containerChanged();
    void visibleChanged(bool visible);

private:
    // 绑定下拉框主体，用于计算弹出位置
    QWidget *targetWidget{ nullptr };
    // 弹出动画
    QPropertyAnimation *animation{ nullptr };
    // 弹出框实际展示的widget
    CuteBasicComboContainer *container{ nullptr };
};
