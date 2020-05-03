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

unit wsDB;

{$I wsdefs.inc}

interface

uses
  SysUtils, Classes, Dialogs, Forms, DB,  LiveDiagram,
  atDiagram, wsClasses;

type
  TwsBindary = class(TPersistent)
  private
    FKeyField: string;
    FWorkflowDB: TComponent;
    FTableName: string;
  protected
    property WorkflowDB: TComponent read FWorkflowDB;
  public
    constructor Create(AWorkflowDB: TComponent); virtual;
    destructor Destroy; override;
  published
    property TableName: string read FTableName write FTableName;
    property KeyField: string read FKeyField write FKeyField;
  end;

  TWorkInsBindary = class(TwsBindary)
  private
    FFlowMnFlowContentField: string;  {FFlowMnInsContent}
    FFlowMnBusCodeField: string;
    FFlowMnBusWindowField: string;
    FFlowMnContentField: string;
    FFlowMnCreateTimeField: string;
    FFlowMnCreatorField: string ;
    FFlowMnFKID_FlowField: string ;  {WorkDefKeyField}
    FFlowMnLastUpdateTimeField: string;
    FFlowMnRankField: string ;
    FFlowMnStateField: string;
    FFlowMnSubjectField: string;
    FFlowMoveTimeLimitField: string;
    FFlowMnReportField: string;
    FFlowMnFinishTimeField: string;
    FNextRunTimeField: string;
    FFlowMnPrjIDField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property FlowMnFlowContentField: string read FFlowMnFlowContentField write FFlowMnBusCodeField;
    property FlowMnBusCodeField: string  read FFlowMnBusCodeField write FFlowMnBusCodeField;
    property FlowMnBusWindowField: string read FFlowMnBusWindowField write FFlowMnBusWindowField;
    property FlowMnContentField: string read FFlowMnContentField write FFlowMnContentField;
    property FlowMnCreateTimeField: string read FFlowMnCreateTimeField write FFlowMnCreateTimeField;
    property FlowMnCreatorField: string  read FFlowMnCreatorField write FFlowMnCreatorField;
    property FlowMnFKID_FlowField: string  read FFlowMnFKID_FlowField write FFlowMnFKID_FlowField;
    property FlowMnLastUpdateTimeField: string read FFlowMnLastUpdateTimeField write FFlowMnLastUpdateTimeField;
    property FlowMnRankField: string read FFlowMnRankField write FFlowMnRankField;
    property FlowMnStateField: string read FFlowMnStateField write FFlowMnStateField;
    property FlowMnSubjectField: string read FFlowMnSubjectField write FFlowMnSubjectField;
    property FlowMoveTimeLimitField: string read FFlowMoveTimeLimitField write FFlowMoveTimeLimitField;
    property FlowMnReportField: string read FFlowMnReportField write FFlowMnReportField;
    property FlowMnFinishTimeField: string read FFlowMnFinishTimeField write FFlowMnFinishTimeField;
    property NextRunTimeField: string read FNextRunTimeField write FNextRunTimeField;
    property FlowMnPrjIDField: string read FFlowMnPrjIDField write FFlowMnPrjIDField;
  end;

  TWorkDefBindary = class(TwsBindary)
  private
    FFlowDnContentField: string; {FlowDnContent}
    FFlowDnNameField: string;
    FFlowDnExplanField: string;
    FFlowDnFKID_ClsField: string;
    FFlowDnIsPublicField: string;
    FFlowDnIsTermField: string;
    FFlowDnDateField: string;
    FFlowDnCreatorField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property FlowDnContentField: string read FFlowDnContentField write FFlowDnContentField; {FlowDnContent}
    property FlowDnNameField: string read FFlowDnNameField write FFlowDnNameField;
    property FlowDnExplanField: string read FFlowDnExplanField write FFlowDnExplanField;
    property FlowDnIsPublicField: string read FFlowDnIsPublicField write FFlowDnIsPublicField;
    property FlowDnIsTermField: string read FFlowDnIsTermField write FFlowDnIsTermField;
    property FlowDnDateField: string read FFlowDnDateField write FFlowDnDateField;
    property FlowDnCreatorField: string read FFlowDnCreatorField write FFlowDnCreatorField;
    property FlowDnFKID_ClsField: string read FFlowDnFKID_ClsField write FFlowDnFKID_ClsField;
  end;

  TWorkBizBindary = class(TwsBindary)
  private
    FBizFormIDField: string;
    FBizNameField: string;
    FBizTableNameField: string;
    FBizKeyField: string;
    FBizCheckField: string;
    FBizCheckerField: string;
    FBizCheckDateField: string;
    FBizEditMenuIDField: string;
    FBizCheckValueField:string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property BizFormIDField: string read FBizFormIDField write FBizFormIDField;
    property BizNameField: string read FBizNameField write FBizNameField;
    property BizTableNameField: string read FBizTableNameField write FBizTableNameField;
    property BizKeyField: string read FBizKeyField write FBizKeyField;
    property BizCheckField: string read FBizCheckField write FBizCheckField;
    property BizCheckerField: string read FBizCheckerField write FBizCheckerField;
    property BizCheckDateField: string read FBizCheckDateField write FBizCheckDateField;
    property BizEditMenuIDField: string read FBizEditMenuIDField write FBizEditMenuIDField;
    property BizCheckValueField: string read FBizCheckValueField write FBizCheckValueField;
  end;

  TTaskInsBindary = class(TwsBindary)
  private
    FFlowMnDtlFKIDField: string;  {FWorkInsKeyField}
    FFlowMnDtlNameField: string;    {FNameField}
    FFlowMnDtlSenderField: string;   {FUserIdField}
//    FFlowMnDtlDefIDField: string; {FlowMnDtlDefID}
    FFlowMnDtlTaskContentField: string;       {FTaskField}
    FFlowMnDtlStateField: string;  {FCompletedField}
    FFlowMnDtlResultsField: string;
    FFlowMnDtlChkTimeField: string;
    FFlowMnDtlCommentsField: string;      {FCommentsField}
    FFlowMnDtlLimitTimeField: string;
    FFlowMnDtlNumField: string;
    FFlowMnDtlPrintRightField: string;
    FFlowMnDtlReceiverField: string;    {FModifiedUserIdField}
    FFlowMnDtlRemarkField: string;      {FDescriptionField}
    FFlowMnDtlSendTimeField: string;    {FCreatedOnField}
    FFlowMnDtlStateChgTimeField: string;   {FModifiedOnField}
    FFlowMnDtlUserTypeField: string;
    FFlowMnDtlStepField: string;
    FFlowMnDtlNeedPrjRightField:string;
    FFlowMnDtlPrjIDField:string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property FlowMnDtlTaskContentField: string read FFlowMnDtlTaskContentField write FFlowMnDtlTaskContentField;
    property FlowMnDtlSenderField: string read FFlowMnDtlSenderField write FFlowMnDtlSenderField;
    property FlowMnDtlNameField: string read FFlowMnDtlNameField write FFlowMnDtlNameField;
    property FlowMnDtlFKIDField: string read FFlowMnDtlFKIDField write FFlowMnDtlFKIDField;
//    property FlowMnDtlDefIDField: string read FFlowMnDtlDefIDField write FFlowMnDtlDefIDField;
    property FlowMnDtlStateField: string read FFlowMnDtlStateField write FFlowMnDtlStateField;
    property FlowMnDtlResultsField: string read FFlowMnDtlResultsField write FFlowMnDtlResultsField;
    property FlowMnDtlChkTimeField: string read FFlowMnDtlChkTimeField write FFlowMnDtlChkTimeField;
    property FlowMnDtlCommentsField: string read FFlowMnDtlCommentsField write FFlowMnDtlCommentsField;
    property FlowMnDtlLimitTimeField: string read FFlowMnDtlLimitTimeField write FFlowMnDtlLimitTimeField;
    property FlowMnDtlNumField: string read FFlowMnDtlNumField write FFlowMnDtlNumField;
    property FlowMnDtlPrintRightField: string read FFlowMnDtlPrintRightField write FFlowMnDtlPrintRightField;
    property FlowMnDtlReceiverField: string read FFlowMnDtlReceiverField write FFlowMnDtlReceiverField;
    property FlowMnDtlRemarkField: string read FFlowMnDtlRemarkField write FFlowMnDtlRemarkField;
    property FlowMnDtlSendTimeField: string read FFlowMnDtlSendTimeField write FFlowMnDtlSendTimeField;
    property FlowMnDtlStateChgTimeField: string read FFlowMnDtlStateChgTimeField write FFlowMnDtlStateChgTimeField;
    property FlowMnDtlUserTypeField: string read FFlowMnDtlUserTypeField write FFlowMnDtlUserTypeField;
    property FlowMnDtlStepField: string read FFlowMnDtlStepField write FFlowMnDtlStepField;
    property FlowMnDtlNeedPrjRightField:string read FFlowMnDtlNeedPrjRightField write FFlowMnDtlNeedPrjRightField;
    property FlowMnDtlPrjIDField:string read FFlowMnDtlPrjIDField write FFlowMnDtlPrjIDField;

  end;

  TAttachmentBindary = class(TwsBindary)
  private
    FContentField: string;
    FWorkKeyField: string;
    FCreatedOnField: string;
    FObjectTypeField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property ContentField: string read FContentField write FContentField;
    property WorkKeyField: string read FWorkKeyField write FWorkKeyField;
    property CreatedOnField: string read FCreatedOnField write FCreatedOnField;
    property ObjectTypeField: string read FObjectTypeField write FObjectTypeField;
  end;

  TTaskLogBindary = class(TwsBindary)
  private
    FFlowTaskLogTaskInsIDField: string;
    FFlowTaskInsField: string;
    FFlowTaskLogDateField: string;
    FFlowTaskLogFlowInsIDField: string;
    FFlowTaskLogInfoField: string;
    FFlowTaskLogInfo2Field: string;
    FFlowTaskLogOperationField: string;
    FFlowTaskLogUserIDField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property FlowTaskLogTaskInsIDField: string read FFlowTaskLogTaskInsIDField write FFlowTaskLogTaskInsIDField;
    property FlowTaskInsField: string read FFlowTaskInsField write FFlowTaskInsField;
    property FlowTaskLogDateField: string read FFlowTaskLogDateField write FFlowTaskLogDateField;
    property FlowTaskLogFlowInsIDField: string read FFlowTaskLogFlowInsIDField write FFlowTaskLogFlowInsIDField;
    property FlowTaskLogInfoField: string read FFlowTaskLogInfoField write FFlowTaskLogInfoField;
    property FlowTaskLogInfo2Field: string read FFlowTaskLogInfo2Field write FFlowTaskLogInfo2Field;
    property FlowTaskLogOperationField: string read FFlowTaskLogOperationField write FFlowTaskLogOperationField;
    property FlowTaskLogUserIDField: string read FFlowTaskLogUserIDField write FFlowTaskLogUserIDField;
  end;
  TUserBindary = class(TwsBindary)
  private
    FUserNameField: string;
    FUserCodeField: string;
    FEmailField: string;
    FPhoneCodeField: string;
    FDepartmentField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property UserNameField: string read FUserNameField write FUserNameField;
    property UserCodeField: string read FUserCodeField write FUserCodeField;
    property EmailField: string read FEmailField write FEmailField;
    property PhoneCodeField: string read FPhoneCodeField write FPhoneCodeField;
    property DepartmentField: string read FDepartmentField write FDepartmentField;
  end;
  
  TGroupBindary = class(TwsBindary)
  private
    FGroupNameField: string;
    FIsAdminField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property GroupNameField: string read FGroupNameField write FGroupNameField;
    property IsAdminField: string read FIsAdminField write FIsAdminField;
  end;
  
  TDepartmentBindary = class(TwsBindary)
  private
    FDepartmentNameField: string;
    FDepartmentManagerIdField: string;
