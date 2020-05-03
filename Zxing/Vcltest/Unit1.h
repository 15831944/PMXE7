//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "ImageReaderSource.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/Result.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/common/HybridBinarizer.h>
#include <exception>
#include <zxing/Exception.h>
#include <zxing/common/IllegalArgumentException.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/DecodeHints.h>

#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/multi/qrcode/QRCodeMultiReader.h>
#include <zxing/multi/ByQuadrantReader.h>
#include <zxing/multi/MultipleBarcodeReader.h>
#include <zxing/multi/GenericMultipleBarcodeReader.h>
using namespace std;
using namespace zxing;
using namespace zxing::multi;
using namespace zxing::qrcode;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *edFileName;
	TEdit *edBarcodeFormat;
	TEdit *edBarcodeValue;
	TButton *btDecode;
	TSpeedButton *spbOpen;
	TOpenPictureDialog *OpenPictureDialog1;
	TImage *Image1;
	TButton *btEncode;
	TLabel *Label4;
	TEdit *edError;
	void __fastcall spbOpenClick(TObject *Sender);
	void __fastcall btDecodeClick(TObject *Sender);
private:	// User declarations
	int read_image(Ref<LuminanceSource> source, bool hybrid,bool search_multi);
	Ref<LuminanceSource>  BarcodePicToLuminanceSource(string filename);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
