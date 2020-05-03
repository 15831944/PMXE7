#ifndef PubEnumH
#define PubEnumH

enum TZComboBoxItemIndex
{
  cbItem1,
  cbItem2,
  cbItem3,
  cbItem4,
  cbItem5,
  cbItem6,
  cbItem7
};

enum TZCharType
{
  ctNormal,
  ctCharacter, //����������������ַ�
  ctLetter,   //������ĸ
  ctPlus,     //������
  ctNegative,  //������
  ctInteger,  //����
  ctPlusFloat,      //��������  �� 0
  ctNegativeFloat,  //��������  �� 0
  ctFloat,          //������
  ctID,             //��һ����ĸ���Ժ���ĸ,���֣�"-"��
  ctSelfDefine   //�Զ���
};

enum TZCharCase
{
  ccNormal,
  ccUpperCase,
  ccLowerCase
};
enum TZCalculateType
{
 ctNone,
 ctSum,
 ctAvg,
 ctCount
};
#endif
