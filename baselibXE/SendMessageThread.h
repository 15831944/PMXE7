//---------------------------------------------------------------------------

#ifndef SendMessageThreadH
#define SendMessageThreadH
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include "DSClientProxy.h"
//---------------------------------------------------------------------------
enum  TUserType : unsigned char { utUser, utGroup, utJob, utDepartment, utCompany };
class PACKAGE TSendMessageThread :public TThread
{
private:
	TMessageManageClient* FMessageManage;
	TUserType FUserType;
	String UserID;
	String MsgRemark;
protected:
	TJSONArray* __fastcall GetUserList();
	System::UnicodeString __fastcall GetHTMLEscapedString(System::UnicodeString Value);
	bool __fastcall SendMessageToAll(System::UnicodeString Msg);
	bool __fastcall SendMessageToUser(System::UnicodeString Msg, System::UnicodeString UserID);
	bool __fastcall SendMessageToDepartment(System::UnicodeString Msg, System::UnicodeString DepartmentID);
	bool __fastcall SendMessageToJob(System::UnicodeString Msg, System::UnicodeString JobID);
	bool __fastcall SendMessageToRole(System::UnicodeString Msg, System::UnicodeString RoleID);
public:
	virtual TSendMessageThread(TDBXConnection* DBXConnection, TUserType AUserType,String AMsg,String CurUserID);
	void __fastcall Execute();
};
#endif
