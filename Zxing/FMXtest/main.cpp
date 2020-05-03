//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
#include <System.Ioutils.hpp>
#include <System.UITypes.hpp>
#include "DecodeProc.h"
/*
<AllPackageLibs>rtl.lib;fmx.lib;BigIntlib.lib;ZxingCommon.lib;Azteclib.lib;Datamatrixlib.lib;QRCodelib.lib;Multilib.lib;Onedlib.lib;Pdf417lib.lib;</AllPackageLibs>
#if defined(__ANDROID__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#pragma link "libBigIntlib.a"
#pragma link "libZxingCommon.a"
#pragma link "libAzteclib.a"
#pragma link "libDatamatrixlib.a"
#pragma link "libQRCodelib.a"
#pragma link "libMultilib.a"
#pragma link "libOnedlib.a"
#pragma link "libPdf417lib.a"
#else
#pragma link "BigIntlib.lib"
#pragma link "ZxingCommon.lib"
#pragma link "Azteclib.lib"
#pragma link "Datamatrixlib.lib"
#pragma link "QRCodelib.lib"
#pragma link "Multilib.lib"
#pragma link "Onedlib.lib"
#pragma link "Pdf417lib.lib"
#endif
*/
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
#if  defined(__ANDROID__) || defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
	 AppPath= System::Ioutils::TPath::GetDocumentsPath();
#else
	AppPath = ExtractFilePath(ParamStr(0));
#endif
	BuffBitmap=new TBitmap();
	BarBitmap=new TBitmap();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acDecodeExecute(TObject *Sender)
{
  if(!BuffBitmap->IsEmpty())
  {
	TOnDecodeResults  FOndecodeResults=&OnDecodeResults;
	TDecodeQueueProc *FDecodeQueueProc=new TDecodeQueueProc(FOndecodeResults,BuffBitmap,BarRect,false);
  }
}
void __fastcall TfrmMain::OnDecodeResults(UnicodeString result,UnicodeString barformat)
{
	acEndCamera->Execute();
	edFormat->Text=barformat;
	edResult->Text=result;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TakePhotoFromLibraryAction1DidFinishTaking(TBitmap *Image)

{
 	if (Image->Width > 1024) {
		auto ScaleFactor = Image->Width / 1024;
		Image->Resize(static_cast<int>(Image->Width / ScaleFactor),
			static_cast<int>(Image->Height / ScaleFactor));
	}
	BuffBitmap->Assign(Image);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::TakePhotoFromCameraAction1DidFinishTaking(TBitmap *Image)

{
	if (Image->Width > 1024) {
		auto ScaleFactor = Image->Width / 1024;
		Image->Resize(static_cast<int>(Image->Width / ScaleFactor),
			static_cast<int>(Image->Height / ScaleFactor));
	}
	BuffBitmap->Assign(Image);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acCloseExecute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acBeginCameraExecute(TObject *Sender)
{
  //  ImageContainer->Align=TAlignLayout::Client;
	edFormat->Text="";
	edResult->Text="";
	ImageContainer->Bitmap->Clear(TAlphaColor(TAlphaColorRec::Black));
	BarCameraCom->Kind = TCameraKind::BackCamera;
	BarCameraCom->FocusMode=TFocusMode::ContinuousAutoFocus;
	BarCameraCom->Active = true;
	ImageContainer->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acEndCameraExecute(TObject *Sender)
{
	BarCameraCom->Active = false;
	ImageContainer->Visible=false;
 //	ImageContainer->Bitmap->Assign(BarBitmap);

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acCameraLightOnExecute(TObject *Sender)
{
  if(BarCameraCom->HasTorch)
	BarCameraCom->TorchMode = TTorchMode::ModeOn;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::DrawImage()
{
	BarCameraCom->SampleBufferToBitmap(BuffBitmap, true);
	if (BuffBitmap->Width > 256) {
		auto ScaleFactor = BuffBitmap->Width /256 ;
		BuffBitmap->Resize(static_cast<int>(BuffBitmap->Width / ScaleFactor),
			static_cast<int>(BuffBitmap->Height / ScaleFactor));
	}
	BarRect=TRectF(BuffBitmap->Width/10,BuffBitmap->Height*2/10,BuffBitmap->Width*9/10,BuffBitmap->Height*6/10);
	//获取部分图像
	BarBitmap->SetSize(BarRect.Width(),BarRect.Height());
	BarBitmap->Canvas->BeginScene();
	BarBitmap->Canvas->DrawBitmap(BuffBitmap,BarRect,TRectF(0,0,BarRect.Width(),BarRect.Height()),1);
	BarBitmap->Canvas->EndScene();
	//解析
	TOnDecodeResults  FOndecodeResults=&OnDecodeResults;
	TDecodeQueueProc *FDecodeQueueProc=new TDecodeQueueProc(FOndecodeResults,BarBitmap,false);
	////显示图片
	BuffBitmap->Canvas->BeginScene();
	BuffBitmap->Canvas->Stroke->Color=TAlphaColor(claRed);
	BuffBitmap->Canvas->DrawRect(BarRect,0,0,AllCorners,1);
	BuffBitmap->Canvas->EndScene();

	ImageContainer->Bitmap->Assign(BuffBitmap);

}
void __fastcall TfrmMain::BarCameraComSampleBufferReady(TObject *Sender, const __int64 ATime)
{
  DrawImage();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::acCameraLightOffExecute(TObject *Sender)
{
    if(BarCameraCom->HasTorch)
		BarCameraCom->TorchMode = TTorchMode::ModeOff;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
 delete BuffBitmap;
 delete BarBitmap;
}
//---------------------------------------------------------------------------

