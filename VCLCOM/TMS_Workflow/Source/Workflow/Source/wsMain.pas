{***************************************************************************}
{ Workflow Studio Components for Delphi & C++Builder                        }
{ version 1.5.0.1                                                           }
{                                                                           }
{ written by TMS Software                                                   }
{            copyright ?1997 - 2010                                        }
{            Email : info@tmssoftware.com                                   }
{            Web : http://www.tmssoftware.com                               }
{                                                                           }
{ For information about version history, check the section "What's New"     }
{ at Workflow Studio manual.                                                }
{                                                                           }
{ The source code is given as is. The author is not responsible             }
{ for any possible damage done due to the use of this code.                 }
{ The component can be freely used in any application. The complete         }
{ source code remains property of the author and may not be distributed,    }
{ published, given or sold in any form as such. No parts of the source      }
{ code can be included in any other component or application without        }
{ written authorization of the author.                                      }
{***************************************************************************}

unit wsMain;

{$I wsdefs.inc}

interface

uses
  SysUtils, Classes, DB, Contnrs,
  {$IFDEF USE_INDY}
  IdSMTP, IdFTP, IdFTPCommon,IdMessage, IdMessageClient,
  {$ENDIF}
  {$IFDEF DELPHI6_LVL}
  Variants,
  {$ENDIF}
  wsClasses, wsDB, LiveDiagram, wsBlocks, wsDiagram;

type
  TWorkflowStudio = class;

  TWorkflowManager = class(TCustomWorkflowManager)
  private
    FWorkflowStudio: TWorkflowStudio;
  public
    constructor Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio); reintroduce;
    destructor Destroy; override;
    procedure LoadWorkflowDefinitionList(WorkDefs: TWorkflowDefinitions); override;
    function FindWorkflowDefinitionByName(AName: string): TWorkflowDefinition; override;
    procedure ListWorkflowDefinitionNames(AList: TStrings; BList:TStrings); override;

    function CreateWorkflowBizList: TWorkflowBizs; override;
    procedure LoadWorkflowBizList(WorkflowBizs: TWorkflowBizs); override;
    procedure LoadWorkflowBiz(WorkflowBiz: TWorkflowBiz); override;
    procedure CheckWorkflowBiz(WorkflowBiz: TWorkflowBiz; BizKeyValue: string;BillState:String);override;
    procedure UpdateWorkflowBiz(WorkflowBiz: TWorkflowBiz; WorkIns: TWorkflowInstance);override;

    function CreateWorkflowDefinitionList: TWorkflowDefinitions; override;
    function CreateWorkflowInstance(WorkDef: TWorkflowDefinition): TWorkflowInstance; override;
    function CreateWorkflowInstanceByName(WorkDefName: string): TWorkflowInstance; override;
    function CreateWorkflowInstanceByKey(AKey: string): TWorkflowInstance; override;
    function FindWorkflowInstanceByKey(AKey: string): TWorkflowInstance; override;
    procedure ListPendingWorkflowInstances(AKeyList: TStrings); override;

    procedure DeleteWorkflowInstance(WorkIns: TWorkflowInstance); override;
    procedure DeleteWorkflowInstanceByKey(WorkInsKey: string); override;

    procedure SignalWorkflowInstance(AKey: string); override;
    procedure LoadWorkflowInstance(WorkIns: TWorkflowInstance); override;
    procedure SaveWorkflowInstance(WorkIns: TWorkflowInstance); override;
    procedure SaveWorkflowDefinition(WorkDef: TWorkflowDefinition); override;
    procedure LoadWorkflowDefinition(AKey: string; WorkDef: TWorkflowDefinition); override;
    procedure DeleteWorkflowDefinition(WorkDef: TWorkflowDefinition); override;
    procedure AssignWorkflowInstanceDiagram(ADiagram: TCustomWorkflowDiagram; WorkInsKey: string); override;
  end;

  TTaskManager = class(TCustomTaskManager)
  private
    FWorkflowStudio: TWorkflowStudio;
  public
    constructor Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio); reintroduce;
    procedure CreateTaskInstance(WorkIns: TWorkflowInstance; TaskDef: TTaskDefinition); override;
    procedure LoadTaskInstance(TaskIns: TTaskInstance); override;
    procedure SaveTaskInstance(TaskIns: TTaskInstance); override;
    procedure UpdateTaskInstance(TaskIns: TTaskInstance); override;
    function IsTaskFinished(AKey: string): boolean; override;
    procedure LoadTaskInstanceList(ATasks: TTaskInstanceList;AFilterType: TTaskFilterType; AFilterKey: string;
              completeState: integer; AOtherCondition: string='');override;
    function TaskExpirationDateTime(AWorkIns: TWorkflowInstance; ATaskInstance: TTaskInstance): TDateTime; override;
    procedure LogTaskInfo(TaskIns: TTaskInstance; Info: string); override;
  end;

  TWorkflowFormMode = (wfmModal, wfmMDI);

  TCustomWorkflowUserInterface = class(TComponent)
  private
    FWorkflowStudio: TWorkflowStudio;
  protected
    property WorkflowStudio: TWorkflowStudio read FWorkflowStudio;
  public
    constructor Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio); reintroduce;
    procedure ShowUserTasksDlg(AUserKey: string; AFormMode: TWorkflowFormMode = wfmModal); virtual; abstract;
    procedure ShowUsersTasksDlg(AUserKeys: string; AFormMode: TWorkflowFormMode = wfmModal); virtual; abstract;
    procedure ShowWorkInsTasksDlg(AWorkInsKey: string; AFormMode: TWorkflowFormMode = wfmModal); virtual; abstract;
    procedure ShowWorkflowDefinitionsDlg; virtual; abstract;
    procedure EditWorkflowDefinition(AWorkDef: TWorkflowDefinition); virtual; abstract;
    procedure EditWorkflowInstance(AWorkIns: TWorkflowInstance); virtual; abstract;
  end;

  TUserInterfaceClass = class of TCustomWorkflowUserInterface;

  TSendFtpFileEvent = procedure(Sender: TObject; AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation; var Sent: boolean) of object;
  TGetFtpFileEvent = procedure(Sender: TObject; AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation; var Get: boolean) of object;
  TSendMailEvent = procedure(Sender: TObject; TaskIns: TTaskInstance; AUser: TWorkflowUser; AEmailInfo: TEmailInformation; var Sent: boolean) of object;
  TWorkInsErrorEvent = procedure(Sender: TObject; WI: TWorkflowInstance; ErrMsg: string; var ShowError: boolean) of object;
  TWorkflowInstanceEvent = procedure(Sender: TObject; AWorkIns: TWorkflowInstance) of object;

  /// Description:
  ///   TWorkflowStudio is the main component of Workflow Studio package. A single TWorkflowStudio instance should be
  ///   added to the whole application, and from this component you have access to various methods and properties needed
  ///   to work with Workflow Studio programatically.
  TWorkflowStudio = class(TCustomWorkflowStudio)
  private
    FWorkflowDB: TCustomWorkflowDB;
    FInternalUserInterface: TCustomWorkflowUserInterface;
    FOnSendMail: TSendMailEvent;
    FOnSendFtpFile: TSendFtpFileEvent;
    FOnGetFtpFile: TGetFtpFileEvent;
    FFromEmail: string;
    FFtpInformation: TFtpInformation;
    FOnWorkInsError: TWorkInsErrorEvent;
    {$IFDEF USE_INDY}
    FIndySMTP: TidSMTP;
    FIndyFTP: TIdFTP;
    {$ENDIF}
    FOnRunFinished: TWorkflowInstanceEvent;
    {$IFDEF USE_INDY}
    procedure SetIndySMTP(const Value: TidSMTP);
    procedure SetIndyFTP(const Value: TidFTP);
    {$ENDIF}
    function GetUserInterface: TCustomWorkflowUserInterface;
    procedure SendGroupMail(TaskIns: TTaskInstance; AGroup: TWorkflowGroup; EMailInfo: TEmailInformation);
    procedure SendDepartmentMail(TaskIns: TTaskInstance; ADepartment: TWorkflowDepartment; EMailInfo: TEmailInformation);
    procedure SendJobMail(TaskIns: TTaskInstance; AJob: TWorkflowJob; EMailInfo: TEmailInformation);
    function GetCustomWorkflowDB: TCustomWorkflowDB;
    procedure SetCustomWorkflowDB(const Value: TCustomWorkflowDB);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function SendMail(TaskIns: TTaskInstance; AUser: TWorkflowUser; EMailInfo: TEmailInformation): boolean;
    function SendFtpFile(AttachmentItem: TAttachmentItem): boolean;
    function GetFtpFile(AttachmentItem: TAttachmentItem): boolean;
    procedure WorkflowInstanceError(WI: TWorkflowInstance; ErrMsg: string; var ShowError: boolean); override;
    procedure RunFinished(WI: TWorkflowInstance); override;
    {$IFDEF USE_INDY}
    function IndySendMail(EMailInfo: TEmailInformation): boolean;
    function IndySendFtpFile(AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation):boolean;
    function IndyGetFtpFile(AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation):boolean;
    {$ENDIF}
    property UserInterface: TCustomWorkflowUserInterface read GetUserInterface;
  published
    property WorkflowDB: TCustomWorkflowDB read GetCustomWorkflowDB write SetCustomWorkflowDB;
    property FromEmail: string read FFromEmail write FFromEmail;
    property FtpInformation: TFtpInformation read FFtpInformation write FFtpInformation;
    property OnSendMail: TSendMailEvent read FOnSendMail write FOnSendMail;
    property OnSendFtpFile: TSendFtpFileEvent read FOnSendFtpFile write FOnSendFtpFile;
    property OnGetFtpFile: TGetFtpFileEvent read FOnGetFtpFile write FOnGetFtpFile;
    {$IFDEF USE_INDY}
    property IndySMTP: TidSMTP read FIndySMTP write SetIndySMTP;
    property IndyFTP: TidFTP   read FIndyFTP write SetIndyFTP;
    {$ENDIF}
    property OnWorkInsError: TWorkInsErrorEvent read FOnWorkInsError write FOnWorkInsError;
    property OnRunFinished: TWorkflowInstanceEvent read FOnRunFinished write FOnRunFinished;
