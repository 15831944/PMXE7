//---------------------------------------------------------------------------

#pragma hdrstop

#include "UnicodeAndStdstring.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
UnicodeString stdStringToUnicodeString(string StrSource)
{
	AnsiString Ansi_result;
	Ansi_result.SetLength(StrSource.length());
	memcpy(Ansi_result.c_str(),StrSource.data(),StrSource.length());
	return UnicodeString(Ansi_result);
}
string UnicodeStringTostdString(UnicodeString StrSource)
{
	 AnsiString FileName=StrSource;
	 return string(FileName.c_str());
}
