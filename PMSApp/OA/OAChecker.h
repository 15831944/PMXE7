//---------------------------------------------------------------------------

#ifndef OACheckerH
#define OACheckerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseListForm.h"
#include "StartAndEndDate.h"
#include <FMX.ActnList.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <System.Rtti.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <System.Bindings.Outputs.hpp>
#include "DetailBaseForm.h"
#include <FMX.Controls.Presentation.hpp>
//---------------------------------------------------------------------------
class TfrmOAChecker : public TfrmBaseListForm
{
__published:	// IDE-managed Components
	TTabControl *tcOAControl;
	TTabItem *tabTask;
	TTabItem *tabSearch;
	TToolBar *tbTask;
	TButton *btCheck;
	TButton *btSearch;
	TActionList *OAActionList;
	TAction *CheckAction;
	TAction *SearchAction;
	TAction *SearchOkAction;
	TToolBar *tbSearch;
	TButton *btOk;
	TStringGrid *TaskStringGrid;
	TStringColumn *wtWorkTitleColumn;
	TStringColumn *wtNameColumn;
	TStringColumn *wtStatusColumn;
	TStringColumn *wtReceiverColumn;
	TStringColumn *wtCreateTimeColumn;
	TStringColumn *wtOperaterColumn;
	TStringColumn *wtResultsColumn;
	TStringColumn *wtFinishTimeColumn;
	TStringColumn *wtStepColumn;
	TTabControl *tcTaskControl;
	TTabItem *tabCheck;
	TTabItem *tabTaskLog;
	TTabItem *tabflowChart;
	TLabel *lbTaskDes;
	TMemo *mmFlowMnDtlRemark;
	TLabel *lbTaskComment;
	TEdit *edFlowMnDtlComments;
	TLabel *lbTastResult;
	TComboBox *cbFlowMnDtlResults;
	TLabel *lbWorkTitle;
	TLabel *lbWorkTitleText;
	TStringGrid *TaskLogStringGrid;
	TStringColumn *TaskLogNum;
	TStringColumn *FlowTaskLogTaskNameColumn;
	TStringColumn *FlowTaskLogDateColumn;
	TStringColumn *FlowTaskLogOperationColumn;
	TStringColumn *FlowTaskLogUserNameColumn;
	TStringColumn *FlowTaskLogInfoColumn;
	TLabel *lbBillType;
	TLabel *lbTaskStatus;
	TComboBox *cbBillType;
	TComboBox *cbTaskStatus;
	TLabel *lbBillTitle;
	TEdit *edBillTitle;
	TBindSourceDB *TaskLogBindSourceDB;
	TBindSourceDB *TaskBindSourceDB;
	TBindingsList *DetailBindingsList;
	TBindGridLink *TaskBindGridLink;
	TBindLink *FlowMnDtlCommentsBindLink;
	TBindLink *FlowMnDtlRemarkBindLink;
	TBindLink *FlowMnDtlResultsBindLink;
	TBindGridLink *TaskLogBindGridLink;
	TBindLink *WorkTitleTextBindLink;
	TButton *btClose;
	TTabItem *tabWork;
	TLabel *lbWorkflowContent;
	TMemo *mmWorkflowContent;
	TLabel *lbWorkflowLeve;
	TComboBox *cbWorkflowLeve;
	TLabel *lbWorkflowStatus;
	TComboBox *cbWorkflowStatus;
	TLabel *lbCreator;
	TLabel *lbCreatorName;
	TLabel *lbTaskTitle;
	TLabel *lbTaskTitleText;
	TBindLink *TaskTitleTextBindLink;
	TBindLink *CreatorNameBindLink;
	TBindLink *WorkflowContentBindLink;
	TBindLink *WorkflowLeveBindLink;
	TBindLink *WorkflowStatusBindLink;
	TButton *btBack;
	TToolBar *OATopToolBar;
	TLabel *Label1;
	TButton *btOpenBill;
	TAction *OpenbillAction;
	TScrollBox *FlowScrollBox;
	TLayout *FlowLayout;
	TImage *WorkflowImage;
	TListBox *TaskListBox;
	TListBoxItem *TaskListBoxItem1;
	TListBoxItem *TaskListBoxItem2;
	TListBoxItem *TaskListBoxItem3;
	TListBoxItem *TaskListBoxItem4;
	TListBox *FlowMainListBox;
	TListBoxItem *TaskListBoxItem5;
	TListBoxItem *TaskListBoxItem6;
	TListBoxItem *TaskListBoxItem7;
	TListBoxItem *TaskListBoxItem8;
	TListBoxItem *TaskListBoxItem9;
	TStartAndEndDateFrame *StartAndEndDateFrame1;
	TPanel *QueryPanel;
	void __fastcall tcTaskControlChange(TObject *Sender);
	void __fastcall SearchActionExecute(TObject *Sender);
	void __fastcall SearchOkActionExecute(TObject *Sender);
	void __fastcall CheckActionExecute(TObject *Sender);
	void __fastcall btCloseClick(TObject *Sender);
	void __fastcall btBackClick(TObject *Sender);
	void __fastcall TaskTitleTextBindLinkAssignedValue(TObject *Sender, TBindingAssignValueRec &AssignValueRec,
          const TValue &Value);
	void __fastcall cbFlowMnDtlResultsClosePopup(TObject *Sender);
	void __fastcall OpenbillActionExecute(TObject *Sender);
	void __fastcall FlowLayoutGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
private:	// User declarations
	void __fastcall SetOptCtrlStatus(int status);  //�����麯�������ô���״̬
	void __fastcall TaskStringGridSelectedRowChanged();
	void __fastcall FillBillTypeItems();
	String __fastcall GetOtherConditon();
  	void __fastcall LoadTaskData();
	void __fastcall LoadTaskLogData();
	void __fastcall ShowFlowChart();
	TClientDataSet *TaskDefListDs;
  	TZClientQuery *TaskQuery;
	TZClientQuery *TaskLogQuery;
	String TaskSelected;
	TWorkFlowExec FOnBeforWorkFlowExec;
	double HWScale;
	int FLastDistance ;
	bool FlowChartUpdate;
	bool TaskLogUpdate;
public:		// User declarations
	__property TWorkFlowExec OnBeforWorkFlowExec = {read=FOnBeforWorkFlowExec, write=FOnBeforWorkFlowExec};
	__fastcall TfrmOAChecker(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param);
	virtual __fastcall ~TfrmOAChecker();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOAChecker *frmOAChecker;
//---------------------------------------------------------------------------
#endif
