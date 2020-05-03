//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "LoanCalculation.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmLoanCalculation *frmLoanCalculation;
//---------------------------------------------------------------------------
__fastcall TfrmLoanCalculation::TfrmLoanCalculation(TComponent* Owner,TClientBroker *clBroker)
	: TForm(Owner)
{
	FClientBroker = clBroker;
	cbRate->Items->Strings[4] = L"基准\t"+FormatFloatToStr(FClientBroker->BaseBusiRate,ftQty,2)+"%";
	edAccu->Text = FormatFloatToStr(FClientBroker->BaseAccuRate,ftQty,2)+"%";
	cbLoanTypeChange(cbLoanType);
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoanCalculation::btCalculationClick(TObject *Sender)
{
	double BusinessAmt = nbBusinessAmt->Value;
	double Accumulation= nbAccumulation->Value;
	int Cycles = (cbCycle->ItemIndex + 1) * 12;
	double BaseBusiRate = FClientBroker->BaseBusiRate/100;
	double BusiRate = BaseBusiRate;
	double AccuRate = FClientBroker->BaseAccuRate/100/12;
	switch(cbRate->ItemIndex)
	{
		case 0:
			BusiRate = BaseBusiRate * 0.8 / 12;
			break;
		case 1:
			BusiRate = BaseBusiRate * 0.85 / 12;
			break;
		case 2:
			BusiRate = BaseBusiRate * 0.9 / 12;
			break;
		case 3:
			BusiRate = BaseBusiRate * 0.95 / 12;
			break;
		case 4:
			BusiRate = BaseBusiRate * 1 / 12;
			break;
		case 5:
			BusiRate = BaseBusiRate * 1.05 / 12;
			break;
		case 6:
			BusiRate = BaseBusiRate * 1.1 / 12;
			break;
    }
	if(cbPayWay->ItemIndex == 0)
	{
		double MonthBusiAmt,MonthAccuAmt,BusiPow,AccuPow;
		BusiPow = Power(1+BusiRate,Cycles);
		AccuPow = Power(1+AccuRate,Cycles);
		MonthBusiAmt = BusinessAmt * BusiRate * BusiPow / (BusiPow - 1);
		MonthAccuAmt = Accumulation * AccuRate * AccuPow / (AccuPow - 1);

		edPrincipal->Text = FormatFloatToStr(BusinessAmt + Accumulation,ftAmt,2);
		edInterest->Text =  FormatFloatToStr((Cycles * ConvertFloat(MonthBusiAmt + MonthAccuAmt,ftAmt,2,0) - (BusinessAmt + Accumulation)),ftAmt,2);
		edTotalPay->Text =  FormatFloatToStr(Cycles * ConvertFloat(MonthBusiAmt + MonthAccuAmt,ftAmt,2,0),ftAmt,2);
		edMonthPay->Text =  FormatFloatToStr(MonthBusiAmt + MonthAccuAmt,ftAmt,2,0);
		DetailPayGrid->RowCount = Cycles;
		for(int i = 0;i<Cycles;i++)
		{
			double MonthBusiPrincipal= BusinessAmt * BusiRate * Power(1+BusiRate,i) / (BusiPow - 1);
			double MonthAccuPrincipal= Accumulation * AccuRate * Power(1+AccuRate,i) / (AccuPow - 1);
			double MonthBusiInterest = BusinessAmt * BusiRate * (BusiPow - Power(1+BusiRate,i)) / (BusiPow - 1);
			double MonthAccuInterest = Accumulation * AccuRate * (AccuPow - Power(1+AccuRate,i)) / (AccuPow - 1);
			DetailPayGrid->Cells[0][i] = IntToStr(i + 1);
			DetailPayGrid->Cells[1][i] = FormatFloatToStr(MonthBusiPrincipal + MonthAccuPrincipal,ftAmt,2,0);
			DetailPayGrid->Cells[2][i] = FormatFloatToStr(MonthBusiInterest + MonthAccuInterest,ftAmt,2,0);
			DetailPayGrid->Cells[3][i] = FormatFloatToStr(MonthBusiAmt + MonthAccuAmt,ftAmt,2,0);
		}
	}
	else
	{
		double MonthBusiPrincipal = BusinessAmt / Cycles;
		double MonthAccuPrincipal= Accumulation / Cycles;
		double TotalBusiAmt = ((MonthBusiPrincipal + BusinessAmt * BusiRate) + MonthBusiPrincipal * (1 + BusiRate)) / 2 * Cycles;
		double TotalAccuAmt = ((MonthAccuPrincipal + Accumulation* AccuRate) + MonthAccuPrincipal * (1 + AccuRate)) / 2 * Cycles;
		edPrincipal->Text = FormatFloatToStr(BusinessAmt + Accumulation,ftAmt,2);
		edInterest->Text =  FormatFloatToStr(TotalBusiAmt + TotalAccuAmt - BusinessAmt - Accumulation,ftAmt,2);
		edTotalPay->Text =  FormatFloatToStr(TotalBusiAmt + TotalAccuAmt,ftAmt,2);
		DetailPayGrid->RowCount = Cycles;
		for(int i = 0;i<Cycles;i++)
		{
			double MonthBusiInterest = (BusinessAmt - (MonthBusiPrincipal * i)) * BusiRate;
			double MonthAccuInterest = (Accumulation- (MonthAccuPrincipal * i)) * AccuRate;
            DetailPayGrid->Cells[0][i] = IntToStr(i + 1);
			DetailPayGrid->Cells[1][i] = FormatFloatToStr(MonthBusiPrincipal + MonthAccuPrincipal,ftAmt,2,0);
			DetailPayGrid->Cells[2][i] = FormatFloatToStr(MonthBusiInterest + MonthAccuInterest,ftAmt,2,0);
			DetailPayGrid->Cells[3][i] = FormatFloatToStr(MonthBusiPrincipal + MonthAccuPrincipal + MonthBusiInterest + MonthAccuInterest,ftAmt,2,0);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoanCalculation::btCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoanCalculation::cbLoanTypeChange(TObject *Sender)
{
	if(cbLoanType->ItemIndex == 0)  //纯商贷
	{
		GridPanelLayout1->ControlCollection->Items[6]->Control = Label5;
		GridPanelLayout1->ControlCollection->Items[7]->Control = nbBusinessAmt;
		GridPanelLayout1->ControlCollection->Items[12]->Control = Label8;
		GridPanelLayout1->ControlCollection->Items[13]->Control = cbRate;

		GridPanelLayout1->ControlCollection->Items[8]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[9]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[14]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[15]->Control = NULL;

		Label5->Visible = true;
		nbBusinessAmt->Visible = true;
		if(nbAccumulation->Value>0)
		   nbBusinessAmt->Value=nbAccumulation->Value;
		Label8->Visible = true;
		cbRate->Visible = true;

		Label6->Visible = false;
		nbAccumulation->Visible = false;
		nbAccumulation->Value =0;
		Label9->Visible = false;
		edAccu->Visible = false;
	}
	else if(cbLoanType->ItemIndex == 1) //纯公积金
	{
		GridPanelLayout1->ControlCollection->Items[6]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[7]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[12]->Control = NULL;
		GridPanelLayout1->ControlCollection->Items[13]->Control = NULL;

		GridPanelLayout1->ControlCollection->Items[8]->Control = Label6;
		GridPanelLayout1->ControlCollection->Items[9]->Control = nbAccumulation;
		GridPanelLayout1->ControlCollection->Items[14]->Control = Label9;
		GridPanelLayout1->ControlCollection->Items[15]->Control = edAccu;

		Label6->Visible = true;
		nbAccumulation->Visible = true;
		if(nbBusinessAmt->Value>0)
		  nbAccumulation->Value=nbBusinessAmt->Value;
		Label9->Visible = true;
		edAccu->Visible = true;

		Label5->Visible = false;
		nbBusinessAmt->Visible = false;
		nbBusinessAmt->Value=0;
		Label8->Visible = false;
		cbRate->Visible = false;


	}
	else     //组合贷
	{
        GridPanelLayout1->ControlCollection->Items[6]->Control = Label5;
		GridPanelLayout1->ControlCollection->Items[7]->Control = nbBusinessAmt;
		GridPanelLayout1->ControlCollection->Items[12]->Control = Label8;
		GridPanelLayout1->ControlCollection->Items[13]->Control = cbRate;

		GridPanelLayout1->ControlCollection->Items[8]->Control = Label6;
		GridPanelLayout1->ControlCollection->Items[9]->Control = nbAccumulation;
		GridPanelLayout1->ControlCollection->Items[14]->Control = Label9;
		GridPanelLayout1->ControlCollection->Items[15]->Control = edAccu;

		Label5->Visible = true;
		nbBusinessAmt->Visible = true;
		Label8->Visible = true;
		cbRate->Visible = true;

		Label6->Visible = true;
		nbAccumulation->Visible = true;
		Label9->Visible = true;
		edAccu->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoanCalculation::cbPayWayChange(TObject *Sender)
{
 	if(cbPayWay->ItemIndex == 0)
	{
		Label13->Visible = true;
		edMonthPay->Visible = true;
	}
	else
	{
      	Label13->Visible = false;
		edMonthPay->Visible = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoanCalculation::SetBusinessAmt(double Amt)
{
 	nbBusinessAmt->Value = Amt;
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoanCalculation::FormVirtualKeyboardHidden(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
	FKBBounds = TRectF(0,0,0,0);
	FNeedOffset = False;
	RestorePosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLoanCalculation::FormVirtualKeyboardShown(TObject *Sender, bool KeyboardVisible,
          const TRect &Bounds)
{
 	FKBBounds = TRectF(Bounds);
	FKBBounds = TRectF(ScreenToClient(FKBBounds.TopLeft()),ScreenToClient(FKBBounds.BottomRight()));
	UpdateKBBounds();
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoanCalculation::UpdateKBBounds()
{
	FNeedOffset = false;
	if (Focused != NULL) {
		TControl * LFocused = static_cast<TControl*>(Focused->GetObject());
		TRectF LFocusRect = LFocused->AbsoluteRect;
		LFocusRect.Offset(MainVertScrollBox->ViewportPosition.X, MainVertScrollBox->ViewportPosition.Y);
		if (LFocusRect.IntersectsWith(FKBBounds) && LFocusRect.Bottom > FKBBounds.Top) {
			FNeedOffset = true;
			MainLayout->Align = TAlignLayout::Horizontal;
			MainVertScrollBox->RealignContent();
			Application->ProcessMessages();
			MainVertScrollBox->ViewportPosition = PointF(MainVertScrollBox->ViewportPosition.X,
				LFocusRect.Bottom - FKBBounds.Top);
		}
	}
	if (!FNeedOffset) {
		RestorePosition();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmLoanCalculation::RestorePosition()
{
	MainVertScrollBox->ViewportPosition = PointF(MainVertScrollBox->ViewportPosition.X, 0);
	MainLayout->Align = TAlignLayout::Client;
	MainVertScrollBox->RealignContent();
}
//---------------------------------------------------------------------------
