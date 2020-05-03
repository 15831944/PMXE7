// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SafeDataApi.h"
#include <stdio.h>
#include <stdlib.h>
#include "SafeDataDef.h"
#include "BaseFunc.h"
#include "Shzip.h"
#include "DBHead.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

int InitializeSafety(void) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int ret, dwFileInfo;
	Char szFileDesc[] = L"MMD Author Sunny Zhan";
	/////////////////////////////////////////////////////////////////////
	// Initialize depolic file
	/////////////////////////////////////////////////////////////////////
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_FILE_DESC, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(szFileDesc, StrLen(szFileDesc), 1, fp);
	if ((ret = fseek(fp, SAFETY_FILE_TYPE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 1;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, SAFETY_FILE_INIT, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 160;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, POLIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, POLIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, POLIC_LICSIZE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	fclose(fp);

	/////////////////////////////////////////////////////////////////////
	// Initialize delicres file
	/////////////////////////////////////////////////////////////////////
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_FILE_DESC, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(szFileDesc, StrLen(szFileDesc), 1, fp);
	if ((ret = fseek(fp, SAFETY_FILE_TYPE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 2;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, SAFETY_FILE_INIT, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 160;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);

	if ((ret = fseek(fp, LIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_PRJLEVEL_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_MODULE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_MODULE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_WINDOW_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_WINDOW_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFFUNC_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFFUNC_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_LIBFILE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_SYSTEM_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	dwFileInfo = 0;
	fwrite(&dwFileInfo, sizeof(int), 1, fp);

	Char tag;
	tag = 0;
	if ((ret = fseek(fp, SAFETY_MODULE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	if ((ret = fseek(fp, SAFETY_WINDOW_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	if ((ret = fseek(fp, SAFETY_USERDEFFUNC_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	if ((ret = fseek(fp, SAFETY_USERDEFRIGHT_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	if ((ret = fseek(fp, SAFETY_LIBFILE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	if ((ret = fseek(fp, SAFETY_SYSTEM_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);
	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetPolicKey(int dwPrimalKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwOriKey, dwDesKey, dwDecKey, dwLength, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, POLIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwOriKey, sizeof(int), 1, fp);
	if ((ret = fseek(fp, POLIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwPrimalKey);
	if (dwDecKey == dwOriKey)
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int SetPolicKey(int dwPolicKey, int dwPrimalKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwOriKey, dwDesKey, dwDecKey, dwLength, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, POLIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&dwPolicKey, sizeof(int), 1, fp);
	if ((ret = fseek(fp, POLIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&dwPolicKey, &dwDesKey, sizeof(int), dwPrimalKey);
	fwrite(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetLicFileSize(int dwPolicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesSize, dwDecSize, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, POLIC_LICSIZE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesSize, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesSize, &dwDecSize, sizeof(int), dwPolicKey))
		return dwDecSize;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int SetLicFileSize(int dwLicSize, int dwPolicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesSize, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, POLIC_LICSIZE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&dwLicSize, &dwDesSize, sizeof(int), dwPolicKey);
	fwrite(&dwDesSize, sizeof(int), 1, fp);
	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetLicenseKey(int dwPolicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwOriKey, dwDesKey, dwDecKey, dwLength, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, LIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwOriKey, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwPolicKey);
	if (dwDecKey == dwOriKey)
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int SetLicenseKey(int dwLicKey, int dwPolicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwOriKey, dwDesKey, dwDecKey, dwLength, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, LIC_ORIKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&dwLicKey, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_DESKEY_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&dwLicKey, &dwDesKey, sizeof(int), dwPolicKey);
	fwrite(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetProjectLevel(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, LIC_PRJLEVEL_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int SetProjectLevel(int dwLevel, int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, ret;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, LIC_PRJLEVEL_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&dwLevel, &dwDesKey, sizeof(int), dwLicKey);
	fwrite(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetModuleSize(int dwLicKey) // ����ѹ��������ݴ�С
{
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_MODULE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_MODULE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetModuleAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_MODULE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_MODULE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetModuleActualSize(int dwLicKey) // ���ؽ�ѹ������ݴ�С
{
	int ret, addr, size;
	addr = GetModuleAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetModuleData(Char *szDataBuf, int dwSize, int dwLicKey)
	// dwSize-ActualSize
{
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetModuleAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetModuleSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetModuleActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetModuleData(const Char *szDataBuf, int dwSize, int dwLicKey)
	// dwSize-ActualSize
{
	int ret, moveaddr, movesize, cursize, oldsize, size, actualsize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetModuleSize(dwLicKey);
	winaddr = moveaddr = GetWindowAddr(dwLicKey);
	if (moveaddr == -1) {
		funcaddr = moveaddr = GetUserDefFuncAddr(dwLicKey);
		if (moveaddr == -1) {
			rightaddr = moveaddr = GetUserDefRightAddr(dwLicKey);
			if (moveaddr == -1) {
				libaddr = moveaddr = GetLibFileAddr(dwLicKey);
				if (moveaddr == -1) {
					sysaddr = moveaddr = GetSystemRightAddr(dwLicKey);
					if (moveaddr != -1)
						movesize = GetSystemRightSize(dwLicKey);
				}
				else // libfile
				{
					movesize = GetLibFileSize(dwLicKey);
					sysaddr = GetSystemRightAddr(dwLicKey);
					cursize = GetSystemRightSize(dwLicKey);
					if (cursize != -1)
						movesize += cursize;
				}
			}
			else // defright
			{
				movesize = GetUserDefRightSize(dwLicKey);
				libaddr = GetLibFileAddr(dwLicKey);
				cursize = GetLibFileSize(dwLicKey);
				if (cursize != -1)
					movesize += cursize;
				sysaddr = GetSystemRightAddr(dwLicKey);
				cursize = GetSystemRightSize(dwLicKey);
				if (cursize != -1)
					movesize += cursize;
			}
		}
		else // deffunc
		{
			movesize = GetUserDefFuncSize(dwLicKey);
			rightaddr = GetUserDefRightAddr(dwLicKey);
			cursize = GetUserDefRightSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
			libaddr = GetLibFileAddr(dwLicKey);
			cursize = GetLibFileSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
			sysaddr = GetSystemRightAddr(dwLicKey);
			cursize = GetSystemRightSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
		}
	}
	else // window
	{
		movesize = GetWindowSize(dwLicKey);
		funcaddr = GetUserDefFuncAddr(dwLicKey);
		cursize = GetUserDefFuncSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		rightaddr = GetUserDefRightAddr(dwLicKey);
		cursize = GetUserDefRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		libaddr = GetLibFileAddr(dwLicKey);
		cursize = GetLibFileSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		sysaddr = GetSystemRightAddr(dwLicKey);
		cursize = GetSystemRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
	}

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	if ((size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + size, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_MODULE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// Module size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_MODULE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_MODULE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	modaddr = LIC_DATA_ADDR;
	DynamicEncrypt(&modaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	//////////////////////////////////////////////////////////////////
	// Window size,addr
	//////////////////////////////////////////////////////////////////
	if (winaddr != -1) {
		if ((ret = fseek(fp, LIC_WINDOW_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		winaddr = winaddr + diffaddr;
		DynamicEncrypt(&winaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// UserdefFunc size,addr
	//////////////////////////////////////////////////////////////////
	if (funcaddr != -1) {
		if ((ret = fseek(fp, LIC_USERDEFFUNC_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		funcaddr = funcaddr + diffaddr;
		DynamicEncrypt(&funcaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// UserdefRight size,addr
	//////////////////////////////////////////////////////////////////
	if (rightaddr != -1) {
		if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		rightaddr = rightaddr + diffaddr;
		DynamicEncrypt(&rightaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// LibFile size,addr
	//////////////////////////////////////////////////////////////////
	if (libaddr != -1) {
		if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		libaddr = libaddr + diffaddr;
		DynamicEncrypt(&libaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// System size,addr
	//////////////////////////////////////////////////////////////////
	if (sysaddr != -1) {
		if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		sysaddr = sysaddr + diffaddr;
		DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}

	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetWindowSize(int dwLicKey) // ����ѹ��������ݴ�С
{
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_WINDOW_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_WINDOW_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetWindowAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {

		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_WINDOW_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_WINDOW_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetWindowActualSize(int dwLicKey) // ���ؽ�ѹ������ݴ�С
{
	int ret, addr, size;
	addr = GetWindowAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetWindowData(Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetWindowAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetWindowSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetWindowActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetWindowData(const Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, moveaddr, movesize, cursize, oldsize, size, foresize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetWindowSize(dwLicKey);
	funcaddr = moveaddr = GetUserDefFuncAddr(dwLicKey);
	if (moveaddr == -1) {
		rightaddr = moveaddr = GetUserDefRightAddr(dwLicKey);
		if (moveaddr == -1) {
			libaddr = moveaddr = GetLibFileAddr(dwLicKey);
			if (moveaddr == -1) {
				sysaddr = moveaddr = GetSystemRightAddr(dwLicKey);
				if (moveaddr != -1)
					movesize = GetSystemRightSize(dwLicKey);
			}
			else // libfile
			{
				movesize = GetLibFileSize(dwLicKey);
				sysaddr = GetSystemRightAddr(dwLicKey);
				cursize = GetSystemRightSize(dwLicKey);
				if (cursize != -1)
					movesize += cursize;
			}
		}
		else // defright
		{
			movesize = GetUserDefRightSize(dwLicKey);
			libaddr = GetLibFileAddr(dwLicKey);
			cursize = GetLibFileSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
			sysaddr = GetSystemRightAddr(dwLicKey);
			cursize = GetSystemRightSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
		}
	}
	else // deffunc
	{
		movesize = GetUserDefFuncSize(dwLicKey);
		rightaddr = GetUserDefRightAddr(dwLicKey);
		cursize = GetUserDefRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		libaddr = GetLibFileAddr(dwLicKey);
		cursize = GetLibFileSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		sysaddr = GetSystemRightAddr(dwLicKey);
		cursize = GetSystemRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
	}

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	foresize = 0;
	cursize = GetModuleSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	if ((foresize + size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + foresize + size, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR + foresize, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_WINDOW_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// Window size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_WINDOW_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_WINDOW_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	winaddr = LIC_DATA_ADDR + foresize;
	DynamicEncrypt(&winaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	//////////////////////////////////////////////////////////////////
	// UserdefFunc addr
	//////////////////////////////////////////////////////////////////
	if (funcaddr != -1) {
		if ((ret = fseek(fp, LIC_USERDEFFUNC_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		funcaddr = funcaddr + diffaddr;
		DynamicEncrypt(&funcaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// UserdefRight addr
	//////////////////////////////////////////////////////////////////
	if (rightaddr != -1) {
		if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		rightaddr = rightaddr + diffaddr;
		DynamicEncrypt(&rightaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// LibFile addr
	//////////////////////////////////////////////////////////////////
	if (libaddr != -1) {
		if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		libaddr = libaddr + diffaddr;
		DynamicEncrypt(&libaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// System addr
	//////////////////////////////////////////////////////////////////
	if (sysaddr != -1) {
		if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		sysaddr = sysaddr + diffaddr;
		DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}

	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetUserDefRightSize(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_USERDEFRIGHT_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetUserDefRightAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_USERDEFRIGHT_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetUserDefRightActualSize(int dwLicKey) {
	int ret, addr, size;
	addr = GetUserDefRightAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetUserDefRightData(Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetUserDefRightAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetUserDefRightSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetUserDefRightActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetUserDefRightData(const Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, moveaddr, movesize, cursize, oldsize, size, foresize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetUserDefRightSize(dwLicKey);
	libaddr = moveaddr = GetLibFileAddr(dwLicKey);
	if (moveaddr == -1) {
		sysaddr = moveaddr = GetSystemRightAddr(dwLicKey);
		if (moveaddr != -1)
			movesize = GetSystemRightSize(dwLicKey);
	}
	else // libfile
	{
		movesize = GetLibFileSize(dwLicKey);
		sysaddr = GetSystemRightAddr(dwLicKey);
		cursize = GetSystemRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
	}

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	foresize = 0;
	cursize = GetModuleSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetWindowSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetUserDefFuncSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	if ((foresize + size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + foresize + size, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR + foresize, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_USERDEFRIGHT_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// UserdefRight size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	rightaddr = LIC_DATA_ADDR + foresize;
	DynamicEncrypt(&rightaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	//////////////////////////////////////////////////////////////////
	// LibFile addr
	//////////////////////////////////////////////////////////////////
	if (libaddr != -1) {
		if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		libaddr = libaddr + diffaddr;
		DynamicEncrypt(&libaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// System addr
	//////////////////////////////////////////////////////////////////
	if (sysaddr != -1) {
		if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		sysaddr = sysaddr + diffaddr;
		DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}

	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetUserDefFuncSize(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_USERDEFFUNC_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_USERDEFFUNC_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetUserDefFuncAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_USERDEFFUNC_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_USERDEFFUNC_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetUserDefFuncActualSize(int dwLicKey) {
	int ret, addr, size;
	addr = GetUserDefFuncAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetUserDefFuncData(Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetUserDefFuncAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetUserDefFuncSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetUserDefFuncActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetUserDefFuncData(const Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, moveaddr, movesize, cursize, oldsize, size, foresize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetUserDefFuncSize(dwLicKey);
	rightaddr = moveaddr = GetUserDefRightAddr(dwLicKey);
	if (moveaddr == -1) {
		libaddr = moveaddr = GetLibFileAddr(dwLicKey);
		if (moveaddr == -1) {
			sysaddr = moveaddr = GetSystemRightAddr(dwLicKey);
			if (moveaddr != -1)
				movesize = GetSystemRightSize(dwLicKey);
		}
		else // libfile
		{
			movesize = GetLibFileSize(dwLicKey);
			sysaddr = GetSystemRightAddr(dwLicKey);
			cursize = GetSystemRightSize(dwLicKey);
			if (cursize != -1)
				movesize += cursize;
		}
	}
	else // defright
	{
		movesize = GetUserDefRightSize(dwLicKey);
		libaddr = GetLibFileAddr(dwLicKey);
		cursize = GetLibFileSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
		sysaddr = GetSystemRightAddr(dwLicKey);
		cursize = GetSystemRightSize(dwLicKey);
		if (cursize != -1)
			movesize += cursize;
	}

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		// SetLastError(...);
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	foresize = 0;
	cursize = GetModuleSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetWindowSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	if ((foresize + size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			// SetLastError(...);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + foresize + size, SEEK_SET)) != 0) {
			fclose(fp);
			// SetLastError(...);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR + foresize, SEEK_SET)) != 0) {
		fclose(fp);
		// SetLastError(...);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_USERDEFFUNC_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		// SetLastError(...);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// UserdefFunc size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_USERDEFFUNC_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_USERDEFFUNC_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	funcaddr = LIC_DATA_ADDR + foresize;
	DynamicEncrypt(&funcaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	//////////////////////////////////////////////////////////////////
	// UserdefRight addr
	//////////////////////////////////////////////////////////////////
	if (rightaddr != -1) {
		if ((ret = fseek(fp, LIC_USERDEFRIGHT_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		rightaddr = rightaddr + diffaddr;
		DynamicEncrypt(&rightaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// LibFile addr
	//////////////////////////////////////////////////////////////////
	if (libaddr != -1) {
		if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		libaddr = libaddr + diffaddr;
		DynamicEncrypt(&libaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}
	//////////////////////////////////////////////////////////////////
	// System addr
	//////////////////////////////////////////////////////////////////
	if (sysaddr != -1) {
		if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		sysaddr = sysaddr + diffaddr;
		DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}

	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetLibFileSize(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_LIBFILE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_LIBFILE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetLibFileAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_LIBFILE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetLibFileActualSize(int dwLicKey) {
	int ret, addr, size;
	addr = GetLibFileAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetLibFileData(Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetLibFileAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetLibFileSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetLibFileActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetLibFileData(const Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, moveaddr, movesize, cursize, oldsize, size, foresize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetLibFileSize(dwLicKey);
	sysaddr = moveaddr = GetSystemRightAddr(dwLicKey);
	if (moveaddr != -1)
		movesize = GetSystemRightSize(dwLicKey);

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	foresize = 0;
	cursize = GetModuleSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetWindowSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetUserDefFuncSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetUserDefRightSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	if ((foresize + size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + foresize + size, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR + foresize, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_LIBFILE_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// LibFile size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_LIBFILE_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_LIBFILE_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	libaddr = LIC_DATA_ADDR + foresize;
	DynamicEncrypt(&libaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	//////////////////////////////////////////////////////////////////
	// System addr
	//////////////////////////////////////////////////////////////////
	if (sysaddr != -1) {
		if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		sysaddr = sysaddr + diffaddr;
		DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
		fwrite(&dwDesVal, sizeof(int), 1, fp);
	}

	fclose(fp);
	return 0;
}

// ---------------------------------------------------------------------------
int GetSystemRightSize(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_SYSTEM_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_SYSTEM_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetSystemRightAddr(int dwLicKey) {
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, policfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);
	if ((hFind = FindFirstFile(szFileName, &wfd)) == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (hFind)
		FindClose(hFind);
	FILE *fp;
	int dwDesKey, dwDecKey, dwLength, ret;
	Char tag;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, SAFETY_SYSTEM_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&tag, sizeof(Char), 1, fp);
	if (tag != 1) {
		fclose(fp);
		return -1;
	}
	if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&dwDesKey, sizeof(int), 1, fp);
	fclose(fp);
	if (DynamicDecrypt(&dwDesKey, &dwDecKey, sizeof(int), dwLicKey))
		return dwDecKey;
	else
		return -1;
}

// ---------------------------------------------------------------------------
int GetSystemRightActualSize(int dwLicKey) {
	int ret, addr, size;
	addr = GetSystemRightAddr(dwLicKey);
	if (addr == -1)
		return addr;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fread(&size, sizeof(int), 1, fp);
	fclose(fp);
	return size;
}

// ---------------------------------------------------------------------------
int GetSystemRightData(Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, addr, size, actualsize;
	Char *buf;
	addr = GetSystemRightAddr(dwLicKey);
	if (addr == -1)
		return addr;
	size = GetSystemRightSize(dwLicKey);
	if (size == -1)
		return size;
	actualsize = GetSystemRightActualSize(dwLicKey);
	if (actualsize == -1)
		return actualsize;
	if (actualsize > dwSize)
		return -1;

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	if ((ret = fseek(fp, addr, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	buf = new Char[size];
	fread(buf, size, 1, fp);
	fclose(fp);
	TDecompress *decpress = new TDecompress(buf, size);
	decpress->Read(szDataBuf, actualsize);
	delete decpress;
	delete[]buf;
	return actualsize;
}

// ---------------------------------------------------------------------------
int SetSystemRightData(const Char *szDataBuf, int dwSize, int dwLicKey) {
	int ret, moveaddr, movesize, cursize, oldsize, size, foresize;
	int modaddr, winaddr, funcaddr, rightaddr, libaddr, sysaddr, diffaddr;
	Char *buf, *movebuf, tag;
	movesize = 0;
	moveaddr = -1;
	oldsize = GetLibFileSize(dwLicKey);

	Char szFilePath[255];
	Char szFileName[255];
	int nLeft;
	nLeft = GetModuleFileName(NULL, szFilePath, 255);
	while (nLeft > 0) {
		if (szFilePath[nLeft - 1] == '\\') {
			szFilePath[nLeft] = '\0';
			break;
		}
		nLeft--;
	}
	swprintf(szFileName, L"%s%s", szFilePath, licresfilename);

	FILE *fp;
	if ((fp = _wfopen(szFileName, L"r+b")) == NULL) {
		return -1;
	}
	TCompress *press = new TCompress(szDataBuf, dwSize);
	size = press->Size();
	buf = new Char[press->Size()];
	press->Read(buf, press->Size());
	delete press;
	foresize = 0;
	cursize = GetModuleSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetWindowSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetUserDefFuncSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetUserDefRightSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	cursize = GetLibFileSize(dwLicKey);
	if (cursize != -1)
		foresize += cursize;
	if ((foresize + size + LIC_DATA_ADDR != moveaddr) && (moveaddr != -1)) {
		if ((ret = fseek(fp, moveaddr, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		movebuf = new Char[movesize];
		fread(movebuf, movesize, 1, fp);
		if ((ret = fseek(fp, LIC_DATA_ADDR + foresize + size, SEEK_SET)) != 0) {
			fclose(fp);
			return -1;
		}
		fwrite(movebuf, movesize, 1, fp);
		delete movebuf;
	}
	if ((ret = fseek(fp, LIC_DATA_ADDR + foresize, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(buf, size, 1, fp);
	delete buf;
	tag = 1;
	if ((ret = fseek(fp, SAFETY_SYSTEM_TAG, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	fwrite(&tag, sizeof(Char), 1, fp);

	if (oldsize > -1)
		diffaddr = size - oldsize;
	else
		diffaddr = size;

	int dwDesVal, dwDecVal;
	//////////////////////////////////////////////////////////////////
	// System size,addr
	//////////////////////////////////////////////////////////////////
	if ((ret = fseek(fp, LIC_SYSTEM_SIZE, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	DynamicEncrypt(&size, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);
	if ((ret = fseek(fp, LIC_SYSTEM_ADDR, SEEK_SET)) != 0) {
		fclose(fp);
		return -1;
	}
	sysaddr = LIC_DATA_ADDR + foresize;
	DynamicEncrypt(&sysaddr, &dwDesVal, sizeof(int), dwLicKey);
	fwrite(&dwDesVal, sizeof(int), 1, fp);

	fclose(fp);
	return 0;
}
// ---------------------------------------------------------------------------
