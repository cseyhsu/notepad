#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef COUNTSTRING_H
#define COUNTSTRING_H

#include<QThread>
#include<memory>
#include<QPlainTextEdit>
class CountString :public QThread
{
    Q_OBJECT
signals:
    void wordCount(int i);                                    //发送字符数量的信号
    void cursorIn( int col , int cow);                        // 发送光标的位置的信号
    void lineCount(int cou);                                  //发送最大的行数

public:
    CountString(QObject * parent = nullptr);
    ~CountString();
    int getMaxLine();                                       //获取最大的行数

    std::shared_ptr<QPlainTextEdit> getText();              //获取Text
    void setText( std::shared_ptr<QPlainTextEdit> text);   // 设置 mText
protected slots:
    void countString ();                                    // 当文本发生生变化的时候  就调用
    void onTextCursorChange();                              // 当光标的位置发生变化
    void onLineCount();                                     //文本变化是计算最大行数的处理函数

protected:
    void run();                                             //重写父类的run
private:

    std::shared_ptr<QPlainTextEdit> mText;                  //统计MText
    int col ;                                               //
    int cow;

    int  maxLine = 0;                                       //最大行数




};

#endif // COUNTSTRING_H
