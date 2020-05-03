//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "CustomerEdit.h"
#include "ModuleEnum.h"
#include "CommFunc.h"
#include "FSTMessageDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DetailBaseForm"
#pragma resource "*.fmx"
TCustomerEditForm *CustomerEditForm;
//---------------------------------------------------------------------------
__fastcall TCustomerEditForm::TCustomerEditForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet
								*FDataSet,int DataSetStatus)
	: TfrmDetailBaseForm(Owner,clBroker,ModuleCode,FDataSet, DataSetStatus)
{
	GetSalesType();
	SelectPeople=NULL;
	CientClassType=NULL;
	ClientContract=NULL;
	ClientLevel=NULL;

	ClientResidence=NULL;
	ClientEducation=NULL;
	ClientWork=NULL;
	ClientProp1=NULL;
	ClientProp2=NULL;
	ClientProp3=NULL;
	ClientProp4=NULL;
	ClientProp5=NULL;
	ClientProp6=NULL;
	ClientProp7=NULL;
	ClientProp8=NULL;
	ClientProp9=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::FormInitControl()
{
	 //浏览,新增,修改 ,true-可以用，false-锁住
	if(FormDefRight.Pos(FR_FRMCLIENT_USERNAMECHANGE) <= 0)//修改用户名称权限
	{
		LockControl(1,seClientLevel,false,true,false);
		LockControl(1,seClientTheName,false,true,false);
		LockControl(1,seClientSalerFKID,false,true,false);
		LockControl(1,seClientPhoneNum,false,true,false,1);
		LockControl(1,seClientPaperName,false,true,false);
	}
	else
	{
		LockControl(1,seClientLevel,false,true,true);
		LockControl(1,seClientTheName,false,true,true);
		LockControl(1,seClientSalerFKID,false,true,true);
		LockControl(1,seClientPhoneNum,false,true,true,1);
		LockControl(1,seClientPaperName,false,true,true);
	}
	if(FormDefRight.Pos(FR_FRMCLIENT_VIPCHANGE) <= 0)    //会员管理权限
	{
		LockControl(1,seClientCardNum,false,false,false);
		LockControl(1,seClientIntegral,false,false,false);
		LockControl(1,seClientAgio,false,false,false);
		LockControl(1,seClientClubLevel,false,false,false);
	}
	else
	{
		LockControl(1,seClientCardNum,false,true,true);
		LockControl(1,seClientIntegral,false,true,true);
		LockControl(1,seClientAgio,false,true,true);
		LockControl(1,seClientClubLevel,false,true,true);
	}

	LockControl(1,scClientIsClub,false,true,true);
	LockControl(1,dteClientJoinClubDate,false,true,true);
	LockControl(1,dteClientQuitClubDate,false,true,true);
	LockControl(1,seClientNum,false,true,true,1);
	LockControl(1,seClientTheFKID_Cls,false,true,true);
	LockControl(1,scClientType,false,false,false);
	LockControl(1,seClientAddress,false,true,true,1);
	LockControl(1,seClientPostCode,false,true,true,1);
	LockControl(1,scClientSex,false,true,true,1);
	LockControl(1,seClientCensusReg,false,true,true,1);
	LockControl(1,dteClientBirthday,false,true,true,1);
	LockControl(1,seClientCompany,false,true,true,1);
	LockControl(1,seClientBankName,false,true,true,1);
	LockControl(1,seClientBankAccount,false,true,true,1);
	LockControl(1,mmoClientRemark,false,true,true,1);
	LockControl(1,seClientWorkPhoneNum,false,true,true,1);
	LockControl(1,seClientMobilePhoneNum,false,true,true,1);
	LockControl(1,seClientFaxNum,false,true,true,1);
	LockControl(1,seClientEmail,false,true,true,1);
	LockControl(1,scClientFirstMeetDate,false,true,true,1);

	//初次联系时间
	LockControl(1,dteClientFirstContactDate,false,true,false);
	LockControl(1,dteClientLastMeetDate,false,true,true);

	LockControl(1,mmoClientDemand,false,true,true,1);
	LockControl(1,mmoClientMeetRecord,false,true,true,1);
	LockControl(1,seClientWork,false,true,true);
	LockControl(1,seClientResidence,false,true,true);
	LockControl(1,seClientContactMethod,false,true,true);
	LockControl(1,seClientEducation,false,true,true);
	LockControl(1,seClientAge,false,true,true,1);
	LockControl(1,seClientTrade,false,true,true);
	LockControl(1,seClientPosition,false,true,true);
	LockControl(1,seClientWorkYears,false,true,true);
	LockControl(1,seClientHouseType,false,true,true);
	LockControl(1,seClientHouseArea,false,true,true);
	LockControl(1,seClientHousePop,false,true,true);
	LockControl(1,seClientTraffic,false,true,true);
	LockControl(1,seClientYearIncome,false,true,true);

	seClientNum->MaxLength=20;
	seClientTheName->MaxLength=100;
	seClientPhoneNum->MaxLength=100;
	seClientPaperName->MaxLength=100;
	seClientMobilePhoneNum->MaxLength=100;
	seClientWorkPhoneNum->MaxLength=100;
	seClientEmail->MaxLength=100;
	seClientCompany->MaxLength=50;
	seClientFaxNum->MaxLength=100;
	seClientCensusReg->MaxLength=80;
	mmoClientDemand->MaxLength=500;
	seClientAge->MaxLength=4;
	seClientCardNum->MaxLength=30;
	seClientBankName->MaxLength=100;
	seClientBankAccount->MaxLength=50;
	mmoClientRemark->MaxLength=255;
	WaitInputControl(1,seClientTheName);

}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::FormClearControl(int Section)
{
    seClientTheName->Text = "";
    seClientNum->Text = "";
	seClientTheFKID_Cls->Text="";
	seClientTheFKID_Cls->TagString="";
	scClientType->ItemIndex = 0;
	seClientPhoneNum->Text = "";
    seClientAddress->Text = "";
    seClientPostCode->Text = "";
    scClientSex->ItemIndex =1;
    seClientPaperName->Text = "";
    seClientCensusReg->Text = "";
	dteClientBirthday->IsEmpty=true;//->Date = Now();
	seClientCompany->Text = "";
    seClientBankName->Text = "";
	seClientBankAccount->Text = "";
	mmoClientRemark->Text = "";
	seClientWorkPhoneNum->Text = "";
    seClientMobilePhoneNum->Text = "";
    seClientFaxNum->Text = "";
    seClientEmail->Text = "";
    scClientFirstMeetDate->ItemIndex = 1;

	dteClientFirstContactDate->Date =Now();
	dteClientLastMeetDate->Date =Now();

    mmoClientDemand->Text = "";
    mmoClientMeetRecord->Text = "";

	seClientWork->TagString="";
	seClientWork->Text = "";
	seClientResidence->TagString="";
    seClientResidence->Text = "";
	seClientContactMethod->TagString="";
    seClientContactMethod->Text = "";
	seClientLevel->TagString="";
    seClientLevel->Text = "";
	seClientEducation->TagString="";
    seClientEducation->Text = "";
    seClientAge->Text = "";
	seClientSalerFKID->TagString="";

    seClientTrade->TagString="";
    seClientTrade->Text = "";
	seClientPosition->TagString="";
    seClientPosition->Text = "";
    seClientWorkYears->TagString="";
    seClientWorkYears->Text = "";
	seClientHouseType->TagString="";
    seClientHouseType->Text = "";
	seClientHouseArea->TagString="";
    seClientHouseArea->Text = "";
	seClientHousePop->TagString="";
	seClientHousePop->Text = "";
	seClientTraffic->TagString="";
    seClientTraffic->Text = "";
	seClientYearIncome->TagString="";
    seClientYearIncome->Text = "";
    seClientCardNum->Text = "";
    seClientIntegral->Text = "0";
    seClientAgio->Text = "0";
	seClientClubLevel->TagString="";
    seClientClubLevel->Text = "";
	seClientSalerFKID->TagString=ClientBroker->UserEmpID;
	seClientSalerFKID->Text=ClientBroker->UserName;

    scClientIsClub->ItemIndex = 0;
	dteClientJoinClubDate->IsEmpty=true;//  Date = Now();
	dteClientQuitClubDate->IsEmpty=true;//->Date = Now();
//	sgSaleFollowUp->RowCount = 1;
//	sgSubscribeBill->RowCount = 1;
//	sgIntegral->RowCount = 1;
//	sgDiscount->RowCount = 1;
//	if(FormDefFunc.Pos(FF_FRMClient_MODIFYRECORD) > 0)
//	  sgModify->RowCount = 1;
	LocateID = GetGUIDString();

}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::SendDataToDataSet(int Section)
{
    if(MasterStatus==dsInsert)
	   SetFieldValue("ClientPKID",LocateID);
    SetFieldValue("ClientTheName",seClientTheName->Text);
	SetFieldValue("ClientNum",seClientNum->Text);
    SetFieldValue("TypeName",seClientTheFKID_Cls->Text);
	SetFieldValue("ClientTheFKID_Cls",seClientTheFKID_Cls->TagString);
    SetFieldValue("ClientType",scClientType->ItemIndex);
	SetFieldValue("ClientPhoneNum",seClientPhoneNum->Text);
    SetFieldValue("ClientAddress",seClientAddress->Text);
	SetFieldValue("ClientPostCode",seClientPostCode->Text);
	// ShowMessage("OK1");
	SetFieldValue("ClientSex",scClientSex->ItemIndex);
    SetFieldValue("ClientPaperName",seClientPaperName->Text);
	SetFieldValue("ClientCensusReg",seClientCensusReg->Text);
	if(dteClientBirthday->IsEmpty)
	  SetFieldValue("ClientBirthday","");
	else
	   SetFieldValue("ClientBirthday",dteClientBirthday->Date.FormatString("YYYY-MM-DD"));
	//	ShowMessage("OK2");
	SetFieldValue("ClientCompany",seClientCompany->Text);
	SetFieldValue("ClientBankName",seClientBankName->Text);
	SetFieldValue("ClientBankAccount",seClientBankAccount->Text);
	SetFieldValue("ClientRemark",mmoClientRemark->Text);
	SetFieldValue("ClientAmendDate",Now().FormatString("YYYY-MM-DD"));
	SetFieldValue("ClientAmender",ClientBroker->UserCode);
	SetFieldValue("ClientWorkPhoneNum",seClientWorkPhoneNum->Text);
    SetFieldValue("ClientMobilePhoneNum",seClientMobilePhoneNum->Text);
	SetFieldValue("ClientFaxNum",seClientFaxNum->Text);
	SetFieldValue("ClientEmail",seClientEmail->Text);
	// ShowMessage("OK3");
//	String selval=scClientFirstMeetDate->Items->Strings[scClientFirstMeetDate->ItemIndex];
	SetFieldValue("ClientFirstMeetDate",scClientFirstMeetDate->Items->Strings[scClientFirstMeetDate->ItemIndex]);

	SetFieldValue("ClientFirstContactDate",dteClientFirstContactDate->Date.FormatString("YYYY-MM-DD"));
	SetFieldValue("ClientLastMeetDate",dteClientLastMeetDate->Date.FormatString("YYYY-MM-DD"));
   //  ShowMessage("OK4");
    SetFieldValue("ClientDemand",mmoClientDemand->Text);
	SetFieldValue("ClientMeetRecord",mmoClientMeetRecord->Text);
	SetFieldValue("ClientWorkName",seClientWork->Text);
	SetFieldValue("ClientWork",seClientWork->TagString);
    SetFieldValue("ClientResidenceName",seClientResidence->Text);
	SetFieldValue("ClientResidence",seClientResidence->TagString);
	SetFieldValue("ClientContactMethodName",seClientContactMethod->Text);
    SetFieldValue("ClientContactMethod",seClientContactMethod->TagString);
    SetFieldValue("ClientLevelName",seClientLevel->Text);
	SetFieldValue("ClientLevel",seClientLevel->TagString);
    SetFieldValue("ClientEducationName",seClientEducation->Text);
    SetFieldValue("ClientEducation",seClientEducation->TagString);
    SetFieldValue("ClientAge",seClientAge->Text);
	SetFieldValue("ClientSalerFKID",seClientSalerFKID->TagString);
    SetFieldValue("ClientSalerFKIDName",seClientSalerFKID->Text);
	SetFieldValue("ClientIsClub",scClientIsClub->ItemIndex);
	if(scClientIsClub->ItemIndex==0)
	{
		SetFieldValue("ClientJoinClubDate","");
		SetFieldValue("ClientQuitClubDate","");
	}
	else if(scClientIsClub->ItemIndex==1)
	{
		SetFieldValue("ClientJoinClubDate",dteClientJoinClubDate->Date.FormatString("YYYY-MM-DD"));
		SetFieldValue("ClientQuitClubDate","");
	}
	else
	{
		SetFieldValue("ClientJoinClubDate",dteClientJoinClubDate->Date.FormatString("YYYY-MM-DD"));
		SetFieldValue("ClientQuitClubDate",dteClientQuitClubDate->Date.FormatString("YYYY-MM-DD"));
	}
	SetFieldValue("ClientTradeName",seClientTrade->Text);
    SetFieldValue("ClientTrade",seClientTrade->TagString);
	SetFieldValue("ClientPositionName",seClientPosition->Text);
	SetFieldValue("ClientPosition",seClientPosition->TagString);
	SetFieldValue("ClientWorkYearsName",seClientWorkYears->Text);
	SetFieldValue("ClientWorkYears",seClientWorkYears->TagString);
	SetFieldValue("ClientHouseTypeName",seClientHouseType->Text);
	SetFieldValue("ClientHouseType",seClientHouseType->TagString);
	SetFieldValue("ClientHouseAreaName",seClientHouseArea->Text);
	SetFieldValue("ClientHouseArea",seClientHouseArea->TagString);
	SetFieldValue("ClientHousePopName",seClientHousePop->Text);
	SetFieldValue("ClientHousePop",seClientHousePop->TagString);
	SetFieldValue("ClientTrafficName",seClientTraffic->Text);
	SetFieldValue("ClientTraffic",seClientTraffic->TagString);
	SetFieldValue("ClientYearIncomeName",seClientYearIncome->Text);
	SetFieldValue("ClientYearIncome",seClientYearIncome->TagString);
	SetFieldValue("ClientCardNum",seClientCardNum->Text);
	SetFieldValue("ClientClubLevelName",seClientClubLevel->Text);
	SetFieldValue("ClientClubLevel",seClientClubLevel->TagString);
//	SaveSGIntegralData();
//	SaveSGDiscountData();
//	SaveSGModifyData();
  //	seClientIntegral->Text=GetMaxClientIntegral();
//	seClientAgio->Text=GetMaxClientAgio();
	SetFieldValue("ClientIntegral",seClientIntegral->Text);
	SetFieldValue("ClientAgio",seClientAgio->Text);

}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::GetDataFromDataSet(int Section)
{
	LocateID = GetFieldValue("ClientPKID");
    seClientTheName->Text=GetFieldValue("ClientTheName");
    seClientNum->Text=GetFieldValue("ClientNum");

	seClientTheFKID_Cls->Text=GetFieldValue("TypeName");
	seClientTheFKID_Cls->TagString=GetFieldValue("ClientTheFKID_Cls");
	scClientType->ItemIndex=GetFieldValue("ClientType");

    seClientPhoneNum->Text=GetFieldValue("ClientPhoneNum");
    seClientAddress->Text=GetFieldValue("ClientAddress");
    seClientPostCode->Text=GetFieldValue("ClientPostCode");
    scClientSex->ItemIndex=GetFieldValue("ClientSex");
    seClientPaperName->Text=GetFieldValue("ClientPaperName");
	seClientCensusReg->Text=GetFieldValue("ClientCensusReg");
	String dateStr=GetFieldValue("ClientBirthday");
	if(dateStr.IsEmpty())
	  dteClientBirthday->IsEmpty=true;
	else
	   dteClientBirthday->Date=YMDStrToDate(dateStr);
	seClientCompany->Text=GetFieldValue("ClientCompany");
    seClientBankName->Text=GetFieldValue("ClientBankName");
    seClientBankAccount->Text=GetFieldValue("ClientBankAccount");
    mmoClientRemark->Text=GetFieldValue("ClientRemark");
    seClientWorkPhoneNum->Text=GetFieldValue("ClientWorkPhoneNum");
    seClientMobilePhoneNum->Text=GetFieldValue("ClientMobilePhoneNum");
    seClientFaxNum->Text=GetFieldValue("ClientFaxNum");
    seClientEmail->Text=GetFieldValue("ClientEmail");
//	if(VarToStr(GetFieldValue("ClientFirstMeetDate"))=="未婚")
//	   scClientFirstMeetDate->ItemIndex= 1;
//	else if(VarToStr(GetFieldValue("ClientFirstMeetDate"))=="已婚")
//	   scClientFirstMeetDate->ItemIndex= 2;
//	else
    scClientFirstMeetDate->ItemIndex=scClientFirstMeetDate->Items->IndexOf(GetFieldValue("ClientFirstMeetDate"));
	 dateStr=GetFieldValue("ClientFirstContactDate");
	if(dateStr.IsEmpty())
	  dteClientFirstContactDate->IsEmpty=true;
	else
	   dteClientFirstContactDate->Date=YMDStrToDate(dateStr);
	dateStr=GetFieldValue("ClientLastMeetDate");
	if(dateStr.IsEmpty())
	  dteClientLastMeetDate->IsEmpty=true;
	else
	   dteClientLastMeetDate->Date=YMDStrToDate(dateStr);
    mmoClientDemand->Text=GetFieldValue("ClientDemand");
    mmoClientMeetRecord->Text=GetFieldValue("ClientMeetRecord");

	seClientWork->Text=GetFieldValue("ClientWorkName");
	seClientWork->TagString=GetFieldValue("ClientWork");

	seClientResidence->Text=GetFieldValue("ClientResidenceName");
	seClientResidence->TagString=GetFieldValue("ClientResidence");

	seClientContactMethod->Text=GetFieldValue("ClientContactMethodName");
	seClientContactMethod->TagString=GetFieldValue("ClientContactMethod");

	seClientLevel->Text=GetFieldValue("ClientLevelName");
	seClientLevel->TagString=GetFieldValue("ClientLevel");

	seClientEducation->Text=GetFieldValue("ClientEducationName");
	seClientEducation->TagString=GetFieldValue("ClientEducation");

    seClientAge->Text=GetFieldValue("ClientAge");

	seClientSalerFKID->Text=GetFieldValue("ClientSalerFKIDName");
	seClientSalerFKID->TagString=GetFieldValue("ClientSalerFKID");

	scClientIsClub->ItemIndex=GetFieldValue("ClientIsClub");
	 dateStr=GetFieldValue("ClientJoinClubDate");
	if(dateStr.IsEmpty())
	  dteClientJoinClubDate->IsEmpty=true;
	else
	   dteClientJoinClubDate->Date=YMDStrToDate(dateStr);
	 dateStr=GetFieldValue("ClientQuitClubDate");
	if(dateStr.IsEmpty())
	  dteClientQuitClubDate->IsEmpty=true;
	else
	   dteClientQuitClubDate->Date=YMDStrToDate(dateStr);

	seClientTrade->Text=GetFieldValue("ClientTradeName");
	seClientTrade->TagString=GetFieldValue("ClientTrade");

	seClientPosition->Text=GetFieldValue("ClientPositionName");
	seClientPosition->TagString=GetFieldValue("ClientPosition");

	seClientWorkYears->Text=GetFieldValue("ClientWorkYearsName");
	seClientWorkYears->TagString=GetFieldValue("ClientWorkYears");

	seClientHouseType->Text=GetFieldValue("ClientHouseTypeName");
	seClientHouseType->TagString=GetFieldValue("ClientHouseType");

	seClientHouseArea->Text=GetFieldValue("ClientHouseAreaName");
	seClientHouseArea->TagString=GetFieldValue("ClientHouseArea");

	seClientHousePop->Text=GetFieldValue("ClientHousePopName");
	seClientHousePop->TagString=GetFieldValue("ClientHousePop");

	seClientTraffic->Text=GetFieldValue("ClientTrafficName");
	seClientTraffic->TagString=GetFieldValue("ClientTraffic");

	seClientYearIncome->Text=GetFieldValue("ClientYearIncomeName");
	seClientYearIncome->TagString=GetFieldValue("ClientYearIncome");

    seClientCardNum->Text=GetFieldValue("ClientCardNum");
    seClientIntegral->Text=GetFieldValue("ClientIntegral");
    seClientAgio->Text=GetFieldValue("ClientAgio");

	seClientClubLevel->Text=GetFieldValue("ClientClubLevelName");
	seClientClubLevel->TagString=GetFieldValue("ClientClubLevel");

}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::VerifyFieldData(int Section,int status)
{
  if(seClientTheName->Text.Trim().IsEmpty())
  {
	if(seClientTheName->CanFocus) seClientTheName->SetFocus();
	throw Exception(L"请输入客户名称");
  }
  if(seClientTheFKID_Cls->Text.Trim().IsEmpty())
  {
	if(seClientTheFKID_Cls->CanFocus) seClientTheFKID_Cls->SetFocus();
	throw Exception(L"请输入分类码");
  }

  if((VarToStr(GetFieldValue("ClientPhoneNum"))!=seClientPhoneNum->Text &&  seClientPhoneNum->Text!="") )
  {
    CompFieldData("ClientPhoneNum",seClientPhoneNum->Text);
  }
  if((VarToStr(GetFieldValue("ClientWorkPhoneNum"))!=seClientWorkPhoneNum->Text &&  seClientWorkPhoneNum->Text!="") )
  {
    CompFieldData("ClientWorkPhoneNum",seClientWorkPhoneNum->Text);
  }
  if((VarToStr(GetFieldValue("ClientMobilePhoneNum"))!=seClientMobilePhoneNum->Text &&  seClientMobilePhoneNum->Text!="") )
  {
	CompFieldData("ClientMobilePhoneNum",seClientMobilePhoneNum->Text);
  }
  if((VarToStr(GetFieldValue("ClientFaxNum"))!=seClientFaxNum->Text &&  seClientFaxNum->Text!="") )
  {
    CompFieldData("ClientFaxNum",seClientFaxNum->Text);
  }
  if(seClientPhoneNum->Text=="" &&
     seClientWorkPhoneNum->Text=="" &&
     seClientMobilePhoneNum->Text=="" &&
     seClientFaxNum->Text=="" &&
     (VarToStr(GetFieldValue("ClientTheName"))!=seClientTheName->Text &&  seClientTheName->Text!=""))
  {
    Query->Close();
	Query->SQL->Text="select * from tbClient where ClientTheName="+QuotedStr(seClientTheName->Text);
	Query->Open();
    if(Query->RecordCount>0)
	{
	  OldClientPKID=Query->FieldByName("ClientPKID")->AsString;
	  TMessageDialogEventProc msgDialogCloseProc=&LocateOldeCustomer;
	  ShowMessageBox(L"该客户名称已经存在，是否跟踪查看该客户资料？",msgDialogCloseProc);
//	  if(ShowMessageBox(L"该客户名称已经存在，是否跟踪查看该客户资料？"))
//	  {
//		DS_Close ();
//		MasterWhereString="ClientPKID="+QuotedStr(Query->FieldByName("ClientPKID")->AsString);
//		DS_Open ();
//		 CancelChange();
//		Abort();
//	  }
	}
  }
  if(VarToStr(GetFieldValue("ClientPaperName"))!=seClientPaperName->Text && seClientPaperName->Text!="")
  {
    Query->Close();
	Query->SQL->Text="select * from tbClient where ClientPaperName="+QuotedStr(seClientPaperName->Text);
    Query->Open();
    if(Query->RecordCount>0)
	{
	  OldClientPKID=Query->FieldByName("ClientPKID")->AsString;
	  TMessageDialogEventProc msgDialogCloseProc=&LocateOldeCustomer;
	  ShowMessageBox(L"已经有该客户证件存在了，是否跟踪查看该客户资料？",msgDialogCloseProc);
//	  if(ShowMessageBox(L"已经有该客户证件存在了，是否跟踪查看该客户资料？"))
//	  {
//		DS_Close ();
//		MasterWhereString="ClientPKID="+QuotedStr();
//		DS_Open ();
//		 CancelChange();
//		Abort();
//	  }
    }
    Query->Close();
  }

}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::LocateOldeCustomer(System::Uitypes::TModalResult AResult)
{
	   if(AResult==mrYes)
	   {
	   DS_Close ();
		MasterWhereString="ClientPKID="+QuotedStr(OldClientPKID);
		DS_Open ();
		 CancelChange();
		Abort();
	   }
}
bool __fastcall TCustomerEditForm::BeforeUpdateData(int OptType)
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
			  ShowMessage(_D("非销售主管，不能删除客户资料"));
			  return false;
	   }
	  }
	  default:
          return true;
   }
}
//---------------------------------------------------------------------------
/*
void __fastcall TCustomerEditForm::SetUserCtrlStatus()
{
  //只有数据本人或者是销售主管才能修改数据
  bool canModife=IsSalesManager||(VarToStr(GetFieldValue("ClientSalerFKID"))== ClientBroker->UserEmpID);
  bool bWriteRight=OperateRight.Pos(FR_SYSTEMWRITE)>0;
  bool bBrowse=MasterStatus == dsBrowse;
  DetailEditAction->Enabled= bWriteRight && canModife && bBrowse;
  SaveDetailAction->Enabled= bWriteRight && canModife && !bBrowse;
} */
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::GetSalesType()
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
void __fastcall TCustomerEditForm::CompFieldData(String FieldCode,String FieldValues)
{
  String sSQLstring,SaleManID1,SaleManID2;
  if(seClientSalerFKID->TagString!="")
  {
    sSQLstring="select * from tbClient where ("+FieldCode+"='"+FieldValues+"' and "+FieldCode+"<>'') ";
    Query->Close();
    Query->SQL->Text=sSQLstring;
    Query->Open();
    if(Query->RecordCount>0)
    {
	   SaleManID1=seClientSalerFKID->TagString;
       SaleManID2=Query->FieldValues["ClientSalerFKID"];
       if(SaleManID1==SaleManID2)
	   {
		  OldClientPKID=Query->FieldByName("ClientPKID")->AsString;
		  TMessageDialogEventProc msgDialogCloseProc=&LocateOldeCustomer;
		  ShowMessageBox(L"该客户电话已经存在，是否跟踪查看该客户资料？",msgDialogCloseProc);
//		  if(ShowMessageBox(L"该客户电话已经存在，是否跟踪查看该客户资料？"))
//		  {
//			DS_Close ();
//			MasterWhereString="ClientPKID="+QuotedStr(Query->FieldByName("ClientPKID")->AsString);
//			DS_Open ();
//			CancelChange();
//			Abort();
//		  }
//		  else
//			Abort();
	   }
       else
       {
		 throw Exception(L"该客户电话已经存在，并且该客户已经有所属的置业顾问，请重新输入");
       }
    }
  }
}
//---------------------------------------------------------------------------
double __fastcall TCustomerEditForm::GetMaxClientIntegral()
{
  double dPoint=0;
  String SqlStr="select max(SlMbrSeSumScore) from tbSlMemberScore where  SlMbrSeClientPKID='"+LocateID+"'";
  Query->Close();
  Query->SQL->Text=SqlStr;
  Query->Open();
  if(Query->RecordCount>0)
	dPoint=Query->Fields->Fields[0]->AsFloat;
  return dPoint;
}
//---------------------------------------------------------------------------
double __fastcall TCustomerEditForm::GetMaxClientAgio()
{
  double dPoint=0;
  String SqlStr="select max(SlMbrAigoFinalAigo) from tbSlMemberAgio where  SlMbrAigoPKID_Client='"+LocateID+"'";
  Query->Close();
  Query->SQL->Text=SqlStr;
  Query->Open();
  if(Query->RecordCount>0)
	dPoint=Query->Fields->Fields[0]->AsFloat;
  return dPoint;
}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::ClientAgioEditButtonClick(TObject *Sender)
{
  seClientAgio->Text=GetMaxClientAgio();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientIntegralEditButtonClick(TObject *Sender)

{
	seClientIntegral->Text=GetMaxClientIntegral();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::BaseExpanderResize(TObject *Sender)
{
	EditLayout->Height=BaseExpander->Height+ ContractExpander->Height
						+DetailExpander->Height+VIPExpander->Height
						+ContractRecordExpander->Height;
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientClassEditButtonClick(TObject *Sender)
{
 if(CientClassType==NULL)
 {
	 String FTitle=L"客户分类";
	 String FSQL="SELECT ClientClassPKID,ClientClassName FROM tbClientClass ORDER BY ClientClassName";
	 CientClassType=new TQueryFilterForm(this,ClientBroker,seClientTheFKID_Cls,FTitle,FSQL);
 }
 CientClassType->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientContactMethodEditButtonClick(TObject *Sender)
{
 if(ClientContract==NULL)
 {
	 String FTitle=L"告知途径列表";
	 String FSQL="SELECT ClientContMdPKID,ClientContMdName FROM tbClient_ContactMethod ORDER BY ClientContMdName";
	 ClientContract=new TQueryFilterForm(this,ClientBroker,seClientContactMethod,FTitle,FSQL);
 }
  ClientContract->Show();

}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientLevelEditButtonClick(TObject *Sender)
{
 if(ClientLevel==NULL)
 {
 String FTitle=L"客户级别列表";
 String FSQL="SELECT ClientLvlPKID,ClientLvlName FROM tbClient_Level ORDER BY ClientLvlName";
 ClientLevel=new TQueryFilterForm(this,ClientBroker,seClientLevel,FTitle,FSQL);
 }
 ClientLevel->Show();

}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientPositionEditButtonClick(TObject *Sender)

{
 if(ClientProp2==NULL)
 {
 String FTitle=L"职务资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=2 ORDER BY ClientPropName";
 ClientProp2=new TQueryFilterForm(this,ClientBroker,seClientPosition,FTitle,FSQL);
 }
 ClientProp2->Show();

}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientResidenceEditButtonClick(TObject *Sender)

{
 if(ClientResidence==NULL)
 {
 String FTitle=L"居住地资料列表";
 String FSQL="SELECT ClientRdePKID,ClientRdeName FROM tbClient_Residence ORDER BY ClientRdeName";
 ClientResidence=new TQueryFilterForm(this,ClientBroker,seClientResidence,FTitle,FSQL);
 }
 ClientResidence->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientEducationEditButtonClick(TObject *Sender)

{
 if(ClientEducation==NULL)
 {
  String FTitle=L"教育程度料列表";
  String FSQL="SELECT ClientEduPKID,ClientEduName FROM tbClient_Education ORDER BY ClientEduName";
  ClientEducation=new TQueryFilterForm(this,ClientBroker,seClientEducation,FTitle,FSQL);
 }
 ClientEducation->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientWorkEditButtonClick(TObject *Sender)
{
 if(ClientWork==NULL)
 {
  String FTitle=L"职业资料列表";
 String FSQL="SELECT ClientWorkPKID,ClientWorkName FROM tbClient_Work ORDER BY ClientWorkName";
 ClientWork=new TQueryFilterForm(this,ClientBroker,seClientWork,FTitle,FSQL);
 }
  ClientWork->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientTradeEditButtonClick(TObject *Sender)
{
 if(ClientProp1==NULL)
 {
  String FTitle=L"从事行业资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=1 ORDER BY ClientPropName";
 ClientProp1=new TQueryFilterForm(this,ClientBroker,seClientTrade,FTitle,FSQL);
 }
  ClientProp1->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientWorkYearsEditButtonClick(TObject *Sender)

{
 if(ClientProp3==NULL)
 {
 String FTitle=L"工作年限资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=3 ORDER BY ClientPropName";
 ClientProp3=new TQueryFilterForm(this,ClientBroker,seClientWorkYears,FTitle,FSQL);
 }
 ClientProp3->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientYearIncomeEditButtonClick(TObject *Sender)

{
 if(ClientProp8==NULL)
 {
 String FTitle=L"年收入资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=8 ORDER BY ClientPropName";
 ClientProp8=new TQueryFilterForm(this,ClientBroker,seClientYearIncome,FTitle,FSQL);
 }
 ClientProp8->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientHouseTypeEditButtonClick(TObject *Sender)

{
 if(ClientProp4==NULL)
 {
 String FTitle=L"住宅性质资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=4 ORDER BY ClientPropName";
 ClientProp4=new TQueryFilterForm(this,ClientBroker,seClientHouseType,FTitle,FSQL);
 }
 ClientProp4->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientHouseAreaEditButtonClick(TObject *Sender)

{
 if(ClientProp5==NULL)
 {
 String FTitle=L"面积资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=5 ORDER BY ClientPropName";
 ClientProp5=new TQueryFilterForm(this,ClientBroker,seClientHouseArea,FTitle,FSQL);
 }
 ClientProp5->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientHousePopEditButtonClick(TObject *Sender)

{
 if(ClientProp6==NULL)
 {
 String FTitle=L"居住人口资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=6 ORDER BY ClientPropName";
 ClientProp6=new TQueryFilterForm(this,ClientBroker,seClientHousePop,FTitle,FSQL);
 }
 ClientProp6->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientTrafficEditButtonClick(TObject *Sender)
{
 if(ClientProp7==NULL)
 {
  String FTitle=L"交通工具资料列表";
  String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=7 ORDER BY ClientPropName";
  ClientProp7=new TQueryFilterForm(this,ClientBroker,seClientTraffic,FTitle,FSQL);
 }
 ClientProp7->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::ClientClubLevelEditButtonClick(TObject *Sender)

{
 if(ClientProp9==NULL)
 {
 String FTitle=L"级别资料列表";
 String FSQL="SELECT ClientPropPKID,ClientPropName FROM tbClient_Prop where ClientPropType=9 ORDER BY ClientPropName";
 ClientProp9=new TQueryFilterForm(this,ClientBroker,seClientClubLevel,FTitle,FSQL);
 }
 ClientProp9->Show();
}
//---------------------------------------------------------------------------
void __fastcall TCustomerEditForm::SetConsigner(String ConsignerID)
{
   Query->Close();
   Query->SQL->Text="select * from tbEmployeeData where EmpPKID="+QuotedStr(ConsignerID);
   Query->Open();
   if(Query->RecordCount>0)
   {
	   seClientSalerFKID->Text=Query->FieldByName("EmpName")->AsString;
	   seClientSalerFKID->TagString=Query->FieldByName("EmpPKID")->AsString;
   }
   Query->Close();
}
void __fastcall TCustomerEditForm::ClientSalerEditButtonClick(TObject *Sender)
{
   if(SelectPeople==NULL)
   {
	SelectPeople=new TSelectPeopleForm(this,&SetConsigner,this->ClientBroker);
   }
	SelectPeople->Show();
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::PhoneCallButtonClick(TObject *Sender)
{
  if(seClientPhoneNum->Text!="")
  {
	PhoneDialer(seClientPhoneNum->Text);
  }
  else
  {
	ShowMessage(L"请输入联系电话号码");
	if(seClientPhoneNum->CanFocus)
	  seClientPhoneNum->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::MobilePhoneCallButtonClick(TObject *Sender)
{
   if(seClientMobilePhoneNum->Text!="")
  {
	PhoneDialer(seClientMobilePhoneNum->Text);
  }
  else
  {
	ShowMessage(L"请输入移动电话号码");
	if(seClientMobilePhoneNum->CanFocus)
	  seClientMobilePhoneNum->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TCustomerEditForm::WorkPhoneCallButtonClick(TObject *Sender)
{
  if(seClientWorkPhoneNum->Text!="")
  {
	PhoneDialer(seClientWorkPhoneNum->Text);
  }
  else
  {
	ShowMessage(L"请输入工作电话号码");
	if(seClientWorkPhoneNum->CanFocus)
	  seClientWorkPhoneNum->SetFocus();
  }
}
//---------------------------------------------------------------------------

