#ifndef RBASICCOMBOMODEL_H
#define RBASICCOMBOMODEL_H

#include <QAbstractListModel>

/**
 * @brief BasicComboBox的数据model
 * @author 龚建波
 * @date 2020-7-6
 */
class RBasicComboModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit RBasicComboModel(QObject *parent = nullptr);

    //item个数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //item数据获取
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //自定义接口
    //对应自己的数据类型，普通的下拉框文本足矣
    QList<QString> getModelData() const;
    void setModelData(const QList<QString> &datas);
    //查到内容对应行
    int getDataRow(const QString &text);

private:
    //对应自己的数据类型，普通的下拉框文本足矣
    QList<QString> modelData;
};

#endif // RBASICCOMBOMODEL_H
