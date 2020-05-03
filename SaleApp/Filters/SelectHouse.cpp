//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "SelectHouse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseFilters"
#pragma resource "*.fmx"
TSelectHouseForm *SelectHouseForm;
//---------------------------------------------------------------------------
__fastcall TSelectHouseForm::TSelectHouseForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
	: TBaseFiltersForm(Owner,AOnFilteredDataSet,ABroker)
{
}
//---------------------------------------------------------------------------
String __fastcall TSelectHouseForm::BuildFilters()
{
  if(lvHouseList->ItemIndex>-1)
   return lvHouseList->Selected->ButtonText;
  else
	 throw Exception(L"请选中房源记录");
}
String __fastcall TSelectHouseForm::GetFilters()
{
  String custFilters="";
  if(cbHsgFKID_Est->ItemIndex>0)  //楼盘
  {
	custFilters="HsgFKID_Est="+QuotedStr(cbHsgFKID_Est->Selected->TagString);
  }
  if(cbHsgFKID_Bdg->ItemIndex>0) //楼阁
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+"HsgFKID_Bdg="+QuotedStr(cbHsgFKID_Bdg->Selected->TagString);
  }
  if(cbHsgHouseType->ItemIndex>0) //户型
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+"HsgHouseType="+QuotedStr(cbHsgHouseType->Selected->TagString);
  }
  if(cbHsgOrientation->ItemIndex>0) //朝向
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+"HsgOrientation="+QuotedStr(cbHsgOrientation->Selected->TagString);
  }
  if(cbHsgDecoration->ItemIndex>0) //装修
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+"HsgDecoration="+QuotedStr(cbHsgDecoration->Selected->TagString);
  }
  if(cbHsgLandscape->ItemIndex>0) //景观
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+"HsgLandscape="+QuotedStr(cbHsgLandscape->Selected->TagString);
  }
  if(!edHsgNum->Text.Trim().IsEmpty()) //单元号
  {
	if(custFilters>" ")
		custFilters=custFilters+" and ";
	   custFilters=custFilters+"HsgNum like '%"+edHsgNum->Text.Trim()+"%'";
  }
  if(cbHsgSellArea->ItemIndex>0) //面积
  {
   if(custFilters>" ")
		custFilters=custFilters+" and ";
	custFilters=custFilters+GetSellAreaFilter();
  }
  return custFilters;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitControl()
{
	InitEstNameComboBox();
	InitBdgNameComboBox();
	InitHouseTypeComboBox();
	InitOrientationComboBox();
	InitDecorationComboBox();
	InitLandscapeComboBox();
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitEstNameComboBox()
{
  cbHsgFKID_Est->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgFKID_Est);
  newBoxItem->Text=L"所有楼盘";
  newBoxItem->TagString="0"; //楼盘ID
  cbHsgFKID_Est->AddObject(newBoxItem);
   Query->Close();
   Query->SQL->Text="select EstatePKID,EstateCode,EstateName from tbEstate order by EstateCode";
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgFKID_Est);
		newBoxItem->Text=Query->FieldByName("EstateName")->AsString;
		newBoxItem->TagString=Query->FieldByName("EstatePKID")->AsString;
		cbHsgFKID_Est->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbHsgFKID_Est->Count>0)
   cbHsgFKID_Est->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitBdgNameComboBox()
{
  cbHsgFKID_Bdg->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgFKID_Bdg);
  newBoxItem->Text=L"所有楼阁";
  newBoxItem->TagString="0"; //ID
  cbHsgFKID_Bdg->AddObject(newBoxItem);
  if(cbHsgFKID_Est->ItemIndex>0) //有选中某楼盘
  {
   Query->Close();
   Query->SQL->Text="select BdgPKID,BdgNum,BdgName from tbBuilding where BdgFKID_Est="+QuotedStr(cbHsgFKID_Est->Selected->TagString);
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgFKID_Bdg);
		newBoxItem->Text=Query->FieldByName("BdgName")->AsString;
		newBoxItem->TagString=Query->FieldByName("BdgPKID")->AsString;
		cbHsgFKID_Bdg->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  }
  if(cbHsgFKID_Bdg->Count>0)
   cbHsgFKID_Bdg->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::cbHsgFKID_EstClosePopup(TObject *Sender)
{
   InitBdgNameComboBox();
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitHouseTypeComboBox()
{
  cbHsgHouseType->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgHouseType);
  newBoxItem->Text=L"所有户型";
  newBoxItem->TagString="0";
  cbHsgHouseType->AddObject(newBoxItem);
   Query->Close();
   Query->SQL->Text="select HsgHTPKID,HsgHTRemark from tbHsg_HouseType";
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgHouseType);
		newBoxItem->Text=Query->FieldByName("HsgHTRemark")->AsString;
		newBoxItem->TagString=Query->FieldByName("HsgHTPKID")->AsString;
		cbHsgHouseType->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbHsgHouseType->Count>0)
   cbHsgHouseType->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitOrientationComboBox()
{
  cbHsgOrientation->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgOrientation);
  newBoxItem->Text=L"所有朝向";
  newBoxItem->TagString="0";
  cbHsgOrientation->AddObject(newBoxItem);
   Query->Close();
   Query->SQL->Text="select HsgOnPKID,HsgOnRemark from tbHsg_Orientation";
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgOrientation);
		newBoxItem->Text=Query->FieldByName("HsgOnRemark")->AsString;
		newBoxItem->TagString=Query->FieldByName("HsgOnPKID")->AsString;
		cbHsgOrientation->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbHsgOrientation->Count>0)
   cbHsgOrientation->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitDecorationComboBox()
{
  cbHsgDecoration->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgDecoration);
  newBoxItem->Text=L"所有装修";
  newBoxItem->TagString="0";
  cbHsgDecoration->AddObject(newBoxItem);
   Query->Close();
   Query->SQL->Text="select HsgDnPKID,HsgDnRemark from tbHsg_Decoration";
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgDecoration);
		newBoxItem->Text=Query->FieldByName("HsgDnRemark")->AsString;
		newBoxItem->TagString=Query->FieldByName("HsgDnPKID")->AsString;
		cbHsgDecoration->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbHsgDecoration->Count>0)
   cbHsgDecoration->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::InitLandscapeComboBox()
{
  cbHsgLandscape->Clear();
  TListBoxItem * newBoxItem=new TListBoxItem(cbHsgLandscape);
  newBoxItem->Text=L"所有景观";
  newBoxItem->TagString="0";
  cbHsgLandscape->AddObject(newBoxItem);
   Query->Close();
   Query->SQL->Text="select HsgLePKID,HsgLeRemark from tbHsg_Landscape";
   Query->Open();
   Query->First();
   while(!Query->Eof)
   {
		newBoxItem=new TListBoxItem(cbHsgLandscape);
		newBoxItem->Text=Query->FieldByName("HsgLeRemark")->AsString;
		newBoxItem->TagString=Query->FieldByName("HsgLePKID")->AsString;
		cbHsgLandscape->AddObject(newBoxItem);
		Query->Next();
   }
   Query->Close();
  if(cbHsgLandscape->Count>0)
   cbHsgLandscape->ItemIndex=0;
}
//---------------------------------------------------------------------------
String __fastcall TSelectHouseForm::GetSellAreaFilter()
{
  String AreaFilters="";
  switch(cbHsgSellArea->ItemIndex)
  {
	case 1:
		AreaFilters="HsgSellArea<=100";
		break;
	case 2:
		AreaFilters="HsgSellArea>100 and HsgSellArea<=200";
		break;
	case 3:
		AreaFilters="HsgSellArea>200 and HsgSellArea<=300";
		break;
	case 4:
		AreaFilters="HsgSellArea>300";
		break;
	default:
		AreaFilters="";
  }
  return AreaFilters;
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::SearchEditButton1Click(TObject *Sender)
{
  FillHouseList();
}
//---------------------------------------------------------------------------
void __fastcall TSelectHouseForm::FillHouseList()
{
   String SQLStr="select tbHousing.*,"
					"isnull(EstateName,'') as EstateName,isnull(BdgName,'') as BdgName,"
					"isnull(HsgFnRemark,'') as HsgFnRemark,isnull(HsgHTRemark,'') as HsgHTRemark,"
					"isnull(HsgOnRemark,'') as HsgOnRemark,isnull(HsgLeRemark,'') as HsgLeRemark,"
					"isnull(HsgDnRemark,'') as HsgDnRemark from tbHousing "
					"left join tbEstate on tbHousing.HsgFKID_Est=tbEstate.EstatePKID "
					"left join tbBuilding on tbHousing.HsgFKID_Bdg=tbBuilding.BdgPKID "
					"left join tbHsg_Function on tbHousing.HsgFunction=tbHsg_Function.HsgFnPKID "
					"left join tbHsg_HouseType on tbHousing.HsgHouseType=tbHsg_HouseType.HsgHTPKID "
					"left join tbHsg_Orientation on tbHousing.HsgOrientation=tbHsg_Orientation.HsgOnPKID "
					"left join tbHsg_Landscape on tbHousing.HsgLandscape=tbHsg_Landscape.HsgLePKID "
					"left join tbHsg_Decoration on tbHousing.HsgDecoration=tbHsg_Decoration.HsgDnPKID where HsgState=1 and "+
					GetFilters();
  try{
	  lvHouseList->BeginUpdate();
	  lvHouseList->Items->Clear();
	  Query->Close();
	  Query->SQL->Text=SQLStr;
	  Query->Open();
	  while(!Query->Eof)
	  {
		 TListViewItem* vItem=lvHouseList->Items->Add();
		 vItem->ButtonText=Query->FieldByName("HsgPKID")->AsString;
		 vItem->Text=Query->FieldByName("EstateName")->AsString+
		 		 					+"-"+Query->FieldByName("BdgName")->AsString
		 							+"-"+Query->FieldByName("HsgNum")->AsString
									+L"面积:"+Query->FieldByName("HsgFloorSpace")->AsString;
									//+L"总价:"+Query->FieldByName("HsgSellTotal")->AsString;
		 vItem->Detail=L"功能:"+Query->FieldByName("HsgFnRemark")->AsString
		 							+L"户型:"+Query->FieldByName("HsgHTRemark")->AsString
									+L" 朝向:"+Query->FieldByName("HsgOnRemark")->AsString
		 							+L" 景观:"+Query->FieldByName("HsgLeRemark")->AsString;
								   //	+L"装修:"+Query->FieldByName("HsgDnRemark")->AsString;
		 Query->Next();
	  }
  }
  __finally
  {
   lvHouseList->EndUpdate();
  }
}