//    property GroupAssignmentMode;
    property OnAfterExecuteNode;
    property OnBeforeExecuteNode;
    property OnTaskCreated;
    property OnTaskFinished;
    property OnGetNow;
    property BeforeSaveTaskInstance;
    property OnInitializeScriptEngine;
  end;

  TWorkflowScriptEngineClass = class of TWorkflowScriptEngine;

var
  ScriptEngineClass: TWorkflowScriptEngineClass;
  UserInterfaceClass: TUserInterfaceClass;
  UserfmtSettings: TFormatSettings;
implementation

uses
  wsRes, wsScripter, wsUserInterface;

{ TWorkflowManager }

procedure TWorkflowManager.AssignWorkflowInstanceDiagram(ADiagram: TCustomWorkflowDiagram; WorkInsKey: string);
var
  WorkIns: TWorkflowInstance;
begin
  WorkIns := TWorkflowInstance.Create(nil, FWorkflowStudio);
  try
    WorkIns.Key := WorkInsKey;
    LoadWorkflowInstance(WorkIns);
    WorkIns.AssignToDiagram(ADiagram, true);
  finally
    WorkIns.Free;
  end;
end;

constructor TWorkflowManager.Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio);
begin
  inherited Create(AOwner);
  if AWorkflowStudio <> nil then
    FWorkflowStudio := AWorkflowStudio
  else
    raise Exception.CreateFmt('WorkflowStudio parameter not specified in %s.Create.', [ClassName]);
