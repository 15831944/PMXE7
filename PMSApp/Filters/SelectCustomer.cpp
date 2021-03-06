//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "SelectCustomer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFilters"
#pragma resource "*.fmx"
TSelectCustomerForm *SelectCustomerForm;
//---------------------------------------------------------------------------
__fastcall TSelectCustomerForm::TSelectCustomerForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
	: TBaseFiltersForm(Owner,AOnFilteredDataSet,ABroker)
{
}
//---------------------------------------------------------------------------
String __fastcall TSelectCustomerForm::BuildFilters()
{
   if(lvCustomerList->ItemIndex>-1)
   return lvCustomerList->Selected->ButtonText;
   else
	 throw Exception(L"请选中客户");
}
//---------------------------------------------------------------------------
void __fastcall TSelectCustomerForm::InitControl()
{
   cboxFilterType->Clear();
   cboxFilterType->Items->Add(L"客户名称");
   cboxFilterType->Items->Add(L"联系电话");
   cboxFilterType->Items->Add(L"会员卡号");
   cboxFilterType->Items->Add(L"证件号");
   cboxFilterType->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectCustomerForm::FillCustomerList()
{
 	  String SQLStr="select * from tbClient where ";
	  String SexText="";
	  switch(cboxFilterType->ItemIndex)
	  {
		  case 0:
			 SQLStr=SQLStr+"ClientTheName like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
		  case 1:
			SQLStr=SQLStr+"ClientPhoneNum like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
		  case 2:
			 SQLStr=SQLStr+"ClientCardNum like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
		  case 3:
			 SQLStr=SQLStr+"ClientPaperName like '%"+edFilterValue->Text.Trim()+"%'";
		  break;
	  }
  try{
	  lvCustomerList->BeginUpdate();
	  lvCustomerList->Items->Clear();
	  Query->Close();
	  Query->SQL->Text=SQLStr;
	  Query->Open();
	  while(!Query->Eof)
	  {
		 TListViewItem* vItem=lvCustomerList->Items->Add();
		 int iSex=Query->FieldByName("ClientSex")->AsInteger;
		  if(iSex==1)
		  {
			   SexText=L"(男)";
		  }
		  else if(iSex==2)
		  {
				SexText=L"(女)";
		  }
		  else
		  {
				SexText="";
		  }
		 vItem->ButtonText=Query->FieldByName("ClientPKID")->AsString;
		 vItem->Text=Query->FieldByName("ClientTheName")->AsString+SexText+L"电话:"+Query->FieldByName("ClientPhoneNum")->AsString;
		 vItem->Detail=L"证件:"+Query->FieldByName("ClientPaperName")->AsString+L"卡号:"+Query->FieldByName("ClientCardNum")->AsString;
		 Query->Next();
	  }
  }
  __finally
  {
   lvCustomerList->EndUpdate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TSelectCustomerForm::SearchEditButton1Click(TObject *Sender)
{
	FillCustomerList();
}
//---------------------------------------------------------------------------

