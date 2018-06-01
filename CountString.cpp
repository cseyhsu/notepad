#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#include "CountString.h"
#include<QDebug>


//========================================================================
// 构造 初始化线程
CountString::CountString(QObject * parent)
    : QThread(parent)
{
}


//====================================================================
//=
CountString::~CountString()
{

}


//=====================================================================
// 计算最大行数
int CountString::getMaxLine()
{
    return maxLine;
}

//===============================================================================
// 获取当前的 mText
std::shared_ptr<QPlainTextEdit> CountString::getText()
{
    qDebug()<<"std::shared_ptr<QPlainTextEdit> CountString::getText() tid "<< currentThreadId ();

    return mText;
}


//==============================================================
// 重新设置  mText
void CountString::setText(std::shared_ptr<QPlainTextEdit> text)
{
    qDebug()<<"void CountString::setText(std::shared_ptr<QPlainTextEdit> text)  tid "<<currentThreadId ();
    qDebug()<<text.get ();
    mText = text;

    // 链接处理函数
    connect(mText.get (),SIGNAL(textChanged()),this,SLOT(countString()));
    connect(mText.get (),SIGNAL(cursorPositionChanged()),this,SLOT(onTextCursorChange()));
    connect (mText.get (),SIGNAL(textChanged()),this,SLOT(onLineCount()));

}


//  =========================================================================
//== 计算字符数量 发送字符数量的信号
void CountString::countString()  // 发射字数的信号
{
    qDebug()<<" void CountString::countString() tid "<< currentThreadId ();
    if(mText)
    {
        qDebug()<<" emit   void wordCount(int i);  tid "<<currentThreadId ();
        qDebug()<<mText->toPlainText ();
        emit wordCount (mText->toPlainText ().count ());                      // 发送字符数量的信号
    }
}


//=========================================================================
// 计算光标的坐在的位置。。并且发送光标所在位置的下标
void CountString::onTextCursorChange()
{
    qDebug()<<"void CountString::onTextCursorChange() tid "<<currentThreadId ();
    int ln =  1;
    int col  = 0 ;
    QString str = mText->toPlainText ();
    auto pos = mText->textCursor ().position ();
    int flag = 0 ;
    for( int i = 0 ; i< pos ; ++i)
    {
        // 如果 单个字符等于 回车 行数加 1
        if(str[i] == '\n')
        {
            ++col;
            flag = i ;
        }
    }
    ln = pos - flag;
    //  qDebug()<<" goto to tid "<<currentThreadId ();

    col ++;                             // 为了符合使用习惯
    if(ln == 0)
    {
        ln++;                               //仅仅是为了符合使用习惯
    }
    emit cursorIn (col,ln);
}


//==========================================================================
// 计算行数
void CountString::onLineCount()
{
    QString str =mText ->toPlainText ();
    int col = 1;
    for( int i = 0 ; i< str.count (); ++i)
    {
        if(str[i] == '\n')
        {
            ++ col;
        }
    }
    qDebug()<<"计算文本内容的行数  "<<col;
    maxLine = col;
    emit lineCount (col);               //发送最大行数的信号
}


// ==================================================
// 启动一个线程
void CountString::run()
{
    qDebug()<<" run tid "<<currentThreadId ();
    exec();
}
