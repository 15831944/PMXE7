//---------------------------------------------------------------------------

#ifndef CommFuncH
#define CommFuncH
#include "ZClientQuery.h"
//---------------------------------------------------------------------------
enum TfstFloatType: unsigned char{ftQty=1,ftPrice=2,ftAmt=3,ftRate=4,ftsArea=5,ftsPrice=6,ftsAmt=7,ftsCJPrice=8,ftsCJTotalPrice=9}; //�����������ͣ����������ۡ�������
enum TBusinessType: unsigned char{BtProject=1, BtContract=2, Btdocument=3, BtContractMenu=4,
				   BtdocumentMenu=5, BtInviteProject=6, BtBalance=7, BtSign=8};
//--------------------------------------------------------------------------
// ����˵�� ��ȡ�û����ҵ�������Ȩ��
// ����˵��
// Query Queryָ��
// businessCode ҵ��GUID����,
// businessType  1-��Ŀ,2-��ͬ,3-�ĵ�,4-��ͬĿ¼,5-�ĵ�Ŀ¼,6-�б���Ŀ,7-������Ŀ
// userGUID �û�����
// return value 1 �����ֻ��  2 дȨ��
//--------------------------------------------------------------------------
int __fastcall GetDataRight(TZClientQuery *Query,const String &businessCode,
														TBusinessType businessType,String userGUID = "");
//--------------------------------------------------------------------------
// ����˵�� ��ȡ�û����ҵ�������Ȩ�޹����ַ���
// ����˵��
// Query Queryָ��
// businessType  1-��Ŀ,2-��ͬ,3-�ĵ�,4-��ͬĿ¼,5-�ĵ�Ŀ¼,6-�б���Ŀ,7-������Ŀ
// userGUID �û�����
// return value ��filter string
//--------------------------------------------------------------------------
String __fastcall GetDataRightString(TZClientQuery *Query,TBusinessType businessType,String userGUID = "");
String __fastcall GetGlobeVarValue(String VarName);   //��ȡȫ�ֱ���
String __fastcall GetGUIDString();
String __fastcall myFloatToStr(double data);
//�Զ���ĸ���ת��Ϊ�ַ��������������ѧ������
double __fastcall ConvertFloat(double data,TfstFloatType iType=ftQty,int iPrecision=-1,int iStyle=-1);
//��ʽ���������ݣ�����data-��������iType-�������ͣ�ftQty:������ftPrice:���ۣ�ftAmt:���,ftRate:����ϵ����
//iStyle-С����ȡ��ʽ��0:�������룬1:�ضϣ�-1:ȡϵͳ��ʽ��iPrecision-С�����ȣ�-1:ȡϵͳ����
String __fastcall FormatFloatToStr(double data,TfstFloatType iType=ftQty,int iPrecision=-1,int iStyle=-1);
//��ʽ�������������ַ������������data-��������iType-�������ͣ�ftQty:������ftPrice:���ۣ�ftAmt:���,ftRate:����ϵ����
//iStyle-С����ȡ��ʽ��0:�������룬1:�ضϣ�-1:ȡϵͳ��ʽ��iPrecision-С�����ȣ�-1:ȡϵͳ����
double __fastcall GetFloatFromStr(String str); //���ַ����ж�ȡ������
#endif
