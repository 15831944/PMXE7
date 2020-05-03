//---------------------------------------------------------------------------

#ifndef ItemBaseFormH
#define ItemBaseFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseBillForm.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TfrmItemBaseForm : public TfrmBaseBillForm
{
__published:	// IDE-managed Components
	TToolBar *ItemMainToolBar;
	TButton *btSave;
	TButton *btBaseClose;
	TButton *btEdit;
	TActionList *ItemActionList;
	TAction *ItemEditAction;
	TAction *ItemSaveAction;
	TVertScrollBox *ItemVertScrollBox;
	TLayout *EditLayout;
	TAction *NextItemAction;
	TAction *PriorItemAction;
	TButton *btNextItem;
	TButton *btPriorItem;
	TAction *CloseAction;
	void __fastcall ItemEditActionExecute(TObject *Sender);
	void __fastcall ItemSaveActionExecute(TObject *Sender);
	void __fastcall ItemEditActionUpdate(TObject *Sender);
	void __fastcall ItemSaveActionUpdate(TObject *Sender);
	void __fastcall FormFocusChanged(TObject *Sender);
	void __fastcall FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall NextItemActionExecute(TObject *Sender);
	void __fastcall PriorItemActionExecute(TObject *Sender);
	void __fastcall CloseActionExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// User declarations
	void __fastcall DoCancelDetailChange(System::Uitypes::TModalResult AResult);
	void __fastcall RestorePosition();
	void __fastcall UpdateKBBounds();
	void __fastcall CalcContentBoundsProc(TObject * Sender, System::Types::TRectF &ContentBounds);
	System::Types::TRectF FKBBounds;
	bool FNeedOffset;
protected:  //virtual function
  void __fastcall SetOptCtrlStatus(int status);    //���غ��������ô���״̬
  virtual void __fastcall FormInitControl(){};   //��ʼ�����ݿؼ�
  virtual void __fastcall FormClearControl(int Section){};   //��տؼ�����
  virtual void __fastcall SendDataToDataSet(int Section){};  //�ؼ����ݴ������ݼ�
  virtual void __fastcall GetDataFromDataSet(int Section){}; //���ݼ�������ʾ�ڿؼ���
  virtual void __fastcall VerifyFieldData(int Section,int status){}; //����ǰУ�����ݣ�status��1-������2-�޸�
  virtual void __fastcall SetUserCtrlStatus(){};//�����û�����״̬
public:		// User declarations
	__fastcall TfrmItemBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmItemBaseForm *frmItemBaseForm;
//---------------------------------------------------------------------------
#endif