end;

function TWorkflowManager.CreateWorkflowDefinitionList: TWorkflowDefinitions;
begin
  result := TWorkflowDefinitions.Create(nil);
  result.WorkflowStudio := FWorkflowStudio;
  FWorkflowStudio.WorkflowDB.WorkflowDefinitionLoadList(result);
end;

function TWorkflowManager.CreateWorkflowInstance(WorkDef: TWorkflowDefinition): TWorkflowInstance;
begin
  if WorkDef <> nil then
  begin
    result := TWorkflowInstance.Create(nil, FWorkflowStudio);
    result.AssignFromDiagram(WorkDef.Diagram, false);
    result.DefinitionKey := WorkDef.Key;
    {Allow copying attachments from definition to instance.}
    result.Diagram.Attachments.MakeAllDirty;
    FWorkflowStudio.WorkflowDB.WorkflowInstanceInsert(result, FWorkflowStudio);
  end
  else
    result := nil;
end;

function TWorkflowManager.CreateWorkflowInstanceByKey(AKey: string): TWorkflowInstance;
var
  WorkDefs: TWorkflowDefinitions;
  WorkDef: TWorkflowDefinition;
begin
  result := nil;
  WorkDefs := FWorkflowStudio.WorkflowManager.CreateWorkflowDefinitionList;
  try
    WorkDef := WorkDefs.FindByKey(AKey);
    if WorkDef <> nil then
      result := CreateWorkflowInstance(WorkDef);
  finally
    WorkDefs.Free;
  end;
end;

function TWorkflowManager.CreateWorkflowInstanceByName(WorkDefName: string): TWorkflowInstance;
var
  WorkDefs: TWorkflowDefinitions;
  WorkDef: TWorkflowDefinition;
begin
  result := nil;
  WorkDefs := FWorkflowStudio.WorkflowManager.CreateWorkflowDefinitionList;
  try
    WorkDef := WorkDefs.FindByName(WorkDefName);
    if WorkDef <> nil then
      result := CreateWorkflowInstance(WorkDef);
  finally
    WorkDefs.Free;
  end;
end;

procedure TWorkflowManager.DeleteWorkflowDefinition(WorkDef: TWorkflowDefinition);
begin
  FWorkflowStudio.WorkflowDB.WorkflowDefinitionDelete(WorkDef);
end;

procedure TWorkflowManager.DeleteWorkflowInstance(WorkIns: TWorkflowInstance);
begin
  FWorkflowStudio.WorkflowDB.WorkflowInstanceDelete(WorkIns);
end;

procedure TWorkflowManager.DeleteWorkflowInstanceByKey(WorkInsKey: string);
var
  WorkIns: TWorkflowInstance;
begin
  WorkIns := TWorkflowInstance.Create(nil, FWorkflowStudio);
  try
    WorkIns.Key := WorkInsKey;
    DeleteWorkflowInstance(WorkIns);
  finally
    WorkIns.Free;
  end;
end;

destructor TWorkflowManager.Destroy;
begin
  inherited;
end;

function TWorkflowManager.CreateWorkflowBizList: TWorkflowBizs;
begin
  result := TWorkflowBizs.Create(nil,TWorkflowBiz);
  FWorkflowStudio.WorkflowDB.WorkflowBizLoadList(result);
end;

procedure TWorkflowManager.LoadWorkflowBizList(WorkflowBizs: TWorkflowBizs);
begin
     FWorkflowStudio.WorkflowDB.WorkflowBizLoadList(WorkflowBizs);
end;

procedure TWorkflowManager.LoadWorkflowBiz(WorkflowBiz: TWorkflowBiz);
begin
    FWorkflowStudio.WorkflowDB.WorkflowBizLoad(WorkflowBiz);
end;

procedure TWorkflowManager.CheckWorkflowBiz(WorkflowBiz: TWorkflowBiz; BizKeyValue: string;BillState:String);
begin
      FWorkflowStudio.WorkflowDB.WorkflowBizCheck(WorkflowBiz,BizKeyValue,BillState,FWorkflowStudio);
end;

procedure TWorkflowManager.UpdateWorkflowBiz(WorkflowBiz: TWorkflowBiz; WorkIns: TWorkflowInstance);
begin
      FWorkflowStudio.WorkflowDB.WorkflowBizUpdate(WorkflowBiz,WorkIns,FWorkflowStudio);
end;

function TWorkflowManager.FindWorkflowDefinitionByName(AName: string): TWorkflowDefinition;
var
  WorkDefs: TWorkflowDefinitions;
  WorkDef: TWorkflowDefinition;
begin
  result := nil;
  WorkDefs := CreateWorkflowDefinitionList;
  try
    WorkDef := WorkDefs.FindByName(AName);
    if WorkDef <> nil then
    begin
      result := TWorkflowDefinition.Create(FWorkflowStudio);
      result.Key := WorkDef.Key;
      
      {must load in a different object (result) instead of WorkDef, because WorkDef
       will be destroyed when WorkDefs.Free is called}
      LoadWorkflowDefinition(WorkDef.Key, result);
    end;
  finally
    WorkDefs.Free;
  end;
