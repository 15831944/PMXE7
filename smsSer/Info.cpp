//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Info.h"
#include <ADODB.hpp>
#include <DB.hpp>
#include "ModeInter.hpp"
#include "DecEnc.h"
#include "sms.h"
#define Address "www.mobset.com"
_di_smsPortType SendSevice;
//--------------------------------------------------------------------------------
#pragma package(smart_init)
TRecvThread *RecvThread;
TSendThread *SendThread;
TEndTimeThread *EndTimeThread;
int PWaitLong;
__fastcall TShortInfo::TShortInfo(int Flag)
{
	 FFlag = Flag;
     DllHandle = NULL;
     SendList = NULL;
	 IndexList = NULL;
     RecvThread = NULL;
     SendThread = NULL;
	 OnWriteErrorLog = NULL;
	 LoadDllFunc();
     FWaitLong = 1000;
     FRepeatCount = 3;//默认为三次
	 FComPort=0; //默认com1
	 FModeType=0;//默认一口
	 AccConn=new TADOConnection(NULL);
	 AccConn->LoginPrompt=false;
	 MainTimer = new TTimer(NULL);
}
//---------------------------------------------------------------------------
bool __fastcall TShortInfo::GetDllHandle()
{
/*
   if (DllHandle != NULL) return true;
   String dllfilename=ExtractFilePath(Forms::Application->ExeName)+"SmsSdk.dll";
   DllHandle = LoadLibrary(dllfilename.c_str());
   if(DllHandle)
   {
		(FARPROC&) Sms_Proxy = GetProcAddress(DllHandle,"Sms_Proxy");
		(FARPROC&) Sms_Connect= GetProcAddress(DllHandle,"Sms_Connect");
		(FARPROC&) Sms_DisConnect= GetProcAddress(DllHandle,"Sms_DisConnect");
		(FARPROC&) Sms_Send2 = GetProcAddress(DllHandle,"Sms_Send2");
		(FARPROC&) Sms_Get2 = GetProcAddress(DllHandle,"Sms_Get2");
   }
	else
   {
		FErrorStr = "SmsSdk.dll动态库调用失败!" ;
		WriteErrorLog();
   }
   return DllHandle!=NULL;
*/
}

