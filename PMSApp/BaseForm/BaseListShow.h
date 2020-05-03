//---------------------------------------------------------------------------

#ifndef BaseListShowH
#define BaseListShowH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseListForm.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.ActnList.hpp>
#include <System.Actions.hpp>
#include "BaseListForm.h"
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
//---------------------------------------------------------------------------
class TfrmBaseListShow : public TfrmBaseListForm
{
__published:	// IDE-managed Components
	TToolBar *ListToolBar;
	TButton *btAdd;
	TButton *btDelete;
	TButton *btQuery;
	TActionList *BaseListActionList;
	TAction *LookAction;
	TAction *AddAction;
	TAction *DelAction;
	TAction *QueryAction;
	TAction *EditAction;
	TButton *btClose;
	TAction *CloseAction;
	TBindSourceDB *GridBindSourceDB;
	TBindingsList *ListFormBindingsList;
	TButton *btLook;
	TToolBar *ListBottomToolBar;
	TLabel *lbListTitle;
	void __fastcall LookActionExecute(TObject *Sender);
	void __fastcall DelActionExecute(TObject *Sender);
	void __fastcall QueryActionExecute(TObject *Sender);
	void __fastcall EditActionExecute(TObject *Sender);
	void __fastcall CloseActionExecute(TObject *Sender);
	void __fastcall AddActionExecute(TObject *Sender);
private:	// User declarations
protected:
  void __fastcall SetOptCtrlStatus(int status);  //�����麯�������ô���״̬
  virtual void __fastcall SetUserCtrlStatus(){}; //�麯���������û�����״̬
  virtual void __fastcall InitListShowForm(){};
public:		// User declarations
	__fastcall TfrmBaseListShow(TComponent* Owner);
	__fastcall TfrmBaseListShow(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param);
	__fastcall TfrmBaseListShow(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,String Param);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmBaseListShow *frmBaseListShow;
//---------------------------------------------------------------------------
#endif