//    FDepartmentManagerNameField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property DepartmentNameField: string read FDepartmentNameField write FDepartmentNameField;
    property DepartmentManagerIdField: string read FDepartmentManagerIdField write FDepartmentManagerIdField;
 //   property DepartmentManagerNameField: string read FDepartmentManagerNameField write FDepartmentManagerNameField;
  end;
  
  TJobBindary = class(TwsBindary)
  private
    FJobNameField: string;
  public
    constructor Create(AWorkflowDB: TComponent); override;
  published
    property JobNameField: string read FJobNameField write FJobNameField;
  end;

  TCreateQueryEvent = procedure(Sender: TObject; SQL: string;
    var Dataset: TDataset; var Done: boolean) of object;
  TAssignSQLParamsEvent = procedure(Sender: TObject; Dataset: TDataset;
    AParams: TParams; var Done: boolean) of object;
  TExecuteQueryEvent = procedure(Sender: TObject; Dataset: TDataset;
    var Done: boolean) of object;

  TAttachmentParentType = (ptDefinition, ptInstance);

  TCustomWorkflowDB = class(TComponent)
  private
    FWorkInsBindary: TWorkInsBindary;
    FTaskInsBindary: TTaskInsBindary;
    FWorkDefBindary: TWorkDefBindary;
    FTaskLogBindary: TTaskLogBindary;
    FAttachmentBindary: TAttachmentBindary;
    FUserBindary: TUserBindary;
    FGroupBindary: TGroupBindary;
    FDepartmentBindary: TDepartmentBindary;
    FJobBindary: TJobBindary;
    FWorkBizBindary: TWorkBizBindary;
    FParams: TParams;
    FOnCreateQuery: TCreateQueryEvent;
    FOnExecuteQuery: TExecuteQueryEvent;
    FOnAssignSQLParams: TAssignSQLParamsEvent;
    FDestroyQueries: boolean;
    FStructureChecked: boolean;
    procedure WorkflowDefinitionLoadRecord(DS: TDataset; WorkDef: TWorkflowDefinition);
    procedure WorkflowInstanceLoadRecord(DS: TDataset; WorkIns: TWorkflowInstance);
    procedure TaskInstanceLoadRecord(DS: TDataset; TaskIns: TTaskInstance);
    procedure TaskLogLoadRecord(DS: TDataset; LogItem: TTaskLogItem);
    procedure AttachmentItemLoadRecord(DS: TDataset; AItem: TAttachmentItem);
    function OpenQuery(SQL: string; Params: TParams): TDataset;
    procedure ExecuteQuery(SQL: string; Params: TParams);
    function OpenSysQuery(SQL: string; Params: TParams): TDataset;
    procedure ExecuteSysQuery(SQL: string; Params: TParams);
    function MapOperationToStr(AOperation: TTaskLogOperation): string;
    function MapStrToOperation(AStr: string): TTaskLogOperation;
    function NewID:string;
    function FindNextID(ATableName, AFieldName: string): integer;
    function FindOtherNextID(ATableName, AKeyFieldName,ANumFieldName,AKeyValue: string): integer; virtual;
    procedure WorkflowDepartmentLoadRecord(DS: TDataset; WorkDepartment: TWorkflowDepartment);
    procedure WorkflowJobLoadRecord(DS: TDataset; WorkJob: TWorkflowJob);
    procedure WorkflowGroupLoadRecord(DS: TDataset; WorkGroup: TWorkflowGroup);
    procedure WorkflowUserLoadRecord(DS: TDataset; WorkUser: TWorkflowUser);
    procedure WorkflowBizLoadRecord(DS: TDataset; WorkflowBiz: TWorkflowBiz);    
    procedure CheckStructure;
  protected
    function ComponentToString(AComp: TComponent): string;
    procedure ComponentFromString(AComp: TComponent; AStr: string);
    function BlobFieldToString(AField: TField): string; virtual;
    procedure SetBlobParam(AParam: TParam; BlobStream: string); virtual;
    procedure DoOpenQuery(Dataset: TDataset);virtual;
    procedure DoOpenSysQuery(Dataset: TDataset);virtual;
    function DoCreateQuery(SQL: string): TDataset; virtual;
    procedure DoAssignSQLParams(Dataset: TDataset; AParams: TParams); virtual;
    procedure DoExecuteQuery(Dataset: TDataset); virtual;
    procedure DoExecuteSysQuery(Dataset: TDataset); virtual;
    procedure TaskInstancesDeleteByWorkIns(AWorkInsKey: string);
    procedure SaveAttachments(ADiagram: TCustomWorkflowDiagram; AType: TAttachmentParentType; AWorkKey: string);
    procedure LogTaskOperation(TaskIns: TTaskInstance; Operation: TTaskLogOperation;
      AWorkflowStudio: TCustomWorkflowStudio; Info1: string = ''; Info2: string = '');
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure WorkflowDepartmentLoad(WorkDepartment: TWorkflowDepartment);
    procedure WorkflowDepartmentLoadList(WorkDepartments: TWorkflowDepartments);
		procedure WorkflowDepartmentFillUserIds(WorkDepartment: TWorkflowDepartment);
    procedure WorkflowJobLoad(WorkJob: TWorkflowJob);
    procedure WorkflowJobLoadList(WorkJobs: TWorkflowJobs);
		procedure WorkflowJobFillUserIds(WorkJob: TWorkflowJob);
    procedure WorkflowGroupLoad(WorkGroup: TWorkflowGroup);
    procedure WorkflowGroupLoadList(WorkGroups: TWorkflowGroups);
		procedure WorkflowGroupFillUserIds(WorkGroup: TWorkflowGroup);
    procedure WorkflowUserLoad(WorkUser: TWorkflowUser);
    procedure WorkflowUserLoadList(WorkUsers: TWorkflowUsers);
    procedure TaskLogLoadList(ALogItems: TTaskLogItems;ALogFilterType: TLogFilterType;AFilterKey: string);virtual;
    procedure TaskInstanceInsert(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio); virtual;
    procedure TaskInstanceLoad(TaskIns: TTaskInstance); virtual;
    procedure TaskInstanceUpdate(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio;
      Inserting: boolean = false); virtual;
    procedure TaskInstanceLoadList(ATasks: TTaskInstanceList; AFilterType: TTaskFilterType;
      AFilterKey: string; completeState: integer; AWorkflowStudio: TCustomWorkflowStudio; AOtherCondition: string);virtual;
    procedure WorkflowInstanceInsert(WorkIns: TWorkflowInstance; AWorkflowStudio: TCustomWorkflowStudio); virtual;
    function WorkflowInstanceLoad(WorkIns: TWorkflowInstance): boolean; virtual;
    procedure WorkflowInstanceUpdate(WorkIns: TWorkflowInstance); virtual;
    procedure WorkflowInstanceDelete(WorkIns: TWorkflowInstance); virtual;
    procedure ListPendingWorkflowInstances(AKeyList: TStrings; AToDate: TDateTime); virtual;

    procedure WorkflowDefinitionDelete(WorkDef: TWorkflowDefinition); virtual;
    procedure WorkflowDefinitionInsert(WorkDef: TWorkflowDefinition); virtual;
    procedure WorkflowDefinitionUpdate(WorkDef: TWorkflowDefinition); virtual;
    procedure WorkflowDefinitionLoad(WorkDef: TWorkflowDefinition); virtual;
    procedure WorkflowDefinitionLoadList(WorkDefs: TWorkflowDefinitions;AFlowType: string='');virtual;
    procedure ListWorkflowDefinitionNames(AList: TStrings; BList:TStrings); virtual;
    //处理业务
    Procedure WorkflowBizLoad(WorkflowBiz: TWorkflowBiz);virtual;
    Procedure WorkflowBizLoadList(WorkflowBizs: TWorkflowBizs);virtual;
    Procedure WorkflowBizCheck(WorkflowBiz: TWorkflowBiz; BizKey: string;BillState:String;AWorkflowStudio: TCustomWorkflowStudio);virtual;
    Procedure WorkflowBizUpdate(WorkflowBiz: TWorkflowBiz; WorkIns: TWorkflowInstance;AWorkflowStudio: TCustomWorkflowStudio);virtual;
    //判断项目用户
    function BelongsToProject(AUserID,AProjectID:string):boolean;virtual;

    procedure AttachmentItemInsert(AItem: TAttachmentItem;
      AType: TAttachmentParentType; AWorkKey: string); virtual;
    function AttachmentItemLoad(AItem: TAttachmentItem): boolean; virtual;
    procedure AttachmentItemUpdate(AItem: TAttachmentItem;
      AType: TAttachmentParentType; AWorkKey: string); virtual;

    procedure LogTaskInfo(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio; AInfo: string; AInfo2: string='');

    property WorkInsBindary: TWorkInsBindary read FWorkInsBindary write FWorkInsBindary;
    property WorkDefBindary: TWorkDefBindary read FWorkDefBindary write FWorkDefBindary;
    property TaskInsBindary: TTaskInsBindary read FTaskInsBindary write FTaskInsBindary;
    property TaskLogBindary: TTaskLogBindary read FTaskLogBindary write FTaskLogBindary;
    property AttachmentBindary: TAttachmentBindary read FAttachmentBindary write FAttachmentBindary;
    property UserBindary: TUserBindary read FUserBindary write FUserBindary;
    property GroupBindary: TGroupBindary read FGroupBindary write FGroupBindary;
    property DepartmentBindary: TDepartmentBindary read FDepartmentBindary write FDepartmentBindary;
    property JobBindary: TJobBindary read FJobBindary write FJobBindary;
    property WorkBizBindary: TWorkBizBindary read FWorkBizBindary write FWorkBizBindary;
    property OnCreateQuery: TCreateQueryEvent read FOnCreateQuery write FOnCreateQuery;
    property OnAssignSQLParams: TAssignSQLParamsEvent read FOnAssignSQLParams write FOnAssignSQLParams;
    property OnExecuteQuery: TExecuteQueryEvent read FOnExecuteQuery write FOnExecuteQuery;
    property DestroyQueries: boolean read FDestroyQueries write FDestroyQueries;
  published
  end;

  TWorkflowDB = class(TCustomWorkflowDB)
  published 
    property WorkInsBindary;
    property WorkDefBindary;
    property TaskInsBindary;
    property TaskLogBindary;
    property AttachmentBindary;
    property UserBindary;
    property GroupBindary;
    property DepartmentBindary;
    property JobBindary;
    property OnCreateQuery;
    property OnAssignSQLParams;
    property OnExecuteQuery;
    property DestroyQueries;
  end;

function DiagramToString(Diagram: TLiveDiagram): string;
procedure DiagramFromString(Diagram: TLiveDiagram; AStr: string);
function StateToString(Diagram: TLiveDiagram): string;
procedure StateFromString(Diagram: TLiveDiagram; AStr: string);
procedure wsDBError(msg: string);

implementation

uses
  wsRes, wsMain,StrUtils,ComObj,Variants;

procedure wsDBError(msg: string);
begin
  raise Exception.Create(msg);
end;

procedure DiagramFromString(Diagram: TLiveDiagram;
  AStr: string);
var
  BS: TStringStream;
  oldName: string;
begin
  {avoid exception if string is empty}
  if Trim(AStr) = '' then
  begin
    Diagram.Clear;
    Exit;
  end;

  try
    BS := TStringStream.Create(AStr);
    try
      BS.Position := 0;
      oldName := Diagram.Owner.Name;
      Diagram.Owner.Name := wsFormName;
      Diagram.LoadFromStream(BS, true);
      Diagram.Owner.Name := oldName;
    finally
      BS.Free;
    end;
  except
    on E: Exception do
    begin
      //ShowMessage(E.Message);
      Diagram.Clear;
      Exit;
    end;
  end;
end;

function DiagramToString(Diagram: TLiveDiagram): string;
var
  BS: TStringStream;
  oldName: string;
begin
  BS := TStringStream.Create('');
  try
    oldName := Diagram.Owner.Name;
    Diagram.Owner.Name := wsFormName;
    Diagram.SaveToStream(BS, true);

    Diagram.Owner.Name := oldName;
    BS.Position := 0;
    result := BS.ReadString(MaxInt);
  finally
    BS.Free;
  end;
end;

procedure StateFromString(Diagram: TLiveDiagram; AStr: string);
var
  BS: TStringStream;
  oldName: string;
begin
  BS := TStringStream.Create(AStr);
  try
    BS.Position := 0;
    oldName := Diagram.Owner.Name;
    Diagram.Owner.Name := wsFormName;
    Diagram.LoadStateFromStream(BS, true);
    Diagram.Owner.Name := oldName;
  finally
    BS.Free;
  end;
end;

function StateToString(Diagram: TLiveDiagram): string;
var
  BS: TStringStream;
  oldName: string;
begin
  BS := TStringStream.Create('');
  try
    oldName := Diagram.Owner.Name;
    Diagram.Owner.Name := wsFormName;
    Diagram.SaveStateToStream(BS, true);

    Diagram.Owner.Name := oldName;
    BS.Position := 0;
    result := BS.ReadString(MaxInt);
  finally
    BS.Free;
  end;
end;

{ TCustomWorkflowDB }

procedure TCustomWorkflowDB.ComponentFromString(AComp: TComponent; AStr: string);
var
  CompName: string;
  MemStream: TMemoryStream;
  StrStream: TStringStream;
begin
  MemStream := TMemoryStream.Create;
  StrStream := TStringStream.Create(AStr);
  try
    CompName := AComp.Name;
    try
      AComp.Name := 'z';
      StrStream.Position := 0;
      ObjectTextToBinary(StrStream, MemStream);
      MemStream.Position := 0;
      MemStream.ReadComponent(AComp);
    finally
      AComp.Name := CompName;
    end;
  finally
    MemStream.Free;
    StrStream.Free;
  end;
end;

function TCustomWorkflowDB.ComponentToString(AComp: TComponent): string;
var
  CompName:  string;
  StrStream: TStringStream;
  MemStream: TMemoryStream;
begin
  StrStream := TStringStream.Create('');
  MemStream := TMemoryStream.Create;
  try
    CompName := AComp.Name;
    try
      AComp.Name := '';
      MemStream.WriteComponent(AComp);
      MemStream.Position := 0;
      ObjectBinaryToText(MemStream, StrStream);
      StrStream.Position := 0;
      result := StrStream.ReadString(MaxInt);
    finally
      AComp.Name := CompName;
    end;
  finally
    MemStream.Free;
    StrStream.Free;
  end;
end;

constructor TCustomWorkflowDB.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FWorkInsBindary := TWorkInsBindary.Create(Self);
  FWorkDefBindary := TWorkDefBindary.Create(Self);
  FTaskInsBindary := TTaskInsBindary.Create(Self);
  FAttachmentBindary := TAttachmentBindary.Create(Self);
  FTaskLogBindary := TTaskLogBindary.Create(Self);
  FUserBindary := TUserBindary.Create(Self);
  FGroupBindary := TGroupBindary.Create(Self);
  FDepartmentBindary := TDepartmentBindary.Create(Self);
  FJobBindary := TJobBindary.Create(Self);
  FWorkBizBindary := TWorkBizBindary.Create(Self);
  FParams := TParams.Create;
  FStructureChecked := false;
  //FAutoIncFields := false;
end;

destructor TCustomWorkflowDB.Destroy;
begin
  FParams.Free;
  FTaskLogBindary.Free;
  FWorkInsBindary.Free;
  FWorkDefBindary.Free;
  FTaskInsBindary.Free;
  FAttachmentBindary.Free;
  FUserBindary.Free;
  FGroupBindary.Free;
  FDepartmentBindary.Free;
  FJobBindary.Free;
  FWorkBizBindary.Free;
  inherited;
end;

procedure TCustomWorkflowDB.ExecuteQuery(SQL: string; Params: TParams);
var
  Done: boolean;
  DS: TDataset;
begin
  if not FStructureChecked then
    CheckStructure;

  DS := nil;

  //Create the query
  Done := false;
  if Assigned(FOnCreateQuery) then
    FOnCreateQuery(Self, SQL, DS, Done);
  if not Done then
    DS := DoCreateQuery(SQL);

  //Assign the params
  Done := false;
  if Assigned(FOnAssignSQLParams) then
    FOnAssignSQLParams(Self, DS, Params, Done);
  if not Done then
    DoAssignSQLParams(DS, Params);

  //Execute the query
  Done := false;
  if Assigned(FOnExecuteQuery) then
    FOnExecuteQuery(Self, DS, Done);
  if not Done then
    DoExecuteQuery(DS);

  if DS <> nil then
    DS.Free;
end;

procedure TCustomWorkflowDB.DoExecuteQuery(Dataset: TDataset);
begin
end;

function TCustomWorkflowDB.OpenQuery(SQL: string; Params: TParams): TDataset;
var
  Done: boolean;
begin
  if not FStructureChecked then
    CheckStructure;

  result := nil;

  //Create the query
  Done := false;
  if Assigned(FOnCreateQuery) then
    FOnCreateQuery(Self, SQL, result, Done);
  if not Done then
    result := DoCreateQuery(SQL);

  //Assign the params
  Done := false;
  if Assigned(FOnAssignSQLParams) then
    FOnAssignSQLParams(Self, result, Params, Done);
  if not Done then
    DoAssignSQLParams(result, Params);

  if result <> nil then
    DoOpenQuery(result);
end;

procedure TCustomWorkflowDB.DoOpenQuery(Dataset: TDataset);
begin
    Dataset.Open;
end;

function TCustomWorkflowDB.DoCreateQuery(SQL: string): TDataset;
begin
  result := nil;
end;
procedure TCustomWorkflowDB.ExecuteSysQuery(SQL: string; Params: TParams);
var
  Done: boolean;
  DS: TDataset;
