unit fAssignedUserFrame;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ComCtrls, StdCtrls, wsClasses, wsDiagram;

type
  TfrAssignedUserFrame = class(TFrame)
    Panel1: TPanel;
    lvAssignedUsers: TListView;
    Splitter1: TSplitter;
    Panel2: TPanel;
    Panel3: TPanel;
    btAdd: TButton;
    btDelete: TButton;
    Label3: TLabel;
    cbAssAssignedUserType: TComboBox;
    Label4: TLabel;
    cbAssAssignedUsers: TComboBox;
    Label1: TLabel;
    cbCanPrint: TCheckBox;
    cbkAssignmentMode: TCheckBox;
    Label2: TLabel;
    cbStepDefin: TEdit;
    cbNeedPrjRight: TCheckBox;
    procedure btAddClick(Sender: TObject);
    procedure lvAssignedUsersSelectItem(Sender: TObject; Item: TListItem;
      Selected: Boolean);
    procedure PropertyChanged(Sender: TObject);
    procedure btDeleteClick(Sender: TObject);
    procedure cbAssAssignedUserTypeChange(Sender: TObject);
    procedure cbStepDefinKeyPress(Sender: TObject; var Key: Char);
  private
    { Private declarations }
    FAssignedUsers: TTaskAssignedUsers;
    FUpdatingControls: integer;
    FPreAssignedUserType: TAssignedUserType;
    FDiagram: TCustomWorkflowDiagram;
    procedure BeginUpdateControls;
    procedure DeleteCurrentVar;
    procedure EndUpdateControls;
    procedure LoadVarInEditors(AVar: TTaskAssignedUser);
    procedure SaveEditorsInVar(AVar: TTaskAssignedUser);
    procedure SetControlsEnabled(AEnabled: boolean);
    procedure UpdateListItem(AItem: TListItem; AVar: TTaskAssignedUser);
    procedure VarsToInterface;
    procedure ClearEditors;
    procedure FillAssignedUsers();
    function  GetAssignedUserID(): string;
    procedure Localize;
    function TaskAssignedUserFromItem(AItem: TListItem): TTaskAssignedUser;
  protected
    procedure Loaded; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure LoadTaskInEditors(ATask: TTaskDefinition; ADiagram: TCustomWorkflowDiagram);
    procedure SaveEditorsInTask(ATask: TTaskDefinition);
    procedure ClearAll;
  end;

implementation
uses wsMain, wsBlocks, wsRes;

{$R *.DFM}

procedure TfrAssignedUserFrame.VarsToInterface;
var
  c: integer;
begin
  lvAssignedUsers.Items.Clear;
  ClearEditors;
  for c := 0 to FAssignedUsers.Count - 1 do
    UpdateListItem(lvAssignedUsers.Items.Add, FAssignedUsers[c]);
  if lvAssignedUsers.Items.Count > 0 then
    lvAssignedUsers.Selected := lvAssignedUsers.Items[0];
end;

procedure TfrAssignedUserFrame.UpdateListItem(AItem: TListItem; AVar: TTaskAssignedUser);
begin

  AItem.Caption := AVar.AssignedUserName;
  if AItem.SubItems.Count >0 then
  begin
    AItem.SubItems.Strings[0] := IntToStr(AVar.StepDefin);
    AItem.SubItems.Strings[1]:=cbAssAssignedUserType.Items[Ord(AVar.AssignedUserType)];
    if AVar.CanPrint then
      AItem.SubItems.Strings[2]:='是'
    else
      AItem.SubItems.Strings[2]:='否';
    if AVar.AssignmentMode=gamMultipleTasks then
       AItem.SubItems.Strings[3]:='多任务模式'
    else
       AItem.SubItems.Strings[3]:='单任务模式';
    if AVar.NeedPrjRight then
      AItem.SubItems.Strings[4] :='是'
     else
        AItem.SubItems.Strings[4] :='否';
  end
  else
  begin
    AItem.SubItems.Add(IntToStr(AVar.StepDefin));
    AItem.SubItems.Add(cbAssAssignedUserType.Items[Ord(AVar.AssignedUserType)]);
    if AVar.CanPrint then
       AItem.SubItems.Add('是')
    else
       AItem.SubItems.Add('否');
    if AVar.AssignmentMode=gamMultipleTasks then
       AItem.SubItems.Add('多任务模式')
    else
       AItem.SubItems.Add('单任务模式');
    if AVar.NeedPrjRight then
      AItem.SubItems.Add('是')
     else
        AItem.SubItems.Add('否');
  end;
  {$WARNINGS OFF}
  AItem.Data := AVar;
  {$WARNINGS ON}
