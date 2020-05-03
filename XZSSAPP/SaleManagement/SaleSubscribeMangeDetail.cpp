//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "SaleSubscribeMangeDetail.h"
#include <System.DateUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseBillForm"
#pragma resource "*.fmx"
TfrmSaleSubscribeMangeDetail *frmSaleSubscribeMangeDetail;
//---------------------------------------------------------------------------
__fastcall TfrmSaleSubscribeMangeDetail::TfrmSaleSubscribeMangeDetail(TComponent* Owner,TClientBroker *clBroker,int modulecode,TClientDataSet *m_DataSet,String param)
	: TfrmBaseBillForm(Owner,clBroker,modulecode,m_DataSet,param)
{
	InitBaseData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::InitDictionary()
{
	cbProperty->Clear();
	cbDecoIsCal->Clear();
	cbDecoType->Clear();
	Query = csGetData("/Common/MobileService/GetDictData?IDArray=19,45,55",new TRESTRequestParameterList(this));

	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			if(Query->FieldByName("DtID")->AsString == "19")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbDecoType);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbDecoType->AddObject(newBoxItem);
                cbDecoType->ItemIndex = -1;
			}
			else if(Query->FieldByName("DtID")->AsString == "45")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbProperty);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbProperty->AddObject(newBoxItem);
				if(Query->FieldByName("FID")->AsString == "3715")
					newBoxItem->IsSelected = true;
			}
			else if(Query->FieldByName("DtID")->AsString == "55")
			{
				TListBoxItem * newBoxItem=new TListBoxItem(cbDecoIsCal);
				newBoxItem->Text = Query->FieldByName("Name")->AsString;
				newBoxItem->TagString = Query->FieldByName("FID")->AsString;
				cbDecoIsCal->AddObject(newBoxItem);
            }
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::InitClientData()
{
	cbClientID->Clear();
    Query = csGetData("/Work/tbClient/GetAllData",new TRESTRequestParameterList(this));

	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem * newBoxItem=new TListBoxItem(cbClientID);
			newBoxItem->Text = Query->FieldByName("Name")->AsString;
			newBoxItem->TagString = Query->FieldByName("FID")->AsString;
			cbClientID->AddObject(newBoxItem);
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::InitPayModeData()
{
	cbPayMode->Clear();
    Query = csGetData("/SalesManagement/tbSalePayMode/GetAllData",new TRESTRequestParameterList(this));

	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem * newBoxItem=new TListBoxItem(cbPayMode);
			newBoxItem->Text = Query->FieldByName("Name")->AsString;
			newBoxItem->TagString = Query->FieldByName("FID")->AsString;
			cbPayMode->AddObject(newBoxItem);
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::InitDiscountData()
{
	cbDiscount->Clear();
	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("BuildingID",DataSet->FieldByName("BuildingID")->AsInteger);
	Query = csGetData("/Work/tbSaleDiscountMode/GetList",Params);

	if(Query->Active && Query->RecordCount > 0)
	{
		Query->First();
		while(!Query->Eof)
		{
			TListBoxItem * newBoxItem=new TListBoxItem(cbDiscount);
			newBoxItem->Text = Query->FieldByName("text")->AsString;
			newBoxItem->TagString = Query->FieldByName("id")->AsString;
			cbDiscount->AddObject(newBoxItem);
			Query->Next();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::InitBaseData()
{
  	InitDictionary();
	InitClientData();
	InitPayModeData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::FillData()
{
	if(DataSet->Active && DataSet->RecordCount > 0)
	{
		edEstateName->Text = EstateName;
		edEstateName->TagString = DataSet->FieldByName("EstateID")->AsString;
		edBuildName->Text = BuildName;
		edBuildName->TagString = DataSet->FieldByName("BuildingID")->AsString;
		edRoomName->Text = DataSet->FieldByName("Name")->AsString;
		edRoomName->TagString = DataSet->FieldByName("FID")->AsString;
		cbAreaType->ItemIndex = DataSet->FieldByName("AreaType")->AsInteger;
		edFloorArea->Text = DataSet->FieldByName("FloorArea")->AsString;
		edInnerArea->Text = DataSet->FieldByName("InnerArea")->AsString;
		edFloorAreaPrice->Text = DataSet->FieldByName("FloorAreaPrice")->AsString;
		edInnerAreaPrice->Text = DataSet->FieldByName("InnerAreaPrice")->AsString;
		if(DataSet->FieldByName("SellAmt")->AsString.IsEmpty())
			nbSellAmt->Value = 0;
		else
			nbSellAmt->Value = DataSet->FieldByName("SellAmt")->AsFloat;
		for(int i=0;i<cbDecoType->Count;i++)
		{
			if(cbDecoType->ListItems[i]->TagString == DataSet->FieldByName("Decoration")->AsString)
			{
				cbDecoType->ListItems[i]->IsSelected = true;
				break;
			}
		}
		InitDiscountData();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::SaveData()
{
	TJSONObject *formData = new TJSONObject();
	formData->AddPair("SellerID",ClientBroker->UserID);
	formData->AddPair("EstateID",edEstateName->TagString);
	formData->AddPair("BuildingID",edBuildName->TagString);
	formData->AddPair("RoomID",edRoomName->TagString);
	formData->AddPair("ClientID",cbClientID->Selected->TagString);
	if(cbPayMode->Selected)
		formData->AddPair("PayMode",cbPayMode->Selected->TagString);
	else
        formData->AddPair("PayMode","");
	formData->AddPair("Property",cbProperty->Selected->TagString);
	formData->AddPair("AreaType",cbAreaType->ItemIndex);
	formData->AddPair("FloorArea",DataSet->FieldByName("FloorArea")->AsFloat);
	formData->AddPair("InnerArea",DataSet->FieldByName("InnerArea")->AsFloat);
	formData->AddPair("Amt",nbSellAmt->Value);
	formData->AddPair("FloorPrice",DataSet->FieldByName("FloorAreaPrice")->AsFloat);
	formData->AddPair("InnerPrice",DataSet->FieldByName("InnerAreaPrice")->AsFloat);
	formData->AddPair("DealAmt",nbSellAmt->Value);
	formData->AddPair("DealFloorPrice",DataSet->FieldByName("FloorAreaPrice")->AsFloat);
	formData->AddPair("DealInnerPrice",DataSet->FieldByName("InnerAreaPrice")->AsFloat);
	if(cbDiscount->Selected)
		formData->AddPair("Discount",cbDiscount->Selected->TagString);
	else
        formData->AddPair("Discount","");
	formData->AddPair("DisAmt",nbDisAmt->Value);
	if(cbDecoIsCal->Selected)
		formData->AddPair("DecoIsCal",cbDecoIsCal->Selected->TagString);
	else
		formData->AddPair("DecoIsCal","");
	if(cbDecoType->Selected)
		formData->AddPair("DecoType",cbDecoType->Selected->TagString);
	else
		formData->AddPair("DecoType",DataSet->FieldByName("Decoration")->AsString);
	formData->AddPair("DecoPrice",nbDecoPrice->Value);
	formData->AddPair("DecoAmt",nbDecoAmt->Value);
	formData->AddPair("SubsAmt",0);
	formData->AddPair("AcceAmt",nbAcceAmt->Value);
	formData->AddPair("TotalAmt",nbTotalAmt->Value);
	formData->AddPair("SignDate",Today().FormatString("YYYY-MM-DD"));
	formData->AddPair("OverdueDate",NULL);
	formData->AddPair("BizDate",Today().FormatString("YYYY-MM-DD"));
	formData->AddPair("Remark",mmRemark->Text);
	TJSONArray *fundData = new TJSONArray();
	TJSONArray *ownerData = new TJSONArray();

	TJSONObject *postData = new TJSONObject();
	postData->AddPair("formData",formData);
	postData->AddPair("fundData",fundData);
	postData->AddPair("ownerData",ownerData);

	TRESTRequestParameterList *Params = new TRESTRequestParameterList(this);
	Params->AddItem("Data",postData->ToString());
	Query = PostData("/Work/tbSaleSubscribe/SaveAll",Params);
	if(Query->Active && Query->RecordCount > 0)
	{
		bool Success = Query->FieldByName("Success")->AsBoolean;
		if(Success)
		{
			ShowMessage(L"�ѳɹ�Ԥ������["+edRoomName->Text+L"]��");
			Close();
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
		ShowMessage(L"�ύʧ�ܣ����Ժ�����!");
}
//---------------------------------------------------------------------------
bool __fastcall TfrmSaleSubscribeMangeDetail::VerifyData()
{
	if(!cbClientID->Selected)
	{
		ShowMessage(L"��ѡ��ͻ�!");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSaleSubscribeMangeDetail::BtnSaveClick(TObject *Sender)
{
	if(VerifyData())
	{
      	SaveData();
    }
}
//---------------------------------------------------------------------------

