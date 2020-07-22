#ifndef POPUPDEMO_H
#define POPUPDEMO_H

#include <QWidget>

namespace Ui {
class PopupDemo;
}

/**
 * @brief 弹出框的demo集合页
 * @author 龚建波
 * @date 2020-7-6
 */
class PopupDemo : public QWidget
{
    Q_OBJECT

public:
    explicit PopupDemo(QWidget *parent = nullptr);
    ~PopupDemo();

private:
    void initBasicComboBox();

private:
    Ui::PopupDemo *ui;
};

#endif // POPUPDEMO_H
