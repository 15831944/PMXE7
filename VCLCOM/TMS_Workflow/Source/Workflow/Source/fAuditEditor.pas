unit fAuditEditor;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, StdCtrls, wsClasses, wsBlocks;

type
  TfmAuditEditor = class(TForm) 
    PageControl1: TPageControl;
    TabSheet1: TTabSheet;
    Label1: TLabel;
    edCondition: TEdit;
    btCancel: TButton;
    btOk: TButton;
    procedure btOkClick(Sender: TObject);
    procedure btCancelClick(Sender: TObject);
    procedure edConditionKeyPress(Sender: TObject; var Key: Char);
  private
    { Private declarations }
    FBlock: TWorkflowAuditBlock;
    procedure BlockToInterface;
    procedure InterfaceToBlock;
    procedure Localize;
  protected
    procedure Loaded; override;
  public
    { Public declarations }
    function EditAuditBlock(ABlock: TWorkflowAuditBlock): boolean;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  end;

implementation
uses wsMain, wsRes;

{$R *.DFM}

{ TForm1 }

function TfmAuditEditor.EditAuditBlock(ABlock: TWorkflowAuditBlock): boolean;
begin
  FBlock := ABlock;
  BlockToInterface;
  result := ShowModal = mrOk;
  if result then
    InterfaceToBlock;
end;

procedure TfmAuditEditor.btOkClick(Sender: TObject);
begin
  ModalResult := mrOk;
end;

procedure TfmAuditEditor.btCancelClick(Sender: TObject);
begin
  ModalResult := mrCancel;
end;

procedure TfmAuditEditor.BlockToInterface;
begin
  edCondition.Text := FBlock.BillState;
end;

constructor TfmAuditEditor.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
end;

destructor TfmAuditEditor.Destroy;
begin
  inherited;
end;

procedure TfmAuditEditor.edConditionKeyPress(Sender: TObject; var Key: Char);
begin

      if (edCondition.SelStart=0)  then
      begin
       if (Key<>#8) and (Key<>#13) and(Key<>#45) and ((Key<#48) or (Key>#57)) then
            Key:=#0; //不是数字
      end
      else
         begin
          if (Key<>#8) and (Key<>#13) and ((Key<#48) or (Key>#57)) then
              Key:=#0; //不是数字
         end;
end;

procedure TfmAuditEditor.InterfaceToBlock;
begin
  FBlock.BillState := edCondition.Text;
end;

procedure TfmAuditEditor.Localize;
begin
  Self.Caption := '改变单据状态';
  TabSheet1.Caption := '状态值';
  Label1.Caption := '单据要改变的状态值';
  btCancel.Caption := '取消';
  btOk.Caption := '确定';
end;

procedure TfmAuditEditor.Loaded;
begin
  inherited;
  Localize;
end;

end.
