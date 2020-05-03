//---------------------------------------------------------------------------

#ifndef RoomGridH
#define RoomGridH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Graphics.hpp>
const String RoomGridSettingKey="RoomGridSetting"; //Ĭ��ע��key,����Ӱ������Ӧ��
//---------------------------------------------------------------------------
struct TSelRange  //��Ԫѡȡ�ķ�Χ
{
    int StartFloor;  //��ʼ¥��
    int EndFloor;    //����¥��
    int StartUnit;   //��ʼ��Ԫ��
	int EndUnit;     //������Ԫ��
};

struct TCells
{
  TCells():Status(6){DoubleLays=0;Selected=false;LeaseStatus=6;
    Code="";Title="";Detail="";HouseType="";Direction="";
    Area="";UseArea="";RoofGargdenArea="";Sight="";Fitment="";
    UpArea="";DownArea="";ApportionArea="";UnitPrice="";TotalCost="";
    UnitLeaseCost="";LeaseCost="";CanLease="";SaleArea="";PreSaleArea="";
    RightArea="";
  };
  String Code;  //���
  String Title;  //��������ʾ
  String Detail;  // ϸ������ʾ
  String HouseType;//����
  String Direction; //����
  String Area;      //�������
  String UseArea;   //�������
  String SaleArea; //�������
  String PreSaleArea; //Ԥ�����
  String RightArea; //��Ȩ���
  String RoofGargdenArea;//ƽ̨���
  String Sight;     //����
  String Fitment;   //װ��
  String UpArea;    //�ϲ����
  String DownArea;  //�²����
  String ApportionArea; //��̯���
  String UnitPrice; //����
  String TotalCost; //��Ԫ�ܼ�
  int DoubleLays;       //�Ƿ�ʽ
  int Status;   //��Ԫ״̬
  bool Selected; //�Ƿ�ѡ��
  String UnitLeaseCost; //��λ�����
  String LeaseCost; //�����
  String CanLease; //�Ƿ�ɳ���(��Էʹ��)
  int LeaseStatus;   //����״̬
};

class PACKAGE TRoomGridSetting  //���ر����ã����Ե���THouseGrid:Setup(key)��������
{
public:
    bool AutoFill;  //��Ԫ��߶��Զ�����
    bool DisplaySight; //��ʾ����
    bool DisplayFitment; //��ʾװ��
    bool DisplayDirection; //��ʾ����
    bool DisplayArea;      //��ʾ��������� �����Ԫ�Ǹ�ʽ����ʾ���²����
    bool DisplayUseArea;  //��ʾ��������� �����Ԫ�Ǹ�ʽ����ʾ���²����
    bool DisplaySaleArea; //�������
    bool DisplayRightArea; //��Ȩ���
    bool DisplayPreSaleArea; //Ԥ�����

    bool DisplayRoofGarden;//��ʾƽ̨���
    bool DisplayApportionArea; //��̯���
    bool DisplayHouseType; //��ʾ����
    bool DisplayUnitPrice;  //��Ԫ����
    bool DisplayTotalCost;  //��Ԫ�ܼ�
    bool DisplayDetail;    //��ʾ���ܣ���ǰ��ϸ������
    int  PrintAlign;       //�������з�ʽ
    int  RowHeight;        //��Ԫ��߶ȣ���AutoFill=falseʱ��Ч
    int  ColWidth;         //��Ԫ����
    bool DisplayUnitLeaseCost; //��λ�����
    bool DisplayLeaseCost; //�����
    bool DisplayCanLease; //�Ƿ�ɳ���
};
class PACKAGE TRoomGrid : public TCustomGrid
{
private:
        int FFloorCount;  //¥����
        int FUnitCount;   //ÿ�㻧��
        //String FKeyField;  //���ݻ���Ĺؼ��ֶ�
        bool FIsUnit;  //�Ƿ��ǵ�Ԫ��
        int __fastcall RowToFloor(int ARow); //������תΪ¥����
        TFont *FTitleFont;  //������������
        TFont *FDetailFont; //ϸ����������
        TStringList* FFloorNames;
        bool FDisplaySight;
        bool FDisplayFitment;
        bool FDisplayDirection;
        bool FDisplayArea;
        bool FDisplayHouseType;
        bool FDisplayDetail;
        bool FUnitAutoFill;
        bool FDisplayUseArea;
        bool FDisplaySaleArea;
        bool FDisplayRightArea;
        bool FDisplayPreSaleArea;
        bool FDisplayUnitPrice;
        bool FDisplayTotalCost;
        bool FDisplayRoofGarden;
        bool FDisplayApportionArea;
        bool FDisplayUnitLeaseCost; //��λ�����
        bool FDisplayLeaseCost; //�����
        bool FDisplaySale; //true��ʾ����״̬,false��ʾ����״̬
        bool FDisplayCanLease;
        int FPrintAlign;
        int FFloorNameWidth;
        //bool FValid;  //ѡ�еĵ�Ԫ���Ƿ���Ч
        //TPoint __fastcall CenterPos(TRect ARect, String ShowText);
        void __fastcall SetFloorCount(int Value);
        void __fastcall SetUnitCount(int Value);
        int __fastcall GetFloor();
        int __fastcall GetUnit();
        void __fastcall SetFloor(int Value);
        void __fastcall SetUnit(int Value);
        void __fastcall SetKeyField(String Value);
        void __fastcall SetIsUnit(bool Value);
        TSelRange __fastcall GetSelRange();
        int __fastcall GetColWidth();
        int __fastcall GetRowHeight();
        void __fastcall SetColWidth(int Value);
        void __fastcall SetRowHeight(int Value);
        void __fastcall SetTitleFont(TFont *Value);
        void __fastcall SetDetailFont(TFont *Value);
        void __fastcall SetDisplaySight(bool value);
        void __fastcall SetDisplayFitment(bool value);
        void __fastcall SetDisplayDirection(bool value);
        void __fastcall SetDisplayArea(bool value);
        void __fastcall SetDisplayHouseType(bool value);
        void __fastcall SetDisplayDetail(bool value);
        void __fastcall SetUnitAutoFill(bool value);
        void __fastcall SetGridHeightAndWidth();
        void __fastcall SetDisplayUseArea(bool value);
        void __fastcall SetDisplaySaleArea(bool value);
        void __fastcall SetDisplayPreSaleArea(bool value);
        void __fastcall SetDisplayRightArea(bool value);
        void __fastcall SetDisplayUnitPrice(bool value);
        void __fastcall SetDisplayTotalCost(bool value);
        void __fastcall SetDisplayRoofGarden(bool value);
        void __fastcall SetDisplayApportionArea(bool value);
        void __fastcall SetDisplayUnitLeaseCost(bool value);
        void __fastcall SetDisplayLeaseCost(bool value);
        void __fastcall SetDisplaySale(bool value);
        void __fastcall SetDisplayCanLease(bool value);
        void __fastcall SetPrintAlign(int Value);
        void __fastcall SetFloorNameWidth(int value);
        int __fastcall GetFloorNameWidth();
        bool __fastcall GetCellSelected(int ACol, int ARow);
        void __fastcall SetCellSelected(int ACol, int ARow,bool Selected);
        TColor __fastcall GetDisplayColor(int Status);
        void __fastcall SetDisplayColor(int Status,TColor newColor);

protected:
        virtual void __fastcall DrawCell(int ACol, int ARow, const TRect &ARect, Grids::
                                         TGridDrawState AState);
		void __fastcall DrawCellItem(HDC CanvasHandle,String sDrawText
                          ,TRect &DrawRect,UINT dpAlign);
        MESSAGE void __fastcall WMEraseBkGnd(Messages::TMessage &Message);
        DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
        DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
public:

        __fastcall TRoomGrid(TComponent* Owner);
       __fastcall ~TRoomGrid();
        TCells *UnitRecord;  //�ڸ���Ԫ�������
        TCells *GetCellData(int ACol, int ARow);
        String GetCellString(int ACol, int ARow);
        void __fastcall ReadSetting(String Key = RoomGridSettingKey);  //����Ա���Գ�ʼ���Լ�������
        void __fastcall SaveSetting(String Key = RoomGridSettingKey);  //�����Լ������ã�������ÿ��������
                                                                     //��ͬ��ע���keyֵ
        void __fastcall SelectCells(int StartR, int EndR, int StartC,int EndC);
        void __fastcall ClearSelected();
        int __fastcall GetStatus(TCells * cell);
        __property TColor DisplayColor[int]={read=GetDisplayColor,write=SetDisplayColor};

        __property bool CellSelected[int][int]={read=GetCellSelected,write=SetCellSelected};

