//---------------------------------------------------------------------------
#ifndef ProjectStructH
#define ProjectStructH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <vector>
#include "Vclcomfunc.h"
using namespace std;
using namespace Vclcomfunc;
namespace ProjectStruct
{

//---------------------------------------------------------------------------
enum TFrontType	{
		ftNone=0,    //0-��
		ftFinishStart=1,// 1-���-��ʼ
		ftStartStart=2, // 2-��ʼ-��ʼ
		ftFinishFinish=3,// 3-���-���
		ftStartFinish=4  //4-��ʼ-���
		};
enum TDateStatus{ //����״̬
		dsRest=0, //��Ϣ
		dsWork=1, //����
		dsSuspend=2  //ͣ��
		};
enum TWeekDay{
		wdSunday=0,
		wdMonday=1,
		wdTuesday=2,
		wdWednesday=3,
		wdThursday=4,
		wdFriday=5,
		wdSaturday=6
		};
enum TEditStatus { //�ڵ��޸�״̬
		esNone=0,
		esNew=1,
		esModified=2,
		esDeleted=4,
		esEditTaskStatus=8
		};
enum TTaskStatus{  //����״̬
		tsNotStart=0,
		tsStarted=1,
		tsOvered=2,
		tsPaused=3
		};
enum TTaskType { //�ڵ�����
		ttProject=0,
		ttTask=1,
		ttAbstractTask=2,
		ttForeignProject=3,
		ttMileStone=4
		};
enum TStateDateType {
		sdtPrjProgressDate=0,
		sdtCurrentDate=1,
		sdtManualDate=2
		};//״̬����ȡֵ��ʽ
enum TTaskPctType {
		taptDurtion=0,
		taptWorkLoad=1,
		taptManual=2
		}; //ʵ����ɰٷֱȼ�������
enum TTaskDtnType {
		tdtUnit=0,
		tdtDuration=1,
		tdtLabor=2
		};//��������
enum TProjectType {
			ptSingle, //����Ŀ
			ptComplex //����Ŀ
			};//��Ŀ����
enum TPlanType {ptNone,
				ptASAP,//Խ��Խ��
				ptALAP,//Խ��Խ��
				ptMSO ,//���뿪ʼ��
				ptMFO, //���������
				ptBSO ,//��������...��ʼ
				ptBFO, //��������...���
				ptASO, //��������...��ʼ
				ptAFO //��������...���
			   };

extern PACKAGE String GetPlanTypeName(int plantype);

//---------------------------------------------------------------------------
class PACKAGE TPrjObject; //���нڵ����
class PACKAGE TProject;//��Ŀ
class PACKAGE TTask;//����
class PACKAGE TAbstractTask;//ժҪ����
class PACKAGE TForeignProject;//�ⲿ��Ŀ
class PACKAGE TIteratorBase;//������
class PACKAGE TTaskIterator;
class PACKAGE TTaskLink;//�����ϵ
class PACKAGE TTaskLinkIterator; //�����ϵ������
class PACKAGE TPrjDataChangeLink;
class PACKAGE TCalculator; //������
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TRequestSaveEvent)(TProject* AProject);
typedef void __fastcall (__closure *TLoadDataEvent)(TProject* AProject, TForeignProject* AForPrj);
typedef void __fastcall (__closure *TTaskEvent)(TProject* AProject, TPrjObject* ATask);
typedef void __fastcall (__closure *TTaskLinkEvent)(TProject* AProject,
                                             TTask* ATask, TTaskLink* ATaskLink);
//---------------------------------------------------------------------------
class PACKAGE TCalculator
{
private:
	TDateTime b_StartDate, b_EndDate;
    TProject* FProject;
	void PreCalcTask(TTask *ATask,TDateTime &SDate, TDateTime &EDate, BYTE Plan);
	void CalcTask(TTask *ATask,TDateTime &SDate, TDateTime &EDate, BYTE Plan);
    void CalcTaskPert(TTask *ATask,double &SDate, double &EDate,double &SQ,double &EQ);
    void PreCalcTaskPert(TTask *ATask,double &SDate, double &EDate,double &SQ,double &EQ);
	void CalcTaskFreeDays();
    void ResetCalcFlags();
    TDateTime GetMaxDate();
	TDateTime GetMinDate();
    TDateTime GetProperStartDate(TTask * t);
    TDateTime GetProperEndDate(TTask * t);
    void ResetVisitFlags();
    int GetInDegree(TTask *t);
    String GetErrorLinks();
    int CalculateImpl();
    TDateTime WorkDayOffset(TDateTime BaseDate, int Delta);
public:
    TCalculator(TProject * AProject):FProject(AProject){}
    bool IsCyclic();
    int Calculate();
        void CalculatePert();
};
//---------------------------------------------------------------------------
class PACKAGE TTaskData
{
private:
		TDateTime FBStartDate;
		TDateTime FBEndDate;
		int FBCostDays;             //�������
protected:
		TDateTime   FLimitedDate;       //���ƿ�ʼ���ڣ�����ptMSO�ƻ�������������Ч��
		TDateTime   FLimitedEndDate;    //���ƽ������ڣ�����ptMSO�ƻ�������������Ч��
		bool        FCheckItem;         //�Ƿ������Ŀ
		double      FTaskPctPercent;    //������ɰٷֱ�%,���������;�����
		TTaskPctType FPctType;          //������ɰٷֱȼ������ݣ�0-���ڣ�1-������2-����
		TTaskDtnType FTaskDtnType;  //��������
		String  FDesc;              //��������
		double      FCostDays;          //�ƻ�����
		double      FFactCostDays;      //ʵ�ʹ���
                                    // Ĭ�Ϲ�����������λΪϵͳ������λ"�ٷֱ�"
		double      FWorkLoad;          //�ƻ�������
                                    //�����ɽ��ȱ���ʱ����ʱ���ͳ��������Դ�ڽ��Ƚ�ֹ���ڵļƻ������������������ܵõ�������Ϊ0ʱ������������Ϊ0��
		double      FFactWorkLoad;      //(ʵ�ʹ�����	����
                                    //ʵ�ʹ�����ֻ��С�ڵ��ڼƻ�������,�������깤ʱ������ȫ���
		double      FEnvaluedWorkLoad;    //����������ֵ
		double      FInvest;            //�ƻ���ֵ
		double      FFactInvest;        //ʵ�ʲ�ֵ ����
                                    //ʵ�ʲ�ֵ����С��Ҳ���Դ��ڼƻ���ֵ
		double      FEnvaluedInvest;
		bool        FMileStone;         //��̱���ʶ	0-����̱�	1-��̱�
		double      FOptimismCostDays;  //�ֹ۹���
		double      FAnticipateCostDays;//Ԥ�ڹ���
		double      FPessimismCostDays; //���۹���
		double      FRiskProbability;   //���ո���		�����������յĸ���
		double      FRiskQty;           //������	���շ���ʱԤ�Ʋ�������ʧ
		double      FRiskLoss;          //������ʧ	ʵ�ʷ����ķ�����ʧ
		TDateTime   FProcessDate;       //���Ƚ�ֹ����
		TTaskStatus FTaskStatus;        //���ȱ�ʶ

