//---------------------------------------------------------------------------

#ifndef CommFuncH
#define CommFuncH
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
enum TfstFloatType: unsigned char{ftQty=1,ftPrice=2,ftAmt=3,ftRate=4,ftsArea=5,ftsPrice=6,ftsAmt=7,ftsCJPrice=8,ftsCJTotalPrice=9}; //浮点数据类型，数量、单价、金额、汇率
//--------------------------------------------------------------------------
// 函数说明 获取用户相关业务的数据权限过滤字符串
// 参数说明
// Query Query指针
// businessType  1-项目,2-合同,3-文档,4-合同目录,5-文档目录,6-招标项目,7-核算项目
// userGUID 用户编码
// return value ：filter string
//--------------------------------------------------------------------------
String __fastcall GetGUIDString();
String __fastcall myFloatToStr(double data);
//自定义的浮点转化为字符串函数，避免科学计数法
double __fastcall ConvertFloat(double data,TfstFloatType iType=ftQty,int iPrecision=0,int iStyle=-1);
//格式化浮点数据，参数data-浮点数；iType-数据类型，ftQty:数量，ftPrice:单价，ftAmt:金额,ftRate:汇率系数；
//iStyle-小数截取方式，0:四舍五入，1:截断，-1:取系统方式；iPrecision-小数精度，-1:取系统精度
String __fastcall FormatFloatToStr(double data,TfstFloatType iType=ftQty,int iPrecision=0,int iStyle=-1);
//格式化浮点数据以字符串输出，参数data-浮点数；iType-数据类型，ftQty:数量，ftPrice:单价，ftAmt:金额,ftRate:汇率系数；
//iStyle-小数截取方式，0:四舍五入，1:截断，-1:取系统方式；iPrecision-小数精度，-1:取系统精度
double __fastcall GetFloatFromStr(String str); //从字符串中读取浮点数
String __fastcall EncodeJsonDate(String date);//解析json格式的datetime类型
#endif