end;

procedure TfrAssignedUserFrame.btAddClick(Sender: TObject);
var
  NewVar: TTaskAssignedUser;
  //NewName: string;
  //i: integer;
  NewItem: TListItem;
begin
  if lvAssignedUsers.Enabled then
  begin
    {Add variable in collection}
    ClearEditors;
    NewVar := FAssignedUsers.Add;
    NewVar.CanPrint:=cbCanPrint.Checked;
    NewVar.NeedPrjRight := cbNeedPrjRight.Checked;
    if cbkAssignmentMode.Checked then
       NewVar.AssignmentMode:=gamMultipleTasks
    else
       NewVar.AssignmentMode:=gamSingleTask;
    cbAssAssignedUserType.ItemIndex:=0;
    NewVar.AssignedUserType:=TAssignedUserType(cbAssAssignedUserType.ItemIndex);
 //   NewVar.AssignedUserName:=cbAssAssignedUsers.Text;
    NewVar.AssignedUserID:=GetAssignedUserID();   //需要记录id
    NewVar.AssignedUserName:=cbAssAssignedUsers.Items[cbAssAssignedUsers.ItemIndex];
    NewVar.StepDefin:=StrToInt(cbStepDefin.Text);
    {Create item in list view}
    NewItem := lvAssignedUsers.Items.Add;
    UpdateListItem(NewItem, NewVar);

    {select and start renaming the item. EditCaption already selects the item}
//    NewItem.EditCaption;
  end;
end;

function TfrAssignedUserFrame.TaskAssignedUserFromItem(AItem: TListItem): TTaskAssignedUser;
begin
  {$WARNINGS OFF}
  result := TTaskAssignedUser(AItem.Data);
  {$WARNINGS ON}
end;

procedure TfrAssignedUserFrame.lvAssignedUsersSelectItem(Sender: TObject;
  Item: TListItem; Selected: Boolean);
var
  NewVar: TTaskAssignedUser;
begin
 try
  BeginUpdateControls;
  if lvAssignedUsers.SelCount >0 then
  begin  //请注意顺序
     NewVar:=TaskAssignedUserFromItem(lvAssignedUsers.Selected);
     LoadVarInEditors(NewVar);
//     cbAssAssignedUserTypeChange(nil);
     cbAssAssignedUsers.Text:=NewVar.AssignedUserName;
  end
  else
    ClearEditors;
 finally
    EndUpdateControls;
 end;
end;

procedure TfrAssignedUserFrame.LoadVarInEditors(AVar: TTaskAssignedUser);
var
  c: integer;
  AUser :TWorkflowUser;
  FGroup: TWorkflowGroup;
  FJob: TWorkflowJob;
  FDepartment: TWorkflowDepartment;
begin
  if AVar <> nil then
  begin
    BeginUpdateControls;
    try
      cbAssAssignedUserType.ItemIndex:=Ord(AVar.AssignedUserType);
      cbAssAssignedUserTypeChange(nil);
