#ifndef TOOLTIPDEMO_H
#define TOOLTIPDEMO_H

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
    Ui::ToolTipDemo *ui;
};

#endif // TOOLTIPDEMO_H
