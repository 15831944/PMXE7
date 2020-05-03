//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "HouseReserve.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseListShow"
#pragma resource "*.fmx"
TfrmHouseReserve *frmHouseReserve;
//---------------------------------------------------------------------------
__fastcall TfrmHouseReserve::TfrmHouseReserve(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,
							 String WhereStr)
	: TfrmBaseListShow(Owner,clBroker,ModuleCode,WhereStr)
{
	 GridBindSourceDB->DataSet=this->DataSet->MasterDataSet;
	 GridBindSourceDB->DataSource=this->MasterDataSource;
	 GetSalesType();
	 FiltersForm=NULL;
	 EditForm=NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TForm * __fastcall TfrmHouseReserve::NewEditForm(TZClientDataSet *FDataSet,int status)
{
 if(EditForm==NULL)
	EditForm=new THouseReserveEditForm(this,ClientBroker,this->FormModuleCode,FDataSet,status);
  return EditForm;
}
//---------------------------------------------------------------------------
void __fastcall TfrmHouseReserve::FormFillData()
{
	 BindStringGridLink->Active=false;
	 BindStringGridLink->Active=true;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmHouseReserve::BeforeUpdateData(int OptType)
{
   int UnitChgPriceState;
   bool canModife=(IsSalesManager||(VarToStr(GetFieldValue("ClientSalerFKID"))== ClientBroker->UserEmpID))&&(GetFieldValue("HseRsveProperty")==1);
   Query->Close();
   Query->SQL->Text=String("select * from tbSysParameter where SysParamClassCode='110'")
             +" and SysParamPKID='UnitChgPriceState'";
   Query->Open();
   if(Query->RecordCount>0)
   {
     if(Query->FieldValues["SysParamValue"]==0)
	   UnitChgPriceState = 0 ;
     else
	   UnitChgPriceState = 1 ;
   }
   else
     UnitChgPriceState = 0 ;
   Query->Close();
  switch(OptType)
  {
    case 1:
      if(UnitChgPriceState == 1)
      {
	   ShowMessage(_D("���ڵ�Ԫ����״̬����������"));
        return false;
      }
      return true;
    case 2:
	  if(UnitChgPriceState == 1)
      {
	   ShowMessage(_D("���ڵ�Ԫ����״̬�������޸�"));
        return false;
      }
	  return canModife;
	case 3:
	  if(UnitChgPriceState == 1)
	  {
	   ShowMessage(_D("���ڵ�Ԫ����״̬������ɾ��"));
		return false;
	  }
	  return canModife;
    default :
      return true;
  }

}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmHouseReserve::NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
{
   if(FiltersForm==NULL)
   		FiltersForm=new THouseReserveFiltersForm(this,AOnFilteredDataSet,ABroker);
   return FiltersForm;
}
//---------------------------------------------------------------------------
void __fastcall TfrmHouseReserve::SetUserCtrlStatus()
{
  //ֻ�����ݱ��˻������������ܲ����޸�����
  bool canModife=IsSalesManager||(VarToStr(GetFieldValue("ClientSalerFKID"))== ClientBroker->UserEmpID);
  bool bWriteRight=OperateRight.Pos(FR_SYSTEMWRITE)>0;
  bool bBrowse=MasterStatus == dsBrowse;
  DelAction->Enabled= bWriteRight && canModife && bBrowse;
  EditAction->Enabled= bWriteRight && canModife && !bBrowse;
}
//---------------------------------------------------------------------------
void __fastcall TfrmHouseReserve::GetSalesType()
{
	   IsSalesManager=false;
	   Query->Close();
	   Query->SQL->Text="select EmpSalesPs from tbEmployeeData where EmpPKID='"+ClientBroker->UserEmpID+"'";
	   Query->Open();
	   if(Query->RecordCount>0)
	   {
		   IsSalesManager=Query->FieldByName("EmpSalesPs")->AsInteger==2;
	   }
	   Query->Close();
}
//---------------------------------------------------------------------------