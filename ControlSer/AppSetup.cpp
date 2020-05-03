//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AppSetup.h"
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include "BaseFunc.h"
#include "DecEnc.h"
#include "ServerCtl.h"
#include "EditServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAppSetup *frmAppSetup;
//---------------------------------------------------------------------------
__fastcall TfrmAppSetup::TfrmAppSetup(TComponent* Owner)
	: TForm(Owner)
{
	ReadOptions();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::ReadOptions()
{
	String settingfile=ExtractFilePath(Application->ExeName)+"scDefault.xml";
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
		  if(sNode->GetAttribute("Name")==String("ServerName"))
		  {
				edServerName->Text = sNode->GetAttribute("value");
				ChangeSCAction();
		  }
		  else if(sNode->GetAttribute("Name")==String("HTTP"))
				edHTTP->Text=sNode->GetAttribute("value");
		  else if(sNode->GetAttribute("Name")==String("TCPIP"))
				edTCPIP->Text=sNode->GetAttribute("value");
		  else if(sNode->GetAttribute("Name")==String("Admin"))
				edUser->Text=sNode->GetAttribute("value");
		  else if(sNode->GetAttribute("Name")==String("PassWord"))
				edPassword->Text=sNode->GetAttribute("value");
		  else if(sNode->GetAttribute("Name")==String("Period"))
				edPeriod->Text=sNode->GetAttribute("value");
		}
	}
	  IXMLNode *ServerListNode =selectNode->ChildNodes->FindNode("ServerList");
	  if(ServerListNode)
	  {
		int retval;
		lvServerList->Clear();
		IXMLNodeList *SerChildList = ServerListNode->ChildNodes;
		for(int s=0;s<SerChildList->Count;s++)
		{
		   IXMLNode *sNode = SerChildList->Nodes[s];
		   String NtSer= sNode->GetAttribute("Name");
		   TListItem *item=lvServerList->Items->Add();
		   item->Caption= NtSer;
		   item->SubItems->Add(sNode->GetAttribute("FileName"));
		   item->SubItems->Add(sNode->GetAttribute("Display"));
		   String status=ServiceStatus(NtSer);
		   item->SubItems->Add(status);
		}

	  }
	xdoc->Active=false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::WriteOptions()
{
  String settingfile=ExtractFilePath(Application->ExeName)+"scDefault.xml";
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
		selectNode->SetAttribute("FirstTime",false);
		IXMLNodeList *sNodeList=selectNode->ChildNodes;
		for(int j=0;j<sNodeList->Count;j++)
		{
			IXMLNode *sNode=sNodeList->Nodes[j];
			if(sNode->GetAttribute("Name")==String("ServerName"))
				 sNode->Attributes["value"]=edServerName->Text;
			else if(sNode->GetAttribute("Name")==String("HTTP"))
				sNode->Attributes["value"]=edHTTP->Text;
			else if(sNode->GetAttribute("Name")==String("TCPIP"))
				sNode->Attributes["value"]=edTCPIP->Text;
			else if(sNode->GetAttribute("Name")==String("Admin"))
				sNode->Attributes["value"]=edUser->Text;
			else if(sNode->GetAttribute("Name")==String("PassWord"))
				sNode->Attributes["value"]=edPassword->Text;
			else if(sNode->GetAttribute("Name")==String("Period"))
				sNode->Attributes["value"]=edPeriod->Text;
		}
	  IXMLNode *ServerListNode =selectNode->ChildNodes->FindNode("ServerList");
	  if(ServerListNode)
	  {
		ServerListNode->ChildNodes->Clear();
		for(int i=0;i<lvServerList->Items->Count;i++)
		{
		   TListItem *item=lvServerList->Items->Item[i];
		   IXMLNode *sNode=ServerListNode->AddChild("Server",i);
		   sNode->Attributes["Name"]=item->Caption;
		   sNode->Attributes["FileName"]=item->SubItems->Strings[0];
		   sNode->Attributes["Display"]=item->SubItems->Strings[1];
        }
      }
		xdoc->SaveToFile(settingfile);
	}
	xdoc->Active=false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::btCancelClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::btSaveClick(TObject *Sender)
{
	WriteOptions();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::edHTTPKeyPress(TObject *Sender, char &Key)
{
	if(Key!=13 && Key!=8 && (Key>'9' || Key<'0'))
		Key=0;
}
//---------------------------------------------------------------------------
String __fastcall TfrmAppSetup::ServiceStatus(String ServerName)
{
		   int ServiceStatus;
		   int retval=SCQueryServiceStatus(NULL,ServerName.c_str(),ServiceStatus);
		   String status="状态未知";
		   switch(retval)
		   {
			 case 1:
			  status="无权限";
			 break;
			 case 2:
			  status="未安装" ;
			 break;
			 case 3:
			  status="执行查询出错";
			 break;
			 case 0:
			 {
			  switch(ServiceStatus)
			  {
					case SERVICE_STOPPED:
					status="已停止";
					break;
					case SERVICE_START_PENDING:
					status="正在启动";
					break;
					case SERVICE_STOP_PENDING:
					status="正在停止";
					break;
					case SERVICE_RUNNING:
					status="运行中";
					break;
					case SERVICE_CONTINUE_PENDING:
					status="正在继续";
					break;
					case SERVICE_PAUSE_PENDING:
					status="正在暂停";
					break;
					case SERVICE_PAUSED:
					status="暂停";
					break;
					default:
					 status="状态未知";
			  }

			 }
			 break;
		   }
	 return  status;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::ChangeItemStatus(TListItem *AItem)
{
   String serStatus= ServiceStatus(AItem->Caption);
   AItem->SubItems->Strings[2]=serStatus;
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::acSerStartExecute(TObject *Sender)
{
  TListItem *item=lvServerList->Selected;
  if(item)
  {
	  SCStartService(NULL,item->Caption.c_str());
	  ChangeItemStatus(item);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSerStopExecute(TObject *Sender)
{
    TListItem *item=lvServerList->Selected;
  if(item)
  {
	  SCStopService(NULL,item->Caption.c_str());
	  ChangeItemStatus(item);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSerInstallExecute(TObject *Sender)
{
	TListItem *item=lvServerList->Selected;
  if(item)
  {
	  SCCreateService(NULL,item->Caption.c_str(),item->SubItems->Strings[1].c_str(),item->SubItems->Strings[0].c_str());
	  ChangeItemStatus(item);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSerUnInstExecute(TObject *Sender)
{
   TListItem *item=lvServerList->Selected;
  if(item)
  {
	  SCDeleteService(NULL,item->Caption.c_str());
	  ChangeItemStatus(item);
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acAddExecute(TObject *Sender)
{
  TListItem *item=lvServerList->Items->Add();
  item->SubItems->Add("");
  item->SubItems->Add("");
  item->SubItems->Add("");
//  int itemindex=item->Index;
  TfrmEditServer * es=new TfrmEditServer(this,item,1);
  try{
	  if(es->ShowModal()==mrOk)
	  {
		ChangeItemStatus(item);
	  }
	  else
	  delete item;
	//  lvServerList->Items->Delete(itemindex);
  }
  __finally
  {
	  delete es;
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acEditExecute(TObject *Sender)
{
  TListItem *item=lvServerList->Selected;
  if(item)
  {
   TfrmEditServer * es=new TfrmEditServer(this,item,2);
   try{
	  if(es->ShowModal())
		ChangeItemStatus(item);
	  }
	  __finally
	  {
		  delete es;
	  }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acDeleteExecute(TObject *Sender)
{
  if(lvServerList->Selected)
  {
	 lvServerList->Selected->Delete();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::lvServerListSelectItem(TObject *Sender, TListItem *Item,
          bool Selected)
{
		   String ServerName=Item->Caption;
		   int ServiceStatus;
		   int retval=SCQueryServiceStatus(NULL,ServerName.c_str(),ServiceStatus);
		   switch(retval)
		   {
			 case 1:
			 case 3:
				acSerStart->Enabled=false;
				acSerStop->Enabled=false;
				acSerInstall->Enabled=false;
				acSerUnInst->Enabled=false;
			 break;
			 case 2:
				acSerStart->Enabled=false;
				acSerStop->Enabled=false;
				acSerInstall->Enabled=true;
				acSerUnInst->Enabled=false;
			 break;
			 case 0:
			 {
			  switch(ServiceStatus)
			  {
					case SERVICE_STOPPED:
					case SERVICE_STOP_PENDING:
					case SERVICE_PAUSE_PENDING:
					case SERVICE_PAUSED:
						acSerStart->Enabled=true;
						acSerStop->Enabled=false;
						acSerInstall->Enabled=false;
						acSerUnInst->Enabled=true;
					break;
					case SERVICE_RUNNING:
					case SERVICE_START_PENDING:
					case SERVICE_CONTINUE_PENDING:
						acSerStart->Enabled=false;
						acSerStop->Enabled=true;
						acSerInstall->Enabled=false;
						acSerUnInst->Enabled=true;
					break;
					default:
						acSerStart->Enabled=false;
						acSerStop->Enabled=false;
						acSerInstall->Enabled=false;
						acSerUnInst->Enabled=true;
			  }

			 }
			 break;
		   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSCRegExecute(TObject *Sender)
{
 String SerFileName=ExtractFilePath(Application->ExeName)+"FSControlSer.exe";
 SCCreateService(NULL,edServerName->Text.c_str(),L"先智服务器监控服务",SerFileName.c_str());
  ChangeSCAction();
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSCUnRegExecute(TObject *Sender)
{
  SCDeleteService(NULL,edServerName->Text.c_str());
   ChangeSCAction();
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSCStartExecute(TObject *Sender)
{
 SCStartService(NULL,edServerName->Text.c_str());
  ChangeSCAction();
}
//---------------------------------------------------------------------------

void __fastcall TfrmAppSetup::acSCStopExecute(TObject *Sender)
{
 SCStopService(NULL,edServerName->Text.c_str());
 ChangeSCAction();
}
//---------------------------------------------------------------------------
void __fastcall TfrmAppSetup::ChangeSCAction()
{
		   int ServiceStatus;
		   Sleep(2000);
		   int retval=SCQueryServiceStatus(NULL,edServerName->Text.c_str(),ServiceStatus);
		   String status="状态未知";
		   switch(retval)
		   {
			 case 1:
			 case 3:
				 status="无权限";
				acSCReg->Enabled=false;
				acSCUnReg->Enabled=false;
				acSCStart->Enabled=false;
				acSCStop->Enabled=false;
			 break;
			 case 2:
				status="未安装" ;
				acSCReg->Enabled=true;
				acSCUnReg->Enabled=false;
				acSCStart->Enabled=false;
				acSCStop->Enabled=false;
			 break;
			 case 0:
			 {
			  switch(ServiceStatus)
			  {
					case SERVICE_STOPPED:
					case SERVICE_STOP_PENDING:
					case SERVICE_PAUSE_PENDING:
					case SERVICE_PAUSED:
						 status="停止";
						acSCReg->Enabled=false;
						acSCUnReg->Enabled=true;
						acSCStart->Enabled=true;
						acSCStop->Enabled=false;
					break;
					case SERVICE_RUNNING:
					case SERVICE_START_PENDING:
					case SERVICE_CONTINUE_PENDING:
						status="运行中";
						acSCReg->Enabled=false;
						acSCUnReg->Enabled=true;
						acSCStart->Enabled=false;
						acSCStop->Enabled=true;
					break;
					default:
						acSCReg->Enabled=false;
						acSCUnReg->Enabled=true;
						acSCStart->Enabled=false;
						acSCStop->Enabled=false;
			  }

			 }
			 break;
		   }
		StatusBar1->Panels->Items[0]->Text= "服务器名："+edServerName->Text+"("+status+")";
}
void __fastcall TfrmAppSetup::acRefreshExecute(TObject *Sender)
{
   ReadOptions();
}
//---------------------------------------------------------------------------