		TTaskType   FTaskType;          //�������� 1-���棬2-�飬3-�ⲿ����,4-��̱�
		String  	FGroupCode;         //�ϼ�������
		String  	FForeignID;       //�ⲿ��Ŀ��
		int         FForeignChangeFlag; //�ⲿ�仯��ʶ 0-�ޣ�1-��
		int         FPlanAdjFlag;       //�ƻ�������ʶ
		int         FStopFlag;          //ͦ������־
		int         FWorkLoadType;//���������ͣ�0������ϸ��������1������ϸ������
		double      FProportion;
		String  	FCode;
		String  	FSmartCode;
		String  	FName;
		//�ƻ�ʱ����ʱ����
		TDateTime FTempStartDate;
		TDateTime FTempEndDate;
		TDateTime FTempStartQ;
		TDateTime FTempEndQ;

		TDateTime FStartDate;
		TDateTime FEndDate;
		TDateTime FALAP_StartDate;
		TDateTime FALAP_EndDate;
		TDateTime FASAP_StartDate;
		TDateTime FASAP_EndDate;
		TDateTime FFactStartDate;
		TDateTime FFactEndDate;
		TDateTime FFF_StartDate;
		TDateTime FFF_EndDate;
		int         FIndex;
		TPlanType   FPlanType; //�����������
		bool        FCritical;//��������
		String  FWBS;
		int         FVitalLevel;//0һ�㣬1��Ҫ��2�ش�
		String  FPersonnel;
		double      FFreeDays;
		double      FTotalDiff;
        String FPrjCode;
        String FTaskID;
        String FTempTaskSyncDesc;
        double FPertStartTime;
        double FPertStartQ;
        double FPertEndTime;
        double FPertEndQ;
        double FBCWS;
        double FBCWP;
#pragma option -w-inl
template<class Type>
bool CompareSetValue(Type& OldValue, Type& NewValue)
    {
		if(OldValue != NewValue)
        {
			 OldValue = NewValue;
             if(!ReadingData)
             {
                 FEditStatus = TEditStatus(FEditStatus | esModified);
				 if(((TPrjObject*)(this))->Parent() != NULL)
                     return true;
             }
		}
		return false;
    }
#pragma option -winl
    void __fastcall SetDesc(String);
	void __fastcall SetPctType(TTaskPctType value);
	void __fastcall SetTaskPctPercent(double Value);
	void __fastcall SetWorkLoad(double Value);
    void __fastcall SetFactCostDays(double);
    void __fastcall SetFactWorkLoad(double);
    void __fastcall SetEnvaluedWorkLoad(double);
	void __fastcall SetInvestUnit(String);
    void __fastcall SetInvest(double);
    void __fastcall SetFactInvest(double);
    void __fastcall SetEnvaluedInvest(double);
    void __fastcall SetMileStone(bool);
    void __fastcall SetAnticipateCostDays(double);
    void __fastcall SetOptimismCostDays(double);
    void __fastcall SetPessimismCostDays(double);
    void __fastcall SetRiskProbability(double);
    void __fastcall SetRiskQty(double);
    void __fastcall SetRiskLoss(double);
    void __fastcall SetProcessDate(TDateTime);
    void __fastcall SetTaskStatus(TTaskStatus);
    void __fastcall SetWBS(String AWBS);
    void __fastcall SetFreeDays(double );
    void __fastcall SetTotalDiff(double );

    void __fastcall SetWorkLoadType(int Value);


	void __fastcall SetTaskType(TTaskType Value);          //�������� 1-���棬2-�飬3-�ⲿ���� ��4-��̱�
    void __fastcall SetGroupID(String);         //�ϼ�������
    void __fastcall SetForeignID(String);       //�ⲿ��Ŀ��
    void __fastcall SetForeignChangeFlag(int); //�ⲿ�仯��ʶ 0-�ޣ�1-��
    void __fastcall SetPlanAdjFlag(int);       //�ƻ�������ʶ

