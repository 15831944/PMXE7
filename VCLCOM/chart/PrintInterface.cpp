//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PrintInterface.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TPrintInterface::TPrintInterface()
{
    FPageWidthMm = 210;
    FPageHeightMm = 297;
    FLeftMarginMm = 2;
    FRightMarginMm = 2;
    FTopMarginMm = 2;
    FBottomMarginMm = 2;
    //FSize = 3;
    FTitleFont = new TFont();
    FTitleFont->Name = "����";
    FTitleFont->Size = 16;
    FTitleFont->Color = clBlack;
    FTitleFont->Charset = GB2312_CHARSET;
    SPixelsPerInch=Screen->PixelsPerInch;

    PrinterPixelsPerInchX = 600;//Ĭ��ֵ
    PrinterPixelsPerInchY = 600;
    PrinterPhysicalOffSetX=130;
    PrinterPhysicalOffSetY=150;
//    InitPrintPage();
}
//---------------------------------------------------------------------------
void __fastcall TPrintInterface::Print()
{
    PreparePrint();
    PrintImpl();
	EndPrint();
}
//---------------------------------------------------------------------------
void TPrintInterface::PreviewAPageImpl(HDC dc,int PageNo,RECT cr) //Ԥ��
{
   HENHMETAFILE pvMetaFile = MakeMetalFile(PageNo);
   if(pvMetaFile!=NULL)
   {
     SetMapMode(dc, MM_ANISOTROPIC);
     PlayEnhMetaFile(dc, pvMetaFile, &cr);//��ʸ��ͼ��dc
     DeleteEnhMetaFile(pvMetaFile);
   }

}
//---------------------------------------------------------------------------
void TPrintInterface::PrintAPageImpl(HDC dc,int PageNo) //��ӡ
{
   HENHMETAFILE pvMetaFile = MakeMetalFile(PageNo);
   if(pvMetaFile!=NULL)
   {
     RECT drawrect;
     drawrect.left=0;
     drawrect.top=0;
     drawrect.right=FPageWidthMm*0.03937*PrinterPixelsPerInchX;
     drawrect.bottom=FPageHeightMm*0.03937*PrinterPixelsPerInchY;
     SetMapMode(dc, MM_ANISOTROPIC);
     PlayEnhMetaFile(dc, pvMetaFile, &drawrect);//��ʸ��ͼ��dc
     DeleteEnhMetaFile(pvMetaFile);
   }
}
//---------------------------------------------------------------------------
void TPrintInterface::PrintImpl()
{
      if(PageCount < 1)
         return;
        HDC PrinterDC;
		int Copies;
        int From;
        int To;
//start print
        TPrintDialog* pd = new TPrintDialog(Application);
        pd->Options.Clear();
        pd->Options << poPageNums << poWarning;
        pd->FromPage = 1;
        pd->MinPage = 1;
        pd->ToPage = PageCount;
        pd->MaxPage = PageCount;
        try{
           if(!pd->Execute())
              return;
        }catch(...)
        {
           throw Exception("��ӡ����ʼ��ʧ�ܣ�");
        }
        Printers::Printer()->Copies = pd->Copies;
        Printers::Printer()->Title = "Printer ";
        From = pd->FromPage;
        To = pd->ToPage;
        Printers::Printer()->Orientation = FOrientation;
        if(PageCount > 0)
           Printers::Printer()->BeginDoc();
        PrinterDC = Printers::Printer()->Canvas->Handle;
        delete pd;
        if(PrinterDC == NULL)
           throw Exception("��ӡ����ʼ������" + IntToStr(int(GetLastError())));
       //start document
        PrinterPixelsPerInchX = GetDeviceCaps(PrinterDC, LOGPIXELSX);
        PrinterPixelsPerInchY = GetDeviceCaps(PrinterDC, LOGPIXELSY);
        PrinterPhysicalOffSetX = GetDeviceCaps(PrinterDC, PHYSICALOFFSETX);
        PrinterPhysicalOffSetY = GetDeviceCaps(PrinterDC, PHYSICALOFFSETY);
        InitPrintPage();
        try{//print pages
           for(int i = From-1; i < To;)
           {
              PrintAPage(PrinterDC, i);//��ӡһҳ
              i++;
              if(i < To)
                 Printers::Printer()->NewPage();
           }
        }catch(...)
        {
           AbortDoc(PrinterDC);
           throw Exception("��ӡʧ�ܣ�");
        }
        Printers::Printer()->EndDoc();

}
//---------------------------------------------------------------------------
void __fastcall TPrintInterface::InitPrintPage()
{
   iLeftPageMargin= double(FLeftMarginMm * 0.03937 * SPixelsPerInch)+PrinterPhysicalOffSetX*SPixelsPerInch/PrinterPixelsPerInchX;
   iRightPageMargin= double(FRightMarginMm * 0.03937 * SPixelsPerInch)+PrinterPhysicalOffSetX*SPixelsPerInch/PrinterPixelsPerInchX;
   iFullPageWidth= double(FPageWidthMm*0.03937*SPixelsPerInch);
   iPageWidth=iFullPageWidth-iLeftPageMargin-iRightPageMargin;
   if(!FTitle.IsEmpty())
      iTopPageMargin=double((FTopMarginMm+10)*0.03937*SPixelsPerInch)+PrinterPhysicalOffSetY*SPixelsPerInch/PrinterPixelsPerInchY;
   else
      iTopPageMargin=double(FTopMarginMm*0.03937*SPixelsPerInch)+PrinterPhysicalOffSetY*SPixelsPerInch/PrinterPixelsPerInchY;
   if(FShowPageNo)
       iBottomPageMargin=double((FBottomMarginMm+5)*0.03937*SPixelsPerInch)+PrinterPhysicalOffSetY*SPixelsPerInch/PrinterPixelsPerInchY;
   else
       iBottomPageMargin=double(FBottomMarginMm*0.03937*SPixelsPerInch)+PrinterPhysicalOffSetY*SPixelsPerInch/PrinterPixelsPerInchY;
   iFullPageHeight=double(FPageHeightMm*0.03937*SPixelsPerInch);
   iPageHeight= iFullPageHeight-iTopPageMargin-iBottomPageMargin;
}
//---------------------------------------------------------------------------
void __fastcall TPrintInterface::PreviewAPage(HDC dc,int PageNo,RECT cr)
{
PreviewAPageImpl(dc,PageNo,cr);
}
//---------------------------------------------------------------------------
void __fastcall TPrintInterface::PrintAPage(HDC dc,int PageNo)
{
PrintAPageImpl(dc,PageNo);
}
//---------------------------------------------------------------------------