end;

function TWorkflowManager.FindWorkflowInstanceByKey(AKey: string): TWorkflowInstance;
begin
  result := TWorkflowInstance.Create(nil, FWorkflowStudio);
  result.Key := AKey;
  if not FWorkflowStudio.WorkflowDB.WorkflowInstanceLoad(result) then
  begin
    result.Free;
    result := nil;
  end;
end;

procedure TWorkflowManager.ListPendingWorkflowInstances(AKeyList: TStrings);
begin
  FWorkflowStudio.WorkflowDB.ListPendingWorkflowInstances(AKeyList, FWorkflowStudio.GetNow);
end;

procedure TWorkflowManager.ListWorkflowDefinitionNames(AList: TStrings; BList:TStrings);
begin
  FWorkflowStudio.WorkflowDB.ListWorkflowDefinitionNames(AList,BList);
end;

procedure TWorkflowManager.LoadWorkflowDefinition(AKey: string; WorkDef: TWorkflowDefinition);
begin
  WorkDef.Key := AKey;
  FWorkflowStudio.WorkflowDB.WorkflowDefinitionLoad(WorkDef);
end;

procedure TWorkflowManager.LoadWorkflowDefinitionList(WorkDefs: TWorkflowDefinitions);
begin
  FWorkflowStudio.WorkflowDB.WorkflowDefinitionLoadList(WorkDefs);
end;

procedure TWorkflowManager.LoadWorkflowInstance(WorkIns: TWorkflowInstance);
begin
  if not FWorkflowStudio.WorkflowDB.WorkflowInstanceLoad(WorkIns) then
    wsDBError(Format(_str(SErrorRecordNotFoundKey), ['workflow instance', WorkIns.Key]));
end;

procedure TWorkflowManager.SaveWorkflowDefinition(WorkDef: TWorkflowDefinition);
begin
  if WorkDef.Key = '' then
    FWorkflowStudio.WorkflowDB.WorkflowDefinitionInsert(WorkDef)
  else
    FWorkflowStudio.WorkflowDB.WorkflowDefinitionUpdate(WorkDef);
end;

procedure TWorkflowManager.SaveWorkflowInstance(WorkIns: TWorkflowInstance);
begin
  if WorkIns.Key = '' then
    FWorkflowStudio.WorkflowDB.WorkflowInstanceInsert(WorkIns, FWorkflowStudio)
  else
    FWorkflowStudio.WorkflowDB.WorkflowInstanceUpdate(WorkIns);
end;

procedure TWorkflowManager.SignalWorkflowInstance(AKey: string);
var
  WI: TWorkflowInstance;
begin
  WI := TWorkflowInstance.Create(nil, FWorkflowStudio);
  WI.Key := AKey;
  LoadWorkflowInstance(WI);
  FWorkflowStudio.WorkflowEngine.RunWorkflow(WI);
end;

{ TTaskManager }

constructor TTaskManager.Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio);
begin
  inherited Create(AOwner);
  if AWorkflowStudio <> nil then
    FWorkflowStudio := AWorkflowStudio
  else
    raise Exception.CreateFmt('WorkflowStudio parameter not specified in %s.Create.', [ClassName]);
end;

procedure TTaskManager.CreateTaskInstance(WorkIns: TWorkflowInstance; TaskDef: TTaskDefinition);
var
  TI: TTaskInstance;
  AAssignUsers: TStringList;
  AAssignedUserId: string;
  c: integer;
  EMailInfo: TEmailInformation;
  AUser: TWorkflowUser;
  AGroup: TWorkflowGroup;
  ADepartment: TWorkflowDepartment;
  AJob: TWorkflowJob;
  AAssignmentUsers: TTaskAssignedUsers;
