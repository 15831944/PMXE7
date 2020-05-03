unit ModeInter;

interface
uses
   winapi.Windows, Classes, SysUtils,Vcl.Forms;
function LoadModeDll: Boolean;
var
{
������������DLL�����߳�ģʽִ�У�������Ϣ���Ƶ�ִ�У������ʺ��Է���ʽִ�еĳ���ĵ��á�
����: nMode - 1 �����߳�ģʽ
ע��:һ�������е�fnInitModem��������֮ǰ�����������Ч�����������ɳ�����ҡ�
����: 0-�ɹ���
}
fnSetThreadMode: function(nMode:integer):integer stdcall;
 //����˵��
//�ȳ�ʼ��,Ȼ���ͻ��߽���,�˳�����ǰ�ر�Modem
{*************************************************************}
//  ����: nComNo 0~255 ����COM��ţ�
//        modemType ����è����
//	  0 - 1�ڶ���è��
//        1 - 2�ڶ���è��
//        2 - 4�ڶ���è��
//        3 - 8����è��
//  ����: 0 - �ɹ��� -1���ʹ�
//        XXX -������룬�μ������
{*************************************************************}
fnSetModemType: function(ComNo:integer; modemType:integer{Smallint}) :integer stdcall;

{******************************************************}
//	��ȡ��ǰ����è��������
//����: nComNo 0~255 ����COM��š�
//���� ������è����
//0 - 1�ڶ���è��
//1 - 2�ڶ���è��
//2 - 4�ڶ���è��
//3 - 8����è��
//XXX -������룬�μ������
{*******************************************************}
 fnGetModemType: function(ComNo:integer):integer stdcall;

{***********************��ʼ������***********************}
//����: comx 0~7 ����˿ں��룬 -1 ��ʾ���ж˿�
//����: 0 - �ɹ�  XXX -������룬�μ������
{********************************************************}

 fnInitModem: function(comx:integer):integer;stdcall;

{***********************���ͺ���*************************}
//����: comx 0~7 ����˿ں��룬 -1 ��ʾ����һ�����ö˿�
//receivephone  �����ֻ��ţ�sendmsg��ʾ������Ϣ����
//sendmsg(Ӧ����70���ַ�����,����������,1��������1���ַ�)
//����: ��ϲ����,���Գɹ�!   ����Ϊ10
//����: 0 - ���ͳɹ� 1 - ����ʧ�� XXX -������룬�μ������
{********************************************************}
 fnSendMsg: function(comx:integer;receivephone,sendmsg:pchar):integer stdcall;

{********************************************************}
 // ����: nPortNo 0~255 ����˿ں��룬 -1 ��ʾ���ж˿�
//����: �˿�״̬�����ڵ���8��ʾ�豸�������������Խ��պͷ��͡�
 //     ״̬С��8����ʾ����״̬��С�ڵ���1����������û�нӺû��ߵ�Դ
 //	  û�д򿪣�ע�⸴λ����è��
 //    -1 �˿ں�ָ����������Ƕ˿�δ��

{********************************************************}
 fnGetStatus: function(nPortNo:integer):integer stdcall;
{*************************���պ���***********************}
//����: comx 0~7 ����˿ں��룬
//(-1 ��ʾ����һ�����ö˿�,��ʱ��֧��)
//sendphone ���ͷ��ֻ�����      receivemsg ������Ϣ����
//����: 0~7 - �ɹ�,���ն˿ں���  -1 -  ����Ϣ
//XXX -������룬�μ������
{********************************************************}
 fnReadMsgEx: function(comx:integer;szHeader,receivemsg:pchar):integer stdcall;

 fnGetSndCount: function(nPortNo:integer):integer stdcall;
 fnGetRecCount: function(nPortNo:integer):integer stdcall;
//function fnSetReceive(nType:integer):integer stdcall;

{************************�رպ���************************}
//����: comx 0~7 ����˿ں��룬 -1 ��ʾ���ж˿�
//����: 0 - �ɹ�  XXX -������룬�μ������
{********************************************************}

 fnCloseModem: function(comx:integer):integer stdcall;

{*******************��������:**************************}
// 100: ��Ȩ����(��������)
// 101: ��Ȩ���ʹ���(������������)
// 102: δ��ʼ�������ȳ�ʼ��

// 200: �˿ں����
// 201: ��֧�ֵĶ˿�
// 202: ��Ϣ����
// 203: ���ܷ��Ϳ���Ϣ
// 204: �ֻ������
// 205: �豸��

//5xx: ����xx�Ŷ˿ڴ���
{********************************************************}
implementation
var
  hModeDll: THandle;

function LoadModeDll: Boolean;
var
  DLLName:String ;
begin
  Result := hModeDll > 0;
  if Result then Exit;
  DLLName:=ExtractFilePath(Application.ExeName)+'MonDem.dll';
  hModeDll := LoadLibrary(PChar(DLLName));
  Result := hModeDll > 0;
  if Result then
  begin
        fnSetThreadMode:= GetProcAddress(hModeDll, 'fnSetThreadMode');
        fnSetModemType := GetProcAddress(hModeDll, 'fnSetModemType');
        fnGetModemType := GetProcAddress(hModeDll, 'fnGetModemType');
        fnInitModem    := GetProcAddress(hModeDll, 'fnInitModem');
        fnSendMsg      := GetProcAddress(hModeDll, 'fnSendMsg');
        fnGetStatus    := GetProcAddress(hModeDll, 'fnGetStatus');
        fnReadMsgEx    := GetProcAddress(hModeDll, 'fnReadMsgEx');
        fnGetSndCount  := GetProcAddress(hModeDll, 'fnGetSndCount');
        fnGetRecCount  := GetProcAddress(hModeDll, 'fnGetRecCount');
        fnCloseModem   := GetProcAddress(hModeDll, 'fnCloseModem');
  end;
end;
procedure FreeModeDll;
begin
  if hModeDll > 0 then
  begin
        fnSetThreadMode:= nil;
        fnSetModemType := nil;
        fnGetModemType := nil;
        fnInitModem    := nil;
        fnSendMsg      := nil;
        fnGetStatus    := nil;
        fnReadMsgEx    := nil;
        fnGetSndCount  := nil;
        fnGetRecCount  := nil;
        fnCloseModem   := nil;
        FreeLibrary(hModeDll);
  end;
  hModeDll := 0;
end;
initialization
finalization
  FreeModeDll;
end.
