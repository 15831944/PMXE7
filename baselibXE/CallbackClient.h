//---------------------------------------------------------------------------

#ifndef CallbackClientH
#define CallbackClientH
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Datasnap.DSCommon.hpp>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *THookMsgCallback)( TJSONObject* Arg);
  // Interface for thread callback
class TMsgQueueProc : public TCppInterfacedObject<TThreadProcedure>
  {
  private:
	THookMsgCallback FHookMsgCallback ;
	TJSONObject* FArg;
  public:
	TMsgQueueProc(THookMsgCallback& AHookMsgCallback,TJSONObject* Arg) :FHookMsgCallback(AHookMsgCallback),FArg(Arg) {}
	__fastcall ~TMsgQueueProc() {}
	virtual void __fastcall Invoke()
	{
	  FHookMsgCallback(FArg);
	}
};
// ---------------------------------------------------------------------------
class PACKAGE TCallbackClient :public TDBXCallback
 {
  private:
   String FCallbackName;
   THookMsgCallback FHookCallBack;
   TDSClientCallbackChannelManager* FChannelManager;
  public:
   __fastcall  TCallbackClient(TDSClientCallbackChannelManager* AChannelManager, String ACallbackName);
   __fastcall virtual ~TCallbackClient();
   virtual TJSONValue* __fastcall Execute(TJSONValue* const Arg);
   virtual System::TObject* __fastcall Execute(System::TObject* Arg);
   __property THookMsgCallback HookCallBack = {read = FHookCallBack,write = FHookCallBack};

 };
#endif
