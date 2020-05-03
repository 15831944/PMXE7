// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <dir.h>
#include <System.IOUtils.hpp>
#include "BaseFunc.h"
//#include "MessageDef.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)
// ---------------------------------------------------------------------------
void ClearTempFile(String FileName)
{
	TSearchRec sr;
	int iAttributes = 0;
	iAttributes |= faReadOnly;
	iAttributes |= faHidden;
	iAttributes |= faSysFile;
//	iAttributes |= faVolumeID;
	iAttributes |= faDirectory;
	iAttributes |= faArchive;
	iAttributes |= faAnyFile;

	int done;
	String FilePath, DelFileName;
	FilePath = ExtractFilePath(FileName);
	if(FindFirst(FileName, iAttributes, sr) == 0)
	{
		do
		{
			if((sr.Attr & iAttributes) == sr.Attr)
			{
				DelFileName = FilePath + sr.Name;
				DeleteFile(DelFileName);
			}
		}
		while (FindNext(sr) == 0);
		FindClose(sr);
	}
}

// ---------------------------------------------------------------------------
String GetSpecStr(String srcstr, String begstr, String endstr, int &endpos)
{
	int startindex, begindex, endindex, status;
	String cmpstr;
	status = 0;
	Char *pos;
	if(srcstr.IsEmpty())
	{
		endpos = 0;
		return "";
	}
	// 0-比较开始字串,1-读取开始字串,2-比较结束字串,3-读取结束字串
	pos = srcstr.c_str();
	for (int i = 1; i <= srcstr.Length(); i++)
	{
		if(status == 0)
		{
			if(*pos == begstr[1])
			{
				begindex = i;
				startindex = i;
				status = 1;
			}
		}

		if(status == 1)
		{
			if(i - begindex + 1 < begstr.Length())
			{
				if(*pos != begstr[i - begindex + 1])
					status = 0;
			}
			else if(i - begindex + 1 == begstr.Length())
			{
				if(*pos == begstr[i - begindex + 1])
					status = 2;
				else
					status = 0;
			}
		}

		if(status == 2)
		{
			if(*pos == endstr[1])
			{
				begindex = i;
				status = 3;
			}
		}

		if(status == 3)
		{
			if(i - begindex + 1 < endstr.Length())
			{
				if(*pos != endstr[i - begindex + 1])
					status = 2;
			}
			else if(i - begindex + 1 == endstr.Length())
			{
				if(*pos == endstr[i - begindex + 1])
				{
					endpos = i;
					return srcstr.SubString1(startindex, i - startindex + 1);
				}
				else
				{
					status = 2;
				}
			}
		}
		pos++;
	}
	endpos = 0;
	return "";
}
// ---------------------------------------------------------------------------
String OpenTempFile(String PrefixString)
{
	Char filepath[255], filename[255];
	GetTempPath(255, filepath);
	GetTempFileName(filepath, PrefixString.c_str(), 0, filename);
	return String(filename);
}
// ---------------------------------------------------------------------------
void DeleteTempFile(String FileName)
{
	if (FileExists(FileName))
		DeleteFile(FileName);
}
// ---------------------------------------------------------------------------
FILE* OpenLogFile(String PrefixString)
{
	FILE* logfile;
	String filename;
	filename = PrefixString + TDateTime().CurrentDateTime().FormatString("yymm") + ".log";
	logfile = fopen(AnsiString(filename).c_str(), "a+");
	if (logfile != NULL)
		logfile->bsize = 0;
	return logfile;
}
// ---------------------------------------------------------------------------
void WriteLogFile(FILE* FileHandle, String msg)
{
	String log;
	if(FileHandle == NULL) return;
	log = "[" + TDateTime().CurrentDateTime().FormatString("yyyy-MM-dd HH:mm:ss") + "] " + msg + "\r\n";
	fputs(AnsiString(log).c_str(), FileHandle);
}
// ---------------------------------------------------------------------------
double DataCarry(double data, int scale, int method)
{
	String FStr, FDec, FTmp, FormatStr;
	int FPos, Value, Factor, FInt;
	double FResult;
	bool CarryFlg;
	if (scale < 0)
		throw Exception("小数位数不能小于零");
	if (method != 0 && method != 1 && method != 2)
		throw Exception("取数方法只能是0-四舍五入,1-小数取整,2-向上取整");
	FStr = FormatFloat("0.###############", data);
	FPos = FStr.Pos(".");
	if (FPos == 0)
		return data;
	if (scale == 0)
		Factor = 1;
	else
	{
		FTmp = "1";
		for (int i = 0; i < scale; i++)
			FTmp += "0";
		Factor = FTmp.ToInt();
	}

	switch (method)
	{
	case 0:
		if (FStr.Length() - FPos <= scale)
			return data;
		else
		{
			FDec = FStr.SubString1(FPos + 1, FStr.Length());
			CarryFlg = false;
			for (int i = FDec.Length(); i > scale; i--)
			{
				if (CarryFlg)
				{
					Value = StrToInt(FDec[i]) + 1;
					if (Value >= 5)
						CarryFlg = true;
					else
						CarryFlg = false;
				}
				else
				{
					if (FDec[i] >= '5')
						CarryFlg = true;
					else
						CarryFlg = false;
				}
			}
			if (CarryFlg)
				FInt = data * Factor + 1;
			else
				FInt = data * Factor;
			FResult = (double)FInt / Factor;
			return FResult;
		}
	case 1:
		if (FStr.Length() - FPos <= scale)
			return data;
		else
		{
			FInt = data * Factor;
			FResult = (double)FInt / Factor;
			return FResult;
		}
	case 2:
		if (FStr.Length() - FPos <= scale)
			return data;
		else
		{
			FInt = data * Factor + 1;
			FResult = (double)FInt / Factor;
			return FResult;
		}
	default:
		return 0;
	}
}

