// 
// Created by the DataSnap proxy generator.
// 2014-08-22 10:39:00
// 

#include "DSClientProxy.h"

System::UnicodeString __fastcall TLoginUserClient::GetSysConnectionString()
{
  if (FGetSysConnectionStringCommand == NULL)
  {
    FGetSysConnectionStringCommand = FDBXConnection->CreateCommand();
    FGetSysConnectionStringCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetSysConnectionStringCommand->Text = "TLoginUser.GetSysConnectionString";
    FGetSysConnectionStringCommand->Prepare();
  }
  FGetSysConnectionStringCommand->ExecuteUpdate();
  System::UnicodeString result = FGetSysConnectionStringCommand->Parameters->Parameter[0]->Value->GetWideString();
  return result;
}

bool __fastcall TLoginUserClient::ChangePassword(System::UnicodeString UserCode, System::UnicodeString OldPassword, System::UnicodeString NewPassword, System::UnicodeString &msg)
{
  if (FChangePasswordCommand == NULL)
  {
    FChangePasswordCommand = FDBXConnection->CreateCommand();
    FChangePasswordCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FChangePasswordCommand->Text = "TLoginUser.ChangePassword";
    FChangePasswordCommand->Prepare();
  }
  FChangePasswordCommand->Parameters->Parameter[0]->Value->SetWideString(UserCode);
  FChangePasswordCommand->Parameters->Parameter[1]->Value->SetWideString(OldPassword);
  FChangePasswordCommand->Parameters->Parameter[2]->Value->SetWideString(NewPassword);
  FChangePasswordCommand->Parameters->Parameter[3]->Value->SetWideString(msg);
  FChangePasswordCommand->ExecuteUpdate();
  msg = FChangePasswordCommand->Parameters->Parameter[3]->Value->GetWideString();
  bool result = FChangePasswordCommand->Parameters->Parameter[4]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::UpdateCurUserCallbackClientID(System::UnicodeString Value)
{
  if (FUpdateCurUserCallbackClientIDCommand == NULL)
  {
    FUpdateCurUserCallbackClientIDCommand = FDBXConnection->CreateCommand();
    FUpdateCurUserCallbackClientIDCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateCurUserCallbackClientIDCommand->Text = "TLoginUser.UpdateCurUserCallbackClientID";
    FUpdateCurUserCallbackClientIDCommand->Prepare();
  }
  FUpdateCurUserCallbackClientIDCommand->Parameters->Parameter[0]->Value->SetWideString(Value);
  FUpdateCurUserCallbackClientIDCommand->ExecuteUpdate();
  bool result = FUpdateCurUserCallbackClientIDCommand->Parameters->Parameter[1]->Value->GetBoolean();
  return result;
}

void __fastcall TLoginUserClient::Logout()
{
  if (FLogoutCommand == NULL)
  {
    FLogoutCommand = FDBXConnection->CreateCommand();
    FLogoutCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FLogoutCommand->Text = "TLoginUser.Logout";
    FLogoutCommand->Prepare();
  }
  FLogoutCommand->ExecuteUpdate();
}

void __fastcall TLoginUserClient::SetUsersToFollow(TJSONArray* Users)
{
  if (FSetUsersToFollowCommand == NULL)
  {
    FSetUsersToFollowCommand = FDBXConnection->CreateCommand();
    FSetUsersToFollowCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSetUsersToFollowCommand->Text = "TLoginUser.SetUsersToFollow";
    FSetUsersToFollowCommand->Prepare();
  }
  FSetUsersToFollowCommand->Parameters->Parameter[0]->Value->SetJSONValue(Users, FInstanceOwner);
  FSetUsersToFollowCommand->ExecuteUpdate();
}

bool __fastcall TLoginUserClient::SendMessageToFollow(System::UnicodeString Msg)
{
  if (FSendMessageToFollowCommand == NULL)
  {
    FSendMessageToFollowCommand = FDBXConnection->CreateCommand();
    FSendMessageToFollowCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToFollowCommand->Text = "TLoginUser.SendMessageToFollow";
    FSendMessageToFollowCommand->Prepare();
  }
  FSendMessageToFollowCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
  FSendMessageToFollowCommand->ExecuteUpdate();
  bool result = FSendMessageToFollowCommand->Parameters->Parameter[1]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendMessageToAll(System::UnicodeString Msg)
{
  if (FSendMessageToAllCommand == NULL)
  {
    FSendMessageToAllCommand = FDBXConnection->CreateCommand();
    FSendMessageToAllCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToAllCommand->Text = "TLoginUser.SendMessageToAll";
    FSendMessageToAllCommand->Prepare();
  }
  FSendMessageToAllCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
  FSendMessageToAllCommand->ExecuteUpdate();
  bool result = FSendMessageToAllCommand->Parameters->Parameter[1]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendMessageToUser(System::UnicodeString Msg, System::UnicodeString UserID)
{
  if (FSendMessageToUserCommand == NULL)
  {
    FSendMessageToUserCommand = FDBXConnection->CreateCommand();
    FSendMessageToUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToUserCommand->Text = "TLoginUser.SendMessageToUser";
    FSendMessageToUserCommand->Prepare();
  }
  FSendMessageToUserCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
  FSendMessageToUserCommand->Parameters->Parameter[1]->Value->SetWideString(UserID);
  FSendMessageToUserCommand->ExecuteUpdate();
  bool result = FSendMessageToUserCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendMessageToDepartment(System::UnicodeString Msg, System::UnicodeString DepartmentID)
{
  if (FSendMessageToDepartmentCommand == NULL)
  {
    FSendMessageToDepartmentCommand = FDBXConnection->CreateCommand();
    FSendMessageToDepartmentCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToDepartmentCommand->Text = "TLoginUser.SendMessageToDepartment";
    FSendMessageToDepartmentCommand->Prepare();
  }
  FSendMessageToDepartmentCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
  FSendMessageToDepartmentCommand->Parameters->Parameter[1]->Value->SetWideString(DepartmentID);
  FSendMessageToDepartmentCommand->ExecuteUpdate();
  bool result = FSendMessageToDepartmentCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendMessageToGroup(System::UnicodeString Msg, System::UnicodeString GroupID)
{
  if (FSendMessageToGroupCommand == NULL)
  {
    FSendMessageToGroupCommand = FDBXConnection->CreateCommand();
    FSendMessageToGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToGroupCommand->Text = "TLoginUser.SendMessageToGroup";
    FSendMessageToGroupCommand->Prepare();
  }
  FSendMessageToGroupCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
  FSendMessageToGroupCommand->Parameters->Parameter[1]->Value->SetWideString(GroupID);
  FSendMessageToGroupCommand->ExecuteUpdate();
  bool result = FSendMessageToGroupCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendCMDToFollow(System::UnicodeString cmdStr)
{
  if (FSendCMDToFollowCommand == NULL)
  {
    FSendCMDToFollowCommand = FDBXConnection->CreateCommand();
    FSendCMDToFollowCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToFollowCommand->Text = "TLoginUser.SendCMDToFollow";
    FSendCMDToFollowCommand->Prepare();
  }
  FSendCMDToFollowCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
  FSendCMDToFollowCommand->ExecuteUpdate();
  bool result = FSendCMDToFollowCommand->Parameters->Parameter[1]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendCMDToAll(System::UnicodeString cmdStr)
{
  if (FSendCMDToAllCommand == NULL)
  {
    FSendCMDToAllCommand = FDBXConnection->CreateCommand();
    FSendCMDToAllCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToAllCommand->Text = "TLoginUser.SendCMDToAll";
    FSendCMDToAllCommand->Prepare();
  }
  FSendCMDToAllCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
  FSendCMDToAllCommand->ExecuteUpdate();
  bool result = FSendCMDToAllCommand->Parameters->Parameter[1]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendCMDToUser(System::UnicodeString cmdStr, System::UnicodeString UserID)
{
  if (FSendCMDToUserCommand == NULL)
  {
    FSendCMDToUserCommand = FDBXConnection->CreateCommand();
    FSendCMDToUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToUserCommand->Text = "TLoginUser.SendCMDToUser";
    FSendCMDToUserCommand->Prepare();
  }
  FSendCMDToUserCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
  FSendCMDToUserCommand->Parameters->Parameter[1]->Value->SetWideString(UserID);
  FSendCMDToUserCommand->ExecuteUpdate();
  bool result = FSendCMDToUserCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendCMDToDepartment(System::UnicodeString cmdStr, System::UnicodeString DepartmentID)
{
  if (FSendCMDToDepartmentCommand == NULL)
  {
    FSendCMDToDepartmentCommand = FDBXConnection->CreateCommand();
    FSendCMDToDepartmentCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToDepartmentCommand->Text = "TLoginUser.SendCMDToDepartment";
    FSendCMDToDepartmentCommand->Prepare();
  }
  FSendCMDToDepartmentCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
  FSendCMDToDepartmentCommand->Parameters->Parameter[1]->Value->SetWideString(DepartmentID);
  FSendCMDToDepartmentCommand->ExecuteUpdate();
  bool result = FSendCMDToDepartmentCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendCMDToGroup(System::UnicodeString cmdStr, System::UnicodeString GroupID)
{
  if (FSendCMDToGroupCommand == NULL)
  {
    FSendCMDToGroupCommand = FDBXConnection->CreateCommand();
    FSendCMDToGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToGroupCommand->Text = "TLoginUser.SendCMDToGroup";
    FSendCMDToGroupCommand->Prepare();
  }
  FSendCMDToGroupCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
  FSendCMDToGroupCommand->Parameters->Parameter[1]->Value->SetWideString(GroupID);
  FSendCMDToGroupCommand->ExecuteUpdate();
  bool result = FSendCMDToGroupCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

bool __fastcall TLoginUserClient::SendMgrMessage(int itype, System::UnicodeString Msg)
{
  if (FSendMgrMessageCommand == NULL)
  {
    FSendMgrMessageCommand = FDBXConnection->CreateCommand();
    FSendMgrMessageCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMgrMessageCommand->Text = "TLoginUser.SendMgrMessage";
    FSendMgrMessageCommand->Prepare();
  }
  FSendMgrMessageCommand->Parameters->Parameter[0]->Value->SetInt32(itype);
  FSendMgrMessageCommand->Parameters->Parameter[1]->Value->SetWideString(Msg);
  FSendMgrMessageCommand->ExecuteUpdate();
  bool result = FSendMgrMessageCommand->Parameters->Parameter[2]->Value->GetBoolean();
  return result;
}

TStream* __fastcall TLoginUserClient::GetAccList()
{
  if (FGetAccListCommand == NULL)
  {
    FGetAccListCommand = FDBXConnection->CreateCommand();
    FGetAccListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAccListCommand->Text = "TLoginUser.GetAccList";
    FGetAccListCommand->Prepare();
  }
  FGetAccListCommand->ExecuteUpdate();
  TStream* result = FGetAccListCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::GetCurUserInfo()
{
  if (FGetCurUserInfoCommand == NULL)
  {
    FGetCurUserInfoCommand = FDBXConnection->CreateCommand();
    FGetCurUserInfoCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetCurUserInfoCommand->Text = "TLoginUser.GetCurUserInfo";
    FGetCurUserInfoCommand->Prepare();
  }
  FGetCurUserInfoCommand->ExecuteUpdate();
  TStream* result = FGetCurUserInfoCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::ListOfLoginUser()
{
  if (FListOfLoginUserCommand == NULL)
  {
    FListOfLoginUserCommand = FDBXConnection->CreateCommand();
    FListOfLoginUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FListOfLoginUserCommand->Text = "TLoginUser.ListOfLoginUser";
    FListOfLoginUserCommand->Prepare();
  }
  FListOfLoginUserCommand->ExecuteUpdate();
  TStream* result = FListOfLoginUserCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::ListOfAllLoginUser()
{
  if (FListOfAllLoginUserCommand == NULL)
  {
    FListOfAllLoginUserCommand = FDBXConnection->CreateCommand();
    FListOfAllLoginUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FListOfAllLoginUserCommand->Text = "TLoginUser.ListOfAllLoginUser";
    FListOfAllLoginUserCommand->Prepare();
  }
  FListOfAllLoginUserCommand->ExecuteUpdate();
  TStream* result = FListOfAllLoginUserCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::GetClassList()
{
  if (FGetClassListCommand == NULL)
  {
    FGetClassListCommand = FDBXConnection->CreateCommand();
    FGetClassListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetClassListCommand->Text = "TLoginUser.GetClassList";
    FGetClassListCommand->Prepare();
  }
  FGetClassListCommand->ExecuteUpdate();
  TStream* result = FGetClassListCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::GetAutoCodeFields()
{
  if (FGetAutoCodeFieldsCommand == NULL)
  {
    FGetAutoCodeFieldsCommand = FDBXConnection->CreateCommand();
    FGetAutoCodeFieldsCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAutoCodeFieldsCommand->Text = "TLoginUser.GetAutoCodeFields";
    FGetAutoCodeFieldsCommand->Prepare();
  }
  FGetAutoCodeFieldsCommand->ExecuteUpdate();
  TStream* result = FGetAutoCodeFieldsCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::GetModuleData()
{
  if (FGetModuleDataCommand == NULL)
  {
    FGetModuleDataCommand = FDBXConnection->CreateCommand();
    FGetModuleDataCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetModuleDataCommand->Text = "TLoginUser.GetModuleData";
    FGetModuleDataCommand->Prepare();
  }
  FGetModuleDataCommand->ExecuteUpdate();
  TStream* result = FGetModuleDataCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::RefreshGlobeVar()
{
  if (FRefreshGlobeVarCommand == NULL)
  {
    FRefreshGlobeVarCommand = FDBXConnection->CreateCommand();
    FRefreshGlobeVarCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FRefreshGlobeVarCommand->Text = "TLoginUser.RefreshGlobeVar";
    FRefreshGlobeVarCommand->Prepare();
  }
  FRefreshGlobeVarCommand->ExecuteUpdate();
  TStream* result = FRefreshGlobeVarCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TStream* __fastcall TLoginUserClient::GetModuleRight()
{
  if (FGetModuleRightCommand == NULL)
  {
    FGetModuleRightCommand = FDBXConnection->CreateCommand();
    FGetModuleRightCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetModuleRightCommand->Text = "TLoginUser.GetModuleRight";
    FGetModuleRightCommand->Prepare();
  }
  FGetModuleRightCommand->ExecuteUpdate();
  TStream* result = FGetModuleRightCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetAccInforJSON()
{
  if (FGetAccInforJSONCommand == NULL)
  {
    FGetAccInforJSONCommand = FDBXConnection->CreateCommand();
    FGetAccInforJSONCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAccInforJSONCommand->Text = "TLoginUser.GetAccInforJSON";
    FGetAccInforJSONCommand->Prepare();
  }
  FGetAccInforJSONCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetAccInforJSONCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetAccListJSON()
{
  if (FGetAccListJSONCommand == NULL)
  {
    FGetAccListJSONCommand = FDBXConnection->CreateCommand();
    FGetAccListJSONCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAccListJSONCommand->Text = "TLoginUser.GetAccListJSON";
    FGetAccListJSONCommand->Prepare();
  }
  FGetAccListJSONCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetAccListJSONCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetCurUserJSON()
{
  if (FGetCurUserJSONCommand == NULL)
  {
    FGetCurUserJSONCommand = FDBXConnection->CreateCommand();
    FGetCurUserJSONCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetCurUserJSONCommand->Text = "TLoginUser.GetCurUserJSON";
    FGetCurUserJSONCommand->Prepare();
  }
  FGetCurUserJSONCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetCurUserJSONCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONArray* __fastcall TLoginUserClient::GetUserList()
{
  if (FGetUserListCommand == NULL)
  {
    FGetUserListCommand = FDBXConnection->CreateCommand();
    FGetUserListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetUserListCommand->Text = "TLoginUser.GetUserList";
    FGetUserListCommand->Prepare();
  }
  FGetUserListCommand->ExecuteUpdate();
  TJSONArray* result = (TJSONArray*)FGetUserListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONArray* __fastcall TLoginUserClient::GetAllUserList()
{
  if (FGetAllUserListCommand == NULL)
  {
    FGetAllUserListCommand = FDBXConnection->CreateCommand();
    FGetAllUserListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAllUserListCommand->Text = "TLoginUser.GetAllUserList";
    FGetAllUserListCommand->Prepare();
  }
  FGetAllUserListCommand->ExecuteUpdate();
  TJSONArray* result = (TJSONArray*)FGetAllUserListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetClassListJO()
{
  if (FGetClassListJOCommand == NULL)
  {
    FGetClassListJOCommand = FDBXConnection->CreateCommand();
    FGetClassListJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetClassListJOCommand->Text = "TLoginUser.GetClassListJO";
    FGetClassListJOCommand->Prepare();
  }
  FGetClassListJOCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetClassListJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetAutoCodeFieldsJO()
{
  if (FGetAutoCodeFieldsJOCommand == NULL)
  {
    FGetAutoCodeFieldsJOCommand = FDBXConnection->CreateCommand();
    FGetAutoCodeFieldsJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAutoCodeFieldsJOCommand->Text = "TLoginUser.GetAutoCodeFieldsJO";
    FGetAutoCodeFieldsJOCommand->Prepare();
  }
  FGetAutoCodeFieldsJOCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetAutoCodeFieldsJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetModuleDataJO()
{
  if (FGetModuleDataJOCommand == NULL)
  {
    FGetModuleDataJOCommand = FDBXConnection->CreateCommand();
    FGetModuleDataJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetModuleDataJOCommand->Text = "TLoginUser.GetModuleDataJO";
    FGetModuleDataJOCommand->Prepare();
  }
  FGetModuleDataJOCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetModuleDataJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::RefreshGlobeVarJO()
{
  if (FRefreshGlobeVarJOCommand == NULL)
  {
    FRefreshGlobeVarJOCommand = FDBXConnection->CreateCommand();
    FRefreshGlobeVarJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FRefreshGlobeVarJOCommand->Text = "TLoginUser.RefreshGlobeVarJO";
    FRefreshGlobeVarJOCommand->Prepare();
  }
  FRefreshGlobeVarJOCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FRefreshGlobeVarJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetModuleRightJO()
{
  if (FGetModuleRightJOCommand == NULL)
  {
    FGetModuleRightJOCommand = FDBXConnection->CreateCommand();
    FGetModuleRightJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetModuleRightJOCommand->Text = "TLoginUser.GetModuleRightJO";
    FGetModuleRightJOCommand->Prepare();
  }
  FGetModuleRightJOCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetModuleRightJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONArray* __fastcall TLoginUserClient::GetDepartmentAndUsersList()
{
  if (FGetDepartmentAndUsersListCommand == NULL)
  {
    FGetDepartmentAndUsersListCommand = FDBXConnection->CreateCommand();
    FGetDepartmentAndUsersListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetDepartmentAndUsersListCommand->Text = "TLoginUser.GetDepartmentAndUsersList";
    FGetDepartmentAndUsersListCommand->Prepare();
  }
  FGetDepartmentAndUsersListCommand->ExecuteUpdate();
  TJSONArray* result = (TJSONArray*)FGetDepartmentAndUsersListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

System::UnicodeString __fastcall TLoginUserClient::UserIdToName(System::UnicodeString UserId)
{
  if (FUserIdToNameCommand == NULL)
  {
    FUserIdToNameCommand = FDBXConnection->CreateCommand();
    FUserIdToNameCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUserIdToNameCommand->Text = "TLoginUser.UserIdToName";
    FUserIdToNameCommand->Prepare();
  }
  FUserIdToNameCommand->Parameters->Parameter[0]->Value->SetWideString(UserId);
  FUserIdToNameCommand->ExecuteUpdate();
  System::UnicodeString result = FUserIdToNameCommand->Parameters->Parameter[1]->Value->GetWideString();
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetServerInfo()
{
  if (FGetServerInfoCommand == NULL)
  {
    FGetServerInfoCommand = FDBXConnection->CreateCommand();
    FGetServerInfoCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetServerInfoCommand->Text = "TLoginUser.GetServerInfo";
    FGetServerInfoCommand->Prepare();
  }
  FGetServerInfoCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetServerInfoCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetFtpSetting()
{
  if (FGetFtpSettingCommand == NULL)
  {
    FGetFtpSettingCommand = FDBXConnection->CreateCommand();
    FGetFtpSettingCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetFtpSettingCommand->Text = "TLoginUser.GetFtpSetting";
    FGetFtpSettingCommand->Prepare();
  }
  FGetFtpSettingCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetFtpSettingCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetMailSetting()
{
  if (FGetMailSettingCommand == NULL)
  {
    FGetMailSettingCommand = FDBXConnection->CreateCommand();
    FGetMailSettingCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetMailSettingCommand->Text = "TLoginUser.GetMailSetting";
    FGetMailSettingCommand->Prepare();
  }
  FGetMailSettingCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetMailSettingCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONObject* __fastcall TLoginUserClient::GetSmsSetting()
{
  if (FGetSmsSettingCommand == NULL)
  {
    FGetSmsSettingCommand = FDBXConnection->CreateCommand();
    FGetSmsSettingCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetSmsSettingCommand->Text = "TLoginUser.GetSmsSetting";
    FGetSmsSettingCommand->Prepare();
  }
  FGetSmsSettingCommand->ExecuteUpdate();
  TJSONObject* result = (TJSONObject*)FGetSmsSettingCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONArray* __fastcall TLoginUserClient::GetWorkFlowServerList()
{
  if (FGetWorkFlowServerListCommand == NULL)
  {
    FGetWorkFlowServerListCommand = FDBXConnection->CreateCommand();
    FGetWorkFlowServerListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetWorkFlowServerListCommand->Text = "TLoginUser.GetWorkFlowServerList";
    FGetWorkFlowServerListCommand->Prepare();
  }
  FGetWorkFlowServerListCommand->ExecuteUpdate();
  TJSONArray* result = (TJSONArray*)FGetWorkFlowServerListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}

TJSONArray* __fastcall TLoginUserClient::GetSmsServerList()
{
  if (FGetSmsServerListCommand == NULL)
  {
    FGetSmsServerListCommand = FDBXConnection->CreateCommand();
    FGetSmsServerListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetSmsServerListCommand->Text = "TLoginUser.GetSmsServerList";
    FGetSmsServerListCommand->Prepare();
  }
  FGetSmsServerListCommand->ExecuteUpdate();
  TJSONArray* result = (TJSONArray*)FGetSmsServerListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
  return result;
}


__fastcall  TLoginUserClient::TLoginUserClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
  FDBXConnection = ADBXConnection;
  FInstanceOwner = True;
}


__fastcall  TLoginUserClient::TLoginUserClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
  FDBXConnection = ADBXConnection;
  FInstanceOwner = AInstanceOwner;
}


__fastcall  TLoginUserClient::~TLoginUserClient()
{
  delete FGetSysConnectionStringCommand;
  delete FChangePasswordCommand;
  delete FUpdateCurUserCallbackClientIDCommand;
  delete FLogoutCommand;
  delete FSetUsersToFollowCommand;
  delete FSendMessageToFollowCommand;
  delete FSendMessageToAllCommand;
  delete FSendMessageToUserCommand;
  delete FSendMessageToDepartmentCommand;
  delete FSendMessageToGroupCommand;
  delete FSendCMDToFollowCommand;
  delete FSendCMDToAllCommand;
  delete FSendCMDToUserCommand;
  delete FSendCMDToDepartmentCommand;
  delete FSendCMDToGroupCommand;
  delete FSendMgrMessageCommand;
  delete FGetAccListCommand;
  delete FGetCurUserInfoCommand;
  delete FListOfLoginUserCommand;
  delete FListOfAllLoginUserCommand;
  delete FGetClassListCommand;
  delete FGetAutoCodeFieldsCommand;
  delete FGetModuleDataCommand;
  delete FRefreshGlobeVarCommand;
  delete FGetModuleRightCommand;
  delete FGetAccInforJSONCommand;
  delete FGetAccListJSONCommand;
  delete FGetCurUserJSONCommand;
  delete FGetUserListCommand;
  delete FGetAllUserListCommand;
  delete FGetClassListJOCommand;
  delete FGetAutoCodeFieldsJOCommand;
  delete FGetModuleDataJOCommand;
  delete FRefreshGlobeVarJOCommand;
  delete FGetModuleRightJOCommand;
  delete FGetDepartmentAndUsersListCommand;
  delete FUserIdToNameCommand;
  delete FGetServerInfoCommand;
  delete FGetFtpSettingCommand;
  delete FGetMailSettingCommand;
  delete FGetSmsSettingCommand;
  delete FGetWorkFlowServerListCommand;
  delete FGetSmsServerListCommand;
}

TStream* __fastcall TSyAccBookClient::GetAllAccBook()
{
  if (FGetAllAccBookCommand == NULL)
  {
    FGetAllAccBookCommand = FDBXConnection->CreateCommand();
    FGetAllAccBookCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAllAccBookCommand->Text = "TSyAccBook.GetAllAccBook";
    FGetAllAccBookCommand->Prepare();
}
FGetAllAccBookCommand->ExecuteUpdate();
TStream* result = FGetAllAccBookCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TSyAccBookClient::GetActiveAccBook()
{
  if (FGetActiveAccBookCommand == NULL)
  {
    FGetActiveAccBookCommand = FDBXConnection->CreateCommand();
    FGetActiveAccBookCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetActiveAccBookCommand->Text = "TSyAccBook.GetActiveAccBook";
    FGetActiveAccBookCommand->Prepare();
}
FGetActiveAccBookCommand->ExecuteUpdate();
TStream* result = FGetActiveAccBookCommand->Parameters->Parameter[0]->Value->GetStream(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TSyAccBookClient::GetAllAccBookJO()
{
  if (FGetAllAccBookJOCommand == NULL)
  {
    FGetAllAccBookJOCommand = FDBXConnection->CreateCommand();
    FGetAllAccBookJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAllAccBookJOCommand->Text = "TSyAccBook.GetAllAccBookJO";
    FGetAllAccBookJOCommand->Prepare();
}
FGetAllAccBookJOCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetAllAccBookJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TSyAccBookClient::GetActiveAccBookJO()
{
  if (FGetActiveAccBookJOCommand == NULL)
  {
    FGetActiveAccBookJOCommand = FDBXConnection->CreateCommand();
    FGetActiveAccBookJOCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetActiveAccBookJOCommand->Text = "TSyAccBook.GetActiveAccBookJO";
    FGetActiveAccBookJOCommand->Prepare();
}
FGetActiveAccBookJOCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetActiveAccBookJOCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}


__fastcall  TSyAccBookClient::TSyAccBookClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TSyAccBookClient::TSyAccBookClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TSyAccBookClient::~TSyAccBookClient()
{
  delete FGetAllAccBookCommand;
  delete FGetActiveAccBookCommand;
  delete FGetAllAccBookJOCommand;
  delete FGetActiveAccBookJOCommand;
}

TStream* __fastcall TServerDataSetClient::GetInitResult(System::UnicodeString ClassCode)
{
  if (FGetInitResultCommand == NULL)
  {
    FGetInitResultCommand = FDBXConnection->CreateCommand();
    FGetInitResultCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetInitResultCommand->Text = "TServerDataSet.GetInitResult";
    FGetInitResultCommand->Prepare();
}
FGetInitResultCommand->Parameters->Parameter[0]->Value->SetWideString(ClassCode);
FGetInitResultCommand->ExecuteUpdate();
TStream* result = FGetInitResultCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Open(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FOpenCommand == NULL)
  {
    FOpenCommand = FDBXConnection->CreateCommand();
    FOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FOpenCommand->Text = "TServerDataSet.Open";
    FOpenCommand->Prepare();
}
FOpenCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FOpenCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FOpenCommand->ExecuteUpdate();
TStream* result = FOpenCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Update(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FUpdateCommand == NULL)
  {
    FUpdateCommand = FDBXConnection->CreateCommand();
    FUpdateCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateCommand->Text = "TServerDataSet.Update";
    FUpdateCommand->Prepare();
}
FUpdateCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FUpdateCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FUpdateCommand->ExecuteUpdate();
TStream* result = FUpdateCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Delete(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FDeleteCommand == NULL)
  {
    FDeleteCommand = FDBXConnection->CreateCommand();
    FDeleteCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDeleteCommand->Text = "TServerDataSet.Delete";
    FDeleteCommand->Prepare();
}
FDeleteCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FDeleteCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FDeleteCommand->ExecuteUpdate();
TStream* result = FDeleteCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::DeleteDetail(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FDeleteDetailCommand == NULL)
  {
    FDeleteDetailCommand = FDBXConnection->CreateCommand();
    FDeleteDetailCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDeleteDetailCommand->Text = "TServerDataSet.DeleteDetail";
    FDeleteDetailCommand->Prepare();
}
FDeleteDetailCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FDeleteDetailCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FDeleteDetailCommand->ExecuteUpdate();
TStream* result = FDeleteDetailCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::CmdExec(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FCmdExecCommand == NULL)
  {
    FCmdExecCommand = FDBXConnection->CreateCommand();
    FCmdExecCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCmdExecCommand->Text = "TServerDataSet.CmdExec";
    FCmdExecCommand->Prepare();
}
FCmdExecCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FCmdExecCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCmdExecCommand->ExecuteUpdate();
TStream* result = FCmdExecCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Check(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FCheckCommand == NULL)
  {
    FCheckCommand = FDBXConnection->CreateCommand();
    FCheckCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCheckCommand->Text = "TServerDataSet.Check";
    FCheckCommand->Prepare();
}
FCheckCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FCheckCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCheckCommand->ExecuteUpdate();
TStream* result = FCheckCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::CloseOff(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FCloseOffCommand == NULL)
  {
    FCloseOffCommand = FDBXConnection->CreateCommand();
    FCloseOffCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCloseOffCommand->Text = "TServerDataSet.CloseOff";
    FCloseOffCommand->Prepare();
}
FCloseOffCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FCloseOffCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCloseOffCommand->ExecuteUpdate();
TStream* result = FCloseOffCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Revoke(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FRevokeCommand == NULL)
  {
    FRevokeCommand = FDBXConnection->CreateCommand();
    FRevokeCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FRevokeCommand->Text = "TServerDataSet.Revoke";
    FRevokeCommand->Prepare();
}
FRevokeCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FRevokeCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FRevokeCommand->ExecuteUpdate();
TStream* result = FRevokeCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::Tally(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FTallyCommand == NULL)
  {
    FTallyCommand = FDBXConnection->CreateCommand();
    FTallyCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FTallyCommand->Text = "TServerDataSet.Tally";
    FTallyCommand->Prepare();
}
FTallyCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FTallyCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FTallyCommand->ExecuteUpdate();
TStream* result = FTallyCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::ExecSQL(TStream* Stream)
{
  if (FExecSQLCommand == NULL)
  {
    FExecSQLCommand = FDBXConnection->CreateCommand();
    FExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FExecSQLCommand->Text = "TServerDataSet.ExecSQL";
    FExecSQLCommand->Prepare();
}
FExecSQLCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FExecSQLCommand->ExecuteUpdate();
TStream* result = FExecSQLCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::ValidateData(TStream* Stream, System::UnicodeString ClassCode)
{
  if (FValidateDataCommand == NULL)
  {
    FValidateDataCommand = FDBXConnection->CreateCommand();
    FValidateDataCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FValidateDataCommand->Text = "TServerDataSet.ValidateData";
    FValidateDataCommand->Prepare();
}
FValidateDataCommand->Parameters->Parameter[0]->Value->SetStream(Stream, FInstanceOwner);
FValidateDataCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FValidateDataCommand->ExecuteUpdate();
TStream* result = FValidateDataCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::QueryOpen(System::UnicodeString SQLText)
{
  if (FQueryOpenCommand == NULL)
  {
    FQueryOpenCommand = FDBXConnection->CreateCommand();
    FQueryOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryOpenCommand->Text = "TServerDataSet.QueryOpen";
    FQueryOpenCommand->Prepare();
}
FQueryOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryOpenCommand->ExecuteUpdate();
TStream* result = FQueryOpenCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::QueryExecSQL(System::UnicodeString SQLText)
{
  if (FQueryExecSQLCommand == NULL)
  {
    FQueryExecSQLCommand = FDBXConnection->CreateCommand();
    FQueryExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryExecSQLCommand->Text = "TServerDataSet.QueryExecSQL";
    FQueryExecSQLCommand->Prepare();
}
FQueryExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryExecSQLCommand->ExecuteUpdate();
TStream* result = FQueryExecSQLCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::QuerySysOpen(System::UnicodeString SQLText)
{
  if (FQuerySysOpenCommand == NULL)
  {
    FQuerySysOpenCommand = FDBXConnection->CreateCommand();
    FQuerySysOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysOpenCommand->Text = "TServerDataSet.QuerySysOpen";
    FQuerySysOpenCommand->Prepare();
}
FQuerySysOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysOpenCommand->ExecuteUpdate();
TStream* result = FQuerySysOpenCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TServerDataSetClient::QuerySysExecSQL(System::UnicodeString SQLText)
{
  if (FQuerySysExecSQLCommand == NULL)
  {
    FQuerySysExecSQLCommand = FDBXConnection->CreateCommand();
    FQuerySysExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysExecSQLCommand->Text = "TServerDataSet.QuerySysExecSQL";
    FQuerySysExecSQLCommand->Prepare();
}
FQuerySysExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysExecSQLCommand->ExecuteUpdate();
TStream* result = FQuerySysExecSQLCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}


__fastcall  TServerDataSetClient::TServerDataSetClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TServerDataSetClient::TServerDataSetClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TServerDataSetClient::~TServerDataSetClient()
{
  delete FGetInitResultCommand;
  delete FOpenCommand;
  delete FUpdateCommand;
  delete FDeleteCommand;
  delete FDeleteDetailCommand;
  delete FCmdExecCommand;
  delete FCheckCommand;
  delete FCloseOffCommand;
  delete FRevokeCommand;
  delete FTallyCommand;
  delete FExecSQLCommand;
  delete FValidateDataCommand;
  delete FQueryOpenCommand;
  delete FQueryExecSQLCommand;
  delete FQuerySysOpenCommand;
  delete FQuerySysExecSQLCommand;
}

TStream* __fastcall TServerQueryClient::QueryOpen(System::UnicodeString SQLText)
{
  if (FQueryOpenCommand == NULL)
  {
    FQueryOpenCommand = FDBXConnection->CreateCommand();
    FQueryOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryOpenCommand->Text = "TServerQuery.QueryOpen";
    FQueryOpenCommand->Prepare();
}
FQueryOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryOpenCommand->ExecuteUpdate();
TStream* result = FQueryOpenCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

int __fastcall TServerQueryClient::QueryExecSQL(System::UnicodeString SQLText)
{
  if (FQueryExecSQLCommand == NULL)
  {
    FQueryExecSQLCommand = FDBXConnection->CreateCommand();
    FQueryExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryExecSQLCommand->Text = "TServerQuery.QueryExecSQL";
    FQueryExecSQLCommand->Prepare();
}
FQueryExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryExecSQLCommand->ExecuteUpdate();
int result = FQueryExecSQLCommand->Parameters->Parameter[1]->Value->GetInt32();
return result;
}

int __fastcall TServerQueryClient::ExecSQL(System::UnicodeString SQLText, TParams* Parameters)
{
  if (FExecSQLCommand == NULL)
  {
    FExecSQLCommand = FDBXConnection->CreateCommand();
    FExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FExecSQLCommand->Text = "TServerQuery.ExecSQL";
    FExecSQLCommand->Prepare();
}
FExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FExecSQLCommand->Parameters->Parameter[1]->Value->SetDBXReader(new TDBXParamsReader(Parameters, FInstanceOwner), True);
FExecSQLCommand->ExecuteUpdate();
int result = FExecSQLCommand->Parameters->Parameter[2]->Value->GetInt32();
return result;
}

TStream* __fastcall TServerQueryClient::QuerySysOpen(System::UnicodeString SQLText)
{
  if (FQuerySysOpenCommand == NULL)
  {
    FQuerySysOpenCommand = FDBXConnection->CreateCommand();
    FQuerySysOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysOpenCommand->Text = "TServerQuery.QuerySysOpen";
    FQuerySysOpenCommand->Prepare();
}
FQuerySysOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysOpenCommand->ExecuteUpdate();
TStream* result = FQuerySysOpenCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

int __fastcall TServerQueryClient::QuerySysExecSQL(System::UnicodeString SQLText)
{
  if (FQuerySysExecSQLCommand == NULL)
  {
    FQuerySysExecSQLCommand = FDBXConnection->CreateCommand();
    FQuerySysExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysExecSQLCommand->Text = "TServerQuery.QuerySysExecSQL";
    FQuerySysExecSQLCommand->Prepare();
}
FQuerySysExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysExecSQLCommand->ExecuteUpdate();
int result = FQuerySysExecSQLCommand->Parameters->Parameter[1]->Value->GetInt32();
return result;
}

int __fastcall TServerQueryClient::SysExecSQL(System::UnicodeString SQLText, TParams* Parameters)
{
  if (FSysExecSQLCommand == NULL)
  {
    FSysExecSQLCommand = FDBXConnection->CreateCommand();
    FSysExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSysExecSQLCommand->Text = "TServerQuery.SysExecSQL";
    FSysExecSQLCommand->Prepare();
}
FSysExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FSysExecSQLCommand->Parameters->Parameter[1]->Value->SetDBXReader(new TDBXParamsReader(Parameters, FInstanceOwner), True);
FSysExecSQLCommand->ExecuteUpdate();
int result = FSysExecSQLCommand->Parameters->Parameter[2]->Value->GetInt32();
return result;
}


__fastcall  TServerQueryClient::TServerQueryClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TServerQueryClient::TServerQueryClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TServerQueryClient::~TServerQueryClient()
{
  delete FQueryOpenCommand;
  delete FQueryExecSQLCommand;
  delete FExecSQLCommand;
  delete FQuerySysOpenCommand;
  delete FQuerySysExecSQLCommand;
  delete FSysExecSQLCommand;
}

TJSONObject* __fastcall TJServerDataSetClient::GetInitResult(System::UnicodeString ClassCode)
{
  if (FGetInitResultCommand == NULL)
  {
    FGetInitResultCommand = FDBXConnection->CreateCommand();
    FGetInitResultCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetInitResultCommand->Text = "TJServerDataSet.GetInitResult";
    FGetInitResultCommand->Prepare();
}
FGetInitResultCommand->Parameters->Parameter[0]->Value->SetWideString(ClassCode);
FGetInitResultCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetInitResultCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Open(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FOpenCommand == NULL)
  {
    FOpenCommand = FDBXConnection->CreateCommand();
    FOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FOpenCommand->Text = "TJServerDataSet.Open";
    FOpenCommand->Prepare();
}
FOpenCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FOpenCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FOpenCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FOpenCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Update(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FUpdateCommand == NULL)
  {
    FUpdateCommand = FDBXConnection->CreateCommand();
    FUpdateCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateCommand->Text = "TJServerDataSet.Update";
    FUpdateCommand->Prepare();
}
FUpdateCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FUpdateCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FUpdateCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FUpdateCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Delete(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FDeleteCommand == NULL)
  {
    FDeleteCommand = FDBXConnection->CreateCommand();
    FDeleteCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDeleteCommand->Text = "TJServerDataSet.Delete";
    FDeleteCommand->Prepare();
}
FDeleteCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FDeleteCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FDeleteCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FDeleteCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::DeleteDetail(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FDeleteDetailCommand == NULL)
  {
    FDeleteDetailCommand = FDBXConnection->CreateCommand();
    FDeleteDetailCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDeleteDetailCommand->Text = "TJServerDataSet.DeleteDetail";
    FDeleteDetailCommand->Prepare();
}
FDeleteDetailCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FDeleteDetailCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FDeleteDetailCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FDeleteDetailCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::ValidateData(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FValidateDataCommand == NULL)
  {
    FValidateDataCommand = FDBXConnection->CreateCommand();
    FValidateDataCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FValidateDataCommand->Text = "TJServerDataSet.ValidateData";
    FValidateDataCommand->Prepare();
}
FValidateDataCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FValidateDataCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FValidateDataCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FValidateDataCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::CmdExec(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FCmdExecCommand == NULL)
  {
    FCmdExecCommand = FDBXConnection->CreateCommand();
    FCmdExecCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCmdExecCommand->Text = "TJServerDataSet.CmdExec";
    FCmdExecCommand->Prepare();
}
FCmdExecCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FCmdExecCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCmdExecCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FCmdExecCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Check(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FCheckCommand == NULL)
  {
    FCheckCommand = FDBXConnection->CreateCommand();
    FCheckCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCheckCommand->Text = "TJServerDataSet.Check";
    FCheckCommand->Prepare();
}
FCheckCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FCheckCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCheckCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FCheckCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::CloseOff(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FCloseOffCommand == NULL)
  {
    FCloseOffCommand = FDBXConnection->CreateCommand();
    FCloseOffCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FCloseOffCommand->Text = "TJServerDataSet.CloseOff";
    FCloseOffCommand->Prepare();
}
FCloseOffCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FCloseOffCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FCloseOffCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FCloseOffCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Revoke(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FRevokeCommand == NULL)
  {
    FRevokeCommand = FDBXConnection->CreateCommand();
    FRevokeCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FRevokeCommand->Text = "TJServerDataSet.Revoke";
    FRevokeCommand->Prepare();
}
FRevokeCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FRevokeCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FRevokeCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FRevokeCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::Tally(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FTallyCommand == NULL)
  {
    FTallyCommand = FDBXConnection->CreateCommand();
    FTallyCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FTallyCommand->Text = "TJServerDataSet.Tally";
    FTallyCommand->Prepare();
}
FTallyCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FTallyCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FTallyCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FTallyCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::ExecSQL(TJSONObject* Json)
{
  if (FExecSQLCommand == NULL)
  {
    FExecSQLCommand = FDBXConnection->CreateCommand();
    FExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FExecSQLCommand->Text = "TJServerDataSet.ExecSQL";
    FExecSQLCommand->Prepare();
}
FExecSQLCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FExecSQLCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FExecSQLCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::QueryOpen(System::UnicodeString SQLText)
{
  if (FQueryOpenCommand == NULL)
  {
    FQueryOpenCommand = FDBXConnection->CreateCommand();
    FQueryOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryOpenCommand->Text = "TJServerDataSet.QueryOpen";
    FQueryOpenCommand->Prepare();
}
FQueryOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryOpenCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQueryOpenCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::QueryExecSQL(System::UnicodeString SQLText)
{
  if (FQueryExecSQLCommand == NULL)
  {
    FQueryExecSQLCommand = FDBXConnection->CreateCommand();
    FQueryExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryExecSQLCommand->Text = "TJServerDataSet.QueryExecSQL";
    FQueryExecSQLCommand->Prepare();
}
FQueryExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryExecSQLCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQueryExecSQLCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::QuerySysOpen(System::UnicodeString SQLText)
{
  if (FQuerySysOpenCommand == NULL)
  {
    FQuerySysOpenCommand = FDBXConnection->CreateCommand();
    FQuerySysOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysOpenCommand->Text = "TJServerDataSet.QuerySysOpen";
    FQuerySysOpenCommand->Prepare();
}
FQuerySysOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysOpenCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQuerySysOpenCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerDataSetClient::QuerySysExecSQL(System::UnicodeString SQLText)
{
  if (FQuerySysExecSQLCommand == NULL)
  {
    FQuerySysExecSQLCommand = FDBXConnection->CreateCommand();
    FQuerySysExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysExecSQLCommand->Text = "TJServerDataSet.QuerySysExecSQL";
    FQuerySysExecSQLCommand->Prepare();
}
FQuerySysExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysExecSQLCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQuerySysExecSQLCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}


__fastcall  TJServerDataSetClient::TJServerDataSetClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TJServerDataSetClient::TJServerDataSetClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TJServerDataSetClient::~TJServerDataSetClient()
{
  delete FGetInitResultCommand;
  delete FOpenCommand;
  delete FUpdateCommand;
  delete FDeleteCommand;
  delete FDeleteDetailCommand;
  delete FValidateDataCommand;
  delete FCmdExecCommand;
  delete FCheckCommand;
  delete FCloseOffCommand;
  delete FRevokeCommand;
  delete FTallyCommand;
  delete FExecSQLCommand;
  delete FQueryOpenCommand;
  delete FQueryExecSQLCommand;
  delete FQuerySysOpenCommand;
  delete FQuerySysExecSQLCommand;
}

TJSONObject* __fastcall TJServerQueryClient::QueryOpen(System::UnicodeString SQLText)
{
  if (FQueryOpenCommand == NULL)
  {
    FQueryOpenCommand = FDBXConnection->CreateCommand();
    FQueryOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryOpenCommand->Text = "TJServerQuery.QueryOpen";
    FQueryOpenCommand->Prepare();
}
FQueryOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryOpenCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQueryOpenCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TJServerQueryClient::QueryUpdate(TJSONObject* Json, System::UnicodeString ClassCode)
{
  if (FQueryUpdateCommand == NULL)
  {
    FQueryUpdateCommand = FDBXConnection->CreateCommand();
    FQueryUpdateCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryUpdateCommand->Text = "TJServerQuery.QueryUpdate";
    FQueryUpdateCommand->Prepare();
}
FQueryUpdateCommand->Parameters->Parameter[0]->Value->SetJSONValue(Json, FInstanceOwner);
FQueryUpdateCommand->Parameters->Parameter[1]->Value->SetWideString(ClassCode);
FQueryUpdateCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQueryUpdateCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

int __fastcall TJServerQueryClient::QueryExecSQL(System::UnicodeString SQLText)
{
  if (FQueryExecSQLCommand == NULL)
  {
    FQueryExecSQLCommand = FDBXConnection->CreateCommand();
    FQueryExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQueryExecSQLCommand->Text = "TJServerQuery.QueryExecSQL";
    FQueryExecSQLCommand->Prepare();
}
FQueryExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQueryExecSQLCommand->ExecuteUpdate();
int result = FQueryExecSQLCommand->Parameters->Parameter[1]->Value->GetInt32();
return result;
}

TJSONObject* __fastcall TJServerQueryClient::QuerySysOpen(System::UnicodeString SQLText)
{
  if (FQuerySysOpenCommand == NULL)
  {
    FQuerySysOpenCommand = FDBXConnection->CreateCommand();
    FQuerySysOpenCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysOpenCommand->Text = "TJServerQuery.QuerySysOpen";
    FQuerySysOpenCommand->Prepare();
}
FQuerySysOpenCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysOpenCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FQuerySysOpenCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

int __fastcall TJServerQueryClient::QuerySysExecSQL(System::UnicodeString SQLText)
{
  if (FQuerySysExecSQLCommand == NULL)
  {
    FQuerySysExecSQLCommand = FDBXConnection->CreateCommand();
    FQuerySysExecSQLCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FQuerySysExecSQLCommand->Text = "TJServerQuery.QuerySysExecSQL";
    FQuerySysExecSQLCommand->Prepare();
}
FQuerySysExecSQLCommand->Parameters->Parameter[0]->Value->SetWideString(SQLText);
FQuerySysExecSQLCommand->ExecuteUpdate();
int result = FQuerySysExecSQLCommand->Parameters->Parameter[1]->Value->GetInt32();
return result;
}


__fastcall  TJServerQueryClient::TJServerQueryClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TJServerQueryClient::TJServerQueryClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TJServerQueryClient::~TJServerQueryClient()
{
  delete FQueryOpenCommand;
  delete FQueryUpdateCommand;
  delete FQueryExecSQLCommand;
  delete FQuerySysOpenCommand;
  delete FQuerySysExecSQLCommand;
}

System::UnicodeString __fastcall TWorkFlowServerClient::ServerWorkFlowStartJSON(TJSONObject* jsonValue)
{
  if (FServerWorkFlowStartJSONCommand == NULL)
  {
    FServerWorkFlowStartJSONCommand = FDBXConnection->CreateCommand();
    FServerWorkFlowStartJSONCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FServerWorkFlowStartJSONCommand->Text = "TWorkFlowServer.ServerWorkFlowStartJSON";
    FServerWorkFlowStartJSONCommand->Prepare();
}
FServerWorkFlowStartJSONCommand->Parameters->Parameter[0]->Value->SetJSONValue(jsonValue, FInstanceOwner);
FServerWorkFlowStartJSONCommand->ExecuteUpdate();
System::UnicodeString result = FServerWorkFlowStartJSONCommand->Parameters->Parameter[1]->Value->GetWideString();
return result;
}

System::UnicodeString __fastcall TWorkFlowServerClient::ServerWorkFlowStart(System::UnicodeString FlowDefID, System::UnicodeString Subject, System::UnicodeString Content, System::UnicodeString BusWindow, System::UnicodeString BusCode, int Rank, System::UnicodeString PrjID, System::UnicodeString Report)
{
  if (FServerWorkFlowStartCommand == NULL)
  {
    FServerWorkFlowStartCommand = FDBXConnection->CreateCommand();
    FServerWorkFlowStartCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FServerWorkFlowStartCommand->Text = "TWorkFlowServer.ServerWorkFlowStart";
    FServerWorkFlowStartCommand->Prepare();
}
FServerWorkFlowStartCommand->Parameters->Parameter[0]->Value->SetWideString(FlowDefID);
FServerWorkFlowStartCommand->Parameters->Parameter[1]->Value->SetWideString(Subject);
FServerWorkFlowStartCommand->Parameters->Parameter[2]->Value->SetWideString(Content);
FServerWorkFlowStartCommand->Parameters->Parameter[3]->Value->SetWideString(BusWindow);
FServerWorkFlowStartCommand->Parameters->Parameter[4]->Value->SetWideString(BusCode);
FServerWorkFlowStartCommand->Parameters->Parameter[5]->Value->SetInt32(Rank);
FServerWorkFlowStartCommand->Parameters->Parameter[6]->Value->SetWideString(PrjID);
FServerWorkFlowStartCommand->Parameters->Parameter[7]->Value->SetWideString(Report);
FServerWorkFlowStartCommand->ExecuteUpdate();
System::UnicodeString result = FServerWorkFlowStartCommand->Parameters->Parameter[8]->Value->GetWideString();
return result;
}

bool __fastcall TWorkFlowServerClient::ServerWorkFlowExecJSON(TJSONObject* jsonValue)
{
  if (FServerWorkFlowExecJSONCommand == NULL)
  {
    FServerWorkFlowExecJSONCommand = FDBXConnection->CreateCommand();
    FServerWorkFlowExecJSONCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FServerWorkFlowExecJSONCommand->Text = "TWorkFlowServer.ServerWorkFlowExecJSON";
    FServerWorkFlowExecJSONCommand->Prepare();
}
FServerWorkFlowExecJSONCommand->Parameters->Parameter[0]->Value->SetJSONValue(jsonValue, FInstanceOwner);
FServerWorkFlowExecJSONCommand->ExecuteUpdate();
bool result = FServerWorkFlowExecJSONCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TWorkFlowServerClient::ServerWorkFlowExec(System::UnicodeString FlowDefID, System::UnicodeString WorkInsID, System::UnicodeString TaskInsID, System::UnicodeString TskStatus, System::UnicodeString TskComments, TJSONArray* VariableList)
{
  if (FServerWorkFlowExecCommand == NULL)
  {
    FServerWorkFlowExecCommand = FDBXConnection->CreateCommand();
    FServerWorkFlowExecCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FServerWorkFlowExecCommand->Text = "TWorkFlowServer.ServerWorkFlowExec";
    FServerWorkFlowExecCommand->Prepare();
}
FServerWorkFlowExecCommand->Parameters->Parameter[0]->Value->SetWideString(FlowDefID);
FServerWorkFlowExecCommand->Parameters->Parameter[1]->Value->SetWideString(WorkInsID);
FServerWorkFlowExecCommand->Parameters->Parameter[2]->Value->SetWideString(TaskInsID);
FServerWorkFlowExecCommand->Parameters->Parameter[3]->Value->SetWideString(TskStatus);
FServerWorkFlowExecCommand->Parameters->Parameter[4]->Value->SetWideString(TskComments);
FServerWorkFlowExecCommand->Parameters->Parameter[5]->Value->SetJSONValue(VariableList, FInstanceOwner);
FServerWorkFlowExecCommand->ExecuteUpdate();
bool result = FServerWorkFlowExecCommand->Parameters->Parameter[6]->Value->GetBoolean();
return result;
}

TJSONObject* __fastcall TWorkFlowServerClient::GetTaskList(System::UnicodeString AUserID, int ACompletedState, System::UnicodeString AOtherCondition)
{
  if (FGetTaskListCommand == NULL)
  {
    FGetTaskListCommand = FDBXConnection->CreateCommand();
    FGetTaskListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetTaskListCommand->Text = "TWorkFlowServer.GetTaskList";
    FGetTaskListCommand->Prepare();
}
FGetTaskListCommand->Parameters->Parameter[0]->Value->SetWideString(AUserID);
FGetTaskListCommand->Parameters->Parameter[1]->Value->SetInt32(ACompletedState);
FGetTaskListCommand->Parameters->Parameter[2]->Value->SetWideString(AOtherCondition);
FGetTaskListCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetTaskListCommand->Parameters->Parameter[3]->Value->GetJSONValue(FInstanceOwner);
return result;
}

System::UnicodeString __fastcall TWorkFlowServerClient::GetLogList(int ALogFilterType, System::UnicodeString AFilterKey)
{
  if (FGetLogListCommand == NULL)
  {
    FGetLogListCommand = FDBXConnection->CreateCommand();
    FGetLogListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetLogListCommand->Text = "TWorkFlowServer.GetLogList";
    FGetLogListCommand->Prepare();
}
FGetLogListCommand->Parameters->Parameter[0]->Value->SetInt32(ALogFilterType);
FGetLogListCommand->Parameters->Parameter[1]->Value->SetWideString(AFilterKey);
FGetLogListCommand->ExecuteUpdate();
System::UnicodeString result = FGetLogListCommand->Parameters->Parameter[2]->Value->GetWideString();
return result;
}

System::UnicodeString __fastcall TWorkFlowServerClient::GetDiagram(int ADiagramType, System::UnicodeString ADiagramKey)
{
  if (FGetDiagramCommand == NULL)
  {
    FGetDiagramCommand = FDBXConnection->CreateCommand();
    FGetDiagramCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetDiagramCommand->Text = "TWorkFlowServer.GetDiagram";
    FGetDiagramCommand->Prepare();
}
FGetDiagramCommand->Parameters->Parameter[0]->Value->SetInt32(ADiagramType);
FGetDiagramCommand->Parameters->Parameter[1]->Value->SetWideString(ADiagramKey);
FGetDiagramCommand->ExecuteUpdate();
System::UnicodeString result = FGetDiagramCommand->Parameters->Parameter[2]->Value->GetWideString();
return result;
}

TJSONObject* __fastcall TWorkFlowServerClient::GetTaskdefList(System::UnicodeString TaskInsID)
{
  if (FGetTaskdefListCommand == NULL)
  {
    FGetTaskdefListCommand = FDBXConnection->CreateCommand();
    FGetTaskdefListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetTaskdefListCommand->Text = "TWorkFlowServer.GetTaskdefList";
    FGetTaskdefListCommand->Prepare();
}
FGetTaskdefListCommand->Parameters->Parameter[0]->Value->SetWideString(TaskInsID);
FGetTaskdefListCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetTaskdefListCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONObject* __fastcall TWorkFlowServerClient::GetVariableList(System::UnicodeString TaskInsID)
{
  if (FGetVariableListCommand == NULL)
  {
    FGetVariableListCommand = FDBXConnection->CreateCommand();
    FGetVariableListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetVariableListCommand->Text = "TWorkFlowServer.GetVariableList";
    FGetVariableListCommand->Prepare();
}
FGetVariableListCommand->Parameters->Parameter[0]->Value->SetWideString(TaskInsID);
FGetVariableListCommand->ExecuteUpdate();
TJSONObject* result = (TJSONObject*)FGetVariableListCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TStream* __fastcall TWorkFlowServerClient::GetVariableListDS(System::UnicodeString TaskInsID)
{
  if (FGetVariableListDSCommand == NULL)
  {
    FGetVariableListDSCommand = FDBXConnection->CreateCommand();
    FGetVariableListDSCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetVariableListDSCommand->Text = "TWorkFlowServer.GetVariableListDS";
    FGetVariableListDSCommand->Prepare();
}
FGetVariableListDSCommand->Parameters->Parameter[0]->Value->SetWideString(TaskInsID);
FGetVariableListDSCommand->ExecuteUpdate();
TStream* result = FGetVariableListDSCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TWorkFlowServerClient::GetTaskdefListDS(System::UnicodeString TaskInsID)
{
  if (FGetTaskdefListDSCommand == NULL)
  {
    FGetTaskdefListDSCommand = FDBXConnection->CreateCommand();
    FGetTaskdefListDSCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetTaskdefListDSCommand->Text = "TWorkFlowServer.GetTaskdefListDS";
    FGetTaskdefListDSCommand->Prepare();
}
FGetTaskdefListDSCommand->Parameters->Parameter[0]->Value->SetWideString(TaskInsID);
FGetTaskdefListDSCommand->ExecuteUpdate();
TStream* result = FGetTaskdefListDSCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TWorkFlowServerClient::GetWorkFlowChart(System::UnicodeString FlowID, bool isDef)
{
  if (FGetWorkFlowChartCommand == NULL)
  {
    FGetWorkFlowChartCommand = FDBXConnection->CreateCommand();
    FGetWorkFlowChartCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetWorkFlowChartCommand->Text = "TWorkFlowServer.GetWorkFlowChart";
    FGetWorkFlowChartCommand->Prepare();
}
FGetWorkFlowChartCommand->Parameters->Parameter[0]->Value->SetWideString(FlowID);
FGetWorkFlowChartCommand->Parameters->Parameter[1]->Value->SetBoolean(isDef);
FGetWorkFlowChartCommand->ExecuteUpdate();
TStream* result = FGetWorkFlowChartCommand->Parameters->Parameter[2]->Value->GetStream(FInstanceOwner);
return result;
}

TStream* __fastcall TWorkFlowServerClient::GetTaskListDS(System::UnicodeString AUserID, int ACompletedState, System::UnicodeString AOtherCondition)
{
  if (FGetTaskListDSCommand == NULL)
  {
    FGetTaskListDSCommand = FDBXConnection->CreateCommand();
    FGetTaskListDSCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetTaskListDSCommand->Text = "TWorkFlowServer.GetTaskListDS";
    FGetTaskListDSCommand->Prepare();
}
FGetTaskListDSCommand->Parameters->Parameter[0]->Value->SetWideString(AUserID);
FGetTaskListDSCommand->Parameters->Parameter[1]->Value->SetInt32(ACompletedState);
FGetTaskListDSCommand->Parameters->Parameter[2]->Value->SetWideString(AOtherCondition);
FGetTaskListDSCommand->ExecuteUpdate();
TStream* result = FGetTaskListDSCommand->Parameters->Parameter[3]->Value->GetStream(FInstanceOwner);
return result;
}

bool __fastcall TWorkFlowServerClient::TaskCanUpdate(System::UnicodeString TaskInsID)
{
  if (FTaskCanUpdateCommand == NULL)
  {
    FTaskCanUpdateCommand = FDBXConnection->CreateCommand();
    FTaskCanUpdateCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FTaskCanUpdateCommand->Text = "TWorkFlowServer.TaskCanUpdate";
    FTaskCanUpdateCommand->Prepare();
}
FTaskCanUpdateCommand->Parameters->Parameter[0]->Value->SetWideString(TaskInsID);
FTaskCanUpdateCommand->ExecuteUpdate();
bool result = FTaskCanUpdateCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}


__fastcall  TWorkFlowServerClient::TWorkFlowServerClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TWorkFlowServerClient::TWorkFlowServerClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TWorkFlowServerClient::~TWorkFlowServerClient()
{
  delete FServerWorkFlowStartJSONCommand;
  delete FServerWorkFlowStartCommand;
  delete FServerWorkFlowExecJSONCommand;
  delete FServerWorkFlowExecCommand;
  delete FGetTaskListCommand;
  delete FGetLogListCommand;
  delete FGetDiagramCommand;
  delete FGetTaskdefListCommand;
  delete FGetVariableListCommand;
  delete FGetVariableListDSCommand;
  delete FGetTaskdefListDSCommand;
  delete FGetWorkFlowChartCommand;
  delete FGetTaskListDSCommand;
  delete FTaskCanUpdateCommand;
}

void __fastcall TMessageManageClient::NewMessageGroup(System::UnicodeString GroupName, System::UnicodeString Description)
{
  if (FNewMessageGroupCommand == NULL)
  {
    FNewMessageGroupCommand = FDBXConnection->CreateCommand();
    FNewMessageGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FNewMessageGroupCommand->Text = "TMessageManage.NewMessageGroup";
    FNewMessageGroupCommand->Prepare();
}
FNewMessageGroupCommand->Parameters->Parameter[0]->Value->SetWideString(GroupName);
FNewMessageGroupCommand->Parameters->Parameter[1]->Value->SetWideString(Description);
FNewMessageGroupCommand->ExecuteUpdate();
}

void __fastcall TMessageManageClient::DelMessageGroup(System::UnicodeString GroupID)
{
  if (FDelMessageGroupCommand == NULL)
  {
    FDelMessageGroupCommand = FDBXConnection->CreateCommand();
    FDelMessageGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDelMessageGroupCommand->Text = "TMessageManage.DelMessageGroup";
    FDelMessageGroupCommand->Prepare();
}
FDelMessageGroupCommand->Parameters->Parameter[0]->Value->SetWideString(GroupID);
FDelMessageGroupCommand->ExecuteUpdate();
}

void __fastcall TMessageManageClient::UpdateMessageGroup(System::UnicodeString GroupID, System::UnicodeString GroupName, System::UnicodeString Description)
{
  if (FUpdateMessageGroupCommand == NULL)
  {
    FUpdateMessageGroupCommand = FDBXConnection->CreateCommand();
    FUpdateMessageGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateMessageGroupCommand->Text = "TMessageManage.UpdateMessageGroup";
    FUpdateMessageGroupCommand->Prepare();
}
FUpdateMessageGroupCommand->Parameters->Parameter[0]->Value->SetWideString(GroupID);
FUpdateMessageGroupCommand->Parameters->Parameter[1]->Value->SetWideString(GroupName);
FUpdateMessageGroupCommand->Parameters->Parameter[2]->Value->SetWideString(Description);
FUpdateMessageGroupCommand->ExecuteUpdate();
}

void __fastcall TMessageManageClient::NewMessageMember(System::UnicodeString GroupID, System::UnicodeString GroupMemberID, int IsAdmin)
{
  if (FNewMessageMemberCommand == NULL)
  {
    FNewMessageMemberCommand = FDBXConnection->CreateCommand();
    FNewMessageMemberCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FNewMessageMemberCommand->Text = "TMessageManage.NewMessageMember";
    FNewMessageMemberCommand->Prepare();
}
FNewMessageMemberCommand->Parameters->Parameter[0]->Value->SetWideString(GroupID);
FNewMessageMemberCommand->Parameters->Parameter[1]->Value->SetWideString(GroupMemberID);
FNewMessageMemberCommand->Parameters->Parameter[2]->Value->SetInt32(IsAdmin);
FNewMessageMemberCommand->ExecuteUpdate();
}

void __fastcall TMessageManageClient::UpdateMessageMember(System::UnicodeString GroupID, System::UnicodeString GroupMemberID, int IsAdmin)
{
  if (FUpdateMessageMemberCommand == NULL)
  {
    FUpdateMessageMemberCommand = FDBXConnection->CreateCommand();
    FUpdateMessageMemberCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateMessageMemberCommand->Text = "TMessageManage.UpdateMessageMember";
    FUpdateMessageMemberCommand->Prepare();
}
FUpdateMessageMemberCommand->Parameters->Parameter[0]->Value->SetWideString(GroupID);
FUpdateMessageMemberCommand->Parameters->Parameter[1]->Value->SetWideString(GroupMemberID);
FUpdateMessageMemberCommand->Parameters->Parameter[2]->Value->SetInt32(IsAdmin);
FUpdateMessageMemberCommand->ExecuteUpdate();
}

void __fastcall TMessageManageClient::DelMessageMember(System::UnicodeString GroupID, System::UnicodeString GroupMemberID)
{
  if (FDelMessageMemberCommand == NULL)
  {
    FDelMessageMemberCommand = FDBXConnection->CreateCommand();
    FDelMessageMemberCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FDelMessageMemberCommand->Text = "TMessageManage.DelMessageMember";
    FDelMessageMemberCommand->Prepare();
}
FDelMessageMemberCommand->Parameters->Parameter[0]->Value->SetWideString(GroupID);
FDelMessageMemberCommand->Parameters->Parameter[1]->Value->SetWideString(GroupMemberID);
FDelMessageMemberCommand->ExecuteUpdate();
}

TJSONArray* __fastcall TMessageManageClient::GetMessageGroupList()
{
  if (FGetMessageGroupListCommand == NULL)
  {
    FGetMessageGroupListCommand = FDBXConnection->CreateCommand();
    FGetMessageGroupListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetMessageGroupListCommand->Text = "TMessageManage.GetMessageGroupList";
    FGetMessageGroupListCommand->Prepare();
}
FGetMessageGroupListCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetMessageGroupListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TMessageManageClient::GetGroupMemberList(System::UnicodeString GroudId)
{
  if (FGetGroupMemberListCommand == NULL)
  {
    FGetGroupMemberListCommand = FDBXConnection->CreateCommand();
    FGetGroupMemberListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetGroupMemberListCommand->Text = "TMessageManage.GetGroupMemberList";
    FGetGroupMemberListCommand->Prepare();
}
FGetGroupMemberListCommand->Parameters->Parameter[0]->Value->SetWideString(GroudId);
FGetGroupMemberListCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetGroupMemberListCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TMessageManageClient::GetAllEmployee(System::UnicodeString GroupId)
{
  if (FGetAllEmployeeCommand == NULL)
  {
    FGetAllEmployeeCommand = FDBXConnection->CreateCommand();
    FGetAllEmployeeCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetAllEmployeeCommand->Text = "TMessageManage.GetAllEmployee";
    FGetAllEmployeeCommand->Prepare();
}
FGetAllEmployeeCommand->Parameters->Parameter[0]->Value->SetWideString(GroupId);
FGetAllEmployeeCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetAllEmployeeCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

bool __fastcall TMessageManageClient::IsGroupMember(System::UnicodeString GroupId, System::UnicodeString MemberId)
{
  if (FIsGroupMemberCommand == NULL)
  {
    FIsGroupMemberCommand = FDBXConnection->CreateCommand();
    FIsGroupMemberCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FIsGroupMemberCommand->Text = "TMessageManage.IsGroupMember";
    FIsGroupMemberCommand->Prepare();
}
FIsGroupMemberCommand->Parameters->Parameter[0]->Value->SetWideString(GroupId);
FIsGroupMemberCommand->Parameters->Parameter[1]->Value->SetWideString(MemberId);
FIsGroupMemberCommand->ExecuteUpdate();
bool result = FIsGroupMemberCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

TJSONArray* __fastcall TMessageManageClient::GetUserList()
{
  if (FGetUserListCommand == NULL)
  {
    FGetUserListCommand = FDBXConnection->CreateCommand();
    FGetUserListCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetUserListCommand->Text = "TMessageManage.GetUserList";
    FGetUserListCommand->Prepare();
}
FGetUserListCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetUserListCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}

System::UnicodeString __fastcall TMessageManageClient::GetHTMLEscapedString(System::UnicodeString Value)
{
  if (FGetHTMLEscapedStringCommand == NULL)
  {
    FGetHTMLEscapedStringCommand = FDBXConnection->CreateCommand();
    FGetHTMLEscapedStringCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetHTMLEscapedStringCommand->Text = "TMessageManage.GetHTMLEscapedString";
    FGetHTMLEscapedStringCommand->Prepare();
}
FGetHTMLEscapedStringCommand->Parameters->Parameter[0]->Value->SetWideString(Value);
FGetHTMLEscapedStringCommand->ExecuteUpdate();
System::UnicodeString result = FGetHTMLEscapedStringCommand->Parameters->Parameter[1]->Value->GetWideString();
return result;
}

bool __fastcall TMessageManageClient::UpdateCurUserCallbackClientID(System::UnicodeString Value)
{
  if (FUpdateCurUserCallbackClientIDCommand == NULL)
  {
    FUpdateCurUserCallbackClientIDCommand = FDBXConnection->CreateCommand();
    FUpdateCurUserCallbackClientIDCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateCurUserCallbackClientIDCommand->Text = "TMessageManage.UpdateCurUserCallbackClientID";
    FUpdateCurUserCallbackClientIDCommand->Prepare();
}
FUpdateCurUserCallbackClientIDCommand->Parameters->Parameter[0]->Value->SetWideString(Value);
FUpdateCurUserCallbackClientIDCommand->ExecuteUpdate();
bool result = FUpdateCurUserCallbackClientIDCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToAll(System::UnicodeString Msg)
{
  if (FSendMessageToAllCommand == NULL)
  {
    FSendMessageToAllCommand = FDBXConnection->CreateCommand();
    FSendMessageToAllCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToAllCommand->Text = "TMessageManage.SendMessageToAll";
    FSendMessageToAllCommand->Prepare();
}
FSendMessageToAllCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToAllCommand->ExecuteUpdate();
bool result = FSendMessageToAllCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToUser(System::UnicodeString Msg, System::UnicodeString UserID)
{
  if (FSendMessageToUserCommand == NULL)
  {
    FSendMessageToUserCommand = FDBXConnection->CreateCommand();
    FSendMessageToUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToUserCommand->Text = "TMessageManage.SendMessageToUser";
    FSendMessageToUserCommand->Prepare();
}
FSendMessageToUserCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToUserCommand->Parameters->Parameter[1]->Value->SetWideString(UserID);
FSendMessageToUserCommand->ExecuteUpdate();
bool result = FSendMessageToUserCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToDepartment(System::UnicodeString Msg, System::UnicodeString DepartmentID)
{
  if (FSendMessageToDepartmentCommand == NULL)
  {
    FSendMessageToDepartmentCommand = FDBXConnection->CreateCommand();
    FSendMessageToDepartmentCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToDepartmentCommand->Text = "TMessageManage.SendMessageToDepartment";
    FSendMessageToDepartmentCommand->Prepare();
}
FSendMessageToDepartmentCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToDepartmentCommand->Parameters->Parameter[1]->Value->SetWideString(DepartmentID);
FSendMessageToDepartmentCommand->ExecuteUpdate();
bool result = FSendMessageToDepartmentCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToGroup(System::UnicodeString Msg, System::UnicodeString GroupID)
{
  if (FSendMessageToGroupCommand == NULL)
  {
    FSendMessageToGroupCommand = FDBXConnection->CreateCommand();
    FSendMessageToGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToGroupCommand->Text = "TMessageManage.SendMessageToGroup";
    FSendMessageToGroupCommand->Prepare();
}
FSendMessageToGroupCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToGroupCommand->Parameters->Parameter[1]->Value->SetWideString(GroupID);
FSendMessageToGroupCommand->ExecuteUpdate();
bool result = FSendMessageToGroupCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToJob(System::UnicodeString Msg, System::UnicodeString JobID)
{
  if (FSendMessageToJobCommand == NULL)
  {
    FSendMessageToJobCommand = FDBXConnection->CreateCommand();
    FSendMessageToJobCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToJobCommand->Text = "TMessageManage.SendMessageToJob";
    FSendMessageToJobCommand->Prepare();
}
FSendMessageToJobCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToJobCommand->Parameters->Parameter[1]->Value->SetWideString(JobID);
FSendMessageToJobCommand->ExecuteUpdate();
bool result = FSendMessageToJobCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToRole(System::UnicodeString Msg, System::UnicodeString RoleID)
{
  if (FSendMessageToRoleCommand == NULL)
  {
    FSendMessageToRoleCommand = FDBXConnection->CreateCommand();
    FSendMessageToRoleCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToRoleCommand->Text = "TMessageManage.SendMessageToRole";
    FSendMessageToRoleCommand->Prepare();
}
FSendMessageToRoleCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToRoleCommand->Parameters->Parameter[1]->Value->SetWideString(RoleID);
FSendMessageToRoleCommand->ExecuteUpdate();
bool result = FSendMessageToRoleCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMessageToFollow(System::UnicodeString Msg)
{
  if (FSendMessageToFollowCommand == NULL)
  {
    FSendMessageToFollowCommand = FDBXConnection->CreateCommand();
    FSendMessageToFollowCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMessageToFollowCommand->Text = "TMessageManage.SendMessageToFollow";
    FSendMessageToFollowCommand->Prepare();
}
FSendMessageToFollowCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMessageToFollowCommand->ExecuteUpdate();
bool result = FSendMessageToFollowCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToFollow(System::UnicodeString cmdStr)
{
  if (FSendCMDToFollowCommand == NULL)
  {
    FSendCMDToFollowCommand = FDBXConnection->CreateCommand();
    FSendCMDToFollowCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToFollowCommand->Text = "TMessageManage.SendCMDToFollow";
    FSendCMDToFollowCommand->Prepare();
}
FSendCMDToFollowCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToFollowCommand->ExecuteUpdate();
bool result = FSendCMDToFollowCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToAll(System::UnicodeString cmdStr)
{
  if (FSendCMDToAllCommand == NULL)
  {
    FSendCMDToAllCommand = FDBXConnection->CreateCommand();
    FSendCMDToAllCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToAllCommand->Text = "TMessageManage.SendCMDToAll";
    FSendCMDToAllCommand->Prepare();
}
FSendCMDToAllCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToAllCommand->ExecuteUpdate();
bool result = FSendCMDToAllCommand->Parameters->Parameter[1]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToUser(System::UnicodeString cmdStr, System::UnicodeString UserID)
{
  if (FSendCMDToUserCommand == NULL)
  {
    FSendCMDToUserCommand = FDBXConnection->CreateCommand();
    FSendCMDToUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToUserCommand->Text = "TMessageManage.SendCMDToUser";
    FSendCMDToUserCommand->Prepare();
}
FSendCMDToUserCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToUserCommand->Parameters->Parameter[1]->Value->SetWideString(UserID);
FSendCMDToUserCommand->ExecuteUpdate();
bool result = FSendCMDToUserCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToDepartment(System::UnicodeString cmdStr, System::UnicodeString DepartmentID)
{
  if (FSendCMDToDepartmentCommand == NULL)
  {
    FSendCMDToDepartmentCommand = FDBXConnection->CreateCommand();
    FSendCMDToDepartmentCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToDepartmentCommand->Text = "TMessageManage.SendCMDToDepartment";
    FSendCMDToDepartmentCommand->Prepare();
}
FSendCMDToDepartmentCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToDepartmentCommand->Parameters->Parameter[1]->Value->SetWideString(DepartmentID);
FSendCMDToDepartmentCommand->ExecuteUpdate();
bool result = FSendCMDToDepartmentCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToGroup(System::UnicodeString cmdStr, System::UnicodeString GroupID)
{
  if (FSendCMDToGroupCommand == NULL)
  {
    FSendCMDToGroupCommand = FDBXConnection->CreateCommand();
    FSendCMDToGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToGroupCommand->Text = "TMessageManage.SendCMDToGroup";
    FSendCMDToGroupCommand->Prepare();
}
FSendCMDToGroupCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToGroupCommand->Parameters->Parameter[1]->Value->SetWideString(GroupID);
FSendCMDToGroupCommand->ExecuteUpdate();
bool result = FSendCMDToGroupCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToJob(System::UnicodeString cmdStr, System::UnicodeString JobID)
{
  if (FSendCMDToJobCommand == NULL)
  {
    FSendCMDToJobCommand = FDBXConnection->CreateCommand();
    FSendCMDToJobCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToJobCommand->Text = "TMessageManage.SendCMDToJob";
    FSendCMDToJobCommand->Prepare();
}
FSendCMDToJobCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToJobCommand->Parameters->Parameter[1]->Value->SetWideString(JobID);
FSendCMDToJobCommand->ExecuteUpdate();
bool result = FSendCMDToJobCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendCMDToRole(System::UnicodeString cmdStr, System::UnicodeString RoleID)
{
  if (FSendCMDToRoleCommand == NULL)
  {
    FSendCMDToRoleCommand = FDBXConnection->CreateCommand();
    FSendCMDToRoleCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendCMDToRoleCommand->Text = "TMessageManage.SendCMDToRole";
    FSendCMDToRoleCommand->Prepare();
}
FSendCMDToRoleCommand->Parameters->Parameter[0]->Value->SetWideString(cmdStr);
FSendCMDToRoleCommand->Parameters->Parameter[1]->Value->SetWideString(RoleID);
FSendCMDToRoleCommand->ExecuteUpdate();
bool result = FSendCMDToRoleCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMgrMessage(int itype, System::UnicodeString Msg)
{
  if (FSendMgrMessageCommand == NULL)
  {
    FSendMgrMessageCommand = FDBXConnection->CreateCommand();
    FSendMgrMessageCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMgrMessageCommand->Text = "TMessageManage.SendMgrMessage";
    FSendMgrMessageCommand->Prepare();
}
FSendMgrMessageCommand->Parameters->Parameter[0]->Value->SetInt32(itype);
FSendMgrMessageCommand->Parameters->Parameter[1]->Value->SetWideString(Msg);
FSendMgrMessageCommand->ExecuteUpdate();
bool result = FSendMgrMessageCommand->Parameters->Parameter[2]->Value->GetBoolean();
return result;
}

void __fastcall TMessageManageClient::SendMissMessage(System::UnicodeString UserId)
{
  if (FSendMissMessageCommand == NULL)
  {
    FSendMissMessageCommand = FDBXConnection->CreateCommand();
    FSendMissMessageCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMissMessageCommand->Text = "TMessageManage.SendMissMessage";
    FSendMissMessageCommand->Prepare();
}
FSendMissMessageCommand->Parameters->Parameter[0]->Value->SetWideString(UserId);
FSendMissMessageCommand->ExecuteUpdate();
}

TJSONArray* __fastcall TMessageManageClient::GetLatelyMessage(System::UnicodeString UserId, System::UnicodeString ReceId, int RecType)
{
  if (FGetLatelyMessageCommand == NULL)
  {
    FGetLatelyMessageCommand = FDBXConnection->CreateCommand();
    FGetLatelyMessageCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetLatelyMessageCommand->Text = "TMessageManage.GetLatelyMessage";
    FGetLatelyMessageCommand->Prepare();
}
FGetLatelyMessageCommand->Parameters->Parameter[0]->Value->SetWideString(UserId);
FGetLatelyMessageCommand->Parameters->Parameter[1]->Value->SetWideString(ReceId);
FGetLatelyMessageCommand->Parameters->Parameter[2]->Value->SetInt32(RecType);
FGetLatelyMessageCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetLatelyMessageCommand->Parameters->Parameter[3]->Value->GetJSONValue(FInstanceOwner);
return result;
}

bool __fastcall TMessageManageClient::SendMissMessageToUser(System::UnicodeString Msg, System::UnicodeString FromID, System::UnicodeString FromName, System::UnicodeString MsgID, System::UnicodeString sendtime)
{
  if (FSendMissMessageToUserCommand == NULL)
  {
    FSendMissMessageToUserCommand = FDBXConnection->CreateCommand();
    FSendMissMessageToUserCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMissMessageToUserCommand->Text = "TMessageManage.SendMissMessageToUser";
    FSendMissMessageToUserCommand->Prepare();
}
FSendMissMessageToUserCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMissMessageToUserCommand->Parameters->Parameter[1]->Value->SetWideString(FromID);
FSendMissMessageToUserCommand->Parameters->Parameter[2]->Value->SetWideString(FromName);
FSendMissMessageToUserCommand->Parameters->Parameter[3]->Value->SetWideString(MsgID);
FSendMissMessageToUserCommand->Parameters->Parameter[4]->Value->SetWideString(sendtime);
FSendMissMessageToUserCommand->ExecuteUpdate();
bool result = FSendMissMessageToUserCommand->Parameters->Parameter[5]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMissMessageToDepartment(System::UnicodeString Msg, System::UnicodeString FromID, System::UnicodeString FromName, System::UnicodeString DepartmentID, System::UnicodeString MsgID, System::UnicodeString sendtime)
{
  if (FSendMissMessageToDepartmentCommand == NULL)
  {
    FSendMissMessageToDepartmentCommand = FDBXConnection->CreateCommand();
    FSendMissMessageToDepartmentCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMissMessageToDepartmentCommand->Text = "TMessageManage.SendMissMessageToDepartment";
    FSendMissMessageToDepartmentCommand->Prepare();
}
FSendMissMessageToDepartmentCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMissMessageToDepartmentCommand->Parameters->Parameter[1]->Value->SetWideString(FromID);
FSendMissMessageToDepartmentCommand->Parameters->Parameter[2]->Value->SetWideString(FromName);
FSendMissMessageToDepartmentCommand->Parameters->Parameter[3]->Value->SetWideString(DepartmentID);
FSendMissMessageToDepartmentCommand->Parameters->Parameter[4]->Value->SetWideString(MsgID);
FSendMissMessageToDepartmentCommand->Parameters->Parameter[5]->Value->SetWideString(sendtime);
FSendMissMessageToDepartmentCommand->ExecuteUpdate();
bool result = FSendMissMessageToDepartmentCommand->Parameters->Parameter[6]->Value->GetBoolean();
return result;
}

bool __fastcall TMessageManageClient::SendMissMessageToGroup(System::UnicodeString Msg, System::UnicodeString FromID, System::UnicodeString FromName, System::UnicodeString GroupID, System::UnicodeString MsgID, System::UnicodeString sendtime)
{
  if (FSendMissMessageToGroupCommand == NULL)
  {
    FSendMissMessageToGroupCommand = FDBXConnection->CreateCommand();
    FSendMissMessageToGroupCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendMissMessageToGroupCommand->Text = "TMessageManage.SendMissMessageToGroup";
    FSendMissMessageToGroupCommand->Prepare();
}
FSendMissMessageToGroupCommand->Parameters->Parameter[0]->Value->SetWideString(Msg);
FSendMissMessageToGroupCommand->Parameters->Parameter[1]->Value->SetWideString(FromID);
FSendMissMessageToGroupCommand->Parameters->Parameter[2]->Value->SetWideString(FromName);
FSendMissMessageToGroupCommand->Parameters->Parameter[3]->Value->SetWideString(GroupID);
FSendMissMessageToGroupCommand->Parameters->Parameter[4]->Value->SetWideString(MsgID);
FSendMissMessageToGroupCommand->Parameters->Parameter[5]->Value->SetWideString(sendtime);
FSendMissMessageToGroupCommand->ExecuteUpdate();
bool result = FSendMissMessageToGroupCommand->Parameters->Parameter[6]->Value->GetBoolean();
return result;
}


__fastcall  TMessageManageClient::TMessageManageClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TMessageManageClient::TMessageManageClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TMessageManageClient::~TMessageManageClient()
{
  delete FNewMessageGroupCommand;
  delete FDelMessageGroupCommand;
  delete FUpdateMessageGroupCommand;
  delete FNewMessageMemberCommand;
  delete FUpdateMessageMemberCommand;
  delete FDelMessageMemberCommand;
  delete FGetMessageGroupListCommand;
  delete FGetGroupMemberListCommand;
  delete FGetAllEmployeeCommand;
  delete FIsGroupMemberCommand;
  delete FGetUserListCommand;
  delete FGetHTMLEscapedStringCommand;
  delete FUpdateCurUserCallbackClientIDCommand;
  delete FSendMessageToAllCommand;
  delete FSendMessageToUserCommand;
  delete FSendMessageToDepartmentCommand;
  delete FSendMessageToGroupCommand;
  delete FSendMessageToJobCommand;
  delete FSendMessageToRoleCommand;
  delete FSendMessageToFollowCommand;
  delete FSendCMDToFollowCommand;
  delete FSendCMDToAllCommand;
  delete FSendCMDToUserCommand;
  delete FSendCMDToDepartmentCommand;
  delete FSendCMDToGroupCommand;
  delete FSendCMDToJobCommand;
  delete FSendCMDToRoleCommand;
  delete FSendMgrMessageCommand;
  delete FSendMissMessageCommand;
  delete FGetLatelyMessageCommand;
  delete FSendMissMessageToUserCommand;
  delete FSendMissMessageToDepartmentCommand;
  delete FSendMissMessageToGroupCommand;
}

TJSONArray* __fastcall TContractProjectClient::GetContype()
{
  if (FGetContypeCommand == NULL)
  {
    FGetContypeCommand = FDBXConnection->CreateCommand();
    FGetContypeCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetContypeCommand->Text = "TContractProject.GetContype";
    FGetContypeCommand->Prepare();
}
FGetContypeCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetContypeCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TContractProjectClient::GetContlist(System::UnicodeString userid, System::UnicodeString ctypeid)
{
  if (FGetContlistCommand == NULL)
  {
    FGetContlistCommand = FDBXConnection->CreateCommand();
    FGetContlistCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetContlistCommand->Text = "TContractProject.GetContlist";
    FGetContlistCommand->Prepare();
}
FGetContlistCommand->Parameters->Parameter[0]->Value->SetWideString(userid);
FGetContlistCommand->Parameters->Parameter[1]->Value->SetWideString(ctypeid);
FGetContlistCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetContlistCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TContractProjectClient::GetContdata(System::UnicodeString contid)
{
  if (FGetContdataCommand == NULL)
  {
    FGetContdataCommand = FDBXConnection->CreateCommand();
    FGetContdataCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetContdataCommand->Text = "TContractProject.GetContdata";
    FGetContdataCommand->Prepare();
}
FGetContdataCommand->Parameters->Parameter[0]->Value->SetWideString(contid);
FGetContdataCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetContdataCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TContractProjectClient::GetProtype()
{
  if (FGetProtypeCommand == NULL)
  {
    FGetProtypeCommand = FDBXConnection->CreateCommand();
    FGetProtypeCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetProtypeCommand->Text = "TContractProject.GetProtype";
    FGetProtypeCommand->Prepare();
}
FGetProtypeCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetProtypeCommand->Parameters->Parameter[0]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TContractProjectClient::GetProlist(System::UnicodeString userid, System::UnicodeString ProTypeid)
{
  if (FGetProlistCommand == NULL)
  {
    FGetProlistCommand = FDBXConnection->CreateCommand();
    FGetProlistCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetProlistCommand->Text = "TContractProject.GetProlist";
    FGetProlistCommand->Prepare();
}
FGetProlistCommand->Parameters->Parameter[0]->Value->SetWideString(userid);
FGetProlistCommand->Parameters->Parameter[1]->Value->SetWideString(ProTypeid);
FGetProlistCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetProlistCommand->Parameters->Parameter[2]->Value->GetJSONValue(FInstanceOwner);
return result;
}

TJSONArray* __fastcall TContractProjectClient::GetProdata(System::UnicodeString Projectid)
{
  if (FGetProdataCommand == NULL)
  {
    FGetProdataCommand = FDBXConnection->CreateCommand();
    FGetProdataCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetProdataCommand->Text = "TContractProject.GetProdata";
    FGetProdataCommand->Prepare();
}
FGetProdataCommand->Parameters->Parameter[0]->Value->SetWideString(Projectid);
FGetProdataCommand->ExecuteUpdate();
TJSONArray* result = (TJSONArray*)FGetProdataCommand->Parameters->Parameter[1]->Value->GetJSONValue(FInstanceOwner);
return result;
}


__fastcall  TContractProjectClient::TContractProjectClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TContractProjectClient::TContractProjectClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TContractProjectClient::~TContractProjectClient()
{
  delete FGetContypeCommand;
  delete FGetContlistCommand;
  delete FGetContdataCommand;
  delete FGetProtypeCommand;
  delete FGetProlistCommand;
  delete FGetProdataCommand;
}

TStream* __fastcall THouseViewClient::GetHouseViewStream(System::UnicodeString AKeyID)
{
  if (FGetHouseViewStreamCommand == NULL)
  {
    FGetHouseViewStreamCommand = FDBXConnection->CreateCommand();
    FGetHouseViewStreamCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FGetHouseViewStreamCommand->Text = "THouseView.GetHouseViewStream";
    FGetHouseViewStreamCommand->Prepare();
}
FGetHouseViewStreamCommand->Parameters->Parameter[0]->Value->SetWideString(AKeyID);
FGetHouseViewStreamCommand->ExecuteUpdate();
TStream* result = FGetHouseViewStreamCommand->Parameters->Parameter[1]->Value->GetStream(FInstanceOwner);
return result;
}

bool __fastcall THouseViewClient::UpdateHouseViewFile(System::UnicodeString AKeyID, System::UnicodeString AFileName, TStream* AStream)
{
  if (FUpdateHouseViewFileCommand == NULL)
  {
    FUpdateHouseViewFileCommand = FDBXConnection->CreateCommand();
    FUpdateHouseViewFileCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FUpdateHouseViewFileCommand->Text = "THouseView.UpdateHouseViewFile";
    FUpdateHouseViewFileCommand->Prepare();
}
FUpdateHouseViewFileCommand->Parameters->Parameter[0]->Value->SetWideString(AKeyID);
FUpdateHouseViewFileCommand->Parameters->Parameter[1]->Value->SetWideString(AFileName);
FUpdateHouseViewFileCommand->Parameters->Parameter[2]->Value->SetStream(AStream, FInstanceOwner);
FUpdateHouseViewFileCommand->ExecuteUpdate();
bool result = FUpdateHouseViewFileCommand->Parameters->Parameter[3]->Value->GetBoolean();
return result;
}


__fastcall  THouseViewClient::THouseViewClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  THouseViewClient::THouseViewClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  THouseViewClient::~THouseViewClient()
{
  delete FGetHouseViewStreamCommand;
  delete FUpdateHouseViewFileCommand;
}

void __fastcall TSendUserMessageClient::SendUserMessage(int type, System::UnicodeString AID)
{
  if (FSendUserMessageCommand == NULL)
  {
    FSendUserMessageCommand = FDBXConnection->CreateCommand();
    FSendUserMessageCommand->CommandType = TDBXCommandTypes_DSServerMethod;
    FSendUserMessageCommand->Text = "TSendUserMessage.SendUserMessage";
    FSendUserMessageCommand->Prepare();
}
FSendUserMessageCommand->Parameters->Parameter[0]->Value->SetInt32(type);
FSendUserMessageCommand->Parameters->Parameter[1]->Value->SetWideString(AID);
FSendUserMessageCommand->ExecuteUpdate();
}


__fastcall  TSendUserMessageClient::TSendUserMessageClient(TDBXConnection *ADBXConnection)
{
  if (ADBXConnection == NULL)
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = True;
}


__fastcall  TSendUserMessageClient::TSendUserMessageClient(TDBXConnection *ADBXConnection, bool AInstanceOwner)
{
  if (ADBXConnection == NULL) 
    throw EInvalidOperation("Connection cannot be nil.  Make sure the connection has been opened.");
FDBXConnection = ADBXConnection;
FInstanceOwner = AInstanceOwner;
}


__fastcall  TSendUserMessageClient::~TSendUserMessageClient()
{
  delete FSendUserMessageCommand;
}

