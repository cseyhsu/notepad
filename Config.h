#if MSC_VER >=1000
    #pragma execution_character_set("UTF-8")
#endif

#ifndef CONFIG_H
#define CONFIG_H

#include<QFont>
#include<QList>
#include<QSize>
#include<QApplication>

class Config
{
public:
    Config();                                           //构造
    Config(bool autoLine,QFont font,QSize size,bool toolVisual,bool statusVisual,QList<QString>list);
    ~Config();

    bool restore();                                     //读取数据
    bool isVlaid();                                     //数据是否读取成功
    bool isAutoLine();                                  //是否自动换行
    QFont getFont();                                    //获取字体
    QSize getSize();                                    //窗口的大小
    bool isToolVisual();                                //工具栏是否可见
    bool isStatusVisual();                              //状态栏是否可见
    QList<QString> getFilePath();                       //文件路径列表

protected:
    void store();                                       //存储数据

private:
    bool mAutoLine;                                     //是否自动换行
    QFont mFont;                                        //字体
    QSize mSize;                                        //大小
    bool mToolVisual;                                   //工具栏是否可见
    bool mStatusVisual;                                 //状态栏是否可见
    bool mValid;                                        //是否有效
    QList<QString>mFilePath;                            //文件列表


};

#endif // CONFIG_H
