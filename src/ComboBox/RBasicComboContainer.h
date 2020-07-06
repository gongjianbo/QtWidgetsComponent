#ifndef RBASICCOMBOCONTAINER_H
#define RBASICCOMBOCONTAINER_H

#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>

#include "RBasicComboView.h"

/**
 * @brief BasicComboBox的弹框整体
 * @author 龚建波
 * @date 2020-7-6
 */
class RBasicComboContainer : public QWidget
{
    Q_OBJECT
public:
    explicit RBasicComboContainer(QWidget *parent = nullptr);

    //弹框显示隐藏
    void showPopup();
    void hidePopup();

    //绑定弹出的目标组件，在其上或其下弹出
    void attachTarget(QWidget *widget);
    //列表
    RBasicComboView *getBasicView() const;

private:
    //初始化组件设置
    void initComponent();
    //设置弹出框大小
    void setPopSize(int width,int height);

private:
    //绑定下拉框主体，用于计算弹出位置
    QWidget *targetWidget=nullptr;
    //内部有一个widget用于弹出动画
    QWidget *wrapper=new QWidget(this);
    //弹出框中的列表
    RBasicComboView *view=new RBasicComboView(this);
    //弹出动画
    QPropertyAnimation *animation=nullptr;
};

#endif // RBASICCOMBOCONTAINER_H
