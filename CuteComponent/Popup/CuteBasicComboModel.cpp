#include "CuteBasicComboModel.h"

CuteBasicComboModel::CuteBasicComboModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

int CuteBasicComboModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return modelData.count();
}

QVariant CuteBasicComboModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return modelData.at(index.row());
    }

    return QVariant();
}

QList<QString> CuteBasicComboModel::getModelData() const
{
    return modelData;
}

void CuteBasicComboModel::setModelData(const QList<QString> &datas)
{
    beginResetModel();
    modelData = datas;
    endResetModel();
}

int CuteBasicComboModel::getDataRow(const QString &text)
{
    for (int row = 0; row < modelData.count(); row++) {
        // 可以用qstring的匹配接口，如忽略大小写等
        if (text == modelData.at(row)) {
            return row;
        }
    }
    return -1;
}
