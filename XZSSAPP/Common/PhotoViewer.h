//---------------------------------------------------------------------------

#ifndef PhotoViewerH
#define PhotoViewerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.Objects.hpp>
#include <FMX.TabControl.hpp>
//---------------------------------------------------------------------------
class TfrmPhotoViewer : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar2;
	TButton *btClose;
	TPanel *Panel1;
	TGestureManager *ImageGesMng;
	TTabControl *ImageTabCtr;
	TPanel *Panel2;
	void __fastcall btCloseClick(TObject *Sender);
	void __fastcall ImageTabCtrMouseLeave(TObject *Sender);
private:	// User declarations
	int FLastDIstance;
	TPointF FLastPosition;
	TStringList *FBitmapList;
	void __fastcall BuildTabs();
	void __fastcall ImageGesture(TObject *Sender, const TGestureEventInfo &EventInfo,bool &Handled);
	void __fastcall ImageMouseLeave(TObject *Sender);
	void __fastcall ResetImage();
public:		// User declarations
	__fastcall TfrmPhotoViewer(TComponent* Owner);
	void __fastcall SetBitmapList(TStringList *bitmapList,int showIndex);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPhotoViewer *frmPhotoViewer;
//---------------------------------------------------------------------------
#endif
