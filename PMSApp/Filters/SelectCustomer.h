//---------------------------------------------------------------------------

#ifndef SelectCustomerH
#define SelectCustomerH
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
//---------------------------------------------------------------------------
class TSelectCustomerForm : public TBaseFiltersForm
{
__published:	// IDE-managed Components
	TExpander *FilterExpander;
	TLabel *Label2;
	TLabel *Label1;
	TComboBox *cboxFilterType;
	TEdit *edFilterValue;
	TSearchEditButton *SearchEditButton1;
	TListView *lvCustomerList;
	void __fastcall SearchEditButton1Click(TObject *Sender);
private:	// User declarations
     void __fastcall FillCustomerList();
protected:
	virtual String __fastcall BuildFilters();
	virtual void __fastcall InitControl();
public:		// User declarations
	__fastcall TSelectCustomerForm(TComponent* Owner,TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker);
};
//---------------------------------------------------------------------------
extern PACKAGE TSelectCustomerForm *SelectCustomerForm;
//---------------------------------------------------------------------------
#endif
