#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif

#include "NotePad.h"
#include"ReplaceDlg.h"
#include"AboutNotePad.h"
#include"Finddlg.h"
#include"Config.h"

#include<QFileDialog>
#include<QFileInfo>
#include<QList>
#include<QSize>
#include<QInputDialog>
#include<QAction>
#include<QMenuBar>
#include<QFontDialog>
#include<QMenu>
#include<QStatusBar>
#include<QLabel>
#include<QKeySequence>
#include<QTabWidget>
#include<QPlainTextEdit>
#include<QHBoxLayout>
#include<QToolBar>
#include<QIcon>
#include<QList>
#include<QMimeData>
#include<QUrl>
#include<QFile>
#include<QFileInfo>
#include<QTextStream>
#include<array>
#include<QDebug>
#include<QDesktopServices>
#include<QMessageBox>
#include<QSize>
#include<QTextCursor>
#include<QMenuBar>
#include<QKeyEvent>

using std:: make_shared;
using std:: shared_ptr;



//=============================================================================
// 初始化
NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("notePad --");
    setAcceptDrops (true);
    font.setFamily ("宋体");
    font.setPointSize (12);
    autoLine = true;                                                    //设置为自动换行
    mThread.moveToThread (&mThread);
    mThread.start ();
    qDebug()<<" main tid "<<QThread::currentThreadId ();

    connect(&mThread,SIGNAL(cursorIn(int,int)),this,SLOT(onCursorChange(int,int)));
    connect(&mThread,SIGNAL(wordCount(int)),this,SLOT(onWordChange(int)));
}

//====================================================================================
//== 获取一个 tabWidget
QTabWidget *NotePad::getTab()
{
    return tabWidget;
}

void NotePad::setText(std::shared_ptr<QPlainTextEdit> text)
{
    qDebug()<<" void NotePad::setText(std::shared_ptr<QPlainTextEdit> text)";
    mText = text;
}

std::shared_ptr<QPlainTextEdit> NotePad::getText()
{
    qDebug()<<"std::shared_ptr<QPlainTextEdit> NotePad::getText()";
    return mText;

}

QList<QString> NotePad::getFilePath()
{
    QList<QString> list;
    auto cou = tabWidget->count ();
    for(int i = 0 ; i< cou; ++i)
    {
        auto content = getContent ();
        if(content)
        {
            list.push_back (content->getFilePath ());
        }
    }

    return list;
}

Content* NotePad::getContent()
{
    auto index = tabWidget->currentIndex ();
    auto content =dynamic_cast<Content*>(tabWidget->widget (index));
    return content;
}





NotePad::~NotePad()
{


    auto actionSta= findMenuAction ("状态栏");
    bool bStatu;
    if(actionSta)
    {
        bStatu=actionSta->isChecked ();
    }
    auto actionToo = findMenuAction ("工具栏");
    bool bToo;
    if(actionToo)
    {
        bToo = actionToo->isChecked ();
    }
    QSize sz=size();
    Config config(autoLine,font,sz,bToo,bStatu,getFilePath ());


}

//======================================================================
// 获取一个新的实例
NotePad *NotePad::getNewInstance()
{
    NotePad * notePad = new NotePad();

    if(  notePad)
    {

        notePad->initialNotePad ();
        notePad->setConnect ();

    }
    else
    {
        notePad = nullptr;
    }

    return notePad;
}


//=====================================================
// 重置mTextChange
void NotePad::resetChange(bool re)
{
    mTextChange= re;
}


//========================================================
// 重写的 dropEvent
void  NotePad::dropEvent(QDropEvent *event)
{
    if(event->mimeData ()->hasUrls ())
    {
        auto list =  event->mimeData()->urls ();
        QString path = list[0].toLocalFile ();
        QFileInfo fo(path);

        qDebug()<<"fo.bule Nmae"<<fo.bundleName ()<<
                  fo.canonicalPath ()<<fo.fileName ();
        if(fo.isDir () | ! fo.fileName ().contains (".txt"))
        {
            QMessageBox::critical (this,"遇到","这是一个文件夹(或者不是.text文件)",
                                   QMessageBox::Ok);
            return ;
        }



        QFile file(path);
        if(file.open(QIODevice::Text| QIODevice::ReadOnly))
        {
            QTextStream ou(&file);
            QFileInfo info(file);
            // auto widget = tabWidget->currentWidget ();
            // auto w = static_cast<Content*>(widget);
            auto newWid = new Content(this);
            tabWidget->addTab (newWid,info.baseName ());
            setWindowTitle (info.baseName ());
            auto num =    tabWidget->count ();
            tabWidget->setCurrentIndex (num -1);
            mThread.setText (newWid->getEdit ());
            newWid->getEdit ()->setPlainText (ou.readAll ());
            file.close ();
            //filePath.append (path);

            auto content = getContent ();
            if(content)
            {
                content->setFilePath (path);
            }

            auto list = getFilePath ();
            for( int i = 0 ; i< list.count ();++i)
            {
                qDebug()<< " file Path "<< list[i];
            }
        }
    }
}


