#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif

#include "Finddlg.h"
#include<QLabel>
#include<QPushButton>
#include<QLineEdit>
#include<QGroupBox>
#include<QGridLayout>
#include<QCheckBox>
#include<QRadioButton>
#include<QHBoxLayout>   // 水平布局
#include<QVBoxLayout>  // 垂直布局


//================================================================================
Finddlg::Finddlg(QWidget *parent)    // 设置窗口的类型是 有关闭按钮和最小化按钮的类型
    : QDialog(parent,
              Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint
              )
{
    setWindowTitle ("查找");                                   // 设置窗口的标题
    initial();                                                 // 初始化组件
    mCheck->setEnabled (false);                               //一开始的时候将查找的按钮设置为不可行的状态
    setConnect ();                                            // 链接处理函数
}

//===============================================================================
//==
Finddlg::~Finddlg()
{

}


//===============================================================================
//对话框关闭的处理函数
void Finddlg::closeEvent(QCloseEvent *event)
{
    hide();
}


//==============================================================================
// 设置按钮是否在激活状态
void Finddlg::setFindEnable(QString str)
{

    mCheck->setEnabled (!str.isEmpty ());
}


//===========================================================================
// 查找的处理函数
void Finddlg::onClickCheck()
{
    // 用户点击查找的响应函数
    Qt::CaseSensitivity cs =
            mUpcase->isChecked ()?
                Qt::CaseSensitive:
                Qt::CaseInsensitive;  // 如过大小写的按钮是开着的就大小写敏感

    if(mrboxFor->isChecked ()){
        emit findFor(mEdit->text (),cs);    //向前查找
    }
    else
    {

        emit findBack (mEdit->text (),cs);//向后查找
    }
}


//==============================================================================
//===取消按钮被按下
void Finddlg::onClickCancel()
{
    //
    hide();
}


//==========================================================================
// 初始化界面布局
bool Finddlg::initial()
{
    bool ret = true;
    mLabel  = new QLabel("查找内容",this);            // 初始化 查找文本内容的标签
    mEdit = new QLineEdit(this);                     //初始化 lineedit

    mCheck = new QPushButton("查找",this);            //初始化查找的按钮
    mUpcase = new QCheckBox("大小写",this);            // 初始化 是否开启大小的按钮
    mgroupBox = new QGroupBox("查找方向",this);        // 初始化出啊找方向
    mrboxFor = new QRadioButton("向前",mgroupBox);     //初始化 向前查找
    mrboxBack = new QRadioButton("向后",mgroupBox);    // 初始化向后查找
    mrboxBack->setChecked (true);                     //将向后查找设置为默认的查找方式
    mCancel = new QPushButton  ("取消",this);          // 初始化取消按钮
    QVBoxLayout *hLayout = new QVBoxLayout;           // 初始化一个 垂直布局管理器
    QHBoxLayout *hmLayout = new QHBoxLayout(mgroupBox); // 初始化一个水平的管理器
    hmLayout->addWidget (mrboxFor);                     // 将向前查找的选项加入 水平的管理器
    hmLayout->addWidget (mrboxBack);                  // 将向后查找的选项加入水平的布局guan管理器
    hLayout->addWidget (mgroupBox);                  // 将mgroupBox 加入垂直管理器
    QGridLayout * layout = new QGridLayout(this);    // 初始化一个水平的二维网格布局管理器
    layout->addWidget (mLabel,0,0,1,1);            // 将显示查找文本内容的label 加入 二维的网格布局管理器
    // 左上角的坐标是 0， 0 ， 占 一行 占一列
    layout->addWidget (mEdit,0,1,1,2);             // 将编辑框加入二维网格的布局管理器
    //左上角的坐标是 （ 0， 1） 表示 从 第0 行 第一列
    // 占空间是 一行  两列
    layout->addWidget (mCheck,0,3,1,1);           // 将查找的按钮加入二位布局管理器
    // 坐标从 0 行 第三列开始 占 一行一列

    layout->addWidget (mUpcase,2,0,1,1);
    layout->addLayout (hLayout,1,1,2,2);
    layout->addWidget (mCancel,1,3,1,1);

    return ret;
}


//========================================================
//关联处理函数
void Finddlg::setConnect()
{
    connect(mEdit,SIGNAL(textChanged(QString)),this,SLOT(setFindEnable(QString )));
    connect(mCheck,SIGNAL(clicked(bool)),this,SLOT(onClickCheck()),Qt::DirectConnection);
    connect(mCancel,SIGNAL(clicked(bool)),this,SLOT(onClickCancel()),Qt::DirectConnection);
}
