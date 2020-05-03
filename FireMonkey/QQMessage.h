//---------------------------------------------------------------------------

#ifndef QQMessageH
#define QQMessageH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Types.hpp>
#include "QQMain.h"
#include "ChatRoomServerMethods.h"
#include <FMX.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmQQMessage : public TForm
{
__published:	// IDE-managed Components
	TPanel *plRight;
	TPanel *plMain;
	TPanel *plBottom;
	TMemo *memMsg;
	TPanel *Panel1;
	TMemo *memMsgEdit;
	TSpeedButton *sbViration;
	TSpeedButton *sbRing;
	TButton *bbtSend;
	TTimer *Timer1;
	TLabel *lbvirate;
	TLabel *lbRing;
	void __fastcall bbtSendClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall sbVirationClick(TObject *Sender);
	void __fastcall sbRingClick(TObject *Sender);
	void __fastcall memMsgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	TClientBroker *FClientBroker;
	TQQUserType FQQUserType;
	TQQUserRecord *FQQUserRecord;
	String FUserID;
	TChatRoomServerMethods*  FChatRoomServerMethodsClient;
	TJSONObject* FLastRecMessage;
	void __fastcall SetLastRecMessage(TJSONObject* Value);
	void __fastcall SendCommang(String cmdText);
	void __fastcall Viration();
public:		// User declarations
	__fastcall TfrmQQMessage(TComponent* Owner,TClientBroker *AClientBroker,TQQUserRecord *AQQUserRecord);
	__property TQQUserRecord *QQUserRecord={read=FQQUserRecord,write=FQQUserRecord};
	__property TJSONObject* LastRecMessage={read=FLastRecMessage,write=SetLastRecMessage};
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmQQMessage *frmQQMessage;
//---------------------------------------------------------------------------
#endif
