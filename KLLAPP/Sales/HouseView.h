//---------------------------------------------------------------------------

#ifndef HouseViewH
#define HouseViewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "CommBase.h"
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <FMX.ActnList.hpp>
#include <System.Actions.hpp>
#include <FMX.Media.hpp>
//---------------------------------------------------------------------------
class THouseViewForm : public TCommBaseForm
{
__published:	// IDE-managed Components
	TExpander *FilterExpander;
	TComboBox *cbHsgFKID_Bdg;
	TComboBox *cbHsgFKID_Est;
	TComboBox *cbHsgHouseType;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TListView *lvHouseList;
	TSpeedButton *sbSearch;
	TAniIndicator *WaitAniIndicator;
	TButton *btDown;
	TActionList *HVActionList;
	TAction *DownLoadAction;
	TMediaPlayer *MediaPlayer1;
	TAction *SearchAction;
	void __fastcall cbHsgFKID_EstClosePopup(TObject *Sender);
	void __fastcall lvHouseListButtonClick(const TObject *Sender, const TListViewItem *AItem,
          const TListItemSimpleControl *AObject);
	void __fastcall DownLoadActionExecute(TObject *Sender);
	void __fastcall SearchActionExecute(TObject *Sender);

private:	// User declarations
	String AppPath;
	TFormatSettings fmtSettings;
	TZClientQuery *MedalQuery;
	String __fastcall GetFilters();
	void __fastcall FillHouseList();
	void __fastcall DowndLoadListFile();
	void __fastcall DoDowndLoadProc(System::Uitypes::TModalResult AResult);
	void __fastcall InitEstNameComboBox();
	void __fastcall InitBdgNameComboBox();
	void __fastcall InitHouseTypeComboBox();
protected:
	virtual void __fastcall InitControl();
public:		// User declarations
	__fastcall THouseViewForm(TComponent* Owner,TClientBroker * ABroker);
	__fastcall ~THouseViewForm();
    void __fastcall SetCurHouseType(String EstName,String BdgName,String HousType);//ֱ�Ӷ�λ����
};
//---------------------------------------------------------------------------
extern PACKAGE THouseViewForm *HouseViewForm;
//---------------------------------------------------------------------------
#endif
