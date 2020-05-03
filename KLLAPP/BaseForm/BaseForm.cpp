//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BaseForm.h"
#include "BaseGlobeVars.h"
#include <FMX.PhoneDialer.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmBaseForm *frmBaseForm;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TfrmBaseForm::TfrmBaseForm(TComponent* Owner)
        : TForm(Owner)
{
  FClientBroker=NULL;
  CreateStyle = fcsNone;
  InitCommon();
  //���ù��ù�������ɫ
 // InitDevLocalize();
}
void __fastcall TfrmBaseForm::InitCommon()
{
  FFormID=0;
  FMoneyHide=false;
  FFormDefReport=false;
  FWorkflowReport = false;
  FCanDefReport=false;
  FWorkflowSend = false;
  m_DataSet = NULL;
  FormModuleCode = -1;
  FFlowDefinID="";
  FWorkflowID="";
  FWorkflowStatus=wsNotStarted;
  m_GlobeVars = new TCZDataSet;
  m_GlobeVars->LoadFromString(gGlobeVarsStr); //���Ի�ȫ�ֱ���
  FormatSettings=new TFormatSettings();
  	FormatSettings->ShortDateFormat="yyyy-MM-dd";
	FormatSettings->DateSeparator='-';
	FormatSettings->TimeSeparator=':';
	FormatSettings->LongTimeFormat="hh:mm:ss.zzz";
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseForm::TfrmBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param)
  : TForm(Owner)
{
  FClientBroker=clBroker;
  CreateStyle = fcsClass;
  InitCommon();

  MasterDataSource = new Data::Db::TDataSource(this);
  DetailDataSource = new Data::Db::TDataSource(this);
  //��ȡ����Ȩ��
  FormModuleCode = ModuleCode;//StrToInt(ClassCode);  //����ģ�����
  String sClassCode = String(GetClassCode(ModuleCode)); //��ȡ����ҵ�������
  FFormID=GetFormCode(ModuleCode);
  InitWithClassCode(Owner,sClassCode,Param); //ʹ��ҵ���ഴ�����ݼ�
	//���ù��ù�������ɫ
  InitDevLocalize();
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseForm::TfrmBaseForm(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,String Param)
  : TForm(Owner)
{
  FClientBroker=clBroker;
  CreateStyle = fCZDataSet;
  InitCommon();

  MasterDataSource = new Data::Db::TDataSource(this);
  DetailDataSource = new Data::Db::TDataSource(this);
  //��ȡ����Ȩ��
  FormModuleCode = ModuleCode;//StrToInt(ClassCode);  //����ģ�����
  FFormID=GetFormCode(ModuleCode);
  InitWithDataSet(Owner,FDataSet,Param); //�����������ݼ�
  //���ù��ù�������ɫ
  InitDevLocalize();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::InitWithClassCode(TComponent* Owner,String ClassCode,String Param)
{
  AddReportType(L"�����������",0);
  m_DataSet=CreateDataSet(ClassCode);
  m_DataSet->MasterDataSetBeforePost=MasterDataSetBeforePost;
//  m_DataSet->MasterDataSetAfterPost=MasterDataSetAfterPost;
  m_DataSet->DetailDataSetBeforePost=DetailDataSetBeforePost;
//  m_DataSet->DetailDataSetAfterPost=DetailDataSetAfterPost;
  MasterDataSource->DataSet = m_DataSet->MasterDataSet;
  if(m_DataSet->DataSetType ==2)
    DetailDataSource->DataSet = m_DataSet->DetailDataSet;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::InitWithDataSet(TComponent* Owner,TZClientDataSet *FDataSet,String Param)
{
  AddReportType(L"�����������",0);
  m_DataSet=FDataSet;
  m_DataSet->MasterDataSetBeforePost=MasterDataSetBeforePost;
//  m_DataSet->MasterDataSetAfterPost=MasterDataSetAfterPost;
  m_DataSet->DetailDataSetBeforePost=DetailDataSetBeforePost;
//  m_DataSet->DetailDataSetAfterPost=DetailDataSetAfterPost;
  MasterDataSource->DataSet = m_DataSet->MasterDataSet;
  if(m_DataSet->DataSetType ==2)
	DetailDataSource->DataSet = m_DataSet->DetailDataSet;
}
//---------------------------------------------------------------------------
__fastcall TfrmBaseForm::~TfrmBaseForm()
{
  if (FReportTypeList)
  {
	 delete FReportTypeList;
  }
  delete m_GlobeVars;
  delete dsFormDefRpt;

  if(MasterDataSource)
    delete MasterDataSource;
  if(DetailDataSource)
    delete DetailDataSource;
  if(CreateStyle == fcsClass)
  {
    FreeDataSet(DataSet);
  }
  delete FormatSettings;
}
//---------------------------------------------------------------------------
TZClientDataSet * __fastcall TfrmBaseForm::CreateDataSet(String ClassCode)
{
  return (new TZClientDataSet(this,FClientBroker,ClassCode));
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::FreeDataSet(TZClientDataSet * ds)
{
  if(ds==NULL)
	return;
  delete ds;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_GlobeVarValue(String varname)
{
  if(m_GlobeVars->RecordCount>0)
  {
    m_GlobeVars->First();
    return m_GlobeVars->FieldValues[varname];
  }
  else
	throw Exception(L"ϵͳδ����ȫ�ֱ���");
}
//---------------------------------------------------------------------------
/************�ض������ݼ��ķ���������*****BEGIN*****/
void __fastcall TfrmBaseForm::DS_Open()
{
  BeforeDataSetOperate(otOpen);//ִ�в���ǰ�Ĵ���
  m_DataSet->Open();
  AfterDataSetOperate(otOpen); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_AddNew()
{
  BeforeDataSetOperate(otAdd); //ִ�в���ǰ�Ĵ���
  m_DataSet->AddNew();
  AfterDataSetOperate(otAdd);  //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Insert()
{
  BeforeDataSetOperate(otInsert); //ִ�в���ǰ�Ĵ���
  m_DataSet->Insert();
  AfterDataSetOperate(otInsert);  //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Edit()
{
  BeforeDataSetOperate(otEdit); //ִ�в���ǰ�Ĵ���
  m_DataSet->Edit();
  AfterDataSetOperate(otEdit); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Delete()
{
  BeforeDataSetOperate(otDel); //ִ�в���ǰ�Ĵ���
  m_DataSet->Delete();
  AfterDataSetOperate(otDel); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Update()
{
  BeforeDataSetOperate(otUpdate); //ִ�в���ǰ�Ĵ���
  m_DataSet->Update();
  AfterDataSetOperate(otUpdate); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Cancel()
{
  BeforeDataSetOperate(otCancel); //ִ�в���ǰ�Ĵ���
  m_DataSet->Cancel();
  AfterDataSetOperate(otCancel); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_AddDetail()
{
  BeforeDataSetOperate(otAddDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->AddDetail();
  AfterDataSetOperate(otAddDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_InserDetail()
{
  BeforeDataSetOperate(otInsertDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->InserDetail();
  AfterDataSetOperate(otInsertDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_EditDetail()
{
  BeforeDataSetOperate(otEditDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->EditDetail();
  AfterDataSetOperate(otEditDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DeleteDetail()
{
  BeforeDataSetOperate(otDelDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->DeleteDetail();
  AfterDataSetOperate(otDelDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_UpdateDetail()
{
  BeforeDataSetOperate(otUpdateDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->UpdateDetail();
  AfterDataSetOperate(otUpdateDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_CancelDetail()
{
  BeforeDataSetOperate(otCancelDetail); //ִ�в���ǰ�Ĵ���
  m_DataSet->CancelDetail();
  AfterDataSetOperate(otCancelDetail); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Close()
{
  BeforeDataSetOperate(otClose); //ִ�в���ǰ�Ĵ���
  m_DataSet->Close();
  AfterDataSetOperate(otClose); //ִ�в�����Ĵ���
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_MoveFirst()
{
  m_DataSet->MoveFirst();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_MoveLast()
{
  m_DataSet->MoveLast();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_MoveNext()
{
  m_DataSet->MoveNext();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_MovePrior()
{
  m_DataSet->MovePrior();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_MoveTo(int Distance)
{
  m_DataSet->MoveTo(Distance);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DetailFirst()
{
  m_DataSet->DetailFirst();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DetailLast()
{
  m_DataSet->DetailLast();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DetailPrior()
{
  m_DataSet->DetailPrior();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DetailNext()
{
  m_DataSet->DetailNext();
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_DetailMoveTo(int Distance)
{
  m_DataSet->DetailMoveTo(Distance);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::DS_LocateMaster(String KeyValues)
{
 return m_DataSet->LocateMaster(KeyValues);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::DS_LocateDetail(String KeyValues)
{
 return m_DataSet->LocateDetail(KeyValues);
}
//---------------------------------------------------------------------------
TField *__fastcall TfrmBaseForm::DS_FindMasterField(String fieldname)
{
 return m_DataSet->FindMasterField(fieldname);
}
//---------------------------------------------------------------------------
TField *__fastcall TfrmBaseForm::DS_FindDetailField(String fieldname)
{
 return m_DataSet->FindDetailField(fieldname);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::DS_FindMasterKey(String KeyValues)
{
 return m_DataSet->FindMasterKey(KeyValues);
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::DS_FindDetailKey(String KeyValues)
{
 return m_DataSet->FindDetailKey(KeyValues);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseForm::DS_GetMasterValue(String fieldname)
{
  Variant tmpval;
  tmpval=m_DataSet->GetMasterValue(fieldname);
  if(tmpval.IsNull())
  {
    TField *tmpfield;
    tmpfield = m_DataSet->MasterFields->FieldByName(fieldname);
    if(tmpfield == NULL)
	  throw Exception(L"�ֶ�<"+fieldname+L">������");
    else
    {
      switch(tmpfield->DataType)
      {
        case ftSmallint:
        case ftInteger:
        case ftWord:
        case ftBoolean:
        case ftFloat:
        case ftCurrency:
        case ftLargeint:
        case ftBCD:
          return Variant(0);
        default:
          return Variant("");
      }
    }
  }
  else
    return tmpval;
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseForm::DS_GetMasterOldValue(String fieldname)
{
  Variant tmpval;
  tmpval=m_DataSet->GetMasterOldValue(fieldname);
  if(tmpval.IsNull())
  {
    TField *tmpfield;
    tmpfield = m_DataSet->MasterFields->FieldByName(fieldname);
    if(tmpfield == NULL)
	  throw Exception(L"�ֶ�<"+fieldname+L">������");
    else
    {
      switch(tmpfield->DataType)
      {
        case ftSmallint:
        case ftInteger:
        case ftWord:
        case ftBoolean:
        case ftFloat:
        case ftCurrency:
        case ftLargeint:
        case ftBCD:
          return Variant(0);
        default:
          return Variant("");
      }
    }
  }
  else
    return tmpval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_SetMasterValue(String fieldname,Variant value)
{
  TField *tmpfield;
  tmpfield = m_DataSet->MasterFields->FieldByName(fieldname);
  if(tmpfield == NULL)
	throw Exception(L"�ֶ�<"+fieldname+L">������");
  else
  {
    switch(tmpfield->DataType)
    {
      case ftInteger:
       try
       {
         int tmp = int(value);
       }
       catch(EVariantError &e)
       {
		 throw Exception(L"��Ч��������ֵ");
       }
       if(abs(int(value)) > 2E9) //��������λ�����ܳ���10λ
		 throw Exception(L"������ֵλ���������ﷶΧ");
       break;
      case ftFloat:
      case ftCurrency:
      case ftBCD:
       try
       {
         double tmp = double(value);
       }
       catch(EVariantError &e)
       {
		throw Exception(L"��Ч�ĸ�����ֵ");
       }
       if(fabs(double(value)) >= 1E12) //��������λ�����ܳ���12λ
		 throw Exception(L"������ֵλ���������ﷶΧ");
       break;
      case ftString:
	   if(VarToStr(value).Length() > tmpfield->Size)
		 throw Exception(L"�ı����ȳ������ﷶΧ");
      default:
        break;
	}
  }
  m_DataSet->SetMasterValue(fieldname,value);
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseForm::DS_GetDetailValue(String fieldname)
{
  Variant tmpval;
  tmpval=m_DataSet->GetDetailValue(fieldname);
  if(tmpval.IsNull())
  {
    TField *tmpfield;
    tmpfield = m_DataSet->DetailFields->FieldByName(fieldname);
    if(tmpfield == NULL)
	  throw Exception(L"�ֶ�<"+fieldname+L">������");
    else
    {
      switch(tmpfield->DataType)
      {
        case ftSmallint:
        case ftInteger:
        case ftWord:
        case ftBoolean:
        case ftFloat:
        case ftCurrency:
        case ftLargeint:
        case ftBCD:
          return Variant(0);
        default:
          return Variant("");
      }
    }
  }
  else
    return tmpval;
}
//---------------------------------------------------------------------------
Variant __fastcall TfrmBaseForm::DS_GetDetailOldValue(String fieldname)
{
  Variant tmpval;
  tmpval=m_DataSet->GetDetailOldValue(fieldname);
  if(tmpval.IsNull())
  {
    TField *tmpfield;
    tmpfield = m_DataSet->DetailFields->FieldByName(fieldname);
    if(tmpfield == NULL)
	  throw Exception(L"�ֶ�<"+fieldname+L">������");
    else
    {
      switch(tmpfield->DataType)
      {
        case ftSmallint:
        case ftInteger:
        case ftWord:
        case ftBoolean:
        case ftFloat:
        case ftCurrency:
        case ftLargeint:
        case ftBCD:
          return Variant(0);
        default:
          return Variant("");
      }
    }
  }
  else
    return tmpval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_SetDetailValue(String fieldname,Variant value)
{
  TField *tmpfield;
  tmpfield = m_DataSet->DetailFields->FieldByName(fieldname);
  if(tmpfield == NULL)
	throw Exception(L"�ֶ�<"+fieldname+L">������");
  else
  {
    switch(tmpfield->DataType)
    {
      case ftInteger:
       try
       {
         int tmp = int(value);
       }
       catch(EVariantError &e)
       {
		 throw Exception(L"��Ч��������ֵ");
       }
       if(abs(int(value)) > 2E9) //��������λ�����ܳ���10λ
		 throw Exception(L"������ֵλ���������ﷶΧ");
       break;
      case ftFloat:
      case ftCurrency:
      case ftBCD:
       try
       {
         double tmp = double(value);
       }
       catch(EVariantError &e)
       {
		throw Exception(L"��Ч�ĸ�����ֵ");
       }
       if(fabs(double(value)) >= 1E12) //��������λ�����ܳ���12λ
		 throw Exception(L"������ֵλ���������ﷶΧ");
       break;
      case ftString:
	   if(VarToStr(value).Length() > tmpfield->Size)
         throw Exception(L"�ı����ȳ������ﷶΧ");
      default:
        break;
    }
  }
  m_DataSet->SetDetailValue(fieldname,value);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_CmdExec(String Param)
{
 m_DataSet->CmdExec(Param);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Check(int IsCheck)
{
 m_DataSet->Check(IsCheck);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_CloseOff(int IsClose)
{
 m_DataSet->CloseOff(IsClose);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Revoke(int IsRevoke)
{
 m_DataSet->Revoke(IsRevoke);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_Tally(int IsTally)
{
 m_DataSet->Tally(IsTally);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_ValidField(TValidType vt,TValidAffect va)
{
// m_DataSet->ValidField(vt,va);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_ExecSQL(String sqlstr)
{
 m_DataSet->ExecSQL(sqlstr);
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::DS_ExitClientDataSet()
{
 m_DataSet->ExitClientDataSet();
}
//---------------------------------------------------------------------------
//***********************
//property function
//***********************
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterSQLString()
{
 return m_DataSet->MasterSQLString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterSQLString(String value)
{
 //m_DataSet->MasterSQLString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailSQLString()
{
 return m_DataSet->DetailSQLString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailSQLString(String value)
{
 //m_DataSet->DetailSQLString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterWhereString()
{
 return m_DataSet->MasterWhereString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterWhereString(String value)
{
 m_DataSet->MasterWhereString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailWhereString()
{
 return m_DataSet->DetailWhereString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailWhereString(String value)
{
 m_DataSet->DetailWhereString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterOrderString()
{
 return m_DataSet->MasterOrderString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterOrderString(String value)
{
 m_DataSet->MasterOrderString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailOrderString()
{
 return m_DataSet->DetailOrderString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailOrderString(String value)
{
 m_DataSet->DetailOrderString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterGroupString()
{
 return m_DataSet->MasterGroupString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterGroupString(String value)
{
 m_DataSet->MasterGroupString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailGroupString()
{
 return m_DataSet->DetailGroupString;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailGroupString(String value)
{
 m_DataSet->DetailGroupString=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterFixWhereString()
{
 return m_DataSet->MasterFixedWhereString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterFixGroupString()
{
 return m_DataSet->MasterFixedGroupString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterFixOrderString()
{
 return m_DataSet->MasterFixedOrderString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailFixWhereString()
{
 return m_DataSet->DetailFixedWhereString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailFixGroupString()
{
 return m_DataSet->DetailFixedGroupString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailFixOrderString()
{
 return m_DataSet->DetailFixedOrderString;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterKeyFields()
{
 return m_DataSet->MasterKeyFields;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterKeyFields(String value)
{
 m_DataSet->MasterKeyFields=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailKeyFields()
{
 return m_DataSet->DetailKeyFields;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailKeyFields(String value)
{
 m_DataSet->DetailKeyFields=value;
}
//---------------------------------------------------------------------------

String __fastcall TfrmBaseForm::get_LinkFields()
{
 return m_DataSet->LinkFields;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_LinkFields(String value)
{
 m_DataSet->LinkFields=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterLinkString()
{
 return m_DataSet->MasterLinkStr;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterLinkFields()
{
 return m_DataSet->MasterLinkFields;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterSort()
{
 return m_DataSet->MasterSort;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterSort(String value)
{
 m_DataSet->MasterSort=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailSort()
{
 return m_DataSet->DetailSort;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailSort(String value)
{
 m_DataSet->DetailSort=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_Filter()
{
 return m_DataSet->Filter;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_Filter(String value)
{
 m_DataSet->Filter=value;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_Filtered()
{
 return m_DataSet->Filtered;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_Filtered(bool value)
{
 m_DataSet->Filtered=value;
}
//---------------------------------------------------------------------------
TDataSetState __fastcall TfrmBaseForm::get_MasterStatus()
{
 return m_DataSet->MasterStatus;
}
//---------------------------------------------------------------------------
TDataSetState __fastcall TfrmBaseForm::get_DetailStatus()
{
 return m_DataSet->DetailStatus;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_DataSetType()
{
 return m_DataSet->DataSetType;
}
//---------------------------------------------------------------------------
TZClientQuery * __fastcall TfrmBaseForm::get_Query()
{
 return m_DataSet->Query;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_Bof()
{
 return m_DataSet->Bof;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_Eof()
{
 return m_DataSet->Eof;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_RecordCount()
{
 return m_DataSet->RecordCount;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_CurRecNo()
{
 return m_DataSet->CurRecNo;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_MasterFieldCount()
{
 return m_DataSet->MasterFieldCount;
}
//---------------------------------------------------------------------------
TFields * __fastcall TfrmBaseForm::get_MasterFields()
{
 return m_DataSet->MasterFields;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_DetailBof()
{
 return m_DataSet->DetailBof;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_DetailEof()
{
 return m_DataSet->DetailEof;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_DetailCount()
{
 return m_DataSet->DetailCount;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_DetailRecNo()
{
 return m_DataSet->DetailRecNo;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_DetailFieldCount()
{
 return m_DataSet->DetailFieldCount;
}
//---------------------------------------------------------------------------
TFields * __fastcall TfrmBaseForm::get_DetailFields()
{
 return m_DataSet->DetailFields;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterFieldName(int index)
{
 return m_DataSet->MasterFieldName[index];
}
//---------------------------------------------------------------------------
TFieldType __fastcall TfrmBaseForm::get_MasterFieldType(int index)
{
 return m_DataSet->MasterFieldType[index];
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_MasterFieldSize(int index)
{
 return m_DataSet->MasterFieldSize[index];
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailFieldName(int index)
{
 return m_DataSet->DetailFieldName[index];
}
//---------------------------------------------------------------------------
TFieldType __fastcall TfrmBaseForm::get_DetailFieldType(int index)
{
 return m_DataSet->DetailFieldType[index];
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_DetailFieldSize(int index)
{
 return m_DataSet->DetailFieldSize[index];
}

//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_DataSetActive()
{
 return m_DataSet->Active;
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::get_TimeOut()
{
 return m_DataSet->TimeOut;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_TimeOut(int value)
{
 m_DataSet->TimeOut=value;
}
//---------------------------------------------------------------------------
bool __fastcall TfrmBaseForm::get_NeedValid()
{
 return m_DataSet->NeedValid;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_NeedValid(bool value)
{
 m_DataSet->NeedValid=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_MasterValidFieldList()
{
 return m_DataSet->MasterValidFieldList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_MasterValidFieldList(String value)
{
 m_DataSet->MasterValidFieldList=value;
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::get_DetailValidFieldList()
{
 return m_DataSet->DetailValidFieldList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::set_DetailValidFieldList(String value)
{
 m_DataSet->DetailValidFieldList=value;
}
/************�ض������ݼ��ķ���������****END******/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::SetFormModuleCode(int value)
{
  FFormModuleCode = value;
  if(FFormModuleCode >= 0)
  {
	GetModuleRight(FFormModuleCode,FOperateRight);  //��ȡ����ϵͳȨ��
	FOperateRight = String(FOperateRight);
    GetModuleDefRight(FFormModuleCode,FFormDefRight);  //��ȡ�����Զ���Ȩ��
    GetModuleDefFunc(FFormModuleCode,FFormDefFunc);  //��ȡ�����Զ��幦��
	GetDataAccess(FFormModuleCode,FFormDataAccess);  //��ȡ�����Զ�������Ȩ��
  }
  else
  {
    FOperateRight = ""; //����Ȩ��
    FFormDefRight = ""; //�����Զ���Ȩ��
    FFormDefFunc = "";  //�����Զ��幦��
    FFormDataAccess = "";  //�����Զ�������Ȩ��
  }
}
//---------------------------------------------------------------------------
int __fastcall TfrmBaseForm::GetFormID()
{
//  if(FFormID<=0)
//    throw Exception("������ֵ����");
  return FFormID;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::SendMsgToOther(int itype,String Msg)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::AddReportType(String RptTypeName,int AType)
{
/*  if(!FReportTypeList)
	FReportTypeList = new TStringList();
  int * IData;
  IData = new int(AType);  //���ƶ��豸�ϱ���
  FReportTypeList->AddObject(RptTypeName,(TObject *)IData); */
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::InitDevLocalize()
{
  //this->Caption=Caption+FFormModuleCode;
}
//---------------------------------------------------------------------------
void __fastcall TfrmBaseForm::PhoneDialer(String APhoneNum)
{
   if (APhoneNum!= "")
   {
	_di_IFMXPhoneDialerService PhoneDialerService;
	/*  test whether the PhoneDialer services are supported */
	if(TPlatformServices::Current->SupportsPlatformService(__uuidof(IFMXPhoneDialerService)) &&
		(PhoneDialerService = TPlatformServices::Current->GetPlatformService(__uuidof(IFMXPhoneDialerService)))) {
		PhoneDialerService->Call(APhoneNum);
		}
	}
}
//---------------------------------------------------------------------------
TDate __fastcall TfrmBaseForm::YMDStrToDate(String AYMDStr)
{
   return StrToDate(AYMDStr,*FormatSettings);
}
//---------------------------------------------------------------------------
String __fastcall TfrmBaseForm::DateToYMDStr(TDate ADate)
{
  return ADate.FormatString("yyyy-mm-dd");
}
//---------------------------------------------------------------------------