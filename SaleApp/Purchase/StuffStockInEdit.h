//---------------------------------------------------------------------------

#ifndef StuffStockInEditH
#define StuffStockInEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "DetailBaseForm.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <FMX.DateTimeCtrls.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Objects.hpp>
#include "StockInItemEdit.h"
#include <FMX.Gestures.hpp>
#include "QueryFilter.h"
//---------------------------------------------------------------------------
class TfrmStuffStockInEdit : public TfrmDetailBaseForm
{
__published:	// IDE-managed Components
	TStringGrid *ItemStringGrid;
	TBindSourceDB *ItemBindSourceDB;
	TBindGridLink *ItemBindGridLink;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn7;
	TStringColumn *StringColumn8;
	TStringColumn *StringColumn9;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TStringColumn *StringColumn12;
	TStringColumn *StringColumn13;
	TLabel *Label1;
	TEdit *seStuSkInNum;
	TLabel *Label2;
	TEdit *seStuSkInFKID_Rep;
	TLabel *Label3;
	TEdit *seStuSkInFKID_Cont;
	TEdit *seStuSkInSupplier;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *seStuSkInFKID_Unit;
	TEdit *seStuSkInTranNum;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *seStuSkInInvNum;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *seStuSkInMonth;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *seStuSkInCreator;
	TEdit *seStuSkInChecker;
	TEdit *deStuSkInDate;
	TEdit *sdCheckDate;
	TComboBox *cbCheckState;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TEditButton *ebStuSkInFKID_Rep;
	TEditButton *ebStuSkInFKID_Cont;
	TEditButton *ebStuSkInSupplier;
	TEditButton *ebStuSkInFKID_Unit;
	TEditButton *ebStuSkInMonth;
	TDateEdit *dtStuSkInInvDate;
	TLabel *Label15;
	TLabel *Label16;
	TEdit *edProjectName;
	TEdit *edDepotName;
	TEditButton *EditButton1;
	TEditButton *DepotNameButton;
	void __fastcall ebStuSkInFKID_RepClick(TObject *Sender);
	void __fastcall ebStuSkInFKID_ContClick(TObject *Sender);
	void __fastcall ebStuSkInSupplierClick(TObject *Sender);
	void __fastcall ebStuSkInFKID_UnitClick(TObject *Sender);
	void __fastcall ebStuSkInMonthClick(TObject *Sender);
	void __fastcall edWorkflowTitleEnter(TObject *Sender);
	void __fastcall DepotNameButtonClick(TObject *Sender);
private:	// User declarations

	int FFlag,FCheckFlag;
	int FPrjRight;
	bool IsBalance;
	//String FDepotCode,FDepotName,FPrjID;
	String FGuidBillID,FMasterID;
	TfrmStockInItemEdit *StockInItemEdit;
	TQueryFilterForm *InMonthFilter;
	TQueryFilterForm *InStoreFilter;
	TQueryFilterForm *SupplierSelectForm;
	void __fastcall FormInitControl();
	void __fastcall InitForm();
	void __fastcall FormClearControl(int Section);
	void __fastcall SendDataToDataSet(int Section);
	void __fastcall GetDataFromDataSet(int Section);
	void __fastcall VerifyFieldData(int Section,int status);
	bool __fastcall BeforeUpdateData(int OptType);
	void __fastcall CheckData(int oldstatus,int newstatus);
	void __fastcall AfterDataSetOperate(TOperateType OptType);
    virtual TForm * __fastcall NewItemEditForm(TZClientDataSet *m_DataSet, int status);//������������ϸ����
	//ϵͳ��дȨ�޿���
	void __fastcall SetUserCtrlStatus();
	void __fastcall FormFillData();
	void __fastcall DetailDataSetBeforePost(TDataSet* DataSet);
	void __fastcall BeforWorkFlowStart(TWorkFlowStartData *AWorkFlowStartData);
	void __fastcall BeforWorkFlowExec(TWorkFlowExecData *AWorkFlowExecData,TClientDataSet *varDataSet);
	void __fastcall SetStuSupplyNote(String StuSupplyNoteID);

public:		// User declarations
//	String __property DepotCode={write=FDepotCode};
//	String __property DepotName={write=FDepotName};
//	String __property ProjectCode={write=FPrjID};
	String __property GuidBillID={read=FGuidBillID};
	__property int PrjRight={read=FPrjRight,write=FPrjRight};
	__fastcall TfrmStuffStockInEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *m_DataSet,
			 int DataSetStatus,int Flag=1);
	__fastcall TfrmStuffStockInEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param,int Flag=1);
	virtual __fastcall ~TfrmStuffStockInEdit();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStuffStockInEdit *frmStuffStockInEdit;
//---------------------------------------------------------------------------
#endif
