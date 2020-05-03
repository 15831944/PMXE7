//---------------------------------------------------------------------------

#pragma hdrstop

#include "SetAllFieldCanModify.h"
#include <xml.xmldoc.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
void __fastcall SetAllFieldsCanModify(TClientDataSet *AClientDataSet)
{
 //���ж��Ƿ���ֻ���ֶΣ����û��ֱ�ӷ���ʡ�����ʱ��Ϳռ�Ч��
 bool HasReadOnlyField=false;
 if(AClientDataSet==NULL) return;
 for(int i=0;i<AClientDataSet->FieldCount;i++)
 {
	  TField *cf=AClientDataSet->Fields->Fields[i];
	  if(cf->ReadOnly)
	  {
		HasReadOnlyField=true;
		break;
      }
 }
 if(!HasReadOnlyField) return;
 //���������������д���;
 _di_IXMLDocument DataXMLDocument=NewXMLDocument();
 try{
	 DataXMLDocument->LoadFromXML(AClientDataSet->XMLData);
//	 DataXMLDocument->Active=true;
	 IXMLNode *DATAPACKETNode=DataXMLDocument->ChildNodes->FindNode("DATAPACKET");
	 IXMLNode *METADATANode=NULL;
	 IXMLNode *FIELDSNode=NULL;
	 IXMLNodeList  *FieldChildList=NULL;
	 IXMLNode *FieldNode=NULL;
	 if(DATAPACKETNode!=NULL)
			 METADATANode= DATAPACKETNode->ChildNodes->FindNode("METADATA");
	 if(METADATANode!=NULL)
		 FIELDSNode =METADATANode->ChildNodes->FindNode("FIELDS");
	 if(FIELDSNode!=NULL)
		FieldChildList=FIELDSNode->ChildNodes;
	  if(FieldChildList!=NULL)
	  {
		  int fieldCount=FieldChildList->Count;
		  for(int j=0;j<fieldCount;j++)
		  {
			  FieldNode=FieldChildList->Nodes[j];  //����ֻ�����ԣ���ɵ��������
			  if(FieldNode->HasAttribute("readonly"))
				 FieldNode->AttributeNodes->Delete("readonly");
		  }
		 AClientDataSet->XMLData=DataXMLDocument->XML->Text;
	  }
 }
 __finally
 {
   DataXMLDocument->Active=false;
   DataXMLDocument=NULL;
 }

}