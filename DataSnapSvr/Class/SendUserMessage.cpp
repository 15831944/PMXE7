//---------------------------------------------------------------------------

#pragma hdrstop

#include "SendUserMessage.h"
#include "MessageManage.h"
#include "FSServerContainer.h"
#include "SHConst.h"
#include "SendEmailMessageThread.h"
#include "SendSMSMessageThread.h"
#include "SendMemoMessageThread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall TSendUserMessage::TSendUserMessage(TComponent* Owner)
	: TDMBaseServerClass(Owner) {
}

// ---------------------------------------------------------------------------
void  TSendUserMessage::SendUserMessage(int type,String AID)
{
  switch(type)
  {
	  case 11:
	   SendEmailMessage(AID);
	  break;
	  case 12:
		SendSMSMessage(AID);
	  break;
	  case 13:
	   SendMemoMessage(AID);
	  break;
  }
}
void  TSendUserMessage::SendEmailMessage(String AEmailID)
{
   TSendEmailMessageThread *smt=new TSendEmailMessageThread(AccConnection,AEmailID);
}
// ---------------------------------------------------------------------------
void  TSendUserMessage::SendSMSMessage(String ASMSID)
{
	TSendSMSMessageThread *smt=new TSendSMSMessageThread(AccConnection,ASMSID);
}
// ---------------------------------------------------------------------------
void  TSendUserMessage::SendMemoMessage(String AMemoID)
{
   TSendMemoMessageThread *smt=new TSendMemoMessageThread(AccConnection,AMemoID);
}
// ---------------------------------------------------------------------------
