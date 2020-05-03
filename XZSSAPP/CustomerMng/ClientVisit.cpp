//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ClientVisit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseBillForm"
#pragma resource "*.fmx"
TfrmCliengVisit *frmCliengVisit;
const int PageSize = 20;
//---------------------------------------------------------------------------
__fastcall TfrmCliengVisit::TfrmCliengVisit(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseBillForm(Owner,clBroker,modulecode,param)
{
    FilterParam = new TJSONArray();
	FLoadMore = ListViewClient->Items->Add();
	FLoadMore->Purpose = TListItemPurpose::Footer;
	FLoadMore->Text = L"���ظ���";
	FTotal = 0;
	InitQueryView();
   //	LoadData(0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCliengVisit::InitQueryView()
{
	TListBoxItem * nullBoxItem=new TListBoxItem(cbEstate);
	nullBoxItem->Text = "";
	nullBoxItem->TagString = "";
	cbEstate->AddObject(nullBoxItem);
	Query = csGetData("/SalesManagement/tbEstate/GetAllData",new TRESTRequestParameterList(this));
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem * newBoxItem=new TListBoxItem(cbEstate);
			newBoxItem->Text = Query->FieldByName("Name")->AsString;
			newBoxItem->TagString = Query->FieldByName("FID")->AsString;
			cbEstate->AddObject(newBoxItem);
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCliengVisit::LoadData(int index)
{
//	ClearSeachBox();

	if(FLoadMore)
	{
		ListViewClient->Items->Delete(FLoadMore->Index);
//		delete FLoadMore;
		FLoadMore = NULL;
	}
	if(index == 0)
	{
		ListViewClient->Items->Clear();
		FItemCount = 0;
	}
	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("pageSize",PageSize);
	Params->AddItem("pageIndex",index);
    Params->AddItem("FilterParam",FilterParam->ToString());
	Query = csGetData("/Work/tbClientVisit/GetData",Params);
	FTotal = Query->FieldByName("total")->AsInteger;
	Query = GetElementData("data");
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListViewItem* item = ListViewClient->Items->Add();
			item->Text = L"��" + Query->FieldByName("Num")->AsString + L"��" + Query->FieldByName("Name")->AsString;
			item->Detail = Query->FieldByName("Phone")->AsString;
			item->Data["FID"] = TValue::_op_Implicit(Query->FieldByName("FID")->AsString);
            item->Data["Ttype"] = TValue::_op_Implicit(Query->FieldByName("Ttype")->AsInteger);
			FItemCount++;
			Query->Next();
		}
		if(ListViewClient->Items->Count < FTotal)
		{
			FLoadMore = ListViewClient->Items->Add();
			FLoadMore->Purpose = TListItemPurpose::Footer;
			FLoadMore->Text = L"�������ظ���";
		}
		ListViewClient->Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCliengVisit::ListViewClientPullRefresh(TObject *Sender)
{
	ListViewClient->BeginUpdate();
	try
	{
		LoadData(0);
	}
	__finally
	{
    	ListViewClient->EndUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCliengVisit::ListViewClientPaint(TObject *Sender, TCanvas *Canvas,
          const TRectF &ARect)
{
	if(FItemCount >= 0 && FItemCount < FTotal)
	{
		TSearchBox* searchBox;
		for (int i = 0; i < ListViewClient->Controls->Count; i++) {
			searchBox = dynamic_cast<TSearchBox*>(ListViewClient->Controls->Items[i]);
			if (searchBox) break;
		}
		int pos = ListViewClient->ScrollViewPos;
		int viewHeight = ListViewClient->Height;
		double itemHeight = ListViewClient->ItemAppearance->ItemHeight;
		double footerHeight = ListViewClient->ItemAppearance->FooterHeight;
		double searchBoxHeight = 0;
		if(ListViewClient->SearchVisible)
			searchBoxHeight = searchBox->Height;
		if(pos+viewHeight >= FItemCount*itemHeight+searchBoxHeight+footerHeight)
		{
			LoadData(FItemCount/PageSize);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCliengVisit::BtnSearchClick(TObject *Sender)
{
    FilterParam = new TJSONArray();
	if(cbEstate->Selected && !cbEstate->Selected->TagString.IsEmpty())
	{
		TJSONObject *EstateID = new TJSONObject();
		EstateID->AddPair("Key","EstateID");
		EstateID->AddPair("Value",cbEstate->Selected->TagString);
		EstateID->AddPair("Oper","AddMatch");
		FilterParam->Add(EstateID);
	}
	if(!edCard->Text.IsEmpty())
	{
		TJSONObject *MemberNum = new TJSONObject();
		MemberNum->AddPair("Key","MemberNum");
		MemberNum->AddPair("Value",edCard->Text);
		MemberNum->AddPair("Oper","AddMatch");
		FilterParam->Add(MemberNum);
	}
	if(!edName->Text.IsEmpty())
	{
		TJSONObject *Name = new TJSONObject();
		Name->AddPair("Key","Name");
		Name->AddPair("Value",edName->Text);
		Name->AddPair("Oper","AddMatch");
		FilterParam->Add(Name);
	}
	if(!edNum->Text.IsEmpty())
	{
		TJSONObject *PapersNum = new TJSONObject();
		PapersNum->AddPair("Key","PapersNum");
		PapersNum->AddPair("Value",edNum->Text);
		PapersNum->AddPair("Oper","AddMatch");
		FilterParam->Add(PapersNum);
	}
	if(!edTel->Text.IsEmpty())
	{
		TJSONObject *Phone = new TJSONObject();
		Phone->AddPair("Key","Phone");
		Phone->AddPair("Value",edTel->Text);
		Phone->AddPair("Oper","AddMatch");
		FilterParam->Add(Phone);
	}
    QueryView->HideMaster();
    LoadData(0);
}
//---------------------------------------------------------------------------


void __fastcall TfrmCliengVisit::ListViewClientItemClick(TObject * const Sender, TListViewItem * const AItem)

{

	if(EditForm == NULL)
	{
		EditForm = new TfrmClientVisitEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	int FTtype = AItem->Data["Ttype"].AsInteger();
	EditForm->Ttype = FTtype;
	String id = AItem->Data["FID"].AsString();
	EditForm->FillData(id);
	EditForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmCliengVisit::BtnAddClick(TObject *Sender)
{
 	if(EditForm == NULL)
	{
		EditForm = new TfrmClientVisitEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	EditForm->Ttype = 1;
	EditForm->FillData("");
	EditForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmCliengVisit::ListViewClientDeletingItem(TObject *Sender, int AIndex,
		  bool &ACanDelete)
{
	int FTtype = ListViewClient->Items->Item[AIndex]->Data["Ttype"].AsInteger();
	if(FTtype != 1)
	{
		ShowMessage(L"�ͻ������ݣ�û��Ȩ��ɾ����");
        ACanDelete = false;
	}
	else
	{
		String deletingID = ListViewClient->Items->Item[AIndex]->Data["FID"].AsString();
		if(!deletingID.IsEmpty())
		{
			TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
			Params->AddItem("ids",deletingID);
			Query = PostData("/Work/tbClientVisit/Remove",Params);

			if(Query->Active && Query->RecordCount > 0)
			{
				bool Success = Query->FieldByName("Success")->AsBoolean;
				if(Success)
				{
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
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmCliengVisit::EditFormClose(TObject *Sender, TCloseAction &Action)
{
	TfrmClientVisitEdit *p = dynamic_cast<TfrmClientVisitEdit*>(Sender);
	if(p)
	{
		if(p->NeedFreshList)
		{
         	LoadData(0);
        }
	}
}
//---------------------------------------------------------------------------
