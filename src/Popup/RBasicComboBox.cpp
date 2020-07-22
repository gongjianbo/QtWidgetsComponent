#include "RBasicComboBox.h"

#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QMouseEvent>

#include <QDebug>

RBasicComboBox::RBasicComboBox(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground); //支持样式表
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    initComponent();
    initStyleSheet();
}

RBasicComboBox::~RBasicComboBox()
{
    boxPop->hidePopup();
}

int RBasicComboBox::getCurrentIndex() const
{
    if(boxPop->getContainer()){
        return boxPop->getContainer()->getCurrentIndex();
    }
    return -1;
}

void RBasicComboBox::setCurrentIndex(int index)
{
    if(boxPop->getContainer()){
        boxPop->getContainer()->setCurrentIndex(index);
    }
}

QString RBasicComboBox::getCurrentText() const
{
    return boxEdit->text();
}

void RBasicComboBox::setCurrentText(const QString text)
{
    editTimer->stop();
    boxEdit->setText(text);
}

QList<QString> RBasicComboBox::getItems() const
{
    if(boxPop->getContainer()){
        return boxPop->getContainer()->getItems();
    }
    return QList<QString>();
}

void RBasicComboBox::setItems(const QList<QString> &items)
{
    if(boxPop->getContainer()){
        boxPop->getContainer()->setItems(items);
    }
}

RBasicComboPopup *RBasicComboBox::getPopup() const
{
    return boxPop;
}

bool RBasicComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==boxEdit){
        //过滤编辑框事件
        switch (event->type()) {
        case QEvent::KeyRelease:
        {
            if(boxPop->getContainer()){
                //这里只考虑了edit可编辑的情况
                QKeyEvent *key_event=static_cast<QKeyEvent*>(event);
                if(key_event->key()==Qt::Key_Up){
                    setCurrentText(boxPop->getContainer()->getPrevText());
                }else if(key_event->key()==Qt::Key_Down){
                    setCurrentText(boxPop->getContainer()->getNextText());
                }
            }
        }
            break;
        case QEvent::FocusAboutToChange:
        case QEvent::FocusIn:
        {
            //获得焦点时全选
            QFocusEvent *focus_event=static_cast<QFocusEvent*>(event);
            if(focus_event->gotFocus()){
                QTimer::singleShot(20,boxEdit,&QLineEdit::selectAll);
            }
        }
            break;
        default:
            break;
        }
    }else if(watched==boxButton){
        //过频按钮事件
    }else if(watched==boxPop){
        //过滤弹框事件
        if(event->type()==QEvent::Show){
            boxButton->setChecked(true);
        }else if(event->type()==QEvent::Hide){
            boxButton->setChecked(false);
        }
    }
    return false;
}

void RBasicComboBox::initComponent()
{
    //按钮设置
    boxButton->setObjectName("button");
    boxButton->setFixedWidth(20);
    boxButton->setCheckable(true); //用选中来表示弹框弹出
    boxButton->setFocusPolicy(Qt::NoFocus);
    boxButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    boxButton->installEventFilter(this);

    //编辑框设置
    boxEdit->setObjectName("edit");
    boxEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    boxEdit->installEventFilter(this);
    //index和text两个属性可以看成独立的
    connect(boxEdit,&QLineEdit::textChanged,this,&RBasicComboBox::currentTextChanged);
    //编辑时，延迟一会儿进行查询-currentindex
    connect(boxEdit,&QLineEdit::textEdited,[this]{
        editTimer->start(300);
    });
    //编辑结束，进行查询-currentindex
    connect(boxEdit,&QLineEdit::editingFinished,this,&RBasicComboBox::checkTextRow);
    editTimer->setSingleShot(true);
    connect(editTimer,&QTimer::timeout,this,&RBasicComboBox::checkTextRow);

    //弹框容器设置
    boxPop->attachTarget(this);
    boxPop->installEventFilter(this);
    initContainer();
    connect(boxPop,&RBasicComboPopup::containerChanged,this,&RBasicComboBox::initContainer);

    //布局
    boxLayout->setMargin(1);
    boxLayout->setSpacing(1);
    boxLayout->addWidget(boxEdit);
    boxLayout->addWidget(boxButton);
    //点击按钮，弹出
    //这里有个问题，已经弹出时，点击别的地方会隐藏，
    //然后我事件过滤又把按钮设置成了非选中，所以点击按钮只会触发弹出
    connect(boxButton,&QPushButton::toggled,[this](bool checked){
        if(checked){
            boxPop->showPopup();
        }else{
            boxPop->hidePopup();
        }
    });
}

void RBasicComboBox::initContainer()
{
    if(!boxPop->getContainer())
        return;

    connect(boxPop->getContainer(),&RBasicComboContainer::currentIndexChanged,this,&RBasicComboBox::currentIndexChanged);
    connect(boxPop->getContainer(),&RBasicComboContainer::updateData,[this]{
        setCurrentText(boxPop->getContainer()->getCurrentText());
    });
}

void RBasicComboBox::initStyleSheet()
{
    setStyleSheet(R"(
                  RBasicComboBox{
                  color:black;
                  background: rgb(160,160,160);
                  }
                  RBasicComboBox:disable{
                  color:rgb(160,160,160);
                  background: rgb(160,160,160);
                  }
                  RBasicComboBox:hover{
                  background: rgb(0,120,215);
                  }
                  RBasicComboBox #edit{
                  border:0;
                  margin:0;
                  padding:0px 3px;
                  color:black;
                  background:white;
                  }
                  RBasicComboBox #button{
                  border:5px solid white;
                  padding:0;
                  margin:0;
                  min-width:12px;
                  max-width:12px;
                  background-color: rgb(200,200,200);
                  }
                  RBasicComboBox #button:hover,
                  RBasicComboBox #button:checked{
                  background-color: rgb(180,215,243);
                  }
                  RBasicComboContainer{
                  background: transparent;
                  border:1px solid rgb(0,120,215);
                  }
                  RBasicComboView{
                  background:white;
                  border:0;
                  font:13px "宋体";
                  }
                  RBasicComboView::item{
                  height:20px;
                  color:black;
                  background:white;
                  }
                  RBasicComboView::item:selected,
                  RBasicComboView::item:hover{
                  color:white;
                  background:rgb(0,120,215);
                  }
                  )");
}

void RBasicComboBox::checkTextRow()
{
    //如果model中有匹配的文本，就修改view的currentIndex
    if(boxPop->getContainer()){
        if(boxPop->getContainer()->checkTextRow(boxEdit->text())>=0){
            setCurrentText(boxPop->getContainer()->getCurrentText());
        }
    }
}
