#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#ifndef DLG_H
#define DLG_H

#include<QDialog>
class Dlg : public QDialog
{
public:
    Dlg(QWidget * parent = nullptr);
    ~Dlg();
    void run();
protected:
    virtual bool initial() =0;
    virtual  bool setContent() = 0 ;

};

#endif // DLG_H
