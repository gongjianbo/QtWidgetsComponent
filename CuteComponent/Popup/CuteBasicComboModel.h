#pragma once
#include <QAbstractListModel>
#include "CuteComponentExport.h"

/**
 * @brief BasicComboBox 默认弹框的 model
 * @author 龚建波
 * @date 2020-7-6
 */
class Cute_API CuteBasicComboModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CuteBasicComboModel(QObject *parent = nullptr);

    // item 个数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // item 数据获取
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 自定义接口
    // 对应自己的数据类型，普通的下拉框文本足矣
    QList<QString> getModelData() const;
    void setModelData(const QList<QString> &datas);
    // 查到内容对应行
    int getDataRow(const QString &text);

private:
    // 对应自己的数据类型，普通的下拉框文本足矣
    QList<QString> modelData;
};
