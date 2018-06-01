#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#include "AboutNotePad.h"
#include<QLabel>
#include<QPushButton>
#include<QPlainTextEdit>
#include<QFrame>
#include<QImage>
#include<QPixmap>
#include<QPalette>
#include<QDebug>

//==========================================================================
// 构造一个AboutNotePad对话框
AboutNotePad::AboutNotePad(QWidget * parent )
    : QDialog(parent,Qt::WindowCloseButtonHint)
{
    initial();                                      //初始化关于对话框
    setFixedSize (500,530);                         //固定对话框的大小
    setWindowTitle ("关于notePad--");               //设置标题
}


//=======================================================================
AboutNotePad::~AboutNotePad()
{

}

//========================================================================
// 初始化
void AboutNotePad::initial()
{
    QString str ="开发者: xingcent\n当前版本: 1.0\n";      //str说明
    mLogo = new QLabel(this);                             //在堆上弄一个Label
    mText = new QPlainTextEdit(this);                     //在堆上弄一个plainTextEidt
    mText->setFrameStyle (QFrame::NoFrame);               //去除边框
    mClose = new QPushButton("关闭",this);                //关闭对话框按钮

    QImage img (":img/logo.png");                         // 获取img
    QPixmap map  = QPixmap::fromImage (img);              //从img中获取pixmap
    map.scaled (340,340,Qt::IgnoreAspectRatio);            // scale图像
    mLogo->setPixmap (map);                               // 设置图像 NotePadmm's logo
    mLogo->setGeometry (80,20,340,340);                   // 设置logo所在的位置以及大小
    mText->setPlainText (str);                            //设置文本的内容
    mText->setReadOnly (true);                            //设置为只读的
    mText->setGeometry (80,360,320,60);                   //设置文本框的大小和位置
    mClose->setGeometry (420,440,70,40);                  //设置关闭按钮的大小和位置
    auto pa =mText-> palette ();                          //获取一个调色板的对象

    pa.setColor(QPalette::Active,
                QPalette::Base,
                palette().color(QPalette::Active,
                                QPalette::Background)
                );                                       //设置调色板的颜色
    mText->setPalette (pa);                               //给文本编辑框设置调色板

    connect(mClose,SIGNAL(clicked(bool)),this,SLOT(close())); // 链接关闭处理函数



}