    bool __fastcall GetIsNew();
    void __fastcall SetIsNew(bool Value);
    bool __fastcall GetIsModified();
    void __fastcall SetIsModified(bool Value);
	void __fastcall SetCode(String Code);
    void __fastcall SetSmartCode(String Code);
    void __fastcall SetName(String Name);
    void __fastcall SetIndex(int AIndex);
    void __fastcall SetCostDays(double ACostDays);
    void __fastcall SetStartDate(TDateTime AStartDate);
    void __fastcall SetEndDate(TDateTime AEndDate);
    void __fastcall SetASAP_StartDate(TDateTime ASAP_StartDate);
    void __fastcall SetASAP_EndDate(TDateTime ASAP_EndDate);
    void __fastcall SetALAP_StartDate(TDateTime ALAP_StartDate);
    void __fastcall SetALAP_EndDate(TDateTime ALAP_EndDate);
    void __fastcall SetFactStartDate(TDateTime FactStartDate);
    void __fastcall SetFactEndDate(TDateTime FactEndDate);
    void __fastcall SetPlanType(TPlanType PlanType);
    void __fastcall SetCritical(bool Critical);
    void __fastcall SetProportion(double dProportion);
    void __fastcall SetVitalLevel(int ivalue);
    String __fastcall GetGroupID();
    String __fastcall GetSortCode();
    String __fastcall GetForeignID();
    TTaskType __fastcall GetTaskType();
    void __fastcall SetCheckItem(bool bCheckItem);
    void __fastcall SetPrjCode(String value);
    void __fastcall SetTaskID(String value);
    double __fastcall GetCV();
    double __fastcall GetSV();
    double __fastcall GetCPI();
    double __fastcall GetSPI();
    void __fastcall SetBCWS(double Value);
    void __fastcall SetBCWP(double Value);
    double __fastcall GetACWP();
    double __fastcall CalBCWP();
    double __fastcall CalBCWS();    
    double __fastcall GetPertDuration();
    double __fastcall GetPertDurationQ();
    void __fastcall SetPertStartTime(double value);
    void __fastcall SetPertStartQ(double value);
    void __fastcall SetPertEndTime(double value);
    void __fastcall SetPertEndQ(double value);
    TEditStatus FEditStatus;
public:
    static bool ReadingData;
    static bool Moving;
    TTaskData();
    void InitData();
    TTaskData& operator=(TTaskData&);
    void SumBCWS();
    void SumWorkLoad();
	void SumFactCostDays();
	void SumFreeDays();
	void SumFactWorkLoad();
    void SumEnvaluedWorkLoad();
    void SumInvest();
    void SumFactInvest();
	void SumEnvaluedInvest();
    void SumAnticipateCostDays();
    void SumOptimismCostDays();
    void SumPessimismCostDays();
    void SumProcessDate();
    void SumTaskStatus();
    void SumCostDays();
    void SumASAP_StartDate();
    void SumASAP_EndDate();
    void SumALAP_StartDate();
    void SumALAP_EndDate();
    void SumStartDate();
    void SumEndDate();
    void SumStartEndDate();
    void SumFactStartDate();
    void SumFactEndDate();
    void SumMileStone();
    void SumFFDate();
	void SumAll();
	void SumPctPercent();

	virtual int GetCumulateDays();
	TDateTime GetptyLimitedDate();
    void SetptyLimitedDate(TDateTime date);
    void SumPertStartTime();
	void SumPertEndTime();
    __property TDateTime    LimitedDate = {read=FLimitedDate, write=FLimitedDate};//���ƿ�ʼ����
    __property TDateTime    LimitedEndDate = {read=FLimitedEndDate, write=FLimitedEndDate}; //���ƽ�������
    __property TDateTime    ptyLimitedDate = {read=GetptyLimitedDate, write=SetptyLimitedDate};
	__property String   	Desc = {read=FDesc, write=SetDesc};
    __property double       FactCostDays = {read=FFactCostDays, write=SetFactCostDays}; //ʵ����Ͷ�빤��
    __property double       WorkLoad = {read=FWorkLoad,write=SetWorkLoad }; //�ƻ�����  ����Ŀ�۲���Ӧ��ɵĽ��ȣ�
    __property double       FactWorkLoad = {read=FFactWorkLoad, write=SetFactWorkLoad}; //ʵ�ʽ���
    __property double       EnvaluedWorkLoad = {read=FEnvaluedWorkLoad, write=SetEnvaluedWorkLoad};   //����ʵ�ʽ���
    __property double       Invest = {read=FInvest, write=SetInvest};    //�ƻ���Ͷ��/��ֵ
    __property double       FactInvest = {read=FFactInvest, write=SetFactInvest};//ʵ����Ͷ��/��ֵ ACWP/AC
    __property double       EnvaluedInvest = {read=FEnvaluedInvest, write=SetEnvaluedInvest}; //����ʵ����Ͷ��
    __property double       BCWS  = { read=FBCWS,write=SetBCWS }; // �ƻ����Ͷ�� BCWS/PV
    __property double       ACWP  = { read=GetACWP }; // ʵ����Ͷ�� ACWP/AC
    __property double       BCWP  = { read=FBCWP,write=SetBCWP};  // �����Ͷ��  Ӯ��ֵ BCWP/EV
    __property double       CV  = { read=GetCV };  //����ƫ�
    __property double       SV  = { read=GetSV };  //����ƫ�
    __property double       CPI  = { read=GetCPI }; //���ü�Чָ����
    __property double       SPI  = { read=GetSPI }; //���ȼ�Чָ����
    __property bool         MileStone = {read=FMileStone, write=SetMileStone};//��̱�
    __property double       AnticipateCostDays = {read=FAnticipateCostDays, write=SetAnticipateCostDays}; //�ֹ۹���
    __property double       OptimismCostDays = {read=FOptimismCostDays, write=SetOptimismCostDays}; //��������
    __property double       PessimismCostDays = {read=FPessimismCostDays, write=SetPessimismCostDays}; //���۹���
    __property double       RiskProbability = {read=FRiskProbability, write=SetRiskProbability};  // ���ո���
    __property double       RiskQty = {read=FRiskQty, write=SetRiskQty};               //������
    __property double       RiskLoss = {read=FRiskLoss, write=SetRiskLoss};            //������ʧ
    __property TDateTime    ProcessDate = {read=FProcessDate, write=SetProcessDate};    //���Ƚ�����
    __property TTaskStatus  TaskStatus = {read=FTaskStatus, write=SetTaskStatus};       //����״̬

	__property TTaskType    TaskType = {read=GetTaskType, write=SetTaskType};          //�������� 1-���棬2-�飬3-�ⲿ����
	__property String   	GroupID = {read=GetGroupID, write=SetGroupID};         //�ϼ�������
	__property String   	ForeignID = {read=GetForeignID, write=SetForeignID};       //�ⲿ��Ŀ��
    __property int          ForeignChangeFlag = {read=FForeignChangeFlag, write=SetForeignChangeFlag}; //�ⲿ�仯��ʶ 0-�ޣ�1-��
    __property int          PlanAdjFlag = {read=FPlanAdjFlag, write=SetPlanAdjFlag};       //�ƻ�������ʶ

