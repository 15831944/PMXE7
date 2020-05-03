//---------------------------------------------------------------------------

#ifndef ZCalculatorH
#define ZCalculatorH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <map>
#include <vector>
using namespace std;
typedef map<String,Variant,less<String> > TVarLists;//ϵͳ����������ֵ��¼��
typedef void __fastcall (__closure *TFunctionEvent)(TObject* Sender, String FuncName,Variant &Args, Variant &Result);
typedef void __fastcall (__closure *TVariantEvent)(TObject* Sender, String VarName, Variant &Result);
//---------------------------------------------------------------------------
typedef struct FormulaItem  //�ڵ�
{
  bool  bIsCac;       //�Ѽ������ʾ
  bool  bIsFormula;   //�Ƿ��Ǽ��㹫ʽ
  Variant varValue;     // ����ֵ
  String strFormual; //���㹫ʽ
}TFormulaItem;
typedef vector<TFormulaItem*> TFormulaItems; //��̬����
typedef vector<TFormulaItems*> TFormulaItemArry; //��̬����
//TFormulaItemArry[row][col]-->TFormulaItem;
//---------------------------------------------------------------------------
class PACKAGE TZCalculator : public TComponent
{
private:
        TVarLists FVarLists;
        TFunctionEvent FOnFunction;
        TVariantEvent FOnVar;
        String FUserFunctionName;
        int FMaxVarNum;
        int FMaxIteration;
        int FIterationCycle;
        TStringList *ExtFunctionList;
        void __fastcall SetMaxVarNum(int value);
protected:
public:
        __fastcall TZCalculator(TComponent* Owner);
        __fastcall virtual ~TZCalculator();
        Variant __fastcall GetVarValue(String VarName); //��ȡĳ������ֵ
        Variant __fastcall GetFunctionValue(String FuncName,Variant Args); //��ȡĳ������ֵ
        void __fastcall SetVarValue(String VarName, Variant Value);  //����ϵͳ����������ֵ
        int __fastcall AddExtFunction(String FunctName);       //�����û��Զ��庯��
        void __fastcall DeleteExtFunction(String FunctName);   //ɾ���û��Զ��庯��
        void __fastcall ClearExtFunction();                        //��������Զ��庯��
        int __fastcall IsExtFunction(String FunctName);       //�����Զ��庯�����б��е�λ��
__published:
        __property TFunctionEvent OnFunction  = { read=FOnFunction, write=FOnFunction };//��������ʱ����
        __property TVariantEvent OnVar  = { read=FOnVar, write=FOnVar };//��������ʱ����
        __property String UserFunctionName  = { read=FUserFunctionName, write=FUserFunctionName }; //�Զ��庯��
        __property int MaxVarNum  = { read=FMaxVarNum, write=SetMaxVarNum }; //����������
        __property int MaxIteration  = { read=FMaxIteration, write=FMaxIteration }; //�������㼶,��������
};
Variant __fastcall PACKAGE CalculateFmla(TZCalculator *FCalculate,String strFormula); //������ʽ������ֵ
String __fastcall PACKAGE RepFmlaWithPeriod(String strFormula,String ReseWord,String iPeriod);//�����滻���ַ���
//---------------------------------------------------------------------------
#endif

