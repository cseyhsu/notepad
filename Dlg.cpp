#if MSC_VER >=1000
#pragma execution_character_set("UTF-8")
#endif
#include "Dlg.h"

Dlg::Dlg(QWidget * parent):QDialog(parent)
{

}

Dlg::~Dlg()
{

}

void Dlg::run()
{
 initial ();
 setContent();
}