// ---------------------------------------------------------------------------
double Mod(double x, double y)
{
	while (x > y)
		x -= y;
	return x;
}

// ------------------------------------------------------------------------------
String EncAndDec(String Password, bool Encrypt, int Factor)
{
	int MaskXor;
	int i;
	String EncryptStr;
	if (Password.Length() < 1)
		return Password;
	MaskXor = (Password.Length() * 3945 / 22 + 29) * Factor;
	while (MaskXor >= 128)
		MaskXor -= 128;
	if (Encrypt)
	{
		for (i = 1; i <= Password.Length(); i++)
		{
			if (i == 1)
				EncryptStr += Char(Password[i] ^ MaskXor);
			else
				EncryptStr += Char(Password[i] ^ Char(Mod((EncryptStr[i - 1] + MaskXor), 128)));
		}
	}
	else
	{
		for (i = 1; i <= Password.Length(); i++)
		{
			if (i == 1)
				EncryptStr += Char(Password[i] ^ MaskXor);
			else
				EncryptStr += Char(Password[i] ^ Char(Mod((Password[i - 1] + MaskXor), 128)));
		}
	}
	return EncryptStr;
}

// ------------------------------------------------------------------------------
String GetInifileValue(String lpSection, String lpKey, String lpFilename)
{
	Char Buff[256];
	int mSize;
	String ReturnValue;
	mSize = GetPrivateProfileString(lpSection.c_str(), lpKey.c_str(), L"", Buff, 256, lpFilename.c_str());
	for (int i = 0; i < mSize; i++)
		ReturnValue += Buff[i];
	return ReturnValue;
}

// ------------------------------------------------------------------------------
bool SetInifileValue(String lpSection, String lpKey, String lpValue, String lpFilename)
{
	int Rvalue;
	Rvalue = WritePrivateProfileString(lpSection.c_str(), lpKey.c_str(), lpValue.c_str(), lpFilename.c_str());
	if (Rvalue != 0)
		return true;
	else
		return false;
}
// ------------------------------------------------------------------------------
String GetComputerIP(String ComputerName)
{
	String ComputerIP;
	char HostName[80];
	LPHOSTENT lpHostEnt;
	struct in_addr addr[10];
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		return "";
	}

	for (int i = 0; i < 10; i++)
		memset(&addr[i], 0, sizeof(in_addr));

	if (ComputerName.IsEmpty())
	{
		if (gethostname(HostName, sizeof(HostName)) == SOCKET_ERROR)
		{
			return "";
		}
	}
	else
		strcpy(HostName, AnsiString(ComputerName).c_str());

	lpHostEnt = gethostbyname(HostName);
	if (!lpHostEnt)
	{
		return "";
	}
	for (int i = 0; lpHostEnt->h_addr_list[i] != 0; i++)
		memcpy(&addr[i], lpHostEnt->h_addr_list[i], sizeof(in_addr));
	ComputerIP = inet_ntoa(addr[0]);
	// SecondIP=inet_ntoa(addr[1]);
	WSACleanup();
	return ComputerIP;
}
// ------------------------------------------------------------------------------
String GetMachineName()
{
	Char ComputerName[255];
	unsigned long MaxLength = 255;
	bool RetVal;
	RetVal = GetComputerName(ComputerName, &MaxLength);
	if (RetVal == 0)
		return "";
	return String(ComputerName);
}
// ------------------------------------------------------------------------------
String GetRemoteName(String szAddr)
{
	WSADATA wsaData;
	AnsiString name;
	struct hostent *host;
	struct sockaddr_in addr;
	int addrlen, ret;
	char *p;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		return "";
	addr.sin_family = AF_INET;
	addr.sin_port = 80;
	addr.sin_addr.S_un.S_addr = inet_addr(AnsiString(szAddr).c_str());
	addrlen = sizeof(addr);
	host = gethostbyaddr((char *)&addr.sin_addr, addrlen, AF_INET);
	if (host == NULL)
	{
		WSACleanup();
		return "";
	}
	p = host->h_name;
	name.SetLength(strlen(p) + 1);
	strcpy(name.c_str(), p);
	return name;
}

