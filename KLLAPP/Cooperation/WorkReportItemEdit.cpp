//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "WorkReportItemEdit.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemBaseForm"
#pragma resource "*.fmx"
TfrmWorkReportItemEdit *frmWorkReportItemEdit;
//---------------------------------------------------------------------------
__fastcall TfrmWorkReportItemEdit::TfrmWorkReportItemEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus)
	: TfrmItemBaseForm(Owner, clBroker,ModuleCode,FDataSet, DataSetStatus)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::FormInitControl()   //��ʼ�����ݿؼ�
{
	FCheckFlag=GetMasterValue("WorkReportCheckState");
	FGuidBillID=GetMasterValue("WorkReportID");
	LockControl(2,spWorkDetailIndex,false,true,true);
	LockControl(2,edWorkDetailDataTime,false,true,true);
	LockControl(2,edWorkDetailProssState,false,true,true);
	LockControl(2,edWorkDetailNextContent,false,true,true);
	LockControl(2,mmWorkDetailContent,false,true,true);
	LockControl(2,spWorkDetailScore,false,true,true);
	LockControl(2,spWorkDetailRate,false,true,true);
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::FormClearControl(int Section)   //��տؼ�����,Section=2
{
	FDetailID = GetGUIDString();
	spWorkDetailIndex->Text="";
	edWorkDetailDataTime->Text="";
	edWorkDetailProssState->Text="";
	edWorkDetailNextContent->Text="";
	mmWorkDetailContent->Text="";
	spWorkDetailScore->Text="0";
	spWorkDetailRate->Text="0";
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::SendDataToDataSet(int Section)  //�ؼ����ݴ������ݼ�,Section=2
{
	SetDetailValue("WorkDetailReportID",FGuidBillID);
	SetDetailValue("WorkDetailID",FDetailID);
	SetDetailValue("WorkDetailIndex",StrToInt(spWorkDetailIndex->Text));
	SetDetailValue("WorkDetailDataTime",edWorkDetailDataTime->Text);
	SetDetailValue("WorkDetailProssState",edWorkDetailProssState->Text);
	SetDetailValue("WorkDetailNextContent",edWorkDetailNextContent->Text);
	SetDetailValue("WorkDetailContent",mmWorkDetailContent->Text);
	SetDetailValue("WorkDetailScore",GetFloatFromStr(spWorkDetailScore->Text));
	SetDetailValue("WorkDetailRate",GetFloatFromStr(spWorkDetailRate->Text));
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::GetDataFromDataSet(int Section) //���ݼ�������ʾ�ڿؼ���,Section=2
{
	FGuidBillID = GetMasterValue("WorkReportID");
	FDetailID = GetDetailValue("WorkDetailID");
	spWorkDetailIndex->Text=GetDetailValue("WorkDetailIndex");
	edWorkDetailDataTime->Text=GetDetailValue("WorkDetailDataTime");
	edWorkDetailProssState->Text=GetDetailValue("WorkDetailProssState");
	edWorkDetailNextContent->Text=GetDetailValue("WorkDetailNextContent");
	mmWorkDetailContent->Text=GetDetailValue("WorkDetailContent");
	spWorkDetailScore->Text=GetDetailValue("WorkDetailScore");
	spWorkDetailRate->Text=GetDetailValue("WorkDetailRate");
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::VerifyFieldData(int Section,int status) //����ǰУ������,Section=2��status��3-������4-�޸�
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::SetUserCtrlStatus()//�����û�����״̬
{
  bool bChecked= FCheckFlag>0;
  bool bWriteRight=OperateRight.Pos(FR_SYSTEMWRITE)>0;
  bool bCheckRight=FormDefRight.Pos(FR_FRMSTUFFSTOCKINLIST_CHECKRIGHT) > 0;
  bool bPriceCheck=FormDefRight.Pos(FR_FRMSTUFFSTOCKINLIST_CHECKPRICE) > 0;
  bool bBrowse= DetailStatus==dsBrowse;
  ItemEditAction->Enabled= bWriteRight && !bChecked && bBrowse;
  ItemSaveAction->Enabled=bWriteRight && !bChecked && !bBrowse;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportItemEdit::DetailDataSetBeforePost(TDataSet* DataSet)
{

}
//---------------------------------------------------------------------------

