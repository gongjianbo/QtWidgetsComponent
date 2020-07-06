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
    container->hidePopup();
}

int RBasicComboBox::getCurrentIndex() const
{
    return container->getBasicView()->getCurrentRow();
}

void RBasicComboBox::setCurrentIndex(int index)
{
    container->getBasicView()->setCurrentRow(index);
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
    //没有判断指针有效性
    return container->getBasicView()->getBasicModel()->getModelData();
}

void RBasicComboBox::setItems(const QList<QString> &items)
{
    container->getBasicView()->getBasicModel()->setModelData(items);
}

bool RBasicComboBox::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==boxEdit){
        //过滤编辑框事件
        switch (event->type()) {
        case QEvent::KeyRelease:
        {
            //这里只考虑了edit可编辑的情况
            QKeyEvent *key_event=static_cast<QKeyEvent*>(event);
            if(key_event->key()==Qt::Key_Up){
                setCurrentText(container->getBasicView()->getPrevText());
            }else if(key_event->key()==Qt::Key_Down){
                setCurrentText(container->getBasicView()->getNextText());
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
    }else if(watched==container){
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
    container->attachTarget(this);
    container->installEventFilter(this);
    connect(container->getBasicView(),&RBasicComboView::currentRowChanged,this,&RBasicComboBox::currentIndexChanged);
    connect(container->getBasicView(),&RBasicComboView::rowClicked,[this]{
        setCurrentText(container->getBasicView()->getCurrentText());
    });
    connect(container->getBasicView(),&RBasicComboView::modelReseted,[this]{
        setCurrentText(container->getBasicView()->getCurrentText());
    });
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
            container->showPopup();
        }else{
            container->hidePopup();
        }
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
                  RBasicComboBox #wrapper{
                  background: transparent;
                  border:1px solid rgb(0,120,215);
                  }
                  RBasicComboBox #view{
                  background:white;
                  border:0;
                  font:13px "宋体";
                  }
                  RBasicComboBox #view::item{
                  height:20px;
                  color:black;
                  background:white;
                  }
                  RBasicComboBox #view::item:selected,
                  RBasicComboBox #view::item:hover{
                  color:white;
                  background:rgb(0,120,215);
                  }
                  )");
}

void RBasicComboBox::checkTextRow()
{
    //如果model中有匹配的文本，就修改view的currentIndex
    if(container->getBasicView()->checkTextRow(boxEdit->text())>=0){
        setCurrentText(container->getBasicView()->getCurrentText());
    }
}
