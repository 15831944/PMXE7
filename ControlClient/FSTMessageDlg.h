//---------------------------------------------------------------------------

#ifndef FSTMessageDlgH
#define FSTMessageDlgH
#include <System.Classes.hpp>
#include <FMX.Dialogs.hpp>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TMessageDialogEventProc)(const System::Uitypes::TModalResult AResult);
class TMessageDialogHandler :public TCppInterfacedObject<TInputCloseDialogProc>
{
private:
	  TMessageDialogEventProc FMessageDialogEventProc;
public:
	void __fastcall Invoke(const System::Uitypes::TModalResult AResult) {
	  FMessageDialogEventProc(AResult);
	}
	__fastcall TMessageDialogHandler(TMessageDialogEventProc &AMessageDialogEventProc):FMessageDialogEventProc(AMessageDialogEventProc){}
};
void __fastcall FSTMessageDlg(const System::UnicodeString AMessage, const System::Uitypes::TMsgDlgType ADialogType, const System::Uitypes::TMsgDlgButtons AButtons, const int AHelpContext,TMessageDialogEventProc &AMessageDialogEventProc);
void __fastcall ShowMessageBox(const System::UnicodeString AMessage,TMessageDialogEventProc &AMessageDialogEventProc);
#endif

