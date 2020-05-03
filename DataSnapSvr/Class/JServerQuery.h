//---------------------------------------------------------------------------

#ifndef JServerQueryH
#define JServerQueryH
#include <ADODB.hpp>
#include <DB.hpp>
#include "BaseServerClass.h"
//---------------------------------------------------------------------------
class TJServerQuery : public TDMBaseServerClass
{
	private:
		int FSysDbType;
		int FAccDbType;
		String FUserCode;
		String FUserIP;
		String FAcckBook;
		TADOConnection *m_Connection;
		TADOConnection *m_BuildConn;
		TADOQuery *m_Query;
		TADOQuery *b_Query;
		TADOQuery *LogQuery;
		void  WriteUserLog(int type, String Desc);
		TJSONObject*  SendFailMessage(int operate, String errormessage);
		TJSONObject*  SendErrorMessage(int errortype, int errorcode, String errormessage);
	public:
		__fastcall TJServerQuery(TComponent* Owner);
		__fastcall ~TJServerQuery();
	public:
		TJSONObject*  QueryOpen(String SQLText);
		/*********************************************************************
		// 传入JSONObject格式
		//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
		//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
		// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
		//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
		// MasterStatus:}
		// 返回JSONObject格式由数据(Data)、列信息(Cols)和业务类信息组成：
		//{Master:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
		//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
		// Detail:{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
		//         Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]},
		// Status:}
		**********************************************************************/
		TJSONObject*  QueryUpdate(TJSONObject *Json, String ClassCode);
		int  QueryExecSQL(String SQLText);

		// 执行系统库
		TJSONObject*  QuerySysOpen(String SQLText);
		int  QuerySysExecSQL(String SQLText);
};
#endif