    __property bool         IsNew = {read=GetIsNew, write=SetIsNew};
    __property bool         IsModified = {read=GetIsModified, write=SetIsModified};
	__property String   	Code = {read=FCode, write=SetCode};
	__property String   	SmartCode = {read=FSmartCode, write=SetSmartCode};   //�ڹ�·����������ֹ׮��
	__property String   	Name = {read=FName, write=SetName};
    __property int          Index = {read=FIndex, write=SetIndex};
    __property double       CostDays = {read=FCostDays, write=SetCostDays};//�ƻ�����
	__property double       CumulateDays = {read=GetCumulateDays};//�ۻ����ڣ������������
	__property TDateTime    StartDate = {read=FStartDate, write=SetStartDate};//�ƻ���ʼʱ��
    __property TDateTime    EndDate = {read=FEndDate, write=SetEndDate};//�ƻ�����ʱ��
    __property TDateTime    ALAP_StartDate = {read=FALAP_StartDate, write=SetALAP_StartDate};//����ʼʱ��
    __property TDateTime    ALAP_EndDate = {read=FALAP_EndDate, write=SetALAP_EndDate};//�������ʱ��
    __property TDateTime    ASAP_StartDate = {read=FASAP_StartDate, write=SetASAP_StartDate};//���翪ʼʱ��
    __property TDateTime    ASAP_EndDate = {read=FASAP_EndDate, write=SetASAP_EndDate};//�������ʱ��
    __property TDateTime    FF_StartDate = {read=FFF_StartDate, write=FFF_StartDate};//�������ʱ��(�м����)
    __property TDateTime    FF_EndDate = {read=FFF_EndDate, write=FFF_EndDate};//�������ʱ��      (�м����)
    __property TDateTime    FactStartDate = {read=FFactStartDate, write=SetFactStartDate};//ʵ�ʿ�ʼʱ��
    __property TDateTime    FactEndDate = {read=FFactEndDate, write=SetFactEndDate};//ʵ�ʽ���ʱ��
    __property TPlanType    PlanType = {read=FPlanType, write=SetPlanType};//�ƻ�����
    __property bool         Critical = {read=FCritical, write=SetCritical};//�Ƿ�ؼ�
	__property String   	WBS = {read=FWBS, write=SetWBS};
	__property double       FreeDays= {read=FFreeDays, write=SetFreeDays};      //����ʱ��
    __property double       TotalDiff= {read=FTotalDiff, write=SetTotalDiff};   //��ʱ��
    __property int          VitalLevel = {read=FVitalLevel, write=SetVitalLevel}; //������Ҫ��
	__property String   	Personnel = {read=FPersonnel, write=FPersonnel};    //���ε�λ
	__property int          WorkLoadType = {read=FWorkLoadType, write=SetWorkLoadType}; //0-��ͨ��1-��λ���̣�2-�ֲ����̣�3-�����
	__property String   	SortCode = {read=GetSortCode};                //�����
	__property String   	PrjCode  = { read=FPrjCode, write=SetPrjCode }; //������������Ŀ��
	__property String   	TaskID  = { read=FTaskID, write=SetTaskID }; //����ID������Ŀ��Ψһ
    __property TDateTime    TempStartDate  = { read=FTempStartDate, write=FTempStartDate }; //�������
    __property TDateTime    TempEndDate  = { read=FTempEndDate, write=FTempEndDate };   //�������
	__property String   	TempTaskSyncDesc  = { read=FTempTaskSyncDesc, write=FTempTaskSyncDesc };//�������ⲿ��Ŀͬ�����
    __property double       Proportion = {read=FProportion,write=SetProportion};  //Ȩ�أ����ڼ�����ϼ�������Ľ��ȹ���
    __property int          StopFlag = {write=FStopFlag,read=FStopFlag};//ͣ����־ 0:û��ͣ����¼ 1ͣ�� 2����
    __property bool         CheckItem={read=FCheckItem,write=SetCheckItem};     //�Ƿ��Ǻ�����Ŀ
    __property TTaskPctType PctType  = { read=FPctType, write=SetPctType };     //������ɰٷֱȼ�������
    __property double       TaskPctPercent  = { read=FTaskPctPercent, write=SetTaskPctPercent };  //������ɰٷֱ�
    __property TTaskDtnType TaskDtnType  = { read=FTaskDtnType, write=FTaskDtnType };  // ��������
	__property double 		PertStartTime  = { read=FPertStartTime, write=SetPertStartTime };   //������ʼʱ��
	__property double 		PertStartQ  = { read=FPertStartQ, write=SetPertStartQ };
	__property double 		PertEndTime  = { read=FPertEndTime, write=SetPertEndTime };    //��������ʱ��
	__property double 		PertEndQ  = { read=FPertEndQ, write=SetPertEndQ };
	__property double 		PertDuration  = { read=GetPertDuration };                 //��������
	__property double 		PertDurationQ  = { read=GetPertDurationQ };
	__property TDateTime 	TempStartQ  = { read=FTempStartQ, write=FTempStartQ };   //�������
	__property TDateTime 	TempEndQ  = { read=FTempEndQ, write=FTempEndQ };
	__property TDateTime 	BStartDate  = { read=FBStartDate, write=FBStartDate };
	__property TDateTime 	BEndDate  = { read=FBEndDate, write=FBEndDate };
	__property int 			BCostDays  = { read=FBCostDays, write=FBCostDays };


};
//---------------------------------------------------------------------------
class PACKAGE TPrjCalendar
{
private:
        TDateStatus FPrjCalendar[7];
        DynamicArray<TDateTime> FHolidays;
        DynamicArray<TDateTime> FOutOfdays;
        TDateStatus __fastcall GetPrjCalendar(TWeekDay AWeekDay);
        void __fastcall SetPrjCalendar(TWeekDay AWeekDay, TDateStatus AStatus);
public:
        void SetCalendar(TDateStatus Sunday, TDateStatus Monday,
                         TDateStatus Tuesday, TDateStatus Wednesday,
                         TDateStatus Thursday, TDateStatus Friday,
                         TDateStatus Saturday);
        void AddHoliday(TDateTime ADate);
        void AddOutOfday(TDateTime ADate);
        void DelOutOfday(TDateTime ADate);
        void DelHoliday(TDateTime ADate);
        void ClearHoliday();
        void ClearOutOfday();
        bool IsWorkDay(TDateTime ADate);
        bool IsOutOfDay(TDateTime ADate);
        bool IsHoliday(TDateTime ADate);
		__property TDateStatus PrjCalendar[TWeekDay AWeekDay] = {read=GetPrjCalendar, write=SetPrjCalendar};
};
//---------------------------------------------------------------------------
class PACKAGE TIteratorBase
{
        friend TProject;
        friend TForeignProject;
private:
        virtual void OnTaskAdd(TPrjObject&) = 0;
        virtual void OnTaskDelete(TPrjObject&) = 0;
        virtual void OnTaskLinkAdd(TTaskLink&) = 0;
        virtual void OnTaskLinkDelete(TTaskLink&) = 0;
        virtual void OnClear() = 0;
protected:
        TProject& FProject;
        TIteratorBase(TProject&);
        virtual ~TIteratorBase();
};
//---------------------------------------------------------------------------
class PACKAGE TTaskIterator : public TIteratorBase
{
private:
        virtual void OnTaskAdd(TPrjObject&);
        virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        virtual void OnClear();
		TPointerArray< TTask > FTasks;
        TTask* GetNextTask(TTask*ATask);
        TTask* GetChildTask(TPrjObject*AObj);
		void AddObjectChilds(TPrjObject&);
        void DeleteObjectChilds(TPrjObject&);
public:
        TTaskIterator(TProject&);
        TTask* operator [] (int Index);
        int Count();
        int TaskIndexOf(TTask&);
};
//---------------------------------------------------------------------------
class PACKAGE TTaskLinkIterator : public TIteratorBase
{
private:
        virtual void OnTaskAdd(TPrjObject&);
        virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        virtual void OnClear();
        TPointerArray< TTaskLink > FTaskLinks;
public:
        TTaskLinkIterator(TProject&);
        TTaskLink* operator [] (int Index);
        int Count();
};
//---------------------------------------------------------------------------
class PACKAGE TExpandFoldIterator : public TIteratorBase
{
private:
        virtual void OnTaskAdd(TPrjObject&);
        virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        virtual void OnClear();
        TPointerArray< TPrjObject > FObjects;
        int ExpandChild(TPrjObject&AObj, int Index);
        void ReIndex();
        void InsertObjectChilds(TPrjObject&, int& Index);
        void AddObjectChilds(TPrjObject&);
        void DeleteObjectChilds(TPrjObject&);
public:
        TExpandFoldIterator(TProject&);
        TPrjObject* operator [] (int Index);
        int  Count();
        void Expand(int Index);
        void Expand(TPrjObject*);
        void Fold(int Index);
        bool CanExpandFold(int Index);
        bool IsVisible(TPrjObject& AObj);
};
//---------------------------------------------------------------------------
class PACKAGE TObjectIterator : public TIteratorBase
{//���ж��󣬰����ⲿ��Ŀ������
private:
        virtual void OnTaskAdd(TPrjObject&);
        virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        virtual void OnClear();
        TPointerArray< TPrjObject > FObjects;
        TPrjObject* GetNextObject(TPrjObject&);
        void InsertObjectChilds(TPrjObject&, int& Index);
        void AddObjectChilds(TPrjObject&);
        void DeleteObjectChilds(TPrjObject&);
public:
        TObjectIterator(TProject&);
        TPrjObject* operator [] (int Index);
        int Count();
};
//---------------------------------------------------------------------------
class PACKAGE TInnerObjectIterator : public TIteratorBase
{//���ж��󣬲������ⲿ��Ŀ������
private:
        virtual void OnTaskAdd(TPrjObject&);
        virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        virtual void OnClear();
        TPointerArray< TPrjObject > FObjects;
        TPrjObject* GetNextObject(TPrjObject&);
        void InsertObjectChilds(TPrjObject&, int& Index);
        void AddObjectChilds(TPrjObject&);
        void DeleteObjectChilds(TPrjObject&);
public:
        TInnerObjectIterator(TProject&);
        TPrjObject* operator [] (int Index);
        int Count();
};
//---------------------------------------------------------------------------
class PACKAGE TPrjObject : public TTaskData
{
        friend TProject;
        friend TAbstractTask;
        friend TForeignProject;
private:
        virtual TTaskType GetTypeImpl() = 0;
        virtual TPrjObject& GetChildImpl(int Index);
        virtual int ChildCountImpl();
        virtual TPrjObject& InsertChildImpl(int Position, TTaskType AType);
        virtual void InsertChildObjectImpl(int Position, TPrjObject*);
        virtual void DeleteChildImpl(TPrjObject&AChild);
        virtual void RemoveChildImpl(TPrjObject&AChild);
        virtual void ClearImpl();
        virtual int ChildIndexImpl(TPrjObject&AChild);

