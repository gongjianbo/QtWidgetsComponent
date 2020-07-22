#ifndef RBASICCOMBOBOX_H
#define RBASICCOMBOBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>

#include "RBasicComboPopup.h"

/**
 * @brief 使用QLineEdit+QPushButton+弹出框组合的下拉框
 * @author 龚建波
 * @date 2020-7-6
 * @history
 * [2020-7-7]
 * 重构弹出框部分，增加可扩展性
 * 基础组件：Box+Popup，继承Container实现接口后设置给Popup
 */
class RBasicComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit RBasicComboBox(QWidget *parent = nullptr);
    ~RBasicComboBox();

    //当前行
    int getCurrentIndex() const;
    void setCurrentIndex(int index);
    //当前文本
    QString getCurrentText() const;
    void setCurrentText(const QString text);
    //数据项
    QList<QString> getItems() const;
    void setItems(const QList<QString> &items);
    //弹出框
    RBasicComboPopup *getPopup() const;

protected:
    //过滤组件事件
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    //初始化组件设置
    void initComponent();
    //popup-container
    void initContainer();
    //默认样式设置
    void initStyleSheet();
    //编辑后查询对应行并设置model
    void checkTextRow();

signals:
    void currentIndexChanged(int index);
    void currentTextChanged(const QString text);

private:
    //可以使用前置声明，然后把new放到初始化列表去
    //文本框
    QLineEdit *boxEdit=new QLineEdit(this);
    //按钮
    QPushButton *boxButton=new QPushButton(this);
    //布局
    QHBoxLayout *boxLayout=new QHBoxLayout(this);
    //弹框
    RBasicComboPopup *boxPop=new RBasicComboPopup(this);
    //定时器
    QTimer *editTimer=new QTimer(this);
};

#endif // RBASICCOMBOBOX_H