//      cbAssAssignedUsers.Text:=AVar.AssignedUserName;
//      aa := cbAssAssignedUsers.Items;
      case TAssignedUserType(cbAssAssignedUserType.ItemIndex) of
        autUser:
        begin
          for c := 0 to cbAssAssignedUsers.Items.Count - 1 do
          begin
            AUser:=TWorkflowUser(cbAssAssignedUsers.Items.Objects[c]);
            if Assigned(AUser) then
              if AUser.UserId = AVar.AssignedUserID then
              begin
                cbAssAssignedUsers.ItemIndex := c;
                break;
              end;
          end;
        end;
        autGroup:
        begin
          for c := 0 to cbAssAssignedUsers.Items.Count - 1 do
          begin
            FGroup:=TWorkflowGroup(cbAssAssignedUsers.Items.Objects[c]);
            if Assigned(FGroup) then
              if FGroup.GroupId = AVar.AssignedUserID then
              begin
                cbAssAssignedUsers.ItemIndex := c;
                break;
              end;
          end;
        end;
        autJob:
        begin
          for c := 0 to cbAssAssignedUsers.Items.Count - 1 do
          begin
            FJob:= TWorkflowJob(cbAssAssignedUsers.Items.Objects[c]);
            if Assigned(FJob) then
              if FJob.JobId = AVar.AssignedUserID then
              begin
                cbAssAssignedUsers.ItemIndex := c;
                break;
              end;
          end;
        end;
        autDepartment:
        begin
          for c := 0 to cbAssAssignedUsers.Items.Count - 1 do
          begin
            FDepartment:=TWorkflowDepartment(cbAssAssignedUsers.Items.Objects[c]);
            if Assigned(FDepartment) then
              if FDepartment.DepartmentId = AVar.AssignedUserID then
              begin
                cbAssAssignedUsers.ItemIndex := c;
                break;
              end;
          end;
        end;
        autDepartmentManager:
          cbAssAssignedUsers.Text:=AVar.AssignedUserName;
      end;

      cbStepDefin.Text:=IntToStr(AVar.StepDefin);
      cbCanPrint.Checked:=AVar.CanPrint;
      cbNeedPrjRight.Checked :=AVar.NeedPrjRight;
    if AVar.AssignmentMode=gamMultipleTasks then
       cbkAssignmentMode.Checked :=True
    else
       cbkAssignmentMode.Checked :=False;
      SetControlsEnabled(true);
    finally
      EndUpdateControls;
    end;
  end;
end;


procedure TfrAssignedUserFrame.SaveEditorsInVar(AVar: TTaskAssignedUser);
var
 c: integer;
 cItem: TListItem;
 AssignedUser: TTaskAssignedUser;
begin
  if AVar <> nil then
  begin
  AVar.AssignedUserType:=TAssignedUserType(cbAssAssignedUserType.ItemIndex);
  AVar.CanPrint:=cbCanPrint.Checked;
  AVar.NeedPrjRight := cbNeedPrjRight.Checked;
  if cbkAssignmentMode.Checked then
       AVar.AssignmentMode:=gamMultipleTasks
  else
       AVar.AssignmentMode:=gamSingleTask;
 // AVar.AssignedUserName:=cbAssAssignedUsers.Text;
  AVar.AssignedUserID:=GetAssignedUserID();//                     需要保存ID
  AVar.AssignedUserName:=cbAssAssignedUsers.Items[cbAssAssignedUsers.ItemIndex];
  AVar.StepDefin:=StrToInt(cbStepDefin.Text);
  end;
    //判断是否重复
    for c := 0 to lvAssignedUsers.Items.Count - 1 do
     begin
       cItem:=lvAssignedUsers.Items[c];
       if cItem<>lvAssignedUsers.Selected then
       begin
          AssignedUser := TaskAssignedUserFromItem(cItem);
         { if AVar.StepDefin=AssignedUser.StepDefin then
           begin
            ShowMessage('该步骤号在列表中已存在');
            break;
           end; }
          if AVar.AssignedUserID=AssignedUser.AssignedUserID then
           begin
            ShowMessage('该操作员在列表中已存在');
            break;
           end;
       end;
     end;
end;

procedure TfrAssignedUserFrame.ClearEditors;
begin
  BeginUpdateControls;
  try
    cbAssAssignedUserType.ItemIndex := Ord(FPreAssignedUserType);
    cbCanPrint.Checked :=false;
    cbNeedPrjRight.Checked :=false;
    cbkAssignmentMode.Checked :=false;
    cbAssAssignedUsers.Clear;    
    FPreAssignedUserType:= autDepartmentManager;
   // FillAssignedUsers();
    cbStepDefin.Text := '0';
    SetControlsEnabled(false);
  finally
    EndUpdateControls;
  end;
end;

