//----------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "FSServerContainer.h"
#include <tchar.h>
#include <stdio.h>
#include <memory>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <System.Ioutils.hpp>
#include "SyAccBook.h"
#include "LoginUserRecord.h"
#include "WorkFlowSerRecord.h"
#include "SmsSerRecord.h"
#include "LoginUser.h"
#include "ServerDataSet.h"
#include "ServerQuery.h"
#include "JServerDataSet.h"
#include "JServerQuery.h"
#include "BaseFunc.h"
#include "ChatRoomServer.h"
#include "SendUserMessage.h"
#include "HouseView.h"
#include "CallbackTunnelEventProc.h"
//#include "ChatRoomServerMethods.h"
#include "WorkFlowServer.h"
#include "MessageManage.h"
#include "ContractProject.h"
#include "DSEventProc.h"
#include "Version.h"
#include "APIbase.h"
#include "SafeDataApi.h"
//#include "SHConst.h"
#include "DBHead.h"
#include "DecEnc.h"
#include "SHConst.h"
#include "LogQueueProc.h"
#define DEVENDDATE "2015-12-30"
#define _TESTDATE_ 30
#define LASTTIMES 180  //3小时//
#ifndef _BYDEV_ || _DEMO_
#include "rwdog.h"
#endif
/*  重新生成客户端代理函数时，需要修改该过程
void __fastcall TLoginUserClient::Logout()
{
  if (FLogoutCommand == NULL)
  {
	FLogoutCommand = FDBXConnection->CreateCommand();
    FLogoutCommand->CommandType = TDBXCommandTypes_DSServerMethod;
	FLogoutCommand->Text = "TLoginUser.Logout";
	FLogoutCommand->CommandTimeout=3;   //防止连接断开
	FLogoutCommand->Prepare();
  }
  FLogoutCommand->ExecuteUpdate();
}
*/
//---------------------------------------------------------------------------
//#pragma link "Datasnap.DSProxyObjectiveCiOS"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TDMServerContainer *DMServerContainer;
/****************************************************************************
//
****************************************************************************/
void _fastcall aDSProc(TObject *Sender, const TDSSessionEventType EventType, TDSSession* const Session)
{
	// Remove user when we detect a drop
	if(EventType == SessionClose)
	{
		DMServerContainer->RemoveUser(((TDSSession*)Session)->Id);
	}
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
__fastcall TDMServerContainer::TDMServerContainer(TComponent* Owner)
	: TDataModule(Owner)
{
    fmtSettings=fmtSettings.Create();
	fmtSettings.DateSeparator   = '-';
	fmtSettings.TimeSeparator   = ':';
	fmtSettings.ShortDateFormat = "yyyy-MM-dd";
	fmtSettings.ShortTimeFormat = "HH:mm:ss";
	FDevEndDate = StrToDate(DEVENDDATE, fmtSettings);
	FBeginRunTime=Now();

	FLicenseKey     = -1;
	IsUpdateMenu    = false;
	FTestOverTimes   = false;
	IsTrial         = false;
	FSoftDog		=false;
	FMaxUserCount   = 5;
	FMaxOAUserCount = 5;
	FLogUnigue      = 0;
	FEventLog       = 0;
	FDbType         = 1; // 1-SQL,2-ORC,3-
	FPrjLevel       = 5; // 这个是初始化，根据安全文件会刷新
	FTCPIPPort     		= 211;
	FHttpPort       = 8080;
	FHttpsPort		= 8081;
	FVersion        = 8; // 内部版本控制

	m_DataSet = new TADOQuery(this);
	m_DataSet->ParamCheck = false;
	m_DataSet->Connection = SysADOConnection;

	// 下面为处理账套菜单创建的，连接串临时才创建
	AccQuery = new TADOQuery(this);
	AccQuery->ParamCheck = false;

	LogQuery = new TADOQuery(this);
	LogQuery->ParamCheck = false;
	LogQuery->Connection = SysADOConnection;

	m_Query = new TADOQuery(this);
	m_Query->ParamCheck = false;
   /*Nt服务器路径不对*/
	 // AppPath = ExpandFileName(FSDSHTTPServiceFileDispatcher->RootDirectory);
	 //ParamStr(0)
	  AppPath = ExtractFilePath(Forms::Application->ExeName);
	  if (!DirectoryExists(AppPath))
		  ForceDirectories(AppPath);

	 if ( !AnsiEndsStr("\\", AppPath) && !AnsiEndsStr("/", AppPath))
	 	AppPath = AppPath + Sysutils::PathDelim;

	 ProxyPath =System::Ioutils::TPath::Combine(AppPath,"proxy")+ Sysutils::PathDelim;

	// 先检测一下狗
#ifndef _BYDEV_
#ifndef _DEMO_
	if(CheckSoftDog())
		OpenSoftDog();
#endif
#endif
}
//----------------------------------------------------------------------------
void __fastcall TDMServerContainer::DataModuleCreate(TObject *Sender)
{
	ServerUsersDestroyed = false;
	ConnectionList       = new TStringList();
	SysConnectionList       = new TStringList();
	UserList             = new TStringList();
	WorkFlowSerList		 = new TStringList();
	SmsSerList			= new TStringList();
	//对话事件
	TDSSessionEventProc dse      = &aDSProc;
	if(dse)
	{
		_di_TDSSessionEvent methProc(static_cast<TDSSessionEvent*>(new TDSEventProc(dse)));
		TDSSessionManager::Instance->AddSessionEvent(methProc);
	}
	//callbackTunnel事件
	TDSCallbackTunnelEventProc CallbackTunnelEventProc =&CallbackTunnelEvent;
	if(CallbackTunnelEventProc)
	{
	_di_TDSCallbackTunnelEvent CallbackmethProc(static_cast<TDSCallbackTunnelEvent*>(new TCallbackTunnelEventProc(CallbackTunnelEventProc)));
	 TDSCallbackTunnelManager::Instance->AddTunnelEvent(CallbackmethProc);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::DataModuleDestroy(TObject *Sender)
{
	ServerUsersDestroyed = true;
	delete AccQuery;
	delete m_DataSet;
	delete LogQuery;
	delete m_Query;
	delete ConnectionList;
	delete SysConnectionList;
	delete UserList;
	delete WorkFlowSerList;
	delete SmsSerList;
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::StartServer()
{

   //读参数
  InitServer();
  FSDSTCPServerTransport->Port = FTCPIPPort;
  FSDSHTTPService->DSPort	   = FTCPIPPort;
  //FSDSHTTPServiceSS->DSPort	   = FTCPIPPort;
  FSDSHTTPService->HttpPort    = FHttpPort;
//  FSDSHTTPServiceSS->HttpPort  = FHttpsPort;
  FSDSHTTPServiceFileDispatcher->RootDirectory=AppPath;
 // FSDSCertFiles->CertFile =  ProxyPath + "xzepm.crt";
 // FSDSCertFiles->KeyFile= ProxyPath + "xzepm.key";
 // FSDSCertFiles->RootCertFile=ProxyPath + "ca.pem";
  FSDSServer->Start();
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::Shutdown()
{
	FSDSServer->Stop();
	for (int i=0;i<ConnectionList->Count;i++)
	{
		TADOConnection *acon = (TADOConnection*)ConnectionList->Objects[i];
		delete acon;
	}
	ConnectionList->Clear();

	for (int i=0;i<SysConnectionList->Count;i++)
	{
		TADOConnection *scon = (TADOConnection*)SysConnectionList->Objects[i];
		delete scon;
	}
	SysConnectionList->Clear();

	for (int j=0;j<UserList->Count;j++)
	{
		TLoginUserRecord *ur = (TLoginUserRecord*)UserList->Objects[j];
		delete ur;
	}
	UserList->Clear();
	for(int k=0;k<WorkFlowSerList->Count;k++)
	{
	  TWorkFlowSerRecord *wfr=(TWorkFlowSerRecord*)WorkFlowSerList->Objects[k];
	  delete wfr;
	}
	WorkFlowSerList->Clear();
	for(int s=0;s<SmsSerList->Count;s++)
	{
	  TSmsSerRecord *smsr=(TSmsSerRecord*)SmsSerList->Objects[s];
	  delete smsr;
	}
	SmsSerList->Clear();
    FSDSServer->Stop();
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::InitServer()
{
	try
	{
		// 加载参数
		LoadSetting();
		LoadFtpSetting();
		if(FVersion >= 7)
			LoadMailSetting();

		AnsiString ps = "";
		CheckDog(); // 检测狗并读取受权码
		BeginTime = GetTickCount();
#ifndef _BYDEV_
#ifndef _DEMO_
		if(!IsTrial && ReadProductSerial(ps))
		{
			if(ReadModulesInfor())
				m_ModuleList = GetAllModulesSign();
			int totalTrialTimes = 0, canUseTimes = 0;
			int DogStatus = 0;
			ReadDogStatus(DogStatus);
			if(DogStatus != 0)
			{
				ReadTrialTimes(totalTrialTimes);
				ReadUsedTimes(canUseTimes);
				if(canUseTimes < totalTrialTimes)
				{
					FTestOverTimes = false;
					WriteUsedTimes(canUseTimes + 1);
				}
				else
				{
					FTestOverTimes = true;
				}
			}
		}
#endif
#endif
		InitConnData();
		TrialTimer->Enabled = IsTrial;
		return true;
	}
	catch (Exception &e)
	{
		LogMessage("初始化代理服务器参数设置错误.");
		return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::CheckDog()
{
#ifdef _BYDEV_
	m_SysTag = String(GetLocalVersion());
	FLicenseKey = 0;
	return true;
#endif
	try
	{
#ifndef _DEMO_
#ifndef _BYDEV_
		if(CheckSoftDog())
		{
			if(OpenSoftDog())
			{
               FSoftDog=true;
				AnsiString spassword;
				if(ReadUIDKey(spassword))
				{
					FLicenseKey = MakePrimalKey(spassword.c_str());
					FLicenseKey = GetPolicKey(FLicenseKey);
					FLicenseKey = GetLicenseKey(FLicenseKey);
				}
				else
				{
					FLicenseKey = -1;
				}

				if(!ReadUserCount(FMaxUserCount))
					FMaxUserCount = 1;

				if(!ReadUserCount2(FMaxOAUserCount))
					FMaxOAUserCount = FMaxUserCount;

				IsTrial = false;
				AnsiString ps;
				if(ReadProductSerial(ps))
					m_SysTag = ps;
				return true;
			}
		}
		else
		{
			LogMessage("找不到软件狗，系统运行在试用版状态");
		}
#endif
#endif
		// 试用版
		// FPrimaryKey
		IsTrial = true;
		m_SysTag = String(GetLocalVersion());
		unsigned char maskstr = 73;
		String sFileName = System::Ioutils::TPath::Combine(AppPath,saftfilename);
		if(FileExists(sFileName))
		{
			AnsiString spassword;
			spassword.SetLength(16);
			TMemoryStream *ms = new TMemoryStream();
			ms->LoadFromFile(sFileName);
			ms->Seek(600, soFromBeginning);
			ms->Read(spassword.c_str(), 16);
			for (int i = 1; i <= spassword.Length(); i++)
				spassword[i] = spassword[i] ^ maskstr;
			int index = spassword.Pos("\r\n");
			spassword.Delete(index, spassword.Length() - index + 1);

			FLicenseKey = MakePrimalKey(spassword.c_str());
			FLicenseKey = GetPolicKey(FLicenseKey);
			FLicenseKey = GetLicenseKey(FLicenseKey);
			delete ms;
		}
		else
		{
			FLicenseKey = -1;
    }
		return false;
	}
	catch (...)
	{
		LogMessage("读取安全数据或软件狗错误");
		return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::InitConnData()
{
	try
	{
		// 开发用
#ifdef _BYDEV_
		 if(Date()>DevEndDate)
		 {
			 Application->Terminate();
		 }
#endif
		SysADOConnection->Close();
		SysADOConnection->ConnectionString = FDBConnectString;
		SysADOConnection->Open();

		AccADOConnection->Close();
		AccADOConnection->ConnectionString = FAccDBConnectString;
		AccADOConnection->Open();
		// ADOConnTimer->Enabled=false;
#ifndef _BYDEV_
/*		if (!IsUpdateMenu && SysADOConnection->Connected) {
			FlashSystemMenu(); // zws 20101116重新启用，不但要刷新系统库，而且要刷新每个账套库
			// 下面是获取系统菜单，记录可用菜单记录
			m_DataSet->Close();
			m_DataSet->SQL->Text =
				"select count(*) as menucount from syModule where SyModuleEnable=1";
			m_DataSet->Open();
			if (m_DataSet->RecordCount > 0)
				SysMenuCount = m_DataSet->Fields->Fields[0]->AsInteger;
		}  */
#endif
		// 开发用屏蔽
#ifdef _DEMO_
		if(IsTrial)
		{
			String DefaultDB = SysADOConnection->DefaultDatabase;
			m_DataSet->Close();
			if(FDbType == 1)
				m_DataSet->SQL->Text = "select crdate from master..sysdatabases where name=" +QuotedStr(DefaultDB);
			else // ORACLE
				m_DataSet->SQL->Text = "SELECT CREATED FROM V$DATABASE WHERE UPPER(NAME)=" +QuotedStr(UpperCase(DefaultDB));
			m_DataSet->Open();
			if(m_DataSet->RecordCount > 0)
			{
				TDateTime createdate = StrToDateTime(m_DataSet->Fields->Fields[0]->AsString,fmtSettings);
				TestOverTimes = (double)(Now() - createdate) > _TESTDATE_;
			}
			m_DataSet->Close();
		}
		// ORACLE
#endif
		return SysADOConnection->Connected;
	}
	catch (Exception &e)
	{
		LogMessage("连接系统数据库错误");
		return false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::LoadSetting()
{
	String settingfile = System::Ioutils::TPath::Combine(AppPath,"default.xml");
	try
	{
		if(!FileExists(settingfile))
		{
			LogMessage("找不到系统配置文件" + settingfile);
			return;
		}
		_di_IXMLDocument xdoc = NewXMLDocument();
		xdoc->LoadFromFile(settingfile);
		xdoc->Active = true;
		IXMLNodeList *NodeList = xdoc->ChildNodes;
		IXMLNode *selectNode = NULL;
		for (int i = 0; i < NodeList->Count; i++)
		{
			IXMLNode *curNode = NodeList->Nodes[i];
			if(curNode->LocalName == WideString("DefaultSettings") && (curNode->GetAttribute("Version") == String("2.0.0.2")))
			{
				selectNode = curNode;
				break;
			}
		}

		if(selectNode != NULL)
		{
			IXMLNodeList *sNodeList = selectNode->ChildNodes;
			for (int j = 0; j < sNodeList->Count; j++)
			{
				IXMLNode *sNode = sNodeList->Nodes[j];
				if(sNode->GetAttribute("Name") == String("DbConStr")) // 系统库连接串
					FDBConnectString = DecryptAES(sNode->GetAttribute("value"));
				else if(sNode->GetAttribute("Name") == String("DbType"))
					FDbType = sNode->GetAttribute("value"); // 数据库类型
				if(sNode->GetAttribute("Name") == String("AccDbConStr")) // 账套库连接串
					FAccDBConnectString = DecryptAES(sNode->GetAttribute("value"));
				else if(sNode->GetAttribute("Name") == String("AccDbType"))
					FAccDbType = sNode->GetAttribute("value"); // 数据库类型
				else if(sNode->GetAttribute("Name") == String("TCPIP"))
					FTCPIPPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("HTTP"))
					FHttpPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("HTTPS"))
					FHttpsPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("Admin"))
					FAdminName = sNode->GetAttribute("value"); // 管理员
				else if(sNode->GetAttribute("Name") == String("PassWord"))
					SPassWord = sNode->GetAttribute("value"); // 口令
				else if(sNode->GetAttribute("Name") == String("UniqueLog"))
					LogUnigue = sNode->GetAttribute("value"); // 是否限制同一帐户唯一登录

			}
		}
		xdoc->Active = false;
	}
	catch (Exception &e)
	{
		LogMessage("读取参数错误," + e.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::LoadFtpSetting()
{
	bool FirstTime = false;
	String settingfile = System::Ioutils::TPath::Combine(AppPath,"Ftpdefault.xml");
	try
	{
		if (!FileExists(settingfile))
		{
			LogMessage("找不到系统配置文件" + settingfile);
			return;
		}
		_di_IXMLDocument xdoc = NewXMLDocument();
		xdoc->LoadFromFile(settingfile);
		xdoc->Active = true;
		IXMLNodeList *NodeList = xdoc->ChildNodes;
		IXMLNode *selectNode = NULL;
		for (int i = 0; i < NodeList->Count; i++)
		{
			IXMLNode *curNode = NodeList->Nodes[i];
			if (curNode->LocalName == WideString("DefaultSettings") &&
				(curNode->GetAttribute("Version") == String("2.0.0.2")))
			{
				selectNode = curNode;
				FirstTime = selectNode->GetAttribute("FirstTime");
				break;
			}
		}

		if (selectNode != NULL)
		{
			selectNode->SetAttribute("FirstTime", false);
			IXMLNodeList *sNodeList = selectNode->ChildNodes;
			for (int j = 0; j < sNodeList->Count; j++)
			{
				IXMLNode *sNode = sNodeList->Nodes[j];
				if (sNode->GetAttribute("Name") == String("NeedFTP"))
					ftpNeedFTP = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("InternetIP"))
					ftpInternetIP = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("InteranetIP"))
					ftpInteranetIP = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("Passive"))
					ftpPassive = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("Port"))
					ftpPort = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("UserName"))
					ftpUserName = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("Password"))
					ftpPassword = DecryptAES(sNode->GetAttribute("value"));
				else if (sNode->GetAttribute("Name") == String("UpdateURL"))
					FUpdateURL = sNode->GetAttribute("value");
			}
		}
		xdoc->Active = false;
	}
	catch (Exception &e)
	{
		LogMessage("读取Ftp参数错误," + e.Message);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::LoadMailSetting()
{
	bool FirstTime = false;
	String settingfile = System::Ioutils::TPath::Combine(AppPath,"MailDefault.xml");
	try
	{
		if(!FileExists(settingfile))
		{
			LogMessage("找不到系统配置文件" + settingfile);
			return;
		}
		_di_IXMLDocument xdoc = NewXMLDocument();
		xdoc->LoadFromFile(settingfile);
		xdoc->Active = true;
		IXMLNodeList *NodeList = xdoc->ChildNodes;
		IXMLNode *selectNode = NULL;
		for (int i = 0; i < NodeList->Count; i++)
		{
			IXMLNode *curNode = NodeList->Nodes[i];
			if (curNode->LocalName == WideString("DefaultSettings") && (curNode->GetAttribute("Version") == String("2.0.0.2")))
			{
				selectNode = curNode;
				FirstTime = selectNode->GetAttribute("FirstTime");
				break;
			}
		}

		if(selectNode != NULL)
		{
			selectNode->SetAttribute("FirstTime", false);
			IXMLNodeList *sNodeList = selectNode->ChildNodes;
			for (int j = 0; j < sNodeList->Count; j++)
			{
				IXMLNode *sNode = sNodeList->Nodes[j];
				if (sNode->GetAttribute("Name") == String("MailServer"))
					FMailServer = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("SendEMail"))
					FSendEMail = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("Port"))
					FMailPort = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("UserName"))
					FMailUserName = sNode->GetAttribute("value");
				else if (sNode->GetAttribute("Name") == String("Password"))
					FMailPassword = DecryptAES(sNode->GetAttribute("value"));
			}
		}
		xdoc->Active = false;
	}
	catch (Exception &e)
	{
		LogMessage("读取Mail参数错误," + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::FSDSServerConnect(TDSConnectEventObject *DSConnectEventObject)
{
	//增加登录日志
	//从浏览器客户端获取不了 ChannelInfo
	if((DSConnectEventObject!=NULL)&&(DSConnectEventObject->ChannelInfo!=NULL))
	{
		//通过转换可以获取更多信息
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		threadSession->PutData("IpAddress",DSConnectEventObject->ChannelInfo->ClientInfo.IpAddress);
		threadSession->PutData("ClientPort",DSConnectEventObject->ChannelInfo->ClientInfo.ClientPort);
		threadSession->PutData("Protocol",DSConnectEventObject->ChannelInfo->ClientInfo.Protocol);
		threadSession->PutData("AppName",DSConnectEventObject->ChannelInfo->ClientInfo.AppName);

		TLoginUserRecord * curUser=GetCurLoginUser();
		if(curUser)
		{
			curUser->IP               = threadSession->GetData("IpAddress");
			curUser->ClientPort       = threadSession->GetData("ClientPort");
			curUser->Protocol         = threadSession->GetData("Protocol");
			curUser->LoginTime        = threadSession->StartDateTime;
	//TLoginUser::UpdateCurUserCallbackClientID(String const Value)中处理登录事件
//			NotifyUserChange(curUser,true);

		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::FSDSAuthenticationManagerUserAuthenticate(
TObject *Sender, const UnicodeString Protocol, const UnicodeString Context,
const UnicodeString User, const UnicodeString Password, bool &valid, TStrings *UserRoles)
{
	// DONE : Validate the client user and password.
	// If role-based authorization is needed, add role names to the UserRoles parameter
	String msg="";
	if((User== GetSysAdmin()) && (Password==GetSysPassWord()))      //管理工具，工作流，短信服务器等,
	 {	valid=true;										 //连接参数需要设置
        UserRoles->Add("admin");
	 }
	else
		valid = IsValidUser(User,Password,msg);
	if(valid==false&&msg>"")
	   throw Exception(msg);
}
//----------------------------------------------------------------------------
void __fastcall TDMServerContainer::LogMessage(String Message, unsigned EventType, int Category, int ID)
{
	if(FEventLog == 0)
		FEventLog = RegisterEventSource(NULL, L"FSAppBroker");
	LPCTSTR p = Message.c_str();
	ReportEvent(FEventLog, EventType, Category, ID, NULL, 1, 0, &p, NULL);
}
//---------------------------------------------------------------------------
double __fastcall TDMServerContainer::GetRemainTimes()
{
	if(IsTrial)
		return LASTTIMES - (GetTickCount() - BeginTime) / (1000*60);
	else
		return -1;
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::ADOConnTimerTimer(TObject *Sender)
{
	if(SysADOConnection->Connected == false)
	{
		InitConnData();
	}
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::SysADOConnectionAfterConnect(TObject *Sender)
{
	ADOConnTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::SysADOConnectionAfterDisconnect(TObject *Sender)
{
	ADOConnTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::WriteUserLog(String UserID, String IPAddress, String ClassCode,String AcckBook, int type, String Desc)
{
	System::TMonitor::Enter(LogQuery);
	try
	{
	   //if(Desc.Length()>300) //不要太长了，占空间
	  //	Desc=Desc.SubString0(0,300);
		String InsertString = " Insert into SyUserOpLog(SyULogUserCode,SyULogIP,SyULogClassCode,SyULogOperType,SyULogAccbook,SyULogDesc)"
													" Values ('"+UserID+"','"+IPAddress+"','"+ClassCode+"',"+IntToStr(type)+",'"+AcckBook+"',"+QuotedStr(Desc)+")";
		LogQuery->Close();
		LogQuery->SQL->Text = InsertString;
		LogQuery->ExecSQL();
	}
	__finally
	{
	  System::TMonitor::Exit(LogQuery);
	 //	LogMessage("记录日志错误:" + e.Message);
	}
}
//---------------------------------------------------------------------------
TMsgStream* __fastcall TDMServerContainer::GetAccInformation()
{
	TMsgStream *MsgStream = new TMsgStream();
	TStringList* AccList  = new TStringList;
	String AccString;
	int index;
	try
	{
		MsgStream->ValueAttribute["status"]     = "s";
		MsgStream->ValueAttribute["modulelist"] = m_ModuleList;
		MsgStream->ValueAttribute["softname"]   = "先智软件";
		MsgStream->ValueAttribute["version"]    = m_SysTag;
		MsgStream->ValueAttribute["devcorp"]    = "广州先智软件科技有限公司";
		MsgStream->ValueAttribute["homepage"]   = "www.xzepm.com";
		MsgStream->ValueAttribute["email"]      = "zhanws@xzepm.com";
		MsgStream->ValueAttribute["IsTrial"]    = IsTrial ? "1" : "0";
		// 2004-11-06
		// ftp
		MsgStream->ValueAttribute["ftpInteranetIP"] = ftpInteranetIP;
		MsgStream->ValueAttribute["ftpInternetIP"]  = ftpInternetIP;
		MsgStream->ValueAttribute["ftpUserName"]    = ftpUserName;
		MsgStream->ValueAttribute["ftpPassword"]    = ftpPassword;
		MsgStream->ValueAttribute["ftpPort"]        = ftpPort;
		MsgStream->ValueAttribute["ftpNeedFTP"]     = ftpNeedFTP ? 1 : 0;
		MsgStream->ValueAttribute["ftpPassive"]     = ftpPassive ? 1 : 0;
		MsgStream->ValueAttribute["updateurl"]      = FUpdateURL;
		// mail
		MsgStream->ValueAttribute["mailServer"]     = FMailServer;
		MsgStream->ValueAttribute["sendEmail"]      = FSendEMail;
		MsgStream->ValueAttribute["mailUserName"]   = FMailUserName;
		MsgStream->ValueAttribute["mailPassword"]   = FMailPassword;
		MsgStream->ValueAttribute["mailPort"]       = FMailPort;
       	//sysconnectString
		MsgStream->ValueAttribute["sysdbconstring"]      = FDBConnectString;
		// dbtype
		MsgStream->ValueAttribute["sysdbtype"] = FDbType;
		//accconnectString
		MsgStream->ValueAttribute["accdbconstring"]      = FAccDBConnectString;
		// accdbtype
		MsgStream->ValueAttribute["accdbtype"] = FAccDbType;
		SysADOQuery->Close();
		SysADOQuery->SQL->Text="select * from SyAccBook where SyAccBookStatue=1" ;
		SysADOQuery->Open();
		SysADOQuery->First();
		while (!SysADOQuery->Eof)
		{
			AccString = SysADOQuery->FieldByName("SyAccBookCode")->AsString + "=" + SysADOQuery->FieldByName("SyAccBookName")->AsString;
			AccList->Add(AccString);
			SysADOQuery->Next();
		}

		if (AccList->Count > 0)
		{
			AccList->SaveToStream(MsgStream->MasterDataSet);
		}
		delete AccList;
		return MsgStream;
	}
	catch (Exception &e)
	{
		MsgStream->ValueAttribute["status"] = "f";
		LogMessage("代理向客户端发账套数据" + e.Message);
		return MsgStream;
	}
}
//---------------------------------------------------------------------------
TJSONObject* __fastcall TDMServerContainer::GetAccInforJSON()
{
	TJSONObject *elmObject=new TJSONObject();
	try
	{
		elmObject->AddPair(new TJSONPair("status","s"));
		elmObject->AddPair(new TJSONPair("modulelist",m_ModuleList));
		elmObject->AddPair(new TJSONPair("modulelist", m_ModuleList));
		elmObject->AddPair(new TJSONPair("softname", "先智软件"));
		elmObject->AddPair(new TJSONPair("version", m_SysTag));
		elmObject->AddPair(new TJSONPair("devcorp", "广州先智软件科技有限公司"));
		elmObject->AddPair(new TJSONPair("homepage", "www.xzepm.com"));
		elmObject->AddPair(new TJSONPair("email", "zhanws@xzepm.com"));
		elmObject->AddPair(new TJSONPair("IsTrial", IsTrial ? "1" : "0"));
		// 2004-11-06
		// ftp
		elmObject->AddPair(new TJSONPair("ftpInteranetIP", ftpInteranetIP));
		elmObject->AddPair(new TJSONPair("ftpInternetIP", ftpInternetIP));
		elmObject->AddPair(new TJSONPair("ftpUserName", ftpUserName));
		elmObject->AddPair(new TJSONPair("ftpPassword", ftpPassword));
		elmObject->AddPair(new TJSONPair("ftpPort", ftpPort));
		elmObject->AddPair(new TJSONPair("ftpNeedFTP", ftpNeedFTP ? 1 : 0));
		elmObject->AddPair(new TJSONPair("ftpPassive", ftpPassive ? 1 : 0));
		elmObject->AddPair(new TJSONPair("updateurl", FUpdateURL));
		// mail
		elmObject->AddPair(new TJSONPair("mailServer", FMailServer));
		elmObject->AddPair(new TJSONPair("sendEmail", FSendEMail));
		elmObject->AddPair(new TJSONPair("mailUserName", FMailUserName));
		elmObject->AddPair(new TJSONPair("mailPassword", FMailPassword));
		elmObject->AddPair(new TJSONPair("mailPort", FMailPort));
		//sysconnectString
		elmObject->AddPair(new TJSONPair("sysdbconstring", FDBConnectString));
		// dbtype
		elmObject->AddPair(new TJSONPair("sysdbtype", FDbType));

		//accconnectString
		elmObject->AddPair(new TJSONPair("accdbconstring", FAccDBConnectString));
		// accdbtype
		elmObject->AddPair(new TJSONPair("accdbtype", FAccDbType));

	 /*	TJSONArray* AccList=new TJSONArray();
		SysADOQuery->Close();
		SysADOQuery->SQL->Text="select * from SyAccBook where SyAccBookStatue=1" ;
		SysADOQuery->Open();
		SysADOQuery->First();
		while (!SysADOQuery->Eof)
		{
			TJSONObject *rcdOject=new TJSONObject();
			rcdOject->AddPair(new TJSONPair("syaccbookcode",SysADOQuery->FieldByName("SyAccBookCode")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccbookname",SysADOQuery->FieldByName("SyAccBookName")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccdbtype",SysADOQuery->FieldByName("SyAccBookDBType")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccbookconstring",DecryptAES(SysADOQuery->FieldByName("SyAccBookConString")->AsString)));
			AccList->AddElement(rcdOject);
			SysADOQuery->Next();
		}
		SysADOQuery->Close();
		elmObject->AddPair(new TJSONPair("accbooks",AccList));  */
		return elmObject;

	}
	catch (Exception &e)
	{
		elmObject->AddPair(new TJSONPair("status","f"));
		elmObject->AddPair(new TJSONPair("errormessage",e.Message));
		LogMessage("代理向客户端发账套数据" + e.Message);
		return elmObject;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::FlashSystemMenu()
{
	// 开发用
#ifdef _BYDEV_
	FPrjLevel = 10;
	IsUpdateMenu = true;
	return;
#endif
	// 刷新菜单数据库
	if (FLicenseKey != -1)
	{
		try
		{
			FPrjLevel = GetProjectLevel(FLicenseKey);
			UpdateModuleData();
			UpdateWindow();
			UpdateUserDefRight();
			UpdateUserDefFunc();
			UpdateLibFile();
			UpdateSystemRight();
			IsUpdateMenu = true;
		}
		catch (...)
		{
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::InputData(TADOQuery * mQuery, String tablename)
{
	TADOQuery *InsertQuery = new TADOQuery(NULL);
	InsertQuery->Connection = SysADOConnection;
	InsertQuery->ParamCheck = false;
	try
	{
		// 清除记录
		InsertQuery->Close();
		InsertQuery->SQL->Text = "truncate table " + tablename;
		InsertQuery->ExecSQL();
		// 打开空表
		InsertQuery->Close();
		InsertQuery->SQL->Text = "select * from " + tablename;
		InsertQuery->Open();
		// 逐条插入记录
		mQuery->First();
		while (!mQuery->Eof)
		{
			InsertQuery->Append();
			for (int j = 0; j < InsertQuery->FieldCount; j++)
			{
				InsertQuery->FieldByName(InsertQuery->Fields->Fields[j]->FieldName)->Value = mQuery->FieldByName(InsertQuery->Fields->Fields[j]->FieldName)->Value;
			}
			InsertQuery->Post();
			mQuery->Next();
		}
	}
	__finally
	{
		delete InsertQuery;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateModuleData()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SyModule.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetModuleActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetModuleData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if (szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SyModule");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if (FileExists(temFile))
			DeleteFile(temFile);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateWindow()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SyClientWin.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetWindowActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetWindowData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if (szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SyClientWin");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if (FileExists(temFile))
			DeleteFile(temFile);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateUserDefRight()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SyWinRight.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetUserDefRightActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetUserDefRightData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if (szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SyWinRight");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if (FileExists(temFile))
			DeleteFile(temFile);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateUserDefFunc()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SyWinFunc.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetUserDefFuncActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetUserDefFuncData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if (szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SyWinFunc");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if(FileExists(temFile))
			DeleteFile(temFile);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateLibFile()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SyClientLib.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetLibFileActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetLibFileData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if(szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SyClientLib");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if(FileExists(temFile))
			DeleteFile(temFile);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateSystemRight()
{
	String temFile = System::Ioutils::TPath::Combine(AppPath,"SySystemRight.dat");
	TMemoryStream *ms = new TMemoryStream();
	Char *szDataBuf;
	TADOQuery *mQuery = new TADOQuery(NULL);
	mQuery->ParamCheck = false;
	try
	{
		int dwsize = GetSystemRightActualSize(FLicenseKey);
		szDataBuf = new Char[dwsize];
		GetSystemRightData(szDataBuf, dwsize, FLicenseKey);
		ms->Write(szDataBuf, dwsize);
		if(szDataBuf)
			delete[]szDataBuf;
		ms->Size = dwsize;
		ms->Seek(0, soFromBeginning);
		ms->SaveToFile(temFile);
		mQuery->LoadFromFile(temFile);
		InputData(mQuery, "SySystemRight");
	}
	__finally
	{
		delete ms;
		delete mQuery;
		if(FileExists(temFile))
			DeleteFile(temFile);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::IsValidAdmin(String UserLoginCode,String strPassword,String &msg)
{
	bool IsValid = false;
	String Password = EncryptAES(Trim(strPassword));
	m_Query->Close();
	m_Query->Connection = SysADOConnection;
	m_Query->SQL->Text = "select * from SyAdmin where SyAdminCode="+QuotedStr(UserLoginCode)+ " AND SyAdminPassword="+QuotedStr(Password);
	m_Query->Open();
	if(m_Query->RecordCount>0)
		IsValid=true;
	else
		msg = "系统没有找到您的登录资料,向管理员申请登记吧";
	return IsValid;
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::IsValidUser(String UserLoginCode,String strPassword,String &msg)
{
	//AccbookID==""为管理工具认证
	bool IsValid = false;
	msg="";
	int threadSessionId = TDSSessionManager::Instance->GetThreadSession()->Id;

	String UserName="",UserCode="",UserID="";
	String Password = EncryptAES(Trim(strPassword));
	TDateTime st, et, ct;
#ifndef _DEMO_   //试用版 IsTrial=true
#ifndef _BYDEV_  //开发版 IsTrial=true
		if(IsTrial) // 正式版 有狗IsTrial=false,否则IsTrial=true
		{
			msg = "软件狗未找到,不能提供服务!";
			return IsValid;
		}

		/*			if (IsFromRemoter && !HaveRemoter) {
		msg="该产品没有包含远程模块，请购买远程模块!";
		goto EXITDO;
		}  */

#endif
#endif

#ifdef _BYDEV_
		if(Date() > DevEndDate)
		{
			msg = "已过开发试用期,拒绝提供服务!";
			return IsValid;
		}
#endif

		if(TestOverTimes)
		{
			if(!IsTrial)
				msg = "软件狗已过试用期限或次数,请激活,拒绝提供服务!";
			else
				msg = "已过试用期限,请使用正式版,请与代理商联系!";
			return IsValid;
		}

		if(FLicenseKey == -1)
		{
			msg = "安全数据授权码信息不对,请与开发商联系,拒绝提供服务!";
			return IsValid;
		}

		int maxUserCounts = FMaxUserCount;
		if(FMaxOAUserCount > maxUserCounts)
			maxUserCounts = FMaxOAUserCount;
		if(UserList->Count > maxUserCounts)
		{
			msg = "登录用户数已满,请稍候再试登录!";
			return IsValid;
		}

		if(FLogUnigue)
		{
			bool bFind = false;
			for(int i=0;i<UserList->Count;i++)
			{
				TLoginUserRecord *cuser = (TLoginUserRecord*) UserList->Objects[i];
				if(cuser && cuser->UserLoginCode==UserLoginCode)
				{
				   //检测连接是否有效
				   if(!cuser->CallBackClientID.IsEmpty())
				   {
						bFind=TestClientConnection(cuser->CallBackClientID,cuser->AccBookID);
						if(!bFind)//删除无效链接记录
						{
							 RemoveUser(cuser->ThreadSessionId);
							 //UserList->Delete(i);
							 //delete cuser;
						}
					}
					break; //离开循环
				}
			}

			if(bFind)
			{
				msg = "该用户已登录,且不允许再次登录,请稍候再试登录!";
				return IsValid;
			}
		}

		// 转入帐套处理
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->Connection = AccADOConnection;//GetConnection();
		// 通过用户编码进行关联，由登录编码UserLogonCode找用户编码UserIDCode，最后找到用户信息tbEmployeeData
		m_Query->SQL->Text = " select EmpPKID,EmpState,EmpName,EmpNum from tbEmployeeData"
												 " where upper(EmpNum)=(select Upper(UserIDCode) from tbUser where Upper(UserLogonCode)='"+UserLoginCode.UpperCase()+"')";
		m_Query->Open();
		if(m_Query->RecordCount < 1)
		{
			m_Query->Close();
			msg = "用户未找到或未授予登录信息,请于管理员确认后再试登录!";
			return IsValid;
		}
		else
		{
			UserName = m_Query->FieldByName("EmpName")->AsString;
			UserCode = m_Query->FieldByName("EmpNum")->AsString;
			UserID   = m_Query->FieldByName("EmpPKID")->AsString;
			// 是否辞职了
			if(m_Query->FieldByName("EmpState")->AsInteger == 2)
			{
				m_Query->Close();
				msg = "该员工已辞职不允许登陆,请与管理员确认后再试登录!";
				return IsValid;
			}
		}
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->SQL->Text = "select * from tbUser where Upper(UserLogonCode)='"+UserLoginCode.UpperCase() + "'";
		m_Query->Open();
		// 未找到登录信息
		if(m_Query->RecordCount < 1)
		{
			m_Query->Close();
			msg = "用户未找到或未授予登录信息,请与管理员确认后再试登录!";
			return IsValid;
		}

		// 口令不对
		if(m_Query->FieldByName("UserPassword")->AsString != Password)
		{
			m_Query->Close();
			msg = "用户登录密码不正确,请重输密码再试登录!";
			return IsValid;
		}

		// 账户是否过期
		if(StrToDate(m_Query->FieldByName("UserLimitDate")->AsString,fmtSettings) < Now())
		{
			m_Query->Close();
			msg = "当前用户帐号已过期,请与系统管理员联系!";
			return IsValid;
		}

		// 网卡地址限制
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		String UserIP = threadSession->GetData("IpAddress");
		{
			if(m_Query->FieldByName("UserSpecalLimit")->AsInteger == 1
			|| m_Query->FieldByName("UserSpecalLimit")->AsInteger == 3)
			{
				if(m_Query->FieldByName("UserLoginIP")->AsString!=UserIP)
				{
					m_Query->Close();
					msg = "用户不在指定的机器登录!";
					return IsValid;
				}
			}
		}

		// 登录时间限制
		if(m_Query->FieldByName("UserSpecalLimit")->AsInteger == 2
		|| m_Query->FieldByName("UserSpecalLimit")->AsInteger == 3)
		{
			try
			{
				st = StrToTime(m_Query->FieldByName("UserStartTime")->AsString, fmtSettings);
				et = StrToTime(m_Query->FieldByName("UserEndTime")->AsString, fmtSettings);
			}
			catch (...)
			{
				m_Query->Close();
				msg = "登录时间不是合法时间,请重新设置!";
				return IsValid;
			}
			ct = Now();
			if(ct < st || ct > et)
			{
				m_Query->Close();
				msg = "您不能在当前时间登录!";
				return IsValid;
			}
		}
	IsValid = true;
	SetConnection(threadSessionId); //创建帐套连接，后续才能进行帐套用户判断
	SetSysConnection(threadSessionId);
	UpdateCurUserRecord(UserID,UserLoginCode,UserCode,UserName);
	return IsValid;
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::SetConnection(int SessionID)
{
	TADOConnection *aConnection = NULL;
	System::TMonitor::Enter(ConnectionList);
	try
	{
		int oindex = ConnectionList->IndexOf(SessionID);
		if(oindex>-1)
			aConnection = (TADOConnection*)ConnectionList->Objects[oindex];
		if(aConnection==NULL)
			{
				aConnection = new TADOConnection(NULL);
				aConnection->LoginPrompt = false;
				ConnectionList->AddObject(SessionID,aConnection);
			}
			aConnection->Close();
			aConnection->ConnectionString = FAccDBConnectString;
			aConnection->Tag = FAccDbType;//保存数据库类型
/*		SysADOQuery->Close();
		SysADOQuery->SQL->Text="select * from SyAccBook where SyAccBookCode="+QuotedStr(AccbookID);
		SysADOQuery->Open();
		if(SysADOQuery->RecordCount>0)
		{
			int FaccDbType = SysADOQuery->FieldByName("SyAccBookDBType")->AsInteger;
			String ConString = Trim(DecryptAES(SysADOQuery->FieldByName("SyAccBookConString")->AsString));
			if(aConnection==NULL)
			{
				aConnection = new TADOConnection(NULL);
				aConnection->LoginPrompt = false;
				ConnectionList->AddObject(threadSessionId,aConnection);
			}
			aConnection->Close();
			aConnection->ConnectionString = ConString;
			aConnection->Tag = FaccDbType;//保存数据库类型
		} */
	}
	__finally
	{
		System::TMonitor::Exit(ConnectionList);
	}
}
//---------------------------------------------------------------------------
TADOConnection* __fastcall TDMServerContainer::GetConnection()
{
	int threadSessionId = TDSSessionManager::Instance->GetThreadSession()->Id;
	TADOConnection *reADOCon = NULL;
	System::TMonitor::Enter(ConnectionList);
	try
	{
		int oindex = ConnectionList->IndexOf(threadSessionId);
		if(oindex>-1)
			reADOCon = (TADOConnection*)ConnectionList->Objects[oindex];
	}
	__finally
	{
		System::TMonitor::Exit(ConnectionList);
	}
	return reADOCon;
}
TADOConnection* __fastcall TDMServerContainer::GetSysConnection()
{
	int threadSessionId = TDSSessionManager::Instance->GetThreadSession()->Id;
	TADOConnection *reADOCon = NULL;
	System::TMonitor::Enter(SysConnectionList);
	try
	{
		int oindex = SysConnectionList->IndexOf(threadSessionId);
		if(oindex>-1)
			reADOCon = (TADOConnection*)SysConnectionList->Objects[oindex];
	}
	__finally
	{
		System::TMonitor::Exit(SysConnectionList);
	}
	return reADOCon;
}
void __fastcall TDMServerContainer::SetSysConnection(int SessionID)
{
	TADOConnection *aConnection = NULL;
	System::TMonitor::Enter(SysConnectionList);
	try
	{
		int oindex = SysConnectionList->IndexOf(SessionID);
		if(oindex>-1)
			aConnection = (TADOConnection*)SysConnectionList->Objects[oindex];
		if(aConnection==NULL)
			{
				aConnection = new TADOConnection(NULL);
				aConnection->LoginPrompt = false;
				SysConnectionList->AddObject(SessionID,aConnection);
			}
			aConnection->Close();
			aConnection->ConnectionString = FDBConnectString;
			aConnection->Tag = FDbType;//保存数据库类型
	}
	__finally
	{
		System::TMonitor::Exit(SysConnectionList);
	}
}
//----------------------------------------------------------------------------
void __fastcall TDMServerContainer::UpdateCurUserRecord(String UserID,String UserLoginCode,String UserCode,String UserName)
{
	System::TMonitor::Enter(UserList);
	try
	{
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		TLoginUserRecord *user = new TLoginUserRecord();
		user->ThreadSessionId  = threadSession->Id;
		//下面这些参数还没获取到
//		user->IP               = threadSession->GetData("IpAddress");
//		user->ClientPort       = threadSession->GetData("ClientPort");
//		user->Protocol         = threadSession->GetData("Protocol");
//		user->LoginTime        = threadSession->StartDateTime;
		user->UserID           = UserID;
		user->UserLoginCode    = UserLoginCode;
		user->UserCode         = UserCode;
		user->UserName         = UserName;
		user->AccBookID        = "0";
		user->SysDbType        = FDbType;
		user->AccDbType        = FAccDbType;
		UserList->AddObject(threadSession->Id,(TObject*)user);
	}
	__finally
	{
		System::TMonitor::Exit(UserList);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::DSSyAccBookGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
  PersistentClass= __classid(TSyAccBook);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSLoginUserGetClass(TDSServerClass *DSServerClass,
          TPersistentClass &PersistentClass)
{
	PersistentClass= __classid(TLoginUser);
}
//---------------------------------------------------------------------------
TLoginUserRecord * __fastcall TDMServerContainer::GetCurLoginUser()
{
	TLoginUserRecord *user = NULL;
	int threadSessionId = TDSSessionManager::Instance->GetThreadSession()->Id;
	System::TMonitor::Enter(UserList);
	try
	{
		int uindex = UserList->IndexOf(threadSessionId);
		if(uindex>-1)
		{
			user = (TLoginUserRecord*)UserList->Objects[uindex];
		}
	}
	__finally
	{
		System::TMonitor::Exit(UserList);
	}
	return user;
}
//---------------------------------------------------------------------------
TLoginUserRecord * __fastcall TDMServerContainer::GetLoginUser(String UserID,String AccbookID)
{
	TLoginUserRecord *resultUser = NULL;
	System::TMonitor::Enter(UserList);
	try
	{
		for (int i=0; i < UserList->Count; i++)
		{
			TLoginUserRecord *loginUserRecord = (TLoginUserRecord*)UserList->Objects[i];
			if(loginUserRecord)
			{
				if((loginUserRecord->UserID==UserID)&&(loginUserRecord->AccBookID==AccbookID))
				{
					resultUser = loginUserRecord;
					break;
				}
			}
		}
	}
	__finally
	{
		System::TMonitor::Exit(UserList);
	}
	return resultUser;
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::ListOfLoginUser(TCustomClientDataSet *DataSet,bool bAll)
{
	TLoginUserRecord *loginUserRecord = NULL;
	TLoginUserRecord *curUserRecord = NULL;
  if(bAll==false)
  {
		curUserRecord = GetCurLoginUser();
		if(curUserRecord==NULL) return;
	}

  System::TMonitor::Enter(UserList);
	try
	{
		for (int i=0; i < UserList->Count; i++)
		{
			loginUserRecord = (TLoginUserRecord*)UserList->Objects[i];
			if(loginUserRecord)
			{
				if(bAll==false)
				{
					if(curUserRecord->AccBookID!=loginUserRecord->AccBookID)
					continue;
				}
				DataSet->Append();
				DataSet->FieldValues["ThreadSessionId"]  = loginUserRecord->ThreadSessionId;
				DataSet->FieldValues["CallBackClientID"] = loginUserRecord->CallBackClientID;
				DataSet->FieldValues["UserLoginCode"]    = loginUserRecord->UserLoginCode;
				DataSet->FieldValues["UserID"]           = loginUserRecord->UserID;
				DataSet->FieldValues["UserCode"]         = loginUserRecord->UserCode;
				DataSet->FieldValues["UserName"]         = loginUserRecord->UserName;
				DataSet->FieldValues["IP"]               = loginUserRecord->IP;
				DataSet->FieldValues["ClientPort"]       = loginUserRecord->ClientPort;
				DataSet->FieldValues["Protocol"]         = loginUserRecord->Protocol;
				DataSet->FieldValues["AccBookID"]        = loginUserRecord->AccBookID;
				DataSet->FieldValues["LoginTime"]        = loginUserRecord->LoginTime;
				DataSet->FieldValues["AccDbType"]        = loginUserRecord->AccDbType;
				DataSet->FieldValues["SysDbType"]        = loginUserRecord->SysDbType;
				DataSet->Post();
			}
		}
  }
  __finally
  {
		System::TMonitor::Exit(UserList);
  }
}
//---------------------------------------------------------------------------
TJSONArray* __fastcall TDMServerContainer::GetLoginUserList(bool bAll)
{
	TLoginUserRecord *loginUserRecord = NULL;
	TLoginUserRecord *curUserRecord = NULL;
	TJSONArray *result = new TJSONArray();
	if(bAll==false)
	{
		curUserRecord=GetCurLoginUser();
		if(curUserRecord==NULL) return result;
	}

	System::TMonitor::Enter(UserList);
	try
	{
		for (int i=0; i < UserList->Count; i++)
		{
			loginUserRecord = (TLoginUserRecord*)UserList->Objects[i];
			if(loginUserRecord)
			{
				if(bAll==false)
				{
					if(curUserRecord->AccBookID!=loginUserRecord->AccBookID)
					continue;
				}
				result->Add(loginUserRecord->ObjectToJSON());
			}
		}
	}
	__finally
	{
		System::TMonitor::Exit(UserList);
	}
	return result;
}
//---------------------------------------------------------------------------

void __fastcall	TDMServerContainer::NotifyUserChange(TLoginUserRecord *user, bool WasAdded)
{
	TUserNotifyThread *newUserNotifyThread = new TUserNotifyThread(FSDSServer, user, WasAdded);
	if(WasAdded)
	   NotifyWriteLog(user->UserLoginCode,user->IP,user->AccBookID,"0",4,"登录系统");
	else
	   NotifyWriteLog(user->UserLoginCode,user->IP,user->AccBookID,"0",5,"退出系统");
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::NotifyWriteLog(String UserID, String IPAddress, String AcckBook,String ClassCode, int type, String Desc)
{
  TWriteUserLog wLog=&WriteUserLog;
  String sDesc= Desc;
  if(sDesc.Length()>250) sDesc.SetLength(250);
  TLogQueueProc * LogQueueProc=new TLogQueueProc(wLog, UserID, IPAddress, AcckBook, ClassCode, type, sDesc);
}
void __fastcall TDMServerContainer::RemoveUser(int threadSessionId)
{
	//删除账套数据连接
 //	int threadSessionId = Session->Id;
	System::TMonitor::Enter(ConnectionList);
	try
	{
		int oindex = ConnectionList->IndexOf(threadSessionId);
		if(oindex>-1)
		{
			TADOConnection *acon = (TADOConnection*)ConnectionList->Objects[oindex];
			ConnectionList->Delete(oindex);
			delete acon;
		}
	}
	__finally
	{
		System::TMonitor::Exit(ConnectionList);
	}
		//删除系统数据连接
		System::TMonitor::Enter(SysConnectionList);
	try
	{
		int sindex = SysConnectionList->IndexOf(threadSessionId);
		if(sindex>-1)
		{
			TADOConnection *scon = (TADOConnection*)SysConnectionList->Objects[sindex];
			SysConnectionList->Delete(sindex);
			delete scon;
		}
	}
	__finally
	{
		System::TMonitor::Exit(SysConnectionList);
	}

	//删除用户
	System::TMonitor::Enter(UserList);
	try
	{
		int uindex = UserList->IndexOf(threadSessionId);
		if(uindex>-1)
		{
			TLoginUserRecord *user = (TLoginUserRecord*)UserList->Objects[uindex];
			NotifyUserChange(user,false);
			UserList->Delete(uindex);
			delete user;
		}
	}
	__finally
	{
		System::TMonitor::Exit(UserList);
	}
	//短信服务器
	System::TMonitor::Enter(SmsSerList);
	try
	{
		int oindex = SmsSerList->IndexOf(threadSessionId);
		if(oindex>-1)
		{
			TSmsSerRecord *smsr = (TSmsSerRecord*)SmsSerList->Objects[oindex];
			SmsSerList->Delete(oindex);
			delete smsr;
		}
	}
	__finally
	{
		System::TMonitor::Exit(SmsSerList);
	}
	// 工作流服务器
		System::TMonitor::Enter(WorkFlowSerList);
	try
	{
		int oindex = WorkFlowSerList->IndexOf(threadSessionId);
		if(oindex>-1)
		{
			TWorkFlowSerRecord *wfr = (TWorkFlowSerRecord*)WorkFlowSerList->Objects[oindex];
			WorkFlowSerList->Delete(oindex);
			delete wfr;
		}
	}
	__finally
	{
		System::TMonitor::Exit(WorkFlowSerList);
	}
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSServerDataSetGetClass(TDSServerClass *DSServerClass,
			TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TServerDataSet);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSServerQueryGetClass(TDSServerClass *DSServerClass,
					TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TServerQuery);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSJSeverDataSetGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
	 PersistentClass = __classid(TJServerDataSet);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSJServerQueryGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TJServerQuery);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSWorkFlowServerGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TWorkFlowServer);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSContractProjectGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TContractProject);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSMessageManageGetClass(TDSServerClass *DSServerClass,
		  TPersistentClass &PersistentClass)
{
	PersistentClass = __classid(TMessageManage);
}
//---------------------------------------------------------------------------
TJSONObject* __fastcall TDMServerContainer::GetAccListJSON()
{
	TJSONObject *elmObject=new TJSONObject();
	try
	{
		TJSONArray* AccList=new TJSONArray();
		SysADOQuery->Close();
		SysADOQuery->SQL->Text="select * from SyAccBook where SyAccBookStatue=1" ;
		SysADOQuery->Open();
		SysADOQuery->First();
		while (!SysADOQuery->Eof)
		{
			TJSONObject *rcdOject=new TJSONObject();
			rcdOject->AddPair(new TJSONPair("syaccbookcode",SysADOQuery->FieldByName("SyAccBookCode")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccbookname",SysADOQuery->FieldByName("SyAccBookName")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccdbtype",SysADOQuery->FieldByName("SyAccBookDBType")->AsString));
			rcdOject->AddPair(new TJSONPair("syaccbookconstring",DecryptAES(SysADOQuery->FieldByName("SyAccBookConString")->AsString)));
			AccList->AddElement(rcdOject);
			SysADOQuery->Next();
		}
		elmObject->AddPair(new TJSONPair("accbooks",AccList));
		return elmObject;
	}
	catch (Exception &e)
	{
		elmObject->AddPair(new TJSONPair("status","f"));
		elmObject->AddPair(new TJSONPair("errormessage",e.Message));
		LogMessage("代理向客户端发账套数据" + e.Message);
		return elmObject;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::CallbackTunnelEvent(TObject *Sender,const TDSCallbackTunnelEventItem &EventItem)
{
   switch(EventItem.EventType)
   {
	 case TunnelCreate:
		break;
	 case CallbackAdded:
		 if(EventItem.CallbackId==SMsgCallback)
		 {  //已经是不同线程会话了
//			TLoginUserRecord *user=GetCurLoginUser();
//			if(user)
//			  user->CallBackClientID=EventItem.TunnelId;
		 }
		 else if(EventItem.CallbackId==SWFCallback)
		   AddWorkflowSer(EventItem);
		  else if(EventItem.CallbackId==SSmsCallback)
			AddSMSSer(EventItem);
		 break;
	 case TunnelClose:
	 case CallbackRemoved:
		 if(EventItem.CallbackId==SWFCallback)
		   RemoveWorkflowSer(EventItem);
		  else if(EventItem.CallbackId==SSmsCallback)
			RemoveSMSSer(EventItem);
		 break;
   }
}
//工作流服务器
void __fastcall TDMServerContainer::AddWorkflowSer(const TDSCallbackTunnelEventItem &EventItem)
{
	System::TMonitor::Enter(WorkFlowSerList);
	try
	{
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		TWorkFlowSerRecord *wfr=new TWorkFlowSerRecord();
		wfr->ThreadSessionId=threadSession->Id;
		wfr->IpAdress= threadSession->GetData("IpAddress");
		wfr->CallBackClientID= EventItem.TunnelId;
		wfr->ClientPort=threadSession->GetData("ClientPort");
		wfr->Protocol= threadSession->GetData("Protocol");
		wfr->AppName= threadSession->GetData("AppName");
		WorkFlowSerList->AddObject(wfr->ThreadSessionId,(TObject*)wfr);
	}
	__finally
	{
		System::TMonitor::Exit(WorkFlowSerList);
	}

}
void __fastcall TDMServerContainer::RemoveWorkflowSer(const TDSCallbackTunnelEventItem &EventItem)
{
	System::TMonitor::Enter(WorkFlowSerList);
	try
	{   TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		int oindex = WorkFlowSerList->IndexOf(threadSession->Id);
		if(oindex>-1)
		{
			TWorkFlowSerRecord *wfr = (TWorkFlowSerRecord*)WorkFlowSerList->Objects[oindex];
			WorkFlowSerList->Delete(oindex);
			delete wfr;
		}
	}
	__finally
	{
		System::TMonitor::Exit(WorkFlowSerList);
	}
}
TJSONArray* TDMServerContainer::GetWorkFlowServerList()
{
	System::TMonitor::Enter(WorkFlowSerList);
	TJSONArray *result = new TJSONArray();
	try
	{
	  for(int i=0;i<WorkFlowSerList->Count;i++)
	  {
		  TWorkFlowSerRecord *wfr=(TWorkFlowSerRecord*)WorkFlowSerList->Objects[i];
		  if(wfr)
		  result->Add(wfr->ObjectToJSON()) ;
      }
	}
	__finally
	{
		System::TMonitor::Exit(WorkFlowSerList);
	}
	return result;
}
//短信服务器
void __fastcall TDMServerContainer::AddSMSSer(const TDSCallbackTunnelEventItem &EventItem)
{
	System::TMonitor::Enter(SmsSerList);
	try
	{
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		TSmsSerRecord *smsr=new TSmsSerRecord();
		smsr->ThreadSessionId=threadSession->Id;
		smsr->IpAdress= threadSession->GetData("IpAddress");
		smsr->CallBackClientID= EventItem.TunnelId;
		smsr->ClientPort=threadSession->GetData("ClientPort");
		smsr->Protocol= threadSession->GetData("Protocol");
		smsr->AppName= threadSession->GetData("AppName");
		SmsSerList->AddObject(smsr->ThreadSessionId,(TObject*)smsr);
	}
	__finally
	{
		System::TMonitor::Exit(SmsSerList);
	}

}
void __fastcall TDMServerContainer::RemoveSMSSer(const TDSCallbackTunnelEventItem &EventItem)
{
	System::TMonitor::Enter(SmsSerList);
	try
	{   TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		int oindex = SmsSerList->IndexOf(threadSession->Id);
		if(oindex>-1)
		{
			TSmsSerRecord *smsr = (TSmsSerRecord*)SmsSerList->Objects[oindex];
			SmsSerList->Delete(oindex);
			delete smsr;
		}
	}
	__finally
	{
		System::TMonitor::Exit(SmsSerList);
	}
}
TJSONArray*  TDMServerContainer::GetSmsServerList()
{
	System::TMonitor::Enter(SmsSerList);
	TJSONArray *result = new TJSONArray();
	try
	{
	  for(int i=0;i<SmsSerList->Count;i++)
	  {
		  TSmsSerRecord *smsr=(TSmsSerRecord*)SmsSerList->Objects[i];
		  if(smsr)
		  result->Add(smsr->ObjectToJSON()) ;
      }
	}
	__finally
	{
		System::TMonitor::Exit(SmsSerList);
	}
	return result;
}
void __fastcall TDMServerContainer::FSDSCertFilesGetPEMFilePasskey(TObject *ASender,
		  AnsiString &APasskey)
{
   APasskey="fstxzepm123";
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::TestClientConnection(String CallBackClientID,String AccBookID)
{
	TJSONValue *Resp= NULL;
	bool result = false;
	TJSONObject* JSONMsg = new  TJSONObject();
	JSONMsg->AddPair(new TJSONPair("notificationType", "test"));
	JSONMsg->AddPair(new TJSONPair("datetime", Now().FormatString("YYYY-mm-DD HH:mm:ss")));
	JSONMsg->AddPair(new TJSONPair("accbookid",AccBookID));
	JSONMsg->AddPair(new TJSONPair("message", "测试通道是否有效"));
	//Send the message to logged in user
	result = FSDSServer->NotifyCallback(CallBackClientID, SMsgCallback, JSONMsg, Resp,5000);
	if(Resp!=NULL)
		Resp->Free();
	return result;
}


void __fastcall TDMServerContainer::DSHouseViewGetClass(TDSServerClass *DSServerClass,
          TPersistentClass &PersistentClass)
{
		PersistentClass = __classid(THouseView);
}
//---------------------------------------------------------------------------

void __fastcall TDMServerContainer::DSSendUserMessageGetClass(TDSServerClass *DSServerClass,
          TPersistentClass &PersistentClass)
{
			PersistentClass = __classid(TSendUserMessage);
}
//---------------------------------------------------------------------------