//======================================================================
// 重写的dragEnterEvent
void  NotePad::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData () ->hasUrls ())
    {
        auto list =  event->mimeData()->urls ();
        QString path = list[0].toLocalFile ();
        QFileInfo fo(path);
        if(!fo.isDir ())
        {
            if(fo.fileName ().contains (".txt"))
            {
                event->accept ();

            }
            else
            {
                event->ignore ();
            }

        }

        else {
            event->ignore ();
        }
    }

    else{
        event->ignore ();
    }
}

void NotePad::closeEvent(QCloseEvent *event)
{
    if(tabWidget->count () == 0)
    {
        qDebug()<<" 我只有个一个文件的tab了";
        auto actionSta= findMenuAction ("状态栏");
        bool bStatu;
        if(actionSta)
        {
            bStatu=actionSta->isChecked ();
        }
        auto actionToo = findMenuAction ("工具栏");
        bool bToo;
        if(actionToo)
        {
            bToo = actionToo->isChecked ();
        }
        QSize sz=size();
        Config config(autoLine,font,sz,bToo,bStatu,getFilePath ());
        event->accept ();
    }
    auto ret =  QMessageBox::critical (this,"关闭窗口","shifou",QMessageBox::Ok,QMessageBox::Cancel);
    if(ret == QMessageBox::Ok)
    {
        auto actionSta= findMenuAction ("状态栏");
        bool bStatu;
        if(actionSta)
        {
            bStatu=actionSta->isChecked ();
        }
        auto actionToo = findMenuAction ("工具栏");
        bool bToo;
        if(actionToo)
        {
            bToo = actionToo->isChecked ();
        }
        QSize sz=size();
        Config config(autoLine,font,sz,bToo,bStatu,getFilePath ());
        event->accept ();
    }
    else
    {
        event->ignore ();
    }
}

// ==================================================
// 打开about 对话框
void NotePad::onAbout()
{
    qDebug()<<" call on about";
    AboutNotePad  *about = new AboutNotePad;
    about->setAttribute (Qt::WA_DeleteOnClose);
    about->show ();
}

//=============================================================
// 打开 help 的链接
void NotePad::onHelp()
{
    QDesktopServices::openUrl (QUrl("http://www.baidu.com/s?wd=有问题找百度"));
}


//===========================================================================
//= 当前的文本的光标移动的事件
void NotePad::onCursorChange(int col, int ln)
{
    QString str="第 "+ QString::number (col)+"  行 , 第  "+QString::number (ln)+" 列";
    mshowColLn ->setText (str);
}

//============================================================================
// 当前的文本的字数发生变化的事件
void NotePad::onWordChange(int wd)
{
    mShowWord->setText ("word : "+QString::number(wd));
    mTextChange = true;
}

//===========================================================================
// 弹出 查找对话框
void NotePad::onFindClick()
{
    Finddlg * find = new Finddlg;
    find ->setAttribute (Qt::WA_DeleteOnClose);
    find->show ();

    connect(find,SIGNAL(findBack(QString,Qt::CaseSensitivity)),this,SLOT(onFindBackText(QString,Qt::CaseSensitivity)));
    connect(find, SIGNAL(findFor(QString,Qt::CaseSensitivity)),this,SLOT(onFindForText(QString,Qt::CaseSensitivity)));
}


//==============================================================================
// ==弹出查找下一个的对话框
void NotePad::onFindNextClick()
{
    qDebug()<<"void NotePad::onFindNextClick()";
}


//=============================================================================
//==替换对话框
void NotePad::onReplaceClick()
{
    qDebug()<<"void NotePad::onReplaceClick()";
    auto reDlg = new ReplaceDlg;
    reDlg->setAttribute (Qt::WA_DeleteOnClose);

    connect(reDlg,SIGNAL(findNext(QString,Qt::CaseSensitivity)),this,SLOT(onFind(QString,Qt::CaseSensitivity)));
    connect (reDlg,SIGNAL(replaceAll(QString,QString,Qt::CaseSensitivity)),this,SLOT(onReplaceAll(QString,QString,Qt::CaseSensitivity)));
    connect(reDlg,SIGNAL(replaceOne(QString,QString,Qt::CaseSensitivity)),this,SLOT(onReplaceOne(QString,QString,Qt::CaseSensitivity)));

    reDlg->show ();
}

