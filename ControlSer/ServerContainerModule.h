//----------------------------------------------------------------------------

#ifndef ServerContainerModuleH
#define ServerContainerModuleH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <DataSnap.DSCommonServer.hpp>
#include <DataSnap.DSServer.hpp>
#include <DataSnap.DSTCPServerTransport.hpp>
#include <DataSnap.DSHTTP.hpp>
#include <Datasnap.DSProxyJavaAndroid.hpp>
#include <Datasnap.DSProxyJavaBlackBerry.hpp>
#include <Datasnap.DSProxyObjectiveCiOS.hpp>
#include <Datasnap.DSProxyCsharpSilverlight.hpp>
#include <Datasnap.DSProxyFreePascal_iOS.hpp>
#include <DataSnap.DSAuth.hpp>
#include <Datasnap.DSClientMetadata.hpp>
#include <Datasnap.DSHTTPServiceProxyDispatcher.hpp>
#include <Datasnap.DSMetadata.hpp>
#include <Datasnap.DSProxyJavaScript.hpp>
#include <Datasnap.DSServerMetadata.hpp>
#include <IPPeerServer.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <Vcl.ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TServerControlModule : public TDataModule
{
__published:	// IDE-managed Components
	TDSServer *DSServer1;
	TDSTCPServerTransport *DSTCPServerTransport1;
	TDSHTTPService *DSHTTPService1;
	TDSAuthenticationManager *DSAuthenticationManager1;
	TDSHTTPServiceProxyDispatcher *DSHTTPServiceProxyDispatcher1;
	TDSProxyGenerator *DSProxyGenerator1;
	TDSServerMetaDataProvider *DSServerMetaDataProvider1;
	TDSHTTPServiceFileDispatcher *DSHTTPServiceFileDispatcher1;
	TDSServerClass *DSServerClass1;
	TTimer *Timer1;
	void __fastcall DSServerClass1GetClass(TDSServerClass *DSServerClass, TPersistentClass &PersistentClass);
	void __fastcall DSAuthenticationManager1UserAuthenticate(TObject *Sender, const UnicodeString Protocol,
          const UnicodeString Context, const UnicodeString User,
          const UnicodeString Password, bool &valid,  TStrings *UserRoles);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall DSServer1Connect(TDSConnectEventObject *DSConnectEventObject);
	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall DataModuleDestroy(TObject *Sender);
private:	// User declarations
	int FTCPIP;
	int FHttpPort;
	int FHttpsPort;
	String FAdminName;
	String FPassWord;
	TJSONArray *ServerList;
	HANDLE FEventLog;
	String AppPath;
	String settingfile;
	int FPeriod;
	String FComputerIP;
	String MsgLogFile;
	void __fastcall ReadOptions();
	void __fastcall InitServerStatus();
	void  __fastcall WritLog(String ErrorLong,TDateTime dtime);
public:		// User declarations
	__fastcall TServerControlModule(TComponent* Owner);
	void __fastcall StartServer();
	void __fastcall StopServer();
	TJSONArray* __fastcall CopyServerList();
	void __fastcall UpdateServerList(String ServerName);
	void __fastcall NotifyWriteLog(String Msg);
};
//----------------------------------------------------------------------------
extern PACKAGE TServerControlModule *ServerControlModule;
//----------------------------------------------------------------------------
#endif

