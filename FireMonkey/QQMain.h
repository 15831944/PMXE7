//---------------------------------------------------------------------------

#ifndef QQMainH
#define QQMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.Types.hpp>
#include "ClientBroker.h"
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
//---------------------------------------------------------------------------
enum TQQUserType: unsigned char {qutCompany,qutDepartment,qutUser,qutGroup};
typedef struct QQUserRecord
{
  int State;//�Ƿ�����0,1
  int  iMessageFormID; //�򿪵���Ϣ�����ʶ
  TQQUserType UserType;  //�û�����
  String ID;          //�û�ID
  String Name;
} TQQUserRecord ;
class TfrmQQMain : public TForm
{
__published:	// IDE-managed Components
	TTabControl *TabGroup;
	TTabItem *TabCompany;
	TTabItem *TabGroupItem;
	TTreeView *tvCompany;
	TListView *lvGroup;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall tvCompanyDblClick(TObject *Sender);
private:	// User declarations
	TClientBroker *FClientBroker;
	void __fastcall FillCompanyTv();
	void __fastcall tvCompanyDeletion();
	void __fastcall MessageCallBackHook(TJSONObject* Arg); // ������Ϣ
	TTreeViewItem* __fastcall LocateCompanyTVNode(TQQUserType AUserType,String AID );
	void __fastcall UpdateLoginUserNode();
public:		// User declarations
	__fastcall TfrmQQMain(TComponent* Owner,TClientBroker *AClientBroker);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmQQMain *frmQQMain;
//---------------------------------------------------------------------------
#endif