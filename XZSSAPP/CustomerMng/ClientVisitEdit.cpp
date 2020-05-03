//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ClientVisitEdit.h"
#include "System.DateUtils.hpp"
#include "FSTMessageDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseBillForm"
#pragma resource "*.fmx"
TfrmClientVisitEdit *frmClientVisitEdit;
//---------------------------------------------------------------------------
__fastcall TfrmClientVisitEdit::TfrmClientVisitEdit(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseBillForm(Owner,clBroker,modulecode,param)
{
	FID = "";
	InitComboBox();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::InitComboBox()
{
	cbSex->Clear();
	cbWork->Clear();
	cbVehicle->Clear();
	cbInformPath->Clear();
	cbFunctions->Clear();
	cbHouseType->Clear();
	cbAmt->Clear();
	cbArea->Clear();
	cbFloor->Clear();
	cbOrientation->Clear();
	cbDecoration->Clear();
	cbPapersType->Clear();

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

	Query = csGetData("/Common/MobileService/GetDictData?IDArray=27,13,14,16,17,21,20,15,18,26,19",new TRESTRequestParameterList(this));
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
			else if(Query->FieldByName("DtID")->AsString == "14")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbInformPath);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbInformPath->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "16")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbFunctions);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbFunctions->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "17")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbHouseType);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbHouseType->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "21")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbAmt);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbAmt->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "20")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbArea);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbArea->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "15")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbFloor);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbFloor->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "18")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbOrientation);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbOrientation->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "19")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbDecoration);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbDecoration->AddObject(newBoxItem);
			}
			else if(Query->FieldByName("DtID")->AsString == "26")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbPapersType);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbPapersType->AddObject(newBoxItem);
			}
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::ClearData()
{
	FID = "";
	FDataChanged = false;
	FNeedFreshList = false;

	edNum->Text = "";
	edName->Text = "";
	edPhone->Text = "";
	nbAge->Value = 0;
	cbSex->ItemIndex = -1;
	cbVisitType->ItemIndex = 0;
	edAddress->Text = "";
	edContactAdd->Text = "";
	cbWork->ItemIndex = -1;
	cbProperty->ItemIndex = 0;
	cbMotive->ItemIndex = 0;
	cbVehicle->ItemIndex = -1;
	cbPapersType->ItemIndex = -1;
	edPapersNum->Text = "";
	deFirstVisitDate->Date = Today();
	deRecentVisitDate->Date = Today();
	cbInformPath->ItemIndex = -1;
	cbFunctions->ItemIndex = -1;
	cbHouseType->ItemIndex = -1;
	cbAmt->ItemIndex = -1;
	cbArea->ItemIndex = -1;
	cbFloor->ItemIndex = -1;
	cbOrientation->ItemIndex = -1;
	cbDecoration->ItemIndex = -1;
	mmRemark->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::FillData(String id)
{
    TabControl->TabIndex = 0;
	if(id.IsEmpty())
		ClearData();
	else
	{
		String loadUrl;
		if(FTtype == 1)
		{
			loadUrl = "/Work/tbClientVisit/Load";
			FID = id;
		}
		else
		{
			loadUrl = "/Work/tbClient/Load";
			FID = "";
		}
		FDataChanged = false;
		FNeedFreshList = false;
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("ID",id);
		Query = csGetData(loadUrl,Params);
		if(Query->Active && Query->RecordCount > 0)
		{
			Query->First();
			if(FTtype == 1)
				FClientID = Query->FieldByName("ClientID")->AsString;
			else
				FClientID = Query->FieldByName("FID")->AsString;

			edNum->Text = Query->FieldByName("Num")->AsString;
			edName->Text = Query->FieldByName("Name")->AsString;
			edPhone->Text = Query->FieldByName("Phone")->AsString;
			nbAge->Value = Query->FieldByName("Age")->AsFloat;
			for(int i=0;i<cbSex->Count;i++)
			{
				if(cbSex->ListItems[i]->TagString == Query->FieldByName("Sex")->AsString)
				{
					cbSex->ItemIndex = cbSex->ListItems[i]->Index;
					break;
				}
			}
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

			if(FTtype == 1)
			{
				cbVisitType->ItemIndex = Query->FieldByName("VisitType")->AsInteger;
				cbMotive->ItemIndex = Query->FieldByName("Motive")->AsInteger;
				deFirstVisitDate->Text = Query->FieldByName("FirstVisitDate")->AsString;
				deRecentVisitDate->Text = Query->FieldByName("FirstVisitDate")->AsString;
				for(int i=0;i<cbInformPath->Count;i++)
				{
					if(cbInformPath->ListItems[i]->TagString == Query->FieldByName("InformPath")->AsString)
					{
						cbInformPath->ItemIndex = cbInformPath->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbFunctions->Count;i++)
				{
					if(cbFunctions->ListItems[i]->TagString == Query->FieldByName("Functions")->AsString)
					{
						cbFunctions->ItemIndex = cbFunctions->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbHouseType->Count;i++)
				{
					if(cbHouseType->ListItems[i]->TagString == Query->FieldByName("HouseType")->AsString)
					{
						cbHouseType->ItemIndex = cbHouseType->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbAmt->Count;i++)
				{
					if(cbAmt->ListItems[i]->TagString == Query->FieldByName("Amt")->AsString)
					{
						cbAmt->ItemIndex = cbAmt->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbArea->Count;i++)
				{
					if(cbArea->ListItems[i]->TagString == Query->FieldByName("Area")->AsString)
					{
						cbArea->ItemIndex = cbArea->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbFloor->Count;i++)
				{
					if(cbFloor->ListItems[i]->TagString == Query->FieldByName("Floor")->AsString)
					{
						cbFloor->ItemIndex = cbFloor->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbOrientation->Count;i++)
				{
					if(cbOrientation->ListItems[i]->TagString == Query->FieldByName("Orientation")->AsString)
					{
						cbOrientation->ItemIndex = cbOrientation->ListItems[i]->Index;
						break;
					}
				}
				for(int i=0;i<cbDecoration->Count;i++)
				{
					if(cbDecoration->ListItems[i]->TagString == Query->FieldByName("Decoration")->AsString)
					{
						cbDecoration->ItemIndex = cbDecoration->ListItems[i]->Index;
						break;
					}
				}
				mmRemark->Text = Query->FieldByName("Remark")->AsString;
			}
			else
			{
				cbVisitType->ItemIndex = 0;
				cbMotive->ItemIndex = 0;
				deFirstVisitDate->Date = Today();
				deRecentVisitDate->Date = Today();
				cbInformPath->ItemIndex = -1;
				cbFunctions->ItemIndex = -1;
				cbHouseType->ItemIndex = -1;
				cbAmt->ItemIndex = -1;
				cbArea->ItemIndex = -1;
				cbFloor->ItemIndex = -1;
				cbOrientation->ItemIndex = -1;
				cbDecoration->ItemIndex = -1;
				mmRemark->Text = "";
			}
		}
		FillDetailData(0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::FillDetailData(int index)
{
	if(index == 0)
	{
		ListViewVisit->Items->Clear();
	}
	for(int i=0;i<ListViewVisit->Items->Count;i++)
	{
		if(!ListViewVisit->Items->Item[i]->HasData["FID"])
		{
          	ListViewVisit->Items->Delete(i);
        }
    }
	if(FTtype == 1)
	{
		TJSONArray* FilterParam = new TJSONArray();
		TJSONObject *MasterID = new TJSONObject();
		MasterID->AddPair("Key","MasterID");
		MasterID->AddPair("Value",FID);
		MasterID->AddPair("Oper","AddEqualTo");
		FilterParam->Add(MasterID);
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("pageSize",20);
		Params->AddItem("pageIndex",index);
		Params->AddItem("FilterParam",FilterParam->ToString());
		Query = csGetData("/Work/tbClientVisitDetail/GetGridData",Params);
        int FTotal = Query->FieldByName("total")->AsInteger;
		Query = GetElementData("data");
		if(Query->Active && Query->RecordCount > 0)
		{
			Query->First();
			while(!Query->Eof)
			{
				TListViewItem* item = ListViewVisit->Items->Add();
				item->Text = L"��" + Query->FieldByName("Address")->AsString + L"��" + Query->FieldByName("Subject")->AsString;
				item->Detail = Query->FieldByName("Date")->AsString;
				item->Data["FID"] = TValue::_op_Implicit(Query->FieldByName("FID")->AsString);
				Query->Next();
			}
			if(ListViewVisit->Items->Count < FTotal)
			{
				TListViewItem* FLoadMore = ListViewVisit->Items->Add();
				FLoadMore->Purpose = TListItemPurpose::Footer;
				FLoadMore->Text = L"�������ظ���";
			}
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TfrmClientVisitEdit::VerifyData()
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
void __fastcall TfrmClientVisitEdit::SaveData()
{
	if(!VerifyData())
		return;

	TJSONObject *formData = new TJSONObject();
	formData->AddPair("Num",edNum->Text);
	formData->AddPair("Name",edName->Text);
	formData->AddPair("Phone",edPhone->Text);
	formData->AddPair("Age",nbAge->Value);
	if(cbSex->ItemIndex >= 0)
		formData->AddPair("Sex",cbSex->Selected->TagString);
	else
        formData->AddPair("Sex",0);
	formData->AddPair("VisitType",cbVisitType->ItemIndex);
	formData->AddPair("Address",edAddress->Text);
	formData->AddPair("ContactAdd",edContactAdd->Text);
	if(cbWork->ItemIndex >= 0)
		formData->AddPair("Work",cbWork->Selected->TagString);
	else
		formData->AddPair("Work",0);
	formData->AddPair("Property",cbProperty->ItemIndex);
	formData->AddPair("Motive",cbMotive->ItemIndex);
	if(cbVehicle->ItemIndex >= 0)
		formData->AddPair("Vehicle",cbVehicle->Selected->TagString);
	else
		formData->AddPair("Vehicle",0);
	if(cbPapersType->ItemIndex >= 0)
		formData->AddPair("PapersType",cbPapersType->Selected->TagString);
	else
		formData->AddPair("PapersType",0);
	formData->AddPair("PapersNum",edPapersNum->Text);
	formData->AddPair("FirstVisitDate",deFirstVisitDate->Text);
	formData->AddPair("RecentVisitDate",deRecentVisitDate->Text);
	if(cbInformPath->ItemIndex >= 0)
		formData->AddPair("InformPath",cbInformPath->Selected->TagString);
	else
		formData->AddPair("InformPath",0);
	if(cbFunctions->ItemIndex >= 0)
		formData->AddPair("Functions",cbFunctions->Selected->TagString);
	else
		formData->AddPair("Functions",0);
	if(cbHouseType->ItemIndex >= 0)
		formData->AddPair("HouseType",cbHouseType->Selected->TagString);
	else
		formData->AddPair("HouseType",0);
	if(cbAmt->ItemIndex >= 0)
		formData->AddPair("Amt",cbAmt->Selected->TagString);
	else
		formData->AddPair("Amt",0);
	if(cbArea->ItemIndex >= 0)
		formData->AddPair("Area",cbArea->Selected->TagString);
	else
		formData->AddPair("Area",0);
	if(cbFloor->ItemIndex >= 0)
		formData->AddPair("Floor",cbFloor->Selected->TagString);
	else
		formData->AddPair("Floor",0);
	if(cbOrientation->ItemIndex >= 0)
		formData->AddPair("Orientation",cbOrientation->Selected->TagString);
	else
		formData->AddPair("Orientation",0);
	if(cbDecoration->ItemIndex >= 0)
		formData->AddPair("Decoration",cbDecoration->Selected->TagString);
	else
		formData->AddPair("Decoration",0);
	formData->AddPair("Remark",mmRemark->Text);

	if(FTtype == 1)
	{
		if(FID.IsEmpty())
		{
			FNeedFreshList = true;
		}
		else
		{
			formData->AddPair("FID",FID);
			FNeedFreshList = FDataChanged;
		}
	}
	else
	{
		formData->AddPair("ClientID",FClientID);
		FNeedFreshList = true;
	}

	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddUrlSegment("data",formData->ToString());
	Query = PostData("/Work/tbClientVisit/Save?Data={data}",Params);

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
void __fastcall TfrmClientVisitEdit::BtnSaveClick(TObject *Sender)
{
 	SaveData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::DataChange(TObject *Sender)
{
	FDataChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmClientVisitEdit::PhooneCallClick(TObject *Sender)
{
	String phoneNum = edPhone->Text;
	if(!phoneNum.IsEmpty())
		PhoneDialer(phoneNum);
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::ListViewVisitDeletingItem(TObject *Sender, int AIndex,
          bool &ACanDelete)
{
	deletingID = ListViewVisit->Items->Item[AIndex]->Data["FID"].AsString();
    if(!deletingID.IsEmpty())
	{
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("ids",deletingID);
		Query = PostData("/Work/tbClientVisitDetail/Remove",Params);

		if(Query->Active && Query->RecordCount > 0)
		{
			bool Success = Query->FieldByName("Success")->AsBoolean;
			if(Success)
			{
//				FillDetailData(0);
				deletingID = "";
				ACanDelete = true;
				ShowMessage(L"ɾ���ɹ�!");
			}
			else
			{
				String Message = Query->FieldByName("Message")->AsString;
				if(Message.IsEmpty())
					Message = L"ɾ��ʧ�ܣ����Ժ�����!";
				ACanDelete = false;
				ShowMessage(Message);
			}
		}
		else
		{
            ACanDelete = false;
			ShowMessage(L"ɾ��ʧ��!");
		}
	}

//	TMessageDialogEventProc msgDialogDeleteProc=&deleteDetail;
//	FSTMessageDlg(_D("ȷ��Ҫɾ��������") ,TMsgDlgType::mtConfirmation,mbYesNo,0,msgDialogDeleteProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::deleteDetail(System::Uitypes::TModalResult AResult)
{
//	if(AResult==mrYes)
//	{
//		if(!deletingID.IsEmpty())
//		{
//			TJSONArray* ids = new TJSONArray();
//			ids->Add(deletingID);
//
//            TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
//			Params->AddItem("ids",ids->ToString());
//			Query = PostData("/Work/tbClientVisitDetail/Remove",Params);
//
//			if(Query->Active && Query->RecordCount > 0)
//			{
//				bool Success = Query->FieldByName("Success")->AsBoolean;
//				if(Success)
//				{
//					FillDetailData(0);
//					deletingID = "";
//					ShowMessage(L"ɾ���ɹ�!");
//				}
//				else
//				{
//					String Message = Query->FieldByName("Message")->AsString;
//					if(Message.IsEmpty())
//						Message = L"ɾ��ʧ�ܣ����Ժ�����!";
//					ShowMessage(Message);
//				}
//			}
//			else
//				ShowMessage(L"ɾ��ʧ��!");
//		}
//	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmClientVisitEdit::ListViewVisitItemClick(TObject * const Sender,
          TListViewItem * const AItem)
{
	if(EditForm == NULL)
	{
		EditForm = new TfrmClientVisitDetailEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	EditForm->MasterID = FID;
	String id = AItem->Data["FID"].AsString();
	EditForm->FillData(id);
	EditForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::EditFormClose(TObject *Sender, TCloseAction &Action)
{
	TfrmClientVisitDetailEdit *p = dynamic_cast<TfrmClientVisitDetailEdit*>(Sender);
	if(p)
	{
		if(p->NeedFreshList)
		{
         	FillDetailData(0);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmClientVisitEdit::TabControlChange(TObject *Sender)
{
	BtnAddDetail->Visible = (TabControl->ActiveTab == RecoTab) && (!FID.IsEmpty());
}
//---------------------------------------------------------------------------

void __fastcall TfrmClientVisitEdit::BtnAddDetailClick(TObject *Sender)
{
  	if(EditForm == NULL)
	{
		EditForm = new TfrmClientVisitDetailEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	EditForm->MasterID = FID;
	EditForm->FillData("");
	EditForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmClientVisitEdit::ListViewVisitPullRefresh(TObject *Sender)
{
	ListViewVisit->BeginUpdate();
	try
	{
		FillDetailData(0);
	}
	__finally
	{
		ListViewVisit->EndUpdate();
	}
}
//---------------------------------------------------------------------------

