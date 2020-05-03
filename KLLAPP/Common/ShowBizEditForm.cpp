//---------------------------------------------------------------------------

#pragma hdrstop

#include "ShowBizEditForm.h"
//#include "StuffStockInEdit.h"
#include "HouseReserveEdit.h"
#include "WorkReportEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TForm* __fastcall ShowBizEditForm(TComponent* Owner,int modulecode,String Param,int Flag)
{
  switch(modulecode)
  {
	  case 1200302:  //����Ԥ��
	  {
		THouseReserveEditForm *p=new THouseReserveEditForm(Owner,GClientBroker,modulecode,Param,Flag);
		return p;
	  }
	  break;
	  case 1240410:  //�ձ��༭
	  {
		TfrmWorkReportEdit *p=new TfrmWorkReportEdit(Owner,GClientBroker,modulecode,Param,Flag,4);
		return p;
	  }
	  break;
	  case 1240409:  //�ܱ��༭
	  {
		TfrmWorkReportEdit *p=new TfrmWorkReportEdit(Owner,GClientBroker,modulecode,Param,Flag,0);
		return p;
	  }
	  break;
	  case 1240411:  //�±��༭
	  {
		TfrmWorkReportEdit *p=new TfrmWorkReportEdit(Owner,GClientBroker,modulecode,Param,Flag,1);
		return p;
	  }
	  break;
	  default:
	  return NULL;// throw Exception(L"�˵���:"+IntToStr(modulecode)+L"��־:"+IntToStr(Flag)+L"����:"+Param);
  }

}
