//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ZReportbase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frxClass"
#pragma link "frxDesgn"
#pragma link "frxPreview"
#pragma link "frxDBSet"
#pragma link "ZStatusBar"
#pragma link "ZComboBox"
#pragma link "frxExportImage"
#pragma link "frxExportPDF"
#pragma link "frxExportRTF"
#pragma link "frxExportXLS"
#pragma link "frxBarcode"
#pragma link "frxChart"
#pragma link "frxChBox"
#pragma link "frxCross"
#pragma link "frxDCtrl"
#pragma link "frxGradient"
#pragma link "frxOLE"
#pragma link "frxRich"
#pragma resource "*.dfm"
TFRPBaseForm *FRPBaseForm;
const String Num[16] = {"��", "Ҽ", "��", "��", "��", "��", "½", "��",
    "��", "��", "ʰ", "��", "Ǫ", "��", "��", "��"};
//---------------------------------------------------------------------------
__fastcall TFRPBaseForm::TFRPBaseForm(TComponent* Owner)
        : TForm(Owner)
{
	FZoom = 1;
	FOnSaveReport=NULL;
	FOnUserDefineFunction=NULL;
	FOnSetFieldAliases=NULL;
	FOnGetVariables=NULL;
	FOnGetValue=NULL;
	FOnAfterExportReport=NULL;
	FOnBeforDesig=NULL;
    FMDLinkString="";
    FMDLinkString2="";
    STBBottom->Panels->Items[1]->Text = "���ű�����"+FloatToStr(FZoom*100)+"%";
	frxRpt->AddFunction("function MoneyCn(mmje: Double): String","MyFunction","����Ҵ�д");
	frxRpt->AddFunction("function GetUserName(mmje: String): String","MyFunction","�����û�����");
	frxRpt->AddFunction("function GetEmployeeName(mmje: String): String","MyFunction","����Ա������");
	frxRpt->AddFunction("function GetDeptName(mmje: String): String","MyFunction","���ز�������");
	frxRpt->AddFunction("function GetOraniztionName(mmje: String): String","MyFunction","������ص�λ����");
	frxRpt->AddFunction("function GetProjectName(mmje: String): String","MyFunction","������Ŀ����");
	frxRpt->AddFunction("function GetContractName(mmje: String): String","MyFunction","���غ�ͬ����");
}
//---------------------------------------------------------------------------
void __fastcall TFRPBaseForm::SetReportPath(String  AReportPath)
{
     if (!AReportPath.IsEmpty())
     {
       try
       {
          FReportPath = AReportPath;
          frxRpt->LoadFromFile(FReportPath);
          STBBottom->Panels->Items[3]->Text = ExtractFileName(AReportPath);
       }
       catch(...)
       {
           throw Exception("��ȡ�����ļ�ʧ��,ϵͳ��ʾ");
       }
     }
}

void __fastcall TFRPBaseForm::SetMDataSet(String  AMDataSet)
{
    if (AMDataSet != "")
    {
       FMDataSet->LoadFromFile(AMDataSet);
    }
}

void __fastcall TFRPBaseForm::SetDDataSet(String ADDataSet)
{
    if (ADDataSet != "")
    {
        FDDataSet->LoadFromFile(ADDataSet);
      //  FDDataSet->Active=true;
    }

}
void __fastcall TFRPBaseForm::SetDDataSetOther(String ADDataSetOther)
{
    if (ADDataSetOther != "")
    {
        FDDataSet2->LoadFromFile(ADDataSetOther);
	}

}

