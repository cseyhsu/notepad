#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#include "Content.h"
#include<QPlainTextEdit>
#include<QDebug>
#include<QPalette>

//==============================================================================
// Content的构造
Content::Content(QWidget * parent)
    :QMainWindow(parent)
{
    mText =  std::make_shared<QPlainTextEdit>(new QPlainTextEdit (this));   //
    mText->setAcceptDrops (false);              //不允许拖动文件
    setCentralWidget (mText.get ());            //设置mText为主的widget

    auto palette = mText.get ()->palette ();    //获取调色板

    palette.setColor (QPalette::Active,
                      QPalette::Highlight,
                      Qt::blue);                //设置调色板的颜色

    palette.setColor (QPalette::Inactive,
                      QPalette::HighlightedText,
                      Qt::white);              //*********叫我星星

    palette.setColor (QPalette::Inactive,
                      QPalette::Highlight,
                      Qt::blue);                //*******x

    mText.get ()->setPalette (palette);         //将调色板设置到mText
}


//============================================================================
//析构了你
Content::~Content()
{
    qDebug()<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
}


//========================================================================
// 获取一个 mText
std::shared_ptr<QPlainTextEdit > Content::getEdit()
{
    return mText;
}


//=======================================================================
// 获取当前的文件路径
QString Content::getFilePath()
{
    return mFilePath;
}


//====================================================================
// 设置文件路径
void Content::setFilePath(QString path)
{
    mFilePath = path;
}