        virtual TTaskLink* __fastcall GetFrontTask(int Index);
        virtual TTaskLink* __fastcall GetLateTask(int Index);
        virtual int __fastcall GetFrontTaskCount();
        virtual int __fastcall GetLateTaskCount();
		virtual TTaskLink* AddFrontTaskImpl(const String& FrontCode,const String& FrontPrjCode);
		virtual TTaskLink* AddLateTaskImpl(const String& LateCode,const String& LatePrjCode);
        virtual void DelFrontTaskImpl(TTaskLink&ATaskLink);
        virtual void DelLateTaskImpl(TTaskLink&ATaskLink);
		virtual void ClearFrontTaskImpl();
        virtual void ClearLateTaskImpl();
        virtual void AllChildIsModified(TPrjObject&AChild);
protected:
		TProject*   FProject;
		TPrjObject* FParent;
		TPrjCalendar* FPrjCalendar;
		virtual int ReIndexImpl(int Start);
		TPrjObject(TProject*AProject, TPrjObject*AParent, TPrjCalendar*APrjCalendar);
		virtual ~TPrjObject();
public:
		TPrjObject* Parent();
        TProject*   Project();
		TTaskType     GetType()                                 { return GetTypeImpl(); }
        TPrjObject& GetChild(int Index)                       { return GetChildImpl(Index); }
        int           ChildCount()                              { return ChildCountImpl(); }
        TPrjObject& InsertChild(int Position, TTaskType AType){ return InsertChildImpl(Position, AType); }
        TPrjObject& AddChild(TTaskType AType)                 { return InsertChildImpl(ChildCount(), AType); }
        void          DeleteChild(TPrjObject&AChild)          { DeleteChildImpl(AChild); }
        void          RemoveChild(TPrjObject&AChild)          { RemoveChildImpl(AChild); }
        void          Clear()                                   { ClearImpl(); }
        int           ChildIndex(TPrjObject&AChild)           { return ChildIndexImpl(AChild); }
        void SetAllChildIsModified(TPrjObject&AChild)         { AllChildIsModified(AChild); };
        bool          IsForeign();
        int           Layer();
        virtual void  Assign(TPrjObject*);
		TPrjObject*      ObjByID(String AID,String PrjID="");   //ĿǰPrjID=PrjCode
		TPrjObject*      ObjByCode(String ACode,String PrjCode="");

