#ifndef RBASICCOMBOBOX_H
#define RBASICCOMBOBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>

#include "RBasicComboContainer.h"

/**
 * @brief 使用QLineEdit+QPushButton+QListView组合的下拉框
 * @author 龚建波
 * @date 2020-7-6
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
protected:
    //过滤组件事件
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    //初始化组件设置
    void initComponent();
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
    RBasicComboContainer *container=new RBasicComboContainer(this);
    //定时器
    QTimer *editTimer=new QTimer(this);
};

#endif // RBASICCOMBOBOX_H
