//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "WorkReportEdit.h"
#include "System.DateUtils.hpp"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DetailBaseForm"
#pragma resource "*.fmx"
TfrmWorkReportEdit *frmWorkReportEdit;
//---------------------------------------------------------------------------
__fastcall TfrmWorkReportEdit::TfrmWorkReportEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,TZClientDataSet *m_DataSet,int DataSetStatus,int FormType)
	: TfrmDetailBaseForm(Owner,clBroker,ModuleCode,m_DataSet,DataSetStatus)
{
	fFormType = FormType;
	InitForm();
}
//---------------------------------------------------------------------------
__fastcall TfrmWorkReportEdit::TfrmWorkReportEdit(TComponent* Owner,TClientBroker * clBroker,int ModuleCode,String Param,int Flag,int FormType)
	: TfrmDetailBaseForm(Owner,clBroker,ModuleCode,Param)
{
   fFormType = FormType;
   InitForm();
   if(Flag==3) //ר��ΪOA��������,Param=BillID;
   {
	  this->MasterWhereString="WorkReportID="+QuotedStr(Param);
	  SetFormStatus(0);//���״̬
	  GetWorkflowDefine();  //Ϊ����ʾ����
   }
   this->RefreshData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::InitForm()
{
     TabDetailItem->Visible=true;
	 btMainNext->Visible=true;
	 //������ҵ�����̹����¼���
	 OnBeforWorkFlowStart=BeforWorkFlowStart;
	 OnBeforWorkFlowExec=BeforWorkFlowExec;
	 OnAfterWorkFlowStart=AfterWorkFlowStart;
	 OnAfterWorkFlowExec=AfterWorkFlowExec;

	 DataSet->DetailDataSetAfterPost=DetailDataSetAfterPost;
 /*
	 if(fFormType == 0)
		{
			Label2->Text = L"  �ܣ�";
			spWorkReportWeek->Max = WeeksInAYear(YearOf(Now()));
			deWorkReportDate->Visible = false;
		}
		else if(fFormType == 1)
		{
			Label2->Text = L"�·ݣ�";
			spWorkReportWeek->Max = 12;
			deWorkReportDate->Visible = false;
		}
		else if(fFormType == 2)
		{
			Label2->Text = L"���ȣ�";
			spWorkReportWeek->Max = 4;
			deWorkReportDate->Visible = false;
		}
		else if(fFormType == 3)
		{
			Label2->Visible = false;
			spWorkReportWeek->Visible = false;
			deWorkReportDate->Visible = false;
		}
		else if(fFormType == 4)
		{
			Label2->Text = L"���ڣ�";
			spWorkReportWeek->Visible = false;
			deWorkReportDate->Visible = true;
		}*/
	switch(fFormType)
	{
		case 0:
		 Caption=L"�����ܱ��༭";
			Label2->Text = L"  �ܣ�";
			spWorkReportWeek->Max = WeeksInAYear(YearOf(Now()));
			deWorkReportDate->Visible = false;
		break;
		case 1:
		 Caption=L"�����±��༭";
			Label2->Text = L"�·ݣ�";
			spWorkReportWeek->Max = 12;
			deWorkReportDate->Visible = false;
		break;
		case 2:
		 Caption=L"���������༭";
			Label2->Text = L"���ȣ�";
			spWorkReportWeek->Max = 4;
			deWorkReportDate->Visible = false;
		break;
		case 3:
		 Caption=L"�����걨�༭";
		 Label2->Visible = false;
		 spWorkReportWeek->Visible = false;
		 deWorkReportDate->Visible = false;
		break;
		default:
		 Caption=L"�����ձ��༭";
			Label2->Text = L"���ڣ�";
			spWorkReportWeek->Visible = false;
			deWorkReportDate->Visible = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::FormInitControl()
{
	if(fFormType == 4)
		LockControl(1,spWorkReportYear,false,false,false);
	else
		LockControl(1,spWorkReportYear,false,true,true);

	LockControl(1,spWorkReportWeek,false,true,true);
	LockControl(1,seWorkReportStartDate,false,false,false);
	LockControl(1,seWorkReportEndDate,false,false,false);
	LockControl(1,seEmpName,false,false,false);
	LockControl(1,mmWorkReportSelfOpinion,false,true,true);
	LockControl(1,mmWorkReportCheckerOpinion,false,false,false);
	LockControl(1,seWorkReportState,false,false,false);
	LockControl(1,seWorkReportUpdateDatetime,false,false,false);
	LockControl(1,seCheckerName,false,false,false);
	LockControl(1,seWorkReportCheckDatetime,false,false,false);
	LockControl(1,deWorkReportDate,false,true,true);
//	WaitInputControl(1,spWorkReportYear);

	GetWorkflowDefine();
 }
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::FormClearControl(int Section)
{
	if(Section==1)
	{

		TDateTime dtime;
		int fYear,fMonth,fWeek,fDay;
		fYear = YearOf(Now());
		fMonth = MonthOf(Now());
		fWeek = WeekOf(Now());
		fDay = DayOf(Now());
		FGuidBillID=GetGUIDString();

		seEmpName->Text = ClientBroker->UserName;
		seEmpName->TagString = ClientBroker->UserEmpID;
		spWorkReportYear->Text = IntToStr(fYear);

		if(fFormType == 0)
		{
			spWorkReportWeek->Text = IntToStr(fWeek);
		}
		else if(fFormType == 1)
		{
			spWorkReportWeek->Text = IntToStr(fMonth);
		}
		else if(fFormType == 2)
		{
			spWorkReportWeek->Text = IntToStr(fMonth/3 + 1);
		}
		else if(fFormType == 4)
		{
			deWorkReportDate->Text = Now().FormatString("YYYY-MM-DD");
		}

		if(fFormType == 4)
            deWorkReportDateChange(NULL);
		else
			spWorkReportWeekChange(NULL);

		mmWorkReportSelfOpinion->Text = "";
		mmWorkReportCheckerOpinion->Text="";
		seWorkReportState->Text = "";
		seWorkReportUpdateDatetime->Text="";
		seCheckerName->Text = "";
		seWorkReportCheckDatetime->Text = "";
	}
}
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::SendDataToDataSet(int Section)
{
	if(Section == 1)
	{
		if(MasterStatus==dsInsert)
		{
			SetMasterValue("WorkReportID",FGuidBillID);
			SetMasterValue("EmpName",ClientBroker->UserName);
			SetMasterValue("WorkReportEmpID",ClientBroker->UserEmpID);
			SetMasterValue("WorkReportCreateDate",Now().FormatString("YYYY-MM-DD"));
			SetMasterValue("WorkReportType",fFormType);
			SetMasterValue("WorkReportWeek",0);
			SetMasterValue("WorkReportQuarter",0);
			SetMasterValue("WorkReportMonth",0);
			SetMasterValue("WorkReportNextStart","");
            SetMasterValue("WorkReportNextFinish","");
			SetMasterValue("WorkReportUpdateDatetime","");
			SetMasterValue("WorkReportState",0);
			SetMasterValue("WorkReportCheckerID","");
			SetMasterValue("WorkReportCheckState",0);
			SetMasterValue("WorkReportCheckDatetime","");
			SetMasterValue("WorkReportSelfScore",0);
			SetMasterValue("WorkReportCheckScore",0);
			SetMasterValue("WorkReportDate","");
		}
		SetMasterValue("WorkReportYear",StrToInt(spWorkReportYear->Text));

		if(fFormType == 0)
			SetMasterValue("WorkReportWeek",StrToInt(spWorkReportWeek->Text));
		else if(fFormType == 1)
			SetMasterValue("WorkReportMonth",StrToInt(spWorkReportWeek->Text));
		else if(fFormType == 2)
			SetMasterValue("WorkReportQuarter",StrToInt(spWorkReportWeek->Text));
		else if(fFormType == 4)
			SetMasterValue("WorkReportDate",deWorkReportDate->Date.FormatString("YYYY-MM-DD"));

		SetMasterValue("WorkReportStartDate",seWorkReportStartDate->Text);
		SetMasterValue("WorkReportEndDate",seWorkReportEndDate->Text);
		SetMasterValue("WorkReportSelfOpinion",mmWorkReportSelfOpinion->Text);
		SetMasterValue("WorkReportCheckerOpinion",mmWorkReportCheckerOpinion->Text);
	}
}
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::GetDataFromDataSet(int Section)
{
	if(Section == 1)
	{
		FGuidBillID=GetMasterValue("WorkReportID");
		seEmpName->Text = GetMasterValue("EmpName");
		seEmpName->TagString = GetMasterValue("WorkReportEmpID");
		spWorkReportYear->Text = GetMasterValue("WorkReportYear");

		if(fFormType == 0)
		{
			spWorkReportWeek->Max = WeeksInAYear(StrToInt(VarToStr(GetMasterValue("WorkReportYear"))));
			spWorkReportWeek->Text = GetMasterValue("WorkReportWeek");
		}
		else if(fFormType == 1)
		{
			spWorkReportWeek->Text = GetMasterValue("WorkReportMonth");
		}
		else if(fFormType == 2)
		{
			spWorkReportWeek->Text = GetMasterValue("WorkReportQuarter");
		}
		else if(fFormType == 4)
		{
			deWorkReportDate->Text = GetMasterValue("WorkReportDate");
        }
		seWorkReportStartDate->Text = GetMasterValue("WorkReportStartDate");
		seWorkReportEndDate->Text = GetMasterValue("WorkReportEndDate");
		mmWorkReportSelfOpinion->Text = GetMasterValue("WorkReportSelfOpinion");
		mmWorkReportCheckerOpinion->Text=GetMasterValue("WorkReportCheckerOpinion");

		if(GetMasterValue("WorkReportCheckState") == 1)
			seWorkReportState->Text = L"�����";
		else if(GetMasterValue("WorkReportState") == 1)
			seWorkReportState->Text = L"���ύ";
		else
			seWorkReportState->Text = L"�ݸ�";

		seWorkReportUpdateDatetime->Text=GetMasterValue("WorkReportUpdateDatetime");
		seCheckerName->Text = GetMasterValue("CheckerName");
		seWorkReportCheckDatetime->Text = GetMasterValue("WorkReportCheckDatetime");

		GetWorkflowRunID(FGuidBillID);
	}
}
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::VerifyFieldData(int Section,int status)
{
  	if(Section==1)
	{
		if(mmWorkReportSelfOpinion->Text.Trim() == "")
			throw Exception (L"����������������");
    }
}
//--------------------------------------------------------------------------
bool __fastcall TfrmWorkReportEdit::BeforeUpdateData(int OptType)
{
 	return true;
}
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::SetUserCtrlStatus()
{
  	int Check = GetFieldValue("WorkReportCheckState");
	if(Check != 0)
	{
		AddItemAction->Enabled = false;
		DeleteItemAction->Enabled = false;
		DetailEditAction->Enabled = false;
		SaveDetailAction->Enabled = false;
    }
}
//--------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::spWorkReportWeekChange(TObject *Sender)
{
	int fYear = StrToInt(spWorkReportYear->Text);
	int fWeek = StrToInt(spWorkReportWeek->Text);

	if(fYear<spWorkReportYear->Min)
		fYear = spWorkReportYear->Min;
	if(fYear>spWorkReportYear->Max)
		fYear = spWorkReportYear->Max;
	spWorkReportWeek->Max = WeeksInAYear(fYear);
	if(fWeek<spWorkReportWeek->Min)
		fWeek = spWorkReportWeek->Min;
	if(fWeek>spWorkReportWeek->Max)
		fWeek = spWorkReportWeek->Max;
	if(fFormType == 0)
	{
		seWorkReportStartDate->Text = StartOfAWeek(fYear,fWeek).FormatString("YYYY-MM-DD");
		seWorkReportEndDate->Text = EndOfAWeek(fYear,fWeek).FormatString("YYYY-MM-DD");
	}
	else if(fFormType == 1)
	{
		seWorkReportStartDate->Text = StartOfAMonth(fYear,fWeek).FormatString("YYYY-MM-DD");
		seWorkReportEndDate->Text = EndOfAMonth(fYear,fWeek).FormatString("YYYY-MM-DD");
	}
	else if(fFormType == 2)
	{
		seWorkReportStartDate->Text = StartOfAMonth(fYear,(fWeek-1)*3+1).FormatString("YYYY-MM-DD");
		seWorkReportEndDate->Text = EndOfAMonth(fYear,fWeek*3).FormatString("YYYY-MM-DD");
	}
	else if(fFormType == 3)
	{
		seWorkReportStartDate->Text = StartOfAYear(fYear).FormatString("YYYY-MM-DD");
		seWorkReportEndDate->Text = EndOfAYear(fYear).FormatString("YYYY-MM-DD");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::BeforWorkFlowStart(TWorkFlowStartData *AWorkFlowStartData)
{
//��ʼ��һЩҵ����ص�ֵ���������̡�
   AWorkFlowStartData->BusWindow=FormID;
   AWorkFlowStartData->BusCode=FGuidBillID;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::BeforWorkFlowExec(TWorkFlowExecData *AWorkFlowExecData,TClientDataSet *varDataSet)
{
  //����һЩ���̵ı���ֵ��ҵ�񵥾ݣ�����ҵ�񵥾ݵ�ֵ���ݸ�����varDataSet��

}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::AfterWorkFlowStart(String WorkInsID)
{
 	DS_Edit();
	SetMasterValue("WorkReportState",1);
	SetMasterValue("WorkReportUpdateDatetime",Now().FormatString("YYYY-MM-DD hh:mm:ss"));
	DS_Update();
	FormFillData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::AfterWorkFlowExec(String WorkInsID,String TaskInsID)
{
	String MasterID = FGuidBillID;
	Query->Close();
	Query->SQL->Text = "update tbWorkReport set WorkReportCheckerOpinion='"+edFlowMnDtlComments->Text+"'  where WorkReportID='"+FGuidBillID+"'";
	Query->ExecSQL();
	RefreshDataSet();
	LocateMaster(MasterID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::FormFillData()
{
	ItemStringGrid->RowCount = 0;
	ItemStringGrid->RowCount = DataSet->DetailDataSet->RecordCount;
	DS_DetailFirst();
	int index = 0;
	while(!DetailEof)
	{
		ItemStringGrid->Cells[0][index] = VarToStr(GetDetailValue("WorkDetailID"));
		ItemStringGrid->Cells[1][index] = VarToStr(GetDetailValue("WorkDetailIndex"));
		ItemStringGrid->Cells[2][index] = VarToStr(GetDetailValue("WorkDetailContent"));
		ItemStringGrid->Cells[3][index] = VarToStr(GetDetailValue("WorkDetailDataTime"));
		ItemStringGrid->Cells[4][index] = VarToStr(GetDetailValue("WorkDetailProssState"));
		ItemStringGrid->Cells[5][index] = VarToStr(GetDetailValue("WorkDetailNextContent"));

		DS_DetailNext();
		index++;
	}
}
//---------------------------------------------------------------------------
TForm * __fastcall TfrmWorkReportEdit::NewItemEditForm(TZClientDataSet *m_DataSet, int status)//������������ϸ����
{
  if(WorkReportItemEdit==NULL)
	WorkReportItemEdit=new TfrmWorkReportItemEdit(this,ClientBroker,this->FormModuleCode,m_DataSet,status);
   return WorkReportItemEdit;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::DetailDataSetAfterPost(TDataSet* DataSet)
{
    SetOptCtrlStatus(0);
	FormFillData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::ItemStringGridSelChanged(TObject *Sender)
{
	int row = ItemStringGrid->Selected;
	String fDetailID =  VarToStr(ItemStringGrid->Cells[0][row]);
	DS_LocateDetail(FGuidBillID+"\t"+fDetailID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmWorkReportEdit::deWorkReportDateChange(TObject *Sender)
{
	int fYear = YearOf(deWorkReportDate->Date);
	spWorkReportYear->Text = IntToStr(fYear);
    spWorkReportYear->ReadOnly = true;
	seWorkReportStartDate->Text = deWorkReportDate->Text;
	seWorkReportEndDate->Text = deWorkReportDate->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfrmWorkReportEdit::edWorkflowTitleEnter(TObject *Sender)
{
	if(edWorkflowTitle->Text.IsEmpty())
   {
	String ReportStr="";
	 switch(fFormType)
	 {
		case 0:
		  ReportStr=L"�����ܱ�,"+spWorkReportYear->Text+L"���"+spWorkReportWeek->Text+L"��";
		  break;
		case 1:
		  ReportStr=L"�����±�,"+spWorkReportYear->Text+L"���"+spWorkReportWeek->Text+L"��";
		  break;
		case 2:
		  ReportStr=L"��������,"+spWorkReportYear->Text+L"���"+spWorkReportWeek->Text+L"��";
		  break;
		case 3:
		  ReportStr=L"�����걨,"+spWorkReportYear->Text+L"���";
		  break;
		case 4:
		  ReportStr=L"�����ձ�,����"+deWorkReportDate->Text;
		  break;
	 }
	  edWorkflowTitle->Text=seEmpName->Text+L"��"+ ReportStr;
	  mmWorkflowContent->Lines->Add(edWorkflowTitle->Text);
	  mmWorkflowContent->Lines->Add(L"�ύʱ��:"+Now().FormatString("YYYY-MM-DD HH:MM:SS"));
   }
}
//---------------------------------------------------------------------------


