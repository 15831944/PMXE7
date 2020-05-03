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
#define LASTTIMES 180  //3Сʱ//
#ifndef _BYDEV_ || _DEMO_
#include "rwdog.h"
#endif
/*  �������ɿͻ��˴�������ʱ����Ҫ�޸ĸù���
void __fastcall TLoginUserClient::Logout()
{
  if (FLogoutCommand == NULL)
  {
	FLogoutCommand = FDBXConnection->CreateCommand();
    FLogoutCommand->CommandType = TDBXCommandTypes_DSServerMethod;
	FLogoutCommand->Text = "TLoginUser.Logout";
	FLogoutCommand->CommandTimeout=3;   //��ֹ���ӶϿ�
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
	FPrjLevel       = 5; // ����ǳ�ʼ�������ݰ�ȫ�ļ���ˢ��
	FTCPIPPort     		= 211;
	FHttpPort       = 8080;
	FHttpsPort		= 8081;
	FVersion        = 8; // �ڲ��汾����

	m_DataSet = new TADOQuery(this);
	m_DataSet->ParamCheck = false;
	m_DataSet->Connection = SysADOConnection;

	// ����Ϊ�������ײ˵������ģ����Ӵ���ʱ�Ŵ���
	AccQuery = new TADOQuery(this);
	AccQuery->ParamCheck = false;

	LogQuery = new TADOQuery(this);
	LogQuery->ParamCheck = false;
	LogQuery->Connection = SysADOConnection;

	m_Query = new TADOQuery(this);
	m_Query->ParamCheck = false;
   /*Nt������·������*/
	 // AppPath = ExpandFileName(FSDSHTTPServiceFileDispatcher->RootDirectory);
	 //ParamStr(0)
	  AppPath = ExtractFilePath(Forms::Application->ExeName);
	  if (!DirectoryExists(AppPath))
		  ForceDirectories(AppPath);

	 if ( !AnsiEndsStr("\\", AppPath) && !AnsiEndsStr("/", AppPath))
	 	AppPath = AppPath + Sysutils::PathDelim;

	 ProxyPath =System::Ioutils::TPath::Combine(AppPath,"proxy")+ Sysutils::PathDelim;

	// �ȼ��һ�¹�
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
	//�Ի��¼�
	TDSSessionEventProc dse      = &aDSProc;
	if(dse)
	{
		_di_TDSSessionEvent methProc(static_cast<TDSSessionEvent*>(new TDSEventProc(dse)));
		TDSSessionManager::Instance->AddSessionEvent(methProc);
	}
	//callbackTunnel�¼�
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

   //������
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
		// ���ز���
		LoadSetting();
		LoadFtpSetting();
		if(FVersion >= 7)
			LoadMailSetting();

		AnsiString ps = "";
		CheckDog(); // ��⹷����ȡ��Ȩ��
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
		LogMessage("��ʼ�������������������ô���.");
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
			LogMessage("�Ҳ�����������ϵͳ���������ð�״̬");
		}
