﻿// CodeGear C++Builder
// Copyright (c) 1995, 2014 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'DiagramShadow.pas' rev: 28.00 (Windows)

#ifndef DiagramshadowHPP
#define DiagramshadowHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <Winapi.Messages.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <Vcl.Graphics.hpp>	// Pascal unit
#include <Vcl.Controls.hpp>	// Pascal unit
#include <Vcl.Forms.hpp>	// Pascal unit
#include <System.Variants.hpp>	// Pascal unit
#include <Vcl.Dialogs.hpp>	// Pascal unit
#include <Vcl.ExtCtrls.hpp>	// Pascal unit
#include <Vcl.StdCtrls.hpp>	// Pascal unit
#include <Vcl.ComCtrls.hpp>	// Pascal unit
#include <atDiagram.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Diagramshadow
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TShadowEditor;
class PASCALIMPLEMENTATION TShadowEditor : public Vcl::Forms::TForm
{
	typedef Vcl::Forms::TForm inherited;
	
__published:
	Vcl::Comctrls::TUpDown* UpDown1;
	Vcl::Comctrls::TUpDown* UpDown2;
	Vcl::Extctrls::TPaintBox* PaintBox1;
	Vcl::Stdctrls::TLabel* Label1;
	Vcl::Extctrls::TPanel* Panel1;
	Vcl::Dialogs::TColorDialog* ColorDialog1;
	Vcl::Stdctrls::TButton* Button1;
	Vcl::Stdctrls::TButton* Button2;
	Vcl::Extctrls::TPaintBox* PaintBox2;
	void __fastcall Panel1Click(System::TObject* Sender);
	void __fastcall UpDown1ChangingEx(System::TObject* Sender, bool &AllowChange, short NewValue, Vcl::Comctrls::TUpDownDirection Direction);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall FormDestroy(System::TObject* Sender);
	void __fastcall UpDown2ChangingEx(System::TObject* Sender, bool &AllowChange, short NewValue, Vcl::Comctrls::TUpDownDirection Direction);
	void __fastcall PaintBox1Paint(System::TObject* Sender);
	void __fastcall PaintBox2Paint(System::TObject* Sender);
	
public:
	Atdiagram::TBlockShadow* Shad;
	bool __fastcall EditShadow(Atdiagram::TBlockShadow* AShadow);
public:
	/* TCustomForm.Create */ inline __fastcall virtual TShadowEditor(System::Classes::TComponent* AOwner) : Vcl::Forms::TForm(AOwner) { }
	/* TCustomForm.CreateNew */ inline __fastcall virtual TShadowEditor(System::Classes::TComponent* AOwner, int Dummy) : Vcl::Forms::TForm(AOwner, Dummy) { }
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TShadowEditor(void) { }
	
public:
	/* TWinControl.CreateParented */ inline __fastcall TShadowEditor(HWND ParentWindow) : Vcl::Forms::TForm(ParentWindow) { }
	
};


//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE TShadowEditor* ShadowEditor;
}	/* namespace Diagramshadow */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_DIAGRAMSHADOW)
using namespace Diagramshadow;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// DiagramshadowHPP
