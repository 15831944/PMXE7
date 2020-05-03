//---------------------------------------------------------------------------

#ifndef LoginH
#define LoginH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.MobilePreview.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TLoginForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *Header;
	TToolBar *Footer;
	TLabel *HeaderLabel;
	TLabel *Label1;
	TEdit *edUserLoginCode;
	TLabel *Label2;
	TEdit *edPassword;
	TButton *btOk;
	TButton *btCancel;
	TCheckBox *ckbRember;
	TCheckBox *ckbAuto;
	TLayout *Layout1;
	void __fastcall btOkClick(TObject *Sender);
	void __fastcall ckbAutoChange(TObject *Sender);
	void __fastcall ckbRemberChange(TObject *Sender);
private:	// User declarations
	String __fastcall get_LogonCode();
	String __fastcall get_Password();
	void __fastcall SetLogonCode(String Value);
	void __fastcall SetPassword(String Value);
	bool __fastcall GetRemember();
	void __fastcall SetRemember(bool Value);
	bool __fastcall GetAutoLogin();
	void __fastcall SetAutoLogin(bool Value);
public:		// User declarations
	__fastcall TLoginForm(TComponent* Owner);
	__property String LogonCode={read=get_LogonCode,write=SetLogonCode};
	__property String Password={read=get_Password,write=SetPassword};
	__property bool   Remember={read=GetRemember,write=SetRemember};
	__property bool   AutoLogin={read=GetAutoLogin,write=SetAutoLogin};

};
//---------------------------------------------------------------------------
extern PACKAGE TLoginForm *LoginForm;
//---------------------------------------------------------------------------
#endif
