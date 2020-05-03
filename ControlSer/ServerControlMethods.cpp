//----------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.StrUtils.hpp>
#pragma hdrstop

#include "ServerControlMethods.h"
#include "ServerContainerModule.h"
#include "ServerCtl.h"
#include <registry.hpp>
//----------------------------------------------------------------------------
#pragma package(smart_init)
//----------------------------------------------------------------------------
TJSONArray* TServerControlMethods::GetServerList()
{
  TJSONArray *ServerList=ServerControlModule->CopyServerList();
  return ServerList;
}
//----------------------------------------------------------------------------
TJSONObject * TServerControlMethods::GetServerInfo(String ServerName)
{
  TJSONObject *retNode=NULL;
  int ServiceStatus=-1;
  TJSONArray *ServerList=ServerControlModule->CopyServerList();
  try
  {
  for(int i=0;i<ServerList->Count;i++)
  {
	TJSONObject *sNode=(TJSONObject *)(ServerList->Items[i]);
	String NtSer=sNode->GetValue("Name")->Value();
	if(NtSer.UpperCase()==ServerName.UpperCase())
	{
	   retNode=(TJSONObject *)(sNode->Clone());
	   break;
	}
  }
  }
  __finally
  {
	 delete ServerList;
  }
  return retNode;
}
//----------------------------------------------------------------------------
int  TServerControlMethods::GetServerStatus(String ServerName)
{
  int retval,ServiceStatus;
  retval=SCQueryServiceStatus(NULL,ServerName.c_str(),ServiceStatus);
  if(retval!=0)
	 ServiceStatus=-1;//
  return  ServiceStatus;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::StartServer(String ServerName)
{
  int bok=SCStartService(NULL,ServerName.c_str());
  ServerControlModule->UpdateServerList(ServerName);
  ServerControlModule->NotifyWriteLog(L"��������:"+ServerName);
  return bok==0;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::StopServer(String ServerName)
{
  int bok=SCStopService(NULL,ServerName.c_str());
  ServerControlModule->UpdateServerList(ServerName);
  ServerControlModule->NotifyWriteLog(L"ֹͣ����:"+ServerName);
  return bok==0;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::RegServer(String ServerName)
{
	 int retvalue=-1;
	  TJSONObject *sNode= GetServerInfo(ServerName);
	  if(sNode&&(sNode->GetValue("ServiceStatus")->Value()=="-1"))  //δ��װ
		{
			String DisplayName=sNode->GetValue("Display")->Value();
			String BinFileName= sNode->GetValue("FileName")->Value()+" -instance="+ServerName;
			retvalue=SCCreateService(NULL,ServerName.c_str(),DisplayName.c_str(),BinFileName.c_str());
			TRegistry *reg;
			reg = new TRegistry();
			reg->RootKey = HKEY_LOCAL_MACHINE;
			try
			{
				reg->OpenKey("SYSTEM",false);
				reg->OpenKey("CurrentControlSet",false);
				reg->OpenKey("Services",false);
				reg->OpenKey(ServerName,true);
				reg->WriteString("Description",DisplayName+"ʵ��Ϊ"+ServerName);
				reg->CloseKey();
			}
			__finally
			{
				reg->Free();
			}
			 ServerControlModule->UpdateServerList(ServerName);
		   ServerControlModule->NotifyWriteLog(L"ע�����:"+ServerName);
	  }
	 return retvalue==0;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::UnregServer(String ServerName)
{
	int retvalue=SCDeleteService(NULL,ServerName.c_str());
	ServerControlModule->UpdateServerList(ServerName);
	 ServerControlModule->NotifyWriteLog(L"ж�ط���:"+ServerName);
	return retvalue==0;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::RebootComputer()
{
   HANDLE hToken;
   TOKEN_PRIVILEGES tkp;

   // Get a token for this process.

   if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
      return( FALSE );

   // Get the LUID for the shutdown privilege.

   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

   tkp.PrivilegeCount = 1;  // one privilege to set
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Get the shutdown privilege for this process.

   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	CloseHandle(hToken);

   if (GetLastError() != ERROR_SUCCESS)
      return FALSE;

   // Reboot the system and force all applications to close.

   if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE,
               SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
               SHTDN_REASON_MINOR_UPGRADE |
               SHTDN_REASON_FLAG_PLANNED))
      return FALSE;
   ServerControlModule->NotifyWriteLog(L"��������");
   //Reboot was successful
   return TRUE;
}
//----------------------------------------------------------------------------
bool TServerControlMethods::ShutdownComputer()
{
   HANDLE hToken;
   TOKEN_PRIVILEGES tkp;

   // Get a token for this process.

   if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
      return( FALSE );

   // Get the LUID for the shutdown privilege.

   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

   tkp.PrivilegeCount = 1;  // one privilege to set
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

   // Get the shutdown privilege for this process.

   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

   CloseHandle(hToken);

   if (GetLastError() != ERROR_SUCCESS)
      return FALSE;

   // Shut down the system and force all applications to close.

   if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
               SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
               SHTDN_REASON_MINOR_UPGRADE |
               SHTDN_REASON_FLAG_PLANNED))
	  return FALSE;
   ServerControlModule->NotifyWriteLog(L"�رյ���");
   //shutdown was successful
   return TRUE;
}
//----------------------------------------------------------------------------

