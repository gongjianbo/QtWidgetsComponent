#pragma once
#include <QWidget>
#include <QListView>
#include "CuteComponentExport.h"
class CuteBasicComboModel;

/**
 * @brief BasicComboBox 默认弹框中的列表
 * @author 龚建波
 * @date 2020-7-6
 */
class Cute_API CuteBasicComboView : public QListView
{
    Q_OBJECT
public:
    explicit CuteBasicComboView(QWidget *parent = nullptr);

    // 获取当前行，避免和已有接口重名
    int getCurrentRow() const;
    void setCurrentRow(int row);
    // 根据文本设置当前行，返回当前行，-1为无效
    int checkTextRow(const QString &text);
    // 切换并获取当前行文本
    QString getCurrentText();
    QString getNextText();
    QString getPrevText();
    // 设置 model
    CuteBasicComboModel *getBasicModel() const;
    void setBasicModel(CuteBasicComboModel *model);
    // 获取内容高度
    int getContentsHeight() const;

protected:
    // 过滤一些按键操作
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initModel();
    void initView();

signals:
    // 手动点击触发信号
    void rowClicked(int row);
    // model 重置了数据
    void modelReseted();
    // 当前行改变
    void currentRowChanged(int row);

private:
    CuteBasicComboModel *basicModel{ nullptr };
};