//=========================================================================
// 转到某一行的对话框
void NotePad::onGotoClick()
{
    qDebug()<<"void NotePad::onGotoClick()";
    bool ok;
    auto box =  QInputDialog::getInt (this,"转到","行数 , 最大值为 :"+QString::number(mThread.getMaxLine ()),1,1,mThread.getMaxLine (),1,&ok,Qt::WindowCloseButtonHint|Qt::Drawer);
    qDebug()<<"get : "<<box;

    if(ok)
    {
        qDebug()<<mText.get ();


        auto cursor = mText->textCursor ();
        QString str = mText->toPlainText ();
        int index = 0;
        int next = -1 ;

        for( int i = 0 ; i< box; ++i)
        {
            index = next+1;
            next = str.indexOf ('\n',index);
        }

        cursor.setPosition (index);
        mText->setTextCursor (cursor);

    }
}

// ============================================================================
//==全选的对话框
void NotePad::onSelectClick()
{
    qDebug()<<"void NotePad::onSelectClick()";
    mText->selectAll ();
}


// =======================================================
//  向前查找
void NotePad::onFindForText(QString str, Qt::CaseSensitivity cs)
{
    qDebug()<<"void NotePad::onFindForText(QString str, Qt::CaseSensitivity cs) ";
    qDebug()<<" find text is "<<str<<" wenbeineirong "<<mText->toPlainText ();

    static int index = -1;
    QString strText =mText->toPlainText ();
    int pos = strText.lastIndexOf (str,index,cs);

    if(-1 !=  pos)
    {

        index =  pos - str.length ();
        auto textC = mText->textCursor ();
        textC.setPosition (pos);
        textC.setPosition (pos+str.length (),QTextCursor::KeepAnchor);
        mText->setTextCursor (textC);
    } else
    {
        index = -1;
    }


}


//=================================================================
//  向后查找
void NotePad::onFindBackText(QString str, Qt::CaseSensitivity cs)
{
    qDebug()<<"void NotePad::onFindBackText(QString str, Qt::CaseSensitivity cs)";
    qDebug()<<" find Text is "<< str;

    static int index = 0 ;

    QString strText = mText->toPlainText ();

    int pos = strText.indexOf (str,index,cs);
    if(-1 != pos)
    {
        qDebug()<<" pos = "<<pos<<" index = "<<index;
        index = pos+str.length ();
        auto textC = mText->textCursor ();
        textC.setPosition (pos);
        textC.setPosition(pos+str.length (),QTextCursor::KeepAnchor);
        mText->setTextCursor (textC);

    } else
    {
        index = 0;
    }
}

//=============================================================
// == 查找
void NotePad::onFind(QString str, Qt::CaseSensitivity cs)
{
    onFindBackText(str,cs);
}



//===============================================================
// 单个 替换
void NotePad::onReplaceOne(QString str, QString goal, Qt::CaseSensitivity cs)
{
    auto strText= mText->toPlainText ();
    auto cur=mText->textCursor ();
    auto selec= cur.selectedText ();
    int pos =cur.position ();
    auto afterText=strText.replace (pos-selec.length (),selec.length (),goal);
    mText->setPlainText (afterText);
}


// =====================================================================
// 全部替换
void NotePad::onReplaceAll(QString str, QString goal, Qt::CaseSensitivity cs)
{
    QString strText= mText->toPlainText ();
    QString after = strText.replace (str,goal,cs);
    mText->setPlainText (after);
}

// ===========================================================================
// 自动换行的对话框
void NotePad::onAutoLineClick()
{
    qDebug()<<"void NotePad::onAutoLineClick()";
    auto action= findMenuAction ("自动换行");
    if(action)
    {
        auto check= action->isChecked ();
        qDebug()<<"是否是可以 checkAble : "<<action->isCheckable ()<<"当前是否是可以： "<<!check;

        autoLine = check;
        if(autoLine){

            mText->setLineWrapMode( QPlainTextEdit::WidgetWidth);

        } else
        {

            mText->setLineWrapMode( QPlainTextEdit::NoWrap);

        }
    }
}


//===========================================================================
// 改变字体
void NotePad::onFontClick()
{
    qDebug()<<"void NotePad::onFontClick()";
    QFontDialog  dlg(this);
    if(dlg.exec () == QFontDialog::Accepted)
    {
        font = dlg.selectedFont ();          //选择字体设置新的字体
        mText->setFont (font);              // 重新设置字体
    }


}

//============================================================================
//= 状态栏的对话框
void NotePad::onStatusClick()
{
    qDebug()<<"void NotePad:onStatusClick()";
    auto action =findMenuAction ("状态栏");
    if(action)
    {
        if(action->isChecked ())
        {
            statusBar ()->setVisible (true);

        } else
        {
            statusBar ()->hide ();

        }
    }
}



//=============================================================================
// 是否是显示toolBar的对话框
void NotePad::onTooBarClick()
{
    qDebug()<<" void NotePad::onToolBarClick()";

    auto action =findMenuAction ("工具栏");
    if(action)
    {
        QToolBar * toolBar = findToolBar ();


        if(toolBar)
        {
            if(action->isChecked ())
            {
                toolBar->setVisible (true);

            } else
            {
                toolBar->hide ();
            }
        }
    }

}