procedure TfrAssignedUserFrame.PropertyChanged(Sender: TObject);
begin
  if (lvAssignedUsers.SelCount >0) and (FUpdatingControls = 0) then
  begin
    SaveEditorsInVar(TaskAssignedUserFromItem(lvAssignedUsers.Selected));
    if lvAssignedUsers.Selected <> nil then
      UpdateListItem(lvAssignedUsers.Selected, TaskAssignedUserFromItem(lvAssignedUsers.Selected));
  end;
end;

procedure TfrAssignedUserFrame.btDeleteClick(Sender: TObject);
begin
  if lvAssignedUsers.Enabled then
  begin
    if MessageDlg(_str(SConfirmDeleteItems), 
       mtConfirmation,[mbYes,mbNo],0) = mrYes then
    begin
       while lvAssignedUsers.SelCount > 0 do
          DeleteCurrentVar;
       if lvAssignedUsers.Items.Count > 0 then
          lvAssignedUsers.Selected := lvAssignedUsers.Items[lvAssignedUsers.items.Count - 1]
       else
          ClearEditors;
       lvAssignedUsers.SetFocus;
    end;
  end;
end;

procedure TfrAssignedUserFrame.cbAssAssignedUserTypeChange(Sender: TObject);
var
  curAssignedUserType :TAssignedUserType;
begin
  curAssignedUserType:=TAssignedUserType(cbAssAssignedUserType.ItemIndex);
   FillAssignedUsers();
   case curAssignedUserType of
   autUser:
      begin
        cbkAssignmentMode.Enabled :=false;
        cbkAssignmentMode.Checked :=false;
        cbNeedPrjRight.Enabled :=False;
        cbNeedPrjRight.Checked :=False;
      end;
   autGroup:
      begin
        cbkAssignmentMode.Enabled :=True;
       // cbkAssignmentMode.Checked :=false;
        cbNeedPrjRight.Enabled :=True;
//        cbNeedPrjRight.Checked :=False;
      end;
   autJob:
       begin
        cbkAssignmentMode.Enabled :=True;
       // cbkAssignmentMode.Checked :=false;
        cbNeedPrjRight.Enabled :=True;
//        cbNeedPrjRight.Checked :=False;
      end;
   autDepartment:
      begin
        cbkAssignmentMode.Enabled :=True;
       // cbkAssignmentMode.Checked :=false;
       cbNeedPrjRight.Enabled :=False;
//        cbNeedPrjRight.Checked :=False;
      end;
   autDepartmentManager:
     begin
        cbkAssignmentMode.Enabled :=false;
        cbkAssignmentMode.Checked :=false;
        cbNeedPrjRight.Enabled :=False;
        cbNeedPrjRight.Checked :=False;
      end;
   end;
   PropertyChanged(Sender);
end;

procedure TfrAssignedUserFrame.cbStepDefinKeyPress(Sender: TObject;
  var Key: Char);