		int GetCumulateDays();

		TTaskLink* AddFrontTask(const String& FrontCode,const String& FrontPrjCode="")  { return AddFrontTaskImpl(FrontCode,FrontPrjCode); }
		TTaskLink* AddLateTask(const String& LateCode,const String& LatePrjCode="")  { return AddLateTaskImpl(LateCode,LatePrjCode); }
		void DelFrontTask(TTaskLink&ATaskLink)                { DelFrontTaskImpl(ATaskLink); }
		void DelLateTask(TTaskLink&ATaskLink)                 { DelLateTaskImpl(ATaskLink); }
		void ClearFrontTask()                                   { ClearFrontTaskImpl(); }
		void ClearLateTask()                                    { ClearLateTaskImpl(); }
		__property TTaskLink* FrontTasks[int Index] = {read=GetFrontTask};
		__property TTaskLink* LateTasks[int Index] = {read=GetLateTask};
		__property int FrontTaskCount = {read=GetFrontTaskCount};
		__property int LateTaskCount = {read=GetLateTaskCount};

		bool CheckExclusiveWBS(String sWBS);
        void CheckWBSRule(String sWBS);
        String FillPrefix(String sWBS);
        String BuildWBS(int iLevel,int order);
        virtual void BuildChildWBS();
        void BuildSelfWBS();
        void ClearChildWBS();

		TTask* GetTaskByID(TPrjObject* AObj, const String& AID,const String& APrjID="");
protected:
		bool        FCalced;
		void        ResetEditStatus();
public:
		__property TPrjObject& Childs[int Index] = {read = GetChildImpl};
		__property TPrjCalendar*     Calendar = { read  = FPrjCalendar };
		__property bool          Calced = {read=FCalced,write=FCalced};
public:
		bool  Visited;//�Ƿ��Ѿ�����
		int   EFExpandIndex;//֧��չ����£
		bool  EFExpanded;//֧��չ����£
		int   NCLeft;  //����ͼ
		int   NCTop;
		RECT  LGRect;//�߼�ͼ
        int   DCStartEvent;//˫��������ͼ
        int   DCEndEvent;//˫��������ͼ
        bool  StartInherit;//ǰ�ڵ��Ǽ̳е�
        bool  EndInherit;  //��ڵ��Ǽ̳е�
		int   DCY;			//˫��������ͼ
        int   StartLinkCount;//��ʼ�ڵ��������
		int   EndLinkCount;  //�����ڵ�ĳ�����
        int   EndFSCount;  //�����ڵ�����߹�ϵΪFS�ĸ���
};
//---------------------------------------------------------------------------
struct TDeletedTaskLinkItem{
       String TaskID;
	   String FrontID;
	   String ProjectID;
	   String FrontPrjID;
};
class TDeletedTaskLinks : public TPointerArray< TDeletedTaskLinkItem >
{
       static void OnDeleteTaskLinkItem(void*AItem){ delete (TDeletedTaskLinkItem*)AItem; }
public:
       TDeletedTaskLinks() : TPointerArray< TDeletedTaskLinkItem >(OnDeleteTaskLinkItem) {}
	   void Add(const String& TaskID, const String& FrontID,const String& ProjectID,const String& FrontPrjID)
       {
		   TDeletedTaskLinkItem* AItem = new TDeletedTaskLinkItem();
           AItem->TaskID = TaskID;
		   AItem->FrontID = FrontID;
		   AItem->ProjectID = ProjectID;
		   AItem->FrontPrjID = FrontPrjID;
		   TPointerArray< TDeletedTaskLinkItem >::Add(AItem);
	   }
};
//---------------------------------------------------------------------------
//WBS����
class PACKAGE TWBSRule
{
private:
public:
    int Level;
    int CharType;
    int Length;
    String Separator;

    TWBSRule();
    String GetChar(int order);
    String BuildWBS(int order);
    String FillPrefix(String sWBS);
    void CheckWBSRule(String sWBS);
};
class PACKAGE TProject : public TPrjObject
{
        friend TTask;
        friend TTaskLink;
		friend TPrjDataChangeLink;
        friend TAbstractTask;
        friend TForeignProject;
        friend TIteratorBase;
        friend TExpandFoldIterator;
private:
		String FCalendarType;//��������	��������
		String FPrjType;//��Ŀ����

        TCalculator* Calc;
		void __fastcall SetCalendarType(String ACalendarType);//��������
		void __fastcall SetPrjType(String APrjType);//��Ŀ����

        bool FUsePCWBS;
		TPrjCalendar FPrjCalendarObj;
        TPointerArray<TPrjDataChangeLink> __FPrjDataChangeLink;

