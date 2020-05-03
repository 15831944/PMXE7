//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "DataSnapStream.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TDataSnapStream::TDataSnapStream()
 :TMemoryStream()
{
}
//---------------------------------------------------------------------------
__fastcall TDataSnapStream::TDataSnapStream(TStream* AStream)
 :TMemoryStream()
{
	LoadFromStream(AStream);
}
//---------------------------------------------------------------------------
__fastcall TDataSnapStream::~TDataSnapStream()
{
}
//---------------------------------------------------------------------------
void __fastcall TDataSnapStream::LoadFromStream(TStream* AStream)
{
	Position = 0;
	Size = 0;
	AStream->Position = 0;
	if(AStream->Size==-1)  //不知道大小
	{
		const int BufSize=0xF000;
		void *Buffer;
		int ReadCount=0;
		Buffer = AllocMem(NativeInt(BufSize));
		while(true)
		{
			ReadCount = AStream->Read(Buffer, BufSize);

			if(ReadCount > 0)
				WriteBuffer(Buffer, ReadCount);

			if(ReadCount < BufSize)
				break;
		}
		FreeMemory(Buffer);
	}
	else
	{
		CopyFrom(AStream, 0);
	}
	Position = 0;
}
//---------------------------------------------------------------------------
