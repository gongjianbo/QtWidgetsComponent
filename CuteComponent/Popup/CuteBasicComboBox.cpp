#include "CuteBasicComboBox.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QDebug>

CuteBasicComboBox::CuteBasicComboBox(QWidget *parent)
    : QWidget{parent}
    , boxEdit{new QLineEdit(this)}
    , boxDown{new QPushButton(this)}
    , boxLayout{new QHBoxLayout(this)}
    , boxPop{new CuteBasicComboPopup(this)}
    , editTimer{new QTimer(this)}
{
    // 支持样式表
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    initComponent();
}

CuteBasicComboBox::~CuteBasicComboBox()
{
    boxPop->hidePopup();
}

int CuteBasicComboBox::getBorderWidth() const
{
    return borderWidth;
}

void CuteBasicComboBox::setBorderWidth(int px)
{
    if (borderWidth == px)
        return;
    borderWidth = px;
    boxLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
    emit borderWidthChanged(borderWidth);
}

bool CuteBasicComboBox::getPopupVisible() const
{
    return popupVisible;
}

void CuteBasicComboBox::setPopupVisible(bool visible)
{
    // qDebug() << __FUNCTION__ << popupVisible << visible;
    if (popupVisible == visible)
        return;
    popupVisible = visible;
    emit popupVisibleChanged(popupVisible);
}

int CuteBasicComboBox::getCurrentIndex() const
{
    if (boxPop->getContainer()) {
        return boxPop->getContainer()->getCurrentIndex();
    }
    return -1;
}

void CuteBasicComboBox::setCurrentIndex(int index)
{
    if (boxPop->getContainer()) {
        boxPop->getContainer()->setCurrentIndex(index);
    }
}

QString CuteBasicComboBox::getCurrentText() const
{
    return boxEdit->text();
}

void CuteBasicComboBox::setCurrentText(const QString &text)
{
    editTimer->stop();
    boxEdit->setText(text);
}

QList<QString> CuteBasicComboBox::getItems() const
{
    if (boxPop->getContainer()) {
        return boxPop->getContainer()->getItems();
    }
    return QList<QString>();
}

void CuteBasicComboBox::setItems(const QList<QString> &items)
{
    if (boxPop->getContainer()) {
        boxPop->getContainer()->setItems(items);
    }
}

CuteBasicComboPopup *CuteBasicComboBox::getPopup() const
{
    return boxPop;
}

bool CuteBasicComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == boxEdit) {
        // 过滤编辑框事件
        switch (event->type()) {
        case QEvent::KeyRelease:
        {
            if (boxPop->getContainer()) {
                // 这里只考虑了edit可编辑的情况
                QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
                if (key_event->key() == Qt::Key_Up) {
                    setCurrentText(boxPop->getContainer()->getPrevText());
                } else if(key_event->key() == Qt::Key_Down) {
                    setCurrentText(boxPop->getContainer()->getNextText());
                }
            }
        }
        break;
        case QEvent::FocusAboutToChange:
        case QEvent::FocusIn:
        {
            // 获得焦点时全选
            QFocusEvent *focus_event = static_cast<QFocusEvent*>(event);
            if (focus_event->gotFocus()) {
                QTimer::singleShot(20, boxEdit, &QLineEdit::selectAll);
            }
        }
        break;
        default:
            break;
        }
    } else if(watched == boxDown) {
        // 过滤按钮事件
    } else if(watched == boxPop) {
        // 过滤弹框事件
    }
    return false;
}

void CuteBasicComboBox::initComponent()
{
    // 按钮设置
    boxDown->setObjectName("down");
    boxDown->setFocusPolicy(Qt::NoFocus);
    boxDown->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    boxDown->installEventFilter(this);

    // 编辑框设置
    boxEdit->setObjectName("edit");
    boxEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    boxEdit->installEventFilter(this);
    // index 和 text 两个属性可以看成独立的
    connect(boxEdit, &QLineEdit::textChanged, this, &CuteBasicComboBox::currentTextChanged);
    // 编辑时，延迟一会儿进行查询-currentindex
    connect(boxEdit, &QLineEdit::textEdited, [this] {
        editTimer->start(300);
    });
    // 编辑结束，进行查询-currentindex
    connect(boxEdit, &QLineEdit::editingFinished, this, &CuteBasicComboBox::checkTextRow);
    editTimer->setSingleShot(true);
    connect(editTimer, &QTimer::timeout, this, &CuteBasicComboBox::checkTextRow);

    // 弹框容器设置
    boxPop->attachTarget(this);
    boxPop->installEventFilter(this);
    initContainer();
    connect(boxPop, &CuteBasicComboPopup::containerChanged, this, &CuteBasicComboBox::initContainer);
    connect(boxPop, &CuteBasicComboPopup::visibleChanged, this, &CuteBasicComboBox::setPopupVisible);

    // 布局
    boxLayout->setContentsMargins(borderWidth, borderWidth, borderWidth, borderWidth);
    boxLayout->setSpacing(0);
    boxLayout->addWidget(boxEdit);
    boxLayout->addWidget(boxDown);
    // 点击按钮，弹出
    // 这里有个问题，切换焦点导致弹框自动关闭，再进入该逻辑则visible已经是false，于是又弹出了
    connect(boxDown, &QPushButton::pressed, [this]() {
        // qDebug() << "toggled" << getPopupVisible();
        if (getPopupVisible()) {
            boxPop->hidePopup();
        } else {
            boxPop->showPopup();
        }

    });
}

void CuteBasicComboBox::initContainer()
{
    if(!boxPop->getContainer())
        return;

    connect(boxPop->getContainer(), &CuteBasicComboContainer::currentIndexChanged, this, &CuteBasicComboBox::currentIndexChanged);
    connect(boxPop->getContainer(), &CuteBasicComboContainer::updateData, [this] {
        setCurrentText(boxPop->getContainer()->getCurrentText());
    });
}

void CuteBasicComboBox::checkTextRow()
{
    // 如果 model 中有匹配的文本，就修改 view 的 currentIndex
    if (boxPop->getContainer() && boxPop->getContainer()->checkTextRow(boxEdit->text()) >= 0){
        setCurrentText(boxPop->getContainer()->getCurrentText());
    }
}
