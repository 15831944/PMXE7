//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ItemBaseForm.h"
#include "FSTMessageDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseBillForm"
#pragma resource "*.fmx"
TfrmItemBaseForm *frmItemBaseForm;
//---------------------------------------------------------------------------
__fastcall TfrmItemBaseForm::TfrmItemBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus)
	:TfrmBaseBillForm(Owner, clBroker,ModuleCode,FDataSet, DataSetStatus)
{
	ItemVertScrollBox->OnCalcContentBounds = CalcContentBoundsProc;
}
//---------------------------------------------------------------------------
void __fastcall TfrmItemBaseForm::SetOptCtrlStatus(int status)    //���غ��������ô���״̬
{
  OperateStatus = status;
  if(OperateRight.Pos(FR_SYSTEMWRITE) <= 0 && OperateRight.Pos(FR_SYSTEMREAD) > 0) //ֻ��Ȩ��
  {
		ItemEditAction->Enabled = false;
		ItemSaveAction->Enabled = false;
  }
  else
  {
    switch(status) //����״̬����
    {
      case 0: //���
		ItemEditAction->Enabled = true;
		ItemSaveAction->Enabled = false;
		break;
	  case 3: //����
		ItemEditAction->Enabled = false;
		ItemSaveAction->Enabled = true;
		break;
	  case 4: //�༭
		ItemEditAction->Enabled = true;
		ItemSaveAction->Enabled = false;
		break;
	  default:
		break;
    }
  }
  NextItemAction->Enabled=(!DetailEof)&&(DetailCount>0);
  PriorItemAction->Enabled=(!DetailBof)&&(DetailCount>0);
  SetUserCtrlStatus();    //��״̬�����û�����״̬
}
void __fastcall TfrmItemBaseForm::ItemEditActionExecute(TObject *Sender)
{
	 this->EditItemData();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::ItemSaveActionExecute(TObject *Sender)
{
     this->SaveItemData();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::ItemEditActionUpdate(TObject *Sender)
{
  btEdit->Visible=ItemEditAction->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::ItemSaveActionUpdate(TObject *Sender)
{
	btSave->Visible=ItemSaveAction->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::CalcContentBoundsProc(TObject * Sender, System::Types::TRectF &ContentBounds)
{
	if (FNeedOffset && FKBBounds.Top > 0) {
		ContentBounds.Bottom = Max(ContentBounds.Bottom, 2 * ClientHeight - FKBBounds.Top);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::FormFocusChanged(TObject *Sender)
{
   	FKBBounds = TRectF(0,0,0,0);
	UpdateKBBounds();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
	FKBBounds = TRectF(0,0,0,0);
	FNeedOffset = False;
	RestorePosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
	FKBBounds = TRectF(Bounds);
	FKBBounds = TRectF(ScreenToClient(FKBBounds.TopLeft()),ScreenToClient(FKBBounds.BottomRight()));
	UpdateKBBounds();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::UpdateKBBounds()
{
	FNeedOffset = false;
	if (Focused != NULL) {
		TControl * LFocused = static_cast<TControl*>(Focused->GetObject());
		TRectF LFocusRect = LFocused->AbsoluteRect;
		LFocusRect.Offset(ItemVertScrollBox->ViewportPosition.X, ItemVertScrollBox->ViewportPosition.Y);
		if (LFocusRect.IntersectsWith(FKBBounds) && LFocusRect.Bottom > FKBBounds.Top) {
			FNeedOffset =true;// ;
			EditLayout->Align = TAlignLayout::Horizontal;
			ItemVertScrollBox->RealignContent();
			Application->ProcessMessages();
			ItemVertScrollBox->ViewportPosition = PointF(ItemVertScrollBox->ViewportPosition.X,
				LFocusRect.Bottom - FKBBounds.Top);
		}
	}
	if (!FNeedOffset) {
		RestorePosition();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::RestorePosition()
{
	ItemVertScrollBox->ViewportPosition = PointF(ItemVertScrollBox->ViewportPosition.X, 0);
	EditLayout->Align = TAlignLayout::Client;
	ItemVertScrollBox->RealignContent();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::NextItemActionExecute(TObject *Sender)
{
	if(DataSetType==2 && DetailStatus!=dsBrowse) //���ڱ༭״̬
	{
		TMessageDialogEventProc msgDialogCloseProc=&DoCancelDetailChange;
		ShowMessageBox(L"��ϸ�޸�δ���棬�Ƿ�ȷ��ȡ����",msgDialogCloseProc);
	}
	else
	 this->DetailNext();

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::PriorItemActionExecute(TObject *Sender)
{
	if(DataSetType==2 && DetailStatus!=dsBrowse) //���ڱ༭״̬
	{
		TMessageDialogEventProc msgDialogCloseProc=&DoCancelDetailChange;
		ShowMessageBox(L"��ϸ�޸�δ���棬�Ƿ�ȷ��ȡ����",msgDialogCloseProc);
	}
	else
	  this->DetailPrior();
}
//---------------------------------------------------------------------------
void __fastcall TfrmItemBaseForm::DoCancelDetailChange(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
   CancelDetailChange();
  }
}

//---------------------------------------------------------------------------
void __fastcall TfrmItemBaseForm::CloseActionExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmItemBaseForm::FormCloseQuery(TObject *Sender, bool &CanClose)

{
	if(DataSetType==2 && DetailStatus!=dsBrowse) //���ڱ༭״̬
	{
		CanClose=false;
		TMessageDialogEventProc msgDialogCloseProc=&DoCancelDetailChange;
		ShowMessageBox(L"��ϸ�޸�δ���棬�Ƿ�ȷ��ȡ����",msgDialogCloseProc);

	}
}
//---------------------------------------------------------------------------


