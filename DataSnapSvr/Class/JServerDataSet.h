//---------------------------------------------------------------------------

#ifndef JServerDataSetH
#define JServerDataSetH
#include <ADODB.hpp>
#include <DB.hpp>
#include "BaseServerClass.h"
//---------------------------------------------------------------------------
class TJServerDataSet : public TDMBaseServerClass
{
private:
	TADODataSet *m_MasterDataSet;
	TADODataSet *m_DetailDataSet;
	TADOStoredProc *m_StoredProc;

	TADOQuery *m_BuildQuery;

	TADOConnection *m_Connection;
	TADOConnection *m_BuildConn;
	TADOQuery *m_Query;
	TADOQuery *b_Query;
	TADOQuery *LogQuery;
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
	TJSONObject*  SendErrorMessage(int errortype, int errorcode, String errormessage);
	TJSONObject*  SendFailMessage(int operate, String errormessage);
	void  WriteUserLog(int type, String Desc);
	void  InitServerDataSet(String ClassCode);
protected:
	String  BuildLinkStr();
	String  BuildLinkFields();
	void  ExtractLinkFields(String value);
	void  BuildDetailFilter();
	int  CheckFieldType(TField *Field);
public:
	__fastcall TJServerDataSet(TComponent* Owner);
	__fastcall ~TJServerDataSet();

	/*********************************************************************
	// 返回JSONObject格式由数据(Data)、列信息(Cols)和业务类信息组成：
	//{Master:{Data:[],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Detail:{Data:[],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Operate:,Status:,DataSetType:,MasterKeyFields:,MasterDisplayName:,
	// MasterDisplayField:,MasterFilterFields:,MasterFieldAttr:,MasterAttrType:,MasterFieldExpr:,
	// MasterSqlString:,MasterFixedWhere:,MasterFixedOrder:,MasterFixedGroup:,
	// DetailKeyFields:,DetailDisplayName:,DetailDisplayField:,DetailFilterFields:,
	// DetailFieldAttr:,DetailAttrType:,DetailFieldExpr:,DetailSqlString:,
	// DetailFixedWhere:,DetailFixedOrder:,DetailFixedGroup,MasterLinkFields:,DetailLinkFields:}
	**********************************************************************/
	TJSONObject*  GetInitResult(String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	// {MasterWhere:,MasterOrder:,DetailWhere:,DetailOrder:}
	// 返回JSONObject格式由数据(Data)和列信息(Cols)组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  Open(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	//{OldMaster:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// OldDetail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// MasterStatus:1-浏览;2-修改;3-新增, DetailStatus:1-浏览;3-新增;2-修改;0-删除}
	// 返回JSONObject格式由数据(Data)、列信息(Cols)和业务类信息组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  Update(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	// {Status:}
	**********************************************************************/
	TJSONObject*  Delete(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	// {Status:}
	**********************************************************************/
	TJSONObject*  DeleteDetail(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	// 返回JSONObject格式由业务类信息组成：
	// {Status:}
	**********************************************************************/
	TJSONObject*  ValidateData(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	// 返回JSONObject格式由业务类信息组成：
	// {Status:}
	**********************************************************************/
	TJSONObject*  CmdExec(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  Check(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  CloseOff(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  Revoke(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]}
	// 返回JSONObject格式由业务类信息组成：
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  Tally(TJSONObject *Json, String ClassCode);
	/*********************************************************************
	// 传入JSONObject格式
	// {SQLStr:}
	// 返回JSONObject格式
	// {Status:}
	**********************************************************************/
	TJSONObject*  ExecSQL(TJSONObject *Json);
	/*********************************************************************
	// 返回JSONObject格式
	//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
	// Status:}
	**********************************************************************/
	TJSONObject*  QueryOpen(String SQLText);
	/*********************************************************************
	// 返回JSONObject格式
	// {Status:,RowsAffected:}
	**********************************************************************/
	TJSONObject*  QueryExecSQL(String SQLText);

	// 执行系统库
	TJSONObject*  QuerySysOpen(String SQLText);
	TJSONObject*  QuerySysExecSQL(String SQLText);
};
#endif
