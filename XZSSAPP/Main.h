//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include "ClientBroker.h"
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <IPPeerClient.hpp>
#include <REST.Client.hpp>
#include <REST.Response.Adapter.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.adomxmldom.hpp>
#include "WaitThread.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *MainPanel;
	TToolBar *mainFooter;
	TButton *btSetup;
	TButton *btLogin;
	TLabel *UserLabel;
	TPanel *plOaCheck;
	TImage *imgOaCheck;
	TLabel *lbOaCheck;
	TClientDataSet *MainFormDataSet;
	TPanel *plLoanCalculation;
	TImage *imgLoanCalculation;
	TLabel *Label1;
	TPanel *plRoom;
	TImage *Image1;
	TLabel *Label2;
	TStyleBook *StyleBook;
	TPanel *plSaleSubscribeCancel;
	TImage *Image2;
	TToolBar *Header;
	TLabel *HeaderLabel;
	TLabel *Label3;
	TFlowLayout *gplayoutMain;
	TPanel *plClient;
	TImage *Image3;
	TLabel *Label4;
	TPanel *plEstateTotalReport;
	TImage *Image4;
	TLabel *Label5;
	TPanel *Panel1;
	TImage *Image5;
	TLabel *Label6;
	void __fastcall btLoginClick(TObject *Sender);
	void __fastcall btSetupClick(TObject *Sender);
	void __fastcall icMenuClick(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
private:	// User declarations
	TClientBroker *FClientBroker;
	String FAgentAddress;
	int FAgentPort;
	String FCommunicationProtocol;
	String settingfile;
	String AppPath;
	String FLogInURL,FLogOutURL,FLogUserDataURL;
	double FBaseBusiRate,FBaseAccuRate;
	TWaitThread *FWaitThread;
	void __fastcall TMainForm::LoadConnectSetting();
	void __fastcall Exit();
	void __fastcall AfterShow(TObject *Sender);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	__property TClientBroker *ClientBroker={read=FClientBroker,write=FClientBroker};
	void __fastcall ChangeControlState();
	void __fastcall ShowClientForm(int modulecode);
	void __fastcall LogIn();
	void __fastcall LogOff(System::Uitypes::TModalResult AResult);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
