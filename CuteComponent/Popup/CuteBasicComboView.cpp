#include "CuteBasicComboView.h"
#include "CuteBasicComboModel.h"
#include <QKeyEvent>
#include <QDebug>

CuteBasicComboView::CuteBasicComboView(QWidget *parent)
    : QListView{parent}
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    initModel();
    initView();
}

int CuteBasicComboView::getCurrentRow() const
{
    // 获取当前行
    const QModelIndex current = currentIndex();
    if (current.isValid()) {
        return current.row();
    }
    return -1;
}

void CuteBasicComboView::setCurrentRow(int row)
{
    // 设置当前行
    if (row == getCurrentRow())
        return;
    if (row >= 0 && row < basicModel->rowCount()) {
        setCurrentIndex(basicModel->index(row, 0));
        emit currentRowChanged(row);
    }
}

int CuteBasicComboView::checkTextRow(const QString &text)
{
    // 根据内容设置当前行
    const int row = basicModel->getDataRow(text);
    if (row >= 0) {
        setCurrentRow(row);
        return row;
    }
    return -1;
}

QString CuteBasicComboView::getCurrentText()
{
    QModelIndex current = currentIndex();
    // 如果当前行无效就去设置一个默认行0
    if (!current.isValid()) {
        if (basicModel->rowCount() > 0) {
            current = basicModel->index(0, 0);
            // setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current, Qt::DisplayRole).toString();
        }
        return QString();
    }
    return basicModel->data(current, Qt::DisplayRole).toString();
}

QString CuteBasicComboView::getNextText()
{
    QModelIndex current = currentIndex();
    if (current.isValid()) {
        if (current.row() < basicModel->rowCount() - 1) {
            // 设置到下一行
            current = basicModel->index(current.row() + 1, 0);
            // setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current, Qt::DisplayRole).toString();
        }
    }
    return getCurrentText();
}

QString CuteBasicComboView::getPrevText()
{
    QModelIndex current = currentIndex();
    if (current.isValid()) {
        if (current.row() > 0) {
            // 设置到上一行
            current = basicModel->index(current.row() - 1, 0);
            // setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current, Qt::DisplayRole).toString();
        }
    }
    return getCurrentText();
}

CuteBasicComboModel *CuteBasicComboView::getBasicModel() const
{
    return basicModel;
}

void CuteBasicComboView::setBasicModel(CuteBasicComboModel *model)
{
    // 删除原来的
    if (basicModel) {
        basicModel->deleteLater();
    }
    basicModel = model;
    setModel(basicModel);
    if (basicModel) {
        connect(basicModel, &CuteBasicComboModel::modelReset, this, &CuteBasicComboView::modelReseted);
    }
}

int CuteBasicComboView::getContentsHeight() const
{
    return contentsSize().height() +
           contentsMargins().top() +
           contentsMargins().bottom() + 2;
}

void CuteBasicComboView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    // 表示选中当前行，类似点击
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
    {
        QModelIndex index = currentIndex();
        if (index.isValid()) {
            emit rowClicked(index.row());
            emit currentRowChanged(index.row());
        }
    }
    break;
    default:
        break;
    }
    QListView::keyReleaseEvent(event);
}

void CuteBasicComboView::initModel()
{
    basicModel = new CuteBasicComboModel(this);
    setModel(basicModel);
    // 重置了 model data
    connect(basicModel, &CuteBasicComboModel::modelReset, this, &CuteBasicComboView::modelReseted);
}

void CuteBasicComboView::initView()
{
    // 无数据时大小
    setMinimumSize(1, 1);
    // 单选中
    setSelectionMode(QAbstractItemView::SingleSelection);
    // 取消编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    connect(this, &CuteBasicComboView::clicked, this, [this](const QModelIndex &index) {
        if (index.isValid()) {
            emit rowClicked(index.row());
            emit currentRowChanged(index.row());
        }
    });
}
