#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#ifndef ABOUTNOTEPAD_H
#define ABOUTNOTEPAD_H

#include<QDialog>



class QLabel ;
class QPlainTextEdit;
class QPushButton ;

class AboutNotePad : public QDialog
{
public:
    AboutNotePad(QWidget * parent = nullptr);
    ~AboutNotePad();
protected:

    void initial();                         //初始化
private:
    QLabel * mLogo;                         //显示logo图片的标签
    QPlainTextEdit *mText;                  //显示文本内容的文本编辑器
    QPushButton * mClose;                   //关闭对话框的按钮

};

#endif // ABOUTNOTEPAD_H
