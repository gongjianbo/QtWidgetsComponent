#ifndef COMBOBOXDEMO_H
#define COMBOBOXDEMO_H

#include <QWidget>

namespace Ui {
class ComboBoxDemo;
}


/**
 * @brief 下拉框的demo集合页
 * @author 龚建波
 * @date 2020-7-6
 */
class ComboBoxDemo : public QWidget
{
    Q_OBJECT

public:
    explicit ComboBoxDemo(QWidget *parent = nullptr);
    ~ComboBoxDemo();

private:
    Ui::ComboBoxDemo *ui;
};

#endif // COMBOBOXDEMO_H
