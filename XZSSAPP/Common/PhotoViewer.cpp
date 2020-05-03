//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "PhotoViewer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmPhotoViewer *frmPhotoViewer;
//---------------------------------------------------------------------------
__fastcall TfrmPhotoViewer::TfrmPhotoViewer(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::btCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::SetBitmapList(TStringList *bitmapList,int showIndex)
{
	FBitmapList = bitmapList;
	BuildTabs();
	if(ImageTabCtr->TabCount > 0)
	{
		if(showIndex >= 0 && showIndex < ImageTabCtr->TabCount)
			ImageTabCtr->ActiveTab = ImageTabCtr->Tabs[showIndex];
		else
			ImageTabCtr->ActiveTab = ImageTabCtr->Tabs[0];
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::BuildTabs()
{
/*	for(int i=ImageTabCtr->TabCount-1;i>=0;i--)
	{
//		ImageTabCtr->Tabs[i]->DeleteChildren();
		ImageTabCtr->Tabs[i]->DisposeOf();
		ImageTabCtr->Delete(i);
	}*/
	try
	{
	 ImageTabCtr->BeginUpdate();
	 while(ImageTabCtr->TabCount>0)
	 {
	   ImageTabCtr->Tabs[ImageTabCtr->TabCount-1]->DisposeOf();
	   ImageTabCtr->Delete(ImageTabCtr->TabCount-1);
	 }
	}
	__finally
	{
	  ImageTabCtr->EndUpdate();
    }
	TMemoryStream* mapStream = new TMemoryStream;
	String mFileName;
	try{
		for(int i=0;i<FBitmapList->Count;i++)
		{
			mFileName= FBitmapList->Strings[i];
			mapStream->LoadFromFile(mFileName);
			mapStream->Position=0;
			TTabItem *tempTab = ImageTabCtr->Add();
			tempTab->Name = "TabItem" + IntToStr(i);
			TImage *tempImage = new TImage(ImageTabCtr);
			tempImage->Name = tempTab->Name + "_Image";
			tempImage->Width = ImageTabCtr->Width;
			tempImage->Height = ImageTabCtr->Height;
			tempImage->Align = TAlignLayout::Center;
		  //	tempImage->Bitmap->Assign((TBitmap*) FBitmapList->Items[i]);
			tempImage->Bitmap->LoadFromStream(mapStream);
			tempImage->Touch->GestureManager = ImageGesMng;
			tempImage->Touch->InteractiveGestures << TInteractiveGesture::Zoom << TInteractiveGesture::Pan;
			tempImage->OnGesture = ImageGesture;
			tempImage->OnMouseLeave = ImageMouseLeave;
			tempImage->Parent = tempTab;
			tempTab->Parent = ImageTabCtr;
		}
	}
	__finally
	{
		delete mapStream;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::ImageGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
	TImage *ImageViewer = (TImage *)Sender;
	short id = EventInfo.GestureID;
	if(id == igiZoom)
	{
		if(!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfBegin) &&
			!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfEnd))
		{
			int fWidth,fHeight;

			fWidth = ImageViewer->Width + (EventInfo.Distance - FLastDIstance);
			if(fWidth > ImageViewer->Bitmap->Width)
				fWidth = ImageViewer->Bitmap->Width;

			fHeight = fWidth * ImageViewer->Bitmap->Height / ImageViewer->Bitmap->Width;

			ImageViewer->Width = fWidth;
			ImageViewer->Height = fHeight;

		}
		FLastDIstance = EventInfo.Distance;
	}
	else if(id == igiPan)
	{
		if(!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfBegin) &&
			!EventInfo.Flags.Contains(TInteractiveGestureFlag::gfEnd))
		{
			ImageViewer->Position->X = ImageViewer->Position->X + (EventInfo.Location.X - FLastPosition.X);

			ImageViewer->Position->Y = ImageViewer->Position->Y + (EventInfo.Location.Y - FLastPosition.Y);
		}
		FLastPosition = EventInfo.Location;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::ImageMouseLeave(TObject *Sender)
{
	TImage *ImageViewer = dynamic_cast<TImage *>(Sender);
	try
	{
		ImageViewer->OnMouseLeave = NULL;
		ResetImage();
	}
	__finally
	{
		ImageViewer->OnMouseLeave = ImageMouseLeave;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::ResetImage()
{
  	String imageName = ImageTabCtr->ActiveTab->Name + "_Image";
	TImage *ImageViewer = dynamic_cast<TImage *>(ImageTabCtr->FindComponent(imageName));
	if(ImageViewer != NULL)
	{
		try
		{
			ImageViewer->OnGesture = NULL;
			if(ImageViewer->Height < ImageTabCtr->Height)
				ImageViewer->Height = ImageTabCtr->Height;
			if(ImageViewer->Width < ImageTabCtr->Width)
				ImageViewer->Width = ImageTabCtr->Width;

			if(ImageViewer->Position->X > 0)
			{
				ImageTabCtr->Previous(TTabTransition::Slide);
				ImageViewer->Position->X = 0;
			}
			else if(ImageViewer->Position->X < (ImageTabCtr->Width - ImageViewer->Width))
			{
				ImageTabCtr->Next(TTabTransition::Slide);
				ImageViewer->Position->X = ImageTabCtr->Width - ImageViewer->Width;
			}

			if(ImageViewer->Position->Y > 0)
				ImageViewer->Position->Y = 0;
			if(ImageViewer->Position->Y < (ImageTabCtr->Height - ImageViewer->Height))
				ImageViewer->Position->Y = ImageTabCtr->Height - ImageViewer->Height;
		}
		__finally
		{
			ImageViewer->OnGesture = ImageGesture;
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPhotoViewer::ImageTabCtrMouseLeave(TObject *Sender)
{
	try
	{
		ImageTabCtr->OnMouseLeave = NULL;
		ResetImage();
	}
	__finally
	{
		ImageTabCtr->OnMouseLeave = ImageTabCtrMouseLeave;
	}
}
//---------------------------------------------------------------------------


