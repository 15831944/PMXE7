// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NetResource.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TZNetResource::TZNetResource() {
	dwThreadId = 0;
	hThread = NULL;
	m_Handle = AllocateHWnd(WndProc);
	FResourceBuffer = new TStringList;
}

// ---------------------------------------------------------------------------
__fastcall TZNetResource::~TZNetResource() {
	if (hThread) {
		TerminateThread(hThread, NULL);
		hThread = NULL;
		dwThreadId = 0;
	}
	if (m_Handle)
		DeallocateHWnd(m_Handle);
	delete FResourceBuffer;
}

// ---------------------------------------------------------------------------
DWORD WINAPI ThreadProc(void *lpParam) {
	((TZNetResource*)lpParam)->EnumNetRes(NULL);
	SendMessage(((TZNetResource*)lpParam)->m_Handle, WM_USER + 0x100, 0, 0);
	((TZNetResource*)lpParam)->hThread = NULL;
	((TZNetResource*)lpParam)->dwThreadId = 0;
	return 0;
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::WndProc(TMessage &msg) {
	switch (msg.Msg) {
	case WM_USER + 0x100:
		if (FOnEnumComplete)
			FOnEnumComplete(NULL);
		break;
	default:
		break;
	}
	msg.Result = DefWindowProc(m_Handle, msg.Msg, msg.WParam, msg.LParam);
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::EnumNetRes(NETRESOURCE* nr) {
	HANDLE hEnum;
	if (WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY,
		RESOURCEUSAGE_CONTAINER // |RESOURCEUSAGE_CONNECTABLE
		, nr, &hEnum) != NO_ERROR) {
		// 出错表明拒绝访问
		// 如果不想继续遍历， 可以扔出一个意外
		// RaiseLastWin32Error();
		return;
	}
	// 遍历所有的资源
	DWORD Count = -1;
	DWORD BufferSize = 0;
	DWORD ReturnVal = 0;
	// 为 NetResource 数组分配空间
	NETRESOURCE *NetRes = (NETRESOURCE*)new Char[1024];
	ZeroMemory(NetRes, sizeof(NETRESOURCE));
	for (; ;) {
		if (FTerminated)
			break;
		ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
		if (ReturnVal == ERROR_MORE_DATA) // 由我们设置 NetRes 的大小
		{
			// 但如果数据量超出了我们设置的大小
			Count = -1;
			delete[]NetRes;
			NetRes = (NETRESOURCE*)new Char[BufferSize];
			ZeroMemory(NetRes, sizeof(NETRESOURCE));
			ReturnVal = WNetEnumResource(hEnum, &Count, NetRes, &BufferSize);
		}
		if (ReturnVal != NO_ERROR)
			break;
		if (ReturnVal == 0 && Count == 0) {
			delete[]NetRes;
			WNetCloseEnum(hEnum);
			break;
		}
		// 加到 ListBox
		for (unsigned int i = 0; i < Count; i++) {
			if (NetRes[i].dwType == RESOURCETYPE_ANY && NetRes[i]
				.dwDisplayType == 2 && NetRes[i]
				.dwUsage == RESOURCEUSAGE_CONTAINER)
				FResourceBuffer->Add(String(NetRes[i].lpRemoteName).SubString0(2,
				StrLen(NetRes[i].lpRemoteName)));
			if (FTerminated)
				break;
			if ((NetRes[i].dwUsage & RESOURCEUSAGE_CONTAINER)
				== RESOURCEUSAGE_CONTAINER)
				EnumNetRes(&NetRes[i]);
		}
		if (ReturnVal == ERROR_NO_MORE_ITEMS) {
			// 遍历完成
			delete[]NetRes;
			WNetCloseEnum(hEnum);
		}
	}
	delete[]NetRes;
	WNetCloseEnum(hEnum);
}

// ---------------------------------------------------------------------------
int __fastcall TZNetResource::GetCount() {
	return FResourceBuffer->Count;
}

// ---------------------------------------------------------------------------
String __fastcall TZNetResource::GetNetResName(int index) {
	return FResourceBuffer->Strings[index];
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::EnumNetResource() {
	FResourceBuffer->Clear();
	EnumNetRes(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::AsynEnumNetResource() {
	FResourceBuffer->Clear();
	FTerminated = false;
	hThread = CreateThread(NULL, 0, &ThreadProc, (void*)this, 0, &dwThreadId);
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::Terminate() {
	FTerminated = true;
}

// ---------------------------------------------------------------------------
void __fastcall TZNetResource::KillThread() {
	if (hThread)
		TerminateThread(hThread, NULL);
}
// ---------------------------------------------------------------------------
