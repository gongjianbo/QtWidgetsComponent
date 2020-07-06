#include "RBasicComboModel.h"

RBasicComboModel::RBasicComboModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int RBasicComboModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return modelData.count();
}

QVariant RBasicComboModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role==Qt::DisplayRole){
        return modelData.at(index.row());
    }

    return QVariant();
}

QList<QString> RBasicComboModel::getModelData() const
{
    return modelData;
}

void RBasicComboModel::setModelData(const QList<QString> &datas)
{
    beginResetModel();
    modelData=datas;
    endResetModel();
}

int RBasicComboModel::getDataRow(const QString &text)
{
    for(int row=0;row<modelData.count();row++){
        //可以用qstring的匹配接口，如忽略大小写等
        if(text==modelData.at(row)){
            return row;
        }
    }
    return -1;
}