// ==============================================================================
// 设置剪切这个action 是否可用
void NotePad::setCutAvailable()
{
    qDebug()<<"void NotePad::setCutAvailable()";
    if(mText)
    {
        if(mText->toPlainText ().count ()  == 0 )
        {
            auto action =findMenuAction ("剪切");
            if(action)
            {
                action->setEnabled (false);
            }
        }
    }
}





void NotePad::setActionEnable(QString actionName)
{
    auto action = findMenuAction (actionName);                      //查找action
    if(action)
    {
        action->setEnabled (!mText->toPlainText ().isEmpty ());
    }
}

void NotePad::onTabChange(int index)
{

    auto item =static_cast<Content*>( tabWidget->widget (index));
    setText(item->getEdit ());
    mThread.setText (item->getEdit ());
    onWordChange (item->getEdit ()->toPlainText ().count ());       //强制刷新显示字数的标签
    onCursorChange (0,0);                                           //强制更新光标的位置
    mText->setFont (font);                                          //设置当前tab的字体

    auto content= getContent ();
    if(content)
    {
        QString filePath = content->getFilePath ();
        QFileInfo    info(filePath);
        if(! filePath.isEmpty ())
        {
            setWindowTitle (info.baseName ());
            openFile (filePath);
            tabWidget->setTabText (index,info.baseName ());
        }
        else {

            setWindowTitle ("notePad--*[txt]");
        }

    }


    connect(mText.get (),SIGNAL(textChanged()),this,SLOT(setCutAvailable()));  //将新的mText链接处理函数
}


//==========================================================================
// 新建文件
void NotePad::onNewClick()
{
    qDebug()<<" 新建一个文件";


    auto  newItem = new Content(this);              //新建一个tab
    setWindowTitle ("新建 *");                        //给新建的tab 起名字
    tabWidget ->addTab (newItem,"新建");           //将新建的tab 添加到 tabWidget 中去
    setText (newItem->getEdit ());                  //重新设置当前的 mText(智能指针)
    mText->setFont (font);                          //重新设置字体
    initialEditAction ();                           //重新初始化编辑的状态
    auto coun = tabWidget->count();                 //计算tabWidget 中的weidget 数量
    tabWidget->setCurrentIndex (coun-1);            //将当前的tabWidget 设置为当前新添加的一个
    // 将当前的 文本内容和 编辑框中的状态链接在一起
    connect(newItem->getEdit ().get (),SIGNAL(textChanged()),this,SLOT(initialEditAction()));

    //filePath.push_back (QString(""));           //计入一个空的文件路径

    newItem->setFilePath ("");

    auto list = getFilePath ();
    for( int i = 0 ; i< list.count () ; ++i)
    {
        qDebug()<<" 文件路径名 ： "<<list[i];
    }

}


// ========================================================================
//=== 关闭一个 widget
void NotePad::onTabCloseRe(int index)
{
    qDebug()<<" onTabClodeRe";


    auto cou = tabWidget->count ();         //获取当前的tab的数量
    if(cou == 1)
    {
        close();
        qDebug()<<" the last one";
    }
    auto wid=tabWidget->widget (index);

    tabWidget->removeTab (index);
    delete wid;
    //    filePath.removeAt (index);

    auto list = getFilePath ();
    for(  int i =0 ; i< list .count (); ++i)
    {
        qDebug()<<" after remove tab "<<list[i];
    }
}


//================================================================
//打开一个新的文件
void NotePad::onFileOpenClick()
{
    qDebug()<<"void NotePad::onFileOpenClick() ";

    if(mTextChange)
    {
        onFileSaveClick ();
        resetChange ();
    }
    if(mText)
    {
        mText->setFont (font);
    }
    QString filter = "TEXT[文本文件] (*.txt)";          //设置一个文本过滤


    QString ret = selectFile (QString("打开文件"),QFileDialog::AcceptOpen,QFileDialog::ExistingFile,filter);
    if(!ret .isEmpty ())                                    //如果选择的文件的路径不为空
    {
        QFile file(ret);                                    // 打开文件   方式
        if(file.open (QIODevice::ReadOnly| QIODevice::Text))
        {
            // 如果打开文件成功
            QTextStream ou(&file);                          //把文件关联到流

            Content * newWidget= new Content(this);         // 新见一个Contente， 用于显示新文本的内容
            QFileInfo info(ret);                            //获取文本的信息
            setWindowTitle(info.baseName ());               // 获取文本的基础名字
            setText (newWidget->getEdit ());                //重新关联mText 数据
            tabWidget->addTab (newWidget,info.baseName ()); //将新的tab 加入到tabWidge,(加)夹到tab的尾巴
            auto cou = tabWidget->count ();                 //获取tabWidget的tab的数量
            tabWidget->setCurrentIndex (cou-1);             //重新设置当前的tabWidget
            mText->setPlainText (ou.readAll ());            //将文本的内容读取到mText
            file.close ();                                  //关闭文件

            //filePath.push_back (ret);                       //将当前的文件路径添加到filePath

            newWidget->setFilePath (ret);

            auto list = getFilePath ();
            for( int i = 0 ; i< list.count();++i)
            {
                qDebug()<<"文件路径 : "<<list[i];
            }
        }
    }

}


