//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ServerDataSet.h"
#include "BaseFunc.h"
#include "SHConst.h"
#include "XMLData.h"
#include "FSServerContainer.h"
//---------------------------------------------------------------------------
// 如果虚构的字段没有赋值，将会报错E_Fail
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TServerDataSet::TServerDataSet(TComponent* Owner)
:TDMBaseServerClass(Owner)
{
	m_Active   = false; // 关闭数据集
	m_Status   = -1; // -1-尚未初始化,0-准备初始化,1-已初始化
	FSysDbType = 1;
	FAccDbType = 1;


	m_MasterDataSet = new TClientDataSet(NULL);

	m_DetailDataSet = new TClientDataSet(NULL);
	DeltaDetailDataSet =new TClientDataSet(NULL);

	m_MasterLinkFields = new TStringList;
	m_DetailLinkFields = new TStringList;

	m_Query = new TADOQuery(NULL);
	m_Query->ParamCheck = false;
	m_Query->EnableBCD = false;
	m_Query->CommandTimeout = 60;

	b_Query = new TADOQuery(NULL);
	b_Query->ParamCheck = false;
	b_Query->EnableBCD = false;
	b_Query->CommandTimeout = 60;

	m_StoredProc = new TADOStoredProc(NULL);
	m_StoredProc->EnableBCD = false;

	m_BuildQuery = new TADOQuery(NULL);
	m_BuildQuery->ParamCheck = false;
	m_BuildQuery->EnableBCD = false;

	LogQuery = new TADOQuery(NULL);
	LogQuery->ParamCheck = false;

	m_Connection = this->AccConnection;
	m_Query->Connection = m_Connection;
	m_StoredProc->Connection = m_Connection;

	m_BuildConn = this->SysConnection;
	m_BuildQuery->Connection = m_BuildConn;
	b_Query->Connection = m_BuildConn;
	LogQuery->Connection = m_BuildConn;

	m_MasterSQLString    = "";
	m_DetailSQLString    = "";
	m_MasterWhereString  = "";
	m_DetailWhereString  = "";
	m_MasterOrderString  = "";
	m_DetailOrderString  = "";
	m_MasterGroupString  = "";
	m_DetailGroupString  = "";

	m_MasterKeyFields    = "";
	m_DetailKeyFields    = "";
	m_MasterDisplayName  = "";
	m_DetailDisplayName  = "";
	m_MasterDisplayField = "";
	m_DetailDisplayName  = "";

	TLoginUserRecord *curLoginUser = GetCurLoginUser();
	if(curLoginUser)
	{
		FSysDbType = curLoginUser->SysDbType;
		FAccDbType = curLoginUser->AccDbType;
		FUserCode  = curLoginUser->UserLoginCode;
		FUserIP    = curLoginUser->IP;
		FAcckBook  = curLoginUser->AccBookID;
	}
}
//---------------------------------------------------------------------------
__fastcall TServerDataSet::~TServerDataSet()
{
	delete m_MasterDataSet;
	delete m_DetailDataSet;
	delete DeltaDetailDataSet;
	delete m_StoredProc;
	delete m_BuildQuery;
	delete m_Query;
	delete b_Query;
	delete LogQuery;
	delete m_MasterLinkFields;
	delete m_DetailLinkFields;
}
//---------------------------------------------------------------------------
void  TServerDataSet::InitServerDataSet(String ClassCode)
{
	String sqlstr, sqlstr1, sendbuff, retstr;
	int index, endpos;
	String s1, s2;
	m_ClassCode = ClassCode;
	try
	{
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add("select * from SyMiddleClass where SyMCCode='" + m_ClassCode + "'");
		m_BuildQuery->Open();
		if(m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("尚未建立主表应用服务器数据");
		}

		m_DataSetType            = m_BuildQuery->FieldByName("SyMCType")->AsInteger;
		m_MasterSQLString        = m_BuildQuery->FieldByName("SyMCMasterSQL")->AsString.Trim();
		m_MasterFixedWhereString = m_BuildQuery->FieldByName("SyMCMasterFixCondition")->AsString.Trim();
		m_MasterWhereString      = m_BuildQuery->FieldByName("SyMCMasterCondition")->AsString.Trim();
		m_MasterFixedOrderString = m_BuildQuery->FieldByName("SyMCMasterFixOrder")->AsString.Trim();
		m_MasterOrderString      = m_BuildQuery->FieldByName("SyMCMasterOrder")->AsString.Trim();
		m_MasterFixedGroupString = m_BuildQuery->FieldByName("SyMCMasterFixGroup")->AsString.Trim();
		m_MasterGroupString      = m_BuildQuery->FieldByName("SyMCMasterGroup")->AsString.Trim();
		m_MasterKeyFields        = m_BuildQuery->FieldByName("SyMCMasterKeyFields")->AsString.Trim();
		if(m_DataSetType == 2)
		{
			m_DetailSQLString        = m_BuildQuery->FieldByName("SyMCDetailSQL")->AsString.Trim();
			m_DetailFixedWhereString = m_BuildQuery->FieldByName("SyMCDetailFixCondition")->AsString.Trim();
			m_DetailWhereString      = m_BuildQuery->FieldByName("SyMCDetailCondition")->AsString.Trim();
			m_DetailFixedOrderString = m_BuildQuery->FieldByName("SyMCDetailFixOrder")->AsString.Trim();
			m_DetailOrderString      = m_BuildQuery->FieldByName("SyMCDetailOrder")->AsString.Trim();
			m_DetailFixedGroupString = m_BuildQuery->FieldByName("SyMCDetailFixGroup")->AsString.Trim();
			m_DetailGroupString      = m_BuildQuery->FieldByName("SyMCDetailGroup")->AsString.Trim();
			m_DetailKeyFields        = m_BuildQuery->FieldByName("SyMCDetailKeyFields")->AsString.Trim();
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add("select * from SyMCRelaField where SyMCRFClassCode='" + m_ClassCode + "'");
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("尚未建立从表应用服务器数据");
			}

			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				m_MasterLinkFields->Add(m_BuildQuery->FieldByName("SyRFMasterField")->AsString.Trim());
				m_DetailLinkFields->Add(m_BuildQuery->FieldByName("SyMCRFDetailField")->AsString.Trim());
				m_BuildQuery->Next();
			}
			m_BuildQuery->Close();

			m_BuildQuery->SQL->Text = " select SyDFFieldName,SyDFTitle,SyDFIsQuery,SyDFFieldType,SyMCExpandFieldSQL,SyMCExpandFieldType"
																" from SyMCDetailField left outer join SyMCExpandField on SyDFFieldType=SyMCExpandFieldCode"
																" where SyDFClassCode='" + m_ClassCode + "' order by SyDFNo";
			m_BuildQuery->Open();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				m_DetailDisplayName  += m_BuildQuery->FieldByName("SyDFTitle")->AsString + "\r\n";
				m_DetailDisplayField += m_BuildQuery->FieldByName("SyDFFieldName")->AsString + "\r\n";

				m_DetailFilterFields += m_BuildQuery->FieldByName("SyDFIsQuery")->AsString + "\r\n";
				m_DetailFieldAttr    += m_BuildQuery->FieldByName("SyDFFieldType")->AsString + "\r\n";

				if(m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->IsNull)
					s1 = "";
				else
					s1 = m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->AsString;
				s2.SetLength(4);
				swprintf(s2.c_str(), L"%04d", s1.Length());

				m_DetailFieldExpr += s2 + s1;
				s1 = "";
				s2 = "";
				if(m_BuildQuery->FieldByName("SyMCExpandFieldType")->IsNull)
					m_DetailAttrType += "0\r\n";
				else
					m_DetailAttrType += m_BuildQuery->FieldByName("SyMCExpandFieldType")->AsString + "\r\n";

				m_BuildQuery->Next();
			}
		}
		m_BuildQuery->Close();

		m_BuildQuery->SQL->Text = " select SyMFFieldName,SyMFTitle,SyMFIsQuery,SyMFFieldType,SyMCExpandFieldSQL,SyMCExpandFieldType"
															" from SyMCMasterField left outer join SyMCExpandField on SyMFFieldType=SyMCExpandFieldCode"
															" where SyMFClassCode='" + m_ClassCode + "' order by SyMFNo";
		m_BuildQuery->Open();
		m_BuildQuery->First();
		while (!m_BuildQuery->Eof)
		{
			m_MasterDisplayName  += m_BuildQuery->FieldByName("SyMFTitle")->AsString + "\r\n";
			m_MasterDisplayField += m_BuildQuery->FieldByName("SyMFFieldName")->AsString + "\r\n";

			m_MasterFilterFields += m_BuildQuery->FieldByName("SyMFIsQuery")->AsString + "\r\n";
			m_MasterFieldAttr    += m_BuildQuery->FieldByName("SyMFFieldType")->AsString + "\r\n";

			// **以后都要改成与用户定义无关的系统数据结构定义,如上面的字段属性类型(SyMCExpandFieldSQL)
			if(m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->IsNull)
				s1 = "";
			else
				s1 = m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->AsString;
			s2.SetLength(4);
			swprintf(s2.c_str(), L"%04d", s1.Length());

			m_MasterFieldExpr += s2 + s1;
			s1 = "";
			s2 = "";
			if(m_BuildQuery->FieldByName("SyMCExpandFieldType")->IsNull)
				m_MasterAttrType += "0\r\n";
			else
				m_MasterAttrType += m_BuildQuery->FieldByName("SyMCExpandFieldType")->AsString + "\r\n";

			m_BuildQuery->Next();
		}
		m_BuildQuery->Close();

		if(m_MasterSQLString.Trim().IsEmpty())
			throw Exception("主表SQL语句尚未赋值.");


		if(m_DataSetType == 2)
		{
			if(m_DetailSQLString.Trim().IsEmpty())
				throw Exception("从表SQL语句尚未赋值.");

			if(m_MasterLinkFields->Count == 0)
				throw Exception("尚未建立关联字段.");
		}
		m_Status = 1;
	}
	catch (Exception &e)
	{
		throw Exception("初始化服务器数据集错误，\r\n" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::GetInitResult(String ClassCode)
{
	String sqlstr;
	TMsgStream *sxs;
	TMemoryStream *ms = new TMemoryStream();
	try
	{
		InitServerDataSet(ClassCode);
		sxs = new TMsgStream();
		sxs->Operate = asCreateSDataSet;
		sxs->ValueAttribute["status"] = "s";
		sxs->ValueAttribute["datasettype"] = m_DataSetType;
		sxs->ValueAttribute["masterkeyfields"] = m_MasterKeyFields;
		sxs->ValueAttribute["detailkeyfields"] = m_DetailKeyFields;
		sxs->ValueAttribute["masterdisplayname"] = m_MasterDisplayName;
		sxs->ValueAttribute["detaildisplayname"] = m_DetailDisplayName;

		sxs->ValueAttribute["masterdisplayfield"] = m_MasterDisplayField;
		sxs->ValueAttribute["detaildisplayfield"] = m_DetailDisplayField;

		sxs->ValueAttribute["masterfilterfields"] = m_MasterFilterFields;
		sxs->ValueAttribute["detailfilterfields"] = m_DetailFilterFields;
		sxs->ValueAttribute["masterfieldattr"] = m_MasterFieldAttr;
		sxs->ValueAttribute["detailfieldattr"] = m_DetailFieldAttr;
		sxs->ValueAttribute["masterattrtype"] = m_MasterAttrType;
		sxs->ValueAttribute["detailattrtype"] = m_DetailAttrType;
		sxs->ItemAttribute["masterfieldexpr"] = m_MasterFieldExpr;
		sxs->ItemAttribute["detailfieldexpr"] = m_DetailFieldExpr;

		sxs->ValueAttribute["mastersqlstring"] = m_MasterSQLString;
		sxs->ValueAttribute["masterFixedwhere"] = m_MasterFixedWhereString;
		sxs->ValueAttribute["masterFixedorder"] = m_MasterFixedOrderString;
		sxs->ValueAttribute["masterfixedgroup"] = m_MasterFixedGroupString;

		if(m_DataSetType == 2)
		{
			sxs->ItemAttribute["masterlinkfields"] = m_MasterLinkFields->Text;
			sxs->ItemAttribute["detaillinkfields"] = m_DetailLinkFields->Text;

			sxs->ValueAttribute["detailsqlstring"] = m_DetailSQLString;
			sxs->ValueAttribute["detailFixedwhere"] = m_DetailFixedWhereString;
			sxs->ValueAttribute["detailFixedorder"] = m_DetailFixedOrderString;
			sxs->ValueAttribute["detailfixedgroup"] = m_DetailFixedGroupString;

		}

		// 只获取表结构
		sqlstr = m_MasterSQLString + " \r\n where 1=0";

		m_Query->Close();
		m_Query->Connection = m_Connection;
		m_Query->SQL->Text = sqlstr;
		m_Query->Open();
		sxs->LoadMasterFromStream(AdoDataSetToCDStream(m_Query));
		m_Query->Close();
		if(m_DataSetType == 2)
		{
			// 只获取表结构
			sqlstr = m_DetailSQLString + " \r\n where 1=0";

			m_Query->Close();
			m_Query->Connection = m_Connection;
			m_Query->SQL->Text = sqlstr;
			m_Query->Open();
			sxs->LoadDetailFromStream(AdoDataSetToCDStream(m_Query));
			m_Query->Close();
		}
		sxs->SaveToStream(ms);
		ms->Position=0;
		delete sxs;
		return ms;
	}
	catch (Exception &e)
	{
		delete ms;
		return SendFailMessage(asDSError, "发送初始化数据错误，\r\n" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Open(TStream *Stream,String ClassCode)
{
	String sqlstr, sqlstr1, retstr;

	TMsgStream *sxs = NULL;
	String mlfs, dlfs, sendbuff;
	int endpos;
	bool retval;
	TMsgStream  *MsgStream = new TMsgStream(Stream);
	try
	{
		InitServerDataSet(ClassCode);
		if(m_MasterSQLString.Trim().IsEmpty())
			throw Exception("主表SQL语句尚未赋值.");

		sxs = new TMsgStream();
		sxs->Operate = asDSOpen;
		sxs->ValueAttribute["status"] = "s";

		sqlstr = m_MasterSQLString;
		if(!m_MasterFixedWhereString.Trim().IsEmpty())
		{
			sqlstr += " where " + m_MasterFixedWhereString;
			if(MsgStream->ValueAttribute["masterwhere"].Trim().IsEmpty())
			{
				if(!m_MasterWhereString.Trim().IsEmpty())
					sqlstr += " and (" + m_MasterWhereString + ")";
			}
			else
			{
				sqlstr += " and (" + MsgStream->ValueAttribute["masterwhere"] + ")";
			}
		}
		else
		{
			if(MsgStream->ValueAttribute["masterwhere"].Trim().IsEmpty())
			{
				if(!m_MasterWhereString.Trim().IsEmpty())
					sqlstr += " where " + m_MasterWhereString;
			}
			else
			{
				sqlstr += " where " + MsgStream->ValueAttribute["masterwhere"];
			}
		}

		if(!m_MasterFixedOrderString.Trim().IsEmpty())
		{
			sqlstr += " Order by " + m_MasterFixedOrderString;
			if(MsgStream->ValueAttribute["masterorder"].Trim().IsEmpty())
			{
				if(!m_MasterOrderString.Trim().IsEmpty())
					sqlstr += "," + m_MasterOrderString;
			}
			else
				sqlstr += "," + MsgStream->ValueAttribute["masterorder"];
		}
		else
		{
			if(MsgStream->ValueAttribute["masterorder"].Trim().IsEmpty())
			{
				if(!m_MasterOrderString.Trim().IsEmpty())
					sqlstr += " Order by " + m_MasterOrderString;
			}
			else
				sqlstr += " Order by " + MsgStream->ValueAttribute["masterorder"];
		}

		m_Query->Close();
		m_Query->Connection = m_Connection;
		m_Query->SQL->Text = sqlstr;
		m_Query->Open();
		sxs->LoadMasterFromStream(AdoDataSetToCDStream(m_Query));
		m_Query->Close();
		if(m_DataSetType == 2)
		{
			if(m_DetailSQLString.Trim().IsEmpty())
			  throw Exception("从表SQL语句尚未赋值.");

			if(m_MasterLinkFields->Count == 0)
				throw Exception("尚未建立关联字段.");

			sqlstr1 = m_DetailSQLString;
			sqlstr1 += " where exists(" + m_MasterSQLString;
			if(!m_MasterFixedWhereString.Trim().IsEmpty())
			{
				sqlstr1 += " where " + m_MasterFixedWhereString;
				if(MsgStream->ValueAttribute["masterwhere"].Trim().IsEmpty())
				{
					if(!m_MasterWhereString.Trim().IsEmpty())
						sqlstr1 += " and (" + m_MasterWhereString + ")";
				}
				else
				{
					sqlstr1 += " and (" + MsgStream->ValueAttribute["masterwhere"] + ")";
				}
				sqlstr1 += " and " + BuildLinkStr() + ")";
			}
			else
			{
				if(MsgStream->ValueAttribute["masterwhere"].Trim().IsEmpty())
				{
					if(!m_MasterWhereString.Trim().IsEmpty())
						sqlstr1 += " where " + m_MasterWhereString + " and " + BuildLinkStr() + ")";
					else
						sqlstr1 += " where " + BuildLinkStr() + ")";
				}
				else
					sqlstr1 += " where " + MsgStream->ValueAttribute["masterwhere"] + " and " + BuildLinkStr() + ")";
			}

			if(!m_DetailFixedWhereString.Trim().IsEmpty())
			{
				sqlstr1 += " and " + m_DetailFixedWhereString;
				if(MsgStream->ValueAttribute["detailwhere"].Trim().IsEmpty())
				{
					if(!m_DetailWhereString.Trim().IsEmpty())
						sqlstr1 += " and " + m_DetailWhereString;
				}
				else
					sqlstr1 += " and " + MsgStream->ValueAttribute["detailwhere"];
			}
			else
			{
				if(MsgStream->ValueAttribute["detailwhere"].Trim().IsEmpty())
				{
					if(!m_DetailWhereString.Trim().IsEmpty())
						sqlstr1 += " and " + m_DetailWhereString;
				}
				else
					sqlstr1 += " and " + MsgStream->ValueAttribute["detailwhere"];
			}

			if(!m_DetailFixedOrderString.Trim().IsEmpty())
			{
				sqlstr1 += " order by " + m_DetailFixedOrderString;
				if(MsgStream->ValueAttribute["detailorder"].Trim().IsEmpty())
				{
					if(!m_DetailOrderString.Trim().IsEmpty())
						sqlstr1 += "," + m_DetailOrderString;
				}
				else
					sqlstr1 += "," + MsgStream->ValueAttribute["detailorder"];
			}
			else
			{
				if(MsgStream->ValueAttribute["detailorder"].Trim().IsEmpty())
				{
					if(!m_DetailOrderString.Trim().IsEmpty())
						sqlstr1 += " order by " + m_DetailOrderString;
				}
				else
					sqlstr1 += " order by " + MsgStream->ValueAttribute["detailorder"];
			}

			m_Query->Close();
			m_Query->Connection = m_Connection;
			m_Query->SQL->Text = sqlstr1;
			m_Query->Open();
			sxs->LoadDetailFromStream(AdoDataSetToCDStream(m_Query));
			m_Query->Close();
		}
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		delete sxs;
		resultStream->Position = 0;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "打开数据集错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Update(TStream *Stream,String ClassCode)
{
	String SqlStr, s, FParamName, Descstr = "";
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs=NULL;
	Stream->Position=0;
	TParameter *FParam;
	bool retval;
	Variant VarVal;
	TMsgStream  *MsgStream = new TMsgStream(Stream);
	try
	{
		InitServerDataSet(ClassCode);
		sxs = new TMsgStream();
		sxs->Operate = asDSUpdate;
		m_MasterDataSet->Close();
		MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		m_Connection->BeginTrans();
		if(MsgStream->ValueAttribute["masterstatus"].ToInt() == dsInsert)
		{
			SqlStr = " select SyMCMasterAddProc,SyMCMAddProcParam.*,SyDataTypeForCPP"
							 " from SyMiddleClass,SyMCMAddProcParam,SyDataType"
							 " where SyMCCode='"+m_ClassCode+"' and SyMCMAClassCode=SyMCCode and SyDataTypeCode=SyMCMAParamDataType"
							 " order by SyMCMAParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(SqlStr);
			m_BuildQuery->Open();
			if (m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				m_Connection->RollbackTrans();
				throw Exception("主表增加存储过程尚未建立.");
			}
			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterAddProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger == 1)
					dir = pdInput;
				else if (m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMAParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMAParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMAParamSize")->AsInteger, NULL);
				VarVal = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMAParamField")->AsString];
				FParam->DataType = datatype;
				FParam->Value = VarVal;
				Descstr += VarToStr(VarVal) + ",";
				m_BuildQuery->Next();
			}
			m_StoredProc->ExecProc();
			WriteUserLog(1, Descstr);
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMAParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMAParamField")->AsString)->ReadOnly = false;

					if(FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMAParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMAParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMAParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
//			m_MasterDataSet->MergeChangeLog();
			sxs->LoadMasterFromDataSet(m_MasterDataSet);
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_StoredProc->Close();
		}
		else //if (MsgStream->ValueAttribute["masterstatus"].ToInt() == dsEdit)
		{
			SqlStr = " select SyMCMasterEditProc,SyMCMEditProcParam.*,SyDataTypeForCPP"
							 " from SyMiddleClass,SyMCMEditProcParam,SyDataType"
							 " where SyMCCode='"+m_ClassCode+"' and SyMCMEClassCode=SyMCCode and SyDataTypeCode=SyMCMEParamDataType"
							 " order by SyMCMEParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(SqlStr);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				m_Connection->RollbackTrans();
				throw Exception("主表更新存储过程尚未建立.");
			}
			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterEditProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if (m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger == 1)
					dir = pdInput;
				else if (m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if (FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMEParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMEParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMEParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				if(m_BuildQuery->FieldByName("SyMCMEParamOldValue")->AsInteger == 1)
				{
					FParam->Value = m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMEParamField")->AsString)->OldValue;
				}
				else
				{
					FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMEParamField")->AsString];
				}
				Descstr += VarToStr(FParam->Value) + ",";
				m_BuildQuery->Next();
			}
			m_StoredProc->ExecProc();
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMEParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMEParamField")->AsString)->ReadOnly = false;

					if (FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMEParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMEParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMEParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
			sxs->LoadMasterFromDataSet(m_MasterDataSet);
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_StoredProc->Close();
			WriteUserLog(2, Descstr);
		}

		if(m_DataSetType == 2)
		{
			m_DetailDataSet->Close();
			MsgStream->SaveDetailToDataSet(m_DetailDataSet);
			//先专门处理一下删除记录
			if(m_DetailDataSet->ChangeCount>0)
			{
			DeltaDetailDataSet->Data=m_DetailDataSet->Delta;
			DeltaDetailDataSet->First();
			while (!DeltaDetailDataSet->Eof)
			{
				TUpdateStatus dupdatestatus= DeltaDetailDataSet->UpdateStatus();
				if (dupdatestatus==usDeleted)
				{
					SqlStr = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
									 " from SyMiddleClass,SyMCDDelProcParam,SyDataType"
									 " where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
									 " order by SyMCDDParamNo";
					m_BuildQuery->Close();
					m_BuildQuery->SQL->Clear();
					m_BuildQuery->SQL->Add(SqlStr);
					m_BuildQuery->Open();
					if (m_BuildQuery->RecordCount < 1)
					{
						m_BuildQuery->Close();
						m_Connection->RollbackTrans();
						 throw Exception("删除存储过程尚未建立.");
					}
					m_BuildQuery->First();
					m_StoredProc->Close();
					m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
					m_StoredProc->Parameters->Clear();

					while (!m_BuildQuery->Eof)
					{
						datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
						if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
							dir = pdInput;
						else if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
							dir = pdOutput;
						else
							dir = pdInputOutput;

						if (FAccDbType == 1)
							FParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
						else
							FParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

						FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir,m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
						FParam->DataType = datatype;
						FParam->Value = DeltaDetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDDParamField")->AsString)->OldValue;
						m_BuildQuery->Next();
					}
					m_StoredProc->ExecProc();
					m_BuildQuery->Close();
					m_StoredProc->Close();
				}
				DeltaDetailDataSet->Next();

			}
			//处理新增和修改的
			m_DetailDataSet->First();
			while (!m_DetailDataSet->Eof)
			{
			  TUpdateStatus dupdatestatus= m_DetailDataSet->UpdateStatus();
				if(dupdatestatus==usInserted)
				{
					SqlStr = " select SyMCDetailAddProc,SyMCDAddProcParam.*,SyDataTypeForCPP"
									 " from SyMiddleClass,SyMCDAddProcParam,SyDataType"
									 " where SyMCCode='"+m_ClassCode+"' and SyMCDAClassCode=SyMCCode and SyDataTypeCode=SyMCDAParamDataType"
									 " order by SyMCDAParamNo";
					m_BuildQuery->Close();
					m_BuildQuery->SQL->Clear();
					m_BuildQuery->SQL->Add(SqlStr);
					m_BuildQuery->Open();
					if (m_BuildQuery->RecordCount < 1)
					{
						m_BuildQuery->Close();
						m_Connection->RollbackTrans();
						throw Exception("从表增加存储过程尚未建立.");
					}
					m_BuildQuery->First();
					m_StoredProc->Close();
					m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailAddProc")->AsString;
					m_StoredProc->Parameters->Clear();
					while (!m_BuildQuery->Eof)
					{
						datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
						if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 1)
							dir = pdInput;
						else if (m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 2)
							dir = pdOutput;
						else
							dir = pdInputOutput;

						if (FAccDbType == 1)
							FParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
						else
							FParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

						FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDAParamSize")->AsInteger, NULL);
						FParam->DataType = datatype;
						FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDAParamField")->AsString];
						m_BuildQuery->Next();
					}
					m_StoredProc->ExecProc();
					m_DetailDataSet->Edit();
					m_BuildQuery->First();
					while (!m_BuildQuery->Eof)
					{
						if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger != 1)
						{
							if(m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDAParamField")->AsString)->ReadOnly)
								m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDAParamField")->AsString)->ReadOnly = false;

							if (FAccDbType == 1)
								FParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
							else
								FParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

							m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDAParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
						}
						m_BuildQuery->Next();
					}
					m_DetailDataSet->Post();
					m_BuildQuery->Close();
					m_StoredProc->Close();
				}
				else if (dupdatestatus==usModified)
				{
					SqlStr = " select SyMCDetailEditProc,SyMCDEditProcParam.*,SyDataTypeForCPP"
									 " from SyMiddleClass,SyMCDEditProcParam,SyDataType"
									 " where SyMCCode='"+m_ClassCode+"' and SyMCDEClassCode=SyMCCode and SyDataTypeCode=SyMCDEParamDataType"
									 " order by SyMCDEParamNo";
					m_BuildQuery->Close();
					m_BuildQuery->SQL->Clear();
					m_BuildQuery->SQL->Add(SqlStr);
					m_BuildQuery->Open();
					if (m_BuildQuery->RecordCount < 1)
					{
						m_BuildQuery->Close();
						m_Connection->RollbackTrans();
						throw Exception("从表更新存储过程尚未建立.");
					}
					m_BuildQuery->First();
					m_StoredProc->Close();
					m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailEditProc")->AsString;
					m_StoredProc->Parameters->Clear();
					while (!m_BuildQuery->Eof)
					{
						datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
						if (m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 1)
							dir = pdInput;
						else if (m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 2)
							dir = pdOutput;
						else
							dir = pdInputOutput;

						if (FAccDbType == 1)
							FParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
						else
							FParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

						FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDEParamSize")->AsInteger, NULL);
						FParam->DataType = datatype;
						if (m_BuildQuery->FieldByName("SyMCDEParamOldValue")->AsInteger == 1)
							FParam->Value = m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->OldValue;
						else
							FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDEParamField")->AsString];
						m_BuildQuery->Next();
					}
					m_StoredProc->ExecProc();
					m_DetailDataSet->Edit();
					m_BuildQuery->First();
					while (!m_BuildQuery->Eof)
					{
						if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger != 1)
						{
							if(m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->ReadOnly)
								m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->ReadOnly = false;

							if (FAccDbType == 1)
								FParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
							else
								FParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

							m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDEParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
						}
						m_BuildQuery->Next();
					}
					m_DetailDataSet->Post();
					m_BuildQuery->Close();
					m_StoredProc->Close();
				}
			  /*  //前面已经先处理了
				else if (dupdatestatus==usDeleted)
				{
					SqlStr = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
									 " from SyMiddleClass,SyMCDDelProcParam,SyDataType"
									 " where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
									 " order by SyMCDDParamNo";
					m_BuildQuery->Close();
					m_BuildQuery->SQL->Clear();
					m_BuildQuery->SQL->Add(SqlStr);
					m_BuildQuery->Open();
					if (m_BuildQuery->RecordCount < 1)
					{
						m_BuildQuery->Close();
						m_Connection->RollbackTrans();
						 throw Exception("删除存储过程尚未建立.");
					}
					m_BuildQuery->First();
					m_StoredProc->Close();
					m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
					m_StoredProc->Parameters->Clear();

					while (!m_BuildQuery->Eof)
					{
						datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
						if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
							dir = pdInput;
						else if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
							dir = pdOutput;
						else
							dir = pdInputOutput;

						if (FAccDbType == 1)
							FParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
						else
							FParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

						FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir,m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
						FParam->DataType = datatype;
						FParam->Value = m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDDParamField")->AsString)->OldValue;
						m_BuildQuery->Next();
					}
					m_StoredProc->ExecProc();
					m_BuildQuery->Close();
					m_StoredProc->Close();
				} */
				m_DetailDataSet->Next();
			}
		   }
			m_DetailDataSet->Filtered = false; // zws 20090107
			sxs->LoadDetailFromDataSet(m_DetailDataSet);
			m_DetailDataSet->Close();
		}
		m_BuildQuery->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		m_Connection->CommitTrans();
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		if(sxs)delete sxs;
		return SendFailMessage(asDSError, "更新记录时错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Delete(TStream *Stream,String ClassCode)
{
	String SqlStr, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs;
	TMsgStream *MsgStream = new TMsgStream(Stream);
	TParameter *FParam;
	bool retval;
	try
	{
		InitServerDataSet(ClassCode);
		MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		SqlStr = " select SyMCMasterDelProc,SyMCMDelProcParam.*,SyDataTypeForCPP"
						 " from SyMiddleClass,SyMCMDelProcParam,SyDataType"
						 " where SyMCCode='"+m_ClassCode+"' and SyMCMDClassCode=SyMCCode and SyDataTypeCode=SyMCMDParamDataType"
						 " order by SyMCMDParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if(m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("删除存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterDelProc")->AsString;
		m_StoredProc->Parameters->Clear();
		String DescStr = "";
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCMDParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCMDParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCMDParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCMDParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMDParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMDParamField")->AsString];
			DescStr += VarToStr(FParam->Value) + ",";
			m_BuildQuery->Next();
		}

		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_Connection->CommitTrans();
		m_StoredProc->Close();
		WriteUserLog(0, DescStr);
		sxs = new TMsgStream();
		sxs->Operate = asDSDelete;
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream *resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position = 0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "删除记录时错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::DeleteDetail(TStream *Stream,String ClassCode)
{
	String SqlStr, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs;
	TParameter *FParam;
	bool retval;
	TMsgStream *MsgStream = new TMsgStream(Stream);
	try
	{
		InitServerDataSet(ClassCode);
		MsgStream->SaveDetailToDataSet(m_DetailDataSet);
		SqlStr = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
						 " from SyMiddleClass,SyMCDDelProcParam,SyDataType"
						 " where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
						 " order by SyMCDDParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if(m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception( "删除存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
		m_StoredProc->Parameters->Clear();
		String DescStr = "";
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDDParamField")->AsString];
			DescStr += VarToStr(FParam->Value) + ",";
			m_BuildQuery->Next();
		}
		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_Connection->CommitTrans();
		m_StoredProc->Close();

		WriteUserLog(6, DescStr);
		sxs = new TMsgStream();
		sxs->Operate = asDSDeleteDetail;
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream *resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position = 0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "删除记录时错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::ValidateData(TStream *Stream,String ClassCode)
{
	TMsgStream *sxs;
	bool retval;
	try
	{
		InitServerDataSet(ClassCode);
		sxs = new TMsgStream();
		sxs->Operate = asDSVerify;
		sxs->ClassID = m_ClassCode;
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "校验数据错误(系统)，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::CmdExec(TStream *Stream,String ClassCode)
{
	String SqlStr, s, LibName, LibPath, retstr;
	TMsgStream *sxs;
	bool retval;
	HINSTANCE hInst;
	typedef char*(CALLBACK * HCommandExecute)(int, int);
	HCommandExecute CommandExecute;
	TMsgStream  *MsgStream=new TMsgStream(Stream);
	int index;
	try
	{
	   InitServerDataSet(ClassCode);
		SqlStr =
			"select SyMLFileName,SyMLFilePath from SyMiddleLib,SyMiddleClass where SyMCCode='" +
			m_ClassCode + "' and SyMCOwnCode=SyMLCode";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Text = SqlStr;
		m_BuildQuery->Open();
		if (m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception( "业务类尚未建立.");
		}
		LibName = m_BuildQuery->FieldByName("SyMLFileName")->AsString;
		LibPath = m_BuildQuery->FieldByName("SyMLFilePath")->AsString;
		m_BuildQuery->Close();
		if (!LibName.Trim().IsEmpty())
		{
			index = LibName.Pos0(".");
			if (index > 0)
				LibName = LibName.SubString0(0, index+1) + "dll";
			else
				LibName = LibName + ".dll";
			hInst = LoadLibrary(LibName.c_str());
			if (hInst != NULL)
			{
				try
				{
					CommandExecute = (HCommandExecute)GetProcAddress(hInst, "CommandExecute");
					if (CommandExecute != NULL)
					{
						MsgStream->ValueAttribute["connptr"] = (int)m_Connection;
						retstr = CommandExecute(m_ClassCode.ToInt(), (int)MsgStream);
						if(retstr.Trim().IsEmpty())
						{
							sxs = new TMsgStream();
							sxs->Operate = asDSCommExecute;
							sxs->ValueAttribute["status"] = "s";
							sxs->MasterDataSet = MsgStream->MasterDataSet;
							sxs->DetailDataSet = MsgStream->DetailDataSet;
							TMemoryStream * resultStream=new TMemoryStream();
							sxs->SaveToStream(resultStream);
							resultStream->Position=0;
							delete sxs;
							FreeLibrary(hInst);
							return resultStream;
						}
						else
						{
							FreeLibrary(hInst);
							throw Exception( retstr);
						}
					}
					else
					{
						FreeLibrary(hInst);
						throw Exception( "读取库文件函数失败.");
					}
				}
				catch (Exception &e)
				{
					FreeLibrary(hInst);
					return SendFailMessage(asDSError, "执行通用命令错误，" + e.Message);
				}
			}
			else
				throw Exception( "载入库文件:" + LibName + "失败");
		}
		else
			throw Exception( "中间层处理库文件未定义.");
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "执行通用命令错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Check(TStream *Stream,String ClassCode)
{
	String SqlStr, s, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs;
	TParameter *FParam;
	TMsgStream  *MsgStream=new TMsgStream(Stream);
	bool retval;
	try
	{
		 InitServerDataSet(ClassCode);
		 MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		sxs = new TMsgStream();
		sxs->Operate = asDSCheck;

		SqlStr =
			"select SyMCMasterCheckProc,SyMCMCheckProcParam.*,SyDataTypeForCPP from SyMiddleClass,SyMCMCheckProcParam,SyDataType";
		SqlStr += " where SyMCCode='" + m_ClassCode +
			"' and SyMCMCkClassCode=SyMCCode and SyDataTypeCode=SyMCMCkParamDataType order by SyMCMCkParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if (m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("审核存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCheckProc")->AsString;
		m_StoredProc->Parameters->Clear();
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCMCkParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCMCkParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCkParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString];
			m_BuildQuery->Next();
		}

		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_MasterDataSet->Edit();
		m_BuildQuery->First();
		while (!m_BuildQuery->Eof)
		{
			if(m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger != 1)
			{
				if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString)->ReadOnly)
					m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString)->ReadOnly = false;

				if (FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCkParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCkParamOrcName")->AsString;

				m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
			}
			m_BuildQuery->Next();
		}
		m_MasterDataSet->Post();
//		m_MasterDataSet->MergeChangeLog();
		sxs->LoadMasterFromDataSet(m_MasterDataSet);
		m_MasterDataSet->Close();
		m_BuildQuery->Close();
		m_Connection->CommitTrans();
		m_StoredProc->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream=new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "审核数据错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::CloseOff(TStream *Stream,String ClassCode)
{
	String SqlStr, s, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs=NULL;
	TParameter *FParam;
	TMsgStream  *MsgStream=new TMsgStream(Stream);
	bool retval;
	try
	{
		InitServerDataSet(ClassCode);
		MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		sxs = new TMsgStream();
		sxs->Operate = asDSCloseOff;
		SqlStr =
			"select SyMCMasterCloseProc,SyMCMCloseProcParam.*,SyDataTypeForCPP from SyMiddleClass,SyMCMCloseProcParam,SyDataType";
		SqlStr += " where SyMCCode='" + m_ClassCode +
			"' and SyMCMCClassCode=SyMCCode and SyDataTypeCode=SyMCMCParamDataType order by SyMCMCParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if (m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("结清存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCloseProc")->AsString;
		m_StoredProc->Parameters->Clear();
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCMCParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCMCParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCParamField")->AsString];
			m_BuildQuery->Next();
		}

		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_MasterDataSet->Edit();
		m_BuildQuery->First();
		while (!m_BuildQuery->Eof)
		{
			if(m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger != 1)
			{
				if (m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCParamField")->AsString)->ReadOnly)
					m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCParamField")->AsString)->ReadOnly = false;

				if (FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCParamOrcName")->AsString;

				m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
			}
			m_BuildQuery->Next();
		}
		m_MasterDataSet->Post();
//		m_MasterDataSet->MergeChangeLog();
		sxs->LoadMasterFromDataSet(m_MasterDataSet);
		m_MasterDataSet->Close();
		m_BuildQuery->Close();
		m_Connection->CommitTrans();
		m_StoredProc->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream=new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		if(sxs) delete sxs;
		return SendFailMessage(asDSError, "结清数据错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Revoke(TStream *Stream,String ClassCode)
{
	String SqlStr, s, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs=NULL;
	TParameter *FParam;
	bool retval;
	TMsgStream  *MsgStream=new TMsgStream(Stream);
	try
	{
		InitServerDataSet(ClassCode);
		MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		sxs = new TMsgStream();
		sxs->Operate = asDSCancel;
		SqlStr =
			"select SyMCMasterCancelProc,SyMCMCancelProcParam.*,SyDataTypeForCPP from SyMiddleClass,SyMCMCancelProcParam,SyDataType";
		SqlStr += " where SyMCCode='" + m_ClassCode +
			"' and SyMCMCanClassCode=SyMCCode and SyDataTypeCode=SyMCMCanParamDataType order by SyMCMCanParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if (m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("作废存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCancelProc")->AsString;
		m_StoredProc->Parameters->Clear();
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCMCanParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCMCanParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCanParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString];
			m_BuildQuery->Next();
		}

		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_MasterDataSet->Edit();
		m_BuildQuery->First();
		while (!m_BuildQuery->Eof)
		{
			if (m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger != 1)
			{
				if (m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString)->ReadOnly)
					m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString)->ReadOnly = false;

				if (FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCanParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCanParamOrcName")->AsString;

				m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
			}
			m_BuildQuery->Next();
		}
		m_MasterDataSet->Post();
	   //	m_MasterDataSet->MergeChangeLog();
		sxs->LoadMasterFromDataSet(m_MasterDataSet);
		m_MasterDataSet->Close();
		m_BuildQuery->Close();
		m_Connection->CommitTrans();
		m_StoredProc->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream=new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "作废数据错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::Tally(TStream *Stream,String ClassCode)
{
	String SqlStr, s, FParamName;
	TFieldType datatype;
	TParameterDirection dir;
	TMsgStream *sxs=NULL;
	TParameter *FParam;
	bool retval;
	TMsgStream  *MsgStream=new TMsgStream(Stream);
	try
	{
		InitServerDataSet(ClassCode);
		MsgStream->SaveMasterToDataSet(m_MasterDataSet);
		sxs = new TMsgStream();
		sxs->Operate = asDSRecord;
		SqlStr =
			"select SyMCMasterCheckOutProc,SyMCMCheckOutProcParam.*,SyDataTypeForCPP from SyMiddleClass,SyMCMCheckOutProcParam,SyDataType";
		SqlStr += " where SyMCCode='" + m_ClassCode +
			"' and SyMCMCOClassCode=SyMCCode and SyDataTypeCode=SyMCMCOParamDataType order by SyMCMCOParamNo";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add(SqlStr);
		m_BuildQuery->Open();
		if (m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("记账存储过程尚未建立.");
		}
		m_BuildQuery->First();
		m_StoredProc->Close();
		m_StoredProc->ProcedureName =	m_BuildQuery->FieldByName("SyMCMasterCheckOutProc")->AsString;
		m_StoredProc->Parameters->Clear();
		while (!m_BuildQuery->Eof)
		{
			datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
			if (m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger == 1)
				dir = pdInput;
			else if (m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger == 2)
				dir = pdOutput;
			else
				dir = pdInputOutput;

			if (FAccDbType == 1)
				FParamName = m_BuildQuery->FieldByName("SyMCMCOParamName")->AsString;
			else
				FParamName = m_BuildQuery->FieldByName("SyMCMCOParamOrcName")->AsString;

			FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCOParamSize")->AsInteger, NULL);
			FParam->DataType = datatype;
			FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString];
			m_BuildQuery->Next();
		}

		m_Connection->BeginTrans();
		m_StoredProc->ExecProc();
		m_MasterDataSet->Edit();
		m_BuildQuery->First();
		while (!m_BuildQuery->Eof)
		{
			if(m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger != 1)
			{
				if (m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString)->ReadOnly)
					m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString)->ReadOnly = false;

				if (FAccDbType == 1) // mssql
						FParamName = m_BuildQuery->FieldByName("SyMCMCOParamName")->AsString;
				else // oracle
						FParamName = m_BuildQuery->FieldByName("SyMCMCOParamOrcName")->AsString;

				m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
			}
			m_BuildQuery->Next();
		}
		m_MasterDataSet->Post();
//		m_MasterDataSet->MergeChangeLog();
		sxs->LoadMasterFromDataSet(m_MasterDataSet);
		m_MasterDataSet->Close();
		m_BuildQuery->Close();
		m_Connection->CommitTrans();
		m_StoredProc->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream=new TMemoryStream();
		sxs->SaveToStream(resultStream);
		delete sxs;
		resultStream->Position=0;
		return resultStream;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		if (sxs) delete sxs;
		return SendFailMessage(asDSError, "记账数据错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::ExecSQL(TStream *Stream)
{
	TMsgStream *sxs;
	int affectedrows;
	TExecuteOptions eos;
	String sqlstr;
	bool retval;
	TMsgStream  *MsgStream = new TMsgStream(Stream);
	try
	{
		if(MsgStream->ValueAttribute["sqlstr"].Trim().IsEmpty())
			throw Exception( "执行语句尚未赋值.");

		sqlstr = MsgStream->ValueAttribute["sqlstr"];
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->SQL->Text=sqlstr;
		affectedrows=m_Query->ExecSQL();
		WriteUserLog(12, sqlstr);
		sxs = new TMsgStream();
		sxs->Operate = asDSSQL;
		sxs->ValueAttribute["status"] = "s";
		sxs->ValueAttribute["rowsaffected"] = affectedrows;
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "执行SQL语句错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TStream*  TServerDataSet::QueryOpen(String SQLText)
{
	TMsgStream *sxs=NULL;
	String sendbuff;
	bool retval;
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception("查询语句尚未赋值.");

		sxs = new TMsgStream();
		sxs->Operate = asQueryOpen;
		m_Query->Close();
		m_Query->SQL->Text = SQLText;
		m_Query->Open();
		String MDataXMLStr=DataSetToXMLData(m_Query);
		sxs->LoadMasterFromString(MDataXMLStr);
		m_Query->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		if(sxs)	delete sxs;
		return SendFailMessage(asQError, "打开数据集错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::QueryExecSQL(String SQLText)
{
	TMsgStream *sxs;
	String FQueryID;
	int affectedrows;
	TExecuteOptions eos;
	String sqlstr;
	bool retval;
	try
	{
		if (SQLText.Trim().IsEmpty())
			throw Exception( "执行语句尚未赋值.");

		sqlstr = SQLText;
		m_Query->Close();
		m_Query->SQL->Clear();
		m_Query->SQL->Text=sqlstr;
		affectedrows=m_Query->ExecSQL();
		WriteUserLog(12, sqlstr);
		sxs = new TMsgStream();
		sxs->Operate = asQuerySQL;
		sxs->ValueAttribute["status"] = "s";
		sxs->ValueAttribute["rowsaffected"] = affectedrows;
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "执行SQL语句错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
	// 执行系统库
//---------------------------------------------------------------------------
TStream*  TServerDataSet::QuerySysOpen(String SQLText)
{
	TMsgStream *sxs=NULL;
	String FQueryID;
	String sendbuff;
	bool retval;
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception( "查询语句尚未赋值.");

		sxs = new TMsgStream();
		sxs->Operate = asQuerySysOpen;
		b_Query->Close();
		b_Query->SQL->Text = SQLText;
		b_Query->Open();
		String MDataXMLStr=DataSetToXMLData(b_Query);
		sxs->LoadMasterFromString(MDataXMLStr);
		b_Query->Close();
		sxs->ValueAttribute["status"] = "s";
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "打开数据集错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::QuerySysExecSQL(String SQLText)
{
	TMsgStream *sxs;
	int affectedrows;
	TExecuteOptions eos;
	String sqlstr;
	bool retval;
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception("执行语句尚未赋值.");

		sqlstr = SQLText;
		b_Query->Close();
		b_Query->SQL->Text = sqlstr;
		affectedrows = b_Query->ExecSQL();
		WriteUserLog(12, sqlstr);
		sxs = new TMsgStream();
		sxs->Operate = asQuerySysSQL;
		sxs->ValueAttribute["status"] = "s";
		sxs->ValueAttribute["rowsaffected"] = affectedrows;
		TMemoryStream * resultStream = new TMemoryStream();
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "执行SQL语句错误，" + e.Message);
	}
}
//---------------------------------------------------------------------------
TStream*  TServerDataSet::SendErrorMessage(int errortype, int errorcode, String errormessage)
{
	TMemoryStream * resultStream=new TMemoryStream();
	TMsgStream *sxs = new TMsgStream();
	try
	{
		sxs->Operate = asDSError;
		sxs->ValueAttribute["errortype"] = errortype;
		sxs->ValueAttribute["errorcode"] = errorcode;
		sxs->ItemAttribute["errormessage"] = errormessage;
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
	}
	__finally
	{
		delete sxs;
	}
	return resultStream;
}
// ---------------------------------------------------------------------------
TStream*  TServerDataSet::SendFailMessage(int operate, String errormessage)
{
	TMemoryStream * resultStream=new TMemoryStream();
	TMsgStream *sxs = new TMsgStream();
	try
	{
		sxs->Operate = operate;
		sxs->ValueAttribute["status"] = "f";
		sxs->ItemAttribute["errormessage"] = errormessage;
		sxs->SaveToStream(resultStream);
		resultStream->Position=0;
		delete sxs;
		return resultStream;
	}
	catch (Exception &e)
	{
		return SendErrorMessage(100, 100, "发送失败消息错误，\r\n" + e.Message);
	}
}
// ---------------------------------------------------------------------------
void  TServerDataSet::WriteUserLog(int type, String Desc)
{
   DMServerContainer->NotifyWriteLog(FUserCode,FUserIP,FAcckBook,m_ClassCode,type,Desc);
}
// ---------------------------------------------------------------------------
String  TServerDataSet::BuildLinkFields()
{
	String s;
	if(m_DataSetType == 1)	return "";
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		s += m_MasterLinkFields->Strings[i] + "=" + m_DetailLinkFields->Strings[i] + "\t";
	}
	return s;
}
// ---------------------------------------------------------------------------
void  TServerDataSet::ExtractLinkFields(String value)
{
	String s;
	int index;
	if(m_DataSetType == 1)	return;
	index = value.Pos0("=");
	m_MasterLinkFields->Clear();
	m_DetailLinkFields->Clear();
	while (index > -1)
	{
		s = value.SubString0(0, index).Trim();
		m_MasterLinkFields->Add(s);
		value.Delete0(0, index+1);
		index = value.Pos("\t");
		s = value.SubString0(0, index );
		m_DetailLinkFields->Add(s);
		value.Delete0(0, index+1);
		index = value.Pos0("=");
	}
}
// ---------------------------------------------------------------------------
String  TServerDataSet::BuildLinkStr()
{
	String s;
	if(m_DataSetType == 1) return "";
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		s += m_MasterLinkFields->Strings[i] + "=" + m_DetailLinkFields->Strings[i];
		if (i < m_MasterLinkFields->Count - 1)
			s += " and ";
	}
	return s;
}
// ---------------------------------------------------------------------------
void  TServerDataSet::BuildDetailFilter()
{
	String filterstr;
	int ftype;
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		ftype = CheckFieldType(m_DetailDataSet->FieldByName(m_DetailLinkFields->Strings[i]));
		if(ftype == 1)
			filterstr += m_DetailLinkFields->Strings[i] + "='" + m_MasterDataSet->FieldByName(m_MasterLinkFields->Strings[i])->AsString + "'";
		else
			filterstr += m_DetailLinkFields->Strings[i] + "=" + m_MasterDataSet->FieldByName(m_MasterLinkFields->Strings[i])->AsString;

		if(i < m_MasterLinkFields->Count - 1)
			filterstr += " and ";
	}
	m_DetailDataSet->Filtered = false;
	m_DetailDataSet->Filter = filterstr;
	m_DetailDataSet->Filtered = true;
}
// ---------------------------------------------------------------------------
int  TServerDataSet::CheckFieldType(TField *Field)
{
	switch (Field->DataType)
	{
		case ftString:
		case ftDate:
		case ftTime:
		case ftDateTime:
		case ftMemo:
		case ftFixedChar:
		case ftWideString:
			return 1;
		case ftSmallint:
		case ftInteger:
		case ftBoolean:
		case ftBytes:
		case ftWord:
		case ftFloat:
		case ftCurrency:
		case ftBCD:
		case ftAutoInc:
		case ftLargeint:
			return 2;
		default:
			return 0;
	}
}
//---------------------------------------------------------------------------

