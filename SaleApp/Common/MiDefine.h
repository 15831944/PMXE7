//---------------------------------------------------------------------------

#ifndef MiDefineH
#define MiDefineH
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
enum TQQUserType: unsigned char {qutCompany,qutDepartment,qutUser,qutGroup};
typedef struct QQUserRecord
{
  int State;//是否在线0,1
  int  iMessageFormID; //打开的消息窗体标识
  TQQUserType UserType;  //用户类型
  String ID;          //用户ID
  String Name;
} TQQUserRecord ;
#endif
