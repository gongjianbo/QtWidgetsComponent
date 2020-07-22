#ifndef RBASICDIALOG_H
#define RBASICDIALOG_H

#include <QDialog>

class QPushButton;

namespace Ui {
class RBasicDialog;
}

/**
 * @brief 对话框/消息框基类
 * @author 龚建波
 * @date 2020-7-22
 * @details
 * 一、带ui文件子类使用方式:
 * 1.创建子类时，基类选择QDialog
 * 2.h头文件中#include "BasicDialog.h"，并作为父类
 * 3.cpp实现文件中默认的setupUi(this)改为setupUi(contentDialog)
 * 4.窗口resize或setFixedSize为目标大小（目前没有拖动大小功能）
 * 5.独立窗口给contentDialog设置样式表，这样不会把BasicDialog样式覆盖
 * 二、不带ui文件子类使用方式:
 * 1.h头文件中#include "BasicDialog.h"，并作为父类
 * 2.将内容布局Layout放到contentDialog中
 *
 * @note
 * 子类只设置内容区域，不设置标题栏
 */
class RBasicDialog : public QDialog
{
    Q_OBJECT
public:
    // 按键类型枚举，便于判别exec()返回值
    // setResult设置，result获取
    enum ButtonType
    {
        NoButton=0 //没有按钮
        ,Ok=1 //确认
        ,Cancel = 2 //取消，如点击关闭或取消
        ,Yes=3 //是
        ,No=4 //否
        //拓展注意位或操作
    };
public:
    explicit RBasicDialog(QWidget *parent = nullptr);
    // buttons传入ButtonType
    explicit RBasicDialog(int buttons, QWidget *parent = nullptr);
    ~RBasicDialog();

    // 设置标题
    void setDialogTitle(const QString& text);
    // 添加按钮，预定义枚举类型
    void addDefaultButton(int type,const QString &text=QString());
    // 添加按钮，type可作为exec返回值判断
    void addButton(QPushButton *button, int type);

protected:
    // 重写mouse，用于拖动对话框
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // 显示
    void showEvent(QShowEvent *event) override;
    // 点击关闭按钮，子类可重载处理取消的逻辑
    virtual void closeButtonClick();
    // 点击了操作按钮，确认取消等
    virtual void buttonClick(int type);

private:
    Ui::RBasicDialog *basicui;
    // 拖动状态
    bool onMoving = false;
    // 拖动位置暂存
    QPoint posTemp = QPoint(0,0);

protected:
    // 用于子类setupUi
    QDialog *contentDialog = nullptr;
};

#endif // RBASICDIALOG_H
