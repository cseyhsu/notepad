#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef REPLACEDLG_H
#define REPLACEDLG_H

#include <QObject>
#include<QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;

class ReplaceDlg :public QDialog
{
    Q_OBJECT

signals:
    void findNext(QString text,Qt::CaseSensitivity cs);          //发送向前查找的信号
    void replaceOne(QString text,
                    QString goal,
                    Qt::CaseSensitivity cs);                //发送单个替换
    void replaceAll(QString text,
                    QString goal,
                    Qt::CaseSensitivity cs);                //发送全部替换

protected slots:
    void setFindEnable(QString str);                                  //设置查找按钮的可用性
    void setReplaceEnable(QString str);                               //设置替换按钮是否可用
    void onFindClick();
    void onReplaceClick();                                          //单个替换
    void onReplaceAllClick();                                       //全部替换
    void onCancelClick();                                           //取消

public:
    ReplaceDlg(QWidget *parent = nullptr);
    ~ReplaceDlg();

protected:
    QLabel * mLabel;                //显示查找内容的标签
    QLineEdit * mEdit;              // 文本输入框
    QPushButton  * mCheck;          // 查找下一个

    QLabel * mReplaceL;             //替换内容
    QLineEdit * mReplaceEdit ;      // 替换的内容文本输入
    QPushButton * mReplaceBtn;      //替换按钮

    QPushButton * mReplaceBtnAll;  // 全部替换
    QPushButton * mCancel;          //取消按钮
    QCheckBox *mUpcase ;            // 是否忽略大小写

private :
    bool initial();                 //布局。。初始化                                  //初始化各种的组件和布局
    void setConnect();              //链接处理函数

};

#endif // REPLACEDLG_H
