//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ActnList.hpp>
#include <FMX.MediaLibrary.Actions.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdActns.hpp>
#include <System.Actions.hpp>
#include <FMX.Media.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TToolBar *Header;
	TToolBar *Footer;
	TLabel *HeaderLabel;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *edFormat;
	TEdit *edResult;
	TActionList *ActionList1;
	TTakePhotoFromLibraryAction *TakePhotoFromLibraryAction1;
	TTakePhotoFromCameraAction *TakePhotoFromCameraAction1;
	TButton *btCamer;
	TAction *acDecode;
	TImage *ImageContainer;
	TButton *btEndCamera;
	TButton *btClose;
	TAction *acClose;
	TCameraComponent *BarCameraCom;
	TAction *acBeginCamera;
	TAction *acEndCamera;
	TAction *acCameraLightOn;
	TAction *acCameraLightOff;
	void __fastcall acDecodeExecute(TObject *Sender);
	void __fastcall TakePhotoFromLibraryAction1DidFinishTaking(TBitmap *Image);
	void __fastcall TakePhotoFromCameraAction1DidFinishTaking(TBitmap *Image);
	void __fastcall acCloseExecute(TObject *Sender);
	void __fastcall acBeginCameraExecute(TObject *Sender);
	void __fastcall acEndCameraExecute(TObject *Sender);
	void __fastcall acCameraLightOnExecute(TObject *Sender);
	void __fastcall BarCameraComSampleBufferReady(TObject *Sender, const __int64 ATime);
	void __fastcall acCameraLightOffExecute(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:	// User declarations
	TBitmap *BuffBitmap;
	TBitmap *BarBitmap;
	TRectF BarRect;
	System::String AppPath;
	void __fastcall DrawImage();
	void __fastcall OnDecodeResults(UnicodeString result,UnicodeString barformat);
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
