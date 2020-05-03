// ---------------------------------------------------------------------------

//#include <fmx.h>
#pragma hdrstop

#include "MsgStream.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TMsgStream::TMsgStream()
{
	CreateClientDataSet();
}

// ---------------------------------------------------------------------------
__fastcall TMsgStream::TMsgStream(String XMLstr)
{
	// ��������˳��
	CreateClientDataSet();
	MsgClientDataSet->XMLData = XMLstr;
	DecClientMsg();
}

// ---------------------------------------------------------------------------
__fastcall TMsgStream::TMsgStream(TStream *xfs)
{
	// ��������˳��
	CreateClientDataSet();
	LoadFromStream(xfs);
}

// ---------------------------------------------------------------------------
__fastcall TMsgStream::~TMsgStream()
{
	if (FOptValue)
		delete FOptValue;
	if (FOptItem)
		delete FOptItem;
	if (FMasterDataSet)
		delete FMasterDataSet;
	if (FDetailDataSet)
		delete FDetailDataSet;
	if (MsgClientDataSet)
		delete MsgClientDataSet;
}
// ---------------------------------------------------------------------------
int __fastcall TMsgStream::get_XmlOperate()
{
	return FOperate;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlClassID()
{
	return FClassID;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlClientID()
{
	return FClientID;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlServerID()
{
	return FServerID;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlOptValue(String Attribute)
{
	String RetVal = "";
	TLocateOptions Opts;
	Opts.Clear();
	Opts << loCaseInsensitive;
	Variant locvalue;
	locvalue = Variant(Attribute);
	if (FOptValue->Locate("Name", locvalue, Opts))
		RetVal = FOptValue->FieldByName("Value")->AsString;
	return RetVal;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlOptItem(String Attribute)
{
	String RetVal = "";
	TLocateOptions Opts;
	Opts.Clear();
	Opts << loCaseInsensitive;
	Variant locvalue;
	locvalue = Variant(Attribute);
	if (FOptItem->Locate("Name", locvalue, Opts))
		RetVal = FOptItem->FieldByName("Value")->AsString;
	return RetVal;
}
// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlString()
{
	BuildClientMsg();
	return MsgClientDataSet->XMLData;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlOperate(int value)
{
	FOperate = value;
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlClassID(String value)
{
	FClassID = value;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlClientID(String value)
{
	FClientID = value;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlServerID(String value)
{
	FServerID = value;
}

// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_XmlQueryID()
{
	return FQueryID;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlQueryID(String value)
{
	FQueryID = value;
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlOptValue(String Attribute, String value)
{
	TLocateOptions Opts;
	Opts.Clear();
	Opts << loCaseInsensitive;
	Variant locvalue;
	locvalue = Variant(Attribute);
	if (FOptValue->Locate("Name", locvalue, Opts))
		FOptValue->Edit();
	else
	{
		FOptValue->Append();
		FOptValue->FieldValues["Name"] = Attribute;
	}
	FOptValue->FieldValues["Value"] = value;
	FOptValue->Post();
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlOptItem(String Attribute, String value)
{
	TLocateOptions Opts;
	Opts.Clear();
	Opts << loCaseInsensitive;
	Variant locvalue;
	locvalue = Variant(Attribute);
	if (FOptItem->Locate("Name", locvalue, Opts))
		FOptItem->Edit();
	else
	{
		FOptItem->Append();
		FOptItem->FieldValues["Name"] = Attribute;
	}
	FOptItem->FieldValues["Value"] = value;
	FOptItem->Post();
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_MasterDataSet(TStream* value)
{
	FMasterDataSet->LoadFromStream(value);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_DetailDataSet(TStream* value)
{
	FDetailDataSet->LoadFromStream(value);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_XmlString(String value)
{
	MsgClientDataSet->XMLData = value;
	DecClientMsg();
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::DecClientMsg()
{
	if (MsgClientDataSet->RecordCount == 0) return;
	TMemoryStream *ms = new TMemoryStream();
	try
	{
		FOperate = MsgClientDataSet->FieldByName("Operate")->AsInteger;
		FClassID = MsgClientDataSet->FieldByName("ClassID")->AsString;
		FClientID = MsgClientDataSet->FieldByName("ClientID")->AsString;
		TField * QIDField = MsgClientDataSet->FindField("QueryID");
		if (QIDField != NULL) // zws20101104���ݵװ汾��
				FQueryID = QIDField->AsString;
		else
			FQueryID = 0;
		// FQueryID=MsgClientDataSet->FieldByName("QueryID")->AsString;
		FServerID = MsgClientDataSet->FieldByName("ServerID")->AsString;
		FMasterLinkData = MsgClientDataSet->FieldByName("MasterLinkData")
			->AsString;
		FDetailLinkData = MsgClientDataSet->FieldByName("DetailLinkData")
			->AsString;

		TBlobField *bOptItemField = (TBlobField*)MsgClientDataSet->FieldByName
			("OptItem");
		ms->Clear();
		bOptItemField->SaveToStream(ms);
		ms->Position = 0;
		FOptItem->LoadFromStream(ms);

		TBlobField *bOptValueField = (TBlobField*)MsgClientDataSet->FieldByName
			("OptValue");
		ms->Clear();
		bOptValueField->SaveToStream(ms);
		ms->Position = 0;
		FOptValue->LoadFromStream(ms);

		TBlobField *bMasterDataSetField =
			(TBlobField*)MsgClientDataSet->FieldByName("MasterDataSet");
		FMasterDataSet->Size=0;
		/* ms->Clear();
		 bMasterDataSetField->SaveToStream(ms);
		 DecompressStream(ms, FMasterDataSet); // ��ѹ
		 // */ bMasterDataSetField->SaveToStream(FMasterDataSet);
		FMasterDataSet->Position = 0;

		TBlobField *bDetailDataSetField =
			(TBlobField*)MsgClientDataSet->FieldByName("DetailDataSet");
		FDetailDataSet->Size=0;
		/* ms->Clear();
		 bDetailDataSetField->SaveToStream(ms);
		 DecompressStream(ms, FDetailDataSet); // ��ѹ
		 // */ bDetailDataSetField->SaveToStream(FDetailDataSet);
		FDetailDataSet->Position = 0;
	}
	__finally
	{
		delete ms;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::BuildClientMsg()
{
	TMemoryStream *ms = new TMemoryStream();
	try
	{
		if (MsgClientDataSet->RecordCount == 0)
			MsgClientDataSet->Append();
		else
			MsgClientDataSet->Edit();
		MsgClientDataSet->FieldByName("Operate")->Value = FOperate;
		MsgClientDataSet->FieldByName("ClassID")->Value = FClassID;
		MsgClientDataSet->FieldByName("ClientID")->Value = FClientID;
		TField * QIDField = MsgClientDataSet->FindField("QueryID");
		if (QIDField != NULL) // zws20101104���ݵװ汾��
				QIDField->Value = FQueryID;
		// MsgClientDataSet->FieldByName("QueryID")->Value = FQueryID;
		MsgClientDataSet->FieldByName("ServerID")->Value = FServerID;
		MsgClientDataSet->FieldByName("MasterLinkData")->Value =
			FMasterLinkData;
		MsgClientDataSet->FieldByName("DetailLinkData")->Value =
			FDetailLinkData;

		TBlobField *bOptItemField = (TBlobField*)MsgClientDataSet->FieldByName
			("OptItem");
		ms->Clear();
		FOptItem->SaveToStream(ms);
		ms->Position = 0;
		bOptItemField->LoadFromStream(ms);

		TBlobField *bOptValueField = (TBlobField*)MsgClientDataSet->FieldByName
			("OptValue");
		ms->Clear();
		FOptValue->SaveToStream(ms);
		ms->Position = 0;
		bOptValueField->LoadFromStream(ms);

		TBlobField *bMasterDataSetField =
			(TBlobField*)MsgClientDataSet->FieldByName("MasterDataSet");
		FMasterDataSet->Position = 0;
		/* ms->Clear();
		 CompressStream(FMasterDataSet, ms); // ѹ��
		 ms->Position = 0;
		 bMasterDataSetField->LoadFromStream(ms);
		 FMasterDataSet->Position = 0;

		 // */ bMasterDataSetField->LoadFromStream(FMasterDataSet);

		TBlobField *bDetailDataSetField =
			(TBlobField*)MsgClientDataSet->FieldByName("DetailDataSet");
		FDetailDataSet->Position = 0;
		/* ms->Clear();
		 CompressStream(FDetailDataSet, ms); // ѹ��
		 ms->Position = 0;
		 bDetailDataSetField->LoadFromStream(ms);
		 FDetailDataSet->Position = 0;
		 // */ bDetailDataSetField->LoadFromStream(FDetailDataSet);

		MsgClientDataSet->Post();
	}
	__finally
	{
		delete ms;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveToFile(String fn)
{
	BuildClientMsg();
	MsgClientDataSet->SaveToFile(fn, dfBinary);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveToStream(TStream *fs)
{
	BuildClientMsg();
	fs->Position=0;
	MsgClientDataSet->SaveToStream(fs, dfBinary);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadFromFile(String fn)
{
	MsgClientDataSet->LoadFromFile(fn);
	DecClientMsg();
}
// ---------------------------------------------------------------------------
TStream * __fastcall TMsgStream::GetDataSnapStream(TStream *dsStream)
{
	if (dsStream->Size == -1) // ��֪����С
	{
		const int BufSize = 0xF000;
		void *Buffer;
		int ReadCount=0,TotalCount=0;
		TMemoryStream *ms = new TMemoryStream();
		try
		{
			dsStream->Position=0;
			Buffer = AllocMem(NativeInt(BufSize));
			while(true)
			{
				ReadCount = dsStream->Read(Buffer, BufSize);
				TotalCount=TotalCount+ReadCount;
				if (ReadCount > 0)
					ms->WriteBuffer(Buffer, ReadCount);
				if (ReadCount < BufSize)
					break;
			}
			 delete dsStream;
			 dsStream=ms;
		}
		__finally
		{
			FreeMemory(Buffer);
			//delete ms;
		}
	}
	dsStream->Position=0;
	return dsStream;
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadFromStream(TStream *fs)
{
	TStream *dsStream=GetDataSnapStream(fs);
	MsgClientDataSet->LoadFromStream(dsStream);
	DecClientMsg();
}

// ---------------------------------------------------------------------------
/*void __fastcall TMsgStream::LoadFromDataBlock(_di_IDataBlock datablock) {
	TMemoryStream *ms = new TMemoryStream();
	try {
		ms->Size = datablock->Size;
		ms->Write((void*)(int(datablock->Memory) + datablock->BytesReserved),
			datablock->Size);
		ms->Position = 0;
		MsgClientDataSet->LoadFromStream(ms);
		DecClientMsg();
	}
	__finally {
		delete ms;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveToDataBlock(_di_IDataBlock dataBlock) {
	TMemoryStream *ms = new TMemoryStream();
	try {
		BuildClientMsg();
		MsgClientDataSet->SaveToStream(ms);
		ms->Position = 0;
		dataBlock->Clear();
		dataBlock->Write(ms->Memory, ms->Size);
	}
	__finally {
		delete ms;
	}
}
 */
// ---------------------------------------------------------------------------
TStream* __fastcall TMsgStream::get_MasterDataSet()
{
	return FMasterDataSet;
}

// ---------------------------------------------------------------------------
TStream* __fastcall TMsgStream::get_DetailDataSet() {
	return FDetailDataSet;
}

// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_MasterLinkData() {
	return FMasterLinkData;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_MasterLinkData(String value) {
	FMasterLinkData = value;
}

// ---------------------------------------------------------------------------
String __fastcall TMsgStream::get_DetailLinkData() {
	return FDetailLinkData;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::set_DetailLinkData(String value)
{
	FDetailLinkData = value;
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadDetailFromDataSet(TCustomClientDataSet *DataSet)
{
//	TADODataSetStream *ADODSStream=new TADODataSetStream(DataSet);
//	try
//	{
//		LoadDetailFromStream(ADODSStream);
//	}
//	__finally
//	{
//		delete ADODSStream;
//	}
	DataSet->SaveToStream(FDetailDataSet,dfBinary);
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveDetailToDataSet(TCustomClientDataSet *DataSet)
{
//	TADODataSetStream *ADODSStream = new TADODataSetStream();
//	try
//	{
//		ADODSStream->LoadFromStream(FDetailDataSet);
//		ADODSStream->SaveToDataSet(DataSet);
//	}
//	__finally
//	{
//		delete ADODSStream;
//	}
	FDetailDataSet->Position=0;
 	DataSet->LoadFromStream(FDetailDataSet);
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadMasterFromDataSet(TCustomClientDataSet *DataSet)
{
//	TADODataSetStream *ADODSStream = new TADODataSetStream(DataSet);
//	try
//	{
//		LoadMasterFromStream(ADODSStream);
//	}
//	__finally
//	{
//		delete ADODSStream;
//	}
	DataSet->SaveToStream(FMasterDataSet,dfBinary);
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveMasterToDataSet(TCustomClientDataSet *DataSet)
{
//	TADODataSetStream *ADODSStream = new TADODataSetStream();
//	try
//	{
//		ADODSStream->LoadFromStream(FMasterDataSet);
//		ADODSStream->SaveToDataSet(DataSet);
//	}
//	__finally
//	{
//		delete ADODSStream;
//		ADODSStream = NULL;
//	}
   FMasterDataSet->Position=0;
   DataSet->LoadFromStream(FMasterDataSet);
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveMasterToFile(String fn)
{
	if(FileExists(fn)) DeleteFile(fn);
	FMasterDataSet->SaveToFile(fn);
}
// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveMasterToStream(TStream *fs)
{
	fs->Position = 0;
	fs->Size = FMasterDataSet->Size;
	FMasterDataSet->SaveToStream(fs);
	fs->Position = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadMasterFromFile(String fn)
{
	FMasterDataSet->Clear();
	FMasterDataSet->LoadFromFile(fn);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadMasterFromStream(TStream *fs)
{
	FMasterDataSet->Clear();
	TStream *dsStream = GetDataSnapStream(fs);
	FMasterDataSet->LoadFromStream(dsStream);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveDetailToFile(String fn)
{
	if (FileExists(fn))
		DeleteFile(fn);
	FDetailDataSet->SaveToFile(fn);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveDetailToStream(TStream *fs)
{
	fs->Position = 0;
	fs->Size = FDetailDataSet->Size;
	FDetailDataSet->SaveToStream(fs);
	fs->Position = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadDetailFromFile(String fn) {
	FDetailDataSet->Clear();
	FDetailDataSet->LoadFromFile(fn);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadDetailFromStream(TStream *fs) {
	FDetailDataSet->Clear();
	TStream *dsStream=GetDataSnapStream(fs);
	FDetailDataSet->LoadFromStream(dsStream);
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveMasterToString(String &ToStr) {
#ifdef _DELPHI_STRING_UNICODE
	TStringStream *ss = new TStringStream();
	try {
		ss->LoadFromStream(FMasterDataSet);
		ss->Position = 0;
		ToStr = ss->DataString;
	}
	__finally {
		delete ss;
	}
#else
	ToStr.SetLength(FMasterDataSet->Size);
	FMasterDataSet->Position = 0;
	FMasterDataSet->Read(ToStr.c_str(), FMasterDataSet->Size);
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::SaveDetailToString(String &ToStr) {
#ifdef _DELPHI_STRING_UNICODE
	TStringStream *ss = new TStringStream();

	try {
		ss->LoadFromStream(FDetailDataSet);
		ss->Position = 0;
		ToStr = ss->DataString;
	}
	__finally {
		delete ss;
	}
#else
	ToStr.SetLength(FDetailDataSet->Size);
	FDetailDataSet->Position = 0;
	FDetailDataSet->Read(ToStr.c_str(), FDetailDataSet->Size);
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadMasterFromString(String FromStr) {
#ifdef _DELPHI_STRING_UNICODE
	TStringStream *ss = new TStringStream();
	try {
		FMasterDataSet->Clear();
		ss->WriteString(FromStr);
		ss->Position = 0;
		FMasterDataSet->LoadFromStream(ss);
	}
	__finally {
		delete ss;
	}
#else
	FMasterDataSet->Clear();
	int Size = FromStr.Length();
	FMasterDataSet->Position = 0;
	FMasterDataSet->Write(FromStr.c_str(), Size);
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::LoadDetailFromString(String FromStr) {
#ifdef _DELPHI_STRING_UNICODE
	TStringStream *ss = new TStringStream();

	try {
		FDetailDataSet->Clear();
		ss->WriteString(FromStr);
		ss->Position = 0;
		FDetailDataSet->LoadFromStream(ss);
	}
	__finally {
		delete ss;
	}
#else
	FDetailDataSet->Clear();
	int Size = FromStr.Length();
	FDetailDataSet->Position = 0;
	FDetailDataSet->Write(FromStr.c_str(), Size);
#endif
}

// ---------------------------------------------------------------------------
void __fastcall TMsgStream::CreateClientDataSet() {
	FOperate = -1;
	FClassID = 0;
	FClientID = 0;
	FQueryID = 0;
	FServerID = 0;

	FOptValue = new TClientDataSet(NULL);
	TFieldDefs *oDefs = FOptValue->FieldDefs;
	TFieldDef *oDef = oDefs->AddFieldDef();
	oDef->DataType = TFieldType::ftString;
	oDef->Size = 50;
	oDef->Name = "Name";

	oDef = oDefs->AddFieldDef();
	oDef->DataType = TFieldType::ftMemo;
	// oDef->Size = 500;
	oDef->Name = "Value";
	TIndexDef *IoDef = FOptValue->IndexDefs->AddIndexDef();
	IoDef->Fields = "Name";
	IoDef->Name = "NameIndex";
	FOptValue->CreateDataSet();

	FOptItem = new TClientDataSet(NULL);
	TFieldDefs *Items = FOptItem->FieldDefs;
	TFieldDef *Item = Items->AddFieldDef();
	Item->DataType = TFieldType::ftString;
	Item->Size = 50;
	Item->Name = "Name";

	Item = Items->AddFieldDef();
	Item->DataType = TFieldType::ftMemo;
	// Item->Size = 500;
	Item->Name = "Value";

	TIndexDef *pIItem = FOptItem->IndexDefs->AddIndexDef();
	pIItem->Fields = "Name";
	pIItem->Name = "NameIndex";
	FOptItem->CreateDataSet();

	FMasterDataSet = new TMemoryStream();
	FDetailDataSet = new TMemoryStream();
	FMasterLinkData = "";
	FDetailLinkData = "";

	MsgClientDataSet = new TClientDataSet(NULL);
	TFieldDefs *pDefs = MsgClientDataSet->FieldDefs;
	// m_Length
	TFieldDef *pDef = pDefs->AddFieldDef();
	// FOperate
	pDef->DataType = TFieldType::ftInteger;
	pDef->Name = "Operate";

	// FClassID
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 10;
	pDef->Name = "ClassID";

	// FClientID
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 10;
	pDef->Name = "ClientID";
	// FQueryID
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 10;
	pDef->Name = "QueryID";

	// FServerID
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 10;
	pDef->Name = "ServerID";

	// FMasterLinkData
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 100;
	pDef->Name = "MasterLinkData";

	// FDetailLinkData
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftString;
	pDef->Size = 100;
	pDef->Name = "DetailLinkData";

	// FMasterDataSet
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftBlob;
	pDef->Name = "MasterDataSet";

	// FDetailDataSet
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftBlob;
	pDef->Name = "DetailDataSet";
	// FOptValue
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftBlob;
	pDef->Name = "OptValue";
	// FOptItem
	pDef = pDefs->AddFieldDef();
	pDef->DataType = TFieldType::ftBlob;
	pDef->Name = "OptItem";

	TIndexDef *pIDef = MsgClientDataSet->IndexDefs->AddIndexDef();
	pIDef->Fields = "Operate";
	pIDef->Name = "IntIndex";
	MsgClientDataSet->CreateDataSet();

}
