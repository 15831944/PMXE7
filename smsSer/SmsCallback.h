//---------------------------------------------------------------------------

#ifndef SmsCallbackH
#define SmsCallbackH
#include "Classes.hpp"
#include "SysUtils.hpp"
#include <DSHTTPCommon.hpp>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *THookSmsCallback)( TJSONObject* Arg,TJSONObject *Ret);
typedef void __fastcall (__closure *TRunCycleProc)();
  // Interface for thread callback
class TSmsQueueProc : public TCppInterfacedObject<TThreadProcedure>
  {
  private:
	THookSmsCallback FHookSmsCallback ;
	TJSONObject* FArg;
	TJSONObject* FRet;
  public:
	TSmsQueueProc(THookSmsCallback& AHookSmsCallback,TJSONObject* Arg,TJSONObject *Ret) :FHookSmsCallback(AHookSmsCallback),FArg(Arg),FRet(Ret) {}
	__fastcall ~TSmsQueueProc() {}
	virtual void __fastcall Invoke()
	{
	  FHookSmsCallback(FArg,FRet);
	}
};
//循环调用超时工作流
class TRunCycleQueueProc : public TCppInterfacedObject<TThreadProcedure>
  {
  private:
	TRunCycleProc FRunCycleProc ;
	TJSONObject* FArg;
	TJSONObject* FRet;
  public:
	TRunCycleQueueProc(TRunCycleProc& ARunCycleProc) :FRunCycleProc(ARunCycleProc){}
	__fastcall ~TRunCycleQueueProc() {}
	virtual void __fastcall Invoke()
	{
	  FRunCycleProc();
	}
};
// ---------------------------------------------------------------------------
class PACKAGE TSmsCallbackClient :public TDBXCallback
 {
  private:
   String FCallbackName;
   THookSmsCallback FHookCallBack;
   TDSClientCallbackChannelManager* FChannelManager;
  public:
   __fastcall  TSmsCallbackClient(TDSClientCallbackChannelManager* AChannelManager, String ACallbackName);
   __fastcall virtual ~TSmsCallbackClient();
   virtual TJSONValue* __fastcall Execute(TJSONValue* const Arg);
   virtual System::TObject* __fastcall Execute(System::TObject* Arg);
   __property THookSmsCallback HookCallBack = {read = FHookCallBack,write = FHookCallBack};

 };
#endif

