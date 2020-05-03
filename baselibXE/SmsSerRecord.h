//---------------------------------------------------------------------------

#ifndef SmsSerRecordH
#define SmsSerRecordH
#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
//#include <Dialogs.hpp>
#include "Baseobject.h"
//---------------------------------------------------------------------------
class TSmsSerRecord : public TBaseObject
{
private:
	int FThreadSessionId;
	String FIpAdress;
	String FCallBackClientID;
	TDateTime FLoginTime;
	String FClientPort;
	String FProtocol;
	String FAppName;
	void __fastcall Initialized();
public:
	__fastcall TSmsSerRecord();
	__fastcall virtual ~TSmsSerRecord();
	__fastcall TSmsSerRecord(TJSONObject* Json);
	TJSONObject* __fastcall  ObjectToJSON();
	__property int ThreadSessionId = {read=FThreadSessionId,write=FThreadSessionId};
	__property String CallBackClientID = {read = FCallBackClientID,write = FCallBackClientID};
	__property String IpAdress = {read =FIpAdress,write = FIpAdress};
	__property String ClientPort = {read = FClientPort,write = FClientPort};
	__property String Protocol = {read = FProtocol, write = FProtocol};
	__property String AppName = {read = FAppName, write = FAppName};
	__property TDateTime LoginTime = {read = FLoginTime, write = FLoginTime};
};
#endif
