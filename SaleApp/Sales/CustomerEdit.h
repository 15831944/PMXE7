//---------------------------------------------------------------------------

#ifndef CustomerEditH
#define CustomerEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "DetailBaseForm.h"
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <FMX.ActnList.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
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
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <FMX.DateTimeCtrls.hpp>
#include "SelectPeople.h"
#include "QueryFilter.h"
#include <FMX.Gestures.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.NumberBox.hpp>
//---------------------------------------------------------------------------
class TCustomerEditForm : public TfrmDetailBaseForm
{
__published:	// IDE-managed Components
	TExpander *BaseExpander;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TExpander *ContractExpander;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *Label20;
	TExpander *DetailExpander;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label29;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label32;
	TEdit *seClientNum;
	TEdit *seClientTheName;
	TComboBox *scClientSex;
	TEdit *seClientPhoneNum;
	TEdit *seClientPaperName;
	TEdit *seClientContactMethod;
	TEditButton *ClientContactMethodEditButton;
	TEdit *seClientSalerFKID;
	TEditButton *ClientSalerEditButton;
	TComboBox *scClientType;
	TEdit *seClientLevel;
	TEditButton *ClientLevelEditButton;
	TEdit *seClientMobilePhoneNum;
	TEdit *seClientWorkPhoneNum;
	TEdit *seClientFaxNum;
	TEdit *seClientEmail;
	TEdit *seClientCompany;
	TEdit *seClientPosition;
	TEditButton *ClientPositionEditButton;
	TEdit *seClientResidence;
	TEdit *seClientEducation;
	TEditButton *ClientEducationEditButton;
	TEdit *seClientWork;
	TEditButton *ClientWorkEditButton;
	TEdit *seClientTrade;
	TEditButton *ClientTradeEditButton;
	TEdit *seClientWorkYears;
	TEditButton *ClientWorkYearsEditButton;
	TEdit *seClientYearIncome;
	TEdit *seClientHouseType;
	TEdit *seClientHouseArea;
	TEditButton *ClientYearIncomeEditButton;
	TEditButton *ClientHouseTypeEditButton;
	TEditButton *ClientHouseAreaEditButton;
	TEdit *seClientHousePop;
	TEditButton *ClientHousePopEditButton;
	TEdit *seClientTraffic;
	TEditButton *ClientTrafficEditButton;
	TEdit *seClientCensusReg;
	TExpander *VIPExpander;
	TLabel *Label33;
	TLabel *Label34;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
	TLabel *Label38;
	TLabel *Label39;
	TLabel *Label40;
	TLabel *Label41;
	TLabel *Label42;
	TEdit *seClientCardNum;
	TComboBox *scClientIsClub;
	TEdit *seClientIntegral;
	TEdit *seClientClubLevel;
	TEdit *seClientAgio;
	TEdit *seClientBankName;
	TEdit *seClientBankAccount;
	TEditButton *ClientClubLevelEditButton;
	TEdit *seClientTheFKID_Cls;
	TEditButton *ClientClassEditButton;
	TComboBox *scClientFirstMeetDate;
	TLabel *Label43;
	TEditButton *ClientResidenceEditButton;
	TExpander *ContractRecordExpander;
	TMemo *mmoClientMeetRecord;
	TLabel *Label44;
	TEdit *seClientPostCode;
	TLabel *Label45;
	TLabel *Label46;
	TEditButton *ClientIntegralEditButton;
	TEditButton *ClientAgioEditButton;
	TNumberBox *seClientAge;
	TMemo *seClientAddress;
	TMemo *mmoClientRemark;
	TMemo *mmoClientDemand;
	TEditButton *PhoneCallButton;
	TEditButton *MobilePhoneCallButton;
	TEditButton *WorkPhoneCallButton;
	TPanel *Panel1;
	TPanel *Panel2;
	TDateEdit *dteClientFirstContactDate;
	TDateEdit *dteClientLastMeetDate;
	TPanel *Panel3;
	TDateEdit *dteClientBirthday;
	TPanel *Panel4;
	TPanel *Panel5;
	TDateEdit *dteClientJoinClubDate;
	TDateEdit *dteClientQuitClubDate;
	void __fastcall ClientAgioEditButtonClick(TObject *Sender);
	void __fastcall ClientIntegralEditButtonClick(TObject *Sender);
	void __fastcall BaseExpanderResize(TObject *Sender);
	void __fastcall ClientClassEditButtonClick(TObject *Sender);
	void __fastcall ClientContactMethodEditButtonClick(TObject *Sender);
	void __fastcall ClientLevelEditButtonClick(TObject *Sender);
	void __fastcall ClientPositionEditButtonClick(TObject *Sender);
	void __fastcall ClientResidenceEditButtonClick(TObject *Sender);
	void __fastcall ClientEducationEditButtonClick(TObject *Sender);
	void __fastcall ClientWorkEditButtonClick(TObject *Sender);
	void __fastcall ClientTradeEditButtonClick(TObject *Sender);
	void __fastcall ClientWorkYearsEditButtonClick(TObject *Sender);
	void __fastcall ClientYearIncomeEditButtonClick(TObject *Sender);
	void __fastcall ClientHouseTypeEditButtonClick(TObject *Sender);
	void __fastcall ClientHouseAreaEditButtonClick(TObject *Sender);
	void __fastcall ClientHousePopEditButtonClick(TObject *Sender);
	void __fastcall ClientTrafficEditButtonClick(TObject *Sender);
	void __fastcall ClientClubLevelEditButtonClick(TObject *Sender);
	void __fastcall ClientSalerEditButtonClick(TObject *Sender);
	void __fastcall PhoneCallButtonClick(TObject *Sender);
	void __fastcall MobilePhoneCallButtonClick(TObject *Sender);
	void __fastcall WorkPhoneCallButtonClick(TObject *Sender);
private:	// User declarations
	String LocateID;
	TFormatSettings fmtSettings;
	void __fastcall GetSalesType();
	bool IsSalesManager;
	String OldClientPKID;
	void __fastcall CompFieldData(String FieldCode,String FieldValues);
	void __fastcall LocateOldeCustomer(System::Uitypes::TModalResult AResult);
	double __fastcall GetMaxClientAgio();
	double __fastcall GetMaxClientIntegral();
	void __fastcall FormInitControl();
	void __fastcall FormClearControl(int Section);
	void __fastcall SendDataToDataSet(int Section);
	void __fastcall GetDataFromDataSet(int Section);
	void __fastcall VerifyFieldData(int Section,int status);
	bool __fastcall BeforeUpdateData(int OptType);
//	void __fastcall SetUserCtrlStatus();
	TSelectPeopleForm *SelectPeople;
	void __fastcall SetConsigner(String ConsignerID);
	TQueryFilterForm *CientClassType;
	TQueryFilterForm *ClientContract;
	TQueryFilterForm *ClientLevel;

	TQueryFilterForm *ClientResidence;
	TQueryFilterForm *ClientEducation;
	TQueryFilterForm *ClientWork;
	TQueryFilterForm *ClientProp1;
	TQueryFilterForm *ClientProp2;
	TQueryFilterForm *ClientProp3;
	TQueryFilterForm *ClientProp4;
	TQueryFilterForm *ClientProp5;
	TQueryFilterForm *ClientProp6;
	TQueryFilterForm *ClientProp7;
	TQueryFilterForm *ClientProp8;
	TQueryFilterForm *ClientProp9;
public:		// User declarations
	__fastcall TCustomerEditForm(TComponent* Owner,TClientBroker *clBroker,int ModuleCode,TZClientDataSet
								*FDataSet,int DataSetStatus);
};
//---------------------------------------------------------------------------
extern PACKAGE TCustomerEditForm *CustomerEditForm;
//---------------------------------------------------------------------------
#endif
