
//---------------------------------------------------------------------------

#ifndef InfoH
#define InfoH
#include <ADODB.hpp>
#include <ExtCtrls.hpp>
#include <Classes.hpp>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure * Tfunc)();
typedef void __fastcall (__closure * TfunWritError)( String);
class TShortInfo
{
    private:	// User declarations
       String FCordId,FUserName,FPassword;
       int FFlag;
       int FWaitLong; // ��Ϣ�������ʱ�� ����Ϊ��λ Ĭ�� 1000��
       TTimer *MainTimer;
       String FErrorStr;  //������Ϣ
       TTime FEndTime,FBeginTime;  //��ʼ����ʱ��
//       int AConnCount ;//��Ч������
       TStringList *SendList; //����ID������
       TStringList *IndexList;//��Ϣ���
	   TADOConnection *AccConn; // ��Ч��������
       int FRepeatCount;           //���ʹ����ʱ���ظ�����
       HINSTANCE DllHandle;
	   String FDBConnectString;
	   String FAccConnectString;
	   int FComPort;
	   int FModeType;
		//�õ���̬��
       //����è����
/*       long (* fnInitModem )(long);//��ʼ���˿�
       long (* fnGetStatus)(long);//ȡ�˿�״̬
       long (* fnGetSndCount)(long);//ȡ���ͻ�������¼��
       long (* fnGetRecCount)(long);//ȡ���ջ�������¼��
       long (* fnGetPortMax)();//�õ����˿ں�
       long (* fnSendMsg)(long , char * , char *); //������Ϣ
	   long (* fnReadMsgEx)(long ,char * ,char *);//������Ϣ
	   */
	   //���غ���

       long (WINAPI *Sms_Proxy)(long lProxyType,const char * pProxyHost,long lProxyPort,const char * pProxyUser,const char * pProxyPasswd);
       long (WINAPI *Sms_Connect)(const char * pServer , long lCorpID,const char * pLoginName, const char * pPasswd , long lTimeOut, HWND hWnd);
       long (WINAPI *Sms_Send2)( const char * pSendExNum,const char * pMobile , const char * pMsg , long * lSmsID);
       long (WINAPI *Sms_Get2)(char * pSendNum , char * pRecvNum , char * pMsg , char * pTime);
       long (WINAPI *Sms_DisConnect)(void);

       bool __fastcall GetDllHandle();//�õ���̬��
       bool __fastcall LoadDllFunc();//ת�ض�̬����
       String __fastcall ErorType(int Flag);// ��������
	   void __fastcall SendShortMessage();//������Ϣ
	   int __fastcall SendData(String Heard,String Detail);
       void __fastcall ReadShortMessage();//������Ϣ
       void __fastcall WriteErrorLog();//д������־
	   void __fastcall OnTimer(TObject *Sender);
       String __fastcall ResultStr(String Source,int flag);//�����ַ���
	public:		// User declarations
		void __fastcall Initializtion();//��ʼ��ϵͳ
		String __property ErrorStr={read=FErrorStr};//�õ�������Ϣ
        TfunWritError OnWriteErrorLog;//д������־����,
        TTime __property BeginTime = {write = FBeginTime};//��ʼʱ��
        TTime __property EndTime = {write=FEndTime}; //����ʱ��
        int   __property WaitLong = {write = FWaitLong};//��Ϣ�������ʱ�� ����Ϊ��λ Ĭ�� 1000��
        int   __property RepeatCount = {write = FRepeatCount};//���ʹ���ʱ����Ҫ�ظ�����
        String __property CordId = {write = FCordId};
        String __property UserName = {write = FUserName};
        String __property PassWord = {write = FPassword};
        void __fastcall StarTimer(); //�����̹߳���
		__fastcall TShortInfo(int Flag=0);//Flag =0 ����è��1 ����
        __fastcall ~TShortInfo();
		__property String DBConnectString  = { read=FDBConnectString, write=FDBConnectString };
		__property String AccConnectString  = { read=FAccConnectString, write=FAccConnectString };
        __property int ComPort  = { read=FComPort, write=FComPort };
		__property int ModeType  = { read=FModeType, write=FModeType };
		String __fastcall getStatus();
}
;
// �����߳�
class TSendThread : public TThread
{
private:

protected:
        void __fastcall Execute();
public:
        __fastcall TSendThread(bool CreateSuspended);
        Tfunc SendFun;
};
// �����߳�
class TRecvThread : public TThread
{
private:

protected:
        void __fastcall Execute();
public:
        __fastcall TRecvThread(bool CreateSuspended);
        Tfunc Recvfun;
};

// ����ʱ����߳�
class TEndTimeThread : public TThread
{
private:
     TTime FEndTime,FBeginTime;
protected:
        void __fastcall Execute();
public:
        __fastcall TEndTimeThread(bool CreateSuspended);
        TTime __property EndTime = {write=FEndTime};
        TTime __property BeginTime = {write = FBeginTime};
};
#endif
