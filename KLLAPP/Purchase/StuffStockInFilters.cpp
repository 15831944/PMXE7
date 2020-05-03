//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "StuffStockInFilters.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFilters"
#pragma link "StartAndEndDate"
#pragma resource "*.fmx"
TStuffStockInFiltersForm *StuffStockInFiltersForm;
//---------------------------------------------------------------------------
__fastcall TStuffStockInFiltersForm::TStuffStockInFiltersForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker,int Flag)
	: TBaseFiltersForm(Owner,AOnFilteredDataSet,ABroker)
{
	  FFlag=Flag;
}
//---------------------------------------------------------------------------
String __fastcall TStuffStockInFiltersForm::BuildFilters()
{
  String ClientFilter="";
  if(cbStore->ItemIndex>0)
  {
	  if (FFlag == 1)
	  {
		 ClientFilter = " StuSkInFKID_Rep<>''  and StuSkInFKID_Dep="+QuotedStr(cbStore->Selected->TagString);
	  }
	  else
	  {
		 ClientFilter = " StuSkInFKID_Rep=''  and StuSkInFKID_Dep="+QuotedStr(cbStore->Selected->TagString);
	  }
  }
  else
  {
	  String AllStorFilters="";
	  for(int i=1;i<cbStore->Items->Count;i++)
	  {
		AllStorFilters=AllStorFilters+"StuSkInFKID_Dep="+QuotedStr(cbStore->ListItems[i]->TagString)+" or ";
	  }
	  if(AllStorFilters.Length()>10) //" or "
	   AllStorFilters=AllStorFilters.SubString0(0,AllStorFilters.Length()-4);
	  if (FFlag == 1)
	  {
		 if(AllStorFilters.Length()>0)
		 ClientFilter = " StuSkInFKID_Rep<>''  and ("+AllStorFilters+")";
		 else
		  ClientFilter = " StuSkInFKID_Rep<>'' " ;
	  }
	  else
	  {
		 if(AllStorFilters.Length()>0)
		  ClientFilter = " StuSkInFKID_Rep=''  and ("+AllStorFilters+")";
		 else
		   ClientFilter = " StuSkInFKID_Rep='' ";
	  }
  }
  if(cbBoxState->ItemIndex>0)
  {
	if(ClientFilter>"")
	ClientFilter=ClientFilter+" and ";
	ClientFilter=ClientFilter+"StuSkInChkState="+IntToStr(cbBoxState->ItemIndex-1);
  }
  if(!edStuSkInSupplier->Text.Trim().IsEmpty())
  {
	if(ClientFilter>"")
	ClientFilter=ClientFilter+" and ";
		ClientFilter=ClientFilter+"CorpName like '%"+edStuSkInSupplier->Text.Trim()+"%'";
  }
	if(!edStuSkInFKID_Cont->Text.Trim().IsEmpty())
  {
	if(ClientFilter>"")
	ClientFilter=ClientFilter+" and ";
		ClientFilter=ClientFilter+"ContNumber like '%"+edStuSkInFKID_Cont->Text.Trim()+"%'";
  }
	if(!edStuSkInNum->Text.Trim().IsEmpty())
  {
	if(ClientFilter>"")
	ClientFilter=ClientFilter+" and ";
		ClientFilter=ClientFilter+"StuSkInNum like '%"+edStuSkInNum->Text.Trim()+"%'";
  }
  if(BillDateExpander->IsChecked)
  {
	  if(ClientFilter>"")
		ClientFilter=ClientFilter+" and ";
	  ClientFilter=ClientFilter+"StuSkInDate>='"+deStuSkInDate->BeginDate
					+"' and StuSkInDate<='" +deStuSkInDate->EndDate+"'";
  }
  return ClientFilter;
}
//---------------------------------------------------------------------------
void __fastcall TStuffStockInFiltersForm::InitControl()
{
   FillStoreComBox();
   deStuSkInDate->cbDateRange->ItemIndex=0;

}
//---------------------------------------------------------------------------
void __fastcall TStuffStockInFiltersForm::FillStoreComBox()
{
  cbStore->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbStore);
  newBoxItem->Text=L"���вֿ�";
  newBoxItem->TagString="0";
  cbStore->AddObject(newBoxItem);
  String tDataString = GetDataRightString(Query,BtProject);
  String sSql=" select StuDepPKID,StuDepName,StuDepFKID_Prj,StuDepFKID_Cont,PrjAmender=isnull(PrjAmender,''),PrjFKID_StartCyc=isnull(PrjFKID_StartCyc,''),\
  ContCreator=isnull(ContCreator,''),PrjPKID,ContPKID,ContStmtChangeNum=isnull(ContStmtChangeNum,'') from tbStuffDepot \
  left join tbContract on StuDepFKID_Cont=ContPKID \
  left join tbProject on StuDepFKID_Prj=PrjPKID \
  join tbContType on CTypePKID = ContType\
  where " + tDataString;
  sSql+=" order by StuDepName";
  Query->Close();
  Query->SQL->Text=sSql;
  Query->Open();
  Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbStore);
		newBoxItem->Text=Query->FieldByName("StuDepName")->AsString;
		newBoxItem->TagString=Query->FieldByName("StuDepPKID")->AsString;
		cbStore->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbStore->Count>0)
   cbStore->ItemIndex=0;
}
//---------------------------------------------------------------------------


