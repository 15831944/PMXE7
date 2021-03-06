unit DiagramEditor;

{$I DIAGRAM.INC}

interface
uses Classes, Forms,
  fDiagramEditor,
  atDiagram;

type

  /// Description:
  ///   TDiagramEditor is a wrapper for a full-feature diagram designer. Using TDiagramEditor you don't have
  ///   to worry about building a new diagram editor, including menu, toolbar, etc.. Just drop a TDiagramEditor
  ///   in the form, call Execute method and a diagram editor form will be displayed.
  TDiagramEditor = class(TComponent)
  private
    FOnCloseDesigner: TNotifyEvent;
    FOnCreateDesigner: TNotifyEvent;
    FOnShowDesigner: TNotifyEvent;
    FForm: TfmDiagramEditor;
    FTitle: string;
    FDiagramClass: TatDiagramClass;
    FOnSaveDiagram: TNotifyEvent;
    FLibManager: TDgrLibraryManager;
    FDiagramInstance: TatDiagram;
    procedure CreateDesignerForm;
    procedure FormClosed(Sender: TObject; var Action: TCloseAction);
    procedure SetLibManager(const Value: TDgrLibraryManager);
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    /// Description:
    ///   Call Execute method to open the editor form and start diagram editing.
    procedure Execute;

    /// Description:
    ///   Specifies the class of the diagram to be used in the diagram editor. You can use your own specific
    ///   descendant for TatDiagram.
    property DiagramClass: TatDiagramClass read FDiagramClass write FDiagramClass;

    /// Description:
    ///   Specifies the diagram instance to be edited by diagram editor. If not assigned,
    ///   designer will create a new diagram instance when displayed.
    property Diagram: TatDiagram read FDiagramInstance write FDiagramInstance; 

    /// Description:
    ///   OnCreateDesigner is called when the designer form is created. The form is passed as the Sender.
    ///   You can do some low level customization by casting Sender to a TForm and creating/removing components
    ///   in the form.
    property OnCreateDesigner: TNotifyEvent read FOnCreateDesigner write FOnCreateDesigner;

    /// Description:
    ///   OnShowDesigner is called when the designer form is displayed. The form is passed as the Sender.
    ///   You can do some low level customization by casting Sender to a TForm and creating/removing components
    ///   in the form.
    property OnShowDesigner: TNotifyEvent read FOnShowDesigner write FOnShowDesigner;

    /// Description:
    ///   OnSaveDiagram event is called whenever the user saves the diagram.
    property OnSaveDiagram: TNotifyEvent read FOnSaveDiagram write FOnSaveDiagram;
  published

    /// Description:
    ///   Use Title property to set the caption of diagram designer editor window.  
    property Title: string read FTitle write FTitle;

    /// Description:
    ///   OnCloseDesigner event is fired when the editor form is closed by the end-user.
    property OnCloseDesigner: TNotifyEvent read FOnCloseDesigner write FOnCloseDesigner;
    property LibManager: TDgrLibraryManager read FLibManager write SetLibManager;
  end;

implementation


{ TDiagramEditor }

procedure TDiagramEditor.Execute;
begin
  CreateDesignerForm;
  try
    if Assigned(FOnCreateDesigner) then
      FOnCreateDesigner(FForm);
    FForm.OnSaveDiagram := FOnSaveDiagram;
    FForm.OnNotifyShow := FOnShowDesigner;
    FForm.LibManager := FLibManager;
    FForm.Title := FTitle;
    FForm.ShowModal;
  finally
    FForm.Free;
    FForm := nil;
  end;
end;

procedure TDiagramEditor.FormClosed(Sender: TObject; var Action: TCloseAction);
begin
  if Assigned(FOnCloseDesigner) then
    FOnCloseDesigner(FForm);
end;

procedure TDiagramEditor.Notification(AComponent: TComponent; Operation: TOperation);
begin
  inherited;
  if Operation = opRemove then
  begin
    if AComponent = FLibManager then
      FLibManager := nil;
  end;
end;

procedure TDiagramEditor.SetLibManager(const Value: TDgrLibraryManager);
begin
  if FLibManager <> Value then
  begin
    if FLibManager <> nil then
      FLibManager.RemoveFreeNotification(Self);
    FLibManager := Value;
    if FLibManager <> nil then
      FLibManager.FreeNotification(Self);
  end;
end;

procedure TDiagramEditor.CreateDesignerForm;
begin
  if FDiagramInstance <> nil then
    FForm := TfmDiagramEditor.CreateWithInstance(Self, FDiagramInstance)
  else if FDiagramClass <> nil then
    FForm := TfmDiagramEditor.CreateWithClass(Self, FDiagramClass)
  else
    FForm := TfmDiagramEditor.Create(Self);
  FForm.OnClose := FormClosed;
end;

constructor TDiagramEditor.Create(AOwner: TComponent);
begin
  inherited;
  FTitle := '����ͼ�༭��';
  FDiagramClass := nil;
  FDiagramInstance := nil;
end;

destructor TDiagramEditor.Destroy;
begin
  inherited;
end;

end.