        virtual TTaskType GetTypeImpl();
        TPointerArray< TPrjObject > FChilds;
        static void OnDeleteChild(void*);
        virtual TPrjObject& GetChildImpl(int Index);
        virtual int ChildCountImpl();
        virtual TPrjObject& InsertChildImpl(int Position, TTaskType AType);
        virtual void InsertChildObjectImpl(int Position, TPrjObject*);
        virtual void DeleteChildImpl(TPrjObject&AChild);
        virtual void RemoveChildImpl(TPrjObject&AChild);
        virtual void ClearImpl();
        virtual int ChildIndexImpl(TPrjObject&AChild);
        TPointerArray< TIteratorBase > FIterators;
        void OnTaskAdd(TPrjObject&);
        void OnTaskDelete(TPrjObject&);
        void OnTaskLinkAdd(TTaskLink&);
        void OnTaskLinkDelete(TTaskLink&);
		 //����Ҫ��������ʱ��������ֻ��MoveTask�����õ���
		TRequestSaveEvent   FOnRequestSave;
		TLoadDataEvent      FOnLoadData;
        TDateTime GetStateDate();
        TDateTime FStateDate;
		double __fastcall GetFactWorkLoad();
		double __fastcall GetEnvaluedWorkLoad();
		double __fastcall GetCV();
		double __fastcall GetSV();
		double __fastcall GetCPI();
		double __fastcall GetSPI();
		double __fastcall GetBCWS();
		double __fastcall GetACWP();
		double __fastcall GetBCWP();
protected:
		TStrings* FDeletedTasks;
        TPrjObject*  FSelected;
        bool FNeedEditStatus;
		void __fastcall SetNeedEditStatus(bool AStatus);
        void __fastcall SetSelected(TPrjObject* ATask);
public:
		TProject();
        virtual    ~TProject();
        void       BeginUpdate();
        void       EndUpdate();
        void       ReIndex();
        void       AddIterator(TIteratorBase*Itr);
		void       DeleteIterator(TIteratorBase*Itr);
		TTask*     TaskByID(String AID,String APrjID="");
        void       UpdateEditStatus();
		void       Assign(TPrjObject*);
        void       RequestUpdate();
        void       LoadForeignData(TForeignProject* AForPrj);

        TPrjObject*      MoveTask(int OldIndex, int NewIndex);//�ƶ�һ������
        TPrjObject*      AddChildTask(int Child, int Parent);//�ƶ�һ������
		TAbstractTask*   AddAbstractWidthSelection();//�ɵ�ǰѡ�е��������ժҪ����
        void               CheckNoChildAbstract();

        int                Calculate();
        bool               IsCyclic();

        TTaskIterator                            Tasks;
        TTaskLinkIterator                        TaskLinks;
        TExpandFoldIterator                      ExpandFoldTasks;
        TObjectIterator                          Objects;
        TInnerObjectIterator                     InnerObjects;
		TPointerArray< TPrjObject > Selections;
        double                                   StatError;//�������
		TDeletedTaskLinks                      DeletedTaskLinks;
		__property TStrings*     	DeletedTasks = {read=FDeletedTasks};
		__property bool          	NeedEditStatus = {read=FNeedEditStatus, write=SetNeedEditStatus};
		__property TPrjObject* 		Selected = {read=FSelected, write=SetSelected};
		__property bool          	UsePCWBS = {read=FUsePCWBS, write=FUsePCWBS};
		__property String    		CalendarType = {read=FCalendarType, write=SetCalendarType};//��������
		__property String    		PrjType = {read=FPrjType, write=SetPrjType};//��Ŀ����
		__property TRequestSaveEvent  RequestSave = {read=FOnRequestSave, write=FOnRequestSave};
		__property TLoadDataEvent     OnLoadData = {read=FOnLoadData, write=FOnLoadData};
		__property TDateTime 		  StateDate  = { read=GetStateDate };
		//���ܺ���
        String CreateNewTaskCode();
		//��Ŀ��������
		String 		ParentProject;//����Ŀ
		String 		ParentProjectTask;//����Ŀ����
		int        SynchronizationMark;//ͬ����ʶ 0��ͬ����1���������
        TDateTime  TaskASAP_StartDate;//����Ŀ�������翪��
        TDateTime  TaskASAP_EndDate;
        TDateTime  TaskALAP_StartDate;
        TDateTime  TaskALAP_EndDate;
        TPlanType  TaskPlanType;//����Ŀ����ƻ�����

        TDateTime  PBeginDate; //Ԥ�ƿ�ʼʱ��
        TDateTime  PEndDate;   //��ͬ�涨�������ʱ��

        TPointerArray< TWBSRule >     WBSRules;//WBS����
		static void OnDeleteWBSRule(void*Item);
        String WBSPreNo; //WBS��Ŀǰ׺
        TWBSRule* GetWBSRule(int index);
        void BuildChildWBS();
        __property double FactWorkLoad  = { read=GetFactWorkLoad };
		__property double EnvaluedWorkLoad  = { read=GetEnvaluedWorkLoad };
		__property double BCWS  = { read=GetBCWS }; // �ƻ����Ͷ�� BCWS/PV
		__property double ACWP  = { read=GetACWP }; // ʵ����Ͷ�� ACWP/AC
		__property double BCWP  = { read=GetBCWP };  // �����Ͷ��  Ӯ��ֵ BCWP/EV
		__property double CV  = { read=GetCV };  //����ƫ�
		__property double SV  = { read=GetSV };  //����ƫ�
		__property double CPI  = { read=GetCPI }; //���ü�Чָ����
		__property double SPI  = { read=GetSPI }; //���ȼ�Чָ����
};
//---------------------------------------------------------------------------
class PACKAGE TAbstractTask : public TPrjObject
{
        friend TProject;
        friend TForeignProject;
private:
        TPointerArray< TPrjObject > FChilds;
        static void OnDeleteChild(void*AChild);

        virtual TTaskType GetTypeImpl();
        virtual TPrjObject& GetChildImpl(int Index);
        virtual int ChildCountImpl();
        virtual TPrjObject& InsertChildImpl(int Position, TTaskType AType);
        virtual void InsertChildObjectImpl(int Position, TPrjObject*);
        virtual void DeleteChildImpl(TPrjObject&AChild);
        virtual void RemoveChildImpl(TPrjObject&AChild);
        virtual void ClearImpl();
        virtual int ChildIndexImpl(TPrjObject&AObj);
		TAbstractTask(TProject&AProject, TPrjObject&AParent, TPrjCalendar*APrjCalendar);
public:
		virtual void Assign(TPrjObject*);
};
//---------------------------------------------------------------------------
class PACKAGE TTask : public TPrjObject
{
	   friend TProject;
       friend TTaskLink;
       friend TAbstractTask;
       friend TForeignProject;
private:
       int __AddLateTask(TTaskLink*ATaskLink);
       void __DelLateTask(TTaskLink*ATaskLink);
	   static void OnFrontTaskDelete(void*);
       virtual TTaskType GetTypeImpl();
protected:
	   TPointerArray<TTaskLink> FFrontTasks;
	   TPointerArray<TTaskLink> FLateTasks;
       virtual TTaskLink* __fastcall GetFrontTask(int Index);
       virtual TTaskLink* __fastcall GetLateTask(int Index);
       virtual int __fastcall GetFrontTaskCount();
       virtual int __fastcall GetLateTaskCount();
	   virtual TTaskLink* AddFrontTaskImpl(const String& FrontID,const String& FrontPrjID);
	   virtual TTaskLink* AddLateTaskImpl(const String& LateID,const String& LatePrjID);
       virtual void DelFrontTaskImpl(TTaskLink&ATaskLink);
       virtual void DelLateTaskImpl(TTaskLink&ATaskLink);
       virtual void ClearFrontTaskImpl();
       virtual void ClearLateTaskImpl();

