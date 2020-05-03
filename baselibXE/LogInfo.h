#ifndef LogInfoH
#define LogInfoH
#include <string.h>
//---------------------------------------------------------------------------
typedef struct LogInfo
{
  bool IsSystemer; //ϵͳ����Ա
  bool IsRemoter;  //�Ƿ��Ǵ�Զ������	
  int  AgentPort; //�˿�
  Char AccBookID[10];//��¼���׺�
  Char UserCode[20]; //�û����
  Char UserEmpID[40];//Ա��id
  Char LogonCode[20]; //��¼�û���,���ܰ�����ǰ�汾δʹ��
  Char UserName[20]; //��¼�û���
  Char AccBookName[20];//��¼��������
  Char Version[20];//�汾
  Char AgentAddress[30];//�����ַ
  Char SoftName[40];//�������
  Char Email[100];//�����ʼ�
  Char HomePage[255];//��ҳ
  //ftp
  int  ftpPort;  //ftp�˿�
  bool ftpNeedFTP; //�Ƿ�����ftp
  bool ftpPassive; //�Ƿ񱻶�
  Char ftpIP[64]; //�ⲿ/�ڲ���������ַ ,�Ѿ����ݿͻ����������ȷ��
  Char ftpUserName[20];  //��¼�û�
  Char ftpPassword[20];  //��¼����
  //QQ
  Char QQID[20];       //QQID��
  Char QQPassword[20];   //QQ��½����
  //mail
  Char mailServer[255];  //�ʼ�������
  Char sendEMail[100];  //��������
  Char mailUserName[20]; //��¼�û���
  Char mailPassword[20]; //��¼����
  int  mailPort;        //���Ӷ˿�
  Char UpdateURL[255];
}TLogInfo;
#endif
