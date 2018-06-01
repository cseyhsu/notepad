/*=============================================================
 *
 *  说明 : 即兴搞机
 *  作者 : xingcent
 *  完成日期 ：2017年8月21日
 *
 *
 * =============================================================*/


/*==============================================================
 *
 *  修改世间         修改原因                          修改人
 *
 * =============================================================*/

#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#include "NotePad.h"
#include <QApplication>
#include<QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon (QIcon(":img/note.png"));
    NotePad *w = NotePad::getNewInstance ();
    if(w)
    {
        w->setMinimumSize (800,600);
        w->show ();

        return a.exec();
    } else
    {
        return 1;
    }
}