#endif
#endif
		// ���ð�
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
		LogMessage("��ȡ��ȫ���ݻ�����������");
		return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::InitConnData()
{
	try
	{
		// ������
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
			FlashSystemMenu(); // zws 20101116�������ã�����Ҫˢ��ϵͳ�⣬����Ҫˢ��ÿ�����׿�
			// �����ǻ�ȡϵͳ�˵�����¼���ò˵���¼
			m_DataSet->Close();
			m_DataSet->SQL->Text =
				"select count(*) as menucount from syModule where SyModuleEnable=1";
			m_DataSet->Open();
			if (m_DataSet->RecordCount > 0)
				SysMenuCount = m_DataSet->Fields->Fields[0]->AsInteger;
		}  */
#endif
		// ����������
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
		LogMessage("����ϵͳ���ݿ����");
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
			LogMessage("�Ҳ���ϵͳ�����ļ�" + settingfile);
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
				if(sNode->GetAttribute("Name") == String("DbConStr")) // ϵͳ�����Ӵ�
					FDBConnectString = DecryptAES(sNode->GetAttribute("value"));
				else if(sNode->GetAttribute("Name") == String("DbType"))
					FDbType = sNode->GetAttribute("value"); // ���ݿ�����
				if(sNode->GetAttribute("Name") == String("AccDbConStr")) // ���׿����Ӵ�
					FAccDBConnectString = DecryptAES(sNode->GetAttribute("value"));
				else if(sNode->GetAttribute("Name") == String("AccDbType"))
					FAccDbType = sNode->GetAttribute("value"); // ���ݿ�����
				else if(sNode->GetAttribute("Name") == String("TCPIP"))
					FTCPIPPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("HTTP"))
					FHttpPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("HTTPS"))
					FHttpsPort = sNode->GetAttribute("value"); //
				else if(sNode->GetAttribute("Name") == String("Admin"))
					FAdminName = sNode->GetAttribute("value"); // ����Ա
				else if(sNode->GetAttribute("Name") == String("PassWord"))
					SPassWord = sNode->GetAttribute("value"); // ����
				else if(sNode->GetAttribute("Name") == String("UniqueLog"))
					LogUnigue = sNode->GetAttribute("value"); // �Ƿ�����ͬһ�ʻ�Ψһ��¼

			}
		}
		xdoc->Active = false;
	}
	catch (Exception &e)
	{
		LogMessage("��ȡ��������," + e.Message);
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
			LogMessage("�Ҳ���ϵͳ�����ļ�" + settingfile);
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
		LogMessage("��ȡFtp��������," + e.Message);
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
			LogMessage("�Ҳ���ϵͳ�����ļ�" + settingfile);
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
		LogMessage("��ȡMail��������," + e.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDMServerContainer::FSDSServerConnect(TDSConnectEventObject *DSConnectEventObject)
{
	//���ӵ�¼��־
	//��������ͻ��˻�ȡ���� ChannelInfo
	if((DSConnectEventObject!=NULL)&&(DSConnectEventObject->ChannelInfo!=NULL))
	{
		//ͨ��ת�����Ի�ȡ������Ϣ
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
	//TLoginUser::UpdateCurUserCallbackClientID(String const Value)�д�����¼�¼�
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
	if((User== GetSysAdmin()) && (Password==GetSysPassWord()))      //�������ߣ������������ŷ�������,
	 {	valid=true;										 //���Ӳ�����Ҫ����
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
	   //if(Desc.Length()>300) //��Ҫ̫���ˣ�ռ�ռ�
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
	 //	LogMessage("��¼��־����:" + e.Message);
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
		MsgStream->ValueAttribute["softname"]   = "��������";
		MsgStream->ValueAttribute["version"]    = m_SysTag;
		MsgStream->ValueAttribute["devcorp"]    = "�������������Ƽ����޹�˾";
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
		LogMessage("������ͻ��˷���������" + e.Message);
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
		elmObject->AddPair(new TJSONPair("softname", "��������"));
		elmObject->AddPair(new TJSONPair("version", m_SysTag));
		elmObject->AddPair(new TJSONPair("devcorp", "�������������Ƽ����޹�˾"));
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
		LogMessage("������ͻ��˷���������" + e.Message);
		return elmObject;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TDMServerContainer::FlashSystemMenu()
{
	// ������
#ifdef _BYDEV_
	FPrjLevel = 10;
	IsUpdateMenu = true;
	return;
#endif
	// ˢ�²˵����ݿ�
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
		// �����¼
		InsertQuery->Close();
		InsertQuery->SQL->Text = "truncate table " + tablename;
		InsertQuery->ExecSQL();
		// �򿪿ձ�
		InsertQuery->Close();
		InsertQuery->SQL->Text = "select * from " + tablename;
		InsertQuery->Open();
		// ���������¼
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
		msg = "ϵͳû���ҵ����ĵ�¼����,�����Ա����Ǽǰ�";
	return IsValid;
}
//---------------------------------------------------------------------------
bool __fastcall TDMServerContainer::IsValidUser(String UserLoginCode,String strPassword,String &msg)
{
	//AccbookID==""Ϊ����������֤
	bool IsValid = false;
	msg="";
	int threadSessionId = TDSSessionManager::Instance->GetThreadSession()->Id;

	String UserName="",UserCode="",UserID="";
	String Password = EncryptAES(Trim(strPassword));
	TDateTime st, et, ct;
#ifndef _DEMO_   //���ð� IsTrial=true
#ifndef _BYDEV_  //������ IsTrial=true
		if(IsTrial) // ��ʽ�� �й�IsTrial=false,����IsTrial=true
		{
			msg = "������δ�ҵ�,�����ṩ����!";
			return IsValid;
		}

		/*			if (IsFromRemoter && !HaveRemoter) {
		msg="�ò�Ʒû�а���Զ��ģ�飬�빺��Զ��ģ��!";
		goto EXITDO;
		}  */

#endif
#endif

#ifdef _BYDEV_
		if(Date() > DevEndDate)
		{
			msg = "�ѹ�����������,�ܾ��ṩ����!";
			return IsValid;
		}
#endif

		if(TestOverTimes)
		{
			if(!IsTrial)
				msg = "�������ѹ��������޻����,�뼤��,�ܾ��ṩ����!";
			else
				msg = "�ѹ���������,��ʹ����ʽ��,�����������ϵ!";
			return IsValid;
		}

		if(FLicenseKey == -1)
		{
			msg = "��ȫ������Ȩ����Ϣ����,���뿪������ϵ,�ܾ��ṩ����!";
			return IsValid;
		}

		int maxUserCounts = FMaxUserCount;
		if(FMaxOAUserCount > maxUserCounts)
			maxUserCounts = FMaxOAUserCount;
		if(UserList->Count > maxUserCounts)
		{
			msg = "��¼�û�������,���Ժ����Ե�¼!";
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
				   //��������Ƿ���Ч
				   if(!cuser->CallBackClientID.IsEmpty())
				   {
						bFind=TestClientConnection(cuser->CallBackClientID,cuser->AccBookID);
						if(!bFind)//ɾ����Ч���Ӽ�¼
						{
							 RemoveUser(cuser->ThreadSessionId);
							 //UserList->Delete(i);
							 //delete cuser;
						}
					}
					break; //�뿪ѭ��
				}
			}

			if(bFind)
			{
				msg = "���û��ѵ�¼,�Ҳ������ٴε�¼,���Ժ����Ե�¼!";
				return IsValid;
			}
		}

		// ת�����״���
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->Connection = AccADOConnection;//GetConnection();
		// ͨ���û�������й������ɵ�¼����UserLogonCode���û�����UserIDCode������ҵ��û���ϢtbEmployeeData
		m_Query->SQL->Text = " select EmpPKID,EmpState,EmpName,EmpNum from tbEmployeeData"
												 " where upper(EmpNum)=(select Upper(UserIDCode) from tbUser where Upper(UserLogonCode)='"+UserLoginCode.UpperCase()+"')";
		m_Query->Open();
		if(m_Query->RecordCount < 1)
		{
			m_Query->Close();
			msg = "�û�δ�ҵ���δ�����¼��Ϣ,���ڹ���Աȷ�Ϻ����Ե�¼!";
			return IsValid;
		}
		else
		{
			UserName = m_Query->FieldByName("EmpName")->AsString;
			UserCode = m_Query->FieldByName("EmpNum")->AsString;
			UserID   = m_Query->FieldByName("EmpPKID")->AsString;
			// �Ƿ��ְ��
			if(m_Query->FieldByName("EmpState")->AsInteger == 2)
			{
				m_Query->Close();
				msg = "��Ա���Ѵ�ְ��������½,�������Աȷ�Ϻ����Ե�¼!";
				return IsValid;
			}
		}
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->SQL->Text = "select * from tbUser where Upper(UserLogonCode)='"+UserLoginCode.UpperCase() + "'";
		m_Query->Open();
		// δ�ҵ���¼��Ϣ
		if(m_Query->RecordCount < 1)
		{
			m_Query->Close();
			msg = "�û�δ�ҵ���δ�����¼��Ϣ,�������Աȷ�Ϻ����Ե�¼!";
			return IsValid;
		}

		// �����
		if(m_Query->FieldByName("UserPassword")->AsString != Password)
		{
			m_Query->Close();
			msg = "�û���¼���벻��ȷ,�������������Ե�¼!";
			return IsValid;
		}

		// �˻��Ƿ����
		if(StrToDate(m_Query->FieldByName("UserLimitDate")->AsString,fmtSettings) < Now())
		{
			m_Query->Close();
			msg = "��ǰ�û��ʺ��ѹ���,����ϵͳ����Ա��ϵ!";
			return IsValid;
		}

		// ������ַ����
		TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
		String UserIP = threadSession->GetData("IpAddress");
		{
			if(m_Query->FieldByName("UserSpecalLimit")->AsInteger == 1
			|| m_Query->FieldByName("UserSpecalLimit")->AsInteger == 3)
			{
				if(m_Query->FieldByName("UserLoginIP")->AsString!=UserIP)
				{
					m_Query->Close();
					msg = "�û�����ָ���Ļ�����¼!";
					return IsValid;
				}
			}
		}

		// ��¼ʱ������
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
				msg = "��¼ʱ�䲻�ǺϷ�ʱ��,����������!";
				return IsValid;
			}
			ct = Now();
			if(ct < st || ct > et)
			{
				m_Query->Close();
				msg = "�������ڵ�ǰʱ���¼!";
				return IsValid;
			}
		}
	IsValid = true;
	SetConnection(threadSessionId); //�����������ӣ��������ܽ��������û��ж�
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
			aConnection->Tag = FAccDbType;//�������ݿ�����
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
			aConnection->Tag = FaccDbType;//�������ݿ�����
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
			aConnection->Tag = FDbType;//�������ݿ�����
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
		//������Щ������û��ȡ��
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
	   NotifyWriteLog(user->UserLoginCode,user->IP,user->AccBookID,"0",4,"��¼ϵͳ");
	else
	   NotifyWriteLog(user->UserLoginCode,user->IP,user->AccBookID,"0",5,"�˳�ϵͳ");
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
	//ɾ��������������
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
		//ɾ��ϵͳ��������
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

	//ɾ���û�
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
	//���ŷ�����
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
	// ������������
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
		LogMessage("������ͻ��˷���������" + e.Message);
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
		 {  //�Ѿ��ǲ�ͬ�̻߳Ự��
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
//������������
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
//���ŷ�����
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
	JSONMsg->AddPair(new TJSONPair("message", "����ͨ���Ƿ���Ч"));
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
