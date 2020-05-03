//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "CurrencyDataEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DetailBaseForm"
#pragma resource "*.fmx"
TfrmCurrencyDataEdit *frmCurrencyDataEdit;
//---------------------------------------------------------------------------
__fastcall TfrmCurrencyDataEdit::TfrmCurrencyDataEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet
								*FDataSet,int DataSetStatus)
	: TfrmDetailBaseForm( Owner, clBroker,ModuleCode,FDataSet, DataSetStatus)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCurrencyDataEdit::FormInitControl()
{
  LockControl(1,seCurDataPKID,false,true,false);
  LockControl(1,seCurDataName,false,true,true);
  LockControl(1,chkCurDataIsStandard,false,true,false);
  LockControl(1,seCurDataExchangeRate,false,true,true);
//  CtrlFloatInput(seCurDataExchangeRate,ftRate);
  WaitInputControl(1,seCurDataPKID);
}
void __fastcall TfrmCurrencyDataEdit::FormClearControl(int Section)
{
   seCurDataPKID->Text="";
  seCurDataName->Text="";
  seCurDataExchangeRate->Text="";
  chkCurDataIsStandard->IsChecked =false ;
}
void __fastcall TfrmCurrencyDataEdit::SendDataToDataSet(int Section)
{
   SetFieldValue("CurDataPKID",seCurDataPKID->Text);
  SetFieldValue("CurDataName",seCurDataName->Text);
  SetFieldValue("CurDataExchangeRate",seCurDataExchangeRate->Text);
  SetFieldValue("CurDataIsStandard",chkCurDataIsStandard->IsChecked ? "1":"0");
}
void __fastcall TfrmCurrencyDataEdit::GetDataFromDataSet(int Section)
{
  seCurDataPKID->Text=GetFieldValue("CurDataPKID");
  seCurDataName->Text=GetFieldValue("CurDataName");
  seCurDataExchangeRate->Text=GetFieldValue("CurDataExchangeRate");
  chkCurDataIsStandard->IsChecked=StrToInt(GetFieldValue("CurDataIsStandard"));
}
void __fastcall TfrmCurrencyDataEdit::VerifyFieldData(int Section,int status)
{
   if(seCurDataPKID->Text.Trim().IsEmpty())
  {
    seCurDataPKID->SetFocus();
    throw Exception("��������Ҵ���");
  }
  if( status==1||VarToStr(GetFieldValue("CurDataPKID"))!=seCurDataPKID->Text)
  {
    Query->Close();
	Query->SQL->Text="select * from tbCurrencyData where CurDataPKID="+QuotedStr(seCurDataPKID->Text)+"";
    Query->Open();
    if(Query->RecordCount>0)
	{
      Query->Close();
      seCurDataPKID->SetFocus();
      throw Exception("["+seCurDataPKID->Text+"]���Ҵ����Ѵ���");
    }
    Query->Close();
  }
  if(seCurDataName->Text.Trim().IsEmpty())
  {
	if(seCurDataName->CanFocus)
    {
	  seCurDataName->SetFocus();
      throw Exception("�������������");
    }
  }
  if(seCurDataExchangeRate->Text.Trim().IsEmpty())
  {
	if(seCurDataExchangeRate->CanFocus)
    {
      seCurDataExchangeRate->SetFocus();
      throw Exception("���������");
    }
  }
  if(seCurDataExchangeRate->Text.Trim().ToDouble()<=0)
  {
	if(seCurDataExchangeRate->CanFocus)
    {
      seCurDataExchangeRate->SetFocus();
      throw Exception("����Ҫ����0");
    }
  }
  if(GetFieldValue("CurDataIsStandard")!=1&&chkCurDataIsStandard->IsChecked)
  {
    Query->Close();
    Query->SQL->Text="select * from tbCurrencyData where CurDataIsStandard=1";
    Query->Open();
    if(Query->RecordCount>=1)
    {
      Query->Close();
      throw Exception("�Ѵ��ڱ�λ�ң���λ��ֻ����һ�֡�");
    }
    Query->Close();
  }
}
void __fastcall TfrmCurrencyDataEdit::chkCurDataIsStandardChange(TObject *Sender)

{
   if (chkCurDataIsStandard->IsChecked)
  {
	seCurDataExchangeRate->Text=1;
	seCurDataExchangeRate->Enabled=false;
	LockControl(1,seCurDataExchangeRate,false,true,false);
	LockControl(1,seCurDataName,false,true,false);
  }
  else
  {
	seCurDataExchangeRate->Enabled=true;
	LockControl(1,seCurDataExchangeRate,false,true,true);
	LockControl(1,seCurDataName,false,true,true);
  }

}
//---------------------------------------------------------------------------
bool __fastcall TfrmCurrencyDataEdit::BeforeUpdateData(int OptType)
{
     switch(OptType)
   {
	  case 1:   //Add
         return true;
	  case 2:  //Edit
         return true;
      case 3:  //Del
          if(GetFieldValue("CurDataIsStandard")==1)
          {
 			  ShowMessage(_D("ϵͳʹ���˵�ǰ���֣�����ɾ��"));
			  return false;
          }
          return true;

      default:
          return true;
   }

}

