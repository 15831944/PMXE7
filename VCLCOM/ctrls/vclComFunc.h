//---------------------------------------------------------------------------

#ifndef vclComFuncH
#define vclComFuncH
#include <Graphics.hpp>
//---------------------------------------------------------------------------
//   ������
//---------------------------------------------------------------------------
namespace Vclcomfunc{

void WriteInt(TStream* Stream, const int Value);
int ReadInt(TStream* Stream);
void WriteDouble(TStream* Stream, const double Value);
double ReadDouble(TStream* Stream);
String ReadString(TStream* Stream);
void WriteString(TStream* Stream, String AValue);
void WriteStrings(TStream* Stream, TStrings* AValue);
void ReadStrings(TStream* Stream, TStrings* AValue);
//--------------------------------------------------------------------
void Byte2ToChar3(BYTE*b, char*c);
void Char3ToByte2(char*c, BYTE*b);
char* StreamToString(TStream*AStream, int * R_Length);
void StringToStream(TStream*AStream, char* AString);
void WarpTextOut(String Text, TRect ARect, TCanvas* ACanvas);
bool MouseIn(POINT AP, TRect AR);
int PACKAGE TextHeight(HDC dc, String Text);
int PACKAGE TextWidth(HDC dc, String Text);
//---------------------------------------------------------------------
//---------------------------------------------------------------------------
//   ͨ��TFont��Ϣ����Font
//---------------------------------------------------------------------------
HFONT  CreateFontByTFont(TFont* AFont)
{
  LOGFONT pFont;
  //��������
  HFONT hFont;
  pFont.lfHeight = AFont->Height; ;
  pFont.lfWidth = 0;
  pFont.lfEscapement = 0;
  pFont.lfOrientation = 0;
  if(AFont->Style.Contains(fsBold))
	 pFont.lfWeight = FW_BOLD;
  else
	 pFont.lfWeight = FW_NORMAL;
  pFont.lfItalic = AFont->Style.Contains(fsItalic);
  pFont.lfUnderline = AFont->Style.Contains(fsUnderline);
  pFont.lfStrikeOut = AFont->Style.Contains(fsStrikeOut);
  pFont.lfCharSet = AFont->Charset;
  wcscpy(pFont.lfFaceName, AFont->Name.c_str());
  pFont.lfQuality = DEFAULT_QUALITY;
  pFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
  pFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  switch(AFont->Pitch)
  {
	 case TFontPitch::fpVariable:
		pFont.lfPitchAndFamily = VARIABLE_PITCH;
		break;
	 case TFontPitch::fpFixed:
		pFont.lfPitchAndFamily = FIXED_PITCH;
		break;
	 default:
		pFont.lfPitchAndFamily = DEFAULT_PITCH;
  }
  hFont = CreateFontIndirect(&pFont);
  return hFont;
}
/*
ջ�ڴ����������ٶ�ԶԶ���ڶ��ڴ棬������ClassTypeΪ���ͣ�
MinSizeΪ������С���������ջ��С��������������ڴ�
*/
template<class ClassType, int MinSize>
class StackArray
{
private:
     ClassType* __Pointer__;
     ClassType  __Array__[MinSize];
     bool       MustDelete;
     StackArray();
     StackArray& operator = (const StackArray&);
     StackArray(const StackArray&AArray);
public:
     StackArray(int Size)
     {
		 if(Size <= MinSize)
         {
             MustDelete = false;
             __Pointer__ = __Array__;
         }else
         {
             __Pointer__ = new ClassType[Size];
             MustDelete = true;
         }
     }
     ~StackArray()
     {
         if(MustDelete)
             delete [] __Pointer__;
     }
     ClassType& operator[](const int Index)
     {
         return __Pointer__[Index];
     }
     operator ClassType* ()
     {
         return __Pointer__;
     }
};
class TItemObject{
private:
      Variant varObj;
public:
		__fastcall TItemObject();
        __fastcall ~TItemObject();
        void __fastcall PutElement(Variant var, int index);
        Variant __fastcall GetElement(int index);

};
/*
  ���ָ��ļ��ϣ��и��ֲ���
  OnDelete:�����Ҫ�ڶ����ַɾ��ʱɾ��
   �ƶ��ڶ�������ʱ�Ƿ�ɾ�������еĶ��󣬺�������ʱ����itemָ��,
*/
typedef void (*PointerArrayOnDelete)(void*);
class PACKAGE TPointerArrayBase
{
private:
	 void    IncCapability();
	 void    DecCapability();
protected:
	 void ** FPoints;
	 int     FCount;
	 int     FCapability;
	 PointerArrayOnDelete        FOnDelete;
	 int     FMinCapability;
	 int     FCapabilityInc;

	 TPointerArrayBase(PointerArrayOnDelete, int, int);
	 virtual ~TPointerArrayBase();
	 PointerArrayOnDelete SetOnDelete(PointerArrayOnDelete);
	 int   Count();
	 int   Add(void*);
	 void  Insert(void*, int);
	 void  DeleteByIndex(int);
	 int   DeleteByItem(void*);
	 void  RemoveByIndex(int);
	 int   RemoveByItem(void*);
	 int   IndexOf(void*);
	 void  Clear();
     void  Assign(const TPointerArrayBase&);
};
template< class ItemType, int MinCapability = 64, int CapabilityInc = 32 >
class TPointerArray : private TPointerArrayBase
{
private:
public:
	 TPointerArray(PointerArrayOnDelete OnDelete)
              :TPointerArrayBase(OnDelete, MinCapability, CapabilityInc){}
     TPointerArray():TPointerArrayBase(NULL, MinCapability, CapabilityInc){}
     TPointerArray(const TPointerArray&AArray): TPointerArrayBase(false)
     {
		  TPointerArrayBase::Assign(AArray);
     }
     PointerArrayOnDelete SetOnDelete(PointerArrayOnDelete AValue){ return TPointerArrayBase::SetOnDelete(AValue); }
     ItemType& operator[](const int Index)
     {
          return *((ItemType*)(TPointerArrayBase::FPoints[Index]));
     }
	 TPointerArray& operator = (const TPointerArray&AArray)
     {
          TPointerArrayBase::Assign(AArray);
          return *this;
     }
     int  Count()                          { return TPointerArrayBase::FCount; }
     int  Add(ItemType*AItem)              { return TPointerArrayBase::Add(AItem); }
     void Insert(ItemType*AItem, int Index){ TPointerArrayBase::Insert(AItem, Index); }
     void Delete(int Index)                { TPointerArrayBase::DeleteByIndex(Index); }
     int  Delete(ItemType&AItem)           { return TPointerArrayBase::DeleteByItem(&AItem); }
     void Remove(int Index)                { TPointerArrayBase::RemoveByIndex(Index); }
     int  Remove(ItemType&AItem)           { return TPointerArrayBase::RemoveByItem(&AItem); }
     int  IndexOf(ItemType&AItem)          { return TPointerArrayBase::IndexOf(&AItem); }
     void Clear()                          { TPointerArrayBase::Clear(); }
};


}
#endif
