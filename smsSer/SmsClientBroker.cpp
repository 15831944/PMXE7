//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SmsClientBroker.h"
#include "SHConst.h"
#include "DSClientProxy.h"
#include "Version.h"
#include <Xml.Win.msxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include "DecEnc.h"
#include <DSHTTPLayer.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall TSmsClientBroker::TSmsClientBroker()
{
	fmtSettings=fmtSettings.Create();
	fmtSettings.DateSeparator   = '-';
	fmtSettings.TimeSeparator   = ':';
	fmtSettings.ShortDateFormat = "yyyy-MM-dd";
	fmtSettings.ShortTimeFormat = "HH:mm:ss";

	FSysDbType = 1;
	ClientBrokerStatus = 0; // -1-系统错误,0-尚未注册,1-尚未登录,2-系统准备好
	FcurrAcckIndex=0;
	FAppConnected = false;
	FAccBooks = new TCZDataSet;
	FPreAccBookID="";

	FSmsCordId="";
	FSmsUserName="",
	FSmsPassword="";
	FMonType=0;
	FPortNum=0;
	FSMSType=0;
	FBeginTime=7 ;//开始时间
	FEndTime=22; //结束时间
	FWaitLong=1;//消息处理间隔时间 秒为单位
	FRepeatCount=3;//发送错误时，需要重复次数
    ShortInfo=NULL;

	FAccBooks->AddField("accbookcode");
	FAccBooks->AddField("accbookname");
	FAccBooks->AddField("accbookconnstring");
	FAccBooks->KeyField = "accbookcode";

	SysConnection=new TADOConnection(NULL);
	SysConnection->LoginPrompt = false;

	SysQuery = new TADOQuery(NULL);
	SysQuery->Connection = SysConnection;

	AccConnection=new TADOConnection(NULL);
	AccConnection->LoginPrompt = false;
	m_Query=new TADOQuery(NULL);
	m_Query->Connection = AccConnection;

	//临界值
	SmsCriticalSection=new TRTLCriticalSection();
	InitializeCriticalSection(SmsCriticalSection);

	LoginUserClient       = NULL;
	DSConnection          = new TSQLConnection(NULL);
	LChannelManager       = NULL;
	SmsCallBack           = NULL;
}
// ---------------------------------------------------------------------------
TDBXConnection* __fastcall TSmsClientBroker::GetDBXConnection()
{
	return DSConnection->DBXConnection;
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::SetHookCallBack(THookSmsCallback Value)
{
	if(SmsCallBack)
		SmsCallBack->HookCallBack = Value;
}
// ---------------------------------------------------------------------------
THookSmsCallback __fastcall TSmsClientBroker::GetHookCallBack()
{
	if(SmsCallBack)
		return SmsCallBack->HookCallBack;
	else
		return NULL;
}
// ---------------------------------------------------------------------------
bool __fastcall TSmsClientBroker::InitClientComm()
{
   	LoadSetting();
	String NL = "\r\n";
	String ConnectionStr = "DriverName=Datasnap"+NL+
												 "DriverUnit=Data.DBXDataSnap"+NL+
												 "HostName="+FAgentAddress+NL+
												 "Port="+IntToStr(FAgentPort)+NL+
												 "CommunicationProtocol="+FProtocol+NL+
												 "DatasnapContext=datasnap/"+NL+
												 "DriverAssemblyLoader=Borland.Data.TDBXClientDriverLoader,Borland.Data.DbxClientDriver,Version=16.0.0.0,Culture=neutral,PublicKeyToken=91d62ebb5b0d1b1b"+NL+
												 "DSAuthenticationUser="+GetSysAdmin()+NL+
												 "DSAuthenticationPassword="+GetSysPassWord()+NL;

	DSConnection->DriverName      = "Datasnap";
	DSConnection->Params->Text    = ConnectionStr;
	DSConnection->LoginPrompt     = false;
	DSConnection->KeepConnection  = true;
	DSConnection->AfterConnect    = AppOnConnect;
	DSConnection->AfterDisconnect = AppOnDisConnect;
	DSConnection->Open();
	return DSConnection->Connected;
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::AppOnConnect(TObject * Sender)
{
	ClientBrokerStatus = 1;
	LoginUserClient = new TLoginUserClient(DSConnection->DBXConnection,false);
	GetAccInforJSON();
	FAppConnected = true;
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::AppOnDisConnect(TObject * Sender)
{
	FAppConnected   = false;

}
// ---------------------------------------------------------------------------
__fastcall TSmsClientBroker::~TSmsClientBroker()
{
	ClientCommTerminate(NULL);
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::ClientCommTerminate(TObject *Sender)
{
  try
  {
	UnRegCallBack();
	delete DSConnection;
	delete SysConnection;
	delete AccConnection;
	DeleteCriticalSection(SmsCriticalSection);
	delete SmsCriticalSection;
  }
  catch(Exception &e)
  {
	WritLog("服务结束时出错"+e.Message);
  }
}
void __fastcall TSmsClientBroker::GetAccInforJSON()
{
	TJSONObject *ms = LoginUserClient->GetAccInforJSON();
	GetAccInforJSONBack((TJSONObject*)ms->Clone());
}
// ---------------------------------------------------------------------------
// 接收账套数据
void __fastcall TSmsClientBroker::GetAccInforJSONBack(TJSONObject *fdata)
{
	String s;
	try
	{
		if(fdata->GetValue("status")->Value() == "f")
		{
//			s = fdata->GetValue("errormessage")->Value();
//			MessageBox(Application->Handle, s.c_str(), L"错误提示", MB_ICONWARNING);
//			Application->Terminate();
			return;
		}
//		FModuleList     = fdata->GetValue("modulelist")->Value();
//		FAdmin       = fdata->GetValue("fadmin")->Value();
//		FPassword        = fdata->GetValue("fpassword")->Value();

		FftpInteranetIP = fdata->GetValue("ftpInteranetIP")->Value();
		FftpInternetIP  = fdata->GetValue("ftpInternetIP")->Value();
		FftpUserName    = fdata->GetValue("ftpUserName")->Value();
		FftpPassword    = fdata->GetValue("ftpPassword")->Value();
		FftpPort        = fdata->GetValue("ftpPort")->Value().ToInt();
		FftpNeedFTP     = fdata->GetValue("ftpNeedFTP")->Value() == 1;
		FftpPassive     = fdata->GetValue("ftpPassive")->Value() == 1;
		FUpdateURL      = fdata->GetValue("updateurl")->Value();

		// mail
		FMailServer     = fdata->GetValue("mailServer")->Value();
		FSendEMail      = fdata->GetValue("sendEmail")->Value();
		FMailUserName   = fdata->GetValue("mailUserName")->Value();
		FMailPassword   = fdata->GetValue("mailPassword")->Value();
		FMailPort       = fdata->GetValue("mailPort")->Value().ToInt();
		FSysConnectiongString = fdata->GetValue("sysdbconstring")->Value();
		FAccConnectiongString = fdata->GetValue("accdbconstring")->Value();

		String sdbType = fdata->GetValue("sysdbtype")->Value();
		if(!sdbType.IsEmpty())
		{
			FSysDbType = sdbType.ToInt();
		}

		String adbType = fdata->GetValue("accdbtype")->Value();
		if(!adbType.IsEmpty())
		{
			FAccDbType = adbType.ToInt();
		}

//		TJSONArray * AccList = (TJSONArray *)(fdata->GetValue("accbooks"));
//		FAccBooks->Clear();
//		for (int i = 0; i < AccList->Size(); i++)
//		{
//			TJSONObject *recdObject = (TJSONObject*)(AccList->GetValue(i));
//			FAccBooks->AddNew();
//			FAccBooks->FieldValues["accbookcode"] = recdObject->GetValue("syaccbookcode")->Value();
//			FAccBooks->FieldValues["accbookname"] = recdObject->GetValue("syaccbookname")->Value();
//			FAccBooks->FieldValues["accbookconnstring"] = recdObject->GetValue("syaccbookconstring")->Value();
//			FAccBooks->Update();
//		}
//		FRecvAccBook = true;
		delete fdata;
	}
	catch (Exception &e)
	{
		WritLog(e.Message);
	}
}
bool __fastcall TSmsClientBroker::RegCallBack()
{
	bool regCallBack=false;
	if(DSConnection->Connected)
	{
		if(LChannelManager==NULL)
		{
			LChannelManager = new TDSClientCallbackChannelManager(NULL);
			LChannelManager->ChannelName = SSmsCallback;
			LChannelManager->DSHostname  = DSConnection->Params->Values["HostName"];
			LChannelManager->DSPort      = DSConnection->Params->Values["Port"];
			// LChannelManager->DSPath = DSConnection->Params->Values["URLPath"];
			LChannelManager->CommunicationProtocol = DSConnection->Params->Values["CommunicationProtocol"];
			//LChannelManager->OnServerConnectionTerminate = OnServerConnectionTerminate;
			//LChannelManager->OnServerConnectionError = OnServerConnectionError;
			//LChannelManager->OnChannelStateChange = OnChannelStateChange;
			LChannelManager->UserName = DSConnection->Params->Values["DSAuthenticationUser"];
			LChannelManager->Password = DSConnection->Params->Values["DSAuthenticationPassword"];
			SmsCallBack = new TSmsCallbackClient(LChannelManager,SSmsCallback);
			SmsCallBack->HookCallBack=SmsCallBackHook;
			ClientBrokerStatus=2;
			regCallBack=true;
		}
	}
	return regCallBack;
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::UnRegCallBack()
{
		ClientBrokerStatus = 0;
		if(LChannelManager)
		{
		   SmsCallBack->HookCallBack=NULL;
		   LChannelManager->CloseClientChannel();
			delete  LChannelManager;
			LChannelManager = NULL;
			SmsCallBack=NULL;
		}
		if(LoginUserClient)
			delete LoginUserClient;
		LoginUserClient = NULL;

		if(DSConnection)
		{
		  if(DSConnection->Connected)
			  DSConnection->Close();
		}
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::SmsCallBackHook(TJSONObject* Arg,TJSONObject* Ret) // 接收消息
{

   if(Arg->Count<=1) //数据有问题
   {
		Ret->AddPair("result","false");
		return;
   }
   String notyType=Arg->GetValue("notificationType")->Value();
   if(notyType!="sms") //非法命令
   {
	   Ret->AddPair("result","false");
	   return;
   }
   String msgType=Arg->GetValue("msgtype")->Value();
   Ret->AddPair(new TJSONPair("msgtype", msgType));  //返回运行类型
   if(msgType=="setting")  //参数
   {
	 Ret->AddPair("result","true");
	 GetSmsSetting(Ret);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::GetSmsSetting(TJSONObject* Ret)
{
	Ret->AddPair(new TJSONPair("SmsCordId",FSmsCordId));
	Ret->AddPair(new TJSONPair("SmsUserName",FSmsUserName));
	Ret->AddPair(new TJSONPair("SmsPassword",FSmsPassword));
	Ret->AddPair(new TJSONPair("MonType",FMonType));
	Ret->AddPair(new TJSONPair("PortNum",FPortNum));
	Ret->AddPair(new TJSONPair("SMSType",FSMSType));
	Ret->AddPair(new TJSONPair("BeginTime",FBeginTime.TimeString()));//开始时间
	Ret->AddPair(new TJSONPair("EndTime",FEndTime.TimeString())); //结束时间
	Ret->AddPair(new TJSONPair("WaitLong",FWaitLong));//消息处理间隔时间 秒为单位
	Ret->AddPair(new TJSONPair("RepeatCount",FRepeatCount));//发送错误时，需要重复次数
}
// ---------------------------------------------------------------------------
void  __fastcall TSmsClientBroker::WritLog(String ErrorLong)
{
   String Errorfile=ExtractFilePath(Forms::Application->ExeName)+FormatDateTime("YYYYMM",Now())+"SmsLong.txt";
   AnsiString  strLong = DateTimeToStr(Now())+"  "+ErrorLong+"\r\n";
   int iFileHandle;
   try
   {
       if (FileExists(Errorfile))
           iFileHandle = FileOpen(Errorfile,fmOpenWrite);
       else
           iFileHandle = FileCreate(Errorfile);
	   FileSeek(iFileHandle,0,2);
	  FileWrite(iFileHandle,strLong.c_str(),strLong.Length());
   }
   __finally
   {
	   FileClose(iFileHandle);
   }
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::LoadSetting()
{
  String settingfile=ExtractFilePath(Forms::Application->ExeName)+"smsdefault.xml";
  try
  {
	 CoInitialize(NULL);
   if(!FileExists(settingfile))
	 throw Exception("找不到系统配置文件"+settingfile);
	_di_IXMLDocument xdoc=NewXMLDocument();
	xdoc->LoadFromFile(settingfile);
	xdoc->Active=true;
	IXMLNodeList *NodeList=xdoc->ChildNodes;
	IXMLNode *selectNode=NULL;
	for(int i=0;i<NodeList->Count;i++)
	{
	   IXMLNode *curNode=NodeList->Nodes[i];
	   if(curNode->LocalName==WideString("DefaultSettings")&&(curNode->GetAttribute("Version")==String("2.0.0.2")))
		 {
			selectNode=curNode;
			break;
		 }
	}
	if(selectNode!=NULL)
	{
		IXMLNodeList *sNodeList=selectNode->ChildNodes;
		for(int j=0;j<sNodeList->Count;j++)
		{
			IXMLNode *sNode=sNodeList->Nodes[j];
			if(sNode->GetAttribute("Name")==String("AgentAddress"))
				FAgentAddress=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("AgentPort"))
				FAgentPort=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("Protocol"))
				FProtocol=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("SMSType"))
			   FSMSType=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("BeginTime"))
			   FBeginTime=StrToDateTime(sNode->GetAttribute("value"),fmtSettings);
			else if(sNode->GetAttribute("Name")==String("EndTime"))
			   FEndTime=StrToDateTime(sNode->GetAttribute("value"),fmtSettings);
			else if(sNode->GetAttribute("Name")==String("WaitLong"))
			   FWaitLong=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("RepeatCount"))
			   FRepeatCount=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("MonType"))
			   FMonType=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("PortNum"))
			   FPortNum=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("GateCordId"))
			   FSmsCordId=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("GateUserName"))
			   FSmsUserName=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("GatePassWord"))
			   FSmsPassword=DecryptAES(sNode->GetAttribute("value"));
		}
	}
	xdoc->Active=false;
		CoUninitialize();
 }
 catch(Exception &e)
 {
   WritLog("读取参数错误,"+e.Message);
 }
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::ServiceStart()
{
	if (FSMSType >0)
	 {
	 int Flag = FSMSType - 1;
	 ShortInfo = new TShortInfo(Flag);
     ShortInfo->BeginTime=FBeginTime;
     ShortInfo->EndTime=FEndTime;
     ShortInfo->WaitLong=FWaitLong*1000;
	 ShortInfo->RepeatCount=FRepeatCount;
	 ShortInfo->DBConnectString=FSysConnectiongString;
	 ShortInfo->AccConnectString=FAccConnectiongString;
	 ShortInfo->OnWriteErrorLog  = WritLog;
	 ShortInfo->ModeType = FMonType;
	 ShortInfo->ComPort = FPortNum;
	 ShortInfo->CordId = FSmsCordId;
	 ShortInfo->UserName = FSmsUserName;
	 ShortInfo->PassWord = FSmsPassword;
     ShortInfo->Initializtion();
	 ShortInfo->StarTimer();
     }
	 WritLog("短信服务器已运行,类型："+IntToStr(FSMSType));
}
// ---------------------------------------------------------------------------
void __fastcall TSmsClientBroker::ServiceStop()
{
	 try
	 {
      try
       {
		 if ((ShortInfo!=NULL)&&(FSMSType != 0))
			delete ShortInfo;
			ShortInfo=NULL;
			UnRegCallBack();
	   }
	   catch(Exception &e)
	   {
		WritLog(e.Message);
	   }
	  }
	  __finally
      {
	   WritLog("短信服务器已停止");
      }
}
// ---------------------------------------------------------------------------


