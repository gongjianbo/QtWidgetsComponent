#include "RBasicComboView.h"

#include <QKeyEvent>
#include <QDebug>

RBasicComboView::RBasicComboView(QWidget *parent)
    : QListView(parent)
{
    setFocusPolicy(Qt::StrongFocus); //key press
    setMouseTracking(true); //hover

    initModel();
    //initDelegate();
    initView();
}

int RBasicComboView::getCurrentRow() const
{
    //获取当前行
    const QModelIndex current=currentIndex();
    if(current.isValid()){
        return current.row();
    }
    return -1;
}

void RBasicComboView::setCurrentRow(int row)
{
    //设置当前行
    if(row==getCurrentRow())
        return;
    if(row>=0&&row<basicModel->rowCount()){
        setCurrentIndex(basicModel->index(row,0));
        emit currentRowChanged(row);
    }
}

int RBasicComboView::checkTextRow(const QString &text)
{
    //根据内容设置当前行
    const int row=basicModel->getDataRow(text);
    if(row>=0){
        setCurrentRow(row);
        return row;
    }
    return -1;
}

QString RBasicComboView::getCurrentText()
{
    QModelIndex current=currentIndex();
    //如果当前行无效就去设置一个默认行0
    if(!current.isValid()){
        if(basicModel->rowCount()>0){
            current=basicModel->index(0,0);
            //setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current,Qt::DisplayRole).toString();
        }
        return QString();
    }
    return basicModel->data(current,Qt::DisplayRole).toString();
}

QString RBasicComboView::getNextText()
{
    QModelIndex current=currentIndex();
    if(current.isValid()){
        if(current.row()<basicModel->rowCount()-1){
            //设置到下一行
            current=basicModel->index(current.row()+1,0);
            //setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current,Qt::DisplayRole).toString();
        }
    }
    return getCurrentText();
}

QString RBasicComboView::getPrevText()
{
    QModelIndex current=currentIndex();
    if(current.isValid()){
        if(current.row()>0){
            //设置到上一行
            current=basicModel->index(current.row()-1,0);
            //setCurrentIndex(current);
            setCurrentRow(current.row());
            return basicModel->data(current,Qt::DisplayRole).toString();
        }
    }
    return getCurrentText();
}

RBasicComboModel *RBasicComboView::getBasicModel() const
{
    return basicModel;
}

void RBasicComboView::setBasicModel(RBasicComboModel *model)
{
    //删除原来的
    if(basicModel){
        basicModel->deleteLater();
    }
    basicModel=model;
    setModel(basicModel);
    if(basicModel){
        connect(basicModel,&RBasicComboModel::modelReset,this,&RBasicComboView::modelReseted);
    }
}

int RBasicComboView::getContentsHeight() const
{
    return contentsSize().height()+
            contentsMargins().top()+
            contentsMargins().bottom();
}

void RBasicComboView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    //表示选中当前行，类似点击
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
    {
        QModelIndex index=currentIndex();
        if(index.isValid()){
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

void RBasicComboView::initModel()
{
    basicModel=new RBasicComboModel(this);
    setModel(basicModel);
    //重置了model data
    connect(basicModel,&RBasicComboModel::modelReset,this,&RBasicComboView::modelReseted);
}

void RBasicComboView::initView()
{
    //无数据时大小
    setMinimumSize(1,1);
    //单选中
    setSelectionMode(QAbstractItemView::SingleSelection);
    //取消编辑
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //
    connect(this,&RBasicComboView::clicked,this,[this](const QModelIndex &index){
        if(index.isValid()){
            emit rowClicked(index.row());
            emit currentRowChanged(index.row());
        }
    });
}