//---------------------------------------------------------------------------
String __fastcall TShortInfo::ErorType(int Flag)
{
   String ResultStr;
   switch( Flag)
   {
        case 100: ResultStr = "授权错误(检测软件狗)";
               break;
        case 101: ResultStr = "授权类型错误(检测软件狗类型)";
               break;
        case 102: ResultStr = "未初始化，请先初始化";
               break;
        case 103: ResultStr = "DLL文件错误";
               break;
        case 200: ResultStr = "端口号码错";
               break;
        case 201: ResultStr = "不支持的端口";
               break;
        case 202: ResultStr = "信息超长";
               break;
        case 203: ResultStr = "不能发送空信息";
               break;
        case 204: ResultStr = "手机号码错";
               break;
        case 205: ResultStr = "设备错";
               break;
        case 206: ResultStr = "检测中心错";
               break;
        case 207: ResultStr = "网络登录错";
               break;
        case 208: ResultStr = "设置格式错";
               break;
        case 209: ResultStr = "系统忙";
               break;
		case 210: ResultStr = "发送超时";
               break;
		default : if(Flag >=500)
				  {
					int port = Flag - 499;
					ResultStr = IntToStr(port)+"号端口不存在或被其他程序占用" ;
				  }
				  else
					ResultStr = "未知错误";
   }
   return ResultStr;
}
//---------------------------------------------------------------------------
bool __fastcall  TShortInfo::LoadDllFunc()
{
	if (FFlag == 0)   //短信猫
	{
	  return LoadModeDll();
	}
    else     //短信网关
    {
	  return GetDllHandle();
	}
}
//---------------------------------------------------------------------------
__fastcall TShortInfo::~TShortInfo()
{
   int i;
   MainTimer->Enabled =false;
   delete AccConn;
   if (DllHandle != NULL)
       FreeLibrary(DllHandle);
   if (SendList != NULL)
       delete SendList;
   if (IndexList != NULL)
       delete IndexList;
   if (SendThread != NULL)
   {
       SendThread->Terminate();
   }
   if (RecvThread != NULL)
   {
       RecvThread->Terminate();
   }
   if (EndTimeThread != NULL)
   {
       EndTimeThread->Suspend();
       EndTimeThread->Terminate();
   }
   if (FFlag == 0)
	   fnCloseModem(FComPort);
   else
	   if(SendSevice)
         SendSevice->Release();
   delete MainTimer;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TShortInfo::Initializtion()
{
	int index,LResult,i=0;
	String Result;
	if (FFlag==0)
    {
		//fnCloseModem(-1);
		fnSetThreadMode(-1);
		for(int i=0;i<10;i++)
		  fnSetModemType(i,0);
		fnGetModemType(FComPort);
		fnInitModem(FComPort);
	}
    else
    {
	  try
	  {
		 SendSevice = GetsmsPortType(true);
		 StockDetails *SeviceDetail = SendSevice->directGetStockDetails(FCordId,FUserName,FPassword);
		 if(SeviceDetail->retCode == "Faild")
		 {
			 FErrorStr = "网关连接失败："+SeviceDetail->message;
			 WriteErrorLog();
			 return;
		 }
		 else
		 {
			 FErrorStr = "网关连接成功！\n短信量余额:"+IntToStr(SeviceDetail->stockRemain)+"\n短信总发送量:"+IntToStr(SeviceDetail->sendTotal);
			 WriteErrorLog();
         }
	  }
      __finally
      {

      }
    }
	AccConn->ConnectionString=FAccConnectString;
	//初始化发送序列
	SendList = new TStringList();
	IndexList = new TStringList();
	//创建使用的线程
	SendThread = new TSendThread(true);
	SendThread->SendFun = SendShortMessage;
    if (FFlag == 0)
    {
	   RecvThread = new TRecvThread(true);
       RecvThread->Recvfun = ReadShortMessage;
    }
    else
	   RecvThread = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TShortInfo::SendShortMessage()
{
	 int index = 0,result;
     String * PNo;
     String Str;
     String szHeader;
	 TADOQuery* Query = new TADOQuery(NULL);
	 TADOQuery* QueryTemp = new TADOQuery(NULL);
	 try{
		  Query->Close();
		  Query->Connection = AccConn;
		  QueryTemp->Connection = AccConn;
		  Query->SQL->Text = "Exec sd_OperMessage";//邮件转为短信息
		  Query->ExecSQL();

		  Query->Close();
		  Query->SQL->Text = "select  * from vi_SendCache";
		  Query->Open();
		  Query->First();
		  while (!Query->Eof)
		  {
			 if(FFlag == 0)
             {
                PNo = new String (IntToStr(index));
				SendList->AddObject(Query->FieldByName("SCTPKID")->AsString,(TObject *)PNo);
                IndexList->Add(IntToStr(SendList->Count));
				szHeader = Query->FieldByName("SCTRecvPsPhone")->AsString+","+
                           IntToStr(SendList->Count)+",0,"+
						   Query->FieldByName("SCTSendPsPhone")->AsString+",0,1";
				try
				{
					result=SendData(szHeader,Query->FieldByName("SCTConet")->AsString);
					if(result == -2)
					{
					  FErrorStr = "发送短信失败！端口号指定错误或者是短信猫连线没有接好!";
					  WriteErrorLog();
					  return;
					}
					else if(result == -3)
					{
					  FErrorStr = "发送短信失败！短信猫连线没有接好或者电源没有打开!";
					  WriteErrorLog();
					  return;
					}
					else if(result >=0 && result <= 10)
					{
                       result = 0;
                    }
					else if(result >= 100)
					{
					  FErrorStr = ErorType(result);
					  WriteErrorLog();
					}
				}
                catch(...)
                {
                    FErrorStr = "发送短信失败";
					WriteErrorLog();
				}
			 }
             else //网关发送
			 {
                try
                {
                    MainTimer->Enabled = false;
					result=SendData(Query->FieldByName("SCTRecvPsPhone")->AsString
									 ,Query->FieldByName("SCTConet")->AsString);
                    MainTimer->Enabled = true;
                }
                catch(...)
                {
                    FErrorStr = "发送短信失败";
                    WriteErrorLog();
                    MainTimer->Enabled = true;
				}
			 }

			  try
			  {
                  QueryTemp->Close();
				  Str ="Exec sd_OperShortMessage "+IntToStr(result)+",'"
							  +Query->FieldByName("SCTPKID")->AsString+"',"
                              +IntToStr(FRepeatCount);
                  QueryTemp->SQL->Text = Str;
                  QueryTemp->ExecSQL();
              }
			  catch(...)           
			  {
                 FErrorStr = "发送后处理失败";
                 WriteErrorLog();
			  }
              Query->Next();
		  }
	 }
	__finally
	{
		delete QueryTemp;
		delete Query;
	}
}
//---------------------------------------------------------------------------
int __fastcall TShortInfo::SendData(String Heard,String Detail)
{
   long lSmsID;
   int Status;
   int i = 0;
   if (FFlag == 0)//猫发送
   {
	   Status = fnGetStatus(FComPort);

	   if(Status <= -1)
	   {
		 return -2;
	   }
	   else if(Status <= 1)
	   {
		 return -3;
	   }
	   else
	   {
		 return fnSendMsg(FComPort,Heard.c_str(),Detail.c_str());
	   }
   }
   else
   {
	   SendStatus *resultStatus;
	   SendSevice = GetsmsPortType(true);
	   resultStatus = SendSevice->directSend(FCordId,FUserName,FPassword,Heard,Detail,"",1,"",1);
	   if(resultStatus->retCode == "Faild")
	   {
		  FErrorStr = "短信发送失败："+resultStatus->message;
		  WriteErrorLog();
		  return -1;
	   }
	   else
		  return 1;
   }
}
//---------------------------------------------------------------------------
void __fastcall TShortInfo::ReadShortMessage()
{
	 int result;
	 int index;
	 Char * szHeader;
	 Char * MainMessge;
	 int i,j;
     String *PNo;
     String Str,ID;
	 TADOQuery* Query = new TADOQuery(NULL);
	 Query->Connection=AccConn;
     try
     {
		szHeader = StrAlloc(500);
        MainMessge = StrAlloc(500);
		result = fnReadMsgEx(-1,szHeader,MainMessge);
		if ( result == 0 )
		{
			Str = szHeader;
			i = StrToInt(ResultStr(Str,1));
		   if (i > 0 && i < 6) //接收发送信息消息
		   {
			   j = IndexList->IndexOf(StrToInt(ResultStr(Str,5)));
			   if (j == -1)
			   {
				   FErrorStr = "接收到信息，无法更新对应纪录";
				   WriteErrorLog();
				   return;
			   }
			   ID=SendList->Strings[j];
			   Query->Close() ;
			   PNo = (String *)SendList->Objects[j];
			   Query->SQL->Text = "Exec tbMsgSendRcd_updateState '"+ID+"',"+IntToStr(i);
			   try
			   {
				  Query->ExecSQL();
				  SendList->Delete(j);
				  IndexList->Delete(j);
			   }
			   catch(...)
			   {
				  FErrorStr = "更新数据失败!";
				  WriteErrorLog();
			   }
		   }
		}
    }
    __finally
    {
        StrDispose(szHeader);
        StrDispose(MainMessge);
    }
}
//--------------------------------------------------------------
void __fastcall TShortInfo::StarTimer()
{
	MainTimer->Interval = FWaitLong;
	MainTimer->OnTimer = OnTimer;
    MainTimer->Enabled = true;
}
//-----------------------------------------------------------------
void __fastcall TShortInfo::WriteErrorLog()
{
    if (OnWriteErrorLog != NULL)
        OnWriteErrorLog(FErrorStr);
}
String __fastcall TShortInfo::ResultStr(String Source,int flag)
{
  int i,index;
  String Str;
  for (index = 1;index<=flag;index++)
  {
     i = Source.Pos(",");
     if (i != 0)
     {
         Str = Source.SubString(1,i-1);
         Source = Source.SubString(i+1,Source.Length()-i);
     }
     else
     {
        if (index==flag)
            return Source;
        else
            return "";
     }
  }
  return Str;
}
//------------------------------------------------------------
void __fastcall TShortInfo::OnTimer(TObject *Sender)
{
	if ( (StrToTime(FormatDateTime("HH:mm:ss",Now()))>=FBeginTime)
         &&(StrToTime(FormatDateTime("HH:mm:ss",Now()))<=FEndTime)
         )
	 {
		 if (RecvThread!= NULL && RecvThread->Suspended)
			 RecvThread->Resume();
		 if (SendThread!= NULL && SendThread->Suspended)
			 SendThread->Resume();
	 }
}
//---------------------------------------------------------------------------
__fastcall TSendThread::TSendThread(bool CreateSuspended )
        : TThread(CreateSuspended)
{
     FreeOnTerminate = true;
     this->Priority = tpIdle;
}
//---------------------------------------------------------------------------

void __fastcall TSendThread::Execute()
{
        //---- Place thread code here ----
        while (!this->Terminated)
        {
            if (SendFun!=NULL)
            {
                try
				{
                        Synchronize(SendFun);
                }
                catch(...)
                {
                         //以后处理
                }
            }
			this->Suspend();
        }
}

//---------------------------------------------------------------------------
__fastcall TRecvThread::TRecvThread(bool CreateSuspended )
        : TThread(CreateSuspended)
{
     FreeOnTerminate = true;
     this->Priority = tpIdle;
}

//---------------------------------------------------------------------------
void __fastcall TRecvThread::Execute()
{
        //---- Place thread code here ----
        while (!this->Terminated)
        {
            if (Recvfun!=NULL)
            {
               try
               {
                    Synchronize(Recvfun);
               }
               catch(...)
               {
                   //以后处理
               }
            }
			this->Suspend();
        }
}
//---------------------------------------------------------------------------
__fastcall TEndTimeThread::TEndTimeThread(bool CreateSuspended )
        : TThread(CreateSuspended)
{
     FreeOnTerminate = true;
     this->Priority = tpIdle ;
}
//---------------------------------------------------------------------------
void __fastcall TEndTimeThread::Execute()
{
        //---- Place thread code here ----
        while (!this->Terminated)
        {
			if ( (StrToTime(FormatDateTime("HH:mm:ss",Now()))>=FBeginTime)
				 &&(StrToTime(FormatDateTime("HH:mm:ss",Now()))<=FEndTime))
            {
				if (RecvThread != NULL && RecvThread->Suspended)
					RecvThread->Resume();
				else if (SendThread != NULL && SendThread->Suspended)
					SendThread->Resume();
            }
            else
            {
                if (RecvThread != NULL && (!RecvThread->Suspended))
                    RecvThread->Suspend();
				if (SendThread != NULL && (!SendThread->Suspended))
                    SendThread->Suspend();
            }
        }
}
String _fastcall TShortInfo::getStatus()
{
  return IntToStr(fnGetStatus(FComPort));
}
