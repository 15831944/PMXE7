//---------------------------------------------------------------------------

#ifndef DataSnapClientDataSetH
#define DataSnapClientDataSetH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
//---------------------------------------------------------------------------
class PACKAGE TDataSnapClientDataSet : public TClientDataSet
{
private:
   String FSort;
   void __fastcall SetSort(String Value);
protected:
  //	virtual void __fastcall GetDetailLinkFields(System::Generics::Collections::TList__1<TField*> * MasterFields, System::Generics::Collections::TList__1<TField*> * DetailFields)/* overload */;
public:
	__fastcall TDataSnapClientDataSet(TComponent* Owner);
	__fastcall virtual ~TDataSnapClientDataSet();
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	__property String Sort = {read=FSort, write = SetSort};
};
//---------------------------------------------------------------------------
#endif
