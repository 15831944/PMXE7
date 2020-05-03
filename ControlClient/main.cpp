//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include <IPPeerClient.hpp>
//#include <Datasnap.DSHTTPLayer.hpp>
#include <System.Ioutils.hpp>
#include <xml.xmldoc.hpp>
#include "loginShowMethod.h"
#include "FSTMessageDlg.h"
#include "Config.h"
#include "Login.h"
#include "SHConst.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	Timer1->Enabled=false;
	FAgentAddress="192.168.1.23";
	FAgentPort=4040;
	FCommunicationProtocol="HTTP";
	FRemember=false;
	FAutoLogin=false;
	LChannelManager=NULL;
	scControlClient=NULL;
	SerList=NULL;
	ToControlState=cssNone;
	ChangeControlState();
#if  defined(__ANDROID__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
	AppPath= System::Ioutils::TPath::GetDocumentsPath();
#else
	AppPath = ExtractFilePath(ParamStr(0));
#endif
#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
	 btExit->Visible=false;
#else
	btExit->Visible=true;
#endif
	settingfile=System::Ioutils::TPath::Combine(AppPath, "csClientConfig.xml");
	LoadConnectSetting(); //�����Ӳ���
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoadConnectSetting()
{

	if (!FileExists(settingfile))
		throw Exception(L"�����ļ�" + settingfile + L"�Ҳ���");
	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);
	IXMLNodeList *NodeList = XMLDocument1->ChildNodes;
	IXMLNode *selectNode = NULL;
	for (int i = 0; i < NodeList->Count; i++) {
		IXMLNode *curNode = NodeList->Nodes[i];
		if (curNode->LocalName == String("DefaultSettings") &&
			(curNode->GetAttribute("Version") == String("2.0.0.2"))) {
			selectNode = curNode;
			break;
		}
	}
	if (selectNode != NULL) {
		IXMLNodeList *sNodeList = selectNode->ChildNodes;
		for (int j = 0; j < sNodeList->Count; j++) {
			IXMLNode *sNode = sNodeList->Nodes[j];
			if (sNode->GetAttribute("Name") == String("AgentAddress"))
				FAgentAddress = sNode->GetAttribute("value");
			else if (sNode->GetAttribute("Name") == String("AgentPort"))
				FAgentPort = sNode->GetAttribute("value");
			else if (sNode->GetAttribute("Name") == String("Protocol"))
				FCommunicationProtocol = sNode->GetAttribute("value");
			else if (sNode->GetAttribute("Name") == String("UserCode"))
				FUserCode = sNode->GetAttribute("value");
			else if (sNode->GetAttribute("Name") == String("Password"))
				FPassword = sNode->GetAttribute("value");
			else if (sNode->GetAttribute("Name") == String("Remember"))
				FRemember = StrToBool(sNode->GetAttribute("value"));
			else if (sNode->GetAttribute("Name") == String("AutoLogin"))
				FAutoLogin = StrToBool(sNode->GetAttribute("value"));
		}
	}
	XMLDocument1->Active = false;
}
void __fastcall TfrmMain::btSetupClick(TObject *Sender)
{
	 TConfigForm * cf=new TConfigForm(this);
	 cf->Show();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btLoginClick(TObject *Sender)
{
	if(!FConnection->Connected)
	{
		 if(!FAutoLogin)
		 {
 		     LoadConnectSetting(); //�����Ӳ���
			 TLoginForm * pLogin=new TLoginForm(this);
			 pLogin->LogonCode=FUserCode;
			 pLogin->Password=FPassword;
			 pLogin->Remember=FRemember;
			 pLogin->AutoLogin=FAutoLogin;
			 TLoginMethodReference  *Lm=new TLoginMethodReference;
			 Lm->dlg=pLogin;
			 Lm->OwerForm=this;
			 pLogin->ShowModal(Lm);
		 }
		 else
			InitClientComm();
	}
	else
	{
	   TMessageDialogEventProc msgDialogCloseProc=&LogOut;
	   ShowMessageBox(_D("��ȷ��Ҫע����"),msgDialogCloseProc);

	}
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMain::InitClientComm()
{
	if(FConnection->Connected)
	{
	  LogOut(System::Uitypes::TModalResult(mrYes));
	}
	String NL = "\r\n";
	String ConnectionStr = "DriverName=Datasnap"+NL+												 "DriverUnit=Data.DBXDataSnap"+NL+												 "HostName="+FAgentAddress+NL+												 "Port="+IntToStr(FAgentPort)+NL+												 "CommunicationProtocol="+FCommunicationProtocol+NL+												 "DatasnapContext=datasnap/"+NL+												 "DriverAssemblyLoader=Borland.Data.TDBXClientDriverLoader,Borland.Data.DbxClientDriver,Version=16.0.0.0,Culture=neutral,PublicKeyToken=91d62ebb5b0d1b1b"+NL+												 "DSAuthenticationUser="+FUserCode+NL+												 "DSAuthenticationPassword="+FPassword+NL;	FConnection->DriverName      = "Datasnap";	FConnection->Params->Text    = ConnectionStr;	FConnection->LoginPrompt     = false;	FConnection->KeepConnection  = true;	FConnection->AfterConnect    = AppOnConnect;  	FConnection->AfterDisconnect = AppOnDisConnect;	try{	FConnection->Open();	}	catch(Exception &e)	{		FAutoLogin=false;
		throw Exception(L"���ӷ�����ʧ�ܣ�"+e.Message);
    }	return FConnection->Connected;
}
void __fastcall TfrmMain::SaveSetting()
{
	if (!FileExists(settingfile))
		throw Exception(L"�����ļ�" + settingfile + L"�Ҳ���");
   //	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);

	XMLDocument1->LoadFromFile(settingfile);
	IXMLNodeList *NodeList = XMLDocument1->ChildNodes;
	IXMLNode *selectNode = NULL;
	for (int i = 0; i < NodeList->Count; i++) {
		IXMLNode *curNode = NodeList->Nodes[i];
		if (curNode->LocalName == String("DefaultSettings") &&
			(curNode->GetAttribute("Version") == String("2.0.0.2"))) {
			selectNode = curNode;
			break;
		}
	}
	if (selectNode != NULL) {
		IXMLNodeList *sNodeList = selectNode->ChildNodes;
		for (int j = 0; j < sNodeList->Count; j++) {
			IXMLNode *sNode = sNodeList->Nodes[j];
			if (sNode->GetAttribute("Name") == String("UserCode"))
			{
				 if(FRemember)
				   sNode->Attributes["value"]=FUserCode;
				 else
				   sNode->Attributes["value"]="";
			}
			else if (sNode->GetAttribute("Name") == String("Password"))
			{
				if(FRemember)
				  sNode->Attributes["value"]=FPassword;
				else
                  sNode->Attributes["value"]="";
			}
			else if (sNode->GetAttribute("Name") == String("Remember"))
			{
				 sNode->Attributes["value"]=FRemember;
			}
			else if (sNode->GetAttribute("Name") == String("AutoLogin"))
			{
				 sNode->Attributes["value"]=FAutoLogin;
			}
		}
		XMLDocument1->SaveToFile(settingfile);
	}
	XMLDocument1->Active = false;
}
void __fastcall TfrmMain::LogOut(System::Uitypes::TModalResult AResult)
{
	 if(AResult == mrYes)
	 {
		if(LChannelManager)
		{
			MsgCallBack->HookCallBack=NULL;
			LChannelManager->CloseClientChannel();
			MsgCallBack=NULL;
			delete  LChannelManager;
			LChannelManager = NULL;

		}
		if(scControlClient)
		  {
			delete scControlClient;
			scControlClient=NULL;
          }
		if(FConnection->Connected)
		{
		   FConnection->Close();
		   ChangeControlState();
		}
		FAutoLogin=false;
	 }
}
void __fastcall TfrmMain::AppOnConnect(TObject * Sender)
{
  ChangeControlState();
   RegMsgCallBack();
   if(scControlClient==NULL)
	 scControlClient=new TServerControlMethodsClient(FConnection->DBXConnection,false);
}
void __fastcall TfrmMain::AppOnDisConnect(TObject * Sender)
{
   LogOut(System::Uitypes::TModalResult(mrYes));
}
void __fastcall TfrmMain::RegMsgCallBack()
{
		if(LChannelManager==NULL)
		{
			LChannelManager = new TDSClientCallbackChannelManager(NULL);
			LChannelManager->ChannelName = SMessageChannel;
			LChannelManager->DSHostname  = FConnection->Params->Values["HostName"];
			LChannelManager->DSPort      = FConnection->Params->Values["Port"];
			LChannelManager->CommunicationProtocol = FConnection->Params->Values["CommunicationProtocol"];
			LChannelManager->UserName = FConnection->Params->Values["DSAuthenticationUser"];
			LChannelManager->Password = FConnection->Params->Values["DSAuthenticationPassword"];
			MsgCallBack = new TCallbackClient(LChannelManager,SMsgCallback);
			MsgCallBack->HookCallBack=MessageCallBackHook;//&MessageCallBackHook;
		}
}
void __fastcall TfrmMain::MessageCallBackHook(TJSONObject* Arg) // ������Ϣ
{
	String pc= L"����:"+Arg->GetValue("computerip")->Value()+L"ʱ��:"+Arg->GetValue("datetime")->Value();
	String msg= Arg->GetValue("servername")->Value()+Arg->GetValue("msg")->Value();
	mmMsg->Lines->Add(pc);
	mmMsg->Lines->Add(msg);
	//����֪ͨ
	if(NotificationC->Supported())
	{
		TNotification * Notification = NotificationC->CreateNotification();

		Notification->Name = "FSTNotification";
		Notification->AlertBody = msg;
		Notification->FireDate = Now();

		// Send notification in Notification Center
		NotificationC->ScheduleNotification(Notification);
		// also this method is equivalent
		// Free the notification
		Notification->DisposeOf();
	 }
	FillServerList();
}
void __fastcall TfrmMain::btExitClick(TObject *Sender)
{
//   if(FConnection->Connected)
//	  LogOut();
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ChangeControlState()
{
   	btSetup->Enabled =!FConnection->Connected;
	acReboot->Enabled =FConnection->Connected;
	btFill->Enabled =FConnection->Connected;
	TabServer->Enabled = FConnection->Connected;
	if(!FConnection->Connected)
		tcControl->ActiveTab=TabServerMsg;
	if(FConnection->Connected)
	{
	   UserLabel->Text= L"������";
	   btLogin->Text=L"ע��";
	}
	else
	{
		btLogin->Text=L"��¼";
		UserLabel->Text= L"δ��¼";
	}
}
void __fastcall TfrmMain::FillServerList()
{
   lvServerList->Items->Clear();
   if(SerList)
	 delete SerList;
   TJSONArray *ListObject=scControlClient->GetServerList();
   SerList=( TJSONArray *)(ListObject->Clone());
   for(int i=0;i<SerList->Count;i++)
   {
	   TJSONObject *jNode=(TJSONObject*)SerList->Items[i];
	   TListViewItem* vItem=lvServerList->Items->Add();
	   ChangeListItem(vItem,jNode);
   }
   if(lvServerList->Items->Count>0)
   {
	 lvServerList->ItemIndex=0;
	 lvServerListItemClick(NULL,lvServerList->Items->Item[0]);
   }
   else
   {
			acSCReg->Enabled=false;
			acSCUnReg->Enabled=false;
			acSCStart->Enabled=false;
			acSCStop->Enabled=false;
   }
}
void __fastcall TfrmMain::btFillClick(TObject *Sender)
{
  FillServerList();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::ChangeActionList(int serverstatus)
{
  switch(serverstatus)
   {
		case SERVICE_STOPPED:
		case SERVICE_STOP_PENDING:
		case SERVICE_PAUSE_PENDING:
		case SERVICE_PAUSED:
			acSCReg->Enabled=false;
			acSCUnReg->Enabled=true;
			acSCStart->Enabled=true;
			acSCStop->Enabled=false;
		break;
		case SERVICE_RUNNING:
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_START_PENDING:
			acSCReg->Enabled=false;
			acSCUnReg->Enabled=true;
			acSCStart->Enabled=false;
			acSCStop->Enabled=true;
		break;
		default:
			acSCReg->Enabled=true;
			acSCUnReg->Enabled=false;
			acSCStart->Enabled=false;
			acSCStop->Enabled=false;
	}
}
void __fastcall TfrmMain::ChangeListItem(TListViewItem *sItem,TJSONObject *jNode)
{
   int ServiceStatus=jNode->GetValue("ServiceStatus")->Value().ToInt();
   String status=L"δ��װ" ;
   switch(ServiceStatus)
   {
		case SERVICE_STOPPED:
		status=L"��ֹͣ";
		break;
		case SERVICE_START_PENDING:
		status=L"��������";
		break;
		case SERVICE_STOP_PENDING:
		status=L"����ֹͣ";
		break;
		case SERVICE_RUNNING:
		status=L"������";
		break;
		case SERVICE_CONTINUE_PENDING:
		status=L"���ڼ���";
		break;
		case SERVICE_PAUSE_PENDING:
		status=L"������ͣ";
		break;
		case SERVICE_PAUSED:
		status=L"��ͣ";
		break;
		default:
		 status=L"δ��װ";
	}
   sItem->Text=jNode->GetValue("Name")->Value()+"("+status+")"+jNode->GetValue("Display")->Value();
   sItem->Detail=jNode->GetValue("FileName")->Value();
}
void __fastcall TfrmMain::lvServerListItemClick(const TObject *Sender, const TListViewItem *AItem)

{
  TListViewItem *sItem=const_cast<TListViewItem *>(AItem);
  TJSONObject *jNode=(TJSONObject *)(SerList->Items[sItem->Index]);
  int serverstatus=jNode->GetValue("ServiceStatus")->Value().ToInt();
  ChangeActionList(serverstatus);

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ControlServer(System::Uitypes::TModalResult AResult)
{
if (AResult == mrYes)
{
  switch(ToControlState)
  {
	  case cssNone:
		   break;
	  case cssReg:
		  scControlClient->RegServer(CurServerName);
		  break;
	  case cssStart:
		scControlClient->StartServer(CurServerName);
			break;
	  case cssStop:
		scControlClient->StopServer(CurServerName);
		  break;
	  case cssUnReg:
	   scControlClient->UnregServer(CurServerName);
	   break;
	   case cssReboot:
		 scControlClient->RebootComputer();
		 break;
  }
}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::acSCRegExecute(TObject *Sender)
{
  TListViewItem *sItem=lvServerList->Selected;
  if(sItem)
  {
	TJSONObject *jNode=(TJSONObject *)(SerList->Items[sItem->Index]);
	CurServerName=jNode->GetValue("Name")->Value();
	ToControlState=cssReg;
	TMessageDialogEventProc msgDialogCloseProc=&ControlServer;
	ShowMessageBox(_D("��ȷ��Ҫע��"+CurServerName+"������"),msgDialogCloseProc);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acSCUnRegExecute(TObject *Sender)
{
  TListViewItem *sItem=lvServerList->Selected;
  if(sItem)
  {
	  TJSONObject *jNode=(TJSONObject *)(SerList->Items[sItem->Index]);
	  CurServerName=jNode->GetValue("Name")->Value();
	  ToControlState=cssUnReg;
	  TMessageDialogEventProc msgDialogCloseProc=&ControlServer;
	  ShowMessageBox(L"��ȷ��Ҫж��"+CurServerName+L"������" ,msgDialogCloseProc);
 }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acSCStartExecute(TObject *Sender)
{
  TListViewItem *sItem=lvServerList->Selected;
    if(sItem)
  {
	TJSONObject *jNode=(TJSONObject *)(SerList->Items[sItem->Index]);
	CurServerName=jNode->GetValue("Name")->Value();
	ToControlState=cssStart;
	TMessageDialogEventProc msgDialogCloseProc=&ControlServer;
	ShowMessageBox(L"��ȷ��Ҫ����"+CurServerName+L"������" ,msgDialogCloseProc);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acSCStopExecute(TObject *Sender)
{
  TListViewItem *sItem=lvServerList->Selected;
    if(sItem)
  {
   TJSONObject *jNode=(TJSONObject *)(SerList->Items[sItem->Index]);
   CurServerName=jNode->GetValue("Name")->Value();
   ToControlState=cssStop;
	TMessageDialogEventProc msgDialogCloseProc=&ControlServer;
	ShowMessageBox(L"��ȷ��Ҫ�ر�"+CurServerName+L"������" ,msgDialogCloseProc);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::acRebootExecute(TObject *Sender)
{
	ToControlState=cssReboot;
	TMessageDialogEventProc msgDialogCloseProc=&ControlServer;
	ShowMessageBox(L"��ȷ��Ҫ����Զ�̻����� \r\n��Ӱ��û����ϵ����з���������û�" ,msgDialogCloseProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tcControlChange(TObject *Sender)
{
 if(tcControl->ActiveTab==TabServer)
	FillServerList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   if(FConnection->Connected)
	 Action= TCloseAction::caHide;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled=false;
	btLoginClick(NULL);
}
//---------------------------------------------------------------------------


