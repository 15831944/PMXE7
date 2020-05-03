//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "WorkFlowBrowseForm.h"
#include "WorkFlowDetailForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseForm"
#pragma resource "*.fmx"
TfrmWorkFlowBrowseForm *frmWorkFlowBrowseForm;
//---------------------------------------------------------------------------
__fastcall TfrmWorkFlowBrowseForm::TfrmWorkFlowBrowseForm(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseForm(Owner,clBroker,modulecode,param)
{
	FFlowID = -1;
	FGridName = "";
	FCommitUrl = "";
    FNotPassUrl = "";
	LoadConfig();
	InitFilter();
	LoadGridConfig();
	filteData();
}
//---------------------------------------------------------------------------
__fastcall TfrmWorkFlowBrowseForm::~TfrmWorkFlowBrowseForm()
{

}
//---------------------------------------------------------------------------
String __fastcall TfrmWorkFlowBrowseForm::GetConfigUrl()
{
	return "";
}
//---------------------------------------------------------------------------
String __fastcall TfrmWorkFlowBrowseForm::GetIDColumnName()
{
 	return "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::LoadConfig()
{
	String FConfigUrl = GetConfigUrl();
	if(!FConfigUrl.IsEmpty())
	{
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("MenuID",ModuleCode);
		Query = csGetData(FConfigUrl,Params);
		if(Query->Active && Query->RecordCount > 0)
		{
			FFlowID = Query->FieldByName("FlowID")->AsInteger;
			FFilterState = Query->FieldByName("FilterState")->AsInteger;
			FCommitUrl = Query->FieldByName("CommitUrl")->AsString;
			FNotPassUrl = Query->FieldByName("NotPassUrl")->AsString;
			FLoadUrl = Query->FieldByName("loadUrl")->AsString;
			Query = GetElementData("MainGridConfig");
			FGridName = Query->FieldByName("Name")->AsString;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::LoadGridConfig()
{
  	for(int i=BrowseGrid->ColumnCount-1;i>=0;i--)
		BrowseGrid->Columns[i]->DisposeOf();

	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("Key",FGridName);
	Query = csGetData("/Common/MobileService/GetWorkFlowGridConfig",Params);
	if(Query->Active && Query->RecordCount > 0)
	{
		FGridDataUrl = Query->FieldByName("Url")->AsString;
		Query = GetElementData("ColumnConfig");
		if(Query->Active && Query->RecordCount > 0)
		{
			TStringColumn *IDCol = new TStringColumn(BrowseGrid);
			IDCol->TagString = GetIDColumnName();
			IDCol->ReadOnly = true;
			IDCol->Visible = false;
			IDCol->Parent = BrowseGrid;
			Query->First();
			while(!Query->Eof)
			{
				TStringColumn *tempCol = new TStringColumn(BrowseGrid);
				tempCol->Header = Query->FieldByName("Title")->AsString;
				tempCol->Width =  Query->FieldByName("Width")->AsFloat;
				tempCol->TagString = Query->FieldByName("Name")->AsString;
				tempCol->ReadOnly = true;
				tempCol->Visible = Query->FieldByName("Visible")->AsString.Trim().LowerCase() == "true";
				tempCol->Parent = BrowseGrid;
				Query->Next();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::LoadGridData(TJSONArray *FilterParam)
{
    BrowseGrid->RowCount = 0;
	if(!FGridDataUrl.IsEmpty())
	{
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("FilterParam",FilterParam->ToString());

		Query = csGetData(FGridDataUrl,Params);
		if(Query->Active && Query->RecordCount > 0)
		{
			BusinessDataSet->Data = Query->Data;
			BrowseGrid->RowCount = Query->RecordCount;
			Query->First();
			int row = 0;
			while(!Query->Eof)
			{
				for(int i=0;i<BrowseGrid->ColumnCount;i++)
				{
					TStringColumn *tempCol = dynamic_cast<TStringColumn *>(BrowseGrid->Columns[i]);
					BrowseGrid->Cells[i][row] = Query->FieldByName(tempCol->TagString)->AsString;
				}
				Query->Next();
				row++;
			}
		}
	}
	if(BrowseGrid->RowCount>0)
     	BrowseGrid->SelectRow(0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::ShowEditForm()
{
	TfrmWorkFlowDetailForm *DetailForm;
	try
	{
		TForm *p = NewEditForm();  //获取编辑窗体指针
		if(p)
		{
			DetailForm =dynamic_cast<TfrmWorkFlowDetailForm *>(p); //类型转换
			if(DetailForm)
			{
				DetailForm->OnClose = DetailClose;
				DetailForm->Show();
			}
		}
		else
			ShowMessage(L"未实现编辑窗体!");
	}
	catch(System::Sysutils::Exception &e)
	{
		throw System::Sysutils::Exception(e.Message);
	}
}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmWorkFlowBrowseForm::NewEditForm()
{
  return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWorkFlowBrowseForm::BtnShowInfoClick(TObject *Sender)
{
   	ShowEditForm();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::InitFilter()
{
  	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("MenuID",ModuleCode);
	Params->AddItem("FlowID",FFlowID);
	Query = csGetData("/WorkFlow/WorkFlowBusinessPage/GetUserWorkWfsArray",Params);
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
        while(!Query->Eof)
		{
			TListBoxItem * newBoxItem=new TListBoxItem(cbStatusFilter);
			newBoxItem->Text = Query->FieldByName("text")->AsString;
			newBoxItem->TagString = Query->FieldByName("id")->AsString;
			cbStatusFilter->AddObject(newBoxItem);
			if(StrToInt(Query->FieldByName("id")->AsString) == FFilterState)
             	newBoxItem->IsSelected = true;
			Query->Next();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::filteData()
{
	int Status = FFilterState;
	if(cbStatusFilter->Selected && !cbStatusFilter->Selected->TagString.IsEmpty())
		Status = StrToInt(cbStatusFilter->Selected->TagString);

	TJSONArray *FilterParam = new TJSONArray();
	TJSONObject *FlowIDEqu = new TJSONObject();
	FlowIDEqu->AddPair("Key","FlowID");
	FlowIDEqu->AddPair("Value",FFlowID);
	FlowIDEqu->AddPair("Oper","AddEqualTo");
	TJSONObject *FilterEqu = new TJSONObject();
	FilterEqu->AddPair("Key","WfsID");
	FilterEqu->AddPair("Value",Status);
	FilterEqu->AddPair("Oper","AddEqualTo");
	FilterParam->AddElement(FlowIDEqu);
	FilterParam->AddElement(FilterEqu);
	LoadGridData(FilterParam);
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::btnQueryClick(TObject *Sender)
{
	filteData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkFlowBrowseForm::DetailClose(TObject *Sender, TCloseAction &Action)
{
	TfrmWorkFlowDetailForm *DetailForm =dynamic_cast<TfrmWorkFlowDetailForm *>(Sender);
	if(DetailForm && DetailForm->NeedRefresh)
	{
		filteData();
    }
}
//---------------------------------------------------------------------------
