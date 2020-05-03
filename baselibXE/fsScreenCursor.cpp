//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "fsScreenCursor.h"
#include <FMX.Forms.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)
void __fastcall TfsScreenCursor::SetCurCursor(TCursor Value)
{
   Application->MainForm->Cursor=Value;  //����ʱ�޸�,������ʵ��
}
//---------------------------------------------------------------------------
TCursor __fastcall TfsScreenCursor::GetCurCursor()
{
   return Application->MainForm->Cursor;
}
//---------------------------------------------------------------------------
__fastcall TfsScreenCursor::TfsScreenCursor()
{
   SaveCursor=Application->MainForm->Cursor;   //���浱ǰ���
   Application->MainForm->Cursor=crHourGlass;  //���õȴ����
}
//---------------------------------------------------------------------------
__fastcall TfsScreenCursor::~TfsScreenCursor()
{
   Application->MainForm->Cursor=SaveCursor; //�ָ����
}
//---------------------------------------------------------------------------
