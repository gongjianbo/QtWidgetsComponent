#pragma once
#include <QWidget>

namespace Ui {
class PopupDemo;
}

class PopupDemo : public QWidget
{
    Q_OBJECT
public:
    explicit PopupDemo(QWidget *parent = nullptr);
    ~PopupDemo();

private:
    void initComboBox();

private:
    Ui::PopupDemo *ui;
};
