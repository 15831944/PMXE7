//---------------------------------------------------------------------------

#ifndef LoginUserRecordH
#define LoginUserRecordH
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Data.DB.hpp>
#include "Baseobject.h"
//---------------------------------------------------------------------------
//#pragma explicit_rtti fields(__published)
class __declspec(delphiclass) TLoginUserRecord : public TBaseObject
{
private:
	int FThreadSessionId;
	String FUserLoginCode; //�û���¼��
	String FUserCode; //�û����
	String FUserID;  //Ա��ID
	String FUserName;  //Ա������
	String FIP;
	String FAccBookID;
	String FCallBackClientID;
	TDateTime FLoginTime;
	int FAccDbType;
	int FSysDbType;
	String FClientPort;
	String FProtocol;
	TStringList * FFollowList;
	void __fastcall Initialized();
	void __fastcall SetThreadSessionId(int Value);
	void __fastcall SetCallBackClientID(String Value);
	void __fastcall SetUserLoginCode(String Value);
	void __fastcall SetUserCode(String Value);
	void __fastcall SetUserID(String Value);
	void __fastcall SetUserName(String Value);
	void __fastcall SetIP(String Value);
	void __fastcall SetAccBookID(String Value);
	void __fastcall SetLoginTime(TDateTime Value);
	void __fastcall SetAccDbType(int Value);
	void __fastcall SetFollowList(TStringList* Value);
public:
	__fastcall TLoginUserRecord();
	__fastcall virtual ~TLoginUserRecord();
	__fastcall TLoginUserRecord(TJSONObject* Json);
	TJSONObject* __fastcall  ObjectToJSON();
	__property int ThreadSessionId = {read=FThreadSessionId,write=SetThreadSessionId};
	__property String CallBackClientID = {read = FCallBackClientID,write = SetCallBackClientID};
	__property String UserLoginCode = {read = FUserLoginCode,write = SetUserLoginCode};
	__property String UserCode = {read = FUserCode,write = SetUserCode};
	__property String UserID = {read = FUserID,write = SetUserID};
	__property String UserName = {read = FUserName, write = SetUserName};
	__property String IP = {read =FIP,write = SetIP};
	__property String ClientPort = {read = FClientPort,write = FClientPort};
	__property String Protocol = {read = FProtocol, write = FProtocol};
	__property String AccBookID = {read = FAccBookID,write = SetAccBookID};
	__property TDateTime LoginTime = {read = FLoginTime, write = SetLoginTime};
	__property int AccDbType = {read = FAccDbType,write = SetAccDbType};
	__property int SysDbType = {read = FSysDbType,write =SetAccDbType };
	__property TStringList * FollowList ={read = FFollowList,write =SetFollowList};

};
#endif