// ------------------------------------------------------------------------------
int ScanPort(char *szServer, int &lDefPort, int lCount)
{
	struct sockaddr_in server;
	struct hostent *host = NULL;
	int sockfd, len, sockport;
	bool bfound;
	WSADATA WSAData;
	sockport = lDefPort;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData))
	{
		// MessageBox(NULL,"wsock32.dll不能被初始化!","Warning",MB_OK);
		lDefPort = 0;
		return 2;
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(szServer);
	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			// MessageBox(NULL,"创建地址错误","Warning",MB_OK);
			lDefPort = 0;
			closesocket(sockfd);
			WSACleanup();
			return 3;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}
	bfound = false;
	for (int i = lDefPort; i < lDefPort + lCount; i++)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET)
		{
			// MessageBox(NULL,"创建套接字错误","Warning",MB_OK);
			lDefPort = 0;
			WSACleanup();
			return 4;
		}
		server.sin_port = htons((unsigned short)sockport);
		if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			bfound = true;
			lDefPort = sockport;
			closesocket(sockfd);
			WSACleanup();
			break;
		}
		closesocket(sockfd);
		sockport++;
	}
	if (bfound)
	{
		return 0;
	}
	else
	{
		WSACleanup();
		lDefPort = 0;
		return 1;
	}
}

// ------------------------------------------------------------------------------
String GetGUIDString(void)
{
	UUID *NewUuid = new UUID;
	unsigned short * StringUuid;
	if (UuidCreate(NewUuid) == RPC_S_OK)
	{
		UuidToString(NewUuid, &StringUuid);
		return (Char*)StringUuid;
	}
	else
	{
		return "";
	}
}

