{***************************************************************************}
{ Diagram Studio                                                            }
{ for Delphi & C++Builder                                                   }
{                                                                           }
{ written by TMS Software                                                   }
{            copyright � 2003-2011                                          }
{            Email : info@tmssoftware.com                                   }
{            Web : http://www.tmssoftware.com                               }
{                                                                           }
{ The source code is given as is. The author is not responsible             }
{ for any possible damage done due to the use of this code.                 }
{ The components can be freely used in any application. The complete        }
{ source code remains property of the author and may not be distributed,    }
{ published, given or sold in any form as such. No parts of the source      }
{ code can be included in any other component or application without        }
{ written authorization of the author.                                      }
{***************************************************************************}

unit fDiagramEditor;

{$I DIAGRAM.INC}

interface

uses
  Windows, Messages, SysUtils,
  {$IFDEF DELPHI6_LVL}
  Variants,
  {$ENDIF}
  Classes, Graphics, Controls, Forms,
  Dialogs, atDiagram, ExtCtrls, DiagramExtra, ImgList, StdCtrls, Menus,
  DgrEditorActions,
  ComCtrls, ToolWin,
  {$IFDEF DELPHI9_LVL}
  CategoryButtons, DgrButtons, DockTabSet, Tabs,
  {$ENDIF}
  DgrSelectors, DgrCombo, ActnList,
  fDgrLayerManager, fDgrLibraryManagerEditor;

type
  TfmDiagramEditor = class(TForm, IDiagramSubscriber)
    MainMenu1: TMainMenu;
    Edittext1: TMenuItem;
    Edit1: TMenuItem;
    File1: TMenuItem;
    Opendiagram1: TMenuItem;
    Savediagram1: TMenuItem;
    Copyasimage1: TMenuItem;
    Print1: TMenuItem;
    Edit2: TMenuItem;
    Cut1: TMenuItem;
    Copy1: TMenuItem;
    Paste1: TMenuItem;
    Delete1: TMenuItem;
    Selectall1: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    Newdiagram1: TMenuItem;
    N1: TMenuItem;
    StatusBar1: TStatusBar;
    Preview1: TMenuItem;
    View1: TMenuItem;
    Leftruler1: TMenuItem;
    Topruler1: TMenuItem;
    N2: TMenuItem;
    Zoom1: TMenuItem;
    N1001: TMenuItem;
    N5: TMenuItem;
    N3001: TMenuItem;
    N1501: TMenuItem;
    N751: TMenuItem;
    N501: TMenuItem;
    N251: TMenuItem;
    N101: TMenuItem;
    PopupMenu1: TPopupMenu;
    DiagramShapeColor11: TMenuItem;
    DiagramLineColor11: TMenuItem;
    Sourcearrow1: TMenuItem;
    argetarrow1: TMenuItem;
    None1: TMenuItem;
    Solidarrow1: TMenuItem;
    Linearrow1: TMenuItem;
    Ellipse1: TMenuItem;
    Rectangle1: TMenuItem;
    Diamond1: TMenuItem;
    None2: TMenuItem;
    Solidarrow2: TMenuItem;
    Linearrow2: TMenuItem;
    Ellipse2: TMenuItem;
    Rectangle2: TMenuItem;
    Diamond2: TMenuItem;
    DiagramBlockPicture11: TMenuItem;
    Shapecolor1: TMenuItem;
    Linecolor1: TMenuItem;
    Picture1: TMenuItem;
    Sourcearrow2: TMenuItem;
    argetarrow2: TMenuItem;
    None3: TMenuItem;
    Solidarrow3: TMenuItem;
    Linearrow3: TMenuItem;
    Ellipse3: TMenuItem;
    Rectangle3: TMenuItem;
    Diamond3: TMenuItem;
    None4: TMenuItem;
    Solidarrow4: TMenuItem;
    Linearrow4: TMenuItem;
    Ellipse4: TMenuItem;
    Rectangle4: TMenuItem;
    Diamond4: TMenuItem;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    DiagramBringToFront11: TMenuItem;
    DiagramSendToBack11: TMenuItem;
    Font1: TMenuItem;
    N9: TMenuItem;
    Font2: TMenuItem;
    Edittext2: TMenuItem;
    Grid1: TMenuItem;
    Gradient1: TMenuItem;
    N10: TMenuItem;
    Undo1: TMenuItem;
    DiagramRedoAction11: TMenuItem;
    N11: TMenuItem;
    Automaticnodes1: TMenuItem;
    ControlBar1: TControlBar;
    ToolBar1: TToolBar;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    ToolButton3: TToolButton;
    ToolButton4: TToolButton;
    ToolButton9: TToolButton;
    ToolButton5: TToolButton;
    ToolButton6: TToolButton;
    ToolButton7: TToolButton;
    ToolButton8: TToolButton;
    ToolButton19: TToolButton;
    ToolButton13: TToolButton;
    ToolButton14: TToolButton;
    ToolButton15: TToolButton;
    ToolButton16: TToolButton;
    DiagramToolBar2: TDiagramToolBar;
    ToolBar3: TToolBar;
    ToolButton17: TToolButton;
    ToolButton18: TToolButton;
    ToolButton20: TToolButton;
    ToolButton21: TToolButton;
    ToolButton22: TToolButton;
    ToolButton23: TToolButton;
    ToolButton24: TToolButton;
    ToolButton25: TToolButton;
    ToolButton26: TToolButton;
    ToolButton27: TToolButton;
    ToolButton28: TToolButton;
    ToolButton29: TToolButton;
    ToolButton30: TToolButton;
    ToolButton31: TToolButton;
    ToolButton32: TToolButton;
    ToolButton34: TToolButton;
    ToolButton35: TToolButton;
    ToolButton36: TToolButton;
    ToolButton37: TToolButton;
    ToolBar2: TToolBar;
    DgrFontSelector1: TDgrFontSelector;
    DgrFontSizeSelector1: TDgrFontSizeSelector;
    TabControl1: TTabControl;
    DiagramToolBar1: TDiagramToolBar;
    N12: TMenuItem;
    Group1: TMenuItem;
    Ungroup1: TMenuItem;
    ToolButton10: TToolButton;
    ToolButton12: TToolButton;
    ToolBar4: TToolBar;
    DgrColorSelector2: TDgrColorSelector;
    DgrGradientDirectionSelector2: TDgrGradientDirectionSelector;
    DgrBrushStyleSelector2: TDgrBrushStyleSelector;
    DgrShadowSelector2: TDgrShadowSelector;
    DgrPenStyleSelector2: TDgrPenStyleSelector;
    DgrPenColorSelector2: TDgrPenColorSelector;
    DgrTransparencySelector2: TDgrTransparencySelector;
    DgrZoomSelector2: TDgrZoomSelector;
    DgrPenWidthSelector2: TDgrPenWidthSelector;
    DgrTextColorSelector2: TDgrTextColorSelector;
    ToolButton11: TToolButton;
    ToolButton33: TToolButton;
    ToolButton38: TToolButton;
    ToolButton39: TToolButton;
    ToolButton40: TToolButton;
    ToolButton41: TToolButton;
    ToolButton42: TToolButton;
    ToolButton43: TToolButton;
    ToolButton44: TToolButton;
    ToolButton45: TToolButton;
    ToolButton46: TToolButton;
    ToolButton47: TToolButton;
    ToolButton48: TToolButton;
    PageSetup1: TMenuItem;
    N13: TMenuItem;
    AddtoLibrary1: TMenuItem;
    ToolButton49: TToolButton;
    DgrLayerSelector1: TDgrLayerSelector;
    N14: TMenuItem;
    Layermanager1: TMenuItem;
    Librarymanager1: TMenuItem;
    FLeftDockPanel: TPanel;
    FRightDockPanel: TPanel;
    FLeftDockSplitter: TSplitter;
    FRightDockSplitter: TSplitter;
    ActionList1: TActionList;
    acViewLayerManager: TAction;
    acViewLibraryManager: TAction;
    Saveas1: TMenuItem;
    acViewDiagramObjects: TAction;
    Diagramobjects1: TMenuItem;
    Background1: TMenuItem;
    Loadpicture1: TMenuItem;
    Clear1: TMenuItem;
    miLayer: TMenuItem;
    procedure ControlBar1DockOver(Sender: TObject; Source: TDragDockObject; X, Y: Integer; State: TDragState; var Accept: Boolean);
    procedure DockPanelUnDock(Sender: TObject; Client: TControl; NewTarget: TWinControl; var Allow: Boolean);
    procedure DockPanelDockDrop(Sender: TObject; Source: TDragDockObject; X, Y: Integer);
    procedure FormShow(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure acViewLayerManagerUpdate(Sender: TObject);
    procedure acViewLibraryManagerUpdate(Sender: TObject);
    procedure acViewLayerManagerExecute(Sender: TObject);
    procedure acViewLibraryManagerExecute(Sender: TObject);
    procedure DgrNotification(AInfo: TDgrNotificationInfo);
    procedure acViewDiagramObjectsUpdate(Sender: TObject);
    procedure acViewDiagramObjectsExecute(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure PopupMenu1Popup(Sender: TObject);
  private
    FDiagram: TatDiagram;
    FDesigner: TdmDgrEditorActions;
    FOnNotifyShow: TNotifyEvent;
    FTitle: string;
    FLayerManager: TDgrLayerManager;
    FLibManager: TDgrLibraryManager;
    FLibraryManagerEditor: TDgrLibraryManagerEditor;
    FShowLayerManager: boolean;
    FShowLibraryManager: boolean;
    {$IFDEF DELPHI9_LVL}
    FDockTabset: TDockTabset;
    FButtonsForm: TForm;
    FButtons: TDiagramButtons;
    procedure FDockTabsetUnDock(Sender: TObject; Client: TControl; NewTarget: TWinControl; var Allow: Boolean);
    procedure FDockTabsetDockDrop(Sender: TObject; Source: TDragDockObject; X, Y: Integer);
    {$ENDIF}
    procedure DiagramModified(Sender: TObject);
    procedure DockFormClose(Sender: TObject; var Action: TCloseAction);
    procedure DesignerFileNameChanged(Sender: TObject);
    function GetLayerManagerVisible: boolean;
    function GetLibraryManagerVisible: boolean;
    function GetOnSaveDiagram: TNotifyEvent;
    procedure SetLayerManagerVisible(const Value: boolean);
    procedure SetLibraryManagerVisible(const Value: boolean);
    procedure SetTitle(const Value: string);
    procedure SetOnSaveDiagram(const Value: TNotifyEvent);
    procedure SetLibManager(const Value: TDgrLibraryManager);
    procedure ShowDockedForm(AForm: TForm; AVisible: boolean; AFocus: boolean=True);
    procedure UpdateDockPanel(APanel: TPanel; AIgnore: TControl=nil);
    procedure SetShowLayerManager(const Value: boolean);
    function GetShowLibraryManager: boolean;
    procedure SetShowLibraryManager(const Value: boolean);
    procedure PrepareLayerMenu(AParent: TMenuItem);
    procedure LayerMenuItemClick(Sender: TObject);
    property LayerManagerVisible: boolean read GetLayerManagerVisible write SetLayerManagerVisible;
    property LibraryManagerVisible: boolean read GetLibraryManagerVisible write SetLibraryManagerVisible;
  public
    constructor CreateWithClass(AOwner: TComponent; ADiagramClass: TatDiagramClass);
    constructor CreateWithInstance(AOwner: TComponent; ADiagramInstance: TatDiagram);
    constructor Create(AOwner: TComponent); override;
    property OnNotifyShow: TNotifyEvent read FOnNotifyShow write FOnNotifyShow;
    property OnSaveDiagram: TNotifyEvent read GetOnSaveDiagram write SetOnSaveDiagram;
    property Title: string read FTitle write SetTitle;
    property Diagram: TatDiagram read FDiagram;
    property Actions: TdmDgrEditorActions read FDesigner;
    property LibManager: TDgrLibraryManager read FLibManager write SetLibManager;
    property ShowLayerManager: boolean read FShowLayerManager write SetShowLayerManager;
    property ShowLibraryManager: boolean read GetShowLibraryManager write SetShowLibraryManager;
    {$IFDEF DELPHI9_LVL}
    property DButtons: TDiagramButtons read FButtons;
    {$ENDIF}
  end;

implementation

{$R *.dfm}

procedure TfmDiagramEditor.DiagramModified(Sender: TObject);
begin
  if FDesigner.Modified then
    StatusBar1.Panels[0].Text := '�޸�'
  else
    StatusBar1.Panels[0].Text := '';
end;

type
  THackSelector = class(TDgrCustomSelector)
  end;

procedure TfmDiagramEditor.FormCreate(Sender: TObject);
var
  c: integer;
  {$IFDEF DELPHI2006_LVL}
  d: integer;
  {$ENDIF}
begin
  FDesigner := TdmDgrEditorActions.Create(Self);
  FDesigner.Diagram := FDiagram;
  FDesigner.OnFileNameChanged := DesignerFileNameChanged;
  FDesigner.PrepareDiagram;
  FDesigner.PrepareForm(Self);

  {$IFDEF DELPHI9_LVL}
  TabControl1.Visible := false;
  {$ELSE}
  FDesigner.PrepareCategoryTab(TabControl1, DiagramToolbar1);
  {$ENDIF}
  FDesigner.OnModified := DiagramModified;

  {$IFDEF DELPHI2006_LVL}
  ControlBar1.DrawingStyle := ExtCtrls.dsGradient;
  {$ENDIF}
  for c := 0 to ControlBar1.ControlCount - 1 do
    if ControlBar1.Controls[c] is TToolBar then
      with TToolbar(ControlBar1.Controls[c]) do
      begin
        EdgeInner := esNone;
        EdgeOuter := esNone;
        Transparent := true;
        {$IFDEF DELPHI2006_LVL}
        DrawingStyle := dsGradient;
        for d := 0 to ControlCount -1 do
          if Controls[c] is TDgrCustomSelector then
          begin
            THackSelector(Controls[d]).Color := ControlBar1.GradientStartColor;
            THackSelector(Controls[d]).ColorTo := ControlBar1.GradientEndColor;
          end;
        {$ENDIF}
      end;
end;

procedure TfmDiagramEditor.DockPanelDockDrop(Sender: TObject; Source: TDragDockObject; X, Y: Integer);
begin
  UpdateDockPanel(TPanel(Sender));
end;

procedure TfmDiagramEditor.DockPanelUnDock(Sender: TObject; Client: TControl; NewTarget: TWinControl; var Allow: Boolean);
begin
  UpdateDockPanel(TPanel(Sender), Client);
end;

{$IFDEF DELPHI9_LVL}
procedure TfmDiagramEditor.FDockTabsetDockDrop(Sender: TObject; Source: TDragDockObject; X, Y: Integer);
begin
  FDockTabset.Visible := true;
end;

procedure TfmDiagramEditor.FDockTabsetUnDock(Sender: TObject; Client: TControl; NewTarget: TWinControl; var Allow: Boolean);
begin
  FDockTabset.Visible := FDockTabset.Tabs.Count > 1;
end;
{$ENDIF}

procedure TfmDiagramEditor.ControlBar1DockOver(Sender: TObject;
  Source: TDragDockObject; X, Y: Integer; State: TDragState;
  var Accept: Boolean);
begin                                              
  Accept := not (Source.Control is TCustomForm);
end;

constructor TfmDiagramEditor.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  {Create the diagram component}
  if FDiagram = nil then
    FDiagram := TatDiagram.Create(Self);
  FDiagram.Parent := Self;
  FDiagram.Align := alClient;
  FDiagram.PopupMenu := PopupMenu1;

  ActiveControl := FDiagram;
  DgrZoomSelector2.Diagram := FDiagram;
  DiagramToolbar1.Diagram := FDiagram;
  DgrColorSelector2.Diagram := FDiagram;
  DgrGradientDirectionSelector2.Diagram := FDiagram;
  DgrBrushStyleSelector2.Diagram := FDiagram;
  DgrShadowSelector2.Diagram := FDiagram;
  DgrPenStyleSelector2.Diagram := FDiagram;
  DgrPenColorSelector2.Diagram := FDiagram;
  DgrPenWidthSelector2.Diagram := FDiagram;
  DgrTextColorSelector2.Diagram := FDiagram;
  DgrFontSelector1.Diagram := FDiagram;
  DgrFontSizeSelector1.Diagram := FDiagram;
  DgrTransparencySelector2.Diagram := FDiagram;
  DgrLayerSelector1.Diagram := FDiagram;
  DgrLayerSelector1.Enabled := false;
  DiagramToolbar2.Diagram := FDiagram;

  FShowLayerManager := True;
  FShowLibraryManager := True;
  LayerManagerVisible := FShowLayerManager;

  {$IFDEF DELPHI9_LVL}
  {Create the TDockTabSet to hold the diagrambuttons}
  FDockTabSet := TDockTabSet.Create(Self);
  FDockTabSet.Parent := Self;
  FDockTabSet.Align := alLeft;
  FDockTabSet.DestinationDockSite := FLeftDockPanel;
  FDockTabSet.ShrinkToFit := true;
  FDockTabSet.Style := tsModernTabs;
  FDockTabSet.TabPosition := tpLeft;
  FDockTabSet.Width := 25;
  FDockTabSet.OnDockDrop := FDockTabsetDockDrop;
  FDockTabSet.OnUnDock := FDockTabsetUnDock;

  {Create the form which contains the diagram buttons and will be docked}
  FButtonsForm := TForm.Create(Self);
  FButtonsForm.Caption := 'ͼ�ζ���';
  FButtonsForm.Width := 300;
  FButtonsForm.Height := 400;
  FButtonsForm.BorderStyle := bsSizeToolWin;
  FButtonsForm.DragKind := dkDock;
  FButtonsForm.DragMode := dmAutomatic;
  FButtonsForm.OnClose := DockFormClose;

  {Create the TDiagramButtons component inside the form}
  FButtons := TDiagramButtons.Create(FButtonsForm);
  FButtons.Parent := FButtonsForm;
  FButtons.Align := alClient;
  FButtons.Diagram := FDiagram;
  FButtons.ButtonFlow := cbfVertical;
  FButtons.ButtonOptions := [boGradientFill, boShowCaptions, boBoldCaptions, boUsePlusMinus, boCaptionOnlyBorder];
  FButtons.ShowHint := true;
  FButtons.ShowEmptyCategory := true;

  {Dock the form to the panel so it begins docked}
  FButtonsForm.ManualDock(FLeftDockPanel, nil, alClient);
  ShowDockedForm(FButtonsForm, True);

  {Hide the docktabset since the form is docked to the panel}
  FDockTabset.Visible := false;
  {$ELSE}
  UpdateDockPanel(FLeftDockPanel);
  {$ENDIF}
end;

procedure TfmDiagramEditor.FormShow(Sender: TObject);
{$IFDEF DELPHI9_LVL}
var
  c: integer;
{$ENDIF}
begin
  {Request realign so the control is alClient aligned}
  FLeftDockPanel.Width := FLeftDockPanel.Width + 1;
  FLeftDockPanel.Width := FLeftDockPanel.Width - 1;
  FRightDockPanel.Width := FRightDockPanel.Width + 1;
  FRightDockPanel.Width := FRightDockPanel.Width -1;

  {$IFDEF DELPHI9_LVL}
  {Fill the categories and buttons in the diagrambuttons control}
  FButtons.Populate;

  {Expand all}
  for c := 0 to FButtons.Categories.Count - 1 do
    FButtons.Categories[c].Collapsed := false;
  {$ELSE}
  FDiagram.AddSubscriber(Self);
  {$ENDIF}


  if Assigned(FOnNotifyShow) then
    FOnNotifyShow(Self);

  FDiagram.Invalidate;
end;

procedure TfmDiagramEditor.SetTitle(const Value: string);
begin
  if FTitle <> Value then
  begin
    FTitle := Value;
    Caption := FTitle;
  end;
end;

constructor TfmDiagramEditor.CreateWithClass(AOwner: TComponent; ADiagramClass: TatDiagramClass);
begin
  FDiagram := ADiagramClass.Create(Self);
  Create(AOwner);
end;

procedure TfmDiagramEditor.SetOnSaveDiagram(const Value: TNotifyEvent);
begin
  if FDesigner <> nil then
    FDesigner.OnSaveDiagram := Value;
end;

function TfmDiagramEditor.GetOnSaveDiagram: TNotifyEvent;
begin
  if FDesigner <> nil then
    result := FDesigner.OnSaveDiagram
  else
    result := nil;
end;

procedure TfmDiagramEditor.SetLibManager(const Value: TDgrLibraryManager);
begin
  FLibManager := Value;
  if FDesigner <> nil then
    FDesigner.LibManager := FLibManager;
  LibraryManagerVisible := ShowLibraryManager;
end;

procedure TfmDiagramEditor.UpdateDockPanel(APanel: TPanel; AIgnore: TControl);
var
  empty: boolean;
  i: integer;
  splitter: TSplitter;
begin
  empty := True;
  {If there is *one* control dock that is visible, and this control is not the ignore control, than the panel is not empty}
  for i:=0 to APanel.DockClientCount-1 do
    if APanel.DockClients[i].Visible and (APanel.DockClients[i] <> AIgnore) then
    begin
      empty := False;
      break;
    end;

  if APanel = FLeftDockPanel then
    splitter := FLeftDockSplitter
  else
    splitter := FRightDockSplitter;

  if empty then
  begin
    if APanel.Width > 0 then
    begin
      APanel.Tag := APanel.Width;
      APanel.Width := 0;
    end;
    splitter.Hide;
  end
  else
  begin
    if APanel.Tag > 0 then
    begin
      APanel.Width := APanel.Tag;
      APanel.Tag := 0;
    end;
    splitter.Show;
    splitter.Left := APanel.Width;
  end;
end;

procedure TfmDiagramEditor.DockFormClose(Sender: TObject; var Action: TCloseAction);
begin
  if (Sender is TForm) and (TForm(Sender).HostDockSite is TPanel) then
    UpdateDockPanel(TPanel(TForm(Sender).HostDockSite), TForm(Sender));
end;

procedure TfmDiagramEditor.ShowDockedForm(AForm: TForm; AVisible, AFocus: boolean);
var
  panel: TPanel;
begin
  AForm.Visible := AVisible;
  if (AForm.HostDockSite <> nil) and (AForm.HostDockSite is TPanel) then
    panel := TPanel(AForm.HostDockSite)
  else
    panel := nil;

  if panel <> nil then // form is docked
  begin
    if AVisible then
    begin
      if Self.Visible and panel.CanFocus and AFocus then
        panel.SetFocus;
      if Self.Visible and AForm.CanFocus and AFocus then
        AForm.SetFocus;
    end;
    UpdateDockPanel(panel);
  end
  else if Self.Visible and AForm.CanFocus and AFocus then
    AForm.SetFocus;
end;

procedure TfmDiagramEditor.acViewLayerManagerUpdate(Sender: TObject);
begin
  acViewLayerManager.Visible := ShowLayerManager;
  acViewLayerManager.Checked := LayerManagerVisible;
end;

procedure TfmDiagramEditor.acViewLibraryManagerUpdate(Sender: TObject);
begin
  acViewLibraryManager.Visible := ShowLibraryManager;
  acViewLibraryManager.Checked := LibraryManagerVisible;
end;

procedure TfmDiagramEditor.acViewDiagramObjectsExecute(Sender: TObject);
begin
  {$IFDEF DELPHI9_LVL}
  if FButtonsForm <> nil then
  begin
    FButtonsForm.Visible := not FButtonsForm.Visible;
    ShowDockedForm(FButtonsForm, FButtonsForm.Visible, FButtonsForm.Visible);
  end;
  {$ENDIF}
end;

procedure TfmDiagramEditor.acViewDiagramObjectsUpdate(Sender: TObject);
begin
  {$IFDEF DELPHI9_LVL}
  acViewDiagramObjects.Visible := FButtonsForm <> nil;
  if acViewDiagramObjects.Visible then
    acViewDiagramObjects.Checked := FButtonsForm.Visible;
  {$ELSE}
  acViewDiagramObjects.Visible := False;
  {$ENDIF}
end;

procedure TfmDiagramEditor.acViewLayerManagerExecute(Sender: TObject);
begin
  LayerManagerVisible := not LayerManagerVisible;
end;

procedure TfmDiagramEditor.acViewLibraryManagerExecute(Sender: TObject);
begin
  LibraryManagerVisible := not LibraryManagerVisible;
end;

procedure TfmDiagramEditor.SetShowLayerManager(const Value: boolean);
begin
  FShowLayerManager := Value;
  if not FShowLayerManager then
    LayerManagerVisible := False;
end;

function TfmDiagramEditor.GetShowLibraryManager: boolean;
begin
  result := FShowLibraryManager and (FLibManager <> nil);
end;

procedure TfmDiagramEditor.LayerMenuItemClick(Sender: TObject);
var
  layer: string;
  i: integer;
begin
  layer := TMenuItem(Sender).Caption;
  if Diagram.SelectedCount > 0 then
  begin
    for i := Diagram.SelectedCount - 1 downto 0 do
      Diagram.Selecteds[i].LayerName := layer;
    Diagram.Modified;
  end;
end;

procedure TfmDiagramEditor.PopupMenu1Popup(Sender: TObject);
begin
  miLayer.Enabled := (Diagram <> nil) and (Diagram.SelectedCount > 0);
  if miLayer.Enabled then
    PrepareLayerMenu(miLayer);
end;

procedure TfmDiagramEditor.PrepareLayerMenu(AParent: TMenuItem);
var
  i: integer;
  mi: TMenuItem;
begin
  AParent.Clear;
  mi := TMenuItem.Create(AParent);
  mi.Caption := '(no layer)';
  mi.OnClick := LayerMenuItemClick;
  AParent.Add(mi);
  for i := 0 to Diagram.Layers.Count - 1 do
  begin
    mi := TMenuItem.Create(AParent);
    mi.Caption := Diagram.Layers[i].LayerName;
    mi.OnClick := LayerMenuItemClick;
    AParent.Add(mi);
  end;
end;

procedure TfmDiagramEditor.SetShowLibraryManager(const Value: boolean);
begin
  FShowLibraryManager := False;
  if not FShowLibraryManager then
    LibraryManagerVisible := False;
end;

function TfmDiagramEditor.GetLayerManagerVisible: boolean;
begin
  result := (FLayerManager <> nil) and FLayerManager.Visible;
end;

procedure TfmDiagramEditor.SetLayerManagerVisible(const Value: boolean);
begin
  if Value and (FLayerManager = nil) then
  begin
    FLayerManager := TDgrLayerManager.Create(Self);
    FLayerManager.Diagram := Self.Diagram;
    FLayerManager.OnClose := DockFormClose;
    FLayerManager.ManualDock(FRightDockPanel, nil, alTop);
    FLayerManager.Execute;
  end;
  ShowDockedForm(FLayerManager, Value, Value);
  FLayerManager.ManualDock(FRightDockPanel, nil, alTop);
end;

function TfmDiagramEditor.GetLibraryManagerVisible: boolean;
begin
  result := (FLibraryManagerEditor <> nil) and FLibraryManagerEditor.Visible;
end;

procedure TfmDiagramEditor.SetLibraryManagerVisible(const Value: boolean);
begin
  if Value and (FLibraryManagerEditor = nil) then
  begin
    FLibraryManagerEditor := TDgrLibraryManagerEditor.Create(Self);
    FLibraryManagerEditor.LibraryManager := FLibManager;
    FLibraryManagerEditor.Diagram := FDiagram;
    FLibraryManagerEditor.OnClose := DockFormClose;
    FLibraryManagerEditor.ManualDock(FRightDockPanel, nil, alBottom);
    FLibraryManagerEditor.Execute;
  end;
  if FLibraryManagerEditor <> nil then
  begin
    ShowDockedForm(FLibraryManagerEditor, Value, Value);
    FLibraryManagerEditor.ManualDock(FRightDockPanel, nil, alBottom);
  end;
end;

procedure TfmDiagramEditor.DgrNotification(AInfo: TDgrNotificationInfo);
begin
  FDesigner.PrepareCategoryTab(TabControl1, DiagramToolbar1);
end;

procedure TfmDiagramEditor.DesignerFileNameChanged(Sender: TObject);
begin
  if FDesigner.CurrentDiagramFileName > '' then
    Caption := Format('%s - %s', [FTitle, ExtractFileName(FDesigner.CurrentDiagramFileName)])
  else
    Caption := FTitle;
end;

constructor TfmDiagramEditor.CreateWithInstance(AOwner: TComponent; ADiagramInstance: TatDiagram);
begin
  FDiagram := ADiagramInstance;
  Create(AOwner);
end;

procedure TfmDiagramEditor.FormResize(Sender: TObject);
begin
  with DiagramToolBar2 do
    Width := ControlCount * ButtonSize;
end;

end.

