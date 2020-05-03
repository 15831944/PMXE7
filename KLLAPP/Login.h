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
#include <FMX.Objects.hpp>
#include <FMX.Controls.Presentation.hpp>
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
	TImage *Image1;
	void __fastcall btOkClick(TObject *Sender);
private:	// User declarations
  String __fastcall get_LogonCode();
  String __fastcall get_Password();
public:		// User declarations
	__fastcall TLoginForm(TComponent* Owner);
	__property String LogonCode={read=get_LogonCode};
	__property String Password={read=get_Password};
};
//---------------------------------------------------------------------------
extern PACKAGE TLoginForm *LoginForm;
//---------------------------------------------------------------------------
#endif
