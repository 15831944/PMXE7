//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Room.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseForm"
#pragma resource "*.fmx"
TfrmRoom *frmRoom;
//---------------------------------------------------------------------------
__fastcall TfrmRoom::TfrmRoom(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param)
	: TfrmBaseForm(Owner,clBroker,modulecode,param)
{
    FCurBtn = NULL;
	EditForm=NULL;
	FDict = new TStringList();
	Query = csGetData("/SalesManagement/tbEstate/GetAllData",new TRESTRequestParameterList(this));
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem *newBoxItem=new TListBoxItem(cbEstateFilter);
			newBoxItem->Text=Query->FieldByName("Name")->AsString;
			newBoxItem->TagString=Query->FieldByName("FID")->AsString;
			cbEstateFilter->AddObject(newBoxItem);
			Query->Next();
		}
	}

	GetDictionary();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoom::cbEstateFilterChange(TObject *Sender)
{
    cbBuildingFilter->Clear();
	String FEstateID =  cbEstateFilter->Selected->TagString;
	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("EstateID",FEstateID);
	Query = csGetData("/SalesManagement/tbBuilding/GetDataByEstateID",Params);
	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem *newBoxItem=new TListBoxItem(cbBuildingFilter);
			newBoxItem->Text=Query->FieldByName("Name")->AsString;
			newBoxItem->TagString=Query->FieldByName("FID")->AsString;
			newBoxItem->ItemData->Detail = Query->FieldByName("FloorQty")->AsString + "\t" + Query->FieldByName("FloorRoomQty")->AsString;
			cbBuildingFilter->AddObject(newBoxItem);
			Query->Next();
        }
	}
	if(cbBuildingFilter->Count>0)
		cbBuildingFilter->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoom::GetDictionary()
{
    StatusLayout->DeleteChildren();
	FDict->Clear();
	Query = csGetData("/Common/MobileService/GetDictData?IDArray=24,16,19,17,18,39,25",new TRESTRequestParameterList(this));
	 _di_ITextSettings Settings;
	if(Query->Active && Query->RecordCount > 0)
	{
        Query->First();
		int i=0;
		TAlphaColor fcolor;
		while(!Query->Eof)
		{
			FDict->Add(Query->FieldByName("FID")->AsString);
			FDict->Values[FDict->Count -1] =  Query->FieldByName("Name")->AsString;

			if(Query->FieldByName("DtID")->AsInteger == 24)
			{
				switch(i)
				{
					case 0: fcolor = TAlphaColor(claGray);
							break;
					case 1: fcolor = TAlphaColor(claBlack);
							break;
					case 2: fcolor = TAlphaColor(claTeal);
							break;
					case 3: fcolor = TAlphaColor(claGreen);
							break;
					case 4: fcolor = TAlphaColor(claYellow);
							break;
					case 5: fcolor = TAlphaColor(claBlue);
							break;
					case 6: fcolor = TAlphaColor(claRed);
							break;
					default: fcolor =TAlphaColor(claGray); //random()%4294967296 - 2147483648;
							break;
				}

				TButton *tempButton = new TButton(StatusLayout);
				tempButton->Align = TAlignLayout::Client;
				tempButton->StyleLookup = "toolbuttonlabel";
				tempButton->Text = Query->FieldByName("Name")->AsString;
				tempButton->Name =  "StatusBtn_"+Query->FieldByName("FID")->AsString;
				if (tempButton->GetInterface(Settings)) {    //Ҫ����������ɫ��Ҫ������ʽ�Ŀ���
					Settings->TextSettings->BeginUpdate();
					try {
						if (Settings->StyledSettings.Contains(TStyledSetting::FontColor)) {
							Settings->StyledSettings =
								Settings->StyledSettings >> TStyledSetting::FontColor;
						}
					}
					catch (System::Sysutils::Exception* e) {
					Settings->TextSettings->EndUpdate();
				}
					Settings->TextSettings->EndUpdate();
				}
				tempButton->FontColor= fcolor;
			   //	tempButton->TintColor = (TAlphaColor)fcolor;
				tempButton->Width = 80;
				tempButton->Height = 40;
				tempButton->Parent = StatusLayout;
				StatusLayout->AddObject(tempButton);
				i++;
			}
			Query->Next();
		}
        StatusLayout->Width = i * 80;
		StatusLayout->Height = 40;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoom::FillRoomsData(int FloorQty,int FloorRoomQty)
{
	BuildingLayout->DeleteChildren();
	BuildingLayout->Width = (FloorRoomQty + 1) * 80;
	BuildingLayout->Height = (FloorQty + 1) * 40;

	if(FloorQty>0 && FloorRoomQty>0)
	{
		TPanel *tempPanel = new TPanel(this);
		tempPanel->Parent = BuildingLayout;
		tempPanel->Width = 80;
		tempPanel->Height = 40;
		BuildingLayout->AddObject(tempPanel);
		for(int i=1;i<FloorRoomQty+1;i++)
		{
			tempPanel = new TPanel(this);
			tempPanel->Width = 80;
			tempPanel->Height = 40;
			TLabel *tempLabel = new TLabel(this);
			tempLabel->Text = IntToStr(i);
			tempLabel->Align = TAlignLayout::Client;
			tempLabel->TextSettings->HorzAlign = TTextAlign::Center;
			tempLabel->TextSettings->Font->Style << fsBold;
			tempLabel->Parent = tempPanel;
			tempPanel->Parent = BuildingLayout;

			BuildingLayout->AddObject(tempPanel);
		}
        	 _di_ITextSettings Settings;
		String FEstateID =  cbEstateFilter->Selected->TagString;
		String FBuildindID = cbBuildingFilter->Selected->TagString;
		TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
		Params->AddItem("EstID",FEstateID);
		Params->AddItem("BuildID",FBuildindID);
		Query = csGetData("/SalesManagement/tbRoom/GetRooms",Params);
		if(Query->Active && Query->RecordCount > 0)
		{
			if(Query->FieldByName("success")->AsString.Trim().LowerCase() == "true")
			{
				DataSet->Data = GetElementData("data")->Data;
				if(DataSet->Active && DataSet->RecordCount > 0)
				{
					DataSet->First();
					int floor = -1;
					while(!DataSet->Eof)
					{
						if(floor != DataSet->FieldByName("FloorNum")->AsInteger)
						{
							floor = DataSet->FieldByName("FloorNum")->AsInteger;
							TFlowLayoutBreak *tempBreak = new TFlowLayoutBreak(this);
							tempBreak->Parent = BuildingLayout;
							BuildingLayout->AddObject(tempBreak);

							tempPanel = new TPanel(this);
							tempPanel->Width = 80;
							tempPanel->Height = 40;
							TLabel *tempLabel = new TLabel(this);
							tempLabel->Text = L"��"+IntToStr(floor)+L"¥";
							tempLabel->Align = TAlignLayout::Client;
							tempLabel->TextSettings->HorzAlign = TTextAlign::Center;
							tempLabel->TextSettings->Font->Style << fsBold;
							tempLabel->Width = 80;
							tempLabel->Height = 40;
							tempLabel->Parent = tempPanel;
							tempPanel->Parent = BuildingLayout;
							BuildingLayout->AddObject(tempPanel);
						}
						TButton *tempButton = new TButton(this);
						tempButton->Align = TAlignLayout::Client;
						tempButton->StyleLookup = "toolbuttonlabel";
//						tempButton->StyleName = "Custombuttonstyle";
						tempButton->Text = DataSet->FieldByName("Name")->AsString;
						tempButton->TagString = DataSet->FieldByName("FID")->AsString;
						tempButton->Name = "RoomBtn_" + DataSet->FieldByName("FID")->AsString;
						String StatusBtnName = "StatusBtn_"+DataSet->FieldByName("Status")->AsString;
						TButton *tStaBtn = (TButton *)StatusLayout->FindComponent(StatusBtnName);
						if(tStaBtn != NULL)
						{
							if (tempButton->GetInterface(Settings)) {
								Settings->TextSettings->BeginUpdate();
								try {
									if (Settings->StyledSettings.Contains(TStyledSetting::FontColor)) {
										Settings->StyledSettings =
											Settings->StyledSettings >> TStyledSetting::FontColor;
									}
								}
								catch (System::Sysutils::Exception* e) {
								Settings->TextSettings->EndUpdate();
							}
								Settings->TextSettings->EndUpdate();
							}
						   tempButton->FontColor = tStaBtn->FontColor;
						  //	tempButton->TintColor = tStaBtn->TintColor;
						}
						tempButton->Width = 80;
						tempButton->Height = 40;
						tempButton->OnClick = RoomClick;
						tempButton->Parent = BuildingLayout;
						BuildingLayout->AddObject(tempButton);

						DataSet->Next();
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoom::RoomClick(TObject *Sender)
{
	TButton *roomBtn = (TButton*)Sender;
	if(roomBtn != FCurBtn)
	{
		FCurBtn = roomBtn;
		return;
    }
	TLocateOptions ops;
	ops.Clear();
	ops << loCaseInsensitive;

	String roomID = roomBtn->Name;
	roomID = roomID.SubString1(9,roomID.Length() - 8);
	if(DataSet->Locate("FID",roomID,ops))
	{
		if(EditForm == NULL)
		{
			EditForm = new TfrmRoomEditForm(this,ClientBroker,ModuleCode,DataSet,"");
			EditForm->StyleBook = this->StyleBook;
		}
		EditForm->Dictionary = FDict;
		EditForm->EstateName = cbEstateFilter->Selected->Text;
		EditForm->BuildName = cbBuildingFilter->Selected->Text;
		EditForm->FillData();
        EditForm->Show();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRoom::btnQueryClick(TObject *Sender)
{
	int FloorQty = 0;
	int FloorRoomQty = 0;
	String ItemData = cbBuildingFilter->Selected->ItemData->Detail;
	int pos = ItemData.Pos0("\t");
	if(pos > 0)
	{
		FloorQty = StrToInt(ItemData.SubString1(1,pos).Trim());
		FloorRoomQty = StrToInt(ItemData.SubString1(pos+2,ItemData.Length()-pos-1).Trim());
	}
	try
	{
		QueryView->HideMaster();
		BuildingLayout->BeginUpdate();
		FillRoomsData(FloorQty,FloorRoomQty);
	}
	__finally
	{
		BuildingLayout->EndUpdate();
    }
}
//---------------------------------------------------------------------------

