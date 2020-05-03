//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Client.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseForm"
#pragma resource "*.fmx"
TfrmClient *frmClient;
const int PageSize = 20;
//---------------------------------------------------------------------------
__fastcall TfrmClient::TfrmClient(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseForm(Owner,clBroker,modulecode,param)
{
    EditForm = NULL;
	FLoadMore = ListViewClient->Items->Add();
	FLoadMore->Purpose = TListItemPurpose::Footer;
	FLoadMore->Text = L"���ظ���";
	FTotal = 0;
	InitListView();
	LoadData(0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmClient::InitListView()
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmClient::LoadData(int index)
{
	ClearSeachBox();

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
	Query = csGetData("/Work/tbClient/GetMobileData",Params);
	FTotal = Query->FieldByName("total")->AsInteger;
	Query = GetElementData("data");
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListViewItem* item = ListViewClient->Items->Add();
			item->Text = Query->FieldByName("Name")->AsString;
			item->Detail = Query->FieldByName("Phone")->AsString;
			item->Data["FID"] = TValue::_op_Implicit(Query->FieldByName("FID")->AsString);
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
void __fastcall TfrmClient::ListViewClientPullRefresh(TObject *Sender)
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
void __fastcall TfrmClient::ListViewClientItemClick(TObject * const Sender, TListViewItem * const AItem)

{
	if(EditForm == NULL)
	{
		EditForm = new TfrmClientEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	String id = AItem->Data["FID"].AsString();
	EditForm->FillData(id);
	EditForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TfrmClient::ListViewClientMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
//	if(ListViewClient->Items->Filtered)
//		return;
//	int pos = ListViewClient->ScrollViewPos;
//	int count = ClientData->RecordCount;
//	if(count >= 0 && count < FTotal)
//	{
//		int viewHeight = ListViewClient->AbsoluteHeight;
//		double itemHeight = ListViewClient->ItemAppearance->ItemHeight;
//		if(pos+viewHeight > count*itemHeight)
//			LoadData(count/PageSize);
//	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmClient::ClearSeachBox()
{
	TSearchBox* searchBox;
	for (int i = 0; i < ListViewClient->Controls->Count; i++) {
		searchBox = dynamic_cast<TSearchBox*>(ListViewClient->Controls->Items[i]);
		if (searchBox)
		{
			searchBox->Text = "";
			break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmClient::ListViewClientPaint(TObject *Sender, TCanvas *Canvas,
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
void __fastcall TfrmClient::EditFormClose(TObject *Sender, TCloseAction &Action)
{
	TfrmClientEdit *p = dynamic_cast<TfrmClientEdit*>(Sender);
	if(p)
	{
		if(p->NeedFreshList)
		{
         	LoadData(0);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmClient::BtnAddClick(TObject *Sender)
{
	if(EditForm == NULL)
	{
		EditForm = new TfrmClientEdit(this,ClientBroker,ModuleCode,"");
		EditForm->OnClose = EditFormClose;
	}
	EditForm->FillData("");
	EditForm->Show();
}
//---------------------------------------------------------------------------
