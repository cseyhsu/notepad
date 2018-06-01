#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#include "Config.h"
#include<QFile>
#include<QDataStream>
#include<QDebug>
#include<QTextStream>

Config::Config()                                        //读取数据
{
     restore ();
}


//================================================================================
// 带参数的构造
Config::Config(bool autoLine,                       //是否自动换行
               QFont font,                          //文本字体
               QSize size,                          //软件的大小
               bool toolVisual,                     //工具栏是否可见
               bool statusVisual,                   //状态栏是否可见
               QList<QString> list                  //文件的路径
               )
    :mAutoLine(autoLine),mFont(font),
      mSize(size),mToolVisual(toolVisual),
      mStatusVisual(statusVisual),mFilePath(list)
{

    store();                                        //存储成员
}


//===========================================================
//===
Config::~Config()
{

}


//===================================================
// 读取数据
bool Config::restore()
{
    bool ret  = true;
    QFile file(QApplication::applicationDirPath ()+"//config.txt"); //文件的路径
    qDebug()<<"what";
    if(file.open(QIODevice::ReadOnly))                              //以只读的方式打开数据文件
    {
        qDebug()<<" config restore the file";

        QDataStream ou(&file);                                      //将文件关联到数据流
        ou>>mAutoLine;                                              //读取数据送到mAutoLine
        qDebug()<<mAutoLine;

        ou>>mFont;
        qDebug()<<mFont;
        ou>>mSize;
        qDebug()<<mSize;
        ou>>mToolVisual;
        ou>>mStatusVisual;
        ou>>mFilePath;                                      //将数据送到mFilePath ..一次读取多条

        mValid = true;
        qDebug()<<mFilePath<<"end";
        file.close ();


    } else
    {
        ret = false;
    }

    return ret;

}


//========================================================================
// 数据是否读取成功
bool Config::isVlaid()
{
    return mValid;
}


//=======================================================================
//是否自动换行
bool Config::isAutoLine()
{
    return mAutoLine;
}


//====================================================================
//工具栏是否可见
bool Config::isToolVisual()
{
    return mToolVisual;
}


//==================================================================
// 状态栏是否可见
bool Config::isStatusVisual()
{
    return mStatusVisual;
}


//=================================================================
// 获取字体
QFont Config::getFont()
{
    return mFont;
}


//=================================================================
// 获取窗口的大小
QSize Config::getSize()
{
    return mSize;
}

QList<QString> Config::getFilePath()
{
    return mFilePath;
}


//=================================================================
// 存储数据成员
void Config::store()
{
    QFile file(QApplication::applicationDirPath ()+"//config.txt"); //文件路径
    if(file.open (QIODevice::WriteOnly))                            //以写的方式写入数据
    {
        QDataStream in(&file);                                      //将数据关联到流
        in<<mAutoLine;
        in<<mFont;
        in<<mSize;
        in<<mToolVisual;
        in<<mStatusVisual;
        in<<mFilePath;
        file.close ();
    }


}

