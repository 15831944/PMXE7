//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <IPPeerClient.hpp>
#include <IPPeerServer.hpp>
#include <System.Tether.AppProfile.hpp>
#include <System.Tether.Manager.hpp>
#include <FMX.ActnList.hpp>
#include <FMX.MediaLibrary.Actions.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdActns.hpp>
#include <System.Actions.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Controls.Presentation.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TTetheringManager *CommandManager;
	TTetheringAppProfile *CommAppProfile;
	TToolBar *ToolBar1;
	TButton *btTake;
	TActionList *ActionList1;
	TTakePhotoFromCameraAction *TakePhotoFromCameraAction1;
	TImage *ImageContainer;
	TButton *btSN;
	TAction *acReadSN;
	TButton *btFind;
	TLabel *Label1;
	TToolBar *ToolBar2;
	TPanel *Panel1;
	TLabel *Label2;
	TEdit *edDeviceSN;
	TEditButton *EditButton1;
	TLabel *Label3;
	TLabel *lbDeviceName;
	TLabel *Label5;
	TLabel *lbDeciceMode;
	TButton *btClose;
	TAction *acClose;
	TAction *acLocateRecord;
	TLabel *lbConnProfiles;
	TComboBox *cbBoxFormat;
	TTimer *tmCheckConnection;
	void __fastcall btFindClick(TObject *Sender);
	void __fastcall CommandManagerRequestManagerPassword(const TObject *Sender, const UnicodeString RemoteIdentifier,
          UnicodeString &Password);
	void __fastcall TakePhotoFromCameraAction1DidFinishTaking(TBitmap *Image);
	void __fastcall acReadSNExecute(TObject *Sender);
	void __fastcall CommandAppResources0ResourceReceived(const TObject *Sender, const TRemoteResource *AResource);
	void __fastcall CommandAppResources1ResourceReceived(const TObject *Sender, const TRemoteResource *AResource);
	void __fastcall CommandAppResources2ResourceReceived(const TObject *Sender, const TRemoteResource *AResource);
	void __fastcall CommandAppResources3ResourceReceived(const TObject *Sender, const TRemoteResource *AResource);
	void __fastcall CommandManagerRemoteManagerShutdown(const TObject *Sender, const UnicodeString ManagerIdentifier);
	void __fastcall acCloseExecute(TObject *Sender);
	void __fastcall acLocateRecordExecute(TObject *Sender);
	void __fastcall FSTAppProfileResources4ResourceReceived(const TObject *Sender, const TRemoteResource *AResource);
	void __fastcall CommandManagerEndAutoConnect(TObject *Sender);
	void __fastcall tmCheckConnectionTimer(TObject *Sender);
private:	// User declarations
   bool IsLocate;
   bool FConnected;
   void __fastcall CheckRemoteProfiles();
   void __fastcall ChangeControlState();
   void __fastcall OnScanGetResult(bool AExecute, System::UnicodeString AResultText, System::UnicodeString ABarcodeFormat);
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
