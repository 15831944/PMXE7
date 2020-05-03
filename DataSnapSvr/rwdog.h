//---------------------------------------------------------------------------

#ifndef rwdogH
#define rwdogH
//---------------------------------------------------------------------------
#define SNLENGHT  21
bool CheckSoftDog(void); //�������֮ǰ
bool OpenSoftDog(void);  //��Ҫ��дʱ����ִ�иú�����
                         //��ִ�иú���ʱ����ִ��CheckSoftDog()��ReadDogSerial(AnsiString &dserial)
bool CloseSoftDog(void); //���ٶ�дʱ��ִ�иú���
bool InitializeDog(void);//����ʲô״̬����ǿ�Ƴ�ʼ��,����������
bool ReadDogSerial(AnsiString &dserial);//����򿪣���ɶ�ȡ�����ID
//0
bool WriteRegistCode(AnsiString regcode);//дע����
bool ReadRegistCode(AnsiString &regcode);//��ע����
bool ReadVersionCode(AnsiString &Version);
//1
bool ReadTrialTimes(int &ttimes);
bool WriteTrialTimes(int ttimes);
bool ReadUsedTimes(int &utimes);
bool WriteUsedTimes(int utimes);
//2
bool ReadDogStatus(int &status); //status:0-�Ѽ���,1-δ����,2-�ѳ�ʼ��,3-δ��ʼ��
bool StartUsing(void);//����״̬Ϊδ����״̬,���ɹ������ͨ��ReadDogStatus����ȡ��ǰ״̬
bool SetDogActive(void);//���ɹ������ͨ��ReadDogStatus����ȡ��ǰ״̬
//3
bool ReadProductSerial(AnsiString &pserial);
bool WriteProductSerial(AnsiString pserial);
//4
bool ReadUserCount(int &UserCount);
bool WriteUserCount(int UserCount);
bool ReadUserCount2(int &UserCount2);
bool WriteUserCount2(int UserCount2);

//�жϸ�ģ���Ƿ����iModule=[0..79](������)���״��ȵ���ReadModulesInfor();
bool GetModuleIsExists(int iModule);

//����ģ������Ϊ����(������),Ҫ����WriteModulesInfor()����д�������
//�״ε���֮ǰ�ȵ���ReadModulesInfor();
bool SetModuleIsExists(int iModule,bool bIsExists);

bool ReadModulesInfor(); //�����������ģ�������Ϣ�����浽������

bool WriteModulesInfor();//��ģ�黺��������д�������

//��������һ���Զ�ȡ����ģ����Ϣ������ǰ��ִ��ReadModulesInfor����
char* GetAllModulesInfor();
AnsiString GetAllModulesSign(); //�÷�ͬ�ϣ�ֻ�������ص�ֵΪ��'\t'������ģ���

//��������һ����д������ģ����Ϣ,��������WriteModulesInfor��������д�������
//mInfors��'0'��'1'��ɵ��ַ������ɶ���0��79��ģ��
bool SetAllModulesInfor(char* mInfors);

//���ظ�ģ���Ƿ���ã�ȷ�ϵ���֮ǰ�ȵ���ReadModulesInfor
bool IsModuleCanUse(int iModuleCode);
//дvip��־
bool WriteVIPSigner(bool bIsVip);
//��vip��־
bool ReadVIPSigner(bool &bIsVip);
//дRemoter��־
bool WriteRemoterSigner(bool bIsRemoter);
//��Remoter��־
bool ReadRemoterSigner(bool &bIsRemoter);
//д�ܳ�
bool WriteUIDKey(AnsiString UIDKey);
//���ܳ�
bool ReadUIDKey(AnsiString &UIDKey);
AnsiString EncryptStr(AnsiString str);
//д���˵�¼��Կ ,���Ȳ�������6λ
bool WriteUserIDKey(AnsiString UserIDKey);
//�����˵�¼��Կ
bool ReadUserIDKey(AnsiString &UserIDKey);
#endif
