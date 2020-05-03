unit fTaskInstanceView;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, wsControls, wsClasses;

type
  TfrTaskInstanceView = class(TFrame)
    Panel1: TPanel;
    Label1: TLabel;
    lbSubject: TLabel;
    mmDescription: TMemo;
    Panel2: TPanel;
    Label2: TLabel;
    cbTaskStatus: TTaskStatusCombo;
    lbExpiration: TLabel;
  private
    FTask: TTaskInstance;
    FExpirationDateTime: TDateTime;
    procedure SetTask(const Value: TTaskInstance);
    procedure TaskToInterface;
    procedure Localize;
    procedure ShowTaskExpiration;
    procedure SetExpirationDateTime(const Value: TDateTime);
  protected
    procedure Loaded; override;
  public
    property Task: TTaskInstance read FTask write SetTask;
    procedure SaveEditorsInVar();
    property ExpirationDateTime: TDateTime read FExpirationDateTime write SetExpirationDateTime;
  end;

implementation

uses
  wsRes;

{$R *.DFM}

{ TFrame1 }

procedure TfrTaskInstanceView.SetTask(const Value: TTaskInstance);
begin
  FTask := Value;
  TaskToInterface;
end;

procedure TfrTaskInstanceView.ShowTaskExpiration;
begin                                 
  if (FExpirationDateTime > 0) and (FTask <> nil) and not FTask.Completed then
    lbExpiration.Caption := _str(SExpiration) + ': ' + DateTimeToStr(FExpirationDateTime)
  else
    lbExpiration.Caption := '';
end;

procedure TfrTaskInstanceView.TaskToInterface;
begin
  cbTaskStatus.TaskInstance := FTask;
  if FTask <> nil then
  begin
    mmDescription.Lines.Text := FTask.TaskDef.Description;
    lbSubject.Caption := FTask.TaskDef.Subject;
    cbTaskStatus.Enabled := not FTask.Completed;
    ShowTaskExpiration;
  end
  else
  begin
    mmDescription.Lines.Text := '';
    lbSubject.Caption := '';
    cbTaskStatus.Enabled := false;
    lbExpiration.Caption := '';
  end;
end;

procedure TfrTaskInstanceView.SaveEditorsInVar();
begin
   if FTask <> nil then
      FTask.Status := cbTaskStatus.Status;
end;
procedure TfrTaskInstanceView.Localize;
begin
  Label1.Caption := _str('frTaskInstanceView.Label1.Caption');
  Label2.Caption := _str('frTaskInstanceView.Label2.Caption');
end;

procedure TfrTaskInstanceView.Loaded;
begin
  inherited;
  Localize;
end;

procedure TfrTaskInstanceView.SetExpirationDateTime(const Value: TDateTime);
begin
  FExpirationDateTime := Value;
  ShowTaskExpiration;
end;

end.

