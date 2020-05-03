//---------------------------------------------------------------------------

#ifndef SelectHouseH
#define SelectHouseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseFilters.h"
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
//---------------------------------------------------------------------------
class TSelectHouseForm : public TBaseFiltersForm
{
__published:	// IDE-managed Components
	TExpander *FilterExpander;
	TComboBox *cbHsgDecoration;
	TComboBox *cbHsgFKID_Bdg;
	TComboBox *cbHsgFKID_Est;
	TComboBox *cbHsgHouseType;
	TComboBox *cbHsgLandscape;
	TComboBox *cbHsgOrientation;
	TComboBox *cbHsgSellArea;
	TEdit *edHsgNum;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TSearchEditButton *SearchEditButton1;
	TListView *lvHouseList;
	void __fastcall SearchEditButton1Click(TObject *Sender);
	void __fastcall cbHsgFKID_EstClosePopup(TObject *Sender);
private:	// User declarations
	void __fastcall InitEstNameComboBox();
	void __fastcall InitBdgNameComboBox();
	void __fastcall InitHouseTypeComboBox();
	void __fastcall InitOrientationComboBox();
	void __fastcall InitDecorationComboBox();
	void __fastcall InitLandscapeComboBox();
	String __fastcall GetSellAreaFilter();
	String __fastcall GetFilters();
	void __fastcall FillHouseList();
protected:
	virtual String __fastcall BuildFilters();
	virtual void __fastcall InitControl();
public:		// User declarations
	__fastcall TSelectHouseForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker);
};
//---------------------------------------------------------------------------
extern PACKAGE TSelectHouseForm *SelectHouseForm;
//---------------------------------------------------------------------------
#endif
