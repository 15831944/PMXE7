//---------------------------------------------------------------------------

#ifndef ShowHSViewH
#define ShowHSViewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.Objects.hpp>
#include <FMX.ActnList.hpp>
#include <FMX.MediaLibrary.Actions.hpp>
#include <FMX.StdActns.hpp>
#include <System.Actions.hpp>
#include <FMX.Memo.hpp>
//---------------------------------------------------------------------------
class TShowViewForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TLabel *lbModule;
	TGestureManager *GestureManager2;
	TImage *Image1;
	TActionList *ActionList1;
	TShowShareSheetAction *ShowShareSheetAction1;
	TMemo *Memo1;
	TButton *btShare;
	TToolBar *ToolBar2;
	TButton *btClose;
	TScrollBox *ScrollBox1;
	TLayout *Layout1;
	void __fastcall ShowShareSheetAction1BeforeExecute(TObject *Sender);
	void __fastcall btCloseClick(TObject *Sender);
	void __fastcall Layout1Gesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ScrollBox1Resize(TObject *Sender);
private:	// User declarations
	   int FLastDistance;
	   String FMFileName;
	   String FModuleText;
	   int imType;
	   int cWidth;
	   int cHeight;
	   double HWScale;
	   void __fastcall SetMType(int Value);
	   void __fastcall SetModuleText(String Value);
	   void __fastcall SetMFileName(String Value);
public:		// User declarations
	__fastcall TShowViewForm(TComponent* Owner);
	__property String MFileName={read=FMFileName,write=SetMFileName};
	__property int MType={read=imType,write=SetMType};
	__property String ModuleText={read=FModuleText,write=SetModuleText};
};
//---------------------------------------------------------------------------
extern PACKAGE TShowViewForm *ShowViewForm;
//---------------------------------------------------------------------------
#endif
