#pragma once
#include <QWidget>

namespace Ui {
class ToolTipDemo;
}

class ToolTipDemo : public QWidget
{
    Q_OBJECT
public:
    explicit ToolTipDemo(QWidget *parent = nullptr);
    ~ToolTipDemo();

private:
    void initToolTip();

private:
    Ui::ToolTipDemo *ui;
};
