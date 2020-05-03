//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ClientEdit.h"
#include "System.DateUtils.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseBillForm"
#pragma resource "*.fmx"
TfrmClientEdit *frmClientEdit;
//---------------------------------------------------------------------------
__fastcall TfrmClientEdit::TfrmClientEdit(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseBillForm(Owner,clBroker,modulecode,param)
{
	FID = "";
	InitComboBox();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::InitComboBox()
{
	cbSex->Clear();
	cbWork->Clear();
	cbVehicle->Clear();
	cbPapersType->Clear();
	cbMaritalStatus->Clear();
	cbPosition->Clear();
	cbEducation->Clear();
	cbIncome->Clear();
	cbFamilyStatus->Clear();
	cbWorkYears->Clear();

	TListBoxItem * maleItem=new TListBoxItem(cbSex);
	maleItem->Text = L"��";
	maleItem->TagString = "1";
	cbSex->AddObject(maleItem);
	TListBoxItem * femaleItem=new TListBoxItem(cbSex);
	femaleItem->Text = L"Ů";
	femaleItem->TagString = "2";
	cbSex->AddObject(femaleItem);
	TListBoxItem * unknownItem=new TListBoxItem(cbSex);
	unknownItem->Text = L"����";
	unknownItem->TagString = "9";
	cbSex->AddObject(unknownItem);

	Query = csGetData("/Common/MobileService/GetDictData?IDArray=27,13,26,29,28,31,32,34,35",new TRESTRequestParameterList(this));
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			if(Query->FieldByName("DtID")->AsString == "27")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbWork);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbWork->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "13")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbVehicle);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbVehicle->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "26")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbPapersType);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbPapersType->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "29")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbMaritalStatus);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbMaritalStatus->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "28")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbPosition);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbPosition->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "31")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbEducation);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbEducation->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "32")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbIncome);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbIncome->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "34")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbFamilyStatus);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbFamilyStatus->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "35")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbWorkYears);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbWorkYears->AddObject(newBoxItem);
			}
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::ClearData()
{
	FID = "";
	FDataChanged = false;
	FNeedFreshList = false;

	edNum->Text = "";
	edName->Text = "";
	edPhone->Text = "";
	edWorkPhone->Text = "";
	edFaxPhone->Text = "";
	edHomePhone->Text = "";
	nbAge->Value = 0;
	cbSex->ItemIndex = -1;
	cbTheType->ItemIndex = -1;
	edAddress->Text = "";
	edContactAdd->Text = "";
	cbWork->ItemIndex = -1;
	cbProperty->ItemIndex = -1;
	deBirthday->Date = Today();
	cbVehicle->ItemIndex = -1;
	cbPapersType->ItemIndex = -1;
	edPapersNum->Text = "";
	edEmail->Text = "";
	edPostalCode->Text = "";
	edCompany->Text = "";
	cbMaritalStatus->ItemIndex = -1;
	edBankName->Text = "";
	edBankAccount->Text = "";
	cbPosition->ItemIndex = -1;
	cbEducation->ItemIndex = -1;
	cbIncome->ItemIndex = -1;
	cbFamilyStatus->ItemIndex = -1;
	cbWorkYears->ItemIndex = -1;
	mmRemark->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::FillData(String id)
{
	if(id.IsEmpty())
		ClearData();
	else
	{
		FID = id;
		FDataChanged = false;
		FNeedFreshList = false;
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("ID",id);
		Query = csGetData("/Work/tbClient/Load",Params);
		if(Query->Active && Query->RecordCount > 0)
		{
			Query->First();
			edNum->Text = Query->FieldByName("Num")->AsString;
			edName->Text = Query->FieldByName("Name")->AsString;
			edPhone->Text = Query->FieldByName("Phone")->AsString;
			edWorkPhone->Text = Query->FieldByName("WorkPhone")->AsString;
			edFaxPhone->Text = Query->FieldByName("FaxPhone")->AsString;
			edHomePhone->Text = Query->FieldByName("HomePhone")->AsString;
			nbAge->Value = Query->FieldByName("Age")->AsFloat;
			for(int i=0;i<cbSex->Count;i++)
			{
				if(cbSex->ListItems[i]->TagString == Query->FieldByName("Sex")->AsString)
				{
					cbSex->ItemIndex = cbSex->ListItems[i]->Index;
					break;
				}
			}
			cbTheType->ItemIndex = Query->FieldByName("TheType")->AsInteger;
			edAddress->Text = Query->FieldByName("Address")->AsString;
			edContactAdd->Text = Query->FieldByName("ContactAdd")->AsString;
			for(int i=0;i<cbWork->Count;i++)
			{
				if(cbWork->ListItems[i]->TagString == Query->FieldByName("Work")->AsString)
				{
					cbWork->ItemIndex = cbWork->ListItems[i]->Index;
					break;
				}
			}
			cbProperty->ItemIndex = Query->FieldByName("Property")->AsInteger;
			deBirthday->Text = Query->FieldByName("Birthday")->AsString;
//			deBirthday->Date = Query->FieldByName("Birthday")->AsDateTime;
			for(int i=0;i<cbVehicle->Count;i++)
			{
				if(cbVehicle->ListItems[i]->TagString == Query->FieldByName("Vehicle")->AsString)
				{
					cbVehicle->ItemIndex = cbVehicle->ListItems[i]->Index;
					break;
				}
			}
			for(int i=0;i<cbPapersType->Count;i++)
			{
				if(cbPapersType->ListItems[i]->TagString == Query->FieldByName("PapersType")->AsString)
				{
					cbPapersType->ItemIndex = cbPapersType->ListItems[i]->Index;
					break;
				}
			}
			edPapersNum->Text = Query->FieldByName("PapersNum")->AsString;
			edEmail->Text = Query->FieldByName("Email")->AsString;
			edPostalCode->Text = Query->FieldByName("PostalCode")->AsString;
			edCompany->Text = Query->FieldByName("Company")->AsString;
			for(int i=0;i<cbMaritalStatus->Count;i++)
			{
				if(cbMaritalStatus->ListItems[i]->TagString == Query->FieldByName("MaritalStatus")->AsString)
				{
					cbMaritalStatus->ItemIndex = cbMaritalStatus->ListItems[i]->Index;
					break;
				}
			}
			edBankName->Text = Query->FieldByName("BankName")->AsString;
			edBankAccount->Text = Query->FieldByName("BankAccount")->AsString;
			for(int i=0;i<cbPosition->Count;i++)
			{
				if(cbPosition->ListItems[i]->TagString == Query->FieldByName("Position")->AsString)
				{
					cbPosition->ItemIndex = cbPosition->ListItems[i]->Index;
					break;
				}
			}
			for(int i=0;i<cbEducation->Count;i++)
			{
				if(cbEducation->ListItems[i]->TagString == Query->FieldByName("Education")->AsString)
				{
					cbEducation->ItemIndex = cbEducation->ListItems[i]->Index;
					break;
				}
			}
			for(int i=0;i<cbIncome->Count;i++)
			{
				if(cbIncome->ListItems[i]->TagString == Query->FieldByName("Income")->AsString)
				{
					cbIncome->ItemIndex = cbIncome->ListItems[i]->Index;
					break;
				}
			}
			for(int i=0;i<cbFamilyStatus->Count;i++)
			{
				if(cbFamilyStatus->ListItems[i]->TagString == Query->FieldByName("FamilyStatus")->AsString)
				{
					cbFamilyStatus->ItemIndex = cbFamilyStatus->ListItems[i]->Index;
					break;
				}
			}
			for(int i=0;i<cbWorkYears->Count;i++)
			{
				if(cbWorkYears->ListItems[i]->TagString == Query->FieldByName("WorkYears")->AsString)
				{
					cbWorkYears->ItemIndex = cbWorkYears->ListItems[i]->Index;
					break;
				}
			}
			mmRemark->Text = Query->FieldByName("Remark")->AsString;
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TfrmClientEdit::VerifyData()
{
	if(edNum->Text.IsEmpty())
	{
        ShowMessage(L"������ͻ����!");
		return false;
	}
	if(edName->Text.IsEmpty())
	{
        ShowMessage(L"������ͻ�����!");
		return false;
	}
	if(edPhone->Text.IsEmpty())
	{
		ShowMessage(L"������ͻ��绰!");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::SaveData()
{
	if(!VerifyData())
		return;

	TJSONObject *formData = new TJSONObject();
	formData->AddPair("Num",edNum->Text);
	formData->AddPair("Name",edName->Text);
	formData->AddPair("Phone",edPhone->Text);
	formData->AddPair("WorkPhone",edWorkPhone->Text);
	formData->AddPair("FaxPhone",edFaxPhone->Text);
	formData->AddPair("HomePhone",edHomePhone->Text);
	formData->AddPair("Age",nbAge->Value);
	if(cbSex->ItemIndex >= 0)
		formData->AddPair("Sex",cbSex->Selected->TagString);
	else
        formData->AddPair("Sex",0);
	formData->AddPair("TheType",cbTheType->ItemIndex);
	formData->AddPair("Address",edAddress->Text);
	formData->AddPair("ContactAdd",edContactAdd->Text);
	if(cbWork->ItemIndex >= 0)
		formData->AddPair("Work",cbWork->Selected->TagString);
	else
		formData->AddPair("Work",0);
	formData->AddPair("Property",cbProperty->ItemIndex);
	formData->AddPair("Birthday",deBirthday->Text);
	if(cbVehicle->ItemIndex >= 0)
		formData->AddPair("Vehicle",cbVehicle->Selected->TagString);
	else
		formData->AddPair("Vehicle",0);
	if(cbPapersType->ItemIndex >= 0)
		formData->AddPair("PapersType",cbPapersType->Selected->TagString);
	else
		formData->AddPair("PapersType",0);
	formData->AddPair("PapersNum",edPapersNum->Text);
	formData->AddPair("Email",edEmail->Text);
	formData->AddPair("PostalCode",edPostalCode->Text);
	formData->AddPair("Company",edCompany->Text);
	if(cbMaritalStatus->ItemIndex >= 0)
		formData->AddPair("MaritalStatus",cbMaritalStatus->Selected->TagString);
	else
		formData->AddPair("MaritalStatus",0);
	formData->AddPair("BankName",edBankName->Text);
	formData->AddPair("BankAccount",edBankAccount->Text);
	if(cbPosition->ItemIndex >= 0)
		formData->AddPair("Position",cbPosition->Selected->TagString);
	else
		formData->AddPair("Position",0);
	if(cbEducation->ItemIndex >= 0)
		formData->AddPair("Education",cbEducation->Selected->TagString);
	else
		formData->AddPair("Education",0);
	if(cbIncome->ItemIndex >= 0)
		formData->AddPair("Income",cbIncome->Selected->TagString);
	else
		formData->AddPair("Income",0);
	if(cbFamilyStatus->ItemIndex >= 0)
		formData->AddPair("FamilyStatus",cbFamilyStatus->Selected->TagString);
	else
		formData->AddPair("FamilyStatus",0);
	if(cbWorkYears->ItemIndex >= 0)
		formData->AddPair("WorkYears",cbWorkYears->Selected->TagString);
	else
		formData->AddPair("WorkYears",0);
	formData->AddPair("Remark",mmRemark->Text);
	if(FID.IsEmpty())
	{
		formData->AddPair("_state","add");
		FNeedFreshList = true;
	}
	else
	{
		formData->AddPair("FID",FID);
		formData->AddPair("_state","edit");
		FNeedFreshList = FDataChanged;
	}

	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddUrlSegment("data",formData->ToString());
	Query = PostData("/Work/tbClient/SaveData?Data={data}",Params);

	if(Query->Active && Query->RecordCount > 0)
	{
		bool Success = Query->FieldByName("Success")->AsBoolean;
		if(Success)
		{
			Query = GetElementData("data");
			if(Query->Active && Query->RecordCount > 0)
				FID = Query->FieldByName("FID")->AsString;
			ShowMessage(L"����ɹ�!");
		}
		else
		{
            String Message = Query->FieldByName("Message")->AsString;
			if(Message.IsEmpty())
				Message = L"�ύʧ�ܣ����Ժ�����!";
			ShowMessage(Message);
        }
	}
	else
		ShowMessage(L"�ύʧ��!");
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::BtnSaveClick(TObject *Sender)
{
 	SaveData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientEdit::DataChange(TObject *Sender)
{
	FDataChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmClientEdit::PhooneCallClick(TObject *Sender)
{
	TImage *btn = dynamic_cast<TImage*>(Sender);
	if(btn)
	{
		String phoneNum = "";
		if(btn->Name == "PhooneCall")
			phoneNum = edPhone->Text;
		else if(btn->Name == "WorkPhoneCall")
			phoneNum = edWorkPhone->Text;
		else if(btn->Name == "HomePhoneCall")
			phoneNum = edHomePhone->Text;
		if(!phoneNum.IsEmpty())
			PhoneDialer(phoneNum);
	}
}
//---------------------------------------------------------------------------

