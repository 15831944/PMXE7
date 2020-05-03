//---------------------------------------------------------------------------

#ifndef CommBaseH
#define CommBaseH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "ZClientQuery.h"
#include "ClientBroker.h"
#include "ModuleEnum.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TCommBaseForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *tbarBaseFilter;
	TButton *btClose;
	TVertScrollBox *ItemVertScrollBox;
	TLayout *EditLayout;
	TLabel *lbTitle;
	void __fastcall btCloseClick(TObject *Sender);
	void __fastcall FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds);
	void __fastcall FormFocusChanged(TObject *Sender);
private:	// User declarations
	TClientBroker *FBroker;
	TZClientQuery *FQuery;
	void __fastcall RestorePosition();
	void __fastcall UpdateKBBounds();
	void __fastcall CalcContentBoundsProc(TObject * Sender, System::Types::TRectF &ContentBounds);
	System::Types::TRectF FKBBounds;
	bool FNeedOffset;
	bool FKeyboardShow;
protected:
	virtual void __fastcall InitControl(){};
public:		// User declarations
	__fastcall TCommBaseForm(TComponent* Owner,TClientBroker * ABroker);
	virtual __fastcall ~TCommBaseForm();
	__property TClientBroker *ClientBroker={ read=FBroker,write=FBroker};
	__property TZClientQuery *Query={ read=FQuery};
};
//---------------------------------------------------------------------------
extern PACKAGE TCommBaseForm *CommBaseForm;
//---------------------------------------------------------------------------
#endif
