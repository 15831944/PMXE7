//---------------------------------------------------------------------------
#include "DSSerMain.h"
#include "FSServerContainer.h"
#include <registry.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//��ʵ��ע�������� FSDSServer -install -instance=InstName;
//����һ��Ӧ�ó������ע�������������ƣ�ע����������Ŷ˿ڲ�Ҫ��ͻ
//һ����ñ��ݵ���ͬĿ¼���޸Ķ˿ڲ�����Ȼ��Ž��з���ע��
TFSService *FSService;

//---------------------------------------------------------------------------
__fastcall TFSService::TFSService(TComponent* Owner)
	: TService(Owner)
{
	FInstanceName="";
	DisplayName="Foresight XE/REST Server";
}
//---------------------------------------------------------------------------
TServiceController __fastcall TFSService::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}
//---------------------------------------------------------------------------
void __stdcall ServiceController(unsigned CtrlCode)
{
	FSService->Controller(CtrlCode);
}
//---------------------------------------------------------------------------
 bool __fastcall TFSService::DoContinue(void)
{
	bool doContinue = TService::DoContinue();
	//DMServerContainer->MaxOAUserCount=50;
   //	DMServerContainer->MaxUserCount=50;
	CoInitialize(NULL);
	DMServerContainer->StartServer();
	CoUninitialize();
	return doContinue;
}
//---------------------------------------------------------------------------
void __fastcall TFSService::DoInterrogate(void)
{
	TService::DoInterrogate();
}
//---------------------------------------------------------------------------
bool __fastcall TFSService::DoPause(void)
{
	DMServerContainer->Shutdown();
	return TService::DoPause();
}
//---------------------------------------------------------------------------
bool __fastcall TFSService::DoStop(void)
{
	DMServerContainer->Shutdown();
	return TService::DoStop();
}
//---------------------------------------------------------------------------
void __fastcall TFSService::ServiceStart(TService* Sender, bool &Started)
{
  CoInitialize(NULL);
  DMServerContainer->MaxOAUserCount=50;
  DMServerContainer->MaxUserCount=50;
  DMServerContainer->StartServer();
  Started=true;
  CoUninitialize();
     // DMServerContainer->FSDSProxyGenerator->Write();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TFSService::ServiceAfterInstall(TService *Sender)
{
  if(FInstanceName!="")
 		  ChangeServiceConfiguration();
	TRegistry *reg;
	reg = new TRegistry();
	reg->RootKey = HKEY_LOCAL_MACHINE;
	try
	{
		reg->OpenKey("SYSTEM",false);
		reg->OpenKey("CurrentControlSet",false);
		reg->OpenKey("Services",false);
		reg->OpenKey(Name,true);
		reg->WriteString("DisplayName",DisplayName);
		reg->WriteString("Description","����Ӧ�÷�����XE,ʵ��Ϊ"+Name);
		reg->CloseKey();
	}
	__finally
	{
		reg->Free();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFSService::ChangeServiceConfiguration()
{
  SC_HANDLE mngr;
  SC_HANDLE svc;
  String newpath;
  // �򿪷��������
  mngr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
  if (mngr == 0)
	RaiseLastOSError();
  try
  {
	// �򿪷���
	svc = OpenService(mngr, Name.c_str(), SERVICE_CHANGE_CONFIG);
	if (svc == 0)
	  RaiseLastOSError();
	try
	{
	  // �ı�����ִ�в���
	  newpath = ParamStr(0) + " " + INSTANCE_SWITCH+FInstanceName; // + any other cmd line params you fancy
	  ChangeServiceConfig(svc, SERVICE_NO_CHANGE, //  dwServiceType
                               SERVICE_NO_CHANGE, //  dwStartType
                               SERVICE_NO_CHANGE, //  dwErrorControl
							   newpath.c_str(),    //  <-- ֻҪ�ı�ִ��·���Ͳ���
							   NULL,               //  lpLoadOrderGroup
							   NULL,               //  lpdwTagId
							   NULL,               //  lpDependencies
							   NULL,               //  lpServiceStartName
							   NULL,               //  lpPassword
							   DisplayName.c_str());              //  lpDisplayName
	}
	__finally
	{
	  CloseServiceHandle(svc);
	}
  }
  __finally
  {
    CloseServiceHandle(mngr);
  }
}
void __fastcall TFSService::SetInstanceName(String Value)
{
   if (FInstanceName != Value)
  {
	FInstanceName = Value;
	if (FInstanceName != "")
	{
	  Name = FInstanceName;
	  DisplayName = "Foresight XE/REST Server("+FInstanceName+")";
	}
  }
}
void __fastcall TFSService::ServiceBeforeInstall(TService *Sender)
{
  String inst=GetInstanceName();
  if(inst>"")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------

void __fastcall TFSService::ServiceBeforeUninstall(TService *Sender)
{
  String inst=GetInstanceName();
  if(inst>"")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------
String __fastcall TFSService::GetInstanceName()
{
  String result = "";
  int PaCount=System::ParamCount();
  for(int index = 1; index<=PaCount; index++)
  {
	String paramString=ParamStr(index);
	if (SameText(INSTANCE_SWITCH, paramString.SubString(1,INSTANCE_SWITCH.Length())))
	{
	  result =paramString.SubString(INSTANCE_SWITCH.Length() + 1, MaxInt);
	  break;
	}
  }
 return result;
}

void __fastcall TFSService::ServiceCreate(TObject *Sender)
{
  String inst=GetInstanceName();
  if(inst>"")
	 InstanceName=inst;
}
//---------------------------------------------------------------------------

