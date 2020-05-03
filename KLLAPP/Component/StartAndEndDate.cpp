//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "StartAndEndDate.h"
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TStartAndEndDateFrame *StartAndEndDateFrame;
//---------------------------------------------------------------------------
__fastcall TStartAndEndDateFrame::TStartAndEndDateFrame(TComponent* Owner)
	: TFrame(Owner)
{
	FormatSettings=new TFormatSettings();
	FormatSettings->ShortDateFormat="yyyy-MM-dd";
	FormatSettings->DateSeparator='-';
	FormatSettings->TimeSeparator=':';
	FormatSettings->LongTimeFormat="hh:mm:ss.zzz";
	deStart->Date=Date();
	deEnd->Date=deStart->Date;
    deStart->Enabled=false;
    deEnd->Enabled=false;

}
//---------------------------------------------------------------------------
__fastcall TStartAndEndDateFrame::~TStartAndEndDateFrame()
{
	delete FormatSettings;
}
String __fastcall TStartAndEndDateFrame::GetBeginDate()
{
	   return deStart->Date.FormatString("yyyy-mm-dd");
}
//---------------------------------------------------------------------------
void __fastcall TStartAndEndDateFrame::SetBeginDate(String Value)
{
		deStart->Date = StrToDate(Value,*FormatSettings);
}
//---------------------------------------------------------------------------
String __fastcall TStartAndEndDateFrame::GetEndDate()
{
	   return deEnd->Date.FormatString("yyyy-mm-dd");
}
//---------------------------------------------------------------------------
void __fastcall TStartAndEndDateFrame::SetEndDate(String Value)
{
		deEnd->Date = StrToDate(Value,*FormatSettings);
}
//---------------------------------------------------------------------------
void __fastcall TStartAndEndDateFrame::cbDateRangeClosePopup(TObject *Sender)
{
  TDateTime dtNow;
//  TZClientQuery *FQuery;
  int iWeek,iDays;
  Word Year,Month,Day;
  switch(cbDateRange->ItemIndex)
  {
    case 0://����
      deStart->Text=Date();
      deEnd->Text=deStart->Text;
      break;
    case 1://����
      dtNow=Date();
	  iWeek=dtNow.DayOfWeek();
	  deStart->Date=(dtNow-iWeek+1);
	  deEnd->Date=(dtNow+7-iWeek);
      break;
    case 2://����������
      dtNow=Date();
      iWeek=dtNow.DayOfWeek();
	  deStart->Date=(dtNow-iWeek+1);
	  deEnd->Date=dtNow;
      break;
    case 3://����
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
	  deStart->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-01",*FormatSettings);
	  deEnd->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(GetMonthDays(Year,Month)),*FormatSettings);
      break;
    case 4://��������
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
	  deStart->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-01",*FormatSettings);
	  deEnd->Date=dtNow;
      break;
	case 5://������
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      if(Month>=1 && Month<=3)//��һ����
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-03-31",*FormatSettings);
      }
      else if(Month>=4 && Month<=6)//�ڶ�����
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-04-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-06-30",*FormatSettings);
      }
      else if(Month>=7 && Month<=9)
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-07-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-09-30",*FormatSettings);
      }
      else
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-10-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-12-31",*FormatSettings);
      }
      break;
    case 6://����������
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      if(Month>=1 && Month<=3)//��һ����
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
      }
      else if(Month>=4 && Month<=6)//�ڶ�����
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-04-01",*FormatSettings);
      }
      else if(Month>=7 && Month<=9)
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-07-01",*FormatSettings);
      }
      else
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-10-01",*FormatSettings);
      }
	  deEnd->Date=dtNow;
      break;
    case 7://����
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
	  deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
	  deEnd->Date=StrToDate(IntToStr(Year)+"-12-31",*FormatSettings);
      break;
    case 8://��������
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
	  deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
	  deEnd->Date=dtNow;
      break;
    case 9://����
      dtNow=Date();
	  deStart->Date=(dtNow-1);
	  deEnd->Date=deStart->Date;
      break;
    case 10://����
      dtNow=Date();
      iWeek=dtNow.DayOfWeek();
	  deStart->Date=(dtNow-iWeek-7+1);
	  deEnd->Date=(dtNow+7-iWeek-7);
      break;
    case 11://���������ܽ���
      dtNow=Date();
      iWeek=dtNow.DayOfWeek();
	  deStart->Date=(dtNow-iWeek-7+1);
	  deEnd->Date=(dtNow-7);
      break;
    case 12://����
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      Month--;
      if(Month<1)
      {
        Year--;
        Month=12;
      }
	  deStart->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-01",*FormatSettings);
	  deEnd->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(GetMonthDays(Year,Month)),*FormatSettings);
      break;
    case 13://���������½���
      dtNow=Date();
	  DecodeDate(dtNow,Year,Month,Day);
      Month--;
      if(Month<1)
      {
        Year--;
        Month=12;
      }
	  deStart->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-01",*FormatSettings);
      iDays=GetMonthDays(Year,Month);
      if(iDays<Day)
        Day=iDays;
	  deEnd->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day),*FormatSettings);
      break;
    case 14://�ϼ���
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      if(Month>=1 && Month<=3)//��һ����
      {
        Year--;
		deStart->Date=StrToDate(IntToStr(Year)+"-10-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-12-31",*FormatSettings);
      }
      else if(Month>=4 && Month<=6)//�ڶ�����
	  {
		deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-03-31",*FormatSettings);
      }
      else if(Month>=7 && Month<=9)//��������
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-04-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-06-30",*FormatSettings);
	  }
      else//���ļ���
	  {
		deStart->Date=StrToDate(IntToStr(Year)+"-07-01",*FormatSettings);
		deEnd->Date=StrToDate(IntToStr(Year)+"-09-30",*FormatSettings);
      }
      break;
    case 15://�ϼ������ϼ��Ƚ���
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      if(Month>=1 && Month<=3)//��һ����
      {
        Year--;
        Month=Month+12-3;
		deStart->Date=StrToDate(IntToStr(Year)+"-10-01",*FormatSettings);
      }
      else if(Month>=4 && Month<=6)//�ڶ�����
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
        Month-=3;
      }
      else if(Month>=7 && Month<=9)//��������
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-04-01",*FormatSettings);
        Month-=3;
      }
      else//���ļ���
      {
		deStart->Date=StrToDate(IntToStr(Year)+"-07-01",*FormatSettings);
        Month-=3;
      }
      iDays=GetMonthDays(Year,Month);
      if(iDays<Day)
          Day=iDays;
	  deEnd->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day),*FormatSettings);
      break;
    case 16://ȥ��
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      Year--;
	  deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
	  deEnd->Date=StrToDate(IntToStr(Year)+"-12-31",*FormatSettings);
      break;
    case 17://ȥ����ȥ�����
      dtNow=Date();
      DecodeDate(dtNow,Year,Month,Day);
      Year--;
	  deStart->Date=StrToDate(IntToStr(Year)+"-01-01",*FormatSettings);
	  deEnd->Date=StrToDate(IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day),*FormatSettings);
			break;
		/* //ȡ����������,Ŀǰֻ��EPM��ʹ��
    case 18://��������
    {
      FQuery=FDataSet->Query;
      FQuery->Close();
      FQuery->SQL->Clear();
      FQuery->SQL->Add(" declare @UserYear varchar(255),@UserMonth varchar(255) ");
      FQuery->SQL->Add(" set @UserYear=0 ");
      FQuery->SQL->Add(" set @UserMonth=0 ");
      FQuery->SQL->Add(" SELECT @UserYear=isnull(SysParamValue,0) from tbSysParameter where SysParamPKID='UserYear'");
      FQuery->SQL->Add(" SELECT @UserMonth=isnull(SysParamValue,0) from tbSysParameter where SysParamPKID='UserMon'");
      FQuery->SQL->Add(" select UserYear=@UserYear,UserMonth=@UserMonth ");
      FQuery->Open();
      if(FQuery->FieldByName("UserYear")->AsString=="0" ||
          FQuery->FieldByName("UserYear")->AsString=="0" )
      {
        deStart->Text="";
        deEnd->Text="";
      }
      else
      {
        deStart->Text=FQuery->FieldByName("UserYear")->AsString+"-"+
                      FQuery->FieldByName("UserMonth")->AsString+
                      "-01";
        deEnd->Text=Date();
      }
	  FQuery->Close();
      break;
		}
		*/
    default://�Զ���
      break;
      //deStart->Text="";
      //deEnd->Text="";
  }

	//����ؼ�״̬
	//�Զ���Ϊԭ����19��ȡ��'�ӿ�������',��˸�Ϊ18
  if(cbDateRange->ItemIndex==18)
  {
    deStart->Enabled=true;
    deEnd->Enabled=true;
  }
  else
  {
    deStart->Enabled=false;
    deEnd->Enabled=false;
  }

}
//---------------------------------------------------------------------------
int __fastcall TStartAndEndDateFrame::GetMonthDays(Word Year,Word Month)
{
  int iMonth;

	int iMonth1[] = {31,31,28,31,30,31,30,31,31,30,31,30,31};
  int iMonth2[]  = {31,31,29,31,30,31,30,31,31,30,31,30,31};
  if((Year%4==0)&&(Year%100 != 0 || Year%400==0))
  	iMonth = iMonth2[Month];
  else
  	iMonth = iMonth1[Month];

  return iMonth;
}
//---------------------------------------------------------------------------