void __fastcall TFRPBaseForm::TBtEditClick(TObject *Sender)
{
   frxDsg->OpenDir = FReportPath;
   if (FOnSetFieldAliases)
	   FOnSetFieldAliases(frxMDataSet,frxDDataSet);
   if(FOnBeforDesig)
	  FOnBeforDesig(frxRpt);
   frxRpt->DesignReport();
   if( FOnSaveReport)
  {
   TMemoryStream  *FMemoryStream=new TMemoryStream();
   String rptFileName=frxRpt->FileName;
   try
   {
	  frxRpt->SaveToStream(FMemoryStream);
	  FOnSaveReport(FMemoryStream,rptFileName);  //�����¼�
   }
   __finally
   {
		 delete FMemoryStream;
   }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TBtPreviewClick(TObject *Sender)
{
   ShowReport();
   String str=" ��%d ҳ    ��%dҳ";
   STBBottom->Panels->Items[0]->Text = Format(str,ARRAYOFCONST((frxPreview->PageCount,frxPreview->PageNo)));;
}
//---------------------------------------------------------------------------
void __fastcall TFRPBaseForm::ShowReport()
{
   if (!FReportPath.IsEmpty())
   {
	   try
       {
          if ( Caption != "")
             this->Caption = FCaption ;
          if (FOnSetFieldAliases)
          	  FOnSetFieldAliases(frxMDataSet,frxDDataSet);
          frxRpt->ShowReport();
       }
       catch( Exception *e)
       {
           throw Exception ("����Ԥ������,����Ĵ��룺"+e->Message);
       }
   }
   else
   TBtEditClick(NULL);
}
void __fastcall TFRPBaseForm::TBtPrintClick(TObject *Sender)
{
  frxPreview->Print();
}
//---------------------------------------------------------------------------



void __fastcall TFRPBaseForm::TbtFirstClick(TObject *Sender)
{
   frxPreview->First();        
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtPreClick(TObject *Sender)
{
   frxPreview->Prior();

}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtNextClick(TObject *Sender)
{
   frxPreview->Next();
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtLastClick(TObject *Sender)
{
   frxPreview->Last();        
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtBigClick(TObject *Sender)
{
  if ( FZoom<2)
  {
      FZoom += 0.1;
      frxPreview->Zoom = FZoom;
      STBBottom->Panels->Items[1]->Text = "���ű�����"+FloatToStr(FZoom*100)+"%";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtSmaillClick(TObject *Sender)
{
   if ( FZoom>0.15)
  {
      FZoom -= 0.1;
      frxPreview->Zoom = FZoom;
      STBBottom->Panels->Items[1]->Text = "���ű�����"+FloatToStr(FZoom*100)+"%";
  }
}
//---------------------------------------------------------------------------


void __fastcall TFRPBaseForm::frxPreviewPageChanged(TfrxPreview *Sender,
      int PageNo)
{
	 String str=" ��%d ҳ    ��%dҳ";
	 STBBottom->Panels->Items[0]->Text = Format(str,ARRAYOFCONST((frxPreview->PageCount,frxPreview->PageNo)));
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::TbtFactClick(TObject *Sender)
{
  if ( FZoom != 1)
  {
      FZoom = 1;
      frxPreview->Zoom = FZoom;
      STBBottom->Panels->Items[1]->Text = "���ű�����"+FloatToStr(FZoom*100)+"%";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::frxRptGetValue(const String VarName,
      Variant &Value)
{

    if (FOnGetValue)
    {
       FOnGetValue(VarName,Value);
    }
    if (Value ==String("")  && VarName.SubString(1,7) != "MoneyCn" && VarName.SubString(1,2)=="CN" )
        Value = "";
}
//---------------------------------------------------------------------------
TColor __fastcall  TFRPBaseForm::GetTitleColor()
{
   return ToolBar1->Color;
}
void __fastcall TFRPBaseForm::SetTitleColor(TColor AColor)
{
   ToolBar1->Color = AColor;
}

void __fastcall TFRPBaseForm::SetCaption(String ACaption)
{
   FCaption = ACaption;
   Caption = ACaption;
}

void __fastcall TFRPBaseForm::frxRptBeginDoc(TObject *Sender)
{
   if (FOnGetVariables)
       FOnGetVariables(frxRpt->Variables);
}
//---------------------------------------------------------------------------
void __fastcall TFRPBaseForm::SetMDLinkStr(String ALinkStr)
{
	if(FMDLinkString!=ALinkStr)
	{
	  FMDLinkString = ALinkStr;
	  GetLinkString(FMDLinkString,0);
	}
}

void __fastcall TFRPBaseForm::SetMDLinkStr2(String ALinkStr)
{
	  if(FMDLinkString2!=ALinkStr)
	  {
	   FMDLinkString2 = ALinkStr;
	   GetLinkString(FMDLinkString2,1);
	  }
}

String __fastcall TFRPBaseForm::GetLinkString(String LinkString,int Atype)
{
    String ReStr="",NewStr=LinkString,VarStr,MFieldStr="",DFieldStr="";
    String binStr="";
	while( NewStr.Trim()!= "")
    {
       if ( NewStr.Pos(";") > 0 )
       {
          VarStr=NewStr.SubString(1,NewStr.Pos(";")-1);
          NewStr = NewStr.SubString(NewStr.Pos(";")+1,NewStr.Length()-NewStr.Pos(";"));
       }
       else
       {
          VarStr = NewStr;
          NewStr = "";
       }
       MFieldStr = MFieldStr +";"+ VarStr.SubString(1,VarStr.Pos("=")-1);
       DFieldStr = DFieldStr +";"+ VarStr.SubString(VarStr.Pos("=")+1,VarStr.Length()-VarStr.Pos("="));
    }
    if ( Atype == 0)
    {
      MFieldStr = MFieldStr.SubString(2,MFieldStr.Length()-1);
      DFieldStr = DFieldStr.SubString(2,DFieldStr.Length()-1);
      FDDataSet->IndexFieldNames  = DFieldStr;
      FDDataSet->MasterFields = MFieldStr;
    }
    else
    {
      MFieldStr = MFieldStr.SubString(2,MFieldStr.Length()-1);
      DFieldStr = DFieldStr.SubString(2,DFieldStr.Length()-1);
      FDDataSet2->IndexFieldNames  = DFieldStr;
      FDDataSet2->MasterFields = MFieldStr;
    }
}
Variant __fastcall TFRPBaseForm::frxRptUserFunction(
	  const String MethodName, Variant &Params)
{
   String FParam;
   Variant retValue="";
   if (MethodName.UpperCase() ==String("MoneyCn").UpperCase())
   {
     if (!Params.IsEmpty())
     {
        if (!Params.GetElement(0).IsNull())
        {
          FParam = Params.GetElement(0);
          if (FParam != "")
              return FParam = GetBigMoney(StrToFloat(FParam.Trim()));
          else
              return "";
		}
        else
            return "";
	 }
   }
   else if(FOnUserDefineFunction)
   {
		FOnUserDefineFunction(MethodName, Params, retValue);
		return retValue;
   }
   else
	return "";

}
//---------------------------------------------------------------------------
String __fastcall TFRPBaseForm::GetBigMoney(double Amount)
{
  String CurrArray[3] = {"Ԫ", "��", "��"};
  String Integrity = "��";
  String ErrCN = "����";
  String AValue, ADecimalA, ADecimalB, AInteger, DotStr;
  DotStr = CurrArray[0];
  AValue = FloatToStr(Amount);
  if(AValue.Pos('E') > 0) //����
    return ErrCN;
  int dotpos = AValue.Pos('.');
  if(dotpos> 0) //�ֽ�ΪС�����ֺ���������
  {
    AInteger = AValue.SubString(1,dotpos-1);
    ADecimalB = AValue.SubString(dotpos+1,AValue.Length());
    ADecimalA = "";
  }
  else
  {
    AInteger = AValue;
    ADecimalB = "";
    ADecimalA = "";
  }
  String Result = "";
  int j;
  for(int i = 1;i<=AInteger.Length();i++)
  {
    Result = Result + Num2Str(StrToInt(AInteger.SubString(i,1)));
    j = AInteger.Length() - i + 1;
    if((j - 1) % 4 == 0)
    {
        while(Result.SubString(Result.Length() - 1, 2) == Num[0]) //����'����'�����
          Result = Result.SubString(1, Result.Length() - 2);
        if((Result.SubString(Result.Length() - 1, 2) != Num[14]) && //����'����'�����
          (Result.SubString(Result.Length() - 1, 2) != Num[15]))
        {
          switch((j - 1) / 4)
          {
            case 1: Result = Result + Num[13];break;
            case 2: Result = Result + Num[14];break;
            case 3: Result = Result + Num[15];
          }
        }
    }
    else if(AInteger.SubString(i,1) != "0")
    {
        if(j % 4 == 0)
          Result = Result + Num[12];
        else if((j + 1) % 4 == 0)
          Result = Result + Num[11];
        else if((j + 2) % 4 == 0)
          Result = Result + Num[10];
    }
  }
  if(ADecimalB.Length() == 0) //x ->> xԪ��
    ADecimalA = ADecimalA + Integrity;
  else
  {
    if(ADecimalB.SubString(1,1) != '0')
      ADecimalA = ADecimalA + Num2Str(StrToInt(ADecimalB.SubString(1,1))) + CurrArray[1];
    if(ADecimalB.Length() == 1)
      ADecimalA = ADecimalA + Integrity;
    else
      ADecimalA = ADecimalA + Num2Str(StrToInt(ADecimalB.SubString(2,1))) + CurrArray[2];
  }

  if(Result == "") //����'0.x'�޷����ϵ����
    Result = Num[0];
  while(Result.Pos(Num[0] + Num[0]) > 0) //����'����'�����
  {
    TReplaceFlags re;
    re.Clear();
    re<<rfReplaceAll;
    Result = StringReplace(Result, Num[0] + Num[0], Num[0], re);
  }

  while((Result.SubString(Result.Length() - 1, 2) == Num[0])
    && (Result != Num[0]) )
    Result = Result.SubString(1, Result.Length() - 2); //�������һ������'��'�����

  if(ADecimalA != "")
    Result = Result + DotStr + ADecimalA; //����С������
  return Result;
}

String __fastcall TFRPBaseForm::Num2Str(int ANum)
{
  String Result = "";
  switch(ANum)
  {
    case 1: Result = Result + Num[1];break;
    case 2: Result = Result + Num[2];break;
    case 3: Result = Result + Num[3];break;
    case 4: Result = Result + Num[4];break;
    case 5: Result = Result + Num[5];break;
    case 6: Result = Result + Num[6];break;
    case 7: Result = Result + Num[7];break;
    case 8: Result = Result + Num[8];break;
    case 9: Result = Result + Num[9];break;
    case 0: Result = Result + Num[0];
  }
  return Result;
}

void __fastcall TFRPBaseForm::Excel1Click(TObject *Sender)
{
	frxPreview->Export(frxXLSExport1);
	if(!frxXLSExport1->FileName.IsEmpty() && FOnAfterExportReport != NULL){
		FOnAfterExportReport(frxXLSExport1->FileName);
		frxXLSExport1->FileName = "";
	}		        
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::Word1Click(TObject *Sender)
{
	frxPreview->Export(frxRTFExport1);
	if(!frxRTFExport1->FileName.IsEmpty() && FOnAfterExportReport != NULL){
		FOnAfterExportReport(frxRTFExport1->FileName);
		frxRTFExport1->FileName = "";
	}	 	 
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::Jpg1Click(TObject *Sender)
{
	frxPreview->Export(frxJPEGExport1);
	if(!frxJPEGExport1->FileName.IsEmpty() && FOnAfterExportReport != NULL){
		FOnAfterExportReport(frxJPEGExport1->FileName);
		frxJPEGExport1->FileName = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFRPBaseForm::PDF1Click(TObject *Sender)
{
	frxPreview->Export(frxPDFExport1);
	if(!frxPDFExport1->FileName.IsEmpty() && FOnAfterExportReport != NULL){
		FOnAfterExportReport(frxPDFExport1->FileName);
    frxPDFExport1->FileName = "";
	}
}
//---------------------------------------------------------------------------