begin
  if not FStructureChecked then
    CheckStructure;

  DS := nil;

  //Create the query
  Done := false;
  if Assigned(FOnCreateQuery) then
    FOnCreateQuery(Self, SQL, DS, Done);
  if not Done then
    DS := DoCreateQuery(SQL);

  //Assign the params
  Done := false;
  if Assigned(FOnAssignSQLParams) then
    FOnAssignSQLParams(Self, DS, Params, Done);
  if not Done then
    DoAssignSQLParams(DS, Params);

  //Execute the query
  Done := false;
  if Assigned(FOnExecuteQuery) then
    FOnExecuteQuery(Self, DS, Done);
  if not Done then
    DoExecuteSysQuery(DS);

  if DS <> nil then
    DS.Free;
end;

procedure TCustomWorkflowDB.DoExecuteSysQuery(Dataset: TDataset);
begin
end;

function TCustomWorkflowDB.OpenSysQuery(SQL: string; Params: TParams): TDataset;
var
  Done: boolean;
begin
  if not FStructureChecked then
    CheckStructure;

  result := nil;

  //Create the query
  Done := false;
  if Assigned(FOnCreateQuery) then
    FOnCreateQuery(Self, SQL, result, Done);
  if not Done then
    result := DoCreateQuery(SQL);

  //Assign the params
  Done := false;
  if Assigned(FOnAssignSQLParams) then
    FOnAssignSQLParams(Self, result, Params, Done);
  if not Done then
    DoAssignSQLParams(result, Params);

  if result <> nil then
    DoOpenSysQuery(result);
end;

procedure TCustomWorkflowDB.DoOpenSysQuery(Dataset: TDataset);
begin
    Dataset.Open;
end;

procedure TCustomWorkflowDB.TaskInstanceInsert(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio);
var
  SQL: string;
  CreatedOn: TDateTime;
