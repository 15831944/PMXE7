//---------------------------------------------------------------------------

#ifndef BaseServerClassH
#define BaseServerClassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Datasnap.DSProviderDataModuleAdapter.hpp>
#include <Datasnap.DSServer.hpp>
#include "LoginUserRecord.h"
#include <Data.SqlExpr.hpp>
#include <IndyPeerImpl.hpp>

//---------------------------------------------------------------------------
class  DECLSPEC_DRTTI TDMBaseServerClass : public TComponent
{
__published:	// IDE-managed Components
private:	// User declarations
	TADOConnection *__fastcall GetSysConnection();
	TADOConnection *__fastcall GetAccConnection();
	int __fastcall GetSysDbType();
protected:
	TLoginUserRecord * __fastcall GetCurLoginUser();
	__property TADOConnection * SysConnection = { read = GetSysConnection };
	__property TADOConnection * AccConnection = { read = GetAccConnection };
	__property int SysDbType ={read = GetSysDbType};

	/*********************************************************************
	//���ص�JSON�ַ�����ʽ������(Data)������Ϣ(Cols)��ɣ�
	//{Data:[{FieldName0:FieldValue0},{FieldName1:FieldValue1},{FieldName2:FieldValue2}...],
	// Cols:[{FieldIndex:},{FieldType:},{FieldSize:},{FieldName:},{Required:}...]
	**********************************************************************/
	String __fastcall DataSetToJson(String SQLText);
	TJSONObject* __fastcall DataSetToJsonObject(String SQLText);
	String __fastcall DataSetSysToJson(String SQLText);
	TJSONObject* __fastcall DataSetSysToJsonObject(String SQLText);
public:		// User declarations
	__fastcall TDMBaseServerClass(TComponent* Owner);
	__fastcall virtual ~TDMBaseServerClass();


};
//---------------------------------------------------------------------------
extern PACKAGE TDMBaseServerClass *DMBaseServerClass;
//---------------------------------------------------------------------------
#endif