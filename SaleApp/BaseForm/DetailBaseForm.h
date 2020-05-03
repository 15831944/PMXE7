//---------------------------------------------------------------------------

#ifndef DetailBaseFormH
#define DetailBaseFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.ActnList.hpp>
#include <System.Actions.hpp>
#include "BaseBillForm.h"
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <System.Rtti.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <System.Bindings.Outputs.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.Controls.Presentation.hpp>
//---------------------------------------------------------------------------
typedef struct WorkFlowStartData
{
int Rank;
String FlowDefID;
String Subject;
String Content;
String BusWindow;
String BusCode;
String ProjectID;
String ReportID;
} TWorkFlowStartData;
typedef struct WorkFlowExecData
{
String FlowDefID;
String WorkInsID;
String TaskInsID;
String TskStatus;
String TskComments;
} TWorkFlowExecData;
typedef void __fastcall (__closure *TWorkFlowStart)(TWorkFlowStartData *AWorkFlowStartData);
typedef void __fastcall (__closure *TWorkFlowAfterStart)(String WorkInsID);
typedef void __fastcall (__closure *TWorkFlowExec)(TWorkFlowExecData *AWorkFlowExecData,TClientDataSet *varDataSet);
typedef void __fastcall (__closure *TWorkFlowAfterExec)(String WorkInsID,String TaskInsID);
class TfrmDetailBaseForm : public TfrmBaseBillForm
{
__published:	// IDE-managed Components
	TTabControl *tcDetailControl;
	TTabItem *tabDetail;
	TTabItem *tabOA;
	TToolBar *DetailMainToolBar;
	TActionList *DetailActionList;
	TChangeTabAction *NetTabAction;
	TChangeTabAction *BackTabAction;
	TButton *btSave;
	TAction *SaveDetailAction;
	TAction *OACheckAction;
	TAction *OACloseAction;
	TTabControl *tcMainControl;
	TTabItem *TabDetailMain;
	TTabItem *TabDetailItem;
	TToolBar *ItemToolBar;
	TButton *btItemBack;
	TButton *btMainNext;
	TNextTabAction *MainNextTabAction;
	TPreviousTabAction *ItemPreviousTabAction;
	TStringGrid *TaskStringGrid;
	TStringColumn *wtNoColumn;
	TStringColumn *wtNameColumn;
	TStringColumn *wtStatusColumn;
	TStringColumn *wtReceiverColumn;
	TStringColumn *wtCreateTimeColumn;
	TStringColumn *wtOperaterColumn;
	TStringColumn *wtResultsColumn;
	TStringColumn *wtFinishTimeColumn;
	TStringColumn *wtStepColumn;
	TBindingsList *DetailBindingsList;
	TBindSourceDB *TaskBindSourceDB;
	TBindGridLink *TaskBindGridLink;
	TBindLink *FlowMnDtlCommentsBindLink;
	TBindLink *FlowMnDtlRemarkBindLink;
	TBindLink *FlowMnDtlResultsBindLink;
	TBindSourceDB *TaskLogBindSourceDB;
	TStringGrid *TaskLogStringGrid;
	TStringColumn *TaskLogNum;
	TStringColumn *FlowTaskLogTaskNameColumn;
	TStringColumn *FlowTaskLogDateColumn;
	TStringColumn *FlowTaskLogOperationColumn;
	TStringColumn *FlowTaskLogInfoColumn;
	TBindGridLink *TaskLogBindGridLink;
	TStringColumn *FlowTaskLogUserNameColumn;
	TAction *ShowflowChartAction;
	TTabControl *tcWorkflowControl;
	TTabItem *TabCheckExec;
	TTabItem *TabCheckStare;
	TLabel *lbTaskDes;
	TMemo *mmFlowMnDtlRemark;
	TLabel *lbTaskComment;
	TEdit *edFlowMnDtlComments;
	TLabel *lbTastResult;
	TComboBox *cbFlowMnDtlResults;
	TLabel *lbWorkflowTitle;
	TLabel *lbWorkflowContent;
	TLabel *lbWorkflowLeve;
	TEdit *edWorkflowTitle;
	TMemo *mmWorkflowContent;
	TComboBox *cbWorkflowLeve;
	TComboBox *cbWorkflowStatus;
	TLabel *lbWorkflowStatus;
	TBindLink *WorkflowTitleBindLink;
	TBindLink *WorkflowContentBindLink;
	TBindLink *WorkflowLeveBindLink;
	TBindLink *WorkflowStatusBindLink;
	TLabel *lbCreator;
	TLabel *lbCreatorName;
	TBindLink *CreatorNameBindLink;
	TAction *OAStareAction;
	TButton *btCheck;
	TButton *btOAStare;
	TTabItem *tabTaskLog;
	TTabItem *tabTaskflow;
	TButton *btBaseClose;
	TAction *CloseFormAction;
	TButton *btEdit;
	TAction *DetailEditAction;
	TButton *btDeleteItem;
	TButton *btAddItem;
	TAction *AddItemAction;
	TAction *DeleteItemAction;
	TAction *BrowseItemAction;
	TButton *btBrowsItem;
	TVertScrollBox *MainVertScrollBox;
	TLayout *MainLayout;
	TVertScrollBox *EditVertScrollBox;
	TLayout *EditLayout;
	TScrollBox *FlowScrollBox;
	TLayout *FlowLayout;
	TGestureManager *DetailGestureManager;
	TListBox *FlowMainListBox;
	TListBoxItem *TaskListBoxItem5;
	TListBoxItem *TaskListBoxItem6;
	TListBoxItem *TaskListBoxItem7;
	TListBoxItem *TaskListBoxItem8;
	TListBoxItem *TaskListBoxItem9;
	TListBox *TaskListBox;
	TListBoxItem *TaskListBoxItem1;
	TListBoxItem *TaskListBoxItem2;
	TListBoxItem *TaskListBoxItem3;
	TListBoxItem *TaskListBoxItem4;
	TLabel *lbTaskTitle;
	TLabel *lbTaskTitleText;
	TBindLink *TaskTileTextBindLink;
	TAction *MoveNextAction;
	TAction *MovePriorAction;
	TImage *WorkflowImage;
	TButton *btNextRecord;
	TButton *btPriorRecord;
	void __fastcall SaveDetailActionExecute(TObject *Sender);
	void __fastcall OACheckActionExecute(TObject *Sender);
	void __fastcall ShowflowChartActionExecute(TObject *Sender);
	void __fastcall tcDetailControlChange(TObject *Sender);

