//---------------------------------------------------------------------------

#pragma hdrstop
#include "ShowBizEditForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TForm* __fastcall ShowBizEditForm(TComponent* Owner,int modulecode,String Param,int Flag)
{
	switch(modulecode)
	{
		default:
		return NULL;// throw Exception(L"�˵���:"+IntToStr(modulecode)+L"��־:"+IntToStr(Flag)+L"����:"+Param);
	}
}
//---------------------------------------------------------------------------