//---------------------------------------------------------------------------

#pragma hdrstop

#include "ShowBizEditForm.h"
#include "StuffStockInEdit.h"
//#include "HouseReserveEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TForm* __fastcall ShowBizEditForm(TComponent* Owner,int modulecode,String Param,int Flag)
{
  switch(modulecode)
  {
	  case 1050208:
	  {
		TfrmStuffStockInEdit *p=new TfrmStuffStockInEdit(Owner,GClientBroker,modulecode,Param,Flag);
		return p;
	  }
	  break;
//	  case 1200302:  //����Ԥ��
//	  {
//		THouseReserveEditForm *p=new THouseReserveEditForm(Owner,GClientBroker,modulecode,Param,Flag);
//		return p;
//	  }
	  break;
	  default:
	  return NULL;// throw Exception(L"�˵���:"+IntToStr(modulecode)+L"��־:"+IntToStr(Flag)+L"����:"+Param);
  }
}
