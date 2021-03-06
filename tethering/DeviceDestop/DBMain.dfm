object frmDBMain: TfrmDBMain
  Left = 0
  Top = 0
  Caption = #35774#22791#22270#29255#37319#38598#32456#31471
  ClientHeight = 528
  ClientWidth = 812
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 400
    Top = 73
    Width = 412
    Height = 455
    Align = alRight
    TabOrder = 0
    object Image1: TImage
      Left = 1
      Top = 1
      Width = 410
      Height = 453
      Align = alClient
      Stretch = True
      ExplicitLeft = 104
      ExplicitTop = 104
      ExplicitWidth = 105
      ExplicitHeight = 105
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 812
    Height = 73
    Align = alTop
    TabOrder = 1
    object Label1: TLabel
      Left = 112
      Top = 16
      Width = 60
      Height = 13
      Caption = #24403#21069#35774#22791#65306
    end
    object lbPhotoFile: TLabel
      Left = 112
      Top = 44
      Width = 52
      Height = 13
      Caption = 'lbPhotoFile'
    end
    object lbDevice: TLabel
      Left = 168
      Top = 16
      Width = 40
      Height = 13
      Caption = 'lbDevice'
    end
    object btConfig: TButton
      Left = 8
      Top = 11
      Width = 75
      Height = 25
      Action = acConnDB
      TabOrder = 0
    end
    object btRefresh: TButton
      Left = 8
      Top = 42
      Width = 75
      Height = 25
      Action = acRefresh
      TabOrder = 1
    end
    object btSetup: TButton
      Left = 699
      Top = 6
      Width = 75
      Height = 25
      Caption = #36830#25509#21442#25968#35774#32622
      TabOrder = 2
      OnClick = btSetupClick
    end
    object edAccDbStr: TEdit
      Left = 464
      Top = 8
      Width = 229
      Height = 21
      TabOrder = 3
      Text = 
        'Provider=SQLOLEDB.1;Password=sql2008!@#;Persist Security Info=Tr' +
        'ue;User ID=sa;Initial Catalog=XZSSWork;Data Source=SERVER2008;Us' +
        'e Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;W' +
        'orkstation ID=SUNNYZHAN-PC;Use Encryption for Data=False;Tag wit' +
        'h column collation when possible=False'
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 73
    Width = 400
    Height = 455
    Align = alClient
    DataSource = DataSource1
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgTitleClick, dgTitleHotTrack]
    TabOrder = 2
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'ID'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SN'
        Width = 120
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Name'
        Title.Caption = #35774#22791#21517#31216
        Width = 200
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Model'
        Title.Caption = #22411#21495
        Width = 200
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Photo'
        Width = 200
        Visible = True
      end>
  end
  object ADOConnection1: TADOConnection
    LoginPrompt = False
    Provider = 'SQLOLEDB'
    Left = 40
    Top = 88
  end
  object ADOTable1: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'tbDeviceList'
    Left = 136
    Top = 80
  end
  object FSTManager: TTetheringManager
    Password = '1314'
    Text = 'FSTManager'
    Left = 40
    Top = 128
  end
  object FSTAppProfile: TTetheringAppProfile
    Manager = FSTManager
    Text = #35774#22791#22270#29255#37319#38598#32456#31471
    Group = 'FSTAppGroup'
    Actions = <
      item
        Name = 'acConnDB'
        IsPublic = True
        Action = acConnDB
        NotifyUpdates = False
      end
      item
        Name = 'acRefresh'
        IsPublic = True
        Action = acRefresh
        NotifyUpdates = False
      end>
    Resources = <
      item
        Name = 'Image'
        IsPublic = True
        ResType = Stream
      end
      item
        Name = 'IsLocate'
        IsPublic = True
      end
      item
        Name = 'DeviceName'
        IsPublic = True
      end
      item
        Name = 'DeviceModel'
        IsPublic = True
      end
      item
        Name = 'DeviceID'
        IsPublic = True
      end>
    OnResourceReceived = FSTAppProfileResourceReceived
    Left = 152
    Top = 136
  end
  object BindSourceDB1: TBindSourceDB
    DataSet = ADOTable1
    ScopeMappings = <>
    Left = 216
    Top = 224
  end
  object BindingsList2: TBindingsList
    Methods = <>
    OutputConverters = <>
    Left = 296
    Top = 144
    object BindLink1: TBindLink
      Category = 'Links'
      SourceMemberName = 'Name'
      ControlComponent = lbDevice
      SourceComponent = BindSourceDB1
      ParseExpressions = <>
      FormatExpressions = <
        item
          ControlExpression = 'Caption'
          SourceExpression = 'AsString'
        end>
      ClearExpressions = <>
      AutoActivate = False
      Track = False
      OnAssignedValue = BindLink1AssignedValue
    end
  end
  object ActionList1: TActionList
    Left = 400
    Top = 200
    object acLocateRecord: TAction
      Caption = #23450#20301#35760#24405
    end
    object acUpdatePhoto: TAction
      Caption = #20445#23384#22270#29255
      OnExecute = acUpdatePhotoExecute
    end
    object acConnDB: TAction
      Caption = #36830#25509#25968#25454#24211
      OnExecute = acConnDBExecute
      OnUpdate = acConnDBUpdate
    end
    object acRefresh: TAction
      Caption = #21047#26032#25968#25454
      OnExecute = acRefreshExecute
    end
  end
  object DataSource1: TDataSource
    DataSet = ADOTable1
    Left = 256
    Top = 336
  end
end
