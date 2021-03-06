
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
       int FWaitLong; // 消息处理间隔时间 毫秒为单位 默认 1000；
       TTimer *MainTimer;
       String FErrorStr;  //错误信息
       TTime FEndTime,FBeginTime;  //开始结束时间
//       int AConnCount ;//有效连接数
       TStringList *SendList; //保存ID和连接
       TStringList *IndexList;//信息编号
	   TADOConnection *AccConn; // 有效连接数组
       int FRepeatCount;           //发送错误的时候，重复次数
       HINSTANCE DllHandle;
	   String FDBConnectString;
	   String FAccConnectString;
	   int FComPort;
	   int FModeType;
		//得到动态库
       //短信猫函数
/*       long (* fnInitModem )(long);//初始化端口
       long (* fnGetStatus)(long);//取端口状态
       long (* fnGetSndCount)(long);//取发送缓冲区记录数
       long (* fnGetRecCount)(long);//取接收缓冲区记录数
       long (* fnGetPortMax)();//得到最大端口号
       long (* fnSendMsg)(long , char * , char *); //发送信息
	   long (* fnReadMsgEx)(long ,char * ,char *);//接收信息
	   */
	   //网关函数

       long (WINAPI *Sms_Proxy)(long lProxyType,const char * pProxyHost,long lProxyPort,const char * pProxyUser,const char * pProxyPasswd);
       long (WINAPI *Sms_Connect)(const char * pServer , long lCorpID,const char * pLoginName, const char * pPasswd , long lTimeOut, HWND hWnd);
       long (WINAPI *Sms_Send2)( const char * pSendExNum,const char * pMobile , const char * pMsg , long * lSmsID);
       long (WINAPI *Sms_Get2)(char * pSendNum , char * pRecvNum , char * pMsg , char * pTime);
       long (WINAPI *Sms_DisConnect)(void);

       bool __fastcall GetDllHandle();//得到动态库
       bool __fastcall LoadDllFunc();//转载动态函数
       String __fastcall ErorType(int Flag);// 错误类型
	   void __fastcall SendShortMessage();//发送信息
	   int __fastcall SendData(String Heard,String Detail);
       void __fastcall ReadShortMessage();//接收信息
       void __fastcall WriteErrorLog();//写错误日志
	   void __fastcall OnTimer(TObject *Sender);
       String __fastcall ResultStr(String Source,int flag);//分类字符串
	public:		// User declarations
		void __fastcall Initializtion();//初始化系统
		String __property ErrorStr={read=FErrorStr};//得到错误信息
        TfunWritError OnWriteErrorLog;//写错误日志函数,
        TTime __property BeginTime = {write = FBeginTime};//开始时间
        TTime __property EndTime = {write=FEndTime}; //结束时间
        int   __property WaitLong = {write = FWaitLong};//消息处理间隔时间 毫秒为单位 默认 1000；
        int   __property RepeatCount = {write = FRepeatCount};//发送错误时，需要重复次数
        String __property CordId = {write = FCordId};
        String __property UserName = {write = FUserName};
        String __property PassWord = {write = FPassword};
        void __fastcall StarTimer(); //启动线程工作
		__fastcall TShortInfo(int Flag=0);//Flag =0 短信猫，1 网关
        __fastcall ~TShortInfo();
		__property String DBConnectString  = { read=FDBConnectString, write=FDBConnectString };
		__property String AccConnectString  = { read=FAccConnectString, write=FAccConnectString };
        __property int ComPort  = { read=FComPort, write=FComPort };
		__property int ModeType  = { read=FModeType, write=FModeType };
		String __fastcall getStatus();
}
;
// 发送线程
class TSendThread : public TThread
{
private:

protected:
        void __fastcall Execute();
public:
        __fastcall TSendThread(bool CreateSuspended);
        Tfunc SendFun;
};
// 接收线程
class TRecvThread : public TThread
{
private:

protected:
        void __fastcall Execute();
public:
        __fastcall TRecvThread(bool CreateSuspended);
        Tfunc Recvfun;
};

// 处理时间段线程
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

