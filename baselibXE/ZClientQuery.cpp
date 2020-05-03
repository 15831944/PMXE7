// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop
#include "ZClientQuery.h"
//#include "BaseFunc.h"
#include "MessageDef.h"
#include "SHConst.h"
//#include "ZClientBroker.h"
#include "ZClientDataset.h"
#include "DSClientProxy.h"
#include "fsScreenCursor.h"
#define _TimeOuts 60  //��
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
__fastcall TZClientQuery::TZClientQuery(TComponent* AOwner, TClientBroker *FBroker)
	: TDataSnapClientDataSet(AOwner)
{
	FClientBroker = FBroker;
	FSQL = new TStringList();
	((TStringList*)FSQL)->OnChange = QueryChanged;
	ServerQuery=new TServerQueryClient(FClientBroker->DBXConnection,false);
}
// ---------------------------------------------------------------------------
__fastcall TZClientQuery::~TZClientQuery()
{
	delete FSQL;
	if(ServerQuery) delete ServerQuery;
	Close();
}
// ---------------------------------------------------------------------------
TStringList* __fastcall TZClientQuery::GetSQL(void)
{
	return FSQL;
}

// ---------------------------------------------------------------------------
void __fastcall TZClientQuery::SetSQL(TStringList* Value)
{
	FSQL->Assign(Value);
}
// ---------------------------------------------------------------------------
void __fastcall TZClientQuery::Open()
{
    Sort="";
	if (!CommandText.IsEmpty())
	{
	  TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
		try
		{
		   TStream *sm=ServerQuery->QueryOpen(this->SQL->Text);
			sm->Position=0;
			LoadFromStream(sm);
	  }
	  __finally
	  {
		  delete ScreenCursor;
	  }
	}
	else
	{
		if (this->Active == false)
			TCustomClientDataSet::Open();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TZClientQuery::SysOpen()
{
	if (!CommandText.IsEmpty())
	{
	  TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
	  TStream *sm=NULL;
		try
		{
			sm=ServerQuery->QuerySysOpen(this->SQL->Text);
			sm->Position=0;
			LoadFromStream(sm);
		}
	  __finally
	  {
		  delete ScreenCursor;
	  }
	}
}

// ---------------------------------------------------------------------------
void __fastcall TZClientQuery::QueryChanged(TObject* Sender)
{
	if (!ComponentState.Contains(csLoading))
		Close();
   //	CommandType = cmdText;
	CommandText = FSQL->Text;
}

// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::ExecSQL()
{
	TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
	try
	{
		FRowsAffected = ServerQuery->QueryExecSQL(this->SQL->Text);
	}
	__finally
	{
		delete ScreenCursor;
	}
	return FRowsAffected;
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::ExecSQLByParams()
{
 TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
 TParams *tParams=new TParams(NULL);
 try
 {
	for(int i=0; i<Params->Count;i++)
	{
	   TParam* newParam= tParams->AddParameter();
	   newParam->Name=Params->Items[i]->Name;
	   newParam->Value=Params->Items[i]->Value;
	}
	FRowsAffected= ServerQuery->ExecSQL(this->SQL->Text,tParams);
 }
 __finally
 {
	 delete ScreenCursor;
	 delete tParams;
 }
 return FRowsAffected;
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::SysExecSQL()
{
 TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
 try
 {
	FRowsAffected= ServerQuery->QuerySysExecSQL(this->SQL->Text);
 }
 __finally
 {
	 delete ScreenCursor;
 }
 return FRowsAffected;
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::SysExecSQLByParams()
{
 TfsScreenCursor * ScreenCursor=new TfsScreenCursor();
  TParams *tParams=new TParams(NULL);
 try
 {
	for(int i=0; i<Params->Count;i++)
	{
	   TParam* newParam= tParams->AddParameter();
	   newParam->Name=Params->Items[i]->Name;
	   newParam->Value=Params->Items[i]->Value;
	}
	FRowsAffected= ServerQuery->SysExecSQL(this->SQL->Text,tParams);
 }
 __finally
 {
	 delete ScreenCursor;
	 delete tParams;
 }
 return FRowsAffected;
}
// ---------------------------------------------------------------------------
TDataSnapClientDataSet* __fastcall TZClientQuery::GetDataSet()
{
	return (TDataSnapClientDataSet*)(this);
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::get_CurRecNo()
{
	return this->RecNo;
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::GetSysDbType()
{
	return FClientBroker->SysDbType;
}
// ---------------------------------------------------------------------------
int __fastcall TZClientQuery::GetAccDbType()
{
	return FClientBroker->AccDbType;
}
// ---------------------------------------------------------------------------
