#ifndef SafeDataDefH
#define SafeDataDefH

#define SAFETY_FILE_DESC    1024      /*��ȫ�ļ�˵��*/
#define SAFETY_FILE_TYPE    1056      /*��ȫ�ļ�����*/
#define SAFETY_FILE_INIT    1060      /*��ȫ�ļ���ʼ����ʶ*/
#define SAFETY_MODULE_TAG   1064      /*ģ�����ݳ�ʼ����ʶ*/
#define SAFETY_WINDOW_TAG   1065      /*ǰ̨�������ݳ�ʼ����ʶ*/
#define SAFETY_USERDEFFUNC_TAG  1066  /*�Զ��幦�����ݳ�ʼ����ʶ*/
#define SAFETY_USERDEFRIGHT_TAG 1067  /*�Զ���Ȩ�����ݳ�ʼ����ʶ*/
#define SAFETY_LIBFILE_TAG  1068      /*ǰ̨���ļ����ݳ�ʼ����ʶ*/
#define SAFETY_SYSTEM_TAG   1069      /*ϵͳȨ�޳�����ʼ����ʶ*/

#define SAFETY_DATAADDR     4096                      /*���ݻ���ַ*/
#define POLIC_ORIKEY_ADDR   SAFETY_DATAADDR+0         /*��ȫ�����ļ������ַ1*/
#define POLIC_DESKEY_ADDR   SAFETY_DATAADDR+4         /*��ȫ�����ļ������ַ2*/
#define POLIC_LICSIZE_ADDR  SAFETY_DATAADDR+8         /*��ȫ�����ļ����֤�ļ���С��ַ*/
#define LIC_ORIKEY_ADDR     SAFETY_DATAADDR+0         /*���֤�ļ������ַ1*/
#define LIC_DESKEY_ADDR     SAFETY_DATAADDR+4         /*���֤�ļ������ַ2*/
#define LIC_PRJLEVEL_ADDR   SAFETY_DATAADDR+8         /*���֤�ļ���Ŀ�㼶��ַ*/
#define LIC_MODULE_SIZE     SAFETY_DATAADDR+12        /*���֤�ļ�ģ�����ݴ�С*/
#define LIC_MODULE_ADDR     SAFETY_DATAADDR+16        /*���֤�ļ�ģ�����ݵ�ַ*/
#define LIC_WINDOW_SIZE     SAFETY_DATAADDR+20        /*���֤�ļ��������ݴ�С*/
#define LIC_WINDOW_ADDR     SAFETY_DATAADDR+24        /*���֤�ļ��������ݵ�ַ*/
#define LIC_USERDEFFUNC_SIZE SAFETY_DATAADDR+28       /*���֤�ļ��Զ��幦�����ݴ�С*/
#define LIC_USERDEFFUNC_ADDR SAFETY_DATAADDR+32       /*���֤�ļ��Զ��幦�����ݵ�ַ*/
#define LIC_USERDEFRIGHT_SIZE SAFETY_DATAADDR+36      /*���֤�ļ��Զ���Ȩ�����ݴ�С*/
#define LIC_USERDEFRIGHT_ADDR SAFETY_DATAADDR+40      /*���֤�ļ��Զ���Ȩ�����ݵ�ַ*/
#define LIC_LIBFILE_SIZE    SAFETY_DATAADDR+44        /*���֤�ļ�ǰ̨���ļ����ݴ�С*/
#define LIC_LIBFILE_ADDR    SAFETY_DATAADDR+48        /*���֤�ļ�ǰ̨���ļ����ݵ�ַ*/
#define LIC_SYSTEM_SIZE     SAFETY_DATAADDR+52        /*���֤�ļ�ϵͳȨ�����ݴ�С*/
#define LIC_SYSTEM_ADDR     SAFETY_DATAADDR+56        /*���֤�ļ�ϵͳȨ�����ݵ�ַ*/
#define LIC_DATA_ADDR       SAFETY_DATAADDR+1024      /*���֤�ļ����ݻ���ַ*/

#endif 