begin
      if (Key<>#8) and (Key<>#13) and ((Key<#48) or (Key>#57)) then
            Key:=#0; //不是数字
end;

procedure TfrAssignedUserFrame.DeleteCurrentVar;
begin
  if Assigned(lvAssignedUsers.Selected) then
  begin
    TaskAssignedUserFromItem(lvAssignedUsers.Selected).Free;
    {$WARNINGS OFF}
    lvAssignedUsers.Selected.Data := nil;
    {$WARNINGS ON}
    lvAssignedUsers.Selected.Free;
  end;
end;

constructor TfrAssignedUserFrame.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FAssignedUsers := TTaskAssignedUsers.Create(nil, TTaskAssignedUser);
end;

destructor TfrAssignedUserFrame.Destroy;
begin
  FAssignedUsers.Free;
  inherited;
end;

procedure TfrAssignedUserFrame.BeginUpdateControls;
begin
  inc(FUpdatingControls);
end;

procedure TfrAssignedUserFrame.EndUpdateControls;
begin
  dec(FUpdatingControls);
end;



procedure TfrAssignedUserFrame.SetControlsEnabled(AEnabled: boolean);
begin
  cbAssAssignedUserType.Enabled := AEnabled;
  cbAssAssignedUsers.Enabled := AEnabled;
  cbStepDefin.Enabled := AEnabled;
  cbCanPrint.Enabled := AEnabled;
  cbkAssignmentMode.Enabled := AEnabled;
  cbNeedPrjRight.Enabled := AEnabled;
end;

procedure TfrAssignedUserFrame.LoadTaskInEditors(ATask: TTaskDefinition; ADiagram: TCustomWorkflowDiagram);
begin
  FDiagram := ADiagram;
  FAssignedUsers.Assign(ATask.Users);
  lvAssignedUsers.Enabled := true;
  btAdd.Enabled := true;
  btDelete.Enabled := true;
  FillAssignedUsers();
  VarsToInterface;
end;

procedure TfrAssignedUserFrame.SaveEditorsInTask(ATask: TTaskDefinition);
begin
  ATask.Users.Assign(FAssignedUsers);
end;

procedure TfrAssignedUserFrame.FillAssignedUsers();
var
  curAssignedUserType :TAssignedUserType;
begin
curAssignedUserType:=TAssignedUserType(cbAssAssignedUserType.ItemIndex);
 if (FPreAssignedUserType <> curAssignedUserType) and (FDiagram<>nil) then
 begin
     cbAssAssignedUsers.Items.Clear;
    case curAssignedUserType of
    autUser:
      FDiagram.WorkflowStudio.UserManager.FillUserList(cbAssAssignedUsers.Items,True);
    autGroup:
      FDiagram.WorkflowStudio.UserManager.FillGroupList(cbAssAssignedUsers.Items,True);
    autJob:
      FDiagram.WorkflowStudio.UserManager.FillJobList(cbAssAssignedUsers.Items,True);
    autDepartment:
      FDiagram.WorkflowStudio.UserManager.FillDepartmentList(cbAssAssignedUsers.Items,True);
    autDepartmentManager:
      begin
        cbAssAssignedUsers.Items.Clear;
        cbAssAssignedUsers.Items.Add('部门负责人');
     end;
    end;
    FPreAssignedUserType:= curAssignedUserType;
    if cbAssAssignedUsers.Items.Count>0 then
     cbAssAssignedUsers.ItemIndex:=0;
 end;
end;

function TfrAssignedUserFrame.GetAssignedUserID():string;
var
 AUser :TWorkflowUser;
 FGroup: TWorkflowGroup;
 FJob: TWorkflowJob;
 FDepartment: TWorkflowDepartment;
begin
  result:='';
  if cbAssAssignedUsers.ItemIndex <0 then
  begin
    result:='';
    Exit;
  end;
  
 case TAssignedUserType(cbAssAssignedUserType.ItemIndex) of
   autUser:
     begin
      AUser:=TWorkflowUser(cbAssAssignedUsers.Items.Objects[cbAssAssignedUsers.ItemIndex]);
      if Assigned(AUser) then
        result:=AUser.UserId;
      end;
   autGroup:
   begin
     FGroup:=TWorkflowGroup(cbAssAssignedUsers.Items.Objects[cbAssAssignedUsers.ItemIndex]);
     if Assigned(FGroup) then
        result:=FGroup.GroupId;
    end;
   autJob:
   begin
      FJob:= TWorkflowJob(cbAssAssignedUsers.Items.Objects[cbAssAssignedUsers.ItemIndex]);
      if Assigned(FJob) then
        result:=FJob.JobId;
   end;
   autDepartment:
   begin
     FDepartment:=TWorkflowDepartment(cbAssAssignedUsers.Items.Objects[cbAssAssignedUsers.ItemIndex]);
     if Assigned(FDepartment) then
       result:=FDepartment.DepartmentId;
   end;
   autDepartmentManager:
     result:='';
 end;
end;
procedure TfrAssignedUserFrame.ClearAll;
begin
  lvAssignedUsers.Items.Clear;
  lvAssignedUsers.Enabled := false;
  btAdd.Enabled := false;
  btDelete.Enabled := false;
  ClearEditors;
end;

procedure TfrAssignedUserFrame.Localize;
begin
 { btAdd.Caption := '增加';//_str('frAssignedUserFrame.btAdd.Caption');
  btDelete.Caption :='删除';//= _str('frAssignedUserFrame.btDelete.Caption');
  }
end;

procedure TfrAssignedUserFrame.Loaded;
begin
  inherited;
  FPreAssignedUserType:= autDepartmentManager;
  Localize;
end;

end.
