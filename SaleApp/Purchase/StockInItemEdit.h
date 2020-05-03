//---------------------------------------------------------------------------

#ifndef StockInItemEditH
#define StockInItemEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "ItemBaseForm.h"
#include <FMX.ActnList.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Actions.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TfrmStockInItemEdit : public TfrmItemBaseForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *edStuSkInDtlFKID_Stu;
	TLabel *Label2;
	TEdit *edStuDefName;
	TLabel *Label3;
	TEdit *edStuDefSpecModel;
	TEdit *edStuSkInDtlFKID_Unit;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *edStuSkInDtlCovFactor;
	TEdit *edStuSkInDtlQty;
	TEdit *edStuSkInDtlPrice;
	TEdit *edStuSkInDtlAmt;
	TEdit *edStuSkInDtlCostPrice;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *edStuSpyNtDtlQty;
	TEdit *edStuSpyNtDtlHaveInQty;
	TEditButton *EditButton1;
	TEditButton *EditButton2;
private:	// User declarations
   int FCheckFlag;
   String FGuidBillID;
   void __fastcall FormInitControl();   //��ʼ�����ݿؼ�
   void __fastcall FormClearControl(int Section);   //��տؼ�����
   void __fastcall SendDataToDataSet(int Section);  //�ؼ����ݴ������ݼ�
   void __fastcall GetDataFromDataSet(int Section); //���ݼ�������ʾ�ڿؼ���
   void __fastcall VerifyFieldData(int Section,int status); //����ǰУ�����ݣ�status��1-������2-�޸�
   void __fastcall SetUserCtrlStatus();//�����û�����״̬
void __fastcall DetailDataSetBeforePost(TDataSet* DataSet);
public:		// User declarations
	__fastcall TfrmStockInItemEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStockInItemEdit *frmStockInItemEdit;
//---------------------------------------------------------------------------
#endif