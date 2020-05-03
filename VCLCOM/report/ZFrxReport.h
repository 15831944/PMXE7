//---------------------------------------------------------------------------

#ifndef ZFrxReportH
#define ZFrxReportH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ADODB.hpp>
#include "ZReportbase.h"

//---------------------------------------------------------------------------
class PACKAGE TZFrxReport : public TComponent
{
private:
   TFRPBaseForm * p;
   String FReportPath;      //�����ŵ�·��+��������
   String FCaption;
   TColor FTitleColor;
   TOnSetFieldAliases FOnSetFieldAliases;
   TOnSaveReport FOnSaveReport;
   TOnGetValue  FOnGetValue;
   TOnGetVariables FOnGetVariables;
   TOnAfterExportReport FOnAfterExportReport; //add by ljh at 2010.1.29
   TOnUserDefineFunction FOnUserDefineFunction;
	TOnBeforDesig FOnBeforDesig;
   TfrxDBDataset * __fastcall GetMDataSet();
   TfrxDBDataset * __fastcall GetDDataSet();
   void __fastcall SetReportPath(String  AReportPath);
   String __fastcall GetReportPath();
protected:
public:
    void __fastcall ShowReport();
    void __fastcall ShowReport(String XmlFileName);
	//AMDLingkString ��ʽ˵����
    void __fastcall ShowReport(String MXmlFileName,String DXmlFileName,String AMDLinkString);
    void __fastcall ShowReport(String MXmlFileName,String DXmlFileName,String DXmlFileNameOther,String AMDLinkString,String AMDLinkStringOther);
    __property  TfrxDBDataset * MDataSet={read=GetMDataSet};
    __property  TfrxDBDataset * DDataSet={read=GetDDataSet};
    __fastcall TZFrxReport(TComponent* Owner);
	__fastcall ~TZFrxReport();
__published:
   __property String  Reportpath={write= SetReportPath,read=GetReportPath};
	__property TOnBeforDesig OnBeforDesig={write=FOnBeforDesig,read=FOnBeforDesig};
   __property TOnSaveReport OnSaveReport={write = FOnSaveReport, read = FOnSaveReport};//�������ݿⷽ��
   __property TOnGetValue OnGetValue={write = FOnGetValue,read = FOnGetValue};//������ʱ����
   __property TOnGetVariables OnGetVariables={write = FOnGetVariables,read = FOnGetVariables};//���������ֵ����
   __property TOnSetFieldAliases OnSetFieldAliases={write = FOnSetFieldAliases, read = FOnSetFieldAliases};
   __property TOnAfterExportReport OnAfterExportReport={write=FOnAfterExportReport, read=FOnAfterExportReport};//add by ljh at 2010.1.29
   __property TOnUserDefineFunction OnUserDefineFunction={write = FOnUserDefineFunction, read = FOnUserDefineFunction};
   __property  String Caption={write = FCaption,read =FCaption};
   __property TColor TitleColor={write = FTitleColor,read =FTitleColor};
};
//---------------------------------------------------------------------------
#endif
 