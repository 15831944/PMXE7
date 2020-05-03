// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
//#include "About.h"
#include "Login.h"
#include "ClientConfg.h"
#include "DataSnapClientDataSet.h"
#include <System.IniFiles.hpp>
#include <xml.xmldoc.hpp>
#include <IPPeerClient.hpp>
#include <Datasnap.DSHTTPLayer.hpp>
#include <System.Ioutils.hpp>
#include "APIbase.h"
#include "ZClientQuery.h"
//#include "Test.h"
#ifdef __APPLE__
//#include "OpenURL.hpp"
#endif
#ifdef __WIN32__  || _WIN64_
#include <shellapi.h>
#endif
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FSTAbout"
#pragma resource "*.fmx"
TfrmMain *frmMain;
// ---------------------------------------------------------------------------

const bool UseTimeOut = false; // 提示维护期
const bool AutoStop = false; // 自动关闭
// ---------------------------------------------------------------------------
const String szGlobalClassName = L"GZFSTCLASS";
const String szCompanyURL = L"www.xzepm.com";
const String szKeyUserFmt = L"%s\\Users\\%s";
const String szModuleCaption = L"modulecaption";
const String szModuleCode = L"modulecode";
const String szModuleRight = L"moduleright";
const String szFormPtr = L"formid";
// ---------------------------------------------------------------------------
const String szNavMyStudio = L"我的工作室";
const String szNavFlowStudio = L"我的导航图";
const String szNavStudioSetup = L"工作室整理";
const String szNavHelpIndex = L"帮助索引";
const String szNavWebSite = L"先智软件主页";
const String szNavSwitchUser = L"切换用户";
const String szNavSave = L"保存修改";
const String szNavDiscard = L"放弃修改";
const String szNavAgentSetup = L"代理设置";
const String szNavOtherStuff = L"相关链接";
const String szNavModule = L"功能模块";
const String szMenuMainWindow = L"主窗体(&P)";
const String szMenuWindowList = L"窗体列表(&W)";
const String szMenuWindowMore = L"其他窗口(&M)...";
const String RegSysPath = L"SOFTWARE\\GZFST\\";
const String RegAttentionPath = L"SOFTWARE\\GZFST\\Attention";
const String RegPmQQPath = L"SOFTWARE\\GZFST\\InfoCenter\\";
// ---------------------------------------------------------------------------
const int ID_MAIN_WINDOW = 0x8086;
const int ID_WINDOW_FIRST = 0xE800;
const int MAX_WINDOW = 0x0100;
const int iAttentionCode = 1170110;
const int iPrjWorkCode = 1170112;
const int iPMQQ = 1240301;
const String szAttentionDll = "PMReminded.dylib";
const String szAttentionBpl = "PMReminded.bpl";
const String szCooperationDll = "PMCoordination.dylib";
const String szCooperationBpl = "PMCoordination.bpl";
// ---------------------------------------------------------------------------
#define FR_SYSTEMREAD "01"   //系统读权限
#define FR_SYSTEMWRITE "02"  //系统写权限

// ---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
	qqMain = NULL;
	FClientBroker = NULL;
	blogin=false;
	FAgentAddress="192.168.1.23";
	FAgentPort=211;
	FCommunicationProtocol="TCP/IP";
	MsgQuery=NULL;
	ChangeControlState();
/*
#if defined (__MSWINDOWS__)
#if defined (__MACOS__)
#if defined (__ANDROID__)
#if defined (__iOS__)
 */
		AppPath = ExtractFilePath(ParamStr(0));