begin
  CreatedOn := AWorkflowStudio.GetNow;
  with TaskInsBindary do
  begin
    {Insert taskinstance unique key}
   { TaskIns.Key := IntToStr(FindNextID(TableName, KeyField));
    SQL := Format('INSERT INTO %s (%s, %s) Values (:id, :createdon)',
      [TableName, KeyField, CreatedOnField]);
    FParams.Clear;
    FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(TaskIns.Key);
    FParams.CreateParam(ftDateTime, 'createdon', ptInput).Value := CreatedOn; }
    TaskIns.FlowMnDtlNum :=FindOtherNextID(TableName,FlowMnDtlFKIDField,FlowMnDtlNumField,TaskIns.WorkInsKey);
    TaskIns.Key :=NewID;

    SQL := Format('INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) Values '
      +'(:FlowMnDtlPKID, :FlowMnDtlFKID, :FlowMnDtlNum, :FlowMnDtlSender, :FlowMnDtlSendTime, '
      +':FlowMnDtlComments, :FlowMnDtlState, :FlowMnDtlResults, :FlowMnDtlPrintRight, :FlowMnDtlRemark, '
      +':FlowMnDtlName, :FlowMnDtlTaskContent, :FlowMnDtlUserType, :FlowMnDtlStep, :FlowMnDtlNeedPrjRight, :FFlowMnDtlPrjID)',
      [TableName, KeyField, FlowMnDtlFKIDField, FlowMnDtlNumField, FlowMnDtlSenderField,
      FlowMnDtlSendTimeField, FlowMnDtlCommentsField, FlowMnDtlStateField,
      FlowMnDtlResultsField, FlowMnDtlPrintRightField, FlowMnDtlRemarkField,
      FlowMnDtlNameField, FlowMnDtlTaskContentField, FlowMnDtlUserTypeField,
      FlowMnDtlStepField, FlowMnDtlNeedPrjRightField, FFlowMnDtlPrjIDField]);

    FParams.Clear;
    FParams.CreateParam(ftString, 'FlowMnDtlPKID', ptInput).AsString := TaskIns.Key;
    FParams.CreateParam(ftString, 'FlowMnDtlFKID', ptInput).AsString := TaskIns.WorkInsKey;
    FParams.CreateParam(ftInteger, 'FlowMnDtlNum', ptInput).AsInteger := TaskIns.FlowMnDtlNum;
    FParams.CreateParam(ftString, 'FlowMnDtlSender', ptInput).AsString := TaskIns.UserID;
    FParams.CreateParam(ftString, 'FlowMnDtlSendTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn);
    FParams.CreateParam(ftString, 'FlowMnDtlComments', ptInput).AsString := TaskIns.Comments;
    FParams.CreateParam(ftInteger, 'FlowMnDtlState', ptInput).AsInteger := 0;
    FParams.CreateParam(ftString, 'FlowMnDtlResults', ptInput).AsString := TaskIns.Status;
    FParams.CreateParam(ftInteger, 'FlowMnDtlPrintRight', ptInput).AsInteger := Ord(TaskIns.CanPrint);
    FParams.CreateParam(ftString, 'FlowMnDtlRemark', ptInput).AsString := TaskIns.FlowMnDtlRemark;
//    FParams.CreateParam(ftString, 'FlowMnDtlName', ptInput).AsString := TaskIns.TaskDef.Name;
    FParams.CreateParam(ftString, 'FlowMnDtlName', ptInput).AsString := TaskIns.TaskDef.Subject;
    FParams.CreateParam(ftMemo, 'FlowMnDtlTaskContent', ptInput).AsMemo := ComponentToString(TaskIns);
//    SetBlobParam(FParams.CreateParam(ftBlob, 'FlowMnDtlTaskContent', ptInput), ComponentToString(TaskIns));
    FParams.CreateParam(ftInteger, 'FlowMnDtlUserType', ptInput).AsInteger := Ord(TaskIns.AssignedUserType);
    FParams.CreateParam(ftInteger, 'FlowMnDtlStep', ptInput).AsInteger := TaskIns.FlowMnDtlStepDefin;
    FParams.CreateParam(ftInteger, 'FlowMnDtlNeedPrjRight', ptInput).AsInteger := Ord(TaskIns.NeedPrjRight);
    FParams.CreateParam(ftString, 'FFlowMnDtlPrjID', ptInput).AsString := TaskIns.FlowMnDtlProjectID;

{    SQL := 'Insert Into '+TableName+'('
          +KeyField
          +','+FlowMnDtlFKIDField
          +','+FlowMnDtlNumField
          +','+FlowMnDtlSenderField
          +','+FlowMnDtlSendTimeField
//          +','+FlowMnDtlLimitTimeField
          +','+FlowMnDtlCommentsField
          +','+FlowMnDtlStateField
          +','+FlowMnDtlResultsField
          +','+FlowMnDtlPrintRightField
          +','+FlowMnDtlRemarkField
          +','+FlowMnDtlNameField
          +','+FlowMnDtlTaskContentField
//          +','+FlowMnDtlDefIDField
          +','+FlowMnDtlUserTypeField
          +','+FlowMnDtlStepField
          +','+FlowMnDtlNeedPrjRightField
          +','+FFlowMnDtlPrjIDField
          +') Values ('
          + QuotedStr(TaskIns.Key)
          +','+QuotedStr(TaskIns.WorkInsKey)
          +','+IntToStr(TaskIns.FlowMnDtlNum)
          +','+QuotedStr(TaskIns.UserID)
          +','+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
//          +','+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',TaskIns.FlowMnDtlLimitTime))
          +','+QuotedStr(TaskIns.Comments)
          +',0'
          +','+QuotedStr(TaskIns.Status)
          +','+IntToStr(Ord(TaskIns.CanPrint))
          +','+QuotedStr(TaskIns.FlowMnDtlRemark)
          +','+QuotedStr(TaskIns.TaskDef.Name)
          +','+QuotedStr(ComponentToString(TaskIns))
//          +','+QuotedStr(TaskIns.DefinitionKey)
          +','+IntToStr(Ord(TaskIns.AssignedUserType))
          +','+IntToStr(TaskIns.FlowMnDtlStepDefin)
          +','+IntToStr(Ord(TaskIns.NeedPrjRight))
          +','+QuotedStr(TaskIns.FlowMnDtlProjectID)
          +')';    }
    ExecuteQuery(SQL, FParams);

    {Save a log for the task creation}
    LogTaskOperation(TaskIns, tlCreate, AWorkflowStudio);

    {if task was inserted then call OnTaskCreated event}
    if Assigned(AWorkflowStudio.OnTaskCreated) then
            AWorkflowStudio.OnTaskCreated(AWorkflowStudio, TaskIns);

    {Save other taskinstance fields}
   // TaskInstanceUpdate(TaskIns, AWorkflowStudio, true);
  end;
end;

function TCustomWorkflowDB.NewID: string ;
var
  guidstr: String;
begin
  guidstr:=CreateClassID;
  Result:=MidStr(guidstr,2,36);
end;

procedure TCustomWorkflowDB.TaskInstanceLoad(TaskIns: TTaskInstance);
var
  SQL: string;
  DS: TDataset;
begin
  {SQL := Format('SELECT * from %s WHERE %s = :id',
    [TaskInsBindary.TableName, TaskInsBindary.KeyField]);
  FParams.Clear;
  FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(TaskIns.Key); }

  SQL := Format('SELECT * from %s WHERE %s = %s',
    [TaskInsBindary.TableName, TaskInsBindary.KeyField,QuotedStr(TaskIns.Key)]);
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      TaskInstanceLoadRecord(DS, TaskIns)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [TaskInsBindary.TableName, TaskIns.Key]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.TaskInstanceLoadList(ATasks: TTaskInstanceList; AFilterType: TTaskFilterType; AFilterKey: string;
     completeState: integer; AWorkflowStudio: TCustomWorkflowStudio; AOtherCondition: string);
var
  SQL: string;
  DS: TDataset;
  UserKeys: TStringList;
  AllUserKeys: TStringList;
  UserCondition: string;
  c: integer;
  CompletedCondition: string;
  TypesIds: TStringList;   {GroupIds JobIds DepartmentIds}
  AUser: TWorkflowUser;
  curUserId: string;
   FTask:TTaskInstance;
begin
  DS := nil;

  {build incomplete condition}
  if completeState=1 then
    CompletedCondition := Format('%s = 1', [TaskInsBindary.FlowMnDtlStateField])
  else if completeState=0 then
    CompletedCondition := Format('%s = 0', [TaskInsBindary.FlowMnDtlStateField])
  else
    CompletedCondition :='0=0';
  if AOtherCondition<>'' then
     CompletedCondition := CompletedCondition+ ' and '+ AOtherCondition;

  Case AFilterType of
    tfWorkIns:
      begin
        SQL := Format('SELECT * FROM %s WHERE %s = %s AND (%s) order by %s',
          [TaskInsBindary.TableName, TaskInsBindary.FlowMnDtlFKIDField,QuotedStr(AFilterKey), CompletedCondition,TaskInsBindary.FlowMnDtlNumField]);
        FParams.Clear;
      //  FParams.CreateParam(ftInteger, 'workinskey', ptInput).AsInteger := StrToInt(AFilterKey);
     //   if OnlyIncomplete then
     //     FParams.CreateParam(ftString, 'completed', ptInput).AsString := 'F';
        DS := OpenQuery(SQL, FParams);
      end;
    tfUser, tfUserList:
      begin
        {Get users key values and build condition for users}
        UserKeys := TStringList.Create;

        try
          {Put the correct keys in the userkeys}
          Case AFilterType of
            tfUser:
              begin
              UserKeys.Add(AFilterKey);
              if AWorkflowStudio.UserManager.IsSameUser(AFilterKey,AWorkflowStudio.UserManager.LoggedUserId) and AWorkflowStudio.UserManager.BelongsToAdminGroup(AWorkflowStudio.UserManager.LoggedUserId) then
                UserKeys.Clear;          {是管理员}
              end;
            tfUserList: UserKeys.CommaText := AFilterKey;
          end;
          {For each user id, include the ids of the groups for which the user id belongs to.
           Do that only for GroupAssignmentMode = gamSingleTask}
          //找出用户列表归属于角色的id列表
            TypesIds := TStringList.Create;
            AllUserKeys := TStringList.Create;
            try
              begin
              AllUserKeys.AddStrings(UserKeys);
              for c := 0 to UserKeys.Count - 1 do
              begin
                AUser := AWorkflowStudio.UserManager.Users.FindById(UserKeys[c]);
                if AUser <> nil then
                begin
                //Groupid
                  TypesIds.Clear;
                  AUser.FillGroupIds(TypesIds);
                  AllUserKeys.AddStrings(TypesIds);
                  //Jobid
                  TypesIds.Clear;
                  AUser.FillJobIds(TypesIds);
                  AllUserKeys.AddStrings(TypesIds);
                  //Departmentid
                  TypesIds.Clear;
                  AUser.FillDepartmentIds(TypesIds);
                  AllUserKeys.AddStrings(TypesIds);
                end;
              end;
               UserKeys.Clear;    //需要增加清理重复的
               curUserId :='';    //一般编码guid不会重复
               AllUserKeys.CaseSensitive := False;
               AllUserKeys.Sort;  //先排序一下
               for c := 0 to AllUserKeys.Count - 1 do
               begin
                if(UpperCase(AllUserKeys.Strings[c])<>UpperCase(curUserID)) then
                begin
                  curUserID := AllUserKeys.Strings[c];
                  UserKeys.Add(curUserID);
                end;
               end;
             end;
            finally
              TypesIds.Free;
              AllUserKeys.Free;
            end;
           //找出用户列表归属于角色的id列表


          {Fill in the condition for the user keys, in the format
           (UserId = :userid0) OR (UserId = :userid1) ....}
          UserCondition := '';
          for c := 0 to UserKeys.Count - 1 do
          begin
            if UserCondition <> '' then
              UserCondition := UserCondition + ' OR ';
            UserCondition := UserCondition + Format('(%s = %s)',
              [TaskInsBindary.FFlowMnDtlSenderField, QuotedStr(UserKeys[c])]);
          end;
          if UserCondition = '' then
            UserCondition := '0=0';

          {Build the SQL statement}
          SQL := Format('SELECT * FROM %s WHERE (%s) AND (%s) order by %s DESC',
            [TaskInsBindary.TableName, UserCondition, CompletedCondition,TaskInsBindary.FlowMnDtlSendTimeField]);

          {Fill in the param values, including the values of user keys in params userid0, userid1, etc.}
          FParams.Clear;
         { for c := 0 to UserKeys.Count - 1 do
            FParams.CreateParam(ftString, Format('userid%d', [c]), ptInput).AsString := UserKeys[c];
          if OnlyIncomplete then
            FParams.CreateParam(ftString, 'completed', ptInput).AsString := 'F';  }
          DS := OpenQuery(SQL, FParams);
        finally
          UserKeys.Free;
        end;
      end;
  end;

  if DS = nil then Exit;
  try
    DS.First;

    ATasks.Clear;
    while not DS.EOF do
    begin
      FTask :=ATasks.Add.Task;
      TaskInstanceLoadRecord(DS, FTask);
      if FTask.NeedPrjRight and (FTask.FlowMnDtlProjectID >'') then
      begin   //清楚无项目权限的节点
           if AWorkflowStudio.UserManager.BelongsToProject(AWorkflowStudio.UserManager.LoggedUserId,FTask.FlowMnDtlProjectID)=False then
           begin
              ATasks.Delete(ATasks.Count-1);
              //ATasks.RemoveItem(FTask);
              FTask:=nil;
           end;
      end;
      DS.Next;
    end;
  finally
    DS.Close;
    if FDestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.TaskInstanceLoadRecord(DS: TDataset; TaskIns: TTaskInstance);
begin
  With TaskInsBindary do
  begin
    ComponentFromString(TaskIns, DS.FieldByName(FlowMnDtlTaskContentField).AsString);
    TaskIns.Key := DS.FieldByName(KeyField).AsString;
    TaskIns.UserID := DS.FieldByName(FlowMnDtlSenderField).AsString;
    TaskIns.TaskDef.Name := DS.FieldByName(FlowMnDtlNameField).AsString;
    TaskIns.WorkInsKey := Ds.FieldByName(FlowMnDtlFKIDField).AsString;
 //   TaskIns.DefinitionKey := Ds.FieldByName(FlowMnDtlDefIDField).AsString;
    TaskIns.CreatedOn := StrToDateTime(DS.FieldByName(FlowMnDtlSendTimeField).AsString,UserfmtSettings);
    TaskIns.Comments := DS.FieldByName(FlowMnDtlCommentsField).AsString;
    TaskIns.FlowMnDtlRemark :=DS.FieldByName(FlowMnDtlRemarkField).AsString;
    if DS.FieldByName(FlowMnDtlPrintRightField).AsInteger=1 then
       TaskIns.CanPrint :=True
    else
       TaskIns.CanPrint :=False;
    //TaskIns.AssignedUserType := TAssignedUserType(DS.FieldByName(FlowMnDtlUserTypeField).AsInteger);
    if TaskIns.Completed  then
      TaskIns.FlowMnDtlChkTime := StrToDateTime(DS.FieldByName(FlowMnDtlChkTimeField).AsString,UserfmtSettings);

    //TaskIns.FlowMnDtlLimitTime := StrToDateTime(DS.FieldByName(FlowMnDtlLimitTimeField).AsString,UserfmtSettings);
    TaskIns.FlowMnDtlNum := DS.FieldByName(FlowMnDtlNumField).AsInteger;
    TaskIns.FlowMnDtlReceiver := DS.FieldByName(FlowMnDtlReceiverField).AsString;
    TaskIns.FlowMnDtlStepDefin :=DS.FieldByName(FFlowMnDtlStepField).AsInteger;
    //if DS.FieldByName(FlowMnDtlStateChgTimeField).IsNull=False then
     //  TaskIns.FlowMnDtlStateChgTime := StrToDateTime(DS.FieldByName(FlowMnDtlStateChgTimeField).AsString,UserfmtSettings);
  end;
end;

procedure TCustomWorkflowDB.TaskInstancesDeleteByWorkIns(AWorkInsKey: string);
var
  SQL: string;
begin
 { SQL := Format('DELETE FROM %s WHERE %s = :workinskey',
    [TaskInsBindary.TableName, TaskInsBindary.WorkInsKeyField]);}
  SQL := Format('DELETE FROM %s WHERE %s = %s',
    [TaskInsBindary.TableName, TaskInsBindary.FlowMnDtlFKIDField,QuotedStr(AWorkInsKey)]);
  FParams.Clear;
  //FParams.CreateParam(ftInteger, 'workinskey', ptInput).AsInteger := StrToInt(AWorkInsKey);
  ExecuteQuery(SQL, FParams);
end;

procedure TCustomWorkflowDB.TaskInstanceUpdate(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio;
  Inserting: boolean = false);
var
  SQL: string;
  TaskInsOld: TTaskInstance;
  NowDate: TDateTime;
  WorkflowBiz: TWorkflowBiz;
  workIns: TWorkflowInstance;
begin
  With TaskInsBindary do
  begin
    if TaskIns.Key <> '' then
    begin
      TaskInsOld := TTaskInstance.Create(nil);
      try
        {Load the current record into a temporary instance for comparing}
        if not Inserting then
        begin
          TaskInsOld.Key := TaskIns.Key;
          TaskInstanceLoad(TaskInsOld);
        end;

        NowDate := AWorkflowStudio.GetNow;
        {Save the current taskinstance in the database}
{       SQL := 'Update '+TableName+' Set '+
            FlowMnDtlFKIDField+'='+QuotedStr(TaskIns.WorkInsKey)
//          +','+FlowMnDtlDefIDField+'='+QuotedStr(TaskIns.DefinitionKey)
//          +','+FlowMnDtlLimitTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',TaskIns.FlowMnDtlLimitTime))
          +','+FlowMnDtlCommentsField+'='+QuotedStr(TaskIns.Comments)
          +','+FlowMnDtlResultsField+'='+QuotedStr(TaskIns.Status)
          +','+FlowMnDtlReceiverField+'='+QuotedStr(AWorkflowStudio.UserManager.LoggedUserId);
          //状态改变
          if CompareText(TaskIns.Status, TaskInsOld.Status) <> 0 then
            begin
              TaskIns.FlowMnDtlStateChgTime := NowDate;
              SQL :=SQL+','+FlowMnDtlStateChgTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',NowDate))
            end;
           //过期时间
          if TaskIns.FlowMnDtlLimitTime>0 then
           SQL :=SQL+','+FlowMnDtlLimitTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',TaskIns.FlowMnDtlLimitTime));
           //完成时间
          if TaskIns.Completed then
          begin
              TaskIns.FlowMnDtlChkTime := NowDate;
            SQL :=SQL+','+FlowMnDtlChkTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',NowDate))
                   +','+FlowMnDtlStateField+'=1';
          end;
          SQL :=SQL+','+FlowMnDtlTaskContentField+'='+QuotedStr(ComponentToString(TaskIns))
          +' WHERE '+KeyField+'='+QuotedStr(TaskIns.Key);
 }
        SQL := 'Update '+TableName+' Set '+
            FlowMnDtlFKIDField+'=:FlowMnDtlFKID, '
           +FlowMnDtlCommentsField+'=:FlowMnDtlComments, '
           +FlowMnDtlResultsField+'=:FlowMnDtlResults, '
           +FlowMnDtlReceiverField+'=:FlowMnDtlReceiver ';
       FParams.Clear;
       FParams.CreateParam(ftString, 'FlowMnDtlFKID', ptInput).AsString := TaskIns.WorkInsKey;
       FParams.CreateParam(ftString, 'FlowMnDtlComments', ptInput).AsString := TaskIns.Comments;
       FParams.CreateParam(ftString, 'FlowMnDtlResults', ptInput).AsString := TaskIns.Status;
       FParams.CreateParam(ftString, 'FlowMnDtlReceiver', ptInput).AsString := AWorkflowStudio.UserManager.LoggedUserId;
          //状态改变
          if CompareText(TaskIns.Status, TaskInsOld.Status) <> 0 then
            begin
              TaskIns.FlowMnDtlStateChgTime := NowDate;
              SQL :=SQL+','+FlowMnDtlStateChgTimeField+'=:FlowMnDtlStateChgTime';
              FParams.CreateParam(ftString, 'FlowMnDtlStateChgTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',NowDate);
            end;
           //过期时间
          if TaskIns.FlowMnDtlLimitTime>0 then
            begin
              SQL :=SQL+','+FlowMnDtlLimitTimeField+'=:FlowMnDtlLimitTime';
              FParams.CreateParam(ftString, 'FlowMnDtlLimitTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',TaskIns.FlowMnDtlLimitTime);
            end;
           //完成时间
          if TaskIns.Completed then
          begin
              TaskIns.FlowMnDtlChkTime := NowDate;
            SQL :=SQL+','+FlowMnDtlChkTimeField+'=:FlowMnDtlChkTime'
                   +','+FlowMnDtlStateField+'=1';
            FParams.CreateParam(ftString, 'FlowMnDtlChkTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',NowDate);
          end;
          SQL :=SQL+','+FlowMnDtlTaskContentField+'=:FlowMnDtlTaskContent'
          +' WHERE '+KeyField+'=:KeyField';
          FParams.CreateParam(ftMemo, 'FlowMnDtlTaskContent', ptInput).AsMemo := ComponentToString(TaskIns);
          FParams.CreateParam(ftString, 'KeyField', ptInput).AsString := TaskIns.Key;
        ExecuteQuery(SQL, FParams);

        {Now compare the new task instance with the old one. If Inserting is true, it means
         that this update is to complete an insert operation, so there is no need for a log.
         If the status has change, log it}
        if CompareText(TaskIns.Status, TaskInsOld.Status) <> 0 then
          begin
            LogTaskOperation(TaskIns, tlStatusChange, AWorkflowStudio, TaskInsOld.Status, TaskIns.Status);

            {if the status was changed and the task is completed, it means that the task
             was just finished now - fire the event}
            if Assigned(AWorkflowStudio.OnTaskFinished) then
              AWorkflowStudio.OnTaskFinished(AWorkflowStudio, TaskIns);
              
            { 任务结束时，更新变量值到业务数据记录中          }
            workIns := TWorkflowInstance.Create(nil,AWorkflowStudio);
            try
                 workIns.Key := TaskIns.WorkInsKey;
                 AWorkflowStudio.WorkflowManager.LoadWorkflowInstance(workIns);
                 WorkflowBiz := TWorkflowBiz.Create(nil);
                  try
                    WorkflowBiz.BizFormID := workIns.FlowMnBusWindow;
                    AWorkflowStudio.WorkflowManager.LoadWorkflowBiz(WorkflowBiz);
                    AWorkflowStudio.WorkflowManager.UpdateWorkflowBiz(WorkflowBiz,workIns);
                  finally
                    WorkflowBiz.Free;
                  end;
            finally
              workIns.Free;
            end;
          end else
          LogTaskOperation(TaskIns, tlUpdate, AWorkflowStudio);

      finally
        TaskInsOld.Free;
      end;
    end else
      wsDBError(Format(_str(SErrorUpdateEmptyKey), [TableName]));
  end;
end;

procedure TCustomWorkflowDB.WorkflowDefinitionDelete(
  WorkDef: TWorkflowDefinition);
var
  SQL: string;
begin
  if WorkDef.Key <> '' then
  begin
    SQL := Format('DELETE FROM %s WHERE %s = %s',
      [WorkDefBindary.TableName, WorkDefBindary.KeyField,QuotedStr(WorkDef.Key)]);
    FParams.Clear;
   // FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkDef.Key);
    ExecuteQuery(SQL, FParams);
  end else
    wsDBError(Format(_str(SErrorDeleteEmptyKey), ['工作流定义']));
end;

procedure TCustomWorkflowDB.WorkflowDefinitionInsert(WorkDef: TWorkflowDefinition);
var
  SQL: string;
begin
  With WorkDefBindary do
  begin
    {Insert workdefinition unique key}
    //WorkDef.Key := IntToStr(FindNextID(TableName, KeyField));
    WorkDef.Key := NewID;
{*    SQL := Format('INSERT INTO %s (%s,%s,%s,%s,%s,%s,%s,%s,%s) Values (%s,%s,%s,%s,%s,%s,%d,%d,%s)',
      [TableName, KeyField,FlowDnNameField,FlowDnFKID_ClsField,FlowDnCreatorField,FlowDnExplanField,FlowDnDateField,FlowDnIsPublicField,FlowDnIsTermField,FlowDnContentField,
      QuotedStr(WorkDef.Key),QuotedStr(WorkDef.Name),QuotedStr(WorkDef.WorkType),QuotedStr(WorkDef.WorkflowStudio.UserManager.LoggedUserId),
      QuotedStr(WorkDef.DisplayName),QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',Now)),WorkDef.FlowIsPublic,WorkDef.FlowIsTerm,QuotedStr(DiagramToString(WorkDef.Diagram))]);*}
{    SQL := Format('INSERT INTO %s (%s,%s,%s,%s,%s,%s,%s,%s) Values (%s,%s,%s,%s,%s,%s,%d,%d)',
      [TableName, KeyField,FlowDnNameField,FlowDnFKID_ClsField,FlowDnCreatorField,FlowDnExplanField,FlowDnDateField,FlowDnIsPublicField,FlowDnIsTermField,
      QuotedStr(WorkDef.Key),QuotedStr(WorkDef.Name),QuotedStr(WorkDef.WorkType),QuotedStr(WorkDef.WorkflowStudio.UserManager.LoggedUserId),
      QuotedStr(WorkDef.DisplayName),QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',Now)),WorkDef.FlowIsPublic,WorkDef.FlowIsTerm]);
    FParams.Clear;
    //FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkDef.Key);   }

    SQL := Format('INSERT INTO %s (%s,%s,%s,%s,%s,%s,%s,%s) Values (:FlowDnPKID,:FlowDnName,:FlowDnFKID_Cls,:FlowDnCreator,:FlowDnExplan,:FlowDnDate,:FlowDnIsPublic,:FlowDnIsTerm)',
      [TableName, KeyField,FlowDnNameField,FlowDnFKID_ClsField,FlowDnCreatorField,FlowDnExplanField,FlowDnDateField,FlowDnIsPublicField,FlowDnIsTermField]);


//      QuotedStr(WorkDef.Key),QuotedStr(WorkDef.Name),QuotedStr(WorkDef.WorkType),QuotedStr(WorkDef.WorkflowStudio.UserManager.LoggedUserId),
//      QuotedStr(WorkDef.DisplayName),QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',Now)),WorkDef.FlowIsPublic,WorkDef.FlowIsTerm]);
    FParams.Clear;
    FParams.CreateParam(ftString, 'FlowDnPKID', ptInput).AsString := WorkDef.Key;
    FParams.CreateParam(ftString, 'FlowDnName', ptInput).AsString := WorkDef.Name;
    FParams.CreateParam(ftString, 'FlowDnFKID_Cls', ptInput).AsString := WorkDef.WorkType;
    FParams.CreateParam(ftString, 'FlowDnCreator', ptInput).AsString := WorkDef.WorkflowStudio.UserManager.LoggedUserId;
    FParams.CreateParam(ftString, 'FlowDnExplan', ptInput).AsString := WorkDef.DisplayName;
    FParams.CreateParam(ftString, 'FlowDnDate', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',Now);
    FParams.CreateParam(ftInteger, 'FlowDnIsPublic', ptInput).AsInteger := WorkDef.FlowIsPublic;
    FParams.CreateParam(ftInteger, 'FlowDnIsTerm', ptInput).AsInteger := WorkDef.FlowIsTerm;
    ExecuteQuery(SQL, FParams);

    {Save other workflow definition fields}
    WorkflowDefinitionUpdate(WorkDef);
  end;
end;

procedure TCustomWorkflowDB.WorkflowDefinitionLoad(WorkDef: TWorkflowDefinition);
var
  SQL: string;
  DS: TDataset;
begin
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [WorkDefBindary.TableName, WorkDefBindary.KeyField,QuotedStr(WorkDef.Key)]);
  FParams.Clear;
  //FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkDef.Key);
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowDefinitionLoadRecord(DS, WorkDef)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [WorkDefBindary.TableName, WorkDef.Key]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDefinitionLoadList(WorkDefs: TWorkflowDefinitions;AFlowType: string='');
var
  SQL: string;
  DS: TDataset;
begin
  FParams.Clear;
  if AFlowType<>'' then
     SQL:=Format('SELECT * from %s where %s=%s', [WorkDefBindary.TableName,WorkDefBindary.FFlowDnFKID_ClsField,QuotedStr(AFlowType)])
  else
    SQL:=Format('SELECT * from %s', [WorkDefBindary.TableName]);

  DS := OpenQuery(SQL, FParams);
  try
    DS.First;
    WorkDefs.Clear;
    while not DS.EOF do
    begin
      WorkflowDefinitionLoadRecord(DS, WorkDefs.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

Procedure TCustomWorkflowDB.WorkflowBizLoad(WorkflowBiz: TWorkflowBiz);
var
  SQL: string;
  DS: TDataset;
begin
  if Trim(WorkflowBiz.BizMenuID) >'' then
  begin
    SQL := Format('SELECT * from %s WHERE %s = %s',
    [WorkBizBindary.TableName, WorkBizBindary.KeyField,WorkflowBiz.BizMenuID]);
  end else
  begin
   SQL := Format('SELECT * from %s WHERE %s = %s',
    [WorkBizBindary.TableName, WorkBizBindary.BizFormIDField,WorkflowBiz.BizFormID]);
  end;
  FParams.Clear;
  DS := OpenSysQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowBizLoadRecord(DS, WorkflowBiz)
    else
     begin
      if Trim(WorkflowBiz.BizMenuID) >'' then
        wsDBError(Format('数据表%s,找不到菜单ID为%s的记录', [WorkBizBindary.TableName, WorkflowBiz.BizMenuID]))
      else
        wsDBError(Format('数据表%s,找不到窗体ID为%s的记录', [WorkBizBindary.TableName, WorkflowBiz.BizFormID]));
     end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowBizLoadRecord(DS: TDataset; WorkflowBiz: TWorkflowBiz);
begin
      with WorkBizBindary do
      begin
         WorkflowBiz.BizName := DS.FieldByName(BizNameField).AsString;
         WorkflowBiz.BizMenuID := DS.FieldByName(KeyField).AsString;
         WorkflowBiz.BizFormID := DS.FieldByName(BizFormIDField).AsString;
         WorkflowBiz.BizTableName := DS.FieldByName(BizTableNameField).AsString;
         WorkflowBiz.BizKey := DS.FieldByName(BizKeyField).AsString;
         WorkflowBiz.BizCheck := DS.FieldByName(BizCheckField).AsString;
         if(not DS.FieldByName(BizCheckerField).IsNull) then
            WorkflowBiz.BizChecker := DS.FieldByName(BizCheckerField).AsString;
         if(not DS.FieldByName(BizCheckDateField).IsNull) then
            WorkflowBiz.BizCheckDate := DS.FieldByName(BizCheckDateField).AsString;
         if(not DS.FieldByName(BizEditMenuIDField).IsNull) then
            WorkflowBiz.BizEditMenuID := DS.FieldByName(BizEditMenuIDField).AsString;
          //查找CheckValue字段是否存在,不存在赋默认值1
          if Assigned(Ds.FindField(BizCheckValueField)) then
             WorkflowBiz.BizCheckValue :=DS.FieldByName(BizCheckValueField).AsString
          else
             WorkflowBiz.BizCheckValue :='1';
      end;
end;

Procedure TCustomWorkflowDB.WorkflowBizLoadList(WorkflowBizs: TWorkflowBizs);
var
  SQL: string;
  DS: TDataset;
begin
  FParams.Clear;
  SQL:=Format('SELECT * from %s', [WorkBizBindary.TableName]);
  DS := OpenSysQuery(SQL, FParams);
  try
    DS.First;
    WorkflowBizs.Clear;
    while not DS.EOF do
    begin
      WorkflowBizLoadRecord(DS, WorkflowBizs.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

Procedure TCustomWorkflowDB.WorkflowBizCheck(WorkflowBiz: TWorkflowBiz; BizKey: string;BillState:String; AWorkflowStudio: TCustomWorkflowStudio);
var
  SQL: string;
  CheckDate: TDateTime;
begin
  CheckDate :=AWorkflowStudio.GetNow;
    if (WorkflowBiz.BizTableName<>'') and (WorkflowBiz.BizCheck<>'') and (WorkflowBiz.BizKey<>'') and (BizKey<>'') then
    begin
    //先处理员工id 转员工编号（20位）
     SQL :='Update '+WorkflowBiz.BizTableName+' set '
     +WorkflowBiz.BizCheck+'=';
     if(BillState>'0') then
        SQL :=SQL+ BillState
     else
        SQL :=SQL+WorkflowBiz.BizCheckValue;
     //     +WorkflowBiz.BizCheck+'=1 ' ;
     //需要审批人
     if WorkflowBiz.BizChecker >'' then
       SQL:= SQL +','+WorkflowBiz.BizChecker+'='+QuotedStr(AWorkflowStudio.UserManager.LoggedUserCode);//用员工编码审核
       //SQL:= SQL +','+WorkflowBiz.BizChecker+'='+QuotedStr(AWorkflowStudio.UserManager.LoggedUserId);
     //需要审批时间
     if WorkflowBiz.BizCheckDate >'' then
       SQL:= SQL +','+WorkflowBiz.BizCheckDate+'='+QuotedStr(FormatDateTime('YYYY-MM-DD',CheckDate));
     //找到该条记录
     SQL:= SQL +' where '+WorkflowBiz.BizKey+'='+QuotedStr(BizKey) ;

     FParams.Clear;
     ExecuteQuery(SQL, FParams);
    end;
end;

Procedure TCustomWorkflowDB.WorkflowBizUpdate(WorkflowBiz: TWorkflowBiz; WorkIns: TWorkflowInstance;AWorkflowStudio: TCustomWorkflowStudio);
var
  wsVar: TWorkflowVariable;
  wsVarList: TWorkflowVariables;
  SQL: string;
  c: Integer;
  DS: TDataset;
  AField: TField;
begin
  FParams.Clear;
  SQL:=Format('SELECT * from %s where %s=%s', [WorkflowBiz.BizTableName,WorkflowBiz.BizKey,QuotedStr(WorkIns.FlowMnBusCode)]);
  DS := OpenQuery(SQL, FParams);
  try
    if DS.RecordCount>0 then
    begin
      SQL :='Update '+WorkflowBiz.BizTableName+' set ';
      wsVarList := WorkIns.Diagram.Variables;
      for c := 0 to wsVarList.Count - 1 do
      begin
        wsVar := wsVarList.Items[c];
        if wsVar.Interaction  then
        begin
         AField := Ds.FindField(wsVar.Name);
         if AField<>nil then
         begin
          case AField.DataType of
              ftSmallint, ftInteger,ftBCD,ftAutoInc,ftLargeint,
               ftBoolean,ftBytes,ftWord,ftFloat,ftCurrency:
              SQL := SQL + wsVar.Name +'='+VarToStr(wsVar.Value)+',';
              ftString,ftDate,ftTime,ftDateTime,
              ftMemo,ftFixedChar,ftWideString:
              SQL := SQL + wsVar.Name +'='+QuotedStr(VarToStr(wsVar.Value))+',';
          end;
         end;
        end;
      end;
      if SQL[Length(SQL)]=',' then  {有字段需要更新}
      begin
        Delete(SQL,Length(SQL),1);
        SQL := SQL +Format(' where %s=%s', [WorkflowBiz.BizKey,QuotedStr(WorkIns.FlowMnBusCode)]);
        FParams.Clear;
        ExecuteQuery(SQL, FParams);
      end;
    end;
  finally
     DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

function TCustomWorkflowDB.BelongsToProject(AUserID,AProjectID:string):boolean;
var
  DS: TDataset;
  SQL: string;
begin
  FParams.Clear;
  result :=False;  //需要创建数据库存储过程GetProjectRight(UserID,ProjectID)
  SQL:='GetProjectRight'+QuotedStr(AUserID)+' ,'+QuotedStr(AProjectID);
  DS := OpenQuery(SQL, FParams);
  try
    if DS.RecordCount>0 then
    begin
       if DS.Fields[0].AsInteger > 0 then
          result :=True;
    end;
  finally
     DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;
function TCustomWorkflowDB.BlobFieldToString(AField: TField): string;
begin
  result := AField.AsString;
end;

procedure TCustomWorkflowDB.WorkflowDefinitionLoadRecord(DS: TDataset; WorkDef: TWorkflowDefinition);
begin
  with WorkDefBindary do
  begin
    DiagramFromString(WorkDef.Diagram, BlobFieldToString(DS.FieldByName(FlowDnContentField)));
    WorkDef.Name := DS.FieldByName(FlowDnNameField).AsString;
    WorkDef.Key := DS.FieldByName(KeyField).AsString;
    WorkDef.FlowExplan := DS.FieldByName(FlowDnExplanField).AsString;
    WorkDef.WorkType := DS.FieldByName(FlowDnFKID_ClsField).AsString;
    WorkDef.FlowIsPublic := DS.FieldByName(FlowDnIsPublicField).AsInteger;
    WorkDef.FlowIsTerm := DS.FieldByName(FlowDnIsTermField).AsInteger;
    WorkDef.FlowCreatOn := StrToDateTime(DS.FieldByName(FlowDnDateField).AsString,UserfmtSettings);
    WorkDef.FlowCreator := DS.FieldByName(FlowDnCreatorField).AsString;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDefinitionUpdate(WorkDef: TWorkflowDefinition);
var
  SQL: string;
begin
  With WorkDefBindary do
  begin
    if WorkDef.Key <> '' then
    begin
    //先存附件
      {Save diagram attachments - it must be BEFORE streaming the diagram, because SaveAttachments procedure
       sets the key value for each attachment item. The diagram should be streamed only after a key value
       is assigned and after DirtyContent property is set to false}
      SaveAttachments(WorkDef.Diagram, ptDefinition, WorkDef.Key);

{    SQL :='Update '+TableName+' Set '+
      FlowDnNameField+'='+QuotedStr(WorkDef.Name)+
      ','+FlowDnExplanField+'='+QuotedStr(WorkDef.FlowExplan)+
      ','+FlowDnFKID_ClsField+'='+QuotedStr(WorkDef.WorkType)+
      ','+FlowDnIsPublicField+'='+IntToStr(WorkDef.FlowIsPublic)+
      ','+FlowDnIsTermField+'='+IntToStr(WorkDef.FlowIsTerm)+
      ','+FlowDnDateField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',Now))+
      ','+FlowDnCreatorField+'='+QuotedStr(WorkDef.WorkflowStudio.UserManager.LoggedUserId)+
      ','+FlowDnContentField+'='+QuotedStr(DiagramToString(WorkDef.Diagram))+
      ' WHERE '+KeyField+'='+QuotedStr(WorkDef.Key);
      FParams.Clear;    }
      {FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkDef.Key);}

//      FParams.Clear;
      {FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkDef.Key);
      FParams.CreateParam(ftMemo, 'workflow', ptInput).AsMemo := DiagramToString(WorkDef.Diagram);
      FParams.CreateParam(ftString, 'name', ptInput).AsString := WorkDef.Name; }

      SQL :='Update '+TableName+' Set '
      +FlowDnNameField+'=:FlowDnName, '//+QuotedStr(WorkDef.Name)+
      +FlowDnExplanField+'=:FlowDnExplan, '//+QuotedStr(WorkDef.FlowExplan)+
      +FlowDnFKID_ClsField+'=:FlowDnFKID_Cls, '//+QuotedStr(WorkDef.WorkType)+
      +FlowDnIsPublicField+'=:FlowDnIsPublic, '//+IntToStr(WorkDef.FlowIsPublic)+
      +FlowDnIsTermField+'=:FlowDnIsTerm, '//+IntToStr(WorkDef.FlowIsTerm)+
      +FlowDnDateField+'=:FlowDnDate, '//+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',Now))+
      +FlowDnCreatorField+'=:FlowDnCreator, '//+QuotedStr(WorkDef.WorkflowStudio.UserManager.LoggedUserId)+
      +FlowDnContentField+'=:FlowDnContent '//+QuotedStr(DiagramToString(WorkDef.Diagram))+
      +' WHERE '+KeyField+'=:FlowDnPKID';//+QuotedStr(WorkDef.Key);
      FParams.Clear;
      FParams.CreateParam(ftString, 'FlowDnName', ptInput).AsString := WorkDef.Name;
      FParams.CreateParam(ftString, 'FlowDnExplan', ptInput).AsString := WorkDef.FlowExplan;
      FParams.CreateParam(ftString, 'FlowDnFKID_Cls', ptInput).AsString := WorkDef.WorkType;
      FParams.CreateParam(ftInteger, 'FlowDnIsPublic', ptInput).AsInteger := WorkDef.FlowIsPublic;
      FParams.CreateParam(ftInteger, 'FlowDnIsTerm', ptInput).AsInteger := WorkDef.FlowIsTerm;
      FParams.CreateParam(ftString, 'FlowDnDate', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',Now);
      FParams.CreateParam(ftString, 'FlowDnCreator', ptInput).AsString := WorkDef.WorkflowStudio.UserManager.LoggedUserId;
      FParams.CreateParam(ftMemo, 'FlowDnContent', ptInput).AsMemo := DiagramToString(WorkDef.Diagram);
      FParams.CreateParam(ftString, 'FlowDnPKID', ptInput).AsString := WorkDef.Key;
      ExecuteQuery(SQL, FParams);

    end else
      wsDBError(Format(_str(SErrorUpdateEmptyKey), [TableName]));
  end;
end;

procedure TCustomWorkflowDB.WorkflowInstanceDelete(WorkIns: TWorkflowInstance);
var
  SQL: string;
begin
  if WorkIns.Key <> '' then
  begin
    SQL := Format('DELETE FROM %s WHERE %s = %s',
      [WorkInsBindary.TableName, WorkInsBindary.KeyField,QuotedStr(WorkIns.Key)]);
    FParams.Clear;
    //FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkIns.Key);
    ExecuteQuery(SQL, FParams);

    TaskInstancesDeleteByWorkIns(WorkIns.Key);
  end else
    wsDBError(Format(_str(SErrorDeleteEmptyKey), ['工作流实例']));
end;

procedure TCustomWorkflowDB.WorkflowInstanceInsert(WorkIns: TWorkflowInstance; AWorkflowStudio: TCustomWorkflowStudio);
var
  SQL: string;
  CreatedOn: TDateTime;
begin
  CreatedOn := AWorkflowStudio.GetNow;
  with WorkInsBindary do
  begin
    // Insert workinstance unique key    WorkIns.Key := IntToStr(FindNextID(TableName, KeyField));
    WorkIns.Key := NewID;
    WorkIns.FlowMnCreator:=AWorkflowStudio.UserManager.LoggedUserId;
    WorkIns.FlowMnCreateTime:= CreatedOn;
{    SQL := 'Insert Into '+TableName+' ('
        +KeyField
        +','+FlowMnCreateTimeField
        +','+FlowMnCreatorField
        +','+FlowMnSubjectField
        +','+FlowMnContentField
        +','+FlowMnBusWindowField
        +','+FlowMnBusCodeField
        +','+FlowMnStateField
        +','+FlowMnRankField
        +','+FlowMnFKID_FlowField
         +','+FlowMnReportField
        +','+FlowMoveTimeLimitField
        +','+FlowMnLastUpDateTimeField
        +','+FlowMnPrjIDField
        +') Values ('
        + QuotedStr(WorkIns.Key)
        +','+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
        +','+QuotedStr(AWorkflowStudio.UserManager.LoggedUserId)
        +','+QuotedStr(WorkIns.FlowMnSubject)
        +','+QuotedStr(WorkIns.FlowMnContent)
        +','+QuotedStr(WorkIns.FlowMnBusWindow)
        +','+QuotedStr(WorkIns.FlowMnBusCode)
        +','+IntToStr(Ord(WorkIns.Status))
        +','+IntToStr(WorkIns.FlowMnRank)
        +','+QuotedStr(WorkIns.DefinitionKey)
        +','+QuotedStr(WorkIns.FlowMnReport)
        +','+IntToStr(WorkIns.FlowMoveTimeLimit)
        +','+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
        +','+QuotedStr(WorkIns.FlowMnPrjID)
        +')';   }

    SQL := 'Insert Into '+TableName+' ('
        +KeyField
        +','+FlowMnCreateTimeField
        +','+FlowMnCreatorField
        +','+FlowMnSubjectField
        +','+FlowMnContentField
        +','+FlowMnBusWindowField
        +','+FlowMnBusCodeField
        +','+FlowMnStateField
        +','+FlowMnRankField
        +','+FlowMnFKID_FlowField
         +','+FlowMnReportField
        +','+FlowMoveTimeLimitField
        +','+FlowMnLastUpDateTimeField
        +','+FlowMnPrjIDField
        +') Values ('
        + ':FlowMnPKID'//QuotedStr(WorkIns.Key)
        +',:FlowMnCreateTime'//+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
        +',:FlowMnCreator'//+QuotedStr(WorkIns.FlowMnCreator)
        +',:FlowMnSubject'//+QuotedStr(WorkIns.FlowMnSubject)
        +',:FlowMnContent'//+QuotedStr(WorkIns.FlowMnContent)
        +',:FlowMnBusWindow'//+QuotedStr(WorkIns.FlowMnBusWindow)
        +',:FlowMnBusCode'//+QuotedStr(WorkIns.FlowMnBusCode)
        +',:FlowMnState'//+IntToStr(Ord(WorkIns.Status))
        +',:FlowMnRank'//+IntToStr(WorkIns.FlowMnRank)
        +',:FlowMnFKID_Flow'//+QuotedStr(WorkIns.DefinitionKey)
        +',:FlowMnReport'//+QuotedStr(WorkIns.FlowMnReport)
        +',:FlowMoveTimeLimit'//+IntToStr(WorkIns.FlowMoveTimeLimit)
        +',:FlowMnLastUpDateTime'//+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
        +',:FlowMnPrjID'//+QuotedStr(WorkIns.FlowMnPrjID)
        +')';
    FParams.Clear;
    FParams.CreateParam(ftString, 'FlowMnPKID', ptInput).AsString := WorkIns.Key;
    FParams.CreateParam(ftString, 'FlowMnCreateTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn);
    FParams.CreateParam(ftString, 'FlowMnCreator', ptInput).AsString := WorkIns.FlowMnCreator;
    FParams.CreateParam(ftString, 'FlowMnSubject', ptInput).AsString := WorkIns.FlowMnSubject;
    FParams.CreateParam(ftString, 'FlowMnContent', ptInput).AsString := WorkIns.FlowMnContent;
    FParams.CreateParam(ftString, 'FlowMnBusWindow', ptInput).AsString := WorkIns.FlowMnBusWindow;
    FParams.CreateParam(ftString, 'FlowMnBusCode', ptInput).AsString := WorkIns.FlowMnBusCode;
    FParams.CreateParam(ftInteger, 'FlowMnState', ptInput).AsInteger := Ord(WorkIns.Status);
    FParams.CreateParam(ftInteger, 'FlowMnRank', ptInput).AsInteger := WorkIns.FlowMnRank;
    FParams.CreateParam(ftString, 'FlowMnFKID_Flow', ptInput).AsString := WorkIns.DefinitionKey;
    FParams.CreateParam(ftString, 'FlowMnReport', ptInput).AsString := WorkIns.FlowMnReport;
    FParams.CreateParam(ftInteger, 'FlowMoveTimeLimit', ptInput).AsInteger := WorkIns.FlowMoveTimeLimit;
    FParams.CreateParam(ftString, 'FlowMnLastUpDateTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn);
    FParams.CreateParam(ftString, 'FlowMnPrjID', ptInput).AsString := WorkIns.FlowMnPrjID;

    ExecuteQuery(SQL, FParams);
    //没有保存附件呢
    // Save other workflow instance fields
    WorkflowInstanceUpdate(WorkIns);
  end;
end;

function TCustomWorkflowDB.WorkflowInstanceLoad(WorkIns: TWorkflowInstance): boolean;
var
  SQL: string;
  DS: TDataset;
begin
  result := false;
  if Trim(WorkIns.Key) = '' then
    exit;

  SQL := Format('SELECT * from %s WHERE %s = %s',
    [WorkInsBindary.TableName, WorkInsBindary.KeyField,QuotedStr(WorkIns.Key)]);
  FParams.Clear;
  //FParams.CreateParam(ftString, WorkInsBindary.KeyField, ptInput).AsString := WorkIns.Key;
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
    begin
      WorkflowInstanceLoadRecord(DS, WorkIns);
      result := true;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowInstanceLoadRecord(DS: TDataset; WorkIns: TWorkflowInstance);
begin
  with WorkInsBindary do
  begin
    ComponentFromString(WorkIns, DS.FieldByName(FlowMnFlowContentField).AsString);
    WorkIns.Key := DS.FieldByName(KeyField).AsString;
    WorkIns.DefinitionKey := DS.FieldByName(FlowMnFKID_FlowField).AsString;
   // WorkIns.Status:= TWorkflowStatus(DS.FieldByName(FlowMnStateField).AsInteger);
    WorkIns.FlowMnCreateTime := StrToDateTime(DS.FieldByName(FlowMnCreateTimeField).AsString,UserfmtSettings);
    WorkIns.FlowMnCreator := DS.FieldByName(FlowMnCreatorField).AsString;
    WorkIns.FlowMnSubject := DS.FieldByName(FlowMnSubjectField).AsString;
    WorkIns.FlowMnContent := DS.FieldByName(FlowMnContentField).AsString;
    WorkIns.FlowMnBusWindow := DS.FieldByName(FlowMnBusWindowField).AsString;
    WorkIns.FlowMnBusCode := DS.FieldByName(FlowMnBusCodeField).AsString;
    WorkIns.FlowMnRank := DS.FieldByName(FlowMnRankField).AsInteger;
    WorkIns.FlowMoveTimeLimit := DS.FieldByName(FlowMoveTimeLimitField).AsInteger;
    WorkIns.FlowMnLastUpDateTime := StrToDateTime(DS.FieldByName(FlowMnLastUpDateTimeField).AsString,UserfmtSettings);
    WorkIns.FlowMnReport := DS.FieldByName(FlowMnReportField).AsString;
    if WorkIns.Status in [wsFinished, wsFinishedWithError] then
        WorkIns.FlowMnFinishTime := StrToDateTime(DS.FieldByName(FlowMnFinishTimeField).AsString,UserfmtSettings);
    if (not DS.FieldByName(NextRunTimeField).IsNull) and (DS.FieldByName(NextRunTimeField).AsString<>'')  then
        WorkIns.NextRunTime := StrToDateTime(DS.FieldByName(NextRunTimeField).AsString,UserfmtSettings);
  end;
end;

procedure TCustomWorkflowDB.WorkflowInstanceUpdate(WorkIns: TWorkflowInstance);
var
  SQL: string;
begin
  with WorkInsBindary do
  begin
    if WorkIns.Key <> '' then
    begin
     //要先存附件，否则附件内容会保存到流程图中，这样太大了
      {Save diagram attachments - it must be BEFORE streaming the diagram, because SaveAttachments procedure
       sets the key value for each attachment item. The diagram should be streamed only after a key value
       is assigned and after DirtyContent property is set to false}
      SaveAttachments(WorkIns.Diagram, ptInstance, WorkIns.Key);

{    SQL := 'Update '+TableName+' Set '+
        FlowMnSubjectField+'='+QuotedStr(WorkIns.FlowMnSubject)
        +','+FlowMnContentField+'='+QuotedStr(WorkIns.FlowMnContent)
        +','+FlowMnBusWindowField+'='+QuotedStr(WorkIns.FlowMnBusWindow)
        +','+FlowMnBusCodeField+'='+QuotedStr(WorkIns.FlowMnBusCode)
        +','+FlowMnStateField+'='+IntToStr(Ord(WorkIns.Status))
        +','+FlowMnRankField+'='+IntToStr(WorkIns.FlowMnRank)
        +','+FlowMnFKID_FlowField+'='+QuotedStr(WorkIns.DefinitionKey)
        +','+FlowMnReportField+'='+QuotedStr(WorkIns.FlowMnReport)
        +','+FlowMnPrjIDField+'='+QuotedStr(WorkIns.FlowMnPrjID)
        +','+FlowMoveTimeLimitField+'='+IntToStr(WorkIns.FlowMoveTimeLimit)
        +','+FlowMnFlowContentField+'='+QuotedStr(ComponentToString(WorkIns));

        if WorkIns.Status in [wsFinished, wsFinishedWithError] then
          begin
            WorkIns.FlowMnFinishTime := Now;   //在 TWorkflowEngine.WorkflowTerminated处理
            SQL :=SQL +','+FlowMnFinishTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.FlowMnFinishTime));
          end;
        if WorkIns.NextRunTime > 0 then
           SQL :=SQL +','+NextRunTimeField+'='+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.NextRunTime));

        SQL :=SQL +'WHERE '+ KeyField+'='+QuotedStr(WorkIns.Key);   }

   {   SQL := Format('UPDATE %s SET '+
        '%s = :workflow, '+
        '%s = :workdefkey, '+
        '%s = :nextruntime '+
        'WHERE %s = :id',
        [TableName, WorkflowField, WorkDefKeyField, NextRunTimeField, KeyField]);
      FParams.Clear;
      FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkIns.Key);}

//      FParams.Clear;
     { FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(WorkIns.Key);
      FParams.CreateParam(ftMemo, 'workflow', ptInput).AsMemo := ComponentToString(WorkIns);
      FParams.CreateParam(ftInteger, 'workdefkey', ptInput).AsInteger := StrToInt(WorkIns.DefinitionKey); 
      if WorkIns.NextRunTime > 0 then
        FParams.CreateParam(ftDateTime, 'nextruntime', ptInput).AsDateTime := WorkIns.NextRunTime
      else
        FParams.CreateParam(ftDateTime, 'nextruntime', ptInput).Clear;   }

      SQL := 'Update '+TableName+' Set '+
        FlowMnSubjectField+'=:FlowMnSubject'//+QuotedStr(WorkIns.FlowMnSubject)
        +','+FlowMnContentField+'=:FlowMnContent'//+QuotedStr(WorkIns.FlowMnContent)
        +','+FlowMnBusWindowField+'=:FlowMnBusWindow'//+QuotedStr(WorkIns.FlowMnBusWindow)
        +','+FlowMnBusCodeField+'=:FlowMnBusCode'//+QuotedStr(WorkIns.FlowMnBusCode)
        +','+FlowMnStateField+'=:FlowMnState'//+IntToStr(Ord(WorkIns.Status))
        +','+FlowMnRankField+'=:FlowMnRank'//+IntToStr(WorkIns.FlowMnRank)
        +','+FlowMnFKID_FlowField+'=:FlowMnFKID_Flow'//+QuotedStr(WorkIns.DefinitionKey)
        +','+FlowMnReportField+'=:FlowMnReport'//+QuotedStr(WorkIns.FlowMnReport)
        +','+FlowMnPrjIDField+'=:FlowMnPrjID'//+QuotedStr(WorkIns.FlowMnPrjID)
        +','+FlowMoveTimeLimitField+'=:FlowMoveTimeLimit'//+IntToStr(WorkIns.FlowMoveTimeLimit)
        +','+FlowMnFlowContentField+'=:FlowMnFlowContent ';//+QuotedStr(ComponentToString(WorkIns));

        FParams.Clear;
        FParams.CreateParam(ftString, 'FlowMnSubject', ptInput).AsString := WorkIns.FlowMnSubject;
        FParams.CreateParam(ftString, 'FlowMnContent', ptInput).AsString := WorkIns.FlowMnContent;
        FParams.CreateParam(ftString, 'FlowMnBusWindow', ptInput).AsString := WorkIns.FlowMnBusWindow;
        FParams.CreateParam(ftString, 'FlowMnBusCode', ptInput).AsString := WorkIns.FlowMnBusCode;
        FParams.CreateParam(ftInteger, 'FlowMnState', ptInput).AsInteger := Ord(WorkIns.Status);
        FParams.CreateParam(ftInteger, 'FlowMnRank', ptInput).AsInteger := WorkIns.FlowMnRank;
        FParams.CreateParam(ftString, 'FlowMnFKID_Flow', ptInput).AsString := WorkIns.DefinitionKey;
        FParams.CreateParam(ftString, 'FlowMnReport', ptInput).AsString := WorkIns.FlowMnReport;
        FParams.CreateParam(ftString, 'FlowMnPrjID', ptInput).AsString := WorkIns.FlowMnPrjID;
        FParams.CreateParam(ftInteger, 'FlowMoveTimeLimit', ptInput).AsInteger := WorkIns.FlowMoveTimeLimit;
        FParams.CreateParam(ftMemo, 'FlowMnFlowContent', ptInput).AsMemo := ComponentToString(WorkIns);

        if WorkIns.Status in [wsFinished, wsFinishedWithError] then
          begin
            WorkIns.FlowMnFinishTime := Now;   //在 TWorkflowEngine.WorkflowTerminated处理
            SQL :=SQL +','+FlowMnFinishTimeField+'=:FlowMnFinishTime ';//+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.FlowMnFinishTime));
            FParams.CreateParam(ftString, 'FlowMnFinishTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.FlowMnFinishTime);
          end;
        if WorkIns.NextRunTime > 0 then
        begin
           SQL :=SQL +','+NextRunTimeField+'=:NextRunTime ';//+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.NextRunTime));
           FParams.CreateParam(ftString, 'NextRunTime', ptInput).AsString := FormatDateTime('YYYY-MM-DD HH:MM:SS',WorkIns.NextRunTime);
        end;

        SQL :=SQL +' WHERE '+ KeyField+'=:FlowMnPKID';//+QuotedStr(WorkIns.Key);
        FParams.CreateParam(ftString, 'FlowMnPKID', ptInput).AsString := WorkIns.Key;
      ExecuteQuery(SQL, FParams);
    end
    else
      wsDBError(Format(_str(SErrorUpdateEmptyKey), [TableName]));
  end;
end;

procedure TCustomWorkflowDB.SaveAttachments(ADiagram: TCustomWorkflowDiagram;
  AType: TAttachmentParentType; AWorkKey: string);
var
  c: integer;
  d: integer;
  AItem: TAttachmentItem;
  AKeys: string;
  SQL: string;
begin
  for c := 0 to ADiagram.Attachments.Count - 1 do           
    for d := 0 to ADiagram.Attachments[c].Items.Count - 1 do
    begin
      AItem := ADiagram.Attachments[c].Items[d];
      AItem.WorkKey := AWorkKey;
      if AItem.DirtyContent then
      begin
        if AItem.Key = '' then
          AttachmentItemInsert(AItem, AType, AWorkKey)
        else
          AttachmentItemUpdate(AItem, AType, AWorkKey);

        {Very important to set DirtyContent to false, so that the content of the attachment
         will not be streamed together with the diagram}
        AItem.DirtyContent := false;
      end;
      if AKeys <> '' then AKeys := AKeys + ', ';
      AKeys := AKeys + QuotedStr(AItem.Key);
    end;

  {Delete the non existant attachments}
  {还没实现清除ftp上面文件}
  FParams.Clear;
  if AKeys = '' then
    AKeys := QuotedStr('-1');
  With AttachmentBindary do
    SQL := Format('Delete from %s Where %s = %s and %s = %d and not (%s in (%s))',
      [TableName, WorkKeyFIeld, QuotedStr(AWorkKey), ObjectTypeField, Ord(AType), KeyField, AKeys]);
  ExecuteQuery(SQL, FParams);
end;

procedure TCustomWorkflowDB.AttachmentItemInsert(AItem: TAttachmentItem;
  AType: TAttachmentParentType; AWorkKey: string);
var
  SQL: string;
  CreatedOn: TDateTime;
begin
  CreatedOn := AItem.WorkflowStudio.GetNow;
  with AttachmentBindary do
  begin
    {Insert attachment unique key}
   { AItem.Key := IntToStr(FindNextID(TableName, KeyField));
    SQL := Format('INSERT INTO %s (%s, %s) Values (:id, :createdon)',
      [TableName, KeyField, CreatedOnField]);
    FParams.Clear;
    FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(AItem.Key);
    FParams.CreateParam(ftDateTime, 'createdon', ptInput).AsDateTime := CreatedOn; }
    if AItem.DirtyContent then
      (AItem.WorkflowStudio as TWorkflowStudio).SendFtpFile(AItem);
    AItem.Key := NewID;
    SQL :='Insert Into '+TableName +'('
        +KeyField
        +','+WorkKeyField
        +','+CreatedOnField
        +','+ContentField
        +','+ObjectTypeField
        +') Values ('
        +QuotedStr(AItem.Key)
        +','+QuotedStr(AWorkKey)
        +','+QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',CreatedOn))
        +','+QuotedStr(AItem.Name)             {AItem.Content}
        +','+IntToStr(Ord(AType))
        +')';

    ExecuteQuery(SQL, FParams);

    {Save other attachment fields}
   // AttachmentItemUpdate(AItem, AType, AWorkKey);
  end;
end;

procedure TCustomWorkflowDB.AttachmentItemUpdate(AItem: TAttachmentItem; AType: TAttachmentParentType; AWorkKey: string);
var
  SQL: string;
begin
  with AttachmentBindary do
  begin
    if AItem.Key <> '' then
    begin
     { SQL := Format('UPDATE %s SET '+
        '%s = :objecttype, '+
        '%s = :workkey, '+
        '%s = :content '+
        'WHERE %s = :id',
        [TableName, ObjectTypeField, WorkKeyField, ContentField, KeyField]);
      FParams.Clear;
      FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(AItem.Key);
      FParams.CreateParam(ftInteger, 'workkey', ptInput).AsInteger := StrToInt(AWorkKey);
      FParams.CreateParam(ftInteger, 'objecttype', ptInput).AsInteger := Ord(AType);


      SetBlobParam(FParams.CreateParam(ftBlob, 'content', ptInput), AItem.Content);   }
      //增加上传FTP服务器的功能
    if AItem.DirtyContent then
      (AItem.WorkflowStudio as TWorkflowStudio).SendFtpFile(AItem);

      SQL := Format('UPDATE %s SET '+
        '%s = '+IntToStr(Ord(AType)) +
        ',%s ='+QuotedStr(AWorkKey)+
        ',%s = '+QuotedStr(AItem.Name)+   {AItem.Content}
        ' WHERE %s ='+QuotedStr(AItem.Key),
        [TableName, ObjectTypeField, WorkKeyField, ContentField, KeyField]);
      ExecuteQuery(SQL, FParams);
    end else
      wsDBError(Format(_str(SErrorUpdateEmptyKey), [TableName]));
  end;
end;

function TCustomWorkflowDB.AttachmentItemLoad(AItem: TAttachmentItem): boolean;
var
  SQL: string;
  DS: TDataset;
begin
  result := false;
  if Trim(AItem.Key) = '' then
    exit;
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [AttachmentBindary.TableName, AttachmentBindary.KeyField,QuotedStr(AItem.Key)]);
  FParams.Clear;
 // FParams.CreateParam(ftInteger, 'id', ptInput).AsInteger := StrToInt(AItem.Key);
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
    begin
      AttachmentItemLoadRecord(DS, AItem);
      result := true;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.AttachmentItemLoadRecord(DS: TDataset;
  AItem: TAttachmentItem);
begin
  With AttachmentBindary do
  begin
    AItem.Key := DS.FieldByName(KeyField).AsString;
    AItem.WorkKey := DS.FieldByName(WorkKeyField).AsString;
    AItem.Name := DS.FieldByName(ContentField).AsString;
    AItem.AttachType :=DS.FieldByName(ObjectTypeField).AsInteger;
    AItem.CreatedOn := StrToDateTime(DS.FieldByName(CreatedOnField).AsString,UserfmtSettings);
    //AItem.OriginalName := AItem.Name;
    AItem.DirtyContent := false;
  end;
end;

procedure TCustomWorkflowDB.SetBlobParam(AParam: TParam; BlobStream: string);
var
  SS: TStringStream;
begin
  SS := TStringStream.Create(BlobStream);
  try
    SS.Position := 0;
    AParam.LoadFromStream(SS, ftBlob);
  finally
    SS.Free;
  end;
end;

function TCustomWorkflowDB.MapOperationToStr(AOperation: TTaskLogOperation): string;
begin
  case AOperation of
    tlStatusChange: result := 'S';
    tlUpdate: result := 'U';
    tlCreate: result := 'C';
    tlInfo: result := 'I';
  else
    result := 'X';
  end;
end;

procedure TCustomWorkflowDB.LogTaskOperation(TaskIns: TTaskInstance; Operation: TTaskLogOperation;
  AWorkflowStudio: TCustomWorkflowStudio; Info1: string = ''; Info2: string = '');
var
  SQL: string;
  NextNum: integer;
  NowTime: TDateTime;
begin
  with TaskLogBindary do
  begin
    {Insert tasklog unique key}
    NowTime := AWorkflowStudio.GetNow;
    NextNum :=FindOtherNextID(TableName,FlowTaskLogFlowInsIDField,KeyField,TaskIns.WorkInsKey);
    SQL := Format('INSERT INTO %s (%s, %s, %s, %s, %s, %s,%s,%s) Values '+
      '(%s, %s, %s, %s, %s, %s,%s,%s)',
        [TableName, KeyField, FlowTaskLogTaskInsIDField,FlowTaskLogFlowInsIDField,
        FlowTaskLogDateField,FlowTaskLogOperationField,
         FlowTaskLogUserIDField, FlowTaskLogInfoField, FlowTaskLogInfo2Field,IntToStr(NextNum),
          QuotedStr(TaskIns.Key), QuotedStr(TaskIns.WorkInsKey), QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',NowTime)),
          QuotedStr(MapOperationToStr(Operation)),QuotedStr(AWorkflowStudio.UserManager.LoggedUserId),
          QuotedStr(Info1),QuotedStr(Info2)]);
    FParams.Clear;
  {  FParams.CreateParam(ftinteger, 'taskinskey', ptInput).AsInteger := StrToInt(TaskIns.Key);
    FParams.CreateParam(ftDateTime, 'eventdate', ptInput).AsDateTime := AWorkflowStudio.GetNow;
    FParams.CreateParam(ftString, 'operation', ptInput).AsString := MapOperationToStr(Operation);
    FParams.CreateParam(ftString, 'userid', ptInput).AsString := AWorkflowStudio.UserManager.LoggedUserId;
    FParams.CreateParam(ftString, 'info', ptInput).AsString := Info1;
    FParams.CreateParam(ftString, 'info2', ptInput).AsString := Info2;}
    ExecuteQuery(SQL, FParams);
  end;
end;

procedure TCustomWorkflowDB.TaskLogLoadList(ALogItems: TTaskLogItems;ALogFilterType: TLogFilterType;AFilterKey: string);
var
  SQL: string;
  DS: TDataset;
begin
  ALogItems.Clear;
  if AFilterKey <> '' then
  begin
    FParams.Clear;
    case ALogFilterType of
        lfWorkIns:
        begin
           SQL := Format('SELECT * from %s where %s = %s order by %s',
           [TaskLogBindary.TableName, TaskLogBindary.FlowTaskLogFlowInsIDField,QuotedStr(AFilterKey),TaskLogBindary.FlowTaskLogDateField]);
        end;
     else
      begin
      SQL := Format('SELECT * from %s where %s = %s order by %s',
      [TaskLogBindary.TableName, TaskLogBindary.FlowTaskLogTaskInsIDField,QuotedStr(AFilterKey),TaskLogBindary.FlowTaskLogDateField]);
      end;
    end;

    DS := OpenQuery(SQL, FParams);
    try
      DS.First;
      while not DS.EOF do
      begin
        TaskLogLoadRecord(DS, ALogItems.Add);
        DS.Next;
      end;
    finally
      DS.Close;
      if DestroyQueries then
        DS.Free;
    end;
  end;
end;

procedure TCustomWorkflowDB.TaskLogLoadRecord(DS: TDataset;
  LogItem: TTaskLogItem);
begin
  With TaskLogBindary do
  begin
    LogItem.Operation := MapStrToOperation(DS.FieldByName(FlowTaskLogOperationField).AsString);
    LogItem.IndexID := DS.FieldByName(KeyField).AsInteger;
    LogItem.TaskInsKey := DS.FieldByName(FlowTaskLogTaskInsIDField).AsString;
    LogItem.FlowInsKey := DS.FieldByName(FlowTaskLogFlowInsIDField).AsString;
    LogItem.EventDate := StrToDateTime(DS.FieldByName(FlowTaskLogDateField).AsString,UserfmtSettings);
    LogItem.UserId := DS.FieldByName(FlowTaskLogUserIDField).AsString;
    LogItem.Info := DS.FieldByName(FlowTaskLogInfoField).AsString;
    LogItem.Info2 := DS.FieldByName(FlowTaskLogInfo2Field).AsString;
  end;
end;

function TCustomWorkflowDB.MapStrToOperation(AStr: string): TTaskLogOperation;
Var
  AChar: string;
begin
  result := tlNone;
  AChar := Uppercase(AStr);
  if AChar = 'S' then
    result := tlStatusChange
  else if AChar = 'U' then
    result := tlUpdate
  else if AChar = 'C' then
    result := tlCreate
  else if AChar = 'I' then
    result := tlInfo;
end;

function TCustomWorkflowDB.FindNextID(ATableName, AFieldName: string): integer;
var
  DS: TDataset;
begin
  FParams.Clear;
  DS := OpenQuery(
    Format('Select Max(%s) from %s', [AFieldName, ATableName]),
    FParams);
  try
    result := DS.Fields[0].AsInteger + 1;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

function TCustomWorkflowDB.FindOtherNextID(ATableName, AKeyFieldName,ANumFieldName,AKeyValue: string): integer;
var
  DS: TDataset;
begin
  FParams.Clear;
  DS := OpenQuery(
    Format('Select Max(%s) from %s where %s=%s', [ANumFieldName, ATableName,AKeyFieldName,QuotedStr(AKeyValue)]),
    FParams);
  try
    result := DS.Fields[0].AsInteger + 1;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.DoAssignSQLParams(Dataset: TDataset;
  AParams: TParams);
begin
end;

procedure TCustomWorkflowDB.ListPendingWorkflowInstances(AKeyList: TStrings; AToDate: TDateTime);
var
  sql: string;
  ds: TDataset;
begin
//工作流未结束，且过期的
  sql := Format('SELECT %s FROM %s WHERE (%s<2) and (%s <= %s)',
    [WorkInsBindary.KeyField, WorkInsBindary.TableName, WorkInsBindary.FlowMnStateField,WorkInsBindary.NextRunTimeField,
    QuotedStr(FormatDateTime('YYYY-MM-DD HH:MM:SS',AToDate))]);
  FParams.Clear;
 // FParams.CreateParam(ftDateTime, 'todate', ptInput).AsDateTime := AToDate;
  ds := OpenQuery(sql, FParams);
  try
    AKeyList.Clear;
    while not ds.EOF do
    begin
      AKeyList.Add(ds.Fields[0].AsString);
      ds.Next;
    end;
  finally
    ds.Close;
    if DestroyQueries then
      ds.Free;
  end;
end;

procedure TCustomWorkflowDB.ListWorkflowDefinitionNames(AList: TStrings; BList:TStrings);
var
  ds: TDataset;
begin
  FParams.Clear;
  ds := OpenQuery(Format('SELECT %s, %s FROM %s order by %s', [WorkDefBindary.KeyField, WorkDefBindary.FlowDnNameField, WorkDefBindary.TableName,WorkDefBindary.FlowDnNameField]), FParams);
  try
    ds.First;
    AList.Clear;
    BList.Clear;
    while not ds.EOF do
    begin
      {$WARNINGS OFF}
      //AList.AddObject(ds.Fields[1].AsString, TObject(PChar(ds.Fields[0].AsString)));
      AList.Add(ds.Fields[1].AsString);
      BList.Add(ds.Fields[0].AsString);
      {$WARNINGS ON}
      ds.Next;
    end;
  finally
    ds.Close;
    if DestroyQueries then
      ds.Free;
  end;
end;

procedure TCustomWorkflowDB.CheckStructure;
var
  ds: TDataset;
  paramList: TParams;
begin
  // check if database structure is up to date according to changes along workflow studio versions
  FStructureChecked := true;
  paramList := TParams.Create;
  try
    try
      // version 1.5
      ds := OpenQuery(Format('SELECT %s FROM %s WHERE 1=0', [WorkInsBindary.NextRunTimeField, WorkInsBindary.TableName]), paramList);
      ds.Close;
      if DestroyQueries then
        ds.Free;
    except
      on e: Exception do
        Application.MessageBox(PChar(Format(
          '检测数据库结构错误 (%s). 请确认数据库已升级到最新版本.',
          [e.Message])), '工作流', $10);
    end;
  finally
    paramList.Free;
  end;
end;

procedure TCustomWorkflowDB.LogTaskInfo(TaskIns: TTaskInstance; AWorkflowStudio: TCustomWorkflowStudio; AInfo, AInfo2: string);
begin
  LogTaskOperation(TaskIns, tlInfo, AWorkflowStudio, AInfo, AInfo2);
end;

procedure TCustomWorkflowDB.WorkflowUserLoadRecord(DS: TDataset;
  WorkUser: TWorkflowUser);
begin
  With UserBindary do
  begin
    WorkUser.UserName := DS.FieldByName(UserNameField).AsString;
    WorkUser.Email := DS.FieldByName(EmailField).AsString;
    WorkUser.UserId := DS.FieldByName(KeyField).AsString;
    WorkUser.PhoneCode := DS.FieldByName(PhoneCodeField).AsString;
    WorkUser.UserCode := DS.FieldByName(UserCodeField).AsString;
    WorkUser.CanPrint :=False;
  end;
end;

procedure TCustomWorkflowDB.WorkflowUserLoadList(
  WorkUsers: TWorkflowUsers);
var
  DS: TDataset;
  SQL: string;
begin
  FParams.Clear;
  SQL:=Format('SELECT * from %s', [UserBindary.TableName]);
  DS := OpenQuery(SQL, FParams);
  try
    DS.First;
    WorkUsers.Clear;
    while not DS.EOF do
    begin
      WorkflowUserLoadRecord(DS, WorkUsers.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowUserLoad(WorkUser: TWorkflowUser);
var
  SQL: string;
  DS: TDataset;
begin
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [UserBindary.TableName, UserBindary.KeyField,QuotedStr(WorkUser.UserId)]);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowUserLoadRecord(DS, WorkUser)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [UserBindary.TableName, WorkUser.UserId]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowGroupLoadRecord(DS: TDataset;
  WorkGroup: TWorkflowGroup);
begin
  With GroupBindary do
  begin
    WorkGroup.GroupName := DS.FieldByName(GroupNameField).AsString;
    WorkGroup.GroupId := DS.FieldByName(KeyField).AsString;
    WorkGroup.IsAdmin := DS.FieldByName(IsAdminField).AsInteger=1;
    WorkGroup.CanPrint :=False;
  end;
end;

procedure TCustomWorkflowDB.WorkflowGroupLoadList(
  WorkGroups: TWorkflowGroups);
var
  DS: TDataset;
  SQL: string;
begin
  FParams.Clear;
  SQL:=Format('SELECT * from %s', [GroupBindary.TableName]);
  DS := OpenQuery(SQL, FParams);
  try
    DS.First;
    WorkGroups.Clear;
    while not DS.EOF do
    begin
      WorkflowGroupLoadRecord(DS, WorkGroups.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowGroupLoad(WorkGroup: TWorkflowGroup);
var
  SQL: string;
  DS: TDataset;
begin
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [GroupBindary.TableName, GroupBindary.KeyField,QuotedStr(WorkGroup.GroupId)]);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowGroupLoadRecord(DS, WorkGroup)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [GroupBindary.TableName, WorkGroup.GroupId]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowGroupFillUserIds(WorkGroup: TWorkflowGroup); 
var
  SQL: string;
  DS: TDataset;
begin
  
  SQL := 'SELECT * from tbRoleEmp WHERE RERolePK_FKID = '+QuotedStr(WorkGroup.GroupId);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
      DS.First;
      WorkGroup.UserIds.Clear;
    	while not DS.Eof do
    		begin
     		WorkGroup.UserIds.Add(DS.FieldByName('REEmpPK_FKID').AsString);
     		DS.Next;
    	end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowJobLoadRecord(DS: TDataset;
  WorkJob: TWorkflowJob);
begin
  With JobBindary do
  begin
    WorkJob.JobName := DS.FieldByName(JobNameField).AsString;
    WorkJob.JobId := DS.FieldByName(KeyField).AsString;
    WorkJob.CanPrint :=False;
  end;
end;

procedure TCustomWorkflowDB.WorkflowJobLoadList(
  WorkJobs: TWorkflowJobs);      
var
  DS: TDataset;
  SQL: string;
begin
  
  FParams.Clear;
  SQL:=Format('SELECT * from %s', [JobBindary.TableName]);
  DS := OpenQuery(SQL, FParams);
  try
    DS.First;
    WorkJobs.Clear;
    while not DS.EOF do
    begin
      WorkflowJobLoadRecord(DS, WorkJobs.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowJobLoad(WorkJob: TWorkflowJob);
var
  SQL: string;
  DS: TDataset;
begin
  
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [JobBindary.TableName, JobBindary.KeyField,QuotedStr(WorkJob.JobId)]);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowJobLoadRecord(DS, WorkJob)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [JobBindary.TableName, WorkJob.JobId]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowJobFillUserIds(WorkJob: TWorkflowJob); 
var
  SQL: string;
  DS: TDataset;
begin
  
  SQL := 'SELECT * from tbEmpDuty WHERE EDDutyPK_FKID = '+QuotedStr(WorkJob.JobId);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
			DS.First;
      WorkJob.UserIds.Clear;
    	while not DS.Eof do
    		begin
     		WorkJob.UserIds.Add(DS.FieldByName('EDEmpPK_FKID').AsString);
     		DS.Next;
    	end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDepartmentLoadRecord(DS: TDataset;
  WorkDepartment: TWorkflowDepartment);
begin
  With DepartmentBindary do
  begin
    WorkDepartment.DepartmentName := DS.FieldByName(DepartmentNameField).AsString;
    WorkDepartment.DepartmentId := DS.FieldByName(KeyField).AsString;
    WorkDepartment.DepartmentManagerId := DS.FieldByName(DepartmentManagerIdField).AsString;
//    WorkDepartment.DepartmentManagerName := DS.FieldByName(DepartmentManagerNameField).AsString;
    WorkDepartment.CanPrint := False;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDepartmentLoadList(
  WorkDepartments: TWorkflowDepartments);      
var
  DS: TDataset;
  SQL: string;
begin
  
  FParams.Clear;
  SQL:=Format('SELECT * from %s', [DepartmentBindary.TableName]);
  DS := OpenQuery(SQL, FParams);
  try
    DS.First;
    WorkDepartments.Clear;
    while not DS.EOF do
    begin
      WorkflowDepartmentLoadRecord(DS, WorkDepartments.Add);
      DS.Next;
    end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDepartmentLoad(WorkDepartment: TWorkflowDepartment);
var
  SQL: string;
  DS: TDataset;
begin
  
  SQL := Format('SELECT * from %s WHERE %s = %s',
    [DepartmentBindary.TableName, DepartmentBindary.KeyField,QuotedStr(WorkDepartment.DepartmentId)]);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
    if not DS.IsEmpty then
      WorkflowDepartmentLoadRecord(DS, WorkDepartment)
    else
      wsDBError(Format(_str(SErrorRecordNotFoundKey), [DepartmentBindary.TableName, WorkDepartment.DepartmentId]));
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

procedure TCustomWorkflowDB.WorkflowDepartmentFillUserIds(WorkDepartment: TWorkflowDepartment); 
var
  SQL: string;
  DS: TDataset;
begin
  SQL := 'SELECT EmpPKID from tbEmployeeData WHERE EmpDeptCodeFKID = '+QuotedStr(WorkDepartment.DepartmentId);
  FParams.Clear;
  DS := OpenQuery(SQL, FParams);
  try
			DS.First;
      WorkDepartment.UserIds.Clear;
    	while not DS.Eof do
    		begin
     		WorkDepartment.UserIds.Add(DS.FieldByName('EmpPKID').AsString);
     		DS.Next;
    	end;
  finally
    DS.Close;
    if DestroyQueries then
      DS.Free;
  end;
end;

{ TwsBindary }

constructor TwsBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create;
  FWorkflowDB := AWorkflowDB;
end;

destructor TwsBindary.Destroy;
begin
  FWorkflowDB := nil;
  inherited;
end;

{ TTaskInsBindary }

{ TTaskInsBindary 任务实例}

constructor TTaskInsBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbFlowMain_Detail';
  KeyField := 'FlowMnDtlPKID';
  FlowMnDtlTaskContentField := 'FlowMnDtlTaskContent';
  FlowMnDtlSenderField := 'FlowMnDtlSender';
  FFlowMnDtlNameField := 'FlowMnDtlName';
  FlowMnDtlFKIDField := 'FlowMnDtlFKID';
//  FlowMnDtlDefIDField := 'FlowMnDtlDefID';
  FlowMnDtlStateField := 'FlowMnDtlState';
  FFlowMnDtlResultsField := 'FlowMnDtlResults';
  FFlowMnDtlChkTimeField := 'FlowMnDtlChkTime';
  FFlowMnDtlCommentsField := 'FlowMnDtlComments';
  FFlowMnDtlLimitTimeField := 'FlowMnDtlLimitTime';
  FFlowMnDtlNumField := 'FlowMnDtlNum';
  FFlowMnDtlPrintRightField := 'FlowMnDtlPrintRight';
  FFlowMnDtlReceiverField := 'FlowMnDtlReceiver';
  FFlowMnDtlRemarkField := 'FlowMnDtlRemark';
  FFlowMnDtlSendTimeField := 'FlowMnDtlSendTime';
  FFlowMnDtlStateChgTimeField := 'FlowMnDtlStateChgTime';
  FFlowMnDtlUserTypeField := 'FlowMnDtlUserType';
  FFlowMnDtlStepField := 'FlowMnDtlStep';
  FFlowMnDtlNeedPrjRightField :='FlowMnDtlNeedPrjRight';
  FFlowMnDtlPrjIDField :='FlowMnDtlPrjID';
end;

{ TWorkDefBindary 工作流定义}

constructor TWorkDefBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbFlowDefin';
  KeyField := 'FlowDnPKID';
  FFlowDnContentField := 'FlowDnContent';
  FFlowDnCreatorField := 'FlowDnCreator';
  FFlowDnDateField := 'FlowDnDate';
  FFlowDnExplanField := 'FlowDnExplan';
  FFlowDnFKID_ClsField := 'FlowDnFKID_Cls';
  FFlowDnIsPublicField := 'FlowDnIsPublic';
  FFlowDnIsTermField := 'FlowDnIsTerm';
  FFlowDnNameField := 'FlowDnName';
end;

{ TWorkInsBindary 工作流实例}

constructor TWorkInsBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbFlowMain';
  KeyField := 'FlowMnPKID';
  FFlowMnFlowContentField :='FlowMnFlowContent';
  FFlowMnBusCodeField :='FlowMnBusCode';
  FFlowMnBusWindowField :='FlowMnBusWindow';
  FFlowMnContentField :='FlowMnContent';
  FFlowMnCreateTimeField :='FlowMnCreateTime';
  FFlowMnCreatorField :='FlowMnCreator';
  FFlowMnFKID_FlowField :='FlowMnFKID_Flow';
//  FFlowMnIsStopField :='FlowMnIsStop';
  FFlowMnLastUpdateTimeField :='FlowMnLastUpdateTime';
  FFlowMnRankField :='FlowMnRank';
  FFlowMnStateField :='FlowMnState';
  FFlowMnSubjectField :='FlowMnSubject';
  FFlowMnReportField :='FlowMnReport';
  FFlowMoveTimeLimitField :='FlowMoveTimeLimit';
  FFlowMnFinishTimeField :='FlowMnFinishTime';
  FNextRunTimeField :='FlowMnNextTime';
  FFlowMnPrjIDField :='FlowMnPrjID';
end;

{ TAttachmentBindary 附件,根据ObjectTypeField识别定义时期还是实例时期}

constructor TAttachmentBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbFlowDefAttch';
  KeyField := 'FlowDefAttchID';
  ContentField := 'FlowDefAttchContent';
  WorkKeyField := 'FlowDefAttchFlowID';
  CreatedOnField := 'FlowDefAttchDate';
  ObjectTypeField := 'FlowDefAttchType';
end;

{ TTaskLogBindary 跟踪日志}

constructor TTaskLogBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbFlowTaskLog';
  KeyField := 'FlowTaskLogID';
  FFlowTaskLogTaskInsIDField := 'FlowTaskLogTaskInsID';
  FFlowTaskLogDateField :='FlowTaskLogDate';
  FFlowTaskLogFlowInsIDField :='FlowTaskLogFlowInsID';
  FFlowTaskLogInfoField :='FlowTaskLogInfo';
  FFlowTaskLogInfo2Field :='FlowTaskLogInfo2';
  FFlowTaskLogOperationField :='FlowTaskLogOperation';
  FFlowTaskLogUserIDField :='FlowTaskLogUserID';
end;
constructor TUserBindary.Create(AWorkflowDB: TComponent);
begin
    inherited Create(AWorkflowDB);
    FTableName := 'tbEmployeeData';
    KeyField := 'EmpPKID';
    FUserNameField := 'EmpName';
    FUserCodeField := 'EmpNum';
    FEmailField := 'EmpEmail';
    FPhoneCodeField:= 'EmpMobile';
    FDepartmentField:='EmpDeptCodeFKID';
end;  

constructor TGroupBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbRoleData';
  KeyField := 'RolePKID';
  FGroupNameField := 'RoleName';
  FIsAdminField := 'RoleIsAdmin';
end;  

constructor TDepartmentBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbCorporationOrganize';
  KeyField := 'CorpOgnPKID';  
  FDepartmentNameField := 'CorpOgnDeptName';
  FDepartmentManagerIdField := 'CorpOgnDeptManager';
//  FDepartmentManagerNameField := 'CorpOgnDeptManagerName';
end;

constructor TJobBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  FTableName := 'tbNewDuty';
  KeyField := 'NewDutyPKID';
  FJobNameField:= 'NewDutyName';
end;

constructor TWorkBizBindary.Create(AWorkflowDB: TComponent);
begin
  inherited Create(AWorkflowDB);
  	FTableName := 'SyWorkBiz';
 	  KeyField := 'BizMenuID';
    BizFormIDField := 'BizFormID';
    BizNameField := 'BizName';
    BizTableNameField := 'BizTableName';
    BizKeyField := 'BizKey';
    BizCheckField := 'BizCheck';
    BizCheckerField := 'BizChecker';
    BizCheckDateField := 'BizCheckDate';
    BizEditMenuIDField := 'BizEditMenuID';
    BizCheckValueField := 'BizCheckValue';
end;

end.


