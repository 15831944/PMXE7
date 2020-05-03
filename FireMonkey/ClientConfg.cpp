//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ClientConfg.h"
#include <xml.xmldoc.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmConfgForm *frmConfgForm;
//---------------------------------------------------------------------------
__fastcall TfrmConfgForm::TfrmConfgForm(TComponent* Owner)
  : TForm(Owner)
{
	Panel3->Enabled=false;
	ClientPath = ExtractFilePath(ParamStr(0));
	if ( !AnsiEndsStr("\\", ClientPath) &&
	   !AnsiEndsStr("/", ClientPath))
	  ClientPath = ClientPath + Sysutils::PathDelim;
	ReadOption();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfgForm::ReadOption()
{
	String settingfile=ClientPath+"ClientConfig.xml";
	if(!FileExists(settingfile))
		throw Exception("�Ҳ���ϵͳ�����ļ�"+settingfile);
	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);
//	XMLDocument1->FileName=settingfile;
//	XMLDocument1->Active=true;
	IXMLNodeList *NodeList=XMLDocument1->ChildNodes;
	int count=NodeList->Count;
	IXMLNode *selectNode=NULL;
	for(int i=0;i<count;i++)
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
				edAgentIP->Text=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("AgentPort"))
				edAgentPort->Text=sNode->GetAttribute("value");
			else if(sNode->GetAttribute("Name")==String("Protocol"))
			{
			  int index=cbProtocol->Items->IndexOf(sNode->GetAttribute("value"));
			  cbProtocol->ItemIndex=index;
			}

		}
	}
	XMLDocument1->Active=false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfgForm::WriteOption()
{
	String settingfile=ClientPath+"ClientConfig.xml";
	if(!FileExists(settingfile))
		throw Exception("�Ҳ���ϵͳ�����ļ�"+settingfile);
	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);
//	XMLDocument1->FileName=settingfile;
//	XMLDocument1->Active=true;
	IXMLNodeList *NodeList=XMLDocument1->ChildNodes;
	int count=NodeList->Count;
	IXMLNode *selectNode=NULL;
	for(int i=0;i<count;i++)
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
				sNode->Attributes["value"]= edAgentIP->Text;
			else if(sNode->GetAttribute("Name")==String("AgentPort"))
				sNode->Attributes["value"]=edAgentPort->Text;
			else if(sNode->GetAttribute("Name")==String("Protocol"))
				sNode->Attributes["value"]=cbProtocol->Items->Strings[cbProtocol->ItemIndex];
		}
		XMLDocument1->SaveToFile(settingfile);
	}
	XMLDocument1->Active=false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfgForm::bbEditClick(TObject *Sender)
{
	Panel3->Enabled=true;
	bbEdit->Enabled=false;
	bbCancel->Enabled=true;
	bbSave->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfgForm::bbSaveClick(TObject *Sender)
{
	Panel3->Enabled=false;
	bbEdit->Enabled=true;
	bbCancel->Enabled=false;
	bbSave->Enabled=false;
	WriteOption();
}
//---------------------------------------------------------------------------
void __fastcall TfrmConfgForm::bbCancelClick(TObject *Sender)
{
	Panel3->Enabled=false;
	bbEdit->Enabled=true;
	bbSave->Enabled=false;
	bbCancel->Enabled=false;
	ReadOption();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfgForm::lableportKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
  	if(Key!=13 && Key!=8 && (Key>'9' || Key<'0'))
		Key=0;
}
//---------------------------------------------------------------------------

