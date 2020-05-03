//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Customer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseListShow"
#pragma resource "*.fmx"
TfrmCustomer *frmCustomer;
//---------------------------------------------------------------------------
__fastcall TfrmCustomer::TfrmCustomer(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,
							 String WhereStr)
	: TfrmBaseListShow(Owner, clBroker,ModuleCode,WhereStr)
{
	 GridBindSourceDB->DataSet=this->DataSet->MasterDataSet;
	 GridBindSourceDB->DataSource=this->MasterDataSource;
	 GetSalesType();
	 FiltersForm=NULL;
	 EditForm=NULL;
}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmCustomer::NewEditForm(TZClientDataSet *FDataSet,int status)
{
 if(EditForm==NULL)
	EditForm=new TCustomerEditForm(this,ClientBroker,this->FormModuleCode,FDataSet,status);
  return EditForm;
}
void __fastcall TfrmCustomer::FormFillData()
{
	 BindStringGridLink->Active=false;
	 BindStringGridLink->Active=true;
}
bool __fastcall TfrmCustomer::BeforeUpdateData(int OptType)
{
   bool canModife=IsSalesManager||(VarToStr(GetFieldValue("ClientSalerFKID"))== ClientBroker->UserEmpID);
	 switch(OptType)
   {
	  case 1:   //Add
         return true;
	  case 2:  //Edit
         return canModife;
	  case 3:  //Del
	  {
	   if(IsSalesManager)
		return true;
	   else
	   {
			  ShowMessage(_D("���������ܣ�����ɾ���ͻ�����"));
			  return false;
	   }
	  }
	  default:
          return true;
   }

}
TForm * __fastcall TfrmCustomer::NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
{
   if(FiltersForm==NULL)
		FiltersForm=new TCustomerFiltersForm(this,AOnFilteredDataSet,ABroker);
   return FiltersForm;
}
void __fastcall TfrmCustomer::SetUserCtrlStatus()
{
  //ֻ�����ݱ��˻������������ܲ����޸�����
  bool canModife=IsSalesManager||(VarToStr(GetFieldValue("ClientSalerFKID"))== ClientBroker->UserEmpID);
  bool bWriteRight=OperateRight.Pos(FR_SYSTEMWRITE)>0;
  bool bBrowse=MasterStatus == dsBrowse;
  DelAction->Enabled= bWriteRight && canModife && bBrowse;
  EditAction->Enabled= bWriteRight && canModife && !bBrowse;
}
void __fastcall TfrmCustomer::GetSalesType()
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
