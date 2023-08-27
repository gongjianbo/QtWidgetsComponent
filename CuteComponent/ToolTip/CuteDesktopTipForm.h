#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class CuteDesktopTipForm;
}

/**
 * @brief CuteDesktopTip 的默认窗口界面
 * @author 龚建波
 * @date 2023-08-27
 * @note
 * 样式设置放在了 ui 中
 * @details
 * 将 CuteDesktopTip 的逻辑和界面拆开
 * 内部包含一组小部件，外部可通过 objectName 查找
 * icon: QLabel
 * title: QLabel
 * close: QPushButton
 * contet: QLabel
 * 如果要把 Form 做成可替换的，也可以用 objectName 或者增加一个接口类
 */
class CuteDesktopTipForm : public QWidget
{
    Q_OBJECT
public:
    explicit CuteDesktopTipForm(QWidget *parent = nullptr);
    ~CuteDesktopTipForm();

private:
    Ui::CuteDesktopTipForm *ui;
};
