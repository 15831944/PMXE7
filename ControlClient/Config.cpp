//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include <System.Ioutils.hpp>
#include "Config.h"
#include <xml.xmldoc.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
	: TForm(Owner)
{
#if  defined(__ANDROID__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
	AppPath= System::Ioutils::TPath::GetDocumentsPath();
#else
	AppPath = ExtractFilePath(ParamStr(0));
#endif
	settingfile=System::Ioutils::TPath::Combine(AppPath, "csClientConfig.xml");
	ReadOption();
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::ReadOption()
{
	if(!FileExists(settingfile))
		throw Exception(L"找不到系统配置文件"+settingfile);
  //	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);
    XMLDocument1->LoadFromFile(settingfile);
	IXMLNodeList *NodeList=XMLDocument1->ChildNodes;
	int count=NodeList->Count;
	IXMLNode *selectNode=NULL;
	for(int i=0;i<count;i++)
	{
		IXMLNode *curNode=NodeList->Nodes[i];
		if(curNode->LocalName==String("DefaultSettings")&&(curNode->GetAttribute("Version")==String("2.0.0.2")))
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
void __fastcall TConfigForm::WriteOption()
{
	if(!FileExists(settingfile))
		throw Exception(L"找不到系统配置文件"+settingfile);
	_di_IXMLDocument XMLDocument1 = LoadXMLDocument(settingfile);
	IXMLNodeList *NodeList=XMLDocument1->ChildNodes;
	int count=NodeList->Count;
	IXMLNode *selectNode=NULL;
	for(int i=0;i<count;i++)
	{
		IXMLNode *curNode=NodeList->Nodes[i];
		if(curNode->LocalName==String("DefaultSettings")&&(curNode->GetAttribute("Version")==String("2.0.0.2")))
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
void __fastcall TConfigForm::btSaveClick(TObject *Sender)
{
	WriteOption();
	Close();
   //	ModalResult=System::Uitypes::TModalResult(mrOk);
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::btCancelClick(TObject *Sender)
{
	 Close();
}
//---------------------------------------------------------------------------

