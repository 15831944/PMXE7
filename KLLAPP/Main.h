//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.MobilePreview.hpp>
#include "ClientBroker.h"
#include "ZClientQuery.h"
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <FMX.Menus.hpp>
#include <Data.SqlExpr.hpp>
#include <Datasnap.DSConnect.hpp>
#include <Data.DBXCommon.hpp>
#include <Data.DBXDataSnap.hpp>
#include <IPPeerClient.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Objects.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Notification.hpp>
#include "MiDefine.h"
#include <FMX.ListBox.hpp>
#include "FSTAbout.h"
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.adomxmldom.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *mainFooter;
	TLabel *UserLabel;
	TButton *btSetup;
	TButton *btLogin;
	TButton *btExit;
	TTabControl *tcMainControl;
	TTabItem *tabMain;
	TTabItem *tabMI;
	TTabControl *TabGroup;
	TTabItem *TabCompany;
	TTreeView *tvCompany;
	TTabItem *TabGroupItem;
	TStyleBook *fstStyle;
	TImage *CompanyImage;
	TImage *DepartImage;
	TImage *UserImage;
	TImage *OnlineImage;
	TListBox *GroupListBox;
	TNotificationCenter *NotificationC;
	TGridPanelLayout *gplayoutMain;
	TPanel *plHouse;
	TImage *imgHouse;
	TLabel *lbHouse;
	TPanel *plReserve;
	TImage *imgReserve;
	TLabel *lbReserve;
	TPanel *plOaCheck;
	TImage *imgOaCheck;
	TLabel *lbOaCheck;
	TPanel *plCustomer;
	TImage *imgCustomer;
	TLabel *lbCustomer;
	TPanel *Panel1;
	TLabel *Label3;
	TImage *Image1;
	TPanel *plHouseView;
	TLabel *Label1;
	TImage *ImgHouseView;
	TPanel *Panel3;
	TLabel *Label4;
	TImage *Image2;
	TPanel *plWorkReport;
	TImage *ImgWorkport;
	TLabel *Label2;
	TXMLDocument *XMLDocument1;
	void __fastcall btSetupClick(TObject *Sender);
	void __fastcall btLoginClick(TObject *Sender);
	void __fastcall btExitClick(TObject *Sender);
	void __fastcall GroupLisBoxItemClick(const TCustomListBox *Sender, const TListBoxItem *Item);
	void __fastcall icMenuClick(TObject *Sender);


private:	// User declarations
	TClientBroker *FClientBroker;
	TZClientQuery* MsgQuery;
	String FAgentAddress;
	int FAgentPort;
	String FCommunicationProtocol;
	String settingfile;
	String AppPath;
	TTreeViewItem* CompanyItem;
	void __fastcall MenuItemClick(TObject* Sender);
	void __fastcall DoApplyStyleLookup(TObject* Sender);
	void __fastcall DoCompanyApplyStyleLookup(TObject* Sender);
	void __fastcall DoCompanyInfoClick(TObject* Sender);
	void __fastcall DoListItemApplyStyleLookup(TObject* Sender);
	void __fastcall DoGroupInfoClick(TObject* Sender);
	void __fastcall LoadConnectSetting();
	void __fastcall ShowClientForm(int modulecode);

	void __fastcall FillCompanyTv();
	void __fastcall FillModuleData();
	void __fastcall FillGroupView();
	void __fastcall tvCompanyDeletion();
	void __fastcall DeleteGroupItems();
	void __fastcall MessageCallBackHook(TJSONObject* Arg); // ������Ϣ
	TTreeViewItem* __fastcall LocateCompanyTVNode(TTreeViewItem* CurItem,TQQUserType AUserType,String AID );
	TListBoxItem* __fastcall LocateGroupNode(String AID);
	void __fastcall UpdateLoginUserNode();
	void __fastcall InitAllChildForms();

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	__fastcall ~TMainForm();
	void __fastcall ChangeControlState();
	void __fastcall LoadStudioPane();
	void __fastcall CreateMsgDataSet();
	void __fastcall LogOff(System::Uitypes::TModalResult AResult);
	__property TClientBroker *ClientBroker={read=FClientBroker,write=FClientBroker};
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
