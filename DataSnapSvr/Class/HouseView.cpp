//---------------------------------------------------------------------------

#pragma hdrstop

#include "HouseView.h"
#include "DataSnapStream.h"
#include <System.Ioutils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
__fastcall THouseView::THouseView(TComponent* Owner)
: TDMBaseServerClass(Owner)
{
	m_Query = new TADOQuery(NULL);
	m_Query->ParamCheck = false;
	m_Query->EnableBCD = false;
	m_Query->CommandTimeout = 60;
	m_Query->Connection  = AccConnection;

	AppPath = ExtractFilePath(ParamStr(0));
	AppPath=System::Ioutils::TPath::Combine(AppPath, "HouseView");  //����ͼ·��
}
__fastcall THouseView::~THouseView()
{
	delete m_Query;
}
TStream* THouseView::GetHouseViewStream(String AKeyID)
{
  TMemoryStream* dsStream  =new TMemoryStream();
  String SQLStr="SELECT * from tbEstateMedia where EstMdaPKID="+QuotedStr(AKeyID);
  m_Query->Close();
  m_Query->SQL->Text=SQLStr;
  m_Query->Open();
  if(m_Query->RecordCount>0)
  {
		 String mFileName=AppPath;
		 String MedalID=m_Query->FieldByName("EstMdaPKID")->AsString;
		 String EstMdaFKID_Est=m_Query->FieldByName("EstMdaFKID_Est")->AsString;
		 String EstMdaFKID_Bdg=m_Query->FieldByName("EstMdaFKID_Bdg")->AsString;
		 String EstMdaHouseType=m_Query->FieldByName("EstMdaHouseType")->AsString;
		 String EstMdaFKID_Hsg=m_Query->FieldByName("EstMdaFKID_Hsg")->AsString;
		 String EstMdaName=m_Query->FieldByName("EstMdaName")->AsString;
		 String FullEstMdaFileName=m_Query->FieldByName("EstMdaFileName")->AsString;
		 int iSaveType=m_Query->FieldByName("EstMdaSaveType")->AsInteger;
		 if(iSaveType==2)
		 {
		 if(FileExists(FullEstMdaFileName))
			dsStream->LoadFromFile(FullEstMdaFileName);
		 else
		 {
			 if(EstMdaFKID_Est>"")
			 {
			  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Est);
			 }
			 if(EstMdaFKID_Bdg>"")
			 {
			  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Bdg);
			 }
			 if(EstMdaHouseType>"")
			 {
			  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaHouseType);
			 }
			 if(EstMdaFKID_Hsg>"")
			 {
			  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Hsg);
			 }
			 mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaName);
		 if(FileExists(mFileName))
			dsStream->LoadFromFile(mFileName);
		 }
		 }
		 else //���ݿ�
		 {
		  TStream *StreamField=m_Query->CreateBlobStream(m_Query->FieldByName("EstMdaMatter"),bmRead);
		  dsStream->CopyFrom(StreamField,StreamField->Size);
		 }
  }
  m_Query->Close();
  dsStream->Position=0;
  return dsStream;
}
bool THouseView::UpdateHouseViewFile(String AKeyID,String AFileName,TStream* AStream)
{
  TDataSnapStream *dsStream=new TDataSnapStream(AStream);
  bool bUpdate=false;
  try
  {
  String SQLStr="SELECT * from tbEstateMedia where EstMdaPKID="+QuotedStr(AKeyID);
  m_Query->Close();
  m_Query->SQL->Text=SQLStr;
  m_Query->Open();
  if(m_Query->RecordCount>0)
  {
	 String mFileName=AppPath;
	 String MedalID=m_Query->FieldByName("EstMdaPKID")->AsString;
	 String EstMdaFKID_Est=m_Query->FieldByName("EstMdaFKID_Est")->AsString;
	 String EstMdaFKID_Bdg=m_Query->FieldByName("EstMdaFKID_Bdg")->AsString;
	 String EstMdaHouseType=m_Query->FieldByName("EstMdaHouseType")->AsString;
	 String EstMdaFKID_Hsg=m_Query->FieldByName("EstMdaFKID_Hsg")->AsString;
	 String EstMdaName=m_Query->FieldByName("EstMdaName")->AsString;
	 String FullEstMdaFileName=m_Query->FieldByName("EstMdaFileName")->AsString;
	 int iSaveType=m_Query->FieldByName("EstMdaSaveType")->AsInteger;
	 if(iSaveType==2)
	 {
		 if(EstMdaFKID_Est>"")
		 {
		  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Est);
		 }
		 if(EstMdaFKID_Bdg>"")
		 {
		  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Bdg);
		 }
		 if(EstMdaHouseType>"")
		 {
		  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaHouseType);
		 }
		 if(EstMdaFKID_Hsg>"")
		 {
		  mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaFKID_Hsg);
		 }
		 ForceDirectories(mFileName);
		 mFileName=System::Ioutils::TPath::Combine(mFileName,EstMdaName);
		 dsStream->SaveToFile(mFileName);
	 }
	  m_Query->Edit();
	  m_Query->FieldValues["EstMdaName"]=AFileName;
	  if(iSaveType==1)
	  {
		  TStream *StreamField=m_Query->CreateBlobStream(m_Query->FieldByName("EstMdaMatter"),bmReadWrite);
		  StreamField->CopyFrom(dsStream,dsStream->Size);
	  }
	  else
		m_Query->FieldValues["EstMdaFileName"]=mFileName;
	  m_Query->Post();
	  bUpdate=true;
  }
   m_Query->Close();
  }
  __finally
  {
	  delete dsStream;
  }
  return bUpdate;
}
