//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Login.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TLoginForm *LoginForm;
//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
String __fastcall TLoginForm::get_LogonCode()
{
	return edUserLoginCode->Text;
}
//---------------------------------------------------------------------------
String __fastcall TLoginForm::get_Password()
{
	return edPassword->Text;
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::btOkClick(TObject *Sender)
{
  if(edUserLoginCode->Text.Trim()=="")
  {
	edUserLoginCode->Text="";
	ShowMessage(L"�������¼�û���.");
	edUserLoginCode->SetFocus();
	return;
  }
  ModalResult=System::Uitypes::TModalResult(mrOk);
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::SetLogonCode(String Value)
{
   edUserLoginCode->Text=Value;
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::SetPassword(String Value)
{
  edPassword->Text=Value;
}
//---------------------------------------------------------------------------
bool __fastcall TLoginForm::GetRemember()
{
  return ckbRember->IsChecked;
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::SetRemember(bool Value)
{
   ckbRember->IsChecked=Value;
}
//---------------------------------------------------------------------------
bool __fastcall TLoginForm::GetAutoLogin()
{
   return ckbAuto->IsChecked;
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::SetAutoLogin(bool Value)
{
   ckbAuto->IsChecked=Value;
}
//---------------------------------------------------------------------------


void __fastcall TLoginForm::ckbAutoChange(TObject *Sender)
{
  	if(ckbAuto->IsChecked)
	 ckbRember->IsChecked=true;
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::ckbRemberChange(TObject *Sender)
{
  	 if(!ckbRember->IsChecked)
	   ckbAuto->IsChecked=false;
}
//---------------------------------------------------------------------------