//==================================================================
//== 保存一个文件
void NotePad::onSaveClick()
{
    qDebug()<<"void NotePad::onSaveClick()";

    QFileDialog flg (this);
    flg.setAcceptMode (QFileDialog::AcceptSave);
    flg.setWindowTitle ("保存文件");

    if(flg.exec () == QFileDialog::Accepted)
    {
        qDebug()<<" accepted ";
    }
}


// ========================================================
//== 保存一个文件
void NotePad::onFileSaveClick()
{
    qDebug()<<"void NotePad::onFileSaveClick()";
    auto list = getFilePath ();
    qDebug()<<" now filePath.count () = "<<list.count ()<<" tabWidge count "<<tabWidget->count ();

    //    int cou = tabWidget->currentIndex ();                   //获取当前的tabWidget的下标
    auto content = getContent ();
    if(content)
    {
        QString filePath = content->getFilePath ();
        if(!filePath.isEmpty ())                           //判断当前的tabWidget所对应的文件的路径是否为空
        {                                                       //如果当前的文件路径不是为空的
            QString path = filePath;                       // 获取当前的的文件路径

            saveFile (path);
        } else{



            QString filter = "TEXT (*.txt)";


            QString ret = selectFile (QString("保存文件"),QFileDialog::AcceptSave,QFileDialog::AnyFile,filter);
            if(!ret .isEmpty ())
            {
                QFileInfo info(ret);
                saveFile(ret);                                          //保存文件
                int cou = tabWidget->currentIndex ();
                // filePath[cou]= ret;                                      //将新的文件路径添加到filepath 用于保存文件
                content->setFilePath (ret);
                tabWidget->setTabText (cou,info.baseName ());           //更新tab的名字
                setWindowTitle (info.baseName ());

            }
        }
    }

}

//===============================================================
// 将文件另存为
void NotePad::onFileSaveAsClick()
{
    qDebug()<<" onFileSaveAsCLick";
    QString str_Save = "将文件另存为";
    QString filter="TEXT[文本文件](*.txt)";
    QString ret = selectFile (str_Save,QFileDialog::AcceptSave,QFileDialog::AnyFile,filter);
    if(!ret.isEmpty ())
    {

        saveFile(ret);                                          //保存文件

        if(tabWidget->count () == 1)
        {
            tabWidget->addTab (new Content(this),"new");        //新建一个

            tabWidget->removeTab (0);                           //将这个tab从tab中移除
            // filePath.removeAt (0);
        }
        else
        {
            auto cou = tabWidget->currentIndex ();
            auto content = tabWidget->widget (cou);
            tabWidget->removeTab (cou);
            //  filePath.removeAt (cou);

            delete content;
        }

        auto list = getFilePath ();
        for( int i = 0 ; i< list.count ();++i)
        {
            qDebug()<<"after saveas file "<<list[i];
        }
    }
}



//=========================================================================
//== 关闭文件
void NotePad::onCloseClick()
{
    qDebug()<<" onCloseClick";

    auto index = tabWidget->currentIndex ();
    tabWidget->removeTab (index);
}


//====================================================================
//撤销操作
void NotePad::onUndoClick()
{
    qDebug()<<"void NotePad::onUndoClick()";
    if(mText)
    {
        qDebug()<<" mText 'cout "<<mText.use_count ();
        mText->undo ();
    }
}



//======================================================================
// 剪切操作
void NotePad::onCutClick()
{
    qDebug()<<"void NotePad::onCutClick()";
    mText->cut ();;
}


// ===============================================================
// 复制操作
void NotePad::onCopyClick()
{
    qDebug()<<"void NotePad::onCopyClick()";
    mText->copy ();
}



//================================================================
//粘贴操作
void NotePad::onPasteClick()
{
    qDebug()<<"void NotePad::onPasteClick()";
    mText->paste ();
}


//==================================================================
//删除操作
void NotePad::onDeleteClick()
{
    qDebug()<<"void NotePad::onDeleteClick()";
    auto event= new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
    QApplication::sendEvent(mText.get (),event);

    delete event;
}


//========================================================================
//初始化
bool NotePad::initialNotePad()
{
    bool ret = true;
    mshowColLn = new QLabel (this);
    mShowWord = new QLabel(this);
    mShowWord->setText ("这里是状态栏");
    mshowColLn ->setText ("行 :  列 : ");
    auto menuB = new QMenuBar(this);
    setMenuBar (menuB);
    initialTabWidget();
    initialMenuFile ();
    initialMenuEdit ();
    initialMenuFormat();
    initialMenuView ();
    initialMenuHelp ();
    initialToolBar ();
    initialStatusBar ();

    getLastUse ();

    return ret;

}

