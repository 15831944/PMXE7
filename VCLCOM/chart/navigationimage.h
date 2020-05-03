//---------------------------------------------------------------------------

#ifndef NavigationImageH
#define NavigationImageH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZChartDraw.h"
//---------------------------------------------------------------------------
const int BorderWidth = 10;
typedef void __fastcall (__closure *TFrameMove)(int X, int Y);
class PACKAGE TNavigationImage : public TWinControl
{
private:
    bool bMouseDowning;           //�����϶�
    TPoint ScreenPoint,OldScrPoint;//�����϶�
    RECT FFrameRect,OldFrameRect;  //������ܵĴ�Сλ��
    TFrameMove FOnFrameMove;
	RECT CtrlRect,DrawRect,SrcRect,ChartRect;//��ͼ��С,����Rect,ͼԭ��СRect,ͼ��ʾ��Rect
	SIZE ChartPosition;//ͼ�Ĺ���λ��
    HENHMETAFILE MetaFile;//ʸ��ͼ

    TWinControl *wcChart;

    double FZoomX;  //���ŵı���.
    double FZoomY;  //���ŵı���.
//    Graphics::TBitmap* FSourceBmp;
    bool FShowing;
    TTimer *trResize;

    void __fastcall ResizeOnTimer(TObject* Sender);
    void __fastcall CalFrameMove();
    void __fastcall SetZoomParameter();
    void __fastcall SetShowing(bool bShow);
    void __fastcall PaintWindow(HDC DC);
    virtual void __fastcall EraseBKGND(TMessage& Msg);
    MESSAGE void __fastcall WMPaint(Messages::TWMPaint &Message);
protected:
    bool __fastcall PointInFrameRect(int X, int Y);
    void __fastcall DrawFrame(HDC dc);

    DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall Resize(void);
    DYNAMIC void __fastcall DblClick(void);

    BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint);
        VCL_MESSAGE_HANDLER(WM_ERASEBKGND, TMessage, EraseBKGND)
    END_MESSAGE_MAP(TWinControl)
public:
    __property bool Showing = {read=FShowing,write=SetShowing};//�Ƿ�����ʾ
    __property double ZoomX = {read=FZoomX,write=FZoomX};  //���ŵı���.
    __property double ZoomY = {read=FZoomY,write=FZoomY};  //���ŵı���.
    __property  TFrameMove OnFrameMove = {read=FOnFrameMove, write=FOnFrameMove};//����ƶ��¼�
    template <class SDChart>
	void __fastcall ShowNavigationPicture(SDChart *AChart)//��ʾͼ��
	{

	  wcChart = (TWinControl *)AChart;
	  if(MetaFile)
	  {
		//ɾ��ʸ��ͼ
		DeleteEnhMetaFile(MetaFile);
		MetaFile = NULL;
	  }
	  String FileName = "NaviImage.wmf";
	  SrcRect.left = 0;
	  SrcRect.top = 0;
	  HDC tempdc = GetDC(0);
	  int iWidthMM = GetDeviceCaps(tempdc, HORZSIZE);
	  int iHeightMM = GetDeviceCaps(tempdc, VERTSIZE);
	  int iWidthPels = GetDeviceCaps(tempdc, HORZRES);
	  int iHeightPels = GetDeviceCaps(tempdc, VERTRES);
	  SrcRect.right = double(AChart->TotalWidth * iWidthMM * 100) / double(iWidthPels);
	  SrcRect.bottom = double(AChart->TotalHeight * iHeightMM * 100) / double(iHeightPels);
	  if(SrcRect.right<=SrcRect.left || SrcRect.bottom<=SrcRect.top)
	  {
		MetaFile = NULL;
	   //	this->Repaint();
	   Invalidate();
		return;
	  }
	  //����ʸ��ͼ
	  HDC MetaFileDc = CreateEnhMetaFile(tempdc, FileName.c_str(), &SrcRect, L"CPM-����ͼ");
	  ReleaseDC(0, tempdc);
	  if(MetaFileDc!=NULL)
	  {
		SrcRect.right = AChart->TotalWidth;
		SrcRect.bottom = AChart->TotalHeight;
		TPrjObject* bkSelected = AChart->Project->Selected;
		AChart->Project->Selected = NULL;

		//HBRUSH Brush = CreateSolidBrush(clWhite);
		//FillRect(MetaFileDc,&SrcRect,Brush);
        //DeleteObject(Brush);

		AChart->Draw(MetaFileDc, SrcRect);
		AChart->Project->Selected = bkSelected;
        //�ر�ʸ��ͼ
		MetaFile = CloseEnhMetaFile(MetaFileDc);
      }

	  SetZoomParameter();

	  //this->Repaint();
	  Invalidate();
	}
//------------------------------------------------------------------------------
//    template <class SDChart>
//    void __fastcall ShowNavigationImage(SDChart *AChar)//��ʾͼ��
//    {
//  //      if(FSourceBmp)
//  //        FSourceBmp->FreeImage();
//  //      delete FSourceBmp;
//  //      FSourceBmp = new Graphics::TBitmap();
//      HDC dc = GetDC(this->Handle);
//      FSourceBmp->Width = Width;
//      FSourceBmp->Height = Height;
//      RECT r;
//      r.left = 0;
//      r.right = Width;
//      r.top = 0;
//      r.bottom = Height;
//      TCanvas *cv = new TCanvas();
//      cv->Handle = dc;
//      cv->FillRect(r);
//  //      AChar->DrawNavigation(FSourceBmp->Canvas->Handle,r);
//      AChar->DrawNavigation(dc,r);
//      FSourceBmp->Canvas->CopyRect(r,cv,r);
//      ReleaseDC(this->Handle,dc);
//      delete cv;
//    }

    void __fastcall SetFrameRect(TRect R,int x,int y);
    void __fastcall SetFrameRect(int x,int y);

    __fastcall TNavigationImage(TComponent* Owner);
    __fastcall ~TNavigationImage();
};
//---------------------------------------------------------------------------
#endif
