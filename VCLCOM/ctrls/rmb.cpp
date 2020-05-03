
#include <vcl.h>        // must define AnsiString before rmb.h
#pragma hdrstop         // only for C++ Builder

#include <math.h>
#include "rmb.h"

//---------------------------------------------------------------------------


// �����ַ������壬���޸�Ҳ�ɱ�Ϊת����ͨ���ֺ������磺һ������ʮ��ǧ��
const char *HanDigiStr[] = {"��","Ҽ","��","��","��","��","½","��","��","��"};

const char *HanDiviStr[] = { "","ʰ","��","Ǫ","��","ʰ","��","Ǫ","��",
				"ʰ","��","Ǫ","��","ʰ","��","Ǫ","��",
				"ʰ","��","Ǫ","��","ʰ","��","Ǫ" };

String PositiveIntegerToHanStr(const String &NumStr)
{					// �����ַ���������������ֻ����ǰ���ո�(�����Ҷ���)��������ǰ����
	String RMBStr;
	int len,n,lastzero=0;
	int hasvalue=0;			// �ڡ����λǰ����ֵ���

	len = NumStr.Length();
	if( len > 24 ) return "��ֵ����!";

	for(int i=len-1;i>=0;i--) {
		if( NumStr[len-i]==' ' ) continue;      // String[] base is 1
		n = NumStr[len-i] - '0';
		if( n<0 || n>9 ) return "���뺬�������ַ�!";

		if( n!=0 ) {
			if( lastzero ) RMBStr += HanDigiStr[0];		// ���������������ֵ��ֻ��ʾһ����
                                                                        // ��������ǰ���㲻��������
//			if( !( n==1 && (i%4)==1 && (lastzero || i==len-1) ) )   // ��ʮ��λǰ����Ҳ����Ҽ���ô���
//			if( !( n==1 && (i%4)==1 && i==len-1 ) )         // ʮ��λ���ڵ�һλ����Ҽ��
				RMBStr += HanDigiStr[n];
			RMBStr += HanDiviStr[i];			// ����ֵ��ӽ�λ����λΪ��
			hasvalue = 1;                                   // �����λǰ��ֵ���
		 }
		else {
			if( (i%8)==0 || ((i%8)==4 && hasvalue) ) 	// ����֮������з���ֵ����ʾ��
				RMBStr += HanDiviStr[i];		// ���ڡ�����
		 }
                if( i%8==0 ) hasvalue = 0;  			// ���λǰ��ֵ��Ƿ��ڸ�λ
		lastzero = (n==0) && (i%4!=0);                  // ����ǰ����󲻼��㣬�磺ʰ��Ǫ
	 }

	if( RMBStr.Length()==0 ) return HanDigiStr[0];	        // ������ַ���"0"������"��"
	return RMBStr;
}

String NumToRMBStr(double val)
{
	String SignStr,TailStr;
	double fraction, integer;
	int jiao,fen;

	if( val<0 ) {
		val = -val;
		SignStr = "��";
	 }
	fraction = modf(val+0.005, &integer);           // �������뵽��

	jiao = int(fraction*10);
	fen = int(fraction*100)-jiao*10;
	if( jiao==0 && fen==0 ) {
		TailStr = "��";
	 }
	else {
		TailStr = HanDigiStr[jiao];
		if( jiao!=0 )
			TailStr += "��";
                if( integer==0 && jiao==0 )             // ��Ԫ��д�㼸��
                        TailStr = "";
		if( fen!=0 )
			TailStr += HanDigiStr[fen] + String("��");
	 }

// ��һ�п����ڷ�������ڳ��ϣ�0.03ֻ��ʾ�����֡������ǡ���Ԫ���֡�
//        if( integer==0 ) return  SignStr+TailStr;

	return SignStr+PositiveIntegerToHanStr( String(integer) )+"Ԫ"+TailStr;
}

//-----------------------------------------------------------

// ��������
// ����Ҵ�д��λ���й涨�á�Ԫ��
// ����ͷ����������������򲻸�
// ��Ϊ��ʱ��д�ǣ��磺�����֣�
// �������뵽��
// Ϊ�����ж��ɻ�һ��Դ�д���Ԥ�ڽϸߣ���
// ��������һ�����λΪԪ��������ҼԲǰ�Լӡ���Ԫ��

// ����ת��
// ���������������ֵ��ֻ��ʾһ���㣬������ʾ
// ��(��)ǰ����󲻼��㣬���ڡ���Ϊһ������λ��
// ���磺ʰ��Ǫ �� ʰ���㷡Ǫ ��˳Щ��
// ��Ϊ����������λ��ֻҪ��λ��������ʾ���磺Ҽ���ڣ�
// ��Ϊ�����λ������֮������з���ֵ����ʾ��
// ���硰Ҽ�ڡ�������ʾΪ��Ҽ���򡱣�
//
// Ϊ���ٱ��ܸĵĿ����ԣ�ʮ��λ�ܷ�Ҽ������������ϰ�߶�����һ��
//      ��ʮ��λ���ڵ�һλ����Ҽ�����硰ʰԪ���ǡ�ҼʰԪ����
//        ʮ��λ��ǰ�����Ƿ񲻷�Ҽ����̫ȷ����
//        �硰��Ǫ��ҼʰԪ�����ǡ���Ǫ��ʰԪ������
// �á�ʰ�򡱲��á�Ҽʰ�򡱣���Ϊÿ��������λ���н�λ��λ��ʰ��Ǫ���ڣ�
// ������ʹ���ǰû�и����ܸĵ�ǰ׺�硰����ҡ�����Ҳ�Ѵܸ�Щ
// ��Ϊ����Ҫ�����������ֲ��ҸĶ������ֱ����λ���ܴܸĳ�
// ���硰ʰ�򡱿ɸĳɡ���ʰ�򡱣�����Ҽʰ������Ҫ�ĳɡ�Ҽ��Ҽʰ�򡱣�


// �˺���ΪBorland C++ Builder��д���ѳ�ֿ��ǵ���ƽ̨������
// ����ת�����������ϣ�
// ֻ���޸��ַ������ʹ�AnsiString����Ӧ�����͡����ܼ�����
// ��ע������char*���ַ���������ֱ����ӣ�����һ����ת����

// public domain freeware by ������ 2003��3��-5�� v1.1
// �˺������в���֮����email: wang_daqing@21cn.com
