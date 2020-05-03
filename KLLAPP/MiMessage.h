//---------------------------------------------------------------------------

#ifndef MiMessageH
#define MiMessageH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include "MiDefine.h"
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include "ChatRoomServerMethods.h"
#include "ZClientQuery.h"
#include "ClientBroker.h"
#include <FMX.MobilePreview.hpp>
#include <FMX.Media.hpp>
//---------------------------------------------------------------------------
class TfrmMessageForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *MsgBottomToolBar;
	TMemo *memMsg;
	TSpeedButton *sbViration;
	TSpeedButton *sbRing;
	TButton *btSend;
	TTimer *MsgTimer;
	TButton *btClose;
	TLabel *UserLabel;
	TToolBar *MsgTopToolBar;
	TButton *btLateMsg;
	TMediaPlayer *MediaPlayer;
	TVertScrollBox *VertScrollBox1;
	TLayout *MainLayout1;
	TPanel *SendPanel;
	TMemo *memMsgEdit;
	void __fastcall MsgTimerTimer(TObject *Sender);
	void __fastcall btSendClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall sbRingClick(TObject *Sender);
	void __fastcall sbVirationClick(TObject *Sender);
	void __fastcall btCloseClick(TObject *Sender);
	void __fastcall btLateMsgClick(TObject *Sender);
	void __fastcall FormFocusChanged(TObject *Sender);
	void __fastcall FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
private:	// User declarations
	TClientBroker *FClientBroker;
	TQQUserType FQQUserType;
	TZClientQuery *msgQuery;
	TTextControl *FNode;
	String FUserID;
	String LastMsgSendTime;
	TChatRoomServerMethods*  FChatRoomServerMethodsClient;
	TJSONObject* FLastRecMessage;
	void __fastcall SetLastRecMessage(TJSONObject* Value);
	void __fastcall SendCommang(String cmdText);
	void __fastcall Viration();
	void __fastcall PlaySound(String AFileName);
	void __fastcall RestorePosition();
	void __fastcall UpdateKBBounds();
	void __fastcall CalcContentBoundsProc(TObject * Sender, System::Types::TRectF &ContentBounds);
	System::Types::TRectF FKBBounds;
	bool FNeedOffset;
public:		// User declarations
	__fastcall TfrmMessageForm(TComponent* Owner,TClientBroker *AClientBroker,TTextControl *ANode);
	virtual __fastcall ~TfrmMessageForm();
	__property TJSONObject* LastRecMessage={read=FLastRecMessage,write=SetLastRecMessage};

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMessageForm *frmMessageForm;
//---------------------------------------------------------------------------
#endif
