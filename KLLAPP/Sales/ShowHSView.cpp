﻿//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ShowHSView.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TShowViewForm *ShowViewForm;
//---------------------------------------------------------------------------
__fastcall TShowViewForm::TShowViewForm(TComponent* Owner)
	: TForm(Owner)
{
	 HWScale=1;
	 FLastDistance = 0;
	 cWidth=ScrollBox1->Width;
	 cHeight=ScrollBox1->Height;
}
//---------------------------------------------------------------------------
void __fastcall TShowViewForm::SetMType(int Value)
{
  if(imType!=Value)
  {
	   imType=Value;
	   if(imType==1)//pic
	   {
		 Layout1->Align=TAlignLayout::None;
		 Memo1->Align=TAlignLayout::None;
		 Memo1->Visible=false;
		 Image1->Visible=true;
	   }
	   else//Txt
	   {
		Image1->Visible=false;
		Image1->Width=100;
		Image1->Height=100;
		Layout1->Align=TAlignLayout::Client;
		Memo1->Lines->Clear();
 		Memo1->Align=TAlignLayout::Client;
		Memo1->Visible=true;
	   }
   }
}
void __fastcall TShowViewForm::SetModuleText(String Value)
{
	lbModule->Text=Value;
}
void __fastcall TShowViewForm::SetMFileName(String Value)
{
   if(FMFileName!=Value)
   {
	FMFileName= Value;
	if(imType==1)
	{
	 TBitmap *lbmap=new TBitmap(FMFileName);
	 if(lbmap->Height>cHeight)
	   Layout1->Height=lbmap->Height;
	 else
	   Layout1->Height=cHeight;
	 if(lbmap->Width>cWidth)
	   Layout1->Width=lbmap->Width;
	 else
	  Layout1->Width=cWidth;
	 Image1->Height=lbmap->Height;
	 Image1->Width=lbmap->Width;
	 HWScale=double(lbmap->Height)/double(lbmap->Width);
	 Image1->Bitmap->Assign(lbmap);
	 delete lbmap;
	}
	else
	{
	  TEncoding *LEncoding = NULL;
	  // 把文件读进字节流.
	  TBytes myBytes;
	  TBytesStream * myBytesStream=new TBytesStream(myBytes);
	  myBytesStream->LoadFromFile(FMFileName);
	  //读取编码值
	  int LOffset = TEncoding::GetBufferEncoding(myBytesStream->Bytes, LEncoding);
	  if(LOffset==0) //不成功,给默认编码
		LEncoding=TEncoding::ANSI;
	  myBytesStream->Position=0;
	  Memo1->Lines->LoadFromStream(myBytesStream,LEncoding);
	  delete myBytesStream;
	}
   }
}
void __fastcall TShowViewForm::ShowShareSheetAction1BeforeExecute(TObject *Sender)

{
 if(imType==1)
 {
   ShowShareSheetAction1->Bitmap->Assign(Image1->Bitmap);
   ShowShareSheetAction1->TextMessage=FModuleText;   //无效
 }
 else
 {
	 ShowShareSheetAction1->TextMessage=FModuleText+":"+Memo1->Text;
 }
}
//---------------------------------------------------------------------------
void __fastcall TShowViewForm::btCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------


void __fastcall TShowViewForm::Layout1Gesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
 	if (EventInfo.GestureID == static_cast<short>(igiZoom)) {
		IControl *LObj = this->ObjectAtPoint(ClientToScreen(EventInfo.Location));
		if (static_cast<TImage*>((TImage*)LObj)) {
			if (!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfBegin) &&
				!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfEnd)) {
				// zoom the image
				TImage * LImage = dynamic_cast<TImage*>(LObj->GetObject());
				if(LImage)//判断是否有效对象
				{
					int miniWidth= LImage->Width + (EventInfo.Distance - FLastDistance);
					int miniHeight=HWScale*miniWidth;//LImage->Height + (EventInfo.Distance - FLastDistance);
					if(miniWidth<100)    //控制最小尺寸
					{
					   miniWidth=100;
					   miniHeight=HWScale*miniWidth;
					}
					if(miniWidth>cWidth)
					  Layout1->Width=miniWidth;
					 else
					  Layout1->Width=cWidth;

					if(miniHeight>cHeight)
					  Layout1->Height=miniHeight;
					 else
					  Layout1->Height=cHeight;
					LImage->Width = miniWidth;
					LImage->Height = miniHeight;
					TPointF LImageCenter = Layout1->Position->Point + PointF(Layout1->Width / 2,
						Layout1->Height / 2);
					LImage->Position->X = LImageCenter.X - LImage->Width / 2;
					LImage->Position->Y = LImageCenter.Y - LImage->Height / 2;
			  }
			}
			FLastDistance = EventInfo.Distance;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TShowViewForm::ScrollBox1Resize(TObject *Sender)
{
	 cWidth=ScrollBox1->Width;
	 cHeight=ScrollBox1->Height;
//	cWidth=Screen->Size().Width;
//	cHeight=Screen->Size().Height-ToolBar1->Height-ToolBar2->Height-30;

}
//---------------------------------------------------------------------------

