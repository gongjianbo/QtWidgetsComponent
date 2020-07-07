#ifndef RBASICCOMBOCONTAINER_H
#define RBASICCOMBOCONTAINER_H

#include <QWidget>

#include "RBasicComboView.h"

/**
 * @brief BasicComboBox的弹框中的实体
 * @author 龚建波
 * @date 2020-7-6
 * @details 自定义时继承该类并实现对应接口
 * 附带了一个ListView版本的默认实现供参考
 */
class RBasicComboContainer : public QWidget
{
    Q_OBJECT
public:
    //init=true则调用默认初始化
    explicit RBasicComboContainer(bool init=true,QWidget *parent = nullptr);

    //box需要回调的接口
    //当前行
    virtual int getCurrentIndex() const;
    virtual void setCurrentIndex(int index);
    //数据项
    virtual QList<QString> getItems() const;
    virtual void setItems(const QList<QString> &items);
    //根据文本设置当前行，返回当前行，-1为无效
    virtual int checkTextRow(const QString &text);
    //切换并获取当前行文本
    virtual QString getCurrentText() const;
    virtual QString getPrevText();
    virtual QString getNextText();

    //popup回调的接口
    virtual int getContentsHeight() const;

private:
    void initDefault();

signals:
    void currentIndexChanged(int index);
    void updateData();

private:
    RBasicComboView *defaultView=nullptr;
};

#endif // RBASICCOMBOCONTAINER_H