// ========================================================
// 初始化tabWidget
bool NotePad::initialTabWidget()
{
    bool ret = true;
    tabWidget = new QTabWidget(this);
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(onTabChange(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabCloseRe(int)));

    tabWidget->setTabPosition (QTabWidget::North);
    tabWidget->setTabsClosable (true);
    // auto me = new Content(this);
    // me->getEdit () ->setPlainText ("这是测试文本");
    // tabWidget ->addTab (me,"自定义");


    setCentralWidget (tabWidget);

    //  me->setFilePath ("");
    return ret;
}


//================================================================
//链接处理函数
void NotePad::setConnect()
{

}

//================================================================================
// 初始化文件的菜单
bool NotePad::initialMenuFile()
{
    bool ret = true;

    auto  mb = menuBar ();                     // 获取 menuBar的 控制信息
    QMenu *menu = new QMenu("文件(&F)",mb);    //新生成一个菜单选项,parent是menuBar
    mb->addMenu (menu);                        // 将新生成的菜单添加到 menuBar

    // 以下是action信息
    const std:: array<QString,5>text={
        "新建",
        "打开",
        "保存",
        "另存为",
        "退出",
    };

    const std:: array<int, 5>key={
        Qt::CTRL+Qt::Key_N,
        Qt::CTRL+Qt::Key_O,
        Qt::CTRL+Qt::Key_S,
        Qt::CTRL+Qt::Key_M,
        0,


    };


    std:: array<QAction * ,5>action={
        nullptr,
    };

    for(unsigned int i = 0; i< action.size(); ++i)
    {
        makeMenuAction(action[i],text[i],key[i],menu);      //初始化action
        if(action[i] != nullptr)
        {
            menu->addAction(action[i]);                     //将新的action 添加到新生成的菜单中去
        }
        else
        {
            ret  = false;

        }
    }

    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onNewClick()));       //新建文件
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onFileOpenClick()));   // 打开文件
    connect(action[2],SIGNAL(triggered(bool)),this,SLOT(onFileSaveClick()));   // 保存文件
    connect(action[3],SIGNAL(triggered(bool)),this,SLOT(onFileSaveAsClick())); // 另存为
    connect(action[4],SIGNAL(triggered(bool)),this,SLOT(close()));             //关闭
    return ret;

}

//===================================================================================
// 初始化状态栏的菜单
bool NotePad::initialStatusBar()
{
    bool ret = true;
    auto sb = statusBar ();                                 //获取状态栏
    sb->addPermanentWidget (mShowWord);                    // 将显示字数的label 添加到 状态栏
    sb->addWidget (mshowColLn);                            // 将显示光标的信息添加到 statusBar
    return ret;
}

// =====================================================================================
//= 初始化编辑菜单
bool NotePad::initialMenuEdit()
{
    bool ret = true;
    auto mb = menuBar ();                               //获取menuBar的控制
    auto menu = new QMenu("编辑(E)",mb);                 //新建一个 menu
    mb->addMenu (menu);                                 // 将新生成的menu 添加到menuBar
    //以下是action 信息

    const std:: array<QString ,10>text={
        "撤销(U)",
        "剪切(T)",
        "复制(C)",
        "粘贴(P)",
        "删除(L)",
        "查找(F)",
        "查找下一个(N)",
        "替换(R)",
        "转到(G)",
        "全选(A)",

    };

    const std:: array<int, 10>key={
        Qt::CTRL+Qt::Key_Z,
        Qt::CTRL+Qt::Key_X,
        Qt::CTRL+Qt::Key_C,
        Qt::CTRL+Qt::Key_V,
        Qt::Key_Delete,
        Qt::CTRL+Qt::Key_F,
        Qt::Key_F3,

        Qt::CTRL+Qt::Key_H,

        Qt::CTRL+Qt::Key_G,
        Qt::CTRL+Qt::Key_A,



    };

    std:: array<QAction * ,10>action={
        nullptr,
    };

    for(unsigned int i = 0; i< action.size(); ++i)
    {
        makeMenuAction(action[i],text[i],key[i],menu);  //将menu作为新的action的parent
        if(action[i] )                                  //如果新的action有效
        {
            menu->addAction(action[i]);                 //将action 添加到menu
        }
        else                                            //退出初始化
        {
            ret  = false;
            return ret;

        }



    }

    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onUndoClick()));           //撤销cao'z操作
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onCutClick()));            // 剪切操作
    connect(action[2],SIGNAL(triggered(bool)),this,SLOT(onCopyClick())) ;          //复制操作
    connect(action[3],SIGNAL(triggered(bool)),this,SLOT(onPasteClick()));         //粘贴操作
    connect(action[4],SIGNAL(triggered(bool)),this,SLOT(onDeleteClick()));         //删除操作
    connect(action[5],SIGNAL(triggered(bool)),this,SLOT(onFindClick()));          //弹出一个查找对话框
    connect(action[6],SIGNAL(triggered(bool)),this,SLOT(onFindNextClick()));      //查找下一个的操作
    connect(action[7],SIGNAL(triggered(bool)),this,SLOT(onReplaceClick()));       //替换的操作
    connect(action[8],SIGNAL(triggered(bool)),this,SLOT(onGotoClick())) ;         //转到某一行的操作
    connect(action[9],SIGNAL(triggered(bool)),this,SLOT(onSelectClick()));        //全选的操作
    return ret;
}

