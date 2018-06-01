#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif

#include "ReplaceDlg.h"
#include<QPushButton>
#include<QCheckBox>
#include<QLabel>
#include<QLineEdit>
#include<QVBoxLayout>
#include<QGridLayout>

#include<QDebug>

//=======================================================================
// 设置查找按钮是否在激活状态
void ReplaceDlg::setFindEnable(QString str)
{
    qDebug()<<str<<"void ReplaceDlg::setFindEnable(QString str)";
    mCheck->setEnabled (!str.isEmpty ());
}


//======================================================================
// 设置替换相关的按钮是否在激活状态
void ReplaceDlg::setReplaceEnable(QString str)
{
    mReplaceBtn->setEnabled (!str.isEmpty ());
    mReplaceBtnAll->setEnabled (!str.isEmpty ());
}


//======================================================================
// 查找按钮的处理
void ReplaceDlg::onFindClick()
{
    Qt::CaseSensitivity cs =
            mUpcase->isChecked ()?
                Qt::CaseSensitive:
                Qt::CaseInsensitive;  // 如过大小写的按钮是开着的就大小写敏感

    emit findNext (mEdit->text (),cs);
}


//=======================================================================
// 替换
void ReplaceDlg::onReplaceClick()
{
    Qt::CaseSensitivity cs =
            mUpcase->isChecked ()?Qt::CaseSensitive:
                                  Qt::CaseInsensitive;  // 如过大小写的按钮是开着的就大小写敏感

    emit replaceOne (mEdit->text (),mReplaceEdit->text(),cs);
}


//================================================================================
// 全部替换被按下
void ReplaceDlg::onReplaceAllClick()
{
    Qt::CaseSensitivity cs =
            mUpcase->isChecked ()?
                Qt::CaseSensitive:
                Qt::CaseInsensitive;  // 如过大小写的按钮是开着的就大小写敏感

    emit replaceAll (mEdit->text (),mReplaceEdit->text(),cs);
}


//=================================================================================
// 取消操作
void ReplaceDlg::onCancelClick()
{
    hide();
}


// ===============================================================================
//构造一个对话框
ReplaceDlg::ReplaceDlg(QWidget *parent)
    :QDialog(parent,Qt::WindowCloseButtonHint)
{
    setWindowFlags (Qt::WindowCloseButtonHint);     //设置窗口的样式
    setWindowTitle ("替换");                        //窗口的标题
    initial();                                      //初始化窗口  布局int.
    setConnect ();                                  //相关的处理函数

    mCheck->setEnabled (false);                     //设置查找按钮是不可用的
    mReplaceBtn->setEnabled (false);                //设置替换按钮是不可用的
    mReplaceBtnAll->setEnabled (false);             //设置全部替换是不可用的
}


//==================================================================================
// 析构
ReplaceDlg::~ReplaceDlg()
{

    // .....................................

}

//=================================================================================
//==布局 申请空间
bool ReplaceDlg::initial()
{
    bool ret = true;
    mLabel=new QLabel("查找内容",this);
    mEdit = new QLineEdit(this);
    if(mEdit)
    {
        mEdit->setPlaceholderText ("我想要查找");
    }
    mCheck = new QPushButton("查找下一个",this);


    mReplaceL = new QLabel("替换为 : ",this);
    mReplaceEdit = new QLineEdit(this);
    if(mReplaceEdit)
    {
        mReplaceEdit->setPlaceholderText ("替换为");
    }

    mReplaceBtn = new QPushButton("替换",this);

    mUpcase = new QCheckBox("大小写",this);
    mReplaceBtnAll = new QPushButton("全部替换",this);

    mCancel = new QPushButton("取消",this);

    auto gridLayout = new QGridLayout(this);

    gridLayout->addWidget (mLabel,0,0,1,1);
    gridLayout->addWidget (mEdit,0,1,1,3);
    gridLayout->addWidget (mCheck,0,4,1,1);

    gridLayout->addWidget (mReplaceL,1,0,1,1);
    gridLayout->addWidget (mReplaceEdit,1,1,1,3);
    gridLayout->addWidget (mReplaceBtn,1,4,1,1);

    gridLayout->addWidget (mUpcase,2,0,1,1);
    gridLayout->addWidget (mReplaceBtnAll,2,4,1,1);

    gridLayout->addWidget (mCancel,3,4,1,1);

    return ret;
}


//==============================================
// 相关的处理函数
void ReplaceDlg::setConnect()
{
    connect(mEdit,SIGNAL(textChanged(QString)),this,SLOT(setFindEnable(QString)));
    connect(mReplaceEdit,SIGNAL(textChanged(QString)),this,SLOT(setReplaceEnable(QString)));
    connect(mCheck,SIGNAL(clicked(bool)),this,SLOT(onFindClick()));
    connect(mReplaceBtn,SIGNAL(clicked(bool)),this,SLOT(onReplaceClick()));
    connect(mReplaceBtnAll,SIGNAL(clicked(bool)),this,SLOT(onReplaceAllClick()));
    connect(mCancel,SIGNAL(clicked(bool)),this,SLOT(onCancelClick()));
}