//	 AppPath= System::Ioutils::TPath::GetDocumentsPath();

	SettingFile=System::Ioutils::TPath::Combine(AppPath, "ClientConfig.xml");

}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::LoadConnectSetting() {
	if (!FileExists(SettingFile))
		throw Exception(L"备置文件" + SettingFile + L"找不到");
	_di_IXMLDocument xdoc = LoadXMLDocument(SettingFile);
	IXMLNodeList *NodeList = xdoc->ChildNodes;
	IXMLNode *selectNode = NULL;
	for (int i = 0; i < NodeList->Count; i++) {
		IXMLNode *curNode = NodeList->Nodes[i];
		if (curNode->LocalName == WideString("DefaultSettings") &&
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
		}
	}
	xdoc->Active = false;
}
void __fastcall TfrmMain::miLogoutClick(TObject *Sender)
{
	if(miLogout->Enabled==false) return;
	if(qqMain) delete qqMain;
	qqMain = NULL;
	ModuleMenu->Clear();
	int i;
	while (true) {
		i = MainMenu->ItemsCount - 2;
		if (i > 0 && i < MainMenu->ItemsCount && MainMenu->Items[i]
			->TagString!="0")
		   delete	MainMenu->Items[i];
		else
			break;
	}
	if(FClientBroker)
	{
		FClientBroker->LogOut();
		delete FClientBroker;
		FClientBroker = NULL;
	}
	blogin=false;
	ChangeControlState();
}
// ---------------------------------------------------------------------------
void __fastcall TfrmMain::miAboutClick(TObject *Sender)
{
	ShowMessage(L"先智软件");
}
// ---------------------------------------------------------------------------
void __fastcall TfrmMain::miLoginClick(TObject *Sender)
{
	if(miLogin->Enabled==false) return;
   	LoadConnectSetting();
	if (FClientBroker!=NULL)
		delete FClientBroker;
	FClientBroker = new TClientBroker(this);
	FClientBroker->AgentAddress = FAgentAddress;  //这些都需要读参数文件
	FClientBroker->AgentPort = FAgentPort;
	FClientBroker->CommunicationProtocol=FCommunicationProtocol;
	bool bInit =FClientBroker->Connected; //FClientBroker->InitClientComm();
	if(!bInit)
	{
			TfrmLogin *pLogin = new TfrmLogin(this);
//			pLogin->Position=  TFormPosition::poOwnerFormCenter;
		   try{
			if (pLogin->ShowModal() == mrOk)
			{
				FClientBroker->LogonCode = pLogin->LogonCode;
				FClientBroker->Password = pLogin->Password;
				FClientBroker->AccBookID = "0";
				if(FClientBroker->InitClientComm())
				{
				  blogin = FClientBroker->RegMsgCallBack();
					if (blogin)
					{
					   miQQClick(NULL);
					   ChangeControlState();
					   LoadStudioPane();
				       CreateMsgDataSet();
					   return;
					}
				}
				else
				{
					if (FClientBroker!=NULL)
						delete FClientBroker;
					FClientBroker = NULL;
					ChangeControlState();
				}
			}
			}
			__finally
			{
			  delete  pLogin;
			}
	}

}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	miLogoutClick(NULL);
	Action=TCloseAction::caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmMain::ChangeControlState()
{

	miLogin->Enabled = !blogin;
	miLogout->Enabled = blogin;
	miQQ->Enabled = blogin;
//	miDataSet->Enabled = blogin;
	miSetting->Enabled = !blogin;
}
void __fastcall TfrmMain::miQQClick(TObject *Sender)
{
  if (blogin)
  {
	if(qqMain==NULL)
	 qqMain = new TfrmQQMain(this, FClientBroker);
	 qqMain->Left=this->Width-qqMain->Width;
	 qqMain->Show();
	 qqMain->BringToFront();
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::miExitClick(TObject *Sender)
{

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::miSettingClick(TObject *Sender)
{
	TfrmConfgForm* p=new TfrmConfgForm(this);
	try
	{
		p->ShowModal();
	}
	__finally
	{
		delete p;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::miForesightClick(TObject *Sender)
{
  // 打开先智软件公司网站
#ifdef __APPLE__
//   NSOpenURL(szCompanyURL);
#endif
#ifdef __WIN32__ || _WIN64_
::ShellExecute(NULL, L"open", szCompanyURL.c_str(), NULL, NULL, SW_SHOW);
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::MenuItemClick(TObject* Sender)
{
// 	assert(Sender->InheritsFrom(__classid(TMenuItem)));
	TMenuItem* p = static_cast<TMenuItem*>(Sender);

	// 使用ImageIndex属性存放对应的Form对象指针
	// 这样就不用遍历ClientForm的数据集来查找
	bool bfind=false;
	TCZDataSet* pdsForms = FClientBroker->ClientForms;
	pdsForms->First();
	while (!pdsForms->Eof)
	{
	  String sCode = pdsForms->FieldValues[szModuleCode];
	  if(p->TagString==sCode)
		{
		   TForm* pForm = (TForm*)(pdsForms->FieldValues[szFormPtr].ToInt());
		   pForm->BringToFront();
		   bfind=true;
		   break;
		}
		pdsForms->Next();
	}
	if(!bfind)
	{
		ShowClientForm(p->TagString.ToInt());
	}
}
void __fastcall TfrmMain::LoadStudioPane()
{
	TDataSnapClientDataSet* cdDataSet = new TDataSnapClientDataSet(NULL);
	FClientBroker->GetModuleData(cdDataSet);
	if (cdDataSet->RecordCount == 0) {
		delete cdDataSet;
		return;
	}

	// 最终下面这条语句得去掉
	// 因为从数据集取过来的模块列表应该是按显示顺序排好了序的
	// 目前保留，是因为数据集没有正确的排序
	cdDataSet->First();
	ModuleMenu->Clear();
	TMenuItem* pParent1 = NULL;
	TMenuItem* pParent2 = NULL;

	// 目前的实现基于以下事实
	// 顶层模块代码为3位编码
	// 第二层代码为5位编码
	// 第三层代码为7位编码
	bool CanFilter = true;
	TStringList* MenuList = new TStringList();
	TStringList* MenuNameList = new TStringList();
	MenuList->Clear();
	MenuNameList->Clear();

	String sRight = "";
	bool bHaveAddOne, bHaveAddTwo;
	int iItemID;
	while (!cdDataSet->Eof) {
		String sTitle = cdDataSet->FieldValues[szModuleCaption];
		String sCode = cdDataSet->FieldValues[szModuleCode];
		int level = cdDataSet->FieldValues["modulelevel"];
		if (CanFilter && level == 3) {
			sRight = cdDataSet->FieldValues[szModuleRight];
			if (sRight == "") {
				cdDataSet->Next();
				continue;
			}
		}
		if (sCode.Length() == 3) {
			TMenuItem* pItem = new TMenuItem(this);
			pItem->Text = sTitle;
			pItem->TagString = sCode;
			pParent1 = pItem;
			bHaveAddOne = false;
		}
		else if (sCode.Length() == 5) // 如果没有子节点, 则删除该节点
		{
			if (CanFilter && pParent2 != NULL) {
				if (pParent2->ItemsCount == 0) {
					delete pParent2;
					pParent2 = NULL;
				}
			}
			TMenuItem* pItem = new TMenuItem(this);
			pItem->Text = sTitle;
			pItem->TagString = sCode;
			pParent2 = pItem;
			bHaveAddTwo = false;
		}
		else if (sCode.Length() == 7) {
			// if(sCode.ToInt()==iAttentionCode)
			// bHasAttention=true;
			TMenuItem* pItem = new TMenuItem(this);
			pItem->Text = sTitle;
			pItem->TagString = sCode;
			pItem->OnClick = MenuItemClick;
			if (bHaveAddTwo) {
				pParent2->AddObject(pItem);
			}
			else {
				bool bAdd;
				if (pParent1 != NULL && pParent2 != NULL) {
					iItemID = MainMenu->ItemsCount - 1;
					if (!bHaveAddOne) {
						MainMenu->InsertObject(iItemID,pParent1);
						MenuList->Add(pParent1->TagString);
						MenuNameList->Add(String(pParent1->Text).SubString(1, 8));
						bHaveAddOne = true;
					}
					if (!bHaveAddTwo) {
						pParent1->AddObject(pParent2);
						bHaveAddTwo = true;
					}
					pParent2->AddObject(pItem);
				}
			}
		}
		cdDataSet->Next();
	}
	delete cdDataSet;
	//CreateLeftWeb(MenuList, MenuNameList);
}
void __fastcall TfrmMain::ShowClientForm(int id)
{
 ::ShowClientForm(abs(id), "");
}
void __fastcall TfrmMain::CreateLeftWeb(TStringList *MenuList,
	TStringList *MenuNameList) {
	TStringList *Tmp = new(TStringList);
	String ExeFilePath = ExtractFilePath(ParamStr(0));
	if ( !AnsiEndsStr("\\", ExeFilePath) && !AnsiEndsStr("/", ExeFilePath))
		ExeFilePath = ExeFilePath + Sysutils::PathDelim;

	try {
		String NewRightPath, RightURLStr;
		if (MenuList->Count > 0) {
			NewRightPath = ExeFilePath + "WebFile\\" + MenuList->Strings[0] +
				".html";
			RightURLStr = MenuList->Strings[0] + ".html";
		}
		else {
			NewRightPath = ExeFilePath + "WebFile\\Empty.html";
			RightURLStr = "Empty.html";
		}
		/*
		 String RightFilePath = ExeFilePath + "WebFile\\Right.html";
		 if(FileExists(NewRightPath))
		 {
		 Tmp->LoadFromFile(NewRightPath);
		 Tmp->SaveToFile(RightFilePath);
		 }
		 */
		String RightHtmlStr;
		RightHtmlStr = "<html>\r\n";
		RightHtmlStr += "<head>\r\n";
		RightHtmlStr +=
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\" />\r\n ";
		RightHtmlStr += "<title>导航图</title>\r\n";
		RightHtmlStr +=
			"<script src=\"js/top.js\" language=\"javascript\"></script>\r\n";
		RightHtmlStr +=
			"<script src=\"js/top-2.js\" language=\"javascript\"></script>\r\n";
		RightHtmlStr +=
			"<link href=\"css.css\" rel=\"stylesheet\" type=\"text/css\" />\r\n";
		RightHtmlStr += "</head>\r\n";
		RightHtmlStr +=
			"<body oncontextmenu=\"window.event.returnValue=false\" style=\"background-image:url(images/box_bg.jpg);width:100%;height:100%;background-position: left top;\" onload=\"javascript:a();\">\r\n";
		RightHtmlStr += "<div id=\"tagContent\">\r\n";
		RightHtmlStr += "<iframe name=\"main\" id=\"main\" src=\"" +
			RightURLStr +
			"\" width=\"98%\" height=\"98%\" frameborder=\"0\" scrolling=\"no\" allowtransparency=true></iframe>\r\n";
		RightHtmlStr += "</div>\r\n";
		RightHtmlStr += "</body>\r\n";
		RightHtmlStr += "</html>\r\n";

		String HtmlFilePath;
		Tmp->Clear();
		Tmp->Text = RightHtmlStr;
		HtmlFilePath = ExeFilePath + "WebFile\\Right.html";
		Tmp->SaveToFile(HtmlFilePath);
	}
	catch (...) {
	}
	Tmp->Clear();
	try {
		String HtmlStr = "<html>\r\n";
		HtmlStr += "<head>\r\n";
		HtmlStr +=
			"<meta   http-equiv='Content-Type'   content='text/html;   charset=gb2312'>\r\n";
		HtmlStr += "<title>系统菜单</title>\r\n";
		HtmlStr +=
			"<link href='css.css' rel='stylesheet' type='text/css' />\r\n";
		HtmlStr +=
			"<script src='js/nav.js' language='javascript'></script>\r\n";
		HtmlStr +=
			"<body oncontextmenu=\"window.event.returnValue=false\" style=\"background-image:url(images/box_bg.jpg); background:#4583bc;width:100%;\">";
		HtmlStr += "<div id=\"left\">";
		HtmlStr += "<ul><li class=\"nav\">系统模块列表</li></ul>";
		HtmlStr += "<ul id=tags style=\"margin-top:10px;\">";
		if (MenuList->Count > 0) {
			for (int i = 0; i < MenuList->Count; i++) {
				String TheStr = MenuNameList->Strings[i];
				int lefPos = TheStr.Pos("(");
				if (lefPos > 1)
					TheStr = TheStr.SubString(1, lefPos - 1);
				if (i == 0)
					HtmlStr = HtmlStr + "<li class=\"selectTag\">";
				else
					HtmlStr = HtmlStr + "<li>";
				HtmlStr = HtmlStr + " <A onClick=\"selectTag('tagContent" +
					IntToStr(i) + "',this)\" href=\"" +
					String(MenuList->Strings[i]) + ".html\" target=\"main\">" +
					TheStr + "</a></li>\r\n";
			}
		}
		HtmlStr += "</ul>";
		HtmlStr += "</div>";
		HtmlStr += "<SCRIPT type=text/javascript>";
		HtmlStr += "function selectTag(showContent,selfObj){";
		// 操作标签
		HtmlStr +=
			"var tag = document.getElementById(\"tags\").getElementsByTagName(\"li\");";
		HtmlStr += "var taglength = tag.length;";
		HtmlStr += "for(i=0; i<taglength; i++){";
		HtmlStr += "tag[i].className = \"\";";
		HtmlStr += "}";
		HtmlStr += "selfObj.parentNode.className = \"selectTag\";";
		// 操作内容
		HtmlStr +=
			"for(i=0; j=document.getElementById(\"tagContent\"+i); i++){";
		HtmlStr += "j.style.display = \"none\";";
		HtmlStr += "}";
		HtmlStr += "}";
		HtmlStr += "</SCRIPT>";
		HtmlStr += "</body>";
		HtmlStr += "</html>";

		String HtmlFilePath;
		Tmp->Clear();
		Tmp->Text = HtmlStr;
		HtmlFilePath = ExeFilePath + "WebFile\\left.html";
		Tmp->SaveToFile(HtmlFilePath);
	}
	__finally {
		if (Tmp)
			delete Tmp;
	}
}

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
 //miLoginClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::CreateMsgDataSet()
{
	if (FClientBroker && FClientBroker->Connected) {
		if (MsgQuery)
			delete MsgQuery;

		MsgQuery = new TZClientQuery(this,FClientBroker);
		// MsgDataSet->Open();
		// 把用户列表查询出来，然后保存到ClientComm->UserDataSet中；
		MsgQuery->SQL->Text =
			" select UserLogonCode as LoginCode,EmpNum as UserCode,EmpName as UserName,EmpPKID as UserID from tbEmployeeData " " left join tbUser on Upper(UserIDCode)=Upper(EmpNum) order by EmpNum";

		MsgQuery->Open();
	   //	int rCount=MsgQuery->RecordCount;   //测试
		FClientBroker->InitUserDataSet(MsgQuery); // 在系统中会保存该用户列表
		MsgQuery->Close();
//		if (bHavePMQQ && MsgQuery) {
//			if (fmMessage)
//				delete fmMessage;
//			fmMessage = new TfmMessage(this, MsgQuery, ClientBroker->UserCode,
//				ClientBroker);
//		}
	}
}
void __fastcall TfrmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
if (MessageDlg(L"您确定要退出项目管理系统吗？",TMsgDlgType::mtConfirmation,mbOKCancel,0) == mrOk) {
		CanClose = true;
		miLogoutClick(NULL);
	}
	else {
		CanClose = false;
	}
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::miTestClick(TObject *Sender)
{
// TfrmTest* test=new TfrmTest(this,FClientBroker);
// test->ShowModal();
// delete test;
}
//---------------------------------------------------------------------------



