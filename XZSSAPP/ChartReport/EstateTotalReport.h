//---------------------------------------------------------------------------

#ifndef EstateTotalReportH
#define EstateTotalReportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseForm.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.MultiView.hpp>
#include "StartAndEndDate.h"
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.TreeView.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <FMXTee.Chart.hpp>
#include <FMXTee.Engine.hpp>
#include <FMXTee.Procs.hpp>
#include <FMXTee.Series.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.Controls.Presentation.hpp>
//---------------------------------------------------------------------------
class TfrmEstateTotalReport : public TfrmBaseForm
{
__published:	// IDE-managed Components
	TButton *btnQueryForm;
	TMultiView *QueryView;
	TStartAndEndDateFrame *StartAndEndDateFrame;
	TTreeView *BuildingTree;
	TClientDataSet *Query;
	TPanel *Panel1;
	TButton *BtnQuery;
	TChart *QtyChart;
	TPieSeries *QtySeries;
	TTabControl *ChartTabCtr;
	TTabItem *TabItem1;
	TTabItem *TabItem2;
	TChart *AmtChart;
	TPieSeries *AmtSeries;
	TGestureManager *GestureManager1;
	void __fastcall BuildingTreeChangeCheck(TObject *Sender);
	void __fastcall BtnQueryClick(TObject *Sender);
	void __fastcall ChartTabCtrGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
private:	// User declarations
	void __fastcall InitBuildingTree();
public:		// User declarations
	__fastcall TfrmEstateTotalReport(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEstateTotalReport *frmEstateTotalReport;
//---------------------------------------------------------------------------
#endif
