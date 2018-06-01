#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include<QFileDialog>
#include<QDragEnterEvent>
#include<QDropEvent>
#include"Content.h"
#include<memory>
#include<QList>
#include<QIcon>
#include<QFont>

#include"CountString.h"
class QPlainTextEdit;
class QAction;
class QTabWidget;
class QLabel ;
class QIcon;
class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    QTabWidget *getTab();                                   //获取tab的控制
    void setText(std::shared_ptr<QPlainTextEdit> text);     //设置文本
    std::shared_ptr<QPlainTextEdit> getText();              //获取文本
    QList<QString> getFilePath() ;                          //获取所有tab的文本路径
    Content *getContent();                                  //获取当前tab的wid
    virtual     ~NotePad();                                 //销毁notepad
    static   NotePad *getNewInstance();                     //获取一个新的NotePad的例子
    void resetChange(bool re=false);                        //重置mTextChnage

protected :
    void  dropEvent (QDropEvent *event)override;            //重写dropEvent
    void dragEnterEvent (QDragEnterEvent *event)override;   //
    void closeEvent(QCloseEvent *event)override;            //

    //===========================================================================================
    //===处理函数
protected slots:
    void onAbout();                                         //关于的处理函数
    void onHelp();                                          //帮助的处理函数
    void onCursorChange(int col , int ln );                 //光标位置变化的处理函数
    void onWordChange(int wd);                              //字数变化的处理函数
    void onTabChange(int index);                            //tab变化的处理函数
    void onNewClick();                                      //新建文本
    void onTabCloseRe(int index);                           //关闭tab
    void onFileOpenClick();                                 //打开文本
    void onSaveClick();                                     //保存文件
    void onFileSaveClick();                                 //保存文本
    void onFileSaveAsClick();                               //文本另存为
    void onCloseClick();                                    //关闭tab
    void onUndoClick();                                     //撤销
    void onCutClick();                                      //剪切
    void onCopyClick();                                     //拷贝
    void onPasteClick();                                    //粘贴
    void onDeleteClick();                                   //删除
    void onFindClick ();                                    //查找
    void onFindNextClick();                                 //查找下一个
    void onReplaceClick();                                  //替换
    void onGotoClick();                                     //转到第 几行
    void onSelectClick();                                   // 全选
    void onFindForText(QString str , Qt::CaseSensitivity cs);
    void onFindBackText(QString str, Qt::CaseSensitivity cs);
    void onFind(QString str,Qt::CaseSensitivity cs);
    void onReplaceOne(QString str,QString goal,Qt::CaseSensitivity cs);
    void onReplaceAll(QString str, QString goal,Qt::CaseSensitivity cs);
    void onAutoLineClick();                             //是否自动换行
    void onFontClick();                                 //选择字体
    void onStatusClick();                                //状态栏是否可见
    void onTooBarClick();                               //工具栏是否可见
    void setCutAvailable();
    void setActionEnable(QString actionName);            //设置action是否可见
    void initialEditAction();
    void saveFile(QString fileName,QIODevice::OpenMode mode=QIODevice::Text|QIODevice::WriteOnly );

private:

    NotePad(QWidget *parent = 0);                       //构造
    bool initialNotePad();                              //初始化
    bool initialTabWidget();                            //初始化tab
    void setConnect();                                  //链接处理函数
    bool initialMenuFile();                             //初始化文件的菜单
    bool initialStatusBar();                            //初始化状态栏
    bool initialMenuEdit();                             //初始化编辑菜单
    bool initialMenuFormat();                           //初始化格式的帮助菜单
    bool initialMenuView();                             //初始化view菜单
    bool initialMenuHelp();                             //初始化帮助菜单
    bool initialToolBar();                              //初始化工具栏
    bool makeMenuAction(QAction *& action, QString text, int key,QMenu *parent );
    void makeToolBarAction(QAction *&action,QString tips,const QString icon);
    void openFile(QString fileName);                    //打开文件
    void getLastUse();                                  //读取上一次操作的记录
    QAction *findMenuAction(QString str);               //依据名字找action
    QToolBar * findToolBar();                           //在children中找 toolBar


    // 从打开的对话框中选择一个文件。。。title 是对话框的标题，，，
    //  AcceptMode 是
    QString selectFile( QString title,QFileDialog::AcceptMode acceptMode,enum QFileDialog::FileMode fileMode,const QString &filter);



    QTabWidget * tabWidget;                               // tab是界面
    QLabel * mshowColLn;                                   //状态栏 显示光标所在的行和列
    QLabel * mShowWord;                                   // 状态栏显示文本字数
    CountString mThread;                                  // 线程子类
    QFont font;                                            //设置字体
    bool autoLine;                                         //是否为自动换行
    std::shared_ptr<QPlainTextEdit> mText;               //共享文本

    bool mTextChange;                                   //判断文本的内容是否改变
    int lastIndex ;                                         //记录上一次的index


};

#endif // NOTEPAD_H
