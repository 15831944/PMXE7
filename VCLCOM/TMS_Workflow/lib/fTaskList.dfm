object fmTaskList: TfmTaskList
  Left = 295
  Top = 160
  Caption = 'Task list'
  ClientHeight = 410
  ClientWidth = 690
  Color = clBtnFace
  Constraints.MinHeight = 350
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 140
    Width = 690
    Height = 3
    Cursor = crVSplit
    Align = alTop
  end
  object Splitter2: TSplitter
    Left = 0
    Top = 375
    Width = 690
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 690
    Height = 140
    Align = alTop
    BevelOuter = bvNone
    BorderWidth = 5
    TabOrder = 0
    object lvTasks: TTaskListView
      Left = 5
      Top = 5
      Width = 680
      Height = 130
      Align = alClient
      HideSelection = False
      RowSelect = True
      PopupMenu = PopupMenu1
      TabOrder = 0
      ViewStyle = vsReport
      OnCustomDrawItem = lvTasksCustomDrawItem
      OnSelectItem = lvTasksSelectItem
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 143
    Width = 690
    Height = 232
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object TabControl1: TTabControl
      Left = 0
      Top = 0
      Width = 690
      Height = 232
      Align = alClient
      TabOrder = 0
      OnChange = TabControl1Change
      inline frTaskView: TfrTaskInstanceView
        Left = 4
        Top = 6
        Width = 682
        Height = 222
        Align = alClient
        TabOrder = 1
        ExplicitLeft = 4
        ExplicitTop = 6
        ExplicitWidth = 682
        ExplicitHeight = 222
        inherited mmDescription: TMemo [0]
          Width = 682
          Height = 153
          Lines.Strings = ()
          ExplicitWidth = 682
          ExplicitHeight = 153
        end
        inherited Panel1: TPanel [1]
          Width = 682
          ExplicitWidth = 682
        end
        inherited Panel2: TPanel
          Top = 182
          Width = 682
          ExplicitTop = 182
          ExplicitWidth = 682
        end
      end
      object WorkflowDiagram1: TWorkflowDiagram
        Left = 4
        Top = 6
        Width = 682
        Height = 222
        NettoExportOffset = 3
        AutomaticNodes = False
        AutoScroll = True
        AutoPage = False
        Background.Scroll = True
        Background.Style = biTile
        Background.Visible = False
        Background.Gradient.Direction = grTopBottom
        Background.Gradient.StartColor = clWhite
        Background.Gradient.EndColor = clYellow
        Background.Gradient.Visible = False
        Background.PrintGradient = False
        SnapGrid.Active = False
        SnapGrid.Force = False
        SnapGrid.Visible = False
        SnapGrid.SizeX = 8.000000000000000000
        SnapGrid.SizeY = 8.000000000000000000
        SnapGrid.Style = gsDots
        SnapGrid.SnapToRuler = False
        ShowLinkPoints = True
        LeftRuler.Visible = False
        LeftRuler.Divisions = 5
        LeftRuler.Font.Charset = DEFAULT_CHARSET
        LeftRuler.Font.Color = clWindowText
        LeftRuler.Font.Height = -9
        LeftRuler.Font.Name = 'Arial'
        LeftRuler.Font.Style = []
        LeftRuler.Units = unCenti
        LeftRuler.MinorTickLength = 4
        LeftRuler.MajorTickLength = 6
        LeftRuler.Color = clWhite
        LeftRuler.TickColor = clBlack
        LeftRuler.Size = 16
        LeftRuler.AutoFactor = True
        LeftRuler.GridColor = clBlack
        TopRuler.Visible = False
        TopRuler.Divisions = 5
        TopRuler.Font.Charset = DEFAULT_CHARSET
        TopRuler.Font.Color = clWindowText
        TopRuler.Font.Height = -9
        TopRuler.Font.Name = 'Arial'
        TopRuler.Font.Style = []
        TopRuler.Units = unCenti
        TopRuler.MinorTickLength = 4
        TopRuler.MajorTickLength = 6
        TopRuler.Color = clWhite
        TopRuler.TickColor = clBlack
        TopRuler.Size = 16
        TopRuler.AutoFactor = True
        TopRuler.GridColor = clBlack
        Zoom = 100
        BorderColor = clGray
        MouseWheelMode = mwOff
        Layers = <>
        LinkCursor = crHandPoint
        PanCursor = crHandPoint
        ZoomCursor = crDefault
        IgnoreScreenDPI = False
        ShowCrossIndicators = False
        PageLines.Visible = False
        PageLines.Pen.Style = psDot
        KeyActions = [kaEscape, kaMove, kaPage, kaResize, kaSelect]
        HandlesStyle = hsClassic
        SmoothMode = smAntiAlias
        TextRenderingMode = tmAntiAlias
        SelectionMode = slmMultiple
        CanMoveOutOfBounds = True
        PageSettings.PaperName = 'Carta - 21,59 x 27,94 cm (8,5 x 11 pol.) '
        PageSettings.PaperId = 1
        PageSettings.PaperWidth = 215.900000000000000000
        PageSettings.PaperHeight = 279.400000000000000000
        PageSettings.Orientation = dpoPortrait
        PageSettings.LeftMarginStr = '25.4'
        PageSettings.TopMarginStr = '25.4'
        PageSettings.RightMarginStr = '25.4'
        PageSettings.BottomMarginStr = '25.4'
        RulerAutoUnit = False
        MeasUnit = duCenti
        WheelZoom = False
        WheelZoomIncrement = 10
        WheelZoomMin = 10
        WheelZoomMax = 500
        Align = alClient
        Anchors = [akLeft, akTop, akRight, akBottom]
        BevelInner = bvNone
        BiDiMode = bdLeftToRight
        BorderStyle = bsNone
        Color = clWhite
        ParentColor = False
        ShowHint = False
        TabOrder = 0
        OnClick = WorkflowDiagram1Click
        IdlePercent = 30
        RunColor = clWhite
        Variables = <>
        Attachments = <>
        DisplayTaskStatus = False
      end
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 378
    Width = 690
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      690
      32)
    object Splitter3: TSplitter
      Left = 0
      Top = 0
      Height = 32
    end
    object btSaveChanges: TButton
      Left = 588
      Top = 3
      Width = 89
      Height = 25
      Anchors = [akTop, akRight]
      Caption = '&Save changes'
      TabOrder = 0
      OnClick = btSaveChangesClick
    end
  end
  object ActionList1: TActionList
    Left = 480
    Top = 40
    object acShowOnlyOpen: TAction
      Caption = 'Show only open tasks'
      OnExecute = acShowOnlyOpenExecute
      OnUpdate = acShowOnlyOpenUpdate
    end
    object acShowAllTasks: TAction
      Caption = 'Show all tasks'
      OnExecute = acShowAllTasksExecute
      OnUpdate = acShowAllTasksUpdate
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 360
    Top = 48
    object miView: TMenuItem
      Caption = '&View'
      object Showonlyopentasks2: TMenuItem
        Action = acShowOnlyOpen
      end
      object Showalltasks2: TMenuItem
        Action = acShowAllTasks
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 300
    OnTimer = Timer1Timer
    Left = 184
    Top = 40
  end
end