	void __fastcall OAStareActionExecute(TObject *Sender);
	void __fastcall FlowMnDtlRemarkBindLinkAssignedValue(TObject *Sender, TBindingAssignValueRec &AssignValueRec,
          const TValue &Value);
	void __fastcall CloseFormActionExecute(TObject *Sender);
	void __fastcall DetailEditActionExecute(TObject *Sender);
	void __fastcall DetailEditActionUpdate(TObject *Sender);
	void __fastcall SaveDetailActionUpdate(TObject *Sender);
	void __fastcall cbFlowMnDtlResultsClosePopup(TObject *Sender);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall DeleteItemActionExecute(TObject *Sender);
	void __fastcall BrowseItemActionExecute(TObject *Sender);
	void __fastcall FormFocusChanged(TObject *Sender);
	void __fastcall FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FlowLayoutGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall MoveNextActionExecute(TObject *Sender);
	void __fastcall MovePriorActionExecute(TObject *Sender);
	void __fastcall tcWorkflowControlChange(TObject *Sender);
private:	// User declarations
	double HWScale;
	int FLastDistance;
	TClientDataSet *TaskDefListDs;
	TZClientQuery *TaskQuery;
	TZClientQuery *TaskLogQuery;
	TWorkFlowStart FOnBeforWorkFlowStart;
	TWorkFlowAfterStart FOnAfterWorkFlowStart;
	TWorkFlowExec FOnBeforWorkFlowExec;
	TWorkFlowAfterExec FOnAfterWorkFlowExec;
	void __fastcall FillTask();
	void __fastcall FillTaskLog();
	void __fastcall TaskStringGridRowChanged();
	String TaskSelected;  //选中的任务
	void __fastcall RestorePosition();
	void __fastcall UpdateKBBounds();
	void __fastcall CalcContentBoundsProc(TObject * Sender, System::Types::TRectF &ContentBounds);
	void __fastcall DoDelItemData(System::Uitypes::TModalResult AResult);
	System::Types::TRectF FKBBounds;
	bool FNeedOffset;
	bool FTaskLogUpdate;
	bool FTaskFlowUpdate;
	void __fastcall InitDetailBase();
protected:  //virtual function
  void __fastcall AddItemData();          //增加数据
  void __fastcall EditItemData();         //修改数据
  void __fastcall DelItemData();          //删除数据
  void __fastcall BrowseItemRecord();     //浏览数据

  void __fastcall SetOptCtrlStatus(int status);    //重载函数，设置窗体状态
  virtual void __fastcall GetWorkflowDefine();
  virtual void __fastcall GetWorkflowRunID(String BillID);
  virtual void __fastcall FormInitControl(){};   //初始化数据控件
  virtual void __fastcall FormClearControl(int Section){};   //清空控件数据
  virtual void __fastcall SendDataToDataSet(int Section){};  //控件数据存入数据集
  virtual void __fastcall GetDataFromDataSet(int Section){}; //数据集数据显示在控件中
  virtual void __fastcall VerifyFieldData(int Section,int status){}; //保存前校验数据，status：1-新增，2-修改
  virtual void __fastcall SetUserCtrlStatus(){};//设置用户构件状态
public:		// constructor and destructor
	 //如果相关业务需要审批流程，要求实现下面两个事件。
	__property TWorkFlowStart OnBeforWorkFlowStart = {read=FOnBeforWorkFlowStart, write=FOnBeforWorkFlowStart};
	__property TWorkFlowAfterStart OnAfterWorkFlowStart = {read=FOnAfterWorkFlowStart, write=FOnAfterWorkFlowStart};
	__property TWorkFlowExec OnBeforWorkFlowExec = {read=FOnBeforWorkFlowExec, write=FOnBeforWorkFlowExec};
	__property TWorkFlowAfterExec OnAfterWorkFlowExec = {read=FOnAfterWorkFlowExec, write=FOnAfterWorkFlowExec};
	__fastcall TfrmDetailBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param);
	__fastcall TfrmDetailBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus);
	virtual __fastcall ~TfrmDetailBaseForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDetailBaseForm *frmDetailBaseForm;
//---------------------------------------------------------------------------
#endif
