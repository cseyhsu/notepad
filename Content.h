#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef _CONTENT_H
#define _CONTENT_H

#include <QMainWindow>
#include<memory>
class QPlainTextEdit;
class Content :public QMainWindow
{
public:
    Content(QWidget *parent  = nullptr);                //构造
    virtual ~Content();                                 //析构
  std::shared_ptr<QPlainTextEdit> getEdit ();           //获取mText
  QString getFilePath();                                //获取文件路径
  void setFilePath(QString path);                       //设置文件路径

private:
       std::shared_ptr<QPlainTextEdit> mText;           //智能成员
       QString  mFilePath="";                           //文件路径默认是 empty
};

#endif // _CONTENT_H
