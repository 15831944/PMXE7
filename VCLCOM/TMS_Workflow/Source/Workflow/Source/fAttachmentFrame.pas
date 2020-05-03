unit fAttachmentFrame;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, ComCtrls, wsControls, Menus, ActnList, wsClasses, ExtCtrls, wsMain,
  System.Actions;

type
  TfrAttachmentFrame = class(TFrame)
    OpenDialog1: TOpenDialog;
    PopupMenu1: TPopupMenu;
    miInsert: TMenuItem;
    miRemove: TMenuItem;
    miOpen: TMenuItem;
    ActionList1: TActionList;
    acInsert: TAction;
    acRemove: TAction;
    acOpen: TAction;
    acSmallIcons: TAction;
    acLargeIcons: TAction;
    acReport: TAction;
    acList: TAction;
    N1: TMenuItem;
    Largeicons1: TMenuItem;
    Smallicons1: TMenuItem;
    List1: TMenuItem;
    Report1: TMenuItem;
    Panel1: TPanel;
    lvItems: TAttachmentListView;
    Panel2: TPanel;
    btAttach: TSpeedButton;
    btRemove: TSpeedButton;
    btOpen: TSpeedButton;
    procedure acInsertExecute(Sender: TObject);
    procedure acRemoveExecute(Sender: TObject);
    procedure acOpenExecute(Sender: TObject);
    procedure IconExecute(Sender: TObject);
    procedure IconUpdate(Sender: TObject);
    procedure acRemoveUpdate(Sender: TObject);
    procedure acOpenUpdate(Sender: TObject);
    procedure lvItemsKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure acInsertUpdate(Sender: TObject);
  private
    FPermissions: TAttachmentPermissions;
    procedure SetPermissions(const Value: TAttachmentPermissions);
    procedure Localize;
    function GetWorkflowStudio: TCustomWorkflowStudio;
    procedure SetWorkflowStudio(const Value: TCustomWorkflowStudio);
  protected
    procedure Loaded; override;
  public
    constructor Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio); reintroduce; virtual; 
    property Permissions: TAttachmentPermissions read FPermissions write SetPermissions;
    property WorkflowStudio: TCustomWorkflowStudio read GetWorkflowStudio write SetWorkflowStudio;
  end;

implementation

uses
  wsRes;

{$R *.DFM}

procedure TfrAttachmentFrame.acInsertExecute(Sender: TObject);
var
  c: integer;
begin
  if OpenDialog1.Execute then
  begin
    for c := 0 to OpenDialog1.Files.Count - 1 do
      lvItems.AddFile(OpenDialog1.Files[c]);
  end;
end;

procedure TfrAttachmentFrame.acRemoveExecute(Sender: TObject);
begin
  if (lvItems.SelCount > 0) and (MessageDlg(_str(SConfirmDeleteAttachments), mtConfirmation,
    [mbYes, mbNo], 0) = mrYes) then
    lvItems.DeleteSelecteds;
end;

procedure TfrAttachmentFrame.acOpenExecute(Sender: TObject);
begin
  if lvItems.SelCount > 0 then
  begin
    if (lvItems.SelCount = 1) or (MessageDlg(_str(SConfirmOpenMoreThanOneFile),
      mtConfirmation, [mbYes, mbNo], 0) = mrYes) then
    begin
      lvItems.OpenSelecteds(apEdit in FPermissions);
    end;
  end;
end;

procedure TfrAttachmentFrame.IconExecute(Sender: TObject);
var
  ViewStyle: TViewStyle;
begin
  ViewStyle := TViewStyle(TComponent(Sender).Tag);
  lvItems.ViewStyle := ViewStyle;
  lvItems.UpdateItems;
end;

procedure TfrAttachmentFrame.IconUpdate(Sender: TObject);
begin
  TAction(Sender).Checked := lvItems.ViewStyle = TViewStyle(TComponent(Sender).Tag);
  TAction(Sender).Enabled := lvItems.Enabled;
end;

procedure TfrAttachmentFrame.acRemoveUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := (lvItems.SelCount > 0) and (apDelete in FPermissions) and (lvItems.Enabled);
  TAction(Sender).Visible := apDelete in FPermissions;
end;

procedure TfrAttachmentFrame.acOpenUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := (lvItems.SelCount > 0) and (lvItems.Enabled);
end;

procedure TfrAttachmentFrame.lvItemsKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if (Key = VK_RETURN) and (lvItems.SelCount > 0) then
    acOpenExecute(acOpen);
end;

procedure TfrAttachmentFrame.acInsertUpdate(Sender: TObject);
begin
  TAction(Sender).Enabled := (apInsert in FPermissions) and (lvItems.Enabled);
  TAction(Sender).Visible := (apInsert in FPermissions);
end;

procedure TfrAttachmentFrame.SetPermissions(
  const Value: TAttachmentPermissions);
begin
  FPermissions := Value;
  lvItems.AllowDropFiles := apInsert in FPermissions;
end;

procedure TfrAttachmentFrame.Localize;
begin
  OpenDialog1.Filter := _str('frAttachmentFrame.OpenDialog1.Filter');
  OpenDialog1.Title := _str('frAttachmentFrame.OpenDialog1.Title');
  miOpen.Caption := _str('frAttachmentFrame.miOpen.Caption');
  miInsert.Caption := _str('frAttachmentFrame.miInsert.Caption');
  miRemove.Caption := _str('frAttachmentFrame.miRemove.Caption');
  acInsert.Hint := _str('frAttachmentFrame.acInsert.Hint');
  acRemove.Hint := _str('frAttachmentFrame.acRemove.Hint');
  acOpen.Hint := _str('frAttachmentFrame.acOpen.Hint');
  acLargeIcons.Caption := _str('frAttachmentFrame.acLargeIcons.Caption');
  acLargeIcons.Hint := _str('frAttachmentFrame.acLargeIcons.Hint');
  acSmallIcons.Caption := _str('frAttachmentFrame.acSmallIcons.Caption');
  acSmallIcons.Hint := _str('frAttachmentFrame.acSmallIcons.Hint');
  acList.Caption := _str('frAttachmentFrame.acList.Caption');
  acList.Hint := _str('frAttachmentFrame.acList.Hint');
  acReport.Caption := _str('frAttachmentFrame.acReport.Caption');
  acReport.Hint := _str('frAttachmentFrame.acReport.Hint');
end;

procedure TfrAttachmentFrame.Loaded;
begin
  inherited;
  with lvItems.Columns.Add do
  begin
    AutoSize := true;
    Caption := _str(SFileName);
  end;
  Localize;
end;

function TfrAttachmentFrame.GetWorkflowStudio: TCustomWorkflowStudio;
begin
  result := lvItems.WorkflowStudio;
end;

procedure TfrAttachmentFrame.SetWorkflowStudio(const Value: TCustomWorkflowStudio);
begin
  lvItems.WorkflowStudio := Value as TWorkflowStudio;
end;
                                
constructor TfrAttachmentFrame.Create(AOwner: TComponent; AWorkflowStudio: TWorkflowStudio);
begin
  inherited Create(AOwner);
  WorkflowStudio := AWorkflowStudio;
end;

end.

