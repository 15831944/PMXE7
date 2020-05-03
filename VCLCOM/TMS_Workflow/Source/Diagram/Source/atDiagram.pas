{***************************************************************************}
{ Diagram Studio                                                            }
{ for Delphi & C++Builder                                                   }
{ version 4.0.4                                                             }
{                                                                           }
{ written by TMS Software                                                   }
{            copyright ?2003-2011                                          }
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

unit atDiagram;                                                                  

{$I diagram.inc}

{Three options for streaming:
1. Ok at run-time, but does not work at design-time (unless a specific design-time
   editor is done. Using Delphi IDE object inspector no longer work)
   To do this: make GetChildOwner return Self
2. Ok at design-time and runtime, but clipboard paste and load operation only
   works when at the time of clipboard copy/cut or save operation, the form name
   was the same of the form that now own the diagram. This is because
   "FormName." prefix is added to every link anchor in clipboard and saved file.
   To do this: current behaviour
3. Ok at design-time and runtime. The only counterpart is that while saving
   diagram or copying to clipboard, the form that owns the diagram has its name
   changed to a temporary name. After copy to clipboard or save diagram operation
   is done, form name goes back to normal.
   This will add a default prefix (like "DiagramForm.") to clipboard or saved file.
   To do this: on GetChildren procedure, change owner name to a temporary name,
   then change it back to old name after operation is done
}

interface

uses
  Windows, Messages, SysUtils, Classes, Forms, Controls, Contnrs, DgrClasses,
  {$IFDEF DELPHI2010_LVL}
  Direct2dClasses,
  {$ENDIF}
  {$IFDEF GDIPLUS}
  DgrGdipApi, DgrGdipObj, GdipClasses,
  {$ENDIF}
  Graphics, clipbrd, StdCtrls,Jpeg,
  DiagramUtils, CommCtrl;

const

/// Description:
///   For internal use only.
  WM_TRANSMEMOINVALIDATE = WM_USER + 500;
  {$IFNDEF ISDELPHI}
/// Description:
///   Constant which defines the maximum number of diagram controls that can be moved
///   at the same time (only for C++Builder). For internal use only.
  MaxControlMoveInfos = 1000;
  {$ENDIF}

type
  number = double;
  TStretchHandle = class;
  TLinkPoint = class;
  TCustomDiagramBlock = class;
  TDiagramControl = class;

  THandleMoveAction = (haEraseCursor, haDrawCursor, haMoved, haPositionCursor);

  /// Summary:
  ///   Contains information about the handle being dragged/moved.
  /// Description:
  ///   THandleMoveInfo holds information about the dragging/moving of a handle.
  THandleMoveInfo = record

    /// Description:
    ///   Contains the canvas object of diagram.
    Canvas: TCanvas;

    /// Description:
    ///   Points to the TStretchHandle object associated with the handle being dragged/moved.
    AHandle: TStretchHandle;

    /// Description:
    ///   Indicates what action is being done to the handle.
    Action: THandleMoveAction;

    /// Description:
    ///   If a handle is being dragged over a link point of a control, AnchorPoint contains the TLinkPoint
    ///   object associated with the link point. Otherwise, AnchorPoint is nil.
    AnchorPoint: TLinkPoint;

    /// Description:
    ///   Provides the diagram control (if any) which the mouse is over.
    DControl: TDiagramControl; // added to see if we're hovering over a control, for autocreating linkpoint

    /// Description:
    ///   Delta contain the relative position of dragged handle related to its original position before
    ///   dragging (in diagram coordinates).
    Delta: TDot; {In diagram coordinates, not rotated}
  end;

  TExportFormat = (efAuto, efBMP, efWMF);
  TExportSize = (esStandard, esCentered, esNetto);

  EDiagramException = class(Exception);

  /// Description:
  ///   For internal use only.
  EDControlNotFound = class(EDiagramException);
  EDiagramPrintException = class(EDiagramException);

  TatDiagram = class;

  TCustomDiagramLine = class;
  TBaseGroupBlock = class;
  TGroupBlock = class;
  TGroupBlockClass = class of TGroupBlock;
  TDiagramMemo = class;
  TDiagramMemoClass = class of TDiagramMemo;

  /// Description:
  ///   TRectPtr is a pointer to a TRect object. Used internally.
  TRectPtr = ^TRect;
  TSelectDirection = (sdUp, sdLeft, sdDown, sdRight);
  TDiagramControlFilter = (cfAll, cfIgnoreMembers, cfIgnoreGroups, cfIgnoreAsMembers);

  /// Description:
  ///   TDiagramUndoItem holds a given state of diagram for undo operations. Used internaly by diagram component.
  TDiagramUndoItem = class(TCollectionItem)
  private
    FActionName: string;
    FState: TStream;
    FObj: TObject;
  public
    /// Description:
    ///   Creates the object.
    constructor Create(Collection: TCollection); override;

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   ActionName property holds the action in diagram (undo, insert block, moving, etc.) which lead to the
    ///   current diagram state holded by the undo item.
    property ActionName: string read FActionName write FActionName;
    property Obj: TObject read FObj write FObj;

    /// Description:
    ///   State property holds a given state of the diagram for undo operation purposes. For internal use only.
    property State: TStream read FState;
  end;

  /// Description:
  ///   TDiagramUndoStack holds the previous states of diagram for undo operations. For internal use only.
  TDiagramUndoStack = class(TCollection)
  private
    FCurrent: integer;
    FDiagram: TatDiagram;
    procedure ClearUndoStack;
    procedure PushState(const AActionName: string);
    procedure Undo;
    procedure Redo;
    function NextUndoAction: string;
    function NextRedoAction: string;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADiagram: TatDiagram);
  end;

  /// Summary:
  ///   Ancestor class for some TPersistent objects used by diagram.
  /// Description:
  ///   TDiagramPersistent is used internally, as an ancestor for some properties of diagram that are TPersistent descendants.
  TDiagramPersistent = class(TPersistent)
  private
    FDControl: TDiagramControl;
  protected

    /// Description:
    ///   Called whenever a property changes.
    procedure Changed; virtual;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADControl: TDiagramControl);
  end;

  THandleAppearanceStyle = (
    hhRectangle, 
    hhEllipse,
    hhDiamond,
    hhCrossedEllipse,
    hhRectangleX,
    hhEllipseX
  );

  THandleAppearance = class(TPersistent)
  private
    FPen: TPen;
    FDisabledBrush: TBrush;
    FBrush: TBrush;
    FSize: integer;
    FStyle: THandleAppearanceStyle;
    FDisabledPen: TPen;
    FVisible: boolean;
    procedure SetBrush(const Value: TBrush);
    procedure SetDisabledBrush(const Value: TBrush);
    procedure SetDisabledPen(const Value: TPen);
    procedure SetPen(const Value: TPen);
    procedure SetSize(const Value: integer);
    procedure SetStyle(const Value: THandleAppearanceStyle);
    procedure SetVisible(const Value: boolean);
  public
    constructor Create;
    destructor Destroy; override;

    /// Description:
    ///   Assign method copies the content of the Source diagram to the current diagram.
    ///   All objects and properties are copied.
    procedure Assign(Source: TPersistent); override;
  published
    property Pen: TPen read FPen write SetPen;
    property Brush: TBrush read FBrush write SetBrush;
    property DisabledPen: TPen read FDisabledPen write SetDisabledPen;
    property DisabledBrush: TBrush read FDisabledBrush write SetDisabledBrush;
    property Size: integer read FSize write SetSize;
    property Style: THandleAppearanceStyle read FStyle write SetStyle;
    property Visible: boolean read FVisible write SetVisible default true;
  end;
  
  THandlesAppearance = class(TPersistent)
  private
    FCustom: THandleAppearance;
    FRotate: THandleAppearance;
    FRotCenter: THandleAppearance;
    FSize: THandleAppearance;
    procedure SetCustom(const Value: THandleAppearance);
    procedure SetRotate(const Value: THandleAppearance);
    procedure SetRotCenter(const Value: THandleAppearance);
    procedure SetSize(const Value: THandleAppearance);
  public
    constructor Create;
    destructor Destroy; override;
    procedure Assign(Source: TPersistent); override;
  published
    property Size: THandleAppearance read FSize write SetSize;
    property Rotate: THandleAppearance read FRotate write SetRotate;
    property Custom: THandleAppearance read FCustom write SetCustom;
    property RotCenter: THandleAppearance read FRotCenter write SetRotCenter;
  end;

  THandleStyle = (hsSize, hsRotate, hsRotCenter, hsCustom);

  /// Summary:
  ///   TStretchHandle contains information about a diagram control handle.
  /// Description:
  ///   TStretchHandle contains information about a handle of a diagram control.
  TStretchHandle = class(TCollectionItem)
  private
    FOrX: double;
    FOrY: double;
    FCursor: TCursor;
    FData: integer;
    FStyle: THandleStyle;
    FVisible: boolean;
    FAlwaysMovable: boolean;
    function GetControl: TDiagramControl;
    function GetOrPoint: TDot;
    procedure SetOrPoint(const Value: TDot);
    procedure SetOrX(const Value: double);
    procedure SetOrY(const Value: double);
    procedure SetStyle(const Value: THandleStyle);
    procedure SetVisible(const Value: boolean);
    function GetVisible: boolean;
  protected

    /// Description:
    ///   DControl contains the TDiagramControl object that holds the handle.
    property DControl: TDiagramControl read GetControl;

    /// Description:
    ///   Returns true if the handle can be moved.
    function Movable: boolean;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(Collection: TCollection); override;

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Copies a handle object into the current handle.
    procedure Assign(Source: TPersistent); override;
    {Diagram point returns handle point in diagram coords}

    /// Description:
    ///   Returns the diagram coordinates of handle. Internally, it calls TDiagramControl.GetHandlePoint method,
    ///   passing OrX and OrY values.
    function DiagramPoint: TDot;

    /// Description:
    ///   Set AlwaysMovable to true if you want the handle to be moved even when the diagram control has
    ///   resizing restrictions. Default is false.
    property AlwaysMovable: boolean read FAlwaysMovable write FAlwaysMovable;

    /// Description:
    ///   Specifies the mouse cursor to be displayed when user moves mouve over the handle.
    property Cursor: TCursor read FCursor write FCursor;

    /// Description:
    ///   For internal use only.
    property Data: integer read FData write FData;

    /// Description:
    ///   OrPoint property retrieves the values of OrX and OrY properties in a TDot record structure.
    property OrPoint: TDot read GetOrPoint write SetOrPoint;

    /// Description:
    ///   Use Visible property to specify if the TStretchHandle will be visible in the diagram or not.
    property Visible: boolean read GetVisible write SetVisible default true;
  published
    {OrX and OrY properties contain original (reference) coordinates for point. Can
     be used for retrieving point in diagram coords}

    /// Description:
    ///   OrX contains horizontal coordinate of handle. The meaning of this value (if it is screen coordinate,
    ///   block coordinate, etc.) is up to the diagram control. The diagram control converts this value to
    ///   diagram coordinates in method TDiagramControl.GetHandlePoint.
    property OrX: double read FOrX write SetOrX;

    /// Description:
    ///   OrY contains vertical coordinate of handle. The meaning of this value (if it is screen coordinate,
    ///   block coordinate, etc.) is up to the diagram control. The diagram control converts this value to
    ///   diagram coordinates in method TDiagramControl.GetHandlePoint.
    property OrY: double read FOrY write SetOrY;

    /// Description:
    ///   Defines the style of handle (what is it for).
    property Style: THandleStyle read FStyle write SetStyle;
  end;

  /// Summary:
  ///   Contains the handles of a diagram control.
  /// Description:
  ///   TStretchHandles holds a collection of handles belonging to a TDiagramControl. Handles are those
  ///   square/round blocks used for resizing/moving controls. There is only need to create/remove handles if
  ///   you are creating descendants of controls.
  TStretchHandles = class(TOwnedCollection)
  private
    FDControl: TDiagramControl;
    function GetItem(Index: integer): TStretchHandle;
  public

    /// Description:
    ///   Create the handle collection. Do not call it directly, handles collection is already created in block.
    constructor Create(ADControl: TDiagramControl);

    /// Description:
    ///   For internal use.
    procedure Update(Item: TCollectionItem); override;

    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Add a new handle in handle list.
    function Add: TStretchHandle; overload;
    function Add(AOrX, AOrY: double; ACursor: TCursor = crSizeNWSE;
      AData: integer = 0; AStyle: THandleStyle = hsSize): TStretchHandle; overload;

    /// Description:
    ///   Call FindByData to find a handle by the value of its Data property.
    function FindByData(AData: integer): TStretchHandle;

    /// Description:
    ///   Returns the handle which performs the rotation of the block.
    function RotateHandle: TStretchHandle;

    /// Description:
    ///   Returns the handle which represents the rotation center of the block.
    function RotCenterHandle: TStretchHandle;

    /// Description:
    ///   Use Items property to access individual TStretchHandle objects.
    property Items[Index: integer]: TStretchHandle read GetItem; default;
  end;

  TLinkPointStyle = (ptNone, ptEllipse, ptRectangle, ptDiamond, ptCross, ptImplicit);

  /// Keywords:
  ///   acPosition; acRemoval
  TAnchorChange = (acPosition, acRemoval);

  /// Keywords:
  ///   aoDown; aoHorz; aoLeft; aoNone; aoRight; aoUp; aoVert
  TAnchorOrientation = (aoNone, aoHorz, aoVert, aoUp, aoDown, aoLeft, aoRight);
  TLinkConstraint = (lcAll, lcSourceOnly, lcTargetOnly, lcNone);

  /// Summary:
  ///   TLinkPoint contains information about a diagram control link point.
  /// Description:
  ///   TLinkPoint contains information about a link point of a diagram control.
  TLinkPoint = class(TCollectionItem)
  private
    FOrX: double;
    FOrY: double;
    FAnchorIndex: integer;
    FAnchor: TDiagramControl;
    FOrientation: TAnchorOrientation;
    FAnchoreds: TList;
    FCollapsable: boolean;
    FCollapsed: boolean;
    FObj: TObject;
    FLinkConstraint: TLinkConstraint;
    FVisible: boolean;
    FCanAnchor: boolean;
    FAcceptAnchoreds: boolean;
    FCanAnchorInMove: boolean;
    function GetDControl: TDiagramControl;
    procedure SetAnchor(const Value: TDiagramControl);
    function GetAnchorLink: TLinkPoint;
    procedure SetAnchorLink(const Value: TLinkPoint);
    procedure SetAnchorIndex(const Value: integer);
    function GetStyle: TLinkPointStyle;
    function GetRealOrientation: TAnchorOrientation;
    function GetAnchoredItem(Index: integer): TLinkPoint;
    function GetOrPoint: TDot;
    procedure SetOrPoint(const Value: TDot);
    function IsNode: boolean;
    procedure SetCollapsed(const Value: boolean);
    function DoCollapseControls(ACollapse: boolean): boolean;
    procedure SetVisible(const Value: boolean);
  public
    /// Description:
    ///   Creates the object.
    constructor Create(Collection: TCollection); override;

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   AnchorChanged is called whenever the control specified by Anchor property is moved or removed.
    procedure AnchorChanged(AChangeType: TAnchorChange);

    /// Description:
    ///   Copies information from another TLinkPoint object to the TLinkPoint.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Returns the diagram coordinates of link point. Internally, it calls TDiagramControl.GetLinkPoint
    ///   method, passing OrX and OrY values.
    function DiagramPoint: TDot;

    /// Description:
    ///   Retrieves the number of other TLinkPoint objects which are anchored to the current link point.
    function AnchoredCount: integer;

    /// Description:
    ///   Call Expand method to expand (show) all blocks linked to the current link point, like a tree view
    ///   node expand operation. This method only has effect if the Collapsable property is set to true.
    procedure Expand;

    /// Description:
    ///   Call Collapse method to collapse (hide) all blocks linked to the current link point, like a tree
    ///   view node collapse operation. This method only has effect if the Collapsable property is set to true.
    procedure Collapse;

    /// Description:
    ///   Retrieves the TDiagramControl which the link point belongs to.
    property DControl: TDiagramControl read GetDControl;

    /// Description:
    ///   Retrieves all the TLinkPoint objects which are anchored to the current link point. The number of
    ///   anchored link points is given by AnchoredCount property.
    property Anchoreds[Index: integer]: TLinkPoint read GetAnchoredItem;

    /// Description:
    ///   RealOrientation contains the orientation of link point (given by Orientation property), but
    ///   considering the rotation of control. For example, if Orientation is aoLeft, but the control is
    ///   rotated by 180 degrees, then RealOrientation would be aoRight.
    property RealOrientation: TAnchorOrientation read GetRealOrientation;

    /// Description:
    ///   AnchorLink points to a TLinkPoint object of another TDiagramControl where the current TLinkPoint
    ///   is anchored to.
    property AnchorLink: TLinkPoint read GetAnchorLink write SetAnchorLink;

    /// Description:
    ///   Specifies the visual style of link point (rectangle, cross, etc.). This property is read-only.
    property Style: TLinkPointStyle read GetStyle;

    /// Description:
    ///   Retrieve the OrX and OrY properties in a TDot record structure.
    property OrPoint: TDot read GetOrPoint write SetOrPoint;

    /// Description:
    ///   A general purpose property to associate a link point any existing object.
    property Obj: TObject read FObj write FObj;

    /// Description:
    ///   When CanAnchor is true, the link point can be anchored to another link point.
    property CanAnchor: boolean read FCanAnchor write FCanAnchor;

    /// Description:
    ///   When CanAnchorInMove is true, the link point can be anchored to another link point in a move
    ///   operation (when the block holding the link point is being moved).
    property CanAnchorInMove: boolean read FCanAnchorInMove write FCanAnchorInMove;

    /// Description:
    ///   Use AcceptAnchoreds property to specify if a linkpoint can have other link points anchored to it.
    property AcceptAnchoreds: boolean read FAcceptAnchoreds write FAcceptAnchoreds;
  published

    /// Description:
    ///   Contains the diagram control where the link point is anchored to.
    property Anchor: TDiagramControl read FAnchor write SetAnchor;

    /// Description:
    ///   Contains the index of TLinkPoint object where the current TLinkPoint is anchored to. For example,
    ///   if AnchorIndex is 3, it means that current TLinkPoint is linked to another TLinkPoint which index is 3 in the linkpoint collection.
    property AnchorIndex: integer read FAnchorIndex write SetAnchorIndex default -1;

    /// Description:
    ///   OrX contains horizontal coordinate of link point. The meaning of this value (if it is screen
    ///   coordinate, block coordinate, etc.) is up to the diagram control. The diagram control converts this
    ///   value to diagram coordinates in method TDiagramControl.GetLinkPoint. For regular blocks, OrX contains
    ///   the % position related to block width.
    property OrX: double read FOrX write FOrX;

    /// Description:
    ///   OrY contains vertical coordinate of link point. The meaning of this value (if it is screen
    ///   coordinate, block coordinate, etc.) is up to the diagram control. The diagram control converts this
    ///   value to diagram coordinates in method TDiagramControl.GetLinkPoint. For regular blocks, OrY contains
    ///   the % position related to block height.
    property OrY: double read FOrY write FOrY;

    /// Description:
    ///   Specifies the orientation of link point. This orientation is used by the lines attached to the link
    ///   point (specially TDiagramSideLine), to know where the default direction the line should follow from
    ///   the link point. For example, linkpoints positioned at the left of control should have orientation
    ///   aoLeft, so when lines are attached to that link point, the lines would never go right.
    property Orientation: TAnchorOrientation read FOrientation write FOrientation default aoNone;

    /// Description:
    ///   Defines if the link point can be collapsed/expanded or not. If Collapsable property is true,
    ///   the end-user can click the link point to show/hide the blocks linked to it, like a tree structure.
    property Collapsable: boolean read FCollapsable write FCollapsable;

    /// Description:
    ///   Read Collapsed property to know if a link point is in the collapsed state (if blocks linked to the
    ///   link point are hidden or not). You can also write to Collapsed property to expand/collapse the link
    ///   point.
    property Collapsed: boolean read FCollapsed write SetCollapsed;

    /// Description:
    ///   Use LinkConstraint to set which extremity of a line can be attached to the linkpoint. It's useful
    ///   for linkpoints which receives only input or output information, so you can restrict if a start/end
    ///   of a line can be attached to the linkpoint.
    property LinkConstraint: TLinkConstraint read FLinkConstraint write FLinkConstraint;

    /// Description:
    ///   Use Visible property to make the link point visible/invisible. When the link point is invisible,
    ///   it will not receive anchors or be anchored to another link point. If you want to keep these
    ///   capabilities but just avoid the link point to be displayed in the screen, use AvoidDrawing property
    ///   instead.
    property Visible: boolean read FVisible write SetVisible;
  end;

  /// Summary:
  ///   Contains the link points of a diagram control.
  /// Description:
  ///   TLinkPoints holds a collection of link points belonging to a TDiagramControl. Link points are points in
  ///   the control where lines can be attached to, or controls can be anchored to. A link point can be anchored
  ///   to another link point (specified by AnchorLink, or Anchor and AnchorIndex properties).
  TLinkPoints = class(TOwnedCollection)
  private
    FDControl: TDiagramControl;
    function GetItem(Index: integer): TLinkPoint;
  protected

    /// Description:
    ///   For internal use.
    procedure Update(Item: TCollectionItem); override;
  public

    /// Description:
    ///   Adds a new TLinkPoint object in the collection.
    function Add: TLinkPoint; overload;
    function Add(AOrX, AOrY: double; AOr: TAnchorOrientation = aoNone): TLinkPoint; overload;

    /// Description:
    ///   Use Items to access individual TLinkPoint objects.
    property Items[Index: integer]: TLinkPoint read GetItem; default;
  end;

  TControlRestriction = (crNoMove, crNoResize, crNoRotation, crNoEdit,
    crNoDelete, crKeepRatio, crNoClipboard, crNoSelect, crNoRotCenterMove,
    crNoLink);

  /// Description:
  ///   TControlRestrictions is a set of TControlRestriction options that specified what actions user
  ///   cannot perform in diagram control.
  TControlRestrictions = set of TControlRestriction;

  TControlMoveAction = (caEraseCursor, caDrawCursor, caMoved, caPositionCursor);

  /// Summary:
  ///   Contains information about the control being moved.
  /// Description:
  ///   TControlMoveInfo holds information about the moving of a diagram control.
  TControlMoveInfo = record

    /// Description:
    ///   Contains the canvas object of diagram.
    Canvas: TCanvas;

    /// Description:
    ///   Not used in current version.
    AHandle: TStretchHandle;

    /// Description:
    ///   Indicates what action step of movement is being done.
    Action: TControlMoveAction;

    /// Description:
    ///   If during moving of control(s) one of its link points is being dragged over a link point
    ///   of another  control (that is not being moved), AnchorPoint contains the TLinkPoint object
    ///   associated with the link point. Otherwise, AnchorPoint is nil.
    AnchorPoint: TLinkPoint;

    /// Description:
    ///   If during moving of control(s) one of its link points is being dragged over a link point of another
    ///   control (that is not being moved), LinkPoint contains the TLinkPoint object that is being dragger
    ///   over the target link point. Otherwise, LinkPoint is nil.
    LinkPoint: TLinkPoint;

    /// Description:
    ///   Delta contain the relative position of moved control related to its original position before
    ///   the movement. (in diagram coordinates).
    Delta: TDot;
  end;

  /// Description:
  ///   TDiagramDrawInfo contains information used for drawing controls
  TDiagramDrawInfo = record

    /// Description:
    ///   Canvas Object of diagram.
    Canvas: TCanvas;

    /// Description:
    ///   Instance for a TBlockDrawerClass to help in the drawing of controls.
    Drawer: TBlockDrawer;

    /// Description:
    ///   Instance for a TDgrBlockDrawer descendant to help in the drawing of controls using specific
    ///   GraphicLib functions (GDI+/TGPBlockDrawer and Direct2D/TD2DBlockDrawer).
    DgrDrawer: TDgrBlockDrawer;
  end;

  /// Description:
  ///   TDiagramDrawBlockInfo contains information used for drawing blocks (along with TDiagramDrawInfo)
  TDiagramDrawBlockInfo = record
    Rect: TSquare;
    Angle: double;
  end;

  /// Description:
  ///   TDiagramDrawLineInfo contains information used for drawing lines (along with TDiagramDrawInfo)
  TDiagramDrawLineInfo = record
    Handles: TStretchHandles;
    DrawArrows: boolean;
  end;

  /// Description:
  ///    TDiagramDrawArrowInfo contains information used for drawing line arrows (along with TDiagramDrawInfo)
  TDiagramDrawArrowInfo = record
    FromPoint: TDot;
    ToPoint: TDot;
    ZoomRatio: double;
  end;

  {$IFDEF ISDELPHI}

  /// Description:
  ///   Used for internal purposed only.
  TControlMoveInfoArray = array of TControlMoveInfo;
  {$ELSE}
  TControlMoveInfoArray = array[0..1000] of TControlMoveInfo;
  {$ENDIF}

  TControlInsertAction = (iaEraseCursor, iaDrawCursor, iaInserted);

  /// Summary:
  ///   Contains information about the control being inserted.
  /// Description:
  ///   TControlInsertInfo holds information about the inserting of a diagram control.
  TControlInsertInfo = record

    /// Description:
    ///   Diagram field contains the diagram component where the control is being inserted to.
    Diagram: TatDiagram;

    /// Description:
    ///   Contains the canvas object of diagram.
    Canvas: TCanvas;

    /// Description:
    ///   Indicates what action step of inserting is being done.
    Action: TControlInsertAction;

    /// Description:
    ///   If during inserting of the control the first mouse click was done around a link point of
    ///   another control, AnchorPoint1 contains the TLinkPoint object associated with the link point.
    ///   Otherwise, AnchorPoint1 is nil.
    AnchorPoint1: TLinkPoint;

    /// Description:
    ///   If during inserting of the control the mouse button release was done around a link point
    ///   of another control, AnchorPoint2 contains the TLinkPoint object associated with the link point.
    ///   Otherwise, AnchorPoint2 is nil.
    AnchorPoint2: TLinkPoint;

    /// Description:
    ///   Contains the DControl (if any) on which the mouse is over.
    DControl: TDiagramControl; // added to see if we're hovering over a control

    /// Description:
    ///   Contains the point (in diagram coordinates) where the mouse was first clicked for inserting the control.
    P1: TDot;

    /// Description:
    ///   Contains the point (in diagram coordinates) where the mouse button was released for inserting the control.
    P2: TDot;

    /// Description:
    ///   Clicked is true if user inserted the control just by clicking the diagram (in this case,
    ///   P1 field coordinates is equal to P2 field coordinates). Clicked is false if user inserted
    ///   the control by pressing mouse button, dragging, and then releasing mouse button.
    Clicked: boolean;
  end;

  TControlDrawAction = (daEraseCursor, daDrawCursor, daStart, daStep, daInserted);

  /// Description:
  ///   TControlDrawInfo holds information about the drawing of a diagram control
  ///   (polygon block, polygon lines, etc.).
  TControlDrawInfo = record

    /// Description:
    ///   Diagram field contains the diagram component where the control is being drawing to.
    Diagram: TatDiagram;

    /// Description:
    ///   Contains the canvas object of diagram.
    Canvas: TCanvas;

    /// Description:
    ///   Indicates what action step of drawing is being done.
    Action: TControlDrawAction;

    /// Description:
    ///   AnchorPoint1 contains the TLinkPoint object where the start point of line is anchored to.
    ///   If there is no link point, AnchorPoint1 is nil.
    AnchorPoint1: TLinkPoint;

    /// Description:
    ///   AnchorPoint2 contains the TLinkPoint object where the end point of line is anchored to.
    ///   If there is no link point, AnchorPoint2 is nil.
    AnchorPoint2: TLinkPoint;

    /// Description:
    ///   Contains the DControl (if any) on which the mouse is over.
    DControl: TDiagramControl; // added to see if we're hovering over a control

    /// Description:
    ///   Contains the previous point (in diagram coordinates) where the mouse was previous clicked for drawing the control.
    P1: TDot;

    /// Description:
    ///   Contains the point (in diagram coordinates) where the mouse button was clicked for drawing the control.
    ///   Each step of drawing is given by P1 and P2 fields.
    P2: TDot;
    
    //Clicked: boolean;
  end;

  TGradientDirection = (grTopBottom, grBottomTop, grLeftRight, grRightLeft,
    grLeftTop, grRightTop, grLeftBottom, grRightBottom,
    grInOut, grOutIn, grHorzInOut, grHorzOutIn, grVertInOut, grVertOutIn);

  /// Description:
  ///   Holds information about background gradient of block.    
  TDiagramGradient = class(TDiagramPersistent)
  private
    FDirection: TGradientDirection;
    FStartColor: TColor;
    FEndColor: TColor;
    FDrawer: TBlockDrawer;
    FVisible: boolean;
    FDiagram: TatDiagram;
    procedure SetDirection(const Value: TGradientDirection);
    procedure SetEndColor(const Value: TColor);
    procedure SetStartColor(const Value: TColor);
    procedure SetVisible(const Value: boolean);
    function GetDgrGradientMode: TDgrGradientMode;
  public

    /// Description:
    ///   Changed method is called whenever a property of the object changes.
    procedure Changed; override;

    /// Description:
    ///   Creates the object.
    constructor Create(ADControl: TDiagramControl; ADiagram: TatDiagram = nil);

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Draws the gradient in the rect ARect, using a Canvas object, rotated by AAngle. DrawGradient is called
    ///   automatically by the control.
    procedure DrawGradient(Canvas: TCanvas; ARect: TSquare; AAngle: double); overload;
    procedure DrawGradient(ADrawer: TDgrBlockDrawer); overload;
  published

    /// Description:
    ///   Specifies the direction (style) of the gradient.
    property Direction: TGradientDirection read FDirection write SetDirection;

    /// Description:
    ///   Specifies the start color of gradient.
    property StartColor: TColor read FStartColor write SetStartColor;

    /// Description:
    ///   Specifies the end color of gradient.
    property EndColor: TColor read FEndColor write SetEndColor;

    /// Description:
    ///   Set Visible to true to make block gradient visible. Gradient is false by default.
    property Visible: boolean read FVisible write SetVisible;
  end;

  /// Description:
  ///   TDControlLinkEvent is the type used for events that notify a link/unlink operation in diagram control.
  ///   ALink parameter contains the line that is being linked/unlinked to ADControl. ALinkPoint contains
  ///   information about the link point.
  TDControlLinkEvent = procedure(ADControl: TDiagramControl; ALink: TCustomDiagramLine;
    ALinkPoint: TLinkPoint) of object;

  /// Description:
  ///   TDiagramLinkEvent is the type used for events that notify a link/unlink operation in diagram. ADControl
  ///   parameter contain the diagram control that is being linked/unlinked, and ALink parameter contains the
  ///   line that is being linked/unlinked to ADControl. ALinkPoint contains information about the link point.
  TDiagramLinkEvent = procedure(Sender: TObject; ADControl: TDiagramControl; ALink: TCustomDiagramLine;
    ALinkPoint: TLinkPoint) of object;

  /// Description:
  ///   TDiagramPosEvent type is used for events where a position must be passed. The position is given
  ///   by X and Y parameters.
  TDiagramPosEvent = procedure(Sender: TObject; X, Y: integer) of object;

/// Description:
///   TAcceptAnchorEvent is the event type for OnAcceptAnchor event of TatDiagram component.
///   That event is fired whenever a link point (ALink parameter) is about to be anchored
///   to another link point (AAnchor parameter). You can set Accept parameter to false if
///   you want avoid that connection to be made.
  TAcceptAnchorEvent = procedure(Sender: TObject; ALink, AAnchor: TLinkPoint;
    var Accept: boolean) of object;

/// Description:
///   DTAcceptLinkEvent is the event type for OnAcceptLink event of TatDiagram component.
///   That event is fired whenever a connection is about to be made between two link points
///   (Anchor1 and Anchor2 parameters). The connection between the two link points is done
///   by a TCustomDiagramLine object (ALink parameter). If you want to avoid that connection
///   to be made, set Accept parameter to false.
  TAcceptLinkEvent = procedure(Sender: TObject; ALink: TCustomDiagramLine; Anchor1,
    Anchor2: TLinkPoint; var Accept: boolean) of object;

  TInsertStyle = (isInsert, isDraw);

  TMouseState = (msBrowsing, msSelecting, msInserting, msToInsert,
    msMoving, msLink, msResizing, msToDraw, msDrawing, msToPan, msPanning,
    msClickingNode, msToZoom, msZooming
    );

  TZoomingState = (zsZoomIn, zsZoomOut);

  TTextCell = class;

  /// Description:
  ///   A TTextCellFrame object holds information about the frame that surrounds the text in the cell.
  TTextCellFrame = class(TPersistent)
  private
    FTextCell: TTextCell;
    FTransparent: boolean;
    FAutoFrame: boolean;
    FAutoFrameMargin: integer;
    FBrush: TBrush;
    FColor: TColor;
    FPen: TPen;
    FVisible: boolean;
    procedure SetAutoFrame(const Value: boolean);
    procedure SetAutoFrameMargin(const Value: integer);
    procedure SetBrush(const Value: TBrush);
    procedure SetColor(const Value: TColor);
    procedure SetPen(const Value: TPen);
    procedure SetTransparent(const Value: boolean);
    procedure SetVisible(const Value: boolean);
    procedure StyleChanged(Sender: TObject);
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ATextCell: TTextCell);

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Notifies the diagram to be redrawn.
    procedure Changed;

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;
  published

    /// Description:
    ///   Sets the background color for the text cell frame.
    property Color: TColor read FColor write SetColor;

    /// Description:
    ///   Sets the brush for the text cell frame.
    property Brush: TBrush read FBrush write SetBrush;

    /// Description:
    ///   Sets the pen for the text cell frame.
    property Pen: TPen read FPen write SetPen;

    /// Description:
    ///   When true, the background color is displayed. If false, no background color is display.
    ///   Only affects Color property.
    property Transparent: boolean read FTransparent write SetTransparent;

    /// Description:
    ///   When set to true, calculates automatically the minimum frame that surrounds the text.
    property AutoFrame: boolean read FAutoFrame write SetAutoFrame;

    /// Description:
    ///   When AutoFrame is true, AutoFrameMargin defines a minimum margin between the text and the frame.
    property AutoFrameMargin: integer read FAutoFrameMargin write SetAutoFrameMargin;

    /// Description:
    ///   Set Visible property to true in order to make the frame visible.
    property Visible: boolean read FVisible write SetVisible;
  end;

  /// Description:
  ///   A TTextCell object holds information for a text cell inside a diagram control. A diagram control can
  ///   have zero, one or more text cells. Each text cell can have its own font, position, color, etc., and can
  ///   be edited by end-user.  
  TTextCell = class(TCollectionItem)
  private
    FWordWrap: boolean;
    FClip: boolean;
    FAngle: double;
    FHeight: number;
    FTop: number;
    FLeft: number;
    FWidth: number;
    FText: string;
    FFont: TFont;
    FDControlFont: boolean;
    FAlignment: TAlignment;
    FVertAlign: TVertAlign;
    FRotateFont: boolean;
    FDControlAngle: boolean;
    FCellFrame: TTextCellFrame;
    FVisible: boolean;
    FObj: TObject;
    FParentTransparency: boolean;
    FTransparency: integer;         
    procedure SetAngle(const Value: double);
    procedure SetClip(const Value: boolean);
    procedure SetFont(const Value: TFont);
    procedure SetHeight(const Value: number);
    procedure SetLeft(const Value: number);
    procedure SetText(const Value: string);
    procedure SetTop(const Value: number);
    procedure SetWidth(const Value: number);
    procedure SetWordWrap(const Value: boolean);
    function IsFontStored: Boolean;
    procedure SetDControlFont(const Value: boolean);
    function GetDControl: TDiagramControl;
    procedure SetBoundsRect(Value: TSquare);
    function GetBoundsRect: TSquare;
    procedure SetAlignment(const Value: TAlignment);
    procedure SetVertAlign(const Value: TVertAlign);
    procedure SetRotateFont(const Value: boolean);
    function GetBottom: number;
    function GetRight: number;
    function IsAngleStored: Boolean;
    procedure SetDControlAngle(const Value: boolean);
    procedure SetCellFrame(const Value: TTextCellFrame);
    procedure SetVisible(const Value: boolean);
    procedure SetParentTransparency(const Value: boolean);
    procedure SetTransparency(const Value: integer);        
  protected

    /// Description:
    ///   GetDiagramRect returns the rect that surrounds the text cell, in client coordinates.
    function GetDiagramRect: TSquare;

    /// Description:
    ///   Called whenever the font settings of text cell changes.
    procedure FontChanged(Sender: TObject);

    /// Description:
    ///   Called whenever the font of the associated diagram control is changed.
    procedure DControlFontChanged;

    /// Description:
    ///   Called whenever the angle of the associated diagram control is changed.
    procedure DControlAngleChanged;

    /// Description:
    ///   BoundsChanged is called whenever the position (BoundsRect) of the text cell is changed.
    procedure BoundsChanged(AOld, ANew: TSquare);

    /// Description:
    ///   Returns the diagram control which the text cell belongs to.
    property DControl: TDiagramControl read GetDControl;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(Collection: TCollection); override;

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Use SetBounds to set the position of a text cell.
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: number);

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   DiagramRect property contains the boundaries of the text cell in absolute client (diagram) coordinates.
    property DiagramRect: TSquare read GetDiagramRect;

    /// Description:
    ///   BoundsRect property contains the rect which defines the position of text cell. The coordinates of the
    ///   rect depends on which diagram control the text cell belongs to. Each diagram control can have its own
    ///   implementation of GetTextCellRect methods, which translates the BoundsRect rect to client (diagram)
    ///   coordinates. For example, for the regular blocks descending from TCustomDiagramBlock, the BoundsRect
    ///   property of a text cell must be interpreted as percentages. For example, if BoundsRect is
    ///   (0, 0, 100, 100), the text cell position will be from the upper left of the block until its bottom
    ///   right point. If BoundsRect is (0, 0, 100, 50), the text cell will be position at the top half of the
    ///   block. Whenever the block size changes, the absolute position of text cell changes accordingly.
    ///   To retrieve the current client position of the text cell, use the DiagramRect read-only property.
    property BoundsRect: TSquare read GetBoundsRect write SetBoundsRect;

    /// Description:
    ///   Returns the right coordinate of text cell. See BoundsRect for more information.
    property Right: number read GetRight;

    /// Description:
    ///   Returns the Bottom coordinate of text cell.
    property Bottom: number read GetBottom;

    /// Description:
    ///   Property for general purpose. You can attach an object information to a text cell.
    property Obj: TObject read FObj write FObj;
  published

    /// Description:
    ///   Font property contains information about the font used to draw text cells' text.
    property Font: TFont read FFont write SetFont stored IsFontStored;

    /// Description:
    ///   if DControlAngle property is true, the Angle of text cell is the same as the diagram control which the
    ///   cell belongs to. Whenever the control is rotated, the text cell rotates together. If DControlAngle is
    ///   false, the Angle property of text cell is not related to the Angle property of diagram control.
    property DControlAngle: boolean read FDControlAngle write SetDControlAngle;

    /// Description:
    ///   Specifies the angle rotation for the text cell. If Angle property changes, DControlAngle property
    ///   sets to false.
    property Angle: double read FAngle write SetAngle stored IsAngleStored;

    /// Description:
    ///   Defines the left coordinate of text cell. See BoundsRect for more information.    
    property Left: number read FLeft write SetLeft;

    /// Description:
    ///   Defines the top coordinate of text cell. See BoundsRect for more information.
    property Top: number read FTop write SetTop;

    /// Description:
    ///   Defines the width of text cell. See BoundsRect for more information.
    property Width: number read FWidth write SetWidth;

    /// Description:
    ///   Defines the height of text cell. See BoundsRect for more information.
    property Height: number read FHeight write SetHeight;

    /// Description:
    ///   Contains the text of text cell.
    property Text: string read FText write SetText;

    /// Description:
    ///   When Clip property is True, the text drawn in the cell is restricted to its boundaries.If Clip is
    ///   False, the text is not clipped.
    property Clip: boolean read FClip write SetClip;

    /// Description:
    ///   Use WordWrap property to specify if text of the cell will be wrapped or not.
    property WordWrap: boolean read FWordWrap write SetWordWrap;

    /// Description:
    ///   If DControlFont property is true, the Font of text cell is the same as the diagram control which the
    ///   cell belongs to. Whenever the control's font changes, the font of text cell changes together.
    ///   If DControlFont is false, the Font property of text cell is not related to the Font property of
    ///   diagram control.
    property DControlFont: boolean read FDControlFont write SetDControlFont;

    /// Description:
    ///   Alignment property specified the horizontal alignment for the block text.
    property Alignment: TAlignment read FAlignment write SetAlignment;

    /// Description:
    ///   VertAlign property specifies the vertical alignment for the cell text.
    property VertAlign: TVertAlign read FVertAlign write SetVertAlign;

    /// Description:
    ///   if RotateFont is true, the text of text cell is rotated by the angle defined by the Angle property.
    ///   If RotateFont is false, the text is not rotated.
    property RotateFont: boolean read FRotateFont write SetRotateFont;

    /// Description:
    ///   Use CellFrame property to specify the visual appearance of the text cell frame.
    property CellFrame: TTextCellFrame read FCellFrame write SetCellFrame;

    /// Description:
    ///   Use Visible property to specify if a text cell is visible or not.
    property Visible: boolean read FVisible write SetVisible;
    property ParentTransparency: boolean read FParentTransparency write SetParentTransparency default true; 
    property Transparency: integer read FTransparency write SetTransparency;                    
  end;

  /// Description:
  ///   TTextCells collection holds the a collection of TTextCell objects for a specified diagram control.
  ///   Each text cell is a block of text anchored to the diagram control which holds the text, color, font,
  ///   position, etc., and can be edited by the end-user. This feature allows a single diagram control to hold
  ///   more than one text cell (text block).  
  TTextCells = class(TOwnedCollection)
  private
    FDControl: TDiagramControl;
    function GetItem(Index: integer): TTextCell;
  protected
    //function GetItemClass: TCollectionItemClass; virtual; abstract;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADControl: TDiagramControl);

    /// Description:
    ///   For internal use.
    procedure Update(Item: TCollectionItem); override;

    /// Description:
    ///   Adds a new TTextCell object to the collection.
    function Add: TTextCell;

    /// Description:
    ///   Retrieves a specified TTextCell object from the collection.
    property Items[Index: integer]: TTextCell read GetItem; default;
  end;

  TLinkPointClass = class of TLinkPoint;

  TDControlPaintMode = (pmNormal, pmNormalCursor, pmCursorOnly);

  TDiagramLayer = class;

  /// Summary:
  ///   Base class for all diagram controls.
  /// Description:
  ///   TDiagramControl is the base (top ancestor) class of all diagram controls (lines and blocks).
  ///   You must not create a TDiagramControl instance, it is an abstract class. Creating a new class descending
  ///   directly from TDiagramControl is not very common. Unless you have full knowledge of diagram architecture
  ///   and want to create a very different control, you does not need to inherit classes from TDiagramControl.
  ///   For creating a new block class, descend it from TCustomDiagramBlock. For creating a new line class,
  ///   descend it from TCustomDiagramLine.
  TDiagramControl = class(TComponent)
  private
    FDiagram: TatDiagram;
    FSelected: boolean;
    FHandles: TStretchHandles;
    FLinkPoints: TLinkPoints;
    FRestrictions: TControlRestrictions;
    FDControlID: string;
    FLinkPointBrush: TBrush;
    FLinkPointPen: TPen;
    FAngle: double;
    FDrawer: TBlockDrawer;
    FTempDrawer: TBlockDrawer; //TempDrawer is only used for internal calculations
    FLinkPointStyle: TLinkPointStyle;
    FOnLinkRemoved: TDControlLinkEvent;
    FOnLinkAdded: TDControlLinkEvent;
    FObj: TObject;
    FInsertStyle: TInsertStyle;
    FHint: string;
    FShowHint: boolean;
    FParentShowHint: Boolean;
    FCursor: TCursor;
    FFont: TFont;
    FDiagramFont: boolean;
    FTextCells: TTextCells;
    FNotifying: integer;
    FStringData: string;
    FVisible: boolean;
    FLayerInteger: integer;
    FLayerObj: TDiagramLayer;
    FCollapsed: boolean;
    FAutoCreateLinkPoints: boolean;
    FIsBackgroundControl: Boolean;
    FDgrDrawer: TDgrBlockDrawer;
    FTempPath: TDgrGraphicsPath;
    FIntPaintMode: TDControlPaintMode;
    FTransparency: integer;
    FGroupBlock: TGroupBlock;
    FLayerNameFix: string;
    FHandlesAppearance: THandlesAppearance;
    FGraphicLib: TDgrGraphicLib;
    FCustomGraphicLib: boolean;
    procedure SetPaintMode(Value: TDControlPaintMode);
    procedure SetDiagram(const Value: TatDiagram);
    procedure SetSelected(Value: boolean);
    procedure SetHandles(const Value: TStretchHandles);
    procedure IntDiagramChanged(Old, New: TatDiagram);
    procedure SetRestrictions(const Value: TControlRestrictions);
    procedure SetLinkPoints(const Value: TLinkPoints);
    procedure DrawLinkPoint(ALinkPoint: TLinkPoint; APoint: TPoint);
    procedure SetLinkPointBrush(const Value: TBrush);
    procedure SetLinkPointPen(const Value: TPen);
    procedure DrawObjectChanged(Sender: TObject);
    procedure AddAnchored(AnchorLinkPoint, ControlLinkPoint: TLinkPoint);
    procedure RemoveAnchored(AnchorLinkPoint, ControlLinkPoint: TLinkPoint);
    procedure SetAngle(const Value: double);
    function IsAngleStored: Boolean;
    procedure DrawLinkPoints(ACanvas: TCanvas; APaintEngine: TDgrPaintEngine; OnlyNodes: boolean = false);
    procedure Paint(Canvas: TCanvas; ADgrPaintEngine: TDgrPaintEngine=nil);
    function GetDControlID: string;
    procedure LoadDControlIDProp(Reader: TReader);
    procedure StoreDControlIDProp(Writer: TWriter);
    procedure SetLinkPointStyle(const Value: TLinkPointStyle);
    function InsertMouseState: TMouseState;
    function IsShowHintStored: Boolean;
    procedure SetShowHint(const Value: boolean);
    procedure SetParentShowHint(const Value: Boolean);
    procedure DiagramShowHintChanged;
    procedure SetDiagramFont(const Value: boolean);
    procedure SetFont(const Value: TFont);
    procedure DiagramFontChanged;
    function IsFontStored: Boolean;
    procedure SetTextCells(const Value: TTextCells);
    procedure SetVisible(const Value: boolean);
    function GetLayerName: string;
    procedure SetLayerName(const Value: string);
    function GetLayerInteger: integer;
    procedure SetLayerInteger(const Value: integer);
    procedure SetLayerObj(Value: TDiagramLayer);
    procedure LoadCollapsedProp(Reader: TReader);
    procedure StoreCollapsedProp(Writer: TWriter);
    function GetIsVisible: boolean;
    function GetSelected: boolean;
    procedure InternalWriteText(Canvas: TCanvas; AFont: TFont;
      ARect: TSquare; const S: string; HAlign: TAlignment;
      VAlign: TVertAlign; AWrap: boolean; AAngle: double; CX, CY: number;
      DrawText: boolean; var NewRect: TSquare; IgnoreZoom: boolean);
    function InternalWriteTextDgr(ADrawer: TDgrBlockDrawer; AFont: TFont;
      ARect: TSquare; const S: string; HAlign: TAlignment;
      VAlign: TVertAlign; AWrap: boolean; AAngle: double; RotC: TDot;
      AClip, ADrawFrame, ADrawText: boolean; ATransparency: integer;
      AFrameMargin: integer=0): TSquare;
    procedure SetIsBackgroundControl(const Value: Boolean);
    procedure SetTransparency(const Value: integer);
    function DrawGrouped: boolean;
    procedure BeginNotifying;
    procedure EndNotifying;
    function GetGroupBlock: TGroupBlock;
    function GetMasterGroup: TGroupBlock;
    procedure IncludedInGroup(AGroup: TGroupBlock);
    procedure RemovedFromGroup(AGroup: TGroupBlock);
    function GetIsMember: boolean;
    function GetIsGroup: boolean;
    function CanRotate: boolean;
    function CanResize: boolean;
    function CanMove: boolean;
    function CanSelect: boolean;
    function CanDoClipboard: boolean;
    function CanEditText: boolean;
    function CanMoveRotCenter: boolean;
    function CanLink: boolean;
    function LayerEditable: boolean;
    function GetAsMember: boolean;
    procedure PaintHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle; AAppearance: THandlesAppearance);
    procedure GetDefaultHandlesAppearance(AAppearance: THandlesAppearance);
    function GetGraphicLib: TDgrGraphicLib;
    procedure SetGraphicLib(const Value: TDgrGraphicLib);
    function GetOriginalRect: TSquare;
    procedure SetOriginalRect(const Value: TSquare);
    {$IFDEF GDIPLUS}
    function GetGPDrawer: TGPBlockDrawer; {$IFDEF DELPHI2005_LVL} deprecated; {$ENDIF}
    {$ENDIF}
    property PaintMode: TDControlPaintMode read FIntPaintMode write SetPaintMode;
    function GetIsGdiPlus: boolean; {$IFDEF DELPHI2005_LVL} deprecated; {$ENDIF}
    procedure SetIsGdiPlus(const Value: boolean); {$IFDEF DELPHI2005_LVL} deprecated; {$ENDIF}
    function GetPathDrawingMode: boolean;
    procedure CheckGraphicLibObjects;
    function DgrBlockDrawerClass: TDgrBlockDrawerClass;
    procedure RestoreLayer;
  protected

    /// Description:
    ///   Called when a handle is destroyed.
    procedure HandleDestroyed(AHandle: TStretchHandle); virtual;

    /// Description:
    ///   Called when a text cell is destroyed.
    procedure TextCellDestroyed(ACell: TTextCell); virtual;

    /// Description:
    ///   For internal use.
    procedure RemoveAnchorsAfterMoving; virtual;

    /// Description:
    ///   Find the TLinkPoint object associated with a specified Handle, if any.
    function LinkPointByHandle(AHandle: TStretchHandle): TLinkPoint; virtual;

    /// Description:
    ///   Returns true if the diagram control has link points which can be anchored to other link points.
    function HasAnchorableLinkPoints: boolean;

    /// Description:
    ///   For internal use.
    procedure StartCursorPlacement; virtual;

    /// Description:
    ///   For internal use.
    procedure EndCursorPlacement; virtual;

    /// Description:
    ///   For internal use.
    procedure InitDgrDrawer(ADrawer: TDgrBlockDrawer); virtual;

    /// Description:
    ///   For internal use.
    procedure InitDrawer(ADrawer: TBlockDrawer); virtual;

    /// Description:
    ///   For internal use only.
    procedure UpdateSelected;

    /// Description:
    ///   Called when text cells are changed.
    procedure TextCellsChanged; virtual;

    /// Description:
    ///   HandlesChanged method is called whenever the handles collection change.
    procedure HandlesChanged; virtual; abstract;

    /// Description:
    ///   For internal use.    
    procedure Loaded; override;

    /// Description:
    ///   Called whenever the font settings changes.
    procedure FontChanged(Sender: TObject);

    /// Description:
    ///   Clips the region specified by R.
    procedure ClipRegion(Canvas: TCanvas; R: HRGN);

    /// Description:
    ///   For internal use only.
    procedure UnclipRegion(Canvas: TCanvas);

    /// Description:
    ///   GetTextCellRect receives the rect settings of a textl cells (in the ARect parameter), and returns
    ///   canvas coordinates of that text cell. It is used, for example, by TCustomDiagramBlock class, which
    ///   interprets the rect of the text cell as relative coordinates, and translate those coordinates into
    ///   canvas coordinates, using the block size.
    function GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare; virtual;

    /// Description:
    ///   Writes a text in the canvas, according to the parameters. For internal use only.
    procedure WriteText(Canvas: TCanvas; AFont: TFont; ARect: TSquare;
      const S: string; HAlign: TAlignment; VAlign: TVertAlign;
      AWrap: boolean; AAngle: double = 0; CX: number = 0; CY: number = 0; IgnoreZoom: boolean = false); virtual;

    /// Description:
    ///   Calculates minimum text rect that surrounds the text specified by S.
    function CalculateTextRect(Canvas: TCanvas; AFont: TFont; ARect: TSquare;
      const S: string; HAlign: TAlignment; VAlign: TVertAlign;
      AWrap: boolean; AAngle: double = 0; CX: number = 0; CY: number = 0; IgnoreZoom: boolean = false): TSquare;

    /// Description:
    ///   DoLinkAdded is called when a line ALink is attached to link point ALinkPoint of control.
    procedure DoLinkAdded(ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint); virtual;

    /// Description:
    ///   DoLinkRemoved is called when a line ALink is deattached from link point ALinkPoint of control.
    procedure DoLinkRemoved(ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint); virtual;

    /// Description:
    ///   For internal use.
    procedure DefineProperties(Filer: TFiler); override;

    /// Description:
    ///   AngleChanged method is called whenever the Angle property is changed.
    procedure AngleChanged; virtual;

    /// Description:
    ///   AngleChangedEx method is called whenever the Angle property is changed from AOld to ANew.
    procedure AngleChangedEx(AOld, ANew: double); virtual;

    /// Description:
    ///   MovedLinkPoint returns the link point coordinates (diagram coordinates) moved ADelta.
    function MovedLinkPoint(ALinkPoint: TLinkPoint; ADelta: TDot): TDot;

    /// Description:
    ///   If the diagram control is moved or removed from diagram, its NotifyAnchoreds method is called to
    ///   notify all controls attached to it, in order for them to repositionte themselves.
    procedure NotifyAnchoreds(AChange: TAnchorChange);

    /// Description:
    ///   AnchorLinkChanged is called when the anchor link of ALinkPoint link point is changed.
    ///   Changing properties AnchorLink, Anchor or AnchorIndex of TLinkPoint will call AnchorLinkChanged.
    procedure AnchorLinkChanged(ALinkPoint: TLinkPoint); virtual; abstract;

    /// Description:
    ///   If ALinkPoint link point is anchored to another link point (and, in turn, to a control),
    ///   then LinkedAnchorChanged is called whenever a change occurs to the anchor.
    ///   AChange parameter contains the type of change occured.
    procedure LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange); virtual; abstract;

    /// Description:
    ///   Redraw method forces the control to be redrawn.
    procedure Redraw; virtual;

    /// Description:
    ///   Draw all text cells of diagram control.
    procedure DrawCells(AInfo: TDiagramDrawInfo); virtual;

    /// Description:
    ///   Draws the specified text cell.
    procedure DrawCell(AInfo: TDiagramDrawInfo; ACell: TTextCell); virtual;

    /// Description:
    ///   PaintControl is called by the diagram when the control needs to be painted. You can override this
    ///   method to perform your own drawing procedure. PaintControl is the most base method for painting the
    ///   control. If you override it, you must handle all painting procedures (erasing background, for example).
    ///   Information about control drawing is given by AInfo record.
    procedure PaintControl(AInfo: TDiagramDrawInfo); virtual; abstract;

    /// Description:
    ///   Returns true if point Pos is inside the control.
    function PointInControl(Pos: TPoint): boolean; virtual; abstract;

    /// Description:
    ///   DblClick method is called when the control is double clicked.
    procedure DblClick; virtual;

    /// Description:
    ///   Click method is called when the control is clicked.
    procedure Click; virtual;

    /// Description:
    ///   MouseDown method is called when the user presses the mouse button when the mouse is over the control.
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); virtual;

    /// Description:
    ///   MouseDown method is called when the user presses the mouse button when the mouse is over the control.
    ///   The AHandle parameter specifies the handle to be used.
    procedure MouseDownEx(Button: TMouseButton; Shift: TShiftState; X, Y: Integer; var AHandle: TStretchHandle); virtual;

    /// Description:
    ///   MouseMove method is called when the user moves the mouse over the control.
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); virtual;

    /// Description:
    ///   MouseUp method is called when the user releases the mouse button when the mouse is over the control.
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); virtual;

    /// Description:
    ///   MovePosition method moves the control by ADeltaX and ADeltaY.
    procedure MovePosition(ADeltaX, ADeltaY: number); virtual; abstract;
    {ControlRect must return the minimum rect that covers all control points (WITHOUT ROTATING)}

    /// Description:
    ///   ControlRect contains the smallest TSquare (in diagram coordinates) that contains the whole control.
    ///   ControlRect ignores if control is rotated (Angle property different from zero), always considering
    ///   an angle of zero.
    function ControlRect: TSquare; virtual; abstract;
    {SurroundRect must return the minimum rect that covers all control points with control already rotated}

    /// Description:
    ///   SurroundRect contains the smallest TSquare (in diagram coordinates) that contains the whole control.
    ///   SurroundRect considers the control rotation.
    function SurroundRect: TSquare; virtual; abstract;

    /// Description:
    ///   StartMoving method is called whenever the control is about to be moved by the user.
    procedure StartMoving; virtual; abstract;

    /// Description:
    ///   UpdateMoving method is called when the user is moving the control. For each mouse move, UpdateMoving
    ///   is called once. Information about moving is given by AInfo. Blocks and lines classes can override
    ///   this method for implementing their own custom responses for control moves.
    procedure UpdateMoving(AInfo: TControlMoveInfo); virtual; abstract;

    /// Description:
    ///   UpdateInserting method is called when the user is inserting the control in the diagram. For each mouse
    ///   click/drag/release, UpdateInserting is called once. Information about inserting is given by AInfo.
    ///   Blocks and lines classes can override this method for implementing their own custom responses for
    ///   control inserting.
    procedure UpdateInserting(AInfo: TControlInsertInfo); virtual; abstract;

    /// Description:
    ///   UpdateDrawing method is called when the user is drawing a control in the diagram (multi-step
    ///   insertion, like poly lines and polygons). For each mouse click/drag/release, UpdateDrawing is called
    ///   once. Information about drawing is given by AInfo. Blocks and lines classes can override this method
    ///   for implementing their own custom responses for control drawing.
    procedure UpdateDrawing(AInfo: TControlDrawInfo); virtual; abstract;

    /// Description:
    ///   StartMovingHandle method is called whenever a handle of control is about to be moved by the user.    
    procedure StartMovingHandle; virtual; abstract;

    /// Description:
    ///   HandleMoving method is called when the user is moving a handle of the control. For each mouse move,
    ///   HandleMoving is called once. Information about handle moving is given by AInfo. Blocks and lines
    ///   classes can override this method for implementing their own custom responses for control handle moves.
    procedure HandleMoving(AInfo: THandleMoveInfo); virtual; abstract;

    /// Description:
    ///   DrawHandles draws the handles of the control. Information about drawing is given by AInfo.
    procedure DrawHandles(AInfo: TDiagramDrawInfo); virtual;

    /// Description:
    ///   Draws the specified handle.
    procedure DrawHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle); virtual;
    {GetHandlePoint and GetLinkPoint should return handle point in diagram coords}

    /// Description:
    ///   GetHandlePoint returns the diagram coordinates of TStretchHandle specified by AHandle.
    ///   Blocks and lines descendants can override this method to perform their own conversion
    ///   from OrX and OrY coordinates of handle to diagram coordinates.
    function GetHandlePoint(AHandle: TStretchHandle): TDot; virtual;

    /// Description:
    ///   GetLinkPoint returns the diagram coordinates of TLinkPoint specified by ALinkPoint.
    ///   Blocks and lines descendants can override this method to perform their own conversion
    ///   from OrX and OrY coordinates of link point to diagram coordinates.
    function GetLinkPoint(ALinkPoint: TLinkPoint): TDot; virtual;

    /// Description:
    ///   DiagramChanged method is called when the Diagram property of control is changed.
    ///   Old and New parameters contain the old value and new value of Diagram property.    
    procedure DiagramChanged(Old, New: TatDiagram); virtual;

    /// Description:
    ///   Used for internal purpose.
    procedure SetParentComponent(AParent: TComponent); override;

    /// Description:
    ///   For internal use.
    procedure ReadState(Reader: TReader); override;

    /// Description:
    ///   SelectedChanged method is called whenever the a control is selected or unselected
    ///  (Selected property changes).    
    procedure SelectedChanged; virtual;

    /// Description:
    ///   Override GetLinkPointClass method to specify a class for the link point. The default is TLinkPoint,
    ///   but you can create a TLinkPoint descendant and return that new class in GetLinkPointClass.
    function GetLinkPointClass: TLinkPointClass; virtual;

    /// Description:
    ///   CreateGraphicLibObjects is called when a non-GDI GraphicLib is selected for drawing and
    ///   specific GraphicLib descendants must be created.
    procedure CreateGraphicLibObjects; virtual;

    /// Description:
    ///   DestroyGraphicLibObjects is called when non-GDI GraphicLib objects must be destroyed.
    procedure DestroyGraphicLibObjects; virtual;

    /// Description:
    ///    CreatePath creates and returns a TDgrGraphicsPath descendant, according to selected GraphicLib
    ///    (when using PathDrawingMode).
    function CreatePath: TDgrGraphicsPath;

    /// Description:
    ///   GetDiagramDrawInfo returns a TDiagramDrawInfo record with current Drawer/Canvas properties assigned to its fields.
    function GetDiagramDrawInfo: TDiagramDrawInfo;

    /// Description:
    ///   DiagramDrawInfo returns a TDiagramDrawInfo record with specified parameters assigned to its fields.
    class function DiagramDrawInfo(ACanvas: TCanvas; ADrawer: TBlockDrawer; ADgrDrawer: TDgrBlockDrawer): TDiagramDrawInfo;

    /// Description:
    ///   Defines if the control is inserted in the regular way, or by drawing (like poly lines or polygons).
    property InsertStyle: TInsertStyle read FInsertStyle write FInsertStyle;

    /// Description:
    ///   Retrieves an instance for a TBlockDrawer class to help in the drawing of the block.
    property Drawer: TBlockDrawer read FDrawer;

    /// Description:
    ///   An instance of a TBlockDrawer object to be used for temporary operations (you can safely change its
    ///   properties).
    property TempDrawer: TBlockDrawer read FTempDrawer;

    {$IFDEF GDIPLUS}
    {$WARNINGS OFF}
    /// Description:
    ///   Retrieves an instance for a TGPBlockDrawer class to help in the drawing of the block using GDI+ functions.
    ///   This property is deprecated, use DgrDrawer instead.
    property GPDrawer: TGPBlockDrawer read GetGPDrawer;
    {$WARNINGS ON}
    {$ENDIF}

    /// Description:
    ///   An instance of a TDgrGraphicsPath descendant to be used for temporary operations
    ///   (you can safely change its properties).
    property TempPath: TDgrGraphicsPath read FTempPath;

    /// Description:
    ///   Contains information about the handles of the control.
    property Handles: TStretchHandles read FHandles write SetHandles;

    /// Description:
    ///   Contains information about the link points of the control.
    property LinkPoints: TLinkPoints read FLinkPoints write SetLinkPoints;

    /// Description:
    ///   Not used in current version.
    property LinkPointPen: TPen read FLinkPointPen write SetLinkPointPen;

    /// Description:
    ///   Not used in current version.
    property LinkPointBrush: TBrush read FLinkPointBrush write SetLinkPointBrush;

    /// Description:
    ///   Not used in current version.    
    property LinkPointStyle: TLinkPointStyle read FLinkPointStyle write SetLinkPointStyle;

    /// Description:
    ///   Use Angle property to specify angle rotation of the control.
    property Angle: double read FAngle write SetAngle stored IsAngleStored;

    /// Description:
    ///   OnLinkAdded event is called when a line is attached to a link point of the control.
    property OnLinkAdded: TDControlLinkEvent read FOnLinkAdded write FOnLinkAdded;

    /// Description:
    ///   OnLinkRemoved event is called when a line is de-attached from a link point of the control.
    property OnLinkRemoved: TDControlLinkEvent read FOnLinkRemoved write FOnLinkRemoved;

    {$WARNINGS OFF}
    /// Description:
    ///   Specifies if a diagram control should be drawn using GDI+ API functions. If directive GDIPLUS is not
    ///   defined, this property will always be false.
    property IsGdiPlus: boolean read GetIsGdiPlus write SetIsGdiPlus;
    {$WARNINGS ON}

    /// Description:
    ///   Specifies the graphic library used for drawing of control. By default all diagram controls
    ///   are drawn using library defined in TatDiagram.GraphicLib property.
    property GraphicLib: TDgrGraphicLib read GetGraphicLib write SetGraphicLib;

    /// Description:
    ///   PathDrawingMode returns true when a non-GDI GraphicLib is selected and drawing is done by
    ///   using paths and TDgrBlockDrawer descendants.
    property PathDrawingMode: boolean read GetPathDrawingMode;

    /// Description
    ///   OriginalRect specifies the source rectangle where control will be painted.
    property OriginalRect: TSquare read GetOriginalRect write SetOriginalRect;
  public

    /// Description:
    ///   Creates an instance of TDiagramControl.
    constructor Create(AOwner: TComponent); override;

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// Description:
    ///   For internal use.
    function HasParent: boolean; override;

    /// Description:
    ///   For internal use.
    function GetParentComponent: TComponent; override;

    /// Description:
    ///   Returns true if the current object has a default text cell.
    function HasDefaultTextCell: boolean;

    /// Description:
    ///   Returns the default text cell of the control (in most cases, TextCells[0]).
    function DefaultTextCell: TTextCell;

    /// Description:
    ///   Brings the diagram control to the front of diagram.
    procedure BringToFront;

    /// Description:
    ///   Sends the diagram control to the back of diagram.
    procedure SendToBack;

    /// Description:
    ///   Start text editing of diagram control.
    procedure EditText;

    /// Description:
    ///   Override this method in TDiagramControl descendants in order to resize the control before it's painted
    ///   in the diagram. CalcSize is called just before the control is painted.
    procedure CalcSize(Canvas: TCanvas); virtual;

    /// Description:
    ///   Call MakeVisible method to make the diagram control visible in the center of diagram (when possible).
    procedure MakeVisible;

    /// Description:
    ///   SelectNextcell returns the next/prior text cell in diagram control, following from ACell.
    function SelectNextCell(ACell: TTextCell; AForward: boolean): TTextCell;

    /// Description:
    ///   TextCells property holds the collection of text cells of the diagram control.
    property TextCells: TTextCells read FTextCells write SetTextCells;

    /// Description:
    ///   Restrictions property specifies what actions end-user can perform on control (moving, resizing,
    ///   editing text, etc.).
    property Restrictions: TControlRestrictions read FRestrictions write SetRestrictions;

    /// Description:
    ///   Indicates if control is selected (true) or not (false).
    property Selected: boolean read GetSelected write SetSelected;

    /// Description:
    ///   Diagram property specifies the diagram that contains the control. When a control is created,
    ///   its Diagram property is set to nil. Set Diagram property to insert the control in the diagram.
    property Diagram: TatDiagram read FDiagram write SetDiagram;

    /// Description:
    ///   Contains the ID of diagram control, as specified in the diagram control registering system.
    property DControlID: string read GetDControlID write FDControlID;

    /// Description:
    ///   Use Obj property to link the control to a object. Obj property is not used by the diagram, and the
    ///   object is not automatically saved with the diagram.    
    property Obj: TObject read FObj write FObj;

    /// Description:
    ///   Holds hinting text for the diagram control.    
    property Hint: string read FHint write FHint;

    /// Description:
    ///   Determines whether the control displays a Help Hint when the mouse pointer rests momentarily on the control.
    ///   Changing the ShowHint value automatically sets the ParentShowHint property to False.
    property ShowHint: boolean read FShowHint write SetShowHint stored IsShowHintStored;

    /// Description:
    ///   If ParentShowHint is True, the control uses the ShowHint property value of the diagram. If ParentShowHint
    ///   is False, the control uses the value of its own ShowHint property.
    property ParentShowHint: Boolean read FParentShowHint write SetParentShowHint;

    /// Description:
    ///   Defines the mouse cursor to be displayed when the mouse is over the control.
    property Cursor: TCursor read FCursor write FCursor;

    /// Description:
    ///   Font property contains information about the font used to draw the text cells of the control.    
    property Font: TFont read FFont write SetFont stored IsFontStored;

    /// Description:
    ///   If DiagramFont is true, the font of the diagram control will be the same as diagram font - whenever
    ///   the diagram font is updated, the control font is updated. If DiagramFont is false, the font of the
    ///   control is not related to the diagram font.
    property DiagramFont: boolean read FDiagramFont write SetDiagramFont;

    /// Description:
    ///   Use StringData to hold string information related to the control. StringData property is not used by
    ///   diagram component. The property is saved with the diagram, so it can be used for persistent information.    
    property StringData: string read FStringData write FStringData;

    /// Description:
    ///   Use Visible property to set if the control must be visible in diagram. Setting Visible to false will
    ///   certainly hide the control, but setting Visible to true does not ensure that the control will be
    ///   visible in the diagram. It will depend of the visible (active) layers, and also will depend if the
    ///   block is collapsed by a node (collapsable link point). To check if a control is effectively visible
    ///   in the diagram, use IsVisible method.
    property Visible: boolean read FVisible write SetVisible;

    /// Description:
    ///   Use Layer property to specify which layer the control belongs to.
    property Layer: integer read GetLayerInteger write SetLayerInteger;

    /// Description:
    ///   Specifies which layer object the diagram control belongs to.
    property LayerObj: TDiagramLayer read FLayerObj write SetLayerObj;

    /// Description:
    ///   Used to get/set layer object the diagram control belongs to, based on the layer name.
    property LayerName: string read GetLayerName write SetLayerName;

    /// Description:
    ///   Used for block objects. When AutoCreateLinkPoints is true, the block will always accept a line
    ///   connection, even if there is no linkpoint for the line to be anchored to. If the end-user "connects"
    ///   the line to the block, a implicit linkpoint is automatically created.
    property AutoCreateLinkPoints: boolean read FAutoCreateLinkPoints write FAutoCreateLinkPoints;

    /// Description:
    ///   IsVisible returns true when the control is visible in the diagram.
    property IsVisible: boolean read GetIsVisible;

    /// Description:
    ///   Specifies if an object is a "background control" or not. Background controls are painted before regular controls,
    ///   and are displayed behind all other controls.
    property IsBackgroundControl: Boolean read FIsBackgroundControl write SetIsBackgroundControl;

    /// Description:
    ///   Use Transparency property to set the transparency level of a control. This is a global transparency
    ///   property, so it will affect back color, image, gradient, pen and text transparencies at the same time.
    ///   Value must range from 0 (opaque control, no transparency) to 100 (total transparency).
    property Transparency: integer read FTransparency write SetTransparency;

    /// Description:
    ///   Use GroupBlock property to retrieve the instance of TGroupBlock object for which the diagram control
    ///   belongs to. If GroupBlock is nil, it means that the control is not grouped.
    property GroupBlock: TGroupBlock read GetGroupBlock;

    /// Description:
    ///   MasterGroup property returns the top-level group block that holds the current diagram control.
    ///   For example, for multiple levels grouping, MasterGroup returns the top group block which holds all
    ///   levels. It's different from GroupBlock property, which holds immediate group block the holds the
    ///   current diagram control. Note the for the top-most group block, GroupBlock will be nil (because the
    ///   group block is not grouped, but it's a group), however MasterGroup will point to the group block itself.
    ///   If the diagram control is not grouped or is not a group, MasterGroup is nil.
    property MasterGroup: TGroupBlock read GetMasterGroup;

    /// Description:
    ///   Use IsMember property to know if the diagram control belongs to a group (IsMember is true in this case).
    ///   If IsMember is false, the diagram control is not grouped.
    property IsMember: boolean read GetIsMember;

    /// Description:
    ///   Use IsGroup property to know if a diagram control is a group (holds other objects inside it).
    property IsGroup: boolean read GetIsGroup;

    /// Description:
    ///   AsMember returns true if the diagram control is a group and is currently selected.
    property AsMember: boolean read GetAsMember;
  end;

  /// Description:
  ///   TDrawPoint object contains a point for drawing the line of a TCustomDiagramLine object.
  TDrawPoint = class(TCollectionItem)
  private
    FX: number;
    FY: number;
    procedure SetX(const Value: number);
    procedure SetY(const Value: number);
    function GetP: TDot;
    procedure SetP(const Value: TDot);
  public
    /// Description:
    ///   Copies information from a TDrawPoint object specified by Source.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Contains the line point.
    property P: TDot read GetP write SetP;
  published

    /// Description:
    ///   X coordinate of the line point.
    property X: number read FX write SetX;

    /// Description:
    ///   Y coordinate of the line point.
    property Y: number read FY write SetY;
  end;

  /// Description:
  ///   TDrawPoints class holds a collection of points used to draw the whole line: a line segment is drawn
  ///   from first point to the second, then from the second to third, etc..  
  TDrawPoints = class(TOwnedCollection)
  private
    function GetItem(Index: integer): TDrawPoint;
  public

    /// Description:
    ///   Add a new TDrawPoint item.
    function Add: TDrawPoint; overload;
    function Add(AX, AY: number): TDrawPoint; overload;

    /// Description:
    ///   Use Append function to append APoints to the current collection of draw points.
    procedure Append(APoints: TDrawPoints);

    /// Description:
    ///   Use Items property to access individual TDrawPoint objects.
    property Items[Index: integer]: TDrawPoint read GetItem; default;
  end;

  TDiagramControlClass = class of TDiagramControl;

  TTransMemo = class(TMemo)
  private
    procedure WMTransMemoInvalidate(var Message:TMessage); message WM_TRANSMEMOINVALIDATE;
    procedure WMGetDlgCode(var Message: TWMGetDlgCode); message WM_GETDLGCODE;
    procedure WMSetText(var Message:TWMSetText); message WM_SETTEXT;
    procedure CNCTLCOLOREDIT(var Message:TWMCTLCOLOREDIT); message CN_CTLCOLOREDIT;
    procedure WMKeyDown(var Message: TWMKeyDown); message WM_KEYDOWN;
    procedure WMEraseBkgnd(var Message: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure WMHScroll(var Message: TWMHScroll); message WM_HSCROLL;
    procedure WMVScroll(var Message: TWMVScroll); message WM_VSCROLL;
  protected

    /// Description:
    ///   For internal use.
    procedure CreateParams(var Params: TCreateParams); override;
  public
    constructor Create(AOwner: TComponent); override;
  end;

  /// Description:
  ///   TDiagramMemo is a TMemo descendant used internally for inplace editing of text of blocks. Only for
  ///   internal use.
  TDiagramMemo = class(TTransMemo)
  private
    FDiagram: TatDiagram;
    FCell: TTextCell;
    FKeepPosition: boolean;
    procedure UpdatePosition;
    function CalculateHeight: integer;
    function DControl: TDiagramControl;
  protected

    /// Description:
    ///   For internal use.
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;

    /// Description:
    ///   For internal use.
    procedure KeyPress(var Key: Char); override;

    /// Description:
    ///   For internal use.
    procedure Change; override;

    /// Description:
    ///   For internal use.
    procedure WndProc(var Msg: TMessage); override;
  public
    /// Description:
    ///   Creates the object.
    constructor Create(AOwner: TComponent); override;
  end;

  TBlockShape = (bsNoShape, bsRectangle, bsEllipse, bsRoundRect, bsDiamond, bsSquareRound);

  /// Description:
  ///   For the ABlock diagram block, you can provide its surround region by setting the APts array. Diagram
  ///   component has its own default way to define the surround region, you can use this event to set it by
  ///   yourself.
  TGetSurroundPtsEvent = procedure(Sender: TObject; ABlock: TCustomDiagramBlock;
    var APts: TPointArray) of object;

  /// Description:
  ///   TDrawBlockEvent is the event type for OnDrawBlock event of TatDiagram component. By handling this
  ///   event user can implement their own block drawing procedures. Information about block drawing
  ///   (Rect, Canvas, Drawer etc.) is given by AInfo and ABlockInfo records. If the block is drawn APainted
  ///   parameter must be set to True. Otherwise, the block will be also drawn using its default drawing procedure.
  ///   TDrawBlockEvent is also handler for OnDrawShape event. Use it for custom drawing of diagram shapes.
  ///   When doing custom shape drawing, you don't need to worry about pen, brush, shadow issues - the diagram
  ///   component does it for you. All you need to do is use the canvas object given by AInfo.Canvas parameter
  ///   (which is already set with the correct pen, brush and other settings), and draw a shape in the region
  ///   given by ABlockInfo.Rect parameter, at the angle given by ABlockInfo.Angle parameter.
  ///   Set APainted parameter to true if you do not the diagram component to perform the default shape drawing.
  TDrawBlockEvent = procedure(Sender: TObject; ABlock: TCustomDiagramBlock;
    AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo;
    var APainted: boolean) of object;

  /// Description:
  ///   TPointArray is a dynamic array of TPoint used for specifying regions and drawing polygons.
  TPointArray = DiagramUtils.TPointArray;

  /// Description:
  ///   Holds information about shadow of block.
  TBlockShadow = class(TDiagramPersistent)
  private
    FVisible: boolean;
    FHOffset: integer;
    FVOffset: integer;
    FColor: TColor;
    FTransparency: integer;
    procedure SetColor(const Value: TColor);
    procedure SetHOffset(const Value: integer);
    procedure SetVisible(const Value: boolean);
    procedure SetVOffset(const Value: integer);
    procedure SetTransparency(const Value: integer);
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADControl: TDiagramControl);

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;
  published

    /// Description:
    ///   Set Visible to true to make block shadow visible. Shadow is false by default.
    property Visible: boolean read FVisible write SetVisible;

    /// Description:
    ///   Specifies color of shadow.
    property Color: TColor read FColor write SetColor;

    /// Description:
    ///   HOffset and VOffset properties specifies position of shadow relative to block position.
    ///   For example, for a very thin left/bottom shadow, set HOffset to -1 and VOffset to 1.
    property HOffset: integer read FHOffset write SetHOffset;

    /// LinkedTo:
    ///   atDiagram.TBlockShadow.HOffset
    property VOffset: integer read FVOffset write SetVOffset;

    /// Description:
    ///   Use Transparency property to set the transparency of shadow. Values must range from 0 (total opacity)
    ///   to 100 (total transparency).
    property Transparency: integer read FTransparency write SetTransparency;
  end;

  TBitmapMode = (bmStretch, bmCenter, bmTile);
  TPictureMode = (pmStretch, pmCenter, pmTile);

  /// Description:
  ///   TCustomDiagramBlock is the ancestor class for all block objects in diagram. If you want to create a new
  ///   block class, descend it from TCustomDiagramBlock, override the correct methods and publish the desired
  ///   properties. When creating new block classes descending from TCustomDiagramBlock, the most often
  ///   overriden methods are DrawShape (or DrawBlock or PaintControl) and SurroundRgn.
  TCustomDiagramBlock = class(TDiagramControl)
  private
    FHeight: number;
    FWidth: number;
    FLeft: number;
    FTop: number;
    FCursorAngle: double;
    FCursorRect: TSquare;
    FCursorRotCenter: TDot;
    FShape: TBlockShape;
    FBrush: TBrush;
    FSelBrush: TBrush;
    FPen: TPen;
    FStrings: TStrings; //FStrings field is use only for backward compatibility
    FClipText: boolean;
    FMinWidth: integer;
    FMinHeight: integer;
    FShadow: TBlockShadow;
    FGradient: TDiagramGradient;
    FBitmap: TBitmap;
    FBitmapMode: TBitmapMode;
    FDgrBitmap: TDgrGraphic; //For optimization purposes
    FDgrMetafile: TDgrGraphic; //For optimization purposes
    FDgrPicture: TDgrGraphic; //For optimization purposes
    FPictureMode: TPictureMode;
    FMetafile: TMetafile;
    FPicture: TPicture;
    FShapeCursor: boolean;
    FSelColor: TColor;
    FColor: TColor;
    FTransparent: boolean;
    FRotationStep: integer;
    FSaveAngle: double;
    FSaveRect: TSquare;
    FSaveRotCenter: TDot;
    FRotCenter: TDot;
    function GetBoundsRect: TSquare;
    procedure SetBoundsRect(const Value: TSquare);
    function GetBottom: number;
    function GetRight: number;
    procedure SetHeight(const Value: number);
    procedure SetLeft(const Value: number);
    procedure SetTop(const Value: number);
    procedure SetWidth(const Value: number);
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: number);
    procedure SetShape(const Value: TBlockShape);
    procedure SetBrush(const Value: TBrush);
    procedure SetPen(const Value: TPen);
    procedure SetSelBrush(const Value: TBrush);
    procedure SetStrings(const Value: TStrings);
    procedure StringsChange(Sender: TObject);
    procedure SetAlignment(const Value: TAlignment);
    procedure SetVertAlign(const Value: TVertAlign);
    procedure SetClipText(const Value: boolean);
    procedure SetWordWrap(const Value: boolean);
//    function ClientToDiagram(APoint: TPoint): TPoint; overload;
//    function ClientToDiagram(ARect: TRect): TRect; overload;
    procedure SetMinHeight(const Value: integer);
    procedure SetMinWidth(const Value: integer);
    function ResizeNewRect(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TSquare;
    function RotateNewAngle(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): double;
    function RotateNewRect(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TSquare;
    function CalcNewRotCenter(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TDot;
    function MoveRectByDelta(ARect: TSquare; ADelta: TDot): TSquare;
    procedure SetShadow(const Value: TBlockShadow);
    procedure SetGradient(const Value: TDiagramGradient);
    procedure SetBitmap(const Value: TBitmap);
    procedure DrawBitmap;
    procedure SetMetafile(const Value: TMetafile);
    procedure SetPicture(const Value: TPicture);
    procedure DrawMetafile;
    procedure DrawPicture;
    procedure BitmapChange(Sender: TObject);
    procedure MetafileChange(Sender: TObject);
    procedure PictureChange(Sender: TObject);
    procedure SetBitmapMode(const Value: TBitmapMode);
    procedure SetPictureMode(const Value: TPictureMode);
    procedure DoDrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);
    procedure SetRotateFont(const Value: boolean);
    procedure SetColor(const Value: TColor);
    procedure SetSelColor(const Value: TColor);
    procedure SetTransparent(const Value: boolean);
    function GetStrings: TStrings;
    function GetAlignment: TAlignment;
    function GetRotateFont: boolean;
    function GetVertAlign: TVertAlign;
    function GetWordWrap: boolean;
    procedure SetBitmapTransparent(const Value: boolean);
    function GetBitmapTransparent: boolean;
    procedure LoadCollapsedNodesProp(Reader: TReader);
    procedure StoreCollapsedNodesProp(Writer: TWriter);
    procedure SetRotationStep(const Value: integer);
    function CurrentColor: TColor;
    function CurrentBrush: TBrush;
    procedure InternalDrawGraphic(Canvas: TCanvas; ARect: TSquare;
      AGraphic: TGraphic; AAngle: double; AMode: TPictureMode);
    procedure InternalDrawGraphicDgr(ADrawer: TDgrBlockDrawer; ARect: TSquare;
      AGraphic: TGraphic; AAngle: double; AMode: TPictureMode; ATransparency: integer);
    procedure DrawGraphic(AGraphic: TGraphic; AMode: TPictureMode);

    procedure SetRotationCenter(const Value: TDot);
    function AngledSurroundRect(AAngle: double): TDotArray;
    function TranslateRotCenter(ARect: TSquare; AAngle: double; ARotCenter: TDot): TDot;
  protected

    /// Description:
    ///   Clips diagram block using Canvas/Drawer parameters given by AInfo record.
    procedure ClipBlock(AInfo: TDiagramDrawInfo);

    /// Description:
    ///   Unclibps diagram block clipped by ClipBlock method.
    procedure UnclipBlock(AInfo: TDiagramDrawInfo);

    /// Description:
    ///   Retrieves the current position of block's rotation center, in client coordinates.
    function DiagramRotCenter: TDot; virtual;

    /// Description:
    ///   Called when the pen properties are modified.
    procedure PenChanged; virtual;

    /// Description:
    ///   This method is called when the bounds of a control is changed from AOld to ANew.
    procedure BoundsChanged(AOld, ANew: TSquare); virtual;

    /// Description:
    ///   For internal use only.
    procedure StartCursorPlacement; override;

    /// Description:
    ///   For internal use only.
    procedure EndCursorPlacement; override;

    /// Description:
    ///   For internal use only.
    procedure InitDgrDrawer(ADrawer: TDgrBlockDrawer); override;

    /// Description:
    ///   For internal use only..
    procedure InitDrawer(ADrawer: TBlockDrawer); override;

    /// Description:
    ///   For internal use only.
    procedure DefineProperties(Filer: TFiler); override;

    /// Description:
    ///   Called whenever the block is resized.
    procedure Resize; virtual;

    /// Description:
    ///   Called whenever the mouse enters the block.
    procedure MouseEnter; virtual;

    /// Description:
    ///   Called whenever the mouse leaves the block.
    procedure MouseLeave; virtual;

    /// Description:
    ///   Returns the surround region of the block.
    function GetSurroundRgn: TDotArray;

    /// Description:
    ///   HandlesChanged method is called whenever the handles colllection changes.
    procedure HandlesChanged; override;

    /// Description:
    ///   AnchorLinkChanged method is called whenever a link point of the block (specified by ALinkPoint)
    ///   changes its anchor (the linkpoint which it is anchored to).
    procedure AnchorLinkChanged(ALinkPoint: TLinkPoint); override;

    /// Description:
    ///   For internal use.    
    procedure Loaded; override;

    /// Description:
    ///   GetTextCellRect returns a TSquare (in diagram coordinates) which indicates where the a text cell will
    ///   be drawn. In TCustomDiagramBlock class, this method returns the full block rectangle. You can override
    ///   this method in descendants if you want the text to be drawn somewhere else (only top half of block,
    ///   for example). In general, GetTextCellRect calculates the TRect based on the TextCell.BoundsRect property.
    function GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.LinkedAnchorChanged
    procedure LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange); override;

    /// Description:
    ///   AngleChanged method is called whenever the value of Angle property changes.
    procedure AngleChanged; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.PaintControl
    procedure PaintControl(AInfo: TDiagramDrawInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.MovePosition
    procedure MovePosition(ADeltaX, ADeltaY: number); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.ControlRect
    function ControlRect: TSquare; override;

    /// Description:
    ///   SurroundRgn method retrieves an array of TPoint that specifies the surround region of the block.
    ///   If you are creating new block classes, override this method to indicate what is the surround region
    ///   of the new block you are creating. Diagram architecture use this information to know when a specified
    ///   point (mouse click, for example) is inside or outside the block.
    function SurroundRgn: TDotArray; virtual;

    /// Description:
    ///   Override GetBlockPath method to provide the block shape to be used when using PathDrawingMode (non-GDI GraphicLib).
    ///   GetBlockPath must retrieve a path in relative coordinates (coordinates must be relative to ADrawer.OriginalRect).
    procedure GetBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer); {$IFDEF GDIPLUS} overload; {$ENDIF} virtual;

    /// Description:
    ///   Override GetTranslatedBlockPath method to provide the block shape to be used when using PathDrawingMode (non-GDI GraphicLib).
    ///   GetTranslatedBlockPath must retrieve the path in client coordinates, already rotated.
    procedure GetTranslatedBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer); {$IFDEF GDIPLUS} overload; {$ENDIF} virtual;

    /// Description:
    ///   Retrieves the path which surrounds the current block, when using PathDrawingMode (non-GDI GraphicLib).
    procedure GetSurroundBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer); {$IFDEF GDIPLUS} overload; {$ENDIF} virtual;

    {$IFDEF GDIPLUS}
    /// Description:
    ///   Override GetBlockPath method to provide the block shape to be used when using GDI+.
    ///   This overload is deprecated, use generic GetBlockPath method instead (APath: TDgrGraphicsPath).
    procedure GetBlockPath(APath: TGPGraphicsPath); overload; virtual; {$IFDEF DELPHI6_LVL} deprecated; {$ENDIF}

    /// Description:
    ///   Override GetTranslatedBlockPath method to prived the block shape to be used when using GDI+.
    ///   This overload is deprecated, use generic GetTranslatedBlockPath method instead (APath: TDgrGraphicsPath).
    procedure GetTranslatedBlockPath(APath: TGPGraphicsPath); overload; virtual; {$IFDEF DELPHI6_LVL} deprecated; {$ENDIF}

    /// Description:
    ///   Retrieves the path which surrounds the current block, when using GDI+.
    ///   This overload is deprecated, use generic GetSurroundBlockPath method instead (APath: TDgrGraphicsPath).
    procedure GetSurroundBlockPath(APath: TGPGraphicsPath); overload; virtual; {$IFDEF DELPHI6_LVL} deprecated; {$ENDIF}
    {$ENDIF}

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.SurroundRect
    function SurroundRect: TSquare; override;

    /// Description:
    ///   GetBlockClipRegion returns a Windows region object with the clipping region for the block. You will
    ///   seldom need to override this method when creating new block classes, since this method uses SurroundRgn
    ///   method internally to creating clipping region. So, by overriding SurroundRgn method will be enough in
    ///   most of cases.
    function GetBlockClipRegion: HRGN;

    /// Description:
    ///   Used internally by diagram to draw the cursor of block.
    procedure DrawBlockCursor(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo); virtual;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.StartMoving
    procedure StartMoving; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateMoving
    procedure UpdateMoving(AInfo: TControlMoveInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateInserting
    procedure UpdateInserting(AInfo: TControlInsertInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateDrawing
    procedure UpdateDrawing(AInfo: TControlDrawInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.StartMovingHandle
    procedure StartMovingHandle; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.HandleMoving
    procedure HandleMoving(AInfo: THandleMoveInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.GetHandlePoint
    function GetHandlePoint(AHandle: TStretchHandle): TDot; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.GetLinkPoint
    function GetLinkPoint(ALinkPoint: TLinkPoint): TDot; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.SelectedChanged
    procedure SelectedChanged; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.DiagramChanged
    procedure DiagramChanged(Old, New: TatDiagram); override;

    /// Description:
    ///   StyleChanged method is called whenever Brush or Pen of the block is changed.
    procedure StyleChanged(Sender: TObject);

    /// Description:
    ///   For internal use only.
    procedure DrawHandles(AInfo: TDiagramDrawInfo); override;

    {All DrawXXXX procedures receive Rect coordinates relative to Canvas}

    /// Description:
    ///   DrawShape method is called by TCustomDiagramBlock to draw block cursor, shadows and to draw the block
    ///   itself. This is the most top-level method to be overriden if you want to create new TCustomDiagramBlock
    ///   descendants with new shapes. Just override this method and implement the drawing of shape taking into
    ///   account the AInfo (information for drawing) and ABlockInfo (Rect and Angle) parameters. You can use
    ///   AInfo.Drawer property to make it easier to draw the diagram block.
    procedure DrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo); virtual;

    /// Description:
    ///   DrawText is called by TCustomDiagramBlock.PaintControl method to draw the text specified by String property.
    ///   The text is drawn in the region specified by GetTextRect method.
    procedure DrawText(AInfo: TDiagramDrawInfo);

    /// Description:
    ///   DrawBlock method is called by TCustomDiagramBlock to draw the block. This method draws the block
    ///   shape, shadow and background image, and it calls DrawShape method to do it. If you want to create new
    ///   TCustomDiagramBlock descendants, you can override DrawBlock method to implement your our custom
    ///   drawing of the block. Remember that you will have to implement all features (shadows, border,
    ///   bitmaps, etc.). If you just want to change shape of the block, you can override DrawShape method instead.
    procedure DrawBlock(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo); virtual;

    /// Description:
    ///   Not used in current version.
    function AcceptOverlap(ABlock: TCustomDiagramBlock): boolean; virtual;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.CreateGraphicLibObjects
    procedure CreateGraphicLibObjects; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.DestroyGraphicLibObjects
    procedure DestroyGraphicLibObjects; override;

    /// Description:
    ///    DiagramDrawBlockInfo returns a TDiagramDrawBlockInfo record with specified parameters assigned to its fields.
    class function DiagramDrawBlockInfo(ARect: TSquare; AAngle: double): TDiagramDrawBlockInfo;

    /// Description:
    ///   If ShapeCursor is True, the block cursor will be drawn by calling DrawShape method. Otherwise,
    ///   the block cursor will be a rectangle around the block area.
    property ShapeCursor: boolean read FShapeCursor write FShapeCursor;

    /// Description:
    ///   For internal use.
    property CursorAngle: double read FCursorAngle write FCursorAngle;

    /// Description:
    ///   For internal use.
    property CursorRect: TSquare read FCursorRect write FCursorRect;

    /// Description:
    ///   For internal use.
    property CursorRotCenter: TDot read FCursorRotCenter write FCursorRotCenter;
  public

    /// Description:
    ///   For internal use.
    constructor Create(AOwner: TComponent); override;

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.PointInControl
    function PointInControl(Pos: TPoint): boolean; override;

    /// Description:
    ///   UpdateBlockHandles recreates the default handles for the block: top left, top, top right, right,
    ///   bottom right, bottom, bottom left, left and rotation handles.
    procedure UpdateBlockHandles;

    /// Description:
    ///   Call DrawBlockInCanvas to draw the block in a specified Canvas, with the size specified by ARect, in
    ///   canvas coordinates.
    procedure DrawBlockInCanvas(Canvas: TCanvas; ARect: TRect);

    /// Description:
    ///   BoundsRect property contains information about the block position: top and left coordinates,
    ///   width and height of the block.
    property BoundsRect: TSquare read GetBoundsRect write SetBoundsRect;

    /// Description:
    ///   Specify the right-most coordinate of block, in diagram coordinates.
    property Right: number read GetRight;

    /// Description:
    ///   Specify the bottom-most coordinate of block, in diagram coordinates.
    property Bottom: number read GetBottom;

    /// Description:
    ///   Specified the Shape of the block.
    property Shape: TBlockShape read FShape write SetShape;

    /// Description:
    ///   Use Brush property to set the background color and fill style of block when it is not selected.
    property Brush: TBrush read FBrush write SetBrush;

    /// Description:
    ///   Use SelBrush propery to set the background color and fill style of block when it is selected.
    property SelBrush: TBrush read FSelBrush write SetSelBrush;

    /// Description:
    ///   Use Pen property to specify the pen used to draw the block surround shape.
    property Pen: TPen read FPen write SetPen;

    /// Description:
    ///   Color property contains the background color of the block when it is not selected.
    property Color: TColor read FColor write SetColor;

    /// Description:
    ///   SelColor property contains the background color of the block when it is selected.
    property SelColor: TColor read FSelColor write SetSelColor;

    /// Description:
    ///   Use Strings property to read/write the text displayed by the block.
    property Strings: TStrings read GetStrings write SetStrings stored false;

    /// Description:
    ///   Alignment property specified the horizontal alignment for the block text.
    property Alignment: TAlignment read GetAlignment write SetAlignment stored false;

    /// Description:
    ///   VertAlign property specifies the vertical alignment for the block text.
    property VertAlign: TVertAlign read GetVertAlign write SetVertAlign stored false;

    /// Description:
    ///   Use WordWrap property to specify if block text will be wrapped or not.
    property WordWrap: boolean read GetWordWrap write SetWordWrap stored false;

    /// Description:
    ///   If RotateFont property is True, the font of block will be rotated together with the block when
    ///   it is rotated. If RotateFont is False, the font will not be rotated: only the text position.
    ///   If the block font is not a True Type font, RotateFont has no effect and behave like its value is False.
    property RotateFont: boolean read GetRotateFont write SetRotateFont stored false;

    /// Description:
    ///   When ClipText property is True, the text drawn in the block is restricted to its shape
    ///   (more specifically, to the clipping region specified by GetBlockClipRegion method).
    ///   If ClipText is False, the text is not clipped.
    property ClipText: boolean read FClipText write SetClipText;

    /// Description:
    ///   MinWidth property specifies the minimum width allowed for the block. The user will not be able to
    ///   resize a block with a lower width than indicated by MinWidth property.
    property MinWidth: integer read FMinWidth write SetMinWidth;

    /// Description:
    ///   MinHeight property specifies the minimum height allowed for the block. The user will not be able to
    ///   resize a block with a lower height than indicated by MinWidth property.
    property MinHeight: integer read FMinHeight write SetMinHeight;

    /// Description:
    ///   Shadow property contains information about block shadow. By using TBlockShadow object, you can set
    ///   shadow visibility, position and color.
    property Shadow: TBlockShadow read FShadow write SetShadow;

    /// Description:
    ///   Gradient property contains information about block gradient. By using TDiagramGradient object, you can
    ///   set gradient visibility, style, colors, etc.
    property Gradient: TDiagramGradient read FGradient write SetGradient;

    /// Description:
    ///   Bitmap property contains the background bitmap of the block. If Bitmap property is empty, no bitmap is displayed.
    property Bitmap: TBitmap read FBitmap write SetBitmap;
    property BitmapMode: TBitmapMode read FBitmapMode write SetBitmapMode;

    /// Description:
    ///   Defines if the bitmap of the block must be drawn transparent.
    property BitmapTransparent: boolean read GetBitmapTransparent write SetBitmapTransparent;

    /// Description:
    ///   Metafile property contains the background metafile of the block. If Metafile property is empty,
    ///   no metafile is displayed.
    property Metafile: TMetafile read FMetafile write SetMetafile;

    /// Description:
    ///   Picture property contains the background picture of the block. If Picture property is empty,
    ///   no picture is displayed. This property is the same as Bitmap or Metafile property, and can be used
    ///   in place of the latter.
    property Picture: TPicture read FPicture write SetPicture;

    /// Description:
    ///   PictureMode property specified how the picture (specified by Picture property) will be displayed in the block.
    property PictureMode: TPictureMode read FPictureMode write SetPictureMode;

    /// Description:
    ///   If Transparent is True, properties Color and SelColor of block are ignored, and no background color
    ///   is drawn. Transparent property does not affect setting in Brush and SelBrush properties.
    property Transparent: boolean read FTransparent write SetTransparent;

    /// Description:
    ///   RotationStep contains the step, in degrees, used for block rotation while end-user is rotating the
    ///   block. Default step is 5. You can set higher steps (30, 45, 90 for example) to only allow specific
    ///   positions while rotating the block.
    property RotationStep: integer read FRotationStep write SetRotationStep;

    /// Description:
    ///   Contains the position of the rotation center of the block, in percentage (relative) coordinates. So, a
    ///   value of (50, 50) will put the rotation center in the middle of the block.    
    property RotationCenter: TDot read FRotCenter write SetRotationCenter;

    property Selected;
    property LinkPoints;
    property Drawer;
    property Angle;
    property Handles;
  published
    {Do not set Left, Top, Width or Height properties directly. They are read-write only for
     streaming. You can read these properties, but if you want to change them, use BoundsRect property}

    /// Description:
    ///   Specifies the left coordinate of block, in diagram coordinates.
    property Left: number read FLeft write SetLeft;

    /// Description:
    ///   Specifies the top coordinate of block, in diagram coordinates.
    property Top: number read FTop write SetTop;

    /// Description:
    ///   Use Width property to read or change width of the block.
    property Width: number read FWidth write SetWidth;

    /// Description:
    ///   Use Height property to read or change height of the block.
    property Height: number read FHeight write SetHeight;
  end;

  /// Description:
  ///   TDiagramBlock is a multi-purpose block for using in diagram. It is a direct descendant of
  ///   TCustomDiagramBlock. TDiagramBlock does not implement any other fuctionality over
  ///   TCustomDiagramBlockatDiagram.TCustomDiagramBlock, it only publishes and public some properties
  ///   and methods of its ancestor. 
  TDiagramBlock = class(TCustomDiagramBlock)
  public
    constructor Create(AOwner: TComponent); override;
  published
    property Angle;
    property Brush;
    property SelBrush;
    property Color;
    property SelColor;
    property Pen;
    property Shape;
    property LinkPoints;
    property Strings;
    property Alignment;
    property VertAlign;
    property ClipText;
    property WordWrap;
    property TextCells;
    property Restrictions;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.Font
    property Font;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.DiagramFont
    property DiagramFont;

    property MinWidth;
    property MinHeight;
    property Shadow;
    property ShowHint;
    property ParentShowHint;
    property Hint;
    property Gradient;
    property Bitmap;
    property BitmapMode;
    property BitmapTransparent;
    property Metafile;
    property Picture;
    property PictureMode;
    property Transparent;
    property Cursor;
    property StringData;
    property Visible;
    property Layer;
    property LayerName;
    property Transparency;
  end;

  /// Description:
  ///   TPolygonPoint class holds information about a polygon point in a TPolygonBlock object.
  ///   It descends from TDrawPoint class.  
  TPolygonPoint = class(TDrawPoint)
  private
    //FLinkPoint: TLinkPoint;
    FHandle: TStretchHandle;
  public

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;
    //property LinkPoint: TLinkPoint read FLinkPoint{ write FLinkPoint};

    /// Description:
    ///   Holds the TStretchHandle object related to the current polygon point.
    property Handle: TStretchHandle read FHandle{ write FHandle};
  end;

  /// Description:
  ///   TPolygonPoints is a TDrawPoints descendant which holds information about the point of the polygon object.
  TPolygonPoints = class(TDrawPoints)
  private
    FDestroyObjects: boolean;
    function GetItem(Index: integer): TPolygonPoint;
    procedure SetItem(Index: integer; const Value: TPolygonPoint);
    function FindByHandle(AHandle: TStretchHandle): TPolygonPoint;
    function PointArray: TDotArray;
  public

    /// Description:
    ///   Use Add function to add a new TPolygonPoint to the collection of polygon points.
    function Add(AX, AY: number): TPolygonPoint; overload;
    function Add: TPolygonPoint; overload;

    /// Description:
    ///   Use Items property to retrieve a specific TPolygonPoint object from the polygon points collection.
    property Items[Index: integer]: TPolygonPoint read GetItem write SetItem; default;
  end;

  /// Description:
  ///   TCustomPolygonBlock is base class for the TPolygonBlock which is a diagram block control which draws a
  ///   polygon. It differs from other blocks because it holds a collection of points which will define the
  ///   polygon to be drawn. After clicking in the polygon button in diagram toolbar, end-user can draw the
  ///   polygon by clicking in the diagram to define each polygon point, and double clicking the diagram to
  ///   finalize drawing the polygon.
  TCustomPolygonBlock = class(TCustomDiagramBlock)
  private
    FPolygon: TPolygonPoints;
    FTempPolygon: TPolygonPoints; //Only used to draw cursor while moving a handle
    FCursorPolygon: TPolygonPoints;
    FSavePolygon: TPolygonPoints;
    TempPolygonPoint: TDot;
    FPointColor: TColor;
    procedure PolygonChanged;
    procedure UpdatePolygonHandles;
    function BuildOriginalRect(APolygon: TPolygonPoints): TSquare;
    procedure UpdatePolygonLinkPoints;
    procedure LoadPolygonProp(Reader: TReader);
    procedure StorePolygonProp(Writer: TWriter);
    procedure RepositionPolygon(APolygon: TPolygonPoints; ANewRect: TSquare);
    procedure RotatePolygon(APolygon: TPolygonPoints; AAngle: double; ARotationBasePolygon: TPolygonPoints = nil);
    procedure DrawOutlineCursor(ACanvas: TCanvas; APolygon: TPolygonPoints);
    function UpdateRotationCenter(ANewRect: TSquare): TDot;
    procedure ChangeOriginalRect(ARect: TSquare);
    function CalcNewPolygonRotCenter(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TDot;
    procedure SetPointColor(const Value: TColor);
  protected
    function GetHandlePoint(AHandle: TStretchHandle): TDot; override;
    function DiagramRotCenter: TDot; override;
    procedure EndCursorPlacement; override;
    procedure StartCursorPlacement; override;
    procedure BoundsChanged(AOld, ANew: TSquare); override;

    /// Description:
    ///   For internal use only.
    procedure Loaded; override;

    /// Description:
    ///   For internal use only.
    procedure DefineProperties(Filer: TFiler); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.GetTextCellRect
    function GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.HandleMoving
    procedure HandleMoving(AInfo: THandleMoveInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateMoving
    procedure UpdateMoving(AInfo: TControlMoveInfo); override;

    /// Description:
    ///   atDiagram.TDiagramControl.UpdateDrawing
    procedure UpdateDrawing(AInfo: TControlDrawInfo); override;

    /// LinkedTo:
    ///   atDiagram.TCustomDiagramBlock.SurroundRgn
    function SurroundRgn: TDotArray; override;

    /// LinkedTo:
    ///   atDiagram.TCustomDiagramBlock.DrawShape
    procedure DrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo); override;

    /// Description:
    ///   Use CanvasPolygon function to get the points of the polygon in canvas coordinates. The APolygon
    ///   parameter receives the original points of the polygon.
    procedure CanvasPolygon(APolygon: TPolygonPoints);

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.DrawHandle
    procedure DrawHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle); override;

    /// LinkedTo:
    ///   atDiagram.TCustomDiagramBlock.GetTranslatedBlockPath
    procedure GetTranslatedBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer); override;
  public

    /// Description:
    ///   Creates the polygon block.
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    /// Description:
    ///   Polygon property holds the TPolygonPoints property which contains the points which defines the polygon
    ///   of the polygon block. Be aware that the points of the polygon are always non-absolute coordiantes,
    ///   relative to the point (0, 0). For example, these points:
    ///  (20, 0)
    ///  (40, 40)
    ///  (0, 40)
    ///  define a triangle, in relative coordinates. The triangle can be stretched or moved, but its polygon
    ///  points remain the same.
    property Polygon: TPolygonPoints read FPolygon;

    /// Description:
    ///   Use ChangePolygon to programatically change the polygon of the polygon block. See Polygon
    ///   property for more details.
    procedure ChangePolygon(APolygon: TPolygonPoints);
    property PointColor: TColor read FPointColor write SetPointColor default clLime;
  end;

  /// Description:
  ///   TPolygonBlock is a diagram block control which draws a polygon. It differs from other blocks because it
  ///   holds a collection of points which will define the polygon to be drawn. After clicking in the polygon
  ///   button in diagram toolbar, end-user can draw the polygon by clicking in the diagram to define each
  ///   polygon point, and double clicking the diagram to finalize drawing the polygon.
  TPolygonBlock = class(TCustomPolygonBlock)
  published
    property Angle;
    property Brush;
    property SelBrush;
    property Color;
    property SelColor;
    property Pen;
    property LinkPoints;
    property Strings;
    property Alignment;
    property VertAlign;
    property ClipText;
    property WordWrap;
    property TextCells;
    property Restrictions;
    property Font;
    property DiagramFont;
    property MinWidth;
    property MinHeight;
    property Shadow;
    property ShowHint;
    property ParentShowHint;
    property Hint;
    property Gradient;
    property Bitmap;
    property BitmapMode;
    property BitmapTransparent;
    property Metafile;
    property Picture;
    property PictureMode;
    property Transparent;
    property Cursor;
    property StringData;
    property Transparency;
    property PointColor;
    property LayerName;
  end;

  /// Description:
  ///   TDiagramLineJoin block is used as a connection point for lines. By dropping a TDiagramLineJoin block
  ///   in diagram, user can connect lines to it. TDiagramLineJoin provides the Anchor and AnchorIndex
  ///   properties, where you can anchor it to an existing block.
  TDiagramLineJoin = class(TCustomDiagramBlock)
  private
    FOldAnchorPos: TDot;
    function GetAnchor: TDiagramControl;
    function GetAnchorIndex: integer;
    procedure SetAnchor(const Value: TDiagramControl);
    procedure SetAnchorIndex(const Value: integer);
    function GetOrientation: TAnchorOrientation;
    procedure SetOrientation(const Value: TAnchorOrientation);
  public

    /// Description:
    ///   For internal use.
    constructor Create(AOwner: TComponent); override;

    /// LinkedTo:
    ///   atdiagram.TCustomDiagramBlock.AnchorLinkChanged
    procedure AnchorLinkChanged(ALinkPoint: TLinkPoint); override;

    /// LinkedTo:
    ///   atdiagram.TCustomDiagramBlock.LinkedAnchorChanged
    procedure LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange); override;
  published
    property Brush;
    property SelBrush;
    property Color;
    property SelColor;
    property Pen;

    /// Description:
    ///   Specifies the control where TDiagramLineJoin is attached to. AnchorIndex must always be specified.
    property Anchor: TDiagramControl read GetAnchor write SetAnchor;

    /// Description:
    ///   Specifies the link point index (in the link point collection of control specified by Anchor property)
    ///   where TDiagramLineJoin is attached to.
    property AnchorIndex: integer read GetAnchorIndex write SetAnchorIndex;

    /// Description:
    ///   Specifies the orientation of the link point. This orientation is used by the lines attached to the
    ///   link point (specially TDiagramSideLine), to know where the default direction the line should follow
    ///   from the link point. For example, linkpoints positioned at the left of control should have orientation
    ///   aoLeft, so when lines are attached to that link point, the lines would never go right.    
    property Orientation: TAnchorOrientation read GetOrientation write SetOrientation;
  end;

  /// Description:
  ///   TTextBlock is a diagram block for displaying text purpose only.
  TTextBlock = class(TCustomDiagramBlock)
  public

    /// Description:
    ///   For internal use.
    constructor Create(AOwner: TComponent); override;
  published
    property Angle;
    property Brush;
    property SelBrush;
    property Color;
    property SelColor;
    property Pen;
    property Shape;
    property LinkPoints;
    property Strings;
    property Alignment;
    property VertAlign;
    property ClipText;
    property WordWrap;
    property TextCells;
    property Restrictions;
    property Font;
    property DiagramFont;
    property MinWidth;
    property MinHeight;
    property Shadow;
    property ShowHint;
    property ParentShowHint;
    property Hint;
    property Gradient;
    property Bitmap;
    property BitmapMode;
    property BitmapTransparent;
    property Metafile;
    property Picture;
    property PictureMode;
    property Transparent;
    property Cursor;
    property StringData;
    property Visible;
    property Layer;
    property LayerName;
    property Transparency;
  end;

  /// Keywords:
  ///   asNone; asSolidArrow; asLineArrow; asDiamond; asRectangle; asEllipse; asHalfLine; asAngledDiamond
  TArrowShape = (asNone, asSolidArrow, asLineArrow, asDiamond, asRectangle, asEllipse, asHalfLine, asAngledDiamond);

  /// Description:
  ///   TLineArrow contains information about the arrow to be drawn at start or end point of a line.
  TLineArrow = class(TPersistent)
  private
    FShape: TArrowShape;
    FHeight: integer;
    FWidth: integer;
    FBrush: TBrush;
    //FPen: TPen;
    FDiagramLine: TCustomDiagramLine;
    FDrawFrom: TDot;
    FDrawTo: TDot;
    procedure SetShape(const Value: TArrowShape);
    procedure SetBrush(const Value: TBrush);
    procedure SetHeight(const Value: integer);
    //procedure SetPen(const Value: TPen);
    procedure SetWidth(const Value: integer);
  protected
    /// Description:
    ///   Changed method is called whenever a property of TLineArrow object is changed.
    procedure Changed;

    /// Description:
    ///   StyleChanged is called whenever a style property (Pen or Brush) is changed.
    procedure StyleChanged(Sender: TObject);

    /// Description:
    ///   For internal use.
    property DrawFrom: TDot read FDrawFrom write FDrawFrom;

    /// Description:
    ///   For internal use.
    property DrawTo: TDot read FDrawTo write FDrawTo;

    /// Description:
    ///   Draw method is called to draw the arrow in the diagram. The arrow is draw from the AFrom point to
    ///   the ATo point. The zoom ratio is given by AZoomRatio parameter.
    procedure Draw(AInfo: TDiagramDrawInfo; AArrowInfo: TDiagramDrawArrowInfo); virtual;

    class function DiagramDrawArrowInfo(AFromPoint, AToPoint: TDot; AZoomRatio: double): TDiagramDrawArrowInfo;
  public
    /// Description:
    ///   Creates the object.
    constructor Create(ADiagramLink: TCustomDiagramLine);

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Restore default settings for the arrow.
    procedure RestoreDefaults;

    /// Description:
    ///   Use DiagramLine to know to which line object the arrow belongs to.
    property DiagramLine: TCustomDiagramLine read FDiagramLine;
  published

    /// Description:
    ///   Specifies the shape of arrow (none, arrow, diamond, etc.).
    property Shape: TArrowShape read FShape write SetShape;

    /// Description:
    ///   Specifies the width of arrow shape.
    property Width: integer read FWidth write SetWidth;

    /// Description:
    ///   Specifies the height of arrow shape.
    property Height: integer read FHeight write SetHeight;

    /// Description:
    ///   Specifies the Brush of arrow shape.
    property Brush: TBrush read FBrush write SetBrush;
    //property Pen: TPen read FPen write SetPen;
  end;

  TCalcHandlesAction = (chNone, chInsert, chMove, chHandleMove, chAnchorChanged);

  /// Description:
  ///   TCalcHandlesInfo is used by TCustomDiagramLine descendants that override CalcNewHandles
  ///   method. It contains information about the line that should be calculated.
  TCalcHandlesInfo = record

    /// Description:
    ///   P1 contains the start point of the line, in diagram coordinates.
    P1: TDot;

    /// Description:
    ///   P2 contains the end point of the line, in diagram coordinates.
    P2: TDot;

    /// Description:
    ///   Anchor1 contains the TLinkPoint object where the start point of line is anchored to.
    ///   If there is no link point, Anchor1 is nil.
    Anchor1: TLinkPoint;

    /// Description:
    ///   Anchor2 contains the TLinkPoint object where the end point of line is anchored to.
    ///   If there is no link point, Anchor2 is nil.
    Anchor2: TLinkPoint;

    /// Description:
    ///   If a handle is being moved, HandleMoved contains the TStretchHandle object associated
    ///   with the handle being moved. Otherwise, HandleMoved contains nil.
    HandleMoved: TStretchHandle;

    /// Description:
    ///   If HandleMoved field is not nil, then DeltaMove contains the new position of handle moved,
    ///   related to its original position.
    DeltaMove: TDot;

    /// Description:
    ///   Indicates what action was performed in order to require the new calculation of the handles.
    Action: TCalcHandlesAction;
  end;

  TTextCellsMode = (tmSegments, tmSingle, tmManual, tmSpecific);

  TDiagramLineStyle = (
  lsLine, /// Straight line
  lsSideLine, /// Side (perpendicular segmented) line
  lsParabolicArc, /// Parabolic arc
  lsCircularArc, /// Circular arc
  lsBezier /// Bezier line
  );

  TLineArrowClass = class of TLineArrow;

  /// Summary:
  ///   Base class for all line objects.
  /// Description:
  ///   TCustomDiagramLine is the ancestor class of line objects. You should not create a TCustomDiagramLine
  ///   instance directly. For creating a new line class, descend it from TCustomDiagramLine. Key methods are
  ///   CalcNewHandles and RecalcPoints. Override those methods to implement your own line algorithm.
  TCustomDiagramLine = class(TDiagramControl)
  private
    FPoints: TDotArray;
    FPen: TPen;                          
    FSelPen: TPen;
    FSourceArrow: TLineArrow;
    FTargetArrow: TLineArrow;
    FTempHandles: TStretchHandles;
    FTextCellsMode: TTextCellsMode;
    FRequiresConnections: boolean;
    FCenterTextCell: TTextCell;
    FSourceTextCell: TTextCell;
    FTargetTextCell: TTextCell;
    FAltSourceTextCell: TTextCell;
    FAltTargetTextCell: TTextCell;
    FOrientation1: TAnchorOrientation;
    FOrientation2: TAnchorOrientation;
    FLineStyle: TDiagramLineStyle;
    FRecalcIntermediateArcHandles: boolean;
    function FixedSideLine: boolean;
    procedure SetOrientation1(const Value: TAnchorOrientation);
    procedure SetOrientation2(const Value: TAnchorOrientation);
    procedure AddFPoint(X, Y: number);
    procedure SetPen(const Value: TPen);
    procedure StyleChanged(Sender: TObject);
    procedure SetSourceArrow(const Value: TLineArrow);
    procedure SetTargetArrow(const Value: TLineArrow);
    procedure SetSelPen(const Value: TPen);
    function HandleByLinkPoint(ALinkPoint: TLinkPoint): TStretchHandle;
    procedure CheckHandles;
    procedure LoadHandlesProp(Reader: TReader);
    procedure LoadLinkPointsProp(Reader: TReader);
    procedure StoreHandlesProp(Writer: TWriter);
    procedure StoreLinkPointsProp(Writer: TWriter);
    function GetSourceLinkPoint: TLinkPoint;
    function GetTargetLinkPoint: TLinkPoint;
    procedure SetTextCellsMode(const Value: TTextCellsMode);
    {procedure SetStrings(const Value: TStrings);
    procedure StringsChange(Sender: TObject);}
    procedure DestroySpecificTextCells;
    procedure CreateSpecificTextCells;
    procedure PositionEndCell(ACell: TTextCell; AFrom, ATo: TDot; ANormal: boolean);
    procedure PositionCenterCell(ACell: TTextCell);
    procedure SetLineStyle(Value: TDiagramLineStyle);
    procedure RecalcBezierPoints(AHandles: TStretchHandles);

    procedure CalcLineNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
    procedure CalcSideLineNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
    procedure CalcArcNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
    procedure CalcBezierNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);

    procedure RecalcLinePoints(AHandles: TStretchHandles);
    procedure RecalcSideLinePoints(AHandles: TStretchHandles);
    procedure RecalcArcPoints(AHandles: TStretchHandles);
  protected
    function IsSideLine: boolean;
    procedure TextCellDestroyed(ACell: TTextCell); override;

    /// Description:
    ///   Invert orientation of arc lines.
    procedure InvertOrientation;

    /// Description:
    ///   Update the orientations of diagram side line start/end.    
    procedure UpdateOrientations;

    /// Description:
    ///   For internal use.
    procedure CheckLinkPoints;

    /// Description:
    ///   Called when a pen property is changed.
    procedure PenChanged; virtual;

    /// Description:
    ///   Called when a property of SelPen is changed.
    procedure SelPenChanged; virtual;
    procedure StartCursorPlacement; override;
    procedure EndCursorPlacement; override;

    /// Description:
    ///   DrawLine is a protected method that draws the line in the diagram canvas.
    procedure DrawLine(AInfo: TDiagramDrawInfo; ALineInfo: TDiagramDrawLineInfo); virtual;

    /// Description:
    ///   A line has handles and linkpoints in their start and end points. So, LinkPointByHandle returns the
    ///   TLinkPoint object associated with the TStretchHandle of the line.
    function LinkPointByHandle(AHandle: TStretchHandle): TLinkPoint; override;

    /// Description:
    ///   DrawLineCursor is called when the line must be drawn in diagram, but for cursor purposes
    ///  (when the line is dragged, for example).
    procedure DrawLineCursor(Canvas: TCanvas; AHandles: TStretchHandles);

    /// Description:
    ///   RecalcTextCells is called whenever the text cells must be recalculated. For example, for a side line,
    ///   whenever a line segment is added or removed, the RecalcTextCells is called and the text cells are
    ///   recalculated so that each line segment has its own text cell.
    procedure RecalcTextCells; virtual;

    /// Description:
    ///   HandlesChanged method is called whenever the handles colllection changes.    
    procedure HandlesChanged; override;

    /// Description:
    ///   For internal use.
    procedure Loaded; override;

    /// Description:
    ///   For internal use.
    procedure DefineProperties(Filer: TFiler); override;

    /// Description:
    ///   CalcNewHandles is called when the handles of line need to be recalculated. Override this method in
    ///   descendants to implement your own line algorithm.
    procedure CalcNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo); virtual;

    /// Description:
    ///   RecalcPoints method is called to fill Points property. The Points property is used for drawing the
    ///   line. RecalcPoints must calculate drawing points from the line handles. Override this method to
    ///   implement your own line algorithm.
    procedure RecalcPoints(AHandles: TStretchHandles); virtual;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.AnchorLinkChanged
    procedure AnchorLinkChanged(ALinkPoint: TLinkPoint); override;

    /// Description:
    ///   RecalcAnchors is called whenever a line anchor is changed. It is used to calculate new line coordinates
    ///   given the anchor changes.    
    procedure RecalcAnchors; virtual;

    /// Description:
    ///   RecalcHandles is called internally. It calls CalcNewHandles.
    procedure RecalcHandles; virtual;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.AnchorLinkChanged
    procedure LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.DiagramChanged
    procedure DiagramChanged(Old, New: TatDiagram); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.GetLinkPoint
    function GetLinkPoint(ALinkPoint: TLinkPoint): TDot; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.PaintControl
    procedure PaintControl(AInfo: TDiagramDrawInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.StartMovingHandle
    procedure StartMovingHandle; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.HandleMoving
    procedure HandleMoving(AInfo: THandleMoveInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.StartMoving
    procedure StartMoving; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateMoving
    procedure UpdateMoving(AInfo: TControlMoveInfo); override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.UpdateInserting
    procedure UpdateInserting(AInfo: TControlInsertInfo); override;

    /// LinkedTo:
    ///   atdiagram.TDiagramControl.UpdateDrawing
    procedure UpdateDrawing(AInfo: TControlDrawInfo); override;

    /// Description:
    ///   atDiagram.TDiagramControl.SelectedChanged    
    procedure SelectedChanged; override;

    /// LinkedTo:
    ///   atDiagram.TDiagramControl.MovePosition
    procedure MovePosition(ADeltaX, ADeltaY: number); override;

    /// Description:
    ///   CurPen contains the current TPen object used to draw the line. If the line is selected, CurPen
    ///   contains the TPen specified by SelPen property , otherwise it contains the TPen specified by Pen property.
    function CurPen: TPen;

    /// Description:
    ///   Override GetLineArrowClass method to specify a class for the line arrow. The default is TLineArrow,
    ///   but you can create a TLineArrow descendant and return that new class in GetLineArrowClass.
    function GetLineArrowClass: TLineArrowClass; virtual;

    class function DiagramDrawLineInfo(AHandles: TStretchHandles; ADrawArrows: boolean): TDiagramDrawLineInfo;

    /// Description:
    ///   Defines how text cells of the lines are automatically calculated.
    property TextCellsMode: TTextCellsMode read FTextCellsMode write SetTextCellsMode;

    /// Description:
    ///   Returns the primary text cell located at the source of line. Available when TextCellsMode is tmSpecific.
    property SourceTextCell: TTextCell read FSourceTextCell;

    /// Description:
    ///   Returns the primary text cell located at the target of line. Available when TextCellsMode is tmSpecific.
    property TargetTextCell: TTextCell read FTargetTextCell;

    /// Description:
    ///   Returns the secondary text cell located at the source of line. Available when TextCellsMode is tmSpecific.
    property AltSourceTextCell: TTextCell read FAltSourceTextCell;

    /// Description:
    ///   Returns the secondary text cell located at the target of line. Available when TextCellsMode is tmSpecific.
    property AltTargetTextCell: TTextCell read FAltTargetTextCell;

    /// Description:
    ///   Returns the text cell located at the middle of line. Available when TextCellsMode is tmSpecific.
    property CenterTextCell: TTextCell read FCenterTextCell;

    /// Description:
    ///   Defines the orientation of line source. Used for side lines only.
    property Orientation1: TAnchorOrientation read FOrientation1 write SetOrientation1;

    /// Description:
    ///   Defines the orientation of line target. Used for side lines only.    
    property Orientation2: TAnchorOrientation read FOrientation2 write SetOrientation2;
  public

    /// Description:
    ///   ControlRect contains the smalles TRect (in diagram coordinates) that holds all line points.
    function ControlRect: TSquare; override;

    /// Description:
    ///   SurroundRect contains the smalles TRect (in diagram coordinates) that holds all line points.
    function SurroundRect: TSquare; override;

    /// Description:
    ///   atDiagram.TDiagramControl.PointInControl    
    function PointInControl(Pos: TPoint): boolean; override;

    /// Description:
    ///   For internal use.
    constructor Create(AOwner: TComponent); override;

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// Description:
    ///   Use LineStyle to define which line will be drawn. You can switch from one line to another.
    ///   Default is lsLine.
    property LineStyle: TDiagramLineStyle read FLineStyle write SetLineStyle;
    property Handles;

    /// Description:
    ///   Points property contains information about the line points. Points property is used to draw the whole
    ///   line: a line segment is drawn from first point to the second, then from the second to third, etc.    
    property Points: TDotArray read FPoints write FPoints;

    /// Description:
    ///   TargetLinkPoint contains information about the link point associated with the end point of the line.
    ///   Use TargetLinkPoint to read or specify an anchor for the line end point.    
    property TargetLinkPoint: TLinkPoint read GetTargetLinkPoint;

    /// Description:
    ///   SourceLinkPoint contains information about the link point associated with the start point of the line.
    ///   Use SourceLinkPoint to read or specify an anchor for the line start point.
    property SourceLinkPoint: TLinkPoint read GetSourceLinkPoint;
    property Selected;

    /// Description:
    ///   Pen property specified the TPen object to be used to draw the line when the line is not selected.
    property Pen: TPen read FPen write SetPen;

    /// Description:
    ///   SelPen property specified the TPen object to be used to draw the line when the line is selected.
    property SelPen: TPen read FSelPen write SetSelPen;

    /// Description:
    ///   SourceArrow property specifies the shape for the start point of line.
    property SourceArrow: TLineArrow read FSourceArrow write SetSourceArrow;

    /// Description:
    ///   TargetArrow property specifies the shape for the end point of line.
    property TargetArrow: TLineArrow read FTargetArrow write SetTargetArrow;

    /// Description:
    ///   When RequiresConnections is true, the diagram will require the line to be connected to a block.
    ///   Both start and end points must be connected. The end-user will not able to insert, move or resize a
    ///   line unless its start and end points are connected to a block.    
    property RequiresConnections: Boolean read FRequiresConnections write FRequiresConnections;
  end;

  TCustomDiagramLineClass = class of TCustomDiagramLine;

  TBaseDiagramLine = class(TCustomDiagramLine)
  public
    property Selected;
  published
    property Pen;
    property SelPen;
    property SourceArrow;
    property TargetArrow;
    property Cursor;
    property TextCells;
    property Font;
    property StringData;
    property Visible;
    property Layer;
    property LayerName;
    property Transparency;
    property LineStyle;
    property Orientation1 stored IsSideLine;
    property Orientation2 stored IsSideLine;
  end;

  /// Description:
  ///   TDiagramLine is a line control that links two points directly with a single line segment.
  TDiagramLine = class(TBaseDiagramLine)
  end;

  TDiagramPolyLine = class(TDiagramLine)
  private
  protected
    procedure CalcNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo); override;
    procedure UpdateDrawing(AInfo: TControlDrawInfo); override;
  public
    constructor Create(AOwner: TComponent); override;
  end;

  /// Description:
  ///   TDiagramSideLine is a line control that links two points with orthogonal line segments.
  TDiagramSideLine = class(TBaseDiagramLine)
  public
    constructor Create(AOwner: TComponent); override;
  end;

  /// Keywords:
  ///   atCircular; atParabolic
  TArcType = (atParabolic, atCircular);

  /// Description:
  ///   TDiagramArc is a diagram control for drawing arcs. It descends from TCustomDiagramLine class, so it can
  ///   also be connected to blocks.
  TDiagramArc = class(TBaseDiagramLine)
  private
    FArcType: TArcType;
    procedure SetArcType(const Value: TArcType);
  public

    /// Description:
    ///   Creates the object.
    constructor Create(AOwner: TComponent); override;
  published

    /// Description:
    ///   Defines the type of arc which will be drawn.
    property ArcType: TArcType read FArcType write SetArcType;
  end;

  /// Description:
  ///   TDiagramBezier is a diagram control for drawing bezier lines. It descends from TCustomDiagramLine class,
  ///   so it can also be connected to blocks.  
  TDiagramBezier = class(TBaseDiagramLine)
  public

    /// Description:
    ///   Creates the object.  
    constructor Create(AOwner: TComponent); override;
  end;

  TDiagramPolyBezier = class(TDiagramPolyLine)
  private
  protected
    procedure UpdateDrawing(AInfo: TControlDrawInfo); override;
    procedure CalcNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo); override;
    procedure RecalcPoints(AHandles: TStretchHandles); override;
  public
    constructor Create(AOwner: TComponent); override;
  end;

(*  TResizeState = (msResizeLeftTop, msResizeTop, msResizeRightTop,
    msResizeLeft, msResizeRight, msResizeLeftBottom, msResizeBottom,
    msResizeRightBottom);*)

  TGridStyle = (gsDots, gsLines, gsRuler);

  /// Description:
  ///   Holds information about snap grid of diagram.
  TSnapGrid = class(TPersistent)
  private
    FForce: boolean;
    FActive: boolean;
    FVisible: boolean;
    FSizeX: double;
    FSizeY: double;
    FDiagram: TatDiagram;
    FPen: TPen;
    FStyle: TGridStyle;
    FSnapToRuler: boolean;
    procedure SetActive(const Value: boolean);
    procedure SetSizeX(const Value: double);
    procedure SetSizeY(const Value: double);
    procedure SetVisible(const Value: boolean);
    procedure SetPen(const Value: TPen);
    procedure StyleChanged(Sender: TObject);
    procedure SetStyle(const Value: TGridStyle);
    procedure SetSnapToRuler(const Value: boolean);
  protected

    /// Description:
    ///   Changed method is called whenever a property of the object changes.
    procedure Changed;

    /// Description:
    ///   Draw method is called for drawing the snap grid.
    procedure Draw(Canvas: TCanvas);

    /// Description:
    ///   UpdateSize method is called whenever the snap grid needs to recalculate the SizeX and SizeY properties.
    procedure UpdateSize;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADiagram: TatDiagram);

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;
  published

    /// Description:
    ///   If Active is True, movements done in diagram follow the snap grid. If active is false, snap grid has
    ///   no effect in the actions performed by the user.
    property Active: boolean read FActive write SetActive;

    /// Description:
    ///   When Force property is true, the object being inserted in the diagram is automatically snapped to grid.
    ///   When Force is false, the object can be placed everywhere.
    property Force: Boolean read FForce write FForce;

    /// Description:
    ///   Set Visible to true to make snap grid visible. Visible is false by default.
    property Visible: boolean read FVisible write SetVisible;

    /// Description:
    ///   Specifies the horizontal size of snap grid step in diagram coordinates.
    property SizeX: double read FSizeX write SetSizeX;

    /// Description:
    ///   Specifies the vertical size of snap grid step in diagram coordinates.
    property SizeY: double read FSizeY write SetSizeY;

    /// Description:
    ///   Specify the visual style of snap grid.
    property Style: TGridStyle read FStyle write SetStyle;

    /// Description:
    ///   When snap grid Style property is set to draw lines, the lines will be drawn using the pen specified
    ///   by Pen property.
    property Pen: TPen read FPen write SetPen;

    /// Description:
    ///   When SnapToRuler is true, SizeX and SizeY properties are ignored, and the snap grid follow the ruler
    ///   subdivisions (ticks).
    property SnapToRuler: boolean read FSnapToRuler write SetSnapToRuler;
  end;
                                            
  TBackImageStyle = (biTile, biFixed);

  /// Description:
  ///   Holds information about diagram background image.
  TDiagramBackground = class(TPersistent)
  private
    FPicture: TPicture;
    FDiagram: TatDiagram;
    FBitmap: TBitmap;
    FStyle: TBackImageStyle;
    FScroll: boolean;
    FVisible: boolean;
    FGradient: TDiagramGradient;
    FPrintGradient: boolean;
    procedure SetBitmap(const Value: TBitmap);
    procedure BitmapChange(Sender: TObject);
    procedure PictureChange(Sender: TObject);
    procedure SetStyle(const Value: TBackImageStyle);
    procedure Changed;
    procedure SetScroll(const Value: boolean);
    procedure SetVisible(const Value: boolean);
    procedure SetGradient(const Value: TDiagramGradient);
    procedure SetPicture(const Value: TPicture);
  public

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   For internal use.
    constructor Create(ADiagram: TatDiagram);

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// Description:
    ///   Draws the background of diagram.
    procedure Draw(Canvas: TCanvas);
  published

    /// Description:
    ///   Holds the bitmap to be drawn in diagram background.
    property Bitmap: TBitmap read FBitmap write SetBitmap;

    /// Description:
    ///   Holds the picture to be drawn in diagram background.
    property Picture: TPicture read FPicture write SetPicture;

    /// Description:
    ///   If Scroll is True, the bitmap scrolls together with diagram. If Scroll is False, the bitmap stays
    ///   fixed in background, regardless the scrolling of diagram.
    property Scroll: boolean read FScroll write SetScroll;

    /// Description:
    ///   Specifies how the bitmap is positioned/drawn in diagram background.
    property Style: TBackImageStyle read FStyle write SetStyle;

    /// Description:
    ///   If Visible is true, background bitmap is displayed. Visible is false by default (no bitmap is displayed).
    property Visible: boolean read FVisible write SetVisible;

    /// Description:
    ///   Use Gradient property to specify a gradient for the diagram background.
    property Gradient: TDiagramGradient read FGradient write SetGradient;

    /// Description:
    ///   Set PrintGradient to true if the gradient background must be printed when the diagram is printed.
    ///   If PrintGradient is false, the gradient is only visible in the screen.
    property PrintGradient: boolean read FPrintGradient write FPrintGradient;
  end;

  TRulerUnits = (unMili, unCenti, unInch, unPixel);
  TRulerPosition = (rpTop, rpLeft);

  /// Description:
  ///   Ancestor class for TDiagramRuler. It holds information about the diagram ruler.
  TCustomRuler = class(TPersistent)
  private
    FZoom: integer;
    FUnits: TRulerUnits;
    FFont: TFont;
    FFont0: HFONT;
    FFont90: HFONT;
    FOffset: integer;
    FMajorTickLength: integer;
    FMinorTickLength: integer;
    FPosition: TRulerPosition;
    FDivisions: integer;
    FColor: TColor;
    FSize: integer;
    FTickColor: TColor;
    FAutoFactor: boolean;
    FGridColor: TColor;
    procedure SetUnits(const Value: TRulerUnits);
    procedure SetZoom(const Value: integer);
    procedure SetFont(const Value: TFont);
    procedure DeleteFonts;
    procedure UpdateFonts;
    procedure SetMajorTickLength(const Value: integer);
    procedure SetMinorTickLength(const Value: integer);
    procedure SetPosition(const Value: TRulerPosition);
    procedure SetDivisions(const Value: integer);
    procedure UpdateDivisions;
    procedure SetColor(const Value: TColor);

    /// Description:
    ///   FontChanged method is called when the font of block is changed.
    procedure FontChanged(Sender: TObject);
    procedure SetSize(const Value: integer);
    procedure SetTickColor(const Value: TColor);
    procedure GetStepAndFactor(var AStep, AFactor: double);
    function RulerStep: double;
    procedure SetAutoFactor(const Value: boolean);
    procedure SetGridColor(const Value: TColor);

    /// Description:
    ///   Specifies the color of the grid.
    property GridColor: TColor read FGridColor write SetGridColor;
  protected
    function GetUnits: TRulerUnits; virtual;

    /// Description:
    ///    Changed method is called when the ruler settings (properties) are changed.
    procedure Changed; virtual;

    /// Description:
    ///   For internal use only.
    procedure UpdateSize(ASize: integer); virtual;

    /// Description:
    ///   Returns the used ScreenDPI for the ruler.
    function ScreenDPI: integer; virtual;

    /// Description:
    ///   Use Divisions property to specify how many divisions will be drawn between one ruler value and
    ///   another. The Divisions property affects the number of minor ticks drawn in ruler.
    property Divisions: integer read FDivisions write SetDivisions;

    /// Description:
    ///   Specifies the Font used to draw ruler numbers.
    property Font: TFont read FFont write SetFont;

    /// Description:
    ///   Specifies the Zoom of ruler. This property is changed automatically by diagram, when diagram
    ///   zoom is changed.
    property Zoom: integer read FZoom write SetZoom;

    /// Description:
    ///   Specify the units of ruler.
    property Units: TRulerUnits read GetUnits write SetUnits;

    /// Description:
    ///   Specifies the length of minor tick.
    property MinorTickLength: integer read FMinorTickLength write SetMinorTickLength;

    /// Description:
    ///   Specifies the length of major tick.
    property MajorTickLength: integer read FMajorTickLength write SetMajorTickLength;

    /// Description:
    ///   Specifies if ruler is top aligned (horizontal) or left aligned (vertical).
    property Position: TRulerPosition read FPosition write SetPosition;

    /// Description:
    ///   Specifies the background color of ruler.
    property Color: TColor read FColor write SetColor;

    /// Description:
    ///   Specifies the color of ticks.
    property TickColor: TColor read FTickColor write SetTickColor;

    /// Description:
    ///   Specifies the height (for top ruler) or width (for left ruler) of ruler. Size has a minimum value, depending
    ///   on the font choosen.
    property Size: integer read FSize write SetSize;

    /// Description:
    ///   Specifies the ruler offset in pixels. If Offset is positive, the ruler will be moved left (starting at
    ///   a point higher than the zero).
    property Offset: integer read FOffset write FOffset;

    /// Description:
    ///   When the diagram is zoomed in or out, the ruler automatically changes its step. For example, when the
    ///   zoom is 100%, the step is 1 centimeter (when Units property is set to unCenti). If the zoom increases,
    ///   the diagram can automatically change the step to 0.5 centimeter or even less.
    ///   This happens when AutoFactor property is true. You can disable this behaviour by setting AutoFactor
    ///   to false. In this case, the step and divisions remain the same, regardless of the zoom factor.
    property AutoFactor: boolean read FAutoFactor write SetAutoFactor;
  public
    /// Description:
    ///   For internal use.
    constructor Create;

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// Description:
    ///   Draws the ruler.
    procedure Draw(Canvas: TCanvas; ARect: TRect; TempOffset: integer = 0; MaxSize: integer = 0; ADrawGrid: boolean = false);
  end;

  /// Description:
  ///   Holds information about the ruler.
  TDiagramRuler = class(TCustomRuler)
  private
    FDiagram: TatDiagram;
    FVisible: boolean;
    procedure SetVisible(const Value: boolean);
  protected

    function GetUnits: TRulerUnits; override;

    /// Description:
    ///   Changes the value of Size property, it's also called by SetSize procedure.
    procedure UpdateSize(ASize: integer); override;

    /// Description:
    ///   Changed method is called whenever a property of the object changes.
    procedure Changed; override;

    /// Description:
    ///   ScreenDPI returns the used screen dpi.
    function ScreenDPI: integer; override;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADiagram: TatDiagram);

    /// Description:
    ///   Copies all properties from another TDiagramRuler object.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   VisibleSize contains the same value of Size property, except when the ruler is not visible.
    ///   In this case, VisibleSize contains zero.
    function VisibleSize: integer;
  published

    /// Description:
    ///   Specifies if ruler is visible or not.
    property Visible: boolean read FVisible write SetVisible;
    property Divisions;
    property Font;
    property Units;
    property MinorTickLength;
    property MajorTickLength;
    property Color;
    property TickColor;
    property Size;
    property AutoFactor;
    property GridColor;
  end;

  {From VCL}
  /// Description:
  ///   TDiagramScrollBar contains information about the scroll bars used in diagram. This class is used
  ///   internally, and contains the same information as VCL's TControlScrollBar.
  TDiagramScrollBar = class(TPersistent)
  private
    FDiagram: TatDiagram;
    FIncrement: TScrollBarInc;
    FPageIncrement: TScrollbarInc;
    FPosition: Integer;
    FRange: Integer;
    FCalcRange: Integer;
    FKind: TScrollBarKind;
    FMargin: Word;
    FVisible: Boolean;
    FTracking: Boolean;
    FSmooth: Boolean;
    FDelay: Integer;
    FColor: TColor;
    FParentColor: Boolean;
    FSize: Integer;
    FStyle: TScrollBarStyle;
    FThumbSize: Integer;
    FPageDiv: Integer;
    FLineDiv: Integer;
    FUpdateNeeded: Boolean;
    constructor Create(ADiagram: TatDiagram; AKind: TScrollBarKind);
//    procedure CalcAutoRange;
    function ControlSize(ControlSB, AssumeSB: Boolean): Integer;
    procedure DoSetRange(Value: Integer);
    function GetScrollPos: Integer;
    function NeedsScrollBarVisible: Boolean;
    function IsIncrementStored: Boolean;
    procedure ScrollMessage(var Msg: TWMScroll);
//    procedure SetButtonSize(Value: Integer);
    procedure SetColor(Value: TColor);
    procedure SetParentColor(Value: Boolean);
    procedure SetPosition(Value: Integer);
    procedure SetRange(Value: Integer);
    procedure SetSize(Value: Integer);
    procedure SetStyle(Value: TScrollBarStyle);
    procedure SetThumbSize(Value: Integer);
    procedure SetVisible(Value: Boolean);
//    function IsRangeStored: Boolean;
    procedure Update(ControlSB, AssumeSB: Boolean);
    function IsRangeStored: Boolean;
  public

    /// Description:
    ///   Copies all properties from another object.
    procedure Assign(Source: TPersistent); override;
    procedure ChangeBiDiPosition;

    /// Description:
    ///   Kind indicates whether the scroll bar has a vertical or horizontal orientation. This value is
    ///   initialized in the constructor, when the scroll bar is created by a scrolling windowed control for
    ///   its VertScrollBar or HorzScrollBar property.
    property Kind: TScrollBarKind read FKind;

    /// Description:
    ///   Indicates if the scroll bar is visible in the diagram.
    function IsScrollBarVisible: Boolean;

    /// Description:
    ///   The read-only ScrollPos property returns the current value of Position when the scroll bar is visible.
    ///   If the scroll bar is not visible, ScrollPos returns 0.
    property ScrollPos: Integer read GetScrollPos;
  published

    /// Description:
    ///   Use Color to specify the scroll bar�s color. When you explicitly assign a value to Color, the
    ///   ParentColor property is automatically set to false. If the scroll bar�s ParentColor property is
    ///   true, then the Color property reflects the Windows button highlight color (clBtnHighlight), and
    ///   changing this color in the Windows control panel automatically updates the value of Color.
    property Color: TColor read FColor write SetColor default clBtnHighlight;

    /// Description:
    ///   Use Increment to control how the scroll bar affects the scrollable region of the diagram.
    ///   If Increment is set to 1, each click scrolls by one position; if Increment is set to 2, each click
    ///   scrolls by two positions; and so forth.
    ///   Note: Do not use the Increment property when Smooth is set to true. When Smooth is true, every time
    ///   the range or visibility of the scroll bar changes, the value of Increment is dynamically recomputed.
    property Increment: TScrollBarInc read FIncrement write FIncrement stored IsIncrementStored default 8;

    /// Description:
    ///   The Margin property determines the minimum number of pixels that must separate each control from the
    ///   edge of the diagram. At runtime, whenever a child control is less than Margin pixels from the edge
    ///   and Visible is set to true, a scroll bar appears.
    property Margin: Word read FMargin write FMargin default 0;

    /// Description:
    ///   Use ParentColor to indicate that the scroll bar should always reflect the Windows button highlight
    ///   color. Using ParentColor differs from setting the Color property to clBtnHighlight in that when
    ///   ParentColor is true, changing the button highlight color in the Windows control panel automatically
    ///   updates the value of Color. When the value of the Color property is changed, its ParentColor
    ///   property is automatically set to false.
    property ParentColor: Boolean read FParentColor write SetParentColor default True;

    /// Description:
    ///   Use Position to programmatically scroll the client area of the diagram. The value of Position changes
    ///    (from 0 to Range) at runtime as the user scrolls the scroll bar.
    property Position: Integer read FPosition write SetPosition default 0;

    /// Description:
    ///   Range represents the virtual size (in pixels) of the diagram�s client area. If the Range of a
    ///   horizontal scroll bar is less than the width of the diagram, no horizontal scroll bar appears.
    ///   If the Range of a vertical scroll bar is less than the height of the diagram, no vertical scroll
    ///   bar appears. The value of Range is set automatically, but can be overridden in the Object Inspector
    ///   or in program code. If AutoScroll property of diagram is set to false, Range property never changes.
    property Range: Integer read FRange write SetRange stored IsRangeStored default 0;

    /// Description:
    ///   Set Smooth to true to specify that the scroll bar should dynamically adjust its scrolling increment
    ///   based on the size of the diagram. When Smooth is true, clicking the end arrows of the scroll bar
    ///   scrolls the diagram by approximately one tenth of its client area, and clicking the scroll bar beside
    ///   the thumb tab scrolls the diagram by the length of the client area minus the amount scrolled by the
    ///   end arrows. When Smooth is false, the Increment property controls the amount of scrolling when the
    ///   user clicks the end arrows, and clicking beside the thumb tab scrolls the diagram by the entire
    ///   length of the client area.
    property Smooth: Boolean read FSmooth write FSmooth default False;

    /// Description:
    ///   Use Size to specify the width of the scroll bar in pixels. Note:The length of the scroll bar is
    ///   determined by the width (for horizontal scroll bars) or height (for vertical scroll bars) of the
    ///   associated scrolling windowed control.
    property Size: Integer read FSize write SetSize default 0;

    /// Description:
    ///    Use Style to specify the scroll bar�s style. The style determines whether the scroll bar appears
    ///   three-dimensional, flat, or uses hot tracking.
    property Style: TScrollBarStyle read FStyle write SetStyle default ssRegular;

    /// Description:
    ///   Use ThumbSize to specify the length of the scroll bar�s thumb tab.
    property ThumbSize: Integer read FThumbSize write SetThumbSize default 0;

    /// Description:
    ///   Use Tracking to control whether the scroll bar and diagram repaint while the thumb tab is dragged
    ///   with the mouse.  If Tracking is true, the image moves as the user drags the thumb tab. If Tracking is
    ///   false, the image remains static until the thumb tab is released. Repainting while the thumb tab is
    ///   dragged can provide useful feedback to the user about how far the diagram has scrolled, but slows
    ///   performance.
    property Tracking: Boolean read FTracking write FTracking default False;

    /// Description:
    ///   If Visible is set to true, a scroll bar appears whenever a control is less than Margin pixels from
    ///   the edge of the diagram. If Visible is set to false, the scroll bar is never visible.
    property Visible: Boolean read FVisible write SetVisible default True;
  end;

  /// Description:
  ///   TPageLines class defines how the page lines will be displayed in the diagram. Page lines are visual
  ///   lines that indicates the printed page in the diagram area.
  TPageLines = class(TPersistent)
  private
    FVisible: boolean;
    FDiagram: TatDiagram;
    FPen: TPen;
    procedure SetVisible(const Value: boolean);
    procedure SetPen(const Value: TPen);
    procedure StyleChanged(Sender: TObject);
  protected

    /// Description:
    ///   Changed method is called whenever a property is changed, and it makes the page lines to be redrawn.
    procedure Changed;

    /// Description:
    ///   Draw method is called to draw the page lines in diagram area.
    procedure Draw(Canvas: TCanvas);
  public

    /// Description:
    ///   Creates the instance.
    constructor Create(ADiagram: TatDiagram);

    /// Description:
    ///   Called when the instance is destroyed.
    destructor Destroy; override;

    /// Description:
    ///   Use Assign method to copy the properties from one TPageLines object to another.
    procedure Assign(Source: TPersistent); override;
  published

    /// Description:
    ///   Use Visible to set the page lines visible or invisible.
    property Visible: boolean read FVisible write SetVisible;

    /// Description:
    ///   The Pen property specifies the pen object used to draw the page lines.
    property Pen: TPen read FPen write SetPen;
  end;

  /// Summary:
  ///   TDControListItem holds information about a item belonging to a diagram control list.
  TDControlListItem = class(TCollectionItem)
  private
    FDControl: TDiagramControl;
    procedure SetDControl(const Value: TDiagramControl);
    procedure MemberRemoved(ADControl: TDiagramControl);
    procedure MemberIncluded(ADControl: TDiagramControl);
  public
    destructor Destroy; override;

    /// Summary:
    ///   Assign an item to another.
    procedure Assign(Source: TPersistent); override;
  published

    /// Summary:
    ///   The DControl property specified the diagram control belonging to the list.
    property DControl: TDiagramControl read FDControl write SetDControl;
  end;

  /// Summary:
  ///   TDControlList class is a collection which holds a list of diagram controls.
  TDControlList = class(TOwnedCollection)
  private
    function GetItems(Index: Integer): TDControlListItem;
    procedure SetItems(Index: Integer; AItem: TDControlListItem);
    function FindByDControl(ADControl: TDiagramControl): TDControlListItem;
  protected

    /// Summary:
    ///   MemberIncluded method is called whenever a new diagram control is added to the list.
    procedure MemberIncluded(ADControl: TDiagramControl); virtual;

    /// Summary:
    ///   MemberRemoved method is called whenever a new diagram control is removed from the list.
    procedure MemberRemoved(ADControl: TDiagramControl); virtual;
  public

    /// Summary:
    ///   Adds a new diagram control in the list.
    function Add(ADControl: TDiagramControl): TDControlListItem;

    /// Summary:
    ///   Remove the diagram control specified by ADControl from the list.
    procedure Remove(ADControl: TDiagramControl);

    /// Summary:
    ///   Retrieve the list item specified by Index.    
    property Items[Index: Integer]: TDControlListItem read GetItems write SetItems; default;
  end;

  /// Summary:
  ///   TGroupMemberList is a diagram control list class used to hold the list of diagram controls
  ///   which belong to a group.
  TGroupMemberList = class(TDControlList)
  private
    FGroup: TBaseGroupBlock;
  protected
    procedure MemberIncluded(ADControl: TDiagramControl); override;
    procedure MemberRemoved(ADControl: TDiagramControl); override;
  end;

  /// Description:
  ///   Generic-purpose function type used for callbacks. The diagram control is passed in the ADControl parameter.
  TGroupMemberProc = procedure(ADControl: TDiagramControl);

  /// Description:
  ///   Common class for group blocks. Used for group block (TGroupBlock) and selection block (TSelectionBlock).
  TBaseGroupBlock = class(TCustomDiagramBlock)
  private
    FMembers: TGroupMemberList;

    {methods for calculating group members position}
    function ResizeMemberRect(ABlock: TCustomDiagramBlock;
      ABlockRect: TSquare; AOldGroup, ANewGroup: TSquare): TSquare;
    function RotateMemberRect(ARect: TSquare; AAngle: double; AGroupRect: TSquare): TSquare;
    procedure RotateMemberLine(ALine: TCustomDiagramLine;
      AHandles: TStretchHandles; AAngle: double; AGroupRect: TSquare);
    procedure ResizeMemberLine(ALine: TCustomDiagramLine; AHandles: TStretchHandles;
      AOld, ANew: TSquare);

    {methods for updating all group members according to new group position/angle.
    Warning: before calling RotateMembers/ResizeMembers in temporary, you must be sure that
    all members of the group have their FCursorRect and FCursorAngle preset}
    procedure RotateMembers(ADeltaAngle: double; ATemporary: boolean);
    procedure ResizeMembers(AOld, ANew: TSquare; ATemporary: boolean);

    procedure NotifyAnchoredMembers;
    procedure IterateAllMembers(AProc: TGroupMemberProc);
    procedure SetMembers(const Value: TGroupMemberList);
    procedure MoveLineHandle(ALine: TCustomDiagramLine;
      AHandles: TStretchHandles; AHandle: TStretchHandle;
      ADeltaMove: TDot);

    procedure DestroyMembers;
  protected
    procedure DiagramChanged(Old, New: TatDiagram); override;
    procedure DrawBlockCursor(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo); override;
    procedure MemberIncluded(AMember: TDiagramControl); virtual;
    procedure MemberRemoved(AMember: TDiagramControl); virtual;

    /// Description:
    ///   For internal use.
    procedure UpdateGroupMembers; virtual;
    procedure AngleChangedEx(AOld, ANew: double); override;
    procedure BoundsChanged(AOld, ANew: TSquare); override;
    function SurroundRgn: TDotArray; override;
    procedure HandleMoving(AInfo: THandleMoveInfo); override;

    /// Summary:
    ///   Returns true if the group block itself or any of its members or submembers is selected.
    /// Return Value:
    ///   True if the block is selected, or any member is selected, or any member of member is selected, and so on. False if all blocks, members
    ///   and submembers are not selected.
    function HasSelecteds: boolean;
  public

    /// Description:
    ///   Call RecalculateRect to update the BoundsRect of the group block based on the position of its members.
    procedure RecalculateRect;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    /// Description:
    ///   Members property holds the list of the diagram objects belonging to that group. To programatically
    ///   add objects to the group, just add items to this property.
    property Members: TGroupMemberList read FMembers write SetMembers;
  end;

  /// Description:
  ///   This class is a special group block class used internally for the block selection mode.
  TSelectionBlock = class(TBaseGroupBlock)
  protected
    procedure UpdateGroupMembers; override;
  end;

  /// Description:
  ///   TGroupBlock is a special diagram block which behaves as a group. You can add members to it and all the
  ///   diagram objects added as member will behave like a single block.
  TGroupBlock = class(TBaseGroupBlock)
  protected
    procedure MemberIncluded(AMember: TDiagramControl); override;
    procedure MemberRemoved(AMember: TDiagramControl); override;
    procedure DrawHandles(AInfo: TDiagramDrawInfo); override;
  published
    property Angle;
    //property Brush;
    //property SelBrush;
    //property Color;
    //property SelColor;
    //property Pen;
    //property Shape;
    //property LinkPoints;
    //property Strings;
    //property Alignment;
    //property VertAlign;
    //property ClipText;
    //property WordWrap;
    //property TextCells;
    property Restrictions;
    //property Font;
    //property DiagramFont;
    property MinWidth;
    property MinHeight;
    //property Shadow;
    //property ShowHint;
    //property ParentShowHint;
    //property Hint;
    //property Gradient;
    //property Bitmap;
    //property BitmapMode;
    //property BitmapTransparent;
    //property Metafile;
    //property Picture;
    //property PictureMode;
    //property Transparent;
    //property Cursor;
    //property StringData;
    property Visible;
    //property Layer;
    //property Transparency;

    {members property must be after angle property}
    property Members;
  end;

  TDiagramPrinterCanvas = class;

  /// Description:
  ///   TDiagramPrinterPaperInfo holds information about a paper for printing purposes.
  TDiagramPrinterPaperInfo = class(TPersistent)
  private
    FPaperHeight: extended;
    FPaperID: integer;
    FPaperWidth: extended;
    FPaperName: string;
  public

   /// Description:
    ///   Copies the object.
    procedure Assign(Source: TPersistent); override;

   /// Description:
    ///   Contains the paper name (A4, Letter, etc.).
    property PaperName: string read FPaperName write FPaperName;

   /// Description:
    ///   Contains the paper ID (standard paper id's, like DMPAPER_A4, DMPAPER_LETTER, etc.).
    property PaperID: integer read FPaperID write FPaperID;

   /// Description:
    ///   Contains the paper width, in milimeters.
    property PaperWidth: extended read FPaperWidth write FPaperWidth;

   /// Description:
    ///   Contains the height of the paper, in milimeters.
    property PaperHeight: extended read FPaperHeight write FPaperHeight;
  end;

  TDiagramPageOrientation = (dpoPortrait, dpoLandscape);

  /// Description:
  ///   TDiagramPrinter is a class the represents an installed printer in the environment.  
  TDiagramPrinter = class
  private
    FBin: Integer;
    FBins: TStrings;
    FCanvas: TDiagramPrinterCanvas;
    FDefOrientation: TDiagramPageOrientation;
    FDefPaper: TDiagramPrinterPaperInfo;
    FDPI: TPoint;
    FInitialized: Boolean;
    FDeviceName: String;
    FPapers: TObjectList;
    FLeftMargin: Extended; // in tenth of mm
    FTopMargin: Extended; // in tenth of mm
    FRightMargin: Extended; // in tenth of mm
    FBottomMargin: Extended; // in tenth of mm
    FOrientation: TDiagramPageOrientation;
    FPort: String;
    FPrinting: Boolean;
    FTitle: String;

    //FPaper: TDiagramPrinterPaperInfo;
    function FindPaper(APaperID: integer): TDiagramPrinterPaperInfo;
    procedure AddDefaultBin;

    {property Bin: Integer read FBin;
    property Bins: TStrings read FBins;}
    function GetPaper(Index: integer): TDiagramPrinterPaperInfo;
    function GetPaperCount: integer;
    function GetBinCount: integer;
    function GetBinID(Index: integer): integer;
    function GetBinName(Index: integer): string;
    procedure UpdateStandardPapers;
    procedure InitializeDefault;
    procedure CheckInitialized;
  protected

    /// Description:
    ///   Do not call this directly, used internally by diagram printing system.
    procedure Initialize; virtual;

    /// Description:
    ///   Begins printing a document. Do not call this directly, used internally by diagram printing system.
    procedure BeginDoc; virtual;

    /// Description:
    ///   Starts a new page in printing job. Do not call this directly, used internally by diagram printing system.
    procedure BeginPage; virtual;

    /// Description:
    ///   Ends a printing job. Do not call this directly, used internally by diagram printing system.
    procedure EndDoc; virtual;

    /// Description:
    ///   Ends a printing page in print job. Do not call this directly, used internally by diagram printing system.
    procedure EndPage; virtual;

    /// Description:
    ///   Do not call this directly, used internally by diagram printing system.
    procedure SetPrintParams(APaperSize: Integer;
      APaperWidth, APaperHeight: Extended; AOrientation: TDiagramPageOrientation); virtual;
  public
    /// Description:
    ///   Instantiates the object.  
    constructor Create(const ADeviceName, APort: PChar);

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Converts an horizontal value in milimeters to pixels, taking into consideration the printer resolution.
    function MMToPixelsX(AMM: extended): integer;

    /// Description:
    ///   Converts a vertical value in milimeters to pixels, taking into consideration the printer resolution.
    function MMToPixelsY(AMM: extended): integer;

    /// Description:
    ///   Call the print properties dialog.
    procedure PropertiesDlg; virtual;

     /// Description:
    ///   Contains the printer's device name.
    property DeviceName: String read FDeviceName;

    /// Description:
    ///   Provides indexed access to the available papers of the printer.
    property Papers[Index: integer]: TDiagramPrinterPaperInfo read GetPaper;

    /// Description:
    ///   Retrieves the number of available papers for the printer.
    property PaperCount: integer read GetPaperCount;

    /// Description:
    ///   Provides indexed access to the names of the paper sources.
    property BinNames[Index: integer]: string read GetBinName;

    /// Description:
    ///   Provides indexed access to the ID's of the paper sources.
    property BinIDs[Index: integer]: integer read GetBinID;

    /// Description:
    ///   Returns the number of paper sources available.
    property BinCount: integer read GetBinCount;
    //property Paper: TDiagramPrinterPaperInfo read FPaper;

    /// Description:
    ///   Returns the default paper size information for the printer.
    property DefPaper: TDiagramPrinterPaperInfo read FDefPaper;

    /// Description:
    ///   Returns the default orientation of the printer's paper
    property DefOrientation: TDiagramPageOrientation read FDefOrientation;

    /// Description:
    ///   Contains the printer's resolution (dots per inch).
    property DPI: TPoint read FDPI;

    /// Description:
    ///   Provides information about the port which the printer is installed.
    property Port: String read FPort;

    /// Description:
    ///   Contains the title of print job. Do not use this, use TatDiagram.PrintSettings.Title instead.
    property Title: String read FTitle write FTitle;
  end;

  /// Description:
  ///   TDiagramRealPrinter is a TDiagramPrinter descendant used when for installed printers. For each printer
  ///   installed in the system, a TDiagramRealPrinter object is created in the DPrinters global collection.
  TDiagramRealPrinter = class(TDiagramPrinter)
  private
    FPrinterHandle: THandle;
    FDevMode: PDeviceMode;
    FDeviceMode: THandle;
    FDC: HDC;
    procedure RecreateDC;
    procedure CreateDeviceMode;
    procedure UpdateDeviceCaps;
  protected

    /// Description:
    ///   Do not call this directly, used internally by diagram printing system.
    procedure Initialize; override;

    /// Description:
    ///   Begins printing a document. Do not call this directly, used internally by diagram printing system.
    procedure BeginDoc; override;

    /// Description:
    ///   Starts a new page in printing job. Do not call this directly, used internally by diagram printing system.
    procedure BeginPage; override;

    /// Description:
    ///   Ends a printing job. Do not call this directly, used internally by diagram printing system.
    procedure EndDoc; override;

    /// Description:
    ///   Ends a printing page in print job. Do not call this directly, used internally by diagram printing system.
    procedure EndPage; override;

    /// Description:
    ///   Do not call this directly, used internally by diagram printing system.
    procedure SetPrintParams(APaperSize: Integer;
      APaperWidth, APaperHeight: Extended; AOrientation: TDiagramPageOrientation); override;
  public

    /// Description:
    ///   Destroys the object.
    destructor Destroy; override;

    /// Description:
    ///   Call the print properties dialog.
    procedure PropertiesDlg; override;
  end;


  /// Description:
  ///   TDiagramGenericPrinter is a TDiagramPrinter descendant used when no printers are installed in the
  ///   environment. Used only to retrieve paper sizes and other meta data information
  TDiagramGenericPrinter = class(TDiagramPrinter)
  protected

    /// Description:
    ///   For internal use only.
    procedure Initialize; override;
  end;

  /// Description:
  ///   TDiagramPrinterCanvas is the canvas object for diagram printing. Do not use this class, it's for
  ///   internal use only.  
  TDiagramPrinterCanvas = class(TCanvas)
  private
    FPrinter: TDiagramPrinter;
    procedure UpdateFont;
  public

   /// Description:
    ///   Internal use only.
    procedure Changing; override;
  end;

  /// Description:
  ///   TDiagramPrinters holds a list of installed printers in the system. If no printer is installed,
  ///   TDiagramPrinters holds a single TDiagramGenericPrinter class.
  TDiagramPrinters = class
  private
    FHasPrinterInstalled: boolean;
  private
    FPrinters: TStrings;
    FPrinterIndex: integer;
    function GetItem(Index: integer): TDiagramPrinter;
    function DefaultPrinterName: string;

    /// Description:
    ///   Clear all diagram, removing all blocks and lines.
    procedure Clear;
    function GetPrinter: TDiagramPrinter;
    function GetPrinterIndex: integer;
    procedure SetPrinterIndex(const Value: integer);
    function IndexOf(DeviceName: string): integer;

   /// Description:
    ///   Provides indexed access to the installed printers.
    property Items[Index: integer]: TDiagramPrinter read GetItem;
    procedure UpdatePrinters;

   /// Description:
    ///   Returns true if there is any printer installed in the system.
    property HasPrinterInstalled: boolean read FHasPrinterInstalled;
  public

   /// Description:
    ///   Creates object.
    constructor Create;

   /// Description:
    ///   Destroys object.
    destructor Destroy; override;

   /// Description:
    ///   Returns the currently selected printer.
    property Printer: TDiagramPrinter read GetPrinter;

   /// Description:
    ///   Use PrinterIndex to specify which is the currently selected printer. If PrinterIndex is set to -1,
    ///   the default printer is selected.
    property PrinterIndex: integer read GetPrinterIndex write SetPrinterIndex;

   /// Description:
    ///   Provides a TStringList object with names of the installed printers.
    property Printers: TStrings read FPrinters;
  end;

  /// Description:
  ///   TDiagramPageSettings object holds information about page settings of diagram for printing purposes.
  ///   You can set properties like Paper Size and Orientation.
  TDiagramPageSettings = class(TPersistent)
  private
    FPaperHeight: extended;
    FPaperWidth: extended;
    FPaperId: integer;
    FPaperName: string;
    FOrientation: TDiagramPageOrientation;
    FDiagram: TatDiagram;
    FTopMargin: extended;
    FLeftMargin: extended;
    FRightMargin: extended;
    FBottomMargin: extended;
    function _NumToStr(N: extended): string;
    function _StrToNum(S: string): extended;
    procedure SetOrientation(const Value: TDiagramPageOrientation);
    procedure SetPaperHeight(const Value: extended);
    procedure SetPaperId(const Value: integer);
    procedure SetPaperName(const Value: string);
    procedure SetPaperWidth(const Value: extended);
    procedure Changed;
    procedure SetBottomMargin(const Value: extended);
    procedure SetLeftMargin(const Value: extended);
    procedure SetRightMargin(const Value: extended);
    procedure SetTopMargin(const Value: extended);
    procedure RetrieveDefaultSize;
    {PrintPaperRect returns the final position (rect) of the page to be printed. It takes into consideration
     the margins, orientation, printer resolution. So, Left/Top properties indicates the first pixel to be printed,
     and Right/Bottom the last pixel to be printed, inside the PaperRect}
    function PrintPageRect: TSquare;
    {PrintPaperSize returns the final paper size of the page to be printed. It takes into consideration
    orientation and printer resolution}
    function PrintPaperSize: TPoint;
    function GetBottomMarginStr: string;
    function GetLeftMarginStr: string;
    function GetRightMarginStr: string;
    function GetTopMarginStr: string;
    procedure SetBottomMarginStr(const Value: string);
    procedure SetLeftMarginStr(const Value: string);
    procedure SetRightMarginStr(const Value: string);
    procedure SetTopMarginStr(const Value: string);
  public

    /// Description:
    ///   Creates the object.
    constructor Create(ADiagram: TatDiagram);

    /// Description:
    ///   Copies one object to another.
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Change the paper size for the paper ID specified by APaperID. For setting standard paper sizes
    ///   (A4, Letter, etc.) you must use this method instead of setting properties individually, because this
    ///   method automatically sets the paper width and height.
    procedure ChangePaperID(APaperID: integer);

    /// Description:
    ///   Specifies the left margin of the page, in milimeters.
    property LeftMargin: extended read FLeftMargin write SetLeftMargin;

    /// Description:
    ///   Specifies the top margin of the page, in milimeters.    
    property TopMargin: extended read FTopMargin write SetTopMargin;

    /// Description:
    ///   Specifies the right margin of the page, in milimeters.
    property RightMargin: extended read FRightMargin write SetRightMargin;

    /// Description:
    ///   Specifies the bottom margin of the page, in milimeters.
    property BottomMargin: extended read FBottomMargin write SetBottomMargin;
  published

    /// Description:
    ///   Specifies the paper name. You don't need to set this if PaperID is a standard size.
    ///   Use ChangePaperID method to change paper size.
    property PaperName: string read FPaperName write SetPaperName;

    /// Description:
    ///   Specifies the paper size id. You don't need to set this if PaperID is a standard size.
    ///   Use ChangePaperID method to change paper size.
    property PaperId: integer read FPaperId write SetPaperId;

    /// Description:
    ///   Specifies the paper width, in milimeters. You don't need to set this if PaperID is a standard size.
    ///   Use ChangePaperID method to change paper size.
    property PaperWidth: extended read FPaperWidth write SetPaperWidth;

    /// Description:
    ///   Specifies the paper height, in milimeters. You don't need to set this if PaperID is a standard size.
    ///   Use ChangePaperID method to change paper size.
    property PaperHeight: extended read FPaperHeight write SetPaperHeight;

    /// Description:
    ///   Specifies the orientation of the page, can be portrait or landscape.    
    property Orientation: TDiagramPageOrientation read FOrientation write SetOrientation;
    property LeftMarginStr: string read GetLeftMarginStr write SetLeftMarginStr;
    property TopMarginStr: string read GetTopMarginStr write SetTopMarginStr;
    property RightMarginStr: string read GetRightMarginStr write SetRightMarginStr;
    property BottomMarginStr: string read GetBottomMarginStr write SetBottomMarginStr;
  end;

  TDgrLibrary = class;

  /// Description:
  ///   TDgrLibraryItem holds information about a block created in a diagram library.
  TDgrLibraryItem = class(TComponent)
  private
    FCaption: string;
    FCategory: string;
    FGlyph: TPicture;
    FData: string;
    FLibrary: TDgrLibrary;
    FItemName: string;
    procedure SetGlyph(const Value: TPicture);
    function RealControlID: string;
    procedure RegisterItem;
  protected
    procedure Loaded; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Saves library item.
    procedure Save;

    /// Description:
    ///   Unregisters item from library.
    procedure UnregisterItem;

    /// Description: creates library control in diagram specified by ADiagram.
    function CreateControl(ADiagram: TatDiagram): TDiagramControl;

    /// Description:
    ///   Returns a reference to library which owns the item.
    property OwnerLibrary: TDgrLibrary read FLibrary;
  published
    property ItemName: string read FItemName write FItemName;
    property Category: string read FCategory write FCategory;
    property Caption: string read FCaption write FCaption;
    property Glyph: TPicture read FGlyph write SetGlyph;
    property Data: string read FData write FData;
  end;

  TDgrLibraryManager = class;

  /// Description:
  ///   TDgrLibrary holds information about a diagram library.
  TDgrLibrary = class(TComponent)
  private
    FLibItems: TObjectList;
    FLibraryName: string;
    FManager: TDgrLibraryManager;
    function GetItem(Index: integer): TDgrLibraryItem;
    procedure AddItem(ALibItem: TDgrLibraryItem);
    procedure RemoveItem(ALibItem: TDgrLibraryItem);
    procedure RegisterLibrary;
    //procedure UnregisterLibrary;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    /// Description:
    ///   Delete all the items from diagram library.
    procedure Clear;

    /// Description:
    ///   Creates a new item in diagram library.
    function NewItem: TDgrLibraryItem;

    /// Description:
    ///   Returns the number of items in diagram library.
    function Count: integer;

    /// Description:
    ///   Delete one item from diagram library.
    procedure DeleteItem(ALibItem: TDgrLibraryItem);

    /// Description:
    ///   Performs a search in diagram library by category and item name. Returns
    ///   a TDgrLibraryItem object if found, or nil otherwise.
    function FindItem(ACategory, AItemName: string): TDgrLibraryItem;

    /// Description:
    ///   Specifies the name of diagram library.
    property LibraryName: string read FLibraryName;
    
    property Items[Index: integer]: TDgrLibraryItem read GetItem; default;
  end;

  TDgrLibraryFiler = class(TComponent)
  protected
    procedure GetLibraryNames(AStrings: TStrings); virtual; abstract;
    function LoadItem(AItem: TDgrLibraryItem): boolean; virtual; abstract;
    function SaveItem(AItem: TDgrLibraryItem): boolean; virtual; abstract;
    procedure DeleteItem(AItem: TDgrLibraryItem); virtual; abstract;
    procedure Load(ALibrary: TDgrLibrary); virtual; abstract;
    procedure Save(ALibrary: TDgrLibrary); virtual; abstract;
    procedure CreateLibrary(ALibrary: TDgrLibrary); virtual; abstract;
    procedure DeleteLibrary(ALibrary: TDgrLibrary); virtual; abstract;
  end;

  TDgrLibraryManager = class(TComponent)
  private
    FFiler: TDgrLibraryFiler;
    FLibs: TObjectList;
    FAutoLoad: boolean;
    procedure SetFiler(const Value: TDgrLibraryFiler);
    procedure CheckFiler;
    procedure AddLibrary(ALib: TDgrLibrary);
    function GetLibrary(Index: integer): TDgrLibrary;
    procedure SaveItem(AItem: TDgrLibraryItem);
    procedure RegisterAll;
  protected
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure Clear;
    procedure Loaded; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure LoadLibraries;
    function LibraryByName(AName: string): TDgrLibrary;
    function CreateLibrary(ALibName: string): TDgrLibrary;
    procedure DeleteLibrary(ALibName: string);
    function CreateItemDlg(ADControl: TDiagramControl): TDgrLibraryItem;
    procedure GetLibraryNames(AStrings: TStrings);
    function LibraryCount: integer;
    procedure ReloadLibrary(ALibName: string);
    property Libraries[Index: integer]: TDgrLibrary read GetLibrary; default;
  published
    property Filer: TDgrLibraryFiler read FFiler write SetFiler;
    property AutoLoad: boolean read FAutoLoad write FAutoLoad default true;
  end;

  TDgrLibraryFolderFiler = class(TDgrLibraryFiler)
  private
    FBaseFolder: string;
    FDefaultExt: string;
    FLibPrefix: string;
    function GetBaseFolder: string;
    function RealBaseFolder: string;
    procedure SetBaseFolder(const Value: string);
    procedure CheckBaseFolder;
    function IsLibraryFileName(ALibName: string): boolean;  
    function BuildLibraryFileName(ALibName: string): string;
    function ExtractLibraryName(AFileName: string): string;
  protected
    procedure GetLibraryNames(AStrings: TStrings); override;
    function LoadItem(AItem: TDgrLibraryItem): boolean; override;
    function SaveItem(AItem: TDgrLibraryItem): boolean; override;
    procedure DeleteItem(AItem: TDgrLibraryItem); override;
    procedure Load(ALibrary: TDgrLibrary); override;
    procedure Save(ALibrary: TDgrLibrary); override;
    procedure CreateLibrary(ALibrary: TDgrLibrary); override;
    procedure DeleteLibrary(ALibrary: TDgrLibrary); override;
  public
    constructor Create(AOwner: TComponent); override;
  published
    property BaseFolder: string read GetBaseFolder write SetBaseFolder;
    property DefaultExt: string read FDefaultExt write FDefaultExt;
  end;

  /// Description:
  ///   Use TDiagramPrintSettings to specify some options for printing.
  TDiagramPrintSettings = class(TPersistent)
  private
    FCopies: integer;
    FTitle: string;
    FPageNumbers: string;
  public

   /// Description:
    ///   Creates object.
    constructor Create;

   /// Description:
    ///   Specifies the name (title) of the printing job.
    property Title: string read FTitle write FTitle;

   /// Description:
    ///   Specifies the number of copies to be printed.
    property Copies: integer read FCopies write FCopies;

   /// Description:
    ///   Specifies the pages to be printed. Page numbers must be separated by commas, and an hifen ("-")
    ///   can be used to specify page ranges. Examples: 
    ///   "1, 5, 10"
    ///   "1, 3-6, 9"
    ///   If PageNumbers is empty (default), all pages will be printed.
    property PageNumbers: string read FPageNumbers write FPageNumbers;
  end;

  TDiagramLayer = class(TCollectionItem)
  private
    FDiagram: TatDiagram;
    FEditable: boolean;
    FVisible: boolean;
    FLayerName: string;
    procedure SetVisible(Value: boolean);
  protected
    property Diagram: TatDiagram read FDiagram;
  public
    constructor Create(Collection: TCollection); override;
    destructor Destroy; override;
  published
    property LayerName: string read FLayerName write FLayerName;
    property Visible: boolean read FVisible write SetVisible default true;
    property Editable: boolean read FEditable write FEditable default true;
  end;                                
  
  TDiagramLayers = class(TOwnedCollection)
  private
    FDiagram: TatDiagram;                                                                
    function GetItem(Index: integer): TDiagramLayer;
    procedure SetItem(Index: integer; Value: TDiagramLayer);
    function FindLayerName: string;
  public
    constructor Create(ADiagram: TatDiagram);
    function FindByName(const AName: string): TDiagramLayer;
    function Add(AName: string = ''): TDiagramLayer;
    property Items[Index: integer]: TDiagramLayer read GetItem write SetItem; default;
  end;

  TDgrEvent = (deLibraryChanged);
  TDgrNotificationInfo = record
    Event: TDgrEvent;
  end;

  IDiagramSubscriber = interface
    procedure DgrNotification(AInfo: TDgrNotificationInfo);
  end;

  /// Description:
  ///   TDControlEvent type is the type used for events that occur related to any diagram control object.
  ///   Use ADControl parameter to know what diagram control object is firing the event.
  TDControlEvent = procedure(Sender: TObject; ADControl: TDiagramControl) of object;

  /// Description:
  ///   TBlockEvent type is the type used for events that occur related to a TCustomDiagramBlock object.
  ///   Use ABlock parameter to know what TCustomDiagramBlock object is firing the event.
  TBlockEvent = procedure(Sender: TObject; ABlock: TCustomDiagramBlock) of object;

  /// Description:
  ///   TLinkEvent type is the type used for events that occur related to a TCustomDiagramLine object. Use
  ///   ALink parameter to know what TCustomDiagramLine object is firing the event.
  TLinkEvent = procedure(Sender: TObject; ALink: TCustomDiagramLine) of object;

  {Event only for design-time purposes}
  TDiagramEventType = (etSelectionChanging, etSelectionChanged, etControlSelected,
    etControlUnselected);

  /// Description:
  ///   For internal use only.
  TDiagramDesignEvent = procedure(Sender: TObject;
    AEventType: TDiagramEventType; Info: integer) of object;

  /// Description:
  ///   TDControlMouseEvent type brings information about a mouse event occuring in a diagram control
  ///   specified by ADControl parameter.
  TDControlMouseEvent = procedure(Sender: TObject; ADControl: TDiagramControl;
    Button: TMouseButton; Shift: TShiftState; X, Y: Integer) of object;

  /// Description:
  ///   TDControlMouseEventEx type brings information about a mouse event occuring in a diagram control
  ///   specified by ADControl parameter. It's similar to TDControlMouseEvent, except that it provides the
  ///   AHandle parameter, which contains the handle object that was clicked (if no handle was clicked, the
  ///   value is nil). You can set AHandle to nil to avoid the behaviour of clicking the handle. By setting
  ///   AHandle to nil, the diagram will behave like the block was click in a position which was no handle.
  TDControlMouseEventEx = procedure(Sender: TObject; ADControl: TDiagramControl;
    Button: TMouseButton; Shift: TShiftState; X, Y: Integer; var AHandle: TStretchHandle) of object;

  /// Description:
  ///   TBlockMouseEvent type brings information about a mouse event occuring in a block diagram
  ///   specified by ABlock parameter.
  TBlockMouseEvent = procedure(Sender: TObject; ABlock: TCustomDiagramBlock;
    Button: TMouseButton; Shift: TShiftState; X, Y: Integer) of object;

  /// Description:
  ///   TDControlMouseMoveEvent brings information about a mouse move over a diagram control specified by
  ///   ADControl parameter.
  TDControlMouseMoveEvent = procedure(Sender: TObject; ADControl: TDiagramControl;
    Shift: TShiftState; X, Y: Integer) of object;

  /// Description:
  ///   TDiagramPaintEvent type is the event handler type for diagram painting events. The ACanvas parameter
  ///   provides the canvas object to be used to perform the painting.    
  TDiagramPaintEvent = procedure(Sender: TObject; ACanvas: TCanvas) of object;

  /// Description:
  ///   TDiagramDrawLinkPointEvent can be used to perform custom drawing of link points. It is called whenever
  ///   a link point is about to be drawn. To perform custom drawing of the link point, use the canvas object
  ///   provided by the ACanvas parameter. The link point object which must be drawn is given by ALinkPoint
  ///   parameter. The center position of the link point is given by APoint parameter, in canvas coordinates.
  ///   You can set DrawingDone to true to avoid the link point to be drawn by the diagram component.  
  TDiagramDrawLinkPointEvent = procedure(Sender: TObject; ACanvas: TCanvas;
    ALinkPoint: TLinkPoint; APoint: TPoint; var DrawingDone: boolean) of object;

  /// Description:
  ///   TDiagramCloseEditorEvent type is the type of event handler for the OnCloseEditor event.
  ///   OnCloseEditor event is fired whenever the text editor of the block is closed. ATextCell parameter
  ///   points to the text cell that was being edited. AUpdate parameter indicates if the text was updated
  ///  (user pressed Ctrl+Enter or clicked in another area of diagram) or not updated (user pressed Esc).
  TDiagramCloseEditorEvent = procedure(Sender: TObject; ATextCell: TTextCell; AUpdate: boolean) of object;
  TBeforeCreateControlEvent = procedure(Sender: TObject; ADControlID: string; var AComponentBaseName: string) of object;

  TPaintStyle = (psScreen, psPreview, psPrint, psBitmap);
  TMouseWheelMode = (mwOff, mwHorizontal, mwVertical);
  TControlSearchMode = (csmAll, csmIgnoreLines);

  TatDiagramClass = class of TatDiagram;

  TDiagramKeyAction = (kaEscape, kaMove, kaPage, kaResize, kaSelect);

  /// Description:
  ///   A set of TDiagramKeyAction items, used to define which keyboard operation will be automatically
  ///   performed by the diagram.
  TDiagramKeyActions = set of TDiagramKeyAction;

  TDiagramDragStyle = (dsOutline, dsShape);
  TDiagramHandlesStyle = (hsClassic, hsVisio);

  TDiagramSelectionMode = (slmMultiple, slmGroup);

  TDiagramMeasUnit = (duMili, duCenti, duInch);

  TDiagramDesignMode = (ddmEditable, ddmNonEditable);

  TDgrSelectionClickMode = (dcmClick, dcmSubClick, dcmShiftClick);

  /// Description:
  ///   TatDiagram component is the main component of diagram component set.
  ///   TatDiagram component holds a collection of diagram controls, like blocks and lines,
  ///   that can be inserted into it, moved, resized and linked together. TatDiagram component
  ///   provides methods for selecting, deleting and inserting controls, clipboard actions,
  ///   load/save diagram, printing and previewing, among many others.
  TatDiagram = class(TWinControl)
  private
    FCurRect: TRect;
    FDragStart: TDot;
    FConnOff: integer;
    FControlCanvas: TCanvas;
    FMouseState: TMouseState;
    FLinkPoint: TLinkPoint;
    FHandle: TStretchHandle;
    FOnInsertBlock: TBlockEvent;
    FOnInsertDControl: TDControlEvent;
    FOnRemoveDControl: TDControlEvent;
    FOnSelectDControl: TDControlEvent;
    FOnUnselectDControl: TDControlEvent;
    FBlocks: TList;
    FLinks: TList;
    FDControls: TList;
    FSelecteds: TList;
    FOldScreenCursor: TCursor;
    FBackground: TDiagramBackground;
    FOnModified: TNotifyEvent;
    FInternalStreaming: boolean;
    FClipboardStreaming: boolean;
    FInternalUpdating: integer;
    FUpdatingPos: integer;
    FUpdatingSel: integer;
    //FBlockClass: TCustomDiagramBlockClass;
    FInsertDControl: TDiagramControl;
    FPreviousDControl: TDiagramControl;
    FDesigning: boolean;
    FOnDesignEvent: TDiagramDesignEvent;
    FOnSelectionChanged: TNotifyEvent;
    FOnSelectionChanging: TNotifyEvent;
    FSnapGrid: TSnapGrid;
    FOnDrawBlock: TDrawBlockEvent;
    FOnDrawShape: TDrawBlockEvent;
    FInplaceMemo: TDiagramMemo;
    FOnInsertLink: TLinkEvent;
    FShowLinkPoints: boolean;
    FUpdateCount: integer;
    FRedrawRequestCount: integer;
    FDControlClicked: TDiagramControl;
    FOnDControlClick: TDControlEvent;
    FOnDControlDblClick: TDControlEvent;
    FOnDControlMouseDown: TDControlMouseEvent;
    FOnDControlMouseDownEx: TDControlMouseEventEx;
    FOnDControlMouseUp: TDControlMouseEvent;
    FOnDControlMouseMove: TDControlMouseMoveEvent;
    FOnBlockMouseEnter: TBlockEvent;
    FOnBlockMouseLeave: TBlockEvent;
    FPainting: boolean;
    FOldDragPos: TDot;
    FOldPosition: TPoint;
    FHandleMoveInfo: THandleMoveInfo;
    FControlInsertInfo: TControlInsertInfo;
    FControlDrawInfo: TControlDrawInfo;
    FControlMoveInfos: TControlMoveInfoArray;
    FTopRuler: TDiagramRuler;
    FLeftRuler: TDiagramRuler;
    FUpdatingScrollBars: boolean;
    FHorzScrollBar: TDiagramScrollBar;
    FVertScrollBar: TDiagramScrollBar;
    FBorderStyle: TBorderStyle;
    FZoomRatio: double;
    FPreviewZoom: double;
    FPaintStyle: TPaintStyle;
    FPreviewOffset: TDot;

    FPrintConvX: single;
    FPrintConvY: single;
    FOnLinkRemoved: TDiagramLinkEvent;
    FOnLinkAdded: TDiagramLinkEvent;
    FBorderColor: TColor;
    FAutoScroll: boolean;
    FOnAcceptAnchor: TAcceptAnchorEvent;
    FOnAcceptLink: TAcceptLinkEvent;
    FOnGetSurroundPts: TGetSurroundPtsEvent;
    FMouseWheelMode: TMouseWheelMode;
    FOnAfterMove: TNotifyEvent;
    FOnBeforeMove: TNotifyEvent;
    FOnMoving: TDiagramPosEvent;
    FOnBeforePaint: TDiagramPaintEvent;
    FOnAfterPaint: TDiagramPaintEvent;
    FOnDrawLinkPoint: TDiagramDrawLinkPointEvent;
    FAutomaticNodes: boolean;
    FNettoExportOffset: Integer;
    FLinkCursor: TCursor;
    FPanCursor: TCursor;
    FZoomCursor: TCursor;
    FClipPrefix: string;
    FUndoStack: TDiagramUndoStack;
    FKeepInserting: boolean;
    FExportBackgroundColor: TColor;
    FZoomingState: TZoomingState;
    FPaintLinesFirst: Boolean;
    FOnCloseEditor: TDiagramCloseEditorEvent;
    FIgnoreScreenDPI: boolean;
    FShowCrossIndicators: boolean;
    FOnDControlResizing: TDiagramPosEvent;
    FOnAfterResize: TNotifyEvent;
    FOnBeforeResize: TNotifyEvent;
    FAutoPage: boolean;
    FPageLines: TPageLines;
    FConnectionLineId: string;
    FKeyActions: TDiagramKeyActions;
    FOnAfterDrawBlock: TDrawBlockEvent;
    FDragStyle: TDiagramDragStyle;
    FHandlesStyle: TDiagramHandlesStyle;
    FAdjustRatioX: double; //used for bitmap previewing only (BuildPreviewBitmap)
    FAdjustRatioY: double; //used for bitmap previewing only (BuildPreviewBitmap)
    FCacheImage: TBitmap; //used to optimize performance
    FUseCache: boolean;
    FSmoothMode: TDgrSmoothMode;
    FTextRenderingMode: TDgrTextRenderingMode;
    FSelBlock: TSelectionBlock;
    FSelectionMode: TDiagramSelectionMode;
    FDoingCache: boolean;
    FCanMoveOutOfBounds: boolean;
    FFixedSideLines: boolean;
    FPageSettings: TDiagramPageSettings;
    FPrintSettings: TDiagramPrintSettings;
    FIntMeasUnit: TDiagramMeasUnit;
    FRulerAutoUnit: boolean;
    FWheelZoom: boolean;
    FWheelZoomIncrement: integer;
    FWheelZoomMax: integer;
    FWheelZoomMin: integer;
    FPrintClipRect: TRect;
    FDesignMode: TDiagramDesignMode;
    FActiveLayers: Int64;
    FLayers: TDiagramLayers;
    FSubList: TInterfaceList;
    FOnBeforeCreateControl: TBeforeCreateControlEvent;
    FGraphicLib: TDgrGraphicLib;
    FCustomGroupBlockClass: TGroupBlockClass;
    FCustomDiagramMemoClass: TDiagramMemoClass;
    procedure BeginCacheBitmap;
    procedure EndCacheBitmap;
    procedure AddBlock(ABlock: TCustomDiagramBlock);
    procedure RemoveBlock(ABlock: TCustomDiagramBlock);
    procedure RemoveDControl(ADControl: TDiagramControl);
    function GetBlocks(Index: integer): TCustomDiagramBlock;
    procedure AddLink(ALink: TCustomDiagramLine);
    procedure RemoveLink(ALink: TCustomDiagramLine);
    function GetLinks(Index: integer): TCustomDiagramLine;
    procedure CalcPanelSize;
    procedure SelectCursor(XPos, YPos: integer; ShowBox: boolean);
    procedure DrawZoomCursor(XPos, YPos: integer; ShowBox: boolean);
    procedure CancelSelecting;
    function SelectionRect(OnlyBlocks: boolean; AAngle: double = 0): TSquare;
    procedure SelectNext( Direction: TSelectDirection);
    procedure SerializeFromClipboard;
    procedure SerializeFromString(S: string);
    function SerializeToString: string;
    procedure BeginInternalUpdate;
    procedure EndInternalUpdate;
    procedure BeginUpdatePos;
    procedure EndUpdatePos;
    procedure WMEraseBkgnd(var m: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure WMHScroll(var Message: TWMHScroll); message WM_HSCROLL;
    procedure WMVScroll(var Message: TWMVScroll); message WM_VSCROLL;
    procedure CMMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
    procedure WMGetDlgCode(var Message: TWMGetDlgCode); message WM_GETDLGCODE;
    procedure CMCtl3DChanged(var Message: TMessage); message CM_CTL3DCHANGED;
    procedure WMNCPaint(var Message: TMessage); message WM_NCPAINT;
    procedure WMPaint(var Message: TWMPaint); message WM_PAINT;
    procedure CMDesignHitTest(var Msg: TCMDesignHitTest); message CM_DESIGNHITTEST;
    procedure CMFontChanged(var Message: TMessage); message CM_FONTCHANGED;
    procedure CMHintShow(var Msg: TMessage); Message CM_HINTSHOW;
    procedure CMShowHintChanged(var Message: TMessage); message CM_SHOWHINTCHANGED;
    function GetSelecteds(Index: integer): TDiagramControl;
    function SnapDeltaPoint(APoint: TDot): TDot;
    procedure SetSnapGrid(const Value: TSnapGrid);
    procedure Redraw;
    procedure UpdateMemoEditor(ACell: TTextCell);
    procedure SetShowLinkPoints(const Value: boolean);
    procedure SelectControlsInRect(ASelRect: TSquare);
    function GetDControls(Index: integer): TDiagramControl;
    procedure AddDControl(ADControl: TDiagramControl);
    function UpdateInserting(XPos, YPos: number): boolean;
    procedure EndInserting(XPos, YPos: integer; Cancel: boolean);
    procedure StartDrawing(XPos, YPos: number);
    procedure UpdateDrawing(XPos, YPos: number);
    procedure DrawingStep(XPos, YPos: number);
    procedure EndDrawing(XPos, YPos: number; Cancel: boolean);
    procedure StartMoving;
    function UpdateMoving(XPos, YPos: number): boolean;
    procedure EndMoving(XPos, YPos: number; Cancel: boolean);
    procedure StartResizing;
    procedure UpdateResizing(XPos, YPos: number);
    procedure EndResizing(XPos, YPos: number; Cancel: boolean);
    function SnapCanvasPoint(APoint: TDot): TDot;
    function RulerAtPos(CP: TDot): TDiagramRuler;
    function MoveDelta(ADControl: TDiagramControl; XPos, YPos: number): TDot;
    function UpdateHandleMoveInfo(X,Y: number; AAction: THandleMoveAction): THandleMoveInfo;
    procedure UpdateControlMoveInfos(X, Y: number; AAction: TControlMoveAction);
    function UpdateControlInsertInfo(X, Y: number; AAction: TControlInsertAction): TControlInsertInfo;
    function UpdateControlDrawingInfo(X, Y: number; AAction: TControlDrawAction): TControlDrawInfo;
    procedure SetBackground(const Value: TDiagramBackground);
    procedure SetLeftRuler(const Value: TDiagramRuler);
    procedure SetTopRuler(const Value: TDiagramRuler);
    procedure UpdateScrollBars;
    procedure SetBorderStyle(const Value: TBorderStyle);
    procedure SetZoom(const Value: integer);
    function GetZoom: integer;
    function PolygonToCanvas(Pts: TDotArray): TDotArray;
    function GetDiagramHeight: integer;
    function GetDiagramWidth: integer;
    function GetDiagramLeft: integer;
    function GetDiagramTop: integer;
    function GetZoomRatio: double;
    function GetPageCount: integer;
    function PrintClientHeight: number;
    function PrintClientWidth: number;
    function PageOffset(Idx: integer): TDot;
    procedure PaintToCanvas(ACanvas: TCanvas; APaintStyle: TPaintStyle; AOffset: TDot);
    procedure UpdatePixelConvertion;
    procedure SetDiagramDefaults;
    function ClipRect(ACanvas: TCanvas): TSquare;
    procedure SetBorderColor(const Value: TColor);
    procedure SetAutoScroll(const Value: boolean);
    procedure SetHorzScrollBar(const Value: TDiagramScrollBar);
    procedure SetVertScrollBar(const Value: TDiagramScrollBar);
    function DoGetSurroundRgn(ABlock: TCustomDiagramBlock): TDotArray;
    function CreateMetaFileCanvas(AWidth, AHeight: integer): TCanvas;
    function DestroyMetaFileCanvas(ACanvas: TCanvas): TMetaFile;
    function IsDesignTime: boolean;
    procedure SetActiveLayers(const Value: Int64);
    procedure SetAutomaticNodes(const Value: boolean);
    procedure GetExportSizeAndPos(exportSize: TExportSize; var ASize: TPoint; var APos: TPoint);
    function ScreenDPI: integer;
    procedure SetShowCrossIndicators(const Value: boolean);
    procedure DeleteControls(restricted: TControlRestrictions);
    procedure SetAutoPage(const Value: boolean);
    procedure SetPageLines(const Value: TPageLines);
    function CanCreateConnection(P: TDot): boolean;
    procedure SetHandlesStyle(const Value: TDiagramHandlesStyle);
    procedure SetSmoothMode(const Value: TDgrSmoothMode);
    procedure SetTextRenderingMode(const Value: TDgrTextRenderingMode);
    procedure CutObjectsToClipboard;
    procedure CopyObjectsToClipboard;
    procedure PasteObjectsFromClipboard;
    procedure CutTextToClipboard;
    procedure CopyTextToClipboard;
    procedure PasteTextFromClipboard;
    function GroupSelection: boolean;
    procedure SetSelectionMode(const Value: TDiagramSelectionMode);
    procedure UpdateSelectionBlock;
    procedure ExportToBitmap(ABitmap: TBitmap; exportSize: TExportSize);
    function ExportToMetafile(exportSize: TExportSize = esStandard): TMetafile;
    procedure DoMemberGroupIncluded(AGroup: TGroupBlock; AMember: TDiagramControl);
    procedure DoMemberGroupRemoved(AGroup: TGroupBlock; AMember: TDiagramControl);
    procedure NameAllDControls;
    procedure CheckGraphicLibObjects;

    /// Summary:
    ///   Selects or unselects the group block and all its members. If ADControl is not a group, then just select or unselect it.
    /// Parameters:
    ///   ADControl - The diagram control or group to be selected or unselected.
    ///   ASelected - Indicates if the group/control must be selected or unselected.
    procedure SetGroupSelected(ADControl: TDiagramControl; ASelected: boolean);
    procedure UpdateSelectionBlockAngle;
    procedure KeepInBounds;
    procedure SetPageSettings(const Value: TDiagramPageSettings);
    procedure SetMeasUnit(const Value: TDiagramMeasUnit);
    procedure SetRulerAutoUnit(const Value: boolean);
    function GetUnitSymbol: string;
    procedure SetDesignMode(const Value: TDiagramDesignMode);
    procedure ReadDiagramStream(AStream: TStream);
    function SerializeObjectToString(ADControl: TDiagramControl): string;
    function GetObjectsRect: TSquare;
    procedure CalculateZoomToFitRatio(TargetRect: TRect; AMargin: integer; var AZoomRatio: double; var AOffset: TPoint);
    procedure SetLayers(const Value: TDiagramLayers);
    procedure UpdateSelecteds;
    procedure NotifySubscribers(AInfo: TDgrNotificationInfo);
    /// Summary:
    ///   Selects the specified control as if it was clicked by the end-user.
    /// Description:
    ///   Call PerformSelection to perform a selection action in the diagram control. The difference
    ///   to simply setting TDiagramControl.Selected property is that PerformSelection implements all the
    ///   logic to select individual group members. For example, if a group is selected and ADControl
    ///   is a member of the group, then the group is unselected and ADControl is selected individually.
    ///   In summary, PerformSelection "simulates" an end-user click in the specified control, not only
    ///   set its Selected property directly.
    ///
    ///   To work properly, ADControl parameter must not be group, since groups can't be clicked by
    ///   the end-user - only "real" member objects.
    /// Parameters:
    ///   ClickedControl - The control to be selected, that was clicked by end-user
    ///   ClickMode - The type of click operation performed.
    ///   SubSelection - Indicates if members can be individually selected or not.
    procedure PerformSelection(ClickedControl: TDiagramControl;
      ClickMode: TDgrSelectionClickMode);

    /// Summary:
    ///   Returns the exact object to be selected when a diagram control is clicked by end-user.
    /// Description:
    ///   When a diagram control is clicked by the end-user, it might be directly selected,
    ///   or a parent group can be selected, if the clicked control is member of a group.
    ///   This method implements the logic for choosing which top control to be selected.
    ///   For example, if a group is selected, and end-user clicks a member of the group,
    ///   then the member is returned, so that it is individually selected.
    /// Parameters:
    ///   ClickedControl - The diagram object that has been clicked. This must not be a group.
    /// Return Value:
    ///   The top group control that must be selected.
    function SubControlToSelect(ClickedControl: TDiagramControl): TDiagramControl;

    procedure SetGraphicLib(const Value: TDgrGraphicLib);
    function DgrPaintEngineClass: TDgrPaintEngineClass;
    procedure TransformPaintEngine(APaintEngine: TDgrPaintEngine);
    function GetPathDrawingMode: boolean;
    function GetUseGdiPlus: boolean; {$IFDEF DELPHI2005_LVL} deprecated; {$ENDIF}
    procedure SetUseGdiPlus(const Value: boolean); {$IFDEF DELPHI2005_LVL} deprecated; {$ENDIF}
    function GetPageColCount: integer;
    function GetPageRowCount: integer;
    procedure SetCustomGroupBlockClass(const Value: TGroupBlockClass);
    procedure SetCustomDiagramMemoClass(const Value: TDiagramMemoClass);
    function GetPaintRect(ACanvas: TCanvas): TRect;
  protected
    function GetMeasUnit: TDiagramMeasUnit; virtual;

    /// Description:
    ///   LinkNodeMouseDown is called whenever the mouse is clicked
    ///   over a linkpoint which is behaving like a node (collapsable).
    procedure LinkNodeMouseDown(ALinkPoint: TLinkPoint); virtual;

    /// Description:
    ///   For internal use only.
    procedure Loaded; override;

    /// Description:
    ///   For internal use only.
    procedure CreateParams(var Params: TCreateParams); override;

    /// Description:
    ///   For internal use only.
    procedure CreateWnd; override;

    /// Description:
    ///   For internal use only.
    procedure ClipRegion(Canvas: TCanvas; ARgn: HRGN);

    /// Description:
    ///   For internal use only.
    procedure UnclipRegion(Canvas: TCanvas);

    /// Description:
    ///   Retrieves canvas coordinate for the point 0, 0 of diagram.
    function CanvasOffset: TDot;

    /// Description:
    ///   RulerOffset returns 0 if rulers are not visible. Otherwise, it returns
    ///   the top left coordinate of diagram considering the rulers visible.
    function RulerOffset: TDot;

    /// Description:
    ///   For internal use.
    function GetChildOwner: TComponent; override;
    {$IFNDEF DELPHI2009_LVL}

    /// Description:
    ///   For internal use.
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    {$ENDIF}

    /// Description:
    ///   For internal use.
    procedure Resize; override;

    /// Description:
    ///   DoMouseWheelDown calls OnMouseWheelDown event. Do not call this method directly.
    function DoMouseWheelDown(Shift: TShiftState; MousePos: TPoint): Boolean; override;

    /// Description:
    ///   DoMouseWheelUp calls OnMouseWheelUp event. Do not call this method directly.
    function DoMouseWheelUp(Shift: TShiftState; MousePos: TPoint): Boolean; override;

    /// Description:
    ///   For internal use.
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;

    /// Description:
    ///   For internal use.
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;

    /// Description:
    ///   For internal use.
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer); override;

    /// Description:
    ///   DoDControlMouseDown is called when the mouse button is pressed over the control specified by ADControl.
    procedure DoDControlMouseDown(ADControl: TDiagramControl; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);

    /// Description:
    ///   DoDControlMouseDownEx is called when the mouse button is pressed over the control specified by ADControl.
    ///   The different from the DoDControlMouseDown is that there is a AHandle var parameter in case you want to
    ///   inform the diagram that a handle was clicked.
    procedure DoDControlMouseDownEx(ADControl: TDiagramControl; Button: TMouseButton; Shift: TShiftState; X, Y: Integer; var AHandle: TStretchHandle);

    /// Description:
    ///   DoDControlMouseEnter is called when the mouse enters the control specified by ADControl.
    procedure DoDControlMouseEnter(ADControl: TDiagramControl);

    /// Description:
    ///   DoDControlMouseLeave is called when the mouse leaves the control specified by ADControl.
    procedure DoDControlMouseLeave(ADControl: TDiagramControl);

    /// Description:
    ///   DoDControlMouseMove is called when the mouse cursor is moved over the control specified by ADControl.
    procedure DoDControlMouseMove(ADControl: TDiagramControl; Shift: TShiftState; X, Y: Integer);

    /// Description:
    ///   DoDControlMouseUp is called when the mouse button is released over the control specified by ADControl.
    procedure DoDControlMouseUp(ADControl: TDiagramControl; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);

    /// Description:
    ///   DoDControlClick is called when the the control specified by ADControl is clicked by the user.
    procedure DoDControlClick(ADControl: TDiagramControl);

    /// Description:
    ///   DoDControlDblClick is called when the the control specified by ADControl is double-clicked by the user.
    procedure DoDControlDblClick(ADControl: TDiagramControl);

    /// Description:
    ///   DoDrawLinkPoint method is called whenever a link point must be drawn. It calls the OnDrawLinkPoint event.
    procedure DoDrawLinkPoint(ACanvas: TCanvas; ALinkPoint: TLinkPoint; APoint: TPoint; var DrawingDone: boolean);

    /// Description:
    ///   The AcceptAnchor method is called by the internal kernel of diagram component to know if the specified
    ///   link between two link points can be performed. It also calls the OnAcceptAnchor event.
    function AcceptAnchor(ALink, AAnchor: TLinkPoint): boolean;

    /// Description:
    ///   The AcceptLink method is called by the internal kernel of diagram component to know if the specified
    ///   link between two link points can be performed. It also calls the OnAcceptLink event.
    function AcceptLink(ALink: TCustomDiagramLine; Anchor1, Anchor2: TLinkPoint): boolean; virtual;

    /// Description:
    ///   DoLinkAdded method is called when a line object specified by ALink is linked to the control specified by ADControl.
    ///   The link point is given by ALinkPoint parameter.
    procedure DoLinkAdded(ADControl: TDiagramControl; ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);

    /// Description:
    ///   DoLinkRemoved method is called when a line object specified by ALink is unlinked from the control specified
    ///   by ADControl. The removed link point is given by ALinkPoint parameter.
    procedure DoLinkRemoved(ADControl: TDiagramControl; ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);

    /// Description:
    ///   InternalPaint method is used for painting the whole diagram. Used for internal purposes only.
    procedure InternalPaint(ACanvas: TCanvas); virtual;

    /// Description:
    ///   DoGetSurroundRegion calls the OnGetSurroundPts event.
    function DoGetSurroundRegion(ABlock: TCustomDiagramBlock): TPointArray;

    /// Description:
    ///   This method fires the OnDrawBlock event, if it exists. Do not call this method directly.
    function CustomDrawBlock(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
      ABlockInfo: TDiagramDrawBlockInfo): boolean;

    /// Description:
    ///   This method is only used internally by diagram. Do not call this method directly.
    function CustomDrawShape(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
      ABlockInfo: TDiagramDrawBlockInfo): boolean;

    /// Description:
    ///   This method fires the OnInsertLink event, if it exists. Do not call this method directly.
    procedure DoInsertLink(ANewLink: TCustomDiagramLine);

    /// Description:
    ///   DoInsertBlock calls OnInsertBlock event. Do not call this method directly.
    procedure DoInsertBlock(ANewBlock: TCustomDiagramBlock);

    /// Description:
    ///   DoInsertDControl calls OnInsertDControl event. Do not call this method directly.
    procedure DoInsertDControl(ANewDControl: TDiagramControl);

    /// Description:
    ///   This method fires the OnDrawBlock event, if it exists. Do not call this method directly.
    procedure DoUnselectDControl(ADControl: TDiagramControl);

    /// Description:
    ///   DoSelectDControl calls OnSelectDControl event. Do not call this method directly.    
    procedure DoSelectDControl(ADControl: TDiagramControl);

    /// Description:
    ///   This method fires the OnDrawShape event, if it exists. Do not call this method directly.
    procedure DesignEvent(Sender: TObject; AEventType: TDiagramEventType; Info: integer);
    {CreateBlock receives X and Y relative to canvas}
    procedure DestroyInsertControl;

    /// Description:
    ///   This method is only used internally by diagram, must not be called directly.
    function CreateControl(ADControlClass: TDiagramControlClass;
      ADControlID: string): TDiagramControl;
    function CreateLibraryControl(AData, ADControlID: string): TDiagramControl;

    /// Description:
    ///   For internal use only.
    function StoreDesignProperty: boolean;

    /// Description:
    ///   Override DiagramStreamClass method to inform the TDiagramStream descendant used for clipboard operations.
    function DiagramStreamClass: TatDiagramClass; virtual;

    /// Description:
    ///   DoAfterDrawBlock is called after the default drawing is performed in the block.
    function DoAfterDrawBlock(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
      ABlockInfo: TDiagramDrawBlockInfo): boolean; virtual;

    /// Description:
    ///   Returns TGroupBlock class used to create new groups of blocks
    ///   (CustomGroupBlockClass if is set, or TGroupBlock by default).
    function GroupBlockClass: TGroupBlockClass;

    /// Description:
    ///   Returns TDiagramMemo class used to create new memo for inplace editing
    ///   (CustomDiagramMemoClass if is set, or TDiagramMemo by default).
    function DiagramMemoClass: TDiagramMemoClass;

    /// Description:
    ///   Do not change this property, it is used by design-time editor.
    property Designing: boolean read FDesigning write FDesigning;

    /// Description:
    ///   Used internaly by the design-time editor.
    property OnDesignEvent: TDiagramDesignEvent read FOnDesignEvent write FOnDesignEvent;
    //property Canvas: TCanvas read FCanvas;

    /// Description:
    ///   InternalUpdating is greater than zero if the diagram is updating itself internally
    ///   (a call to BeginInternalUpdate). For advance use only.    
    property InternalUpdating: integer read FInternalUpdating;
  public
    procedure AddSubscriber(ASubscriber: IDiagramSubscriber);
    procedure RefreshToolbars;
    {$IFDEF DELPHI2009_LVL}
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    {$ENDIF}

    /// Description:
    ///   Use FindCompName to find a valid and unique name for a new diagram control being inserted in the diagram.
    function FindCompName(AComponent: TComponent; ABaseName: string = ''): string;

    /// Description:
    ///   Use CloseEditor to finish text editing. You can check if a text is being edited by calling the EditingText method.
    ///   If AUpdate parameter is true, the changes made by end-user in the text are applied. If AUpdate is false, changes are lost.
    procedure CloseEditor(AUpdate: boolean);
    procedure Assign(Source: TPersistent); override;

    /// Description:
    ///   Converts a client coordinate (in pixels) to the value in inches.
    function ClientToInches(X: double): double;

    /// Description:
    ///   Converts a client coordinate (in pixels) to the value in milimeters.
    function ClientToMilimeters(X: double): double;

    /// Description:
    ///   Converts a coordinate value in milimeters to client coordinates (in pixels).
    function MilimetersToClient(X: double): double;

    /// Description:
    ///   Converts a coordinate value in inches to client coordinates (in pixels).
    function InchesToClient(X: double): double;

    /// Description:
    ///   Use ClientToCanvas method to convert client (absolute) coordinates to canvas coordinates
    ///   (for drawing operations, for example).
    function ClientToCanvas(APoint: TDot): TDot; overload;
    function ClientToCanvas(APoint: TPoint): TPoint; overload;
    function ClientToCanvas(ARect: TRect): TRect; overload;
    function ClientToCanvas(ARect: TSquare): TSquare; overload;

    /// Description:
    ///   Use CanvasToClient method to convert canvas coordinates to client (absolute) coordinates.
    function CanvasToClient(APoint: TPoint): TPoint; overload;
    function CanvasToClient(ARect: TSquare): TSquare; overload;
    function CanvasToClient(APoint: TDot): TDot; overload;

    /// Description:
    ///   DControlAtPos returns the diagram control located at the pos specified by Pos (in canvas coordinates).
    ///   If no control is found, DControlAtPos returns nil. If there are more than one control at the pos,
    ///   it returns the first control found. SearchMode defines the type of the search (which diagram objects
    ///   will be considered when searching).
    function DControlAtPos(const Pos: TDot; SearchMode: TControlSearchMode = csmAll;
      AGroupFilter: TDiagramControlFilter = cfIgnoreGroups): TDiagramControl;

    /// Description:
    ///   HandleAtPos returns the handle (TStretchHandle obejct) located at the mouse position
    ///   specified by Pos (in canvas coordinates). If no handle is found, HandleAtPos returns nil.
    function HandleAtPos(const Pos: TDot; AIgnoreZoom: boolean = false): TStretchHandle;
    //function BuildPreviewMetaFile(APageIdx: integer): TMetaFile;

    /// Description:
    ///   Use internally by the printing system, must not be called directly.
    function BuildPreviewBitmap(APageIdx: integer; AWidth: integer; AHeight: integer = 0): TBitmap;

    /// Description:
    ///   This method is only used internally by diagram, must not be called directly.
    procedure PaintWindow(DC: HDC); override;

    /// Description:
    ///   Creates the component.
    constructor Create(AOwner: TComponent); override;

    /// Description:
    ///   For internal use.
    destructor Destroy; override;

    /// Description:
    ///   GetPixelFormat method returns the pixel format of the current diagram, based on current screen settings.
    function GetPixelFormat: TPixelFormat;

    /// Description:
    ///   Modified method is called when the diagram is modified.
    procedure Modified; virtual;

    /// Description:
    ///   Paints the current diagram to a bitmap.
    procedure PaintToBitmap(ABitmap: TBitmap; exportSize: TExportSize = esStandard);

    /// Description:
    ///   Call BeginUpdate to perform changes in diagram and avoid it to be redrawn.
    procedure BeginUpdate;

    /// Description:
    ///   Call EndUpdate after a call to BeginUpdate to invalidate the diagram and make all changes made to it to be
    ///   reflected in the diagram canvas.
    ///   If AOptimize is true, diagram is redrawn only if really needed.
    function EndUpdate(AOptimize: boolean = False): integer;

    procedure BeginUpdateSel;
    procedure EndUpdateSel;

    /// Description:
    ///   Prints the diagram in the current selected printer.
    ///   If AShowDialog is true, the print dialog is displayed so end-user can choose printer,
    ///   number of copies and pages to be printed.
    function Print(AShowDialog: boolean = False): boolean;

    /// Description:
    ///   Opens a modal dialog that shows the print preview of diagram.
    procedure Preview;

    /// Description:
    ///   Displays a dialog or diagram page setup (paper size, margins, orientation).
    procedure PageSetupDlg;

    /// Description:
    ///   Copies the whole diagram to the clipboard as a bitmap. Size of the bitmap is defined by exportSize parameter.
    procedure CopyBitmapToClipboard(exportSize: TExportSize = esStandard);

    /// Description:
    ///   Copies the whole diagram to the clipboard as a metafile. Size of the metafile is defined by exportSize parameter.
    procedure CopyMetafileToClipboard(exportSize: TExportSize = esStandard);

    /// Description:
    ///   Use ExportToFile method to exports the whole diagram to a file.
    ///   The format of the file is specified by exportFormat. The size and position of the diagram in the file
    ///   is specified by exportSize parameter.
    ///   Current supported formats are bitmap (".bmp" extension) and enhanced meta file (".wmf" or ".emf" extensions).
    procedure ExportToFile(const fileName : String; exportFormat : TExportFormat = efAuto; exportSize : TExportSize = esCentered);

    /// Description:
    ///   Export the current diagram to file as a BMP image.
    procedure ExportAsBMP(const fileName : String; exportSize : TExportSize = esCentered);

    /// Description:
    ///   Export the current diagram to a stream as a BMP image.
    procedure ExportAsBMPStream(AStream: TStream; exportSize : TExportSize = esCentered);

    /// Description:
    ///   Export the current diagram to file as a WMF image.
    procedure ExportAsWMF(fileName: string; exportSize: TExportSize = esStandard);

    /// Description:
    ///   Export the current diagram to a stream as a WMF image.
    procedure ExportAsWMFStream(AStream: TStream; exportSize: TExportSize = esStandard);

    /// Description:
    ///   Export the current diagram to file as a BMP image.
    procedure ExportAsJPG(const fileName : String; exportSize : TExportSize = esCentered);

    /// Description:
    ///   Export the current diagram to a stream as a BMP image.
    procedure ExportAsJPGStream(AStream: TStream; exportSize : TExportSize = esCentered);

    /// Description:
    ///   Cut the current selected diagram controls to clipboard.
    procedure CutToClipboard;

    /// Description:
    ///   Copies the selected diagram controls to clipboard.
    procedure CopyToClipboard;

    /// Description:
    ///   Paste controls from clipboard into diagram. Pasted controls become selected.
    procedure PasteFromClipboard;

    /// Description:
    ///   Returns true if a paste operation is possible to be performed (text or objects). For updating actions.
    function CanPaste: boolean;

    /// Description:
    ///   Returns true if a copy operation is possible to be performed. For updating actions.
    function CanCopy: boolean;
    procedure Clear;

    /// Description:
    ///   Delete the selected controls from diagram.
    procedure DeleteSelecteds;

    /// Description:
    ///   Unselect all diagram controls.    
    procedure UnselectAll;

    /// Description:
    ///   Select all controls in diagram.
    procedure SelectAll;

    /// Description:
    ///   Call StartPanning to put the diagram state in the pan mode.
    ///   End-user will be able to pan (scroll the diagram by dragging it) the diagram,
    ///   until CancelPanning method is called.
    procedure StartPanning;

    /// Description:
    ///   Call CancelPanning method to cancel the panning of the diagram and bring the state back to the
    ///   browsing mode.
    procedure CancelPanning;

    /// Description:
    ///   Call StartZooming to put the diagram state in the zoom in/out mode.
    ///   End-user will be able to zoom in/out the diagram, until CancelZooming method is called.
    ///   ZoomingState parameter indicates if the zoom will be "in" or "out".    
    procedure StartZooming(ZoomingState: TZoomingState);

    /// Description:
    ///   Call CancelZooming method to cancel the zooming in/out of the diagram and bring the state back to the
    ///   browsing mode.
    procedure CancelZooming;

    /// Description:
    ///   Call ZoomIn to zoom in the diagram to the rect specified by ARect parameter, in client coordinates.
    ///   If the ARect is too small (smaller then 10 pixels width/height), then the zoom will be done
    ///   automatically to the next zoom step (for example, from 100% to 125%, from 125% to 150% and so on).
    procedure ZoomIn(ARect: TSquare);

    /// Description:
    ///   Call ZoomOut to zoom out the diagram from the rect specified by ARect parameter, in client coordinates.
    ///   If the ARect is too small (smaller then 10 pixels width/height), then the zoom will be done
    ///   automatically to the previous zoom step (for example, from 125% to 100%, from 100% to 75% and so on).
    procedure ZoomOut(ARect: TSquare);
    procedure ZoomMoveToFit(AMargin: integer = 0);

    /// Description:
    ///   Use BringToFront method to bring the diagram control specified by ADControl to the front of diagram.
    ///   If no ADControl is specified, the method brings the selected controls to front.
    procedure BringToFront(ADControl: TDiagramControl); overload;

    /// Description:
    ///   Use SendToBack method to send the diagram control specified by ADControl to the back of diagram.
    ///   If no ADControl is specified, the method sends the selected controls to back.    
    procedure SendToBack(ADControl: TDiagramControl); overload;
    procedure BringToFront; overload;
    procedure SendToBack; overload;
    procedure MoveControlIndex(ADControl: TDiagramControl; NewIndex: integer);

    /// Description:
    ///   Save diagram to the file specified by AFileName.
    procedure SaveToFile(const AFileName: string);

    /// Description:
    ///   Load diagram from the file specified by AFileName.
    procedure LoadFromFile(const AFileName: string);

    /// Description:
    ///   Save current diagram the stream specified by AStream.
    procedure SaveToStream(AStream: TStream; TextFormat: boolean = false); virtual;

    /// Description:
    ///   Load diagram from the stream specified by AStream.
    procedure LoadFromStream(AStream: TStream; TextFormat: boolean = false); virtual;

    /// Description:
    ///   Start text editing for the block specified by ABlock.
    procedure EditBlockText(ABlock: TCustomDiagramBlock);

    /// Description:
    ///   EditControlText starts editing of the text inside the diagram control specified by ADControl.
    procedure EditControlText(ADControl: TDiagramControl);

    /// Description:
    ///   EditTextCell starts editing of the text inside the text cell specified by ACell.
    procedure EditTextCell(ACell: TTextCell);

    /// Description:
    ///   FindDControl finds a diagram control in the diagram which name is specified by AName parameter.    
    function FindDControl(const AName: string): TDiagramControl;

    /// Description:
    ///   FindBlock finds a block in the diagram which name is specified by AName parameter.
    function FindBlock(const AName: string): TCustomDiagramBlock;

    /// Description:
    ///   For internal use.
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;

    /// Description:
    ///   Call StartInsertingControl to put the diagram in the insert state.
    ///   After calling StartInsertControl, user can click the diagram or drag into the diagram
    ///   to insert the control specified by ADControlClass or ADControlID.
    procedure StartInsertingControl(const ADControlID: string; AKeepInserting: boolean = false); overload;
    procedure StartInsertingControl(ADControlClass: TDiagramControlClass; AKeepInserting: boolean = false); overload;
    procedure StartInsertingControl(ADControl: TDiagramControl; AKeepInserting: boolean = false); overload;

    /// Description:
    ///   Call CancelInsertingBlock to change diagram state from inserting to browsing.
    ///   CancelInsertingBlock only makes sense after a calling to StartInsertingControl. 
    procedure CancelInsertingBlock;

    /// Description:
    ///   Returns the number of selected blocks in diagram.
    function SelectedBlockCount(AFilter: TDiagramControlFilter = cfAll): integer;

    /// Description:
    ///   Returns the number of selected links (lines) in diagram.
    function SelectedLinkCount: integer;

    /// Description:
    ///   Returns the number of line objects in diagram.
    function LinkCount: integer;

    /// Description:
    ///   Returns the number of block objects in diagram.
    function BlockCount: integer;

    /// Description:
    ///    Returns the number of selected objects in diagram.
    function SelectedCount(AFilter: TDiagramControlFilter = cfAll): integer;

    /// Description:
    ///   DControlCount returns the number of diagram controls (objects) in the diagram.
    function DControlCount: integer;

    /// Description:
    ///   Returns true if there is a line object anchored to both ABlock1 and ABlock2.
    function AreBlocksLinked(ABlock1, ABlock2: TCustomDiagramBlock): boolean;

    /// Description:
    ///   Returns a line object that links blocks ABlock1 and ABlock2.
    ///   If there is no line linking the two blocks, LinkBetweenBlocks returns nil.
    function LinkBetweenBlocks(ABlock1, ABlock2: TCustomDiagramBlock): TCustomDiagramLine;

    /// Description:
    ///   Perform undo operation in diagram.
    procedure Undo;

    /// Description:
    ///   Perform redo operation in diagram.
    procedure Redo;

    /// Description:
    ///   Retrieves the name of next undo action.
    ///   If it is an empty string, then there is no undo action to perform.
    function NextUndoAction: string;

    /// Description:
    ///   Retrieves the name of next redo action.
    ///   If it is an empty string, then there is no redo action to perform.
    function NextRedoAction: string;

    /// Description:
    ///   Push current diagram state into undo stack under a name AActionName.
    ///   The diagram state can be then retrieved by calling Undo method.
    procedure PushUndoStack(const AActionName: string);

    /// Description:
    ///   Remove all undo operations from undo stack.
    procedure ClearUndoStack;

    /// Description:
    ///   Returns true if the text of a block is being edited.    
    function EditingText: boolean;

    /// Description:
    ///   Returns the text cell object which is being edited. If text editing is not being performed, returns nil.
    function EditingCell: TTextCell;
    procedure MoveBlocks(ADeltaX, ADeltaY: number; AOnlySelected: boolean = false;
      AOnlyMovable: boolean = true);

    /// Description:
    ///   Deprecated. Use Layers property.
    ///   Use LayerActive function to know if the layer specified by ALayer is active (visible).
    function LayerActive(ALayer: integer): boolean;

    /// Description:
    ///   Deprecated. Use Layers property.
    ///   The ActivateLayer activates (shows) the layer specified by ALayer.
    procedure ActivateLayer(ALayer: integer);

    /// Description:
    ///   Deprecated. Use Layers property.
    ///   DeactivateLayer deactivates (hides) the layer specified by ALayer.
    procedure DeactivateLayer(ALayer: integer);

    /// Description:
    ///   Returns the collapsable TLinkPoint object located at the position specified by Pos.
    function LinkNodeAtPos(const Pos: TDot): TLinkPoint;

    /// Description:
    ///   Returns the TLinkPoint object located at the position specified by Pos.
    function LinkPointAtPos(const Pos: TDot;
      ADControl: TDiagramControl = nil;
      OnlyUnselected: boolean = false; OnlyNodes: boolean = false;
      OnlyAcceptAnchoreds: boolean = false): TLinkPoint;

    /// Description:
    ///   Makes the diagram control specified by AControl visible.
    ///   It moves the scrollbars, if needed, until the control is visible in the most possible central position of diagram.
    procedure MakeControlVisible(AControl: TDiagramControl);

    /// Description:
    ///   ConnectionLineClass returns the class of the line used for automatic connections.
    ///   The ConnectionLineClass is directly related to the content of ConnectionLineId property.
    function ConnectionLineClass: TCustomDiagramLineClass;

    /// Description:
    ///   Adds a new group block to the diagram. The group is empty, you must them add existing
    ///   diagram objects to the group using the TGroupBlock.Members collection.
    function AddGroup: TGroupBlock;

    /// Description:
    ///   Create a new group with the current selected blocks.
    function GroupSelectedBlocks: TGroupBlock;

    /// Description:
    ///   Ungroup the current selected blocks.
    ///   It a group contains several group levels, only the top level will be ungrouped.
    procedure UngroupSelectedBlocks;

    /// Description:
    ///   Returns the currently selected printer for diagram printing.
    function DPrinter: TDiagramPrinter;

    function MMToMeasUnit(AMM: extended): extended;
    function MeasUnitToMM(AUnit: extended): extended;

    procedure ClearDControls;
    procedure DeleteLayerControls(ALayer: TDiagramLayer);

    /// Description:
    ///   ClipboardNamePrefix is a low-level property which defines the prefix to be added to the name of
    ///   all diagram controls being copied to clipboard. This property should be rarely used, it is only
    ///   needed if you're having problems with clipboard operations.    
    property ClipboardNamePrefix: string read FClipPrefix write FClipPrefix;

    /// Description:
    ///   Returns the number of pages needed to print diagram.
    property PageCount: integer read GetPageCount;
    property PageColCount: integer read GetPageColCount;
    property PageRowCount: integer read GetPageRowCount;

    /// Description:
    ///   Points to the instance of the control that is about to be inserted in diagram by the user.
    ///   This instance is create automatically by the diagram component after a call to StartInsertBlock method.
    property InsertDControl: TDiagramControl read FInsertDControl;

    /// Description:
    ///   Use Blocks property to have individual information of a TCustomDiagramBlock object in diagram.
    ///   Use BlockCount method to retrieve the number of blocks in diagram.
    property Blocks[Index: integer]: TCustomDiagramBlock read GetBlocks;

    /// Description:
    ///   Use Links property to have individual information of a TCustomDiagramLine object in diagram.
    ///   Use LinkCount method to retrieve the number of lines in diagram.
    property Links[Index: integer]: TCustomDiagramLine read GetLinks;

    /// Description:
    ///   Use DControls property to have individual access to a TDiagramControl object in diagram.
    ///   Use DControlCount method to retrieve the number of controls in diagram.
    property DControls[Index: integer]: TDiagramControl read GetDControls;

    /// Description:
    ///   Use Selecteds property to have individual information of a TDiagramControl object that is selected in diagram.
    ///   Use SelectedCount method to retrieve the number of selected controls in diagram.
    property Selecteds[Index: integer]: TDiagramControl read GetSelecteds;

    /// Description:
    ///   Use MouseState property to check the state of the diagram in terms of user operation
    ///   (if the user is inserting a block, selecting controls, etc.).    
    property MouseState: TMouseState read FMouseState;

    /// Description:
    ///   Retrieves the width of full diagram area.
    property DiagramWidth: integer read GetDiagramWidth;

    /// Description:
    ///   Retrieves the height of full diagram area.
    property DiagramHeight: integer read GetDiagramHeight;

    /// Description:
    ///   MovingStartPoint property returns the position of the mouse (in canvas coordinates)
    ///   where the move operation was started.
    property MovingStartPoint: TDot read FDragStart;

    /// Description:
    ///   PaintStyle property returns the current painting operation being performed
    ///  (painting to the printer, to the canvas control, to bitmap, etc.)
    property PaintStyle: TPaintStyle read FPaintStyle;

    /// Description:
    ///   Specified the background color to be used when exporting the diagram to a image
    ///   (bitmap or metafile, for example). Default color is clWhite.
    property ExportBackgroundColor: TColor read FExportBackgroundColor write FExportBackgroundColor;

    /// Description:
    ///   Contains the current zoom ratio of diagram. Zoom ratio of 1 is equivalent to 100%.    
    property ZoomRatio: double read GetZoomRatio;

    /// Description:
    ///   Use ConnectionLineId to define a line class that will be used for automatic linkpoint connection.
    ///   The content of ConnectionLineId must be a valid DControlID.
    ///   For example: ConnectionLineId := 'TDiagramLine';
    ///   If ConnectionLineId is not empty (empty is the default) then when the user moves the mouse over
    ///   a linkpoint, he/she can start dragging to insert a new line. The line to be created is the one
    ///   specified by ConnectionLineId. With this feature, line connections can easily be made between
    ///   two linkpoints, even if another block object is selected in the toolbar to be inserted in the diagram.
    property ConnectionLineId: string read FConnectionLineId write FConnectionLineId;

    /// Description:
    ///   Specified the way diagram behaves when the selected controls are moved or resized.
    ///   dsOutline - Default/standard behaviour: a focus rect is drawn while control is dragged
    ///   dsShape - Visio style behaviour: the control is moved/resized at the same time it is dragged
    property DragStyle: TDiagramDragStyle read FDragStyle write FDragStyle;

    /// Description:
    ///   When true, intermediate handles of side lines are not repositioned when the attached blocks are moved.
    ///   The line becomes "fixed" at its position. Default is false.
    property FixedSideLines: boolean read FFixedSideLines write FFixedSideLines;

    /// Description:
    ///   Returns a TDiagramPrintSettings object which contains information about printing settings/options,
    ///   like number of copies and pages to print. Set some of those properties before calling Print method.   
    property PrintSettings: TDiagramPrintSettings read FPrintSettings;

    /// Description:
    ///   Returns the symbol of currently selected unit (specified by MeasUnit property).
    property UnitSymbol: string read GetUnitSymbol;
    property DesignMode: TDiagramDesignMode read FDesignMode write SetDesignMode;

    property GraphicLib: TDgrGraphicLib read FGraphicLib write SetGraphicLib;
    property PathDrawingMode: boolean read GetPathDrawingMode;
    property CustomGroupBlockClass: TGroupBlockClass read FCustomGroupBlockClass write SetCustomGroupBlockClass;
    property CustomDiagramMemoClass: TDiagramMemoClass read FCustomDiagramMemoClass write SetCustomDiagramMemoClass;

    {$WARNINGS OFF}
    /// Description:
    ///   UseGdiPlus property specifies if GDI+ functions will be used to draw diagram objects, when GDI+
    ///   is available and when the diagram object supports GDI Plus. Setting UseGdiPlus property to false
    ///   will make the whole diagram drawing to not use GDI+ function at all, no matter what.
    ///   Default is true.
    property UseGdiPlus: boolean read GetUseGdiPlus write SetUseGdiPlus;
    {$WARNINGS ON}
  published

    /// Description:
    ///   If PaintLinesFirst is true, all the lines (links) are painted in the diagram before the blocks.
    ///   This way, the lines will always appear "behind" the blocks. Also, when clicking/selecting objects,
    ///   the lines will have lower precedence than the blocks (a mouse click over a region which has both
    ///   lines and blocks, the blocks will be selected first). PaintLinesFirst if false by default.
    property PaintLinesFirst: Boolean read FPaintLinesFirst write FPaintLinesFirst default False;

    /// Description:
    ///   NettoExportOffset defines the margin of the diagram when exported to a file or to the clipboard,
    ///   when exportSize is defined as esNetto.    
    property NettoExportOffset: Integer read FNettoExportOffset write FNettoExportOffset;

    /// Description:
    ///   AutomaticNodes property defines if linkpoints are automatically collapsable or not.
    ///   If AutomaticNodes is true, a linkpoint is automatically turn into a node if the are one or more
    ///   blocks connected to the link point. In other words, the linkpoint is made collapsable.
    ///   If AutomaticNodes property is false, the linkpoint will only be a node (collapsable)
    ///   if its Collapsable property is set to true.
    property AutomaticNodes: boolean read FAutomaticNodes write SetAutomaticNodes;

    /// Description:
    ///   if AutoScroll property is set to true, the size of diagram is automatically recalculated to hold the diagram objects. If AutoScroll is false, then the size of diagram is defined by the Range property of its scroll bars, and that size is never changed, regardless of the position and number of diagram objects.
    property AutoScroll: boolean read FAutoScroll write SetAutoScroll;

    /// Description:
    ///   When AutoPage property is set to true, the diagram area is automatically resized to contain
    ///   a full printed page. If the objects are placed outside the page, the diagram area is automatically
    ///   resized again to contain two full pages, and so on.
    property AutoPage: boolean read FAutoPage write SetAutoPage;

    /// Description:
    ///   Use Background property to specify a background bitmap, and the way the bitmap will be displayed in diagram.
    property Background: TDiagramBackground read FBackground write SetBackground;

    /// Description:
    ///   Use SnapGrid property turn snap grid on/off, changing snap grid look and feel, etc.
    property SnapGrid: TSnapGrid read FSnapGrid write SetSnapGrid;

    /// Description:
    ///   Use ShowLinkPoints property to make link points visible or invisible in diagram.    
    property ShowLinkPoints: boolean read FShowLinkPoints write SetShowLinkPoints;

    /// Description:
    ///   LeftRuler property retrieves a TDiagramRuler object with all information about the ruler at the left of diagram.
    property LeftRuler: TDiagramRuler read FLeftRuler write SetLeftRuler;

    /// Description:
    ///   TopRuler property retrieves a TDiagramRuler object with all information about the ruler at the top of diagram.
    property TopRuler: TDiagramRuler read FTopRuler write SetTopRuler;

    /// Description:
    ///   Use Zoom property to set the zoom of diagram. The zoom is presented in % values, the default value is
    ///   100 (Zoom of 100% - normal size).    
    property Zoom: integer read GetZoom write SetZoom;

    /// Description:
    ///   Use BorderColor to set the color of the border of diagram.
    property BorderColor: TColor read FBorderColor write SetBorderColor;

    /// Description:
    ///   Use VertScrollBar to hide, show, or manipulate the vertical scroll bar for the scrolling windowed control.
    property VertScrollBar: TDiagramScrollBar read FVertScrollBar write SetVertScrollBar;

    /// Description:
    ///   Use HorzScrollBar to hide, show, or manipulate the horizontal scroll bar for the diagram.
    property HorzScrollBar: TDiagramScrollBar read FHorzScrollBar write SetHorzScrollBar;

    /// Description:
    ///   Use MouseWheelMode to define what happens when the mouse wheel is used by the end-user.
    ///   It can either do nothing, scroll the horizontal scroll bar, or scroll the vertical scroll bar.
    property MouseWheelMode: TMouseWheelMode read FMouseWheelMode write FMouseWheelMode;

    /// Description:
    ///   Deprecated. Use Layers property.
    ///   Returns an integer which contains information about the active layers.
    ///   Each bit of the integer relates to a layer. If the bit is set, the layer is active,
    ///   otherwise it's inactive. For a more friendly usage of layers (to avoid the bit logic),
    ///   use LayerActive, ActivateLayer and DeactivateLayer methods.
    property ActiveLayers: Int64 read FActiveLayers write SetActiveLayers;

    property Layers: TDiagramLayers read FLayers write SetLayers;

    /// Description:
    ///   Defines the mouse cursor to be displayed whenever the mouse moves over a link point or a link
    ///  (connection between a line and a link point) is about to be made in a move or line move operation.
    property LinkCursor: TCursor read FLinkCursor write FLinkCursor;

    /// Description:
    ///   PanCursor property specifies the mouse cursor to be displayed when the diagram is in panning mode.
    property PanCursor: TCursor read FPanCursor write FPanCursor;

    /// Description:
    ///   Specify the mouse cursor to be used when diagram is in the zooming state (StartZooming was called).
    ///   Default is clDefault.
    property ZoomCursor: TCursor read FZoomCursor write FZoomCursor;

    /// Description:
    ///   When saving/loading diagram in different screen resolutions, the size of blocks may vary,
    ///   because diagram saves the size of blocks in pixels. By setting IgnoreScreenDPI to true,
    ///   the size objects will be kept (in inches or milimeters), regardless of the screen resolution
    ///   they were saved or loaded.
    property IgnoreScreenDPI: boolean read FIgnoreScreenDPI write FIgnoreScreenDPI;

    /// Description:
    ///   When two side lines are being crossed in diagram, a cross indicator might be displayed.
    ///   Set ShowCrossIndicators to true to make those indicators visible. Only applies to side lines.
    property ShowCrossIndicators: boolean read FShowCrossIndicators write SetShowCrossIndicators;

    /// Description:
    ///   Use PageLines property to define how the page lines will be displayed in the diagram background.
    ///   Page lines are visual indications that show the limits of the printed page.
    property PageLines: TPageLines read FPageLines write SetPageLines;
    /// Description:
    ///   Specifies which keyboard operations will be automatically performed by the diagram.
    ///   See TKeyAction for the list of available keyboard operations.
    property KeyActions: TDiagramKeyActions read FKeyActions write FKeyActions;
    property HandlesStyle: TDiagramHandlesStyle read FHandlesStyle write SetHandlesStyle;

    /// Description:
    ///   Specifies the smoothing mode (antialias) when GDI+ functions are used.
    property SmoothMode: TDgrSmoothMode read FSmoothMode write SetSmoothMode;

    /// Description:
    ///   Specifies the text rendering mode (antialias) when GDI+ functions are used. Default is tmAntiAlias.
    ///   tmAntiAlias: Use antialiasing text
    ///   tmAntiAliasHint: Use antialiasing text with hint
    ///   tmDefault: Use the system default rendering mode
    ///   tmNormal: Do not use antialiasing
    ///   tmNormalHint: Do not use antialising, with hint
    ///   tmClearType: Use ClearType to draw text.
    property TextRenderingMode: TDgrTextRenderingMode read FTextRenderingMode write SetTextRenderingMode;

    /// Description:
    ///   Use SelectionMode property to determine how the selection will behave in the diagram. Default value is slmMultiple.
    ///   slmMultiple: In multiple selection, each object is displayed selected individually.
    ///   A selecion rectangle is displayed for each object, and also its handles is displayed individually.
    ///   slmGroup: In multiple selection, all selected objects are displayed as a group, and handles are
    ///   displayed only for the group. It's a more Visio-like approach.
    property SelectionMode: TDiagramSelectionMode read FSelectionMode write SetSelectionMode;

    /// Description:
    ///   When CanMoveOutOfBounds is true, objects can be moved/resized to a negative position.
    ///   When it happens, the whole diagram will be shifted.
    ///   If CanMoveOutOfBounds is false, end-users can't move objects to a negative position.
    ///   If they do, the selected objects will be kept on the edges of diagram, and the unselected
    ///   objects will not be shifted. Default value is true.
    property CanMoveOutOfBounds: boolean read FCanMoveOutOfBounds write FCanMoveOutOfBounds;

    /// Description:
    ///   Returns a TDiagramPageSettings object which contains information about the diagramprinting page (paper size, margins, etc.).
    property PageSettings: TDiagramPageSettings read FPageSettings write SetPageSettings;

    /// Description:
    ///   When RulerAutoUnit is true, the top/left ruler units are specified by property TatDiagram.MeasUnit.
    ///   When RulerAutoUnit is false, MeasUnit property is ignored and TDiagramRuler.Units property is used
    ///   instead. For backward compatibility, this property is false but default, but for new developments,
    ///   it's recommended to set this property to true and use only MeasUnit property.
    property RulerAutoUnit: boolean read FRulerAutoUnit write SetRulerAutoUnit;

    /// Description:
    ///   Specifies the global measurement unit to be used in Diagram.
    ///   For backward compatibility, this property doesn't affect display unit of rulers.
    ///   To make this property also affect ruler units, set RulerAutoUnit property to true.
    property MeasUnit: TDiagramMeasUnit read GetMeasUnit write SetMeasUnit;

    /// Description:
    ///   If WheelZoom is true, the diagram can be zoomed out/in when user press Ctrl key and use mouse wheel.
    ///   This property is false by default, for backward compatibility.    
    property WheelZoom: boolean read FWheelZoom write FWheelZoom;

    /// Description:                 
    ///   WheelZoomIncrement specifies how much the Zoom value will be increased/decreased when
    ///   the end-user mouses mouse wheel up/down.
    property WheelZoomIncrement: integer read FWheelZoomIncrement write FWheelZoomIncrement;

    /// Description:
    ///   Specifies the minimum allowed zoom value when end-user uses mouse whell to zoom in/out.   
    property WheelZoomMin: integer read FWheelZoomMin write FWheelZoomMin;

    /// Description:
    ///   Specifies the maximum allowed zoom value when end-user uses mouse whell to zoom in/out.
    property WheelZoomMax: integer read FWheelZoomMax write FWheelZoomMax;

    /// Description:
    ///   OnInsertBlock event is fired whenever a block is inserted in the diagram.
    property OnInsertBlock: TBlockEvent read FOnInsertBlock write FOnInsertBlock;

    /// Description:
    ///   OnInsertDControl event is fired whenever a diagram control is inserted in the diagram.
    property OnInsertDControl: TDControlEvent read FOnInsertDControl write FOnInsertDControl;

    /// Description:
    ///   OnRemoveDControl event is called whenever a diagram control is removed from the diagram component.
    property OnRemoveDControl: TDCOntrolEvent read FOnRemoveDControl write FOnRemoveDControl;

    /// Description:
    ///   OnSelectDControl event is fired whenever a diagram control is selected in the diagram.
    property OnSelectDControl: TDControlEvent read FOnSelectDControl write FOnSelectDControl;

    /// Description:
    ///   OnUnselectDControl event is fired whenever a diagram control is unselected in the diagram.   
    property OnUnselectDControl: TDControlEvent read FOnUnselectDControl write FOnUnselectDControl;

    /// Description:
    ///   OnInsertLink event is fired whenever a line is inserted in the diagram.
    property OnInsertLink: TLinkEvent read FOnInsertLink write FOnInsertLink;
    
    /// Description:
    ///   OnSelectionChanging event is fired whenever the one or more blocks are about to be selected/unselected.
    property OnSelectionChanging: TNotifyEvent read FOnSelectionChanging write FOnSelectionChanging;
    
    /// Description:
    ///   OnSelectionChanged event is fired by the diagram when one or more blocks are selected/unselected.
    property OnSelectionChanged: TNotifyEvent read FOnSelectionChanged write FOnSelectionChanged;
    
    /// Description:
    ///   OnModified event is fired when a change is made to diagram.
    property OnModified: TNotifyEvent read FOnModified write FOnModified;

    /// Description:
    ///   Use OnDrawBlock event if you want to perform custom drawing for some blocks.
    property OnDrawBlock: TDrawBlockEvent read FOnDrawBlock write FOnDrawBlock;

    /// Description:
    ///   Use OnDrawShape event whenever you want to perform custom drawing of the shape of a block.
    ///   By using OnDrawShape (instead of OnDrawBlock), you cannot worry about setting Pen, Brush,
    ///   or other canvas settings. You just draw the shape using the current Canvas settings.
    ///   The diagram component handles all the canvas settings for you, based on the block properties
    ///   (shadow, pen, brush, etc.)
    property OnDrawShape: TDrawBlockEvent read FOnDrawShape write FOnDrawShape;

    /// Description:
    ///   Use OnGetSurroundPts event to define a surround region for a specific block.
    ///   The diagram component does it by default, but if necessary, you can define your own surround region.
    ///   The diagram component uses the surround region to know if a specific point is inside or outside the block.
    //    For example, for drawing the background of the block (images, etc.), or for selecting the block.   
    property OnGetSurroundPts: TGetSurroundPtsEvent read FOnGetSurroundPts write FOnGetSurroundPts;

    /// Description:
    ///   OnDControlMouseDown is fired when the mouse button is pressed while cursor is over a control in diagram.
    property OnDControlMouseDown: TDControlMouseEvent read FOnDControlMouseDown write FOnDControlMouseDown;

    /// Description:
    ///   OnDControlMouseUp is fired when the mouse button is released while cursor is over a control in diagram.      
    property OnDControlMouseUp: TDControlMouseEvent read FOnDControlMouseUp write FOnDControlMouseUp;

    /// Description:
    ///   OnDControlMouseMove is fired when the mouse cursor is moved over a diagram control.
    property OnDControlMouseMove: TDControlMouseMoveEvent read FOnDControlMouseMove write FOnDControlMouseMove;

    /// Description:
    ///   OnBlockMouseEnter event is called whenever the mouse enters a diagram block.
    property OnBlockMouseEnter: TBlockEvent read FOnBlockMouseEnter write FOnBlockMouseEnter;

    /// Description:
    ///   OnBlockMouseLeave event is called whenever the mouse leaves a diagram block.
    property OnBlockMouseLeave: TBlockEvent read FOnBlockMouseLeave write FOnBlockMouseLeave;

    /// Description:
    ///   OnDControlClick is fired when a diagram control is clicked by the user.
    property OnDControlClick: TDControlEvent read FOnDControlClick write FOnDControlClick;

    /// Description:
    ///   OnDControlDblClick is fired when a diagram control is double clicked by the user.
    property OnDControlDblClick: TDControlEvent read FOnDControlDblClick write FOnDControlDblClick;

    /// Description:
    ///   OnLinkAdded event is fired when a line is linked to a block by the user (or by Delphi code).
    property OnLinkAdded: TDiagramLinkEvent read FOnLinkAdded write FOnLinkAdded;
              
    /// Description:
    ///   OnLinkRemoved event is fired when a line is unlinked from a block by the user (or by Delphi code).
    property OnLinkRemoved: TDiagramLinkEvent read FOnLinkRemoved write FOnLinkRemoved;

    /// Description:
    ///   OnAcceptAnchor event is fired whenever a linkpoint is being linked (anchored) to another link point.
    ///   You can also use this event to avoid the link to be performed.
    property OnAcceptAnchor: TAcceptAnchorEvent read FOnAcceptAnchor write FOnAcceptAnchor;

    /// Description:
    ///   OnAcceptLink event is fired whenever a link between two link points is about to be made (through a line object).
    ///   You can also use this event to avoid the link to be performed.
    property OnAcceptLink: TAcceptLinkEvent read FOnAcceptLink write FOnAcceptLink;

    /// Description:
    ///   OnBeforeMove event is fired when a block selection (one or more blocks) is about to be moved by the end-user.
    property OnBeforeMove: TNotifyEvent read FOnBeforeMove write FOnBeforeMove;
    
    /// Description:
    ///   OnMoving event is called when the end-user is moving the selected blocks (one or more blocks).
    property OnMoving: TDiagramPosEvent read FOnMoving write FOnMoving;

    /// Description:
    ///   OnAfterMove event is fired after a block selection (one or more blocks) is moved by the end-user.
    property OnAfterMove: TNotifyEvent read FOnAfterMove write FOnAfterMove;

    /// Description:
    ///   OnBeforeResize event is fired when a block is about to be resized by the end-user.
    property OnBeforeResize: TNotifyEvent read FOnBeforeResize write FOnBeforeResize;

    /// Description:
    ///   OnDControlResizing event is called when the end-user is resizing a block.
    property OnDControlResizing: TDiagramPosEvent read FOnDControlResizing write FOnDControlResizing;

    /// Description:
    ///   OnAfterResize event is fired after a block is resized by the end-user.
    property OnAfterResize: TNotifyEvent read FOnAfterResize write FOnAfterResize;

    /// Description:
    ///   OnBeforePaint event is called before the painting routines of Diagram are called.
    ///   By handling this event you can draw directly into the diagram canvas.
    ///   All drawing performed in this event will be in the background of diagram.
    property OnBeforePaint: TDiagramPaintEvent read FOnBeforePaint write FOnBeforePaint;

    /// Description:
    ///   OnAfterPaint event is called after the painting routines of Diagram are called.
    ///   By handling this event you can draw directly into the diagram canvas.
    ///   All drawing performed in this event will be over all other diagram objects.    
    property OnAfterPaint: TDiagramPaintEvent read FOnAfterPaint write FOnAfterPaint;

    /// Description:
    ///   Use OnDrawLinkPoint event to perform custom drawing of link points.   
    property OnDrawLinkPoint: TDiagramDrawLinkPointEvent read FOnDrawLinkPoint write FOnDrawLinkPoint;

    /// Description:
    ///   OnCloseEditor event is fired whenever the text editor of the block is closed.
    ///   ATextCell parameter points to the text cell that was being edited.
    ///   AUpdate parameter indicates if the text was updated (user pressed Ctrl+Enter or
    ///   clicked in another area of diagram) or not updated (user pressed Esc).
    property OnCloseEditor: TDiagramCloseEditorEvent read FOnCloseEditor write FOnCloseEditor;

    /// Description:
    ///   Use OnAfterDrawBlock event if you want to perform custom drawing for some blocks,
    ///   but after the default drawing is performed.
    property OnAfterDrawBlock: TDrawBlockEvent read FOnAfterDrawBlock write FOnAfterDrawBlock;
    property OnBeforeCreateControl: TBeforeCreateControlEvent read FOnBeforeCreateControl write FOnBeforeCreateControl;

    property Align stored IsDesignTime;
    property Left stored IsDesignTime;
    property Width stored IsDesignTime;
    property Top stored IsDesignTime;
    property Height stored IsDesignTime;
    property Anchors stored IsDesignTime;
    property BevelEdges stored IsDesignTime;
    property BevelInner;
    property BevelOuter;       
    property BevelKind stored IsDesignTime;
    property BevelWidth stored IsDesignTime;
    property BiDiMode stored IsDesignTime;

    /// Description:
    ///   Defines the style of the border of diagram.
    property BorderStyle: TBorderStyle read FBorderStyle write SetBorderStyle;
    property Constraints stored IsDesignTime;
    property DockSite stored IsDesignTime;
    property DragCursor stored IsDesignTime;
    property DragKind stored IsDesignTime;
    property DragMode stored IsDesignTime;
    property Enabled stored IsDesignTime;
    property Color nodefault;
    property Ctl3D stored IsDesignTime default false;
    property Font;
    property ParentBiDiMode stored IsDesignTime;
    {$IFDEF DELPHI7_LVL}
    property ParentBackground stored IsDesignTime default False;
    {$ENDIF}
    property ParentColor;
    property ParentCtl3D default false;
    property ParentFont;
    property ParentShowHint stored IsDesignTime;
    property PopupMenu stored StoreDesignProperty;
    property ShowHint stored IsDesignTime;
    property TabOrder stored IsDesignTime;
    property TabStop stored IsDesignTime;
    property Visible stored IsDesignTime;
    property OnCanResize;
    property OnClick;
    property OnConstrainedResize;
    property OnContextPopup;
    property OnDblClick;
    property OnDockDrop;
    property OnDockOver;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnMouseWheel;
    property OnMouseWheelDown;
    property OnMouseWheelUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;

  /// Description:
  ///   For internal use only.
  TStreamDiagram = class(TatDiagram)
  private
    procedure WMNCPaint(var Message: TMessage); message WM_NCPAINT;
  protected
  public
    /// Description:
    ///   For internal use only.
    procedure PaintWindow(DC: HDC); override;

    /// Description:
    ///   For internal use only.
    constructor Create(Owner: TComponent); override;
  end;

  {Registering system classes}

  /// Description:
  ///   TRegDControl holds information about a registered block in the diagram system. A registered block is a
  ///   block which is available in toolbar to be inserted by the end-user.
  TRegDControl = class(TCollectionItem)
  private
    FCaption: string;
    FDControlClass: TDiagramControlClass;
    FDControlID: string;
    FCategory: string;
    FData: string;
    FGlyph: TPicture;
    FUseGlyph: boolean;
    procedure SetGlyph(const Value: TPicture);
    property Data: string read FData write FData;
  public

    /// Description:
    ///   Creates the object.
    constructor Create(Collection: TCollection); override;
    destructor Destroy; override;

    /// Description:
    ///   DControlClass property holds the class of a TDiagramControl which will be created for the registered
    ///   block. For example, when registering a new block in the diagram toolbar, the DControlClass property
    ///   can hold a TDiagramBlock class (a TDiagramBlock will be created), a TFlowActionBlock class, and so on.
    ///   You can also use a class created by yourself, if it descends from TDiagramControl class.
    property DControlClass: TDiagramControlClass read FDControlClass write FDControlClass;

    /// Description:
    ///   DControlID is the unique identifier for the registered block. In general, it will hold the name of the
    ///   class provided by DControlClass property. However, if you want to registered several blocks in the
    ///   toolbar which are the same class, then DControlID property can be used to provide a different ID for each
    ///   of those registered blocks.
    property DControlID: string read FDControlID write FDControlID;

    /// Description:
    ///   Category property holds a string which defined the category where the registered block belongs to.
    ///   For example, "Flowchart" or "Electric".
    property Category: string read FCategory write FCategory;

    /// Description:
    ///   Caption property holds the display label for the registered block. For example, "resistor block".
    ///   It is used, for example, for hinting in the toolbar when the mouse is over a tool button related to
    ///   the registered block.
    property Caption: string read FCaption write FCaption;
    property Glyph: TPicture read FGlyph write SetGlyph;
    property UseGlyph: boolean read FUseGlyph write FUseGlyph;
  end;

  /// Description:
  ///   TRegDControls holds a collection of all registered blocks in the diagram toolbar system.
   TRegDControls = class(TOwnedCollection)
   private
      function GetItem(Index: integer): TRegDControl;
   public

    /// Description:
    ///   Use FindByID method to find a registered block in the collection by its DControlID property.
      function FindByID(const ADControlID: string): TRegDControl;

    /// Description:
    ///   Use Add function to add a new TRegDControl object to the collection.
      function Add: TRegDControl;

    /// Description:
    ///   Use Items property to retrieve a specific TRegDControl object from the collection.
      property Items[Index: integer]: TRegDControl read GetItem; default;
   end;

/// Summary:
///   Registers a new block class.
/// Description:
///   Use RegisterDControl procedure to register a new block class into diagram registering system.
///   You must pass the new block class in ADControlClass parameter. If you don't specify ADControlID
///   parameter, the block id will have the same name as the class name. You can specify a different
///   block id. It is useful when registering two different blocks of the same class. ACaption property
///   contains the text that will be shown as a hint in diagram toolbar. Finally ACategory parameter
///   contains the category name for the block (for example, 'Electric', 'Flowchart' or 'Myblock library').
///   For displaying the correct icon for the block in diagram toolbar, create a bitmap resource and
///   give its name as the same name as block id preceded by 'blk_'. For example, if you are register
///   a new block class TMyBlock without specifying a block id, the block id will be 'TMyBlock'.
///   Create a resource name called 'blk_TMyBlock' as a 24x24 bitmap. This bitmap will be used
///   for the diagram block icon in diagram toolbar.
procedure RegisterDControl(ADControlClass: TDiagramControlClass;
  ADControlID: string = ''; ACaption: string = ''; ACategory: string = '');

/// Description:
///   Register the standard diagram controls (TDiagramBlock, TTextBlock, the lines, etc.).
///   You don't need to call this procedure, since it's called by default in the initialization
///   section of atDiagram.pas unit.
procedure RegisterStandardDControls;


/// Description:
///   Returns a TDiagramPrinters collection with a list of all available printers for diagram printing.
function DPrinters: TDiagramPrinters;

var

/// Summary:
///   Clipboard format.
/// Description:
///   CF_DIAGRAM is the clipboard format for diagram.
  CF_DIAGRAM: Word;

/// Description:
///   Global variable which holds a TRegDControls collection - it contains all the available
///   diagram controls (blocks and links) registered.
  RegDControlList: TRegDControls = nil;

resourcestring

/// Summary:
///   String resource.
  SRegisterDControlError = 'Cannot register DControl "%s".';
const
  SDiagramOwnerName = '__DSOwner___';

implementation

uses
  Dialogs, Consts, Math, FlatSB, WinSpool, ShellApi,
  {$IFDEF DELPHI2007_LVL}
  UxTheme, DwmApi, Types,
  {$ENDIF}
  DiagramPreview,
  fDgrPageSettings,
  fDgrPrintDialog,
  fLibItemEditor
  ;

{$R atDiagram.res}

type
  TPaperInfoRec = record
    PID: integer;
    Name: string;
    W: integer;
    H: integer;
  end;

const
  PaperInfoArray: array[0..67] of TPaperInfoRec = (
    (PID:1;  Name: 'Letter';                    W:2159; H:2794),
    (PID:2;  Name: 'Letter Small';              W:2159; H:2794),
    (PID:3;  Name: 'Tabloid';                   W:2794; H:4318),
    (PID:4;  Name: 'Ledger';                    W:4318; H:2794),
    (PID:5;  Name: 'Legal';                     W:2159; H:3556),
    (PID:6;  Name: 'Statement';                 W:1397; H:2159),
    (PID:7;  Name: 'Executive';                 W:1842; H:2667),
    (PID:8;  Name: 'A3';                        W:2970; H:4200),
    (PID:9;  Name: 'A4';                        W:2100; H:2970),
    (PID:10; Name: 'A4 Small';                  W:2100; H:2970),
    (PID:11; Name: 'A5';                        W:1480; H:2100),
    (PID:12; Name: 'B4 (JIS)';                  W:2500; H:3540),
    (PID:13; Name: 'B5 (JIS)';                  W:1820; H:2570),
    (PID:14; Name: 'Folio';                     W:2159; H:3302),
    (PID:15; Name: 'Quarto';                    W:2150; H:2750),
    (PID:16; Name: '10x14';                     W:2540; H:3556),
    (PID:17; Name: '11x17';                     W:2794; H:4318),
    (PID:18; Name: 'Note';                      W:2159; H:2794),
    (PID:19; Name: 'Envelope #9';               W:984;  H:2254),
    (PID:20; Name: 'Envelope #10';              W:1048; H:2413),
    (PID:21; Name: 'Envelope #11';              W:1143; H:2635),
    (PID:22; Name: 'Envelope #12';              W:1207; H:2794),
    (PID:23; Name: 'Envelope #14';              W:1270; H:2921),
    (PID:24; Name: 'C size sheet';              W:4318; H:5588),
    (PID:25; Name: 'D size sheet';              W:5588; H:8636),
    (PID:26; Name: 'E size sheet';              W:8636; H:11176),
    (PID:27; Name: 'Envelope DL';               W:1100; H:2200),
    (PID:28; Name: 'Envelope C5';               W:1620; H:2290),
    (PID:29; Name: 'Envelope C3';               W:3240; H:4580),
    (PID:30; Name: 'Envelope C4';               W:2290; H:3240),
    (PID:31; Name: 'Envelope C6';               W:1140; H:1620),
    (PID:32; Name: 'Envelope C65';              W:1140; H:2290),
    (PID:33; Name: 'Envelope B4';               W:2500; H:3530),
    (PID:34; Name: 'Envelope B5';               W:1760; H:2500),
    (PID:35; Name: 'Envelope B6';               W:1760; H:1250),
    (PID:36; Name: 'Envelope';                  W:1100; H:2300),
    (PID:37; Name: 'Envelope Monarch';          W:984;  H:1905),
    (PID:38; Name: '6 3/4 Envelope';            W:920;  H:1651),
    (PID:39; Name: 'US Std Fanfold';            W:3778; H:2794),
    (PID:40; Name: 'German Std Fanfold';        W:2159; H:3048),
    (PID:41; Name: 'German Legal Fanfold';      W:2159; H:3302),
    (PID:42; Name: 'B4 (ISO)';                  W:2500; H:3530),
    (PID:43; Name: 'Japanese Postcard';         W:1000; H:1480),
    (PID:44; Name: '9x11';                      W:2286; H:2794),
    (PID:45; Name: '10x11';                     W:2540; H:2794),
    (PID:46; Name: '15x11';                     W:3810; H:2794),
    (PID:47; Name: 'Envelope Invite';           W:2200; H:2200),
    (PID:50; Name: 'Letter Extra';              W:2355; H:3048),
    (PID:51; Name: 'Legal Extra';               W:2355; H:3810),
    (PID:52; Name: 'Tabloid Extra';             W:2969; H:4572),
    (PID:53; Name: 'A4 Extra';                  W:2354; H:3223),
    (PID:54; Name: 'Letter Transverse';         W:2101; H:2794),
    (PID:55; Name: 'A4 Transverse';             W:2100; H:2970),
    (PID:56; Name: 'Letter Extra Transverse';   W:2355; H:3048),
    (PID:57; Name: 'Super A';                   W:2270; H:3560),
    (PID:58; Name: 'Super B';                   W:3050; H:4870),
    (PID:59; Name: 'Letter Plus';               W:2159; H:3223),
    (PID:60; Name: 'A4 Plus';                   W:2100; H:3300),
    (PID:61; Name: 'A5 Transverse';             W:1480; H:2100),
    (PID:62; Name: 'B5 (JIS) Transverse';       W:1820; H:2570),
    (PID:63; Name: 'A3 Extra';                  W:3220; H:4450),
    (PID:64; Name: 'A5 Extra';                  W:1740; H:2350),
    (PID:65; Name: 'B5 (ISO) Extra';            W:2010; H:2760),
    (PID:66; Name: 'A2';                        W:4200; H:5940),
    (PID:67; Name: 'A3 Transverse';             W:2970; H:4200),
    (PID:68; Name: 'A3 Extra Transverse';       W:3220; H:4450),
    (PID:69; Name: 'Japanese Double Postcard';  W:2000; H:1480),
    (PID:70; Name: 'A6';                        W:1050; H:1480)
    );

const
  MinMinWidth = 5;
  MinMinHeight = 5;
  _PolygonOffset = 1000;

const
  {Special handle types}
  {Positions and cursors here must be 45 degrees clockwise. Do not change this order.
   TCustomDiagramBlock.UpdateHandleCursor relies on this order}
  hpLeftTop = 1;
  hpTop = 2;
  hpRightTop = 3;
  hpRight = 4;
  hpRightBottom = 5;
  hpBottom = 6;
  hpLeftBottom = 7;
  hpLeft = 8;
  hpRotation = 9;
  hpRotCenter = 10;

var
  _DPrinters: TDiagramPrinters = nil;

function DPrinters: TDiagramPrinters;
begin
  if _DPrinters = nil then
    _DPrinters := TDiagramPrinters.Create;
  result := _DPrinters;
end;

procedure RegisterDControl(ADControlClass: TDiagramControlClass;
  ADControlID: string = ''; ACaption: string = ''; ACategory: string = '');
var ARegDControl: TRegDControl;
begin
  if ADControlClass = nil then
    Exit;
  if ADControlID = '' then
    ADControlID := ADControlClass.ClassName;
  if ACaption = '' then
    ACaption := ADControlID;
  ARegDControl := RegDControlList.FindByID(ADControlID);
  if ARegDControl = nil then
    ARegDControl := RegDControlList.Add;
  with ARegDControl do
  begin
     DControlID := ADControlID;
     DControlClass := ADControlClass;
     Caption := ACaption;
     Category := ACategory;
  end;
  if GetClass(ADControlClass.ClassName) = nil then
    RegisterClass(ADControlClass);
end;

procedure RegisterStandardDControls;
begin
  RegisterDControl(TDiagramBlock, '', '', '');
  RegisterDControl(TDiagramLineJoin, '', '', '');
  RegisterDControl(TTextBlock, '', 'Text', '');
  RegisterDControl(TDiagramLine, '', 'Line', 'Lines');
  RegisterDControl(TDiagramSideLine, '', 'Side line', 'Lines');
  RegisterDControl(TDiagramPolyLine, '', 'Poly line', 'Lines');
  RegisterDControl(TPolygonBlock, '', 'Polygon', '');
  RegisterDControl(TDiagramArc, '', 'Arc', 'Lines');
  RegisterDControl(TDiagramBezier, '', 'Bezier', 'Lines');
end;

procedure _DrawFocusRect(Canvas: TCanvas; ARect: TRect);
var
  AOldBrush: TBrush;
  AOldPen: TPen;
begin
  AOldBrush := TBrush.Create;
  AOldPen := TPen.Create;
  try
    AOldBrush.Assign(Canvas.Brush);
    AOldPen.Assign(Canvas.Pen);
    Canvas.Brush.Style := bsClear;
    Canvas.Pen.Mode := pmNot;
    Canvas.Pen.Style := psDot;
    Canvas.Rectangle(ARect);
  finally
    Canvas.Pen.Assign(AOldPen);
    Canvas.Brush.Assign(AOldBrush);
    AOldBrush.Free;
    AOldPen.Free;
  end;
end;

function MixTransp(Transp1, Transp2: integer): integer;
begin
  result := 100 - ((100 - Transp1) * (100 - Transp2) div 100);
end;

function EmptyPicture(APic: TPicture): boolean;
begin
  Result := (APic = nil) or ((APic.Width = 0) and (APic.Height = 0));
end;

function EmptyGraphic(APic: TGraphic): boolean;
begin
  Result := (APic = nil) or ((APic.Width = 0) and (APic.Height = 0));
end;

function EmptyMetafile(AMeta: TMetafile): boolean;
begin
  Result := (AMeta = nil) or ((AMeta.Width = 0) and (AMeta.Height = 0));
end;

function CalculateItemsRect(AItems: TDControlList;
  OnlyBlocks: boolean; AAngle: double = 0): TSquare;
var
  c: integer;
  MinLeft: double;
  MinTop: double;
  MaxRight: double;
  MaxBottom: double;
  ALine: TCustomDiagramLine;
  PA: TDotArray;
  MX, MY: double;
  ADrawer: TBlockDrawer;
begin
  {Retrieve surround points for selected objects}
  SetLength(PA, 0);
  for c := 0 to AItems.Count -1 do if AItems[c].DControl <> nil then
  begin
    if AItems[c].DControl is TCustomDiagramBlock then
    begin
      PA := ConcatPointsX(PA,
        TCustomDiagramBlock(AItems[c].DControl).AngledSurroundRect(
        TCustomDiagramBlock(AItems[c].DControl).Angle));
    end else
    if (AItems[c].DControl is TCustomDiagramLine) and not OnlyBlocks then
    begin
      ALine := TCustomDiagramLine(AItems[c].DControl);
      PA := ConcatPointsX(PA, ALine.Points);
    end;
  end;

  {Rotate points}
  ADrawer := TBlockDrawer.Create;
  try
    MX := 0;
    MY := 0;
    if (AAngle <> 0) and (Length(PA) > 0) then
    begin
      for c := 0 to Length(PA) - 1 do
      begin
        MX := MX + PA[c].X;
        MY := MY + PA[c].Y;
      end;
      MX := MX / Length(PA);
      MY := MY / Length(PA);

      ADrawer.PushState;
      ADrawer.RotationType := rtRotationCenter;
      ADrawer.RotationCenter := Dot(MX, MY);
      //ADrawer.RotationCenter := Point(0, 0);
      ADrawer.Angle := -AAngle;

      for c := 0 to Length(PA) - 1 do
        PA[c] := ADrawer.RotX(PA[c]);
      ADrawer.PopState;
    end;

    {Retrieve surround rect}
    MinLeft := MaxInt;
    MinTop := MaxInt;
    MaxRight := 0;
    MaxBottom := 0;
    for c := 0 to Length(PA) - 1 do
    begin
      if PA[c].X < MinLeft then
        MinLeft := PA[c].X;
      if PA[c].Y < MinTop then
        MinTop := PA[c].Y;
      if PA[c].X > MaxRight then
        MaxRight := PA[c].X;
      if PA[c].Y > MaxBottom then
        MaxBottom := PA[c].Y;
    end;
    result := Square(MinLeft, MinTop, MaxRight, MaxBottom);

    if AAngle <> 0 then
    begin
      ADrawer.PushState;
      ADrawer.RotationType := rtRotationCenter;
      ADrawer.RotationCenter := Dot(MX, MY);
      ADrawer.Angle := AAngle;
      result := ADrawer.RotByCenter(result);
      ADrawer.PopState;
    end;
  finally
    ADrawer.Free;
  end;
end;

{ TDiagramUndoItem }

constructor TDiagramUndoItem.Create(Collection: TCollection);
begin
  inherited Create(Collection);
  FState := TMemoryStream.Create;
end;

destructor TDiagramUndoItem.Destroy;
begin
  FState.Free;
  inherited;
end;

{ TDiagramUndoStack }

procedure TDiagramUndoStack.ClearUndoStack;
begin
  Clear;
  FCurrent := -1;
  PushState('');
end;

constructor TDiagramUndoStack.Create(ADiagram: TatDiagram);
begin
  inherited Create(TDiagramUndoItem);
  FDiagram := ADiagram;
  FCurrent := -1;
end;

function TDiagramUndoStack.NextUndoAction: string;
begin
  result := '';
  if (FCurrent > -1) and (FCurrent < Count) then
    result := TDiagramUndoItem(Items[FCurrent]).ActionName;
end;

function TDiagramUndoStack.NextRedoAction: string;
begin
  result := '';
  if (FCurrent >= -1) and (FCurrent < Count - 1) then
    result := TDiagramUndoItem(Items[FCurrent + 1]).ActionName;
end;

procedure TDiagramUndoStack.Undo;
begin
  if (FCurrent > 0) and (FCurrent <= Count) then
  begin
    FDiagram.Clear;
    With TDiagramUndoItem(Items[FCurrent - 1]) do
    begin
      FState.Position := 0;
      FDiagram.LoadFromStream(FState);
    end;
    dec(FCurrent);
  end;
end;

procedure TDiagramUndoStack.Redo;
begin
  if (FCurrent >= -1) and (FCurrent < Count - 1) then
  begin
    FDiagram.Clear;
    With TDiagramUndoItem(Items[FCurrent + 1]) do
    begin
      FState.Position := 0;
      FDiagram.LoadFromStream(FState);
    end;
    inc(FCurrent);
  end;
end;

procedure TDiagramUndoStack.PushState(const AActionName: string);
begin
  while FCurrent < Count - 1 do
    Items[Count - 1].Free;

  with TDiagramUndoItem(Add) do
  begin
    ActionName := AActionName;
    FDiagram.SaveToStream(FState);
    inc(FCurrent);
  end;
  if Count > 10 then
  begin
    Items[0].Free;
    dec(FCurrent);
  end;
end;

{ TCustomDiagramBlock }

procedure TCustomDiagramBlock.SelectedChanged;
begin
  if Assigned(FDiagram) then
    Redraw;
end;

constructor TCustomDiagramBlock.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);

  {this point is proportional to original rect}
  FRotCenter := Dot(50, 50);

  FStrings := TStringList.Create;
  TStringList(FStrings).OnChange := StringsChange;
  FBrush := TBrush.Create;
  FSelBrush := TBrush.Create;
  FPen := TPen.Create;
  FBrush.Color := clBlack;
  FBrush.Style := bsClear;
  FSelBrush.Color := clBlack;
  FSelBrush.Style := bsClear;
  FPen.Style := psSolid;
  FPen.Color := clBlack;
  FPen.OnChange := StyleChanged;
  FShadow := TBlockShadow.Create(Self);
  FBrush.OnChange := StyleChanged;
  FSelBrush.OnChange := StyleChanged;
  FGradient := TDiagramGradient.Create(Self);
  FBitmap := TBitmap.Create;
  FBitmap.OnChange := BitmapChange;
  FBitmapMode := bmStretch;
  FMetafile := TMetafile.Create;
  FMetafile.OnChange := MetafileChange;
  FPicture := TPicture.Create;
  FPicture.OnChange := PictureChange;
  FPictureMode := pmStretch;
  FColor := clWhite;
  FSelColor := clWhite;
  FShape := bsNoShape;
  FTransparent := False;
  FClipText := False;
  FRestrictions := [];
  FRotationStep := 5;
  FMinHeight := 40;
  FMinWidth := 40;
  TextCells.Add.SetBounds(0, 0, 100, 100);
  Width := 50;
  Height := 50;
  UpdateBlockHandles;
end;

procedure TCustomDiagramBlock.CreateGraphicLibObjects;
begin
  inherited;
  if FDgrDrawer <> nil then
  begin
    FDgrBitmap := FDgrDrawer.CreateGraphic;
    if not EmptyGraphic(FBitmap) then
      FDgrBitmap.Assign(FBitmap);
    FDgrMetafile := FDgrDrawer.CreateGraphic;
    if not EmptyMetafile(FMetafile) then
      FDgrMetafile.Assign(FMetafile);
    FDgrPicture := FDgrDrawer.CreateGraphic;
    if not EmptyPicture(FPicture) then
      FDgrPicture.Assign(FPicture);
  end;
end;

destructor TCustomDiagramBlock.Destroy;
begin
  FStrings.Free;
  FShadow.Free;
  FGradient.Free;
  FBitmap.Free;
  FMetafile.Free;
  FPicture.Free;
  FBrush.Free;
  FSelBrush.Free;
  FPen.Free;
  inherited;
end;

procedure TCustomDiagramBlock.DestroyGraphicLibObjects;
begin
  inherited;
  if FDgrBitmap <> nil then
    FreeAndNil(FDgrBitmap);
  if FDgrMetaFile <> nil then
    FreeAndNil(FDgrMetafile);
  if FDgrPicture <> nil then
    FreeAndNil(FDgrPicture);
end;

function TCustomDiagramBlock.GetBoundsRect: TSquare;
begin
  result := Square(Left, Top, Left + Width, Top + Height);
end;

procedure TCustomDiagramBlock.SetBounds(ALeft, ATop, AWidth, AHeight: number);
begin
  SetBoundsRect(Square(ALeft, ATop, ALeft + AWidth, ATop + AHeight));
end;

procedure TCustomDiagramBlock.SetBoundsRect(const Value: TSquare);
var
  AOldRect: TSquare;
  ANewRect: TSquare;
begin
  ANewRect := Square(Value.Left, Value.Top, Value.Right, Value.Bottom);
  if not IsSameRect(BoundsRect, ANewRect) then
  begin
    AOldRect := BoundsRect;
    FLeft := ANewRect.Left;
    FTop := ANewRect.Top;
    FWidth := ANewRect.Right - ANewRect.Left;
    FHeight := ANewRect.Bottom - ANewRect.Top;
    BoundsChanged(AOldRect, BoundsRect);
  end;
end;

function TCustomDiagramBlock.GetBottom: number;
begin
  result := Top + Height;
end;

function TCustomDiagramBlock.GetRight: number;
begin
  result := Left + Width;
end;

procedure TCustomDiagramBlock.BoundsChanged(AOld, ANew: TSquare);
begin
  if Assigned(FDiagram) then
  begin
    NotifyAnchoreds(acPosition);
    FDiagram.Modified;
    if not (csLoading in ComponentState) then
      Resize;
    if FDiagram.FUpdatingPos = 0 then
      FDiagram.Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetHeight(const Value: number);
begin
  SetBounds(FLeft, FTop, FWidth, Value);
end;

procedure TCustomDiagramBlock.SetLeft(const Value: number);
begin
  SetBounds(Value, FTop, FWidth, FHeight);
end;

procedure TCustomDiagramBlock.SetTop(const Value: number);
begin
  SetBounds(FLeft, Value, FWidth, FHeight);
end;

procedure TCustomDiagramBlock.SetWidth(const Value: number);
begin
  SetBounds(FLeft, FTop, Value, FHeight);
end;

procedure TCustomDiagramBlock.DoDrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);
begin
  DrawShape(AInfo, ABlockInfo);
end;

procedure TCustomDiagramBlock.DrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);

  procedure _DrawShapeDgr;
  var
    path: TDgrGraphicsPath;
  begin
    path := AInfo.DgrDrawer.CreatePath;
    try
      GetTranslatedBlockPath(path, AInfo.DgrDrawer);
      AInfo.DgrDrawer.Canvas.Path(path);
    finally
      path.Free;
    end;
  end;

  procedure _DrawShape;
  var
    P1, P2: TDot;
    Pts: TDotArray;
    RPts: TPointArray;
  begin
    RPts := nil;
    case Shape of
      bsRectangle:
        with AInfo.Drawer, OriginalRect do
          AutoRotRect(Left, Top, Right, Bottom);
      bsEllipse:
        with AInfo.Drawer, OriginalRect do
          AutoEllipse(Left, Top, Right, Bottom);
      bsRoundRect:
        with AInfo.Drawer, OriginalRect do
          AutoRoundRect(Left, Top, Right, Bottom, (Right - Left) / 4, (Bottom - Top) / 4);
      bsSquareRound:
        begin
          with AInfo.Drawer, OriginalRect do
          begin
            P1 := PP(Left, Top);
            P2 := PP(Right, Bottom);
          end;

          {we have to split the call in three different and put things in a temporary
           variable (Pts) to workaround an internal error in Delphi 7}
          Pts := GetPolyRoundRect(P1.X, P1.Y, P2.X, P2.Y, P2.Y - P1.Y, P2.Y - P1.Y);
          Pts := AInfo.Drawer.RotPoly(Pts);
          RPts := RoundDotArray(Pts);
          AInfo.Drawer.Canvas.Polygon(RPts);
        end;
      bsDiamond:
        begin
          SetLength(Pts, 4);
          Pts[0] := Dot(ABlockInfo.Rect.Left, (ABlockInfo.Rect.Bottom + ABlockInfo.Rect.Top) / 2);
          Pts[1] := Dot((ABlockInfo.Rect.Right + ABlockInfo.Rect.Left) / 2, ABlockInfo.Rect.Top);
          Pts[2] := Dot(ABlockInfo.Rect.Right, (ABlockInfo.Rect.Bottom + ABlockInfo.Rect.Top) / 2);
          Pts[3] := Dot((ABlockInfo.Rect.Right + ABlockInfo.Rect.Left) / 2, ABlockInfo.Rect.Bottom);
          AInfo.Drawer.DrawRotPoly(Pts);
        end;
    end;
  end;

begin
  if (FDiagram = nil) or not FDiagram.CustomDrawShape(Self, AInfo, ABlockInfo) then
  begin
    if PathDrawingMode then
      _DrawShapeDgr
    else
      _DrawShape;
  end;
end;

procedure TCustomDiagramBlock.DrawBitmap;
var
  bmp: TGraphic;
begin
  {Optimization issues}
  if PathDrawingMode and (FDgrBitmap <> nil) then
    bmp := FDgrBitmap
  else
    bmp := FBitmap;

  case BitmapMode of
    bmStretch: DrawGraphic(bmp, pmStretch);
    bmTile: DrawGraphic(bmp, pmTile);
    bmCenter: DrawGraphic(bmp, pmCenter);
  end;
end;

procedure TCustomDiagramBlock.DrawMetafile;
begin
  if PathDrawingMode then
    DrawGraphic(FDgrMetafile, pmStretch)
  else
    DrawGraphic(FMetafile, pmStretch);
end;

procedure TCustomDiagramBlock.DrawPicture;
begin
  if FPicture <> nil then
  begin
    if PathDrawingMode and not (FPicture.Graphic is TDgrGraphic) then
      DrawGraphic(FDgrPicture, PictureMode)
    else
      DrawGraphic(FPicture.Graphic, PictureMode);
  end;
end;

procedure TCustomDiagramBlock.DrawGraphic(AGraphic: TGraphic; AMode: TPictureMode);
begin
  if PathDrawingMode then
    InternalDrawGraphicDgr(FDgrDrawer, FDgrDrawer.DestRect, AGraphic, FDgrDrawer.Angle, AMode, Self.Transparency)
  else
    InternalDrawGraphic(Drawer.Canvas, Drawer.CurRect, AGraphic, Drawer.Angle, AMode);
end;

procedure TCustomDiagramBlock.InternalDrawGraphic(Canvas: TCanvas; ARect: TSquare;
  AGraphic: TGraphic; AAngle: double; AMode: TPictureMode);

  procedure DrawAngledPicture(DestRect: TSquare);
  var
    OGM: integer;
    XF,OXF: TXForm;
    R: TSquare;
  begin
    if AAngle = 0 then
    begin
      Canvas.StretchDraw(ToRect(DestRect), AGraphic);
    end
    else
    begin
      OGM := SetGraphicsMode(Canvas.Handle, GM_ADVANCED);
      try
        GetWorldTransform(Canvas.Handle, OXF);
        with XF do
        begin
          eM11 := cos(AAngle * pi / 180);
          eM12 := sin(AAngle * pi / 180);
          eM21 := -sin(AAngle * pi / 180);
          eM22 := cos(AAngle * pi / 180);
          eDx := DestRect.left + (DestRect.Right - DestRect.Left) / 2;
          eDy := DestRect.top + (DestRect.Bottom - DestRect.Top) / 2;
        end;
        DestRect.Right := DestRect.Right - DestRect.Left;
        DestRect.Bottom := DestRect.Bottom - DestRect.Top;
        DestRect.Left := 0;
        DestRect.Top := 0;
        R.Left := -(DestRect.Right - DestRect.Left) / 2;
        R.Right := (DestRect.Right - DestRect.Left) / 2;
        R.Top := -(DestRect.Bottom - DestRect.Top) / 2;
        R.Bottom := (DestRect.Bottom - DestRect.Top) / 2;
        SetWorldTransform(Canvas.Handle, XF);
        try
          Canvas.StretchDraw(ToRect(R), AGraphic);
        finally
          SetWorldTransform(Canvas.Handle, OXF);
        end;
      finally
        SetGraphicsMode(Canvas.Handle, OGM);
      end;
    end;
  end;

  procedure DrawCenterPicture;
  var
    SrcRect: TSquare;
    DestRect: TSquare;
    ZoomedW, ZoomedH: number;
  begin
    SrcRect := FDiagram.ClientToCanvas(Square(0, 0, AGraphic.Width, AGraphic.Height));
    ZoomedW := SrcRect.Right - SrcRect.Left;
    ZoomedH := SrcRect.Bottom - SrcRect.Top;
    SrcRect := Square(0, 0, AGraphic.Width, AGraphic.Height);
    DestRect := Square(
      (ARect.Right + ARect.Left) / 2 - ZoomedW / 2,
      (ARect.Bottom + ARect.Top) / 2 - ZoomedH / 2,
      (ARect.Right + ARect.Left) / 2 + ZoomedW / 2,
      (ARect.Bottom + ARect.Top) / 2 + ZoomedH / 2
      );
    DrawAngledPicture(DestRect);
  end;

  procedure DrawStretchPicture;
  begin
    DrawAngledPicture(ARect);
  end;

  procedure DrawTilePicture;
  var
    SrcRect: TRect;
    StepX, StepY: number;
    AWidth, AHeight: number;
    OGM: integer;
    XF,OXF: TXForm;
    AMinLeft, AMinTop: number;
    AMaxRight, AMaxBottom: number;

    procedure BuildTilePicture;
    var
      ox, oy: number;
    begin
      ox := AMinLeft;
      oy := AMinTop;
      while oy < AMaxBottom do
      begin
        while ox < AMaxRight do
        begin
          Canvas.StretchDraw(ToRect(Square(ox, oy, ox + StepX, oy + StepY)), AGraphic);
          ox := ox + StepX;
        end;
        ox := AMinLeft;
        oy := oy + StepY;
      end;
    end;

  begin
    SrcRect := FDiagram.ClientToCanvas(Rect(0, 0, AGraphic.Width, AGraphic.Height));
    StepX := Abs(SrcRect.Right - SrcRect.Left);
    StepY := Abs(SrcRect.Bottom - SrcRect.Top);
    if StepX = 0 then
      StepX := 1;
    if StepY = 0 then
      StepY := 1;
    SrcRect := Rect(0, 0, AGraphic.Width, AGraphic.Height);
    AWidth := ARect.Right - ARect.Left;
    AHeight := ARect.Bottom - ARect.Top;

    if AAngle = 0 then
    begin
      AMinLeft := ARect.Left;
      AMinTop := ARect.Top;
      AMaxRight := AMinLeft + AWidth;
      AMaxBottom := AMinTop + AHeight;
      BuildTilePicture;
    end
    else
    begin
      AMaxRight := (AWidth / 2);
      AMaxBottom := (AHeight / 2);
      AMinLeft := -AMaxRight;
      AMinTop := -AMaxBottom;
      OGM := SetGraphicsMode(Canvas.Handle, GM_ADVANCED);
      try
        GetWorldTransform(Canvas.Handle, OXF);
        with XF do
        begin
          eM11 := cos(AAngle * pi / 180);
          eM12 := sin(AAngle * pi / 180);
          eM21 := -sin(AAngle * pi / 180);
          eM22 := cos(AAngle * pi / 180);
          eDx := ARect.left + (ARect.Right - ARect.Left) / 2;
          eDy := ARect.top + (ARect.Bottom - ARect.Top) / 2;
        end;
        SetWorldTransform(Canvas.Handle, XF);
        try
          BuildTilePicture;
        finally
          SetWorldTransform(Canvas.Handle, OXF);
        end;
      finally
        SetGraphicsMode(Canvas.Handle, OGM);
      end;
    end;
  end;

begin
  if (FDiagram <> nil) and not EmptyGraphic(AGraphic) then
    case AMode of
      pmStretch: DrawStretchPicture;
      pmTile: DrawTilePicture;
      pmCenter: DrawCenterPicture;
    end;
end;

procedure TCustomDiagramBlock.InternalDrawGraphicDgr(ADrawer: TDgrBlockDrawer; ARect: TSquare;
  AGraphic: TGraphic; AAngle: double; AMode: TPictureMode; ATransparency: integer);
var
  ANewRect: TSquare;
  AWidth, AHeight: integer;
  ox, oy: double;
begin
  if (FDiagram <> nil) and not EmptyGraphic(AGraphic) then
  begin
    {save state because we're going to rotate the graphic device}
    ADrawer.Canvas.SaveDrawingState;
    try
      {rotate the graphic device. We must prepend the transformation, because the graphics device is
       already transformed because of the zoom}
      ADrawer.Canvas.RotateTransform(AAngle, ADrawer.RotationCenter);

      {draw the picture in destrect coordinates}
      case AMode of
        pmStretch:
          ADrawer.Canvas.StretchDraw(ADrawer.DestRect, AGraphic, ATransparency, BitmapTransparent);
        pmCenter:
          begin
            {First calculate the center of the block}
            ANewRect.Left := (ADrawer.DestRect.Left + ADrawer.DestRect.Right) / 2;
            ANewRect.Top := (ADrawer.DestRect.Top + ADrawer.DestRect.Bottom) / 2;

            {Now move the rect to hold the whole picture without stretching}
            AWidth := AGraphic.Width;
            AHeight := AGraphic.Height;
            ANewRect.Left := ANewRect.Left - AWidth / 2;
            ANewRect.Top := ANewRect.Top - AHeight / 2;
            ANewRect.Right := ANewRect.Left + AWidth;
            ANewRect.Bottom := ANewRect.Top + AHeight;
            ADrawer.Canvas.StretchDraw(ANewRect, AGraphic, ATransparency, BitmapTransparent);
          end;
        pmTile:
          begin
            AWidth := AGraphic.Width;
            AHeight := AGraphic.Height;
            ox := ADrawer.DestRect.Left;
            oy := ADrawer.DestRect.Top;
            if (AWidth > 0) and (AHeight > 0) then
              while oy < ADrawer.DestRect.Bottom do
              begin
                while ox < ADrawer.DestRect.Right do
                begin
                  ADrawer.Canvas.StretchDraw(Square(ox, oy, ox + AWidth, oy + AHeight), AGraphic,
                    ATransparency, BitmapTransparent);
                  ox := ox + AWidth;
                end;
                ox := ADrawer.DestRect.Left;
                oy := oy + AHeight;
              end;
          end;
      end;
    finally
      ADrawer.Canvas.RestoreDrawingState;
    end;
  end;
end;

procedure TCustomDiagramBlock.DrawBlock(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);

  procedure _DrawBlock;
  var
    AShadowRect: TSquare;
    ASaveRect: TSquare;
    SavePen: TPen;
    SaveBrush: TBrush;
    saveDgrRect: TSquare;
    saveDgrPen: TDgrPen;
    saveDgrBrush: TDgrBrush;
  begin
    {Save pens for drawer and gpdrawer}
    SavePen := TPen.Create;
    SaveBrush := TBrush.Create;
    SavePen.Assign(AInfo.Canvas.Pen);
    SaveBrush.Assign(AInfo.Canvas.Brush);
    if PathDrawingMode then
    begin
      saveDgrPen := AInfo.DgrDrawer.CreatePen;
      saveDgrBrush := AInfo.DgrDrawer.CreateBrush;
      saveDgrPen.Assign(AInfo.DgrDrawer.Canvas.Pen);
      saveDgrBrush.Assign(AInfo.DgrDrawer.Canvas.Brush);
    end
    else
    begin
      saveDgrPen := nil;
      saveDgrBrush := nil;
    end;

    try
      {NEXT STEP - Draw Shadow}
      if Shadow.Visible then
      begin
        {Prepare the non-gdiplus drawer (Drawer) to draw the Shadow. Basically, we need to set
         brush to shadow's color, and calculate the rectangle of shadow by offseting the original rectangle}
        AInfo.Drawer.Canvas.Pen.Style := psClear;
        AInfo.Drawer.Canvas.Brush.Style := bsSolid;
        AInfo.Drawer.Canvas.Brush.Color := Shadow.Color;
        if FDiagram <> nil then
          AShadowRect := MoveRectByPoint(AInfo.Drawer.CurRect,
            SubPointX(
              FDiagram.ClientToCanvas(Dot(Shadow.HOffset, Shadow.VOffset)),
              FDiagram.ClientToCanvas(Dot(0, 0))
              ))
        else
          AShadowRect := MoveRectByPoint(AInfo.Drawer.CurRect, Dot(Shadow.HOffset, Shadow.VOffset));
        ASaveRect := AInfo.Drawer.CurRect;
        AInfo.Drawer.CurRect := AShadowRect;

        {Now prepare the gdiplus drawer (GPDrawer) to draw the Shadow. Basically, we need to set
         brush to shadow's color, and calculate the rectangle of shadow by offseting the original rectangle}
        if PathDrawingMode then
        begin
          AInfo.DgrDrawer.Canvas.Pen.Transparency := 100;
          AInfo.DgrDrawer.Canvas.Brush.Color := Shadow.Color;
          AInfo.DgrDrawer.Canvas.Brush.Transparency := MixTransp(Shadow.Transparency, Self.Transparency);
          AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmSolid;
          saveDgrRect := AInfo.DgrDrawer.DestRect;
          AInfo.DgrDrawer.DestRect := MoveRectXByPoint(saveDgrRect, Point(Shadow.HOffset, Shadow.VOffset));
        end;

        {Now call DoDrawShape - all drawers are set ok!}
        DoDrawShape(AInfo, ABlockInfo);

        {Restore the original rectangle in both drawers}
        if PathDrawingMode then
          AInfo.DgrDrawer.DestRect := saveDgrRect;
        AInfo.Drawer.CurRect := ASaveRect;
      end;

      {NEXT STEP - Draw background color}
      if not FTransparent then
      begin
        {Prepare non-gdiplus drawer}
        AInfo.Drawer.Canvas.Brush.Style := bsSolid;
        AInfo.Drawer.Canvas.Brush.Color := CurrentColor;
        AInfo.Drawer.Canvas.Pen.Style := psClear;

        {Prepare gdiplus drawer}
        if PathDrawingMode then
        begin
          AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmSolid;
          AInfo.DgrDrawer.Canvas.Brush.Color := CurrentColor;
          AInfo.DgrDrawer.Canvas.Brush.Transparency := Self.Transparency;
          AInfo.DgrDrawer.Canvas.Pen.Transparency := 100;
        end;

        {draw the background}
        DoDrawShape(AInfo, ABlockInfo);
      end;

      {NEXT STEP - Draw gradient}
      if FGradient.Visible then
      begin
        {Draw the gradient painting it manually}
        ClipBlock(AInfo);
        try
          if PathDrawingMode then
          begin
            AInfo.DgrDrawer.Canvas.Brush.Transparency := Self.Transparency;
            FGradient.DrawGradient(AInfo.DgrDrawer);
          end
          else
            FGradient.DrawGradient(AInfo.Canvas, ABlockInfo.Rect, Angle);
        finally
          UnclipBlock(AInfo);
        end;
      end;

      {NEXT STEP - Draw Background Brush. Set both drawer and gpdrawer brushes}
      AInfo.Drawer.Canvas.Pen.Style := psClear;
      AInfo.Drawer.Canvas.Brush := CurrentBrush;

      if PathDrawingMode then
      begin
        AInfo.DgrDrawer.Canvas.Pen.Transparency := 100;
        {We must set brush transparency before assigning a TBrush object to the TDgrBrush object}
        AInfo.DgrDrawer.Canvas.Brush.Transparency := Self.Transparency;
        AInfo.DgrDrawer.Canvas.Brush.Assign(CurrentBrush);
      end;

      DoDrawShape(AInfo, ABlockInfo);

      {Draw bitmap}
      if not FBitmap.Empty then
      begin
        ClipBlock(AInfo);
        try
          DrawBitmap;
        finally
          UnclipBlock(AInfo);
        end;
      end;

      {Draw metafile}
      if not EmptyMetafile(FMetafile) then
      begin
        ClipBlock(AInfo);
        try
          DrawMetafile;
        finally
          UnclipBlock(AInfo);
        end;
      end;

      {Draw picture}
      if not EmptyPicture(FPicture) then
      begin
        ClipBlock(AInfo);
        try
          DrawPicture;
        finally
          UnclipBlock(AInfo);
        end;
      end;

      {NEXT STEP - Draw Outline. Set both drawer and gpdrawer pens}
      AInfo.Drawer.Canvas.Pen := FPen;
      AInfo.Drawer.Canvas.Brush.Style := bsClear;
      if PathDrawingMode then
      begin
        AInfo.DgrDrawer.Canvas.Pen.Transparency := Self.Transparency;
        AInfo.DgrDrawer.Canvas.Pen.Assign(FPen);
        AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmClear;
      end;

      DoDrawShape(AInfo, ABlockInfo);
    finally
      AInfo.Canvas.Brush.Assign(SaveBrush);
      AInfo.Canvas.Pen.Assign(SavePen);
      SavePen.Free;
      SaveBrush.Free;

      if PathDrawingMode then
      begin
        if saveDgrBrush <> nil then
        begin
          AInfo.DgrDrawer.Canvas.Brush.Assign(saveDgrBrush);
          saveDgrBrush.Free;
        end;
        if saveDgrPen <> nil then
        begin
          AInfo.DgrDrawer.Canvas.Pen.Assign(saveDgrPen);
          saveDgrPen.Free;
        end;
      end;
    end;
  end;

begin
  _DrawBlock;
end;

procedure TCustomDiagramBlock.PaintControl(AInfo: TDiagramDrawInfo);
var
  blockInfo: TDiagramDrawBlockInfo;
begin
  try
    InitDrawer(AInfo.Drawer);
    if PathDrawingMode then
      InitDgrDrawer(AInfo.DgrDrawer);
    blockInfo := DiagramDrawBlockInfo(Drawer.CurRect, Angle);
    if (FDiagram = nil) or not FDiagram.CustomDrawBlock(Self, AInfo, blockInfo) then
      DrawBlock(AInfo, blockInfo);
    if Assigned(FDiagram) then
      DrawText(AInfo);
    if (FDiagram <> nil) then
      FDiagram.DoAfterDrawBlock(Self, AInfo, blockInfo);
  finally
  end;
end;

procedure TCustomDiagramBlock.StartCursorPlacement;
var
  c: integer;
begin
  {If draw as cursor, then save placement information of block and change
   it to cursor placement information}
  FSaveAngle := FAngle;
  FSaveRect := BoundsRect;
  FSaveRotCenter := FRotCenter;

  {Set fields directly, not property, to avoid redrawing}
  FAngle := FCursorAngle;
  FLeft := FCursorRect.Left;
  FTop := FCursorRect.Top;
  FWidth := FCursorRect.Right - FCursorRect.Left;
  FHeight := FCursorRect.Bottom - FCursorRect.Top;
  FRotCenter := FCursorRotCenter;

  {update manually the textcells with dcontrolangle set}
  for c := 0 to TextCells.Count - 1 do
    if TextCells[c].DControlAngle then
      TextCells[c].FAngle := FAngle;
end;

procedure TCustomDiagramBlock.EndCursorPlacement;
var
  c: integer;
begin
  {Set fields directly, not property, to avoid redrawing}
  FAngle := FSaveAngle;
  FLeft := FSaveRect.Left;
  FTop := FSaveRect.Top;
  FWidth := FSaveRect.Right - FSaveRect.Left;
  FHeight := FSaveRect.Bottom - FSaveRect.Top;
  FRotCenter := FSaveRotCenter;

  {update manually the textcells with dcontrolangle set}
  for c := 0 to TextCells.Count - 1 do
    if TextCells[c].DControlAngle then
      TextCells[c].FAngle := FAngle;
end;

procedure TCustomDiagramBlock.DiagramChanged(Old, New: TatDiagram);
begin
  UpdateBlockHandles;
  if Assigned(Old) then
  begin
    NotifyAnchoreds(acRemoval);
    Old.RemoveBlock(Self);
  end;
  if Assigned(New) then
  begin
    New.AddBlock(Self);
    DiagramFontChanged;
    DiagramShowHintChanged;
  end;
end;

class function TCustomDiagramBlock.DiagramDrawBlockInfo(ARect: TSquare; AAngle: double): TDiagramDrawBlockInfo;
begin
  result.Rect := ARect;
  result.Angle := AAngle;
end;

procedure TCustomDiagramBlock.SetShape(const Value: TBlockShape);
begin
  if FShape <> Value then
  begin
    FShape := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetBrush(const Value: TBrush);
begin
  FBrush.Assign(Value);
end;

procedure TCustomDiagramBlock.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure TCustomDiagramBlock.SetSelBrush(const Value: TBrush);
begin
  FSelBrush.Assign(Value);
end;

procedure TCustomDiagramBlock.StyleChanged(Sender: TObject);
begin
  if Sender = FPen then
    PenChanged;
  Redraw;
end;

procedure TCustomDiagramBlock.SetStrings(const Value: TStrings);
begin
  FStrings.Assign(Value);
end;

procedure TCustomDiagramBlock.DrawText(AInfo: TDiagramDrawInfo);
begin
  if ClipText then
  begin
    try
      ClipBlock(AInfo);
      DrawCells(AInfo);
    finally
      UnclipBlock(AInfo);
    end;
  end
  else
    DrawCells(AInfo);
end;

procedure TCustomDiagramBlock.StringsChange(Sender: TObject);
begin
  DefaultTextCell.Text := FStrings.Text;
  Redraw;
end;

procedure TCustomDiagramBlock.SetAlignment(const Value: TAlignment);
begin
  DefaultTextCell.Alignment := Value;
end;

procedure TCustomDiagramBlock.SetVertAlign(const Value: TVertAlign);
begin
  DefaultTextCell.VertAlign := Value;
end;

procedure TCustomDiagramBlock.SetClipText(const Value: boolean);
begin
  if FClipText <> Value then
  begin
    FClipText := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetWordWrap(const Value: boolean);
begin
  DefaultTextCell.WordWrap := Value;
end;

procedure TCustomDiagramBlock.SetMinHeight(const Value: integer);
begin
  if FMinHeight <> Value then
  begin
    FMinHeight := Value;
    if FMinHeight < MinMinHeight then
      FMinHeight := MinMinHeight;
  end;
end;

procedure TCustomDiagramBlock.SetMinWidth(const Value: integer);
begin
  if FMinWidth <> Value then
  begin
    FMinWidth := Value;
    if FMinWidth < MinMinWidth then
      FMinWidth := MinMinWidth;
  end;
end;

function TCustomDiagramBlock.AcceptOverlap(
  ABlock: TCustomDiagramBlock): boolean;
begin
  result := true;
end;

function TCustomDiagramBlock.GetLinkPoint(ALinkPoint: TLinkPoint): TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.CurRect := BoundsRect;
  FTempDrawer.Angle := Angle;
  FTempDrawer.RotationType := rtCurRectCenter;
  result := FTempDrawer.RotX(FTempDrawer.PP(ALinkPoint.OrX, ALinkPoint.OrY));
  FTempDrawer.PopState;
end;

function TCustomDiagramBlock.GetHandlePoint(AHandle: TStretchHandle): TDot;
var
  P: TDot;
begin
  FTempDrawer.PushState;
  Case AHandle.Style of
    hsSize, hsCustom:
      begin
        FTempDrawer.OriginalRect := Drawer.OriginalRect;
        FTempDrawer.CurRect := BoundsRect;
        FTempDrawer.Angle := Angle;
        FTempDrawer.RotationType := rtCurRectCenter;
        result := FTempDrawer.RotX(FTempDrawer.PPX(AHandle.OrX, AHandle.OrY));
      end;
    hsRotate:
      begin
        if not CanRotate then
          result := DummyPointX
        else
        begin
          FTempDrawer.OriginalRect := Drawer.OriginalRect;
          FTempDrawer.CurRect := BoundsRect;
          FTempDrawer.Angle := Angle;
          FTempDrawer.RotationType := rtCurRectCenter;
          P := AHandle.OrPoint;
          if (Handles.RotCenterHandle <> nil) and (Diagram <> nil) and (Diagram.HandlesStyle = hsVisio) then
            With FTempDrawer do
              P.X := UnPPX(UnRotX(Handles.RotCenterHandle.DiagramPoint)).X;

          With FTempDrawer do
            result := PPX(P);

          {keep an equal distance from the top regardless of zooming, when style is visio.
           So, we need to calculate the value of diagram point so that the canvas point is always
           at the same distance from top}
          if (Diagram <> nil) and (Diagram.HandlesStyle = hsVisio) then
          begin
            P := Diagram.ClientToCanvas(result);
            P.Y := P.Y - 24;
            P := Diagram.CanvasToClient(P);
            result.Y := P.Y;
          end;

          result := FTempDrawer.RotX(result);
        end;
      end;
    hsRotCenter:
      begin
        FTempDrawer.OriginalRect := Square(0, 0, 100, 100);
        FTempDrawer.CurRect := BoundsRect;
        FTempDrawer.Angle := Angle;
        FTempDrawer.RotationType := rtCurRectCenter;
        With FTempDrawer do
          result := RotX(PPX(FRotCenter));
      end;
  end;
  FTempDrawer.PopState;
end;

procedure TCustomDiagramBlock.AngleChanged;
const
  CursorArray: array[0..7] of TCursor =
    (crSizeNWSE, crSizeNS, crSizeNESW, crSizeWE,
     crSizeNWSE, crSizeNS, crSizeNESW, crSizeWE);
var
  AngleStep: integer;
  AHandle: TStretchHandle;
  c: integer;
begin
  inherited;
  AngleStep := trunc((Angle + 22.5) / 45);
  for c := Ord(hpLeftTop) to Ord(hpLeft) do
  begin
    AHandle := Handles.FindByData(c);
    if AHandle <> nil then
      AHandle.Cursor := CursorArray[((c - Ord(hpLeftTop)) + AngleStep) mod 8];
  end;
  NotifyAnchoreds(acPosition);
end;

procedure TCustomDiagramBlock.UpdateBlockHandles;

  procedure _FindOrCreate(AOrX, AOrY: double; ACursor: TCursor; AData: integer;
    AStyle: THandleStyle = hsSize);
  var
    AHandle: TStretchHandle;
  begin
    AHandle := Handles.FindByData(AData);
    if AHandle = nil then
      AHandle := Handles.Add;
    AHandle.OrX := AOrX;
    AHandle.OrY := AOrY;
    AHandle.Cursor := ACursor;
    AHandle.Data := AData;
    AHandle.Style := AStyle;
  end;

begin
  with Handles, Drawer.OriginalRect do
  begin
    _FindOrCreate(Left, Top, crSizeNWSE, Ord(hpLeftTop));
    _FindOrCreate((Left + Right) / 2, Top, crSizeNS, Ord(hpTop));
    _FindOrCreate(Right, Top, crSizeNESW, Ord(hpRightTop));
    _FindOrCreate(Right, (Top + Bottom) / 2, crSizeWE, Ord(hpRight));
    _FindOrCreate(Right, Bottom, crSizeNWSE, Ord(hpRightBottom));
    _FindOrCreate((Left + Right) / 2, Bottom, crSizeNS, Ord(hpBottom));
    _FindOrCreate(Left, Bottom, crSizeNESW, Ord(hpLeftBottom));
    _FindOrCreate(Left, (Top + Bottom) / 2, crSizeWE, Ord(hpLeft));
    if Diagram <> nil then
      case Diagram.HandlesStyle of
        hsClassic:
          _FindOrCreate(Right - (Right - Left) / 10, (Top + Bottom) / 2, crHandPoint, hpRotation, hsRotate);
        hsVisio:
          begin
            _FindOrCreate((Left + Right) / 2, Top, crHandPoint, hpRotation, hsRotate);

            {Create the rotation center handle. The position OrX/OrY of the handle doesn't matter,
             because the handle diagram point will be retrieved by GetHandlePoint method
             based on the value of FRotationCenter field}
            _FindOrCreate(0, 0, crHandPoint, hpRotCenter, hsRotCenter);
          end;
      end
    else
      _FindOrCreate(Right - (Right - Left) / 10, (Top + Bottom) / 2, crHandPoint, hpRotation, hsRotate);
  end;
  AngleChanged; {Update cursors}
end;

function TCustomDiagramBlock.RotateNewRect(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TSquare;
var
  DeltaAngle: double;
begin
  DeltaAngle := RotateNewAngle(AHandle, ADeltaX, ADeltaY) - Angle;

  {Rotate rect by center}
  FTempDrawer.PushState;
  FTempDrawer.CurRect := BoundsRect;
  FTempDrawer.RotationType := rtRotationCenter;
  FTempDrawer.RotationCenter := DiagramRotCenter;
  FTempDrawer.Angle := DeltaAngle;
  {result.TopLeft := FTempDrawer.Rot(BoundsRect.TopLeft);
  result.BottomRight := FTempDrawer.Rot(BoundsRect.BottomRight);}
  result := FTempDrawer.RotByCenter(BoundsRect);
  FTempDrawer.PopState;
end;

function TCustomDiagramBlock.CalcNewRotCenter(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TDot;
var
  ANewDiagRotCenter: TDot;
begin
  if not CanMoveRotCenter then
    result := RotationCenter
  else
  begin
    FTempDrawer.PushState;
    FTempDrawer.CurRect := BoundsRect;
    FTempDrawer.RotationType := rtCurRectCenter;
    FTempDrawer.Angle := Angle;
    with FTempDrawer do
    begin
      ANewDiagRotCenter := AddPointX(DiagramRotCenter, Dot(ADeltaX, ADeltaY));
      ANewDiagRotCenter := UnrotX(ANewDiagRotCenter);

      OriginalRect := Square(0, 0, 100, 100);
      result := Dot(UnPXS(ANewDiagRotCenter.X), UnPYS(ANewDiagRotCenter.Y));
    end;
    FTempDrawer.PopState;
  end;
end;

function TCustomDiagramBlock.RotateNewAngle(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): double;

  function CalcAngle(P, Center: TDot): double;
  begin
    P := SubPointX(P, Center);
    if (P.X = 0) then
      result := Sign(Round(P.Y)) * 90
    else
      result := 180 / pi * ArcTan(P.Y / P.X);
    if P.X < 0 then
      result := result + 180;
  end;

var
  C: TDot;
  P: TDot;
  OriginAngle: double;
  NewAngle: double;
begin
  if not CanRotate then
    result := Angle
  else
  begin
    //C := Point((Left + Right) div 2, (Top + Bottom) div 2);
    C := DiagramRotCenter;
    P := AddPointX(AHandle.DiagramPoint, Dot(ADeltaX, ADeltaY));
    OriginAngle := CalcAngle(AHandle.DiagramPoint, C);
    NewAngle := CalcAngle(P, C);

    result := NewAngle - OriginAngle + Angle;
    while result > 360 do
      result := result - 360;
    result := round(result / FRotationStep) * FRotationStep;
  end;
end;


function TCustomDiagramBlock.ResizeNewRect(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TSquare;

  function _HandlePt(AData: integer): TDot;
  var _Handle: TStretchHandle;
  begin
    _Handle := Handles.FindByData(Ord(AData));
    if _Handle <> nil then
      result := _Handle.DiagramPoint
    else
      result := DummyPointX;
  end;

  function _Rect2(P1, P2: TDot): TSquare;
  var
    CX, CY: double;
  begin
    CX := (P1.X + P2.X) / 2;
    CY := (P1.Y + P2.Y) / 2;
    FTempDrawer.CurRect := RectEx(CX - 10, CY - 10, CX + 10, CY + 10);
    result := RectEx(FTempDrawer.UnrotX(P1), FTempDrawer.UnrotX(P2));
  end;

  function _Rect3(P1, P2, P3: TDot): TSquare;
  var
    ADelta, Q1, Q2, Q3: TDot;
  begin
    Q1 := FTempDrawer.UnrotX(P1);
    Q2 := FTempDrawer.UnrotX(P2);
    Q3 := FTempDrawer.UnrotX(P3);
    if Abs(Q2.Y - Q3.Y) < Abs(Q2.X - Q3.X) then
      Q1 := Dot(Q2.X, Q1.Y)
    else
      Q1 := Dot(Q1.X, Q2.Y);

    Q3 := FTempDrawer.RotX(Q3);
    ADelta := SubPointX(Q3, P3);

    Q1 := FTempDrawer.RotX(Q1);
    result := _Rect2(P3, AddPointX(Q1, ADelta));
  end;

  function CheckBlockSize(NewRect, OldRect: TSquare): TSquare;

    procedure KeepMinimumSize;
    begin
      {Assure minimum width of block}
      if NewRect.Right - NewRect.Left < MinWidth then
        NewRect.Right := NewRect.Left + MinWidth;
      if NewRect.Bottom - NewRect.Top < MinHeight then
        NewRect.Top := NewRect.Bottom - MinHeight;
    end;

    procedure KeepRatio;
    begin
     if crKeepRatio in Restrictions then
       NewRect := KeepRectRatio(NewRect, OldRect);
    end;

  var
    QRef, QRef2: TDot;
    ADelta, R1, R2: TDot;
    CX, CY: number;
    PRef, PRefUnrot: TDot;
    P1, P2, P3, P4: TDot;
    D1, D2, D3, D4: double;
  begin
    KeepMinimumSize;
    KeepRatio;

    {All the code from here is only for correct position of the block when it's rotated.
     After changing NewRect, we should reposition it so that the side of the rotated block
     that is not being moved stays at the same position. For example, if the right side of the block
     is being resized, then the left side should remain the same. So, extra calculation need to be done
     so that unrotated NewRect is repositioned to keep left side of rotated block at the same position}
    {Set parameters for correct rotation}
    CX := (BoundsRect.Left + BoundsRect.Right) / 2;
    CY := (BoundsRect.Top + BoundsRect.Bottom) / 2;
    FTempDrawer.CurRect := RectEx(CX - 10, CY - 10, CX + 10, CY + 10);

    {Reposition the rotated block in the new position. Depending on the handle being moved,
     move the block}
    case AHandle.Data of
      hpLeftTop :
        PRef := _HandlePt(hpRightBottom);
      hpRightTop :
        PRef := _HandlePt(hpLeftBottom);
      hpLeftBottom :
        PRef := _HandlePt(hpRightTop);
      hpRightBottom :
        PRef := _HandlePt(hpLeftTop);
      hpLeft :
        PRef := _HandlePt(hpRightBottom);
      hpRight :
        PRef := _HandlePt(hpLeftTop);
      hpTop :
        PRef := _HandlePt(hpLeftBottom);
      hpBottom :
        PRef := _HandlePt(hpLeftTop);
    end;
    {Transform the fixed point to zero graus}
    PRefUnrot := FTempDrawer.UnRotX(PRef);

    {Set parameters for correct rotation}
    CX := (NewRect.Left + NewRect.Right) / 2;
    CY := (NewRect.Top + NewRect.Bottom) / 2;
    FTempDrawer.CurRect := RectEx(CX - 10, CY - 10, CX + 10, CY + 10);

    P1 := FTempDrawer.RotX(NewRect.Left, NewRect.Top);
    P2 := FTempDrawer.RotX(NewRect.Right, NewRect.Top);
    P3 := FTempDrawer.RotX(NewRect.Left, NewRect.Bottom);
    P4 := FTempDrawer.RotX(NewRect.Right, NewRect.Bottom);
    D1 := PointDistance(P1.X, P1.Y, PRef.X, PRef.Y);
    D2 := PointDistance(P2.X, P2.Y, PRef.X, PRef.Y);
    D3 := PointDistance(P3.X, P3.Y, PRef.X, PRef.Y);
    D4 := PointDistance(P4.X, P4.Y, PRef.X, PRef.Y);

    {Calculate the point of newrect which is nearest to PRefUnrot}
    if (D1 <= D2) and (D1 <= D3) and (D1 <= D4) then
    begin
      {D1 is nearest}
      QRef := NewRect.TopLeft;
      QRef2 := NewRect.BottomRight;
    end else
    if (D2 <= D1) and (D2 <= D3) and (D2 <= D4) then
    begin
      {D2 is nearest}
      QRef := Dot(NewRect.Right, NewRect.Top);
      QRef2 := Dot(NewRect.Left, NewRect.Bottom);
    end else
    if (D3 <= D1) and (D3 <= D2) and (D3 <= D4) then
    begin
      {D3 is nearest}
      QRef := Dot(NewRect.Left, NewRect.Bottom);
      QRef2 := Dot(NewRect.Right, NewRect.Top);
    end else
    //if (D4 < D1) and (D4 < D3) and (D4 < D3) then
    begin
      {D4 is nearest}
      QRef := NewRect.BottomRight;
      QRef2 := NewRect.TopLeft;
    end;

    {Rotate QRef and the oposite point}
    QRef := FTempDrawer.RotX(QRef);
    QRef2 := FTempDrawer.RotX(QRef2);

    {if PointDistance(PRef.X, PRef.Y, R1.X, R1.Y) < PointDistance(PRef.X, PRef.Y, R2.X, R2.Y) then
    begin
      QRef := R1;
      Q2 := R2;
    end else
    begin
      QRef := R2;
      Q2 := R1;
    end;}

    ADelta := SubPointX(PRef, QRef);
    R1 := AddPointX(QRef2, ADelta);
    R2 := PRef;

    CX := (R1.X + R2.X) / 2;
    CY := (R1.Y + R2.Y) / 2;
    FTempDrawer.CurRect := RectEx(CX - 10, CY - 10, CX + 10, CY + 10);

    result := RectEx(FTempDrawer.UnRotX(R1), FTempDrawer.UnRotX(R2));
  end;

var
  ARect: TSquare;
  AMoved: TDot;
begin
  if not CanResize then
    result := BoundsRect
  else
  begin
    FTempDrawer.PushState;
    FTempDrawer.OriginalRect := Drawer.OriginalRect;
    FTempDrawer.Angle := Angle;
    FTempDrawer.RotationType := rtCurRectCenter;
    FTempDrawer.CurRect := Drawer.CurRect;
    ARect := BoundsRect;
    result := ARect;
    AMoved := AddPointX(AHandle.DiagramPoint, Dot(ADeltaX, ADeltaY));
    Case AHandle.Data of
      hpLeftTop :
        result := _Rect2(AMoved, _HandlePt(hpRightBottom));
      hpRightTop :
        result := _Rect2(AMoved, _HandlePt(hpLeftBottom));
      hpLeftBottom :
        result := _Rect2(AMoved, _HandlePt(hpRightTop));
      hpRightBottom :
        result := _Rect2(AMoved, _HandlePt(hpLeftTop));
      hpLeft :
        result := _Rect3(AMoved, _HandlePt(hpRightTop), _HandlePt(hpRightBottom));
      hpRight :
        result := _Rect3(AMoved, _HandlePt(hpLeftTop), _HandlePt(hpLeftBottom));
      hpTop :
        result := _Rect3(AMoved, _HandlePt(hpLeftBottom), _HandlePt(hpRightBottom));
      hpBottom :
        result := _Rect3(AMoved, _HandlePt(hpLeftTop), _HandlePt(hpRightTop));
    end;
    result := CheckBlockSize(result, ARect);
    FTempDrawer.PopState;
  end;
end;

procedure TCustomDiagramBlock.HandleMoving(AInfo: THandleMoveInfo);
begin
  with AInfo do
  begin
    case Action of
      haEraseCursor, haDrawCursor:
        case AHandle.Style of
          hsRotate:
            DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(
              FDiagram.ClientToCanvas(RotateNewRect(AHandle, Delta.X, Delta.Y)),
              RotateNewAngle(AHandle, Delta.X, Delta.Y)));
          hsSize:
            DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(
              FDiagram.ClientToCanvas(ResizeNewRect(AHandle, Delta.X, Delta.Y)), Angle));
        end;
      haPositionCursor:
        begin
          FCursorRect := BoundsRect;
          FCursorAngle := Angle;
          FCursorRotCenter := RotationCenter;
          Case AHandle.Style of
            hsRotate:
              begin
                FCursorRect := RotateNewRect(AHandle, Delta.X, Delta.Y);
                FCursorAngle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
              end;
            hsSize:
              begin
                FCursorRect := ResizeNewRect(AHandle, Delta.X, Delta.Y);
              end;
            hsRotCenter:
              begin
                FCursorRotCenter := CalcNewRotCenter(AHandle, Delta.X, Delta.Y);
              end;
          end;
        end;
      haMoved:
        Case AHandle.Style of
          hsRotate:
            begin
              BoundsRect := RotateNewRect(AHandle, Delta.X, Delta.Y);
              Angle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
            end;
          hsSize:
            BoundsRect := ResizeNewRect(AHandle, Delta.X, Delta.Y);
          hsRotCenter:
            begin
              RotationCenter := CalcNewRotCenter(AHandle, Delta.X, Delta.Y);
            end;
        end;
    end;
  end;
end;

procedure TCustomDiagramBlock.DrawBlockCursor(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);
var
  oldBrush: TBrush;
  oldPen: TPen;
  oldGraphicLib: TDgrGraphicLib;
  oldCustomGraphicLib: boolean;
begin
  oldBrush := TBrush.Create;
  oldPen := TPen.Create;
  try
    oldBrush.Assign(AInfo.Canvas.Brush);
    oldPen.Assign(AInfo.Canvas.Pen);
    AInfo.Canvas.Brush.Style := bsClear;
    AInfo.Canvas.Pen.Mode := pmNot;
    AInfo.Canvas.Pen.Style := psDot;

    AInfo.Drawer.Canvas := AInfo.Canvas;
    AInfo.Drawer.Angle := ABlockInfo.Angle;
    AInfo.Drawer.RotationType := rtCurRectCenter;
    AInfo.Drawer.CurRect := ABlockInfo.Rect;
    if FShapeCursor then
    begin
      // GdiPlus does not support pens in pmNot mode. So, we need to draw the line using non-gdi plus drawing
      oldGraphicLib := GraphicLib;
      oldCustomGraphicLib := FCustomGraphicLib;
      try
        GraphicLib := dglGDI;
        DrawShape(AInfo, ABlockInfo);
      finally
        GraphicLib := oldGraphicLib;
        FCustomGraphicLib := oldCustomGraphicLib;
      end;
    end
    else
      Drawer.DrawRotRect(ABlockInfo.Rect.Left, ABlockInfo.Rect.Top, ABlockInfo.Rect.Right, ABlockInfo.Rect.Bottom);
  finally
    AInfo.Canvas.Pen.Assign(oldPen);
    AInfo.Canvas.Brush.Assign(oldBrush);
    oldBrush.Free;
    oldPen.Free;
  end;
end;

function TCustomDiagramBlock.ControlRect: TSquare;
begin
  result := BoundsRect;
end;

function TCustomDiagramBlock.AngledSurroundRect(AAngle: double): TDotArray;
var
  P1, P2, P3, P4: TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.CurRect := ControlRect;
  FTempDrawer.Angle := AAngle;
  FTempDrawer.RotationType := rtCurRectCenter;
  P1 := FTempDrawer.RotX(FTempDrawer.CurRect.Left, FTempDrawer.CurRect.Top);
  P2 := FTempDrawer.RotX(FTempDrawer.CurRect.Right, FTempDrawer.CurRect.Bottom);
  P3 := FTempDrawer.RotX(FTempDrawer.CurRect.Right, FTempDrawer.CurRect.Top);
  P4 := FTempDrawer.RotX(FTempDrawer.CurRect.Left, FTempDrawer.CurRect.Bottom);
  SetLength(result, 4);
  result[0] := P1;
  result[1] := P2;
  result[2] := P3;
  result[3] := P4;
  FTempDrawer.PopState;
end;

function TCustomDiagramBlock.SurroundRect: TSquare;
var
  P: TDotArray;
begin
  P := AngledSurroundRect(Self.Angle);
  result := Square(
   MinValue([P[0].X, P[1].X, P[2].X, P[3].X]),
   MinValue([P[0].Y, P[1].Y, P[2].Y, P[3].Y]),
   MaxValue([P[0].X, P[1].X, P[2].X, P[3].X]),
   MaxValue([P[0].Y, P[1].Y, P[2].Y, P[3].Y]));
end;

function TCustomDiagramBlock.GetSurroundRgn: TDotArray;
var
  _OriginalRect: TSquare;
  _CurRect: TSquare;
  _Angle: double;
  _RotationType: TRotationType;
  _RotationCenter: TDot;
begin
  SetLength(result, 0);

  {For backward compatibility, we must set Drawer properties here so that it is used by descendants
   or in the OnGetSurroundRgn event. Also, we must keep Drawer properties consistent, so if properties of
   Drawer are changed in descendants or events, we must recover it back. So we will save the properties here}

  {Save drawer property is FTempDrawer}
  _OriginalRect := Drawer.OriginalRect;
  _CurRect := Drawer.CurRect;
  _Angle := Drawer.Angle;
  _RotationType := Drawer.RotationType;
  _RotationCenter := Drawer.RotationCenter;

  try
    {Set drawer properties for backward compatibility}
    Drawer.OriginalRect := Drawer.OriginalRect;
    Drawer.CurRect := BoundsRect;
    Drawer.Angle := Angle;
    Drawer.RotationType := rtCurRectCenter;
    if (FDiagram <> nil) then
      result := FDiagram.DoGetSurroundRgn(Self); //DoGetSurroundRgn sets length to 0, too
    if Length(result) = 0 then
      result := SurroundRgn;
  finally
    {Recover Drawer properties}
    Drawer.OriginalRect := _OriginalRect;
    Drawer.CurRect := _CurRect;
    Drawer.Angle := _Angle;
    Drawer.RotationType := _RotationType;
    Drawer.RotationCenter := _RotationCenter;
  end;
end;

function TCustomDiagramBlock.SurroundRgn: TDotArray;
var
  P1, P2: TDot;
  D: double;
begin
  if not PathDrawingMode then
  begin
    FTempDrawer.PushState;
    FTempDrawer.OriginalRect := Drawer.OriginalRect;
    FTempDrawer.CurRect := BoundsRect;
    FTempDrawer.Angle := Angle;
    FTempDrawer.RotationType := rtCurRectCenter;
    case FShape of
      bsDiamond:
        begin
          SetLength(result, 5);
          with FTempDrawer, OriginalRect do
          begin
            result[0] := RotX(PP(Left, (Bottom + Top) / 2));
            result[1] := RotX(PP((Right + Left) / 2, Top));
            result[2] := RotX(PP(Right, (Bottom + Top) / 2));
            result[3] := RotX(PP((Right + Left) / 2, Bottom));
            result[4] := result[0];
          end;
        end;
      bsEllipse:
        result := FTempDrawer.RotPoly(GetPolyEllipse(Left, Top, Right, Bottom));
      bsRoundRect:
        result := FTempDrawer.RotPoly(GetPolyRoundRect(Left, Top, Right, Bottom,
          (Right - Left) / 4, (Bottom - Top) / 4));
      bsSquareRound:
        begin
          With FTempDrawer.OriginalRect do
          begin
            P1 := FTempDrawer.PP(Left, Top);
            P2 := FTempDrawer.PP(Right, Bottom);
          end;
          D := P2.Y - P1.Y;
          result := FTempDrawer.RotPoly(GetPolyRoundRect(P1.X, P1.Y, P2.X, P2.Y, D, D));
        end;
    else {bsNoShape, bsRectangle}
      begin
        SetLength(result, 5);
        With FTempDrawer.OriginalRect do
        begin
          result[0] := FTempDrawer.RotX(FTempDrawer.PP(Left, Top));
          result[1] := FTempDrawer.RotX(FTempDrawer.PP(Right, Top));
          result[2] := FTempDrawer.RotX(FTempDrawer.PP(Right, Bottom));
          result[3] := FTempDrawer.RotX(FTempDrawer.PP(Left, Bottom));
          result[4] := result[0];
        end;
      end;
    end;
    FTempDrawer.PopState;
  end;
end;

procedure TCustomDiagramBlock.GetSurroundBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer);
begin
  GetTranslatedBlockPath(APath, ADrawer);

  {$IFDEF GDIPLUS}
  {$WARNINGS OFF}
  if APath is TGdipGraphicsPath then // GDI+ overload (backward compatibility)
    GetSurroundBlockPath(TGdipGraphicsPath(APath).GPPath);
  {$WARNINGS ON}
  {$ENDIF}

  {If path is empty, then create a single rectangle so that the block can be selected}
  if APath.GetPointCount = 0 then
  begin
    APath.Reset;
    APath.AddRectangle(ADrawer.SourceRect);
    ADrawer.TransformPath(APath);
  end;
end;

procedure TCustomDiagramBlock.GetTranslatedBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer);
begin
  if Shape = bsSquareRound then
  begin
    with ADrawer.DestRect do
      ADrawer.Canvas.AddRoundRectPath(APath, Left, Top, Right, Bottom, (Bottom - Top) / 2, (Bottom - Top) / 2);
    ADrawer.RotatePath(APath);
  end
  else
  begin
    {Get block path in SourceRect coordinates}
    GetBlockPath(APath, ADrawer);

    {Translate SourceRect to DestRect coordinates}
    ADrawer.TransformPath(APath);
  end;

  {$IFDEF GDIPLUS}
  {$WARNINGS OFF}
  if APath is TGdipGraphicsPath then // GDI+ overload (backward compatibility)
    GetTranslatedBlockPath(TGdipGraphicsPath(APath).GPPath);
  {$WARNINGS ON}
  {$ENDIF}
end;

procedure TCustomDiagramBlock.GetBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer);
var
  Pts: TDotArray;
begin
  case Shape of
    bsRectangle:
      with ADrawer.SourceRect do
        APath.AddRectangle(Square(Left, Top, Right, Bottom));
    bsEllipse:
      with ADrawer.SourceRect do
        APath.AddEllipse(Left, Top, Right, Bottom);
    bsRoundRect:
    begin
      with ADrawer.SourceRect do
        FDgrDrawer.Canvas.AddRoundRectPath(APath,
          Left, Top, Right, Bottom,
          (Right - Left) / 4, (Bottom - Top) / 4);
      end;
    bsSquareRound:
    begin
      with ADrawer.SourceRect do
        FDgrDrawer.Canvas.AddRoundRectPath(APath,
          Left, Top, Right, Bottom,
          (Bottom - Top) / 2, (Bottom - Top) / 2);
      end;
    bsDiamond:
      begin
        SetLength(Pts, 4);
        with ADrawer.SourceRect do
        begin
          Pts[0] := Dot(Left, (Bottom + Top) / 2);
          Pts[1] := Dot((Right + Left) / 2, Top);
          Pts[2] := Dot(Right, (Bottom + Top) / 2);
          Pts[3] := Dot((Right + Left) / 2, Bottom);
        end;
        APath.AddPolygon(Pts);
      end;
  end;

  {$IFDEF GDIPLUS}
  {$WARNINGS OFF}
  if APath is TGdipGraphicsPath then // GDI+ overload (backward compatibility)
    GetBlockPath(TGdipGraphicsPath(APath).GPPath);
  {$WARNINGS ON}
  {$ENDIF}
end;

{$IFDEF GDIPLUS}
procedure TCustomDiagramBlock.GetBlockPath(APath: TGPGraphicsPath);
begin
end;

procedure TCustomDiagramBlock.GetTranslatedBlockPath(APath: TGPGraphicsPath);
begin
end;

procedure TCustomDiagramBlock.GetSurroundBlockPath(APath: TGPGraphicsPath);
begin
end;
{$ENDIF}

procedure TCustomDiagramBlock.MovePosition(ADeltaX, ADeltaY: number);
begin
  BoundsRect := MoveRectByDelta(BoundsRect, Dot(ADeltaX, ADeltaY));
end;

function TCustomDiagramBlock.MoveRectByDelta(ARect: TSquare; ADelta: TDot): TSquare;
begin
  result := MoveRectByPoint(ARect, ADelta);
end;

function TCustomDiagramBlock.PointInControl(Pos: TPoint): boolean;
var
  R: HRGN;
  Pts: TPointArray;
  path: TDgrGraphicsPath;
begin
  Pts := RoundDotArray(GetSurroundRgn);
  if Length(Pts) > 0 then
  begin
    R := CreatePolygonRgn(Pts[0], Length(Pts), ALTERNATE);
    try
      result := PtInRegion(R, Pos.X, Pos.Y);
    finally
      DeleteObject(R);
    end;
  end
  else if PathDrawingMode then
  begin
    path := FDgrDrawer.CreatePath;
    try
      InitDgrDrawer(FDgrDrawer);
      GetSurroundBlockPath(path, FDgrDrawer);
      result := path.IsVisible(Pos.X, Pos.Y);
    finally
      path.Free;
    end;
  end
  else
    result := False;
end;

procedure TCustomDiagramBlock.StartMovingHandle;
begin
end;

procedure TCustomDiagramBlock.StartMoving;
begin
end;

procedure TCustomDiagramBlock.UpdateMoving(AInfo: TControlMoveInfo);
begin
  with AInfo do
  begin
    case Action of
      caEraseCursor, caDrawCursor:
        DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(
          FDiagram.ClientToCanvas(MoveRectByDelta(BoundsRect, Delta)), Angle));
      caPositionCursor:
        begin
          FCursorAngle := Angle;
          FCursorRect := MoveRectByDelta(BoundsRect, Delta);
          FCursorRotCenter := FRotCenter;
        end;
      caMoved:
        BoundsRect := MoveRectByDelta(BoundsRect, Delta);
    end;
  end;
end;

procedure TCustomDiagramBlock.UpdateInserting(AInfo: TControlInsertInfo);
var
  ARect: TSquare;
  AW, AH: number;
begin
  with AInfo do
  begin
    if not CanResize or ((Action = iaInserted) and Clicked) then
    begin
      if (P2.X < P1.X) and not CanResize then
        AW := -Width
      else
        AW := Width;
      if (P2.Y < P1.Y) and not CanResize then
        AH := -Height
      else
        AH := Height;
      ARect := RectEx(P1.X, P1.Y, P1.X + AW, P1.Y + AH);
    end
    else
      ARect := RectEx(P1.X, P1.Y, P2.X, P2.Y);
    case Action of
      iaDrawCursor, iaEraseCursor:
        DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer),
          DiagramDrawBlockInfo(Diagram.ClientToCanvas(ARect), Angle));
      iaInserted:
        BoundsRect := ARect;
    end;
  end;
end;

procedure TCustomDiagramBlock.SetShadow(const Value: TBlockShadow);
begin
  FShadow.Assign(Value);
end;

procedure TCustomDiagramBlock.SetGradient(const Value: TDiagramGradient);
begin
  FGradient.Assign(Value);
end;

function TCustomDiagramBlock.GetBlockClipRegion: HRGN;
var
  Pts: TDotArray;
  CPts: TPointArray;
  path: TDgrGraphicsPath;
  region: TDgrRegion;
begin
  Pts := GetSurroundRgn;
  CPts := nil;

  {if there are no points, it means that we must use the translated block path}
  if Length(Pts) > 0 then
  begin
    if FDiagram <> nil then
      Pts := FDiagram.PolygonToCanvas(Pts);
    CPts := RoundDotArray(Pts);
    result := CreatePolygonRgn(CPts[0], Length(CPts), ALTERNATE);
  end
  else if PathDrawingMode then
  begin
    path := FDgrDrawer.CreatePath;
    try
      GetSurroundBlockPath(path, FDgrDrawer);
      region := path.CreateRegion;
      try
        result := region.GetHRGN(FDgrDrawer.Canvas);
      finally
        region.Free;
      end;
    finally
      path.Free;
    end;
  end
  else
    result := 0;
end;

procedure TCustomDiagramBlock.LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange);
begin
  {Do nothing - just to avoid abstract errors}
end;

procedure TCustomDiagramBlock.SetBitmap(const Value: TBitmap);
begin
  FBitmap.Assign(Value);
end;

procedure TCustomDiagramBlock.SetBitmapMode(const Value: TBitmapMode);
begin
  if FBitmapMode <> Value then
  begin
    FBitmapMode := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetPictureMode(const Value: TPictureMode);
begin
  if FPictureMode <> Value then
  begin
    FPictureMode := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetRotateFont(const Value: boolean);
begin
  DefaultTextCell.RotateFont := Value;
end;

procedure TCustomDiagramBlock.ClipBlock(AInfo: TDiagramDrawInfo);
var
  R1: HRGN;
  path: TDgrGraphicsPath;
  region: TDgrRegion;
  done: boolean;
begin
  R1 := GetBlockClipRegion;
  done := R1 <> 0;
  if done then
  begin
    try
      FDiagram.ClipRegion(AInfo.Canvas, R1);
      if PathDrawingMode then
        done := AInfo.DgrDrawer.Canvas.SetClip(R1) = S_OK;
    finally
      DeleteObject(R1);
    end;
  end;

  if PathDrawingMode and not done then
  begin
    path := AInfo.DgrDrawer.CreatePath;
    try
      GetSurroundBlockPath(path, AInfo.DgrDrawer);
      region := path.CreateRegion;
      try
        AInfo.DgrDrawer.Canvas.ClipRegion(region);
      finally
        region.Free;
      end;
    finally
      path.Free;
    end;
  end;
end;

procedure TCustomDiagramBlock.UnclipBlock(AInfo: TDiagramDrawInfo);
begin
  if FDiagram <> nil then
    FDiagram.UnclipRegion(AInfo.Canvas);
  if PathDrawingMode then
    AInfo.DgrDrawer.Canvas.ResetClip;
end;

procedure TCustomDiagramBlock.Loaded;
begin
  inherited;
  DiagramShowHintChanged;
end;

function TCustomDiagramBlock.CurrentColor: TColor;
begin
  if Selected then
    result := FSelColor
  else
    result := FColor;
end;

function TCustomDiagramBlock.CurrentBrush: TBrush;
begin
  if Selected then
    result := FSelBrush
  else
    result := FBrush;
end;

procedure TCustomDiagramBlock.SetColor(const Value: TColor);
begin
  if FColor <> Value then
  begin
    FColor := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetSelColor(const Value: TColor);
begin
  if FSelColor <> Value then
  begin
    FSelColor := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.SetTransparent(const Value: boolean);
begin
  if FTransparent <> Value then
  begin
    FTransparent := Value;
    Redraw;
  end;
end;

procedure TCustomDiagramBlock.InitDgrDrawer(ADrawer: TDgrBlockDrawer);
begin
  inherited;
  if ADrawer <> nil then
  begin
    with BoundsRect do
      ADrawer.RotationCenter := Dot((Left + Right) / 2, (Top + Bottom) / 2);
    ADrawer.Angle := Angle;
    ADrawer.DestRect := BoundsRect;
  end;
end;

procedure TCustomDiagramBlock.InitDrawer(ADrawer: TBlockDrawer);
begin
  ADrawer.CurRect := Diagram.ClientToCanvas(BoundsRect);
  ADrawer.Angle := Angle;
  ADrawer.RotationType := rtCurRectCenter;
end;

procedure TCustomDiagramBlock.BitmapChange(Sender: TObject);
begin
  if FDgrBitmap <> nil then
    FDgrBitmap.Assign(FBitmap);
  Redraw;
end;

procedure TCustomDiagramBlock.MetafileChange(Sender: TObject);
begin
  if FDgrMetafile <> nil then
    FDgrMetafile.Assign(FMetafile);
  Redraw;
end;

procedure TCustomDiagramBlock.PictureChange(Sender: TObject);
begin
  if FDgrPicture <> nil then
    FDgrPicture.Assign(FPicture.Graphic);
  Redraw;
end;

procedure TCustomDiagramBlock.UpdateDrawing(AInfo: TControlDrawInfo);
begin
end;

procedure TCustomDiagramBlock.AnchorLinkChanged(ALinkPoint: TLinkPoint);
begin
end;

function TCustomDiagramBlock.GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare;
var
  P1, P2: TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.CurRect := BoundsRect;
  P1 := FTempDrawer.PP(ARect.Left, ARect.Top);
  P2 := FTempDrawer.PP(ARect.Right, ARect.Bottom);
  result := Square(P1.X, P1.Y, P2.X, P2.Y);
  FTempDrawer.PopState;
end;

function TCustomDiagramBlock.GetStrings: TStrings;
begin
  if HasDefaultTextCell then
  begin
    FStrings.Text := DefaultTextCell.Text;
  end else
    FStrings.Clear;
  result := FStrings;
end;

function TCustomDiagramBlock.GetAlignment: TAlignment;
begin
  if HasDefaultTextCell then
    result := DefaultTextCell.Alignment
  else
    result := taCenter; {default property value for TTextCell}
end;

function TCustomDiagramBlock.GetRotateFont: boolean;
begin
  if HasDefaultTextCell then
    result := DefaultTextCell.RotateFont
  else
    result := true; {default property value for TTextCell}
end;

function TCustomDiagramBlock.GetVertAlign: TVertAlign;
begin
  if HasDefaultTextCell then
    result := DefaultTextCell.VertAlign
  else
    result := vaTop; {default property value for TTextCell}
end;

function TCustomDiagramBlock.GetWordWrap: boolean;
begin
  if HasDefaultTextCell then
    result := DefaultTextCell.WordWrap
  else
    result := true; {default property value for TTextCell}
end;

procedure TCustomDiagramBlock.HandlesChanged;
begin
end;

procedure TCustomDiagramBlock.SetMetafile(const Value: TMetafile);
begin
  FMetafile.Assign(Value);
end;

procedure TCustomDiagramBlock.SetPicture(const Value: TPicture);
begin
  FPicture.Assign(Value);
end;

procedure TCustomDiagramBlock.SetBitmapTransparent(const Value: boolean);
begin
  if FBitmap.Transparent <> Value then
  begin
    FBitmap.Transparent := Value;
    Redraw;
  end;
end;

function TCustomDiagramBlock.GetBitmapTransparent: boolean;
begin
  result := FBitmap.Transparent;
end;

procedure TCustomDiagramBlock.MouseEnter;
begin
end;

procedure TCustomDiagramBlock.MouseLeave;
begin
end;

procedure TCustomDiagramBlock.Resize;
begin
end;

procedure TCustomDiagramBlock.LoadCollapsedNodesProp(Reader: TReader);
var
  c: integer;
  PropValue: integer;
begin
  PropValue := Reader.ReadInteger;
  for c := 0 to LinkPoints.Count - 1 do
    if (PropValue and (1 shl c)) <> 0 then
      LinkPoints[c].FCollapsed := true;
end;

procedure TCustomDiagramBlock.StoreCollapsedNodesProp(Writer: TWriter);
var
  c: integer;
  PropValue: integer;
begin
  PropValue := 0;
  for c := 0 to LinkPoints.Count - 1 do
    if LinkPoints[c].Collapsed then
      PropValue := PropValue or (1 shl c);
  Writer.WriteInteger(PropValue);
end;

procedure TCustomDiagramBlock.DefineProperties(Filer: TFiler);

  function HasCollapsedNodes: boolean;
  var
    c: integer;
  begin
    result := false;
    for c := 0 to LinkPoints.Count - 1 do
      if LinkPoints[c].Collapsed then
      begin
        result := true;
        exit;
      end;
  end;

begin
  inherited;
  Filer.DefineProperty('CollapsedNodes', LoadCollapsedNodesProp, StoreCollapsedNodesProp, HasCollapsedNodes);
end;

procedure TCustomDiagramBlock.DrawBlockInCanvas(Canvas: TCanvas; ARect: TRect);
begin
  DrawBlock(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(Square(ARect), Angle));
end;

procedure TCustomDiagramBlock.SetRotationStep(const Value: integer);
begin
  FRotationStep := Value;
  if FRotationStep <= 0 then
    FRotationStep := 1;
  if FRotationStep > 180 then
    FRotationStep := 180;
end;

procedure TCustomDiagramBlock.DrawHandles(AInfo: TDiagramDrawInfo);
var
  P1: TDot;
  P2: TDot;
  RotHandle: TStretchHandle;
begin
  {Draw the dotted line}
  if (Diagram <> nil) and (Diagram.HandlesStyle = hsVisio) then
    {Do not draw anything if the block behaves as a group member}
    if Selected and not AsMember then
    begin
      {Draw the surround rect}
      AInfo.Canvas.Pen.Width := 1;
      if DrawGrouped then
      begin
        AInfo.Canvas.Pen.Style := psSolid;
        AInfo.Canvas.Pen.Color := clFuchsia;
      end
      else
      begin
        AInfo.Canvas.Pen.Style := psDot;
        AInfo.Canvas.Pen.Color := clLime;
      end;
      AInfo.Canvas.Brush.Style := bsClear;
      TempDrawer.CurRect := Diagram.ClientToCanvas(BoundsRect);
      TempDrawer.Angle := Angle;
      TempDrawer.RotationType := rtCurRectCenter;
      TempDrawer.Canvas := AInfo.Canvas;
      with Diagram.ClientToCanvas(BoundsRect) do
        TempDrawer.DrawRotRect(Left, Top, Right, Bottom);

      {Now draw the line for the rotation}
      {Calculate the handle and rotation points}
      if CanRotate and not DrawGrouped then
      begin
        P1 := Diagram.ClientToCanvas(DiagramRotCenter);
        RotHandle := Handles.RotateHandle;
        if RotHandle <> nil then
        begin
          P2 := RotHandle.DiagramPoint;
          P2 := Diagram.ClientToCanvas(P2);

          {Draw the line}
          AInfo.Canvas.MoveTo(Round(P1.X), Round(P1.Y));
          AInfo.Canvas.LineTo(Round(P2.X), Round(P2.Y));
        end;
      end;
    end;

  {Now draw the handles}
  inherited;
end;

procedure TCustomDiagramBlock.PenChanged;
begin

end;

function TCustomDiagramBlock.TranslateRotCenter(ARect: TSquare; AAngle: double; ARotCenter: TDot): TDot;
begin
  FTempDrawer.PushState;
  //FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.OriginalRect := Square(0, 0, 100, 100);
  FTempDrawer.CurRect := ARect;
  FTempDrawer.Angle := AAngle;
  FTempDrawer.RotationType := rtCurRectCenter;
  result := FTempDrawer.RotX(FTempDrawer.PP(ARotCenter.X, ARotCenter.Y));
  FTempDrawer.PopState;
end;

function TCustomDiagramBlock.DiagramRotCenter: TDot;
begin
  result := TranslateRotCenter(BoundsRect, Angle, FRotCenter);
end;

procedure TCustomDiagramBlock.SetRotationCenter(const Value: TDot);
begin
  if (FRotCenter.X <> Value.X) or (FRotCenter.Y <> Value.Y) then
  begin
    FRotCenter := Value;
    if Assigned(FDiagram) then
    begin
      FDiagram.Modified;
      if FDiagram.FUpdatingPos = 0 then
        FDiagram.Redraw;
    end;
  end;
end;

{ TDiagramScrollBar }

constructor TDiagramScrollBar.Create(ADiagram: TatDiagram;
  AKind: TScrollBarKind);
begin
  inherited Create;
  FDiagram := ADiagram;
  FKind := AKind;
  FPageIncrement := 80;
  FIncrement := FPageIncrement div 10;
  FVisible := True;
  FDelay := 10;
  FLineDiv := 4;
  FPageDiv := 12;
  FColor := clBtnHighlight;
  FParentColor := True;
  FUpdateNeeded := True;
end;

function TDiagramScrollBar.IsIncrementStored: Boolean;
begin
  Result := not Smooth;
end;

procedure TDiagramScrollBar.Assign(Source: TPersistent);
begin
  if Source is TDiagramScrollBar then
  begin
    Visible := TDiagramScrollBar(Source).Visible;
    Range := TDiagramScrollBar(Source).Range;
    Position := TDiagramScrollBar(Source).Position;
    Increment := TDiagramScrollBar(Source).Increment;
    Exit;
  end;
  inherited Assign(Source);
end;

procedure TDiagramScrollBar.ChangeBiDiPosition;
begin
  if Kind = sbHorizontal then
    if IsScrollBarVisible then
      if not FDiagram.UseRightToLeftScrollBar then
        Position := 0
      else
        Position := Range;
end;

function TDiagramScrollBar.IsScrollBarVisible: Boolean;
var
  Style: Longint;
begin
  Style := WS_HSCROLL;
  if Kind = sbVertical then Style := WS_VSCROLL;
  Result := (Visible) and
            (GetWindowLong(FDiagram.Handle, GWL_STYLE) and Style <> 0);
end;

function TDiagramScrollBar.ControlSize(ControlSB, AssumeSB: Boolean): Integer;
var
  BorderAdjust: Integer;

  function ScrollBarVisible(Code: Word): Boolean;
  var
    Style: Longint;
  begin
    Style := WS_HSCROLL;
    if Code = SB_VERT then Style := WS_VSCROLL;
    Result := GetWindowLong(FDiagram.Handle, GWL_STYLE) and Style <> 0;
  end;

  function Adjustment(Code, Metric: Word): Integer;
  begin
    Result := 0;
    if not ControlSB then
      if AssumeSB and not ScrollBarVisible(Code) then
        Result := -(GetSystemMetrics(Metric) - BorderAdjust)
      else if not AssumeSB and ScrollBarVisible(Code) then
        Result := GetSystemMetrics(Metric) - BorderAdjust;
  end;

begin
  BorderAdjust := Integer(GetWindowLong(FDiagram.Handle, GWL_STYLE) and
    (WS_BORDER or WS_THICKFRAME) <> 0);
  if Kind = sbVertical then
    Result := FDiagram.ClientHeight + Adjustment(SB_HORZ, SM_CXHSCROLL) else
    Result := FDiagram.ClientWidth + Adjustment(SB_VERT, SM_CYVSCROLL);
end;

function TDiagramScrollBar.GetScrollPos: Integer;
begin
  Result := 0;
  if Visible then Result := Position;
end;

function TDiagramScrollBar.NeedsScrollBarVisible: Boolean;
begin
  Result := FRange > ControlSize(False, False);
end;

procedure TDiagramScrollBar.ScrollMessage(var Msg: TWMScroll);
var
  Incr, FinalIncr, Count: Integer;
  CurrentTime, StartTime, ElapsedTime: Longint;

  function GetRealScrollPosition: Integer;
  var
    SI: TScrollInfo;
    Code: Integer;
  begin
    SI.cbSize := SizeOf(TScrollInfo);
    SI.fMask := SIF_TRACKPOS;
    Code := SB_HORZ;
    if FKind = sbVertical then Code := SB_VERT;
    Result := Msg.Pos;
    if FlatSB_GetScrollInfo(FDiagram.Handle, Code, SI) then
      Result := SI.nTrackPos;
  end;

begin
  with Msg do
  begin
    if FSmooth and (ScrollCode in [SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN]) then
    begin
      case ScrollCode of
        SB_LINEUP, SB_LINEDOWN:
          begin
            Incr := FIncrement div FLineDiv;
            FinalIncr := FIncrement mod FLineDiv;
            Count := FLineDiv;
          end;
        SB_PAGEUP, SB_PAGEDOWN:
          begin
            Incr := FPageIncrement;
            FinalIncr := Incr mod FPageDiv;
            Incr := Incr div FPageDiv;
            Count := FPageDiv;
          end;
      else
        Count := 0;
        Incr := 0;
        FinalIncr := 0;
      end;
      CurrentTime := 0;
      while Count > 0 do
      begin
        StartTime := GetCurrentTime;
        ElapsedTime := StartTime - CurrentTime;
        if ElapsedTime < FDelay then Sleep(FDelay - ElapsedTime);
        CurrentTime := StartTime;
        case ScrollCode of
          SB_LINEUP: SetPosition(FPosition - Incr);
          SB_LINEDOWN: SetPosition(FPosition + Incr);
          SB_PAGEUP: SetPosition(FPosition - Incr);
          SB_PAGEDOWN: SetPosition(FPosition + Incr);
        end;
        FDiagram.Update;
        Dec(Count);
      end;
      if FinalIncr > 0 then
      begin
        case ScrollCode of
          SB_LINEUP: SetPosition(FPosition - FinalIncr);
          SB_LINEDOWN: SetPosition(FPosition + FinalIncr);
          SB_PAGEUP: SetPosition(FPosition - FinalIncr);
          SB_PAGEDOWN: SetPosition(FPosition + FinalIncr);
        end;
      end;
    end
    else
      case ScrollCode of
        SB_LINEUP: SetPosition(FPosition - FIncrement);
        SB_LINEDOWN: SetPosition(FPosition + FIncrement);
        SB_PAGEUP: SetPosition(FPosition - ControlSize(True, False));
        SB_PAGEDOWN: SetPosition(FPosition + ControlSize(True, False));
        SB_THUMBPOSITION:
            if FCalcRange > 32767 then
              SetPosition(GetRealScrollPosition) else
              SetPosition(Pos);
        SB_THUMBTRACK:
          if Tracking then
            if FCalcRange > 32767 then
              SetPosition(GetRealScrollPosition) else
              SetPosition(Pos);
        SB_TOP: SetPosition(0);
        SB_BOTTOM: SetPosition(FCalcRange);
        SB_ENDSCROLL: begin end;
      end;
  end;
  if FDiagram <> nil then
    FDiagram.SetFocus;
end;

procedure TDiagramScrollBar.SetColor(Value: TColor);
begin
  if Value <> Color then
  begin
    FColor := Value;
    FParentColor := False;
    FUpdateNeeded := True;
    FDiagram.UpdateScrollBars;
  end;
end;

procedure TDiagramScrollBar.SetParentColor(Value: Boolean);
begin
  if ParentColor <> Value then
  begin
    FParentColor := Value;
    if Value then Color := clBtnHighlight;
  end;
end;

procedure TDiagramScrollBar.SetPosition(Value: Integer);
var
  Code: Word;
  (*Form: TCustomForm;
  OldPos: Integer;*)
begin
  if csReading in FDiagram.ComponentState then
    FPosition := Value
  else
  begin
    if Kind = sbHorizontal then
      Code := SB_HORZ else
      Code := SB_VERT;
    if Value > FCalcRange then Value := FCalcRange
    else if Value < 0 then Value := 0;
    (*if Value <> FPosition then
    begin
      OldPos := FPosition;
      FPosition := Value;
      if Kind = sbHorizontal then
        FDiagram.ScrollBy(OldPos - Value, 0) else
        FDiagram.ScrollBy(0, OldPos - Value);
      if csDesigning in FDiagram.ComponentState then
      begin
        Form := GetParentForm(FDiagram);
        if (Form <> nil) and (Form.Designer <> nil) then Form.Designer.Modified;
      end;
    end;*)
    FPosition := Value;
    if FlatSB_GetScrollPos(FDiagram.Handle, Code) <> FPosition then
      FlatSB_SetScrollPos(FDiagram.Handle, Code, FPosition, True);
  end;
end;

procedure TDiagramScrollBar.SetSize(Value: Integer);
const
  SysConsts: array[TScrollBarKind] of Integer = (SM_CYHSCROLL, SM_CYVSCROLL);
var
  NewValue: Integer;
begin
  if Value <> Size then
  begin
    NewValue := Value;
    if NewValue = 0 then
      Value := GetSystemMetrics(SysConsts[Kind]);
    FSize := Value;
    FUpdateNeeded := True;
    FDiagram.UpdateScrollBars;
    if NewValue = 0 then
      FSize := 0;
  end;
end;

procedure TDiagramScrollBar.SetStyle(Value: TScrollBarStyle);
begin
  if Style <> Value then
  begin
    FStyle := Value;
    FUpdateNeeded := True;
    FDiagram.UpdateScrollBars;
  end;
end;

procedure TDiagramScrollBar.SetThumbSize(Value: Integer);
begin
  if Value <> ThumbSize then
  begin
    FThumbSize := Value;
    FUpdateNeeded := True;
    FDiagram.UpdateScrollBars;
  end;
end;

procedure TDiagramScrollBar.DoSetRange(Value: Integer);
begin
  FRange := Value;
  if FRange < 0 then FRange := 0;
  FDiagram.UpdateScrollBars;
end;

procedure TDiagramScrollBar.SetRange(Value: Integer);
begin
  DoSetRange(Value);
end;

procedure TDiagramScrollBar.SetVisible(Value: Boolean);
begin
  FVisible := Value;
  FDiagram.UpdateScrollBars;
end;

procedure TDiagramScrollBar.Update(ControlSB, AssumeSB: Boolean);
type
  TPropKind = (pkStyle, pkButtonSize, pkThumbSize, pkSize, pkBkColor);
const
  Props: array[TScrollBarKind, TPropKind] of Integer = (
    { Horizontal }
    (WSB_PROP_HSTYLE, WSB_PROP_CXHSCROLL, WSB_PROP_CXHTHUMB, WSB_PROP_CYHSCROLL,
     WSB_PROP_HBKGCOLOR),
    { Vertical }
    (WSB_PROP_VSTYLE, WSB_PROP_CYVSCROLL, WSB_PROP_CYVTHUMB, WSB_PROP_CXVSCROLL,
     WSB_PROP_VBKGCOLOR));
  Kinds: array[TScrollBarKind] of Integer = (WSB_PROP_HSTYLE, WSB_PROP_VSTYLE);
  Styles: array[TScrollBarStyle] of Integer = (FSB_REGULAR_MODE,
    FSB_ENCARTA_MODE, FSB_FLAT_MODE);
var
  Code: Word;
  ScrollInfo: TScrollInfo;

  procedure UpdateScrollProperties(Redraw: Boolean);
  begin
    FlatSB_SetScrollProp(FDiagram.Handle, Props[Kind, pkStyle], Styles[Style], Redraw);
    if ThumbSize > 0 then
      FlatSB_SetScrollProp(FDiagram.Handle, Props[Kind, pkThumbSize], ThumbSize, False);
    if Size > 0 then
      FlatSB_SetScrollProp(FDiagram.Handle, Props[Kind, pkSize], Size, False);
    FlatSB_SetScrollProp(FDiagram.Handle, Props[Kind, pkBkColor],
      ColorToRGB(Color), False);
  end;

begin
  FCalcRange := 0;
  Code := SB_HORZ;
  if Kind = sbVertical then Code := SB_VERT;
  if Visible then
  begin
    FCalcRange := Range - ControlSize(ControlSB, AssumeSB);
    if FCalcRange < 0 then FCalcRange := 0;
  end;
  ScrollInfo.cbSize := SizeOf(ScrollInfo);
  ScrollInfo.fMask := SIF_ALL;
  ScrollInfo.nMin := 0;
  if FCalcRange > 0 then
    ScrollInfo.nMax := Range else
    ScrollInfo.nMax := 0;
  ScrollInfo.nPage := ControlSize(ControlSB, AssumeSB) + 1;
  ScrollInfo.nPos := FPosition;
  ScrollInfo.nTrackPos := FPosition;
  UpdateScrollProperties(FUpdateNeeded);
  FUpdateNeeded := False;
  FlatSB_SetScrollInfo(FDiagram.Handle, Code, ScrollInfo, True);
  SetPosition(FPosition);
  FPageIncrement := (ControlSize(True, False) * 9) div 10;
  if Smooth then FIncrement := FPageIncrement div 10;
end;

function TDiagramScrollBar.IsRangeStored: Boolean;
begin
  Result := not FDiagram.AutoScroll and not FDiagram.AutoPage;
end;

{$IFNDEF DELPHI6_LVL}
function FindGlobalComponent(const Name: string): TComponent;
var
  I: Integer;
begin
  for I := 0 to Screen.FormCount - 1 do
  begin
    Result := Screen.Forms[I];
    if not (csInline in Result.ComponentState) and
      (CompareText(Name, Result.Name) = 0) then Exit;
  end;
  for I := 0 to Screen.DataModuleCount - 1 do
  begin
    Result := Screen.DataModules[I];
    if CompareText(Name, Result.Name) = 0 then Exit;
  end;
  Result := nil;
end;
{$ENDIF}

var
  GlobalDiagramOwner: TComponent = nil;

function FindGlobalDiagramOwner(const Name: string): TComponent;
const
  SOwner = '.Owner';
begin
  {$IFNDEF DELPHI6_LVL}
  result := FindGlobalComponent(Name);
  if result <> nil then Exit;
  {$ENDIF}
  if (Name = SDiagramOwnerName) then
    result := GlobalDiagramOwner
  else
  if Copy(Name, Length(Name) - Length(SOwner) + 1, MaxInt) = SOwner then
    result := GlobalDiagramOwner
  else
    result := nil;
end;

{ TatDiagram }

constructor TatDiagram.Create(AOwner: TComponent);
begin
  if GetClass(TGroupBlock.ClassName) = nil then
    RegisterClass(TGroupBlock);
  inherited Create(AOwner);

  FSubList := TInterfaceList.Create;

  FIntMeasUnit := duCenti;
  FWheelZoom := false;
  FWheelZoomIncrement := 10;
  FWheelZoomMin := 10;
  FWheelZoomMax := 500;

  FLayers := TDiagramLayers.Create(Self);

  FPageSettings := TDiagramPageSettings.Create(Self);
  FPageSettings.RetrieveDefaultSize;

  FPrintSettings := TDiagramPrintSettings.Create;
  //FSelectionMode := slmGroup;
  FFixedSideLines := false;
  FSelectionMode := slmMultiple;
  FSmoothMode := smAntiAlias;
  FTextRenderingMode := tmAntiAlias;
  FCanMoveOutOfBounds := true;

  FGraphicLib := DefaultGraphicLib;
  FCacheImage := TBitmap.Create;
  FAdjustRatioX := 1;
  FAdjustRatioY := 1;
  ControlState := ControlState + [csCustomPaint];
  ControlStyle := ControlStyle - [csAcceptsControls] + [csReplicatable];
  {$IFDEF DELPHI7_LVL}
  ControlStyle := ControlStyle + [csNeedsBorderPaint];
  {$ENDIF}
  FKeyActions := [kaEscape, kaMove, kaPage, kaResize, kaSelect];
  FKeepInserting := false;
  FClipPrefix := '___clip___';
  FLinkCursor := crHandPoint;
  FPanCursor := crHandPoint;
  FZoomCursor := crDefault;
  FAutoScroll := true;
  FAutoPage := false;
  FHorzScrollBar := TDiagramScrollBar.Create(Self, sbHorizontal);
  FVertScrollBar := TDiagramScrollBar.Create(Self, sbVertical);
  FUndoStack := TDiagramUndoStack.Create(Self);
  FPreviousDControl := nil;
  FBorderStyle := bsSingle;
  FBlocks := TList.Create;
  FLinks := TList.Create;
  FDControls := TList.Create;
  FSelecteds := TList.Create;
  FSnapGrid := TSnapGrid.Create(Self);
  FPageLines := TPageLines.Create(Self);
  FBackground := TDiagramBackground.Create(Self);
  FControlCanvas := TControlCanvas.Create;
  TControlCanvas(FControlCanvas).Control := Self;
  FTopRuler := TDiagramRuler.Create(Self);
  FTopRuler.Position := rpTop;
  FLeftRuler := TDiagramRuler.Create(Self);
  FLeftRuler.Position := rpLeft;
  FConnOff := 10;
  FInternalStreaming := False;
  FClipboardStreaming := False;
  FInternalUpdating := 0;
  FUpdatingPos := 0;
  FUpdatingSel := 0;
  FUpdateCount := 0;
  FRedrawRequestCount := 0;
  FPainting := False;
  FBorderColor := clGray;
  FExportBackgroundColor := clWhite;
  Width := 200;
  Height := 200;
  ParentCtl3d := false;
  Ctl3d := false;
  DoubleBuffered := True;
  FNettoExportOffset := 3;
  SetDiagramDefaults;
  ClearUndoStack;
  FDragStyle := dsOutline;
  FHandlesStyle := hsClassic;

  FSelBlock := TSelectionBlock.Create(nil);
  FSelBlock.Visible := false;
  {Set FDiagram field (not Diagram property) to avoid this block to
   be included in lists like DControls and Blocks}
  FSelBlock.FDiagram := Self;

  {Call updateblockhandles after diagram is assigned}
  FSelBlock.UpdateBlockHandles;
end;

destructor TatDiagram.Destroy;
begin
  FPrintSettings.Free;
  FPageSettings.Free;
  FSelBlock.Free;
  FCacheImage.Free;
  ClearDControls;
  FSelecteds.Free;
  FDControls.Free;
  FBlocks.Free;
  FLinks.Free;
  FControlCanvas.Free;
  FUndoStack.Free;
  FBackground.Free;
  FSnapGrid.Free;
  FPageLines.Free;
  FTopRuler.Free;
  FLeftRuler.Free;
  if FInplaceMemo <> nil then
    FInplaceMemo.Free;
  FHorzScrollBar.Free;
  FVertScrollBar.Free;
  FLayers.Free;
  FSubList.Free;
  inherited;
end;
          
procedure TatDiagram.DestroyInsertControl;
begin
  if FInsertDControl <> nil then
  begin
    if FInsertDControl.IsGroup then
      TBaseGroupBlock(FInsertDControl).DestroyMembers;
    FInsertDControl.Free;
    FInsertDControl := nil;
  end;
end;

procedure TatDiagram.SetDiagramDefaults;
begin
  FMouseState := msBrowsing;
  FShowLinkPoints := True;
  FZoomRatio := 1;
  FPreviewZoom := 1;

  PageSettings.RetrieveDefaultSize;

  With FBackground do
  begin
    Visible := False;
    Bitmap.Width := 0;
    Bitmap.Height := 0;
    if Picture.Graphic <> nil then
      Picture.Graphic := nil;
  end;
  With FSnapGrid do
  begin
    Visible := False;
    Active := False;
  end;
  With FPageLines do
  begin
    Visible := False;
  end;
end;

procedure TatDiagram.GetChildren(Proc: TGetChildProc; Root: TComponent);
var
  c: Integer;
  AOldName: string;
begin
  if (Owner <> nil) and not (csDesigning in ComponentState) then
  begin
    AOldName := Owner.Name;
    Owner.Name := SDiagramOwnerName;
  end;

  try
    inherited GetChildren(Proc, Root);
    if (csDesigning in ComponentState) or FInternalStreaming or FClipboardStreaming then
    begin
      for c := 0 to DControlCount - 1 do
        if (not FClipboardStreaming) or (Pos(FClipPrefix, DControls[c].Name) <> 0) then
          Proc(DControls[c]);
    end;

  finally
    if (Owner <> nil) and not (csDesigning in ComponentState) then
    begin
      Owner.Name := AOldName;
    end;
  end;
end;

procedure TatDiagram.Redraw;
begin
  if FUpdateCount > 0 then
    Inc(FRedrawRequestCount);

  if (FUpdateCount = 0) and not FPainting and (Parent <> nil) then
    Invalidate;
end;

function TatDiagram.ClipRect(ACanvas: TCanvas): TSquare;
begin
  case FPaintStyle of
    psScreen:
      result := Square(ACanvas.ClipRect);
    psPrint:
      result := PageSettings.PrintPageRect;
    psPreview:
      result := Square(0, 0, PrintClientWidth, PrintClientHeight);
    psBitmap:
      result := Square(0, 0, Max(HorzScrollBar.Range, ClientWidth), Max(VertScrollBar.Range, ClientHeight));
  end;
end;

procedure TatDiagram.InternalPaint(ACanvas: TCanvas);

var
  dgrPaintEngine: TDgrPaintEngine;

  procedure DrawRulers(AGrid: boolean);
  begin
    FTopRuler.GridColor := SnapGrid.Pen.Color;
    FLeftRuler.GridColor := SnapGrid.Pen.Color;
    if FLeftRuler.Size = 0 then FLeftRuler.UpdateSize(0);
    if FTopRuler.Size = 0 then FTopRuler.UpdateSize(0);

    if FTopRuler.Visible then
        FTopRuler.Draw(ACanvas, Rect(FLeftRuler.VisibleSize, 0,
          ClientWidth, FTopRuler.Size),
          HorzScrollBar.Position,
          ClientHeight, AGrid);

    if FLeftRuler.Visible then
        FLeftRuler.Draw(ACanvas, Rect(0, FTopRuler.VisibleSize, FLeftRuler.Size,
          ClientHeight),
          VertScrollBar.Position,
          ClientWidth, AGrid);
    {Draw corner}
    if FTopRuler.Visible and FLeftRuler.Visible then
      With ACanvas do
      begin
        Brush.Style := bsSolid;
        Brush.Color := FTopRuler.Color;
        FillRect(Rect(0, 0, FLeftRuler.Size, FTopRuler.Size));
        Pen.Color := clYellow;
        Pen.Color := FTopRuler.TickColor;
        Pen.Style := psSolid;
        Pen.Width := 1;
        MoveTo(FLeftRuler.Size, 0);
        LineTo(FLeftRuler.Size, FTopRuler.Size);
        Pen.Color := clYellow;
        Pen.Color := FLeftRuler.TickColor;
        LineTo(-1, FTopRuler.Size);
      end;
  end;

  {$IFDEF FREEWARE}
  procedure DrawWatermark;
  var
    SaveFont: TFont;
    SaveBrush: TBrush;
  begin
    SaveFont := TFont.Create;
    SaveBrush := TBrush.Create;
    with ACanvas do
    try
      SaveFont.Assign(Font);
      SaveBrush.Assign(Brush);
      Brush.Style := bsClear;
      Font.Name := 'Arial';
      Font.Size := 12;
      Font.Color := clBlack;
      Font.Size := 10;
      TextOut(Round(CanvasOffset.X + 5), Round(CanvasOffset.Y + 5), 'copyright (c) 2003-2011 - TMS software');
      Brush.Assign(SaveBrush);
    finally
      SaveBrush.Free;
      SaveFont.Free;
    end;
  end;
  {$ENDIF}

  procedure InternalDrawControl(AControl: TDiagramControl);
  var
    CanvasRect: TSquare;
  begin
    if AControl.IsVisible then
    begin
      AControl.CalcSize(ACanvas);
      if AControl.Angle = 0 then
        CanvasRect := ClientToCanvas(AControl.ControlRect)
      else
        CanvasRect := ClientToCanvas(AControl.SurroundRect);
      if RectsTouch(ClipRect(ACanvas), CanvasRect) then
        AControl.Paint(ACanvas, dgrPaintEngine);
    end;
  end;

  type
    TIntPaintModes = set of TDControlPaintMode;

  procedure DrawControl(AControl: TDiagramControl; APaintModes: TIntPaintModes);
  var
    c: integer;
  begin
    if AControl.IsGroup then
    begin
      for c := 0 to TBaseGroupBlock(AControl).Members.Count - 1 do
        if TBaseGroupBlock(AControl).Members[c].DControl <> nil then
          DrawControl(TBaseGroupBlock(AControl).Members[c].DControl, APaintModes);
    end
    else
      if AControl.PaintMode in APaintModes then
        InternalDrawControl(AControl);
  end;

  procedure _DrawBlockHandles(AControl: TDiagramControl);
  var
    c: Integer;
  begin
    if AControl.IsVisible then
    begin
      AControl.DrawHandles(AControl.DiagramDrawInfo(ACanvas, AControl.Drawer, AControl.FDgrDrawer));
      if AControl.IsGroup then
        for c := 0 to TBaseGroupBlock(AControl).Members.Count - 1 do
          if TBaseGroupBlock(AControl).Members[c].DControl <> nil then
            _DrawBlockHandles(TBaseGroupBlock(AControl).Members[c].DControl);
    end;
  end;

var
  c: integer;
  {$IFDEF TRIAL}
  msg: string;
  i: integer;
  {$ENDIF}
begin
  {$IFDEF TRIAL}
  if (Now > 41100) or (Now < 40520) then
  begin
    msg := 'Tfhfiesg hDjigafgdrsacmt yShtgufdeidof dvcevrfseidofnd wheassz aesxdpfitryeudu.g fPvldecaxssed rrteyguiishtjehrg,f dosrd s'+
      'tdor tcfognvtcisnwueed sewvratlhunagtbifnvgc sdzotwfnglhoiaodk jam nnhegwf d'+
      'vdefrrstigovnc sfdreormf xhttgtyph:j/u/iwnwhwg.btfmvscsdowfqtawsadreer.fcrotm';
    i := 2;
    while i < length(msg) do
    begin
      System.Delete(msg, i, 1);
      inc(i);
    end;

    ACanvas.Font.Name := 'Arial';
    ACanvas.Font.Size := 20;
    ACanvas.Font.Color := clBlack;
    ACanvas.Font.Size := 10;
    ACanvas.TextOut(Round(CanvasOffset.X + 5), Round(CanvasOffset.Y + 5), msg);
    exit;
  end;
  {$ENDIF}

  FPainting := True;
  if PathDrawingMode then
    dgrPaintEngine := DgrPaintEngineClass.Create(ACanvas, GetPaintRect(ACanvas))
  else
    dgrPaintEngine := nil;

  try
    if dgrPaintEngine <> nil then
    begin
      dgrPaintEngine.SmoothMode := FSmoothMode;
      dgrPaintEngine.TextRenderingMode := FTextRenderingMode;
    end;

    if not FUseCache then
    begin
      {Draw background rect}
      ACanvas.Brush.Style := bsSolid;
      if FPaintStyle = psScreen then
      begin
        {$IFDEF DELPHI7_LVL}
        if not ParentBackground then
        {$ELSE}
        if true then
        {$ENDIF}
        begin
          ACanvas.Brush.Color := Self.Color;
          ACanvas.FillRect(ToRect(ClipRect(ACanvas)));
        end;
      end
      else
      if FPaintStyle = psBitmap then
      begin
        ACanvas.Brush.Color := FExportBackgroundColor;
        ACanvas.FillRect(ACanvas.ClipRect);
      end;

      {BeforePaint event}
      if Assigned(FOnBeforePaint) then
        FOnBeforePaint(Self, ACanvas);

      {Draw background image}
      if FBackground.Visible then
        FBackground.Draw(ACanvas);

      {Draw rulers}
      if FPaintStyle = psScreen then
        DrawRulers(SnapGrid.Visible and (SnapGrid.Style = gsRuler));

      {Draw snap grid}
      if FPaintStyle = psScreen then
        if SnapGrid.Visible and (SnapGrid.Style <> gsRuler) then
          SnapGrid.Draw(ACanvas);

      {Draw page lines}
      if FPaintStyle = psScreen then
        if PageLines.Visible then
          PageLines.Draw(ACanvas);
    end
    else
    begin
      {Draw cached background image}
      ACanvas.Draw(0, 0, FCacheImage);
    end;

    {Scale transform for gdi+}
    if dgrPaintEngine <> nil then
    begin
      dgrPaintEngine.BeginDraw;
      dgrPaintEngine.ResetTransform;
      TransformPaintEngine(dgrPaintEngine);
      if FPaintStyle = psBitmap then
        dgrPaintEngine.FillRect(ACanvas.ClipRect, FExportBackgroundColor);
    end;

    if not FUseCache then
    begin
      {Paint background controls first}
      for c := 0 to DControlCount - 1 do
        if DControls[c].IsBackgroundControl
          and not DControls[c].IsMember then
          DrawControl(DControls[c], [pmNormal, pmNormalCursor]);

      if PaintLinesFirst then
      begin
        {Paint lines}
        for c := 0 to DControlCount - 1 do
          if (DControls[c] is TCustomDiagramLine) and not DControls[c].IsBackgroundControl
            and not DControls[c].IsMember then
            DrawControl(DControls[c], [pmNormal, pmNormalCursor]);

        {Paint blocks}
        for c := 0 to DControlCount - 1 do
          if not (DControls[c] is TCustomDiagramLine) and not DControls[c].IsBackgroundControl
            and not DControls[c].IsMember then
              DrawControl(DControls[c], [pmNormal, pmNormalCursor]);
      end
      else
      begin
        {Paint controls}
        for c := 0 to DControlCount - 1 do
          if not DControls[c].IsBackgroundControl
            and not DControls[c].IsMember then
              DrawControl(DControls[c], [pmNormal, pmNormalCursor]);
      end;
    end;

    {Set cursor placement for blocks being moved/resized, and then paint cursor controls}
    for c := 0 to DControlCount - 1 do
      if DControls[c].PaintMode in [pmNormalCursor, pmCursorOnly] then
        DControls[c].StartCursorPlacement;
    for c := 0 to DControlCount - 1 do
      if not DControls[c].IsMember then
        DrawControl(DControls[c], [pmNormalCursor, pmCursorOnly]);

    {Paint nodes}
    for c := 0 to DControlCount - 1 do
      if DControls[c].IsVisible then
        DControls[c].DrawLinkPoints(ACanvas, dgrPaintEngine, true);

    if dgrPaintEngine <> nil then
      dgrPaintEngine.EndDraw;

    {Draw handles of controls}
    if FPaintStyle = psScreen then
      for c := 0 to DControlCount - 1 do
        if not DControls[c].IsMember then
          { optimization when checking for selected or isgroup, it's not effectively
           needed because the blocks will draw its handles accordingly. This is just for
           optimization. The test above, AsMember, *is* needed to make handles to be drawn
           correctly (members on top of groups) }
          if DControls[c].IsVisible and (DControls[c].Selected or DControls[c].IsGroup) then
            _DrawBlockHandles(DControls[c]);

    {Draw handles of selection block, if it's in group selection mode}
    if GroupSelection and not FDoingCache then
    begin
      if FSelBlock.PaintMode in [pmNormalCursor, pmCursorOnly] then
        FSelBlock.StartCursorPlacement;
      FSelBlock.DrawHandles(FSelBlock.DiagramDrawInfo(ACanvas, FSelBlock.Drawer, FSelBlock.FDgrDrawer));
      if FSelBlock.PaintMode in [pmNormalCursor, pmCursorOnly] then
        FSelBlock.EndCursorPlacement;
    end;

    {Restore cursor placement for blocks being moved/resized}
    for c := 0 to DControlCount - 1 do
      if DControls[c].PaintMode in [pmNormalCursor, pmCursorOnly] then
        DControls[c].EndCursorPlacement;

    if not FUseCache then
    begin
      if FPaintStyle = psScreen then
        DrawRulers(false);

      {AfterPaint event}
      if Assigned(FOnAfterPaint) then
        FOnAfterPaint(Self, ACanvas);
    end;

    {Draw watermark}
    {$IFDEF FREEWARE}
    DrawWatermark;
    {$ENDIF}
  finally
    FPainting := False;
    if dgrPaintEngine <> nil then
      dgrPaintEngine.Free;
  end;
end;

procedure TatDiagram.DoUnselectDControl(ADControl: TDiagramControl);
begin
  FSelecteds.Remove(ADControl);
  BeginUpdateSel;
  try
    if Assigned(FOnUnselectDControl) then
      FOnUnselectDControl(Self,ADControl);
    DesignEvent(Self, etControlUnselected, 0);
  finally
    EndUpdateSel;
  end;
end;

procedure TatDiagram.DoSelectDControl(ADControl: TDiagramControl);
begin
  if Assigned(ADControl) and (FSelecteds.IndexOf(ADControl) = -1) then
    FSelecteds.Add(ADControl);
  BeginUpdateSel;
  try
    if Assigned(FOnSelectDControl) then
      FOnSelectDControl(Self,ADControl);
    DesignEvent(Self, etControlSelected, 0);
  finally
    EndUpdateSel;
  end;
end;

procedure TatDiagram.SelectAll;
var c: integer;
begin
  BeginUpdateSel;
  try
    for c := 0 to DControlCount - 1 do
      if DControls[c].IsVisible and not DControls[c].Selected then
        if not DControls[c].IsMember then //just for optimization, would work without this test
          DControls[c].Selected := true;
  finally
    EndUpdateSel;
  end;
end;

procedure TatDiagram.UnselectAll;
var c: integer;
begin
  BeginUpdateSel;
  BeginUpdate;
  try
    for c := 0 to DControlCount - 1 do
      if DControls[c].IsVisible and DControls[c].Selected then
        DControls[c].Selected := false;
  finally
    EndUpdate;
    EndUpdateSel;
  end;
end;

function TatDiagram.SnapCanvasPoint(APoint: TDot): TDot;
begin
  result := APoint;
  if SnapGrid.Active then
    result := ClientToCanvas(SnapDeltaPoint(CanvasToClient(APoint)));
end;

function TatDiagram.SnapDeltaPoint(APoint: TDot): TDot;
begin
  result := APoint;
  if SnapGrid.Active then
  begin
    result.X := (round(APoint.X / SnapGrid.SizeX) * SnapGrid.SizeX);
    result.Y := (round(APoint.Y / SnapGrid.SizeY) * SnapGrid.SizeY);
  end
end;

procedure TatDiagram.MoveBlocks(ADeltaX, ADeltaY: number; AOnlySelected: boolean = false; AOnlyMovable: boolean = true);
var
  c: integer;
begin
  if (ADeltaX <> 0) or (ADeltaY <> 0) then
  begin
    BeginInternalUpdate;
    BeginUpdatePos;
    try
      for c := 0 to DControlCount - 1 do
        if (DControls[c].Selected or not AOnlySelected) and
          (DControls[c].CanMove or not AOnlyMovable) and not DControls[c].AsMember then
          DControls[c].MovePosition(ADeltaX, ADeltaY);
      if GroupSelection then
        FSelBlock.MovePosition(ADeltaX, ADeltaY);
    finally
      EndUpdatePos;
      EndInternalUpdate;
      for c := 0 to DControlCount - 1 do
        DControls[c].NotifyAnchoreds(acPosition);
    end;
  end;
end;

procedure TatDiagram.StartMoving;
var c: integer;
begin
  {$IFDEF ISDELPHI}
  SetLength(FControlMoveInfos, 0);
  SetLength(FControlMoveInfos, SelectedCount);
  if GroupSelection then
    SetLength(FControlMoveInfos, SelectedCount + 1);
  {$ELSE}
  if SelectedCount > MaxControlMoveInfos then
  begin
    FMouseState := msBrowsing;
    raise Exception.CreateFmt('Internal error - cannot move more than %d objects', [MaxControlMoveInfos]);
  end;
  {$ENDIF}

  {Create a copy of the background to be used as cache}
  BeginCacheBitmap;
  for c := 0 to SelectedCount - 1 do
  begin
    Selecteds[c].StartMoving;

    {Set selected controls to be painted as cursors}
    if FDragStyle = dsShape then
    begin
      Selecteds[c].PaintMode := pmNormalCursor;
    end;
  end;

  if GroupSelection then
  begin
    FSelBlock.StartMoving;
    if FDragStyle = dsShape then
      FSelBlock.PaintMode := pmCursorOnly;
  end;

  if Assigned(FOnBeforeMove) then
    FOnBeforeMove(Self);
end;

function TatDiagram.UpdateMoving(XPos, YPos: number): boolean;
var
  c: integer;
begin
  if FDragStyle = dsOutline then
  begin
    if not IsDummyPoint(FOldDragPos) then
    begin
      UpdateControlMoveInfos(XPos, YPos, caEraseCursor);
      for c := 0 to SelectedCount - 1 do
        Selecteds[c].UpdateMoving(FControlMoveInfos[c]);
      if GroupSelection then
        FSelBlock.UpdateMoving(FControlMoveInfos[SelectedCount]);
    end;
    UpdateControlMoveInfos(XPos, YPos, caDrawCursor);
  end
  else
    UpdateControlMoveInfos(XPos, YPos, caPositionCursor);

  result := false;
  for c := 0 to SelectedCount - 1 do
  begin
    Selecteds[c].UpdateMoving(FControlMoveInfos[c]);
    if FControlMoveInfos[c].AnchorPoint <> nil then
      result := true;
  end;
  if GroupSelection then
    FSelBlock.UpdateMoving(FControlMoveInfos[SelectedCount]);

  if Assigned(FOnMoving) then
    FOnMoving(Self, round(XPos), round(YPos));
  FOldDragPos := Dot(XPos, YPos);

  if FDragStyle = dsShape then
    Redraw;
end;

procedure TatDiagram.EndMoving(XPos, YPos: number; Cancel: boolean);
var
  c: integer;
  MovingPerformed: boolean;
begin
  MovingPerformed := false;
  {if FOldDragPos is dummypoint, then controls were not moved, but
   only clicked. Only do something if FOldDragPos is not a dummy point}
  if not IsDummyPoint(FOldDragPos) then
  begin
    UpdateControlMoveInfos(XPos, YPos, caEraseCursor);
    for c := 0 to SelectedCount - 1 do
      Selecteds[c].UpdateMoving(FControlMoveInfos[c]);
    if GroupSelection then
      FSelBlock.UpdateMoving(FControlMoveInfos[SelectedCount]);
    if not Cancel and not IsNullPoint(FControlMoveInfos[0].Delta) then
    begin
      BeginUpdatePos;
      try
        UpdateControlMoveInfos(XPos, YPos, caMoved);
        if GroupSelection then
          FSelBlock.UpdateMoving(FControlMoveInfos[SelectedCount])
        else
          for c := 0 to SelectedCount - 1 do if not Selecteds[c].AsMember then
          begin
            Selecteds[c].UpdateMoving(FControlMoveInfos[c]);
            Selecteds[c].RemoveAnchorsAfterMoving;
            if Selecteds[c].IsMember then
              Selecteds[c].GroupBlock.RecalculateRect;
          end;

        {If there is a link point defined, and it is for this control,
         then define it}
        With FControlMoveInfos[0] do
          if (LinkPoint <> nil) and (AnchorPoint <> nil)
             // check for required target stuff on lines
             and
             ( not Assigned(LinkPoint.DControl) or
             ( Assigned(LinkPoint.DControl) and
               ( (LinkPoint.DControl is TCustomDiagramLine)
                 and not TCustomDiagramLine(LinkPoint.DControl).RequiresConnections
               )
               or
               not (LinkPoint.DControl is TCustomDiagramLine)
             )
             )
          then
          begin
            LinkPoint.Anchor := AnchorPoint.DControl;
            LinkPoint.AnchorIndex := AnchorPoint.Index;
          end;
        for c := 0 to SelectedCount - 1 do
          if Selecteds[c].Selected then
            Selecteds[c].NotifyAnchoreds(acPosition);
        KeepInBounds;
      finally
        EndUpdatePos;
      end;
      MovingPerformed := true;
    end;
  end;

  {$IFDEF ISDELPHI}
  SetLength(FControlMoveInfos, 0);
  {$ENDIF}

  for c := 0 to SelectedCount - 1 do
    if FDragStyle = dsShape then
      Selecteds[c].PaintMode := pmNormal;
  if GroupSelection then
    FSelBlock.PaintMode := pmNormal;
  EndCacheBitmap;
  FMouseState := msBrowsing;
  CalcPanelSize;
  Redraw;
  if MovingPerformed then
  begin
    if Assigned(FOnAfterMove) then
      FOnAfterMove(Self);
    Modified;
    PushUndoStack('move');
  end;
  FOldDragPos := DummyPointX;
  Screen.Cursor := FOldScreenCursor;
end;

procedure TatDiagram.MoveControlIndex(ADControl: TDiagramControl; NewIndex: integer);
var
  i: integer;
begin
  i := FDControls.IndexOf(ADControl);
  if i <> -1 then
    FDControls.Move(i, NewIndex);
  Redraw;
end;

function TatDiagram.MoveDelta(ADControl: TDiagramControl;
  XPos, YPos: number): TDot;

  function IsShiftDown: Boolean;
  var
    State: TKeyboardState;
  begin
    GetKeyboardState(State);
    Result := (State[VK_CONTROL] and 128) <> 0;
  end;

begin
  result := Dot(0, 0);
  if (ADControl.Selected and ADControl.CanMove)
    or (ADControl = FSelBlock) then
  begin
    result := Dot(XPos - FDragStart.X, YPos - FDragStart.Y);
    if IsShiftDown then
      if Abs(result.X) >= Abs(result.Y) then
        result.Y := 0
      else
        result.X := 0
    else
    result := MulPoint(result, 1 / ZoomRatio);
    result := SnapDeltaPoint(result);
  end;
end;

procedure TatDiagram.UpdateControlMoveInfos(X, Y: number;
  AAction: TControlMoveAction);
var
  ALinkPoint: TLinkPoint;
  ALink: TCustomDiagramLine;
  AAnchorPoint: TLinkPoint;
  TestAnchor1, TestAnchor2: TLinkPoint;
  ADelta: TDot;
  ADelta2: TDot;
  c: integer;
  d: integer;
begin
  ALinkPoint := nil;
  AAnchorPoint := nil;
  if AAction <> caEraseCursor then
  begin
    {Check if any of lines moved is around a link point of non-moved objects.
    Check for non-member - for now, we will not allow group members to attach to other blocks}
    for c := 0 to SelectedCount - 1 do if not Selecteds[c].IsMember then
    begin
      if Selecteds[c].HasAnchorableLinkPoints and Selecteds[c].CanMove then {Only link lines by now}
        for d := 0 to Selecteds[c].LinkPoints.Count - 1 do if Selecteds[c].LinkPoints[d].CanAnchor and Selecteds[c].LinkPoints[d].CanAnchorInMove then
        begin
          {Calculate deltas for correct anchor position. This is particulary
           tricky when snapgrid is active}
          ADelta := MoveDelta(Selecteds[c], X, Y);
          ADelta2 := SubPointX(CanvasToClient(Dot(X, Y)), AddPointX(CanvasToClient(FDragStart), ADelta));
          ALinkPoint := Selecteds[c].LinkPoints[d];

          {if linkpoint is not anchored to anything, or is anchored to
           a control that is not being moved, then check if it is over
           an anchor}
          if (ALinkPoint.Anchor = nil) or not (ALinkPoint.Anchor.Selected) then
            AAnchorPoint := LinkPointAtPos(
              AddPointX(AddPointX(ALinkPoint.DiagramPoint, ADelta), ADelta2),
              Selecteds[c], true, false, true);

          {If there is an AnchorPoint, then check if anchoring is acceptable}
          if AAnchorPoint <> nil then
          begin
            if Selecteds[c] is TCustomDiagramLine then
            begin
              ALink := TCustomDiagramLine(Selecteds[c]);
              TestAnchor1 := nil;
              TestAnchor2 := nil;
              if ALinkPoint = ALink.SourceLinkPoint then
              begin
                TestAnchor1 := AAnchorPoint;
                TestAnchor2 := ALink.TargetLinkPoint.AnchorLink;
                if (TestAnchor2 <> nil) and not TestAnchor2.DControl.Selected then
                  TestAnchor2 := nil;
              end else
              if ALinkPoint = ALink.TargetLinkPoint then
              begin
                TestAnchor2 := AAnchorPoint;
                TestAnchor1 := ALink.SourceLinkPoint.AnchorLink;
                if (TestAnchor1 <> nil) and not TestAnchor1.DControl.Selected then
                  TestAnchor1 := nil;
              end;
              if not AcceptLink(ALink, TestAnchor1, TestAnchor2) then
                AAnchorPoint := nil;
            end;
            if (AAnchorPoint <> nil) and not AcceptAnchor(ALinkPoint, AAnchorPoint) then
              AAnchorPoint := nil;
          end;
          if (AAnchorPoint <> nil) then
            break
          else
            ALinkPoint := nil;
        end;
      if AAnchorPoint <> nil then
        break;
    end;
  end;
  for c := 0 to SelectedCount - 1 do
  begin
    FControlMoveInfos[c].Canvas := FControlCanvas;
    FControlMoveInfos[c].Action := AAction;
    if (ALinkPoint <> nil) then
    begin
      FControlMoveInfos[c].AnchorPoint := AAnchorPoint;
      FControlMoveInfos[c].LinkPoint := ALinkPoint;
    end else
    begin
      FControlMoveInfos[c].AnchorPoint := nil;
      FControlMoveInfos[c].LinkPoint := nil;
    end;
    Case AAction of
      caDrawCursor, caPositionCursor, caMoved:
        if FControlMoveInfos[c].AnchorPoint = nil then
          FControlMoveInfos[c].Delta := MoveDelta(Selecteds[c], X, Y)
        else
          FControlMoveInfos[c].Delta :=
            SubPointX(FControlMoveInfos[c].AnchorPoint.DiagramPoint,
              FControlMoveInfos[c].LinkPoint.DiagramPoint);
    end;
  end;
  if GroupSelection then
  begin
    FControlMoveInfos[SelectedCount].Canvas := FControlCanvas;
    FControlMoveInfos[SelectedCount].Action := AAction;
    FControlMoveInfos[SelectedCount].AnchorPoint := nil;
    FControlMoveInfos[SelectedCount].LinkPoint := nil;
    FControlMoveInfos[SelectedCount].Delta := MoveDelta(FSelBlock, X, Y);
  end;
end;

procedure _SetPaintModeCursorOnly(ADControl: TDiagramControl);
begin
  ADControl.PaintMode := pmCursorOnly;
end;

procedure _SetDControlVisibleFalse(ADControl: TDiagramControl);
begin
  ADControl.FVisible := false;
end;

procedure _SetDControlVisibleTrue(ADControl: TDiagramControl);
begin
  ADControl.FVisible := true;
end;

procedure TatDiagram.StartResizing;
var
  c: integer;
begin
  if (FHandle <> nil) and (FHandle.DControl <> nil) then
  begin
    FHandle.DControl.StartMovingHandle;

    if FDragStyle = dsShape then
    begin
      {Create cached bitmap. Remove the control being resized from the background}
      FHandle.DControl.FVisible := false;
      if FHandle.DControl.IsGroup then
        TGroupBlock(FHandle.DControl).IterateAllMembers(_SetDControlVisibleFalse);
      if GroupSelection then
        for c := 0 to SelectedCount - 1 do
          Selecteds[c].FVisible := false;

      BeginCacheBitmap;

      FHandle.DControl.FVisible := true;
      if FHandle.DControl.IsGroup then
        TGroupBlock(FHandle.DControl).IterateAllMembers(_SetDControlVisibleTrue);
      if GroupSelection then
        for c := 0 to SelectedCount - 1 do
          Selecteds[c].FVisible := true;

      {Set the block to be drawn as cursor}
      FHandle.DControl.PaintMode := pmCursorOnly;
      if FHandle.DControl.IsGroup then
        TGroupBlock(FHandle.DControl).IterateAllMembers(_SetPaintModeCursorOnly);
      if GroupSelection then
        for c := 0 to SelectedCount - 1 do
          Selecteds[c].PaintMode := pmCursorOnly;

      FSelBlock.PaintMode := pmCursorOnly;
    end;
  end;
  if Assigned(FOnBeforeResize) then
    FOnBeforeResize(Self);
end;

function TatDiagram.UpdateHandleMoveInfo(X, Y: number; AAction: THandleMoveAction): THandleMoveInfo;
var
  ALinkPoint: TLinkPoint;
  ALink: TCustomDiagramLine;
  AAnchor1, AAnchor2: TLinkPoint;
begin
  FHandleMoveInfo.AHandle := FHandle;
  FHandleMoveInfo.Canvas := FControlCanvas;
  FHandleMoveInfo.Action := AAction;
  FHandleMoveInfo.AnchorPoint := LinkPointAtPos(CanvasToClient(Dot(X, Y)),
    FHandle.DControl, false, false, true);
  FHandleMoveInfo.DControl    := DControlAtPos(CanvasToClient(Dot(X, Y)), csmIgnoreLines, cfIgnoreGroups);

  {If an anchor point is found, then check if it is valid.}
  if FHandle.DControl <> nil then
  begin
    if FHandle.DControl.HasAnchorableLinkPoints and (FHandleMoveInfo.AnchorPoint <> nil) then
    begin
      ALinkPoint := FHandle.DControl.LinkPointByHandle(FHandle);
      if FHandle.DControl is TCustomDiagramLine then
      begin
        ALink := TCustomDiagramLine(FHandle.DControl);
        AAnchor1 := nil;
        AAnchor2 := nil;
        if FHandle.Index = 0 then
        begin
          AAnchor1 := FHandleMoveInfo.AnchorPoint;
          AAnchor2 := ALink.TargetLinkPoint.AnchorLink;
        end;
        if FHandle.Index = ALink.Handles.Count - 1 then
        begin
          AAnchor1 := ALink.SourceLinkPoint.AnchorLink;
          AAnchor2 := FHandleMoveInfo.AnchorPoint;
        end;
        if (ALinkPoint = nil) or not AcceptLink(ALink, AAnchor1, AAnchor2) then
          FHandleMoveInfo.AnchorPoint := nil;
      end;

      if not ((ALinkPoint <> nil) and ALinkPoint.CanAnchor and AcceptAnchor(ALinkPoint, FHandleMoveInfo.AnchorPoint)) then
        FHandleMoveInfo.AnchorPoint := nil;
    end
    else
      FHandleMoveInfo.AnchorPoint := nil;

    {if the line requires connections, then never set AnchorPoint to nil -> retrieves back the old anchor point}
    if FHandleMoveInfo.AnchorPoint = nil then
    begin
      if (FHandle.DControl is TCustomDiagramLine) and (TCustomDiagramLine(FHandle.DControl).RequiresConnections) and
         (TCustomDiagramLine(FHandle.DControl).SourceLinkPoint.Anchor <> nil) and
         not TCustomDiagramLine(FHandle.DControl).SourceLinkPoint.Anchor.AutoCreateLinkPoints then
      begin
        ALink := TCustomDiagramLine(FHandle.DControl);
        if FHandle.Index = 0 then
          FHandleMoveInfo.AnchorPoint := ALink.SourceLinkPoint.AnchorLink;
        if FHandle.Index = ALink.Handles.Count - 1 then
          FHandleMoveInfo.AnchorPoint := ALink.TargetLinkPoint.AnchorLink;
      end;
    end;
  end;


  Case AAction of
    haDrawCursor, haPositionCursor, haMoved:
      if FHandleMoveInfo.AnchorPoint = nil then
        FHandleMoveInfo.Delta := SnapDeltaPoint(MulPoint(
          SubPointX(Dot(X, Y), FDragStart), 1 / ZoomRatio))
      else
        FHandleMoveInfo.Delta :=
          MulPoint(SubPointX(
            ClientToCanvas(FHandleMoveInfo.AnchorPoint.DiagramPoint), FDragStart),
            1 / ZoomRatio);
  end;
  result := FHandleMoveInfo;
end;

function TCustomPolygonBlock.GetHandlePoint(AHandle: TStretchHandle): TDot;
var
  APolygonPoint: TPolygonPoint;
begin
  Case AHandle.Style of
    hsRotCenter:
      begin
        result := DiagramRotCenter;
      end;
  else
    {Check handles of polygon points}
    if AHandle.Data = -1 then
    begin
      APolygonPoint := FPolygon.FindByHandle(AHandle);
      if APolygonPoint <> nil then
      begin
        AHandle.OrX := APolygonPoint.X;
        AHandle.OrY := APolygonPoint.Y;
      end;
      FTempDrawer.OriginalRect := Drawer.OriginalRect;
      FTempDrawer.CurRect := BoundsRect;
      FTempDrawer.Angle := Angle;
      FTempDrawer.RotationType := rtCurRectCenter;
      //result := FTempDrawer.Rot(FTempDrawer.PP(AHandle.OrX, AHandle.OrY));
      result := FTempDrawer.RotX(Dot(AHandle.OrX, AHandle.OrY));
    end else
      result := inherited GetHandlePoint(AHandle);
  end;
end;

procedure TatDiagram.UpdateResizing(XPos, YPos: number);
begin
  if Assigned(FHandle) and Assigned(FHandle.DControl) then
  begin
    if FDragStyle = dsShape then
    begin
      FHandle.DControl.HandleMoving(UpdateHandleMoveInfo(XPos, YPos, haPositionCursor));
    end
    else
    begin
      if not IsDummyPoint(FOldDragPos) then
        FHandle.DControl.HandleMoving(UpdateHandleMoveInfo(XPos, YPos, haEraseCursor));
      FHandle.DControl.HandleMoving(UpdateHandleMoveInfo(XPos, YPos, haDrawCursor));
    end;
  end;
  if Assigned(FOnDControlResizing) then
    FOnDControlResizing(Self, Round(XPos), Round(YPos));
  FOldDragPos := Dot(XPos, YPos);

  if FDragStyle = dsShape then
    Redraw;
end;

procedure TatDiagram.EndResizing(XPos, YPos: number; Cancel: boolean);
var
  c: integer;
  ResizingPerformed: boolean;
  AControl: TDiagramControl;
begin
  ResizingPerformed := false;
  if (FHandle <> nil) and (FHandle.DControl <> nil) then
  begin
    {Get reference to AControl already, because the handle will be destroyed when HandleMoving is called}
    AControl := FHandle.DControl;

    if not IsDummyPoint(FOldDragPos) then
    begin
      AControl.HandleMoving(UpdateHandleMoveInfo(XPos, YPos, haEraseCursor));
      if not Cancel then
      begin
        AControl.HandleMoving(UpdateHandleMoveInfo(XPos, YPos, haMoved));
        if AControl.IsMember then
          AControl.GroupBlock.RecalculateRect;
        ResizingPerformed := true;
      end;
    end;
    if FDragStyle = dsShape then
    begin
      FSelBlock.PaintMode := pmNormal;
      {AControl.FPaintMode := pmNormal;
      if GroupSelection then
        for c := 0 to SelectedCount - 1 do
          Selecteds[c].FPaintMode := pmNormal;}

      {Set everything to normal. If we use the commented code above, we will have to
       set to pmNormal all members of AControl, if AControl is a group}
      for c := 0 to DControlCount - 1 do
        DControls[c].PaintMode := pmNormal;
    end;
  end;
  EndCacheBitmap;
  FOldDragPos := DummyPointX;
  FHandle := nil;
  FMouseState := msBrowsing;
  KeepInBounds;
  CalcPanelSize;
  Redraw;
  if ResizingPerformed then
  begin
    if Assigned(FOnAfterResize) then
      FOnAfterResize(Self);
    Modified;
    PushUndoStack('resize');
  end;
  Screen.Cursor := FOldScreenCursor;
end;

function TatDiagram.UpdateControlInsertInfo(X, Y: number; AAction: TControlInsertAction): TControlInsertInfo;
var ILControl1, ILControl2: TCustomDiagramBlock;
    ControlSearchMode: TControlSearchMode;
begin
  FControlInsertInfo.Diagram := Self;
  FControlInsertInfo.Canvas := FControlCanvas;
  FControlInsertInfo.P1 := CanvasToClient(FDragStart);
  FControlInsertInfo.AnchorPoint1 := FLinkPoint;
  FControlInsertInfo.Action := AAction;
  FControlInsertInfo.AnchorPoint2 := LinkPointAtPos(CanvasToClient(Dot(X, Y)), nil, false, false, true);
  if (FInsertDControl is TCustomDiagramLine) and
     (TCustomDiagramLine(FInsertDControl).LinkPointStyle = ptImplicit)
  then
    ControlSearchMode := csmIgnoreLines
  else
    ControlSearchMode := csmAll;
  FControlInsertInfo.DControl := DControlAtPos(CanvasToClient(Dot(X, Y)), ControlSearchMode, cfIgnoreGroups);
  if (FControlInsertInfo.AnchorPoint2 = FLinkPoint) or
    (
    (FInsertDControl is TCustomDiagramLine) and
      (
        not AcceptAnchor(
          TCustomDiagramLine(FInsertDControl).TargetLinkPoint,
          FControlInsertInfo.AnchorPoint2)
        or
        not AcceptLink(
          TCustomDiagramline(FInsertDControl),
          FControlInsertInfo.AnchorPoint1,
          FControlInsertInfo.AnchorPoint2)
      )
    ) then
    FControlInsertInfo.AnchorPoint2 := nil;
  Case AAction of
    iaDrawCursor, iaInserted:
      if (FControlInsertInfo.AnchorPoint2 = nil) then
      begin
        FControlInsertInfo.P2 := CanvasToClient(SnapCanvasPoint(Dot(X, Y)));
        // add an implicit linkpoint for controls that support it
        if (AAction = iaInserted) and Assigned(FControlInsertInfo.DControl) and
           FControlInsertInfo.DControl.AutoCreateLinkPoints and
           (FInsertDControl is TCustomDiagramLine) and // only when inserting Line controls though!!
           TCustomDiagramLine(FInsertDControl).RequiresConnections
        then
        begin
          ILControl1 := FControlInsertInfo.AnchorPoint1.DControl as TCustomDiagramBlock;
          ILControl2 := FControlInsertInfo.DControl as TCustomDiagramBlock;
          FControlInsertInfo.AnchorPoint2 := ILControl2.LinkPoints.Add;
          FControlInsertInfo.AnchorPoint2.Anchor := ILControl2;
          FControlInsertInfo.AnchorPoint1.Anchor := ILControl1;
          FControlInsertInfo.AnchorPoint2.OrPoint := ILControl2.Drawer.UnPP(ClientToCanvas(FControlInsertInfo.P2));
          FControlInsertInfo.AnchorPoint1.OrPoint := ILControl1.Drawer.UnPP(ClientToCanvas(FControlInsertInfo.P1));
        end;
      end
      else
        FControlInsertInfo.P2 := FControlInsertInfo.AnchorPoint2.DiagramPoint;
  end;
  FControlInsertInfo.Clicked :=
    AroundPoint(FControlInsertInfo.P1, FControlInsertInfo.P2, 10);

  {Do not allow zero width or zero length}
  with FControlInsertInfo do
  begin
    if P2.X = P1.X then
      P2.X := P1.X + 1;
    if P2.Y = P1.Y then
      P2.Y := P1.Y + 1;
  end;
  result := FControlInsertInfo;
end;

function TatDiagram.UpdateInserting(XPos, YPos: number): boolean;
begin
  if FInsertDControl <> nil then
  begin
    if not IsDummyPoint(FOldDragPos) then
      FInsertDControl.UpdateInserting(UpdateControlInsertInfo(XPos, YPos, iaEraseCursor));
    FInsertDControl.UpdateInserting(UpdateControlInsertInfo(XPos, YPos, iaDrawCursor));
  end;
  result := (FInsertDControl is TCustomDiagramLine) and
    ( (FControlInsertInfo.AnchorPoint2 <> nil)
    or
     ( (FControlInsertInfo.DControl <> nil) and
        FControlInsertInfo.DControl.AutoCreateLinkPoints)
    );
  FOldDragPos := Dot(XPos, YPos);
end;

procedure TatDiagram.EndInserting(XPos, YPos: integer; Cancel: boolean);
var
  InsertedControlID: string;
begin
  InsertedControlID := '';
  if FInsertDControl <> nil then
  begin
    InsertedControlID := FInsertDControl.DControlID;
    if not IsDummyPoint(FOldDragPos) then
      FInsertDControl.UpdateInserting(UpdateControlInsertInfo(XPos, YPos, iaEraseCursor));
    if Cancel then
      DestroyInsertControl
    else
    begin
      FInsertDControl.UpdateInserting(UpdateControlInsertInfo(XPos, YPos, iaInserted));
      FInsertDControl.Diagram := Self;
      FInsertDControl.Selected := True;
      PushUndoStack('insert');
    end;
  end;
  FOldDragPos := DummyPointX;
  FInsertDControl := nil;
  FMouseState := msBrowsing;
  KeepInBounds;
  CalcPanelSize;
  Redraw;
  Screen.Cursor := FOldScreenCursor;
  if FKeepInserting and (InsertedControlID <> '') and not Cancel then
    StartInsertingControl(InsertedControlID, true);
end;

procedure TatDiagram.SelectCursor(XPos, YPos: integer; ShowBox: boolean);
begin
  if not IsNullRect(FCurRect) then
  begin
    _DrawFocusRect(FControlCanvas, FCurRect);
    FCurRect := Rect(0, 0, 0, 0);
  end;
  if ShowBox then
  begin
    FCurRect := ToRect(RectEx(FDragStart.X, FDragStart.Y, XPos, YPos));
    _DrawFocusRect(FControlCanvas, FCurRect);
  end else
    Repaint;
end;

procedure TatDiagram.DrawZoomCursor(XPos, YPos: integer; ShowBox: boolean);
begin
  if not IsDummyPoint(FOldDragPos) then
  begin
    _DrawFocusRect(FControlCanvas, ToRect(RectEx(FDragStart, FOldDragPos)));
    FOldDragPos := Dot(0, 0);
  end;
  if ShowBox then
  begin
    FOldDragPos := Dot(XPos, YPos);
    _DrawFocusRect(FControlCanvas, ToRect(RectEx(FDragStart, FOldDragPos)));
  end else
    Repaint;
end;

procedure TatDiagram.DeleteControls(restricted: TControlRestrictions);
var
  c: integer;
  ObjList: TObjectList;
begin
  if DesignMode <> ddmEditable then
    Exit;
    
  ObjList := TObjectList.Create(false);
  try
    for c := 0 to SelectedCount - 1 do
      if (Selecteds[c].Restrictions * restricted = []) and Selecteds[c].LayerEditable then
        ObjList.Add(Selecteds[c]);
    for c := 0 to ObjList.Count - 1 do
      ObjList[c].Free;
  finally
    ObjList.Free;
  end;
  PushUndoStack('delete');
end;

procedure TatDiagram.DeleteSelecteds;
begin
  DeleteControls([crNoDelete]);
end;

function TatDiagram.CanCreateConnection(P: TDot): boolean;
var
  ALinkPoint: TLinkPoint;
  ANewLine: TCustomDiagramLine;
begin
  result := false;
  ALinkPoint := LinkPointAtPos(P, nil, false, false, true);
  {If there is a connectionlineid defined, then start creating a connection
   automatically. If the link is possible, of course}
  if (ALinkPoint <> nil) and (ConnectionLineClass <> nil) then
  begin
    ANewLine := TCustomDiagramLine(CreateControl(ConnectionLineClass, ConnectionLineId));
    try
      if AcceptAnchor(ANewLine.SourceLinkPoint, ALinkPoint) and
        AcceptLink(ANewLine, ALinkPoint, nil) then
        result := true;
    finally
      ANewLine.Free;
    end;
  end;
end;

procedure TatDiagram.UpdateSelectionBlockAngle;
var
  c: integer;
  ADControl: TDiagramControl;
begin
  if SelectedCount = 0 then
  begin
    {Restart initial settings of selection block}
    FSelBlock.FAngle := 0;
    FSelBlock.FRotCenter := Dot(50, 50);
  end else
  begin
    ADControl := nil;
    for c := 0 to SelectedCount - 1 do
      if not Selecteds[c].AsMember then
      begin
        {if ADControl <> nil then it's there is more than one object selected}
        if ADControl <> nil then
          exit
        else
          ADControl := Selecteds[c];
      end;
    if ADControl is TCustomDiagramBlock then
      FSelBlock.FAngle := TCustomDiagramBlock(ADControl).Angle;
  end;
end;

procedure TatDiagram.MouseDown(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);

type
  TClickAction = (caNone, caDControlClick, caHandleClick,
    caBackgroundClick, caBackgroundRightClick, caInsertClick, caSelectClick, caLinkNodeClick,
    caAutoConnectionClick);
var
  ILControl: TCustomDiagramBlock;
  ADControl: TDiagramControl;
  AAction: TClickAction;
  AHandle: TStretchHandle;
  ALinkNode: TLinkPoint;
  P: TDot;

begin
  if not (csDesigning in ComponentState) then
  begin
    if EditingText then CloseEditor(true);
    SetFocus;
  end;
  FOldScreenCursor := Screen.Cursor;

  {Only perform mouse actions if not clicked in ruler}
  if RulerAtPos(Dot(X, Y)) = nil then
  begin
    P := CanvasToClient(Dot(X, Y));
    Case MouseState of
      msBrowsing:
        begin
          { Check if user clicked on block or background }
          if PaintLinesFirst then
          begin
            ADControl := DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups);
            if not Assigned(ADControl) then
              ADControl := DControlAtPos(P, csmAll, cfIgnoreGroups);
          end
          else
            ADControl := DControlAtPos(P, csmAll, cfIgnoreGroups);

          AHandle := HandleAtPos(P, true);
          ALinkNode := LinkNodeAtPos(P);

          AAction := caNone;
          if Assigned(AHandle) and (Button = mbLeft) and (AHandle.DControl <> nil)
            and AHandle.Movable
            and AHandle.DControl.CanSelect then
          begin
            AAction := caHandleClick;
            DoDControlMouseDown(AHandle.DControl, Button, Shift, X, Y);
            if ssDouble in Shift then
              DoDControlDblClick(AHandle.DControl)
            else
              FDControlClicked := AHandle.DControl;
            ADControl := AHandle.DControl;
            DoDControlMouseDownEx(AHandle.DControl, Button, Shift, X, Y, AHandle);
          end
          else
          if Assigned(ALinkNode) then
          begin
            AAction := caLinkNodeClick;
          end
          else
          if (ssLeft in Shift) and CanCreateConnection(P) then
          begin
            AAction := caAutoConnectionClick;
          end
          else
          if Assigned(ADControl) then
          begin
            DoDControlMouseDown(ADControl, Button, Shift, X, Y);
            if ssDouble in Shift then
              DoDControlDblClick(ADControl)
            else
            begin
              if ADControl.CanSelect then
              begin
                FDControlClicked := ADControl;
                if Button = mbLeft then
                  AAction := caDControlClick
                else
                if Button = mbRight then
                  AAction := caSelectClick;
              end;
            end;
            DoDControlMouseDownEx(ADControl, Button, Shift, X, Y, AHandle);
          end
          else
          if not Assigned(ADControl) then
          begin
            if Button = mbLeft then
              AAction := caBackgroundClick
            else
            if Button = mbRight then
              AAction := caBackgroundRightClick;
          end;
          Case AAction of
            caHandleClick:
              begin
                FMouseState := msResizing;
                FHandle := AHandle;
                FDragStart := ClientToCanvas(AHandle.DiagramPoint);
                FOldDragPos := DummyPointX;
                StartResizing;
              end;
            caLinkNodeClick:
              begin
                FMouseState := msClickingNode;
                LinkNodeMouseDown(ALinkNode);
              end;
            caAutoConnectionClick:
              begin
                {THere are no strong tests here for LinkPOintAtPos, ConnectionLineCLass, and others,
                 because the CanCreateConnection method was already called above and everything should
                 be ok when reaching this point}
                DestroyInsertControl;
                FInsertDControl := CreateControl(ConnectionLineClass, ConnectionLineId);
                FLinkPoint := LinkPointAtPos(P, nil, false, false, true);

                UnselectAll;
                Perform(WM_PAINT, 0, 0);
                FDragStart := ClientToCanvas(FLinkPoint.DiagramPoint);
                FMouseState := msInserting;
                FOldDragPos := DummyPointX;
              end;
            caDControlClick:
              begin
                if ssShift in Shift then
                begin
                  BeginUpdateSel;
                  try
                    PerformSelection(ADControl, dcmShiftClick);
                  finally
                    EndUpdateSel;

                    {Force repaint now}
                    Perform(WM_PAINT, 0, 0);
                  end;
                end else
                begin
                  FDragStart := Dot(X, Y);
                  BeginUpdateSel;
                  try
                    {if not ADControl.Selected then
                      UnselectAll;}
                    if ssCtrl in Shift then
                      PerformSelection(ADControl, dcmSubClick)
                    else
                      PerformSelection(ADControl, dcmClick);
                  finally
                    EndUpdateSel;
                    {Force repaint now}
                    Perform(WM_PAINT, 0, 0);
                  end;

                  // only start "moving" if the control is NOT a line or IS a line and does NOT require targets
                  if not ((ADControl is TCustomDiagramLine) and TCustomDiagramLine(ADControl).RequiresConnections)
                    and not (ssCtrl in Shift) then
                  begin
                    FMouseState := msMoving;
                    FOldDragPos := DummyPointX;
                    StartMoving;
                  end;
                  // else, do not change anything
                end;
              end;
            caSelectClick:
              begin
                BeginUpdateSel;
                try
                  if not ADControl.Selected then
                  begin
                    if ssCtrl in Shift then
                      PerformSelection(ADControl, dcmSubClick)
                    else
                      PerformSelection(ADControl, dcmClick);
                  end;
                finally
                  EndUpdateSel;
                  Perform(WM_PAINT, 0, 0);
                end;
              end;
            caBackgroundClick:
              begin
                UnselectAll;
                Perform(WM_PAINT, 0, 0);
                FDragStart := Dot(X, Y);
                FMouseState := msSelecting;
              end;
            caBackgroundRightClick:
              begin
                UnselectAll;
                Perform(WM_PAINT, 0, 0);
              end;
          end;
          Case FMouseState of
            msMoving: ;
  //            FOldDragPos := DummyPoint;
            msSelecting:
              FCurRect := Rect(0, 0, 0, 0);
            msLink:
              FCurRect := Rect(0, 0, 0, 0);
          end;
        end;
      msToInsert:
        if ssLeft in Shift then
        begin
          UnselectAll;
          Perform(WM_PAINT, 0, 0);
          if FInsertDControl is TCustomDiagramLine then
          begin
            FLinkPoint := LinkPointAtPos(P, nil, false, false, true);

            // check to create an implicit linkpoint for the first anchor point
            if not Assigned(FLinkPoint) and Assigned(DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups))
              and DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups).AutoCreateLinkPoints then
            begin
              ILControl := DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups) as TCustomDiagramBlock;
              FLinkPoint := ILControl.LinkPoints.Add;

              FLinkPoint.OrX := ILControl.Drawer.UnPx(ClientToCanvas(P).X);
              FLinkPoint.OrY := ILControl.Drawer.UnPY(ClientToCanvas(P).Y);
            end;

            if (not AcceptAnchor(TCustomDiagramLine(FInsertDControl).SourceLinkPoint, FLinkPoint))
              or
               (not AcceptLink(TCustomDiagramLine(FInsertDControl), FLinkPoint, nil))
              then
              FLinkPoint := nil;
          end
          else
            FLinkPoint := nil;
          if FLinkPoint = nil then
            FDragStart := SnapCanvasPoint(Dot(X, Y))
          else
            FDragStart := ClientToCanvas(FLinkPoint.DiagramPoint);
          FMouseState := msInserting;
          FOldDragPos := DummyPointX;

          // if the lines requires to have a target, continue "insert" state
          //    to let the user find the target arrow control
          if not Assigned(FLinkPoint) and
            (FInsertDControl is TCustomDiagramLine) and TCustomDiagramLine(FInsertDControl).RequiresConnections then
          begin
            FMouseState := msToInsert;
          end
        end;
      msToDraw:
        if ssLeft in Shift then
        begin
          UnselectAll;
          Perform(WM_PAINT, 0, 0);
          FDragStart := SnapCanvasPoint(Dot(X, Y));
          FMouseState := msDrawing;
          FOldDragPos := DummyPointX;

          if FInsertDControl is TCustomDiagramLine then
          begin
            FLinkPoint := LinkPointAtPos(P, nil, false, false, true);

            // check to create an implicit linkpoint for the first anchor point
            if not Assigned(FLinkPoint) and Assigned(DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups))
              and DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups).AutoCreateLinkPoints then
            begin
              ILControl := DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups) as TCustomDiagramBlock;
              FLinkPoint := ILControl.LinkPoints.Add;
              FLinkPoint.OrX := ILControl.Drawer.UnPx(ClientToCanvas(P).X);
              FLinkPoint.OrY := ILControl.Drawer.UnPY(ClientToCanvas(P).Y);
            end;

            if (not AcceptAnchor(TCustomDiagramLine(FInsertDControl).SourceLinkPoint, FLinkPoint))
              or
               (not AcceptLink(TCustomDiagramLine(FInsertDControl), FLinkPoint, nil))
              then
              FLinkPoint := nil;
          end
          else
            FLinkPoint := nil;

          if not Assigned(FLinkPoint) and
            (FInsertDControl is TCustomDiagramLine) and TCustomDiagramLine(FInsertDControl).RequiresConnections then
          begin
            FMouseState := msToDraw;
          end
          else
            StartDrawing(FDragStart.X, FDragStart.Y);
        end;
      msToPan:
        if ssLeft in Shift then
        begin
          FDragStart := Dot(X, Y);
          FOldPosition := Point(FHorzScrollBar.Position, FVertScrollBar.Position);
          FMouseState := msPanning;
        end;
      msToZoom:
        if ssLeft in Shift then
        begin
          FDragStart := Dot(X, Y);
          FOldDragPos := DummyPointX;
          FMouseState := msZooming;
        end;
      msDrawing:
        if ssLeft in Shift then
        begin
          if ssDouble in Shift then
            // if the line requires end-points, check for the "Cancel"
            EndDrawing(X, Y, ((FInsertDControl is TCustomDiagramLine) and TCustomDiagramLine(FInsertDControl).RequiresConnections and
                                                   ( (FControlInsertInfo.AnchorPoint2 = nil) and
                                                     ( (FControlInsertInfo.DControl = nil) or
                                                     ((FControlInsertInfo.DControl <> nil) and not
                                                       FControlInsertInfo.DControl.AutoCreateLinkPoints
                                                      ))
                                                   )
                            ));
        end;
    end;
  end;
  inherited MouseDown(Button,Shift,X,Y);
end;

procedure TatDiagram.MouseMove(Shift: TShiftState; X, Y: Integer);
Var
  ADControl: TDiagramControl;
  AHandle: TStretchHandle;
  ACursor: TCursor;
  P: TDot;
  MoveInfo: THandleMoveInfo;
begin
  ACursor := crDefault;
  P := CanvasToClient(Dot(X, Y));
  ADControl := DControlAtPos(P, csmAll, cfIgnoreGroups);
  if Assigned(ADControl) then
  begin
    if ADControl <> FPreviousDControl then
      DoDControlMouseEnter(ADControl);
    DoDControlMouseMove(ADControl, Shift, X, Y);
    ACursor := ADControl.Cursor;
  end;
  if (FPreviousDControl <> nil) and (FPreviousDControl <> ADControl) then
    DoDControlMouseLeave(FPreviousDControl);
  FPreviousDControl := ADControl;
  Case FMouseState of
    msToDraw, // if the line is of insert type "ToDraw", also do this block
    msToInsert:
      if (FInsertDControl is TCustomDiagramLine) and
         ( ( Assigned(LinkPointAtPos(P, nil, false, false, true)) and
             AcceptAnchor(TCustomDiagramLine(FInsertDControl).SourceLinkPoint, LinkPointAtPos(P, nil, false, false, true)) and
             AcceptLink(TCustomDiagramLine(FInsertDControl), LinkPointAtPos(P, nil, false, false, true), nil)
           )
           or //check to see if the current control accepts implicit links
           (
            Assigned(DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups)) and DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups).AutoCreateLinkPoints and
            TCustomDiagramLine(FInsertDControl).RequiresConnections
           )
         )
      then ACursor := FLinkCursor;
    msSelecting:
      SelectCursor(X, Y, true);
    msToPan:
      ACursor := FPanCursor;
    msPanning:
      begin
        FHorzScrollBar.Position := Round(FOldPosition.X - (X - FDragStart.X));
        FVertScrollBar.Position := Round(FOldPosition.Y - (Y - FDragStart.Y));
        ACursor := FPanCursor;
        Redraw;
      end;
    msToZoom:
      ACursor := FZoomCursor;
    msZooming:
      DrawZoomCursor(X, Y, true);
    msBrowsing :
      begin
        if CanCreateConnection(P) then
          ACursor := FLinkCursor;

        {Handle cursor has priority over linkpoint cursor}
        AHandle := HandleAtPos(P, true);
        if (AHandle <> nil) and (AHandle.DControl <> nil)
          and AHandle.Movable then
            ACursor := AHandle.Cursor;
      end;
    msMoving :
      begin
        if UpdateMoving(X, Y) then
          ACursor := FLinkCursor;
      end;
    msResizing:
      begin
        UpdateResizing(X, Y);
        MoveInfo := UpdateHandleMoveInfo(X, Y, haEraseCursor);

        // if there's an anchorpoint OR a control that accepts implicit linkpoints, do the link cursor
        //    and we're resizing a line thing
        if (MoveInfo.AHandle.DControl <> nil) and
           ((MoveInfo.AnchorPoint <> nil) or
           ( (MoveInfo.DControl <> nil) and
             MoveInfo.DControl.AutoCreateLinkPoints
           ))
        then
          ACursor := FLinkCursor
        else
          ACursor := FHandle.Cursor;
      end;
    msInserting :
      if UpdateInserting(X, Y) then
        ACursor := FLinkCursor;
    msDrawing:
      begin
         UpdateDrawing(X, Y);
         // if we're drawing a line that needs a link, try to search for a control and change cursor
         if (FInsertDControl is TCustomDiagramLine) and
            ( ( Assigned(LinkPointAtPos(P, nil, false, false, true)) and
                AcceptAnchor(TCustomDiagramLine(FInsertDControl).SourceLinkPoint, LinkPointAtPos(P, nil, false, false, true)) and
                AcceptLink(TCustomDiagramLine(FInsertDControl), LinkPointAtPos(P, nil, false, false, true), nil)
              )
              or // check to see if the current control accepts implicit links
              (
               Assigned(DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups)) and DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups).AutoCreateLinkPoints and
               TCustomDiagramLine(FInsertDControl).RequiresConnections
              )
            )
         then ACursor := FLinkCursor;
      end;
    msClickingNode: ;
  end;
  if Screen.Cursor <> ACursor then
    Screen.Cursor := ACursor;
  inherited MouseMove(Shift, X, Y);
end;

procedure TatDiagram.CMMouseLeave(var Message: TMessage);
begin
  Case FMouseState of
    msLink: ;
      //LinkCursor(0,0,false);
  end;
  Screen.Cursor := crDefault;
end;

procedure TatDiagram.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
var
  P: TDot;

  procedure CheckMouseUp(ADControl: TDiagramControl);
  begin
    if Assigned(ADControl) then
    begin
      if ADControl = FDControlClicked then
        DoDControlClick(ADControl);
      DoDControlMouseUp(ADControl, Button, Shift, X, Y);
    end;
  end;

var
  ADControl: TDiagramControl;
begin
  P := CanvasToClient(Dot(X, Y));
  Case FMouseState of
    msBrowsing:
      begin
        CheckMouseUp(DControlAtPos(P, csmAll, cfIgnoreGroups));
      end;
    msSelecting:
      begin
        CancelSelecting;
        SelectControlsInRect(CanvasToClient(RectEx(FDragStart, Dot(X, Y))));
        CheckMouseUp(DControlAtPos(P, csmAll, cfIgnoreGroups));
      end;
    msMoving:
      begin
        EndMoving(X, Y, false);
        CheckMouseUp(DControlAtPos(P, csmAll, cfIgnoreGroups));
      end;
    msResizing:
      begin
        if Assigned(FHandle) then
          ADControl := FHandle.DControl
        else
          ADControl := nil;
        EndResizing(X, Y, false);
        CheckMouseUp(ADControl);
      end;
    msInserting:
      begin
        // perform a check on "Cancel" when the diagram requires lines to have a target
        EndInserting(X, Y, ((FInsertDControl is TCustomDiagramLine) and TCustomDiagramLine(FInsertDControl).RequiresConnections and
                                                   ( (FControlInsertInfo.AnchorPoint2 = nil) and
                                                     ( (FControlInsertInfo.DControl = nil) or
                                                     ((FControlInsertInfo.DControl <> nil) and not
                                                       FControlInsertInfo.DControl.AutoCreateLinkPoints
                                                      ))
                                                   )
                            ));
      end;
    msPanning:
      begin
        FMouseState := msToPan;
        Redraw;
      end;
    msZooming:
      Begin
        if FZoomingState = zsZoomIn then
           ZoomIn(CanvasToClient(RectEx(FDragStart, Dot(X, Y))))
        else
           ZoomOut(CanvasToClient(RectEx(FDragStart, Dot(X, Y))));
        DrawZoomCursor(X, Y, false);
        Redraw;
        FMouseState := msToZoom;
      end;
    msDrawing:
      begin
        if (FInsertDControl is TCustomDiagramLine) and
         ( ( Assigned(LinkPointAtPos(P, nil, false, false, true)) and
             AcceptAnchor(TCustomDiagramLine(FInsertDControl).SourceLinkPoint, LinkPointAtPos(P, nil, false, false, true)) and
             AcceptLink(TCustomDiagramLine(FInsertDControl), LinkPointAtPos(P, nil, false, false, true), nil)
           )
           or // check to see if the current control accepts implicit links
           (
            Assigned(DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups)) and DControlAtPos(P, csmIgnoreLines, cfIgnoreGroups).AutoCreateLinkPoints and
            TCustomDiagramLine(FInsertDControl).RequiresConnections
           )
         ) then
        begin
          DrawingStep(X, Y);
          EndDrawing(X, Y, False);
        end
        else
          DrawingStep(X, Y);
      end;
    msClickingNode:
      begin
        FMouseState := msBrowsing;
      end;
  end;
  FDControlClicked := nil;
  inherited MouseUp(Button,Shift,X,Y);
end;

procedure TatDiagram.CancelSelecting;
begin
  SelectCursor(0,0,false);
  FMouseState := msBrowsing;
end;

procedure TatDiagram.DoInsertDControl(ANewDControl: TDiagramControl);
begin
  if Assigned(FOnInsertDControl) and not (csLoading in ComponentState) then
    FOnInsertDControl(Self, ANewDControl);
end;

procedure TatDiagram.DoInsertLink(ANewLink: TCustomDiagramLine);
begin
  if Assigned(FOnInsertLink) and not (csLoading in ComponentState) then
    FOnInsertLink(Self, ANewLink);
end;

procedure TatDiagram.DoInsertBlock(ANewBlock: TCustomDiagramBlock);
begin
  if Assigned(FOnInsertBlock) and not (csLoading in ComponentState) then
    FOnInsertBlock(Self, ANewBlock);
end;

function TatDiagram.FindCompName(AComponent: TComponent; ABaseName: string = ''): string;
var
  c: integer;
  owner1, owner2, owner3: TComponent;
begin
  if ABaseName = '' then
  begin
    if AComponent.ClassName[1] = 'T' then
      ABaseName := Copy(AComponent.ClassName, 2, High(Integer))
    else
      ABaseName := AComponent.ClassName;
  end;

  { make sure that there is no same name for several possible owners:
    the component owner, diagram owner, and the diagram itself;
    and neither a diagram control with same name }
  owner1 := AComponent.Owner;
  owner2 := Self.Owner;
  owner3 := Self;

  c:=1;
  repeat
    result := ABaseName + IntToStr(c);
    inc(c);
  until not (Assigned(owner1) and Assigned(owner1.FindComponent(result)))
    and not (Assigned(owner2) and Assigned(owner2.FindComponent(result)))
    and not (Assigned(owner3) and Assigned(owner3.FindComponent(result)))
    and not Assigned(FindDControl(result));
end;

procedure TatDiagram.AddBlock(ABlock: TCustomDiagramBlock);
begin
  FBlocks.Add(ABlock);
  DoInsertBlock(ABlock);
  Modified;
end;

procedure TatDiagram.RemoveBlock(ABlock: TCustomDiagramBlock);
begin
  FBlocks.Remove(ABlock);
end;

function TatDiagram.CreateControl(ADControlClass: TDiagramControlClass; ADControlID: string): TDiagramControl;
var
  AComponentBaseName: string;
begin
  result := nil;
  if (ADControlClass <> nil) then
  begin
    if Assigned(OnBeforeCreateControl) then
      OnBeforeCreateControl(Self, ADControlID, AComponentBaseName);
    result := ADControlClass.Create(GetChildOwner);
    result.Name := FindCompName(result, AComponentBaseName);
    result.FDControlID := ADControlID;
  end;
end;

function TatDiagram.CreateLibraryControl(AData, ADControlID: string): TDiagramControl;
var
  D: TatDiagram;
  Obj: TDiagramControl;
begin
  Obj := nil;
  D := TatDiagram.Create(GetChildOwner);
  try
    D.SerializeFromString(AData);

    if D.DControlCount > 0  then
    begin
      Obj := D.DControls[0];
      if Obj.IsMember then
        Obj := Obj.MasterGroup;
    end;

    while D.DControlCount > 0 do
    begin
      D.DControls[0].Name := '';
      D.DControls[0].Diagram := nil;
    end;
  finally
    D.Free;
  end;
  Obj.FDControlID := ADControlID;
  result := Obj;
end;

procedure TatDiagram.CancelInsertingBlock;
begin
  DestroyInsertControl;
  FMouseState := msBrowsing;
end;

procedure TatDiagram.StartInsertingControl(const ADControlID: string; AKeepInserting: boolean = false);
var
  AFoundClass: TPersistentClass;
  RegDControl: TRegDControl;
  ADControlClass: TDiagramControlClass;
begin
  RegDControl := RegDControlList.FindByID(ADControlID);
  if RegDControl <> nil then
  begin
    if RegDControl.Data <> '' then
    begin
      StartInsertingControl(CreateLibraryControl(RegDControl.Data, ADControlID), AKeepInserting);
    end
    else
    begin
      AFoundClass := RegDControl.DControlClass;
      if AFoundClass.InheritsFrom(TDiagramControl) then
        ADControlClass := TDiagramControlClass(AFoundClass)
      else
        ADControlClass := TDiagramBlock;

      StartInsertingControl(CreateControl(ADControlClass, ADControlID), AKeepInserting);
    end;
  end
  else
    StartInsertingControl(CreateControl(TDiagramBlock, ADControlID), AKeepInserting);
end;

procedure TatDiagram.StartInsertingControl(ADControlClass: TDiagramControlClass; AKeepInserting: boolean = false);
begin
  StartInsertingControl(CreateControl(ADControlClass, ''), AKeepInserting);
end;

procedure TatDiagram.StartInsertingControl(ADControl: TDiagramControl; AKeepInserting: boolean = false);
begin
  FKeepInserting := AKeepInserting;
  DestroyInsertControl;
  FInsertDControl := ADControl;
  UnselectAll;
  FMouseState := FInsertDControl.InsertMouseState;
end;

function TatDiagram.BlockCount: integer;
begin
  result := FBlocks.Count;
end;

function TatDiagram.LinkCount: integer;
begin
  result := FLinks.Count;
end;

function TatDiagram.DControlCount: integer;
begin
  result := FDControls.Count;
end;

function TatDiagram.GetBlocks(Index: integer): TCustomDiagramBlock;
begin
  {$WARNINGS OFF}
  result := TCustomDiagramBlock(FBlocks[Index]);
  {$WARNINGS ON}
end;

function TatDiagram.LinkNodeAtPos(const Pos: TDot): TLinkPoint;
begin
  result := LinkPointAtPos(Pos, nil, false, true);
end;

function TatDiagram.LinkPointAtPos(const Pos: TDot;
  ADControl: TDiagramControl = nil; OnlyUnselected: boolean = false;
  OnlyNodes: boolean = false; OnlyAcceptAnchoreds: boolean = false): TLinkPoint;

  function TestLinkPoints(TestDControl: TDiagramControl): TLinkPoint;
  var
    d: integer;
  begin
    result := nil;
    if (TestDControl <> ADControl) and
      (not OnlyUnselected or not TestDControl.Selected) then
      for d := 0 to TestDControl.LinkPoints.Count - 1 do
        if TestDControl.LinkPoints[d].Visible and
          (not OnlyNodes or TestDControl.LinkPoints[d].IsNode) and
          AroundPoint(Pos, TestDControl.LinkPoints[d].DiagramPoint) and
          (TestDControl.LinkPoints[d].AcceptAnchoreds or not OnlyAcceptAnchoreds) and
          // ignore implicit linkpoints
          (TestDControl.LinkPoints[d].Style <> ptImplicit) then
        begin
          result := TestDControl.LinkPoints[d];
          break;
        end;
  end;

var
  c: integer;
begin
  result := nil;
  if RulerAtPos(ClientToCanvas(Pos)) = nil then
  begin
    {Test link points of blocks first. Then test link points of other controls}
    for c := BlockCount - 1 downto 0 do
      if Blocks[c].IsVisible then
      begin
        result := TestLinkPoints(Blocks[c]);
        if result <> nil then Exit;
      end;
    for c := DControlCount - 1 downto 0 do
      if DControls[c].IsVisible then
      begin
        result := TestLinkPoints(DControls[c]);
        if result <> nil then Exit;
      end;
  end;
end;

function TatDiagram.HandleAtPos(const Pos: TDot; AIgnoreZoom: boolean = false): TStretchHandle;
var
  c: integer;
  d: integer;
  PointOff: integer;
begin
  result := nil;
  PointOff := 5;
  if AIgnoreZoom then
    PointOff := round(PointOff / FZoomRatio);
  if RulerAtPos(ClientToCanvas(Pos)) = nil then
  begin
    {Check handles of selection block. We must check this manually because
     selection block is not included in DControls list}
    if GroupSelection then
      for d := 0 to FSelBlock.Handles.Count - 1 do if FSelBlock.Handles[d].Visible then
        if AroundPoint(Pos, FSelBlock.Handles[d].DiagramPoint, PointOff) then
        begin
          result := FSelBlock.Handles[d];
          exit;
        end;

    {if not selection block handle, then look at other handles. Use DControlCount instead of
     SelectedCount so we keep order where objects are drawn (from front to back)}
    for c := 0 to DControlCount - 1 do
      if DControls[c].Selected and DControls[c].IsVisible then
        for d := 0 to DControls[c].Handles.Count - 1 do if DControls[c].Handles[d].Visible then
          if AroundPoint(Pos, DControls[c].Handles[d].DiagramPoint, PointOff) then
          begin
            result := DControls[c].Handles[d];
            exit;
          end;

  end;
end;

function TatDiagram.DControlAtPos(const Pos: TDot; SearchMode: TControlSearchMode = csmAll;
  AGroupFilter: TDiagramControlFilter = cfIgnoreGroups): TDiagramControl;
var
  c: Integer;
begin
  result := nil;
  if RulerAtPos(ClientToCanvas(Pos)) = nil then
    for c := DControlCount - 1 downto 0 do
    // added possibility to ignore lines when searching for a control
      if ((SearchMode = csmAll) and
          DControls[c].IsVisible and PtInRectEx(DControls[c].SurroundRect, Pos, 5) and
          DControls[c].PointInControl(RoundPoint(Pos)))
         or
         ((SearchMode = csmIgnoreLines) and not (DControls[c] is TCustomDiagramLine) and
          DControls[c].IsVisible and PtInRectEx(DControls[c].SurroundRect, Pos, 5) and
          DControls[c].PointInControl(RoundPoint(Pos))) then
      begin
        if (AGroupFilter = cfAll)
          or ((AGroupFilter = cfIgnoreMembers) and not DControls[c].IsMember)
          or ((AGroupFilter = cfIgnoreAsMembers) and not DControls[c].AsMember)
          or ((AGroupFilter = cfIgnoreGroups) and not DControls[c].IsGroup) then
        begin
          result := DControls[c];
          break;
        end;
      end;
end;

procedure TatDiagram.WMEraseBkgnd(var m: TWMEraseBkgnd);
begin
  {$IFDEF DELPHI7_LVL}
  if ParentBackground then
    inherited
  else
    m.result := 1;
  {$ELSE}
  m.result := 1;
  {$ENDIF}
end;

function TatDiagram.GetDiagramWidth: integer;
var
  c: integer;
begin
  result := 0;
  for c := 0 to DControlCount - 1 do
    result := Max(Round(DControls[c].SurroundRect.Right), result);
end;

function TatDiagram.GetDiagramHeight: integer;
var
  c: integer;
begin
  result := 0;
  for c := 0 to DControlCount - 1 do
    result := Max(Round(DControls[c].SurroundRect.Bottom), result);
end;


function TatDiagram.GetDiagramLeft: integer;
var
  c: integer;
begin
  result := MaxInt;
  for c := 0 to DControlCount - 1 do
    result := Min(Round(DControls[c].SurroundRect.Left), result);
end;

function TatDiagram.GetDiagramTop: integer;
var
  c: integer;
begin
  result := MaxInt;
  for c := 0 to DControlCount - 1 do
    result := Min(Round(DControls[c].SurroundRect.Top), result);
end;

procedure TatDiagram.CalcPanelSize;
var
  c: integer;
  MaxRight: number;
  MaxBottom: number;
  MinLeft: number;
  MinTop: number;
  ARect: TSquare;
  Page: TDot;
  MaxControlRight: number;
  MaxControlBottom: number;
begin
  if (FUpdateCount = 0) and (FInternalUpdating = 0) and (Parent <> nil) then
  begin
    MaxRight := ClientWidth - FLeftRuler.VisibleSize;
    MaxBottom := ClientHeight - FTopRuler.VisibleSize;
    MaxControlRight := 0;
    MaxControlBottom := 0;
    MinLeft := 0;
    MinTop := 0;
    for c := 0 to DControlCount - 1 do if DControls[c].IsVisible and not DControls[c].IsGroup then
    begin
      ARect := DControls[c].SurroundRect;

      ARect.TopLeft := MulPoint(ARect.TopLeft, FZoomRatio);
      ARect.BottomRight := MulPoint(ARect.BottomRight, FZoomRatio);

      MaxRight := Max(ARect.Right + 1, MaxRight);
      MaxBottom := Max(ARect.Bottom + 1, MaxBottom);
      MaxControlRight := Max(ARect.Right + 1, MaxControlRight);
      MaxControlBottom := Max(ARect.Bottom + 1, MaxControlBottom);
      MinLeft := Min(ARect.Left, MinLeft);
      MinTop := Min(ARect.Top, MinTop);
    end;
    if (MinLeft < 0) or (MinTop < 0) then
    begin
      BeginInternalUpdate;
      BeginUpdatePos;
      try
        for c:= 0 to DControlCount - 1 do
          if not DControls[c].IsMember then
            DControls[c].MovePosition(-MinLeft, -MinTop);
      finally
        EndUpdatePos;
        EndInternalUpdate;
        for c := 0 to DControlCount - 1 do
          DControls[c].NotifyAnchoreds(acPosition);
      end;
    end;

    if FAutoPage then
    begin
      UpdatePixelConvertion;

      {Calculate the size of the page in client pixels}
      if FPrintConvX > 0 then
        Page.X := PrintClientWidth / FPrintConvX
      else
        Page.X := PrintClientWidth;
      if FPrintConvY > 0 then
        Page.Y := PrintClientHeight / FPrintConvY
      else
        Page.Y := PrintClientHeight;
      Page := MulPoint(Page, FZoomRatio);

      if Page.X <> 0 then
        HorzScrollBar.Range := round(Page.X * (trunc(MaxControlRight / Page.X) + 1) + Page.X * 0.05) + FLeftRuler.VisibleSize;
      if Page.Y <> 0 then
        VertScrollBar.Range := round(Page.Y * (trunc(MaxControlBottom / Page.Y) + 1) + Page.Y * 0.05) + FTopRuler.VisibleSize;
    end
    else
    if FAutoScroll then
    begin
      {Calculate new position and range for scroll bars, and update them}
      HorzScrollBar.Range := round(MaxRight - MinLeft + FLeftRuler.VisibleSize);
      VertScrollBar.Range := round(MaxBottom - MinTop + FTopRuler.VisibleSize);
    end;
    HorzScrollBar.Position := HorzScrollBar.Position - Round(MinLeft);
    VertScrollBar.Position := VertScrollBar.Position - Round(MinTop);
    UpdateScrollBars;
  end;
end;

procedure TatDiagram.KeepInBounds;
var
  c: integer;
  MinLeft: number;
  MinTop: number;
  ARect: TSquare;
begin
  if not CanMoveOutOfBounds and (FUpdateCount = 0) and (Parent <> nil) then
  begin
    MinLeft := 0;
    MinTop := 0;
    for c := 0 to DControlCount - 1 do if DControls[c].IsVisible and not DControls[c].IsGroup then
    begin
      ARect := DControls[c].SurroundRect;
      ARect.TopLeft := MulPoint(ARect.TopLeft, FZoomRatio);
      MinLeft := Min(ARect.Left, MinLeft);
      MinTop := Min(ARect.Top, MinTop);
    end;

    if (MinLeft < 0) or (MinTop < 0) then
    begin
      BeginInternalUpdate;
      BeginUpdatePos;
      try
        for c:= 0 to DControlCount - 1 do
          if not DControls[c].AsMember and DControls[c].Selected then
            DControls[c].MovePosition(-MinLeft, -MinTop);
      finally
        EndUpdatePos;
        EndInternalUpdate;
        for c := 0 to DControlCount - 1 do
          DControls[c].NotifyAnchoreds(acPosition);
      end;
    end;
  end;
end;

function TatDiagram.CanvasOffset: TDot;
begin
  result := ClientToCanvas(Dot(0, 0));
end;

function TatDiagram.RulerOffset: TDot;
begin
  result := Dot(FLeftRuler.VisibleSize, FTopRuler.VisibleSize);
end;

function TatDiagram.PolygonToCanvas(Pts: TDotArray): TDotArray;
var
  c: integer;
begin
  SetLength(result, Length(Pts));
  for c := 0 to Length(Pts) - 1 do
    result[c] := ClientToCanvas(Pts[c]);
end;

function TatDiagram.ClientToCanvas(APoint: TPoint): TPoint;
begin
  result := RoundPoint(ClientToCanvas(Dot(APoint.X, APoint.Y)));
end;

function TatDiagram.ClientToCanvas(APoint: TDot): TDot;
var
  ATempResult: TDot;
begin
  ATempResult := APoint;
  case FPaintStyle of
    psScreen:
      begin
        if FZoomRatio <> 1 then
          ATempResult := MulPoint(ATempResult, FZoomRatio);
        ATempResult := AddPointX(ATempResult, Dot(-HorzScrollBar.Position, -VertScrollBar.Position));
        ATempResult := AddPointX(ATempResult, Dot(RulerOffset.X, RulerOffset.Y));
      end;
    psBitmap:
      begin
        ATempResult := AddPointX(ATempResult, Dot(FPreviewOffset.X, FPreviewOffset.Y));
        if FZoomRatio <> 1 then
          ATempResult := MulPoint(ATempResult, FZoomRatio);
        {result := AddPoint(result, Point(-HorzScrollBar.Position, -VertScrollBar.Position));
        result := AddPoint(result, RulerOffset);}
      end;
    psPrint, psPreview:
      begin
        ATempResult := Dot(ATempResult.X * FPrintConvX, ATempResult.Y * FPrintConvY);
        ATempResult := AddPointX(ATempResult, Dot(FPreviewOffset.X, FPreviewOffset.Y));
        ATempResult := Dot(ATempResult.X * FAdjustRatioX, ATempResult.Y * FAdjustRatioY);
        if FPreviewZoom <> 1 then
          ATempResult := MulPoint(ATempResult, FPreviewZoom);
      end;
  end;
  result := ATempResult;
end;

function TatDiagram.CanvasToClient(APoint: TPoint): TPoint;
begin
  result := RoundPoint(CanvasToClient(Dot(APoint.X, APoint.Y)));
end;

function TatDiagram.CanvasToClient(APoint: TDot): TDot;
begin
  result := APoint;
  Case FPaintStyle of
    psScreen:
      begin
        result := SubPointX(result, CanvasOffset);
        if FZoomRatio <> 1 then
          result := MulPoint(result, 1 / FZoomRatio);
      end;
    psBitmap:
      begin
        if FZoomRatio <> 1 then
          result := MulPoint(result, 1 / FZoomRatio);
        result := SubPointX(result, FPreviewOffset);
      end;
    psPrint, psPreview:
      begin
        if FPreviewZoom <> 1 then
          result := MulPoint(result, 1 / FPreviewZoom);
        result := Dot(result.X / FAdjustRatioX, result.Y / FAdjustRatioY);
        result := SubPointX(result, FPreviewOffset);
        result := Dot(result.X / FPrintConvX, result.Y / FPrintConvY);
      end;
  end;
end;

procedure TatDiagram.CheckGraphicLibObjects;
var
  i: integer;
begin
  for i := 0 to DControlCount - 1 do
    DControls[i].CheckGraphicLibObjects;
end;

function TatDiagram.CanvasToClient(ARect: TSquare): TSquare;
begin
  result.TopLeft := CanvasToClient(ARect.TopLeft);
  result.BottomRight := CanvasToClient(ARect.BottomRight);
end;

function TatDiagram.ClientToCanvas(ARect: TRect): TRect;
begin
  result.TopLeft := ClientToCanvas(ARect.TopLeft);
  result.BottomRight := ClientToCanvas(ARect.BottomRight);
end;

function TatDiagram.ClientToCanvas(ARect: TSquare): TSquare;
begin
  result.TopLeft := ClientToCanvas(ARect.TopLeft);
  result.BottomRight := ClientToCanvas(ARect.BottomRight);
end;

function TatDiagram.SelectionRect(OnlyBlocks: boolean; AAngle: double = 0): TSquare;
var
  c: integer;
  AItems: TDControlList;
begin
  AItems := TDControlList.Create(nil, TDControlListItem);
  try
    for c := 0 to SelectedCount - 1 do if not Selecteds[c].AsMember then
      AItems.Add(Selecteds[c]);
    result := CalculateItemsRect(AItems, OnlyBlocks, AAngle);
  finally
    AItems.Free;
  end;
end;


procedure TatDiagram.SelectNext(Direction: TSelectDirection);
Var c: integer;
    ARect: TSquare;
    ACtrlRect: TSquare;
    ACtrlFound: TCustomDiagramBlock;
    AFoundRect: TSquare;
    Dist: number;
begin
  ARect := SelectionRect(true);
  ACtrlFound := nil;
  for c := 0 to BlockCount - 1 do if Blocks[c].IsVisible and not Blocks[c].IsMember then
  begin
    ACtrlRect := Blocks[c].BoundsRect;
    Case Direction of
      sdUp:
        begin
          Dist := ARect.Top - ACtrlRect.Bottom;
          if (Dist > 0) and (ACtrlRect.Right > ARect.Left - Dist) and (ACtrlRect.Left < ARect.Right + Dist) then
            if not Assigned(ACtrlFound) or (ACtrlRect.Bottom > AFoundRect.Bottom) then
            begin
              ACtrlFound := Blocks[c];
              AFoundRect := ACtrlRect;
            end;
        end;
      sdDown:
        begin
          Dist := ACtrlRect.Top - ARect.Bottom;
          if (Dist > 0) and (ACtrlRect.Right > ARect.Left - Dist) and (ACtrlRect.Left < ARect.Right + Dist) then
            if not Assigned(ACtrlFound) or (ACtrlRect.Top < AFoundRect.Top) then
            begin
              ACtrlFound := Blocks[c];
              AFoundRect := ACtrlRect;
            end;
        end;
      sdLeft:
        begin
          Dist := ARect.Left - ACtrlRect.Right;
          if (Dist > 0) and (ACtrlRect.Bottom > ARect.Top - Dist) and (ACtrlRect.Top < ARect.Bottom + Dist) then
            if not Assigned(ACtrlFound) or (ACtrlRect.Right > AFoundRect.Right) then
            begin
              ACtrlFound := Blocks[c];
              AFoundRect := ACtrlRect;
            end;
        end;
      sdRight:
        begin
          Dist := ACtrlRect.Left - ARect.Right;
          if (Dist > 0) and (ACtrlRect.Bottom > ARect.Top - Dist) and (ACtrlRect.Top < ARect.Bottom + Dist) then
            if not Assigned(ACtrlFound) or (ACtrlRect.Left < AFoundRect.Left) then
            begin
              ACtrlFound := Blocks[c];
              AFoundRect := ACtrlRect;
            end;
        end;
    end;
  end;
  if Assigned(ACtrlFound) then
  begin
    BeginUpdateSel;
    try
      UnselectAll;
      ACtrlFound.Selected := true;
    finally
      EndUpdateSel;
    end;
  end;
end;

procedure TatDiagram.WMGetDlgCode(var Message: TWMGetDlgCode);
begin
  Message.Result := DLGC_WANTARROWS;
end;

procedure TatDiagram.KeyDown(var Key: Word; Shift: TShiftState);
begin
  Case Key of
    VK_ESCAPE:
      if kaEscape in FKeyActions then
        Case FMouseState of
          msMoving:
            EndMoving(DummyPoint.X, DummyPoint.Y, true);
          msResizing:
            EndResizing(DummyPoint.X, DummyPoint.Y, true);
          msInserting:
            EndInserting(DummyPoint.X, DummyPoint.Y, true);
          msDrawing:
            EndDrawing(DummyPoint.X, DummyPoint.Y, true);
          msSelecting:
            begin
              CancelSelecting;
              UnselectAll;
            end;
        end;
    VK_UP:
      if (ssCtrl in Shift) then
      begin
        if (kaMove in FKeyActions) then
        begin
          FDragStart := Dot(0, 0);
          if SnapGrid.Active then
            MoveBlocks(0, -SnapGrid.SizeY, true)
          else
            MoveBlocks(0, -1, true);
        end;
      end
      else
      if (ssShift in Shift) then
      begin
        if (kaResize in FKeyActions) then
          FDragStart := Dot(0, 0);
      end
      else if kaSelect in FKeyActions then
        SelectNext(sdUp);
    VK_DOWN:
      if (ssCtrl in Shift) then
      begin
        if (kaMove in FKeyActions) then
        begin
          FDragStart := Dot(0, 0);
          if SnapGrid.Active then
            MoveBlocks(0, SnapGrid.SizeY, true)
          else
            MoveBlocks(0, 1, true);
        end;
      end
      else
      if (ssShift in Shift) then
      begin
        if (kaResize in FKeyActions) then
          FDragStart := Dot(0, 0);
      end
      else if kaSelect in FKeyActions then
        SelectNext(sdDown);
    VK_LEFT:
      if (ssCtrl in Shift) then
      begin
        if (kaMove in FKeyActions) then
        begin
          FDragStart := Dot(0, 0);
          if SnapGrid.Active then
            MoveBlocks(-SnapGrid.SizeY, 0, true)
          else
            MoveBlocks(-1, 0, true);
        end;
      end
      else
      if (ssShift in Shift) then
      begin
        if (kaResize in FKeyActions) then
          FDragStart := Dot(0, 0);
      end
      else if kaSelect in FKeyActions then
        SelectNext(sdLeft);
    VK_RIGHT:
      if (ssCtrl in Shift) then
      begin
        if (kaMove in FKeyActions) then
        begin
          FDragStart := Dot(0, 0);
          if SnapGrid.Active then
            MoveBlocks(SnapGrid.SizeY, 0, true)
          else
            MoveBlocks(1, 0, true);
        end;
      end
      else
      if (ssShift in Shift) then
      begin
        if (kaResize in FKeyActions) then
          FDragStart := Dot(0, 0);
      end
      else if kaSelect in FKeyActions then
        SelectNext(sdRight);
    VK_NEXT:
      if kaPage in FKeyActions then
      begin
        FVertScrollBar.Position := FVertScrollBar.Position + ClientHeight;
        Redraw;
      end;
    VK_PRIOR:
      if kaPage in FKeyActions then
      begin
        FVertScrollBar.Position := FVertScrollBar.Position - ClientHeight;
        Redraw;
      end;
  end;
  inherited KeyDown(Key, Shift);
end;

procedure TatDiagram.Resize;
begin
  inherited;
  CalcPanelSize;
end;

procedure TatDiagram.Modified;
begin
  if (FInternalUpdating = 0) and Assigned(FOnModified)
    and not (csDestroying in ComponentState) 
    and not (csLoading in ComponentState) then
    FOnModified(Self);
end;

function TatDiagram.SerializeToString: string;
Var
  AStream: TStringStream;
  AuxStream: TMemoryStream;
  S: string;
  DiagramName: string;
begin
  AStream := TStringStream.Create(S);
  AuxStream := TMemoryStream.Create;
  DiagramName := Self.Name;
  try
    Self.Name := '';
    AuxStream.WriteComponent(Self);
    AuxStream.Seek(0,soFromBeginning);
    ObjectBinaryToText(AuxStream,AStream);
    AStream.Seek(0,soFromBeginning);
    S := AStream.DataString;
  finally
    Self.Name := DiagramName;
    AStream.Free;
    AuxStream.Free;
  end;
  result := S;
end;

procedure TatDiagram.SerializeFromClipboard;
Var
  H, len: integer;
  Buff: string;
begin
  if not Clipboard.HasFormat(CF_DIAGRAM) then
    Exit;
  H := ClipBoard.GetAsHandle(CF_TEXT);
  len := GlobalSize(H);
  if len = 0 then Exit;
  Buff := Clipboard.AsText;
  SerializeFromString(Buff);
end;

procedure TatDiagram.SerializeFromString(S: string);
var
  AStream: TStringStream;
  AuxStream: TMemoryStream;
  DiagramName: string;
  TempDiagram: TatDiagram;
  i: integer;
begin
  AStream := TStringStream.Create(S);
  AuxStream := TMemoryStream.Create;
  try
    AStream.WriteString(S);
    AStream.Seek(0, soFromBeginning);
    ObjectTextToBinary(AStream, AuxStream);
    AuxStream.Seek(0, soFromBeginning);
    BeginInternalUpdate;
    TempDiagram := DiagramStreamClass.Create(Owner);
    try
      {Save diagram properties to a temporary component}
      TempDiagram.Parent := Self.Parent;
      TempDiagram.Assign(Self);
      {Delete the blocks and lines, keep only the diagram properties}
      TempDiagram.SelectAll;
      TempDiagram.DeleteSelecteds;

      DiagramName := Self.Name;
      Self.Name := '';
      try
        ReadDiagramStream(AuxStream);
      finally
        Self.Name := DiagramName;
      end;
      Self.Assign(TempDiagram);

      for i := 0 to DControlCount - 1 do
        DControls[i].RestoreLayer;
    finally
      TempDiagram.Free;
      EndInternalUpdate;
    end;
  finally
    AStream.Free;
    AuxStream.Free;
  end;
end;

var
  _LocalClipPrefix: string;

procedure _SetClipPrefixName(ADControl: TDiagramControl);
begin
  ADControl.Name := _LocalClipPrefix + ADControl.Name;
end;

procedure TatDiagram.CopyObjectsToClipboard;
var
  c: integer;
  Save: string;
begin
  for c := 0 to SelectedCount - 1 do if not Selecteds[c].AsMember then
    if Selecteds[c].CanDoClipboard then
    begin
      if Selecteds[c].IsGroup then
      begin
        _LocalClipPrefix := FClipPrefix;
        TGroupBlock(Selecteds[c]).IterateAllMembers(_SetClipPrefixName);
      end
      else
        Selecteds[c].Name := FClipPrefix + Selecteds[c].Name;
    end;

  FClipboardStreaming := true;
  try
    Save := SerializeToString;
  finally
    FClipboardStreaming := false;
  end;

  for c := 0 to DControlCount - 1 do
    if Pos(FClipPrefix, DControls[c].Name) <> 0 then
      DControls[c].Name := Copy(DControls[c].Name, Length(FClipPrefix) + 1, MaxInt);

  {Now copy to clipboard}
  with Clipboard do
  begin
    Open;
    try
      SetAsHandle(CF_DIAGRAM, 0);
      AsText := Save;
    finally
      Close;
    end;
  end;
end;

function TatDiagram.SerializeObjectToString(ADControl: TDiagramControl): string;
var
  c: integer;
begin
  if ADControl.IsGroup then
  begin
    _LocalClipPrefix := FClipPrefix;
    TGroupBlock(ADControl).IterateAllMembers(_SetClipPrefixName);
  end
  else
    ADControl.Name := FClipPrefix + ADControl.Name;

  FClipboardStreaming := true;
  try
    result := SerializeToString;
  finally
    FClipboardStreaming := false;
  end;

  for c := 0 to DControlCount - 1 do
    if Pos(FClipPrefix, DControls[c].Name) <> 0 then
      DControls[c].Name := Copy(DControls[c].Name, Length(FClipPrefix) + 1, MaxInt);
end;

procedure TatDiagram.PasteObjectsFromClipboard;

  procedure RecoverCompName(AComp: TDiagramControl; ASelList: TDControlList);
  var
    ANewName: string;
  begin
    if Pos(FClipPrefix, AComp.Name) <> 0 then
    begin
      ANewName := Copy(AComp.Name, Length(FClipPrefix) + 1, MaxInt);
      if Assigned(GetChildOwner.FindComponent(ANewName)) or Assigned(FindDControl(ANewName)) then
        ANewName := FindCompName(AComp);
      AComp.Name := ANewName;
      if AComp is TDiagramControl then
        ASelList.Add(AComp);
    end;
  end;

  procedure AvoidOverlap(ACtrl: TDiagramControl);

    function BlockOverlapped(ABlock: TCustomDiagramBlock): boolean;
    var
      c: integer;
    begin
      result := false;
      for c := 0 to BlockCount - 1 do if Blocks[c].Visible and not Blocks[c].IsMember then
        if (ABlock <> Blocks[c]) and (ABlock.Left = Blocks[c].Left) and
          (ABlock.Top = Blocks[c].Top) then
        begin
          result := true;
          exit;
        end;
    end;

  begin
    if ACtrl is TCustomDiagramBlock then
    begin
      while BlockOverlapped(TCustomDiagramBlock(ACtrl)) do
      begin
        TCustomDiagramBlock(ACtrl).Left := TCustomDiagramBlock(ACtrl).Left + 20;
        TCustomDiagramBlock(ACtrl).Top := TCustomDiagramBlock(ACtrl).Top + 20;
      end;
    end;
  end;

var
  c: integer;
  ASelectList: TDControlList;
begin
  SerializeFromClipboard;

  ASelectList := TDControlList.Create(nil, TDControlListItem);
  try
    for c := 0 to DControlCount - 1 do
      RecoverCompName(DControls[c], ASelectList);
    BeginUpdateSel;
    UnselectAll;
    try
      for c := 0 to ASelectList.Count - 1 do
        if not ASelectList[c].DControl.IsMember then
        begin
          {$WARNINGS OFF}
          ASelectList[c].DControl.Selected := true;
          AvoidOverlap(ASelectList[c].DControl);
          {$WARNINGS ON}
        end;
    finally
      EndUpdateSel;
    end;
    CalcPanelSize;
  finally
    for c := 0 to SelectedCount - 1 do
    begin
      Selecteds[c].NotifyAnchoreds(acPosition);
      DoInsertDControl(Selecteds[c]);
      if Selecteds[c] is TCustomDiagramLine then
        DoInsertLink(TCustomDiagramLine(Selecteds[c]));
      if Selecteds[c] is TCustomDiagramBlock then
        DoInsertBlock(TCustomDiagramBlock(Selecteds[c]));
    end;
    ASelectList.Free;
  end;
  Modified;
  PushUndoStack('paste');
end;

procedure TatDiagram.CutObjectsToClipboard;
begin
  CopyToClipboard;
  DeleteControls([crNoClipboard, crNoDelete]);

  Redraw;
  //PushUndoStack('cut'); <-- Should be commented because DeleteSelecteds already pushes undo stack
end;

function TatDiagram.CanPaste: boolean;
begin
  result := MouseState = msBrowsing;
  result := result and
    (Clipboard.HasFormat(CF_DIAGRAM) and not EditingText)
    or
    (EditingText and Clipboard.HasFormat(CF_TEXT));
end;

function TatDiagram.CanCopy: boolean;
begin
  result := (SelectedCount > 0) and (MouseState = msBrowsing);
end;

procedure TatDiagram.CopyTextToClipboard;
begin
  if FInplaceMemo <> nil then
    FInplaceMemo.CopyToClipboard;
end;

procedure TatDiagram.CutTextToClipboard;
begin
  if FInplaceMemo <> nil then
    FInplaceMemo.CutToClipboard;
end;

procedure TatDiagram.PasteTextFromClipboard;
begin
  if FInplaceMemo <> nil then
  begin
    FInplaceMemo.PasteFromClipboard;
    Modified;
  end;
end;                          

procedure TatDiagram.PerformSelection(ClickedControl: TDiagramControl;
  ClickMode: TDgrSelectionClickMode);
var
  AToSelect: TDiagramControl;
begin
  AToSelect := ClickedControl;
  
  case ClickMode of
    dcmClick:
      begin
        if not AToSelect.Selected then
        begin
          if AToSelect.IsMember then
            AToSelect := AToSelect.MasterGroup;
          UnselectAll;
          SetGroupSelected(AToSelect, true)
        end;
      end;
    dcmSubClick:
      begin
        AToSelect := SubControltoSelect(AToSelect);
        UnselectAll;
        if AToSelect.IsMember and AToSelect.Selected then
          SetGroupSelected(AToSelect.GroupBlock, false);
        SetGroupSelected(AToSelect, true)
      end;
    dcmShiftClick:
      begin
        AToSelect := SubControltoSelect(AToSelect);
        if AToSelect.IsMember and AToSelect.Selected then
        begin
          SetGroupSelected(AToSelect.GroupBlock, false);
          SetGroupSelected(AToSelect, true)
        end
        else
          SetGroupSelected(AToSelect, not AToSelect.Selected);
      end;
  end;

  Redraw;
end;

function TatDiagram.SubControlToSelect(ClickedControl: TDiagramControl): TDiagramControl;
var
  AGroup: TBaseGroupBlock;
  AToSelect: TDiagramControl;
begin
  AToSelect := ClickedControl;
  AGroup := AToSelect.GroupBlock;

  {if the parent block don't have any selected, select the parent group. If the parent
   group block has any block selected, then we must select the current one.}

  {  while (AGroup <> nil) and
   not (
  (AGroup.Selected and not AGroup.AsMember)
  or
  (not AGroup.Selected and AGroup.HasSelecteds)
  )}
  while (AGroup <> nil) and
    (not AGroup.Selected or AGroup.AsMember)
    and
    (AGroup.Selected or not AGroup.HasSelecteds)
  do
  begin
    AToSelect := AToSelect.GroupBlock;
    AGroup := AToSelect.GroupBlock;
  end;

  result := AToSelect;
end;

procedure TatDiagram.TransformPaintEngine(APaintEngine: TDgrPaintEngine);
begin
  case FPaintStyle of
    psScreen:
      begin
        APaintEngine.ScaleTransform(FZoomRatio, FZoomRatio);
        APaintEngine.TranslateTransform(-HorzScrollBar.Position, -VertScrollBar.Position);
        APaintEngine.TranslateTransform(RulerOffset.X, RulerOffset.Y);
      end;
    psBitmap:
      begin
        APaintEngine.TranslateTransform(FPreviewOffset.X, FPreviewOffset.Y);
        APaintEngine.ScaleTransform(FZoomRatio, FZoomRatio);
      end;
    psPrint, psPreview:
      begin
        APaintEngine.PageUnit := dpuPixel;
        APaintEngine.ScaleTransform(FPrintConvX, FPrintConvY);
        APaintEngine.TranslateTransform(FPreviewOffset.X, FPreviewOffset.Y);
        APaintEngine.ScaleTransform(FAdjustRatioX, FAdjustRatioY);
        APaintEngine.ScaleTransform(FZoomRatio, FZoomRatio);
      end;
  end;
end;

procedure TatDiagram.CopyToClipboard;
begin
  if EditingText then
    CopyTextToClipboard
  else
    CopyObjectsToClipboard;
end;

procedure TatDiagram.CutToClipboard;
begin
  if EditingText then
    CutTextToClipboard
  else
    CutObjectsToClipboard;
end;

procedure TatDiagram.PasteFromClipboard;
begin
  if EditingText then
    PasteTextFromClipboard
  else
    PasteObjectsFromClipboard;
end;

procedure TatDiagram.BeginInternalUpdate;
begin
  Inc(FInternalUpdating);
end;

procedure TatDiagram.EndInternalUpdate;
begin
  Dec(FInternalUpdating);
end;

procedure TatDiagram.PushUndoStack(const AActionName: string);
begin
  FUndoStack.PushState(AActionName);
end;

procedure TatDiagram.ClearUndoStack;
begin
  FUndoStack.ClearUndoStack;
end;

procedure TatDiagram.Clear;
begin
  ClearDControls;
  SetDiagramDefaults;
  CancelInsertingBlock;
end;

procedure TatDiagram.BeginUpdatePos;
begin
  Inc(FUpdatingPos);
end;

procedure TatDiagram.EndUpdatePos;
begin
  Dec(FUpdatingPos);
  if FUpdatingPos = 0 then
  begin
    CalcPanelSize;
    Redraw;
  end;
end;

procedure TatDiagram.BeginUpdateSel;
begin
  if FUpdatingSel = 0 then
  begin
    if Assigned(FOnSelectionChanging) then
      FOnSelectionChanging(Self);
    DesignEvent(Self, etSelectionChanging, 0);
  end;
  Inc(FUpdatingSel);
end;

procedure TatDiagram.EndUpdateSel;
begin
  if FUpdatingSel > 0 then
    Dec(FUpdatingSel);
  if (FUpdatingSel = 0) then
  begin
    UpdateSelectionBlockAngle;

    if GroupSelection then
      UpdateSelectionBlock;

    if Assigned(FOnSelectionChanged) then
      FOnSelectionChanged(Self);
    DesignEvent(Self, etSelectionChanged, 0);
  end;
end;

procedure TatDiagram.Undo;
begin
  FUndoStack.Undo;
  Redraw;
end;

procedure TatDiagram.Redo;
begin
  FUndoStack.Redo;
  Redraw;
end;

function TatDiagram.NextUndoAction: string;
begin
  result := FUndoStack.NextUndoAction;
end;

procedure TatDiagram.NameAllDControls;
var
  i: integer;
  dcontrol: TDiagramControl;
begin
  for i := 0 to DControlCount - 1 do
  begin
    dcontrol := DControls[i];
    if dcontrol.Name = '' then
      dcontrol.Name := FindCompName(dcontrol);
  end;
end;

function TatDiagram.NextRedoAction: string;
begin
  result := FUndoStack.NextRedoAction;
end;

procedure TatDiagram.PaintWindow(DC: HDC);
begin
  FControlCanvas.Handle := DC;
  try
    TControlCanvas(FControlCanvas).UpdateTextFlags;
    PaintToCanvas(FControlCanvas, psScreen, Dot(0, 0));
  finally
    FControlCanvas.Handle := 0;
  end;
  if EditingText then
  begin
    FInplaceMemo.UpdatePosition;
    FInplaceMemo.Invalidate;
  end;
end;

procedure TatDiagram.CMDesignHitTest(var Msg: TCMDesignHitTest);
begin
  Msg.Result := Longint(
    FDesigning (*and not (  (Msg.XPos > (ClientWidth - 10)) and (Msg.YPos <= 10))*)
    );
end; 

procedure TatDiagram.DesignEvent(Sender: TObject;
  AEventType: TDiagramEventType; Info: integer);
begin
  if Assigned(FOnDesignEvent) then
    FOnDesignEvent(Sender, AEventType, Info);
end;

function TatDiagram.GetSelecteds(Index: integer): TDiagramControl;
begin
  {$WARNINGS OFF}
  result := TDiagramControl(FSelecteds[Index]);
  {$WARNINGS ON}
end;

function TatDiagram.SelectedCount(AFilter: TDiagramControlFilter = cfAll): integer;
var
  c: integer;
begin
  if AFilter = cfAll then
    result := FSelecteds.Count
  else
  begin
    result := 0;
    for c := 0 to SelectedCount - 1 do
      Case AFilter of
        cfIgnoreAsMembers:
          if not Selecteds[c].AsMember then
            result := result + 1;
        cfIgnoreMembers:
          if not Selecteds[c].IsMember then
            result := result + 1;
        cfIgnoreGroups:
          if not Selecteds[c].IsGroup then
            result := result + 1;
      end;
  end;
end;

procedure TatDiagram.SetSnapGrid(const Value: TSnapGrid);
begin
  FSnapGrid.Assign(Value);
end;

function TatDiagram.DoGetSurroundRgn(ABlock: TCustomDiagramBlock): TDotArray;
var
  Pts: TPointArray;
begin
  SetLength(result, 0);
  if Assigned(FOnGetSurroundPts) then
  begin
    FOnGetSurroundPts(Self, ABlock, Pts);
    result := ToDotArray(Pts);
  end;
end;

function TatDiagram.CustomDrawBlock(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
  ABlockInfo: TDiagramDrawBlockInfo): boolean;
begin
  result := false;
  if Assigned(FOnDrawBlock) then
    FOnDrawBlock(Self, ABlock, AInfo, ABlockInfo, result);
end;

function TatDiagram.CustomDrawShape(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
  ABlockInfo: TDiagramDrawBlockInfo): boolean;
begin
  result := false;
  if Assigned(FOnDrawShape) then
    FOnDrawShape(Self, ABlock, AInfo, ABlockInfo, result);
end;

procedure TatDiagram.UpdateMemoEditor(ACell: TTextCell);
begin
  if Assigned(ACell) then
  begin
    if FInplaceMemo = nil then
    begin
      // use nil instead of Self while creating memo. This will avoid memo streaming
      FInplaceMemo := DiagramMemoClass.Create(nil);
      FInplaceMemo.FDiagram := Self;
      FInplaceMemo.Parent := Self;
    end;
    FInplaceMemo.FKeepPosition := true;
    try
      FInplaceMemo.FCell := ACell;
      FInplaceMemo.WordWrap := ACell.WordWrap;
      FInplaceMemo.Alignment := ACell.Alignment;
      FInplaceMemo.Lines.Text := ACell.Text;
      FInplaceMemo.Font := ACell.Font;
      FInplaceMemo.Font.Size := round(FInplaceMemo.Font.Size * ZoomRatio);
    finally
      FInplaceMemo.FKeepPosition := false;
    end;
    FInplaceMemo.UpdatePosition;
    FInplaceMemo.Visible := true;
    FInplaceMemo.SetFocus;
    FInplaceMemo.SelStart := 0;
    // Call Redraw to remove text from block. Redraw must be called after FInplaceMemo.FCell is assigned
    Redraw;
  end;
end;

procedure TatDiagram.EditBlockText(ABlock: TCustomDiagramBlock);
begin
  EditControlText(ABlock);
end;

procedure TatDiagram.EditControlText(ADControl: TDiagramControl);

  function _FindEditTextMember(AGroup: TGroupBlock): TDiagramControl;
  var
    c: integer;
  begin
    result := nil;
    for c := 0 to AGroup.Members.Count - 1 do
      if AGroup.Members[c].DControl <> nil then
      begin
        if AGroup.Members[c].DControl.IsGroup then
          result := _FindEditTextMember(TGroupBlock(AGroup.Members[c].DControl))
        else
          if AGroup.Members[c].DControl.CanEditText and AGroup.Members[c].DControl.HasDefaultTextCell then
            result := AGroup.Members[c].DControl;

        if result <> nil then
          break;
      end;
  end;

var
  ACell: TTextCell;
begin
  if Assigned(ADControl) and ADControl.CanEditText then
  begin
    if ADControl.IsGroup then
      ADControl := _FindEditTextMember(TGroupBlock(ADControl));

    if (ADControl <> nil) and (ADControl.HasDefaultTextCell) then
    begin
      ACell := ADControl.DefaultTextCell;
      if not ACell.Visible then
        ACell := ADControl.SelectNextCell(ACell, true);

      if (ACell <> nil) and ACell.Visible then
        EditTextCell(ACell);
    end;
  end;
end;

procedure TatDiagram.EditTextCell(ACell: TTextCell);
begin
  if (ACell <> nil) and (ACell.DControl <> nil) and ACell.DControl.CanEditText
    and ACell.Visible and not ACell.DControl.IsGroup then
    UpdateMemoEditor(ACell);
end;

procedure TatDiagram.CloseEditor(AUpdate: boolean);
begin
  if Assigned(FInplaceMemo) then
  begin
    if Assigned(FInplaceMemo.FCell) then
    begin
      if AUpdate then
      begin
        FInplaceMemo.FCell.Text := FInplaceMemo.Lines.Text;
        Modified;
        PushUndoStack('text edit');
      end;
    end;
    if not (csDestroying in ComponentState) and (Parent <> nil) then
    begin
      FInplaceMemo.Hide;
      Redraw;
      if Visible then SetFocus;
    end;

    if Assigned(FOnCloseEditor) then
      FOnCloseEditor(Self, FInplaceMemo.FCell, AUpdate);

    FInplaceMemo.FCell := nil;
  end;
end;

function TatDiagram.EditingText: boolean;
begin
  result := (FInplaceMemo <> nil) and (FInplaceMemo.Visible);
end;

procedure TatDiagram.AddLink(ALink: TCustomDiagramLine);
begin
  FLinks.Add(ALink);
  DoInsertLink(ALink);
  Modified;
end;

procedure TatDiagram.AddDControl(ADControl: TDiagramControl);
var
  p, pnew: TDot;
begin
  if (ADControl.Name = '') and not (csLoading in ComponentState) then
    ADControl.Name := FindCompName(ADControl);

  FDControls.Add(ADControl);
  if ADControl.Selected then
    FSelecteds.Add(ADControl);
  DoInsertDControl(ADControl);
  if (ADControl is TCustomDiagramBlock) and not (csLoading in ComponentState) and SnapGrid.Active and SnapGrid.Force then
  begin
    p := TCustomDiagramBlock(ADControl).BoundsRect.TopLeft;
    pnew := SnapCanvasPoint(p);
    // have to snap another time due to the scrollbar positions not being snapped
    p.X := round(pnew.X / SnapGrid.SizeX) * SnapGrid.SizeX;
    p.Y := round(pnew.Y / SnapGrid.SizeY) * SnapGrid.SizeY;
    TCustomDiagramBlock(ADControl).Top := p.y;
    TCustomDiagramBlock(ADControl).Left := p.x;
  end;
  CalcPanelSize;
  Redraw;
  Modified;
end;

function TatDiagram.GetLinks(Index: integer): TCustomDiagramLine;
begin
  {$WARNINGS OFF}
  result := TCustomDiagramLine(FLinks[Index]);
  {$WARNINGS ON}
end;

function TatDiagram.GetMeasUnit: TDiagramMeasUnit;
begin
  result := FIntMeasUnit;
end;

function TatDiagram.GetDControls(Index: integer): TDiagramControl;
begin
  {$WARNINGS OFF}
  result := TDiagramControl(FDControls[Index]);
  {$WARNINGS ON}
end;

procedure TatDiagram.RemoveDControl(ADControl: TDiagramControl);
begin
  if FPreviousDControl = ADControl then
    FPreviousDControl := nil;
  FDControls.Remove(ADControl);
  FSelecteds.Remove(ADControl);
  if Assigned(FOnRemoveDControl) and not (csDestroying in ComponentState) then
    FOnRemoveDControl(Self, ADControl);
  if not (csDestroying in ComponentState) then
  begin
    CalcPanelSize;
    Redraw;
  end;
  Modified;
end;

procedure TatDiagram.RemoveLink(ALink: TCustomDiagramLine);
begin
  FLinks.Remove(ALink);
end;

function TatDiagram.GetChildOwner: TComponent;
begin
  result := Owner;
end;

procedure TatDiagram.LoadFromStream(AStream: TStream; TextFormat: boolean = false);
var
  DiagramName: string;
  MemStream: TMemoryStream;
begin
  MemStream := TMemoryStream.Create;
  try
    BeginInternalUpdate;
    DiagramName := Self.Name;
    try
      Clear;
      Self.Name := '';

      if TextFormat then
      begin
        ObjectTextToBinary(AStream, MemStream);
        MemStream.Seek(0, soFromBeginning);
        ReadDiagramStream(MemStream);
      end else
        ReadDiagramStream(AStream);
    finally
      Self.Name := DiagramName;
      EndInternalUpdate;
    end;
  finally
    MemStream.Free;
  end;
end;

procedure TatDiagram.SaveToStream(AStream: TStream; TextFormat: boolean = false);
var
  DiagramName:  string;
  MemStream: TMemoryStream;
begin
  MemStream := TMemoryStream.Create;
  try
    FInternalStreaming := True;
    DiagramName := Self.Name;
    try
      Self.Name := '';

      if TextFormat then
      begin
        SaveToStream(MemStream);
        MemStream.Seek(0, soFromBeginning);
        ObjectBinaryToText(MemStream, AStream);
      end else
        AStream.WriteComponent(Self);
    finally
      Self.Name := DiagramName;
      FInternalStreaming := False;
    end;
  finally
    MemStream.Free;
  end;
end;

procedure TatDiagram.LoadFromFile(const AFileName: string);
var
  MemStream: TMemoryStream;
  FileStream: TFileStream;
begin
  FileStream := TFileStream.Create(AFileName, fmOpenRead);
  try
    MemStream := TMemoryStream.Create;
    try
      Clear;
      ObjectTextToBinary(FileStream, MemStream);

      MemStream.Seek(0, soFromBeginning);
      LoadFromStream(MemStream);
      ClearUndoStack;
    finally
      MemStream.Free;
    end;
  finally
    FileStream.Free
  end;
end;

procedure TatDiagram.SaveToFile(const AFileName: string);
var
  MemStream: TMemoryStream;
  FileStream: TFileStream;
begin
  MemStream := TMemoryStream.Create;
  try
    FileStream := TFileStream.Create(AFileName, fmCreate);
    try
      SaveToStream(MemStream);
      MemStream.Seek(0, soFromBeginning);
      ObjectBinaryToText(MemStream, FileStream);
    finally
      FileStream.Free;
    end;
  finally
    MemStream.Free
  end;
end;

function TatDiagram.ScreenDPI: integer;
begin
  if IgnoreScreenDPI then
    result := 96
  else
    result := Screen.PixelsPerInch;
end;

procedure TatDiagram.UpdatePixelConvertion;
begin
  if DPrinter <> nil then
  begin
    FPrintConvX := DPrinter.DPI.X / ScreenDPI;
    FPrintConvY := DPrinter.DPI.Y / ScreenDPI;
  end else
  begin
    FPrintConvX := 1;
    FPrintConvY := 1;
  end;
end;

procedure TatDiagram.PaintToCanvas(ACanvas: TCanvas; APaintStyle: TPaintStyle; AOffset: TDot);
begin
  //Canvas.Handle := ACanvas.Handle;
  FPaintStyle := APaintStyle;
  FPreviewOffset := AOffset;
  try
    {Calculate screen/printer coordenates conversion for a wysiwyg print and preview}
    if FPaintStyle in [psPreview, psPrint] then
      UpdatePixelConvertion;
    InternalPaint(ACanvas);
  finally
    FPaintStyle := psScreen;
    //Canvas.Handle := 0;
  end;
end;

Function TatDiagram.GetPixelFormat : TPixelFormat;
var
  DesktopDC: HDC;
begin
  DesktopDC := GetDC(0);
  try
    Case GetDeviceCaps(DesktopDC, BITSPIXEL) of
      1: Result := pf1bit;
      4: Result := pf4bit;
      8: Result := pf8bit;
      15: Result := pf15bit;
      16: Result := pf16bit;
      24: Result := pf24bit;
      32: Result := pf32bit;
    else
      Result := pf24bit;
    end;
  finally
    ReleaseDC(0, DesktopDC);
  end;
end;

procedure TatDiagram.PaintToBitmap(ABitmap: TBitmap; exportSize: TExportSize = esStandard);
var
  PixelFormat: TPixelFormat;
  expSize: TPoint;
  expPos: TPoint;
begin
  GetExportSizeAndPos(exportSize, expSize, expPos);
  PixelFormat := GetPixelFormat;
  ABitmap.Width := expSize.x;
  ABitmap.Height := expSize.y;
  ABitmap.PixelFormat := PixelFormat;
  ABitmap.PixelFormat := PixelFormat;
  FPreviewZoom := FZoomRatio;
  PaintToCanvas(ABitmap.Canvas, psBitmap, Dot(expPos.x, expPos.y));
  ABitmap.PixelFormat := PixelFormat;
end;

procedure TatDiagram.CopyBitmapToClipboard(exportSize: TExportSize = esStandard);
var
  ABitmap: TBitmap;
begin
  ABitmap := TBitmap.Create;
  try
    PaintToBitmap(ABitmap, exportSize);
    Clipboard.Assign(ABitmap);
  finally
    ABitmap.Free;
  end;
end;

procedure TatDiagram.ExportAsBMP(const fileName : String; exportSize: TExportSize);
var
  ABitmap: TBitmap;
begin
  ABitmap := TBitmap.Create;
  try
    ExportToBitmap(ABitmap, exportSize);
    ABitmap.SaveToFile(fileName);
  finally
    ABitmap.Free;
  end;
end;

procedure TatDiagram.ExportAsBMPStream(AStream: TStream; exportSize: TExportSize);
var
  ABitmap: TBitmap;
begin
  ABitmap := TBitmap.Create;
  try
    ExportToBitmap(ABitmap, exportSize);
    ABitmap.SaveToStream(AStream);
  finally
    ABitmap.Free;
  end;
end;

procedure TatDiagram.ExportToBitmap(ABitmap: TBitmap; exportSize: TExportSize);
var
  PixelFormat: TPixelFormat;
  expSize: TPoint;
  expPos: TPoint;
begin
  GetExportSizeAndPos(exportSize, expSize, expPos);
  PixelFormat := GetPixelFormat;
  ABitmap.Width := expSize.x;
  ABitmap.Height := expSize.y;
  ABitmap.PixelFormat := PixelFormat;
  FPreviewZoom := FZoomRatio;
  PaintToCanvas(ABitmap.Canvas, psBitmap, Dot(expPos.X, expPos.Y));
end;

procedure TatDiagram.CopyMetafileToClipboard(exportSize: TExportSize = esStandard);
var
  AMetaFile: TMetaFile;
  ACanvas: TCanvas;
  expSize: TPoint;
  expPos: TPoint;
begin
  GetExportSizeAndPos(exportSize, expSize, expPos);
  ACanvas := CreateMetafileCanvas(expSize.x, expSize.y);
  try
    FPreviewZoom := FZoomRatio;
    PaintToCanvas(ACanvas, psBitmap, Dot(expPos.X, expPos.Y));
  finally
    AMetaFile := DestroyMetafileCanvas(ACanvas);
  end;
  try
    Clipboard.Assign(AMetaFile);
  finally
    AMetaFile.Free;
  end;
end;

procedure TatDiagram.ExportAsWMF(fileName: string; exportSize: TExportSize = esStandard);
var
  AMetafile: TMetafile;
begin
  AMetafile := ExportToMetafile(exportSize);
  try
    AMetaFile.SaveToFile(fileName);
  finally
    AMetaFile.Free;
  end;
end;

procedure TatDiagram.ExportAsWMFStream(AStream: TStream; exportSize: TExportSize = esStandard);
var
  AMetafile: TMetafile;
begin
  AMetafile := ExportToMetafile(exportSize);
  try
    AMetaFile.SaveToStream(AStream);
  finally
    AMetaFile.Free;
  end;
end;

function TatDiagram.ExportToMetafile(exportSize: TExportSize = esStandard): TMetafile;
var
  ACanvas: TCanvas;
  expPos: TPoint;
  expSize: TPoint;
begin
  GetExportSizeAndPos(exportSize, expSize, expPos);
  ACanvas := CreateMetafileCanvas(expSize.x, expSize.y);
  try
    FPreviewZoom := FZoomRatio;
    PaintToCanvas(ACanvas, psBitmap, Dot(expPos.X, expPos.Y));
  finally
    result := DestroyMetafileCanvas(ACanvas);
  end;
end;

procedure TatDiagram.ExportAsJPG(const fileName : String; exportSize: TExportSize);
var
  ABitmap: TBitmap;
  AJpeg: TJPEGImage;
begin
  ABitmap := TBitmap.Create;
  AJpeg := TJPEGImage.Create;
  try
    ExportToBitmap(ABitmap, exportSize);
    AJpeg.Assign(ABitmap);
    AJpeg.SaveToFile(fileName);
  finally
    ABitmap.Free;
    AJpeg.Free;
  end;
end;

procedure TatDiagram.ExportAsJPGStream(AStream: TStream; exportSize: TExportSize);
var
  ABitmap: TBitmap;
  AJpeg: TJPEGImage;
begin
  ABitmap := TBitmap.Create;
  AJpeg := TJPEGImage.Create;
  try
    ExportToBitmap(ABitmap, exportSize);
    AJpeg.Assign(ABitmap);
    AJpeg.SaveToStream(AStream);
  finally
    ABitmap.Free;
    AJpeg.Free;
  end;
end;

procedure TatDiagram.GetExportSizeAndPos(exportSize: TExportSize; var ASize: TPoint; var APos: TPoint);
var
  OldPaintStyle: TPaintStyle;
begin
  case exportSize of
    esCentered:
      begin
        APos.X   := 0;
        APos.Y   := 0;
        ASize.x := GetDiagramWidth + GetDiagramLeft;
        ASize.y := GetDiagramHeight + GetDiagramTop;
      end;
    esNetto:
      begin
        APos.X   := - (GetDiagramLeft - FNettoExportOffset);
        APos.Y   := - (GetDiagramTop - FNettoExportOffset);
        ASize.x := GetDiagramWidth - GetDiagramLeft + FNettoExportOffset * 2;
        ASize.y := GetDiagramHeight - GetDiagramTop + FNettoExportOffset * 2;
      end;
    else
      begin                        
        APos.X   := 0;
        APos.Y   := 0;
        ASize.x := Max(HorzScrollBar.Range, ClientWidth);
        ASize.y := Max(VertScrollBar.Range, ClientHeight);
      end;
  end;

  OldPaintStyle := FPaintStyle;
  FPaintStyle := psBitmap;
  try
    if FZoomRatio >=1 then
      ASize := ClientToCanvas(ASize);
  finally
    FPaintStyle := OldPaintStyle;
  end;
end;

procedure TatDiagram.ExportToFile(const fileName : String; exportFormat : TExportFormat = efAuto; exportSize : TExportSize = esCentered);
begin
   if (exportFormat = efAuto) Then
   begin
      IF (AnsiCompareText('.wmf', ExtractFileExt(fileName)) = 0) Then
      begin
         exportFormat := efWMF;
      end
      else if (AnsiCompareText('.emf', ExtractFileExt(fileName)) = 0) Then
      begin
         exportFormat := efWMF;
      end
      else begin
         exportFormat := efBMP;
      end;
   end;

   Case exportFormat of
      efBMP : ExportAsBMP(fileName, exportSize);
      efWMF : ExportAsWMF(fileName, exportSize);
   end;
end;

function TatDiagram.PrintClientWidth: number;
var
  Page: TSquare;
begin
  Page := PageSettings.PrintPageRect;
  result := Page.Right - Page.Left;
end;

function TatDiagram.PrintClientHeight: number;
var
  Page: TSquare;
begin
  Page := PageSettings.PrintPageRect;
  result := Page.Bottom - Page.Top;
end;

function TatDiagram.GetPageColCount: integer;
begin
  UpdatePixelConvertion;
  result := Trunc(Round(DiagramWidth * FPrintConvX) / PrintClientWidth) + 1;
end;

function TatDiagram.GetPageCount: integer;
begin
  result := PageColCount * PageRowcount;
end;

function TatDiagram.GetPageRowCount: integer;
begin
  UpdatePixelConvertion;
  result := Trunc(Round(DiagramHeight * FPrintConvY) / PrintClientHeight) + 1;
end;

function TatDiagram.GetPaintRect(ACanvas: TCanvas): TRect;
begin
  case FPaintStyle of
    psPrint, psPreview:
      result := ToRect(Square(0, 0, MilimetersToClient(PageSettings.PaperWidth), MilimetersToClient(PageSettings.PaperHeight)));
    psBitmap:
      result := ACanvas.ClipRect;
    else // psScreen
      result := ClientRect;
  end;
end;

function TatDiagram.PageOffset(Idx: integer): TDot;
var
  ColCount: integer;
  Col, Row: integer;
begin
  UpdatePixelConvertion;
  ColCount := trunc(round(DiagramWidth * FPrintConvX) / PrintClientWidth) + 1;
  Row := (Idx - 1) div ColCount;
  Col := (Idx - 1) - (Row * ColCount);
  result.X := -Col * PrintClientWidth;
  result.Y := -Row * PrintClientHeight;
end;

function TatDiagram.CreateMetaFileCanvas(AWidth, AHeight: integer): TCanvas;
var
  RefDC: HDC;
  R: TRect;
begin
  result := TCanvas.Create;
  RefDC := GetDC(0);
  try
    R.Left := 0;
    R.Top := 0;
    {Convert pixels to 0.1 mm coordinates}
    R.Right := Round(AWidth * GetDeviceCaps(RefDC, HORZSIZE) * 100 /
      GetDeviceCaps(RefDC, HORZRES));
    R.Bottom := Round(AHeight * GetDeviceCaps(RefDC, VERTSIZE) * 100 /
      GetDeviceCaps(RefDC, VERTRES));
    {$WARNINGS OFF}
    result.Handle := CreateEnhMetaFile(RefDC, nil, @R, nil);
    {$WARNINGS ON}
  finally
    ReleaseDC(0, RefDC);
  end;
  Windows.SetMapMode(result.Handle, MM_TEXT);
end;

function TatDiagram.DestroyMetaFileCanvas(ACanvas: TCanvas): TMetaFile;
var
  Temp: integer;
begin
  Temp := ACanvas.Handle;
  ACanvas.Handle := 0;
  Result := TMetaFile.Create;
  if Assigned(Result) then
    Result.Handle := CloseEnhMetaFile(Temp);
  ACanvas.Free;
end;

function TatDiagram.DgrPaintEngineClass: TDgrPaintEngineClass;
begin
  result := nil;
  case GraphicLib of
    dglGDIPlus:
      begin
        {$IFDEF GDIPLUS}
        result := TGdipPaintEngine;
        {$ENDIF}
      end;
    {$IFDEF DELPHI2010_LVL}
    dglDirect2D:
      result := TD2DPaintEngine;
    {$ENDIF}
  end;
end;

function TatDiagram.BuildPreviewBitmap(APageIdx: integer; AWidth: integer; AHeight: integer = 0): TBitmap;
var
  P: TDot;
  R: TRect;
  MarginRect: TRect;
  ABitCanvas: TCanvas;
  ABitmap: TBitmap;
  AOldZoomRatio: double;
  PageRect: TSquare;
  PaperSize: TPoint;
begin
  result := nil;
  AOldZoomRatio := FZoomRatio;
  FZoomRatio := 1;
  ABitmap := TBitmap.Create;
  try
    try
      PageRect := PageSettings.PrintPageRect;
      PaperSize := PageSettings.PrintPaperSize;
      if (PaperSize.X = 0) or (PaperSize.Y = 0) then
        Exit;

      {Adjust ratios for shrinking bitmap previewing}
      if AWidth = 0 then
        AWidth := PaperSize.X;
      if AHeight = 0 then
        AHeight := PaperSize.Y;
      FAdjustRatioX := AWidth / PaperSize.X;
      FAdjustRatioY := AHeight / PaperSize.Y;

      ABitmap.Width := AWidth;
      ABitmap.Height := AHeight;

      ABitCanvas := ABitmap.Canvas;

      P := PageOffset(APageIdx);
      P := AddPointX(P, PageRect.TopLeft);
      PaintToCanvas(ABitCanvas, psPreview, P);

      {Paint the margins again}
      ABitCanvas.Brush.Style := bsSolid;
      ABitCanvas.Brush.Color := clWhite;
      MarginRect := ToRect(Square(PageRect.Left * FAdjustRatioX, PageRect.Top * FAdjustRatioY,
        PageRect.Right * FAdjustRatioX, PageRect.Bottom * FAdjustRatioY));

      R := Rect(0, 0, MarginRect.Left + 1, AHeight + 1);
      ABitCanvas.FillRect(R);
      R := Rect(0, 0, AWidth + 1, MarginRect.Top);
      ABitCanvas.FillRect(R);
      R := Rect(MarginRect.Right, 0, AWidth, AHeight);
      ABitCanvas.FillRect(R);
      R := Rect(0, MarginRect.Bottom, AWidth, AHeight);
      ABitCanvas.FillRect(R);

      Result := ABitmap;
    finally
      FZoomRatio := AOldZoomRatio;
      FAdjustRatioX := 1;
      FAdjustRatioY := 1;
    end;
  except
    ABitmap.Free;
    raise;    
  end;
end;

procedure ExtractPageNumbers(S: string; AList: TStringList; ATotal: integer);
var
  c: integer;
  p: integer;
  n1, n2: integer;
  i: integer;
begin
  AList.CommaText := S;
  c := 0;
  while c < AList.Count do
  begin
    p := Pos('-', AList[c]);
    if p > 0 then
    begin
      if p = 1 then
        n1 := 1
      else
        n1 := StrToInt(Copy(AList[c], 1, p - 1));

      if p = length(AList[c]) then
        n2 := ATotal
      else
        n2 := StrToInt(Copy(AList[c], p + 1, MaxInt));

      AList.Delete(c);
      for i := n1 to n2 do
        AList.Add(IntToStr(i));
    end else
      inc(c);
  end;
end;

function TatDiagram.Print(AShowDialog: boolean = False): boolean;
var
  c: integer;
  cop: integer;
  P: TDot;
  PageRect: TSquare;
  PD: TfmDgrPrintDialog;
  APageList: TStringList;
  ACopies: integer;
  APageNumbers: string;
  AOldZoomRatio: double;
  TempGraphicLib: TDgrGraphicLib;
begin
  if not DPrinters.HasPrinterInstalled then
  begin
    ShowMessage(SNoDefaultPrinter);
    result := false;
    exit;
  end;

  result := true;

  ACopies := PrintSettings.Copies;
  APageNumbers := PrintSettings.PageNumbers;

  DPrinter.SetPrintParams(PageSettings.PaperId, PageSettings.PaperWidth,
    PageSettings.PaperHeight, PageSettings.Orientation);

  {Show printer dialog}
  if AShowDialog then
  begin
    PD := TfmDgrPrintDialog.Create(nil);
    try
      PD.UpDown1.Position := ACopies;
      PD.edPageNumbers.Text := APageNumbers;
      if APageNumbers <> '' then
        PD.rbPageNumbers.Checked := true
      else
        PD.rbAllPages.Checked := true;

      result := (PD.ShowModal = mrOk);

      if result then
      begin
        ACopies := StrToInt(PD.edCopies.Text);
        if PD.rbAllPages.Checked then
          APageNumbers := ''
        else
          APageNumbers := PD.edPageNumbers.Text;
      end;
    finally
      PD.Free;
    end;
  end;

  if not result then
    Exit;

  {Force the page size/orientation for the page settings defined in diagram}
  DPrinter.SetPrintParams(PageSettings.PaperId, PageSettings.PaperWidth,
    PageSettings.PaperHeight, PageSettings.Orientation);

  DPrinter.Title := PrintSettings.Title;
  if DPrinter.Title = '' then
    DPrinter.Title := 'Diagram';

  APageList := TStringList.Create;
  TempGraphicLib := GraphicLib;
  {$IFDEF DELPHI2010_LVL}
  if GraphicLib = dglDirect2D then
    GraphicLib := dglGdiPlus;
  {$ENDIF}
  try
    if DPrinter <> nil then
    begin
      ExtractPageNumbers(APageNumbers, APageList, GetPageCount);
      FPreviewZoom := 1;
      PageRect := PageSettings.PrintPageRect;
      DPrinter.BeginDoc;
      AOldZoomRatio := FZoomRatio;
      FZoomRatio := 1;
      try
        for cop := 1 to ACopies do
        begin
          for c := 1 to GetPageCount do
            if (APageList.Count = 0) or (APageList.IndexOf(IntToStr(c)) >= 0) then
            begin
              DPrinter.BeginPage;
              FPrintClipRect := ToRect(PageRect);
              IntersectClipRect(DPrinter.FCanvas.Handle, FPrintClipRect.Left, FPrintClipRect.Top,
                FPrintClipRect.Right, FPrintClipRect.Bottom);
              P := PageOffset(c);
              P := AddPointX(P, PageRect.TopLeft);
              PaintToCanvas(DPrinter.FCanvas, psPrint, P);
              DPrinter.EndPage;
            end;
        end;
      finally
        FZoomRatio := AOldZoomRatio;
      end;
      DPrinter.EndDoc;
    end;
  finally
    APageList.Free;
    GraphicLib := TempGraphicLib;
  end;
end;

procedure TatDiagram.SetShowLinkPoints(const Value: boolean);
begin
  if FShowLinkPoints <> Value then
  begin
    FShowLinkPoints := Value;
    Redraw;
  end;
end;

procedure TatDiagram.BeginUpdate;
begin
  if FUpdateCount = 0 then
    FRedrawRequestCount := 0;
  Inc(FUpdateCount);
end;

function TatDiagram.EndUpdate(AOptimize: boolean = False): integer;
begin
  result := -1;
  Dec(FUpdateCount);
  if FUpdateCount <= 0 then
  begin
    FUpdateCount := 0;
    CalcPanelSize;
    if AOptimize then
    begin
      result := FRedrawRequestCount;
      if FRedrawRequestCount > 0 then
        Redraw;
      FRedrawRequestCount := 0;
    end
    else
      Redraw;
  end;
end;

procedure TatDiagram.DoDControlClick(ADControl: TDiagramControl);
begin
  ADControl.Click;
  if Assigned(FOnDControlClick) then
    FOnDControlClick(Self, ADControl);
end;

procedure TatDiagram.DoDControlMouseDown(ADControl: TDiagramControl;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  ADControl.MouseDown(Button, Shift, X, Y);
  if Assigned(FOnDControlMouseDown) then
    FOnDControlMouseDown(Self, ADControl, Button, Shift, X, Y);
end;

procedure TatDiagram.DoDControlMouseEnter(ADControl: TDiagramControl);
begin
  if ADControl is TCustomDiagramBlock then
  begin
    TCustomDiagramBlock(ADControl).MouseEnter;
    if Assigned(FOnBlockMouseEnter) then
      FOnBlockMouseEnter(Self, TCustomDiagramBlock(ADControl));
  end;
end;

procedure TatDiagram.DoDControlMouseLeave(ADControl: TDiagramControl);
begin
  if ADControl is TCustomDiagramBlock then
  begin
    TCustomDiagramBlock(ADControl).MouseLeave;
    if Assigned(FOnBlockMouseLeave) then
      FOnBlockMouseLeave(Self, TCustomDiagramBlock(ADControl));
  end;
end;

procedure TatDiagram.DoDControlMouseMove(ADControl: TDiagramControl;
  Shift: TShiftState; X, Y: Integer);
begin
  ADControl.MouseMove(Shift, X, Y);
  if Assigned(FOnDControlMouseMove) then
    FOnDControlMouseMove(Self, ADControl, Shift, X, Y);
end;

procedure TatDiagram.DoDControlMouseUp(ADControl: TDiagramControl;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  ADControl.MouseUp(Button, Shift, X, Y);
  if Assigned(FOnDControlMouseUp) then
    FOnDControlMouseUp(Self, ADControl, Button, Shift, X, Y);
end;

procedure TatDiagram.DoDControlDblClick(ADControl: TDiagramControl);
begin
  ADControl.DblClick;
  if Assigned(FOnDControlDblClick) then
    FOnDControlDblClick(Self, ADControl);
end;

procedure TatDiagram.SelectControlsInRect(ASelRect: TSquare);
var
  c: integer;
  d: integer;
  R1, R2: HRGN;
  RgnResult: integer;
  Pts: TPointArray;
  region: TDgrRegion;
  path: TDgrGraphicsPath;
begin
  SetLength(Pts, 0);
  BeginUpdateSel;
  try
    {when checking for blocks in selection rectangle, ignore groups, because we want
     to "select" only the effective block members. Groups will be selected automatically if a member is selected}
    for c := 0 to BlockCount - 1 do
      if Blocks[c].IsVisible and not Blocks[c].IsGroup then
      begin
        {Do a different test when the block is gdiplus-based. Check if GetSurroundRgn is empty.
         If yes, then use block paths to check the block region}
        Pts := RoundDotArray(Blocks[c].GetSurroundRgn);
        if Length(Pts) = 0 then
        begin
          if PathDrawingMode then
          begin
            path := Blocks[c].FDgrDrawer.CreatePath;
            try
              Blocks[c].InitDgrDrawer(Blocks[c].FDgrDrawer);
              Blocks[c].GetSurroundBlockPath(path, Blocks[c].FDgrDrawer);
              region := path.CreateRegion; //TGPRegion.Create(path.GPPath);
              try
                if region.IsVisible(ASelRect) then
                begin
                  if Blocks[c].IsMember then
                    Blocks[c].MasterGroup.Selected := True
                  else
                    Blocks[c].Selected := True;
                end;
              finally
                region.Free;
              end;
            finally
              path.Free;
            end;
          end;
        end
        else
        begin
          R1 := CreateRectRgn(Round(ASelRect.Left), Round(ASelRect.Top),
            Round(ASelRect.Right), Round(ASelRect.Bottom));
          R2 := CreatePolygonRgn(Pts[0], Length(Pts), ALTERNATE);
          RgnResult := CombineRgn(R1, R1, R2, RGN_AND);
          if RgnResult <> NULLREGION then
          begin
            if Blocks[c].IsMember then
              Blocks[c].MasterGroup.Selected := True
            else
              Blocks[c].Selected := True;
          end;
          DeleteObject(R1);
          DeleteObject(R2);
        end;
      end;
    for c := 0 to LinkCount - 1 do
      if Links[c].IsVisible then
        for d := 0 to Length(Links[c].FPoints) - 1 do
          if PtInRect(ToRect(ASelRect), RoundPoint(Links[c].FPoints[d])) then
          begin
            if Links[c].IsMember then
              Links[c].MasterGroup.Selected := true
            else
              Links[c].Selected := true;
            break;
          end;
  finally
    EndUpdateSel;
  end;
end;

procedure TatDiagram.SetBackground(const Value: TDiagramBackground);
begin
  FBackground.Assign(Value);
  Redraw;
end;

procedure TatDiagram.SetLeftRuler(const Value: TDiagramRuler);
begin
  FLeftRuler.Assign(Value);
end;

procedure TatDiagram.SetTopRuler(const Value: TDiagramRuler);
begin
  FTopRuler.Assign(Value);
end;

procedure TatDiagram.WMHScroll(var Message: TWMHScroll);
begin
  if (Message.ScrollBar = 0) and FHorzScrollBar.Visible then
  begin
    FHorzScrollBar.ScrollMessage(Message);
    {if FLeftRuler.Visible or (FBackGround.Visible and not FBackground.Scroll) then}
      Invalidate;
  end;
end;

procedure TatDiagram.WMVScroll(var Message: TWMVScroll);
begin
  if (Message.ScrollBar = 0) and FVertScrollBar.Visible then
  begin
    FVertScrollBar.ScrollMessage(Message);
    {if FTopRuler.Visible or (FBackground.Visible and not FBackGround.Scroll) then}
      Invalidate;
  end;
end;

procedure TatDiagram.CreateParams(var Params: TCreateParams);
const
  BorderStyles: array[TBorderStyle] of DWORD = (0, WS_BORDER);
begin
  inherited CreateParams(Params);
  with Params do
  begin
    Style := Style or WS_CLIPCHILDREN or WS_HSCROLL or WS_VSCROLL or
      BorderStyles[FBorderStyle];
    if NewStyleControls and Ctl3D and (FBorderStyle = bsSingle) then
    begin
      Style := Style and not WS_BORDER;
      ExStyle := ExStyle or WS_EX_CLIENTEDGE;
    end;
  end;
end;

procedure TatDiagram.CreateWnd;
begin
  inherited CreateWnd;
  if not SysLocale.MiddleEast
    {$IFDEF DELPHI7_LVL}and not CheckWin32Version(5, 1){$ENDIF} then
    InitializeFlatSB(Handle);
  UpdateScrollBars;
end;

procedure TatDiagram.UpdateScrollBars;
begin
  if not FUpdatingScrollBars and HandleAllocated then
    try
      FUpdatingScrollBars := True;
      if FVertScrollBar.NeedsScrollBarVisible then
      begin
        FHorzScrollBar.Update(False, True);
        FVertScrollBar.Update(True, False);
      end
      else if FHorzScrollBar.NeedsScrollBarVisible then
      begin
        FVertScrollBar.Update(False, True);
        FHorzScrollBar.Update(True, False);
      end
      else
      begin
        FVertScrollBar.Update(False, False);
        FHorzScrollBar.Update(True, False);
      end;
    finally
      FUpdatingScrollBars := False;
    end;
end;

procedure TatDiagram.UpdateSelectionBlock;
var
  SelRect: TSquare;
  AMargin: integer;
begin
  if GroupSelection then
  begin
    {Calculate AMargin based on canvas points, so the margin is always the same
     regardless of the zoom factor}
    //AMargin := Abs(CanvasToClient(Point(10, 10)).Y - CanvasToClient(Point(0, 0)).Y);
    AMargin := 0;

    {Calculate surround rect, to calculate center point. Then calculate
     rect rotated}
    SelRect := SelectionRect(false, FSelBlock.Angle);

    {Set fields directly, otherwise Modified and Redraw will be called}
    FSelBlock.FLeft := SelRect.Left - AMargin;
    FSelBlock.FTop := SelRect.Top - AMargin;
    FSelBlock.FWidth := SelRect.Right - SelRect.Left + AMargin + AMargin;
    FSelBlock.FHeight := SelRect.Bottom - SelRect.Top + AMargin + AMargin;
  end;
end;

procedure TatDiagram.SetBorderStyle(const Value: TBorderStyle);
begin
  if Value <> FBorderStyle then
  begin
    FBorderStyle := Value;
    RecreateWnd;
  end;
end;

procedure TatDiagram.SetCustomGroupBlockClass(const Value: TGroupBlockClass);
begin
  FCustomGroupBlockClass := Value;
  if (FCustomGroupBlockClass <> nil) and (GetClass(FCustomGroupBlockClass.ClassName) = nil) then
    RegisterClass(FCustomGroupBlockClass);
end;

procedure TatDiagram.SetCustomDiagramMemoClass(const Value: TDiagramMemoClass);
begin
  FCustomDiagramMemoClass := Value;
end;

procedure TatDiagram.CMCtl3DChanged(var Message: TMessage);
begin
  if NewStyleControls and (FBorderStyle = bsSingle) then RecreateWnd;
  inherited;
end;

function TatDiagram.RulerAtPos(CP: TDot): TDiagramRuler;
begin
  if CP.Y <= FTopRuler.VisibleSize then
    result := FTopRuler
  else
  if CP.X <= FLeftRuler.VisibleSize then
    result := FLeftRuler
  else
    result := nil;
end;

procedure TatDiagram.SetZoom(const Value: integer);
begin
  if GetZoom <> Value then
  begin
    FZoomRatio := Value / 100;
    if FZoomRatio < 0.01 then
      FZoomRatio := 0.01;
    if FZoomRatio > 10 then
      FZoomRatio := 10;
    FTopRuler.Zoom := Trunc(ZoomRatio * 100);
    FLeftRuler.Zoom := FTopRuler.Zoom;
    CalcPanelSize;
    Redraw;
  end;
end;

function TatDiagram.GetZoom: integer;
begin
  result := Trunc(FZoomRatio * 100);
end;

function TatDiagram.GetZoomRatio: double;
begin
  case FPaintStyle of
    psPreview, psPrint:
      result := FPreviewZoom;
  else
    result := FZoomRatio;
  end;
end;

procedure TatDiagram.Preview;
var
  PreviewForm: TfmDiagramPreview;
begin                           
  PreviewForm := TfmDiagramPreview.Create(Application);
  try
    PreviewForm.Diagram := Self;
    PreviewForm.ShowModal;
  finally
    PreviewForm.Free;
  end;
end;

function TatDiagram.AreBlocksLinked(ABlock1,
  ABlock2: TCustomDiagramBlock): boolean;
begin
  result := (LinkBetweenBlocks(ABlock1, ABlock2) <> nil);
end;

function TatDiagram.LinkBetweenBlocks(ABlock1,
  ABlock2: TCustomDiagramBlock): TCustomDiagramLine;
var
  c: integer;
begin
  result := nil;
  if (ABlock1 <> nil) and (ABlock2 <> nil) then
    for c := 0 to LinkCount - 1 do
      With Links[c] do
        if ((SourceLinkPoint.Anchor = ABlock1) and (TargetLinkPoint.Anchor = ABlock2)) or
           ((SourceLinkPoint.Anchor = ABlock2) and (TargetLinkPoint.Anchor = ABlock1)) then
        begin
          result := Links[c];
          break;
        end;
end;

procedure TatDiagram.DoLinkAdded(ADControl: TDiagramControl;
  ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);
begin
  if Assigned(FOnLinkAdded) then
    FOnLinkAdded(Self, ADControl, ALink, ALinkPoint);
end;

procedure TatDiagram.DoLinkRemoved(ADControl: TDiagramControl;
  ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);
begin
  if not (csDestroying in ComponentState) and Assigned(FOnLinkRemoved) then
    FOnLinkRemoved(Self, ADControl, ALink, ALinkPoint);
end;

procedure TatDiagram.CMFontChanged(var Message: TMessage);
var
  c: integer;
begin
  BeginUpdate;
  try
    for c := 0 to DControlCount - 1 do
      DControls[c].DiagramFontChanged;
  finally
    EndUpdate;
  end;
end;

procedure TatDiagram.BringToFront(ADControl: TDiagramControl);
var
  i: integer;
  GM: TDControlListItem;
begin
  if ADControl.IsMember then
  begin
    GM := ADControl.GroupBlock.Members.FindByDControl(ADControl);
    if GM <> nil then
      GM.Index := ADcontrol.GroupBlock.Members.Count - 1;
  end
  else
  begin
    i := FDControls.IndexOf(ADControl);
    if i <> -1 then
      FDControls.Move(i, FDControls.Count - 1);
  end;
  Redraw;
end;

procedure TatDiagram.SendToBack(ADControl: TDiagramControl);
var
  i: integer;
  GM: TDControlListItem;
begin
  if ADControl.IsMember then
  begin
    GM := ADControl.GroupBlock.Members.FindByDControl(ADControl);
    if GM <> nil then
      GM.Index := 0;
  end
  else
  begin
    i := FDControls.IndexOf(ADControl);
    if i <> -1 then
      FDControls.Move(i, 0);
  end;
  Redraw;
end;

procedure TatDiagram.BringToFront;
var
  c: integer;
begin
  for c := 0 to SelectedCount - 1 do
    if not Selecteds[c].AsMember then
      BringToFront(Selecteds[c]);
end;

procedure TatDiagram.SendToBack;
var
  c: integer;
begin
  for c := 0 to SelectedCount - 1 do
    if not Selecteds[c].AsMember then
      SendToBack(Selecteds[c]);
end;

function TatDiagram.UpdateControlDrawingInfo(X, Y: number; AAction: TControlDrawAction): TControlDrawInfo;
var ControlSearchMode: TControlSearchMode;
    ILControl1, ILControl2: TCustomDiagramBlock;
begin
  FControlDrawInfo.Diagram := Self;
  FControlDrawInfo.Canvas := FControlCanvas;
  FControlDrawInfo.AnchorPoint1 := FLinkPoint;
  FControlDrawInfo.Action := AAction;
  FControlDrawInfo.P1 := CanvasToClient(FDragStart);
  FControlDrawInfo.AnchorPoint2 := LinkPointAtPos(CanvasToClient(Dot(X, Y)), nil, false, false, true);

  // added stuff to be able to create "drawing" lines from one control to another
  // find the control we're over. Is this lines wants implicit linkpoints, ignore Line controls to be returned
  if (FInsertDControl is TCustomDiagramLine) and
     (TCustomDiagramLine(FInsertDControl).LinkPointStyle = ptImplicit) then
    ControlSearchMode := csmIgnoreLines
  else
    ControlSearchMode := csmAll;
  FControlDrawInfo.DControl     := DControlAtPos(CanvasToClient(Dot(X, Y)),
                                                 ControlSearchMode, cfIgnoreGroups
                                                );
  Case AAction of
    daDrawCursor, daStep, daInserted:
      FControlDrawInfo.P2 := CanvasToClient(SnapCanvasPoint(Dot(X, Y)))
  end;

  // add an implicit linkpoint for controls that support it
  if (AAction = daInserted) and Assigned(FControlDrawInfo.DControl) and FControlDrawInfo.DControl.AutoCreateLinkPoints then
  begin
    ILControl1 := FControlDrawInfo.AnchorPoint1.DControl as TCustomDiagramBlock;
    ILControl2 := FControlDrawInfo.DControl as TCustomDiagramBlock;
    FControlDrawInfo.AnchorPoint2 := ILControl2.LinkPoints.Add;
    FControlDrawInfo.AnchorPoint2.Anchor := ILControl2;
    FControlDrawInfo.AnchorPoint1.Anchor := ILControl1;
    FControlDrawInfo.AnchorPoint2.OrPoint := ILControl2.Drawer.UnPP(FControlDrawInfo.P2);
    FControlDrawInfo.AnchorPoint1.OrPoint := ILControl1.Drawer.UnPP(FControlDrawInfo.P1);
  end;

  result := FControlDrawInfo;
end;

procedure TatDiagram.DrawingStep(XPos, YPos: number);
var
  AInfo: TControlDrawInfo;
  Ignore: boolean;
begin
  if FInsertDControl <> nil then
  begin
    AInfo := UpdateControlDrawingInfo(XPos, YPos, daEraseCursor);
    Ignore := AroundPoint(AInfo.P1, AInfo.P2);
    if not Ignore then
    begin                            
      if not IsDummyPoint(FOldDragPos) then
        FInsertDControl.UpdateDrawing(AInfo);

      AInfo := UpdateControlDrawingInfo(XPos, YPos, daStep);
      FInsertDControl.UpdateDrawing(AInfo);
      FOldDragPos := DummyPointX;
      FDragStart := SnapCanvasPoint(Dot(XPos, YPos));
    end;
  end;
end;

procedure TatDiagram.EndDrawing(XPos, YPos: number; Cancel: boolean);
begin
  if FInsertDControl <> nil then
  begin
    if not IsDummyPoint(FOldDragPos) then
      FInsertDControl.UpdateDrawing(UpdateControlDrawingInfo(XPos, YPos, daEraseCursor));
    if Cancel then
      DestroyInsertControl
    else
    begin
      FInsertDControl.UpdateDrawing(UpdateControlDrawingInfo(XPos, YPos, daInserted));
      FInsertDControl.Diagram := Self;
      FInsertDControl.Selected := True;
      PushUndoStack('insert');
    end;
  end;
  FOldDragPos := DummyPointX;
  FInsertDControl := nil;
  FMouseState := msBrowsing;
  CalcPanelSize;
  Redraw;
  Screen.Cursor := FOldScreenCursor;
end;

procedure TatDiagram.UpdateDrawing(XPos, YPos: number);
begin
  if FInsertDControl <> nil then
  begin
    if not IsDummyPoint(FOldDragPos) then
      FInsertDControl.UpdateDrawing(UpdateControlDrawingInfo(XPos, YPos, daEraseCursor));
    FInsertDControl.UpdateDrawing(UpdateControlDrawingInfo(XPos, YPos, daDrawCursor));
  end;
  FOldDragPos := Dot(XPos, YPos);
end;

procedure TatDiagram.StartDrawing(XPos, YPos: number);
begin
  if FInsertDControl <> nil then
    FInsertDControl.UpdateDrawing(UpdateControlDrawingInfo(XPos, YPos, daStart));
end;

procedure TatDiagram.CMHintShow(var Msg: TMessage);
var
  HInfo: PHintInfo;
  CanShow: boolean;
  DControl: TDiagramControl;
begin
  {$WARNINGS OFF}
  HInfo := TCMHintShow(Msg).HintInfo;
  {$WARNINGS ON}
  DControl := DControlAtPos(CanvasToClient(Dot(HInfo^.CursorPos)), csmAll, cfIgnoreGroups);
  CanShow := (DControl <> nil) and DControl.ShowHint;
  if CanShow then
  begin
    HInfo^.HintStr := DControl.Hint;
    HInfo^.CursorRect := ToRect(ClientToCanvas(DControl.SurroundRect));
  end;
  Msg.Result := Ord(not CanShow);
end;

procedure TatDiagram.CMShowHintChanged(var Message: TMessage);
var
  c: integer;
begin
  for c := 0 to DControlCount - 1 do
    DControls[c].DiagramShowHintChanged;
end;

procedure TatDiagram.ClipRegion(Canvas: TCanvas; ARgn: HRGN);
var
  Org: TPoint;
begin
  SelectClipRgn(Canvas.Handle, 0);
  GetWindowOrgEx(Canvas.Handle, Org);
  OffsetRgn(ARgn, -Org.X, -Org.Y);

  SelectClipRgn(Canvas.Handle, ARgn);

  {printing without gdiplus sometimes loses the intersectcliprect of printer printing
   (for margin) so we need to always reinforce the cliprect for margins}
  {$IFNDEF GDIPLUS}
  if FPaintStyle = psPrint then
    IntersectClipRect(Canvas.Handle, FPrintClipRect.Left, FPrintClipRect.Top,
      FPrintClipRect.Right, FPrintClipRect.Bottom);
  {$ENDIF}
end;

procedure TatDiagram.UnclipRegion(Canvas: TCanvas);
begin
  SelectClipRgn(Canvas.Handle, 0);

  {printing without gdiplus sometimes loses the intersectcliprect of printer printing
   (for margin) so we need to always reinforce the cliprect for margins}
  {$IFNDEF GDIPLUS}
  if FPaintStyle = psPrint then
    IntersectClipRect(Canvas.Handle, FPrintClipRect.Left, FPrintClipRect.Top,
      FPrintClipRect.Right, FPrintClipRect.Bottom);
  {$ENDIF}

  {With FOldClipRect do
    IntersectClipRect(Canvas.Handle, Left, Top, Right, Bottom);}
end;

function TatDiagram.SelectedBlockCount(AFilter: TDiagramControlFilter = cfAll): integer;
var
  c: integer;
begin
  result := 0;
  for c := 0 to BlockCount - 1 do
    if Blocks[c].Selected then
    begin
      Case AFilter of
        cfIgnoreAsMembers:
          if not Blocks[c].AsMember then
            result := result + 1;
        cfIgnoreMembers:
          if not Blocks[c].IsMember then
            result := result + 1;
        cfIgnoreGroups:
          if not Blocks[c].IsGroup then
            result := result + 1;
      else
        {cfAll: }
        result := result + 1;
      end;
    end;
end;

function TatDiagram.SelectedLinkCount: integer;
var
  c: integer;
begin
  result := 0;
  for c := 0 to LinkCount - 1 do
    if Links[c].Selected then
      result := result + 1;
end;

procedure TatDiagram.WMNCPaint(var Message: TMessage);
var
  ACanvas: TCanvas;
  ADC: HDC;
begin
  inherited;
  if (not Ctl3D) and (BorderStyle = bsSingle) then
  begin
    ADC := GetWindowDC(Handle);
    ACanvas := TCanvas.Create;
    try
      ACanvas.Handle := ADC;
      ACanvas.Brush.Style := bsClear;
      ACanvas.Pen.Style := psSolid;
      ACanvas.Pen.Color := FBorderColor;
      ACanvas.Rectangle(0, 0, Width, Height);
    finally
      ACanvas.Free;
      ReleaseDC(Handle, ADC);
    end;
  end;
end;

procedure TatDiagram.WMPaint(var Message: TWMPaint);
var
  DC, MemDC: HDC;
  MemBitmap, OldBitmap: HBITMAP;
  PS: TPaintStruct;
  {$IFDEF DELPHI2007_LVL}
  PaintBuffer: HPAINTBUFFER;
  {$ENDIF}
begin
  if not FDoubleBuffered or (Message.DC <> 0) then
  begin
    if not (csCustomPaint in ControlState) and (ControlCount = 0) then
      inherited
    else
      PaintHandler(Message);
  end
  else
  begin
    {$IFDEF DELPHI2007_LVL}
    if DwmCompositionEnabled then
    begin
      DC := BeginPaint(Handle, PS);
      try
        PaintBuffer := BeginBufferedPaint(DC, PS.rcPaint, BPBF_COMPOSITED, nil, MemDC);
        if PaintBuffer <> 0 then
          try
            Perform(WM_ERASEBKGND, MemDC, MemDC);
            Perform(WM_PRINTCLIENT, MemDC, PRF_CLIENT);
            BufferedPaintMakeOpaque(PaintBuffer, @PS.rcPaint);
          finally
            EndBufferedPaint(PaintBuffer, True);
          end;
      finally
        EndPaint(Handle, PS);
      end;
    end
    else
    {$ENDIF}
    begin
      DC := BeginPaint(Handle, PS);
      MemBitmap := CreateCompatibleBitmap(DC, PS.rcPaint.Right,
        PS.rcPaint.Bottom);
      {MemBitmap := CreateCompatibleBitmap(DC, PS.rcPaint.Right - PS.rcPaint.Left,
        PS.rcPaint.Bottom - PS.rcPaint.Top);}
      MemDC := CreateCompatibleDC(DC);
      try
        OldBitmap := SelectObject(MemDC, MemBitmap);
        try
          //SetWindowOrgEx(MemDC, PS.rcPaint.Left, PS.rcPaint.Top, nil);
          Perform(WM_ERASEBKGND, MemDC, MemDC);
          Message.DC := MemDC;
          WMPaint(Message);
          Message.DC := 0;
          BitBlt(DC, PS.rcPaint.Left, PS.rcPaint.Top,
            PS.rcPaint.Right - PS.rcPaint.Left,
            PS.rcPaint.Bottom - PS.rcPaint.Top,
            MemDC,
            PS.rcPaint.Left, PS.rcPaint.Top,
            SRCCOPY);
        finally
          SelectObject(MemDC, OldBitmap);
        end;
      finally
        EndPaint(Handle, PS);
        DeleteDC(MemDC);
        DeleteObject(MemBitmap);
      end;
    end;
  end;
end;

procedure TatDiagram.SetBorderColor(const Value: TColor);
begin
  if FBorderColor <> Value then
  begin
    FBorderColor := Value;
    RedrawWindow(Handle, nil, 0, RDW_INVALIDATE or RDW_FRAME or RDW_ERASE);
  end;
end;

procedure TatDiagram.CancelPanning;
begin
  FMouseState := msBrowsing;
  Screen.Cursor := crDefault;
end;

procedure TatDiagram.StartPanning;
begin
  FMouseState := msToPan;
  Screen.Cursor := FPanCursor;
end;

procedure TatDiagram.SetAutoScroll(const Value: boolean);
begin
  if FAutoScroll <> Value then
  begin
    FAutoScroll := Value;
    if Value then
    begin
      CalcPanelSize;
      Redraw;
    end else
    begin
      HorzScrollBar.Range := 0;
      VertScrollBar.Range := 0;
    end;
  end;
end;

procedure TatDiagram.SetHorzScrollBar(const Value: TDiagramScrollBar);
begin
  FHorzScrollBar.Assign(Value);
end;

procedure TatDiagram.SetVertScrollBar(const Value: TDiagramScrollBar);
begin
  FVertScrollBar.Assign(Value);
end;

function TatDiagram.AcceptAnchor(ALink, AAnchor: TLinkPoint): boolean;
begin
  result := true;
  if Assigned(ALink) and Assigned(AAnchor) and (ALink.DControl is TCustomDiagramLine) then
  begin
    Case AAnchor.LinkConstraint of
      lcNone:
        result := false;
      lcSourceOnly:
        result := ALink = TCustomDiagramLine(ALink.DControl).SourceLinkPoint;
      lcTargetOnly:
        result := ALink = TCustomDiagramLine(ALink.DControl).TargetLinkPoint;
    else
      {lcAll}
      result := true;
    end;
    result := result and (ALink.DControl <> nil) and (AAnchor.DControl <> nil)
      and ALink.DControl.CanLink and AAnchor.DControl.CanLink;
  end;
  if Assigned(FOnAcceptAnchor) and (ALink <> nil) and (AAnchor <> nil) then
    FOnAcceptAnchor(Self, ALink, AAnchor, result);

end;

function TatDiagram.AcceptLink(ALink: TCustomDiagramLine; Anchor1, Anchor2: TLinkPoint): boolean;
begin
  result := True;
  if Assigned(FOnAcceptLink) and Assigned(ALink) and ((Anchor1 <> nil) or (Anchor2 <> nil)) then
    FOnAcceptLink(Self, ALink, Anchor1, Anchor2, result);
end;

function TatDiagram.FindDControl(const AName: string): TDiagramControl;
var
  c: integer;
begin
  result := nil;
  for c := 0 to DControlCount - 1 do
    if AnsiCompareText(AName, DControls[c].Name) = 0 then
    begin
      result := DControls[c];
      exit;
    end;
end;

function TatDiagram.FindBlock(const AName: string): TCustomDiagramBlock;
var
  c: integer;
begin
  result := nil;
  for c := 0 to BlockCount - 1 do
    if AnsiCompareText(AName, Blocks[c].Name) = 0 then
    begin
      result := Blocks[c];
      exit;
    end;
end;

procedure TatDiagram.Loaded;
begin
  inherited;
  CalcPanelSize;

  if (PageSettings.PaperWidth = 0) or (PageSettings.PaperHeight = 0) then
  begin
    PageSettings.RetrieveDefaultSize;
  end;

  if FInternalUpdating = 0 then
    ClearUndoStack;

  NameAllDControls;
end;

function TatDiagram.DoGetSurroundRegion(
  ABlock: TCustomDiagramBlock): TPointArray;
begin

end;

function TatDiagram.DoMouseWheelDown(Shift: TShiftState;
  MousePos: TPoint): Boolean;
var
  FBar: TDiagramScrollBar;
begin
  result := inherited DoMouseWheelDown(Shift, MousePos);
  if not result then
  begin
    if ssCtrl in Shift then
    begin
      if FWheelZoom then
      begin
        Zoom := Max(Zoom - FWheelZoomIncrement, FWheelZoomMin);
        result := true;
      end;
    end
    else
    begin
      Case FMouseWheelMode of
        mwHorizontal:
          FBar := HorzScrollBar;
        mwVertical:
          FBar := VertScrollBar;
      else
        FBar := nil;
      end;
      result := FBar <> nil;
      if result then
      begin
        FBar.Position := FBar.Position + FBar.Increment;
        Redraw;
      end;
    end;
  end;
end;

function TatDiagram.DoMouseWheelUp(Shift: TShiftState;
  MousePos: TPoint): Boolean;
var
  FBar: TDiagramScrollBar;
begin
  result := inherited DoMouseWheelUp(Shift, MousePos);
  if not result then
  begin
    if ssCtrl in Shift then
    begin
      if FWheelZoom then
      begin
        Zoom := Min(Zoom + FWheelZoomIncrement, FWheelZoomMax);
        result := true;
      end;
    end 
    else
    begin
      Case FMouseWheelMode of
        mwHorizontal:
          FBar := HorzScrollBar;
        mwVertical:
          FBar := VertScrollBar;
      else
        FBar := nil;
      end;
      result := FBar <> nil;
      if result then
      begin
        FBar.Position := FBar.Position - FBar.Increment;
        Redraw;
      end;
    end;
  end;
end;

function TatDiagram.StoreDesignProperty: boolean;
begin
  { Store only desired properties at runtime, to avoid loosing the correct reference to
    method properties when loading from stream, such as popupmenu property. }
  result := csDesigning in ComponentState;
end;

procedure TatDiagram.Assign(Source: TPersistent);
Var
  aStream : TMemoryStream;
  aSourceName,
  aSelfName : String;
begin
  if Source is TatDiagram then
  begin
    aStream := TMemoryStream.Create;
    aSourceName := TComponent(Source).Name;
    aSelfName := Name;
    (Source as TComponent).Name := '';
    Self.Name := '';
    try
      aStream.WriteComponent(TComponent(Source));
      aStream.Seek(0, soFromBeginning);
      ReadDiagramStream(aStream);
    finally
      TComponent(Source).Name := aSourceName;
      Name := aSelfName;
      aStream.Free;
    end;
  end
  else
    inherited;
end;

function TatDiagram.IsDesignTime: boolean;
begin
  result := (csDesigning in ComponentState);
end;

procedure TatDiagram.DoDrawLinkPoint(ACanvas: TCanvas; ALinkPoint: TLinkPoint; APoint: TPoint;
  var DrawingDone: boolean);
begin
  if Assigned(FOnDrawLinkPoint) then
    FOnDrawLinkPoint(Self, ACanvas, ALinkPoint, APoint, DrawingDone);
end;

procedure TatDiagram.SetActiveLayers(const Value: Int64);
begin                         
  if FActiveLayers <> Value then
  begin
    FActiveLayers := Value;
    Redraw;
  end;
end;

{$IFDEF DELPHI6_LVL}
procedure TatDiagram.ActivateLayer(ALayer: integer);    
begin                                                  
  ActiveLayers := ActiveLayers or Int64((Int64(1) shl Int64(ALayer)));
end;                                                 
                                                 
procedure TatDiagram.DeactivateLayer(ALayer: integer);
begin                                        
  ActiveLayers := ActiveLayers and not Int64((Int64(1) shl Int64(ALayer)));
  UpdateSelecteds;
end;
                                        
function TatDiagram.LayerActive(ALayer: integer): boolean;
begin
  result := (ALayer = 0) or ((Int64(Int64(1) shl Int64(ALayer)) and ActiveLayers) <> 0);
end;
{$ELSE}
procedure TatDiagram.ActivateLayer(ALayer: integer);
begin
  ActiveLayers := ActiveLayers or (1 shl ALayer);
end;

procedure TatDiagram.DeactivateLayer(ALayer: integer);
begin
  ActiveLayers := ActiveLayers and not (1 shl ALayer);
  UpdateSelecteds;
end;

function TatDiagram.LayerActive(ALayer: integer): boolean;
begin
  result := (ALayer = 0) or (((1 shl ALayer) and ActiveLayers) <> 0);
end;
{$ENDIF}

procedure TatDiagram.LinkNodeMouseDown(ALinkPoint: TLinkPoint);
begin
  if ALinkPoint.IsNode then
    ALinkPoint.Collapsed := not ALinkPoint.Collapsed;
end;

procedure TatDiagram.SetAutomaticNodes(const Value: boolean);
begin
  if FAutomaticNodes <> Value then
  begin
    FAutomaticNodes := Value;
    Redraw;
  end;
end;

procedure TatDiagram.StartZooming(ZoomingState: TZoomingState);
begin
  FZoomingState := ZoomingState;
  UnselectAll;
  FMouseState := msToZoom;
  Screen.Cursor := FZoomCursor;
end;

procedure TatDiagram.CancelZooming;
begin
  FMouseState := msBrowsing;
  Screen.Cursor := crDefault;
end;

const
  MaxZoomRatios = 17;
  ZoomRatios: Array[0..MaxZoomRatios - 1] of double =
    (0.05, 0.0833, 0.125, 0.25, 0.3333, 0.5, 0.6667, 0.75, 1,
     1.25, 1.50, 2, 3, 4, 6, 8, 10);

procedure TatDiagram.ZoomIn(ARect: TSquare);
var
  hc, wc: number;
  r1, r2: double;
  Clicked: boolean;
  c: integer;
begin
  Clicked := AroundPoint(ARect.TopLeft, ARect.BottomRight, 10);

  wc := ClientWidth - FLeftRuler.VisibleSize;
  hc := ClientHeight - FTopRuler.VisibleSize;
  if Clicked then
  begin
    c := 0;
    while (c < MaxZoomRatios - 1) and (FZoomRatio >= ZoomRatios[c]) do
      inc(c);
    FZoomRatio := ZoomRatios[c];
  end
  else
  begin
    r1 := wc / (ARect.Right - ARect.left);
    r2 := hc / (ARect.Bottom - ARect.top);
    if r1 < r2 then
      FZoomRatio := r1
    else
      FZoomRatio := r2;
  end;

  if FZoomRatio < 0.01 then
    FZoomRatio := 0.01;
  if FZoomRatio > 10 then
    FZoomRatio := 10;
  FTopRuler.Zoom := Trunc(ZoomRatio * 100);
  FLeftRuler.Zoom := FTopRuler.Zoom;
  CalcPanelSize;
  HorzScrollBar.Position := Round(Trunc(((ARect.Left + ARect.Right) / 2) * FZoomRatio) - wc / 2);
  VertScrollBar.Position := Round(Trunc(((ARect.top + ARect.Bottom) / 2) * FZoomRatio) - hc / 2);
  UpdateScrollBars;
end;

procedure TatDiagram.Zoomout(ARect: TSquare);
var
  hc, wc: number;
  r1, r2: double;
  Clicked: boolean;
  c: integer;
begin
  Clicked := AroundPoint(ARect.TopLeft, ARect.BottomRight, 10);

  wc := ClientWidth - FLeftRuler.VisibleSize;
  hc := ClientHeight - FTopRuler.VisibleSize;
  if Clicked then
  begin
    c := MaxZoomRatios - 1;
    while (c > 0) and (FZoomRatio <= ZoomRatios[c]) do
      dec(c);
    FZoomRatio := ZoomRatios[c];
  end
  else
  begin
    r1 := (ARect.Right - ARect.left) * FZoomratio / ClientWidth;
    r2 := (ARect.Bottom - ARect.top) * FZoomRatio / ClientHeight;
    if r1 < r2 then
      r1 := r2;
    FZoomratio := r1;
  end;

  if FZoomRatio < 0.01 then
    FZoomRatio := 0.01;
  if FZoomRatio > 10 then
    FZoomRatio := 10;
  FTopRuler.Zoom := Trunc(ZoomRatio * 100);
  FLeftRuler.Zoom := FTopRuler.Zoom;
  CalcPanelSize;
  HorzScrollBar.Position := Round(Trunc(((ARect.Left + ARect.Right) / 2) * FZoomRatio) - wc / 2);
  VertScrollBar.Position := Round(trunc(((ARect.top + ARect.Bottom) / 2) * FZoomRatio) - hc / 2);
  UpdateScrollBars;
end;

procedure TatDiagram.ZoomMoveToFit(AMargin: integer = 0);
var
  c: Integer;
  AZoom: double;
  AOffset: TPoint;
  ATarget: TRect;
begin
  ATarget.TopLeft := Point(FLeftRuler.VisibleSize, FTopRuler.VisibleSize);
  if FAutoScroll then
    ATarget.BottomRight := Point(
      FHorzScrollBar.ControlSize(false, false),
      FVertScrollBar.ControlSize(false, false))
  else
    ATarget.BottomRight := Point(ClientWidth, ClientHeight);

  CalculateZoomToFitRatio(ATarget, AMargin, AZoom, AOffset);

  Zoom := Round(AZoom * 100);

  {Remove any left/top margin}
  if (AOffset.X <> 0) or (AOffset.Y <> 0) then
  begin
    BeginInternalUpdate;
    BeginUpdatePos;
    try
      for c := 0 to DControlCount - 1 do
        if not DControls[c].IsMember then
          DControls[c].MovePosition(-AOffset.X, -AOffset.Y);
    finally
      EndUpdatePos;
      EndInternalUpdate;
      for c := 0 to DControlCount - 1 do
        DControls[c].NotifyAnchoreds(acPosition);
    end;
  end;

  CalcPanelSize;
  Redraw;
end;

procedure TatDiagram.CalculateZoomToFitRatio(TargetRect: TRect; AMargin: integer; var AZoomRatio: double; var AOffset: TPoint);
var
  ARect: TSquare;
  ARectSize, ATargetSize, Off: TDot;
begin

  {Get the Rect corresponding to all objects}
  ARect := GetObjectsRect;

  TargetRect.Left := TargetRect.Left + AMargin;
  TargetRect.Top := TargetRect.Top + AMargin;
  TargetRect.Right := TargetRect.Right - 1 - AMargin;
  TargetRect.Bottom := TargetRect.Bottom - 1 - AMargin;

  {calculate the zoom ratio}
  ARectSize := Dot(ARect.Right - ARect.Left, ARect.Bottom - ARect.Top);
  ATargetSize := Dot(TargetRect.Right - TargetRect.Left, TargetRect.Bottom - TargetRect.Top);
  if (ARectSize.X = 0) or (ARectSize.Y = 0) then
  begin
    AZoomRatio := 1;
    exit;                                
  end else
    AZoomRatio := Min(ATargetSize.X / ARectSize.X, ATargetSize.Y / ARectSize.Y);

  {Calculate offset}
  Off := ARect.TopLeft;
  Off.X := Off.X - (AMargin / AZoomRatio);
  Off.Y := Off.Y - (AMargin / AZoomRatio);
  AOffset := RoundPoint(Off);
end;

function TatDiagram.GetObjectsRect: TSquare;
var
  R: TSquare;
  c: integer;
begin
  {calculate maximum and minimum position of controls}
  result := Square(MaxDouble, MaxDouble, 0, 0);
  for c := 0 to DControlCount - 1 do
  begin
    if DControls[c].IsVisible then
    begin
      R := DControls[c].SurroundRect;
      result.Left := Min(R.Left, result.Left);
      result.Top := Min(R.Top, result.Top);
      result.Right := Max(R.Right, result.Right);
      result.Bottom := Max(R.Bottom, result.Bottom);
    end;
  end;

  {If no objects in diagram, return an empty rect}
  if result.Left = MaxInt then
    result := Square(0, 0, 0, 0);
end;

procedure TatDiagram.DoDControlMouseDownEx(ADControl: TDiagramControl;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer;
  var AHandle: TStretchHandle);
begin
  ADControl.MouseDownEx(Button, Shift, X, Y, AHandle);
  if Assigned(FOnDControlMouseDownEx) then
    FOnDControlMouseDownEx(Self, ADControl, Button, Shift, X, Y, AHandle);
end;

procedure TatDiagram.SetShowCrossIndicators(const Value: boolean);
begin
  if FShowCrossIndicators <> Value then
  begin
    FShowCrossIndicators := Value;
    Redraw;
  end;
end;

procedure TatDiagram.SetAutoPage(const Value: boolean);
begin
  if FAutoPage <> Value then
  begin
    FAutoPage := Value;
    if FAutoPage then
    begin
      CalcPanelSize;
      Redraw;
    end;
  end;
end;

procedure TatDiagram.MakeControlVisible(AControl: TDiagramControl);
var
  APoint: TDot;
begin
  if AControl <> nil then
  begin
    {Calculate the central point to be visible}
    with AControl.ControlRect do
      APoint := Dot((Left + Right) / 2, (Top + Bottom) / 2);
    APoint := MulPoint(APoint, FZoomRatio);

    {Move to the correct X and Y position}
    HorzScrollBar.Position := Round(APoint.X - (ClientWidth / 2) + (LeftRuler.VisibleSize / 2));
    VertScrollBar.Position := Round(APoint.Y - (ClientHeight / 2) + (TopRuler.VisibleSize / 2));
    Redraw;
  end;
end;

function TatDiagram.MeasUnitToMM(AUnit: extended): extended;
begin
  Case MeasUnit of
    duMili: result := AUnit;
    duInch: result := AUnit * 25.4;
  else
    //duCenti default
    result := AUnit * 10;
  end;
end;

function TatDiagram.DiagramStreamClass: TatDiagramClass;
begin
  result := TStreamDiagram;
end;

procedure TatDiagram.SetPageLines(const Value: TPageLines);
begin
  FPageLines.Assign(Value);
end;

function TatDiagram.ConnectionLineClass: TCustomDiagramLineClass;
var
  AFoundClass: TPersistentClass;
  RegDControl: TRegDControl;
begin
  result := nil;
  RegDControl := RegDControlList.FindByID(ConnectionLineId);
  if RegDControl <> nil then
    AFoundClass := RegDControl.DControlClass
  else
    AFoundClass := nil;
  if (AFoundClass <> nil) and AFoundClass.InheritsFrom(TCustomDiagramLine) then
    result := TCustomDiagramLineClass(AFoundClass);
end;

function TatDiagram.DoAfterDrawBlock(ABlock: TCustomDiagramBlock; AInfo: TDiagramDrawInfo;
  ABlockInfo: TDiagramDrawBlockInfo): boolean;
begin
  result := true;
  if Assigned(FOnAfterDrawBlock) then
    FOnAfterDrawBlock(Self, ABlock, AInfo, ABlockInfo, result);
end;

function TatDiagram.DPrinter: TDiagramPrinter;
begin
  result := DPrinters.Printer;
end;

procedure TatDiagram.SetHandlesStyle(const Value: TDiagramHandlesStyle);
var
  c: integer;
begin
  if FHandlesStyle <> Value then
  begin
    FHandlesStyle := Value;
    for c := 0 to BlockCount - 1 do
      Blocks[c].UpdateBlockHandles;
    FSelBlock.UpdateBlockHandles;
    Redraw;
  end;
end;

procedure TatDiagram.BeginCacheBitmap;
{var
  DC: HDC;}
begin
{  DC := GetDC(0);
  FCacheImage.Handle :=
    CreateCompatibleBitmap(DC, ClientRect.Right, ClientRect.Bottom);
  ReleaseDC(0, DC);}
  if (FCacheImage.Width <> ClientRect.Right) or (FCacheImage.Height <> ClientRect.Bottom) then
  begin
    FCacheImage.Width := ClientRect.Right;
    FCacheImage.Height := ClientRect.Bottom;
  end;
  FDoingCache := true;
  try
    PaintToCanvas(FCacheImage.Canvas, psScreen, Dot(0, 0));
  finally
    FDoingCache := false;
  end;
  FUseCache := true;
end;

procedure TatDiagram.EndCacheBitmap;
begin
  FUseCache := false;
end;

function TatDiagram.ClientToInches(X: double): double;
begin
  result := X / ScreenDPI;
end;

function TatDiagram.ClientToMilimeters(X: double): double;
begin
  result := ClientToInches(X) * 25.4;
end;

function TatDiagram.InchesToClient(X: double): double;
begin
  result := X * ScreenDPI;
end;

function TatDiagram.MilimetersToClient(X: double): double;
begin
  result := InchesToClient(X / 25.4);
end;

function TatDiagram.MMToMeasUnit(AMM: extended): extended;
begin
  Case MeasUnit of
    duMili: result := AMM;
    duInch: result := AMM / 25.4;
  else
    //duCenti default
    result := AMM / 10;
  end;
end;

procedure TatDiagram.SetSmoothMode(const Value: TDgrSmoothMode);
begin
  if FSmoothMode <> Value then
  begin
    FSmoothMode := Value;
    Redraw;
  end;
end;

procedure TatDiagram.SetTextRenderingMode(const Value: TDgrTextRenderingMode);
begin
  if FTextRenderingMode <> Value then
  begin
    FTextRenderingMode := Value;
    Redraw;
  end;
end;

procedure TatDiagram.SetSelectionMode(const Value: TDiagramSelectionMode);
begin
  if FSelectionMode <> Value then
  begin
    FSelectionMode := Value;
    Redraw;
  end;
end;

function TatDiagram.GroupSelection: boolean;
begin
  result := (SelectionMode = slmGroup) and (SelectedCount(cfIgnoreAsMembers) > 1);
end;

function TatDiagram.AddGroup: TGroupBlock;
const
  SGroupBaseName = '___dgrGroup___';
begin
  result := GroupBlockClass.Create(Self.Owner);
  result.Name := FindCompName(result, SGroupBaseName);

  // set FDiagram directly to avoid include this block in DControls and Blocks list
  result.Diagram := Self;
end;

procedure TatDiagram.DoMemberGroupIncluded(AGroup: TGroupBlock; AMember: TDiagramControl);
begin
  {Select the whole group if a single member was included in the group, and any of group members were selected.}
  SetGroupSelected(AGroup.MasterGroup, AGroup.MasterGroup.HasSelecteds);
end;

procedure TatDiagram.DoMemberGroupRemoved(AGroup: TGroupBlock; AMember: TDiagramControl);
begin

end;

procedure TatDiagram.SetGraphicLib(const Value: TDgrGraphicLib);
begin
  if Value <> FGraphicLib then
  begin
    FGraphicLib := Value;
    CheckGraphicLibObjects;
    Redraw;
  end;
end;

procedure TatDiagram.SetGroupSelected(ADControl: TDiagramControl;
  ASelected: boolean);

  procedure _UpdateSelected(AControl: TDiagramControl);
  begin
    if AControl.FSelected <> ASelected then
    begin
      AControl.FSelected := ASelected;
      if AControl.FSelected then
        DoSelectDControl(AControl)
      else
        DoUnselectDControl(AControl);
    end;
  end;

  procedure _SetGroupSelection(AGroup: TGroupBlock);
  var
    c: integer;
  begin
    _UpdateSelected(AGroup);
    for c := 0 to AGroup.Members.Count - 1 do
      if AGroup.Members[c].DControl <> nil then
      begin
        if AGroup.Members[c].DControl.IsGroup then
          _SetGroupSelection(TGroupBlock(AGroup.Members[c].DControl))
        else
         _UpdateSelected(AGroup.Members[c].DControl);
      end;
  end;

begin
  BeginUpdateSel;
  try
    if ADControl.IsGroup then
      _SetGroupSelection(TGroupBlock(ADControl))
    else
     _UpdateSelected(ADControl);
  finally
    EndUpdateSel;
  end;
end;

function TatDiagram.GroupBlockClass: TGroupBlockClass;
begin
  if FCustomGroupBlockClass <> nil then
    result := FCustomGroupBlockClass
  else
    result := TGroupBlock;
end;

function TatDiagram.DiagramMemoClass: TDiagramMemoClass;
begin
  if FCustomDiagramMemoClass <> nil then
    result := FCustomDiagramMemoClass
  else
    result := TDiagramMemo;
end;

function TatDiagram.GroupSelectedBlocks: TGroupBlock;
var
  AGroup: TGroupBlock;
  AList: TDControlList;
  c: integer;
begin
  AGroup := AddGroup;
  AList := TDControlList.Create(nil, TDControlListItem);
  try
    for c := 0 to DControlCount - 1 do
      if DControls[c].Selected then
      begin
        if not DControls[c].IsMember then
          AList.Add(DControls[c]);
      end;

    for c := 0 to AList.Count - 1 do
      AGroup.Members.Add(AList[c].DControl);
  finally
    AList.Free;
  end;
  if AGroup.Members.Count < 2 then
    AGroup.Free
  else
  begin
    PushUndoStack('group');
  end;

  Result := AGroup; 
end;

procedure TatDiagram.UngroupSelectedBlocks;
var
  AGroupList: TDControlList;
  c: integer;
begin
  AGroupList := TDControlList.Create(nil, TDControlListItem);
  try
    {Retrieve grouped blocks (only top level)}
    for c := 0 to SelectedCount - 1 do
      if Selecteds[c].IsGroup and not Selecteds[c].IsMember then
        AGroupList.Add(Selecteds[c]);

    {Remove the top level group blocks}
    for c := 0 to AGroupList.Count - 1 do
      AGroupList[c].DControl.Free;

    if AGroupList.Count > 0 then
      PushUndoStack('ungroup');
  finally
    AGroupList.Free;
  end;
end;

function TatDiagram.EditingCell: TTextCell;
begin
  if EditingText then
    result := FInplaceMemo.FCell
  else
    result := nil;
end;

procedure TatDiagram.SetPageSettings(const Value: TDiagramPageSettings);
begin
  FPageSettings.Assign(Value);
end;

procedure TatDiagram.PageSetupDlg;
var
  F: TfmDgrPageSettings;
begin
  F := TfmDgrPageSettings.Create(nil);
  try
    if F.EditPageSettings(Self) then
      Redraw;
  finally
    F.Free;
  end;
end;


procedure TatDiagram.SetMeasUnit(const Value: TDiagramMeasUnit);
begin
  if FIntMeasUnit <> Value then
  begin
    FIntMeasUnit := Value;
    Redraw;
  end;
end;

procedure TatDiagram.SetRulerAutoUnit(const Value: boolean);
begin
  if FRulerAutoUnit <> Value then
  begin
    FRulerAutoUnit := Value;
    Redraw;
  end;
end;

function TatDiagram.GetUnitSymbol: string;
begin
  Case MeasUnit of
    duMili: result := 'mm';
    duInch: result := 'in';
  else
    //duCenti default
    result := 'cm';
  end;
end;

procedure TatDiagram.ClearDControls;
begin
  while DControlCount > 0 do
    DControls[0].Free;
end;

procedure TatDiagram.SetDesignMode(const Value: TDiagramDesignMode);
begin
  if FDesignMode <> Value then
  begin
    FDesignMode := Value;
    Redraw;
  end;
end;

{$IFDEF DELPHI6_LVL}
procedure TatDiagram.ReadDiagramStream(AStream: TStream);
begin
  Classes.RegisterFindGlobalComponentProc(FindGlobalDiagramOwner);
  GlobalDiagramOwner := Self.Owner;
  try
    AStream.ReadComponent(Self);
  finally
    GlobalDiagramOwner := nil;
    Classes.UnregisterFindGlobalComponentProc(FindGlobalDiagramOwner);
  end;
end;
{$ELSE}
procedure TatDiagram.ReadDiagramStream(AStream: TStream);
var
  OldGlobal: TFindGlobalComponent;
begin
  OldGlobal := Classes.FindGlobalComponent;
  Classes.FindGlobalComponent := FindGlobalDiagramOwner;
  GlobalDiagramOwner := Self.Owner;
  try
    AStream.ReadComponent(Self);
  finally
    GlobalDiagramOwner := nil;
    Classes.FindGlobalComponent := OldGlobal;
  end;
end;
{$ENDIF}

procedure TatDiagram.SetLayers(const Value: TDiagramLayers);
begin
  FLayers := Value;
end;

procedure TatDiagram.UpdateSelecteds;
var
  c: integer;
begin
  for c := 0 to DControlCount - 1 do
    if DControls[c].Selected then
      DControls[c].UpdateSelected;
end;

procedure TatDiagram.DeleteLayerControls(ALayer: TDiagramLayer);
var
  i: integer;
begin
  for i:=DControlCount-1 downto 0 do
    if DControls[i].LayerObj = ALayer then
      DControls[i].Free;
end;

procedure TatDiagram.AddSubscriber(ASubscriber: IDiagramSubscriber);
begin
  FSubList.Add(ASubscriber);
end;

procedure TatDiagram.RefreshToolbars;
var
  AInfo: TDgrNotificationInfo;
begin
  AInfo.Event := deLibraryChanged;
  NotifySubscribers(AInfo);
end;

procedure TatDiagram.NotifySubscribers(AInfo: TDgrNotificationInfo);
var
  c: integer;
begin
  for c := 0 to FSubList.Count - 1 do
    IDiagramSubscriber(FSubList[c]).DgrNotification(AInfo);
end;

function TatDiagram.GetPathDrawingMode: boolean;
begin
  result := GraphicLib <> dglGDI;
end;

function TatDiagram.GetUseGdiPlus: boolean;
begin                                  
  {$IFDEF GDIPLUS}
  result := GraphicLib = dglGDIPlus;
  {$ELSE}
  result := false;          
  {$ENDIF}
end;

procedure TatDiagram.SetUseGdiPlus(const Value: boolean);
begin
  {$IFDEF GDIPLUS}
  if Value then
    GraphicLib := dglGDIPlus
  else
  {$ENDIF}
    GraphicLib := dglGDI;
end;

{ TCustomDiagramLine }

constructor TCustomDiagramLine.Create(AOwner: TComponent);
var
  ACalcInfo: TCalcHandlesInfo;
begin
  inherited;
  FLineStyle := lsLine;
  {FStrings := TStringList.Create;
  TStringList(FStrings).OnChange := StringsChange;}
  SetLength(FPoints, 0);
  FPen := TPen.Create;
  FPen.Style := psSolid;
  FPen.Color := clBlack;
  FPen.OnChange := StyleChanged;
  FSelPen := TPen.Create;
  FSelPen.Assign(FPen);
  FSelPen.OnChange := StyleChanged;
  FSourceArrow := GetLineArrowClass.Create(Self);
  FTargetArrow := GetLineArrowClass.Create(Self);
  FTempHandles := TStretchHandles.Create(nil); //Pass nil to avoid redrawing
  FTextCellsMode := tmSingle;
  FRecalcIntermediateArcHandles := true;
  CheckLinkPoints;
  ACalcInfo.P1 := Dot(0, 0);
  ACalcInfo.P2 := Dot(100, 50);
  ACalcInfo.Anchor1 := nil;
  ACalcInfo.Anchor2 := nil;
  ACalcInfo.HandleMoved := nil;
  ACalcInfo.Action := chNone;
  CalcNewHandles(Handles, ACalcInfo);
end;

destructor TCustomDiagramLine.Destroy;
begin
  Diagram := nil;
  FPen.Free;
  FSelPen.Free;
  FSourceArrow.Free;
  FTargetArrow.Free;
  FTempHandles.Free;
  //FStrings.Free;
  inherited;
end;

procedure TCustomDiagramLine.CheckLinkPoints;
begin
  {Only 2 link points must exist}
  while FLinkPoints.Count < 2 do
    FLinkPoints.Add;
  {while FLinkPoints.Count > 2 do
    FLinkPoints[FLinkPoints.Count - 1].Free;}
end;

procedure TCustomDiagramLine.CheckHandles;
begin
  {At least 2 handles must exist}
  if Handles.Count = 0 then
    Handles.Add(0, 0); {just numbers, this line should never be executed}
  if Handles.Count = 1 then
    Handles.Add(100, 50); {just numbers, this line should never be executed}
end;

procedure TCustomDiagramLine.DiagramChanged(Old, New: TatDiagram);
begin
  if Assigned(Old) then
  begin
    NotifyAnchoreds(acRemoval);
    Old.RemoveLink(Self);
  end;
  if Assigned(New) then
  begin
    New.AddLink(Self);
    DiagramFontChanged;
  end;
end;

class function TCustomDiagramLine.DiagramDrawLineInfo(AHandles: TStretchHandles; ADrawArrows: boolean): TDiagramDrawLineInfo;
begin
  result.Handles := AHandles;
  result.DrawArrows := ADrawArrows;
end;

procedure TCustomDiagramLine.SelectedChanged;
begin
  Redraw;
end;

procedure TCustomDiagramLine.StyleChanged(Sender: TObject);
begin
  if Sender = FPen then
    PenChanged
  else
  if Sender = FSelPen then
    SelPenChanged;
  Redraw;
end;

procedure TCustomDiagramLine.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure TCustomDiagramLine.SetSourceArrow(const Value: TLineArrow);
begin
  FSourceArrow.Assign(Value);
end;

procedure TCustomDiagramLine.SetTargetArrow(const Value: TLineArrow);
begin
  FTargetArrow.Assign(Value);
end;

function TCustomDiagramLine.CurPen: TPen;
begin
  if Selected then
    result := FSelPen
  else
    result := FPen;
end;

procedure TCustomDiagramLine.SetSelPen(const Value: TPen);
begin
  FSelPen.Assign(Value);
end;

type
  THelperLine = class
  private
    Points: TPointArray;
    procedure AddPoint(X, Y: integer);
    procedure Sort(S: TDot);
  public
    constructor Create;
    destructor Destroy; override;
  end;

procedure THelperLine.AddPoint(X, Y: integer);
var
  L: integer;
begin
  L := Length(Points);
  SetLength(Points, L + 1);
  Points[L].X := X;
  Points[L].Y := Y;
end;

constructor THelperLine.Create;
begin
  Setlength(Points,0);
end;

destructor THelperLine.Destroy;
begin
  Setlength(Points,0);
  inherited;
end;

procedure THelperLine.Sort(S: TDot);
label 10;
var P: TPoint;
    i,j: Integer;
    d,d1: double;

  { Straight insertion sort}
  function dist(A: TPoint; B: TDot): double;
  begin
    result := Sqr(A.X - B.X) + Sqr(A.Y - B.Y);
  end;

begin
  for j := 1 to Length(Points) - 1 do
  begin
    P := Points[j];
    d := dist(P, S);
    For i := j - 1 downto 0 do
    begin
      d1 := dist(Points[i],S);
      if (d1 <= d) then
        goto 10;
      Points[i + 1] := points[i];
    end;
    i := -1;
10: Points[i + 1] := P;
  end;
end;

procedure TCustomDiagramLine.DrawLine(AInfo: TDiagramDrawInfo; ALineInfo: TDiagramDrawLineInfo);
var
  c: integer;
  AZoomRatio: double;
  PC: TPoint;

  function SideLineIntersect(P1, P2: TDot; P3, P4: TPoint; var PI: TPoint): boolean;

    function between(a, b, c: integer): boolean;
    begin
      result := false;
      if (a = b) or (a = c) then exit;
      If (a > b) then result := a < c else result := a > c;
    end;

  begin
    if (P1.Y = P2.Y) then
    begin
      result := between(Trunc(P1.Y), P3.Y, P4.Y) and Between(P3.X, trunc(P1.X), trunc(P2.x));
      if result then
      begin
        PI.X := P3.X;
        PI.Y := trunc(P1.Y);
      end;
    end
    else
    begin
      result :=  between(trunc(P1.X), P3.X, P4.X) and Between(P3.Y, trunc(P1.Y), trunc(P2.Y));
      if result then
      begin
        PI.X := Trunc(P1.X);
        PI.Y := P3.Y;
      end;
    end;
  end;

var
  il: integer;
  i: integer;
  j: integer;
  HL: THelperLine;
  P: TPoint;
  path: TDgrGraphicsPath;
  dgrPaintEngine: TDgrPaintEngine;
  createDgr: boolean;
  PreviousPoint: TDot;
  P1, P2: TDot;
  PZ: TPoint;
begin
  {Calculate zoom ratio}
  if FDiagram <> nil then
  begin
    PZ := SubPoint(
      FDiagram.ClientToCanvas(Point(100, 100)),
      FDiagram.ClientToCanvas(Point(0, 0)));
    AZoomRatio := PZ.Y / 100;
  end
  else
    AZoomRatio := 1;

  RecalcPoints(ALineInfo.Handles);
  il := 0;
  if (FDiagram <> nil) then
    il := FDiagram.FDcontrols.IndexOf(self);

  createDgr := (FDiagram = nil) and PathDrawingMode;
  if createDgr then
  begin
    dgrPaintEngine := AInfo.DgrDrawer.CreatePaintEngine(AInfo.Canvas, AInfo.Canvas.ClipRect);
    AInfo.DgrDrawer.PaintEngine := dgrPaintEngine;
  end
  else
    dgrPaintEngine := nil;
  if PathDrawingMode then
    path := AInfo.DgrDrawer.CreatePath
  else
    path := nil;

  try
    {Now draw the lines based on FPoints. When it's not using GdiPlus, then we need to convert to canvas}
    HL := nil;
    try
      if (Length(FPoints) > 1) then
      begin
        if PathDrawingMode then
          PreviousPoint := FPoints[0]
        else
        begin
          if FDiagram <> nil then
            PC := RoundPoint(FDiagram.ClientToCanvas(FPoints[0]))
          else
            PC := RoundPoint(FPoints[0]);
          AInfo.Canvas.MoveTo(PC.X, PC.Y);
        end;

        {Iterate through points. Calculate intersection and draw the lines at the same time}
        for c := 1 to Length(FPoints) - 1 do
        begin
          if (Self is TCustomDiagramLine) and (Self.LineStyle = lsSideLine) and (FDiagram <> nil) and (FDiagram.ShowCrossIndicators) then
          begin
            for i := 0 to il - 1 do
              if (TCustomDiagramLine(FDiagram.DControls[i]).LineStyle = lsSideLine) then
              begin
                for j := 1 to Length(TCustomDiagramLine(FDiagram.Dcontrols[i]).FPoints) - 1 do
                begin
                  if SideLineIntersect(FPoints[c-1], FPoints[c],
                    RoundPoint(TCustomDiagramLine(Fdiagram.Dcontrols[i]).FPoints[j-1]),
                    RoundPoint(TCustomDiagramLine(Fdiagram.Dcontrols[i]).FPoints[j]),
                    P) then
                  begin
                    if not assigned(HL) then
                      HL := THelperLine.create;
                    if (FPoints[c-1].X = FPoints[c].X) then
                    begin
                      HL.AddPoint(Trunc(P.X), trunc(P.Y) - 4);
                      HL.AddPoint(Trunc(P.X) + 4,Trunc(P.Y));
                      HL.AddPoint(Trunc(P.X), Trunc(P.Y) + 4);
                    end else
                    begin
                      HL.AddPoint(Trunc(P.X) - 4,Trunc(P.Y));
                      HL.AddPoint(Trunc(P.X), Trunc(P.Y) - 4);
                      HL.AddPoint(Trunc(P.X) + 4,Trunc(P.Y));
                    end;
                  end;
                end;
              end;

            if assigned(HL) then
            begin
              HL.Sort(FPoints[c-1]);
              for i := 0 to length(HL.Points)-1 do
                if PathDrawingMode then
                begin
                  path.AddLine(PreviousPoint.X, PreviousPoint.Y, HL.Points[i].X, HL.Points[i].Y);
                  PreviousPoint := Dot(HL.Points[i]);
                end
                else
                begin
                  PC := FDiagram.ClientToCanvas(HL.Points[i]);
                  AInfo.Canvas.LineTo(PC.X, PC.Y);
                end;
              HL.Free;
              HL := nil;
            end;
          end;

          if PathDrawingMode then
          begin
            path.AddLine(PreviousPoint.X, PreviousPoint.Y, FPoints[c].X, FPoints[c].Y);
            PreviousPoint := FPoints[c];
          end
          else
          begin
            if FDiagram <> nil then
              PC := RoundPoint(FDiagram.ClientToCanvas(FPoints[c]))
            else
              PC := RoundPoint(FPoints[c]);
            AInfo.Canvas.LineTo(PC.X, PC.Y);
          end;
        end;

        {Draw the lines for gdiplus, since until now we have only built the path}
        if PathDrawingMode then
          AInfo.DgrDrawer.Canvas.DrawPath(path);

        if ALineInfo.DrawArrows then
        begin
          c := 1;
          repeat
            P2 := FPoints[c];
            P1 := FPoints[c - 1];
            inc(c);
          until (P1.X <> P2.X) or (P1.Y <> P2.Y) or (c = Length(FPoints));
          if (P1.X <> P2.X) or (P1.Y <> P2.Y) then
          begin
            {Set properties for gdiplus drawing}
            SourceArrow.DrawFrom := P2;
            SourceArrow.DrawTo := P1;

            {Now draw!}
            if FDiagram <> nil then
              SourceArrow.Draw(AInfo, SourceArrow.DiagramDrawArrowInfo(FDiagram.ClientToCanvas(P2), FDiagram.ClientToCanvas(P1), AZoomRatio))
            else
              SourceArrow.Draw(AInfo, SourceArrow.DiagramDrawArrowInfo(P2, P1, AZoomRatio));
          end;

          c := Length(FPoints) - 2;
          repeat
            P2 := FPoints[c + 1];
            P1 := FPoints[c];
            dec(c);
          until (P1.X <> P2.X) or (P1.Y <> P2.Y) or (c = -1);
          if (P1.X <> P2.X) or (P1.Y <> P2.Y) then
          begin
            {Set properties for gdiplus drawing}
            TargetArrow.DrawFrom := P1;
            TargetArrow.DrawTo := P2;

            {Now draw!}
            if FDiagram <> nil then
              TargetArrow.Draw(AInfo, TargetArrow.DiagramDrawArrowInfo(FDiagram.ClientToCanvas(P1), FDiagram.ClientToCanvas(P2), AZoomRatio))
            else
              TargetArrow.Draw(AInfo, TargetArrow.DiagramDrawArrowInfo(P1, P2, AZoomRatio));
          end;
        end;
      end;
    finally
    end;
  finally
    if PathDrawingMode then
    begin
      path.Free;
      if createDgr then
      begin
        AInfo.DgrDrawer.PaintEngine := nil;
        dgrPaintEngine.Free;
      end;
    end;
  end;
end;

function TCustomDiagramLine.LinkPointByHandle(AHandle: TStretchHandle): TLinkPoint;
begin
  result := nil;
  if AHandle <> nil then
  begin
    CheckLinkPoints;
    if (AHandle.Index = 0) and (LinkPoints.Count > 0) then
      result := LinkPoints[0]
    else
    if (AHandle.Index = Handles.Count - 1) and (LinkPoints.Count > 1) then
      result := LinkPoints[1];
  end;
end;

function TCustomDiagramLine.HandleByLinkPoint(ALinkPoint: TLinkPoint): TStretchHandle;
begin
  result := nil;
  if ALinkPoint <> nil then
  begin
    CheckHandles;
    if (ALinkPoint.Index = 0) and (Handles.Count > 0) then
      result := Handles[0]
    else
    if (ALinkPoint.Index = 1) and (Handles.Count > 1) then
      result := Handles[Handles.Count - 1]
    else
      result := nil;
  end;
end;

function TCustomDiagramLine.GetLinkPoint(ALinkPoint: TLinkPoint): TDot;
var AHandle: TStretchHandle;
begin
  AHandle := HandleByLinkPoint(ALinkPoint);
  if AHandle <> nil then
    result := AHandle.DiagramPoint
  else
    result := DummyPointX;
end;

procedure TCustomDiagramLine.AnchorLinkChanged(ALinkPoint: TLinkPoint);
begin
  if (ALinkPoint <> nil) and (ALinkPoint.AnchorLink <> nil) then
    RecalcAnchors;
end;

procedure TCustomDiagramLine.LinkedAnchorChanged(ALinkPoint: TLinkPoint;
  AChange: TAnchorChange);
begin
  RecalcAnchors;
end;

procedure TCustomDiagramLine.RecalcAnchors;
var
  AInfo: TCalcHandlesInfo;
begin
  if not (csLoading in ComponentState) and ((Diagram = nil) or (Diagram.FInternalUpdating = 0)) then
  begin
    CheckLinkPoints;
    CheckHandles;
    AInfo.Anchor1 := LinkPoints[0].AnchorLink;
    AInfo.Anchor2 := LinkPoints[1].AnchorLink;
    if AInfo.Anchor1 <> nil then
      AInfo.P1 := AInfo.Anchor1.DiagramPoint
    else
      AInfo.P1 := Handles[0].DiagramPoint;
    if AInfo.Anchor2 <> nil then
      AInfo.P2 := AInfo.Anchor2.DiagramPoint
    else
      AInfo.P2 := Handles[Handles.Count - 1].DiagramPoint;
    AInfo.Action := chAnchorChanged;
    AInfo.HandleMoved := nil;
    CalcNewHandles(Handles, AInfo);
    NotifyAnchoreds(acPosition);
    if FLineStyle = lsSideLine then
      UpdateOrientations;
  end;
end;

procedure TCustomDiagramLine.StartMoving;
begin
  FTempHandles.Clear;
  FTempHandles.Assign(Handles);
end;

procedure TCustomDiagramLine.UpdateMoving(AInfo: TControlMoveInfo);
var
  ACalcInfo: TCalcHandlesInfo;
begin
  if AInfo.Action <> caEraseCursor then
  begin
    CheckLinkPoints;
    ACalcInfo.Action := chMove;
    ACalcInfo.Anchor1 := LinkPoints[0].AnchorLink;
    ACalcInfo.Anchor2 := LinkPoints[1].AnchorLink;
    {For temporary line calculation, remove anchors which controls are not
     being moved together}
    if (ACalcInfo.Anchor1 <> nil) and not ACalcInfo.Anchor1.DControl.Selected then
      ACalcInfo.Anchor1 := nil;
    if (ACalcInfo.Anchor2 <> nil) and not ACalcInfo.Anchor2.DControl.Selected then
      ACalcInfo.Anchor2 := nil;
    {Calculate start and end points of newly moved line, which will be moved
     first and last handle of old handle list}
    CheckHandles;
    ACalcInfo.DeltaMove := AInfo.Delta;
    ACalcInfo.P1 := AddPointX(Handles[0].DiagramPoint, AInfo.Delta);
    ACalcInfo.P2 := AddPointX(Handles[Handles.Count - 1].DiagramPoint, AInfo.Delta);
    {If controls being moved are going to link somewhere, test if it's the
     line that will be linked. If yes, then set anchor1 or anchor2 to the
     anchor to link, and update start or end point of new line to the position
     of that anchor
     The target anchor point SHOULD NOT BE MOVED TOGETHER with line, otherwise
     strange visual bugs might occur. But this is not being tested here, because
     diagram already tested and only passed anchorpoint not being moved}
    if (AInfo.LinkPoint <> nil) and (AInfo.LinkPoint.DControl = Self)
      and (AInfo.AnchorPoint <> nil) then
    begin
      if AInfo.LinkPoint.Index = 0 then
      begin
        ACalcInfo.Anchor1 := AInfo.AnchorPoint;
        ACalcInfo.P1 := AInfo.AnchorPoint.DiagramPoint;
      end else
      begin
        ACalcInfo.Anchor2 := AInfo.AnchorPoint;
        ACalcInfo.P2 := AInfo.AnchorPoint.DiagramPoint;
      end;
    end;
    ACalcInfo.HandleMoved := nil;
    FTempHandles.Assign(Handles);
    CalcNewHandles(FTempHandles, ACalcInfo);
  end;

  with AInfo do
    case Action of
      caEraseCursor, caDrawCursor:
        DrawLineCursor(Canvas, FTempHandles);
      caPositionCursor: ;
        //Do nothing, the ftemphandles were already calculated;
      caMoved:
        begin
          Handles.Assign(FTempHandles);
        end;
    end;
  if (FLineStyle = lsSideLine) and (AInfo.Action = caMoved) then
    UpdateOrientations;
end;

procedure TCustomDiagramLine.StartMovingHandle;
begin
  {Just for cursor}
  FTempHandles.Clear;
  FTempHandles.Assign(Handles);
end;

procedure TCustomDiagramLine.HandleMoving(AInfo: THandleMoveInfo);
var
  ALinkPoint: TLinkPoint;
  ACalcInfo: TCalcHandlesInfo;
begin
  if AInfo.Action <> haEraseCursor then
  begin
    CheckLinkPoints;
    ACalcInfo.Action := chHandleMove;
    ACalcInfo.Anchor1 := LinkPoints[0].AnchorLink;
    ACalcInfo.Anchor2 := LinkPoints[1].AnchorLink;
    CheckHandles;
    ACalcInfo.P1 := Handles[0].DiagramPoint;
    ACalcInfo.P2 := Handles[Handles.Count - 1].DiagramPoint;
    if (AInfo.AHandle.Index = 0) then
    begin
      ACalcInfo.Anchor1 := AInfo.AnchorPoint;
      if ACalcInfo.Anchor1 <> nil then
        ACalcInfo.P1 := ACalcInfo.Anchor1.DiagramPoint
      else
        ACalcInfo.P1 := AddPointX(ACalcInfo.P1, AInfo.Delta);
    end else
    if (AInfo.AHandle.Index = Handles.Count - 1) then
    begin
      ACalcInfo.Anchor2 := AInfo.AnchorPoint;
      if ACalcInfo.Anchor2 <> nil then
        ACalcInfo.P2 := ACalcInfo.Anchor2.DiagramPoint
      else
        ACalcInfo.P2 := AddPointX(ACalcInfo.P2, AInfo.Delta);
    end;
    FTempHandles.Assign(Handles);
    ACalcInfo.HandleMoved := AInfo.AHandle;
    ACalcInfo.DeltaMove := AInfo.Delta;
    CalcNewHandles(FTempHandles, ACalcInfo);
  end;
  Case AInfo.Action of
    haEraseCursor, haDrawCursor:
      DrawLineCursor(AInfo.Canvas, FTempHandles);
    haPositionCursor: ;
      //Do nothing
    haMoved:
      begin
        ALinkPoint := LinkPointByHandle(AInfo.AHandle);
        Handles.Assign(FTempHandles);
        // if there's a control that accepts implicit links, don't modify
        // the current anchorlink -> we're moving the handle inside the control
        if (Assigned(AInfo.DControl) and AInfo.DControl.AutoCreateLinkPoints) then
        begin
          //ALinkPoint.OrPoint := AInfo.AHandle.OrPoint;
        end
        else
        if (ALinkPoint <> nil) then
          ALinkPoint.AnchorLink := AInfo.AnchorPoint;
      end;
  end;
  if (AInfo.Action = haMoved) and (FLineStyle = lsSideLine) then
    UpdateOrientations;
end;

procedure TCustomDiagramLine.DrawLineCursor(Canvas: TCanvas; AHandles: TStretchHandles);
var
  oldBrush: TBrush;
  oldPen: TPen;
  oldGraphicLib: TDgrGraphicLib;
  oldCustomGraphicLib: boolean;
  dinfo: TDiagramDrawInfo;
begin
  oldBrush := TBrush.Create;
  oldPen := TPen.Create;
  try
    oldBrush.Assign(Canvas.Brush);
    oldPen.Assign(Canvas.Pen);
    Canvas.Brush.Style := bsClear;
    Canvas.Pen.Mode := pmNot;
    Canvas.Pen.Style := psDot;

    // GdiPlus does not support pens in pmNot mode. So, we need to draw the line using non-gdi plus drawing
    oldGraphicLib := GraphicLib;
    oldCustomGraphicLib := FCustomGraphicLib;
    try
      GraphicLib := dglGDI;
      dinfo := GetDiagramDrawInfo;
      dinfo.Canvas := Canvas;
      DrawLine(dinfo, DiagramDrawLineInfo(AHandles, False));
    finally
      GraphicLib := oldGraphicLib;
      FCustomGraphicLib := oldCustomGraphicLib;
    end;
  finally
    Canvas.Pen.Assign(oldPen);
    Canvas.Brush.Assign(oldBrush);
    oldBrush.Free;
    oldPen.Free;
  end;
end;

function TCustomDiagramLine.SurroundRect: TSquare;
begin
  result := ControlRect;
end;

function TCustomDiagramLine.ControlRect: TSquare;
var
  c : integer;
  PC: TDot;
begin
  RecalcPoints(Handles);
  result := Square(MaxInt, MaxInt, -MaxInt, -MaxInt);
  for c := 0 to Length(FPoints) - 1 do
  begin
    PC := FPoints[c];
    if result.Left > PC.X then
      result.Left := PC.X;
    if result.Right < PC.X then
      result.Right := PC.X;
    if result.Top > PC.Y then
      result.Top := PC.Y;
    if result.Bottom < PC.Y then
      result.Bottom := PC.Y;
  end;
end;

procedure TCustomDiagramLine.MovePosition(ADeltaX, ADeltaY: number);
var
  c: integer;
begin
  for c := 0 to Handles.Count - 1 do
  begin
    Handles[c].OrX := Handles[c].OrX + ADeltaX;
    Handles[c].OrY := Handles[c].OrY + ADeltaY;
  end;
end;

function TCustomDiagramLine.PointInControl(Pos: TPoint): boolean;
var c: integer;
begin
  result := false;
  RecalcPoints(Handles);
  {Check if point is around one of the lines}
  for c := 1 to Length(FPoints) - 1 do
  begin
    if FPoints[c].X = FPoints[c - 1].X then
      result := AroundVertLine(Pos, FPoints[c].X, FPoints[c].Y, FPoints[c - 1].Y)
    else
    if FPoints[c].Y = FPoints[c - 1].Y then
      result := AroundHorzLine(Pos, FPoints[c].Y, FPoints[c].X, FPoints[c - 1].X)
    else
      result := NearLine(Pos, FPoints[c].X, FPoints[c].Y,
        FPoints[c - 1].X, FPoints[c - 1].Y);
    if result then break;
  end;
end;

procedure TCustomDiagramLine.PaintControl(AInfo: TDiagramDrawInfo);
begin
  {not gdiplus operations}
  InitDrawer(AInfo.Drawer);
  AInfo.Canvas.Pen := CurPen;
  AInfo.Canvas.Brush.Style := bsClear;

  if PathDrawingMode then
  begin
    InitDgrDrawer(AInfo.DgrDrawer);
    AInfo.DgrDrawer.Canvas.Pen.Assign(CurPen);
    AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmClear;
  end;

  DrawLine(AInfo, DiagramDrawLineInfo(Handles, True));
  DrawCells(AInfo);
end;

procedure TCustomDiagramLine.UpdateInserting(AInfo: TControlInsertInfo);

  procedure TempHandlesToCanvas(AHandles: TStretchHandles);
  var
    c: integer;
    P: TPoint;
  begin
    for c := 0 to AHandles.Count - 1 do
    begin
      P := AInfo.Diagram.ClientToCanvas(Point(
        round(AHandles[c].OrX), round(AHandles[c].OrY)));
      AHandles[c].OrX := P.X;
      AHandles[c].OrY := P.Y;
    end;
  end;

var
  ACalcInfo: TCalcHandlesInfo;
begin
  ACalcInfo.Action := chInsert;
  ACalcInfo.P1 := AInfo.P1;
  ACalcInfo.P2 := AInfo.P2;
  ACalcInfo.Anchor1 := AInfo.AnchorPoint1;
  ACalcInfo.Anchor2 := AInfo.AnchorPoint2;
  ACalcInfo.HandleMoved := nil;
  with AInfo do
    case Action of
      iaDrawCursor, iaEraseCursor:
        begin
          if Action = iaDrawCursor then
          begin
            FTempHandles.Assign(Handles);
            CalcNewHandles(FTempHandles, ACalcInfo);
            if AInfo.Diagram <> nil then
              TempHandlesToCanvas(FTempHandles);
          end;
          DrawLineCursor(Canvas, FTempHandles);
        end;
      iaInserted:
        begin
          if Clicked then
            ACalcInfo.P2 := AddPointX(ACalcInfo.P1, Dot(100, 50));
          CalcNewHandles(Handles, ACalcInfo);
          CheckLinkPoints;
          LinkPoints[0].AnchorLink := AnchorPoint1;
          LinkPoints[1].AnchorLink := AnchorPoint2;
        end;
    end;
  if (FLineStyle = lsSideLine) and (AInfo.Action = iaInserted) then
    UpdateOrientations;
end;

procedure TCustomDiagramLine.LoadLinkPointsProp(Reader: TReader);
begin
  Reader.ReadValue;
  Reader.ReadCollection(LinkPoints);
end;

procedure TCustomDiagramLine.StoreLinkPointsProp(Writer: TWriter);
begin
  Writer.WriteCollection(LinkPoints);
end;

procedure TCustomDiagramLine.LoadHandlesProp(Reader: TReader);
begin
  Reader.ReadValue;
  Reader.ReadCollection(Handles);
end;

procedure TCustomDiagramLine.StoreHandlesProp(Writer: TWriter);
begin
  Writer.WriteCollection(Handles);
end;

procedure TCustomDiagramLine.DefineProperties(Filer: TFiler);
begin
  inherited; { allow base classes to define properties }
  Filer.DefineProperty('Handles', LoadHandlesProp, StoreHandlesProp, true);
  Filer.DefineProperty('LinkPoints', LoadLinkPointsProp, StoreLinkPointsProp, true);
end;

function TCustomDiagramLine.GetSourceLinkPoint: TLinkPoint;
begin
  if LinkPoints.Count > 0 then
    result := LinkPoints[0]
  else
    result := nil;
end;

function TCustomDiagramLine.GetTargetLinkPoint: TLinkPoint;
begin
  if LinkPoints.Count > 1 then
    result := LinkPoints[1]
  else
    result := nil;
end;

procedure TCustomDiagramLine.Loaded;
begin
  inherited;
  RecalcAnchors;
end;

procedure TCustomDiagramLine.UpdateDrawing(AInfo: TControlDrawInfo);
begin
end;

procedure TCustomDiagramLine.HandlesChanged;
begin
  if not (csDestroying in ComponentState) then
  begin
    RecalcTextCells;
    NotifyAnchoreds(acPosition);
  end;
end;

procedure TCustomDiagramLine.RecalcTextCells;
const
  AMinimumWidth = 200;
  AMinimumHeight = 30;

  function _FixRect(ARect: TSquare{; AText: string}): TSquare;
  var
    ACurWidth: number;
    ACurHeight: number;
  begin
    result := ARect;
    ACurWidth := result.Right - result.Left;
    if ACurWidth < AMinimumWidth then
    begin
      result.Left := result.Left - (AMinimumWidth - ACurWidth) / 2;
      result.Right := result.Right + (AMinimumWidth - ACurWidth) / 2;
    end;

    {Calculate the number of lines }
    ACurHeight := result.Bottom - result.Top;
    if ACurHeight < AMinimumHeight then
    begin
      result.Top := result.Top - (AMinimumHeight - ACurHeight) / 2;
      result.Bottom := result.Bottom + (AMinimumHeight - ACurHeight) / 2;
    end;
  end;

var
  c: integer;
  ARect: TSquare;
begin
  if FTextCellsMode <> tmManual then
  begin
    RecalcPoints(Handles);
    TextCells.BeginUpdate;
    try
      Case FTextCellsMode of
        tmSegments:
          begin
            {Include as many textcells as the number of lines segments to be drawn}
            while TextCells.Count < (Length(FPoints) - 1) do
              With TextCells.Add do
              begin
                VertAlign := vaCenter;
                WordWrap := true; {WWWW}
                Angle := 0; {WWWW}
                RotateFont := true; {WWWW}
              end;
            while (TextCells.Count > (Length(FPoints) - 1)) and (TextCells.Count > 0) do
              TextCells[TextCells.Count - 1].Free;
            for c := 1 to Length(FPoints) - 1 do
              With TextCells[c - 1] do
              begin
                ARect := _FixRect(RectEx(FPoints[c - 1], FPoints[c]));
                SetBounds(ARect.Left, ARect.Top, ARect.Right - ARect.Left,
                  ARect.Bottom - ARect.Top);
              end;
          end;
        tmSingle:
          begin
            {remove textcells}
            while TextCells.Count > 1 do
              TextCells[TextCells.Count - 1].Free;

            {Include as many textcells as the number of lines segments to be drawn}
            if TextCells.Count = 0 then
              With TextCells.Add do
              begin
                VertAlign := vaCenter;
                WordWrap := true; {WWWW}
                Angle := 0; {WWWW}
                RotateFont := true; {WWWW}
              end;

            PositionCenterCell(TextCells[0]);
            TextCells[0].Clip := false;
          end;
        tmSpecific:
          begin
            CreateSpecificTextCells;
            if (FSourceTextCell <> nil) and (Length(FPoints) >= 2) then
              PositionEndCell(FSourceTextCell, FPoints[1], FPoints[0], true);
            if (FTargetTextCell <> nil) and (Length(FPoints) >= 2) then
              PositionEndCell(FTargetTextCell, FPoints[Length(FPoints) - 2], FPoints[Length(FPoints) - 1], false);

            if (FAltSourceTextCell <> nil) and (Length(FPoints) >= 2) then
              PositionEndCell(FAltSourceTextCell, FPoints[1], FPoints[0], false);
            if (FAltTargetTextCell <> nil) and (Length(FPoints) >= 2) then
              PositionEndCell(FAltTargetTextCell, FPoints[Length(FPoints) - 2], FPoints[Length(FPoints) - 1], true);

            if (FCenterTextCell <> nil) then
              PositionCenterCell(FCenterTextCell);
          end;
      end;
    finally
      TextCells.EndUpdate;
    end;
  end;
end;

procedure TCustomDiagramLine.AddFPoint(X, Y: number);
var
  L: integer;
begin
  L := Length(FPoints);
  SetLength(FPoints, L + 1);
  FPoints[L].X := X;
  FPoints[L].Y := Y;
end;

procedure TCustomDiagramLine.RecalcHandles;
var
  AInfo: TCalcHandlesInfo;
begin
  CheckLinkPoints;
  CheckHandles;
  AInfo.P1 := FHandles[0].DiagramPoint;
  AInfo.P2 := FHandles[FHandles.Count - 1].DiagramPoint;
  AInfo.Anchor1 := FLinkPoints[0].AnchorLink;
  AInfo.Anchor2 := FLinkPoints[1].AnchorLink;
  AInfo.HandleMoved := nil;
  AInfo.DeltaMove := Dot(0, 0);
  AInfo.Action := chNone;
  CalcNewHandles(FHandles, AInfo);
end;

function TCustomDiagramLine.GetLineArrowClass: TLineArrowClass;
begin
  Result := TLineArrow;
end;

procedure TCustomDiagramLine.StartCursorPlacement;
var
  ASwapHandles: TStretchHandles;
begin
  {we must set things so that the line is drawn at the cursor position
   FTempHandles si already calculated by the UpdateMoving and UpdateHandleMoving methods,
   so we just need to recalc text cells.
   One thing that needs to be done is to set FDControl field to nil, so that when
   the FTextCells and FHandles collection are updated, they don't perform extra operation like request redrawing}
  FTextCells.FDControl := nil;
  FHandles.FDControl := nil;
  ASwapHandles := TStretchHandles.Create(nil);
  try
    ASwapHandles.Assign(FHandles);
    FHandles.Assign(FTempHandles);
    FTempHandles.Assign(ASwapHandles);
    RecalcTextCells;
  finally
    ASwapHandles.Free;
    FTextCells.FDControl := Self;
    FHandles.FDControl := Self;
  end;
end;

procedure TCustomDiagramLine.EndCursorPlacement;
var
  ASwapHandles: TStretchHandles;
begin
  {Recover previous handles swapped by StartCursorPlacement}
  FTextCells.FDControl := nil;
  FHandles.FDControl := nil;
  ASwapHandles := TStretchHandles.Create(nil);
  try
    ASwapHandles.Assign(FHandles);
    FHandles.Assign(FTempHandles);
    FTempHandles.Assign(ASwapHandles);
  finally
    ASwapHandles.Free;
    FTextCells.FDControl := Self;
    FHandles.FDControl := Self;
  end;
end;

procedure TCustomDiagramLine.PenChanged;
begin
end;

procedure TCustomDiagramLine.SelPenChanged;
begin
end;

procedure TCustomDiagramLine.SetTextCellsMode(const Value: TTextCellsMode);
begin
  if FTextCellsMode <> Value then
  begin
    if FTextCellsMode = tmSpecific then
      DestroySpecificTextCells;

    FTextCellsMode := Value;

    if FTextCellsMode = tmSpecific then
      CreateSpecificTextCells;
  end;
end;

procedure TCustomDiagramLine.CreateSpecificTextCells;

  function _IsSpecificCell(ACell: TTextCell): boolean;
  begin
    result :=
      (ACell = FCenterTextCell) or
      (ACell = FSourceTextCell) or
      (ACell = FAltSourceTextCell) or
      (ACell = FTargetTextCell) or
      (ACell = FAltTargetTextCell);
  end;

var
  c: integer;
  CanCopy: boolean;
begin
  {workaround to copy text cells. If the number of current text cells is equal
   to the specific text cells, then set a flag to copy the content of those text cells.
   This is useful after loading the line from file or stream, because the text cells
   are not marked as specific}
  CanCopy := (TextCells.Count = 5);
  
  if FCenterTextCell = nil then
  begin
    FCenterTextCell := TextCells.Add;
    if CanCopy then
      FCenterTextCell.Assign(TextCells[0]);
  end;
  if FSourceTextCell = nil then
  begin
    FSourceTextCell := TextCells.Add;
    if CanCopy then
      FSourceTextCell.Assign(TextCells[1]);
  end;
  if FTargetTextCell = nil then
  begin
    FTargetTextCell := TextCells.Add;
    if CanCopy then
      FTargetTextCell.Assign(TextCells[2]);
  end;
  if FAltSourceTextCell = nil then
  begin
    FAltSourceTextCell := TextCells.Add;
    if CanCopy then
      FAltSourceTextCell.Assign(TextCells[3]);
  end;
  if FAltTargetTextCell = nil then
  begin
    FAltTargetTextCell := TextCells.Add;
    if CanCopy then
      FAltTargetTextCell.Assign(TextCells[4]);
  end;

  {Destroy non-specific cells}
  c := 0;
  while c < TextCells.Count do
  begin
    if _IsSpecificCell(TextCells[c]) then
      inc(c)
    else
      TextCells[c].Free;
  end;
end;

procedure TCustomDiagramLine.DestroySpecificTextCells;
begin
  if FCenterTextCell <> nil then
    FCenterTextCell.Free;
  if FSourceTextCell <> nil then
    FSourceTextCell.Free;
  if FTargetTextCell <> nil then
    FTargetTextCell.Free;
  if FAltSourceTextCell <> nil then
    FAltSourceTextCell.Free;
  if FAltTargetTextCell <> nil then
    FAltTargetTextCell.Free;
end;

procedure TCustomDiagramLine.CalcLineNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
begin
  AHandles.BeginUpdate;
  try
    AHandles.Clear;
    AHandles.Add(AInfo.P1.X, AInfo.P1.Y);
    AHandles.Add(AInfo.P2.X, AInfo.P2.Y);
  finally
    AHandles.EndUpdate;
  end;
end;

type
  TSidePoint = record
    P: TDot;
    O: TAnchorOrientation;
  end;

procedure TCustomDiagramLine.CalcSideLineNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
const
  _MinSpace = 10;
var
  TempHandles: TStretchHandles;
  ARecalcLine: boolean;

  function CalcNextDir(S1, S2: TSidePoint): TAnchorOrientation;

    function VertDir(Y: number): TAnchorOrientation;
    begin
      if Y >= 0 then
        result := aoDown
      else
        result := aoUp;
    end;

    function HorzDir(X: number): TAnchorOrientation;
    begin
      if X >= 0 then
        result := aoRight
      else
        result := aoLeft;
    end;

  var
    D: TDot;
  begin
    D := SubPointX(S2.P, S1.P);
    result := S1.O;
    Case S1.O of
      aoNone:
        Case S2.O of
          aoNone:
            if (Abs(D.X) > Abs(D.Y)) then
              result := HorzDir(D.X)
            else
              result := VertDir(D.Y);
          aoHorz:
            result := VertDir(D.Y);
          aoVert:
            result := HorzDir(D.X);
          aoLeft, aoRight: result := VertDir(D.Y);
          aoUp, aoDown: result := HorzDir(D.X);
        end;
      aoHorz: result := HorzDir(D.X);
      aoVert: result := VertDir(D.Y);
    end;
  end;

  function HalfDirect(S1, S2: TSidePoint): boolean;
  var
    D: TDot;
  begin
    S1.O := CalcNextDir(S1, S2);
    S2.O := CalcNextDir(S2, S1);
    D := SubPointX(S2.P, S1.P);
    result :=
      ((S1.O = aoRight) and (D.X > 0) and (S2.O = aoLeft) and (S1.P.Y <> S2.P.Y)) or
      ((S1.O = aoLeft) and (D.X < 0) and (S2.O = aoRight) and (S1.P.Y <> S2.P.Y)) or
      ((S1.O = aoDown) and (D.Y > 0) and (S2.O = aoUp) and (S1.P.X <> S2.P.X)) or
      ((S1.O = aoUp) and (D.Y < 0) and (S2.O = aoDown) and (S1.P.X <> S2.P.X));
  end;

  function IsDirect(S1, S2: TSidePoint): boolean;
  var
    D: TDot;
  begin
    S1.O := CalcNextDir(S1, S2);
    S2.O := CalcNextDir(S2, S1);
    D := SubPointX(S2.P, S1.P);
    result := not HalfDirect(S1, S2) and
      (
      ((S1.O = aoRight) and (D.X > 0)) or
      ((S1.O = aoLeft) and (D.X < 0)) or
      ((S1.O = aoDown) and (D.Y > 0)) or
      ((S1.O = aoUp) and (D.Y < 0))
      );
  end;

  function DoDirect(S1, S2: TSidePoint): TSidePoint;
  begin
    S1.O := CalcNextDir(S1, S2);
    S2.O := CalcNextDir(S2, S1);
    Case S1.O of
      aoLeft, aoRight:
        begin
          S1.P.X := S2.P.X;
          S1.O := aoVert;
        end;
      aoUp, aoDown:
        begin
          S1.P.Y := S2.P.Y;
          S1.O := aoHorz;
        end;
    end;
    result := S1;
  end;

  function GetDeltaHandle(i1: integer; ANegative: boolean; AMaxDelta: number; S1: TSidePoint): TDot;
  var
    i2: integer;
  begin
    With AInfo do
      if (abs(i1) > 0) and (abs(i1) < Self.Handles.Count - 1) then
      begin
        i2 := abs(i1) - sign(i1);
        if (i2 <> 0) and (i2 <> (Self.Handles.Count - 1)) then
          i2 := i2 - sign(i1);
        i1 := abs(i1);

        result := Dot(0, 0);
        if FixedSideLine then
          result := Dot(Self.Handles[i1].OrX - S1.P.X, Self.Handles[i1].OrY - S1.P.Y)
        else
          result := Dot(Self.Handles[i1].OrX - Self.Handles[i2].OrX, Self.Handles[i1].OrY - Self.Handles[i2].OrY);

        if (AInfo.HandleMoved <> nil) and (AInfo.HandleMoved.Index = i1) then
        begin
          result := Dot(Self.Handles[i1].OrX - Self.Handles[i2].OrX, Self.Handles[i1].OrY - Self.Handles[i2].OrY);
          result := AddPointX(result, DeltaMove);
        end;

        if ANegative then
        begin
          if (result.X >= -_MinSpace) and not (S1.O in [aoUp, aoDown, aoVert]) then
          begin
            result.X := -_MinSpace;
            ARecalcLine := true;
          end;
          if (result.Y >= -_MinSpace) and not (S1.O in [aoLeft, aoRight, aoHorz]) then
          begin
            result.Y := -_MinSpace;
            ARecalcLine := true;
          end;
        end else
        begin
          if (result.X <= _MinSpace) and not (S1.O in [aoUp, aoDown, aoVert]) then
          begin
            result.X := _MinSpace;
            ARecalcLine := true;
          end;
          if (result.Y <= _MinSpace) and not (S1.O in [aoLeft, aoRight, aoHorz]) then
          begin
            result.Y := _MinSpace;
            ARecalcLine := true;
          end;
        end;
      end
      else
        if AMaxDelta = MaxInt then
          result := Dot(_MinSpace, _MinSpace)
        else
          result := Dot(AMaxDelta / 2, AMaxDelta / 2);
    result := Dot(Abs(result.X), Abs(result.Y));

    if result.X >=  AMaxDelta then
      result.X := AMaxDelta - 1;
    if result.Y >= AMaxDelta then
      result.Y := AMaxDelta - 1;
    if ANegative then
      result := Dot(-result.X, -result.Y);
  end;

  function DoIndirect(S1, S2: TSidePoint; i1: integer): TSidePoint;
  var
    D: TDot;
    ADelta: TDot;
    MaxDelta: number;
    IsHalf: boolean;
  begin
    S1.O := CalcNextDir(S1, S2);
    S2.O := CalcNextDir(S2, S1);
    D := SubPointX(S2.P, S1.P);
    IsHalf := HalfDirect(S1, S2);
    if IsHalf then
    begin
      if S1.O in [aoLeft, aoRight] then
        MaxDelta := Abs(S1.P.X - S2.P.X) - _MinSpace
      else
        MaxDelta := Abs(S1.P.Y - S2.P.Y) - _MinSpace;
    end
    else
      MaxDelta := MaxInt;

    ADelta := GetDeltaHandle(i1, S1.O in [aoLeft, aoUp], MaxDelta, S1);
    Case S1.O of
      aoLeft, aoRight: S1.P.X := S1.P.X + ADelta.X;
      aoUp, aoDown: S1.P.Y := S1.P.Y + ADelta.Y;
    end;
    if S1.O in [aoLeft, aoRight] then
      S1.O := aoVert
    else
      S1.O := aoHorz;
    result := S1;
  end;

  procedure RemoveDuplicatedPoints(APoints: TDrawPoints);
  var c: integer;
  begin
    c := 1;
    while c < APoints.Count do
    begin
      if (APoints[c].X = APoints[c - 1].X) and (APoints[c].Y = APoints[c - 1].Y) then
        APoints[c].Free
      else
        inc(c);
    end;
  end;

  procedure CalcLinkPoints;
  var
    c: integer;
    S1, S2: TSidePoint;
    i1, i2: integer;
    h1, h2: integer;
    ADone: boolean;
    APoints: TDrawPoints;
  begin
    APoints := TDrawPoints.Create(Self, TDrawPoint);
    try
      S1.P := AInfo.P1;
      S2.P := AInfo.P2;
      if AInfo.Anchor1 <> nil then
        S1.O := AInfo.Anchor1.RealOrientation
      else
        S1.O := Orientation1;
      if AInfo.Anchor2 <> nil then
        S2.O := AInfo.Anchor2.RealOrientation
      else
        S2.O := Orientation2;
      TempHandles.Clear;
      TempHandles.Add(S1.P.X, S1.P.Y);
      TempHandles.Add(S2.P.X, S2.P.Y);
      APoints.Add(S1.P.X, S1.P.Y);
      APoints.Add(S2.P.X, S2.P.Y);
      i1 := 0;
      i2 := 1;
      h1 := 0;
      h2 := 1;
      ADone := false;
      repeat
        if not IsDirect(S1, S2) then
        begin
          inc(i1);
          inc(h1);
          S1 := DoIndirect(S1, S2, h1);
          APoints.Add(S1.P.X, S1.P.Y).Index := i1;
        end else
        if not IsDirect(S2, S1) then
        begin
          S2 := DoIndirect(S2, S1, -(Self.Handles.Count - h2 - 1));
          APoints.Add(S2.P.X, S2.P.Y).Index := APoints.Count - i2 - 1;
          inc(i2);
          inc(h2);
        end else
        begin
          inc(i1);
          S1 := DoDirect(S1, S2);
          APoints.Add(S1.P.X, S1.P.Y).Index := i1;
          ADone := true;
        end;
      until ADone;

      RemoveDuplicatedPoints(APoints);

      {Should always be 2 or more points. But we're gonna check, to avoid runtime errors}
      if (APoints.Count >= 2) then
      begin
        if APoints[0].X = APoints[1].X then
          TempHandles[0].Data := Ord(aoVert)
        else
          TempHandles[0].Data := Ord(aoHorz);
        if APoints[APoints.Count - 1].X = APoints[APoints.Count - 2].X then
          TempHandles[TempHandles.Count - 1].Data := Ord(aoVert)
        else
          TempHandles[TempHandles.Count - 1].Data := Ord(aoHorz);
      end;

      for c := 1 to APoints.Count - 3 do
      begin
        if APoints[c].X = APoints[c + 1].X then
          TempHandles.Add(APoints[c].X, (APoints[c].Y + APoints[c + 1].Y) / 2).Index := c
        else
          TempHandles.Add((APoints[c].X + APoints[c + 1].X) / 2, APoints[c].Y).Index := c;
      end;
    finally
      APoints.Free;
    end;
  end;

  function SameOrientation(O1, O2: TAnchorOrientation): boolean;
  begin
    result := ( (O1 in [aoHorz, aoLeft, aoRight]) and (O2 in [aoHorz, aoLeft, aoRight]) )
      or ( (O1 in [aoVert, aoUp, aoDown]) and (O2 in [aoVert, aoUp, aoDown]) );
  end;

begin
  UpdateOrientations;
  TempHandles := TStretchHandles.Create(nil);
  try
    TempHandles.Assign(AHandles);
    CalcLinkPoints;

    if ARecalcLine then
      CalcLinkPoints;
    AHandles.Assign(TempHandles);
  finally
    TempHandles.Free;
  end;
end;

procedure TCustomDiagramLine.CalcArcNewHandles(AHandles: TStretchHandles;
  AInfo: TCalcHandlesInfo);
var
  H1, H2, HM: TStretchHandle;
  OldDistance: double;

  procedure CalculateIntermediateHandles;
  var
    c: integer;
  begin
    if FRecalcIntermediateArcHandles then
    begin
      for c := 1 to AHandles.Count - 2 do
        if FLineStyle = lsParabolicArc then
          AHandles[c].OrPoint := CalcDistPoint(AInfo.P1, AInfo.P2, MeanPoint(AInfo.P1, AInfo.P2), OldDistance)
        else
          AHandles[c].OrPoint := RotatePointByLines(AHandles[c].OrPoint,
            H1.OrPoint, H2.OrPoint, AInfo.P1, AInfo.P2);
    end;
  end;

begin
  {Be sure that there are three handles in the line. This will probably never happen}
  While AHandles.Count < 3 do
    AHandles.Add(0, 0); //<-- Call Add(0, 0) to set default properties
  while AHandles.Count > 3 do
    AHandles[AHandles.Count - 1].Free;

  H1 := AHandles[0]; {Start point handle}
  H2 := AHandles[2]; {End point handle}
  HM := AHandles[1];

  OldDistance := DistanceFromLine(H1.OrPoint, H2.OrPoint,
    HM.OrPoint);

  With AInfo do
  begin
    Case Action of
      chNone, chInsert:
        HM.OrPoint := CalcDistPoint(P1, P2, MeanPoint(P1, P2), PointDistance(P1, P2) / 4 {just a guess});
      chMove, chAnchorChanged:
          CalculateIntermediateHandles;
      chHandleMove:
          if HandleMoved.Index <> 1 then
            CalculateIntermediateHandles
          else
            if FLineStyle = lsParabolicArc then
              AHandles[HandleMoved.Index].OrPoint := CalcDistPoint(P1, P2, MeanPoint(P1, P2),
                DistanceFromLine(P1, P2, AddPointX(HandleMoved.OrPoint, DeltaMove)))
            else
              AHandles[HandleMoved.Index].OrPoint := AddPointX(HandleMoved.OrPoint, DeltaMove);
    end;
    H1.OrPoint := Dot(P1.X, P1.Y);
    H2.OrPoint := Dot(P2.X, P2.Y);
  end;
end;

procedure TCustomDiagramLine.CalcBezierNewHandles(AHandles: TStretchHandles;
  AInfo: TCalcHandlesInfo);
var
  H1, H2: TStretchHandle;

  procedure CalculateInitialHandles;
  var
    HM1, HM2: TStretchHandle;
  begin
    HM1 := AHandles[1];
    HM2 := AHandles[AHandles.Count - 2];
    HM1.OrPoint := RotatePointByLines(Dot(0, -20), Dot(0, 0), Dot(100, 0), AInfo.P1, AInfo.P2);
    HM2.OrPoint := RotatePointByLines(Dot(100, 20), Dot(0, 0), Dot(100, 0), AInfo.P1, AInfo.P2);
  end;

  procedure CalculateIntermediateHandles;
  var
    c: integer;
  begin
    for c := 1 to AHandles.Count - 2 do
      AHandles[c].OrPoint := RotatePointByLines(AHandles[c].OrPoint,
        H1.OrPoint, H2.OrPoint, AInfo.P1, AInfo.P2);
  end;

begin
  {Be sure that there are four handles in the line}
  While AHandles.Count < 4 do
    AHandles.Add(0, 0); //<-- Call Add(0, 0) to set default properties
  While AHandles.Count > 4 do
    AHandles[AHandles.Count - 1].Free;

  H1 := AHandles[0]; {Start point handle}
  H2 := AHandles[AHandles.Count - 1]; {End point handle}

  With AInfo do
  begin
    Case Action of
      chNone, chInsert:
        CalculateInitialHandles;
      chMove, chAnchorChanged:
        CalculateIntermediateHandles;
      chHandleMove:
        if (HandleMoved.Index = 0) or (HandleMoved.Index = AHandles.Count - 1) then
          CalculateIntermediateHandles
        else
          AHandles[HandleMoved.Index].OrPoint := AddPointX(HandleMoved.OrPoint, DeltaMove);
    end;
    H1.OrPoint := Dot(P1.X, P1.Y);
    H2.OrPoint := Dot(P2.X, P2.Y);
  end;
end;

procedure TCustomDiagramLine.RecalcLinePoints(AHandles: TStretchHandles);
var
  c: integer;
begin
  SetLength(FPoints, AHandles.Count);
  for c := 0 to AHandles.Count - 1 do
  begin
    FPoints[c].X := round(AHandles[c].OrX);
    FPoints[c].Y := round(AHandles[c].OrY);
  end;
end;

procedure TCustomDiagramLine.RecalcSideLinePoints(AHandles: TStretchHandles);
var
  c: integer;
  O: TAnchorOrientation;
begin
  SetLength(FPoints, 0);
  if AHandles.Count >= 2 then
  begin
    AddFPoint(AHandles[0].DiagramPoint.X, AHandles[0].DiagramPoint.Y);
    if AHandles[0].Data = 0 then
      O := Orientation1
    else
      O := TAnchorOrientation(AHandles[0].Data);
    for c := 1 to AHandles.Count - 1 do
      if O in [aoLeft, aoRight, aoHorz] then
      begin
        AddFPoint(AHandles[c].DiagramPoint.X, AHandles[c - 1].DiagramPoint.Y);
        O := aoVert;
      end else
      begin
        AddFPoint(AHandles[c - 1].DiagramPoint.X, AHandles[c].DiagramPoint.Y);
        O := aoHorz;
      end;
    AddFPoint(AHandles[AHandles.Count - 1].DiagramPoint.X,
      AHandles[AHandles.Count - 1].DiagramPoint.Y)
  end;
end;

procedure TCustomDiagramLine.RecalcArcPoints(AHandles: TStretchHandles);
begin
  if AHandles.Count >= 3 then
  begin
    if AroundPoint(RoundPoint(AHandles[0].OrPoint), RoundPoint(AHandles[2].OrPoint)) then
    begin
      SetLength(FPoints, 3);
      FPoints[0] := AHandles[0].OrPoint;
      FPoints[1] := AHandles[1].OrPoint;
      FPoints[2] := AHandles[2].OrPoint;
    end
    else
      Case FLineStyle of
        lsParabolicArc:
          FPoints := ParabolicArc(AHandles[0].OrX, AHandles[0].OrY,
            AHandles[2].OrX, AHandles[2].OrY,
            AHandles[1].OrX, AHandles[1].OrY);
        lsCircularArc:
          FPoints := CircumArc(AHandles[0].OrX, AHandles[0].OrY,
            AHandles[2].OrX, AHandles[2].OrY,
            AHandles[1].OrX, AHandles[1].OrY);
      end;
  end;
end;

procedure TCustomDiagramLine.RecalcBezierPoints(AHandles: TStretchHandles);
begin 
  if AHandles.Count >= 4 then
  begin
    FPoints := Bezier(AHandles[0].OrX, AHandles[0].OrY,
      AHandles[1].OrX, AHandles[1].OrY,
      AHandles[2].OrX, AHandles[2].OrY,
      AHandles[3].OrX, AHandles[3].OrY, 10);
  end;
end;

procedure TCustomDiagramLine.SetOrientation1(
  const Value: TAnchorOrientation);
begin
  if FOrientation1 <> Value then
  begin
    FOrientation1 := Value;
    if not (csLoading in ComponentState) then
      RecalcHandles;
    Redraw;
  end;
end;

procedure TCustomDiagramLine.SetOrientation2(
  const Value: TAnchorOrientation);
begin
  if FOrientation2 <> Value then
  begin
    FOrientation2 := Value;
    if not (csLoading in ComponentState) then
      RecalcHandles;
    Redraw;
  end;
end;

procedure TCustomDiagramLine.UpdateOrientations;
begin
  CheckHandles;
  FOrientation1 := TAnchorOrientation(Handles[0].Data);
  FOrientation2 := TAnchorOrientation(Handles[Handles.Count - 1].Data);
end;

procedure TCustomDiagramLine.InvertOrientation;
var
  h1, h2, hm: TDot;
  nx, ny: double;
  ALineANgle, ADeltaAngle, ANewAngle, ADist: double;
begin
  if (Handles.Count >= 3) and (FLineStyle in [lsParabolicArc, lsCircularArc]) then
  begin
    h1 := Handles[0].DiagramPoint;
    h2 := Handles[2].DiagramPoint;
    hm := Handles[1].DiagramPoint;

    ALineAngle := VectorAngle(h2.x, h2.Y, h1.X, h1.y);
    ADeltaAngle := VectorAngle(hm.x, hm.y, h1.x, h1.y) - ALineAngle;
    ANewAngle := ALineAngle - ADeltaAngle;
    ADist := PointDistance(h1.x, h1.y, hm.x, hm.y);
    MakeVector(h1.x, h1.Y, ADist, ANewAngle, nx, ny);
    Handles[1].OrPoint := Dot(nx, ny);
  end;
end;

procedure TCustomDiagramLine.SetLineStyle(Value: TDiagramLineStyle);
begin
  if FLineStyle <> Value then
  begin
    FLineStyle := Value;
    RecalcHandles;
    Redraw;
  end;
end;

procedure TCustomDiagramLine.CalcNewHandles(AHandles: TStretchHandles;
  AInfo: TCalcHandlesInfo);
begin
  Case FLineStyle of
    lsSideLine:
      CalcSideLineNewHandles(AHandles, AInfo);
    lsParabolicArc:
      CalcArcNewHandles(AHandles, AInfo);
    lsCircularArc:
      CalcArcNewHandles(AHandles, AInfo);
    lsBezier:
      CalcBezierNewHandles(AHandles, AInfo);
  else
    //ltLine:
      CalcLineNewHandles(AHandles, AInfo);
  end;
end;

procedure TCustomDiagramLine.RecalcPoints(AHandles: TStretchHandles);
begin
  Case FLineStyle of
    lsSideLine:
      RecalcSideLinePoints(AHandles);
    lsParabolicArc:
      RecalcArcPoints(AHandles);
    lsCircularArc:
      RecalcArcPoints(AHandles);
    lsBezier:
      RecalcBezierPoints(AHandles);
  else
    //ltLine:
      RecalcLinePoints(AHandles);
  end;
end;

procedure TCustomDiagramLine.TextCellDestroyed(ACell: TTextCell);
begin
  inherited;
  if ACell = FCenterTextCell then
    FCenterTextCell := nil;
  if ACell = FSourceTextCell then
    FSourceTextCell := nil;
  if ACell = FTargetTextCell then
    FTargetTextCell := nil;
  if ACell = FAltSourceTextCell then
    FAltSourceTextCell := nil;
  if ACell = FAltTargetTextCell then
    FAltTargetTextCell := nil;
end;

procedure TCustomDiagramLine.PositionEndCell(ACell: TTextCell; AFrom, ATo: TDot; ANormal: boolean);
const
  AOffset: TDot = (X: 20; Y: 20);
  AWidth = 400;
  AHeight = 100;
var
  P1, P2: TDot;
  Target: TDot;
  CellRect: TSquare;
begin
  {Calculate the position of the text cell corner}
  CalcPerpendicularPoints(P1, P2, AFrom, ATo, AOffset);

  if ANormal then
    Target := P1
  else
    Target := P2;

  if Target.X < ATo.X then
  begin
    ACell.Alignment := taRightJustify;
    CellRect.Right := Target.X;
    CellRect.Left := Target.X - AWidth;
  end
  else
  begin
    ACell.Alignment := taLeftJustify;
    CellRect.Left := Target.X;
    CellRect.Right := Target.X + AWidth;
  end;

  if Target.Y < ATo.Y then
  begin
    ACell.VertAlign := vaBottom;
    CellRect.Bottom := Target.Y;
    CellRect.Top := Target.Y - AHeight;
  end
  else
  begin
    ACell.VertAlign := vaTop;
    CellRect.Top := Target.Y;
    CellRect.Bottom := Target.Y + AHeight;
  end;

  ACell.BoundsRect := CellRect;
end;

procedure TCustomDiagramLine.PositionCenterCell(ACell: TTextCell);

  function _FixRect(ARect: TRect{; AText: string}): TRect;
  const
    AMinimumWidth = 300;
    AMinimumHeight = 100;
  var
    ACurWidth: integer;
    ACurHeight: integer;
  begin
    result := ARect;
    ACurWidth := result.Right - result.Left;
    if ACurWidth < AMinimumWidth then
    begin
      Case ACell.Alignment of
        taLeftJustify:
          begin
            result.Right := result.Right + (AMinimumWidth - ACurWidth);
          end;
        taCenter:
          begin
            result.Left := result.Left - (AMinimumWidth - ACurWidth) div 2;
            result.Right := result.Right + (AMinimumWidth - ACurWidth) div 2;
          end;
        taRightJustify:
          begin
            result.Left := result.Left - (AMinimumWidth - ACurWidth);
          end;
      end;
    end;

    {Calculate the number of lines }
    ACurHeight := result.Bottom - result.Top;
    if ACurHeight < AMinimumHeight then
    begin
      Case ACell.VertAlign of
        vaTop:
          begin
            result.Bottom := result.Bottom + (AMinimumHeight - ACurHeight);
          end;
        vaCenter:
          begin
            result.Top := result.Top - (AMinimumHeight - ACurHeight) div 2;
            result.Bottom := result.Bottom + (AMinimumHeight - ACurHeight) div 2;
          end;
        vaBottom:
          begin
            result.Top := result.Top - (AMinimumHeight - ACurHeight);
          end;
      end;
    end;
  end;

var
  TotalDist: double;
  CalcDist: double;
  RemainingDist: double;
  c: integer;

  MP: TPoint;
  P1, P2: TDot;
  ARect: TRect;
  AAngle: double;
begin
  if Length(FPoints) >= 2 then
  begin
    {Calculate total distance}
    TotalDist := 0;
    for c := 1 to Length(FPoints) - 1 do
      TotalDist := TotalDist + PointDistance(FPoints[c].X, FPoints[c].Y, FPoints[c - 1].X, FPoints[c - 1].Y);
    TotalDist := TotalDist / 2;

    {Calculate last point which surpasses half of total distance}
    CalcDist := 0;
    RemainingDist := 0;
    for c := 1 to Length(FPoints) - 1 do
    begin
      RemainingDist := TotalDist - CalcDist;
      CalcDist := CalcDist + PointDistance(FPoints[c].X, FPoints[c].Y, FPoints[c - 1].X, FPoints[c - 1].Y);
      if CalcDist >= TotalDist then
        break;
    end;

    {Calculate the exact middle point. The point which will be half of the total line distance}
    P1 := FPoints[c - 1];
    P2 := FPoints[c];
    AAngle := VectorAngle(P1.X, P1.Y, P2.X, P2.Y);
    MP.X := Round(P1.X - cos(AAngle) * RemainingDist);
    MP.Y := Round(P1.Y + sin(AAngle) * RemainingDist);

    {Do a small shift in the line position so it's not over the line}
    MP.X := MP.X - round(10 * cos(pi / 2 + AAngle));
    MP.Y := MP.Y + round(10 * sin(pi / 2 + AAngle));

    {Set the cell angle to be according to the line orientation}
    //ACell.Angle := 180 - AAngle * 180 / pi;

    {Warning for the _FixRect: it only works with alignment centered. Extra code is needed for
     it to work with angled cells}
    ARect := _FixRect(Rect(MP.X - 10, MP.Y - 10, MP.X + 10, MP.Y + 10));

    ACell.SetBounds(ARect.Left, ARect.Top, ARect.Right - ARect.Left,
      ARect.Bottom - ARect.Top);
  end;
end;

function TCustomDiagramLine.FixedSideLine: boolean;
begin
  if Diagram <> nil then
    result := Diagram.FixedSideLines
  else
    result := false;
end;

function TCustomDiagramLine.IsSideLine: boolean;
begin
  result := FLineStyle = lsSideLine;
end;

{ TLinkPoints }

function TLinkPoints.Add: TLinkPoint;
begin
  result := TLinkPoint(inherited Add);
end;

function TLinkPoints.Add(AOrX, AOrY: double; AOr: TAnchorOrientation = aoNone): TLinkPoint;
begin
  result := Add;
  With result do
  begin
    FOrX := AOrX;
    FOrY := AOrY;
    FOrientation := AOr;
  end;
end;

function TLinkPoints.GetItem(Index: integer): TLinkPoint;
begin
  result := TLinkPoint(inherited Items[Index]);
end;

procedure TLinkPoints.Update(Item: TCollectionItem);
begin
  if (FDControl <> nil) and (FDControl.FDiagram <> nil) then
    FDControl.FDiagram.Redraw;
end;

{ TLinkPoint }

procedure TLinkPoint.Assign(Source: TPersistent);
begin
   if Source is TLinkPoint then
   begin
     FOrX := TLinkPoint(Source).FOrX;
     FOrY := TLinkPoint(Source).FOrY;
     Changed(false);
   end else
     inherited Assign(Source);
end;

function TLinkPoint.GetDControl: TDiagramControl;
begin
  result := nil;
  if (Collection <> nil) and (Collection is TLinkPoints) then
    result := TLinkPoints(Collection).FDControl;
end;

constructor TLinkPoint.Create(Collection: TCollection);
begin
  inherited;
  FVisible := true;
  FAnchorIndex := -1;
  FOrientation := aoNone;
  FAnchoreds := TList.Create;
  FCollapsable := false;
  FCollapsed := false;
  FLinkConstraint := lcAll;
  FCanAnchor := TLinkPoints(Collection).FDControl is TCustomDiagramLine;
  FCanAnchorInMove := FCanAnchor;
  FAcceptAnchoreds := true;
end;

function TLinkPoint.DiagramPoint: TDot;
begin
  if (DControl <> nil) then
    result := DControl.GetLinkPoint(Self)
  else
    result := Dot(OrX, OrY);
end;

destructor TLinkPoint.Destroy;
begin
  Anchor := nil; {Remove any anchor notification}
  FAnchoreds.Free;
  inherited;
end;

procedure TLinkPoint.AnchorChanged(AChangeType: TAnchorChange);
begin
  Case AChangeType of
    acPosition:
      if DControl <> nil then
        DControl.LinkedAnchorChanged(Self, AChangeType);
    acRemoval:
      begin
        Anchor := nil;
        AnchorIndex := -1;
        if DControl <> nil then
          DControl.LinkedAnchorChanged(Self, AChangeType);
      end;
  end;
end;

function TLinkPoint.GetAnchorLink: TLinkPoint;
begin
  if (Anchor <> nil) and (AnchorIndex >= 0) and
    (AnchorIndex < Anchor.LinkPoints.Count) then
    result := Anchor.LinkPoints[AnchorIndex]
  else
    result := nil;
end;

procedure TLinkPoint.SetAnchorLink(const Value: TLinkPoint);
begin
  if Value <> nil then
  begin
    Anchor := Value.DControl;
    AnchorIndex := Value.Index;
  end else
  begin
    Anchor := nil;
    AnchorIndex := -1;
  end;
end;

procedure TLinkPoint.SetAnchor(const Value: TDiagramControl);
begin
  if Value <> FAnchor then
  begin
    if (FAnchor <> nil) and (FAnchorIndex > -1) and (FAnchorIndex < FAnchor.LinkPoints.Count) then
      FAnchor.RemoveAnchored(Self, FAnchor.LinkPoints[FAnchorIndex]);
    FAnchor := Value;
    if (FAnchor <> nil) and (FAnchorIndex > -1) then
    begin
      if FAnchorIndex < FAnchor.LinkPoints.Count then
        FAnchor.AddAnchored(Self, FAnchor.LinkPoints[FAnchorIndex])
      else
        FAnchorIndex := -1;
    end;
    if (DControl <> nil) and not (csDestroying in DControl.ComponentState) then
      DControl.AnchorLinkChanged(Self);
  end;
end;

procedure TLinkPoint.SetAnchorIndex(const Value: integer);
begin
  if FAnchorIndex <> Value then
  begin
    if (FAnchor <> nil) and (FAnchorIndex > -1) and (FAnchorIndex < FAnchor.LinkPoints.Count) then
      FAnchor.RemoveAnchored(Self, FAnchor.LinkPoints[FAnchorIndex]);
    FAnchorIndex := Value;
    if (FAnchor <> nil) and (FAnchorIndex > -1) then
    begin
      if FAnchorIndex < FAnchor.LinkPoints.Count then
        FAnchor.AddAnchored(Self, FAnchor.LinkPoints[FAnchorIndex])
      else
        FAnchorIndex := -1;
    end;
    if DControl <> nil then
      DControl.AnchorLinkChanged(Self);
  end;
end;

function TLinkPoint.GetStyle: TLinkPointStyle;
begin
  if (DControl <> nil) and (DControl.Diagram <> nil) then
    result := DControl.LinkPointStyle
  else
    result := ptCross;
end;

function TLinkPoint.GetRealOrientation: TAnchorOrientation;

  function GetNextOr(A: array of TAnchorOrientation; Idx: integer): TAnchorOrientation;
  begin
    result := A[Idx - 1];
  end;

var
  AngleStep: integer;
begin
  result := FOrientation;
  if (DControl <> nil) and (DControl.Angle <> 0) then
  begin
    AngleStep := trunc(((DControl.Angle + 45) / 90));
    if AngleStep in [1..3] then
      Case FOrientation of
        aoHorz:  result := GetNextOr([aoVert , aoHorz , aoVert ], AngleStep);
        aoVert:  result := GetNextOr([aoHorz , aoVert , aoHorz ], AngleStep);
        aoUp:    result := GetNextOr([aoRight, aoDown , aoLeft ], AngleStep);
        aoDown:  result := GetNextOr([aoLeft , aoUp   , aoRight], AngleStep);
        aoLeft:  result := GetNextOr([aoUp   , aoRight, aoDown ], AngleStep);
        aoRight: result := GetNextOr([aoDown , aoLeft , aoUp   ], AngleStep);
      end;
  end;
end;

function TLinkPoint.AnchoredCount: integer;
begin
  result := FAnchoreds.Count;
end;

function TLinkPoint.GetAnchoredItem(Index: integer): TLinkPoint;
begin
  {$WARNINGS OFF}
  result := TLinkPoint(FAnchoreds[Index]);
  {$WARNINGS ON}
end;

function TLinkPoint.GetOrPoint: TDot;
begin
  result := Dot(FOrX, FOrY);
end;

procedure TLinkPoint.SetOrPoint(const Value: TDot);
begin
  if (FOrX <> Value.X) or (FOrY <> Value.Y) then
  begin
    FOrX := Value.X;
    FOrY := Value.Y;
  end;
end;

function TLinkPoint.IsNode: boolean;
begin
  result :=
    (AnchoredCount > 0) and
      (Collapsable or
        (
          (DControl <> nil) and (DControl.Diagram <> nil) and (DControl.Diagram.AutomaticNodes)
        )
      );
end;

function TLinkPoint.DoCollapseControls(ACollapse: boolean): boolean;
var
  DControls: TList;
  Canceled: boolean;

  procedure DoCollapseLinkPoint(ALinkPoint: TLinkPoint);

    procedure DoCollapseDControl(ADControl: TDiagramControl);
    var
      c: integer;
    begin
      if ADControl <> nil then
      begin
        {The linkpoint being collapsed/expanded cannot have a connection to the same dcontrol}
        if ADControl = Self.DControl then
          Canceled := true
        else
          if DControls.IndexOf(ADControl) = -1 then
          begin
            DControls.Add(ADControl);
            for c := 0 to ADControl.LinkPoints.Count - 1 do
              if (not Canceled) and (ADControl.LinkPoints[c] <> Self) and not ADControl.LinkPoints[c].Collapsed then
                DoCollapseLinkPoint(ADControl.LinkPoints[c]);
          end;
      end;
    end;

  var
    c: integer;
  begin
    for c := 0 to ALinkPoint.AnchoredCount - 1 do
      if not Canceled then
        DoCollapseDControl(ALinkPoint.Anchoreds[c].DControl);
    if (not Canceled) and (ALinkPoint.AnchorLink <> Self) then
      DoCollapseDControl(ALinkPoint.Anchor);
  end;

var
  c: integer;
begin
  Canceled := false;
  DControls := TList.Create;
  try
    DoCollapseLinkPoint(Self);
    if not Canceled then
      for c := 0 to DControls.Count - 1 do
        {$WARNINGS OFF}
        TDiagramControl(DControls[c]).FCollapsed := ACollapse;
        {$WARNINGS ON}
  finally
    DControls.Free;
  end;
  result := not Canceled;
end;

procedure TLinkPoint.SetCollapsed(const Value: boolean);
begin
  if (DControl <> nil) and not (csLoading in DControl.ComponentState) then
  begin
    if FCollapsed <> (Value and IsNode) then
    begin
      if DoCollapseControls(Value and IsNode) then
      begin
        FCollapsed := (Value and IsNode);
        if DControl <> nil then
          DControl.Redraw;
      end;
    end;
  end else
    FCollapsed := Value
end;

procedure TLinkPoint.Collapse;
begin
  Collapsed := true;
end;

procedure TLinkPoint.Expand;
begin
  Collapsed := false;
end;

procedure TLinkPoint.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
  end;
end;

{ TSnapGrid }

procedure TSnapGrid.Assign(Source: TPersistent);
begin
  if Source is TSnapGrid then
  begin
    FActive := TSnapGrid(Source).FActive;
    FVisible := TSnapGrid(Source).FVisible;
    FSizeX := TSnapGrid(Source).FSizeX;
    FSizeY := TSnapGrid(Source).FSizeY;
    FStyle := TSnapGrid(Source).FStyle;
    {Leave pen assignment at the end, so Changed method is called automatically}
    FPen.Assign(TSnapGrid(Source).FPen);
  end else
    inherited Assign(Source);
end;

procedure TSnapGrid.Changed;
begin
  if Assigned(FDiagram) then
    FDiagram.Redraw;
end;

constructor TSnapGrid.Create(ADiagram: TatDiagram);
begin
  inherited Create;
  FDiagram := ADiagram;
  FActive := false;
  FVisible := false;
  FSizeX := 8;
  FSizeY := 8;
  FPen := TPen.Create;
  FPen.Style := psSolid;
  FPen.Color := clBlack;
  FPen.OnChange := StyleChanged;
  FStyle := gsDots;
end;

destructor TSnapGrid.Destroy;
begin
  FPen.Free;
  inherited;
end;

procedure TSnapGrid.StyleChanged(Sender: TObject);
begin
  Changed;
end;

procedure TSnapGrid.SetActive(const Value: boolean);
begin
  if FActive <> Value then
    FActive := Value;
end;

procedure TSnapGrid.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure TSnapGrid.SetSizeX(const Value: double);
begin
  if FSizeX <> Value then
  begin
    FSizeX := Value;
    Changed;
  end;
end;

procedure TSnapGrid.SetSizeY(const Value: double);
begin
  if FSizeY <> Value then
  begin
    FSizeY := Value;
    Changed;
  end;
end;

procedure TSnapGrid.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

procedure TSnapGrid.Draw(Canvas: TCanvas);
var
  c: double;
  d: double;
  ZoomedSizeX, ZoomedSizeY: double;
begin
  if Assigned(FDiagram) then With FDiagram do
  begin
    ZoomedSizeX := SizeX * ZoomRatio;
    ZoomedSizeY := SizeY * ZoomRatio;
    Case Style of
      gsDots:
        begin
          d := FDiagram.CanvasOffset.Y;
          while d < ClipRect(Canvas).Bottom do
          begin
            c := FDiagram.CanvasOffset.X;
            while c < ClipRect(Canvas).Right do
            begin
              Canvas.Pixels[round(c), round(d)] := FPen.Color;
              c := c + ZoomedSizeX;
            end;
            d := d + ZoomedSizeY;
          end;
        end;
      gsLines:
        begin
          Canvas.Pen := FPen;
          c := FDiagram.CanvasOffset.X;
          while c < ClipRect(Canvas).Right do
          begin
            Canvas.MoveTo(round(c), Round(FDiagram.CanvasOffset.Y));
            Canvas.LineTo(round(c), Round(ClipRect(Canvas).Bottom));
            c := c + ZoomedSizeX;
          end;
          d := FDiagram.CanvasOffset.Y;
          while d < ClipRect(Canvas).Bottom do
          begin
            Canvas.MoveTo(Round(FDiagram.CanvasOffset.X), round(d));
            Canvas.LineTo(Round(ClipRect(Canvas).Right), round(d));
            d := d + ZoomedSizeY;
          end;
        end;
    end;
  end;
end;

procedure TSnapGrid.SetStyle(const Value: TGridStyle);
begin
  if FStyle <> Value then
  begin
    FStyle := Value;
    Changed;
  end;
end;

procedure TSnapGrid.UpdateSize;
begin
  exit; // to be reviewed
  if FSnapToRuler and (FDiagram <> nil) then
  begin
    SizeX := FDiagram.TopRuler.RulerStep;
    SizeY := FDiagram.LeftRuler.RulerStep;
  end;
end;

procedure TSnapGrid.SetSnapToRuler(const Value: boolean);
begin
  if FSnapToRuler <> Value then
  begin
    FSnapToRuler := Value;
    UpdateSize;
  end;
end;

{ TDiagramMemo }

constructor TDiagramMemo.Create(AOwner: TComponent);
begin
  inherited;
  BorderStyle := bsNone;
end;

function TDiagramMemo.DControl: TDiagramControl;
begin
  if FCell <> nil then
    result := FCell.DControl
  else
    result := nil;
end;

procedure TDiagramMemo.KeyDown(var Key: Word; Shift: TShiftState);
var
  ACell: TTextCell;
begin
  inherited KeyDown(Key, Shift);
  Case Key of
    VK_RETURN:
      if ssCtrl in Shift then
        FDiagram.CloseEditor(true);
    VK_TAB:
      begin
        Key := 0;
        if DControl <> nil then
        begin
          ACell := DControl.SelectNextCell(FCell, not (ssShift in Shift));
          if (ACell <> nil) and (ACell <> FCell) then
          begin
            FDiagram.CloseEditor(true);
            FDiagram.EditTextCell(ACell);
          end;
        end;
      end;
  end;
end;

procedure TDiagramMemo.KeyPress(var Key: Char);
begin
  Case Key of
    #27: FDiagram.CloseEditor(false);
    #9: Key := #0;
  end;
  inherited;
end;

function TDiagramMemo.CalculateHeight: integer;
var
  DC: HDC;
  SaveFont: HFont;
  SysMetrics, Metrics: TTextMetric;
  ALineCount: integer;
begin
  DC := GetDC(0);
  GetTextMetrics(DC, SysMetrics);
  SaveFont := SelectObject(DC, Font.Handle);
  GetTextMetrics(DC, Metrics);
  SelectObject(DC, SaveFont);
  ReleaseDC(0, DC);
  ALineCount := Lines.Count;
  if SendMessage(Handle, EM_GETLINECOUNT, 0, 0) <> ALineCount then
  begin
    if CaretPos.Y = ALineCount then
      ALineCount := ALineCount + 1;
  end;
  result := (Metrics.tmHeight * ALineCount);
end;

procedure TDiagramMemo.UpdatePosition;
var
  ARect: TSquare;
  HorzMargin: integer;
  AMemoHeight: number;
  AMemoWidth: number;
  TempRect: TRect;
begin
  if (not FKeepPosition) and (FCell <> nil) and (DControl <> nil) and (DControl.Diagram <> nil) then
  begin
    {Adjust memo position to keep memo text as closer as possible to drawn text}
    ARect := FCell.DiagramRect;
    HorzMargin := SendMessage(Handle, EM_GETMARGINS, 0, 0);
    HorzMargin := HorzMargin and $0000FFFF;
    AMemoWidth := ARect.Right - ARect.Left + 1 + HorzMargin + HorzMargin;
    AMemoHeight := CalculateHeight;
    {CalculateHeight is already in canvas coordinates, so we must convert back to client coordinates}
    TempRect := ToRect(DControl.Diagram.CanvasToClient(Square(0, 0, 0, AMemoHeight)));
    AMemoHeight := TempRect.Bottom - TempRect.Top;

    Case FCell.Alignment of
      taLeftJustify: ARect.Left := ARect.Left - HorzMargin;
      taRightJustify: ARect.Left := ARect.Left + HorzMargin;
      taCenter: ARect.Left := ARect.Left - HorzMargin;
    end;
    ARect.Right := ARect.Left + AMemoWidth;
    Case FCell.VertAlign of
      vaTop: ;
      vaBottom: ARect.Top := ARect.Bottom - AMemoHeight - 2;
      vaCenter: ARect.Top := (ARect.Bottom + ARect.Top) / 2 -
        (AMemoHeight / 2) - 1;
    end;
    ARect.Bottom := ARect.Top + AMemoHeight + 2;
    ARect := DControl.Diagram.ClientToCanvas(ARect);
    SetBounds(Round(ARect.Left), Round(ARect.Top), Round(ARect.Right - ARect.Left),
      Round(ARect.Bottom - ARect.Top));
    ClientWidth := Round(ARect.Right - ARect.Left);
    ClientHeight := Round(ARect.Bottom - ARect.Top);
  end;
end;

procedure TDiagramMemo.Change;
begin
  inherited;
  UpdatePosition;
end;

procedure TDiagramMemo.WndProc(var Msg: TMessage);
var
  ASelStart, ASelLength: integer; 
begin
  Case Msg.Msg of
    WM_MOUSEFIRST..WM_MOUSELAST, WM_KEYFIRST..WM_KEYLAST:
      begin
        ASelStart := SelStart;
        ASelLength := SelLength;
        inherited;
        if (ASelStart <> SelStart) or (ASelLength <> SelLength) then
          UpdatePosition;
      end; 
  else
    inherited;
  end;
end;

{ TDiagramControl }

constructor TDiagramControl.Create(AOwner: TComponent);
begin
  inherited;

  FAutoCreateLinkPoints := false;
  FCollapsed := false;
  FVisible := true;
  FFont := TFont.Create;
  FFont.OnChange := FontChanged;
  FDiagramFont := True;
  FTextCells := TTextCells.Create(Self);
  FCursor := crDefault;
  FParentShowHint := True;
  FInsertStyle := isInsert;
  FHandles := TStretchHandles.Create(Self);
  FLinkPoints := TLinkPoints.Create(Self, GetLinkPointClass);
  FLinkPoints.FDControl := Self;
  FLinkPointPen := TPen.Create;
  FHandlesAppearance := THandlesAppearance.Create;
  with FLinkPointPen do
  begin
    Color := clBlue;
    Style := psSolid;
    OnChange := DrawObjectChanged;
  end;
  FLinkPointBrush := TBrush.Create;
  with FLinkPointBrush do
  begin
    Color := clBlack;
    Style := bsSolid;
    OnChange := DrawObjectChanged;
  end;
  FLinkPointStyle := ptCross;
  FAngle := 0;
  FDrawer := TBlockDrawer.Create;
  FDrawer.OriginalRect := Square(0, 0, 100, 100); //Easy to use original rect
  FTempDrawer := TBlockDrawer.Create;
  FTempDrawer.OriginalRect := Square(0, 0, 100, 100);

  FGraphicLib := DefaultGraphicLib;
  FCustomGraphicLib := False;

  CreateGraphicLibObjects;
end;

procedure TDiagramControl.CreateGraphicLibObjects;
var
  srcRect: TSquare;
begin
  if Assigned(FDgrDrawer) then
    srcRect := FDgrDrawer.SourceRect
  else if Assigned(FDrawer) then
    srcRect := FDrawer.OriginalRect
  else
    srcRect := Square(0, 0, 100, 100); // easy to use original rect
  DestroyGraphicLibObjects;
  if (DgrBlockDrawerClass <> nil) and not (csDestroying in ComponentState) then
  begin
    FDgrDrawer := DgrBlockDrawerClass.Create;
    FTempPath := FDgrDrawer.CreatePath;
    FDgrDrawer.SourceRect := srcRect;
  end;
end;

function TDiagramControl.CreatePath: TDgrGraphicsPath;
begin
  if PathDrawingMode then
    result := FDgrDrawer.CreatePath
  else
    result := nil;
end;

destructor TDiagramControl.Destroy;
begin
  if IsMember then
    GroupBlock.Members.Remove(Self);
    
  if Assigned(Diagram) and Diagram.EditingText and (Diagram.FInplaceMemo.DControl = Self) then
    Diagram.CloseEditor(false);
  Diagram := nil;
  FTextCells.Free;
  FFont.Free;
  FDrawer.Free;
  FTempDrawer.Free;
  FHandlesAppearance.Free;
  FHandles.Free;
  FLinkPoints.Free;
  FLinkPointPen.Free;
  FLinkPointBrush.Free;
  DestroyGraphicLibObjects;
  inherited;
end;

procedure TDiagramControl.DestroyGraphicLibObjects;
begin
  if FDgrDrawer <> nil then
    FreeAndNil(FDgrDrawer);
  if FTempPath <> nil then
    FreeAndNil(FTempPath);
end;

function TDiagramControl.DgrBlockDrawerClass: TDgrBlockDrawerClass;
begin
  result := nil;
  case GraphicLib of
    dglGDIPlus:
      begin
        {$IFDEF GDIPLUS}
        result := TGPBlockDrawer;
        {$ENDIF}
      end;
    {$IFDEF DELPHI2010_LVL}
    dglDirect2D:
      result := TD2DBlockDrawer;
    {$ENDIF}
  end;
end;

procedure TDiagramControl.DrawObjectChanged(Sender: TObject);
begin
  Redraw;
end;

{$IFDEF GDIPLUS}
function TDiagramControl.GetGPDrawer: TGPBlockDrawer;
begin
  // property GPDrawer for GDI+ (backward compatibility)
  if (FDgrDrawer <> nil) and (FDgrDrawer is TGPBlockDrawer) then
    result := TGPBlockDrawer(FDgrDrawer)
  else
    result := nil;
end;
{$ENDIF}

procedure TDiagramControl.IntDiagramChanged(Old, New: TatDiagram);
begin
  DiagramChanged(Old, New);
  if Assigned(Old) then
    Old.RemoveDControl(Self);
    
  {if diagram has changed, then remove reference to current layer object.}
  if Old <> New then
    FLayerObj := nil;
    
  if Assigned(New) then
  begin
    New.AddDControl(Self);
    RestoreLayer;
  end;

  CreateGraphicLibObjects;
end;

procedure TDiagramControl.DiagramChanged(Old, New: TatDiagram);
begin
  if Assigned(New) then
    DiagramFontChanged;
end;

function TDiagramControl.GetHandlePoint(AHandle: TStretchHandle): TDot;
begin
  result := AHandle.OrPoint;
end;

function TDiagramControl.GetParentComponent: TComponent;
begin
  result := nil;
  if FDiagram <> nil then
    Result := FDiagram;
end;

procedure TDiagramControl.SetFont(const Value: TFont);
begin
  FFont.Assign(Value);
end;

procedure TDiagramControl.SetGraphicLib(const Value: TDgrGraphicLib);
begin
  if Value <> GraphicLib then
  begin
    FGraphicLib := Value;
    FCustomGraphicLib := True;
    CreateGraphicLibObjects;
    Redraw;
  end;
end;

procedure TDiagramControl.FontChanged(Sender: TObject);
var
  c: integer;
begin
  FDiagramFont := False;
  for c := 0 to TextCells.Count - 1 do
    TextCells[c].DControlFontChanged;
  Redraw;
end;

procedure TDiagramControl.SetDiagramFont(const Value: boolean);
begin
  if FDiagramFont <> Value then
  begin
    FDiagramFont := Value;
    DiagramFontChanged;
  end;
end;

procedure TDiagramControl.DiagramFontChanged;
begin
  if (FDiagram <> nil) and FDiagramFont then
  begin
    FFont.Assign(FDiagram.Font);
    FDiagramFont := True;
  end;
end;

function TDiagramControl.HasParent: boolean;
begin
  result := True;
end;

procedure TDiagramControl.ReadState(Reader: TReader);
begin
  inherited ReadState(Reader);
  if Reader.Parent is TatDiagram then
    Diagram := TatDiagram(Reader.Parent);
end;

procedure TDiagramControl.SelectedChanged;
begin
end;

procedure TDiagramControl.SetDiagram(const Value: TatDiagram);
var
  AOld: TatDiagram;
begin
  if Value <> FDiagram then
  begin
    AOld := FDiagram;
    FDiagram := Value;
    IntDiagramChanged(AOld, FDiagram);
  end;
end;

procedure TDiagramControl.SetHandles(const Value: TStretchHandles);
begin
  FHandles.Assign(Value);
end;

procedure TDiagramControl.SetParentComponent(AParent: TComponent);
begin
  if not (csLoading in ComponentState) then
    if AParent is TatDiagram then
      Diagram := AParent as TatDiagram
end;

procedure TDiagramControl.SetSelected(Value: boolean);
begin
  if not CanSelect then
    Value := false;
  if (FSelected <> Value) and (not Value or IsVisible) then
  begin
    FSelected := Value;
    if Assigned(FDiagram) then
    begin
      if FSelected then
        FDiagram.DoSelectDControl(Self)
      else
        FDiagram.DoUnselectDControl(Self);

      {If a group is selected, then ensure that all its members are selected. But the parent
       group doesn't change as in previous versions.}
      FDiagram.SetGroupSelected(Self, FSelected);
    end;
    SelectedChanged;
  end;
end;

procedure TDiagramControl.DrawHandles(AInfo: TDiagramDrawInfo);
var
  c: integer;
begin
  for c := 0 to Handles.Count - 1 do
    if Handles[c].Visible then
      DrawHandle(AInfo, Handles[c]);
end;

procedure TDiagramControl.PaintHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle; AAppearance: THandlesAppearance);
var
  P: TPoint;
  S: integer;
  HA: THandleAppearance;
  AEnabled: boolean;
  AVisible: Boolean;
begin
  if Diagram <> nil then
  begin
    AVisible := true;
    case AHandle.Style of
      hsRotate:
        begin
          HA := AAppearance.Rotate;
          AEnabled := CanRotate;
          AVisible := CanRotate;
        end;
      hsCustom:
        begin
          HA := AAppearance.Custom;
          AEnabled := true;
        end;
      hsRotCenter:
        begin
          HA := AAppearance.RotCenter;
          AEnabled := CanMoveRotCenter;
          AVisible := CanRotate;
        end;
    else // hsSize:
      begin
        HA := AAppearance.Size;
        AEnabled := CanResize;
      end;
    end;

    AVisible := AVisible and HA.Visible;
    if not AVisible then Exit;

    if AEnabled then
    begin
      AInfo.Canvas.Brush := HA.Brush;
      AInfo.Canvas.Pen := HA.Pen;
    end
    else
    begin
      AInfo.Canvas.Brush := HA.DisabledBrush;
      AInfo.Canvas.Pen := HA.DisabledPen;
    end;
    
    S := HA.Size;
    P := RoundPoint(Diagram.ClientToCanvas(AHandle.DiagramPoint));
    case HA.Style of
      hhRectangle:
        AInfo.Canvas.Rectangle(Rect(P.X - S, P.Y - S, P.X + S + 1, P.Y + S + 1));
      hhEllipse:
        AInfo.Canvas.Ellipse(P.X - S, P.Y - S, P.X + S + 1, P.Y + S + 1);
      hhRectangleX:
        begin
          AInfo.Canvas.Rectangle(Rect(P.X - S, P.Y - S, P.X + S + 1, P.Y + S + 1));
          AInfo.Canvas.Pen.Color := clGreen;
          AInfo.Canvas.MoveTo(P.X - S, P.Y - S);
          AInfo.Canvas.Lineto(P.X + S, P.Y + S);
          AInfo.Canvas.MoveTo(P.X + S, P.Y - S);
          AInfo.Canvas.Lineto(P.X - S, P.Y + S);
        end;
      hhEllipseX:
        begin
          AInfo.Canvas.Ellipse(P.X - S, P.Y - S, P.X + S + 1, P.Y + S + 1);
          AInfo.Canvas.Pen.Color := clGreen;
          AInfo.Canvas.MoveTo(P.X - S + 1, P.Y - S + 1);
          AInfo.Canvas.Lineto(P.X + S - 1, P.Y + S - 1);
          AInfo.Canvas.MoveTo(P.X + S - 1, P.Y - S + 1);
          AInfo.Canvas.Lineto(P.X - S + 1, P.Y + S - 1);
        end;
      hhCrossedEllipse:
        begin
          AInfo.Canvas.Ellipse(P.X - S, P.Y - S, P.X + S + 1, P.Y + S + 1);
          AInfo.Canvas.MoveTo(P.X, P.Y - S + 2);
          AInfo.Canvas.LineTo(P.X, P.Y + S - 1);
          AInfo.Canvas.MoveTo(P.X - S + 2, P.Y);
          AInfo.Canvas.LineTo(P.X + S - 1, P.Y);
        end;
      hhDiamond:
        AInfo.Canvas.Polygon([
          Point(P.X, P.Y - S),
          Point(P.X + S, P.Y),
          Point(P.X, P.Y + S),
          Point(P.X - S, P.Y),
          Point(P.X, P.Y - S)
          ]);
    end;
  end;
end;

procedure TDiagramControl.DrawHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle);
begin
  GetDefaultHandlesAppearance(FHandlesAppearance);
  PaintHandle(AInfo, AHandle, FHandlesAppearance);
end;


procedure THandleAppearance.Assign(Source: TPersistent);
begin
  if Source is THandleAppearance then
  begin
    Pen := THandleAppearance(Source).Pen;
    Brush := THandleAppearance(Source).Brush;
    DisabledPen := THandleAppearance(Source).DisabledPen;
    DisabledBrush := THandleAppearance(Source).DisabledBrush;
    Size := THandleAppearance(Source).Size;
    Style := THandleAppearance(Source).Style;
    Visible := THandleAppearance(Source).Visible;
  end
  else
    inherited Assign(Source);
end;

constructor THandleAppearance.Create;
begin
  FPen := TPen.Create;
  FDisabledPen := TPen.Create;
  FBrush := TBrush.Create;
  FDisabledBrush := TBrush.Create;
  FVisible := true;
end;

destructor THandleAppearance.Destroy;
begin
  FPen.Free;
  FDisabledPen.Free;
  FBrush.Free;
  FDisabledBrush.Free;
  inherited;
end;

procedure THandleAppearance.SetBrush(const Value: TBrush);
begin
  FBrush.Assign(Value);
end;

procedure THandleAppearance.SetDisabledBrush(const Value: TBrush);
begin
  FDisabledBrush.Assign(Value);
end;

procedure THandleAppearance.SetDisabledPen(const Value: TPen);
begin
  FDisabledPen.Assign(Value);
end;

procedure THandleAppearance.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure THandleAppearance.SetSize(const Value: integer);
begin
  FSize := Value;
end;

procedure THandleAppearance.SetStyle(const Value: THandleAppearanceStyle);
begin
  FStyle := Value;
end;

procedure THandleAppearance.SetVisible(const Value: boolean);
begin
  FVisible := Value;
end;

procedure TDiagramControl.SetIsGdiPlus(const Value: boolean);
begin
  {$IFDEF GDIPLUS}
  if Value then
    GraphicLib := dglGDIPlus
  else
  {$ENDIF}
    GraphicLib := dglGDI;
end;

function TDiagramControl.GetPathDrawingMode: boolean;
begin
  result := GraphicLib <> dglGDI;
end;

{ THandlesAppearance }

procedure THandlesAppearance.Assign(Source: TPersistent);
begin
  if Source is THandlesAppearance then
  begin
    Size := THandlesAppearance(Source).Size;
    Rotate := THandlesAppearance(Source).Rotate;
    Custom := THandlesAppearance(Source).Custom;
    RotCenter := THandlesAppearance(Source).RotCenter;
  end
  else
    inherited Assign(Source);
end;

constructor THandlesAppearance.Create;
begin
  FCustom := THandleAppearance.Create;
  FRotate := THandleAppearance.Create;
  FRotCenter := THandleAppearance.Create;
  FSize := THandleAppearance.Create;
end;

destructor THandlesAppearance.Destroy;
begin
  FCustom.Free;
  FRotate.Free;
  FRotCenter.Free;
  FSize.Free;
  inherited;
end;

procedure THandlesAppearance.SetCustom(const Value: THandleAppearance);
begin
  FCustom.Assign(Value);
end;

procedure THandlesAppearance.SetRotate(const Value: THandleAppearance);
begin
  FRotate.Assign(Value);
end;

procedure THandlesAppearance.SetRotCenter(const Value: THandleAppearance);
begin
  FRotCenter.Assign(Value);
end;

procedure THandlesAppearance.SetSize(const Value: THandleAppearance);
begin
  FSize.Assign(Value);
end;

procedure TDiagramControl.SetRestrictions(
  const Value: TControlRestrictions);
begin
  if FRestrictions <> Value then
  begin
    FRestrictions := Value;
  end;
end;

function TDiagramControl.GetLinkPoint(ALinkPoint: TLinkPoint): TDot;
begin
  result := ALinkPoint.OrPoint;
end;

procedure TDiagramControl.SetLinkPoints(const Value: TLinkPoints);
begin
  FLinkPoints.Assign(Value);
end;

procedure TDiagramControl.MouseDown(Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
end;

procedure TDiagramControl.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
end;

procedure TDiagramControl.MouseUp(Button: TMouseButton; Shift: TShiftState;
  X, Y: Integer);
begin
end;

procedure TDiagramControl.DblClick;
begin
end;

procedure TDiagramControl.Click;
begin
end;

procedure TDiagramControl.DrawLinkPoint(ALinkPoint: TLinkPoint; APoint: TPoint);

  procedure _DrawLinkPoint;
  begin
    with Drawer.Canvas do
    begin
      if ALinkPoint.IsNode then
      begin
        {Set Pen and Brush for node}
        Pen.Style := psSolid;
        Pen.Color := clGray;
        Brush.Style := bsSolid;
        Brush.Color := clWhite;
        Rectangle(APoint.X - 4, APoint.Y - 4, APoint.X + 5, APoint.Y + 5);
        Pen.Color := clBlack;
        {Draw the "-"}
        MoveTo(APoint.X - 2, APoint.Y);
        LineTo(APoint.X + 3, APoint.Y);
        {Draw the "+"}
        if ALinkPoint.Collapsed then
        begin
          MoveTo(APoint.X, APoint.Y - 2);
          LineTo(APoint.X, APoint.Y + 3);
        end;
      end
      else
      begin
        Brush := LinkPointBrush;
        Pen := LinkPointPen;
        case ALinkPoint.Style of
          ptRectangle:
              Rectangle(Rect(APoint.X - 2, APoint.Y - 2, APoint.X + 3, APoint.Y + 3));
          ptDiamond :
            Polygon([
              Point(APoint.X, APoint.Y - 3),
              Point(APoint.X + 3, APoint.Y),
              Point(APoint.X, APoint.Y + 3),
              Point(APoint.X - 3, APoint.Y)]);
          ptEllipse:
            Ellipse(APoint.X - 2, APoint.Y - 2, APoint.X + 3, APoint.Y + 3);
          ptCross:
            begin
              MoveTo(APoint.X - 2, APoint.Y - 2);
              LineTo(APoint.X + 3, APoint.Y + 3);
              MoveTo(APoint.X + 2, APoint.Y - 2);
              LineTo(APoint.X - 3, APoint.Y + 3);
            end;
        end;
      end;
    end;
  end;

  procedure _DrawLinkPointDgr;
  var
    _pen: TPen;
  begin
    with FDgrDrawer.Canvas do
    begin
      if ALinkPoint.IsNode then
      begin
        _pen := TPen.Create;
        try
          _pen.Style := psSolid;
          _pen.Color := clGray;
          Pen.Assign(_pen);
          Brush.BrushMode := bmSolid;
          Brush.Color := clWhite;
          Rectangle(APoint.X - 4, APoint.Y - 4, APoint.X + 5, APoint.Y + 5);
          Pen.Color := clBlack;
          DrawLine(APoint.X - 2, APoint.Y, APoint.X + 3, APoint.Y);
          if ALinkPoint.Collapsed then
            DrawLine(APoint.X, APoint.Y - 2, APoint.X, APoint.Y + 3);
        finally
          _pen.Free;
        end;
      end
      else
      begin
        Brush.Assign(LinkPointBrush);
        Pen.Assign(LinkPointPen);
        case ALinkPoint.Style of
          ptRectangle:
            Rectangle(APoint.X - 2, APoint.Y - 2, APoint.X + 2, APoint.Y + 2);
          ptDiamond:
            Polygon(DotArray([
              Dot(APoint.X, APoint.Y - 3),
              Dot(APoint.X + 3, APoint.Y),
              Dot(APoint.X, APoint.Y + 3),
              Dot(APoint.X - 3, APoint.Y)]));
          ptEllipse:
            Ellipse(APoint.X - 2, APoint.Y - 2, APoint.X + 2, APoint.Y + 2);
          ptCross:
            begin
              DrawLine(APoint.X - 2, APoint.Y - 2, APoint.X + 2, APoint.Y + 2);
              DrawLine(APoint.X + 2, APoint.Y - 2, APoint.X - 2, APoint.Y + 2);
            end;
        end;
      end;
    end;
  end;

var
  DrawingDone: boolean;
begin
  DrawingDone := false;
  if Assigned(FDiagram) then
    FDiagram.DoDrawLinkPoint(Drawer.Canvas, ALinkPoint, APoint, DrawingDone);
  if not DrawingDone and ALinkPoint.Visible then
  begin
    if PathDrawingMode then
      _DrawLinkPointDgr
    else
      _DrawLinkPoint;
  end;
end;

procedure TDiagramControl.DrawLinkPoints(ACanvas: TCanvas; APaintEngine: TDgrPaintEngine; OnlyNodes: boolean = false);
var
  c: integer;
begin
  if (FDiagram.ShowLinkPoints or OnlyNodes) and (FDiagram.FPaintStyle = psScreen) then
  begin
    if PathDrawingMode then
    begin
      FDgrDrawer.PaintEngine := APaintEngine;
      FDgrDrawer.Canvas.SaveDrawingState;
      APaintEngine.ResetTransform;
    end;
    Drawer.Canvas := ACanvas;
    try
      for c := 0 to LinkPoints.Count - 1 do
        if not (Self is TCustomDiagramLine) then
          if not OnlyNodes or LinkPoints[c].IsNode then
            DrawLinkPoint(LinkPoints[c], RoundPoint(FDiagram.ClientToCanvas(LinkPoints[c].DiagramPoint)));
    finally
      if PathDrawingMode then
      begin
        FDgrDrawer.Canvas.RestoreDrawingState;
        FDgrDrawer.PaintEngine := nil;
      end;
    end;
  end;
end;

procedure TDiagramControl.SetLinkPointBrush(const Value: TBrush);
begin
  FLinkPointBrush.Assign(Value);
end;

procedure TDiagramControl.SetLinkPointPen(const Value: TPen);
begin
  FLinkPointPen.Assign(Value);
end;

procedure TDiagramControl.Redraw;
begin
  if Assigned(FDiagram) then
    FDiagram.Redraw;
end;

procedure TDiagramControl.NotifyAnchoreds(AChange: TAnchorChange);
var
  c, d: integer;
  ATempList: TList;
begin
  if FNotifying = 0 then
  begin
    {Use a temporary copied list because while calling AnchorChanged
     the FAnchoreds list might change}
    ATempList := TList.Create;
    {Avoid recursive/cyclic notifications}
    BeginNotifying;
    try
      ATempList.Clear;
      for c := 0 to LinkPoints.Count - 1 do
        for d := 0 to LinkPoints[c].FAnchoreds.Count - 1 do
          ATempList.Add(LinkPoints[c].FAnchoreds[d]);
      for c := 0 to ATempList.Count - 1 do
        {$WARNINGS OFF}
        TLinkPoint(ATempList[c]).AnchorChanged(AChange);
        {$WARNINGS ON}
    finally
      EndNotifying;
      ATempList.Free;
    end;
  end;
end;

procedure TDiagramControl.AddAnchored(AnchorLinkPoint,
  ControlLinkPoint: TLinkPoint);
begin
  ControlLinkPoint.FAnchoreds.Add(AnchorLinkPoint);
  if (AnchorLinkPoint.DControl <> nil) and
   (AnchorLinkPoint.DControl is TCustomDiagramLine) then
    DoLinkAdded(TCustomDiagramLine(AnchorLinkPoint.DControl), AnchorLinkPoint);
end;

procedure TDiagramControl.RemoveAnchored(AnchorLinkPoint,
  ControlLinkPoint: TLinkPoint);
begin
  ControlLinkPoint.FAnchoreds.Remove(AnchorLinkPoint);
  if (AnchorLinkPoint.DControl <> nil) and
    (AnchorLinkPoint.DControl is TCustomDiagramLine) then
    DoLinkRemoved(TCustomDiagramLine(AnchorLinkPoint.DControl), AnchorLinkPoint);
end;

procedure TDiagramControl.DoLinkAdded(ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);
begin
  if Assigned(FOnLinkAdded) then
    FOnLinkAdded(Self, ALink, ALinkPoint);
  if Assigned(FDiagram) then
    FDiagram.DoLinkAdded(Self, ALink, ALinkPoint);
end;

procedure TDiagramControl.DoLinkRemoved(ALink: TCustomDiagramLine; ALinkPoint: TLinkPoint);
begin
  if Assigned(FOnLinkRemoved) then
    FOnLinkRemoved(Self, ALink, ALinkPoint);
  if Assigned(FDiagram) then
    FDiagram.DoLinkRemoved(Self, ALink, ALinkPoint);
end;

function TDiagramControl.MovedLinkPoint(ALinkPoint: TLinkPoint; ADelta: TDot): TDot;
begin
  if ALinkPoint <> nil then
    result := AddPointX(ALinkPoint.DiagramPoint, ADelta)
  else
    result := DummyPointX;
end;

procedure TDiagramControl.SetAngle(const Value: double);
var
  AOld: double;
begin
  if FAngle <> Value then
  begin
    AOld := FAngle;
    FAngle := Value;
    while FAngle < 0 do
      FAngle := FAngle + 360;
    while FAngle >= 360 do
      FAngle := FAngle - 360;
    AngleChangedEx(AOld, FAngle);
    Redraw;
  end;
end;

procedure TDiagramControl.AngleChangedEx(AOld, ANew: double);
begin
  AngleChanged;
end;

procedure TDiagramControl.AngleChanged;
var
  c: integer;
begin
  for c := 0 to TextCells.Count - 1 do
    TextCells[c].DControlAngleChanged;
  if Diagram <> nil then
    Diagram.UpdateSelectionBlockAngle;
end;
            
function TDiagramControl.IsAngleStored: Boolean;
begin
  result := Angle <> 0;
end;

procedure TDiagramControl.Paint(Canvas: TCanvas; ADgrPaintEngine: TDgrPaintEngine);
begin
  {Init GDI and non-GDI drawers}
  if PathDrawingMode then
    FDgrDrawer.PaintEngine := ADgrPaintEngine;

  Drawer.Canvas := Canvas;
  try
    PaintControl(GetDiagramDrawInfo);
    DrawLinkPoints(Canvas, ADgrPaintEngine);
  finally
    if PathDrawingMode then
      FDgrDrawer.PaintEngine := nil;
  end;
end;

procedure TDiagramControl.LoadDControlIDProp(Reader: TReader);
begin
  FDControlID := Reader.ReadString;
end;

procedure TDiagramControl.StoreDControlIDProp(Writer: TWriter);
begin
  Writer.WriteString(FDControlID);
end;

procedure TDiagramControl.LoadCollapsedProp(Reader: TReader);
begin
  FCollapsed := Reader.ReadBoolean; 
end;

procedure TDiagramControl.StoreCollapsedProp(Writer: TWriter);
begin
  Writer.WriteBoolean(FCollapsed);
end;

procedure TDiagramControl.DefineProperties(Filer: TFiler);
begin
  inherited;
  Filer.DefineProperty('DControlID', LoadDControlIDProp, StoreDControlIDProp,
    DControlID <> ClassName);
  Filer.DefineProperty('Collapsed', LoadCollapsedProp, StoreCollapsedProp, true);
end;

function TDiagramControl.GetAsMember: boolean;
begin
  result := IsMember and GroupBlock.Selected;
end;

function TDiagramControl.GetDControlID: string;
begin
  if FDControlID <> '' then
    result := FDControlID
  else
    result := ClassName;
end;

procedure TDiagramControl.GetDefaultHandlesAppearance(
  AAppearance: THandlesAppearance);
var
  HS: TDiagramHandlesStyle;
begin
  if Diagram <> nil then
    HS := Diagram.HandlesStyle
  else
    HS := hsVisio;

  Case HS of
    hsClassic:
      begin
        with FHandlesAppearance.Size do
        begin
          Brush.Style := bsSolid;
          Brush.Color := clBlack;
          Pen.Style := psSolid;
          Pen.Color := clBlack;
          Pen.Width := 1;

          DisabledBrush.Style := bsSolid;
          DisabledBrush.Color := clGray;
          DisabledPen.Style := psSolid;
          DisabledPen.Color := clGray;
          DisabledPen.Width := 1;

          Size := 2;
          Style := hhRectangle;
        end;
        with FHandlesAppearance.Rotate do
        begin
          Brush.Color := clBlack;
          Brush.Style := bsClear;
          Pen.Color := clBlack;
          Pen.Style := psSolid;
          Pen.Width := 1;
          Size := 2;
          Style := hhEllipse;
        end;

        FHandlesAppearance.RotCenter.Visible := false;

        with FHandlesAppearance.Custom do
        begin
          Brush.Color := clYellow;
          Brush.Style := bsSolid;
          Pen.Color := clBlack;
          Pen.Style := psSolid;
          Pen.Width := 1;
          Size := 3;
          Style := hhDiamond;
        end;
      end;
    hsVisio:
      begin
        with FHandlesAppearance.Size do
        begin
          Brush.Style := bsSolid;
          Brush.Color := clLime;
          Pen.Style := psSolid;
          Pen.Color := clBlack;
          Pen.Width := 1;

          DisabledBrush.Style := bsSolid;
          DisabledBrush.Color := clSilver;
          DisabledPen.Style := psSolid;
          DisabledPen.Color := clBlack;
          DisabledPen.Width := 1;

          Size := 3;
          Style := hhRectangle;

          if IsMember then
          begin
            Pen.Color := clGray;
            Style := hhRectangleX;
          end;
        end;
        with FHandlesAppearance.Rotate do
        begin
          Brush.Color := clLime;
          Pen.Color := clBlack;
          Brush.Style := bsSolid;
          Pen.Style := psSolid;
          Pen.Width := 1;
          Size := 4;
          Style := hhEllipse;

          if IsMember then
          begin
            Pen.Color := clGray;
            Style := hhEllipseX;
          end;
        end;
        with FHandlesAppearance.RotCenter do
        begin
          Brush.Style := bsSolid;
          Brush.Color := clLime;
          Pen.Color := clBlack;
          Pen.Style := psSolid;
          Pen.Width := 1;

          DisabledBrush.Style := bsSolid;
          DisabledBrush.Color := clSilver;
          DisabledPen.Color := clBlack;
          DisabledPen.Style := psSolid;
          DisabledPen.Width := 1;
          Size := 3;
          Style := hhCrossedEllipse;

          if IsMember then
            Pen.Color := clGray;
        end;
        with FHandlesAppearance.Custom do
        begin
          Brush.Color := clYellow;
          Brush.Style := bsSolid;
          Pen.Color := clBlack;
          Pen.Style := psSolid;
          Pen.Width := 1;
          Size := 3;
          Style := hhDiamond;
        end;
      end;
  end;
end;

function TDiagramControl.GetDiagramDrawInfo: TDiagramDrawInfo;
begin
  result := DiagramDrawInfo(Drawer.Canvas, Drawer, FDgrDrawer);
end;

procedure TDiagramControl.SetLinkPointStyle(const Value: TLinkPointStyle);
begin
  if FLinkPointStyle <> Value then
  begin
    FLinkPointStyle := Value;
    Redraw;
  end;
end;

procedure TDiagramControl.SetOriginalRect(const Value: TSquare);
begin
  Drawer.OriginalRect := Value;
  if FDgrDrawer <> nil then
    FDgrDrawer.SourceRect := Value;
end;

procedure TDiagramControl.BringToFront;
begin
  if FDiagram <> nil then
    FDiagram.BringToFront(Self);
end;

procedure TDiagramControl.SendToBack;
begin
  if FDiagram <> nil then
    FDiagram.SendToBack(Self);
end;

function TDiagramControl.InsertMouseState: TMouseState;
begin
  Case FInsertStyle of
    isDraw:
      result := msToDraw;
  else
    result := msToInsert;
  end
end;

function TDiagramControl.IsShowHintStored: Boolean;
begin
  Result := not ParentShowHint;
end;

procedure TDiagramControl.SetShowHint(const Value: boolean);
begin
  if FShowHint <> Value then
  begin
    FShowHint := Value;
    FParentShowHint := False;
  end;
end;

procedure TDiagramControl.SetParentShowHint(const Value: Boolean);
begin
  if FParentShowHint <> Value then
  begin
    FParentShowHint := Value;
    DiagramShowHintChanged;
  end;
end;

procedure TDiagramControl.DiagramShowHintChanged;
begin
  if FParentShowHint and (FDiagram <> nil) then
  begin
    SetShowHint(FDiagram.ShowHint);
    FParentShowHint := True;
  end;
end;

procedure TDiagramControl.WriteText(Canvas: TCanvas; AFont: TFont; ARect: TSquare;
  const S: string; HAlign: TAlignment; VAlign: TVertAlign;
  AWrap: boolean; AAngle: double = 0; CX: number = 0; CY: number = 0; IgnoreZoom: boolean = false);
var
  DummyRect: TSquare;
begin
  InternalWriteText(Canvas, AFont, ARect, S, HAlign, VAlign, AWrap, AAngle, CX, CY, true, DummyRect, IgnoreZoom);
end;

function TDiagramControl.CalculateTextRect(Canvas: TCanvas; AFont: TFont; ARect: TSquare;
  const S: string; HAlign: TAlignment; VAlign: TVertAlign;
  AWrap: boolean; AAngle: double = 0; CX: number = 0; CY: number = 0; IgnoreZoom: boolean = false): TSquare;
begin
  InternalWriteText(Canvas, AFont, ARect, S, HAlign, VAlign, AWrap, AAngle, CX, CY, false, result, IgnoreZoom);
end;

function TDiagramControl.InternalWriteTextDgr(ADrawer: TDgrBlockDrawer; AFont: TFont;
  ARect: TSquare; const S: string; HAlign: TAlignment; VAlign: TVertAlign;
  AWrap: boolean; AAngle: double; RotC: TDot; AClip, ADrawFrame,
  ADrawText: boolean; ATransparency, AFrameMargin: integer): TSquare;

var
  gState2: integer;
  strBounds, maxRect: TSquare;
  rightToLeft: boolean;
begin
  {Even if S is empty, we should continue drawing, because this function also draws the
   background and pen for the rect}
  {if S = '' then
  begin
    result := Rect(0, 0, 0, 0);
    Exit;
  end;}

  {Save current state and restore later, because we're going to change transformation matrix}
  ADrawer.Canvas.SaveDrawingState;

  {There is an odd behavior (windows bug?) when using mixed gdiplus and gdi: after calling DrawString in gdiplus,
   further calls to ExtTextOut (gdi) outputs the text wrongly. It looks like the gdiplus methods affects the gdi text output.
   So, we need to save (and then restore) the state of the gdi canvas so that we can do ExtTextOut gdi calls even after
   using gdiplus. This is needed because we can have blocks being drawn by gdiplus functions, and others by gdi functions}
  gState2 := SaveDC(Drawer.Canvas.Handle);
  try
    {Set world transformations according to rotation center and angle.
     Rotate the graphic device. We must prepend the transformation, becauase the graphics device is
     already transformed because of the zoom}
    ADrawer.Canvas.RotateTransform(AAngle, RotC);
    rightToLeft := (Diagram <> nil) and Diagram.IsRightToLeft;

    {Clip the frame}
    if AClip then
      ADrawer.Canvas.SetClip(ARect)
    else
    begin
      if AWrap then
        maxRect := Square(0, 0, ARect.Right - ARect.Left, MaxInt)
      else
        maxRect := Square(0, 0, MaxInt, MaxInt);
      strBounds := ADrawer.Canvas.StringBounds(S, AFont, maxRect, HAlign, VAlign, AWrap, rightToLeft);

      {workaround clipping: adjust rect size}
      if (strBounds.Bottom - strBounds.Top) > (ARect.Bottom - ARect.Top) then
      begin
        case VAlign of
          vaTop:
            ARect.Top := ARect.Top - ((strBounds.Bottom - strBounds.Top) - (ARect.Bottom - ARect.Top));
          vaCenter:
            ARect.Top := ARect.Top - ((strBounds.Bottom - strBounds.Top) - (ARect.Bottom - ARect.Top)) / 2;
        end;
        ARect.Bottom := ARect.Top + (strBounds.Bottom - strBounds.Top);
      end;
      if not AWrap and ((strBounds.Right - strBounds.Left) > (ARect.Right - ARect.Left)) then
      begin
        case HAlign of
          taLeftJustify:
            ARect.Left := ARect.Left - ((strBounds.Right - strBounds.Left) - (ARect.Right - ARect.Left));
          taCenter:
            ARect.Left := ARect.Left - ((strBounds.Right - strBounds.Left) - (ARect.Right - ARect.Left)) / 2;
        end;
        ARect.Right := ARect.Left + (strBounds.Right - strBounds.Left);
      end;
    end;

    {Measure the string}
    strBounds := ADrawer.Canvas.StringBounds(S, AFont, ARect, HAlign, VAlign, AWrap, rightToLeft);

    result := strBounds;

    {Draw the frame}
    if ADrawFrame then
      ADrawer.Canvas.Rectangle(ARect.Left, ARect.Top, ARect.Right, ARect.Bottom);

    {Draw the string with transparent brush}

    if ADrawText then
    begin
      if AFrameMargin > 0 then
        with ARect do
          ARect := Square(Left + AFrameMargin, Top + AFrameMargin, Right - AFrameMargin, Bottom - AFrameMargin);
      ADrawer.Canvas.DrawString(S, AFont, ARect, HAlign, VAlign, AWrap, rightToLeft, ATransparency);
    end;
  finally
    {Restore the state}
    if AClip then
      ADrawer.Canvas.ResetClip;
    ADrawer.Canvas.RestoreDrawingState;
    RestoreDC(Drawer.Canvas.Handle, gState2);
  end;
end;

procedure TDiagramControl.InternalWriteText(Canvas: TCanvas; AFont: TFont; ARect: TSquare;
  const S: string; HAlign: TAlignment; VAlign: TVertAlign;
  AWrap: boolean; AAngle: double; CX: number; CY: number; DrawText: boolean;
  var NewRect: TSquare; IgnoreZoom: boolean);
var
  SL: TStrings;

  function RotateFont(AFont: TFont; AAngle: double): HFont;
  var
    F: TLogFont;
  begin
    AAngle := -AAngle;
    while AAngle < 0 do AAngle := AAngle + 360;
    while AAngle > 360 do AAngle := AAngle - 360;
    {$WARNINGS OFF}
    GetObject(AFont.Handle, SizeOf(TLogFont), @F);
    {$WARNINGS ON}
    F.lfEscapement := round(AAngle * 10);
    F.lfOrientation := round(AAngle * 10);
    Result := CreateFontIndirect(F);
  end;

  procedure WrapText(SL: TStrings; AW: number);

    function WrapLine(AText: string; I: integer): integer;
    var
      CurW, P: Integer;
      ALine: String;
      c: integer;
    begin
      if AText = '' then
        c := 1
      else
        c := 0;
      while AText <> '' do
      begin
        ALine := AText;
        AText := '';
        CurW := Canvas.TextWidth(ALine);
        while CurW > AW do
        begin
          {wrap words}
          P := Length(ALine);
          while (P >= 1) and (ALine[P] <> ' ') do
            dec(P);
          if P > 0 then
          begin
            if AText <> '' then
              AText := ' ' + AText;
            AText := Copy(ALine, P + 1, MaxInt) + AText;
            ALine := Copy(ALine, 1, P - 1);
          end else
          begin
            {wrap chars}
            P := Length(ALine) - 1;
            while (P >= 1) and (Canvas.TextWidth(Copy(ALine, 1, P)) > AW) do
              dec(P);
            if P > 0 then
            begin
              if AText <> '' then
                AText := ' ' + AText;
              AText := Copy(ALine, P + 1, MaxInt) + AText;
              ALine := Copy(ALine, 1, P);
            end else
            begin
              if (AText <> '') then
                AText := ' ' + AText;
              ALine := ALine + AText;
              AText := '';
              AW := MaxInt;
            end;
          end;
          CurW := Canvas.TextWidth(ALine);
        end;
        if (c = 0) then
          SL[I] := ALine
        else
          SL.Insert(I + c, ALine);
        inc(c);
      end;
      result := c;
    end;

  var
    i: integer;
  begin
    i := 0;
    while i < SL.Count do
      i := i + WrapLine(SL[i], i);
  end;

  procedure OutTextLines;
  var
    TM : TTextMetric;
    dy: integer; {distance between one line to another}
    totaly: integer; {total height of text}
    cury: number; {y-coord of current line}
    curx: number; {x-coord of current line}
    P: TDot;
    ARotate: boolean;
    NewFont, OldFont: HFONT;
    //OldOriginalRect: TRect;

    procedure DrawTextLines;
    var
      c: integer;
    begin
      {Get initial y and delta y}
      dy := TM.tmHeight;
      totaly := dy * SL.Count;
      Case VAlign of
        vaBottom:
          cury := ARect.Bottom - totaly;
        vaCenter:
          cury := ((ARect.Bottom - ARect.Top - totaly) / 2) + ARect.Top;
      else {vaTop}
        cury := ARect.Top;
      end;

      {Set the initial y for the text}
      NewRect.Top := cury;

      {Go through lines, calculating X and then outputing text}
      for c := 0 to SL.Count - 1 do
      begin
        Case HAlign of
          taRightJustify:
            curx := ARect.Right - Canvas.TextWidth(SL[c]) + 1;
          taCenter:
            curx := ((ARect.Right - ARect.Left - Canvas.TextWidth(SL[c])) / 2)
              + ARect.Left;
        else {taLeftJustify}
          curx := ARect.Left;
        end;

        P := Dot(curx, cury);

        {calculate the rect around the text}
        if curx < NewRect.Left then
          NewRect.Left := curx;
        if curx + Canvas.TextWidth(SL[c]) > NewRect.Right then
          NewRect.Right := curx + Canvas.TextWidth(SL[c]);

        if ARotate then
          P := FTempDrawer.RotX(P);

        if DrawText then
          Canvas.TextOut(Round(P.X), Round(P.Y), SL[c]);

        cury := cury + dy;
        NewRect.Bottom := cury;
      end;
    end;


  begin
    GetTextMetrics(Canvas.Handle, TM);
    ARotate := (AAngle <> 0) and ((TM.tmPitchAndFamily and TMPF_TRUETYPE) <>0);

    {Rotate font}
    if ARotate then
    begin
      NewFont := RotateFont(Canvas.Font, AAngle);
      OldFont :=  SelectObject(Canvas.Handle, NewFont);
      FTempDrawer.Angle := AAngle;
      //OldOriginalRect := FDrawer.OriginalRect;
      //FDrawer.OriginalRect := ARect;
      FTempDrawer.CurRect := ARect;
      FTempDrawer.RotationCenter := Dot(CX, CY);
      FTempDrawer.RotationType := rtRotationCenter;
      //FDrawer.OriginalRect := OldOriginalRect;
      try
        DrawTextLines;
      finally
        SelectObject(Canvas.Handle, OldFont);
        DeleteObject(NewFont);
      end;
    end else
      DrawTextLines;
  end;

  procedure UpdateFontForPreview(AFont: TFont);
  {var
    FontSize: Integer;}
  begin
    {The code below is used when previewing is done using bitmaps (see DiagramPreview.pas).
     Current behaviour = bitmap}
    if FDiagram <> nil then
      AFont.Size := round(AFont.Size * FDiagram.FPrintConvY * FDiagram.FAdjustRatioY);

    {The code below is used when previewing is done using metafiles (see DiagramPreview.pas)}
    {if (FDiagram <> nil) and FDiagram.HasPrinter then
    begin
      if GetDeviceCaps(Printer.Handle, LOGPIXELSY) <> AFont.PixelsPerInch then
      begin
        FontSize := AFont.Size;
        AFont.PixelsPerInch := GetDeviceCaps(Printer.Handle, LOGPIXELSY);
        AFont.Size := FontSize;
      end;
    end;}
  end;

begin
  FTempDrawer.PushState;
  ARect := RectEx(ARect.TopLeft, ARect.BottomRight);

  ARect.Left := ARect.Left + 0;
  ARect.Top := ARect.Top + 0;
  ARect.Right := ARect.Right + 0;
  ARect.Bottom := ARect.Bottom + 0;

  {NewRect is the calculated Rect which surrounds the written text}
  NewRect.Left := MaxInt;
  NewRect.Right := -MaxInt;
  NewRect.Top := MaxInt;
  NewRect.Bottom := -MaxInt;

  SL := TStringList.Create;
  try
    Canvas.Font := AFont;
    if (FDiagram <> nil) and (FDiagram.ZoomRatio <> 1) and not IgnoreZoom then
      Canvas.Font.Size := round(Canvas.Font.Size * FDiagram.ZoomRatio);

    if (FDiagram <> nil) and (FDiagram.FPaintStyle = psPreview) then
      UpdateFontForPreview(Canvas.Font);

    SL.Text := S;
    if AWrap then
      WrapText(SL, ARect.Right - ARect.Left);

    OutTextLines;
  finally
    SL.Free;
  end;

  if (NewRect.Left = MaxInt) or (NewRect.Right = -MaxInt) or (NewRect.Top = MaxInt) or (NewRect.Bottom = -MaxInt) then
    NewRect := ARect;
    
  FTempDrawer.PopState;
end;

function TDiagramControl.IsFontStored: Boolean;
begin
  result := not DiagramFont;
end;

procedure TDiagramControl.Loaded;
begin
  inherited;
  DiagramFontChanged;
  AngleChanged;
end;

procedure TDiagramControl.DrawCells(AInfo: TDiagramDrawInfo);
var
  c: integer;
  oldBrushStyle: TBrushStyle;
begin
  if FDiagram <> nil then
  begin
    oldBrushStyle := AInfo.Canvas.Brush.Style;
    AInfo.Canvas.Brush.Style := bsClear;
    for c := 0 to TextCells.Count - 1 do
      if TextCells[c].Visible and not (FDiagram.EditingText and (FDiagram.FInplaceMemo.FCell = TextCells[c])) then
        DrawCell(AInfo, TextCells[c]);
    AInfo.Canvas.Brush.Style := oldBrushStyle;
  end;
end;

procedure TDiagramControl.DrawCell(AInfo: TDiagramDrawInfo; ACell: TTextCell);

  procedure _DrawCell;
  var
    RotCenter: TDot;
    AAngle: double;
    ARect: TSquare;
    CellRect: TSquare;
    ClipRgn: HRGN;
    Pts: TPointArray;
    SaveBrush: TBrush;
    SavePen: TPen;
    FrameRect: TSquare;
  begin
    SetLength(Pts, 0);
    if FDiagram <> nil then
    begin
      {Put Cell's DiagramRect in the variable CellRect to avoid recalculation}
      CellRect := FDiagram.ClientToCanvas(ACell.DiagramRect);

      {Calculate rotation center}
      if Self is TCustomDiagramBlock then
        With ControlRect do
          RotCenter := Dot((Left + Right) / 2, (Top + Bottom) / 2)
      else
        With ACell.DiagramRect do
          RotCenter := Dot((Left + Right) / 2, (Top + Bottom) / 2);

      RotCenter := FDiagram.ClientToCanvas(RotCenter);

      {Use a block drawer to rotate the cell. Even if we paint with gdiplus, we can use the
       normal block drawer - there is no need to use the GPBlockDrawer because we are not
       going to paint anything, just calculate rotations}
      AAngle := ACell.Angle;
      FTempDrawer.CurRect := CellRect;
      FTempDrawer.RotationType := rtRotationCenter;
      FTempDrawer.RotationCenter := RotCenter;
      FTempDrawer.Angle := AAngle;
      FTempDrawer.Canvas := AInfo.Canvas;
      if ACell.RotateFont then
      begin
        {if font will be rotated, then do not change text cell position -
         WriteText procedure will rotate the font automatically}
        ARect := CellRect;
      end
      else
      begin
        {if font will not be rotated, then rotate text cell position
         so that text position is rotated, but not the font itself}
        ARect := FTempDrawer.RotByCenter(CellRect);
        AAngle := 0;
        FTempDrawer.Angle := 0;
      end;

      {Calculate the frame that surrounds the text}
      FrameRect := ARect;
      if ACell.CellFrame.AutoFrame then
      begin
        if ACell.Text <> '' then
        begin
          FrameRect := CalculateTextRect(AInfo.Canvas, ACell.Font, ARect, ACell.Text, ACell.Alignment,
            ACell.VertAlign, ACell.WordWrap, AAngle, RotCenter.X, RotCenter.Y);
          FrameRect.Left := FrameRect.Left - ACell.CellFrame.AutoFrameMargin;
          FrameRect.Top := FrameRect.Top - ACell.CellFrame.AutoFrameMargin;
          FrameRect.Right := FrameRect.Right + ACell.CellFrame.AutoFrameMargin;
          FrameRect.Bottom := FrameRect.Bottom + ACell.CellFrame.AutoFrameMargin;
        end
        else
          FrameRect := Square(0, 0, 0, 0);
      end;


      {Fill the text cell}
      SavePen := TPen.Create;
      SaveBrush := TBrush.Create;
      SavePen.Assign(AInfo.Canvas.Pen);
      SaveBrush.Assign(AInfo.Canvas.Brush);
      try
        if ACell.CellFrame.Visible and not IsNullRect(FrameRect) then
        begin
          {Draw background}
          if not ACell.CellFrame.Transparent then
          begin
            AInfo.Canvas.Brush.Style := bsSolid;
            AInfo.Canvas.Brush.Color := ACell.CellFrame.Color;
            AInfo.Canvas.Pen.Assign(ACell.CellFrame.Pen);
            FTempDrawer.DrawRotRect(FrameRect.Left, FrameRect.Top, FrameRect.Right, FrameRect.Bottom);
          end;

          {Draw Brush and Pen}
          AInfo.Canvas.Brush.Assign(ACell.CellFrame.Brush);
          AInfo.Canvas.Pen.Assign(ACell.CellFrame.Pen);
          FTempDrawer.DrawRotRect(FrameRect.Left, FrameRect.Top, FrameRect.Right, FrameRect.Bottom);
        end;
      finally
        AInfo.Canvas.Brush.Assign(SaveBrush);
        AInfo.Canvas.Pen.Assign(SavePen);
        SavePen.Free;
        SaveBrush.Free;
      end;

      if ACell.Clip then
      begin
        if (AAngle = 0) then
          ClipRgn := CreateRectRgn(Round(ARect.Left), Round(ARect.Top),
            Round(ARect.Right), Round(ARect.Bottom))
        else
        begin
          Pts := RoundDotArray(FTempDrawer.RotPoly(DotArray([Dot(ARect.Left, ARect.Top),
            Dot(ARect.Right, ARect.Top),
            Dot(ARect.Right, ARect.Bottom),
            Dot(ARect.Left, ARect.Bottom),
            Dot(ARect.Left, ARect.Top)
            ])));
          ClipRgn := CreatePolygonRgn(Pts[0], Length(Pts), ALTERNATE);
        end;
        try
          ClipRegion(AInfo.Canvas, ClipRgn);
          try
            WriteText(AInfo.Canvas, ACell.Font, ARect, ACell.Text, ACell.Alignment,
              ACell.VertAlign, ACell.WordWrap, AAngle, RotCenter.X, RotCenter.Y);
          finally
            UnclipRegion(AInfo.Canvas);
          end;
        finally
          DeleteObject(ClipRgn);
        end;
      end
      else
        WriteText(AInfo.Canvas, ACell.Font, ARect, ACell.Text, ACell.Alignment,
          ACell.VertAlign, ACell.WordWrap, AAngle, RotCenter.X, RotCenter.Y);
    end;
  end;

  procedure _DrawCellDgr;
  var
    RotCenter: TDot;
    AAngle: double;
    CellRect: TSquare;
    Pts: TPointArray;
    SaveBrush: TDgrBrush;
    SavePen: TDgrPen;
    FrameRect: TSquare;
    ATransp: integer;
  begin
    SetLength(Pts, 0);
    if FDiagram <> nil then
    begin
      {Put Cell's DiagramRect in the variable CellRect to avoid recalculation}
      CellRect := ACell.DiagramRect;

      {Calculate rotation center}
      if Self is TCustomDiagramBlock then
        with ControlRect do
          RotCenter := Dot((Left + Right) / 2, (Top + Bottom) / 2)
      else
        with ACell.DiagramRect do
          RotCenter := Dot((Left + Right) / 2, (Top + Bottom) / 2);

      AAngle := ACell.Angle;
      if not ACell.RotateFont then
      begin
        {if font will not be rotated, then rotate text cell position
         so that text position is rotated, but not the font itself
         Use a block drawer to rotate the cell. Even if we paint with gdiplus, we can use the
         normal block drawer - there is no need to use the GPBlockDrawer because we are not
         going to paint anything, just calculate rotations}
        FTempDrawer.RotationType := rtRotationCenter;
        FTempDrawer.RotationCenter := RotCenter;
        FTempDrawer.Angle := AAngle;
        CellRect := FTempDrawer.RotByCenter(CellRect);
        AAngle := 0;
      end;

      {Calculate the frame that surrounds the text}
      FrameRect := CellRect;
      if ACell.CellFrame.AutoFrame then
      begin
        if ACell.Text <> '' then
        begin
          if ACell.ParentTransparency then
            ATransp := Self.Transparency
          else
            ATransp := ACell.Transparency;
          FrameRect := InternalWriteTextDgr(AInfo.DgrDrawer, ACell.Font, FrameRect, ACell.Text, ACell.Alignment,
            ACell.VertAlign, ACell.WordWrap, AAngle, RotCenter, false, false, false, ATransp);
          FrameRect.Left := FrameRect.Left - ACell.CellFrame.AutoFrameMargin;
          FrameRect.Top := FrameRect.Top - ACell.CellFrame.AutoFrameMargin;
          FrameRect.Right := FrameRect.Right + ACell.CellFrame.AutoFrameMargin;
          FrameRect.Bottom := FrameRect.Bottom + ACell.CellFrame.AutoFrameMargin;
        end
        else
          FrameRect := Square(0, 0, 0, 0);
      end;

      {save pen and brush for drawer}
      SavePen := AInfo.DgrDrawer.Canvas.CreatePen;
      SaveBrush := AInfo.DgrDrawer.Canvas.CreateBrush;
      SavePen.Assign(AInfo.DgrDrawer.Canvas.Pen);
      SaveBrush.Assign(AInfo.DgrDrawer.Canvas.Brush);
      try
        if ACell.CellFrame.Visible then
        begin
          {Choose Brush and Pen}
          AInfo.DgrDrawer.Canvas.Brush.Assign(ACell.CellFrame.Brush);
          AInfo.DgrDrawer.Canvas.Pen.Assign(ACell.CellFrame.Pen);

          {Set background color}
          if not ACell.CellFrame.Transparent then
          begin
            if ACell.CellFrame.Brush.Style = bsClear then
            begin
              AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmSolid;
              AInfo.DgrDrawer.Canvas.Brush.Color := ACell.CellFrame.Color;
            end
            else
              AInfo.DgrDrawer.Canvas.Brush.Color2 := ACell.CellFrame.Color;
          end;
        end
        else
        begin
          {do not draw the frame}
          AInfo.DgrDrawer.Canvas.Brush.BrushMode := bmClear;
          AInfo.DgrDrawer.Canvas.Pen.Transparency := 100;
        end;

        {Now write the text using gdiplus}
        if ACell.ParentTransparency then
          ATransp := Self.Transparency
        else
          ATransp := ACell.Transparency;
        InternalWriteTextDgr(AInfo.DgrDrawer, ACell.Font, FrameRect, ACell.Text, ACell.Alignment, ACell.VertAlign,
          ACell.WordWrap, AAngle, RotCenter, ACell.Clip, true, true, ATransp, ACell.CellFrame.AutoFrameMargin);
      finally
        AInfo.DgrDrawer.Canvas.Brush.Assign(SaveBrush);
        AInfo.DgrDrawer.Canvas.Pen.Assign(SavePen);
        SavePen.Free;
        SaveBrush.Free;
      end;
    end;
  end;

begin
  FTempDrawer.PushState;
  if PathDrawingMode then
    _DrawCellDgr
  else
    _DrawCell;
  FTempDrawer.PopState;
end;

procedure TDiagramControl.ClipRegion(Canvas: TCanvas; R: HRGN);
begin
  if FDiagram <> nil then
    FDiagram.ClipRegion(Canvas, R);
end;

procedure TDiagramControl.UnclipRegion(Canvas: TCanvas);
begin
  if FDiagram <> nil then
    FDiagram.UnclipRegion(Canvas);
end;

function TDiagramControl.GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare;
begin
  result := ARect;
end;

procedure TDiagramControl.SetTextCells(const Value: TTextCells);
begin
  FTextCells.Assign(Value);
end;

function TDiagramControl.DefaultTextCell: TTextCell;
begin
  if TextCells.Count = 0 then
    TextCells.Add;
  result := TextCells[0];
end;

function TDiagramControl.HasDefaultTextCell: boolean;
begin
  result := TextCells.Count > 0;
end;

function TDiagramControl.SelectNextCell(ACell: TTextCell; AForward: boolean): TTextCell;

  procedure _BuildCellList(ADControl: TDiagramControl; AList: TObjectList);
  var
    c: integer;
  begin
    if ADControl <> nil then
    begin
      if ADControl is TGroupBlock then
        for c := 0 to TGroupBlock(ADControl).Members.Count - 1 do
          _BuildCellList(TGroupBlock(ADControl).Members[c].DControl, AList)
      else
        for c := 0 to ADControl.TextCells.Count - 1 do
          if ADControl.TextCells[c].Visible then
            AList.Add(ADControl.TextCells[c]);
    end;
  end;

var
  AIndex: integer;
  CellList: TObjectList;
begin
  CellList := TObjectList.Create(false);
  try
    {Build a list of available text cells}
    if Self.IsMember then
      _BuildCellList(Self.MasterGroup, CellList)
    else
      _BuildCellList(Self, CellList);
    AIndex := CellList.IndexOf(ACell);

    if AIndex >=0 then
    begin
      result := ACell;
      repeat
        AIndex := CellList.IndexOf(result);
        if AForward then
          AIndex := AIndex + 1
        else
          AIndex := AIndex - 1;

        if AIndex >= CellList.Count then
          AIndex := 0
        else
          if AIndex < 0 then
            AIndex := CellList.Count - 1;

        result := TTextCell(CellList[AIndex]);
      until result.Visible or (result = ACell);
    end
    else
      result := nil;
  finally
    CellList.Free;
  end;
end;

procedure TDiagramControl.EditText;
begin
  if Assigned(FDiagram) then
    FDiagram.EditControlText(Self);
end;

procedure TDiagramControl.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    UpdateSelected;
    Redraw;
  end;
end;

function TDiagramControl.GetIsVisible: boolean;
begin                                     
  result := (FVisible) and (Diagram <> nil) and Diagram.LayerActive(Layer) and not FCollapsed and ((LayerObj = nil) or (LayerObj.Visible));
end;

procedure TDiagramControl.UpdateSelected;
begin
  if not IsVisible then
    Selected := false;
end;

function TDiagramControl.GetLayerInteger: integer;
begin
  result := FLayerInteger;
end;

procedure TDiagramControl.SetLayerInteger(const Value: integer);
begin                                  
  if FLayerInteger <> Value then
  begin          
    FLayerInteger := Value;
    UpdateSelected;
    Redraw;
  end;
end;

function TDiagramControl.GetLayerName: string;
begin
  if LayerObj <> nil then
    result := LayerObj.LayerName
  else
    result := '';
end;
                                            
procedure TDiagramControl.SetLayerName(const Value: string);
begin
  FLayerNameFix := '';

  if Diagram <> nil then
  begin
    LayerObj := Diagram.Layers.FindByName(Value);
  end
  else if csLoading in ComponentState then
    FLayerNameFix := Value;
end;

procedure TDiagramControl.SetLayerObj(Value: TDiagramLayer);
begin                                  
  if (FLayerObj <> Value) then
  begin
    FLayerObj := Value;
    
    {Do not allow object to belong to a layer of a different diagram.}
    if (FLayerObj <> nil) and (FLayerObj.Diagram <> Self.Diagram) then
      FLayerObj := nil;
      
    UpdateSelected;
    Redraw;
  end;
end;

function TDiagramControl.GetLinkPointClass: TLinkPointClass;
begin
  result := TLinkPoint;
end;

procedure TDiagramControl.MouseDownEx(Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer; var AHandle: TStretchHandle);
begin
end;

function TDiagramControl.GetSelected: boolean;
begin
  if not CanSelect then
    result := false
  else
    result := FSelected;
end;

class function TDiagramControl.DiagramDrawInfo(ACanvas: TCanvas; ADrawer: TBlockDrawer; ADgrDrawer: TDgrBlockDrawer): TDiagramDrawInfo;
begin
  result.Canvas := ACanvas;
  result.Drawer := ADrawer;
  result.DgrDrawer := ADgrDrawer;
end;

procedure TDiagramControl.MakeVisible;
begin
  if Diagram <> nil then
    Diagram.MakeControlVisible(Self);  
end;

procedure TDiagramControl.CalcSize(Canvas: TCanvas);
begin
  // virtual method
end;

procedure TDiagramControl.SetIsBackgroundControl(const Value: Boolean);
begin
  FIsBackgroundControl := Value;
  Redraw;
end;

function TDiagramControl.GetIsGdiPlus: boolean;
begin
  {$IFDEF GDIPLUS}
  result := GraphicLib = dglGDIPlus;
  {$ELSE}
  result := false;
  {$ENDIF}
end;

procedure TDiagramControl.InitDgrDrawer(ADrawer: TDgrBlockDrawer);
begin
end;

procedure TDiagramControl.InitDrawer(ADrawer: TBlockDrawer);
begin
end;

procedure TDiagramControl.EndCursorPlacement;
begin
end;

procedure TDiagramControl.StartCursorPlacement;
begin
end;

procedure TDiagramControl.SetTransparency(const Value: integer);
begin
  if FTransparency <> Value then
  begin
    FTransparency := Value;
    Redraw;
  end;
end;

function TDiagramControl.HasAnchorableLinkPoints: boolean;
var
  c: integer;
begin
  result := false;
  for c := 0 to LinkPoints.Count - 1 do
    if LinkPoints[c].CanAnchor then
    begin
      result := true;
      break;
    end;
end;

function TDiagramControl.DrawGrouped: boolean;
begin
  result := ((Diagram <> nil) and (Diagram.GroupSelection) and (Self <> Diagram.FSelBlock));
end;

procedure TDiagramControl.TextCellsChanged;
begin
  if (FDiagram <> nil) then
    FDiagram.Redraw;
end;

function TDiagramControl.LinkPointByHandle(
  AHandle: TStretchHandle): TLinkPoint;
begin
  result := nil;
end;

procedure TDiagramControl.RemoveAnchorsAfterMoving;
var
  d: integer;
begin
  {Remove anchors that were not moved together}
  // avoid LinkPoints for lines that require targets being un-linked by checking
  // for "RequiresConnections"
  for d := 0 to LinkPoints.Count - 1 do
    if (LinkPoints[d].Anchor <> nil) and not
      (LinkPoints[d].Anchor.Selected) and
      ( ( (Self is TCustomDiagramLine) and
        not TCustomDiagramLine(Self).RequiresConnections)
        or
        ( not (Self is TCustomDiagramLine)
        )
      )
    then
    begin
      LinkPoints[d].Anchor := nil;
      LinkPoints[d].AnchorIndex := -1;
    end;
end;

procedure TDiagramControl.HandleDestroyed(AHandle: TStretchHandle);
begin
end;

procedure TDiagramControl.TextCellDestroyed(ACell: TTextCell);
begin
  if (FDiagram <> nil) and (FDiagram.FInplaceMemo <> nil) and (FDiagram.FInplaceMemo.FCell = ACell)
    and not (csDestroying in FDiagram.ComponentState) then
    FDiagram.CloseEditor(false);
end;

procedure TDiagramControl.BeginNotifying;
begin
  Inc(FNotifying);
end;

procedure TDiagramControl.EndNotifying;
begin
  Dec(FNotifying);
end;

function TDiagramControl.GetGraphicLib: TDgrGraphicLib;
begin
  if FCustomGraphicLib or (Diagram = nil) then
    result := FGraphicLib
  else
    result := Diagram.GraphicLib;
end;

function TDiagramControl.GetGroupBlock: TGroupBlock;
begin
  result := FGroupBlock;
end;

procedure TDiagramControl.IncludedInGroup(AGroup: TGroupBlock);
begin
  FGroupBlock := AGroup;
  if FDiagram <> nil then
    FDiagram.DoMemberGroupIncluded(AGroup, Self);
end;

procedure TDiagramControl.RemovedFromGroup(AGroup: TGroupBlock);
begin
  FGroupBlock := nil;
  if FDiagram <> nil then
    FDiagram.DoMemberGroupRemoved(AGroup, Self);
end;

procedure TDiagramControl.RestoreLayer;
begin
  {If we need to set the layer based on a temporary layer name, do it}
  if (FLayerNameFix <> '') and (Diagram <> nil) then
  begin
    FLayerObj := Diagram.Layers.FindByName(FLayerNameFix);
    FLayerNameFix := '';
  end;
end;

function TDiagramControl.GetIsMember: boolean;
begin
  result := FGroupBlock <> nil;
end;

function TDiagramControl.GetIsGroup: boolean;
begin
  result := (Self is TGroupBlock) and not (Self is TSelectionBlock);
end;

function TDiagramControl.GetMasterGroup: TGroupBlock;
begin
  if IsGroup then
    result := TGroupBlock(Self)
  else
    result := FGroupBlock;
  while (result <> nil) and (result.GroupBlock <> nil) do
    result := result.GroupBlock;
end;

function TDiagramControl.GetOriginalRect: TSquare;
begin
  if PathDrawingMode and (FDgrDrawer <> nil) then
    result := FDgrDrawer.SourceRect
  else
    result := Drawer.OriginalRect;
end;

procedure TDiagramControl.SetPaintMode(Value: TDControlPaintMode);
begin
  FIntPaintMode := Value;
  if AsMember then
    GroupBlock.PaintMode := Value;
end;

function TDiagramControl.CanRotate: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoRotation in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.CanResize: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoResize in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.CanMove: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoMove in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.CanMoveRotCenter: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoRotCenterMove in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.CanSelect: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoSelect in Restrictions);
end;

procedure TDiagramControl.CheckGraphicLibObjects;
begin
  if (FDgrDrawer = nil) or ((DgrBlockDrawerClass <> nil) and not (FDgrDrawer is DgrBlockDrawerClass)) then
    CreateGraphicLibObjects;
end;

function TDiagramControl.CanDoClipboard: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoClipboard in Restrictions);
end;

function TDiagramControl.CanEditText: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoEdit in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.CanLink: boolean;
begin
  result := ((Diagram = nil) or (Diagram.DesignMode = ddmEditable))
    and not (crNoLink in Restrictions);
  result := result and LayerEditable;
end;

function TDiagramControl.LayerEditable: boolean;
begin
  result := (LayerObj = nil) or (LayerObj.Editable);
end;

{ TLineArrow }

procedure TLineArrow.Assign(Source: TPersistent);
begin
  if Source is TLineArrow then
  begin
    FShape := TLineArrow(Source).FShape;
    FWidth := TLineArrow(Source).FWidth;
    FHeight := TLineArrow(Source).FHeight;
    FBrush.Assign(TLineArrow(Source).FBrush);
    //FPen.Assign(TLineArrow(Source).FPen);
  end else
    inherited Assign(Source);
end;

procedure TLineArrow.Changed;
begin
  if Assigned(FDiagramLine) then
    FDiagramLine.Redraw;
end;

constructor TLineArrow.Create(ADiagramLink: TCustomDiagramLine);
begin
  inherited Create;
  FDiagramLine := ADiagramLink;
  FBrush := TBrush.Create;
  FBrush.OnChange := StyleChanged;
  RestoreDefaults;
end;

destructor TLineArrow.Destroy;
begin
  FBrush.Free;
  inherited;
end;

class function TLineArrow.DiagramDrawArrowInfo(AFromPoint, AToPoint: TDot; AZoomRatio: double): TDiagramDrawArrowInfo;
begin
  result.FromPoint := AFromPoint;
  result.ToPoint := AToPoint;
  result.ZoomRatio := AZoomRatio;
end;

procedure TLineArrow.Draw(AInfo: TDiagramDrawInfo; AArrowInfo: TDiagramDrawArrowInfo);
var
  Zoomed: TDot;
  P1, P2, P3, P4: TDot;
  path: TDgrGraphicsPath;
begin
  if (FDiagramLine <> nil) and (FDiagramLine.Diagram <> nil) then
  begin
    if FDiagramLine.PathDrawingMode then
    begin
      path := AInfo.DgrDrawer.CreatePath;
      try
        AInfo.DgrDrawer.Canvas.Brush.Assign(FBrush);
        AInfo.DgrDrawer.Canvas.Pen.Assign(FDiagramLine.Pen);
        Zoomed := Dot(Width, Height);
        case Shape of
          asSolidArrow:
            begin
              CalcPerpendicularPoints(P1, P2, DrawFrom, DrawTo, Zoomed);
              path.AddLine(DrawTo.X, DrawTo.Y, P1.X, P1.Y);
              path.AddLine(P2.X, P2.Y, DrawTo.X, DrawTo.Y);
              path.CloseFigure;
              AInfo.DgrDrawer.Canvas.Path(path);
            end;
          asLineArrow:
            begin
              CalcPerpendicularPoints(P1, P2, DrawFrom, DrawTo, Zoomed);
              path.AddLine(P1.X, P1.Y, DrawTo.X, DrawTo.Y);
              path.AddLine(DrawTo.X, DrawTo.Y, P2.X, P2.Y);
              AInfo.DgrDrawer.Canvas.DrawPath(path);
            end;
          asHalfLine:
            begin
              CalcPerpendicularPoints(P1, P2, DrawFrom, DrawTo, Zoomed);
              //APath.AddLine(P1.X, P1.Y, DrawTo.X, DrawTo.Y);
              path.AddLine(DrawTo.X, DrawTo.Y, P2.X, P2.Y);
              AInfo.DgrDrawer.Canvas.DrawPath(path);
            end;
          asDiamond:
            begin
              P1 := Dot(DrawTo.X - Zoomed.X / 2, DrawTo.Y);
              P2 := Dot(DrawTo.X, DrawTo.Y - Zoomed.Y / 2);
              P3 := Dot(DrawTo.X + Zoomed.X / 2, DrawTo.Y);
              P4 := Dot(DrawTo.X, DrawTo.Y + Zoomed.Y / 2);
              path.AddLine(P1.X, P1.Y, P2.X, P2.Y);
              path.AddLine(P3.X, P3.Y, P4.X, P4.Y);
              path.CloseFigure;
              AInfo.DgrDrawer.Canvas.Path(path);
            end;
          asRectangle:
            begin
              P1 := Dot(DrawTo.X - Zoomed.X / 2, DrawTo.Y - Zoomed.Y / 2);
              P2 := Dot(DrawTo.X + Zoomed.X / 2 + 1, DrawTo.Y + Zoomed.Y / 2 + 1);
              AInfo.DgrDrawer.Canvas.Rectangle(P1.X, P1.Y, P2.X, P2.Y);
            end;
          asEllipse:
            begin
              P1 := Dot(DrawTo.X - Zoomed.X / 2, DrawTo.Y - Zoomed.Y / 2);
              P2 := Dot(DrawTo.X + Zoomed.X / 2 + 1, DrawTo.Y + Zoomed.Y / 2 + 1);
              AInfo.DgrDrawer.Canvas.Ellipse(P1.X, P1.Y, P2.X, P2.Y);
            end;
          asAngledDiamond:
            begin
              CalcPerpendicularPoints(P1, P2, DrawFrom, DrawTo, Dot(Zoomed.X, Zoomed.Y / 2));
              CalcPerpendicularPoints(P3, P4, P1, P2, Dot(Zoomed.Y, Zoomed.X / 2));
              path.AddLine(DrawTo.X, DrawTo.Y, P1.X, P1.Y);
              path.AddLine(P4.X, P4.Y, P2.X, P2.Y);
              path.CloseFigure;
              AInfo.DgrDrawer.Canvas.Path(path);
            end;
        end;
      finally
        path.Free;
      end;
    end
    else
    begin
      AInfo.Canvas.Brush := FBrush;
      AInfo.Canvas.Pen := FDiagramLine.Pen;
      Zoomed :=
        SubPointX(
          FDiagramLine.FDiagram.ClientToCanvas(Dot(Width, Height)),
          FDiagramLine.FDiagram.ClientToCanvas(Dot(0,0)));
      case Shape of
        asSolidArrow:
          begin
            CalcPerpendicularPoints(P1, P2, AArrowInfo.FromPoint, AArrowInfo.ToPoint, Zoomed);
            AInfo.Canvas.Polygon(RoundDotArray(DotArray([AArrowInfo.ToPoint, P1, P2, AArrowInfo.ToPoint])));
          end;
        asLineArrow:
          begin
            CalcPerpendicularPoints(P1, P2, AArrowInfo.FromPoint, AArrowInfo.ToPoint, Zoomed);
            AInfo.Canvas.MoveTo(Round(P1.X), Round(P1.Y));
            AInfo.Canvas.LineTo(Round(AArrowInfo.ToPoint.X), Round(AArrowInfo.ToPoint.Y));
            AInfo.Canvas.LineTo(Round(P2.X), Round(P2.Y));
            AInfo.Canvas.LineTo(Round(AArrowInfo.ToPoint.X), Round(AArrowInfo.ToPoint.Y));
          end;
        asHalfLine:
          begin
            CalcPerpendicularPoints(P1, P2, AArrowInfo.FromPoint, AArrowInfo.ToPoint, Zoomed);
            AInfo.Canvas.MoveTo(Round(P2.X), Round(P2.Y));
            AInfo.Canvas.LineTo(Round(AArrowInfo.ToPoint.X), Round(AArrowInfo.ToPoint.Y));
          end;
        asDiamond:
          begin
            P1 := Dot(AArrowInfo.ToPoint.X - Zoomed.X / 2, AArrowInfo.ToPoint.Y);
            P2 := Dot(AArrowInfo.ToPoint.X, AArrowInfo.ToPoint.Y - Zoomed.Y / 2);
            P3 := Dot(AArrowInfo.ToPoint.X + Zoomed.X / 2, AArrowInfo.ToPoint.Y);
            P4 := Dot(AArrowInfo.ToPoint.X, AArrowInfo.ToPoint.Y + Zoomed.Y / 2);
            AInfo.Canvas.Polygon(RoundDotArray(DotArray([P1, P2, P3, P4, P1])));
          end;
        asRectangle:
          begin
            P1 := Dot(AArrowInfo.ToPoint.X - Zoomed.X / 2, AArrowInfo.ToPoint.Y - Zoomed.Y / 2);
            P2 := Dot(AArrowInfo.ToPoint.X + Zoomed.X / 2 + 1, AArrowInfo.ToPoint.Y + Zoomed.Y / 2 + 1);
            AInfo.Canvas.Rectangle(Round(P1.X), Round(P1.Y), Round(P2.X), Round(P2.Y));
          end;
        asEllipse:
          begin
            P1 := Dot(AArrowInfo.ToPoint.X - Zoomed.X / 2, AArrowInfo.ToPoint.Y - Zoomed.Y / 2);
            P2 := Dot(AArrowInfo.ToPoint.X + Zoomed.X / 2 + 1, AArrowInfo.ToPoint.Y + Zoomed.Y / 2 + 1);
            AInfo.Canvas.Ellipse(Round(P1.X), Round(P1.Y), Round(P2.X), Round(P2.Y));
          end;
        asAngledDiamond:
          begin
            CalcPerpendicularPoints(P1, P3, AArrowInfo.FromPoint, AArrowInfo.ToPoint, Dot(Zoomed.X, Zoomed.Y / 2));
            CalcPerpendicularPoints(P2, P4, P1, P3, Dot(Zoomed.Y, Zoomed.X / 2));
            AInfo.Canvas.Polygon(RoundDotArray(DotArray([P1, P2, P3, P4])));
          end;
      end;
    end;
  end;
end;

procedure TLineArrow.RestoreDefaults;
begin
  FShape := asNone;
  FWidth := 10;
  FHeight := 10;
  FBrush.Style := bsSolid;
  if Assigned(FDiagramLine) and Assigned(FDiagramLine.FDiagram) then
    FBrush.Color := FDiagramLine.Pen.Color
  else
    FBrush.Color := clBlack;
end;

procedure TLineArrow.SetBrush(const Value: TBrush);
begin
  FBrush.Assign(Value);
end;

procedure TLineArrow.SetHeight(const Value: integer);
begin
  if FHeight <> Value then
  begin
    FHeight := Value;
    Changed;
  end;
end;

procedure TLineArrow.SetShape(const Value: TArrowShape);
begin
  if FShape <> Value then
  begin
    FShape := Value;
    Changed;
  end;
end;

procedure TLineArrow.SetWidth(const Value: integer);
begin
  if FWidth <> Value then
  begin
    FWidth := Value;
    Changed;
  end;
end;

procedure TLineArrow.StyleChanged(Sender: TObject);
begin
  Changed;
end;

{ TTransMemo }

procedure TTransMemo.CNCTLCOLOREDIT(var Message: TWMCTLCOLOREDIT);
begin
  inherited;
  with Message do
  begin
    SetBkMode(ChildDC, TRANSPARENT);
    Result := GetStockObject(HOLLOW_BRUSH)
  end
end;

constructor TTransMemo.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  {$IFDEF DELPHI2009_LVL}
  ParentDoubleBuffered := false;
  {$ENDIF}
end;

procedure TTransMemo.CreateParams(var Params: TCreateParams);
begin
  inherited CreateParams(Params);
  with Params do
  begin
   ExStyle := ExStyle or WS_EX_TRANSPARENT and not WS_EX_WINDOWEDGE
     and not WS_EX_STATICEDGE and not WS_EX_DLGMODALFRAME
     and not WS_EX_CLIENTEDGE;

  end;
end;

procedure TTransMemo.WMEraseBkgnd(var Message: TWMEraseBkgnd);
begin
  Message.Result := 1; 
end;

procedure TTransMemo.WMGetDlgCode(var Message: TWMGetDlgCode);
begin
  inherited;
  Message.Result := Message.Result or DLGC_WANTTAB;
end;

procedure TTransMemo.WMHScroll(var Message: TWMHScroll);
begin
  inherited;
  PostMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
end;

procedure TTransMemo.WMKeyDown(var Message: TWMKeyDown);
begin
  SendMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
  inherited;
  PostMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
end;

procedure TTransMemo.WMSetText(var Message: TWMSetText);
begin
  inherited;
  if not (csDesigning in ComponentState) then
    PostMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
end;

procedure TTransMemo.WMTransMemoInvalidate(var Message: TMessage);
var
  R: TRect;
begin
  if Parent<>nil then
  begin
    R := ClientRect;
    R.TopLeft := Parent.ScreenToClient(ClientToScreen(R.TopLeft));
    R.BottomRight := Parent.ScreenToClient(ClientToScreen(R.BottomRight));
    {$WARNINGS OFF}
    InvalidateRect(Parent.Handle, @R, True);
    {$WARNINGS ON}
    RedrawWindow(Handle, nil, 0, RDW_FRAME + RDW_INVALIDATE);
  end;
end;

procedure TTransMemo.WMVScroll(var Message: TWMVScroll);
begin
  SendMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
  inherited;
  PostMessage(Handle, WM_TRANSMEMOINVALIDATE, 0, 0);
end;

{ TDiagramBlock }

constructor TDiagramBlock.Create(AOwner: TComponent);
begin
  inherited;
  FShape := bsRectangle;
end;

{ TDiagramLineJoin }

constructor TDiagramLineJoin.Create(AOwner: TComponent);
begin
  inherited;
  Restrictions := [crNoResize, crNoEdit, crNoRotation];
  Width := 20;
  Height := 20;
  Drawer.OriginalRect := Square(0, 0, 20, 20);
  UpdateBlockHandles;
  {Create link point that will work as a join}
  LinkPoints.Add(10, 10, aoNone);
end;

function TDiagramLineJoin.GetAnchor: TDiagramControl;
begin
  result := LinkPoints[0].Anchor;
end;

function TDiagramLineJoin.GetAnchorIndex: integer;
begin
  result := LinkPoints[0].AnchorIndex;
end;

procedure TDiagramLineJoin.AnchorLinkChanged(ALinkPoint: TLinkPoint);
begin
  if (ALinkPoint <> nil) and (ALinkPoint.AnchorLink <> nil) then
    FOldAnchorPos := ALinkPoint.AnchorLink.DiagramPoint;
end;

procedure TDiagramLineJoin.LinkedAnchorChanged(ALinkPoint: TLinkPoint; AChange: TAnchorChange);
var
  ADelta: TDot;
begin
  if (ALinkPoint <> nil) and (ALinkPoint.AnchorLink <> nil) then
  begin
    ADelta := SubPointX(ALinkPoint.AnchorLink.DiagramPoint, FOldAnchorPos);
    MovePosition(ADelta.X, ADelta.Y);
  end;
end;

procedure TDiagramLineJoin.SetAnchor(const Value: TDiagramControl);
begin
  LinkPoints[0].Anchor := Value;
end;

procedure TDiagramLineJoin.SetAnchorIndex(const Value: integer);
begin
  LinkPoints[0].AnchorIndex := Value;
end;

function TDiagramLineJoin.GetOrientation: TAnchorOrientation;
begin
  result := LinkPoints[0].Orientation;
end;

procedure TDiagramLineJoin.SetOrientation(const Value: TAnchorOrientation);
begin
  LinkPoints[0].Orientation := Value;
end;

{ TTextBlock }

constructor TTextBlock.Create(AOwner: TComponent);
begin
  inherited;
  FShape := bsNoShape;
  Restrictions := [crNoRotation]; 
  Strings.Text := 'Text block'; 
end;

{ TDrawPoint }

procedure TDrawPoint.Assign(Source: TPersistent);
begin
  if Source is TDrawPoint then
  begin
    FX := TDrawPoint(Source).FX;
    FY := TDrawPoint(Source).FY;
  end else
    inherited Assign(Source);
end;

function TDrawPoint.GetP: TDot;
begin
  result := Dot(X, Y);
end;

procedure TDrawPoint.SetP(const Value: TDot);
begin
  X := Value.X;
  Y := Value.Y;
end;

procedure TDrawPoint.SetX(const Value: number);
begin
  FX := Value;
end;

procedure TDrawPoint.SetY(const Value: number);
begin
  FY := Value;
end;

{ TDrawPoints }

function TDrawPoints.Add: TDrawPoint;
begin
  result := TDrawPoint(inherited Add);
end;

function TDrawPoints.Add(AX, AY: number): TDrawPoint;
begin
  result := Add;
  with result do
  begin
    X := AX;
    Y := AY;
  end;
end;

procedure TDrawPoints.Append(APoints: TDrawPoints);
var
  c: integer;
begin
  for c := 0 to APoints.Count - 1 do
    Add.P := APoints[c].P;
end;

function TDrawPoints.GetItem(Index: integer): TDrawPoint;
begin
  result := TDrawPoint(inherited Items[Index]);
end;

{ TStretchHandles }

function TStretchHandles.Add: TStretchHandle;
begin
  result := TStretchHandle(inherited Add);
end;

function TStretchHandles.Add(AOrX, AOrY: double;
  ACursor: TCursor = crSizeNWSE; AData: integer = 0;
  AStyle: THandleStyle = hsSize): TStretchHandle;
begin
  result := Add;
  With result do
  begin
    OrX := AOrX;
    OrY := AOrY;
    Cursor := ACursor;
    Data := AData;
    Style := AStyle;
  end;
end;

procedure TStretchHandles.Assign(Source: TPersistent);
var
  Src: TStretchHandles;
  c: integer;
begin
  {Do workaround here, to avoid the handles to be destroyed.
   Diagram operations has lot of assigning between handles and temphandles,
   and sometimes a reference to a handle must be kept. So, let's try to keep the pointers,
   avoiding destroying the handles}
  if (Source is TStretchHandles) and (TStretchHandles(Source).Count > 1) and (Count > 1) then
  begin
    Src := TStretchHandles(Source);
    BeginUpdate;
    try
      {Make sure start and end points keep the same pointers}
      Items[0].Assign(Src[0]);
      Items[Count - 1].Assign(Src[Src.Count - 1]);

      if Src.Count > Count then
      begin
        {if the source is bigger than current collection, then copy existing ones, then insert new ones}
        for c := 1 to Count - 2 do
          Items[c].Assign(Src[c]);
        for c := Count - 1 to Src.Count - 2 do
          With Add do
          begin
            Assign(Src[c]);
            Index := c;
          end;
      end else
      begin
        {if the source is smaller than current collection, then copy existing ones, then delete remaining ones}
        for c := 1 to Src.Count - 2 do
          Items[c].Assign(Src[c]);
        while Count > Src.Count do
          Delete(Count - 2);
      end;
    finally
      EndUpdate;
    end;
    Exit;
  end else
    inherited Assign(Source);
end;

constructor TStretchHandles.Create(ADControl: TDiagramControl);
begin
  inherited Create(ADControl, TStretchHandle);
  FDControl := ADControl;
end;

function TStretchHandles.FindByData(AData: integer): TStretchHandle;
var
  c: integer;
begin
  result := nil;
  for c := 0 to Count - 1 do
    if Items[c].Data = AData then
    begin
      result := Items[c];
      break;
    end;
end;

function TStretchHandles.GetItem(Index: integer): TStretchHandle;
begin
  result := TStretchHandle(inherited Items[Index]);
end;

function TStretchHandles.RotateHandle: TStretchHandle;
var
  c: integer;
begin
  result := nil;
  for c := 0 to Count - 1 do
    if Items[c].Style = hsRotate then
    begin
      result := Items[c];
      break;
    end;
end;

function TStretchHandles.RotCenterHandle: TStretchHandle;
var
  c: integer;
begin
  result := nil;
  for c := 0 to Count - 1 do
    if Items[c].Style = hsRotCenter then
    begin
      result := Items[c];
      break;
    end;
end;

procedure TStretchHandles.Update(Item: TCollectionItem);
begin
  if (FDControl <> nil) then
    FDControl.HandlesChanged;
end;

{ TStretchHandle }

procedure TStretchHandle.Assign(Source: TPersistent);
begin
  if Source is TStretchHandle then
  begin
    FOrX := TStretchHandle(Source).FOrX;
    FOrY := TStretchHandle(Source).FOrY;
    FCursor := TStretchHandle(Source).FCursor;
    FData := TStretchHandle(Source).FData;
  end else
    inherited Assign(Source);
end;

constructor TStretchHandle.Create(Collection: TCollection);
begin
  inherited Create(Collection);
  FAlwaysMovable := false;
  FCursor := crSizeNWSE;
  FVisible := True;
end;

destructor TStretchHandle.Destroy;
begin
  if DControl <> nil then
    DControl.HandleDestroyed(Self);
  inherited;
end;

function TStretchHandle.DiagramPoint: TDot;
begin
  if (DControl <> nil) then
    result := DControl.GetHandlePoint(Self)
  else
    result := OrPoint;
end;

function TStretchHandle.GetControl: TDiagramControl;
begin
  if (Collection <> nil) and (Collection is TStretchHandles) then
    result := TStretchHandles(Collection).FDControl
  else
    result := nil;
end;

function TStretchHandle.GetOrPoint: TDot;
begin
  result := Dot(FOrX, FOrY);
end;

function TStretchHandle.GetVisible: boolean;
var
  AMustHide: boolean;
begin
  AMustHide := (DControl <> nil) and not (DControl is TSelectionBlock) and
    (DControl.DrawGrouped or DControl.AsMember or not DControl.Selected);
  result := FVisible and not AMustHide;
end;

function TStretchHandle.Movable: boolean;
begin
  if DControl <> nil then
  begin
    case Style of
      hsSize:
        result := DControl.CanResize;
      hsRotate:
        result := DControl.CanRotate;
      hsRotCenter:
        result := DControl.CanMoveRotCenter and DControl.CanRotate;
      hsCustom:
        result := DControl.CanResize;        
    else
      result := false;
    end;
    result := result or AlwaysMovable;
  end
  else
    result := true;
end;

procedure TStretchHandle.SetOrPoint(const Value: TDot);
begin
  if (FOrX <> Value.X) or (FOrY <> Value.Y) then
  begin
    FOrX := Value.X;
    FOrY := Value.Y;
    Changed(false);
  end;
end;

procedure TStretchHandle.SetOrX(const Value: double);
begin
  if FOrX <> Value then
  begin
    FOrX := Value;
    Changed(false);
  end;
end;

procedure TStretchHandle.SetOrY(const Value: double);
begin
  if FOrY <> Value then
  begin
    FOrY := Value;
    Changed(false);
  end;
end;

procedure TStretchHandle.SetStyle(const Value: THandleStyle);
begin
  if FStyle <> Value then
  begin
    FStyle := Value;
    Changed(false)
  end;
end;

procedure TStretchHandle.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed(false);
  end;
end;

{ TDiagramSideLine }

constructor TDiagramSideLine.Create(AOwner: TComponent);
begin
  inherited;
  TextCellsMode := tmSegments;
  LineStyle := lsSideLine;
end;

{ TBlockShadow }

procedure TBlockShadow.Assign(Source: TPersistent);
begin
  if Source is TBlockShadow then
  begin
    FVisible := TBlockShadow(Source).FVisible;
    FColor := TBlockShadow(Source).FColor;
    FHOffset := TBlockShadow(Source).FHOffset;
    FVOffset := TBlockShadow(Source).FVOffset;
    Changed;
  end else
    inherited Assign(Source);
end;

constructor TBlockShadow.Create(ADControl: TDiagramControl);
begin
  inherited Create(ADControl);
  FColor := clGray;
  FHOffset := 2;
  FVOffset := 2;
  FVisible := False;
  FTransparency := 0;
end;

procedure TBlockShadow.SetColor(const Value: TColor);
begin
  if FColor <> Value then
  begin
    FColor := Value;
    Changed;
  end;
end;

procedure TBlockShadow.SetHOffset(const Value: integer);
begin
  if FHOffset <> Value then
  begin
    FHOffset := Value;
    Changed;
  end;
end;

procedure TBlockShadow.SetTransparency(const Value: integer);
begin
  if FTransparency <> Value then
  begin
    FTransparency := Value;
    Changed;
  end;
end;

procedure TBlockShadow.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

procedure TBlockShadow.SetVOffset(const Value: integer);
begin
  if FVOffset <> Value then
  begin
    FVOffset := Value;
    Changed;
  end;
end;

{ TDiagramPersistent }

procedure TDiagramPersistent.Changed;
begin
  if FDControl <> nil then
    FDControl.Redraw;
end;

constructor TDiagramPersistent.Create(ADControl: TDiagramControl);
begin
  FDControl := ADControl;
end;

{TDiagramGradient}

constructor TDiagramGradient.Create(ADControl: TDiagramControl; ADiagram: TatDiagram = nil);
begin
  inherited Create(ADControl);
  FDiagram := ADiagram;
  FDirection := grTopBottom;
  FStartColor := clWhite;
  FEndColor := clYellow;
  FDrawer := TBlockDrawer.Create;
  FDrawer.OriginalRect := Square(0, 0, 100, 100);
  FVisible := False;
end;

destructor TDiagramGradient.Destroy;
begin
  FDrawer.Free;
  inherited;
end;

procedure TDiagramGradient.Assign(Source: TPersistent);
begin
  if (Source is TDiagramGradient) then
  begin
    StartColor := (Source as TDiagramGradient).StartColor;
    EndColor := (Source as TDiagramGradient).EndColor;
    Direction := (Source as TDiagramGradient).Direction;
    Visible := (Source as TDiagramGradient).Visible;
  end;
end;

procedure TDiagramGradient.DrawGradient(ADrawer: TDgrBlockDrawer);
var
  path: TDgrGraphicsPath;
begin
  {Draw the gradient using gdiplus drawer}
  ADrawer.Canvas.Pen.Transparency := 100;
  ADrawer.Canvas.Brush.BrushMode := bmGradient;
  ADrawer.Canvas.Brush.GradientMode := GetDgrGradientMode;
  ADrawer.Canvas.Brush.Color := StartColor;
  ADrawer.Canvas.Brush.Color2 := EndColor;

  {Do not set brush transparency, it's being set by drawblock}
  //ADrawer.GPCanvas.Brush.Transparency := 0;
  path := ADrawer.Canvas.CreatePath;
  try
    path.AddRectangle(ADrawer.DestRect);
    ADrawer.RotatePath(path);
    ADrawer.Canvas.Path(path);
  finally
    path.Free;
  end;
end;

procedure TDiagramGradient.DrawGradient(Canvas: TCanvas; ARect: TSquare; AAngle: double);

  procedure GradRect(X1, Y1, X2, Y2: integer);
  begin
    if AAngle = 0 then
      Canvas.Rectangle(Round(X1 + ARect.Left), Round(Y1 + ARect.Top),
        Round(X2 + ARect.Left + 1), Round(Y2 + ARect.Top + 1))
    else
      FDrawer.DrawRotRect(Round(X1 + ARect.Left), Round(Y1 + ARect.Top),
        Round(X2 + ARect.Left + 1), Round(Y2 + ARect.Top + 1));
  end;

var
  StartR, StartG, StartB: Byte;
  EndR, EndG, EndB: Byte;
  DiffR, DiffG, DiffB: integer;
  i: Integer;
  StepHeight, StepWidth: double;
  ACount: integer;
  AWidth, AHeight: integer;
begin
  FDrawer.Canvas := Canvas;
  FDrawer.OriginalRect := ARect;
  FDrawer.CurRect := ARect;
  FDrawer.Angle := AAngle;
  FDrawer.RotationType := rtCurRectCenter;
  {Swap colors for some specific directions}
  if (FDirection in [grBottomTop, grRightLeft, grInOut, grHorzInOut, grVertInOut]) then
  begin
    StartR := FEndColor and $0000FF;
    StartG := (FEndColor and $00FF00) shr 8;
    StartB := (FEndColor and $FF0000) shr 16;
    EndR := FStartColor and $0000FF;
    EndG := (FStartColor and $00FF00) shr 8;
    EndB := (FStartColor and $FF0000) shr 16;
  end
  else
  begin
    StartR := FStartColor and $0000FF;
    StartG := (FStartColor and $00FF00) shr 8;
    StartB := (FStartColor and $FF0000) shr 16;
    EndR := FEndColor and $0000FF;
    EndG := (FEndColor and $00FF00) shr 8;
    EndB := (FEndColor and $FF0000) shr 16;
  end;
  {Start variables. Some are needed, some just to avoid compiler warnings}
  AWidth := Round(ARect.Right - ARect.Left);
  AHeight := Round(ARect.Bottom - ARect.Top);
  ACount := Max(AWidth div 2, AHeight div 2);
  if ACount = 0 then
    Exit;

  StepHeight := AHeight / (ACount + 1);
  StepWidth := AWidth / (ACount + 1);
  {Calculate steps, start and end according to direction}
  case FDirection of
    grTopBottom, grBottomTop:
      StepHeight := AHeight / ACount;
    grLeftRight, grRightLeft:
      StepWidth := AWidth / ACount;
    grOutIn, grInOut:
      begin
        StepWidth := AWidth / (2 * ACount);
        StepHeight := AHeight / (2 * ACount);
      end;
    grHorzInOut, grHorzOutIn:
      StepHeight := AHeight / (2 * ACount);
    grVertInOut, grVertOutIn:
      StepWidth := AWidth / (2 * ACount);
    grLeftTop, grLeftBottom, grRightTop, grRightBottom:
      begin
        StepWidth := AWidth / ACount;
        StepHeight := AHeight / ACount;
      end;
  end;
  {Calculate color steps}
  DiffR := EndR - StartR;
  DiffG := EndG - StartG;
  DiffB := EndB - StartB;
  {Draw color steps}
  for i := 0 to ACount - 1 do
    with Canvas do
    begin
      Pen.Style := psClear;
      if ACount > 1 then
        Brush.Color := StartR + MulDiv(i, DiffR, ACount - 1) +
          (StartG + MulDiv(i, DiffG, ACount - 1)) shl 8 +
          (StartB + MulDiv(i, DiffB, ACount - 1)) shl 16
      else
        Brush.Color := StartR + StartG shl 8 + StartB shl 16;
      case FDirection of
        grTopBottom, grBottomTop:
          GradRect(0, Trunc(StepHeight * i), AWidth, Trunc(StepHeight * (i + 1)));
        grLeftRight, grRightLeft:
          GradRect(Trunc(StepWidth * i), 0, Trunc(StepWidth * (i + 1)), AHeight);
        grOutIn, grInOut:
          begin
            GradRect(Trunc(StepWidth * i),
              Trunc(StepHeight * i),
              AWidth - Trunc(StepWidth * (i + 1) - 1) - 1,
              Trunc(StepHeight * (i + 1))
              );
            GradRect(Trunc(StepWidth * i),
              AHeight - Trunc(StepHeight * (i + 1) - 1) - 1,
              AWidth - Trunc(StepWidth * (i + 1) - 1) - 1,
              AHeight - Trunc(StepHeight * i)
              );
            GradRect(Trunc(StepWidth * i),
              Trunc(StepHeight * i),
              Trunc(StepWidth * (i + 1)),
              AHeight - Trunc(StepHeight * i)
              );
            GradRect(AWidth - Trunc(StepWidth * (i + 1) - 1) - 1,
              Trunc(StepHeight * i),
              AWidth - Trunc(StepWidth * i),
              AHeight - Trunc(StepHeight * i)
              );
          end;
        grHorzInOut, grHorzOutIn:
          begin
            GradRect(0, Trunc(StepHeight * i), AWidth, Trunc(StepHeight * (i + 1)));
            GradRect(0, AHeight - Trunc(StepHeight * (i + 1) - 1) - 1,
              AWidth, AHeight - Trunc(StepHeight * i)
              );
          end;
        grVertInOut, grVertOutIn:
          begin
            GradRect(Trunc(StepWidth * i), 0, Trunc(StepWidth * (i + 1)), AHeight);
            GradRect(AWidth - Trunc(StepWidth * (i + 1) - 1) - 1, 0,
              AWidth - Trunc(StepWidth * i), AHeight
            );
          end;
        grRightBottom:
          begin
            GradRect(0,
              AHeight - Trunc(StepHeight * (i + 1) - 1) - 1,
              AWidth,
              AHeight - Trunc(StepHeight * i)
              );
            GradRect(AWidth - Trunc(StepWidth * (i + 1) - 1) - 1,
              0,
              AWidth - Trunc(StepWidth * i),
              AHeight);
          end;
        grLeftTop:
          begin
            GradRect(0,
              Trunc(StepHeight * i),
              AWidth,
              Trunc(StepHeight * (i + 1)));
            GradRect(Trunc(StepWidth * i),
              0,
              Trunc(StepWidth * (i + 1)),
              AHeight
              );
          end;
        grLeftBottom:
          begin
            GradRect(0,
              AHeight - Trunc(StepHeight * (i + 1) - 1) - 1,
              AWidth,
              AHeight - Trunc(StepHeight * i)
              );
            GradRect(Trunc(StepWidth * i),
              0,
              Trunc(StepWidth * (i + 1)),
              AHeight
              );
          end;
        grRightTop:
          begin
            GradRect(0,
              Trunc(StepHeight * i),
              AWidth,
              Trunc(StepHeight * (i + 1)));
            GradRect(AWidth - Trunc(StepWidth * (i + 1) - 1) - 1,
              0,
              AWidth - Trunc(StepWidth * i),
              AHeight);
          end;
      end;
    end;
  with Canvas do
  begin
    Pen.Style := psClear;
    Brush.Color := EndR + EndG shl 8 + EndB shl 16;
    case FDirection of
      grTopBottom, grBottomTop:
        GradRect(0, AHeight - 1, AWidth, AHeight);
      grLeftRight, grRightLeft:
        GradRect(AWidth - 1, 0, AWidth, AHeight);
      grOutIn, grInOut:
        GradRect(AWidth div 2 - 1, AHeight div 2 - 1,
          AWidth div 2 + 1, AHeight div 2 + 1);
      grHorzInOut, grHorzOutIn:
        GradRect(0, AHeight div 2 - 1,
          AWidth, AHeight div 2 + 1);
      grVertInOut, grVertOutIn:
        GradRect(AWidth div 2 - 1,
          0, AWidth div 2 + 1, AHeight);
      grLeftTop:
        begin
          GradRect(AWidth - 1, 0, AWidth, AHeight);
          GradRect(0, AHeight - 1, AWidth, AHeight);
        end;
      grRightBottom:
        begin
          GradRect(0, 0, AWidth, 1);
          GradRect(0, 0, 1, AHeight);
        end;
      grRightTop:
        begin
          GradRect(0, 0, 1, AHeight);
          GradRect(0, AHeight - 1, AWidth, AHeight);
        end;
      grLeftBottom:
        begin
          GradRect(AWidth - 1, 0, AWidth, AHeight);
          GradRect(0, 0, AWidth, 1);
        end;
    end;
  end;
end;

procedure TDiagramGradient.SetDirection(const Value: TGradientDirection);
begin
  if FDirection <> Value then
  begin
    FDirection := Value;
    Changed;
  end;
end;

procedure TDiagramGradient.SetEndColor(const Value: TColor);
begin
  if FEndColor <> Value then
  begin
    FEndColor := Value;
    Changed;
  end;
end;

procedure TDiagramGradient.SetStartColor(const Value: TColor);
begin
  if FStartColor <> Value then
  begin
    FStartColor := Value;
    Changed;
  end;
end;

procedure TDiagramGradient.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

{ TatRuler }

constructor TCustomRuler.Create;
begin
  FGridColor := clSilver;
  FZoom := 100;
  FUnits := unCenti;
  FFont := TFont.Create;
  FFont.OnChange := FontChanged;
  FFont0 := 0;
  FFont90 := 0;
  FMinorTickLength := 4;
  FMajorTickLength := 6;
  FAutoFactor := true;
  UpdateFonts;
  UpdateDivisions;
  UpdateSize(0);
end;

destructor TCustomRuler.Destroy;
begin
  FFont.Free;
  DeleteFonts;
  inherited;
end;

procedure TCustomRuler.DeleteFonts;
begin
  if FFont0 <> 0 then
  begin
    DeleteObject(FFont0);
    FFont0 := 0;
  end;
  if FFont90 <> 0 then
  begin
    DeleteObject(FFont90);
    FFont90 := 0;
  end;
end;

function TCustomRuler.GetUnits: TRulerUnits;
begin
  result := FUnits;
end;

procedure TCustomRuler.UpdateFonts;
var
  F: TLogFont;
begin
  DeleteFonts;
  {$WARNINGS OFF}
  GetObject(FFont.Handle, SizeOf(TLogFont), @F);
  FFont0 := CreateFontIndirect(F);
  GetObject(FFont.Handle, SizeOf(TLogFont), @F);
  {$WARNINGS ON}
  F.lfEscapement := 900;
  F.lfOrientation := 900;
  FFont90 := CreateFontIndirect(F);
end;

function TCustomRuler.ScreenDPI: integer;
begin
  result := Screen.PixelsPerInch;
end;

procedure TCustomRuler.GetStepAndFactor(var AStep, AFactor: double);
begin
  Case Units of
    unInch: AStep := ScreenDPI / Divisions * Zoom / 100;
    unPixel: AStep := 50 / Divisions * Zoom / 100;
  else {unMili, unCenti}
    AStep := ScreenDPI / Divisions * Zoom / 100 / 2.54;
  end;

  if FAutoFactor then
    case Zoom of
      1: AFactor := 100;
      2: AFactor := 50;
      3..5: AFactor := 25;
      6..8: AFactor := 20;
      9..12: AFactor := 10;
      13..25: AFactor := 5;
      26..35: AFactor := 4;
      36..50: AFactor := 2;
      51..125: AFactor := 1;
      126..300: AFactor :=  0.5;
      301..400: AFactor := 0.25;
      401..500: AFactor := 0.2;
      501..1000: AFactor := 0.1;
    end
  else
    AFactor := 1;

  AStep := AStep * AFactor;
  Case Units of
    unMili: AFactor := AFactor * 10;
    unPixel: AFactor := AFactor * 50;
  end;
end;

function TCustomRuler.RulerStep: double;
var
  Dummy: double;
begin
  GetStepAndFactor(result, Dummy);
end;

procedure TCustomRuler.Draw(Canvas: TCanvas; ARect: TRect; TempOffset: integer = 0; MaxSize: integer = 0; ADrawGrid: boolean = false);
{$IFDEF GDIPLUS}
var
  GP: TGPGraphics;
  GPPen: TGPPen;
{$ENDIF}

  procedure _TickPen;
  begin
    Canvas.Pen.Style := psSolid;
    {for some reason, we have to set pen color twice to make it work.
     It happens in some pieces of code in InternalPaint too}
    Canvas.Pen.Color := clYellow;
    Canvas.Pen.Color := TickColor;
  end;

  procedure _GridLine(x1, y1, x2, y2: integer; AMajor: boolean);
  begin
    if ADrawGrid then
    begin
      {$IFDEF GDIPLUS}
      if not AMajor then //Optimization
      begin
        {if AMajor then
          GPPen.SetDashStyle(DashStyleDash)
        else}
          GPPen.SetDashStyle(DashStyleDot);
        GP.DrawLine(GPPen, x1, y1, x2, y2);
      end
      else
      {$ENDIF}
      begin
        Canvas.Pen.Style := psDot;
        Canvas.Pen.Color := GridColor;
        Canvas.MoveTo(x1, y1);
        Canvas.LineTo(x2, y2);
      end;
    end;
  end;

  procedure DrawTicksAndLabels;
  var
    i: double;
    intx: integer;
    AStep: double;
    AEnd: integer;
    N: integer;
    S: string;
    TextOrg: integer;
    AFactor: double;
    GridCount: integer;
    GridCounter: integer;
    DrawGridLine: boolean;
    MiddleOnly: boolean;
    PreviousMiddle: integer;
  begin
    GetStepAndFactor(AStep, AFactor);
    N := 0;
    i := 0;
    AEnd := 0;
    TextOrg := 0;

    {Calculate when the grid lines will be drawn - not all ruler ticks will have grid lines}
    MiddleOnly := false;
    GridCount := 1;
    while (AStep * GridCount) < 10 do
    begin
      GridCount := GridCount * 2;
      if (Divisions mod GridCount) <> 0 then
      begin
        MiddleOnly := true;
        break;
      end;
    end;
    GridCounter := 0;

    Canvas.Pen.Style := psSolid;
    Canvas.Pen.Width := 1;
    Case FPosition of
      rpTop:
        begin
          i := ARect.Left - (Offset + TempOffset);
          while (i + AStep) < ARect.Left do
          begin
            i := i + AStep;
            Inc(N);
          end;
          AEnd := ARect.Right;
          TextOrg := ARect.Top;
          SelectObject(Canvas.Handle, FFont0);
        end;
      rpLeft:
        begin
          i := ARect.Top - (Offset + TempOffset);
          while (i + AStep) < ARect.Top do
          begin
            i := i + AStep;
            Inc(N);
          end;
          AEnd := ARect.Bottom;
          TextOrg := ARect.Left;
          SelectObject(Canvas.Handle, FFont90);
        end;
    end;
    while i < AEnd do
    begin
      {Draw grid lines every GridCount iterations in subdivisions}
      Inc(GridCounter);
      if GridCounter = GridCount then
      begin
        GridCounter := 0;
        DrawGridLine := true;
      end else
        DrawGridLine := false;
      DrawGridLine := DrawGridLine and not MiddleOnly;
      PreviousMiddle := round(i - (AStep * Divisions) / 2);

      intx := Round(i);
      Case FPosition of
        rpTop:
          begin
            Canvas.MoveTo(intx, ARect.Bottom);
            if (N Mod Divisions) = 0 then
            begin
              _TickPen;
              Canvas.LineTo(intx, TextOrg);
              {Draw label}
              S := FormatFloat('0.##', N / Divisions * AFactor);

              Canvas.TextOut(intx + 2, TextOrg, S);

              DrawGridLine := true;
              GridCounter := 0;
            end
            else
            if not Odd(Divisions) and ((N Mod (Divisions div 2)) = 0) then
            begin
              _TickPen;
              Canvas.LineTo(intx, ARect.Bottom - MajorTickLength - 1);
            end
            else
            begin
              _TickPen;
              Canvas.LineTo(intx, ARect.Bottom - MinorTickLength - 1);
            end;

            if DrawGridLine then
            begin
              _GridLine(intX, ARect.Bottom + 1, intX, MaxSize, (N Mod Divisions) = 0);
              if MiddleOnly and (PreviousMiddle >= ARect.Left) then
                _GridLine(PreviousMiddle, ARect.Bottom + 1, PreviousMiddle, MaxSize, false);
            end;
          end;
        rpLeft:
          begin
            Canvas.MoveTo(ARect.Right, intx);
            if (N Mod Divisions) = 0 then
            begin
              _TickPen;
              Canvas.LineTo(TextOrg, intx);
              {Draw label}
              S := FormatFloat('0.##', N / Divisions * AFactor);
              Canvas.TextOut(TextOrg, intx + 2 + Canvas.TextWidth(S), S);

              DrawGridLine := true;
              GridCounter := 0;
            end
            else
            if not Odd(Divisions) and ((N Mod (Divisions div 2)) = 0) then
            begin
              _TickPen;
              Canvas.LineTo(ARect.Right - MajorTickLength - 1, intx);
            end
            else
            begin
              _TickPen;
              Canvas.LineTo(ARect.Right - MinorTickLength - 1, intx);
            end;

            if DrawGridLine then
            begin
              _GridLine(ARect.Right + 1, intx, MaxSize, intx, (N Mod Divisions) = 0);
              if MiddleOnly and (PreviousMiddle >= ARect.Top) then
                _GridLine(ARect.Right + 1, PreviousMiddle, MaxSize, PreviousMiddle, false);
            end;
          end;
      end;

      i := i + AStep;
      inc(N);
    end;
  end;

begin
  if FSize = 0 then
    UpdateSize(0);
  try
    //IntersectClipRect(Canvas.Handle, ARect.Left, ARect.Top, ARect.Right, ARect.Bottom);
    {Draw background}
    with Canvas do
    begin
      Brush.Style := bsSolid;
      Brush.Color := FColor;
      FillRect(ARect);
      Brush.Style := bsClear;
    end;
    {Set default font (SelectObject for font does not update font color, apparently)}
    Canvas.Font := Self.FFont;

    {$IFDEF GDIPLUS}
    GP := TGPGraphics.Create(Canvas.Handle);
    GP.SetCompositingQuality(CompositingQualityHighSpeed);
    GP.SetSmoothingMode(SmoothingModeNone);
    GP.SetPixelOffsetMode(PixelOffsetModeNone);
    GPPen := TGPPen.Create(ColorToARGB(GridColor, 255), 1);
    try
      DrawTicksAndLabels;
    finally
      GPPen.free;
      GP.Free;
    end;
    {$ELSE}
    DrawTicksAndLabels;
    {$ENDIF}
  finally
    //SelectClipRgn(Canvas.Handle, 0);
  end;
end;

procedure TCustomRuler.UpdateDivisions;
begin
  Case Units of
    unInch: FDivisions := 8;
    unPixel: FDivisions := 10;
  else {unMili, unCenti}
    FDivisions := 5;
  end;
end;

procedure TCustomRuler.SetFont(const Value: TFont);
begin
  FFont.Assign(Value);
end;

procedure TCustomRuler.SetUnits(const Value: TRulerUnits);
begin
  if FUnits <> Value then
  begin
    FUnits := Value;
    UpdateDivisions;
    Changed;
  end;
end;

procedure TCustomRuler.SetZoom(const Value: integer);
begin
  if FZoom <> Value then
  begin
    FZoom := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetAutoFactor(const Value: boolean);
begin
  if FAutoFactor <> Value then
  begin
    FAutoFactor := Value;
    Changed;
  end;
end;

procedure TCustomRuler.Changed;
begin
  UpdateSize(FSize);
end;
            
procedure TCustomRuler.SetGridColor(const Value: TColor);
begin
  if FGridColor <> Value then
  begin
    FGridColor := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetMajorTickLength(const Value: integer);
begin
  if FMajorTickLength <> Value then
  begin
    FMajorTickLength := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetMinorTickLength(const Value: integer);
begin
  if FMinorTickLength <> Value then
  begin
    FMinorTickLength := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetPosition(const Value: TRulerPosition);
begin
  if FPosition <> Value then
  begin
    FPosition := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetDivisions(const Value: integer);
begin
  if FDivisions <> Value then
  begin
    FDivisions := Value;
    Changed;
  end;
end;

procedure TCustomRuler.SetColor(const Value: TColor);
begin
  if FColor <> Value then
  begin
    FColor := Value;
    Changed;
  end;
end;

procedure TCustomRuler.FontChanged(Sender: TObject);
begin
  UpdateFonts;
  Changed;
end;

procedure TCustomRuler.SetSize(const Value: integer);
begin
  if FSize <> Value then
    UpdateSize(Value);
end;

procedure TCustomRuler.UpdateSize(ASize: integer);
begin
  if FSize <> ASize then
  begin
    FSize := ASize;
    Changed;
  end
  else
    FSize := ASize;
end;

procedure TCustomRuler.SetTickColor(const Value: TColor);
begin
  if FTickColor <> Value then
  begin
    FTickColor := Value;
    Changed;
  end;
end;

procedure TDiagramGradient.Changed;
begin
  if FDiagram <> nil then
    FDiagram.Redraw
  else
    inherited;
end;

function TDiagramGradient.GetDgrGradientMode: TDgrGradientMode;
begin
  case FDirection of
    grTopBottom:   result := gmTopBottom;
    grBottomTop:   result := gmBottomTop;
    grLeftRight:   result := gmLeftRight;
    grRightLeft:   result := gmRightLeft;
    grLeftTop:     result := gmLeftTop;
    grRightTop:    result := gmRightTop;
    grLeftBottom:  result := gmLeftBottom;
    grRightBottom: result := gmRightBottom;
    grInOut:       result := gmInOut;
    grOutIn:       result := gmOutIn;
    grHorzInOut:   result := gmHorzInOut;
    grHorzOutIn:   result := gmHorzOutIn;
    grVertInOut:   result := gmVertInOut;
    grVertOutIn:   result := gmVertOutIn;
    else
      result := gmTopBottom;
  end;
end;

{TDiagramRuler}

procedure TDiagramRuler.Changed;
begin
  inherited;
  if FDiagram <> nil then
  begin
    FDiagram.SnapGrid.UpdateSize;
    FDiagram.Redraw;
  end;
end;

constructor TDiagramRuler.Create(ADiagram: TatDiagram);
begin
  inherited Create;
  FDiagram := ADiagram;
  FColor := clWhite;
  FFont.Name := 'Arial';
  FFont.Size := 7;
end;

procedure TDiagramRuler.Assign(Source: TPersistent);
begin
  if Source is TDiagramRuler then
  begin
    FVisible := TDiagramRuler(Source).FVisible;
    Divisions := TDiagramRuler(Source).FDivisions;
    FUnits  := TDiagramRuler(Source).FUnits;
    FMinorTickLength := TDiagramRuler(Source).FMinorTickLength;
    FMajorTickLength := TDiagramRuler(Source).FMajorTickLength;
    FColor := TDiagramRuler(Source).FColor;
    FSize := TDiagramRuler(Source).FSize;
    FFont.Assign(TDiagramRuler(Source).FFont);
    Changed;
  end else
    inherited Assign(Source);
end;

procedure TDiagramRuler.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

procedure TDiagramRuler.UpdateSize(ASize: integer);
var
  MinSize: integer;
  ANewSize: integer;
  FOldFont: TFont;
begin
  if (FDiagram <> nil) and FDiagram.HandleAllocated then
  begin
    FOldFont := FDiagram.FControlCanvas.Font;
    FDiagram.FControlCanvas.Font := FFont;
    try
      MinSize := MinorTickLength + FDiagram.FControlCanvas.TextHeight('0123456789.,');
    finally
      FDiagram.FControlCanvas.Font := FOldFont;
    end;
    ANewSize := Max(ASize, MinSize)
  end
  else
    ANewSize := ASize;
  if ANewSize <> FSize then
  begin
    FSize := ANewSize;
    Changed;
  end;
end;

function TDiagramRuler.VisibleSize: integer;
begin
  if Visible then
    result := Size
  else
    result := 0;
end;

function TDiagramRuler.ScreenDPI: integer;
begin
  result := FDiagram.ScreenDPI;
end;

function TDiagramRuler.GetUnits: TRulerUnits;
const
  _ConvUnit: array[TDiagramMeasUnit] of TRulerUnits =
    (unMili, unCenti, unInch);
begin
  if (FDiagram <> nil) and (FDiagram.RulerAutoUnit) then
  begin
    result := _ConvUnit[FDiagram.MeasUnit];
  end else
    result := FUnits;
end;

{ TDiagramBackground }

procedure TDiagramBackground.Assign(Source: TPersistent);
begin
  if Source is TDiagramBackground then
  begin
    FBitmap.Assign(TDiagramBackground(Source).FBitmap);
    FPicture.Assign(TDiagramBackground(Source).FPicture);
    FStyle := TDiagramBackground(Source).FStyle;
    FGradient.Assign(TDiagramBackground(Source).FGradient);
    Changed;
  end
  else
    inherited Assign(Source);
end;

procedure TDiagramBackground.BitmapChange(Sender: TObject);
begin
  Changed;
end;

procedure TDiagramBackground.PictureChange(Sender: TObject);
begin
  Changed;
end;

procedure TDiagramBackground.Changed;
begin
  if Assigned(FDiagram) then
    FDiagram.Redraw;
end;

constructor TDiagramBackground.Create(ADiagram: TatDiagram);
begin
  FDiagram := ADiagram;
  FBitmap := TBitmap.Create;
  FBitmap.OnChange := BitmapChange;
  FPicture := TPicture.Create;
  FPicture.OnChange := PictureChange;
  FStyle := biTile;
  FScroll := True;
  FVisible := False;
  FGradient := TDiagramGradient.Create(nil, FDiagram);
  FPrintGradient := false;
end;

destructor TDiagramBackground.Destroy;
begin
  FBitmap.Free;
  FPicture.Free;
  FGradient.Free;
  inherited;
end;

procedure TDiagramBackground.Draw(Canvas: TCanvas);
var
  AGraphic: TGraphic;

  procedure DrawTile;
  var
    W, H: number;
    ZoomedW, ZoomedH: number;
    StartX: number;
    srcRect, destRect: TSquare;
  begin
    srcRect := Square(0, 0, AGraphic.Width, AGraphic.Height);
    destRect := FDiagram.ClientToCanvas(srcRect);
    if not FScroll and (FDiagram.FPaintStyle = psScreen) then
      DestRect := MoveRect(DestRect, FDiagram.RulerOffset.X - FDiagram.CanvasOffset.X,
        FDiagram.RulerOffset.Y - FDiagram.CanvasOffset.Y);
    ZoomedW := destRect.Right - destRect.Left;
    ZoomedH := destRect.Bottom - destRect.Top;
    StartX := destRect.Left;
    W := FDiagram.ClipRect(Canvas).Right;
    H := FDiagram.ClipRect(Canvas).Bottom;
    while destRect.Top <= H do
    begin
      while destRect.Left <= W do
      begin
        {Canvas.CopyMode := cmSrcCopy;
        Canvas.CopyRect(destRect, FBitmap.Canvas, srcRect);}
        Canvas.StretchDraw(ToRect(destRect), AGraphic);
        destRect.Left := destRect.Left + ZoomedW;
        destRect.Right := destRect.Right + ZoomedW;
      end;
      destRect.Left := StartX;
      destRect.Right := destRect.Left + ZoomedW;
      destRect.Top := destRect.Top + ZoomedH;
      destRect.Bottom := destRect.Bottom + ZoomedH;
    end; 
  end;

  procedure DrawFixed;
  var
    SrcRect: TSquare;
    DestRect: TSquare;
  begin
    SrcRect := Square(0, 0, AGraphic.Width, AGraphic.Height);
    {DestRect := Rect(
      trunc(AOffset.X * FDiagram.ZoomRatio),
      trunc(AOffset.Y * FDiagram.ZoomRatio),
      trunc((AOffset.X + FBitmap.Width) * FDiagram.ZoomRatio),
      trunc((AOffset.Y + FBitmap.Height) * FDiagram.ZoomRatio));}
    DestRect := FDiagram.ClientToCanvas(SrcRect);
    if not FScroll and (FDiagram.FPaintStyle = psScreen) then
      DestRect := MoveRect(DestRect, FDiagram.RulerOffset.X - FDiagram.CanvasOffset.X,
        FDiagram.RulerOffset.Y - FDiagram.CanvasOffset.Y);
    Canvas.StretchDraw(ToRect(DestRect), AGraphic);
  end;

begin
  if (FDiagram <> nil) then
  begin
    if FGradient.Visible and ((FDiagram.FPaintStyle in [psScreen, psBitmap]) or FPrintGradient) then
    begin
      FGradient.DrawGradient(Canvas, FDiagram.ClipRect(Canvas), 0);
    end;

    AGraphic := nil;
    if not EmptyPicture(FPicture) then
      AGraphic := FPicture.Graphic
    else
    if not FBitmap.Empty then
      AGraphic := FBitmap;

    if AGraphic <> nil then
    begin
      case FStyle of
        biTile: DrawTile;
        biFixed: DrawFixed;
      end;
    end;
  end;
end;

procedure TDiagramBackground.SetBitmap(const Value: TBitmap);
begin
  FBitmap.Assign(Value);
end;

procedure TDiagramBackground.SetGradient(const Value: TDiagramGradient);
begin
  FGradient.Assign(Value);
end;

procedure TDiagramBackground.SetPicture(const Value: TPicture);
begin
  FPicture.Assign(Value);
end;

procedure TDiagramBackground.SetScroll(const Value: boolean);
begin
  if FScroll <> Value then
  begin
    FScroll := Value;
    Changed;
  end;
end;

procedure TDiagramBackground.SetStyle(const Value: TBackImageStyle);
begin
  if FStyle <> Value then
  begin
    FStyle := Value;
    Changed;
  end;
end;

procedure TDiagramBackground.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

{ TDiagramPolyLine }

procedure TDiagramPolyLine.UpdateDrawing(AInfo: TControlDrawInfo);

  procedure TempHandlesToCanvas(AHandles: TStretchHandles);
  var
    c: integer;
    P: TPoint;
  begin                   
    for c := 0 to AHandles.Count - 1 do
    begin
      P := AInfo.Diagram.ClientToCanvas(Point(
        round(AHandles[c].OrX), round(AHandles[c].OrY)));
      AHandles[c].OrX := P.X;
      AHandles[c].OrY := P.Y;
    end;
  end;

var
  calcInfo: TCalcHandlesInfo;
  P: TPoint;
begin
  calcInfo.Action := chInsert;
  calcInfo.P1 := AInfo.P1;
  calcInfo.P2 := AInfo.P2;
  calcInfo.Anchor1 := nil;
  calcInfo.Anchor2 := nil;
  calcInfo.HandleMoved := nil;
  with AInfo do
  begin
    case Action of
      daStart:
        begin
          Handles.Clear;
          Handles.Add(AInfo.P1.X, AInfo.P1.Y);
        end;
      daStep:
        begin
          Handles.Add(AInfo.P2.X, AInfo.P2.Y);
          FTempHandles.Assign(Handles);
          if AInfo.Diagram <> nil then
            TempHandlesToCanvas(FTempHandles);
          Canvas.Pen := CurPen;
          if FTempHandles.Count > 1 then
          begin
            P := RoundPoint(FTempHandles[FTempHandles.Count - 2].OrPoint);
            AInfo.Canvas.MoveTo(P.X, P.Y);
            P := RoundPoint(FTempHandles[FTempHandles.Count - 1].OrPoint);
            AInfo.Canvas.LineTo(P.X, P.Y);
          end;
        end;
      daDrawCursor, daEraseCursor:
        begin
          if Action = daDrawCursor then
          begin
            FTempHandles.Clear;
            CalcNewHandles(FTempHandles, calcInfo);
            if AInfo.Diagram <> nil then
              TempHandlesToCanvas(FTempHandles);
          end;
          DrawLineCursor(Canvas, FTempHandles);
        end;
      daInserted:
        begin
          {CalcNewHandles(Handles, ACalcInfo);}
          CheckLinkPoints;
          LinkPoints[0].AnchorLink := AnchorPoint1;
          LinkPoints[1].AnchorLink := AnchorPoint2;
        end;
    end;
  end;
end;

procedure TDiagramPolyLine.CalcNewHandles(AHandles: TStretchHandles; AInfo: TCalcHandlesInfo);
var
  c: integer;
begin
  {Be sure that there are two or more handles}
  while AHandles.Count < 2 do
    AHandles.Add(0, 0); //<-- Call Add(0, 0) to set default properties
  {Update end point handles}
  AHandles[0].OrPoint := Dot(AInfo.P1.X, AInfo.P1.Y);
  AHandles[AHandles.Count - 1].OrPoint := Dot(AInfo.P2.X, AInfo.P2.Y);
  {Update position of intermediate handles}
  for c := 1 to AHandles.Count - 2 do
    Case AInfo.Action of
      chMove:
        AHandles[c].OrPoint := AddPointX(AHandles[c].OrPoint, AInfo.DeltaMove);
      chHandleMove:
        if AHandles[c].Index = AInfo.HandleMoved.Index then
          AHandles[c].OrPoint := AddPointX(AHandles[c].OrPoint, AInfo.DeltaMove);
    end;
end;

constructor TDiagramPolyLine.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FInsertStyle := isDraw;
  //FHandles.Add(200, 50);
end;

{ TTextCells }

function TTextCells.Add: TTextCell;
begin
  result := TTextCell(inherited Add);
end;

constructor TTextCells.Create(ADControl: TDiagramControl);
begin
  FDControl := ADControl;
  inherited Create(FDControl, TTextCell);
end;

function TTextCells.GetItem(Index: integer): TTextCell;
begin
  result := TTextCell(inherited Items[Index]);
end;

procedure TTextCells.Update(Item: TCollectionItem);
begin
  if (FDControl <> nil) then
    FDControl.TextCellsChanged;
end;

{ TTextCell }

procedure TTextCell.Assign(Source: TPersistent);
begin
  if Source is TTextCell then
  begin
    FLeft := TTextCell(Source).FLeft;
    FTop := TTextCell(Source).FTop;
    FWidth := TTextCell(Source).FWidth;
    FHeight := TTextCell(Source).FHeight;
    FText := TTextCell(Source).FText;
    FClip := TTextCell(Source).FClip;
    FWordWrap := TTextCell(Source).FWordWrap;
    FAlignment := TTextCell(Source).FAlignment;
    FVertAlign := TTextCell(Source).FVertAlign;
    FRotateFont := TTextCell(Source).FRotateFont;
    FVisible := TTextCell(Source).FVisible;
    Font.Assign(TTextCell(Source).Font);
    {Must set properties directly (Angle, DControlAngle) and not
     fields (FAngle, FDControlAngle)}
    Angle := TTextCell(Source).FAngle;
    DControlAngle := TTextCell(Source).FDControlAngle;


 end else
    inherited Assign(Source);
end;

procedure TTextCell.BoundsChanged(AOld, ANew: TSquare);
begin
  Changed(false);
end;

constructor TTextCell.Create(Collection: TCollection);
begin
  inherited Create(Collection);
  FVisible := true;
  FFont := TFont.Create;
  FFont.OnChange := FontChanged;
  FDControlFont := True;
  FDControlAngle := True;
  FAlignment := taCenter;
  FVertAlign := vaTop;
  FWordWrap := true;
  FClip := false;
  FRotateFont := true;
  FParentTransparency := true;
  FTransparency := 0;                              
  FCellFrame := TTextCellFrame.Create(Self);

  DControlFontChanged;
  DControlAngleChanged;
end;

procedure TTextCell.DControlAngleChanged;
begin
  if (DControl <> nil) and FDControlAngle then
  begin
    Angle := DControl.Angle;
    FDControlAngle := true;
  end;
end;

procedure TTextCell.DControlFontChanged;
begin
  if (DControl <> nil) and FDControlFont then
  begin
    FFont.Assign(DControl.Font);
    FDControlFont := True;
  end;
end;

destructor TTextCell.Destroy;
begin
  if DControl <> nil then
    DControl.TextCellDestroyed(Self);
  FFont.Free;
  FCellFrame.Free;
  inherited;
end;

procedure TTextCell.FontChanged(Sender: TObject);
begin
  FDControlFont := False;
  Changed(false);
  if (DControl <> nil) and (DControl.Diagram <> nil) and DControl.Diagram.EditingText then
    DControl.Diagram.FInplaceMemo.Font := Self.Font;
end;

function TTextCell.GetBottom: number;
begin
  result := Top + Height;
end;

function TTextCell.GetBoundsRect: TSquare;
begin
  result := Square(Left, Top, Left + Width, Top + Height);
end;

function TTextCell.GetDControl: TDiagramControl;
begin
  result := nil;
  if (Collection <> nil) and (Collection is TTextCells) then
    result := TTextCells(Collection).FDControl;
end;

function TTextCell.GetDiagramRect: TSquare;
begin
  if (DControl <> nil) then
    result := DControl.GetTextCellRect(Self, BoundsRect)
  else
    result := BoundsRect;
end;

function TTextCell.GetRight: number;
begin
  result := Left + Width;
end;

function TTextCell.IsAngleStored: Boolean;
begin
  result := not FDControlAngle;
end;

function TTextCell.IsFontStored: Boolean;
begin
  result := not FDControlFont;
end;

procedure TTextCell.SetAlignment(const Value: TAlignment);
begin
  if FAlignment <> Value then
  begin
    FAlignment := Value;
    Changed(false);
    if (DControl <> nil) and (DControl.Diagram <> nil) and DControl.Diagram.EditingText then
      DControl.Diagram.FInplaceMemo.Alignment := FAlignment;
  end;
end;

procedure TTextCell.SetAngle(const Value: double);
begin
  if FAngle <> Value then
  begin
    FAngle := Value;
    FDControlAngle := False;
    Changed(false);
  end;
end;

procedure TTextCell.SetBounds(ALeft, ATop, AWidth, AHeight: number);
begin
  SetBoundsRect(Square(ALeft, ATop, ALeft + AWidth, ATop + AHeight));
end;

procedure TTextCell.SetBoundsRect(Value: TSquare);
var
  AOldRect: TSquare;
  ANewRect: TSquare;
begin
  ANewRect := RectEx(Value.Left, Value.Top, Value.Right, Value.Bottom);
  if not IsSameRect(BoundsRect, ANewRect) then
  begin
    AOldRect := BoundsRect;
    FLeft := ANewRect.Left;
    FTop := ANewRect.Top;
    FWidth := ANewRect.Right - ANewRect.Left;
    FHeight := ANewRect.Bottom - ANewRect.Top;
    BoundsChanged(AOldRect, BoundsRect);
  end;
end;

procedure TTextCell.SetCellFrame(const Value: TTextCellFrame);
begin
  FCellFrame.Assign(Value);
end;

procedure TTextCell.SetClip(const Value: boolean);
begin
  if FClip <> Value then
  begin
    FClip := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetDControlAngle(const Value: boolean);
begin
  if FDControlAngle <> Value then
  begin
    FDControlAngle := Value;
    if (DControl <> nil) and (csLoading in DControl.ComponentState) and not FDControlAngle then
      FAngle := 0;
    DControlAngleChanged;
  end;
end;

procedure TTextCell.SetDControlFont(const Value: boolean);
begin
  if FDControlFont <> Value then
  begin
    FDControlFont := Value;
    DControlFontChanged;
  end;
end;

procedure TTextCell.SetFont(const Value: TFont);
begin
  FFont.Assign(Value);
end;

procedure TTextCell.SetHeight(const Value: number);
begin
  SetBounds(FLeft, FTop, FWidth, Value);
end;

procedure TTextCell.SetLeft(const Value: number);
begin
  SetBounds(Value, FTop, FWidth, FHeight);
end;

procedure TTextCell.SetRotateFont(const Value: boolean);
begin
  if FRotateFont <> Value then
  begin
    FRotateFont := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetText(const Value: string);
begin
  if FText <> Value then
  begin
    FText := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetTop(const Value: number);
begin
  SetBounds(FLeft, Value, FWidth, FHeight);
end;

procedure TTextCell.SetVertAlign(const Value: TVertAlign);
begin
  if FVertAlign <> Value then
  begin
    FVertAlign := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetWidth(const Value: number);
begin
  SetBounds(FLeft, FTop, Value, FHeight);
end;

procedure TTextCell.SetWordWrap(const Value: boolean);
begin
  if FWordWrap <> Value then
  begin
    FWordWrap := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetParentTransparency(const Value: boolean);   
begin
  if FParentTransparency <> Value then
  begin
    FParentTransparency := Value;
    Changed(false);
  end;
end;

procedure TTextCell.SetTransparency(const Value: integer);
begin
  if FTransparency <> Value then
  begin
    FTransparency := Value;
    Changed(false);
  end;
end;

{ TRegDControls }

function TRegDControls.Add: TRegDControl;
begin
  result:=TRegDControl(inherited Add);
end;

function TRegDControls.FindByID(const ADControlID: string): TRegDControl;
var c: integer;
begin
  result:=nil;
  for c:=0 to Count-1 do
     if SameText(ADControlID, Items[c].DControlID) then
     begin
        result:=Items[c];
        break;
     end;
end;

function TRegDControls.GetItem(Index: integer): TRegDControl;
begin
   result:=TRegDControl(inherited Items[Index]);
end;

{ TRegDControl }

constructor TRegDControl.Create(Collection: TCollection);
begin
  inherited;
  FGlyph := TPicture.Create;
end;

destructor TRegDControl.Destroy;
begin
  FGlyph.Free;
  inherited;
end;

procedure TRegDControl.SetGlyph(const Value: TPicture);
begin
  FGlyph.Assign(Value);
end;

{ TCustomPolygonBlock }

constructor TCustomPolygonBlock.Create(AOwner: TComponent);
begin
  inherited;
  FPolygon := TPolygonPoints.Create(Self, TPolygonPoint);
  FPolygon.FDestroyObjects := True;
  FTempPolygon := TPolygonPoints.Create(Self, TPolygonPoint);
  FTempPolygon.FDestroyObjects := False;
  FCursorPolygon := TPolygonPoints.CReate(Self, TPolygonPoint);
  FCursorPolygon.FDestroyObjects := False;
  FSavePolygon := TPolygonPoints.CReate(Self, TPolygonPoint);
  FSavePolygon.FDestroyObjects := False;
  FPointColor := clLime;
  Shape := bsNoShape;
  Alignment := taCenter;
  VertAlign := vaCenter;
  FInsertStyle := isDraw;
  ShapeCursor := True;
end;

procedure TCustomPolygonBlock.RepositionPolygon(APolygon: TPolygonPoints; ANewRect: TSquare);
var
  c: integer;
  P: TDot;
begin
  if APolygon <> nil then
  begin
    TempDrawer.OriginalRect := BuildOriginalRect(APolygon);
    TempDrawer.CurRect := ANewRect;
    for c := 0 to APolygon.Count - 1 do
    begin
      P := TempDrawer.PP(APolygon[c].X, APolygon[c].Y);
      APolygon[c].X := P.X;
      APolygon[c].Y := P.Y;
    end;
  end;
end;

procedure TCustomPolygonBlock.DrawShape(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);
var
  path: TDgrGraphicsPath;
begin
  if PathDrawingMode then
  begin
    path := AInfo.DgrDrawer.CreatePath;
    try
      GetTranslatedBlockPath(path, AInfo.DgrDrawer);
      AInfo.DgrDrawer.Canvas.Path(path);
    finally
      path.Free;
    end;
  end
  else
  begin
    FTempPolygon.Assign(FPolygon);
    RotatePolygon(FTempPolygon, Angle);
    CanvasPolygon(FTempPolygon);
    AInfo.Canvas.Polygon(RoundDotArray(FTempPolygon.PointArray));
  end;
end;

function TCustomPolygonBlock.SurroundRgn: TDotArray;
begin
  if not PathDrawingMode then
  begin
    FTempPolygon.Assign(FPolygon);
    RotatePolygon(FTempPolygon, Angle);
    result := FTempPolygon.PointArray;
  end;
end;

procedure TCustomPolygonBlock.PolygonChanged;
begin
  ChangeOriginalRect(BuildOriginalRect(FPolygon));
  UpdateBlockHandles;
  UpdatePolygonHandles;
  UpdatePolygonLinkPoints;
end;

function TCustomPolygonBlock.UpdateRotationCenter(ANewRect: TSquare): TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.CurRect := ANewRect;

  {Make absolute}
  result := AddPointX(FRotCenter, Drawer.OriginalRect.TopLeft);
  with FTempDrawer do
    result := Dot(PXS(result.X), PYS(result.Y));
  result := SubPointX(result, ANewRect.TopLeft);
  FTempDrawer.PopState;
end;

procedure TCustomPolygonBlock.UpdatePolygonHandles;
var
  c: integer;
begin
  for c := 0 to FPolygon.Count - 1 do
  begin
    if FPolygon[c].FHandle = nil then
      FPolygon[c].FHandle := Handles.Add;
    FPolygon[c].FHandle.Data := -1;
    {FPolygon[c].FHandle.OrX := FPolygon[c].X;
    FPolygon[c].FHandle.OrY := FPolygon[c].Y;}
  end;
end;

procedure TCustomPolygonBlock.UpdatePolygonLinkPoints;
begin
end;

procedure TCustomPolygonBlock.ChangeOriginalRect(ARect: TSquare);
begin
  FRotCenter := UpdateRotationCenter(ARect);
  Drawer.OriginalRect := ARect;
end;

function TCustomPolygonBlock.BuildOriginalRect(APolygon: TPolygonPoints): TSquare;
var
  MinP, MaxP: TDot;
  c: integer;
begin
  if APolygon.Count > 0 then
  begin
    MinP := Dot(MaxDouble, MaxDouble);
    MaxP := Dot(-MaxDouble, -MaxDouble);
    for c := 0 to APolygon.Count - 1 do
    begin
      if APolygon[c].X < MinP.X then
        MinP.X := APolygon[c].X;
      if APolygon[c].Y < MinP.Y then
        MinP.Y := APolygon[c].Y;
      if APolygon[c].X > MaxP.X then
        MaxP.X := APolygon[c].X;
      if APolygon[c].Y > MaxP.Y then
        MaxP.Y := APolygon[c].Y;
    end;
    result := Square(MinP.X, MinP.Y, MaxP.X, MaxP.Y);
  end
  else
    result := Square(0, 0, 0, 0);
end;

procedure TCustomPolygonBlock.UpdateDrawing(AInfo: TControlDrawInfo);

  procedure DrawCursorLine;
  var
    AOldBrush: TBrush;
    AOldPen: TPen;
    P: TPoint;
  begin
    AOldBrush := TBrush.Create;
    AOldPen := TPen.Create;
    try
      AOldBrush.Assign(AInfo.Canvas.Brush);
      AOldPen.Assign(AInfo.Canvas.Pen);
      AInfo.Canvas.Brush.Style := bsClear;
      AInfo.Canvas.Pen.Mode := pmNot;
      AInfo.Canvas.Pen.Style := psDot;
      {Draw the line}
      if FPolygon.Count > 0 then
      begin
        P := RoundPoint(AInfo.Diagram.ClientToCanvas(FPolygon[FPolygon.Count - 1].P));
        AInfo.Canvas.MoveTo(P.X, P.Y);
        P := RoundPoint(AInfo.Diagram.ClientToCanvas(TempPolygonPoint));
        AInfo.Canvas.LineTo(P.X, P.Y);
      end;
    finally
      AInfo.Canvas.Pen.Assign(AOldPen);
      AInfo.Canvas.Brush.Assign(AOldBrush);
      AOldBrush.Free;
      AOldPen.Free;
    end;
  end;

  procedure DrawStepLine;
  var
    AOldPen: TPen;
    P: TPoint;
  begin
    AOldPen := TPen.Create;
    AOldPen.Assign(AInfo.Canvas.Pen);
    try
      AInfo.Canvas.Pen.Assign(Self.Pen);
      if FPolygon.Count > 1 then
      begin
        P := RoundPoint(AInfo.Diagram.ClientToCanvas(FPolygon[FPolygon.Count - 2].P));
        AInfo.Canvas.MoveTo(P.X, P.Y);
        P := RoundPoint(AInfo.Diagram.ClientToCanvas(FPolygon[FPolygon.Count - 1].P));
        AInfo.Canvas.LineTo(P.X, P.Y);
      end;
    finally
      AInfo.Canvas.Pen.Assign(AOldPen);
      AOldPen.Free;
    end;
  end;

begin
  With AInfo do
    Case Action of
      daStart:
        begin
          FPolygon.Clear;
          FPolygon.Add(AInfo.P1.X, AInfo.P1.Y);
        end;
      daStep:
        begin
          FPolygon.Add(AInfo.P2.X, AInfo.P2.Y);
          DrawStepLine;
        end;
      daDrawCursor, daEraseCursor:
        begin
          if Action = daDrawCursor then
            TempPolygonPoint := AInfo.P2;
          DrawCursorLine;
        end;
      daInserted:
        begin
          ChangeOriginalRect(BuildOriginalRect(FPolygon));
          Drawer.OriginalRect := BuildOriginalRect(FPolygon);
          BoundsRect := Drawer.OriginalRect;
          PolygonChanged;
        end;
    end;
end;

procedure TCustomPolygonBlock.DrawHandle(AInfo: TDiagramDrawInfo; AHandle: TStretchHandle);
begin
  if (AHandle <> nil) and (AHandle.Data = -1) then
  begin
    GetDefaultHandlesAppearance(FHandlesAppearance);
    FHandlesAppearance.Size.Brush.Color := PointColor;
    PaintHandle(AInfo, AHandle, FHandlesAppearance);
  end
  else
    inherited;
end;

procedure TCustomPolygonBlock.DrawOutlineCursor(ACanvas: TCanvas; APolygon: TPolygonPoints);
var
  AOldBrush: TBrush;
  AOldPen: TPen;
begin
  begin
    AOldBrush := TBrush.Create;
    AOldPen := TPen.Create;
    try
      AOldBrush.Assign(ACanvas.Brush);
      AOldPen.Assign(ACanvas.Pen);
      ACanvas.Brush.Style := bsClear;
      ACanvas.Pen.Mode := pmNot;
      ACanvas.Pen.Style := psDot;

      CanvasPolygon(APolygon);
      ACanvas.Polygon(RoundDotArray(APolygon.PointArray));
    finally
      ACanvas.Pen.Assign(AOldPen);
      ACanvas.Brush.Assign(AOldBrush);
      AOldBrush.Free;
      AOldPen.Free;
    end;
  end;
end;

procedure TCustomPolygonBlock.UpdateMoving(AInfo: TControlMoveInfo);
begin
  With AInfo do
    Case Action of
      caEraseCursor, caDrawCursor:
        begin
          FTempPolygon.Assign(FPolygon);
          RepositionPolygon(FTempPolygon, MoveRectByDelta(BoundsRect, Delta));
          DrawOutlineCursor(AInfo.Canvas, FTempPolygon);
        end;
      caPositionCursor:
        begin
          FCursorPolygon.Assign(FPolygon);
          RepositionPolygon(FCursorPolygon, MoveRectByDelta(BoundsRect, Delta));
          FCursorAngle := Angle;
          FCursorRect := MoveRectByDelta(BoundsRect, Delta);
          FCursorRotCenter := FRotCenter;
        end;
      caMoved:
        BoundsRect := MoveRectByDelta(BoundsRect, Delta);
    end;
end;

procedure TCustomPolygonBlock.HandleMoving(AInfo: THandleMoveInfo);

  procedure AddDeltaToPolygonPoint(APoint: TDrawPoint);
  begin
    APoint.P := AddPointX(APoint.P, AInfo.Delta);
  end;

var
  APolygonPoint: TPolygonPoint;
begin
  With AInfo do
  begin
    if AInfo.AHandle <> nil then
      APolygonPoint := FPolygon.FindByHandle(AInfo.AHandle)
    else
      APolygonPoint := nil;
    if APolygonPoint <> nil then
    begin
      Case Action of
        haEraseCursor, haDrawCursor:
          begin
            FTempPolygon.Assign(FPolygon);
            RotatePolygon(FTempPolygon, Angle);
            AddDeltaToPolygonPoint(FTempPolygon[APolygonPoint.Index]);
            DrawOutlineCursor(AInfo.Canvas, FTempPolygon);
          end;
        haPositionCursor:
          begin
            FCursorAngle := Angle;
            FCursorPolygon.Assign(FPolygon);
            RotatePolygon(FCursorPolygon, Angle);
            AddDeltaToPolygonPoint(FCursorPolygon[APolygonPoint.Index]);
            RotatePolygon(FCursorPolygon, -Angle);
            //RepositionPolygon(FCursorPolygon, ResizeNewRect(AHandle, Delta.X, Delta.Y));
            FCursorRect := BuildOriginalRect(FCursorPolygon);
            FCursorRotCenter := UpdateRotationCenter(FCursorRect);
          end;
        haMoved:
          begin
            FTempPolygon.Assign(FPolygon);
            RotatePolygon(FPolygon, Angle);
            AddDeltaToPolygonPoint(APolygonPoint);
            RotatePolygon(FPolygon, -Angle{, FTempPolygon});
            ChangeOriginalRect(BuildOriginalRect(FPolygon));
            BoundsRect := Drawer.OriginalRect;
            PolygonChanged;
          end;
      end;
    end else
      With AInfo do
        Case Action of
          haEraseCursor, haDrawCursor:
            Case AHandle.Style of
              hsRotate:
                begin
                  FTempPolygon.Assign(FPolygon);
                  RepositionPolygon(FTempPolygon, RotateNewRect(AHandle, Delta.X, Delta.Y));
                  RotatePolygon(FTempPolygon, RotateNewAngle(AHandle, Delta.X, Delta.Y));
                  DrawOutlineCursor(AInfo.Canvas, FTempPolygon);
                end;
              hsSize:
                begin
                  FTempPolygon.Assign(FPolygon);
                  RepositionPolygon(FTempPolygon, ResizeNewRect(AHandle, Delta.X, Delta.Y));
                  RotatePolygon(FTempPolygon, Angle);
                  DrawOutlineCursor(AInfo.Canvas, FTempPolygon);
                end;
            end;
          haPositionCursor:
            begin
              FCursorPolygon.Assign(FPolygon);
              FCursorAngle := Angle;
              FCursorRotCenter := RotationCenter;
              Case AHandle.Style of
                hsRotate:
                  begin
                    RepositionPolygon(FCursorPolygon, RotateNewRect(AHandle, Delta.X, Delta.Y));
                    FCursorAngle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
                    FCursorRect := BuildOriginalRect(FCursorPolygon);
                  end;
                hsSize:
                  begin
                    RepositionPolygon(FCursorPolygon, ResizeNewRect(AHandle, Delta.X, Delta.Y));
                    FCursorRect := BuildOriginalRect(FCursorPolygon);
                    FCursorRotCenter := UpdateRotationCenter(FCursorRect);
                  end;
                hsRotCenter:
                  begin
                    FCursorRotCenter := CalcNewPolygonRotCenter(AHandle, Delta.X, Delta.Y);
                    FCursorRect := BuildOriginalRect(FCursorPolygon);
                  end;
              end;
            end;
          haMoved:
            Case AHandle.Style of
              hsRotate:
                begin
                  BoundsRect := RotateNewRect(AHandle, Delta.X, Delta.Y); 
                  Angle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
                end;
              hsSize:
                BoundsRect := ResizeNewRect(AHandle, Delta.X, Delta.Y);
              hsRotCenter:
                RotationCenter := CalcNewPolygonRotCenter(AHandle, Delta.X, Delta.Y);
            end;
        end;
  end;
end;

destructor TCustomPolygonBlock.Destroy;
var
  c: integer;
begin
  {Remove handles created in polygon}
  for c := 0 to FPolygon.Count - 1 do
    if FPolygon[c].FHandle <> nil then
    begin
      FPolygon[c].FHandle.Free;
      FPolygon[c].FHandle := nil;
    end;
  FPolygon.Free;
  FTempPolygon.Free;
  FCursorPolygon.Free;
  FSavePolygon.Free;
  inherited;
end;

procedure TCustomPolygonBlock.SetPointColor(const Value: TColor);
begin
  if FPointColor <> Value then
  begin
    FPointColor := Value;
    Redraw;
  end;
end;

procedure TCustomPolygonBlock.StartCursorPlacement;
var
  c: integer;
begin
  inherited;
  FSavePolygon.Assign(FPolygon);

  {Save handles}
  for c := 0 to FPolygon.Count - 1 do
    FSavePolygon[c].FHandle := FPolygon[c].FHandle;

  FPolygon.Assign(FCursorPolygon);

  {Retrieve handles}
  for c := 0 to FPolygon.Count - 1 do
    FPolygon[c].FHandle := FSavePolygon[c].FHandle;
end;

procedure TCustomPolygonBlock.EndCursorPlacement;
var
  c: integer;
begin
  FPolygon.Assign(FSavePolygon);

  {Save handles}
  for c := 0 to FPolygon.Count - 1 do
    FPolygon[c].FHandle := FSavePolygon[c].FHandle;
  inherited;
end;

function TCustomPolygonBlock.GetTextCellRect(ACell: TTextCell; ARect: TSquare): TSquare;
var
  OldOriginalRect: TSquare;
begin
  {TextCells still uses the original rect of 0, 0, 100, 100}
  OldOriginalRect := Drawer.OriginalRect;
  Drawer.OriginalRect := Square(0, 0, 100, 100);
  try
    result := inherited GetTextCellRect(ACell, ARect);
  finally
    Drawer.OriginalRect := OldOriginalRect;
  end;
end;

procedure TCustomPolygonBlock.LoadPolygonProp(Reader: TReader);
begin
  Reader.ReadValue;
  Reader.ReadCollection(FPolygon);
end;

procedure TCustomPolygonBlock.StorePolygonProp(Writer: TWriter);
begin
  Writer.WriteCollection(FPolygon);
end;

procedure TCustomPolygonBlock.DefineProperties(Filer: TFiler);
begin
  inherited; { allow base classes to define properties }
  Filer.DefineProperty('Polygon', LoadPolygonProp, StorePolygonProp, true);
end;

procedure TCustomPolygonBlock.Loaded;
begin
  inherited;
  PolygonChanged;
end;

procedure TCustomPolygonBlock.ChangePolygon(APolygon: TPolygonPoints);
begin
//  FPolygon.Assign(APolygon);
  PolygonChanged;
end;

procedure TCustomPolygonBlock.GetTranslatedBlockPath(APath: TDgrGraphicsPath; ADrawer: TDgrBlockDrawer);
var
  Pts: TDotArray;
  c: integer;
begin
  FTempPolygon.Assign(FPolygon);
  RepositionPolygon(FTempPolygon, ADrawer.DestRect);
  SetLength(Pts, FTempPolygon.Count);
  for c := 0 to FTempPolygon.Count - 1 do
  begin
    Pts[c].X := FTempPolygon[c].X;
    Pts[c].Y := FTempPolygon[c].Y;
  end;
  APath.AddPolygon(Pts);
  ADrawer.RotatePath(APath);
  SetLength(Pts, 0);
end;

procedure TCustomPolygonBlock.BoundsChanged(AOld, ANew: TSquare);
begin
  inherited;
  if FPolygon <> nil then
  begin
    RepositionPolygon(FPolygon, ANew);
    PolygonChanged;
  end;
end;

procedure TCustomPolygonBlock.RotatePolygon(APolygon: TPolygonPoints; AAngle: double;
  ARotationBasePolygon: TPolygonPoints = nil);
var
  c: integer;
  Pts: TDotArray;
begin
  if APolygon <> nil then
  begin
    if ARotationbasePolygon = nil then
      ARotationBasePolygon := APolygon;
    SetLength(Pts, APolygon.Count);
    for c := 0 to APolygon.Count - 1 do
      Pts[c] := Dot(APolygon[c].X, APolygon[c].Y);
    TempDrawer.OriginalRect := BuildOriginalRect(ARotationBasePolygon);
    TempDrawer.CurRect := TempDrawer.OriginalRect;
    TempDrawer.Angle := AAngle;
    Pts := TempDrawer.RotPoly(Pts);
    for c := 0 to Length(Pts) - 1 do
    begin
      APolygon[c].X := Pts[c].X;
      APolygon[c].Y := Pts[c].Y;
    end;
  end;
end;

procedure TCustomPolygonBlock.CanvasPolygon(APolygon: TPolygonPoints);
var
  c: integer;
  P: TDot;
begin
  if FDiagram <> nil then
    for c := 0 to APolygon.Count - 1 do
    begin
      P := FDiagram.ClientToCanvas(APolygon[c].P);
      APolygon[c].X := P.X;
      APolygon[c].Y := P.Y;
    end;
end;

function TCustomPolygonBlock.DiagramRotCenter: TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.OriginalRect := Drawer.OriginalRect;
  FTempDrawer.CurRect := BoundsRect;
  FTempDrawer.Angle := Angle;
  FTempDrawer.RotationType := rtCurRectCenter;
  result := FTempDrawer.RotX(AddPointX(FRotCenter, BoundsRect.TopLeft));
  FTempDrawer.PopState;
end;

function TCustomPolygonBlock.CalcNewPolygonRotCenter(AHandle: TStretchHandle; ADeltaX, ADeltaY: number): TDot;
var
  ANewDiagRotCenter: TDot;
begin
  FTempDrawer.PushState;
  FTempDrawer.CurRect := BoundsRect;
  FTempDrawer.RotationType := rtCurRectCenter;
  FTempDrawer.Angle := Angle;
  with FTempDrawer do
  begin
    ANewDiagRotCenter := AddPointX(DiagramRotCenter, Dot(ADeltaX, ADeltaY));
    ANewDiagRotCenter := UnrotX(ANewDiagRotCenter);
    result := SubPointX(ANewDiagRotCenter, BoundsRect.TopLeft);
  end;
  FTempDrawer.PopState;
end;

{ TPolygonPoints }

function TPolygonPoints.Add: TPolygonPoint;
begin
  result := TPolygonPoint(inherited Add);
end;

function TPolygonPoints.Add(AX, AY: number): TPolygonPoint;
begin
  result := Add;
  result.X := AX;
  result.Y := AY
end;

function TPolygonPoints.GetItem(Index: integer): TPolygonPoint;
begin
  result := TPolygonPoint(inherited Items[Index]);
end;

procedure TPolygonPoints.SetItem(Index: integer;
  const Value: TPolygonPoint);
begin
  Items[Index].Assign(Value);
end;

function TPolygonPoints.FindByHandle(AHandle: TStretchHandle): TPolygonPoint;
var
  c: integer;
begin
  result := nil;
  for c := 0 to Count - 1 do
    if Items[c].Handle = AHandle then
    begin
      result := Items[c];
      break;
    end;
end;

function TPolygonPoints.PointArray: TDotArray;
var
  c: integer;
begin
  SetLength(result, Count + 1);
  for c := 0 to Count - 1 do
    result[c] := Dot(Items[c].X, Items[c].Y);
  result[Count] := result[0];
end;

{ TPolygonPoint }

procedure TPolygonPoint.Assign(Source: TPersistent);
begin
  inherited Assign(Source);
  if Source is TPolygonPoint then
  begin
    //FLinkPoint := TPolygonPoint(Source).FLinkPoint;

    {do not copy FHandle}
    //FHandle := TPolygonPoint(Source).FHandle;
  end;
end;

destructor TPolygonPoint.Destroy;
begin
  if (Collection is TPolygonPoints) and TPolygonPoints(Collection).FDestroyObjects then
  begin
    {if Handle <> nil then
      Handle.Free;}
    {if LinkPoint <> nil then
      LinkPoint.Free;}
  end;
  inherited;
end;

{ TDiagramArc }

constructor TDiagramArc.Create(AOwner: TComponent);
var
  ACalcInfo: TCalcHandlesInfo;
begin
  inherited;
  FArcType := atParabolic;
  LineStyle := lsParabolicArc;

  {Recreate the arc in a different position - it should not have a point in 0,0,
   otherwise part of the arc will be outside the diagram, and the diagram
   will automatically scroll down}
  ACalcInfo.P1 := Dot(50, 50);
  ACalcInfo.P2 := Dot(100, 100);
  ACalcInfo.Anchor1 := nil;
  ACalcInfo.Anchor2 := nil;
  ACalcInfo.HandleMoved := nil;
  ACalcInfo.Action := chNone;
  CalcNewHandles(Handles, ACalcInfo);
  TextCellsMode := tmSingle;
end;

procedure TDiagramArc.SetArcType(const Value: TArcType);
begin
  if FArcType <> Value then
  begin
    FArcType := Value;

    {Only change line type to an arc if it's *already* an arc}
    if FLineStyle in [lsParabolicArc, lsCircularArc] then
      Case FArcType of
        atParabolic:
          LineStyle := lsParabolicArc
      else
        LineStyle := lsCircularArc;
      end;
    //Redraw;
  end;
end;

{ TDiagramBezier }

constructor TDiagramBezier.Create(AOwner: TComponent);
begin
  inherited;
  TextCellsMode := tmSingle;
  LineStyle := lsBezier;
end;

{ TDiagramPolyBezier }

constructor TDiagramPolyBezier.Create(AOwner: TComponent);
begin
  inherited;
  FInsertStyle := isDraw;
end;

procedure TDiagramPolyBezier.UpdateDrawing(AInfo: TControlDrawInfo); 
begin
  if AInfo.Action = daStep then
  begin
    Handles.Add.OrPoint :=
      RotatePointByLines(Dot(10, 0), Dot(0, 0), Dot(100, 0), AInfo.P1, AInfo.P2);
    Handles.Add.OrPoint :=
      RotatePointByLines(Dot(90, 0), Dot(0, 0), Dot(100, 0), AInfo.P1, AInfo.P2);
  end;
  inherited UpdateDrawing(AInfo);
end;

procedure TDiagramPolyBezier.CalcNewHandles(AHandles: TStretchHandles;
  AInfo: TCalcHandlesInfo);
var
  H1, H2: TStretchHandle;

  procedure CalculateIntermediateHandles;
  var
    c: integer;
  begin
    for c := 1 to AHandles.Count - 2 do
      AHandles[c].OrPoint := RotatePointByLines(AHandles[c].OrPoint,
        H1.OrPoint, H2.OrPoint, AInfo.P1, AInfo.P2);
  end;

  procedure AdjustHandles;
  var
    A, B: double;
    c: integer;
    H1, H2, H3: TDot;
    V1X, V1Y, V3X, V3Y: double;
  begin
    c := 3;
    while c <= AHandles.Count - 4 do
    begin
      H1 := AHandles[c - 1].OrPoint;
      H2 := AHandles[c].OrPoint;
      H3 := AHandles[c + 1].OrPoint;
      A := VectorAngle(H1.X, H1.Y, H2.X, H2.Y);
      B := VectorAngle(H3.X, H3.Y, H2.X, H2.Y);
      MakeVector(H2.X, H2.Y, PointDistance(H1.X, H1.Y, H2.X, H2.Y), (A + B) / 2 - PI / 2, V1X, V1Y);
      MakeVector(H2.X, H2.Y, PointDistance(H3.X, H3.Y, H2.X, H2.Y), (A + B) / 2 + PI / 2, V3X, V3Y);
      AHandles[c - 1].OrPoint := Dot(V1X, V1Y);
      AHandles[c + 1].OrPoint := Dot(V3X, V3Y);
      c := c + 3;
    end;
  end;

begin
  {Be sure that there are three handles in the line. This will probably never happen}
  While AHandles.Count < 4 do
    AHandles.Add(0, 0); //<-- Call Add(0, 0) to set default properties

  H1 := AHandles[0]; {Start point handle}
  H2 := AHandles[AHandles.Count - 1]; {End point handle}

  With AInfo do
  begin
    Case Action of
      chNone, chInsert: ;
        //CalculateInitialHandles;
      chMove, chAnchorChanged:
        begin
          CalculateIntermediateHandles;
          AdjustHandles;
        end;
      chHandleMove:
        {if (HandleMoved.Index = 0) or (HandleMoved.Index = AHandles.Count - 1) then
          CalculateIntermediateHandles
        else}
        begin
          AHandles[HandleMoved.Index].OrPoint := AddPointX(HandleMoved.OrPoint, DeltaMove);
          if ((HandleMoved.Index mod 3) = 0) and (HandleMoved.Index > 0) then
            AHandles[HandleMoved.Index - 1].OrPoint := AddPointX(
              AHandles[HandleMoved.Index - 1].OrPoint, DeltaMove);
          if ((HandleMoved.Index mod 3) = 0) and (HandleMoved.Index < AHandles.Count - 1) then
            AHandles[HandleMoved.Index + 1].OrPoint := AddPointX(
              AHandles[HandleMoved.Index + 1].OrPoint, DeltaMove);
        end;
    end;
    H1.OrPoint := Dot(P1.X, P1.Y);
    H2.OrPoint := Dot(P2.X, P2.Y);
  end;
end;

procedure TDiagramPolyBezier.RecalcPoints(AHandles: TStretchHandles);
var
  Tg1, Tg2: TDot;
  H1, H2: TDot;
  c: integer;
begin
  SetLength(FPoints, 0);
  if (AHandles.Count > 0) and ((AHandles.Count - 1) mod 3 = 0) then
  begin
    c := 3;
    while C < AHandles.Count do
    begin
      H1 := AHandles[c - 3].OrPoint;
      H2 := AHandles[c].OrPoint;
      Tg1 := SubPointX(AHandles[c - 2].OrPoint, H1);
      Tg2 := SubPointX(AHandles[c - 1].OrPoint, H2);
      FPoints := ConcatPointsX(FPoints,
        Bezier(H1.X, H1.Y, Tg1.X, Tg1.Y, H2.X, H2.Y, Tg2.X, Tg2.Y, 10));
      c := c + 3;
    end;
  end;
end;

{ TStreamDiagram }

constructor TStreamDiagram.Create(Owner: TComponent);
begin
  inherited;
  SetBounds(0, 0, 0, 0);
end;

procedure TStreamDiagram.PaintWindow(DC: HDC);
begin
end;

procedure TStreamDiagram.WMNCPaint(var Message: TMessage);
begin
end;

{ TTextCellFrame }

procedure TTextCellFrame.Assign(Source: TPersistent);
begin
  if Source is TTextCell then
  begin
    FColor := TTextCellFrame(Source).FColor;
    FTransparent := TTextCellFrame(Source).FTransparent;
    FAutoFrame := TTextCellFrame(Source).FAutoFrame;
    FAutoFrameMargin := TTextCellFrame(Source).FAutoFrameMargin;
    Brush.Assign(TTextCellFrame(Source).Brush);
    Pen.Assign(TTextCellFrame(Source).Pen);
    FVisible := TTextCellFrame(Source).FVisible;
 end else
    inherited Assign(Source);
end;

procedure TTextCellFrame.Changed;
begin
  if FTextCell <> nil then
   FTextCell.DControl.Redraw;
end;

constructor TTextCellFrame.Create(ATextCell: TTextCell);
begin
  FVisible := false;
  FTextCell := ATextCell;
  FColor := clNone;
  FPen := TPen.Create;
  FBrush := TBrush.Create;
  FPen.OnChange := StyleChanged;
  FBrush.OnChange := StyleChanged;
  FTransparent := true;
  FColor := clWhite;
  FAutoFrame := false;
  FAutoFrameMargin := 0;
end;

destructor TTextCellFrame.Destroy;
begin
  FBrush.Free;
  FPen.Free;
  inherited;
end;

procedure TTextCellFrame.SetAutoFrame(const Value: boolean);
begin
  if FAutoFrame <> Value then
  begin
    FAutoFrame := Value;
    Changed;
  end;
end;

procedure TTextCellFrame.SetAutoFrameMargin(const Value: integer);
begin
  if FAutoFrameMargin <> Value then
  begin
    FAutoFrameMargin := Value;
    Changed;
  end;
end;

procedure TTextCellFrame.SetBrush(const Value: TBrush);
begin
  FBrush.Assign(Value);
end;

procedure TTextCellFrame.SetColor(const Value: TColor);
begin
  if FColor <> Value then
  begin
    FColor := Value;
    Changed;
  end;
end;

procedure TTextCellFrame.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure TTextCellFrame.SetTransparent(const Value: boolean);
begin
  if FTransparent <> Value then
  begin
    FTransparent := Value;
    Changed;
  end;
end;

procedure TTextCellFrame.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    Changed;
  end;
end;

procedure TTextCellFrame.StyleChanged(Sender: TObject);  
begin
  Changed;
end;

{ TPageLines }

procedure TPageLines.Assign(Source: TPersistent);
begin
  if Source is TPageLines then
  begin
    FVisible := TPageLines(Source).FVisible;
    {Leave pen assignment at the end, so Changed method is called automatically}
    FPen.Assign(TPageLines(Source).FPen);
  end else
    inherited Assign(Source);
end;

procedure TPageLines.Changed;
begin
  if Assigned(FDiagram) then
    FDiagram.Redraw;
end;

constructor TPageLines.Create(ADiagram: TatDiagram);
begin
  inherited Create;
  FDiagram := ADiagram;
  FVisible := false;
  FPen := TPen.Create;
  FPen.Style := psDot;
  FPen.Color := clBlack;
  FPen.OnChange := StyleChanged;
end;

destructor TPageLines.Destroy;
begin
  FPen.Free;
  inherited;
end;

procedure TPageLines.Draw(Canvas: TCanvas);
var
  c: integer;
  Pos: number;
  P: TDot;
  CanvasRect: TSquare;
  SavePen: TPen;
begin
  if FDiagram <> nil then With FDiagram do
  begin
    SavePen := TPen.Create;
    try
      SavePen.Assign(Canvas.Pen);
      Canvas.Pen := Self.Pen;
      FDiagram.UpdatePixelConvertion;
      CanvasRect := ClipRect(Canvas);
      if (FPrintConvX > 0) and (FPrintConvY > 0) and (PrintClientWidth > 0) and (PrintClientHeight > 0) then
      begin
        {Draw vertical lines}
        c := 1;
        repeat
          Pos := PrintClientWidth / FPrintConvX * c;
          P := ClientToCanvas(Dot(Pos, 0));
          if (P.X > CanvasRect.Left) and (P.X < CanvasRect.Right) then
          begin
            Canvas.MoveTo(Round(P.X), TopRuler.VisibleSize);
            Canvas.LineTo(Round(P.X), Round(CanvasRect.Bottom));
          end;
          inc(c);
        until P.X > CanvasRect.Right;

        {Draw horizontal lines}
        c := 1;
        repeat
          Pos := PrintClientHeight / FPrintConvY * c;
          P := ClientToCanvas(Dot(0, Pos));
          if (P.Y > CanvasRect.Top) and (P.Y < CanvasRect.Bottom) then
          begin
            Canvas.MoveTo(LeftRuler.VisibleSize, Round(P.Y));
            Canvas.LineTo(Round(CanvasRect.Right), Round(P.Y));
          end;
          inc(c);
        until P.Y > CanvasRect.Bottom;
      end;
      Canvas.Pen.Assign(SavePen);
    finally
      SavePen.Free;
    end;
  end;
end;

procedure TPageLines.SetPen(const Value: TPen);
begin
  FPen.Assign(Value);
end;

procedure TPageLines.SetVisible(const Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value and (FDiagram <> nil){ and (FDiagram.Printer <> nil)};
    Changed;
  end;
end;

procedure TPageLines.StyleChanged(Sender: TObject);
begin
  Changed;
end;

{ TGroupBlock }

constructor TBaseGroupBlock.Create(AOwner: TComponent);
begin
  {Crete members before inherited to guarantee that FMembers is always accesible}
  FMembers := TGroupMemberList.Create(nil, TDControlListItem);
  FMembers.FGroup := Self;
  inherited;
  Shape := bsNoShape;
end;

procedure _PrepareMemberCursor(ADControl: TDiagramControl);
begin
  if ADControl is TCustomDiagramBlock then
  begin
    TCustomDiagramBlock(ADControl).FCursorRect := TCustomDiagramBlock(ADControl).BoundsRect;
    TCustomDiagramBlock(ADControl).FCursorAngle := TCustomDiagramBlock(ADControl).Angle;
    TCustomDiagramBlock(ADControl).FCursorRotCenter := TCustomDiagramBlock(ADControl).RotationCenter;
    {if ADControl is TPolygonBlock then
      TPolygonBlock(ADControl).FCursorP}
  end else
  if ADControl is TCustomDiagramLine then
  begin
    TCustomDiagramLine(ADControl).FTempHandles.Assign(TCustomDiagramLine(ADControl).Handles)
  end;
end;

procedure TBaseGroupBlock.HandleMoving(AInfo: THandleMoveInfo);

  function UpdatedAnchorLink(ALine: TCustomDiagramLine; ALinkPoint: TLinkPoint): TLinkPoint;
  begin
    {This function returns the anchor link for the link point, or nil,
     if the anchor must be removed for some reason}

    result := ALinkPoint;

    {if line is anchored to a control that is not being moved/rotated, then
     we must remove the link to it, unless the line requires connections}
    if (result <> nil) and (ALine <> nil) and not ALine.RequiresConnections and
      (result.AnchorLink <> nil) and (result.AnchorLink.DControl <> nil)
     and not result.AnchorLink.DControl.Selected then
      result := nil;

    if result <> nil then
      result := result.AnchorLink;
  end;

var
  c: integer;
  ALine: TCustomDiagramLine;
begin
  UpdateGroupMembers;
  with AInfo do
  begin
    case Action of
      haEraseCursor, haDrawCursor:
        Case AHandle.Style of
          hsRotate:
            DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(
              FDiagram.ClientToCanvas(RotateNewRect(AHandle, Delta.X, Delta.Y)),
              RotateNewAngle(AHandle, Delta.X, Delta.Y)));
          hsSize:
            DrawBlockCursor(DiagramDrawInfo(Canvas, Drawer, FDgrDrawer), DiagramDrawBlockInfo(
              FDiagram.ClientToCanvas(ResizeNewRect(AHandle, Delta.X, Delta.Y)), Angle));
        end;
      haPositionCursor:
        begin
          {Update the cursor of the selection block}
          FCursorRect := BoundsRect;
          FCursorAngle := Angle;
          FCursorRotCenter := RotationCenter;
          Case AHandle.Style of
            hsRotate:
              begin
                IterateAllMembers(_PrepareMemberCursor);
                FCursorRect := RotateNewRect(AHandle, Delta.X, Delta.Y);
                {Simulate BoundsChanged}
                ResizeMembers(BoundsRect, FCursorRect, true);

                FCursorAngle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
                {Simulate AngleChanged}
                RotateMembers(FCursorAngle - Angle, true);
              end;
            hsSize:
              begin
                IterateAllMembers(_PrepareMemberCursor);
                FCursorRect := ResizeNewRect(AHandle, Delta.X, Delta.Y);
                {Simulate BoundsChanged}
                ResizeMembers(BoundsRect, FCursorRect, true);
              end;
            hsRotCenter:
              begin
                IterateAllMembers(_PrepareMemberCursor);
                FCursorRotCenter := CalcNewRotCenter(AHandle, Delta.X, Delta.Y);
              end;
          end;
        end;
      haMoved:
        begin
          Diagram.BeginUpdatePos;
          try
            {Remove anchors that are not being moved together. This must be done
             before setting new block position, because this will allow the line members
             to be positioned accordingly (not connected to anchors that will be removed)}
            for c := 0 to FMembers.Count - 1 do if FMembers[c].DControl <> nil then
              if FMembers[c].DControl is TCustomDiagramLine then
              begin
                ALine := TCustomDiagramLine(FMembers[c].DControl);
                ALine.SourceLinkPoint.AnchorLink := UpdatedAnchorLink(ALine, ALine.SourceLinkPoint);
                ALine.TargetLinkPoint.AnchorLink := UpdatedAnchorLink(ALine, ALine.TargetLinkPoint);
              end;


            Case AHandle.Style of
              hsRotate:
                begin
                  BoundsRect := RotateNewRect(AHandle, Delta.X, Delta.Y);
                  Angle := RotateNewAngle(AHandle, Delta.X, Delta.Y);
                end;
              hsSize:
                begin
                  BoundsRect := ResizeNewRect(AHandle, Delta.X, Delta.Y);
                end;
              hsRotCenter:
                begin
                  RotationCenter := CalcNewRotCenter(AHandle, Delta.X, Delta.Y);
                end;
            end;
          finally
            Diagram.EndUpdatePos;
          end;
        end;
    end;
  end;
end;


function TBaseGroupBlock.HasSelecteds: boolean;

  function _AnyMemberSelected(AGroup: TBaseGroupBlock): boolean;
  var
    c: Integer;
  begin
    result := false;
    for c := 0 to AGroup.Members.Count - 1 do
      if AGroup.Members[c].DControl <> nil then
      begin
        result := result or AGroup.Members[c].DControl.Selected;
        if AGroup.Members[c].DControl.IsGroup then
          result := result or _AnyMemberSelected(TGroupBlock(AGroup.Members[c].DControl));

        if result then
          exit;
      end;
  end;

begin
  result := Self.Selected or _AnyMemberSelected(Self);
end;

function TBaseGroupBlock.ResizeMemberRect(ABlock: TCustomDiagramBlock;
  ABlockRect: TSquare; AOldGroup, ANewGroup: TSquare): TSquare;
var
  NewRect: TSquare;
  NewC: TDot;
  NewW, NewH: number;
  OldC: TDot;
  OldW, OldH: number;

  procedure _UpdatePars;
  begin
    OldC := Dot((result.Left + result.Right) / 2, (result.Top + result.Bottom) / 2);
    OldW := result.Right - result.Left;
    OldH := result.Bottom - result.Top;
    NewC := Dot((NewRect.Left + NewRect.Right) / 2, (NewRect.Top + NewRect.Bottom) / 2);
    NewW := NewRect.Right - NewRect.Left;
    NewH := NewRect.Bottom - NewRect.Top;
  end;

var
  AAngle: double;
begin
  AAngle := Self.Angle;

  result := ABlockRect;
  FTempDrawer.PushState;

  {Rotate the block back to original position (angle = 0)}
  FTempDrawer.RotationType := rtRotationCenter;
  FTempDrawer.Angle := -AAngle;
  {Rotation center must be the central point of the grouping block}
  FTempDrawer.RotationCenter := Dot((AOldGroup.Left + AOldGroup.Right) / 2, (AOldGroup.Top + AOldGroup.Bottom) / 2);
  result := FTempDrawer.RotByCenter(result);

  {Now resize rect in proportion to the resized selection block}
  FTempDrawer.OriginalRect := AOldGroup;
  FTempDrawer.CurRect := ANewGroup;
  NewRect := Square(
    FTempDrawer.PX(result.Left),
    FTempDrawer.PY(result.Top),
    FTempDrawer.PX(result.Right),
    FTempDrawer.PY(result.Bottom));

  {Apply restrictions}
  if not ABlock.CanResize then
  begin
    {Keep previous size, shrinking/expanding from center}
    _UpdatePars;
    NewRect.Left := NewC.X - (OldW / 2);
    NewRect.Right := NewC.X + (OldW / 2);
    NewRect.Top:= NewC.Y - (OldH / 2);
    NewRect.Bottom := NewC.Y + (OldH / 2);
  end;

  if not ABlock.CanMove then
  begin
    {Keep same center}
    _UpdatePars;
    NewRect.Left := NewRect.Left + OldC.X - NewC.X;
    NewRect.Right := NewRect.Right + OldC.X - NewC.X;
    NewRect.Top:= NewRect.Top + OldC.Y - NewC.Y;
    NewRect.Bottom := NewRect.Bottom + OldC.Y - NewC.Y;
  end;

  if crKeepRatio in ABlock.Restrictions then
  begin
    NewRect := KeepRectRatio(NewRect, result);
  end;

  {Do not apply size restrictions for now. By design, the min/max width and height apply
  for blocks, but once it belongs to a group, restrictions do not apply anymore}
  {if NewRect.Right - NewRect.Left < ABlock.MinWidth then
    NewRect.Right := NewRect.Left + ABlock.MinWidth;
  if NewRect.Bottom - NewRect.Top < ABlock.MinHeight then
    NewRect.Top := NewRect.Bottom - ABlock.MinHeight;}

  result := NewRect;

  {And not rotate the block again based on new rect}
  FTempDrawer.RotationType := rtRotationCenter;
  FTempDrawer.Angle := AAngle;
  {Rotation center must be the central point of the grouping block}
  FTempDrawer.RotationCenter := Dot((ANewGroup.Left + ANewGroup.Right) / 2, (ANewGroup.Top + ANewGroup.Bottom) / 2);

  result := FTempDrawer.RotByCenter(result);

  FTempDrawer.PopState;
end;

procedure TBaseGroupBlock.MoveLineHandle(ALine: TCustomDiagramLine;
  AHandles: TStretchHandles; AHandle: TStretchHandle; ADeltaMove: TDot);
var
  ACalcInfo: TCalcHandlesInfo;
begin
  ACalcInfo.P1 := AHandles[0].OrPoint;
  if AHandle = AHandles[0] then
    ACalcInfo.P1 := AddPointX(ACalcInfo.P1, ADeltaMove);

  ACalcInfo.P2 := AHandles[AHandles.Count - 1].OrPoint;
  if AHandle = AHandles[AHandles.Count - 1] then
    ACalcInfo.P2 := AddPointX(ACalcInfo.P2, ADeltaMove);

  {set linkpoints just for orientation. WARNING, Anchor1 and Anchor2
   are only being used for orientation, if this behavior changes,
   then we must review this}
  {ACalcInfo.Anchor1 := UpdatedAnchorLink(ALine.SourceLinkPoint);
  ACalcInfo.Anchor2 := UpdatedAnchorLink(ALine.TargetLinkPoint);}
  ACalcInfo.Anchor1 := ALine.SourceLinkPoint;
  ACalcInfo.Anchor2 := ALine.TargetLinkPoint;

  ACalcInfo.HandleMoved := AHandle;
  ACalcInfo.Action := chHandleMove;
  ACalcInfo.DeltaMove := ADeltaMove;
  ALine.CalcNewHandles(AHandles, ACalcInfo);
end;

procedure TBaseGroupBlock.RotateMemberLine(ALine: TCustomDiagramLine;
  AHandles: TStretchHandles; AAngle: double; AGroupRect: TSquare);

  function _RotateHandle(H: TStretchHandle): TDot;
  begin
    FTempDrawer.PushState;
    FTempDrawer.RotationType := rtRotationCenter;

    FTempDrawer.RotationCenter := Dot((AGroupRect.Left + AGroupRect.Right) / 2, (AGroupRect.Top + AGroupRect.Bottom) / 2);
    //FTempDrawer.RotationCenter := RoundPoint(DiagramRotCenter);

    FTempDrawer.Angle := AAngle;

    result := SubPointX(FTempDrawer.RotX(H.OrPoint), H.OrPoint);

    FTempDrawer.PopState;
  end;

var
  c: integer;
begin
  if ALine.LineStyle in [lsSideLine, lsBezier, lsCircularArc, lsParabolicArc] then
  begin
    {Rotate initial and final handles, if not attached}
    MoveLineHandle(ALine, AHandles, AHandles[0], _RotateHandle(AHandles[0]));
    MoveLineHandle(ALine, AHandles, AHandles[AHandles.Count - 1], _RotateHandle(AHandles[AHandles.Count - 1]));
  end else
  begin
    {Rotate all handles}
    for c := 0 to ALine.Handles.Count - 1 do
      MoveLineHandle(ALine, AHandles, AHandles[c], _RotateHandle(AHandles[c]));
  end;
end;

procedure TBaseGroupBlock.ResizeMemberLine(ALine: TCustomDiagramLine;
  AHandles: TStretchHandles; AOld, ANew: TSquare);

  function _ResizeHandle(H: TStretchHandle): TDot;
  var
    P: TDot;
  begin
    P := H.OrPoint;
    FTempDrawer.PushState;

    {Rotate the block back to original position (angle = 0)}
    FTempDrawer.RotationType := rtRotationCenter;
    FTempDrawer.Angle := -Self.Angle;
    {Rotation center must be the central point of the grouping block}
    FTempDrawer.RotationCenter := Dot((AOld.Left + AOld.Right) / 2, (AOld.Top + AOld.Bottom) / 2);
    P := FTempDrawer.RotX(P);

    {Now resize rect in proportion to the resized selection block}
    FTempDrawer.OriginalRect := AOld;
    FTempDrawer.CurRect := ANew;
    P := Dot(FTempDrawer.PXS(P.X), FTempDrawer.PYS(P.Y));

    {And not rotate the block again based on new rect}
    FTempDrawer.RotationType := rtRotationCenter;
    FTempDrawer.Angle := Self.Angle;
    {Rotation center must be the central point of the grouping block}
    FTempDrawer.RotationCenter := Dot((ANew.Left + ANew.Right) / 2, (ANew.Top + ANew.Bottom) / 2);
    P := FTempDrawer.RotX(P);

    FTempDrawer.PopState;

    result := SubPointX(P, H.OrPoint);
  end;

var
  c: integer;
begin
  if ALine.LineStyle in [lsSideLine, lsBezier, lsCircularArc, lsParabolicArc] then
  begin
    {Rotate initial and final handles, if not attached}
    ALine.FRecalcIntermediateArcHandles := false; // do not recalculate intermediate arc handles when resizing the group
    MoveLineHandle(ALine, AHandles, AHandles[0], _ResizeHandle(AHandles[0]));
    MoveLineHandle(ALine, AHandles, AHandles[AHandles.Count - 1], _ResizeHandle(AHandles[AHandles.Count - 1]));
    if ALine.LineStyle in [lsCircularArc, lsParabolicArc] then
      MoveLineHandle(ALine, AHandles, AHandles[1], _ResizeHandle(AHandles[1]));
    ALine.FRecalcIntermediateArcHandles := true;
  end
  else
  begin
    {Rotate all handles}
    for c := 0 to ALine.Handles.Count - 1 do
      MoveLineHandle(ALine, AHandles, AHandles[c], _ResizeHandle(AHandles[c]));
  end;
end;

function TBaseGroupBlock.RotateMemberRect(ARect: TSquare;
  AAngle: double; AGroupRect: TSquare): TSquare;
begin
  {Rotate rect by center}
  FTempDrawer.PushState;

  FTempDrawer.RotationType := rtRotationCenter;

  FTempDrawer.RotationCenter :=
    //RoundPoint(TranslateRotCenter(AGroupRect, AAngle, ARotCenter));
    Dot((AGroupRect.Left + AGroupRect.Right) / 2, (AGroupRect.Top + AGroupRect.Bottom) / 2);

  FTempDrawer.Angle := AAngle;
  result := FTempDrawer.RotByCenter(ARect);
  FTempDrawer.PopState;
end;

function TBaseGroupBlock.SurroundRgn: TDotArray;
begin
  {make the block not selectable}
  SetLength(result, 0);
end;

procedure TBaseGroupBlock.RotateMembers(ADeltaAngle: double; ATemporary: boolean);
var
  c: integer;
  ANewAngle: double;
  AOldRect: TSquare;
  ABlock: TCustomDiagramBlock;
begin
  UpdateGroupMembers;

  if not ATemporary then
    for c := 0 to FMembers.Count - 1 do
      if FMembers[c].DControl <> nil then
        FMembers[c].DControl.BeginNotifying;

  for c := 0 to FMembers.Count - 1 do if FMembers[c].DControl <> nil then
    if FMembers[c].DControl is TCustomDiagramBlock then
    begin
      ABlock := TCustomDiagramBlock(FMembers[c].DControl);

      if CanRotate then
        ANewAngle := ABlock.Angle + ADeltaAngle
      else
        ANewAngle := ABlock.Angle; //No change

      if ATemporary then
      begin
        AOldRect := ABlock.FCursorRect;
        ABlock.FCursorRect := RotateMemberRect(ABlock.FCursorRect, ADeltaAngle, Self.FCursorRect);
        {propagate changes because when temporary, changes are not propagated automatically by AngleChangedEx/BoundsChanged methods.
         In other words, simulate BoundsChanged}
        if ABlock.IsGroup then
          TGroupBlock(ABlock).ResizeMembers(AOldRect, ABlock.FCursorRect, true);

        ABlock.FCursorAngle := ANewAngle;
        {Simulate AngleChanged}
        if ABlock.IsGroup then
          TGroupBlock(ABlock).RotateMembers(ADeltaAngle, true);


        {SPECIFIC CODE FOR POLYGON BLOCK. Copied from TPolygonBlock.HandleMoving}
        if ABlock is TPolygonBlock then
        begin
          with TPolygonBlock(ABlock) do
          begin
            FCursorPolygon.Assign(FPolygon);
            RepositionPolygon(FCursorPolygon, FCursorRect);
            FCursorRect := BuildOriginalRect(FCursorPolygon);
          end;
        end;
      end else
      begin
        ABlock.BoundsRect := RotateMemberRect(ABlock.BoundsRect, ADeltaAngle, Self.BoundsRect);
        ABlock.Angle := ANewAngle;
      end;
    end else
    if FMembers[c].DControl is TCustomDiagramLine then
    begin
      if ATemporary then
        RotateMemberLine(
          TCustomDiagramLine(FMembers[c].DControl),
          TCustomDiagramLine(FMembers[c].DControl).FTempHandles,
          ADeltaAngle, Self.FCursorRect)
      else
        RotateMemberLine(
          TCustomDiagramLine(FMembers[c].DControl),
          TCustomDiagramLine(FMembers[c].DControl).Handles,
          ADeltaAngle, Self.BoundsRect)
    end;

  if not ATemporary then
    for c := 0 to FMembers.Count - 1 do
      if FMembers[c].DControl <> nil then
        FMembers[c].DControl.EndNotifying;
end;

destructor TBaseGroupBlock.Destroy;
begin
  FMembers.Free;
  inherited;
end;

procedure TBaseGroupBlock.DestroyMembers;
begin
  while Members.Count > 0 do
  begin
    if Members[0].DControl.IsGroup then
      TBaseGroupBlock(Members[0].DControl).DestroyMembers;
    Members[0].DControl.Free;
  end;
end;

procedure TBaseGroupBlock.DiagramChanged(Old, New: TatDiagram);
var
  c: Integer;
begin
  inherited;
  if not (csDestroying in ComponentState) then
  begin
    for c := 0 to Members.Count - 1 do
      if (Members[c].DControl <> nil) and not (csDestroying in Members[c].DControl.ComponentState) then
        Members[c].DControl.Diagram := New;
  end;
end;

procedure TBaseGroupBlock.DrawBlockCursor(AInfo: TDiagramDrawInfo; ABlockInfo: TDiagramDrawBlockInfo);
//var
//  NewRect: TRect;
//  c: integer;
//  ABlock: TCustomDiagramBlock;
begin
  inherited;
//  for c := 0 to Members.Count - 1 do
//    if (Members[c].DControl <> nil) then
//    begin
//      if Members[c].DControl is TCustomDiagramBlock then
//      begin
//        ABlock := TCustomDiagramBlock(Members[c].DControl);
//        NewRect := ResizeMemberRect(ABlock, ABlock.BoundsRect, Self.BoundsRect, ARect);
//        ABlock.DrawBlockCursor(Canvas, NewRect, AAngle);
//      end else
//      if Members[c].DControl is TCustomDiagramLine then
//      begin
//
//      end;
//    end;
end;

procedure TBaseGroupBlock.UpdateGroupMembers;
begin
end;

procedure TBaseGroupBlock.ResizeMembers(AOld, ANew: TSquare;
  ATemporary: boolean);
var
  c: integer;
  AOldRect: TSquare;
  ABlock: TCustomDiagramBlock;
begin
  UpdateGroupMembers;

  if not ATemporary then
    for c := 0 to FMembers.Count - 1 do
      if FMembers[c].DControl <> nil then
        FMembers[c].DControl.BeginNotifying;

  for c := 0 to FMembers.Count - 1 do if FMembers[c].DControl <> nil then
  begin
    if FMembers[c].DControl is TCustomDiagramBlock then
    begin
      ABlock := TCustomDiagramBlock(FMembers[c].DControl);

      if ATemporary then
      begin
        AOldRect := ABlock.FCursorRect;
        ABlock.FCursorRect := ResizeMemberRect(ABlock, ABlock.FCursorRect, AOld, ANew);;
        if ABlock.IsGroup then
          TGroupBlock(ABlock).ResizeMembers(AOldRect, ABlock.FCursorRect, true);

        {SPECIFIC CODE FOR POLYGON BLOCK. Copied from TPolygonBlock.HandleMoving}
        if ABlock is TPolygonBlock then
        begin
          with TPolygonBlock(ABlock) do
          begin
            FCursorPolygon.Assign(FPolygon);
            RepositionPolygon(FCursorPolygon, FCursorRect);
            FCursorRect := BuildOriginalRect(FCursorPolygon);
          end;
        end;
      end
      else
        ABlock.BoundsRect := ResizeMemberRect(ABlock, ABlock.BoundsRect, AOld, ANew);

    end else
    if (FMembers[c].DControl is TCustomDiagramLine) then
    begin
      if ATemporary then
        ResizeMemberLine(
          TCustomDiagramLine(FMembers[c].DControl),
          TCustomDiagramLine(FMembers[c].DControl).FTempHandles,
          AOld, ANew)
      else
        ResizeMemberLine(
          TCustomDiagramLine(FMembers[c].DControl),
          TCustomDiagramLine(FMembers[c].DControl).Handles,
          AOld, ANew);
    end;
  end;

  if not ATemporary then
    for c := 0 to FMembers.Count - 1 do
      if FMembers[c].DControl <> nil then
        FMembers[c].DControl.EndNotifying;
end;

procedure TBaseGroupBlock.AngleChangedEx(AOld, ANew: double);
begin
  inherited;
  RotateMembers(ANew - AOld, false);
  NotifyAnchoredMembers;
end;

procedure TBaseGroupBlock.BoundsChanged(AOld, ANew: TSquare);
begin
  inherited;
  ResizeMembers(AOld, ANew, false);
  NotifyAnchoredMembers;
end;

procedure TBaseGroupBlock.NotifyAnchoredMembers;
var
  c: integer;
begin
  for c := 0 to FMembers.Count - 1 do
    if FMembers[c].DControl <> nil then
      FMembers[c].DControl.NotifyAnchoreds(acPosition);
end;

procedure TBaseGroupBlock.RecalculateRect;
var
  ARect: TSquare;
begin
  ARect := CalculateItemsRect(Members, false, Angle);
                                                                 
  {Set block position. Set fields directly because we don't want
   the block to recalculate members position}
  if (FLeft <> ARect.Left) or (FTop <> ARect.Top) or
    (FWidth <> (ARect.Right - ARect.Left)) or (FHeight <> (ARect.Bottom - ARect.Top)) then
  begin
    FLeft := ARect.Left;
    FTop := ARect.Top;
    FWidth := ARect.Right - ARect.Left;
    FHeight := ARect.Bottom - ARect.Top;

    if Self.IsMember then
      Self.GroupBlock.RecalculateRect;
  end;
end;

procedure TBaseGroupBlock.IterateAllMembers(AProc: TGroupMemberProc);

  procedure _IterateGroup(AGroup: TBaseGroupBlock);
  var
    c: integer;
  begin
    AProc(AGroup);
    for c := 0 to AGroup.Members.Count - 1 do
      if AGroup.Members[c].DControl <> nil then
      begin
        if AGroup.Members[c].DControl.IsGroup then
          _IterateGroup(TGroupBlock(AGroup.Members[c].DControl))
        else
          AProc(AGroup.Members[c].DControl);
      end;
  end;

begin
  _IterateGroup(Self);
end;

procedure TBaseGroupBlock.MemberIncluded(AMember: TDiagramControl);
begin
end;

procedure TBaseGroupBlock.MemberRemoved(AMember: TDiagramControl);
begin
end;

{ TGroupBlock }

procedure TGroupBlock.DrawHandles(AInfo: TDiagramDrawInfo);
begin
  inherited;

  {if not selected by has selecteds, then draw a gray dotted line}
  if not Selected and HasSelecteds and (Diagram <> nil) then
  begin
    {Draw the surround rect}
    AInfo.Canvas.Pen.Width := 1;
    AInfo.Canvas.Pen.Style := psDot;
    AInfo.Canvas.Pen.Color := clGray;
    AInfo.Canvas.Brush.Style := bsClear;
    TempDrawer.CurRect := Diagram.ClientToCanvas(BoundsRect);
    TempDrawer.Angle := Angle;
    TempDrawer.RotationType := rtCurRectCenter;
    TempDrawer.Canvas := AInfo.Canvas;
    with Diagram.ClientToCanvas(BoundsRect) do
      TempDrawer.DrawRotRect(Left, Top, Right, Bottom);
  end;
end;

procedure TGroupBlock.MemberIncluded(AMember: TDiagramControl);
begin
  RecalculateRect;
  if AMember <> nil then
    AMember.IncludedInGroup(Self);
end;

procedure TGroupBlock.MemberRemoved(AMember: TDiagramControl);
begin
  if AMember <> nil then
    AMember.RemovedFromGroup(Self);
  if not (csDestroying in ComponentState) then
    RecalculateRect;
end;

{ TDControlList }

function TDControlList.Add(ADControl: TDiagramControl): TDControlListItem;
begin
  result := TDControlListItem(inherited Add);
  result.DControl := ADControl;
end;

function TDControlList.GetItems(Index: Integer): TDControlListItem;
begin
  Result := TDControlListItem(inherited Items[Index]);
end;

function TDControlList.FindByDControl(ADControl: TDiagramControl): TDControlListItem;
var
  c: integer;
begin
  result := nil;
  for c := 0 to Count - 1 do
    if Items[c].DControl = ADControl then
    begin
      result := Items[c];
      break;
    end;
end;

procedure TDControlList.Remove(ADControl: TDiagramControl);
var
  AItem: TDControlListItem;
begin
  AItem := FindByDControl(ADControl);
  if AItem <> nil then
    AItem.Free;
end;

procedure TDControlList.SetItems(Index: Integer; AItem: TDControlListItem);
begin
  Items[Index].Assign(AItem);
end;

procedure TDControlList.MemberIncluded(ADControl: TDiagramControl);
begin
end;

procedure TDControlList.MemberRemoved(ADControl: TDiagramControl);
begin
end;

{ TGroupMemberList }

procedure TGroupMemberList.MemberIncluded(ADControl: TDiagramControl);
begin
  if FGroup <> nil then
    FGroup.MemberIncluded(ADControl);
end;

procedure TGroupMemberList.MemberRemoved(ADControl: TDiagramControl);
begin
  if FGroup <> nil then
    FGroup.MemberRemoved(ADControl);
end;

{ TSelectionBlock }

procedure TSelectionBlock.UpdateGroupMembers;
var
  c: integer;
begin
  inherited;
  FMembers.Clear;
  if Diagram <> nil then
    for c := 0 to Diagram.SelectedCount - 1 do
      if not Diagram.Selecteds[c].AsMember then
        FMembers.Add(Diagram.Selecteds[c]);
end;

{ TDControlListItem }

procedure TDControlListItem.Assign(Source: TPersistent);
begin
  if Source is TDControlListItem then
  begin
    DControl := TDControlListItem(Source).DControl;
  end else
    inherited Assign(Source);
end;

destructor TDControlListItem.Destroy;
begin
  DControl := nil;
  inherited;
end;

procedure TDControlListItem.MemberIncluded(ADControl: TDiagramControl);
begin
  if Collection is TDControlList then
    TDControlList(Collection).MemberIncluded(ADControl);
end;

procedure TDControlListItem.MemberRemoved(ADControl: TDiagramControl);
begin
  if Collection is TDControlList then
    TDControlList(Collection).MemberRemoved(ADControl);
end;

procedure TDControlListItem.SetDControl(const Value: TDiagramControl);
var
  OldDControl: TDiagramControl;
begin
  if FDControl <> Value then
  begin
    OldDControl := FDControl;
    FDControl := Value;
    if OldDControl <> nil then
      MemberRemoved(OldDControl);
    if FDControl <> nil then
      MemberIncluded(FDControl);
  end;
end;

procedure TBaseGroupBlock.SetMembers(const Value: TGroupMemberList);
begin
  FMembers.Assign(Value);
end;

{ TDiagramPageSettings }

procedure TDiagramPageSettings.Assign(Source: TPersistent);
begin
  if Source is TDiagramPageSettings then
  begin
    FPaperName := TDiagramPageSettings(Source).FPaperName;
    FPaperId := TDiagramPageSettings(Source).FPaperId;
    FPaperWidth := TDiagramPageSettings(Source).FPaperWidth;
    FPaperHeight := TDiagramPageSettings(Source).FPaperHeight;
    FOrientation := TDiagramPageSettings(Source).FOrientation;
    FLeftMargin := TDiagramPageSettings(Source).FLeftMargin;
    FTopMargin := TDiagramPageSettings(Source).FTopMargin;
    FRightMargin := TDiagramPageSettings(Source).FRightMargin;
    FBottomMargin := TDiagramPageSettings(Source).FBottomMargin;
    Changed;
  end else
    inherited Assign(Source);
end;

procedure TDiagramPageSettings.Changed;
begin
  {if Assigned(FDiagram) then
    FDiagram.Redraw;}
end;

procedure TDiagramPageSettings.ChangePaperID(APaperID: integer);
var
  APaper: TDiagramPrinterPaperInfo;
begin
  APaper := FDiagram.DPrinter.FindPaper(APaperID);
  FPaperID := APaperID;
  if APaper <> nil then
  begin
    FPaperWidth := APaper.PaperWidth;
    FPaperHeight := APaper.PaperHeight;
    FPaperName := APaper.PaperName;
  end;
end;

constructor TDiagramPageSettings.Create(ADiagram: TatDiagram);
begin
  FDiagram := ADiagram;
end;

function TDiagramPageSettings.GetBottomMarginStr: string;
begin
  result := _NumToStr(BottomMargin);
end;

function TDiagramPageSettings.GetLeftMarginStr: string;
begin
  result := _NumToStr(LeftMargin);
end;

function TDiagramPageSettings.GetRightMarginStr: string;
begin
  result := _NumToStr(RightMargin);
end;

function TDiagramPageSettings.GetTopMarginStr: string;
begin
  result := _NumToStr(TopMargin);
end;

function TDiagramPageSettings.PrintPageRect: TSquare;
var
  MMRect: TSquare;
  _DPI: TPoint;
  Temp: integer;
  W, H: extended;
begin
  if (FDiagram <> nil) and (FDiagram.DPrinter <> nil) then
    _DPI := FDiagram.DPrinter.DPI
  else
    _DPI := Point(300, 300);

  if Orientation = dpoPortrait then
  begin
    MMRect.Left := Max(LeftMargin, 0);
    MMRect.Top := Max(TopMargin, 0);
    MMRect.Right := Min(PaperWidth - RightMargin, PaperWidth);
    MMRect.Bottom := Min(PaperHeight - BottomMargin, PaperHeight);
  end
  else
  //poLandscape:
  begin
    MMRect.Left := Max(BottomMargin, 0);
    MMRect.Top := Max(LeftMargin, 0);
    MMRect.Right := Min(PaperHeight - TopMargin, PaperHeight);
    MMRect.Bottom := Min(PaperWidth - RightMargin, PaperWidth);

    Temp := _DPI.X;
    _DPI.X := _DPI.Y;
    _DPI.Y := Temp;
  end;

  {Ensure valid MMRect and minimum 10 mm for MMRect size}
  MMRect.Right := Max(MMRect.Right, MMRect.Left);
  MMRect.Bottom := Max(MMRect.Bottom, MMRect.Top);

  W := MMRect.Right - MMRect.Left;
  H := MMRect.Bottom - MMRect.Top;
  if W < 10 then
  begin
    MMRect.Right := Min(PaperWidth, MMRect.Right + (10 - W));
    MMRect.Left := Min(MMRect.Left, MMRect.Right - 10);
  end;
  if H < 10 then
  begin
    MMRect.Bottom := Min(PaperHeight, MMRect.Bottom + (10 - H));
    MMRect.Top := Min(MMRect.Top, MMRect.Bottom - 10);
  end;

  result.Left := MMRect.Left / 25.4 * _DPI.X;
  result.Top := MMRect.Top / 25.4 * _DPI.Y;
  result.Right := MMRect.Right / 25.4 * _DPI.X;
  result.Bottom := MMRect.Bottom / 25.4 * _DPI.Y;
end;

function TDiagramPageSettings.PrintPaperSize: TPoint;
var
  MMSize: TDot;
  _DPI: TPoint;
  Temp: integer;
begin
  if (FDiagram <> nil) and (FDiagram.DPrinter <> nil) then
    _DPI := FDiagram.DPrinter.DPI
  else
    _DPI := Point(300, 300);

  if Orientation = dpoPortrait then
  begin
    MMSize.X := PaperWidth;
    MMSize.Y := PaperHeight;
  end
  else
  //poLandscape:
  begin
    MMSize.X := PaperHeight;
    MMSize.Y := PaperWidth;

    Temp := _DPI.X;
    _DPI.X := _DPI.Y;
    _DPI.Y := Temp;
  end;

  result.X := Trunc(MMSize.X / 25.4 * _DPI.X);
  result.Y := Trunc(MMSize.Y / 25.4 * _DPI.Y);
end;

procedure TDiagramPageSettings.RetrieveDefaultSize;
begin
  FLeftMargin := 25.4;
  FTopMargin := 25.4;
  FBottomMargin := 25.4;
  FRightMargin := 25.4;
  FPaperName := 'Letter';
  FPaperID := DMPAPER_LETTER;
  FPaperWidth := 215.9;
  FPaperHeight := 279.4;
  FOrientation := dpoPortrait;

  if (FDiagram <> nil) and (FDiagram.DPrinter <> nil) and
    (FDiagram.DPrinter.DefPaper.PaperID > 0) and
    (FDiagram.DPrinter.DefPaper.PaperWidth > 0) and
    (FDiagram.DPrinter.DefPaper.PaperHeight > 0) then
  begin
    FPaperID := FDiagram.DPrinter.DefPaper.PaperID;
    FPaperWidth := FDiagram.DPrinter.DefPaper.PaperWidth;
    FPaperHeight := FDiagram.DPrinter.DefPaper.PaperHeight;
    FPaperName := FDiagram.DPrinter.DefPaper.PaperName;
    FOrientation := FDiagram.DPrinter.DefOrientation;

    {If paper dimension is too short, reduce margins.}
    if FPaperWidth < 100 then
    begin
      FLeftMargin := Max(FPaperWidth * 0.254, FDiagram.DPrinter.FLeftMargin);
      FRightMargin := FLeftMargin;
    end;
    if FPaperHeight < 100 then
    begin
      FTopMargin := Max(FPaperHeight * 0.254, FDiagram.DPrinter.FTopMargin);
      FBottomMargin := FTopMargin;
    end;
  end;
end;

procedure TDiagramPageSettings.SetBottomMargin(const Value: extended);
begin
  if FBottomMargin <> Value then
  begin
    FBottomMargin := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetBottomMarginStr(const Value: string);
begin
  FBottomMargin := _StrToNum(Value);
end;

procedure TDiagramPageSettings.SetLeftMargin(const Value: extended);
begin
  if FLeftMargin <> Value then
  begin
    FLeftMargin := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetLeftMarginStr(const Value: string);
begin
  FLeftMargin := _StrToNum(Value);
end;

procedure TDiagramPageSettings.SetOrientation(
  const Value: TDiagramPageOrientation);
begin
  if FOrientation <> Value then
  begin
    FOrientation := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetPaperHeight(const Value: extended);
begin
  if FPaperHeight <> Value then
  begin
    FPaperHeight := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetPaperId(const Value: integer);
begin
  if FPaperId <> Value then
  begin
    FPaperId := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetPaperName(const Value: string);
begin
  if FPaperName <> Value then
  begin
    FPaperName := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetPaperWidth(const Value: extended);
begin
  if FPaperWidth <> Value then
  begin
    FPaperWidth := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetRightMargin(const Value: extended);
begin
  if FRightMargin <> Value then
  begin
    FRightMargin := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetRightMarginStr(const Value: string);
begin
  FRightMargin := _StrToNum(Value);
end;

procedure TDiagramPageSettings.SetTopMargin(const Value: extended);
begin
  if FTopMargin <> Value then
  begin
    FTopMargin := Value;
    Changed;
  end;
end;

procedure TDiagramPageSettings.SetTopMarginStr(const Value: string);
begin
  FTopMargin := _StrToNum(Value);
end;

function TDiagramPageSettings._NumToStr(N: extended): string;
var
  C: Char;
begin
  C := {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator;
  {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator := '.';
  try
    result := FloatToStr(N);
  finally
    {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator := C;
  end;
end;

function TDiagramPageSettings._StrToNum(S: string): extended;
var
  C: Char;
begin
  C := {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator;
  {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator := '.';
  try
    result := StrToFloat(S);
  finally
    {$IFDEF DELPHI2011_LVL}FormatSettings.{$ENDIF}DecimalSeparator := C;
  end;
end;

{ TDiagramPrinters }

procedure TDiagramPrinters.Clear;
begin
  while FPrinters.Count > 0 do
  begin
    FPrinters.Objects[0].Free;
    FPrinters.Delete(0);
  end;
end;

constructor TDiagramPrinters.Create;
begin
  inherited;
  FPrinters := TStringList.Create;
  UpdatePrinters;

  if FPrinters.Count = 0 then
  begin
    FPrinters.AddObject('Generic printer',
      TDiagramGenericPrinter.Create('Generic printer', ''));
    FHasPrinterInstalled := false;
  end else
    FHasPrinterInstalled := true;

  PrinterIndex := -1; //Set to default printer 
end;

destructor TDiagramPrinters.Destroy;
begin
  Clear;
  FPrinters.Free;
  inherited;
end;

function TDiagramPrinters.GetItem(Index: integer): TDiagramPrinter;
begin
  result := TDiagramPrinter(FPrinters.Objects[Index]);
end;

function TDiagramPrinters.GetPrinter: TDiagramPrinter;
begin
  if (PrinterIndex >= 0) and (PrinterIndex < FPrinters.Count) then
    result := TDiagramPrinter(FPrinters.Objects[FPrinterIndex])
  else
    result := nil;
end;

function TDiagramPrinters.GetPrinterIndex: integer;
begin
  result := FPrinterIndex;
end;

function TDiagramPrinters.IndexOf(DeviceName: string): integer;
var
  c: integer;
begin
  result := -1;
  for c := 0 to FPrinters.Count - 1 do
  begin
    if SameText(DeviceName, Items[c].DeviceName) then
    begin
      result := c;
      break;
    end;
  end;
end;

procedure TDiagramPrinters.SetPrinterIndex(const Value: integer);
begin
  if Value >= 0 then
    FPrinterIndex := Value
  else
  begin
    FPrinterIndex := Indexof(DefaultPrinterName);
    if (FPrinterIndex = -1) then
      if (FPrinters.Count > 0) then
        FPrinterIndex := 0;
  end;

  if Printer <> nil then
    Printer.Initialize;
end;

function TDiagramPrinters.DefaultPrinterName: string;
var
  DefPrinterName: array[0..79] of Char;
begin
  GetProfileString('windows', 'device', '', DefPrinterName,  SizeOf(DefPrinterName));
  result := Copy(DefPrinterName, 1, Pos(',', DefPrinterName) - 1);
end;

procedure TDiagramPrinters.UpdatePrinters;

  function FetchStr(var Str: PChar): PChar;
  var
    P: PChar;
  begin
    Result := Str;
    if Str = nil then Exit;
    P := Str;
    while P^ = ' ' do Inc(P);
    Result := P;
    while (P^ <> #0) and (P^ <> ',') do Inc(P);
    if P^ = ',' then
    begin
      P^ := #0;
      Inc(P);
    end;
    Str := P;
  end;

var
  LineCur, Port: PChar;
  Buffer, PrinterInfo: PByte;
  Flags, Count, NumInfo: DWORD;
  I: Integer;
  Level: Byte;
begin
  Clear;
  if Win32Platform = VER_PLATFORM_WIN32_NT then
  begin
    Flags := PRINTER_ENUM_CONNECTIONS or PRINTER_ENUM_LOCAL;
    Level := 4;
  end
  else
  begin
    Flags := PRINTER_ENUM_LOCAL;
    Level := 5;
  end;
  Count := 0;
  EnumPrinters(Flags, nil, Level, nil, 0, Count, NumInfo);
  if Count = 0 then Exit;

  GetMem(Buffer, Count);
  try
    if not EnumPrinters(Flags, nil, Level, PByte(Buffer), Count, Count, NumInfo) then
      Exit;
    PrinterInfo := Buffer;
    for i := 0 to NumInfo - 1 do
    begin
      if Level = 4 then
        with PPrinterInfo4(PrinterInfo)^ do
        begin
          FPrinters.AddObject(pPrinterName,
            TDiagramRealPrinter.Create(pPrinterName, ''));
          Inc(PrinterInfo, sizeof(TPrinterInfo4));
        end
      else
        with PPrinterInfo5(PrinterInfo)^ do
        begin
          LineCur := pPortName;
          Port := FetchStr(LineCur);
          while Port^ <> #0 do
          begin
            FPrinters.AddObject(pPrinterName,
              TDiagramRealPrinter.Create(pPrinterName, Port));
            Port := FetchStr(LineCur);
          end;
          Inc(PrinterInfo, sizeof(TPrinterInfo5));
        end;
    end;
  finally
    FreeMem(Buffer, Count);
  end;
end;

{ TDiagramPrinter }

procedure TDiagramPrinter.AddDefaultBin;
begin
  FBins.AddObject('Default tray', TObject(DMBIN_AUTO));
end;

procedure TDiagramPrinter.BeginDoc;
begin
end;

procedure TDiagramPrinter.BeginPage;
begin
end;

procedure TDiagramPrinter.CheckInitialized;
begin
  if not FInitialized then
    raise EDiagramPrintException.Create('Printer not initialized');
end;

constructor TDiagramPrinter.Create(const ADeviceName, APort: PChar);
begin
  FDeviceName := ADeviceName;
  FPort := APort;
  FCanvas := TDiagramPrinterCanvas.Create;
  FCanvas.FPrinter := Self;
  FPapers := TObjectList.Create;
  FBins := TStringList.Create;
  //FPaper := TDiagramPrinterPaperInfo.Create;

  FDefPaper := TDiagramPrinterPaperInfo.Create;

  InitializeDefault;
end;

destructor TDiagramPrinter.Destroy;
begin
  FCanvas.Free;
  FPapers.Free;
  FBins.Free;
  //FPaper.Free;
  FDefPaper.Free;
  inherited;
end;

procedure TDiagramPrinter.EndDoc;
begin
end;

procedure TDiagramPrinter.EndPage;
begin
end;

function TDiagramPrinter.FindPaper(
  APaperID: integer): TDiagramPrinterPaperInfo;
var
  c: integer;
begin
  result := nil;
  for c := 0 to PaperCount - 1 do
    if Papers[c].PaperID = APaperID then
    begin
      result := Papers[c];
      break;
    end;
end;

function TDiagramPrinter.GetBinCount: integer;
begin
  result := FBins.Count;
end;

function TDiagramPrinter.GetBinID(Index: integer): integer;
begin
  result := integer(FBins.Objects[Index]);
end;

function TDiagramPrinter.GetBinName(Index: integer): string;
begin
  result := FBins[Index];
end;

function TDiagramPrinter.GetPaper(Index: integer): TDiagramPrinterPaperInfo;
begin
  result := TDiagramPrinterPaperInfo(FPapers[Index]);
end;

function TDiagramPrinter.GetPaperCount: integer;
begin
  result := FPapers.Count;
end;

procedure TDiagramPrinter.Initialize;
begin
end;

procedure TDiagramPrinter.InitializeDefault;
begin
  FDPI := Point(300, 300);

  FDefPaper.PaperHeight := 297;
  FDefPaper.PaperWidth := 210;
  FDefOrientation := dpoPortrait;
  FOrientation := FDefOrientation;

  UpdateStandardPapers;
  AddDefaultBin;

  FBin := -1;

  FDefPaper.PaperID := DMPAPER_A4;
  if FindPaper(FDefPaper.PaperID) <> nil then
    FDefPaper.PaperName := FindPaper(FDefPaper.PaperID).PaperName;
  //FPaper.Assign(FDefPaper);
end;

function TDiagramPrinter.MMToPixelsX(AMM: extended): integer;
begin
  result := trunc(AMM / 25.4 * DPI.X);
end;

function TDiagramPrinter.MMToPixelsY(AMM: extended): integer;
begin
  result := trunc(AMM / 25.4 * DPI.Y);
end;

procedure TDiagramPrinter.PropertiesDlg;
begin
end;


procedure TDiagramPrinter.SetPrintParams(APaperSize: Integer; APaperWidth,
  APaperHeight: Extended; AOrientation: TDiagramPageOrientation);
begin

end;

procedure TDiagramPrinter.UpdateStandardPapers;
var
  i: integer;
  PaperInfo: TDiagramPrinterPaperInfo;
begin
  FPapers.Clear;
  for i := Low(PaperInfoArray) to High(PaperInfoArray) do
  begin
    PaperInfo := TDiagramPrinterPaperInfo.Create;
    PaperInfo.PaperName := PaperInfoArray[i].Name;
    PaperInfo.PaperID := PaperInfoArray[i].PID;
    PaperInfo.PaperWidth := PaperInfoArray[i].W / 10;
    PaperInfo.PaperHeight := PaperInfoArray[i].H / 10;
    FPapers.Add(PaperInfo);
  end;
end;

{ TDiagramPrinterCanvas }

procedure TDiagramPrinterCanvas.Changing;
begin
  inherited;
  UpdateFont;
end;

procedure TDiagramPrinterCanvas.UpdateFont;
var
  FontSize: Integer;
begin
  if (FPrinter <> nil) and (FPrinter.DPI.Y <> Font.PixelsPerInch) then
  begin
    FontSize := Font.Size;
    Font.PixelsPerInch := FPrinter.DPI.Y;
    Font.Size := FontSize;
  end;
end;


{ TDiagramPrinterPaperInfo }

procedure TDiagramPrinterPaperInfo.Assign(Source: TPersistent);
begin
  if Source is TDiagramPrinterPaperInfo then
  begin
    FPaperHeight := TDiagramPrinterPaperInfo(Source).FPaperHeight;
    FPaperID := TDiagramPrinterPaperInfo(Source).FPaperID;
    FPaperWidth := TDiagramPrinterPaperInfo(Source).FPaperWidth;
    FPaperName := TDiagramPrinterPaperInfo(Source).FPaperName;
  end else
    inherited Assign(Source);
end;

{ TDiagramRealPrinter }

procedure TDiagramRealPrinter.BeginDoc;
var
  DocInfo: TDocInfo;
begin
  CheckInitialized;
  FPrinting := True;
  RecreateDC;

  FillChar(DocInfo, SizeOf(DocInfo), 0);
  DocInfo.cbSize := SizeOf(DocInfo);
  DocInfo.lpszDocName := PChar(Title);
  StartDoc(FDC, DocInfo);
end;

procedure TDiagramRealPrinter.BeginPage;
begin
  CheckInitialized;
  StartPage(FDC);
end;

procedure TDiagramRealPrinter.CreateDeviceMode;
var
  ASize: Integer;
  {$IFNDEF DELPHI2009_LVL}
  StubDevMode: TDeviceMode;
  {$ENDIF}
begin
  if OpenPrinter(PChar(FDeviceName), FPrinterHandle, nil) then
  begin
    {$IFDEF DELPHI2009_LVL}
    ASize := DocumentProperties(0, FPrinterHandle, PChar(FDeviceName), nil, nil, 0);
    {$ELSE} 
    ASize := DocumentProperties(0, FPrinterHandle, PChar(FDeviceName), StubDevMode, StubDevMode, 0);
    {$ENDIF}
    if ASize > 0 then
    begin
      FDeviceMode := GlobalAlloc(GHND, ASize);
      if FDeviceMode <> 0 then
      begin
        FDevMode := GlobalLock(FDeviceMode);
        if DocumentProperties(0, FPrinterHandle, PChar(FDeviceName), FDevMode^, FDevMode^, DM_OUT_BUFFER) < 0 then
        begin
          GlobalUnlock(FDeviceMode);
          GlobalFree(FDeviceMode);
          FDeviceMode := 0;
          FDevMode := nil;
        end
      end;
    end;
  end;
end;

destructor TDiagramRealPrinter.Destroy;
begin
  if FDeviceMode <> 0 then
  begin
    GlobalUnlock(FDeviceMode);
    GlobalFree(FDeviceMode);
  end;
  FDeviceMode := 0;

  if FPrinterHandle <> 0 then
  begin
    ClosePrinter(FPrinterHandle);
    FPrinterHandle := 0;
  end;

  {if FCanvas <> nil then
    FCanvas.Handle := 0;}

  {if FDC <> 0 then
    DeleteDC(FDC);
  FDC := 0;}
  inherited;
end;

procedure TDiagramRealPrinter.EndDoc;
begin
  CheckInitialized;
  try
    Windows.EndDoc(FDC);
  except
  end;

  FPrinting := False;
  RecreateDC;
  FBin := -1;
end;

procedure TDiagramRealPrinter.EndPage;
begin
  CheckInitialized;
  Windows.EndPage(FDC);
end;

procedure TDiagramRealPrinter.Initialize;

  procedure UpdatePapers;
  type
    TPaperName = array[0..63] of char;
  var
    i: integer;
    NumPaperSizes: integer;
    Papers: array[0..255] of Word;
    PaperNames: array of TPaperName;
    PaperSizes: array of TPoint;
    PaperInfo: TDiagramPrinterPaperInfo;
  begin
    FPapers.Clear;
    FillChar(Papers, SizeOf(Papers), 0);
    NumPaperSizes := DeviceCapabilities(PChar(FDeviceName), PChar(FPort), DC_PAPERS, @Papers, FDevMode);
    SetLength(PaperNames, NumPaperSizes);
    SetLength(PaperSizes, NumPaperSizes);
    DeviceCapabilities(PChar(FDeviceName), PChar(FPort), DC_PAPERNAMES, PChar(PaperNames), FDevMode);
    DeviceCapabilities(PChar(FDeviceName), PChar(FPort), DC_PAPERSIZE, PChar(PaperSizes), FDevMode);
    for i := 0 to NumPaperSizes - 1 do
      if Papers[i] <> 256 then
      begin
        PaperInfo := TDiagramPrinterPaperInfo.Create;
        PaperInfo.PaperName := PaperNames[i];
        PaperInfo.PaperID := Papers[i];
        PaperInfo.PaperWidth := PaperSizes[i].X / 10;
        PaperInfo.PaperHeight := PaperSizes[i].Y / 10;
        FPapers.Add(PaperInfo);
      end;
  end;

  procedure UpdateBins;
  type
    TBinName = array[0..23] of char;
  var
    i: integer;
    NumBins: integer;
    Bins: array[0..255] of Word;
    BinNames: array of TBinName;
  begin
    FBins.Clear;
    FillChar(Bins, SizeOf(Bins), 0);
    NumBins := DeviceCapabilities(PChar(FDeviceName), PChar(FPort), DC_BINS, @Bins, FDevMode);
    SetLength(BinNames, NumBins);
    DeviceCapabilities(PChar(FDeviceName), PChar(FPort), DC_BINNAMES, PChar(BinNames), FDevMode);
    for i := 0 to NumBins - 1 do
      if Bins[i] <> DMBIN_AUTO then
        FBins.AddObject(BinNames[i], TObject(Bins[i]));
  end;

begin
  if FInitialized then Exit;

  CreateDeviceMode;
  if FDeviceMode = 0 then Exit;
  RecreateDC;
  UpdateDeviceCaps;

  if FDevMode.dmOrientation = DMORIENT_PORTRAIT then
    FDefOrientation := dpoPortrait
  else
    FDefOrientation := dpoLandscape;
  FOrientation := FDefOrientation;

  UpdatePapers;

  UpdateBins;

  FBin := -1;

  FDefPaper.PaperID := FDevMode.dmPaperSize;
  if FindPaper(FDefPaper.PaperID) <> nil then
    FDefPaper.PaperName := FindPaper(FDefPaper.PaperID).PaperName;
  //FPaper.Assign(FDefPaper);

  FInitialized := True;
end;

procedure TDiagramRealPrinter.PropertiesDlg;
var
  h: THandle;
begin
  CheckInitialized;
  if Screen.ActiveForm <> nil then
    h := Screen.ActiveForm.Handle
  else
    h := 0;
  if DocumentProperties(h, FPrinterHandle, PChar(FDeviceName), FDevMode^,
    FDevMode^, DM_IN_BUFFER or DM_OUT_BUFFER or DM_IN_PROMPT) > 0 then
  begin
    FBin := FDevMode.dmDefaultSource;
    RecreateDC;
  end;
end;

procedure TDiagramRealPrinter.RecreateDC;
begin
  if FDC <> 0 then
    try
      DeleteDC(FDC);
    except
    end;

  if FPrinting then
    FDC := CreateDC(nil, PChar(FDeviceName), nil, FDevMode)
  else
    FDC := CreateIC(nil, PChar(FDeviceName), nil, FDevMode);
  FCanvas.Handle := FDC;
  FCanvas.Refresh;
  FCanvas.UpdateFont;
end;

procedure TDiagramRealPrinter.SetPrintParams(APaperSize: Integer;
  APaperWidth, APaperHeight: Extended; AOrientation: TDiagramPageOrientation);
begin
  CheckInitialized;
  FDevMode.dmFields := FDevMode.dmFields or DM_PAPERSIZE or DM_ORIENTATION;

  if APaperSize = 256 then
  begin
    FDevMode.dmFields := FDevMode.dmFields or DM_PAPERLENGTH or DM_PAPERWIDTH;
    FDevMode.dmPaperLength := Round(APaperHeight * 10);
    FDevMode.dmPaperWidth := Round(APaperWidth * 10);
  end
  else
  begin
    FDevMode.dmPaperLength := 0;
    FDevMode.dmPaperWidth := 0;
  end;

  FDevMode.dmPaperSize := APaperSize;

  if AOrientation = dpoPortrait then
    FDevMode.dmOrientation := DMORIENT_PORTRAIT
  else
    FDevMode.dmOrientation := DMORIENT_LANDSCAPE;

  FDC := ResetDC(FDC, FDevMode^);
  FDC := ResetDC(FDC, FDevMode^);
  FCanvas.Refresh;
  UpdateDeviceCaps;
  FOrientation := AOrientation;
end;

procedure TDiagramRealPrinter.UpdateDeviceCaps;
begin
  FDPI := Point(GetDeviceCaps(FDC, LOGPIXELSX), GetDeviceCaps(FDC, LOGPIXELSY));
  if (FDPI.X = 0) or (FDPI.Y = 0) then
    Exit;
  FDefPaper.PaperHeight := GetDeviceCaps(FDC, PHYSICALHEIGHT) / FDPI.Y * 25.4;
  FDefPaper.PaperWidth := GetDeviceCaps(FDC, PHYSICALWIDTH) / FDPI.X * 25.4;
  FLeftMargin := GetDeviceCaps(FDC, PHYSICALOFFSETX) / FDPI.X * 25.4;
  FTopMargin := GetDeviceCaps(FDC, PHYSICALOFFSETY) / FDPI.Y * 25.4;
  FRightMargin := FDefPaper.PaperWidth - GetDeviceCaps(FDC, HORZRES) / FDPI.X * 25.4 - FLeftMargin;
  FBottomMargin := FDefPaper.PaperHeight - GetDeviceCaps(FDC, VERTRES) / FDPI.Y * 25.4 - FTopMargin;
end;

{ TDiagramGenericPrinter }

procedure TDiagramGenericPrinter.Initialize;
begin
  FInitialized := True;
end;

{ TDiagramPrintSettings }

constructor TDiagramPrintSettings.Create;
begin
  FCopies := 1;
  FTitle := '';
  FPageNumbers := '';  
end;

{ TDgrLibraryItem }

procedure TDgrLibraryItem.Assign(Source: TPersistent);
begin
  if Source is TDgrLibraryItem  then
  begin
    FItemName := TDgrLibraryItem(Source).FItemName;
    FCategory := TDgrLibraryItem(Source).FCategory;
    FCaption := TDgrLibraryItem(Source).FCaption;
    FGlyph.Assign(TDgrLibraryItem(Source).FGlyph);
    FData := TDgrLibraryItem(Source).FData;
  end;
end;

constructor TDgrLibraryItem.Create(AOwner: TComponent);
begin
  inherited;
  FGlyph := TPicture.Create;
end;

function TDgrLibraryItem.CreateControl(ADiagram: TatDiagram): TDiagramControl;
begin
  result := ADiagram.CreateLibraryControl(Data, RealControlID);
  result.Diagram := ADiagram;
end;

destructor TDgrLibraryItem.Destroy;
begin
  if FLibrary <> nil then
    FLibrary.RemoveItem(Self);
  FGlyph.Free;
  inherited;
end;

procedure TDgrLibraryItem.Loaded;
begin
  inherited;
  RegisterItem;
end;

function TDgrLibraryItem.RealControlID: string;
begin
  result := 'LIB_' + ItemName;
end;

procedure TDgrLibraryItem.RegisterItem;
var
  ARegDControl: TRegDControl;
begin
  if ItemName = '' then Exit;

  ARegDControl := RegDControlList.FindByID(RealControlID);
  if ARegDControl = nil then
    ARegDControl := RegDControlList.Add;
  ARegDControl.DControlID := RealControlID;
  ARegDControl.DControlClass := TDiagramBlock;

  if Self.Caption = '' then
    ARegDControl.Caption := Self.ItemName
  else
    ARegDControl.Caption := Self.Caption;

  ARegDControl.Category := Self.Category;
  ARegDControl.Data := Self.Data;
  ARegDControl.Glyph.Assign(Self.Glyph);
  ARegDControl.UseGlyph := (Glyph.Bitmap.Width > 0) or not Glyph.Graphic.Empty;
end;

procedure TDgrLibraryItem.Save;
begin
  if (FLibrary <> nil) and (FLibrary.FManager <> nil) then
    FLibrary.FManager.SaveItem(Self);
end;

procedure TDgrLibraryItem.SetGlyph(const Value: TPicture);
begin
  FGlyph.Assign(Value);
end;

procedure TDgrLibraryItem.UnregisterItem;
var
  ARegDControl: TRegDControl;
begin
  ARegDControl := RegDControlList.FindByID(RealControlID);
  if ARegDControl <> nil then
    ARegDControl.Free;
end;

{ TDgrLibrary }

procedure TDgrLibrary.AddItem(ALibItem: TDgrLibraryItem);
begin
  FLibItems.Add(ALibItem);
  ALibItem.FLibrary := Self;
end;

procedure TDgrLibrary.Clear;
begin
  while FLibItems.Count > 0 do
    FLibItems[0].Free;
end;

function TDgrLibrary.Count: integer;
begin
  result := FLibItems.Count;
end;

constructor TDgrLibrary.Create(AOwner: TComponent);
begin
  inherited;
  FLibItems := TObjectList.Create(false);
end;

procedure TDgrLibrary.DeleteItem(ALibItem: TDgrLibraryItem);
begin
  if FManager <> nil then
  begin
    FManager.CheckFiler;
    FManager.FFiler.DeleteItem(ALibItem);
  end;
end;

destructor TDgrLibrary.Destroy;  
begin
  Clear;
  FLibItems.Free;
  if FManager <> nil then
    FManager.FLibs.Remove(Self);
  inherited;
end;

function TDgrLibrary.FindItem(ACategory, AItemName: string): TDgrLibraryItem;
var
  i: integer;
begin
  for i := 0 to Count - 1 do
    if SameText(ACategory, Items[i].Category) and SameText(AItemName, Items[i].ItemName) then
    begin
      result := Items[i];
      exit;
    end;
  result := nil;
end;

function TDgrLibrary.GetItem(Index: integer): TDgrLibraryItem;
begin
  result := TDgrLibraryItem(FLibItems[Index]);
end;

function TDgrLibrary.NewItem: TDgrLibraryItem;
begin
  result := TDgrLibraryItem.Create(nil);
  AddItem(result);
end;

procedure TDgrLibrary.RegisterLibrary;
var
  c: Integer;
begin
  for c := 0 to Count - 1 do
    Items[c].RegisterItem;
end;

procedure TDgrLibrary.RemoveItem(ALibItem: TDgrLibraryItem);
begin
  FLibItems.Remove(ALibItem);
end;

//procedure TDgrLibrary.UnregisterLibrary;
//var
//  c: Integer;
//begin
//  for c := 0 to Count - 1 do
//    Items[c].UnregisterItem;
//end;

{ TDgrLibraryFolderFiler }

function TDgrLibraryFolderFiler.BuildLibraryFileName(
  ALibName: string): string;
begin
  result := FLibPrefix + ALibName;
end;

procedure TDgrLibraryFolderFiler.CheckBaseFolder;
begin
  if FBaseFolder = '' then
    raise EDiagramException.Create('Base folder not specified in library filer component.');
end;

constructor TDgrLibraryFolderFiler.Create(AOwner: TComponent);
begin
  inherited;
  FDefaultExt := '.dgo'; //diagram object
  FLibPrefix := 'dlib_';
end;

{$IFNDEF DELPHI6_LVL}
function DirectoryExists(const Directory: string): Boolean;
var
  Code: Integer;
begin
  Code := GetFileAttributes(PChar(Directory));
  Result := (Code <> -1) and (FILE_ATTRIBUTE_DIRECTORY and Code <> 0);
end;

function ForceDirectories(Dir: string): Boolean;
begin
  if Length(Dir) = 0 then
    raise Exception.CreateRes(@SCannotCreateDir);
  Dir := ExcludeTrailingBackslash(Dir);
  if (Length(Dir) < 3) or DirectoryExists(Dir)
    or (ExtractFilePath(Dir) = Dir) then
  begin
    Result := True;
    Exit; 
  end;
  Result := ForceDirectories(ExtractFilePath(Dir)) and CreateDir(Dir);
end;
{$ENDIF}

procedure TDgrLibraryFolderFiler.CreateLibrary(ALibrary: TDgrLibrary);
begin
  CheckBaseFolder;
  ForceDirectories(Format('%s\%s', [RealBaseFolder, BuildLibraryFileName(ALibrary.LibraryName)]));
end;

procedure TDgrLibraryFolderFiler.DeleteItem(AItem: TDgrLibraryItem);
var
  AFile: string;
begin
  CheckBaseFolder;
  AFile := Format('%s\%s\%s%s', [RealBaseFolder, BuildLibraryFileName(AItem.OwnerLibrary.LibraryName),
    AItem.ItemName, DefaultExt]);
  DeleteFile(AFile);
end;

procedure TDgrLibraryFolderFiler.DeleteLibrary(ALibrary: TDgrLibrary);
var
  AFolder: string;
  c: Integer;
begin
  CheckBaseFolder;
  for c := 0 to ALibrary.Count - 1 do
    DeleteItem(ALibrary.Items[c]);
  AFolder := Format('%s\%s', [RealBaseFolder, BuildLibraryFileName(ALibrary.LibraryName)]);
  RemoveDir(AFolder);
end;

function TDgrLibraryFolderFiler.ExtractLibraryName(
  AFileName: string): string;
begin
  result := Copy(AFileName, Length(FLibPrefix) + 1, MaxInt);
end;

function TDgrLibraryFolderFiler.GetBaseFolder: string;
begin
  result := FBaseFolder;
end;

procedure TDgrLibraryFolderFiler.GetLibraryNames(AStrings: TStrings);
var
  SR: TSearchRec;
  AMask: string;
  Attr: integer;
begin
  AStrings.Clear;
  try
    CheckBaseFolder;
  except
    Exit;
  end;


  AMask := Format('%s\*', [RealBaseFolder]);
  {$WARNINGS OFF}
  Attr := faDirectory;
  {$WARNINGS ON}
  if FindFirst(AMask, Attr, SR) = 0 then
  begin
    if (SR.Name <> '.') and (SR.Name <> '..') and (SR.Attr and faDirectory <> 0) and IsLibraryFileName(SR.Name) then
      AStrings.Add(ExtractLibraryName(SR.Name));
    while FindNext(SR) = 0 do
      if (SR.Name <> '.') and (SR.Name <> '..') and (SR.Attr and faDirectory <> 0) and IsLibraryFileName(SR.Name) then
        AStrings.Add(ExtractLibraryName(SR.Name));
  end;
  FindClose(SR);
end;

function TDgrLibraryFolderFiler.IsLibraryFileName(ALibName: string): boolean;
begin
  result := SameText(Copy(ALibName, 1 , Length(FLibPrefix)), FLibPrefix) and (Length(ALibName) > Length(FLibPrefix)); 
end;

procedure TDgrLibraryFolderFiler.Load(ALibrary: TDgrLibrary);

  procedure _NewItem(AName: string);
  var
    AItem: TDgrLibraryItem;
  begin
    AName := ExtractFileName(ChangeFileExt(AName, ''));
    AItem := ALibrary.NewItem;
    AItem.ItemName := AName;
    LoadItem(AItem);
  end;

var
  SR: TSearchRec;
  AMask: string;
  Attr: integer;
begin
  ALibrary.Clear;
  CheckBaseFolder;
  AMask := Format('%s\%s\*%s', [RealBaseFolder, BuildLibraryFileName(ALibrary.LibraryName), DefaultExt]);
  {$WARNINGS OFF}
  Attr := faReadOnly and faArchive;
  {$WARNINGS ON}
  if FindFirst(AMask, Attr, SR) = 0 then
  begin
    _NewItem(SR.Name);
    while FindNext(SR) = 0 do
      _NewItem(SR.Name);
  end;
  FindClose(SR);
end;

function TDgrLibraryFolderFiler.LoadItem(AItem: TDgrLibraryItem): boolean;
var
  AFileName: string;
  TempItem: TDgrLibraryItem;
begin
  result := false;
  CheckBaseFolder;
  AFileName := Format('%s\%s\%s%s', [RealBaseFolder, BuildLibraryFileName(AItem.OwnerLibrary.LibraryName),
    AItem.ItemName, DefaultExt]);
  if FileExists(AFileName) then
  begin
    TempItem := TDgrLibraryItem.Create(nil);
    try
      try
        ReadComponentResFile(AFileName, TempItem);
      except
        result := false;
      end;
      TempItem.ItemName := AItem.ItemName;
      AItem.Assign(TempItem);
      result := true;
    finally
      TempItem.Free;
    end;
  end;
end;

function TDgrLibraryFolderFiler.RealBaseFolder: string;
begin
  CheckBaseFolder;
  {$IFDEF DELPHI6_LVL}
  result := ExcludeTrailingPathDelimiter(FBaseFolder);
  {$ELSE}
  result := ExcludeTrailingBackslash(FBaseFolder);
  {$ENDIF}
end;

procedure TDgrLibraryFolderFiler.Save(ALibrary: TDgrLibrary);
begin
end;

function TDgrLibraryFolderFiler.SaveItem(AItem: TDgrLibraryItem): boolean;

  function CalculateID(S: string): string;
  var
    c: integer;
  begin
    result := '';
    for c := 1 to Length(S) do
      if IsValidIdent(result + S[c]) then
        result := result + S[c];
  end;

var
  AFileName: string;
begin
  CheckBaseFolder;
  result := true;
  if AItem.ItemName = '' then
  begin
    if AItem.Caption = '' then
      raise EDiagramException.Create('Item caption cannot be empty.');
    AItem.ItemName := CalculateID(AItem.Caption);
  end;
  AFileName := Format('%s\%s\%s%s', [RealBaseFolder, BuildLibraryFileName(AItem.OwnerLibrary.LibraryName),
    AItem.ItemName, DefaultExt]);
  WriteComponentResFile(AFileName, AItem);
end;

procedure TDgrLibraryFolderFiler.SetBaseFolder(const Value: string);
begin
  FBaseFolder := Value;
end;

{ TDgrLibraryManager }

procedure TDgrLibraryManager.AddLibrary(ALib: TDgrLibrary);
begin
  ALib.FManager := Self;
  FLibs.Add(ALib);
end;

procedure TDgrLibraryManager.CheckFiler;
begin
  if FFiler = nil then
    raise EDiagramException.Create('Filer object not specified for diagram library manager.');
end;

procedure TDgrLibraryManager.Clear;
begin
  while FLibs.Count > 0 do
    FLibs[0].Free;
end;

constructor TDgrLibraryManager.Create(AOwner: TComponent);
begin
  inherited;
  FLibs := TObjectList.Create(false);
  FAutoLoad := true;
end;

function TDgrLibraryManager.CreateLibrary(ALibName: string): TDgrLibrary;
begin
  result := LibraryByName(ALibName);
  if result = nil then
  begin
    CheckFiler;
    result := TDgrLibrary.Create(nil);
    try
      result.FLibraryName := ALibName;
      FFiler.CreateLibrary(result);
    except
      result.Free;
      raise;
    end;
    AddLibrary(result);
  end;
end;

procedure TDgrLibraryManager.DeleteLibrary(ALibName: string);
var
  ALib: TDgrLibrary;
begin
  CheckFiler;
  ALib := LibraryByName(ALibName);
  if ALib <> nil then
  begin
    FFiler.DeleteLibrary(ALib);
    ALib.Free;
  end;
end;

destructor TDgrLibraryManager.Destroy;
begin
  Clear;
  FLibs.Free;
  inherited;
end;

function TDgrLibraryManager.GetLibrary(Index: integer): TDgrLibrary;
begin
  result := TDgrLibrary(FLibs[Index]);
end;

procedure TDgrLibraryManager.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (AComponent = FFiler) and (Operation = opRemove) then
    FFiler := nil;
end;

procedure TDgrLibraryManager.RegisterAll;
var
  c: Integer;
begin
  for c := 0 to LibraryCount - 1 do
    Libraries[c].RegisterLibrary;
end;

function TDgrLibraryManager.LibraryByName(AName: string): TDgrLibrary;
var
  c: Integer;
begin
  result := nil;
  for c := 0 to LibraryCount - 1 do
    if SameText(Libraries[c].LibraryName, AName) then
    begin
      result := Libraries[c];
      break;
    end;
end;

function TDgrLibraryManager.LibraryCount: integer;
begin
  result := FLibs.Count;
end;

procedure TDgrLibraryManager.LoadLibraries;
var
  SL: TStrings;
  c: Integer;
  Lib: TDgrLibrary;
begin
  CheckFiler;

  FLibs.Clear;
  SL := TStringList.Create;
  try
    FFiler.GetLibraryNames(SL);

    for c := 0 to SL.Count - 1 do
    begin
      Lib := TDgrLibrary.Create(nil);
      AddLibrary(Lib);
      Lib.FLibraryName := SL[c];
      FFiler.Load(Lib);
    end;
  finally
    SL.Free;
  end;
  RegisterAll;
end;

procedure TDgrLibraryManager.SaveItem(AItem: TDgrLibraryItem);
begin
  CheckFiler;
  FFiler.SaveItem(AItem);
end;

procedure TDgrLibraryManager.SetFiler(const Value: TDgrLibraryFiler);
begin
  if FFiler <> Value then
  begin
    if FFiler <> nil then
      FFiler.RemoveFreeNotification(Self);
    FFiler := Value;
    if FFiler <> nil then
      FFiler.FreeNotification(Self);
  end;
end;

function TDgrLibraryManager.CreateItemDlg(ADControl: TDiagramControl): TDgrLibraryItem;
var
  AItem: TDgrLibraryItem;
  ALibrary: TDgrLibrary;
begin
  result := nil;
  if (ADControl <> nil) and (ADControl.Diagram <> nil) then
  begin
    AItem := TDgrLibraryItem.Create(nil);
    try
      if LibraryCount > 0 then
        ALibrary := Libraries[0]
      else
        ALibrary := CreateLibrary('Default');
      AItem.Data := ADControl.Diagram.SerializeObjectToString(ADControl);

      if TfmLibItemEditor.EditLibraryItem(AItem, Self, ALibrary) then
      begin
        ALibrary.AddItem(AItem);
        AItem.Save;
        result := AItem;
        result.RegisterItem;
      end
      else
        AItem.Free;
    except
      AItem.Free;
      raise;
    end;
  end;
end;

procedure TDgrLibraryManager.GetLibraryNames(AStrings: TStrings);
begin
  CheckFiler;
  FFiler.GetLibraryNames(AStrings);
end;

procedure TDgrLibraryManager.ReloadLibrary(ALibName: string);
var
  ALib: TDgrLibrary;
begin
  CheckFiler;
  ALib := LibraryByName(ALibName);
  if ALib <> nil then
    FFiler.Load(ALib);
end;

procedure TDgrLibraryManager.Loaded;
begin
  inherited;
  if FAutoLoad then
    LoadLibraries;
end;

{TDiagramLayers}

function TDiagramLayers.Add(AName: string = ''): TDiagramLayer;
begin
  result := TDiagramLayer(inherited Add);
  if AName <> '' then
    result.LayerName := AName
  else
    result.LayerName := FindLayerName;
end;                                
                                     
constructor TDiagramLayers.Create(ADiagram: TatDiagram);
begin
  FDiagram := ADiagram;
  inherited Create(ADiagram, TDiagramLayer);
end;

function TDiagramLayers.FindByName(const AName: string): TDiagramLayer;
var 
  c: integer;
begin
  result := nil;
  if AName <> '' then
    for c := 0 to Count - 1 do
      if SameText(Items[c].LayerName, AName) then
      begin
        result := Items[c];
        break;
      end;
end;

function TDiagramLayers.FindLayerName: string;
var
  c: integer;
begin
  c := 1;
  repeat
    result := Format('Layer%d', [c]);
    inc(c);
  until FindByName(result) = nil;
end;

function TDiagramLayers.GetItem(Index: integer): TDiagramLayer;
begin
  result := TDiagramLayer(inherited Items[Index]);
end;

procedure TDiagramLayers.SetItem(Index: integer; Value: TDiagramLayer);
begin
  Items[Index].Assign(Value);
end;

{TDiagramLayer}

constructor TDiagramLayer.Create(Collection: TCollection); 
begin
  inherited Create(Collection);
  if Collection is TDiagramLayers then
    FDiagram := TDiagramLayers(Collection).FDiagram;
  FVisible := true;
  FEditable := true;
end;

destructor TDiagramLayer.Destroy;
var
  c: integer;
begin
  {Remove references to this layer.}
  if Diagram <> nil then
    for c := 0 to Diagram.DControlCount - 1 do
      if Diagram.DControls[c].LayerObj = Self then
      begin
        if csLoading in Diagram.ComponentState then // restore control layer after load
          Diagram.DControls[c].FLayerNameFix := Self.LayerName;
        Diagram.DControls[c].LayerObj := nil;
      end;
  inherited;
end;

procedure TDiagramLayer.SetVisible(Value: boolean);
begin
  if FVisible <> Value then
  begin
    FVisible := Value;
    if FDiagram <> nil then
    begin
      FDiagram.UpdateSelecteds;
      FDiagram.Redraw;
    end;
  end;
end;

initialization
  RegDControlList := TRegDControls.Create(nil, TRegDControl);
  RegisterStandardDControls;
  CF_DIAGRAM := RegisterClipboardFormat('Diagram Studio');

finalization
  RegDControlList.Free;
  if _DPrinters <> nil then
    _DPrinters.Free;
  _DPrinters := nil;
end.

