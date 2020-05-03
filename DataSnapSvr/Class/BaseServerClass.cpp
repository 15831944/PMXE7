//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BaseServerClass.h"
#include "FSServerContainer.h"
#include "BaseFunc.h"
#include "DBJson.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma resource "*.dfm"
TDMBaseServerClass *DMBaseServerClass;
//---------------------------------------------------------------------------
__fastcall TDMBaseServerClass::TDMBaseServerClass(TComponent* Owner)
	: TComponent(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TDMBaseServerClass::~TDMBaseServerClass()
{
}
//---------------------------------------------------------------------------
TADOConnection *__fastcall TDMBaseServerClass::GetSysConnection()
{
	return DMServerContainer->GetSysConnection();
}
//---------------------------------------------------------------------------
TADOConnection *__fastcall TDMBaseServerClass::GetAccConnection()
{
	return DMServerContainer->GetConnection();
}
//---------------------------------------------------------------------------
int __fastcall TDMBaseServerClass::GetSysDbType()
{
	return DMServerContainer->DbType;
}
//---------------------------------------------------------------------------
TLoginUserRecord * __fastcall TDMBaseServerClass::GetCurLoginUser()
{
	return DMServerContainer->CurLoginUser;
}
//---------------------------------------------------------------------------
String __fastcall TDMBaseServerClass::DataSetToJson(String SQLText)
{
  //�о�һ�����ݼ����ת��Json���ݼ�����Ҫ�����ֶ�ת��
	String json = "";
	if(SQLText.Trim().IsEmpty()) return json;
	TADOQuery *Acc_Query = new TADOQuery(NULL);
	try
	{
		Acc_Query->Connection = AccConnection;
		Acc_Query->Close();
		Acc_Query->SQL->Clear();
		Acc_Query->SQL->Text = SQLText;
		Acc_Query->Open();
		json = TTableJson::DataSetToJson(Acc_Query);
		Acc_Query->Close();
	}
	__finally
	{
		delete Acc_Query;
		Acc_Query = NULL;
  }
	return json;
}
//---------------------------------------------------------------------------
TJSONObject* __fastcall TDMBaseServerClass::DataSetToJsonObject(String SQLText)
{
	if(SQLText.Trim().IsEmpty()) return (TJSONObject*)(new TJSONFalse());
	String json = DataSetToJson(SQLText);
	return (TJSONObject*)TJSONObject::ParseJSONValue(json);
}
//---------------------------------------------------------------------------
String __fastcall TDMBaseServerClass::DataSetSysToJson(String SQLText)
{
	String json = "";
	if(SQLText.Trim().IsEmpty()) return json;
	TADOQuery *Sys_Query = new TADOQuery(NULL);
	try
	{
		Sys_Query->Connection = SysConnection;
		Sys_Query->Close();
		Sys_Query->SQL->Clear();
		Sys_Query->SQL->Text = SQLText;
		Sys_Query->Open();
		json = TTableJson::DataSetToJson(Sys_Query);
		Sys_Query->Close();
	}
	__finally
	{
		delete Sys_Query;
		Sys_Query = NULL;
  }
  return json;
}
//---------------------------------------------------------------------------
TJSONObject* __fastcall TDMBaseServerClass::DataSetSysToJsonObject(String SQLText)
{
  if(SQLText.Trim().IsEmpty()) return (TJSONObject*)(new TJSONFalse());
	String json = DataSetSysToJson(SQLText);
	return (TJSONObject*)TJSONObject::ParseJSONValue(json);
}
//---------------------------------------------------------------------------
