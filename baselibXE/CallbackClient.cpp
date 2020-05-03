//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "CallbackClient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TCallbackClient:: TCallbackClient(TDSClientCallbackChannelManager* AChannelManager, String ACallbackName)
:TDBXCallback()
{
  FHookCallBack=NULL;
  FChannelManager=AChannelManager;
  FCallbackName=ACallbackName;
  if(FChannelManager)
	 FChannelManager->RegisterCallback(FCallbackName,this);
}
//---------------------------------------------------------------------------
__fastcall TCallbackClient:: ~TCallbackClient()
{
//   if(FChannelManager)
//   {
//	 FChannelManager->UnregisterCallback(FCallbackName);
//	 FChannelManager->CloseClientChannel();
//   }
}
TJSONValue* __fastcall TCallbackClient::Execute(TJSONValue* const Arg)
{
   TJSONValue * Result=new TJSONTrue();
   TJSONObject * FArg=(TJSONObject*)Arg;
  if(FArg->Count>0)
  {
	if(FHookCallBack)
	{
	 /*_di_TThreadProcedure*/
	 TThreadProcedure* methProc(static_cast<TThreadProcedure*>(new TMsgQueueProc(FHookCallBack,(TJSONObject*)(FArg->Clone()))));
	 TThread::Queue(0, methProc);
	 }
   }
   return Result;
}
//---------------------------------------------------------------------------
System::TObject* __fastcall TCallbackClient::Execute(System::TObject* Arg)
{
   return NULL;
}
//---------------------------------------------------------------------------

