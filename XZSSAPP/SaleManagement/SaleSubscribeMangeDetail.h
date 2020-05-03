//---------------------------------------------------------------------------

#ifndef SaleSubscribeMangeDetailH
#define SaleSubscribeMangeDetailH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseBillForm.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.NumberBox.hpp>
#include <FMX.Memo.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
//---------------------------------------------------------------------------
class TfrmSaleSubscribeMangeDetail : public TfrmBaseBillForm
{
__published:	// IDE-managed Components
	TExpander *Expander1;
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *edEstateName;
	TPanel *Panel2;
	TLabel *Label2;
	TEdit *edBuildName;
	TPanel *Panel3;
	TLabel *Label3;
	TEdit *edRoomName;
	TPanel *Panel4;
	TLabel *Label4;
	TComboBox *cbClientID;
	TPanel *Panel5;
	TLabel *Label5;
	TComboBox *cbPayMode;
	TPanel *Panel6;
	TLabel *Label6;
	TComboBox *cbProperty;
	TPanel *Panel7;
	TLabel *Label7;
	TComboBox *cbAreaType;
	TPanel *Panel8;
	TLabel *Label8;
	TEdit *edFloorArea;
	TPanel *Panel9;
	TLabel *Label9;
	TEdit *edInnerArea;
	TPanel *Panel10;
	TLabel *Label10;
	TEdit *edFloorAreaPrice;
	TPanel *Panel11;
	TLabel *Label11;
	TEdit *edInnerAreaPrice;
	TPanel *Panel12;
	TLabel *Label12;
	TNumberBox *nbSellAmt;
	TPanel *Panel13;
	TLabel *Label13;
	TComboBox *cbDiscount;
	TPanel *Panel14;
	TLabel *Label14;
	TNumberBox *nbDisAmt;
	TPanel *Panel15;
	TLabel *Label15;
	TComboBox *cbDecoIsCal;
	TPanel *Panel16;
	TLabel *Label16;
	TComboBox *cbDecoType;
	TPanel *Panel17;
	TLabel *Label17;
	TNumberBox *nbDecoPrice;
	TPanel *Panel18;
	TLabel *Label18;
	TNumberBox *nbDecoAmt;
	TExpander *Expander2;
	TPanel *Panel19;
	TLabel *Label19;
	TNumberBox *nbAcceAmt;
	TPanel *Panel20;
	TLabel *Label20;
	TNumberBox *nbTotalAmt;
	TPanel *Panel21;
	TLabel *Label21;
	TMemo *mmRemark;
	TButton *BtnSave;
	TClientDataSet *Query;
	void __fastcall BtnSaveClick(TObject *Sender);
private:	// User declarations
	String FEstateName,FBuildName;
	void __fastcall InitDictionary();
	void __fastcall InitClientData();
	void __fastcall InitPayModeData();
	void __fastcall InitDiscountData();
	void __fastcall InitBaseData();
	void __fastcall SaveData();
	bool __fastcall VerifyData();
public:		// User declarations
	__fastcall TfrmSaleSubscribeMangeDetail(TComponent* Owner,TClientBroker *clBroker,int modulecode,TClientDataSet *m_DataSet,String param);
	void __fastcall FillData();
	__property String EstateName={ read=FEstateName,write=FEstateName};
	__property String BuildName={ read=FBuildName,write=FBuildName};
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSaleSubscribeMangeDetail *frmSaleSubscribeMangeDetail;
//---------------------------------------------------------------------------
#endif
