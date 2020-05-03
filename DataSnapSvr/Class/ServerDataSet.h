//---------------------------------------------------------------------------

#ifndef ServerDataSetH
#define ServerDataSetH
#include <ADODB.hpp>
#include <DB.hpp>
#include "BaseServerClass.h"
#include "MsgStream.h"
//---------------------------------------------------------------------------
class TServerDataSet : public TDMBaseServerClass
{
private:
	TClientDataSet *m_MasterDataSet;
	TClientDataSet *m_DetailDataSet;
	TClientDataSet *DeltaDetailDataSet;

	TADOStoredProc *m_StoredProc;
	TADOQuery *m_Query;
	TADOQuery *b_Query;
	TADOQuery *LogQuery;

	TADOConnection *m_Connection;
	TADOQuery *m_BuildQuery;
	TADOConnection *m_BuildConn;
	String m_ClassCode;

	bool m_Active;
	int m_Status;
	int m_DataSetType;

	String m_MasterSQLString;
	String m_DetailSQLString;
	String m_MasterFixedWhereString;
	String m_MasterWhereString;
	String m_DetailFixedWhereString;
	String m_DetailWhereString;
	String m_MasterFixedOrderString;
	String m_MasterOrderString;
	String m_DetailFixedOrderString;
	String m_DetailOrderString;
	String m_MasterFixedGroupString;
	String m_MasterGroupString;
	String m_DetailFixedGroupString;
	String m_DetailGroupString;
	String m_MasterDisplayName;
	String m_DetailDisplayName;
	String m_MasterDisplayField;
	String m_DetailDisplayField;
	TStringList *m_MasterLinkFields;
	TStringList *m_DetailLinkFields;
	String m_MasterFilterFields;
	String m_DetailFilterFields;
	String m_MasterFieldAttr;
	String m_DetailFieldAttr;
	String m_MasterFieldExpr;
	String m_DetailFieldExpr;
	String m_MasterAttrType;
	String m_DetailAttrType;

	String m_MasterKeyFields;
	String m_DetailKeyFields;

	String m_ClientDataSetHandle;
	TStringList *m_ServerBuffer;

	int FSysDbType;
	int FAccDbType;
	String FUserCode;
	String FUserIP;
	String FAcckBook;
	TStream*  SendErrorMessage(int errortype, int errorcode, String errormessage);
	TStream*  SendFailMessage(int operate, String errormessage);
	void  WriteUserLog(int type, String Desc);
	void  InitServerDataSet(String ClassCode);
protected:
	String  BuildLinkStr();
	String  BuildLinkFields();
	void  ExtractLinkFields(String value);
	void  BuildDetailFilter();
	int  CheckFieldType(TField *Field);
public:
	__fastcall TServerDataSet(TComponent* Owner);
	__fastcall ~TServerDataSet();


	TStream*  GetInitResult(String ClassCode);
	TStream*  Open(TStream *Stream,String ClassCode);
	TStream*  Update(TStream *Stream,String ClassCode);
	TStream*  Delete(TStream *Stream,String ClassCode);
	TStream*  DeleteDetail(TStream *Stream,String ClassCode);
	TStream*  CmdExec(TStream *Stream,String ClassCode);
	TStream*  Check(TStream *Stream,String ClassCode);
	TStream*  CloseOff(TStream *Stream,String ClassCode);
	TStream*  Revoke(TStream *Stream,String ClassCode);
	TStream*  Tally(TStream *Stream,String ClassCode);
	TStream*  ExecSQL(TStream *Stream);
	TStream*  ValidateData(TStream *Stream,String ClassCode);

	TStream*  QueryOpen(String SQLText);
	//	TStream*  QueryUpdate(TStream *Stream);
	//	TStream*  QueryDelete(TStream *Stream);
	TStream*  QueryExecSQL(String SQLText);
	// ִ��ϵͳ��
	TStream*  QuerySysOpen(String SQLText);
	//	TStream*  QuerySysUpdate(TStream *Stream);
	//	TStream*  QuerySysDelete(TStream *Stream);
	TStream*  QuerySysExecSQL(String SQLText);

};
#endif