begin
  AAssignUsers := TStringList.Create();
  try
    {Prepare script engine context}
    FWorkflowStudio.ScriptEngine.SetRuntimeContext(WorkIns);

    {Get the list of user keys which the task instance will be created for}
    AAssignmentUsers := TaskDef.Users;
   // AAssignment := WorkflowStudio.ScriptEngine.TranslateText(TaskDef.AssignmentRule);
    FWorkflowStudio.UserManager.GetAssignedUserList(AAssignmentUsers, AAssignUsers, WorkIns.FlowMnPrjID);
 { //���������ɣ��п����û������ż�����ĿȨ��
  if WorkIns <> nil and WorkIns.FlowMnPrjID>''  then
    begin  //���û����ĿȨ�ޣ��򲻴�����������
       for c :=  AAssignUsers.Count - 1 to 0 do
       begin
         if AAssignUsers.Objects[c] is TWorkflowUser then  //������û�
           begin
               AUser := TWorkflowUser(AAssignUsers.Objects[c]);
               if AUser.NeedPrjRight then  //��Ҫ������ĿȨ���û�
               begin //������û�û����ĿȨ�ޣ���ɾ�������û���
                   if FWorkflowStudio.UserManager.BelongsToProject(AUser.UserId,WorkIns.FlowMnPrjID)=False then
                       AAssignUsers.Delete(c);
               end;
           end;
       end;
    end;   }


    {Ensure that a task instance will be created, even if it's for a non-existant user}
    if AAssignUsers.Count = 0 then
      raise EWorkflowException.Create(Format(_str(STaskNotCreatedUserNotFound),
        ['�����û�']));

    {Iterate through all users and create a task instance for each one}
    for c := 0 to AAssignUsers.Count - 1 do
    begin
      AGroup := nil;
      AUser := nil;
      ADepartment := nil;
      AJob := nil;
      TI := TTaskInstance.Create(nil);
      try

        {Retrieve the object, if it's a group or user, set in the proper variable, and set the
         AAssignedUserKey variable which will be id to be saved in the database}
        if AAssignUsers.Objects[c] is TWorkflowUser then    {��������Ҳת��Ա������}
        begin
          AUser := TWorkflowUser(AAssignUsers.Objects[c]);
          AAssignedUserId := AUser.UserId;
          TI.AssignedUserType := autUser;
          TI.CanPrint := AUser.CanPrint;
          TI.FlowMnDtlStepDefin :=AUser.StepDefin;
          TI.NeedPrjRight := AUser.NeedPrjRight;
        end else
        if AAssignUsers.Objects[c] is TWorkflowGroup then
        begin
          AGroup := TWorkflowGroup(AAssignUsers.Objects[c]);
          AAssignedUserId := AGroup.GroupId;
          TI.AssignedUserType := autGroup;
          TI.CanPrint := AGroup.CanPrint;
          TI.FlowMnDtlStepDefin :=AGroup.StepDefin;
          TI.NeedPrjRight := AGroup.NeedPrjRight;
        end else
        if AAssignUsers.Objects[c] is TWorkflowDepartment then
        begin
           ADepartment :=TWorkflowDepartment(AAssignUsers.Objects[c]);
           AAssignedUserId := ADepartment.DepartmentId;
           TI.AssignedUserType := autDepartment;
           TI.CanPrint := ADepartment.CanPrint;
           TI.FlowMnDtlStepDefin :=ADepartment.StepDefin;
           TI.NeedPrjRight := ADepartment.NeedPrjRight;
        end else
        if AAssignUsers.Objects[c] is TWorkflowJob then
        begin
            AJob := TWorkflowJob(AAssignUsers.Objects[c]);
            AAssignedUserId := AJob.JobId;
            TI.AssignedUserType := autJob;
            TI.CanPrint := AJob.CanPrint;
            TI.FlowMnDtlStepDefin :=AJob.StepDefin;
            TI.NeedPrjRight := AJob.NeedPrjRight;
        end else
          raise EWorkflowException.Create('�ڲ����󣬴�������ʵ��ʱ�����Ĳ�����ת������');

        {Copy the source Task Definition to the Task definition object inside the Task Instance}
        TI.TaskDef.Assign(TaskDef);

        {evalute expressions in the text properties of task definition}
        TI.TaskDef.Subject := FWorkflowStudio.ScriptEngine.TranslateText(TI.TaskDef.Subject);
        TI.TaskDef.Description := FWorkflowStudio.ScriptEngine.TranslateText(TI.TaskDef.Description);
        TI.FlowMnDtlRemark := WorkIns.FlowMnSubject+#13#10+TI.TaskDef.Description;  //zws20140510,ҵ������+��������

        TI.Status := TaskDef.InitialStatus;

       //UserID := '';
        //TI.Comments := TI.TaskDef.Description;   {�����ò��������������}
        if WorkIns <> nil then
        begin
          TI.WorkInsKey := WorkIns.Key;
          TI.FlowMnDtlProjectID :=WorkIns.FlowMnPrjID;
        end;
//        TI.DefinitionKey := WorkIns.DefinitionKey;
        TI.UserID := AAssignedUserId;

        {get the new key}
        FWorkflowStudio.WorkflowDB.TaskInstanceInsert(TI, FWorkflowStudio);
        TaskDef.TaskInstanceKeys.Add(TI.Key);

        {TASK SAVED!
         Now send e-mail notifications. If it's an group, we must send e-mail to all users in the group.
         If it's an user, just send to the user}
        if TaskDef.MailNotification then
        begin
          //EmailInfo.ToAddr := AUser.Email;
          EMailInfo.From := FWorkflowStudio.FromEmail;
          EMailInfo.Subject := TI.TaskDef.Subject;
          EMailInfo.Text := TI.TaskDef.Description+ #13#10 +TI.Comments;
          case TI.AssignedUserType of
            autUser:
              FWorkflowStudio.SendMail(TI, AUser, EMailInfo);
            autGroup:
              FWorkflowStudio.SendGroupMail(TI, AGroup, EMailInfo);
            autJob:
              FWorkflowStudio.SendJobMail(TI, AJob, EMailInfo);
            autDepartment:
              FWorkflowStudio.SendDepartmentMail(TI, ADepartment, EMailInfo);
          end;
        end;
      finally
        TI.Free;
      end;
    end;
  finally
    AAssignUsers.Free;
  end;
end;

function TTaskManager.IsTaskFinished(AKey: string): boolean;
var
  TI: TTaskInstance;
begin
  TI := TTaskInstance.Create(nil);
  try
    TI.Key := AKey;
    FWorkflowStudio.WorkflowDB.TaskInstanceLoad(TI);
    result := TI.Completed;
  finally
    TI.Free;
  end;
end;

procedure TTaskManager.LoadTaskInstance(TaskIns: TTaskInstance);
begin
  FWorkflowStudio.WorkflowDB.TaskInstanceLoad(TaskIns);
end;

procedure TTaskManager.LoadTaskInstanceList(ATasks: TTaskInstanceList;
  AFilterType: TTaskFilterType; AFilterKey: string; completeState: integer; AOtherCondition: string);
begin
  FWorkflowStudio.WorkflowDB.TaskInstanceLoadList(ATasks, AFilterType, AFilterKey, completeState,FWorkflowStudio,AOtherCondition);
end;

procedure TTaskManager.LogTaskInfo(TaskIns: TTaskInstance; Info: string);
begin
  FWorkflowStudio.WorkflowDB.LogTaskInfo(TaskIns, FWorkflowStudio, Info);
end;

procedure TTaskManager.SaveTaskInstance(TaskIns: TTaskInstance);
var
  save: boolean;
begin
  save := True;
  if Assigned(FWorkflowStudio.BeforeSaveTaskInstance) then
    FWorkflowStudio.BeforeSaveTaskInstance(FWorkflowStudio, TaskIns, save);
  if save then
  begin
    if TaskIns.Key = '' then
      FWorkflowStudio.WorkflowDB.TaskInstanceInsert(TaskIns, FWorkflowStudio)
    else
      FWorkflowStudio.WorkflowDB.TaskInstanceUpdate(TaskIns, FWorkflowStudio);
    FWorkflowStudio.WorkflowManager.SignalWorkflowInstance(TaskIns.WorkInsKey);
  end;
end;

function TTaskManager.TaskExpirationDateTime(AWorkIns: TWorkflowInstance; ATaskInstance: TTaskInstance): TDateTime;
begin
  if (ATaskInstance.TaskDef.ExpirationDateTime > '') and (ATaskInstance.TaskDef.ExpirationStatus > '') then
  begin
    // evaluates the expiration date/time expression
    FWorkflowStudio.ScriptEngine.SetRuntimeContext(AWorkIns, ATaskInstance);
    result := FWorkflowStudio.ScriptEngine.CalculateExpression(ATaskInstance.TaskDef.ExpirationDateTime);
  end
  else
    result := 0;
end;

procedure TTaskManager.UpdateTaskInstance(TaskIns: TTaskInstance);
begin
  if TaskIns.Key > '' then
    FWorkflowStudio.WorkflowDB.TaskInstanceUpdate(TaskIns, FWorkflowStudio);
end;

{ TWorkflowStudio }

constructor TWorkflowStudio.Create;
{$IFDEF TRIAL}
var
  msg: string;
  i: integer;
{$ENDIF}
begin
  inherited Create(AOwner);
  WorkflowManager := TWorkflowManager.Create(nil, Self);
  TaskManager := TTaskManager.Create(nil, Self);
  {$IFDEF TRIAL}
  if (Now > 40850) or (Now < 40470) then
  begin
    msg := 'Thhtirsd fWtourikofkljohwb gSvtfucddirof tvgeyrhsuijohng bh5adse 4erxfp3iersewd2.e dPrldesaaszed xrcefgvigsntherrt,e 3osrq a'+
      'tkoi ucyotnrteiwnquaes decvfavlgubahtnijnmgk jdhogwfndlcosaxda zas qnweewr t'+
      'v4err5sti6oyn4 rf3reo5mt 6hgt5tfp4:r/3/ew4wrw4.dt3mss2seo4frt5wgahrbev.dceowm';
    i := 2;
    while i < length(msg) do
    begin
      System.Delete(msg, i, 1);
      inc(i);
    end;
    raise Exception.Create(msg);
  end;
  {$ENDIF}
end;

destructor TWorkflowStudio.Destroy;
begin
  WorkflowManager.Free;
  TaskManager.Free;
  if FInternalUserInterface <> nil then
  begin
    FInternalUserInterface.Free;
    FInternalUserInterface := nil;
  end;
  inherited;
end;

function TWorkflowStudio.GetUserInterface: TCustomWorkflowUserInterface;
begin
  if FInternalUserInterface = nil then
    FInternalUserInterface := UserInterfaceClass.Create(nil, Self);
  result := FInternalUserInterface;
end;

{$IFDEF USE_INDY}
function TWorkflowStudio.IndySendMail(EMailInfo: TEmailInformation): boolean;
var
  idMessage: TidMessage;
begin
  result := false;
  idMessage := TidMessage.Create(nil);
  try
    With idMessage do
    begin
      Clear;
      From.Text := EmailInfo.From;
      CCList.Add.Text := EMailInfo.Cc;
      BCCList.Add.Text := EMailInfo.Bcc;
      Recipients.Add.Text := EMailInfo.ToAddr;
      Subject := EMailInfo.Subject;
      Body.Text := EMailInfo.Text;
    end;

    if IndySMTP <> nil then with IndySMTP do
    begin
      Connect;
      try
        Send(idMessage);
        result := true;
      finally
        Disconnect;
      end;
    end;
  finally
    idMessage.Free;
  end;
end;
{$ENDIF}

procedure TWorkflowStudio.Notification(AComponent: TComponent; Operation: TOperation);
begin
  inherited;
  if Operation = opRemove then
  begin
    if AComponent = FWorkflowDB then
      FWorkflowDB := nil;
    {$IFDEF USE_INDY}
    if AComponent = FIndySMTP then
      FIndySMTP := nil;
    {$ENDIF}
  end;
end;
               
function TWorkflowStudio.SendMail(TaskIns: TTaskInstance;
  AUser: TWorkflowUser; EMailInfo: TEmailInformation): boolean;
begin
  result := false;
  EMailInfo.ToAddr := AUser.Email;
  if Assigned(FOnSendMail) then
    FOnSendMail(Self, TaskIns, AUser, EMailInfo, result);
  if not result then
  begin
    {$IFDEF USE_INDY}
    result := IndySendMail(EMailInfo);
    {$ENDIF}
  end;
end;

{$IFDEF USE_INDY}
procedure TWorkflowStudio.SetIndySMTP(const Value: TidSMTP);
begin
  if FIndySMTP <> Value then
  begin
    FIndySMTP := Value;
    if Value <> nil then
      Value.FreeNotification(Self);
  end;
end;

procedure TWorkflowStudio.SetIndyFTP(const Value: TidFTP);
begin
  if FIndyFTP <> Value then
  begin
    FIndyFTP := Value;
    if Value <> nil then
      Value.FreeNotification(Value);
  end;
end;
function TWorkflowStudio.IndyGetFtpFile(AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation):boolean;
var
 DestPath: string;
 Buffer: string;
 FullName: string;
 APath: string;
begin
    if IndyFTP <> nil then with IndyFTP do
    begin
       Host := AFtpInfo.HostName;
       Port := AFtpInfo.Port;
       Username := AFtpInfo.UserName;
       Password := AFtpInfo.Password;
       Passive := AFtpInfo.Passive;
       TransferType := ftBinary;
       UseMLIS := True;
      Connect;
      try
       {��ת����ȷftpĿ¼,��û����,ÿ������ʵ������һ��Ŀ¼,�������Է�ֹ���� }
        if (Length(AFtpInfo.Root) > 0) and (AFtpInfo.Root[Length(AFtpInfo.Root)] <> '/') then
          Destpath :=AFtpInfo.Root+'/'+AFtpInfo.SpaceName+'/'+AFtpInfo.FtpDirectory+'/'+AttachmentItem.WorkKey
        else
           DestPath := AFtpInfo.Root+AFtpInfo.SpaceName+'/'+AFtpInfo.FtpDirectory+'/'+AttachmentItem.WorkKey;
        //���Ӵ���ת��Ŀ����Ŀ¼��
          ChangeDir('/');
          //MakeDir(DestPath);
          ChangeDir(DestPath);
        //������ʱĿ¼�ļ�
        SetLength(Buffer, MAX_PATH);
        SetLength(Buffer, GetTempPath(MAX_PATH, PChar(Buffer)));
        APath := Buffer;

        ForceDirectories(APath);

        if (Length(APath) > 0) and (APath[Length(APath)] <> '\') then
            APath := APath + '\';

         FullName := APath + AttachmentItem.Name;
        Get(AttachmentItem.Name,FullName,true);
        AttachmentItem.LoadContentFromFile(AttachmentItem.OriginalName);
        AttachmentItem.OriginalName :=FullName;
        result := true;
      finally
        Disconnect;
      end;
    end;
end;

function TWorkflowStudio.IndySendFtpFile(AttachmentItem: TAttachmentItem; AFtpInfo: TFtpInformation):boolean;
var
 DestPath: string;
 Buffer: string;
 FullName: string;
 APath: string;
begin
    if IndyFTP <> nil then with IndyFTP do
    begin
       Host := AFtpInfo.HostName;
       Port := AFtpInfo.Port;
       Username := AFtpInfo.UserName;
       Password := AFtpInfo.Password;
       Passive := AFtpInfo.Passive;
       TransferType := ftBinary;
       UseMLIS := True;
      Connect;
      try
       {��ת����ȷftpĿ¼,Ȼ������ݴ浽����Ӳ�̣�Ȼ���ϴ�,��û����
        ÿ������ʵ������һ��Ŀ¼,�������Է�ֹ���� }
        if (Length(AFtpInfo.Root) > 0) and (AFtpInfo.Root[Length(AFtpInfo.Root)] <> '/') then
          Destpath :=AFtpInfo.Root+'/'+AFtpInfo.SpaceName+'/'+AFtpInfo.FtpDirectory+'/'+AttachmentItem.WorkKey
        else
           DestPath := AFtpInfo.Root+AFtpInfo.SpaceName+'/'+AFtpInfo.FtpDirectory+'/'+AttachmentItem.WorkKey;
        //���Ӵ���ת��Ŀ����Ŀ¼����Ҫ������Ŀ¼���ܡ�
          ChangeDir('/');
          MakeDir(DestPath);
          ChangeDir(DestPath);
       if AttachmentItem.DirtyContent = True then      {����������}
       begin
       //������ʱĿ¼�ļ�
        SetLength(Buffer, MAX_PATH);
        SetLength(Buffer, GetTempPath(MAX_PATH, PChar(Buffer)));
        APath := Buffer;

        ForceDirectories(APath);

        if (Length(APath) > 0) and (APath[Length(APath)] <> '\') then
            APath := APath + '\';

         FullName := APath + AttachmentItem.Name;

        AttachmentItem.SaveContentToFile(FullName);
        AttachmentItem.OriginalName := FullName;
        Put(FullName,AttachmentItem.Name,True);
        result := true;
       end
       else
        begin
        //�жϸ��ļ��Ƿ����
        if FileExists(AttachmentItem.OriginalName) then
          begin
            Put(AttachmentItem.OriginalName,AttachmentItem.Name,True);
            result := true;
          end;
        end
      finally
        Disconnect;
      end;
    end;
end;
{$ENDIF}

procedure TWorkflowStudio.SendDepartmentMail(TaskIns: TTaskInstance; ADepartment: TWorkflowDepartment; EMailInfo: TEmailInformation);
var
  c: integer;
  AUser: TWorkflowUser;
begin
  for c := 0 to ADepartment.UserIds.Count - 1 do
  begin
    AUser := ADepartment.UserManager.Users.FindById(ADepartment.UserIds[c]);
    if AUser <> nil then
    begin
      if TaskIns.NeedPrjRight and (TaskIns.FlowMnDtlProjectID >'') then
        begin
            if UserManager.BelongsToProject(AUser.UserId,TaskIns.FlowMnDtlProjectID) then
                SendMail(TaskIns, AUser, EMailInfo);
        end
        else
          SendMail(TaskIns, AUser, EMailInfo);
    end;
  end;
end;

procedure TWorkflowStudio.SendJobMail(TaskIns: TTaskInstance; AJob: TWorkflowJob; EMailInfo: TEmailInformation);
var
  c: integer;
  AUser: TWorkflowUser;
begin
  for c := 0 to AJob.UserIds.Count - 1 do
  begin
    AUser := AJob.UserManager.Users.FindById(AJob.UserIds[c]);
    if AUser <> nil then
    begin
      if TaskIns.NeedPrjRight and (TaskIns.FlowMnDtlProjectID >'') then
        begin
            if UserManager.BelongsToProject(AUser.UserId,TaskIns.FlowMnDtlProjectID) then
                SendMail(TaskIns, AUser, EMailInfo);
        end
        else
          SendMail(TaskIns, AUser, EMailInfo);
    end;
  end;
end;

function TWorkflowStudio.SendFtpFile(AttachmentItem: TAttachmentItem): boolean;
var
    AFtpInfo: TFtpInformation;
begin
   result := false;

   AFtpInfo.HostName := FFtpInformation.HostName;
   AFtpInfo.UserName := FFtpInformation.UserName;
   AFtpInfo.Password := FFtpInformation.Password;
   AFtpInfo.Root := FFtpInformation.Root;
   AFtpInfo.SpaceName := FFtpInformation.SpaceName;{��Ÿ��ڵ㣬һ��������Ŀ¼}
   AFtpInfo.FtpDirectory := FFtpInformation.FtpDirectory; {��ҵ���Ŀ¼}
   AFtpInfo.Port := FFtpInformation.Port;
   AFtpInfo.Passive := FFtpInformation.Passive;
   if Assigned(FOnSendFtpFile) then
    FOnSendFtpFile(Self,AttachmentItem,AFtpInfo,result);
   if not result then
   begin
    {$IFDEF USE_INDY}
    result := IndySendFtpFile(AttachmentItem,AFtpInfo);
    {$ENDIF}
   end;
end;

function TWorkflowStudio.GetFtpFile(AttachmentItem: TAttachmentItem): boolean;
var
    AFtpInfo: TFtpInformation;
begin
   result := false;

   AFtpInfo.HostName := FFtpInformation.HostName;
   AFtpInfo.UserName := FFtpInformation.UserName;
   AFtpInfo.Password := FFtpInformation.Password;
   AFtpInfo.Root := FFtpInformation.Root;
   AFtpInfo.SpaceName := FFtpInformation.SpaceName;  {��Ÿ��ڵ㣬һ��������Ŀ¼}
   AFtpInfo.FtpDirectory := FFtpInformation.FtpDirectory; {��ҵ���Ŀ¼}
   AFtpInfo.Port := FFtpInformation.Port;
   AFtpInfo.Passive := FFtpInformation.Passive;
   if Assigned(FOnGetFtpFile) then
    FOnGetFtpFile(Self,AttachmentItem,AFtpInfo,result);
   if not result then
   begin
    {$IFDEF USE_INDY}
    result := IndyGetFtpFile(AttachmentItem,AFtpInfo);
    {$ENDIF}
   end;
end;


procedure TWorkflowStudio.WorkflowInstanceError(WI: TWorkflowInstance; ErrMsg: string; var ShowError: boolean);
begin
  if Assigned(FOnWorkInsError) then
    FOnWorkInsError(Self, WI, ErrMsg, ShowError);
end;

procedure TWorkflowStudio.SendGroupMail(TaskIns: TTaskInstance;
  AGroup: TWorkflowGroup; EMailInfo: TEmailInformation);
var
  c: integer;
  AUser: TWorkflowUser;
begin
  for c := 0 to AGroup.UserIds.Count - 1 do
  begin
    AUser := AGroup.UserManager.Users.FindById(AGroup.UserIds[c]);
    if AUser <> nil then
    begin
      if TaskIns.NeedPrjRight and (TaskIns.FlowMnDtlProjectID >'') then
        begin
            if UserManager.BelongsToProject(AUser.UserId,TaskIns.FlowMnDtlProjectID) then
                SendMail(TaskIns, AUser, EMailInfo);
        end
        else
          SendMail(TaskIns, AUser, EMailInfo);
    end;
  end;
end;

procedure TWorkflowStudio.RunFinished(WI: TWorkflowInstance);
begin
  if Assigned(FOnRunFinished) then
    FOnRunFinished(Self, WI);
end;

function TWorkflowStudio.GetCustomWorkflowDB: TCustomWorkflowDB;
begin
  if not (csDesigning in ComponentState) and (FWorkflowDB = nil) then
    raise EWorkflowException.Create('WorkflowDB property not specified.')
  else
    result := FWorkflowDB;
end;

procedure TWorkflowStudio.SetCustomWorkflowDB(const Value: TCustomWorkflowDB);
begin
  if (FWorkflowDB <> Value) then
  begin
    FWorkflowDB := Value;
    if Value <> nil then
      Value.FreeNotification(Self);
  end;
end;

{ TCustomWorkflowUserInterface }

constructor TCustomWorkflowUserInterface.Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio);
begin
  inherited Create(AOwner);
  if AWorkflowStudio <> nil then
    FWorkflowStudio := AWorkflowStudio
  else
    raise Exception.CreateFmt('WorkflowStudio parameter not specified in %s.Create.', [ClassName]);
end;

end.

