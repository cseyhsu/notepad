#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef _FINDDLG_H
#define _FINDDLG_H

#include <QDialog>
#include<QCloseEvent>
#include<QEvent>
class QLabel;
class QPushButton ;
class QLineEdit;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QLineEdit;

class Finddlg : public QDialog
{
    Q_OBJECT

public:
    Finddlg(QWidget *parent = 0);
   virtual ~Finddlg();
protected:
    void closeEvent (QCloseEvent *event) override;               //重写closeEvent 将close事件设置未隐藏状态

protected slots:
    void setFindEnable(QString str);                            //设置查找的按钮是否为可用状态
    void onClickCheck();                                         //点击查找按钮
    void onClickCancel();                                        //点击取消的按钮
private :
    bool initial();                                              //初始化各种的组件和布局
    void setConnect();                                           //链接处理函数


signals:

    void findFor(QString text,Qt::CaseSensitivity cs);          //发送向前查找的信号
     void findBack(QString text,Qt::CaseSensitivity cs);       //发送向后查找的信号
protected:
    QLabel * mLabel;                                            //显示查找内容的标签
    QLineEdit * mEdit;                                          // 文本输入框
    QPushButton  * mCheck;                                      // 查找按钮
    QPushButton * mCancel;                                       //取消按钮
    QGroupBox *mgroupBox;                                       //
    QCheckBox *mUpcase ;                                         // 是否忽略大小写
    QRadioButton * mrboxFor;                                     //先前查找
    QRadioButton * mrboxBack;                                    //向后查找

};

#endif // _FINDDLG_H
