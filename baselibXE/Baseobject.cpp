//---------------------------------------------------------------------------
//#include <fmx.h>
#pragma hdrstop

#include "Baseobject.h"

//---------------------------------------------------------------------------
//#pragma startup RegisterClass(TBaseObject);
#pragma package(smart_init)
__fastcall  TBaseObject::TBaseObject()
   :TPersistent()
{

}
//---------------------------------------------------------------------------
void __fastcall TBaseObject::RegisterConverter(TJSONMarshal *m)
{

}
//---------------------------------------------------------------------------
void __fastcall TBaseObject::RegisterReverter(TJSONUnMarshal *unm)
{

}
//---------------------------------------------------------------------------

