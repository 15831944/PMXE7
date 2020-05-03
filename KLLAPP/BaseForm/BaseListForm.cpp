//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BaseListForm.h"
#include "BaseBillForm.h"
#include "FSTMessageDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BaseForm"
#pragma resource "*.fmx"
TfrmBaseListForm *frmBaseListForm;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner)
          :TfrmBaseForm(Owner)
{
  InitBaseListForm();
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner, TClientBroker * clBroker,
						int ModuleCode, String Param)
          :TfrmBaseForm(Owner, clBroker, ModuleCode, Param)
{
  InitBaseListForm();
  InitDevLocalize();
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::TfrmBaseListForm(TComponent * Owner,TClientBroker * clBroker,
							 int ModuleCode, TZClientDataSet * FDataSet, String Param)
		  :TfrmBaseForm(Owner, clBroker,ModuleCode,FDataSet, Param)
{
  InitBaseListForm();
  InitDevLocalize();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::InitBaseListForm()
{
  RelaFormSet=new TCZDataSet();
  RelaFormSet->AddField("Caption");
  RelaFormSet->AddField("ModuleCode");
  RelaFormSet->AddField("ImageIndex");
  RelaFormSet->AddField("CreateStyle");

  //设置窗体初始状态
  if(DataSet != NULL)
  {
    SetOptCtrlStatus(0);
  }
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseListForm::~TfrmBaseListForm()
{
  delete RelaFormSet;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowEditForm(int status)
{
  //非新增状态，浏览或修改操作时
  if(status != 1)
  {
	if(RecordCount > 0)
    {
      if(Eof)
        DS_MoveLast();
	  else if(Bof)
		DS_MoveFirst();
    }
    else
	{
	  ShowMessage(_D("当前没有数据，无法执行操作"));
      return;
    }
  }
  //创建并显示数据编辑窗体
	  TfrmBaseBillForm *EditForm;
	  try
	  {
		TForm *p = NewEditForm(DataSet, status);  //获取编辑窗体指针
		if(p)
		{
			EditForm =dynamic_cast<TfrmBaseBillForm *>(p); //类型转换
			if(EditForm)
			{   //屏蔽下面两行，主要对权限数据获取太慢，已经在构造函数进行处理
				//传入moduleCode，一次性解决权限和窗体ID取值
			   //	EditForm->FormModuleCode = FormModuleCode;
			   //	EditForm->FormID=this->FormID;
				EditForm->SetFormStatus(status); //设置编辑窗体初始状态
				EditForm->Show();
			}
		}
		else
		 ShowMessage(L"未实现编辑窗体!");
	  }
	 catch(Exception &e)
	  {
	   throw Exception(e.Message);
      }

}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::BrowseRecord()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otBrowse) == true)
    {
      ShowEditForm(0);
    }
  }
  else
  ShowMessage(_D("无可查看数据"));
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::AddData()
{
  if(BeforeUpdateData((int)otAdd) == true)
  {
     ShowEditForm(1);
  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::EditData()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otEdit) == true)
    {
      ShowEditForm(2);
    }
  }
  else
	 ShowMessage(_D("无可编辑数据"));
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::DelData()
{
  if(RecordCount > 0)
  {
    if(BeforeUpdateData((int)otDel) == true)
	{
	  TMessageDialogEventProc msgDialogCloseProc=&DoDelData;
	  ShowMessageBox(_D("是否要删除当前数据") ,msgDialogCloseProc);
 	}
  }
  else
	ShowMessage(_D("无可删除数据"));
}
void __fastcall TfrmBaseListForm::DoDelData(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
		 DS_Delete();
		 if(DataSet != NULL)
		 {
			SetOptCtrlStatus(0);
			FormFillData();
		 }
  }
}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmBaseListForm::NewEditForm(TZClientDataSet * m_DataSet, int status)
{
  return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::DoCheckBillProc(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
		DS_Edit();
		if(BeforeCheck())
		{
		  CheckBill();
		  AfterCheck();
		  SetOptCtrlStatus(0);
		}
	else
	  DS_Cancel();
  }
}
void __fastcall TfrmBaseListForm::DoUnCheckBillProc(System::Uitypes::TModalResult AResult)
{
  if(AResult==mrYes)
  {
      DS_Edit();
	  if(BeforeUnCheck())
	  {
		UnCheckBill();
		AfterUnCheck();
		SetOptCtrlStatus(0);
	  }
	  else
		DS_Cancel();

  }
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::CheckBillData()
{
  TMessageDialogEventProc msgDialogCloseProc=&DoCheckBillProc;
  ShowMessageBox(_D("是否要审核当前数据？"),msgDialogCloseProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::CheckBill()
{
  DS_Check(1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::UnCheckBillData()
{
  TMessageDialogEventProc msgDialogCloseProc=&DoUnCheckBillProc;
  ShowMessageBox(_D("是否要反审核当前数据？"),msgDialogCloseProc);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::UnCheckBill()
{
  DS_Check(0);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::RefreshDataSet()
{
	TDataSetNotifyEvent m_AfterScroll = DataSet->MasterDataSet->AfterScroll;
	try
	{
		DataSet->MasterDataSet->DisableControls();
		DataSet->MasterDataSet->AfterScroll = NULL;
		DS_Close();
		DS_Open();
		FormFillData();
		if(RecordCount > 0) DS_MoveFirst();
		SetOptCtrlStatus(0);
	}
	__finally
	{
		DataSet->MasterDataSet->AfterScroll = m_AfterScroll;
		DataSet->MasterDataSet->EnableControls();

		if(m_AfterScroll) //触发一次好点
			m_AfterScroll(DataSet->MasterDataSet);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::RefreshData(String Filters)
{
	MasterWhereString=Filters;
	RefreshDataSet();
}
//---------------------------------------------------------------------------
bool __fastcall  TfrmBaseListForm::LocateRecord(String KeyValues)
{
  bool retval;
  retval = DS_LocateMaster(KeyValues);
  SetOptCtrlStatus(0);
  return retval;
}
TForm * __fastcall TfrmBaseListForm::NewFilterForm(TOnFilteredDataSet AOnFilteredDataSet,TClientBroker * ABroker)
{
   return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::QueryData()
{
	  TOnFilteredDataSet  FileredDataSet=&RefreshData;
	  TForm* qp=NewFilterForm(FileredDataSet,ClientBroker);
	  if(qp)   //自定义查询界面
		qp->Show();
	  else   //没有过滤功能
        RefreshDataSet();

}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetDetailValue(String fieldname)
{
  return DS_GetDetailValue(fieldname);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetDetailOldValue(String fieldname)
{
  return DS_GetDetailOldValue(fieldname);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::SetDetailValue(String fieldname, Variant value)
{
  DS_SetDetailValue(fieldname, value);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetFieldValue(String fieldname)
{
  return DS_GetMasterValue(fieldname);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetFieldOldValue(String fieldname)
{
  return DS_GetMasterOldValue(fieldname);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::SetFieldValue(String fieldname, Variant value)
{
  DS_SetMasterValue(fieldname, value);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetMasterValue(String Mastername)
{
  return DS_GetMasterValue(Mastername);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseListForm::GetMasterOldValue(String Mastername)
{
  return DS_GetMasterOldValue(Mastername);
}
//---------------------------------------------------------------------------
void __fastcall  TfrmBaseListForm::SetMasterValue(String Mastername, Variant value)
{
  DS_SetMasterValue(Mastername, value);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::AddNavItem(String sCaption,int iCode,
                                             int iStyle,int iImageIndex)
{
  RelaFormSet->AddNew();
  RelaFormSet->FieldValues["Caption"]=sCaption;
  RelaFormSet->FieldValues["ModuleCode"]=IntToStr(iCode);
  RelaFormSet->FieldValues["ImageIndex"]=IntToStr(iImageIndex);
  RelaFormSet->FieldValues["CreateStyle"]=IntToStr(iStyle);
  RelaFormSet->Update();
}
//---------------------------------------------------------------------------
//显示相关业务窗体
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowRelationForm(int ModuleCode)
{
  String ParamStr = GetRelaFormParam(ModuleCode);   //获取过滤条件
//  ShowClientFormModal(ClientCommHandle,ModuleCode,ParamStr); //自动打开相关业务窗体
  AfterRelaFormModal(ModuleCode);                       //关闭相关业务窗体后的处理
}
//---------------------------------------------------------------------------
//显示导航面板
//---------------------------------------------------------------------------
void __fastcall TfrmBaseListForm::ShowNavPane(bool bShowPane)
{
 /* QuickPane->Clear();

  int iCode=0,iImageIndex;
  bool IsGroupCreate=false;

  RelaFormSet->First();   //添加辅助工具
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "2")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 57; //默认图标

      //添加NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("功能列表",0,2);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }

  IsGroupCreate = false;
  RelaFormSet->First();   //添加相关业务菜单项
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "1" || RelaFormSet->FieldValues["createstyle"] == "3")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 41; //默认图标

      //添加NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("业务向导",0,1);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }

  IsGroupCreate = false;
  RelaFormSet->First();   //添加系统功能菜单项
  while(!RelaFormSet->Eof)
  {
    if(RelaFormSet->FieldValues["createstyle"] == "0")
    {
      if(RelaFormSet->FieldValues["ModuleCode"] != "")
        iCode = StrToInt(RelaFormSet->FieldValues["ModuleCode"]);
      else
        iCode = 0;
      if(RelaFormSet->FieldValues["ImageIndex"] != "")
        iImageIndex = StrToInt(RelaFormSet->FieldValues["ImageIndex"]);
      else
        iImageIndex = -1;
      if(iImageIndex == -1)
        iImageIndex = 7; //默认图标

      //添加NavItem
      if(!IsGroupCreate)
      {
        QuickPane->AddGroup("系统设置",0,0);
        IsGroupCreate = true;
      }
      QuickPane->AddItem(QuickPane->GroupCount-1,RelaFormSet->FieldValues["Caption"],
                           iImageIndex,iCode);
    }
    RelaFormSet->Next();
  }
  if (bShowPane) // 需要显示
  {
    QuickPane->DockPane();
    QuickPane->ShowPane();
  }
  */
}
//---------------------------------------------------------------------------

