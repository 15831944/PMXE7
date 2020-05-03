//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "SelectPeople.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFilters"
#pragma resource "*.fmx"
TSelectPeopleForm *SelectPeopleForm;
//---------------------------------------------------------------------------
__fastcall TSelectPeopleForm::TSelectPeopleForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
	: TBaseFiltersForm(Owner,AOnFilteredDataSet,ABroker)
{
}
//---------------------------------------------------------------------------
String __fastcall TSelectPeopleForm::BuildFilters()
{
   if(lvPeopleList->ItemIndex>-1)
   return lvPeopleList->Selected->ButtonText;
   else
	 throw Exception(L"��ѡ�м�¼");
}
//---------------------------------------------------------------------------
void __fastcall TSelectPeopleForm::InitControl()
{
   cboxFilterType->Clear();
   cboxFilterType->Items->Add(L"Ա������");
   cboxFilterType->Items->Add(L"��ϵ�绰");
   cboxFilterType->Items->Add(L"Ա�����");
   cboxFilterType->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectPeopleForm::FillPeopleList()
{
	String SQLStr=" Select tbEmployeeData.*,CorpOgnDeptName, "
				   " EmpDutyFKIDName=Isnull((select NewDutyName from tbNewDuty where NewDutyPKID=EmpDutyFKID),'') "
                   "  from tbEmployeeData left join tbCorporationOrganize  on EmpDeptCodeFKID=CorpOgnPKID"
				   " where ";
	  switch(cboxFilterType->ItemIndex)
	  {
		  case 0:
			 SQLStr=SQLStr+"EmpName like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
		  case 1:
			SQLStr=SQLStr+"EmpContactTel like '%"+edFilterValue->Text.Trim()+"%' or ";
			SQLStr=SQLStr+"EmpMobile like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
		  case 2:
			 SQLStr=SQLStr+"EmpNum like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
	  }
  try{
  	  String SexText="";
	  lvPeopleList->BeginUpdate();
	  lvPeopleList->Items->Clear();
	  Query->Close();
	  Query->SQL->Text=SQLStr;
	  Query->Open();
	  while(!Query->Eof)
	  {
		 TListViewItem* vItem=lvPeopleList->Items->Add();
		 int iSex=Query->FieldByName("EmpSex")->AsInteger;
		  if(iSex==1)
		  {
			   SexText=L"(��)";
		  }
		  else if(iSex==2)
		  {
				SexText=L"(Ů)";
		  }
		  else
		  {
				SexText="";
		  }
		 vItem->ButtonText=Query->FieldByName("EmpPKID")->AsString;
		 vItem->Text=Query->FieldByName("EmpName")->AsString+SexText
					+L"�ֻ�:"+Query->FieldByName("EmpMobile")->AsString;
		 vItem->Detail=Query->FieldByName("CorpOgnDeptName")->AsString
					+L" ����:"+Query->FieldByName("EmpNum")->AsString
					+L" �绰:"+Query->FieldByName("EmpContactTel")->AsString;
		 Query->Next();
	  }
  }
  __finally
  {
   lvPeopleList->EndUpdate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TSelectPeopleForm::SearchEditButton1Click(TObject *Sender)
{
    FillPeopleList();
}
//---------------------------------------------------------------------------
