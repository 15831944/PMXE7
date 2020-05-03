//---------------------------------------------------------------------------

#ifndef WorkFlowDetailFormH
#define WorkFlowDetailFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "BaseBillForm.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TfrmWorkFlowDetailForm : public TfrmBaseBillForm
{
__published:	// IDE-managed Components
	TButton *BtnCommit;
	TButton *BtnNotPass;
	void __fastcall BtnCommitClick(TObject *Sender);
	void __fastcall BtnNotPassClick(TObject *Sender);
private:	// User declarations
	String FCommitUrl,FNotPassUrl,FLoadUrl;
	bool FNeedRefresh;
protected:
	virtual TRESTRequestParameterList* __fastcall GetCommitParam();
	virtual TRESTRequestParameterList* __fastcall GetNotPassParam(String reason);
	virtual void __fastcall Commit(System::Uitypes::TModalResult AResult);
	virtual void __fastcall AfterCommit(TClientDataSet *Result){};
	virtual void __fastcall AfterNotPass(TClientDataSet *Result){};
public:		// User declarations
	__fastcall TfrmWorkFlowDetailForm(TComponent* Owner,TClientBroker *clBroker,int modulecode,String param);
	__fastcall TfrmWorkFlowDetailForm(TComponent* Owner,TClientBroker *clBroker,int modulecode,TClientDataSet *m_DataSet,String param);
	__property String CommitUrl={ read=FCommitUrl,write=FCommitUrl};
	__property String NotPassUrl={ read=FNotPassUrl,write=FNotPassUrl};
	__property String LoadUrl={ read=FLoadUrl,write=FLoadUrl};
	__property bool NeedRefresh={ read=FNeedRefresh,write=FNeedRefresh};
	virtual void __fastcall NotPass(String reason);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWorkFlowDetailForm *frmWorkFlowDetailForm;
//---------------------------------------------------------------------------
#endif
