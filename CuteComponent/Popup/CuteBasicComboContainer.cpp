#include "CuteBasicComboContainer.h"
#include "CuteBasicComboView.h"
#include "CuteBasicComboModel.h"
#include <QVBoxLayout>

CuteBasicComboContainer::CuteBasicComboContainer(bool init, QWidget *parent)
    : QWidget{parent}
{
    if (init) {
        initDefault();
    }
}

int CuteBasicComboContainer::getCurrentIndex() const
{
    if (defaultView) {
        return defaultView->getCurrentRow();
    }
    return -1;
}

void CuteBasicComboContainer::setCurrentIndex(int index)
{
    if (defaultView) {
        defaultView->setCurrentRow(index);
    }
}

QList<QString> CuteBasicComboContainer::getItems() const
{
    if (defaultView && defaultView->getBasicModel()) {
        return defaultView->getBasicModel()->getModelData();
    }
    return QList<QString>();
}

void CuteBasicComboContainer::setItems(const QList<QString> &items)
{
    if (defaultView && defaultView->getBasicModel()) {
        defaultView->getBasicModel()->setModelData(items);
    }
}

int CuteBasicComboContainer::checkTextRow(const QString &text)
{
    if (defaultView) {
        return defaultView->checkTextRow(text);
    }
    return -1;
}

QString CuteBasicComboContainer::getCurrentText() const
{
    if (defaultView) {
        return defaultView->getCurrentText();
    }
    return QString();
}

QString CuteBasicComboContainer::getPrevText()
{
    if (defaultView) {
        return defaultView->getPrevText();
    }
    return QString();
}

QString CuteBasicComboContainer::getNextText()
{
    if (defaultView) {
        return defaultView->getNextText();
    }
    return QString();
}

int CuteBasicComboContainer::getContentsHeight() const
{
    if (defaultView) {
        return defaultView->getContentsHeight();
    }
    return 0;
}

void CuteBasicComboContainer::initDefault()
{
    defaultView = new CuteBasicComboView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(defaultView);
    //
    connect(defaultView, &CuteBasicComboView::currentRowChanged, this, &CuteBasicComboContainer::currentIndexChanged);
    connect(defaultView, &CuteBasicComboView::rowClicked, this, &CuteBasicComboContainer::updateData);
    connect(defaultView, &CuteBasicComboView::modelReseted, this, &CuteBasicComboContainer::updateData);
}