//  ====================================================================================
//初始化g格式  菜单
bool NotePad::initialMenuFormat()
{
    bool ret = true;
    auto mb = menuBar ();
    auto menu = new QMenu("格式(O)",mb);
    mb->addMenu (menu);

    qDebug()<<__LINE__<<" initail format Menu "<<menu;


    const std::array<QString ,2 > text=
    {
        "自动换行",
        "字体",
    };
    std:: array<int  , 2> key={
        0,
    };

    std::array<QAction * , 2> action={
        nullptr,
    };

    for( unsigned int i = 0 ; i< action.size(); ++i)
    {
        makeMenuAction(action[i],text[i],key[i],menu);
        if(action[i] )
        {
            menu->addAction(action[i]);
        }
        else
        {
            ret  = false;
            return ret;

        }

    }
    action[0]->setCheckable(true);

    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onAutoLineClick()));   // 自动换行
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onFontClick()));        // 选择字体
    return ret;
}


//=======================================================================================
// 初始化查看菜单
bool NotePad::initialMenuView()
{
    bool ret = true;
    auto mb = menuBar ();
    auto menu = new QMenu("查看(V)",mb);
    mb->addMenu (menu);
    qDebug()<<__LINE__<<" initail view Menu "<<menu;

    const std::array<QString ,2 > text=
    {
        "状态栏(S)",
        "工具栏(T)",
    };
    std:: array<int  , 2> key={
        0,
    };

    std::array<QAction * , 2> action={
        nullptr,
    };

    for( unsigned int i = 0 ; i< action.size(); ++i)
    {
        makeMenuAction(action[i],text[i],key[i],menu);
        if(action[i] )
        {
            menu->addAction(action[i]);
            action[i]->setCheckable(true);
        }
        else
        {
            ret  = false;
            return ret;

        }


    }

    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onStatusClick()));   //状态栏
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onTooBarClick())) ; // 工具栏的是否可见
    return ret;
}

//=======================================================================================
// 初始化帮助菜单
bool NotePad::initialMenuHelp()
{
    bool ret = true;
    auto mb = menuBar ();
    auto menu = new QMenu("帮助(H)",mb);
    mb->addMenu (menu);
    qDebug()<<__LINE__<<" initail help Menu "<<menu;

    const std::array<QString ,2 > text=
    {
        "查看帮助(H)",
        "关于notePad--(A)",
    };
    std:: array<int  , 2> key={
        0,
    };

    std::array<QAction * , 2> action={
        nullptr,
    };

    for( unsigned int i = 0 ; i< action.size(); ++i)
    {
        makeMenuAction(action[i],text[i],key[i],menu);
        if(action[i] )
        {
            menu->addAction(action[i]);
        }
        else
        {
            ret  = false;
            return ret;

        }


    }
    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onHelp()));      //帮助
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onAbout()));    //关于
    return ret;
}


// ==================================================
// 初始化toolBar
bool NotePad::initialToolBar()
{
    bool ret = true;

    auto toob = new  QToolBar("toolBar",this);
    toob->setToolTip ("我是工具栏");
    toob->setMovable (true);
    toob->setFloatable (true);
    toob->setIconSize (QSize(20,20));
    addToolBar (toob);

    const std:: array<QString,4>tips={
        "新建",
        "打开",
        "保存",
        "关闭",
    };

    const std::array<QString,4> icon={
        ":img/new.png",
        ":img/open.png",
        ":img/save.png",
        ":img/close.png",
    };

    std::array<QAction* ,4> action={
        nullptr,
    };

    for( unsigned int i = 0 ; i< action.size (); ++i)
    {
        makeToolBarAction (action[i],tips[i],icon[i]);
        if(action[i])
        {
            toob->addAction(action[i]);
        } else
        {
            ret = false;
            return ret;
        }
    }


    connect(action[0],SIGNAL(triggered(bool)),this,SLOT(onNewClick()));  // 新建文件
    connect(action[1],SIGNAL(triggered(bool)),this,SLOT(onFileOpenClick())); // 打开文件
    connect(action[2],SIGNAL(triggered(bool)),this,SLOT(onFileSaveClick()));  // 保存文件
    connect(action[3],SIGNAL(triggered(bool)),this,SLOT(onCloseClick())); // 关闭当前的tab


    return ret;
}


