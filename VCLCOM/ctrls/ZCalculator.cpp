//---------------------------------------------------------------------------
/*
���¸���
2004-07-5
3�������˺�������������������©�����磺SZQS(var1,var2)100�ǲ������﷨��
2���滻�����ں� �����÷�����������ʽ���߽����߼��㣬������Ĳ�������Ϊ���ʽ
1���������� int MaxIteration ����������ƣ�Ĭ��50�������������ʽ�д�
2004-4-15
TsdCalculate��������Ӧ��
���ԣ�
 1--UserFunctionName  �û��Զ��庯������Ĭ��SZQS���ѷ������������������Զ��庯��������
 2--MaxVarNum  �û��Զ��庯������������,Ĭ��3(�ѷ�������������������)
�¼���
 1--OnFunction ����������ʱ�����¼���(ϵͳ��������CELLS(int col,row)��ZB(...)��SZQS(...));
     �����¼�����ʱ�������غ������������б�ͺ����ķ���ֵ��ַ���û���Ҫʵ�ָ��Ե��ⲿ������
 2--OnVar  �����������Ǵ����¼�
    �����¼�����ʱ�������ر������ͷ���ֵ��ַ
������
	void __fastcall SetVarValue(String VarName, Variant Value);//ϵͳ������������ֵ
	�û�������ϵͳ�����Զ������
	Variant __fastcall GetVarValue(String VarName); //�������
	ȡ�û��Զ��������ֵ������Ҳ������򴥷�OnVar�¼�
	Variant __fastcall GetFunctionValue(String FuncName,Variant Args); //������ֵ
        ȡ��������ֵ��������OnFunction�¼�
        int __fastcall AddExtFunction(String FunctName);   //�����Զ��庯��,���ز���λ�ã�
                                                                //���������б��н��Դ�д�洢
        ����Ա���Ե��ø÷��������Լ��Զ��庯������Ȼ����ԱҪ�Լ���OnFunction�м�Ⲣʵ���Զ���
        �����ķ���ֵ
        void __fastcall DeleteExtFunction(String FunctName);//ɾ���Զ��庯��
        void __fastcall ClearExtFunction(); //����Զ��庯���б�(Ϊ�˼�����ǰ�汾���������ᱣ����������SZQS��ZB��CELLS��
        int __fastcall IsExtFunction(String FunctName);//�Զ��庯�������ı�־ֵ���б��е�λ��Index��
        ���б�������SZQS(Index=0),CELLS(Index=1),ZB(Index=2)

*/
#include <vcl.h>
#pragma hdrstop

