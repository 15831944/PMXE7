//---------------------------------------------------------------------------

#ifndef HouseQueryH
#define HouseQueryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseListShow.h"
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <FMX.ActnList.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Layouts.hpp>
#include <System.Rtti.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <System.Bindings.Outputs.hpp>
#include "HouseFilters.h"
#include "HouseInformation.h"
#include <FMX.MediaLibrary.Actions.hpp>
#include <FMX.StdActns.hpp>
//---------------------------------------------------------------------------
class TfrmHouseQuery : public TfrmBaseListShow
{
__published:	// IDE-managed Components
	TStringGrid *HouseStringGrid;
	TBindGridLink *BindStringGridLink;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TStringColumn *StringColumn8;
	TStringColumn *StringColumn9;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TButton *btMedia;
	TButton *btResever;
	TAction *ReseverAction;
	TAction *HouseViewAction;
	void __fastcall ReseverActionExecute(TObject *Sender);
	void __fastcall HouseViewActionExecute(TObject *Sender);
private:	// User declarations
		TBitmap *FRawBitmap;
		THouseFiltersForm *FiltersForm;
		THouseInformationForm *InforForm;
		TForm * __fastcall NewEditForm(TZClientDataSet *FDataSet,int status);
		void __fastcall FormFillData();
		bool __fastcall BeforeUpdateData(int OptType);
		void __fastcall SetUserCtrlStatus();
		TForm * __fastcall NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker);
public:		// User declarations
	__fastcall TfrmHouseQuery(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,
							 String WhereStr);
	 virtual __fastcall ~TfrmHouseQuery();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHouseQuery *frmHouseQuery;
//---------------------------------------------------------------------------
#endif