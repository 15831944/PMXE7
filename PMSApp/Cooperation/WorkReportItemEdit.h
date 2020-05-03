//---------------------------------------------------------------------------

#ifndef WorkReportItemEditH
#define WorkReportItemEditH
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
#include <FMX.Memo.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.SpinBox.hpp>
//---------------------------------------------------------------------------
class TfrmWorkReportItemEdit : public TfrmItemBaseForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label5;
	TEdit *edWorkDetailDataTime;
	TEdit *edWorkDetailProssState;
	TEdit *edWorkDetailNextContent;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TMemo *mmWorkDetailContent;
	TSpinBox *spWorkDetailScore;
	TSpinBox *spWorkDetailRate;
	TSpinBox *spWorkDetailIndex;
private:	// User declarations
   int FCheckFlag;
   String FGuidBillID,FDetailID;
   void __fastcall FormInitControl();   //��ʼ�����ݿؼ�
   void __fastcall FormClearControl(int Section);   //��տؼ�����
   void __fastcall SendDataToDataSet(int Section);  //�ؼ����ݴ������ݼ�
   void __fastcall GetDataFromDataSet(int Section); //���ݼ�������ʾ�ڿؼ���
   void __fastcall VerifyFieldData(int Section,int status); //����ǰУ�����ݣ�status��1-������2-�޸�
   void __fastcall SetUserCtrlStatus();//�����û�����״̬
void __fastcall DetailDataSetBeforePost(TDataSet* DataSet);
public:		// User declarations
	__fastcall TfrmWorkReportItemEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *FDataSet,int DataSetStatus);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWorkReportItemEdit *frmWorkReportItemEdit;
//---------------------------------------------------------------------------
#endif