#include "ZCalculator.h"
#include "Lexer.h"
#include "CPLexer.h"
#include "tokstr.h"
#include "MathParser.h"
#pragma package(smart_init)
#define OPND 'D'
#define OPTR 'R'
#define  CONTINUE_READ_NEXT_NOTATION 'C'
#define  STOP_READ_NEXT_NOTATION     'S'
//ע�⣺����'����'�в��ܰ���'+-*/()[]#=%'
//�������ֻ��Ϊ'+-*/()%'
#define MAX_EXP_LEN       65535
#define	MAX_NOTATION_NUM  1000	/* һ�����ʽ�е����������� */
#define MAX_NOTATION_LEN  30		/* ���ʽ��һ�����������ִ����� */
typedef struct  VarStack {					/* ���ʽ�����ջ */
	Variant OpdStack[MAX_NOTATION_NUM];	/* ��������ջ */
	char 	OpStack[MAX_NOTATION_NUM];	/* �������ջ */
	int     OpdStackTop,		/* ��������ջ��ָ�� */
        	OpStackTop; 		/* �������ջ��ָ�� */
} ExpressionCalculateStack;
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TZCalculator *)
{
        new TZCalculator(NULL);
}
//---------------------------------------------------------------------------
__fastcall TZCalculator::TZCalculator(TComponent* Owner)
        : TComponent(Owner)
{
    FVarLists.clear();
    FMaxVarNum=3;
    FUserFunctionName="szqs";
    FMaxIteration=50;
    FIterationCycle=0;
    ExtFunctionList=new TStringList();
    ClearExtFunction();
}
//---------------------------------------------------------------------------
namespace Zcalculator
{
        void __fastcall PACKAGE Register()
        {
                 TComponentClass classes[1] = {__classid(TZCalculator)};
                 RegisterComponents("ZAdditional", classes, 0);
        }
}
//---------------------------------------------------------------------------
Variant __fastcall TZCalculator::GetVarValue(String VarName)
{
  Variant Result=NULL;
  TVarLists::iterator p=FVarLists.find(VarName.UpperCase());
 if (p != FVarLists.end())
    Result= p->second;
  else if(FOnVar)
  {
      FIterationCycle++;
      if(FIterationCycle>FMaxIteration)
        {
          FIterationCycle=0;
           throw Exception("������ʽ�ѳ���ϵͳ������ĵݹ����"+IntToStr(FMaxIteration)+"�㣬���ܹ�ʽ����");
        }

      FOnVar(this,VarName,Result);

      FIterationCycle--;
  }
  return Result;
}
//---------------------------------------------------------------------------
Variant __fastcall TZCalculator::GetFunctionValue(String FuncName,Variant Args)
{
  Variant Result=NULL;
  if(FuncName=="IF")
  {
    int ivarCount=0;
    if(!Args.IsEmpty())
    {
       ivarCount=Args.ArrayHighBound()-Args.ArrayLowBound()+1;//���ر�������
    }
    if(ivarCount!=3)
      throw Exception("IF���ʽ������3������");
    bool bvar = Args.GetElement(0);
    String svar = "";
    if(bvar)
    {
      svar=Args.GetElement(1);
      Result = Args.GetElement(1);
    }
    else
    {
      svar=Args.GetElement(2);
      Result = Args.GetElement(2);
    }
    if(svar.Trim().IsEmpty())
      Result = 0;
  }
  else
  {
    if(FOnFunction)
    {
      FIterationCycle++;
      if(FIterationCycle>FMaxIteration)
        {
          FIterationCycle=0;
           throw Exception("������ʽ�ѳ���ϵͳ������ĵݹ����"+IntToStr(FMaxIteration)+"�㣬���ܹ�ʽ����");
        }

      FOnFunction(this,FuncName,Args,Result);

      FIterationCycle--;
    }
  }
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TZCalculator::SetVarValue(String VarName, Variant Value)
{
  TVarLists::iterator p=FVarLists.find(VarName.UpperCase());
  if (p != FVarLists.end())
      p->second=Value;
   else
     FVarLists.insert(TVarLists::value_type(VarName.UpperCase().c_str(),Value));
}
//---------------------------------------------------------------------------
void __fastcall TZCalculator::SetMaxVarNum(int value)
{
        if(FMaxVarNum != value) {
         if(value<0)
            FMaxVarNum=0;
          else
           FMaxVarNum = value;
        }
}
int __fastcall TZCalculator::AddExtFunction(String FunctName)
{
        int findex=IsExtFunction(FunctName);
        if(findex==-1)
          return ExtFunctionList->Add(FunctName.Trim().UpperCase());
        else
          return findex;
}

void __fastcall TZCalculator::DeleteExtFunction(String FunctName)
{
        int findex=IsExtFunction(FunctName);
        if(findex>-1)
           ExtFunctionList->Delete(findex);
}

void __fastcall TZCalculator::ClearExtFunction()
{
   ExtFunctionList->Clear();
   ExtFunctionList->Add("SZQS");
   ExtFunctionList->Add("CELLS");
   ExtFunctionList->Add("ZB");
   ExtFunctionList->Add("SUM");   
   ExtFunctionList->Add("IF");   
}

int __fastcall TZCalculator::IsExtFunction(String FunctName)
{
       return ExtFunctionList->IndexOf(FunctName.Trim().UpperCase());
}

__fastcall TZCalculator::~TZCalculator()
{
    if(ExtFunctionList)
    {
       ExtFunctionList->Clear();
       delete ExtFunctionList;
    }
}
//---------------------------------------------------------------------------

char  opstr[] = "+-*/()#"; //��������� ��ʽ��'%'='/100'���д���
int  priortable[7][7] =    //���ȼ�
  /*  +  -   *   /   (  )  #  */
  {1, 1, -1, -1, -1, 1, 1,
   1, 1, -1, -1, -1, 1, 1,
   1, 1, 1, 1, -1, 1, 1,
   1, 1, 1, 1, -1, 1, 1,
   -1, -1, -1, -1, -1, 0, 'I',
   1, 1, 1, 1, 'I', 1, 1,
   -1, -1, -1, -1, -1, 'I', 0,
  };
//---------------------------------------------------------------------------
char __fastcall GetTypeOfNotation (char *notation)
{
  int i;

  if (notation[0] == '\0')
    return NULL;

  for (i = 0; opstr[i] != '\0'; i++)
    if (notation[0] == opstr[i])
      return OPTR;

  return OPND;
}
//---------------------------------------------------------------------------
int __fastcall CompareOpPrior (char op2, char op1)
{
  char  i, j;
  for (i = 0; opstr[i] != '\0'; i++)
    if (op1 == opstr[i])
      break;

  for (j = 0; opstr[j] != '\0'; j++)
    if (op2 == opstr[j])
      break;

  return priortable[i][j];
}
//---------------------------------------------------------------------------
void __fastcall push (char type, char op, Variant opnd,
		  ExpressionCalculateStack * s)
{

  if (type == OPND)
    s->OpdStack[s->OpdStackTop++] = opnd;
  else
    s->OpStack[s->OpStackTop++] = op;
}
//---------------------------------------------------------------------------
double __fastcall pop (char type, ExpressionCalculateStack * s,TZCalculator *FCalculate)
{
  if (type == OPND)
    {
      bool  bIsDigit = true;
      String   VarName;
      if(s->OpdStackTop==0)//"ջ�ѿ�"
         throw Exception("��ʽ���Ϸ�");
      String retVal= s->OpdStack[--s->OpdStackTop];
      for (int i = 1; i <= retVal.Length (); i++)
	{
	  if (!(isdigit (retVal[i])||retVal[i]=='.'||retVal[i]=='-')) //zy.2003.06.17����жϡ�||retVal[i]=='-'�������ܴ�������
	    {
	      bIsDigit = false;
	      break;
	    }
	}
      if (bIsDigit)
	return retVal.ToDouble ();
       else
	{
          if((retVal[1]=='\"')||retVal[retVal.Length()]=='\"')
             throw Exception("��ʱ�޷������ַ�������:"+retVal);
          else
          {
           return   (double)FCalculate->GetVarValue(retVal);
          }
	}
    }
  else
    return (double) (s->OpStack[--s->OpStackTop]);
}
//---------------------------------------------------------------------------
double __fastcall calculate (double opnd1, char op, double opnd2)
{

  switch (op)
    {
    case '+':
      return opnd1 + opnd2;
    case '-':
      return opnd1 - opnd2;
    case '*':
      return opnd1 * opnd2;
    case '/':
      if (opnd2 != 0.0)
	return opnd1 / opnd2;
      else
	throw Exception("���ֳ������");
    }
  return 0.0;
}
//---------------------------------------------------------------------------
void __fastcall GetNextNotation (char *notation, char *exp,int &index)
{
  int  j, type, lasttype;

  lasttype = GetTypeOfNotation (exp + index);
  for (j = 0; exp[index] != '\0'; j++, index++)
    {
//     if(j>18)
//      throw Exception("����������ϵͳ����ĳ���18λ");
      if (exp[index] == ' ')
	continue;
      notation[j] = exp[index];
      type = GetTypeOfNotation (notation + j);
      if (type == OPTR && lasttype == OPTR)
	{
	  index++, j++;
	  break;
	}
      if (type == OPTR && lasttype == OPND)
	break;

      lasttype = type;
    }

  notation[j] = NULL;
  if (notation[0] == '#') // ����������index=0;
    index = 0;
}
//---------------------------------------------------------------------------

Variant __fastcall CalculateFmla(TZCalculator *FCalculate,String strFormula)
{
//     String myexp=strFormula.UpperCase();
	 AnsiString myexp=strFormula;
     if(myexp.Trim().IsEmpty()||myexp[1]!='=') //��ʽ�Ƿ�
       return 0.0;
     if(myexp.Length()<=1)
          throw Exception("���ʽ�Ƿ�,ֻ��һ��'='");
       //����ٷֺ�%
     TReplaceFlags Flags;
     Flags<<rfReplaceAll<<rfIgnoreCase;
     myexp=myexp.SubString(2,myexp.Length()-1);//ȥ��'='
     myexp=StringReplace(myexp,"%","/100",Flags);
     //ȥ���ո�
	  AnsiString newexp="";
      for(int l=1;l<=myexp.Length();l++)
	  {
         if(myexp[l]!=' '&&myexp[l]!='\n'&&myexp[l]!='\r')
            newexp+=myexp[l];
      }
     Variant Result;
	 TMathParser mp(FCalculate);
	 char *ErrMsg = mp.Parse( newexp.c_str(), &Result );
     if(ErrMsg!=NULL)
        throw Exception(ErrMsg);
	 return Result;
}

String __fastcall RepFmlaWithPeriod(String strFormula,String ReseWord,String iPeriod)
{
        String Formula=strFormula;
		if(Formula.Trim().IsEmpty())
           return Formula;
		String newexp="";
        for(int l=1;l<=Formula.Length();l++)
        {
           if(strFormula[l]!=' '&&strFormula[l]!='\n'&&Formula[l]!='\r')
              newexp+=Formula[l];
        }
        Formula=newexp;
		TToken tok;
        TTokenString tokenString;
        TStringIStream *StrStream=new TStringIStream(newexp);
        TCPPScan Scan (StrStream,tokenString);
        newexp="";
         while (true)
          {
             tok = Scan.NextToken ();
              if (tok.type == TOK_EOF) break;
              if(tok.type==TOK_ID)
              {
                if(UpperCase(tok.text)==ReseWord.Trim().UpperCase())
                    tok.text=iPeriod;
              }
               newexp+=tok.text;
          }
        Formula=newexp;
        return Formula;                      
}


