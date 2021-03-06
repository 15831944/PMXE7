//---------------------------------------------------------------------------
#pragma hdrstop

#include "LoginUserRecord.h"
#include <Data.DBXJSONReflect.hpp>	// Pascal unit
//---------------------------------------------------------------------------
void myRegister()
{
 RegisterClass(__classid(TBaseObject));
 RegisterClass(__classid(TLoginUserRecord));
}
//#pragma startup myRegister 100
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLoginUserRecord::TLoginUserRecord()
:TBaseObject()
{
  Initialized();
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::Initialized()
{
 	FSysDbType=1;
	FAccDbType=1;
	FUserLoginCode=""; //用户登录号
	FUserCode=""; //用户编号
	FUserID="";  //员工ID
	FUserName="";  //员工名称
	FIP="";
	FAccBookID="";
	FClientPort="";
	FCallBackClientID="";
	FLoginTime=Now();
	FFollowList=new TStringList();
}
//---------------------------------------------------------------------------
__fastcall TLoginUserRecord::~TLoginUserRecord()
{
  delete FFollowList;
}
//---------------------------------------------------------------------------
__fastcall TLoginUserRecord::TLoginUserRecord(TJSONObject* Json)
:TBaseObject()
{
   Initialized();
   if (Json->Count>0)
   {
   FThreadSessionId=Json->GetValue("threadSessionid")->Value().ToInt();
   FCallBackClientID=Json->GetValue("callbackclientid")->Value();
   FUserLoginCode=Json->GetValue("userlogincode")->Value();
   FUserCode=Json->GetValue("usercode")->Value();
   FUserID=Json->GetValue("userid")->Value();
   FUserName=Json->GetValue("username")->Value();
   FIP=Json->GetValue("ip")->Value();
   FClientPort=Json->GetValue("clientport")->Value();
   FProtocol=Json->GetValue("protocol")->Value();
   FAccBookID=Json->GetValue("accbookid")->Value();
   FLoginTime=StrToDateTime(Json->GetValue("logintime")->Value());
   FAccDbType=Json->GetValue("accdbtype")->Value().ToInt();
   FSysDbType=Json->GetValue("sysdbtype")->Value().ToInt();
   TJSONArray *Follows= (TJSONArray*)Json->GetValue("follows");
   if(Follows->Count>0)
   {
	 for(int i=0;i<Follows->Count;i++)
	{
	   FFollowList->Add(Follows->Items[i]->Value());
	}
   }
   }
  }
//---------------------------------------------------------------------------

void __fastcall TLoginUserRecord::SetThreadSessionId(int Value)
{
	FThreadSessionId=Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetUserLoginCode(String Value)
{
   FUserLoginCode=Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetUserCode(String Value)
{
	 FUserCode = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetCallBackClientID(String Value)
{
	 FCallBackClientID = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetUserID(String Value)
{
	 FUserID = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetUserName(String Value)
{
	 FUserName = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetIP(String Value)
{
	 FIP= Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetAccBookID(String Value)
{
	  FAccBookID = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetLoginTime(TDateTime Value)
{
	  FLoginTime = Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetAccDbType(int Value)
{
	FAccDbType = Value;
}
//---------------------------------------------------------------------------
TJSONObject* __fastcall TLoginUserRecord::ObjectToJSON()
{
  TJSONObject *result=new  TJSONObject();
  result->AddPair(new TJSONPair("threadSessionid", IntToStr(FThreadSessionId)));
  result->AddPair(new TJSONPair("callbackclientid", FCallBackClientID));
  result->AddPair(new TJSONPair("userlogincode", FUserLoginCode));
  result->AddPair(new TJSONPair("usercode", FUserCode));
  result->AddPair(new TJSONPair("userid", FUserID));
  result->AddPair(new TJSONPair("username", FUserName));
  result->AddPair(new TJSONPair("ip", FIP));
  result->AddPair(new TJSONPair("protocol", FProtocol));
  result->AddPair(new TJSONPair("clientport", FClientPort));
  result->AddPair(new TJSONPair("accbookid", FAccBookID));
  result->AddPair(new TJSONPair("logintime", FormatDateTime("yyyy-MM-dd HH:mm:ss",FLoginTime)));
  result->AddPair(new TJSONPair("accdbtype", IntToStr(FAccDbType)));
  result->AddPair(new TJSONPair("sysdbtype", IntToStr(FSysDbType)));
  TJSONArray *Follows=new TJSONArray();
  for(int i=0;i<FFollowList->Count;i++)
  {
	 Follows->AddElement(new TJSONString(FFollowList->Strings[i]));
  }
  result->AddPair(new TJSONPair("follows",Follows ));
  return result;
}
//---------------------------------------------------------------------------
void __fastcall TLoginUserRecord::SetFollowList(TStringList* Value)
{
  FFollowList->Clear();
  FFollowList->AddStrings(Value);
}
//---------------------------------------------------------------------------
