//---------------------------------------------------------------------------

#pragma hdrstop

#include "ContractProject.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall TContractProject::TContractProject(TComponent* Owner)
	:TDMBaseServerClass(Owner)
{
	AccQuery = new TADOQuery(NULL);
	AccQuery->ParamCheck = false;
	AccQuery->EnableBCD  = false;
	AccQuery->CommandTimeout = 60;
	AccQuery->Connection = AccConnection;
}
//---------------------------------------------------------------------------
__fastcall TContractProject::~TContractProject()
{
	delete AccQuery;
}
//---------------------------------------------------------------------------
TJSONArray*  TContractProject::GetContype()
{
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;

	AccQuery->Close();
	AccQuery->SQL->Text = " select a.CTypePKID,a.CTypeName,a.CTypePreNumFKID,b.CTypeName as PTypename from tbContType a left join  tbContType b on a.CTypePreNumFKID=b.CTypePKID  order by a.CTypeSortCode";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
TJSONArray*  TContractProject::GetContlist(String userid,String ctypeid)
{
	if(userid==NULL||ctypeid==NULL) return NULL;
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;
	String usernum="";
	AccQuery->Close();
	AccQuery->SQL->Text = "select EmpNum from tbEmployeeData where EmpPKID ='"+userid+"'";
	AccQuery->Open();
	if(AccQuery->RecordCount>0)
		usernum = AccQuery->FieldByName("EmpNum")->AsString;
	AccQuery->Close();
	AccQuery->SQL->Text = " select ContPKID, ContName,(select CorpName from tbCorporation where CorpPKID = ContFistPty_FKID)as ContFistPtyName, "
						 " (select CorpName from tbCorporation where CorpPKID = ContSecondPty_FKID)as ContSecondPtyName, "
						 " ContCondition,ContSignsDate from tbContract where "
						 " (ContPKID in (select distinct(SysDRDataID) from vi_GetDataRight "
						 " where  SysDROrigin =2 and SysDROpteType>=1 and "
						 " REEmpPK_FKID='"+usernum+"') or ContCreator ='"+usernum+"') and ContType='"+ctypeid+"'";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
TJSONArray*  TContractProject::GetContdata(String contid)
{
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;

	AccQuery->Close();
	AccQuery->SQL->Text = " SELECT tbContract.*, "
						  " SuportNameA=isnull((select CorpName from tbCorporation where ContFistPty_FKID=CorpPKID),''),"
						  " SuportNameB=isnull((select CorpName from tbCorporation where ContSecondPty_FKID=CorpPKID),''),"
						  " SuportNameC=isnull((select CorpName from tbCorporation where ContThirdPtyFKID=CorpPKID),''),"
						  " SuportNameD=isnull((select CorpName from tbCorporation where ContUnitFKID=CorpPKID),''),"
						  " Department=isnull((select CorpOgnDeptName from tbCorporationOrganize where ContDeptFKID=CorpOgnPKID),''),"
						  " Creator=isnull((select EmpName from tbEmployeeData where EmpNum=ContCreator),''),"
						  " ContractType=CTypeName,UpperName=isnull((select ContName "
						  " from tbContract b where b.ContPKID=tbContract.ContPreContNumFKID),''),"
						  " CurrencyName=CurDataName,LocalCurrency=CurDataIsStandard ,Exchange =CurDataExchangeRate"
						  " FROM tbContract,tbContType,tbCurrencyData"
						  " where ContPKID ='"+contid+"' and ContType=CTypePKID and ContCurFKID=CurDataPKID order by ContNumber";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
TJSONArray*  TContractProject::GetProtype()
{
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;

	AccQuery->Close();
	AccQuery->SQL->Text = " select a.PrjTypePKID,a.PrjTypeName,a.PrjTypeUpper,b.PrjTypeName as PTypename from tbPrjType a left join  tbPrjType b on a.PrjTypeUpper=b.PrjTypePKID  order by a.PrjTypeSort";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
TJSONArray*  TContractProject::GetProlist(String userid,String ProTypeid)
{
	if(userid==NULL||ProTypeid==NULL) return NULL;
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;
	String usernum="";
	AccQuery->Close();
	AccQuery->SQL->Text = "select EmpNum from tbEmployeeData where EmpPKID ='"+userid+"'";
	AccQuery->Open();
	if(AccQuery->RecordCount>0)
	   usernum = AccQuery->FieldByName("EmpNum")->AsString;
	AccQuery->Close();
	AccQuery->SQL->Text = " select PrjPKID,PrjPrjName, PrjState,PrjPlanway,PrjPlanStartDate, "
	                      " PrjPlanEndDate,PrjFactStartDate,PrjFactEndDate,PrjFactTime,"
						  " (select CorpOgnDeptName from tbCorporationOrganize where CorpOgnPKID = PrjFKID_ResponOrg)as PrjFKID_ResponOrg"
						  " from tbProject where "
						  " (PrjPKID in (select distinct(SysDRDataID) from vi_GetDataRight "
						  " where  SysDROrigin =1 and SysDROpteType>=1 and "
						  " REEmpPK_FKID='"+usernum+"') or PrjCreator ='"+usernum+"') and PrjFKID_PrjType='"+ProTypeid+"'";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
TJSONArray*  TContractProject::GetProdata(String Projectid)
{
  TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;

	AccQuery->Close();
	AccQuery->SQL->Text = " Select PrjTskWBS,PrjTskOrder,PrjTskPKFKID_TskID,PrjTskName,PrjTskUpper from tbPrjTsk "
						  "	where PrjTskPKFKID_PrjID ='"+Projectid+"' and "
						  " PrjTskType>0 order by PrjTskPKFKID_PrjID, PrjTskNumber ";
	AccQuery->Open();
	while(!AccQuery->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<AccQuery->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(AccQuery->Fields->Fields[i]->FieldName,AccQuery->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		AccQuery->Next();
	}
	return retArray;
}
//----------------------------------------------------------------------------