// ------------------------------------------------------------------------------
String GetShortGuid()
{
	char str[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ@#$&";
	UUID uuid;
	SYSTEMTIME st;
	AnsiString result1, result2;
	result1 = "";
	result2 = "";
	unsigned int part1, part2;
	int len;
	if (UuidCreate(&uuid) == RPC_S_OK)
	{
		part1 = uuid.Data1;
		while (1)
		{
			part2 = part1 % 40;
			part1 = part1 / 40;
			result1 += str[part2];
			if (part1 < 40)
			{
				result1 += str[part1];
				break;
			}
		}
		len = result1.Length();
		for (int i = len; i < 8; i++)
			result1 = "0" + result1;
		GetLocalTime(&st);
		result1[1] = str[st.wDay];
	}
	return result1;
}
// ------------------------------------------------------------------------------
String RebuildSQLStatement(String sqlstr)
{
	String retval = "";
	int index;
	index = sqlstr.Pos1("'");
	while (index != 0)
	{
		retval += sqlstr.SubString1(1, index) + "'";
		sqlstr.Delete1(1, index);
		index = sqlstr.Pos1("'");
	}
	retval += sqlstr;
	return retval;
}

// ---------------------------------------------------------------------------
String GetMacAddr(String ip)
{
	String s;
	HINSTANCE hInst;
	unsigned char macaddr[6];
	unsigned char pbyte;
	unsigned long *pmacaddr, addrlen;
	typedef unsigned long __stdcall(CALLBACK * HSENDARP)(unsigned long,
		unsigned long, unsigned long*, unsigned long*);
	addrlen = sizeof(macaddr);
	pmacaddr = (unsigned long*)&macaddr[0];
	hInst = LoadLibrary(L"iphlpapi.dll");
	HSENDARP SendARP;
	SendARP = (HSENDARP)GetProcAddress(hInst, "SendARP");
	SendARP(inet_addr(AnsiString(ip).c_str()), 0, pmacaddr, &addrlen);
	if (addrlen > 0)
	{
		s = IntToHex(macaddr[0], 2) + "-";
		s += IntToHex(macaddr[1], 2) + "-";
		s += IntToHex(macaddr[2], 2) + "-";
		s += IntToHex(macaddr[3], 2) + "-";
		s += IntToHex(macaddr[4], 2) + "-";
		s += IntToHex(macaddr[5], 2);
	}
	FreeLibrary(hInst);
	return s;
}

// ---------------------------------------------------------------------------
int MakePrimalKey(const char *szPwd)
{
	DWORD dwBaseKey;
	int dwMaskKey;
	if (strlen(szPwd) == 0)
		return 0;
	else
	{
		dwBaseKey = 0;
		if (strlen(szPwd) > 15)
		{
			for (int i = 0; i < 15; i++)
			{
				dwBaseKey += *(szPwd + i)*(i + 1);
				srand(dwBaseKey);
				dwMaskKey = rand();
				dwBaseKey = dwMaskKey;
			}
		}
		else
		{
			for (unsigned int i = 0; i < strlen(szPwd); i++)
			{
				dwBaseKey += *(szPwd + i)*(i + 1);
				srand(dwBaseKey);
				dwMaskKey = rand();
				dwBaseKey = dwMaskKey;
			}
		}
	}
	return dwMaskKey;
}

// ---------------------------------------------------------------------------
bool DynamicEncrypt(const void *source, void *dest, int len, int key)
{
	unsigned char nkey;
	unsigned char *sustr, *dustr;
	sustr = (unsigned char*)source;
	dustr = new char[len + 1];
	srand(key);
	for (int i = 0; i < len; i++)
	{
		nkey = rand();
		dustr[i] = sustr[i] ^ nkey;
	}
	memcpy(dest, dustr, len);
	delete[]dustr;
	return true;
}

// ---------------------------------------------------------------------------
bool DynamicDecrypt(const void *source, void *dest, int len, int key)
{
	unsigned char nkey;
	unsigned char *sustr, *dustr;
	sustr = (unsigned char*)source;
	dustr = new char[len + 1];
	srand(key);
	for (int i = 0; i < len; i++)
	{
		nkey = rand();
		dustr[i] = sustr[i] ^ nkey;
	}
	memcpy(dest, dustr, len);
	delete[]dustr;
	return true;
}

// ---------------------------------------------------------------------------
int GetFileDateTime(String lpFileName, LPSYSTEMTIME lpFileTime, DWORD dwFileTimeType)
{
	// SYSTEMTIME st;
	FILETIME lft, ftc, fta, ftw;
	HANDLE h = CreateFileW(lpFileName.c_str(), GENERIC_READ, // access (read-write) mode
		FILE_SHARE_READ, // share mode
		NULL, // pointer to security attributes
		OPEN_EXISTING, // how to create
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL // handle to file with attributes to copy
		);
	if (h == INVALID_HANDLE_VALUE)
	{
		// SetLastError(...);
		return -1;
	}
	BOOL b = GetFileTime(h, &ftc, &fta, &ftw);
	CloseHandle(h);
	if (!b)
	{
		// SetLastError(...);
		return b;
	}
	switch (dwFileTimeType)
	{
	case CREATION_FILE_TIME:
		b = FileTimeToLocalFileTime(&ftc, &lft);
		break;
	case LASTACCESS_FILE_TIME:
		b = FileTimeToLocalFileTime(&fta, &lft);
		break;
	case LASTWRITE_FILE_TIME:
		b = FileTimeToLocalFileTime(&ftw, &lft);
		break;
	default:
		return -1;
	}
	if (!b)
	{
		return b;
	}
	b = FileTimeToSystemTime(&lft, lpFileTime);
	if (!b)
	{
		return b;
	}
	else
		return b;
}

// ---------------------------------------------------------------------------
int SetFileDateTime(String lpFileName, LPSYSTEMTIME lpFileTime, DWORD dwFileTimeType)
{
	FILETIME lft, ft;
	BOOL b = SystemTimeToFileTime(lpFileTime, &lft);
	if (!b)
	{
		return b;
	}
	b = LocalFileTimeToFileTime(&lft, &ft);
	if (!b)
	{
		return b;
	}
	HANDLE h = CreateFileW(lpFileName.c_str(), GENERIC_WRITE,
		// access (read-write) mode
		FILE_SHARE_READ, // share mode
		NULL, // pointer to security attributes
		OPEN_EXISTING, // how to create
		FILE_ATTRIBUTE_NORMAL, // file attributes
		NULL // handle to file with attributes to copy
		);
	if (h == INVALID_HANDLE_VALUE)
	{
		// SetLastError(...);
		return -1;
	}
	switch (dwFileTimeType)
	{
	case CREATION_FILE_TIME:
		b = SetFileTime(h, &ft, NULL, NULL);
		break;
	case LASTACCESS_FILE_TIME:
		b = SetFileTime(h, NULL, &ft, NULL);
		break;
	case LASTWRITE_FILE_TIME:
		b = SetFileTime(h, NULL, NULL, &ft);
		break;
	default:
		return -1;
	}
	CloseHandle(h);
	if (!b)
	{
		// SetLastError(...);
	}
	return b;
}

// ---------------------------------------------------------------------------
BOOL SetShareVariable(String szVarName, String szValue)
{
	return SetEnvironmentVariable(szVarName.c_str(), szValue.c_str());
}

// ---------------------------------------------------------------------------
DWORD GetShareVariable(String szVarName, String &szValue)
{
	Char buf[1024];
	DWORD retval;
	memset(buf, 0, 1024);
	retval = GetEnvironmentVariable(szVarName.c_str(), buf, 1024);
	szValue.SetLength(StrLen(buf));
	wcscpy(szValue.c_str(), buf);
	return retval;
}

// ---------------------------------------------------------------------------
void OpenFileNormal(const Char *szFileName)
{
	HINSTANCE hIns;
	HWND hWnd;
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	hFile = FindFirstFile(szFileName, &fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ShowMessage("指定文件不存在!");
		return;
	}
	if (hFile)
		FindClose(hFile);
	hIns = ShellExecute(NULL, NULL, szFileName, NULL, NULL, SW_SHOW);
	if ((int)hIns <= 32)
	{
		ShowMessage("指定文件无法打开!");
	}
}

// ---------------------------------------------------------------------------
void OpenFileModal(const Char *szFileName)
{
	String szCommandLine;
	bool retval;
	Char lpResult[255];
	HINSTANCE hIns;
	HWND hWnd;
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	hFile = FindFirstFile(szFileName, &fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ShowMessage("指定文件不存在!");
		return;
	}
	if (hFile)
		FindClose(hFile);
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_SHOW;
	if (ExtractFileExt(szFileName).LowerCase() == "exe" || ExtractFileExt(szFileName).LowerCase() == "com")
	{
		szCommandLine = szFileName;
	}
	else
	{
		hIns = FindExecutable(szFileName, NULL, lpResult);
		if ((int)hIns <= 32)
		{
			ShowMessage("指定文件无法打开!");
			return;
		}
		szCommandLine = lpResult;
		if (szCommandLine.IsEmpty())
		{
			ShowMessage("指定文件无法打开!");
			return;
		}
	}
	szCommandLine += " " + AnsiQuotedStr(String(szFileName), '"');
	retval = CreateProcess(NULL, szCommandLine.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (retval == false)
	{
		ShowMessage("指定文件无法打开!");
		return;
	}
	// WaitForSingleObject(pi.hThread,INFINITE);
	// CloseHandle( pi.hProcess );
	// CloseHandle( pi.hThread );
	// return;
	DWORD dw;
	MSG msg;
	do
	{
		dw = WaitForSingleObject(pi.hThread, 50);
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_PAINT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (msg.message == WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
			else
				WaitMessage();
		}
	}
	while (dw == WAIT_TIMEOUT);
	if (pi.hProcess)
		CloseHandle(pi.hProcess);
}

// ---------------------------------------------------------------------------
String __fastcall BuildConnStr(int dbtype, String dbserver, String dbname, String dbuid, String dbpwd)
{
	String cnstr;
	if (dbtype == 1)
	{
		cnstr = "Provider=SQLOLEDB.1;Password=" + dbpwd;
		cnstr += ";Persist Security Info=True;User ID=" + dbuid;
		cnstr += ";Initial Catalog=" + dbname;
		cnstr += ";Data Source=" + dbserver;
	}
	else if (dbtype == 2)
	{
		cnstr = "Provider=MSDAORA.1;Password=" + dbpwd;
		cnstr += ";User ID=" + dbuid;
		cnstr += ";Data Source=" + dbserver;
		cnstr += ";Persist Security Info=True";
	}
	return cnstr;
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
