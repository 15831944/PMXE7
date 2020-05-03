#ifndef ProxyH
#define ProxyH

#include "Data.DBXCommon.hpp"
#include "System.Classes.hpp"
#include "System.SysUtils.hpp"
#include "Data.DB.hpp"
#include "Data.SqlExpr.hpp"
#include "Data.DBXDBReaders.hpp"
#include "Data.DBXCDSReaders.hpp"

  class TServerControlMethodsClient : public TObject
  {
  private:
    TDBXConnection *FDBXConnection;
    bool FInstanceOwner;
    TDBXCommand *FGetServerListCommand;
    TDBXCommand *FGetServerInfoCommand;
    TDBXCommand *FGetServerStatusCommand;
    TDBXCommand *FStartServerCommand;
    TDBXCommand *FStopServerCommand;
    TDBXCommand *FRegServerCommand;
    TDBXCommand *FUnregServerCommand;
    TDBXCommand *FRebootComputerCommand;
    TDBXCommand *FShutdownComputerCommand;
  public:
    __fastcall TServerControlMethodsClient(TDBXConnection *ADBXConnection);
    __fastcall TServerControlMethodsClient(TDBXConnection *ADBXConnection, bool AInstanceOwner);
    __fastcall ~TServerControlMethodsClient();
    TJSONArray* __fastcall GetServerList();
    TJSONObject* __fastcall GetServerInfo(System::UnicodeString ServerName);
    int __fastcall GetServerStatus(System::UnicodeString ServerName);
    bool __fastcall StartServer(System::UnicodeString ServerName);
    bool __fastcall StopServer(System::UnicodeString ServerName);
    bool __fastcall RegServer(System::UnicodeString ServerName);
    bool __fastcall UnregServer(System::UnicodeString ServerName);
    bool __fastcall RebootComputer();
    bool __fastcall ShutdownComputer();
  };

#endif
