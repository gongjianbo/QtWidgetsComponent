#ifndef TOOLTIPDEMO_H
#define TOOLTIPDEMO_H

#include <QWidget>

namespace Ui {
class ToolTipDemo;
}

/**
 * @brief 提示框/弹框的demo集合页
 * @author 龚建波
 * @date 2020-6-20
 */
class ToolTipDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ToolTipDemo(QWidget *parent = nullptr);
    ~ToolTipDemo();

private:
    void initToolTip();
    void initDesktopTip();

private:
    Ui::ToolTipDemo *ui;
};

#endif // TOOLTIPDEMO_H
