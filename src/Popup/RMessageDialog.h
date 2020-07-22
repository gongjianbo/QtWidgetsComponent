#ifndef RMESSAGEDIALOG_H
#define RMESSAGEDIALOG_H

#include "RBasicDialog.h"

namespace Ui {
class RMessageDialog;
}

/**
 * @brief 简易的消息对话框
 * @author 龚建波
 * @date 2020-7-22
 * @details 展示RBasicDialog的使用
 */
class RMessageDialog : public RBasicDialog
{
    Q_OBJECT
public:
    explicit RMessageDialog(QWidget *parent = nullptr);
    ~RMessageDialog();

    // 设置提示内容
    void setMessage(const QString &title,const QString &msg);
    // 创建并显示消息框 setMessage + exec
    static int showMessage(const QString &title,const QString &msg);

private:
    Ui::RMessageDialog *ui;
};

#endif // RMESSAGEDIALOG_H
