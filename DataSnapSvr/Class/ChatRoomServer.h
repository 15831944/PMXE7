//---------------------------------------------------------------------------

#ifndef ChatRoomServerH
#define ChatRoomServerH
#include <Windows.hpp>
#include <Messages.hpp>
#include <SysUtils.hpp>
#include <Variants.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Generics.Collections.hpp>
#include <StdCtrls.hpp>
#include <DSServer.hpp>
#include <DbxDataSnap.hpp>
#include <DBXCommon.hpp>
#include <ADODB.hpp>
#include <SqlExpr.hpp>
#include <Menus.hpp>
#include <DSHTTPLayer.hpp>
#include <DSCommonServer.hpp>
#include <IndyPeerImpl.hpp>
#include "LoginUserRecord.h"
//---------------------------------------------------------------------------
/// <summary>
///    Thread for broadcasting a message to all users, notifying them that
///    a user has logged in or out.
/// </summary>
class TUserNotifyThread :public TThread
{
  protected:
	TDSServer *FServer;
	bool FWasAdded;
	String AccBookID;
	String UserID;
	String UserName;
	String CallBackClientID;
  public:
	virtual TUserNotifyThread(TDSServer *Server, TLoginUserRecord* User, bool WasAdded);
	void __fastcall  Execute();
};
///
class TSendMessageThread :public TThread
{
  protected:
	TDSServer *FServer;
	String AccBookID;
	String UserID;
	String UserName;
	String CallBackClientID;
	TJSONValue *FMsgValue;
  public:
	virtual TSendMessageThread(TDSServer *Server, TLoginUserRecord* User, TJSONValue *MsgValue);
	void __fastcall  Execute();
};
/// <summary>
///   Boots the user with the given name.
/// </summary>
class TBootUserThread :public TThread
{
  protected:
	TDSServer* FServer;
	String AccBookID;
	String UserID;
	String UserName;
	String CallBackClientID;
  public:
	virtual  TBootUserThread(TDSServer *Server, TLoginUserRecord* User);
	void __fastcall  Execute();
};
/// <summary>
///    Thread for Send Miss  messages to Login user
/// </summary>
class TSendMissMessageThread :public TThread
{
private:
	TADOConnection *FAccConn;
	TADOQuery *AccQuery;
	TADOQuery *Query;
	void SendMissMessage();
	bool SendMissMessageToUser(String const Msg,String const FromID,String const FromName,String const MsgID,String const sendtime);
	bool SendMissMessageToDepartment(String const Msg,String const FromID,String const FromName, String const DepartmentID,String const MsgID,String const sendtime);
	bool SendMissMessageToGroup(String const Msg, String const FromID,String const FromName,String const GroupID,String const MsgID,String const sendtime);
  protected:
	TDSServer *FServer;
	String AccBookID;
	String UserID;
	String UserName;
	String CallBackClientID;
  public:
	virtual TSendMissMessageThread(TDSServer *Server, TLoginUserRecord* User, TADOConnection *AccConn);
	void __fastcall  Execute();
};
#endif
