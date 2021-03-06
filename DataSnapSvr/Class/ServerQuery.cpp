//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ServerQuery.h"
#include "BaseFunc.h"
#include "SHConst.h"
#include "XMLData.h"
#include "FSServerContainer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TServerQuery::TServerQuery(TComponent* Owner)
:TDMBaseServerClass(Owner)
{
	FSysDbType = 1;
	FAccDbType = 1;
	TLoginUserRecord *curLoginUser = GetCurLoginUser();
	if(curLoginUser)
	{
		FSysDbType = curLoginUser->SysDbType;
		FAccDbType = curLoginUser->AccDbType;
		FUserCode  = curLoginUser->UserLoginCode;
		FUserIP    = curLoginUser->IP;
		FAcckBook  = curLoginUser->AccBookID;
	}
	m_Query = new TADOQuery(NULL);
	m_Query->ParamCheck = false;
	m_Query->EnableBCD = false;
	m_Query->CommandTimeout = 60;

	b_Query = new TADOQuery(NULL);
	b_Query->ParamCheck = false;
	b_Query->EnableBCD = false;
	b_Query->CommandTimeout = 60;

	LogQuery = new TADOQuery(NULL);
	LogQuery->ParamCheck = false;

	m_Connection = this->AccConnection;
	m_Query->Connection = m_Connection;

	m_BuildConn = this->SysConnection;
	b_Query->Connection = m_BuildConn;
	LogQuery->Connection = m_BuildConn;
}
//---------------------------------------------------------------------------
__fastcall TServerQuery::~TServerQuery()
{
	delete m_Query;
	delete b_Query;
	delete LogQuery;
}
//---------------------------------------------------------------------------
TStream*  TServerQuery::QueryOpen(String SQLText)
{
	m_Query->Close();
	m_Query->SQL->Text = SQLText;
	m_Query->Open();
	TStream *ADOStream =AdoDataSetToCDStream(m_Query);
	m_Query->Close();
	ADOStream->Position = 0;
	return ADOStream;
}
//---------------------------------------------------------------------------
int  TServerQuery::QueryExecSQL(String SQLText)
{
  m_Query->Close();
  m_Query->SQL->Clear();
	m_Query->SQL->Text = SQLText;
	return m_Query->ExecSQL();
}
//---------------------------------------------------------------------------
int  TServerQuery::ExecSQL(String SQLText,TParams* Parameters)
{
	m_Query->Close();
	m_Query->SQL->Clear();	m_Query->Parameters->Clear();	if(FAccDbType == 1)		m_Query->ParamCheck = true;	else		m_Query->ParamCheck = false;	m_Query->SQL->Text = SQLText;
	if(Parameters!=NULL)
	{
		for(int i=0;i<Parameters->Count;i++)
		{
			TParam *sParameter = Parameters->Items[i];
			TParameter *tParameter = m_Query->Parameters->ParamByName(sParameter->Name);
			if(tParameter)
				tParameter->Value = sParameter->Value;
		}
	}
	return m_Query->ExecSQL();
}
//---------------------------------------------------------------------------
TStream*  TServerQuery::QuerySysOpen(String SQLText)
{
	b_Query->Close();
	b_Query->SQL->Text = SQLText;
	b_Query->Open();
	TStream *ADOStream =AdoDataSetToCDStream(b_Query);
	b_Query->Close();
	ADOStream->Position = 0;
	return ADOStream;
}
//---------------------------------------------------------------------------
int  TServerQuery::QuerySysExecSQL(String SQLText)
{
  b_Query->Close();
  b_Query->SQL->Clear();
  b_Query->SQL->Text = SQLText;
  return b_Query->ExecSQL();
}
//---------------------------------------------------------------------------
int  TServerQuery::SysExecSQL(String SQLText,TParams* Parameters)
{
	b_Query->Close();
	b_Query->SQL->Clear();
	b_Query->Parameters->Clear();
	if(FSysDbType == 1)
		b_Query->ParamCheck = true;
	else
		b_Query->ParamCheck = false;
	b_Query->SQL->Text = SQLText;

	if(Parameters!=NULL)
	{
		for(int i=0;i<Parameters->Count;i++)
		{
			TParam *sParameter = Parameters->Items[i];
			TParameter *tParameter = b_Query->Parameters->ParamByName(sParameter->Name);
			if(tParameter)
				tParameter->Value = sParameter->Value;
		}
	}
	return b_Query->ExecSQL();
}
//---------------------------------------------------------------------------
TStream*  TServerQuery::SendFailMessage(int operate, String errormessage)
{
	TMemoryStream *resultStream = new TMemoryStream();
	TMsgStream *sxs = new TMsgStream();
	try
	{
		sxs->Operate = operate;
		sxs->ValueAttribute["status"] = "f";
		sxs->ItemAttribute["errormessage"] = errormessage;
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
	}
	__finally
	{
		delete sxs;
	}
	return resultStream;
}
//---------------------------------------------------------------------------
TStream*  TServerQuery::SendErrorMessage(int errortype, int errorcode, String errormessage)
{
	TMsgStream *sxs = new TMsgStream();
	TMemoryStream *resultStream = new TMemoryStream();
	try
	{
		sxs->Operate = asDSError;
		sxs->ValueAttribute["errortype"] = errortype;
		sxs->ValueAttribute["errorcode"] = errorcode;
		sxs->ItemAttribute["errormessage"] = errormessage;
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
	}
	__finally
	{
		delete sxs;
	}
	return resultStream;
}
//---------------------------------------------------------------------------
void  TServerQuery::WriteUserLog(int type, String Desc)
{
   DMServerContainer->NotifyWriteLog(FUserCode,FUserIP,FAcckBook,"100",type,Desc);
}
//---------------------------------------------------------------------------
