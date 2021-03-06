//---------------------------------------------------------------------------

#ifndef StuffStockInH
#define StuffStockInH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseListShow.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <FMX.ListBox.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Data.Bind.Grid.hpp>
#include <Fmx.Bind.Grid.hpp>
#include "StuffStockInEdit.h"
#include "StuffStockInFilters.h"
//---------------------------------------------------------------------------
class TfrmStuffStockIn : public TfrmBaseListShow
{
__published:	// IDE-managed Components
	TStringGrid *ListStringGrid;
	TBindGridLink *BindStringGridLink;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
private:	// User declarations
   String FDepotCode,FDepotName,FProjectCode;
   int FFlag;//1.表示采购入库，2表示其他入库
   int FPrjRight;
   TZClientQuery * StoreQuery;
   TfrmStuffStockInEdit *StuffStockInEdit;
   TStuffStockInFiltersForm *FiltersForm;
	TForm * __fastcall NewEditForm(TZClientDataSet *m_DataSet,int status);
	void __fastcall FormFillData();
	bool __fastcall BeforeUpdateData(int OptType);
	void __fastcall SetUserCtrlStatus();
  virtual TForm * __fastcall NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker); //创建过滤窗口，传递查询事件
public:		// User declarations
	__fastcall TfrmStuffStockIn(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String WhereStr,int Flag=1);
	virtual __fastcall ~TfrmStuffStockIn();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStuffStockIn *frmStuffStockIn;
//---------------------------------------------------------------------------
#endif