// ===========================================================================
//= 初始化 菜单的action
bool NotePad::makeMenuAction(QAction *&action, QString text, int key,QMenu * parent)
{
    bool ret = true;
    action = new QAction (text,parent);
    if( action )
    {
        action ->setShortcut (key);
    } else
    {
        ret = false;
    }
    return ret;
}


//============================================================================
// 初始化工具栏的action
void NotePad::makeToolBarAction(QAction *&action, QString tips,const  QString icon)
{
    action = new QAction(this);
    if(action)
    {
        action ->setToolTip (tips);

        action->setIcon (QIcon(icon));
    }
}

void NotePad::openFile(QString fileName)
{
    QFile file(fileName);
    if(file.open (QIODevice::ReadOnly| QIODevice::Text))
    {
        QTextStream ou(&file);
        mText->setPlainText (ou.readAll ());
        file.close ();;
    }
}

void NotePad::getLastUse()
{
    Config config;
    if(config.isVlaid ())
    {
        auto actionSta= findMenuAction ("状态栏");
        if(actionSta)
        {
            actionSta->setChecked (config.isStatusVisual ());
            statusBar ()->setVisible (config.isStatusVisual ());
        }

        auto actionToo = findMenuAction ("工具栏");
        if(actionToo)
        {
            actionToo->setChecked (config.isToolVisual ());
            auto toolBar = findToolBar ();
            toolBar->setVisible (config.isToolVisual());
        }
        font= config.getFont ();
        QSize sz = config.getSize ();
        resize (sz);


        if(config.getFilePath ().count ()  != 0 )
        {
            auto list = config.getFilePath ();


            for(int i = 0 ; i< list.count ();++i)
            {
                QFileInfo info(list[i]);
                auto content = new Content(this);
                content->setFilePath (list[i]);
                tabWidget->addTab (content,info.baseName ());
                auto cou = tabWidget->count ();
                tabWidget->setTabText (cou-1,info.baseName ());
            }


            tabWidget->setCurrentIndex (tabWidget->count()-1);

        }
    }

}


//===========================================================================================
//初始化编辑的action是否可用
void NotePad::initialEditAction()
{
    const std:: array<QString ,9>text={
        "撤销(U)",
        "剪切(T)",
        "复制(C)",

        "删除(L)",
        "查找(F)",
        "查找下一个(N)",
        "替换(R)",
        "转到(G)",
        "全选(A)",

    };

    for ( unsigned int i = 0 ; i< text .size (); ++i)
    {
        setActionEnable (text[i]);
    }

}


//=============================================================
// 保存一个文件
void NotePad::saveFile(QString fileName, QIODevice::OpenMode mode)
{
    QFile file(fileName);                                   //生成一个file对象
    if(file.open (mode))                                // 打开文件
    {
        QTextStream ou(&file);                          // 将文件关联到QTextStream 对象
        ou<<mText->toPlainText ();                        //将mText中的全部内容全部写到流对象

        file.close ();                                  //关闭文件。。刷新流空间
        return ;                                        //退出
    }
}

QAction *NotePad::findMenuAction(QString str)
{
    QAction * ret = nullptr;

    auto menu = menuBar ();
    auto list =menu->children ();

    for( int i = 0 ; i< list.count ();++i)
    {
        auto sub = dynamic_cast<QMenu*>(list[i]);
        if(sub)
        {
            auto actions = sub->actions ();

            for( int j = 0 ;j< actions.count (); ++j)
            {
                if( static_cast<QAction*>( actions[j]) ->text ().startsWith (str))
                {
                    ret = static_cast<QAction*>( actions[j]) ;
                    return ret;
                }
            }
        }
    }

    qDebug()<<" i get a action "<<ret;
    return ret;
}

QToolBar *NotePad::findToolBar()
{
    QToolBar * toolBar = nullptr;
    auto chl = children ();
    for( int i =0 ; i< chl.count ( ); ++i)
    {
        toolBar = dynamic_cast<QToolBar*>(chl[i]);
        if(toolBar!=nullptr)
        {
            break;
        }
    }

    return toolBar;

}

QString NotePad::selectFile( QString title, QFileDialog::AcceptMode acceptMode, QFileDialog::FileMode fileMode, const QString &filter)
{
    QString ret ="";
    qDebug()<<"QString NotePad::selectFile(const QString title, QFileDialog::AcceptMode acceptMode,"
              "QFileDialog::FileMode fileMode, const QString &filter)";

    QFileDialog flg(this);
    flg.setWindowTitle (title);
    flg.setAcceptMode (acceptMode);
    flg.setFileMode (fileMode);
    flg.setNameFilter (filter);     //设置文本过滤器

    if(flg.exec () == QFileDialog::Accepted)
    {
        ret = flg.selectedFiles ()[0];
    }

    return ret;
}


