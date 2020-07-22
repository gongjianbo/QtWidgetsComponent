#ifndef RBASICCOMBOVIEW_H
#define RBASICCOMBOVIEW_H

#include <QWidget>
#include <QListView>

#include "RBasicComboModel.h"

/**
 * @brief BasicComboBox默认弹框的list
 * @author 龚建波
 * @date 2020-7-6
 */
class RBasicComboView : public QListView
{
    Q_OBJECT
public:
    explicit RBasicComboView(QWidget *parent = nullptr);

    //获取当前行，避免和已有接口重名
    int getCurrentRow() const;
    void setCurrentRow(int row);
    //根据文本设置当前行，返回当前行，-1为无效
    int checkTextRow(const QString &text);
    //切换并获取当前行文本
    QString getCurrentText();
    QString getNextText();
    QString getPrevText();
    //设置model
    RBasicComboModel *getBasicModel() const;
    void setBasicModel(RBasicComboModel *model);
    //获取内容高度
    int getContentsHeight() const;

protected:
    //过滤一些按键操作
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initModel();
    //void initDelegate();
    void initView();

signals:
    //手动点击触发信号
    void rowClicked(int row);
    //model重置了数据
    void modelReseted();
    //当前行改变
    void currentRowChanged(int row);

private:
    RBasicComboModel *basicModel;
};

#endif // RBASICCOMBOVIEW_H
