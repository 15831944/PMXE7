//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CopyRight.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

void  checkCopyRight()
{
 #ifdef _TRIAL_VERSION
   if(Date().FormatString("yyyy-mm-dd")>="2009-10-01")
   {
		throw Exception("���ð��ѹ���,��ʹ����ʽ��!");
   }
 #endif
}








