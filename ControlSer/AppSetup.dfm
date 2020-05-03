object frmAppSetup: TfrmAppSetup
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #25511#21046#26381#21153#22120#35774#32622
  ClientHeight = 307
  ClientWidth = 586
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 12
  object btCancel: TButton
    Left = 488
    Top = 162
    Width = 75
    Height = 25
    Caption = #36864#20986'(&C)'
    TabOrder = 0
    OnClick = btCancelClick
  end
  object btSave: TButton
    Left = 488
    Top = 59
    Width = 75
    Height = 25
    Caption = #20445#23384'(&S)'
    TabOrder = 1
    OnClick = btSaveClick
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 288
    Width = 586
    Height = 19
    Panels = <
      item
        Width = 50
      end>
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 457
    Height = 288
    ActivePage = TabSerSetting
    Align = alLeft
    TabOrder = 3
    object TabDBConnection: TTabSheet
      Caption = #25511#21046#26381#21153#22120#21015#34920
      object lvServerList: TListView
        Left = 0
        Top = 0
        Width = 449
        Height = 219
        Align = alClient
        Columns = <
          item
            Caption = #21517#31216
            Width = 100
          end
          item
            Caption = #25991#20214#21517
            Width = 200
          end
          item
            Caption = #22791#27880
            Width = 100
          end
          item
            Caption = #29366#24577
            Width = 100
          end>
        GridLines = True
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        PopupMenu = PopupMenu1
        TabOrder = 0
        ViewStyle = vsReport
        OnSelectItem = lvServerListSelectItem
        ExplicitLeft = 29
        ExplicitTop = 3
      end
      object Panel1: TPanel
        Left = 0
        Top = 219
        Width = 449
        Height = 41
        Align = alBottom
        TabOrder = 1
        object btAdd: TButton
          Left = 40
          Top = 6
          Width = 75
          Height = 25
          Action = acAdd
          TabOrder = 0
        end
        object btEdit: TButton
          Left = 172
          Top = 6
          Width = 75
          Height = 25
          Action = acEdit
          TabOrder = 1
        end
        object btDelete: TButton
          Left = 305
          Top = 6
          Width = 75
          Height = 25
          Action = acDelete
          TabOrder = 2
        end
      end
    end
    object TabSerSetting: TTabSheet
      Caption = #26381#21153#22120#21442#25968
      ImageIndex = 1
      object GroupBox2: TGroupBox
        Left = 24
        Top = 16
        Width = 401
        Height = 225
        Caption = #26381#21153#22120#21442#25968
        TabOrder = 0
        object Label2: TLabel
          Left = 33
          Top = 87
          Width = 60
          Height = 12
          Caption = 'HTTP'#31471#21475#65306
        end
        object Label3: TLabel
          Left = 21
          Top = 58
          Width = 72
          Height = 12
          Caption = 'TCP/IP'#31471#21475#65306
        end
        object Label5: TLabel
          Left = 21
          Top = 29
          Width = 72
          Height = 12
          Caption = #26381#21153#22120#21517#31216#65306
        end
        object Label1: TLabel
          Left = 57
          Top = 116
          Width = 36
          Height = 12
          Caption = #29992#25143#65306
        end
        object Label4: TLabel
          Left = 57
          Top = 145
          Width = 36
          Height = 12
          Caption = #21475#20196#65306
        end
        object Label6: TLabel
          Left = 27
          Top = 175
          Width = 66
          Height = 12
          Caption = ' '#26816#27979#21608#26399#65306
        end
        object Label7: TLabel
          Left = 224
          Top = 176
          Width = 12
          Height = 12
          Caption = #31186
        end
        object edHTTP: TEdit
          Left = 99
          Top = 85
          Width = 134
          Height = 20
          ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 1
          OnKeyPress = edHTTPKeyPress
        end
        object edTCPIP: TEdit
          Left = 99
          Top = 56
          Width = 134
          Height = 20
          ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 2
          OnKeyPress = edHTTPKeyPress
        end
        object edServerName: TEdit
          Left = 99
          Top = 27
          Width = 134
          Height = 20
          ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
          TabOrder = 0
        end
        object btStart: TButton
          Left = 290
          Top = 104
          Width = 75
          Height = 25
          Action = acSCStart
          TabOrder = 3
        end
        object btStop: TButton
          Left = 290
          Top = 144
          Width = 75
          Height = 25
          Action = acSCStop
          TabOrder = 4
        end
        object btReg: TButton
          Left = 290
          Top = 24
          Width = 75
          Height = 25
          Action = acSCReg
          TabOrder = 5
        end
        object btUnReg: TButton
          Left = 290
          Top = 64
          Width = 75
          Height = 25
          Action = acSCUnReg
          TabOrder = 6
        end
        object edUser: TEdit
          Left = 99
          Top = 114
          Width = 134
          Height = 20
          TabOrder = 7
        end
        object edPassword: TEdit
          Left = 99
          Top = 143
          Width = 134
          Height = 20
          PasswordChar = '*'
          TabOrder = 8
        end
        object edPeriod: TEdit
          Left = 99
          Top = 172
          Width = 121
          Height = 20
          TabOrder = 9
          OnKeyPress = edHTTPKeyPress
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 392
    Top = 96
    object acAdd: TAction
      Category = 'button'
      Caption = #26032#22686'(&A)'
      OnExecute = acAddExecute
    end
    object acEdit: TAction
      Category = 'button'
      Caption = #20462#25913'(&E)'
      OnExecute = acEditExecute
    end
    object acDelete: TAction
      Category = 'button'
      Caption = #21024#38500'(&D)'
      OnExecute = acDeleteExecute
    end
    object acSerStart: TAction
      Category = 'Server'
      Caption = #21551#21160#26381#21153
      OnExecute = acSerStartExecute
    end
    object acSerStop: TAction
      Category = 'Server'
      Caption = #20851#38381#26381#21153
      OnExecute = acSerStopExecute
    end
    object acSerInstall: TAction
      Category = 'Server'
      Caption = #23433#35013#26381#21153
      OnExecute = acSerInstallExecute
    end
    object acSerUnInst: TAction
      Category = 'Server'
      Caption = #21368#36733#26381#21153
      OnExecute = acSerUnInstExecute
    end
    object acSCReg: TAction
      Category = 'ServerControl'
      Caption = #23433#35013#26381#21153
      OnExecute = acSCRegExecute
    end
    object acSCUnReg: TAction
      Category = 'ServerControl'
      Caption = #21368#36733#26381#21153
      OnExecute = acSCUnRegExecute
    end
    object acSCStart: TAction
      Category = 'ServerControl'
      Caption = #21551#21160#26381#21153
      OnExecute = acSCStartExecute
    end
    object acSCStop: TAction
      Category = 'ServerControl'
      Caption = #20572#27490#26381#21153
      OnExecute = acSCStopExecute
    end
    object acRefresh: TAction
      Category = 'button'
      Caption = #21047#26032
      OnExecute = acRefreshExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 228
    Top = 112
    object miStart: TMenuItem
      Action = acSerStart
    end
    object miStop: TMenuItem
      Action = acSerStop
    end
    object miInstall: TMenuItem
      Action = acSerInstall
    end
    object miUnInstall: TMenuItem
      Action = acSerUnInst
    end
  end
end