//---------------------------------------------------------------------------

#ifndef MiDefineH
#define MiDefineH
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
enum TQQUserType: unsigned char {qutCompany,qutDepartment,qutUser,qutGroup};
typedef struct QQUserRecord
{
  int State;//�Ƿ�����0,1
  int  iMessageFormID; //�򿪵���Ϣ�����ʶ
  TQQUserType UserType;  //�û�����
  String ID;          //�û�ID
  String Name;
} TQQUserRecord ;
#endif