        //TBufferDataSet *Buffer;  //�������ݻ���
        __property int Floor={read=GetFloor,write=SetFloor}; //��ǰ��Ԫ��¥��
        __property int Unit={read=GetUnit,write=SetUnit};  //��ǰ��Ԫ�����
        //__property String KeyField={read=FKeyField,write=SetKeyField};  //���ݼ��Ĺؼ��ֶ�
        __property TSelRange SelRange={read=GetSelRange};
        __property bool IsUnit={read=FIsUnit,write=SetIsUnit,default=true};//�Ƿ�����ʾ��Ԫ��
        //__property bool Valid={read=FValid};
        //String DetailField; //��ǰ������Ӧ��ʾ���ֶ�
        //String TitleField;  //��ǰ������Ӧ��ʾ���ֶ�
        //String StateField;  //��ǰÿ����Ԫ��Ӧ��ʾ��״̬
        TColor TitleColor;    //����������ɫ
        TColor DispColor[10];  //��Ԫ�����ɫ��1-δ����2-Ԥ����3-�Ѷ���4-���ۡ�5-���⡢6-11������
		void __fastcall Setup(String Key = RoomGridSettingKey); //�������ô���,�������ò�����KeyΪ�ؼ�ֵ���浽ע���
        int UnitTitleHeight;//��ӡҪ���ʣ����ű��Ҫ��
        int StepLineHeight; //��ӡҪ���ʣ���ӡ��������ÿ�и߶�
        __property ColWidths;
        __property RowHeights;
        __property FixedRows;
        __property FixedCols;
        __property ColCount;
        __property RowCount;
        __property TStringList *FloorNames  = { read=FFloorNames, write=FFloorNames };//¥���������б��ӵ�1���ź������
        __property bool DisplaySight  = { read=FDisplaySight, write=SetDisplaySight };
        __property bool DisplayFitment  = { read=FDisplayFitment, write=SetDisplayFitment };
        __property bool DisplayDirection  = { read=FDisplayDirection, write=SetDisplayDirection };
        __property bool DisplayArea  = { read=FDisplayArea, write=SetDisplayArea };
        __property bool DisplayHouseType  = { read=FDisplayHouseType, write=SetDisplayHouseType };
        __property bool DisplayDetail  = { read=FDisplayDetail, write=SetDisplayDetail };
        __property bool UnitAutoFill  = { read=FUnitAutoFill, write=SetUnitAutoFill };
        __property bool DisplayUseArea  = { read=FDisplayUseArea, write=SetDisplayUseArea };
        __property bool DisplaySaleArea  =    { read=FDisplaySaleArea, write=SetDisplaySaleArea };
        __property bool DisplayPreSaleArea  = { read=FDisplayPreSaleArea, write=SetDisplayPreSaleArea };
        __property bool DisplayRightArea  =   { read=FDisplayRightArea, write=SetDisplayRightArea };
        __property bool DisplayUnitPrice  = { read=FDisplayUnitPrice, write=SetDisplayUnitPrice };
        __property bool DisplayTotalCost  = { read=FDisplayTotalCost, write=SetDisplayTotalCost };
        __property bool DisplayRoofGarden  = { read=FDisplayRoofGarden, write=SetDisplayRoofGarden };
        __property bool DisplayApportionArea  = { read=FDisplayApportionArea, write=SetDisplayApportionArea };

        __property bool DisplayUnitLeaseCost  = { read=FDisplayUnitLeaseCost, write=SetDisplayUnitLeaseCost };
        __property bool DisplayLeaseCost = { read=FDisplayLeaseCost, write=SetDisplayLeaseCost };
        __property bool DisplaySale = { read=FDisplaySale, write=SetDisplaySale };
        __property bool DisplayCanLease = { read=FDisplayCanLease, write=SetDisplayCanLease };

        __property int PrintAlign  = { read=FPrintAlign, write=SetPrintAlign };
        __property int FloorNameWidth  = { read=GetFloorNameWidth, write=SetFloorNameWidth };

BEGIN_MESSAGE_MAP
      VCL_MESSAGE_HANDLER(WM_ERASEBKGND, TMessage, WMEraseBkGnd);
END_MESSAGE_MAP(TWinControl);
__published:
        __property int FloorCount={read=FFloorCount,write=SetFloorCount};
        __property int UnitCount={read=FUnitCount,write=SetUnitCount};
        __property int ColWidth={read=GetColWidth,write=SetColWidth};
        __property int RowHeight={read=GetRowHeight,write=SetRowHeight};
        __property TFont *TitleFont={read=FTitleFont,write=SetTitleFont};
        __property TFont *DetailFont={read=FDetailFont,write=SetDetailFont};
        __property Align;
        __property Anchors;
        __property BiDiMode;
        __property Color;
        __property DragCursor;
        __property DragKind;
        __property DragMode;
        __property Enabled;
        __property FixedColor;
        __property Font;
        __property Options;
        __property Visible;
        __property OnClick;
        __property OnDblClick;
        __property OnContextPopup;
        __property OnDragDrop;
        __property OnDragOver;
        __property OnEndDock;
        __property OnEndDrag;
        __property OnEnter;
        __property OnExit;
        __property OnKeyDown;
        __property OnKeyPress;
        __property OnKeyUp;
        __property OnMouseDown;
        __property OnMouseMove;
        __property OnMouseUp;
        __property OnMouseWheelDown;
        __property OnMouseWheelUp;
        __property OnStartDock;
        __property OnStartDrag;

};
//---------------------------------------------------------------------------
#endif
