#include "RBasicComboContainer.h"

#include <QVBoxLayout>

RBasicComboContainer::RBasicComboContainer(bool init, QWidget *parent)
    : QWidget(parent)
{
    if(init){
        initDefault();
    }
}

int RBasicComboContainer::getCurrentIndex() const
{
    if(defaultView){
        return defaultView->getCurrentRow();
    }
    return -1;
}

void RBasicComboContainer::setCurrentIndex(int index)
{
    if(defaultView){
        defaultView->setCurrentRow(index);
    }
}

QList<QString> RBasicComboContainer::getItems() const
{
    if(defaultView&&defaultView->getBasicModel()){
        return defaultView->getBasicModel()->getModelData();
    }
    return QList<QString>();
}

void RBasicComboContainer::setItems(const QList<QString> &items)
{
    if(defaultView&&defaultView->getBasicModel()){
        defaultView->getBasicModel()->setModelData(items);
    }
}

int RBasicComboContainer::checkTextRow(const QString &text)
{
    if(defaultView){
        return defaultView->checkTextRow(text);
    }
    return -1;
}

QString RBasicComboContainer::getCurrentText() const
{
    if(defaultView){
        return defaultView->getCurrentText();
    }
    return QString();
}

QString RBasicComboContainer::getPrevText()
{
    if(defaultView){
        return defaultView->getPrevText();
    }
    return QString();
}

QString RBasicComboContainer::getNextText()
{
    if(defaultView){
        return defaultView->getNextText();
    }
    return QString();
}

int RBasicComboContainer::getContentsHeight() const
{
    if(defaultView){
        return defaultView->getContentsHeight();
    }
    return 0;
}

void RBasicComboContainer::initDefault()
{
    defaultView=new RBasicComboView(this);
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->setMargin(1);
    layout->addWidget(defaultView);
    //
    connect(defaultView,&RBasicComboView::currentRowChanged,this,&RBasicComboContainer::currentIndexChanged);
    connect(defaultView,&RBasicComboView::rowClicked,this,&RBasicComboContainer::updateData);
    connect(defaultView,&RBasicComboView::modelReseted,this,&RBasicComboContainer::updateData);
}

