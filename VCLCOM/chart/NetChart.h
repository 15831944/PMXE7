//---------------------------------------------------------------------------
#ifndef NetChartH
#define NetChartH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>                         
#include <Forms.hpp>                           
#include "ProjectStruct.h"
#include "ZChartDraw.h"
#include "NavigationImage.h"
using namespace ProjectStruct;
//---------------------------------------------------------------------------
#define TPrintHandle void*
class PACKAGE TNCProject;
class PACKAGE TNetChart;

enum TNCShowZoom {
     ncszSmall8 = 1, //�˷�֮һ
     ncszSmall4 = 2, //�ķ�֮һ
     ncszSmall2 = 4, //����֮һ
     ncszNormal = 8, //����
     ncszLarge2 = 16, //����
     ncszLarge4 = 32, //�ı�
     ncszLarge8 = 64 //�˱�
};

class PACKAGE TNetChart : public TWinControl
{
    friend TNCProject;
private:
	int FTaskWidth, FTaskHeight, FColDistance, FRowDistance;
	TNCShowZoom FShowZoom;
	TPopupMenu* FTaskPopMenu, *FTaskLinkPopMenu, *FNoneTaskPopMenu;
	void __fastcall SetTaskWidth(int AWidth);
	void __fastcall SetColDistance(int AWidth);
	void __fastcall SetRowDistance(int AHeight);
	void __fastcall SetShowZoom(TNCShowZoom Zoom);
	int __fastcall GetTaskWidth();
	int __fastcall GetTaskHeight();
	int __fastcall GetColDistance();
	int __fastcall GetRowDistance();
	TTaskEvent FOnTaskSelect, FOnTaskClick, FOnTaskDblClick;
	TPrintHandle FPrintHandle;
    TPrintHandle __fastcall GetPrintHandle();
    int __fastcall GetTotalHeight();                
    int __fastcall GetTotalWidth();

    SCROLLINFO FHORZ;
    SCROLLINFO FVERT;
    TNCProject * FProject;
    int CalcTaskHeight(HDC dc);
    HDC GetDrawDc();
    TProject* __fastcall GetProject();
    void __fastcall SetProject(TProject*AProject);
    MESSAGE void __fastcall WMHScroll(Messages::TMessage &Message);
    MESSAGE void __fastcall WMVScroll(Messages::TMessage &Message);
    MESSAGE void __fastcall WMKeyDown(Messages::TWMKey &Message);
    MESSAGE void __fastcall WMKeyUp(Messages::TWMKey &Message);
    MESSAGE void __fastcall WMPaint(Messages::TWMPaint &Message);
    MESSAGE void __fastcall WMDblClick(Messages::TMessage &Message);
protected:
    void __fastcall PaintWindow(HDC DC);
    DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall Click(void);
    DYNAMIC void __fastcall AdjustSize(void);
    DYNAMIC void __fastcall Resize(void);
public:
    TNavigationImage * NaviImage;////////

    void __fastcall AdjustOnFrameMove(int X, int Y);/////////////
    void __fastcall ShowNavigation();/////////////
	void __fastcall FindTaskByName(const String TaskName);////////
    __fastcall TNetChart(TComponent* Owner);
    __fastcall ~TNetChart();
    void AdjustScrollBar(bool Force = false);
    void Draw(HDC dc, RECT dr);
    void AutoAlign();
    void AlignLines();
    void SetDefault();
    
	__property TProject* Project = {read=GetProject, write=SetProject};
    __property TNCProject* NCProject = {read=FProject};
    __property TPrintHandle PrintHandle = {read=GetPrintHandle};
    __property int TotalHeight = {read=GetTotalHeight};
    __property int TotalWidth = {read=GetTotalWidth};
    __property int TaskWidth = {read=GetTaskWidth, write=SetTaskWidth};
    __property int TaskHeight = {read=GetTaskHeight};
    __property int ColDistance = {read=GetColDistance, write=SetColDistance};
    __property int RowDistance = {read=GetRowDistance, write=SetRowDistance};
    __property TNCShowZoom ShowZoom = {read=FShowZoom, write=SetShowZoom};
    TFrameDraw*  CriticalNotStartDraw();
    TFrameDraw*  CriticalStartedDraw();
    TFrameDraw*  CriticalOveredDraw();
    TFrameDraw*  NotStartDraw();
    TFrameDraw*  StartedDraw();
    TFrameDraw*  OveredDraw();
    TFrameDraw*  AbstractDraw();//ժҪ����
    TFrameDraw*  MarkerDraw();//��̱�
    TFrameDraw*  WarningDraw();//����������
    TFrameDraw*  ForeignNotStartDraw();//�ⲿδ����
    TFrameDraw*  CriticalForeignNotStartDraw();//�ⲿδ����
    TFrameDraw*  ForeignStartedDraw();//�ⲿ�ѿ���
    TFrameDraw*  CriticalForeignStartedDraw();//�ⲿ�ѿ���
    TFrameDraw*  ForeignOveredDraw();//�ⲿ���깤
    TFrameDraw*  CriticalForeignOveredDraw();//�ⲿ���깤
    TFrameDraw*  ForeignAbstractDraw();//�ⲿժҪ����
    TFrameDraw*  ForeignMarkerDraw();//�ⲿ��̱�
    TFrameDraw*  ForeignProjectDraw();//�ⲿ����

    TNCLinkDraw* CriticalLinkDraw();
    TNCLinkDraw* LinkDraw();
#pragma option -w-inl
BEGIN_MESSAGE_MAP
      VCL_MESSAGE_HANDLER(WM_KEYUP, TWMKeyUp, WMKeyUp);
      VCL_MESSAGE_HANDLER(CN_KEYDOWN, TWMKey, WMKeyDown);
      VCL_MESSAGE_HANDLER(CN_SYSKEYDOWN, TWMKey, WMKeyDown);
      VCL_MESSAGE_HANDLER(WM_HSCROLL, TMessage, WMHScroll);
      VCL_MESSAGE_HANDLER(WM_VSCROLL, TMessage, WMVScroll);
      VCL_MESSAGE_HANDLER(WM_PAINT, TWMPaint, WMPaint);
      VCL_MESSAGE_HANDLER(WM_LBUTTONDBLCLK, TMessage, WMDblClick);
END_MESSAGE_MAP(TWinControl);
#pragma option -winl
	__published:
	__property Align;
	__property Color;
	__property Font;
	__property TPopupMenu* TaskPopMenu = {read=FTaskPopMenu, write=FTaskPopMenu};
	__property TPopupMenu* TaskLinkPopMenu = {read=FTaskLinkPopMenu, write=FTaskLinkPopMenu};
	__property TPopupMenu* NoneTaskPopMenu = {read=FNoneTaskPopMenu, write=FNoneTaskPopMenu};

	__property TTaskEvent OnTaskSelect = {read=FOnTaskSelect, write=FOnTaskSelect};
	__property TTaskEvent OnTaskClick = {read=FOnTaskClick, write=FOnTaskClick};
	__property TTaskEvent OnTaskDblClick = {read=FOnTaskDblClick, write=FOnTaskDblClick};
};
//---------------------------------------------------------------------------
#endif
