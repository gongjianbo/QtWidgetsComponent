#pragma once
#include <QWidget>
#include "CuteComponentExport.h"
#include "CuteBasicComboContainer.h"
#include "CuteBasicComboModel.h"
#include "CuteBasicComboPopup.h"
#include "CuteBasicComboView.h"
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QTimer;

/**
 * @brief 使用 QLineEdit + QPushButton + 弹出框组合的下拉框
 * @author 龚建波
 * @date 2020-7-6
 * @history
 * [2020-7-7]
 * 重构弹出框部分，增加可扩展性
 * 基础组件：Box+Popup，继承 Container 实现接口后设置给 Popup
 */
class Cute_API CuteBasicComboBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderWidth READ getBorderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(bool popupVisible READ getPopupVisible WRITE setPopupVisible NOTIFY popupVisibleChanged)
public:
    explicit CuteBasicComboBox(QWidget *parent = nullptr);
    ~CuteBasicComboBox();

    // 边框 size，自定义属性方便 qss 设置
    int getBorderWidth() const;
    void setBorderWidth(int px);

    // 弹框可见
    bool getPopupVisible() const;
    void setPopupVisible(bool visible);

    // 当前行
    int getCurrentIndex() const;
    void setCurrentIndex(int index);
    // 当前文本
    QString getCurrentText() const;
    void setCurrentText(const QString &text);
    // 数据项
    QList<QString> getItems() const;
    void setItems(const QList<QString> &items);
    // 弹出框
    CuteBasicComboPopup *getPopup() const;

protected:
    // 过滤组件事件
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // 初始化组件设置
    void initComponent();
    // popup-container
    void initContainer();
    // 编辑后查询对应行并设置 model
    void checkTextRow();

signals:
    void borderWidthChanged(int px);
    void popupVisibleChanged(bool visible);
    void currentIndexChanged(int index);
    void currentTextChanged(const QString text);

private:
    // 边框
    int borderWidth{ 3 };
    // 弹框当前可见
    bool popupVisible{ false };

    // 文本框
    QLineEdit *boxEdit{ nullptr };
    // 下拉按钮
    QPushButton *boxDown{ nullptr };
    // 布局
    QHBoxLayout *boxLayout{ nullptr };
    // 弹框
    CuteBasicComboPopup *boxPop{ nullptr };
    // 定时器
    QTimer *editTimer{ nullptr };
};
