//---------------------------------------------------------------------------

#ifndef wfCallbackH
#define wfCallbackH
#include "Classes.hpp"
#include "SysUtils.hpp"
#include <DSHTTPCommon.hpp>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *THookWFCallback)( TJSONObject* Arg,TJSONObject *Ret);
typedef void __fastcall (__closure *TRunPendProc)();
  // Interface for thread callback
class TWFQueueProc : public TCppInterfacedObject<TThreadProcedure>
  {
  private:
	THookWFCallback FHookWFCallback ;
	TJSONObject* FArg;
	TJSONObject* FRet;
  public:
	TWFQueueProc(THookWFCallback& AHookWFCallback,TJSONObject* Arg,TJSONObject *Ret) :FHookWFCallback(AHookWFCallback),FArg(Arg),FRet(Ret) {}
	__fastcall ~TWFQueueProc() {}
	virtual void __fastcall Invoke()
	{
	  FHookWFCallback(FArg,FRet);
	}
};
//循环调用超时工作流
class TRunPendQueueProc : public TCppInterfacedObject<TThreadProcedure>
  {
  private:
	TRunPendProc FRunPendProc ;
  public:
	TRunPendQueueProc(TRunPendProc& ARunPendProc) :FRunPendProc(ARunPendProc){}
	__fastcall ~TRunPendQueueProc() {}
	virtual void __fastcall Invoke()
	{
	  FRunPendProc();
	}
};
// ---------------------------------------------------------------------------
class PACKAGE TWFCallbackClient :public TDBXCallback
 {
  private:
   String FCallbackName;
   THookWFCallback FHookCallBack;
   TDSClientCallbackChannelManager* FChannelManager;
  public:
   __fastcall  TWFCallbackClient(TDSClientCallbackChannelManager* AChannelManager, String ACallbackName);
   __fastcall virtual ~TWFCallbackClient();
   virtual TJSONValue* __fastcall Execute(TJSONValue* const Arg);
   virtual System::TObject* __fastcall Execute(System::TObject* Arg);
   __property THookWFCallback HookCallBack = {read = FHookCallBack,write = FHookCallBack};

 };
#endif