       virtual void Assign(TPrjObject*);
	   void AssignLink(TTask*);

       TTask(TProject&AProject, TPrjObject&AParent, TPrjCalendar*APrjCalendar);
       virtual ~TTask();
};
//---------------------------------------------------------------------------
class PACKAGE TTaskLink
{
       friend TProject;
       friend TTask;
private:
       TFrontType FType;
	   double FFreeTime;
       void __fastcall SetType(TFrontType AType);
       void __fastcall SetFreeTime(double ATime);
       bool __fastcall GetCritical();
       int __fastcall WorkDayCount(TDateTime Start, TDateTime End);
       bool __fastcall GetIsNew();
       void __fastcall SetIsNew(bool Value);
       bool __fastcall GetIsModified();
       void __fastcall SetIsModified(bool Value);
       void ResetEditStatus();
private:
	   void __SetVoidFrontTask(void* Data);
protected:
       TEditStatus FEditStatus;
       TTask* FTask;
       TTask* FFrontTask;
       TProject* FProject;
       __fastcall TTaskLink(TTask& ATask, TTask&AFront);
       virtual __fastcall ~TTaskLink();
       void Assign(TTaskLink*);
public:
       bool IsForeign();
       __property TFrontType   Type = {read=FType, write=SetType};
	   __property double       FreeTime = {read=FFreeTime, write=SetFreeTime};
       __property TTask*     Front = {read=FFrontTask};
       __property TTask*     Task = {read=FTask};
       __property TProject*  Project = {read=FProject};
       __property bool         Critical = {read=GetCritical};
       __property bool         IsNew = {read=GetIsNew, write=SetIsNew};
       __property bool         IsModified = {read=GetIsModified, write=SetIsModified};
public:
	   POINT NCPoint[6];   //��������ͼ����¼���ߵ�λ�ü������
	   int   NCPointCount;//��������ͼ
	   int   NCBridgeLineIndex;//��������ͼ
	   POINT *NCBridgePoint;//��������ͼ
	   int   NCBridgePointCount;//��������ͼ
};
//---------------------------------------------------------------------------
class PACKAGE TForeignTaskIterator : public TIteratorBase
{
private:
        virtual void OnTaskAdd(TPrjObject&);
		virtual void OnTaskDelete(TPrjObject&);
        virtual void OnTaskLinkAdd(TTaskLink&);
        virtual void OnTaskLinkDelete(TTaskLink&);
        TPointerArray< TTask > FTasks;
        TForeignProject&  FForeignProject;
public:
        TForeignTaskIterator(TProject&, TForeignProject&);
        TTask& operator [] (int Index);
        int Count();
};
//---------------------------------------------------------------------------
class PACKAGE TForeignProject : public TTask
{
        friend TProject;
		friend TAbstractTask;
private:
		TPrjCalendar FPrjCalendarObj;
		TPointerArray< TPrjObject > FChilds;
        static void OnDeleteChild(void*AChild);

        virtual TTaskType GetTypeImpl();
        virtual TPrjObject& GetChildImpl(int Index);
        virtual int ChildCountImpl();
        virtual TPrjObject& InsertChildImpl(int Position, TTaskType AType);
        virtual void InsertChildObjectImpl(int Position, TPrjObject*);
        virtual void DeleteChildImpl(TPrjObject&AChild);
        virtual void RemoveChildImpl(TPrjObject&AChild);
        virtual void ClearImpl();
        virtual int ChildIndexImpl(TPrjObject&AObj);
        virtual int ReIndexImpl(int Start);

//		TForeignProject(TProject&AProject, TPrjObject&AParent, TPrjCalendar*APrjCalendar);
		TForeignProject(TProject&AProject, TPrjObject&AParent);
public:
        virtual void Assign(TPrjObject*);
		bool LoadedData;
};
//---------------------------------------------------------------------------
//  TPrjDataChangeLink ���ڵ���Ŀ���ݽṹ�����ݱ䶯ʱ֪ͨ
//  �󶨵������ṹ(�����ͼ������ͼ��)�������ݣ�������һ���ӿڣ��ṩ�����鷽����
//  ��������  ���洢��TProject�� vector<TPrjDataChangeLink*> __FPrjDataChangeLink;
class TPrjDataChangeLink
{
      friend TProject;
private:
protected:
      virtual void NoBind();
      virtual void Bind();
      TProject* FProject;
public:
      void SetProject(TProject*AProject);
	  TPrjDataChangeLink(TProject*AProject);
	  virtual ~TPrjDataChangeLink();

	  virtual void BeginUpdate() = 0;
	  virtual void EndUpdate() = 0;

	  virtual void BeforeProjectChange(){}
	  virtual void AfterProjectChange(){}

	  virtual void BeforeTaskChange(TPrjObject*ATask){}
	  virtual void AfterTaskChange(TPrjObject*ATask){}

	  virtual void BeforeTaskDelete(TPrjObject*ATask){}
	  virtual void AfterTaskDelete(){}

	  virtual void AfterTaskAdd(TPrjObject*ATask){}

	  virtual void BeforeTaskLinkChange(TTaskLink*ATaskLink){}
	  virtual void AfterTaskLinkChange(TTaskLink*ATaskLink){}

	  virtual void BeforeTaskLinkDelete(TTaskLink*ATaskLink){}
	  virtual void AfterTaskLinkAdd(TTaskLink*ATaskLink){}

	  virtual void SetSelect(TPrjObject*ATask, TPrjObject*OldSelect){}
};


}
#endif
