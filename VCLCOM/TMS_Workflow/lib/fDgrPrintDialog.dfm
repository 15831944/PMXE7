object fmDgrPrintDialog: TfmDgrPrintDialog
  Left = 311
  Top = 165
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #25171#21360
  ClientHeight = 263
  ClientWidth = 448
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object btOk: TButton
    Left = 280
    Top = 230
    Width = 75
    Height = 25
    HelpContext = 40
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object btCancel: TButton
    Left = 360
    Top = 230
    Width = 75
    Height = 25
    HelpContext = 50
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 1
  end
  object Label12: TGroupBox
    Left = 4
    Top = 4
    Width = 437
    Height = 97
    Caption = #25171#21360#26426
    TabOrder = 2
    object Label4: TLabel
      Left = 14
      Top = 52
      Width = 30
      Height = 12
      Alignment = taRightJustify
      Caption = #31867#22411':'
    end
    object Label3: TLabel
      Left = 14
      Top = 72
      Width = 30
      Height = 12
      Alignment = taRightJustify
      Caption = #20301#32622':'
    end
    object lbType: TLabel
      Left = 52
      Top = 52
      Width = 36
      Height = 12
      Caption = 'lbType'
    end
    object lbWhere: TLabel
      Left = 52
      Top = 72
      Width = 42
      Height = 12
      Caption = 'lbWhere'
    end
    object cbPrinters: TComboBox
      Left = 8
      Top = 20
      Width = 317
      Height = 20
      HelpContext = 142
      Style = csDropDownList
      ItemHeight = 12
      TabOrder = 0
      OnChange = cbPrintersChange
    end
    object btProperties: TButton
      Left = 332
      Top = 19
      Width = 95
      Height = 22
      HelpContext = 152
      Caption = #23646#24615'...'
      TabOrder = 1
      OnClick = btPropertiesClick
    end
  end
  object Label1: TGroupBox
    Left = 4
    Top = 104
    Width = 229
    Height = 117
    Caption = #39029#30721#33539#22260
    TabOrder = 3
    object lbDescription: TLabel
      Left = 16
      Top = 76
      Width = 210
      Height = 28
      AutoSize = False
      Caption = '    '#35831#36755#20837#39029#30721#25110#39029#30721#33539#22260#65292#29992#20998#21495#38548#24320#65292#22914#65306' 1,3,5-12'
      WordWrap = True
    end
    object rbAllPages: TRadioButton
      Left = 8
      Top = 20
      Width = 125
      Height = 17
      HelpContext = 108
      Caption = #20840#37096
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object rbPageNumbers: TRadioButton
      Left = 8
      Top = 43
      Width = 58
      Height = 17
      HelpContext = 124
      Caption = #39029#30721':'
      TabOrder = 1
      OnClick = rbPageNumbersClick
    end
    object edPageNumbers: TEdit
      Left = 72
      Top = 42
      Width = 145
      Height = 20
      HelpContext = 133
      TabOrder = 2
      OnChange = edPageNumbersChange
    end
  end
  object Label2: TGroupBox
    Left = 240
    Top = 104
    Width = 201
    Height = 117
    Caption = #20221#25968
    TabOrder = 4
    object lbCopies: TLabel
      Left = 64
      Top = 20
      Width = 48
      Height = 12
      Caption = #21103#26412#25968#37327
      FocusControl = edCopies
    end
    object imgNoCollate: TImage
      Left = 12
      Top = 56
      Width = 92
      Height = 40
      AutoSize = True
      Picture.Data = {
        07544269746D6170F6070000424DF60700000000000076000000280000005C00
        0000280000000100040000000000800700000000000000000000100000000000
        000000000000000080000080000000808000800000008000800080800000C0C0
        C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
        FF00777777777777777777777777777777777777777777777777777777777777
        7777777777777777777777777777777700007000000000000000000000777777
        7777000000000000000000000777777777700000000000000000000077777777
        000070FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07777777
        7770FFFFFFFFFFFFFFFFFFF077777777000070FFFFFFFFFFFFFFFFFFF0777777
        77770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF077777777
        000070FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07777777
        7770FFFFFFFFFFFFFFFFFFF077777777000070FFFFFFFFFFFFFFFFFFF0777777
        77770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF077777777
        000070FFFFFFFFFFFFFFF0FFF077777777770FFFFFFFFFFFFFF0000F07777777
        7770FFFFFFFFFFFFFFF00FF077777777000070FFFFFFFFFFFFFFF0FFF0777777
        77770FFFFFFFFFFFFFF0FFFF077777777770FFFFFFFFFFFFFF0FF0F077777777
        000070FFFFFFFFFFFFFFF0FFF077777777770FFFFFFFFFFFFFFF0FFF07777777
        7770FFFFFFFFFFFFFFFFF0F077777777000070FFFFFFFFFFFFFFF0FFF0777777
        77770FFFFFFFFFFFFFFFF0FF077777777770FFFFFFFFFFFFFFFF0FF077777777
        000070FFFFFFFFFFFFFFF0FFF077777777770FFFFFFFFFFFFFFFFF0F07777777
        7770FFFFFFFFFFFFFFFFF0F077777777000070FFFFFFFFFFFFFF00FFF0777777
        77770FFFFFFFFFFFFFF0FF0F077777777770FFFFFFFFFFFFFF0FF0F077777777
        000070FFFFFFFFFFFFFFF0FFF077777777770FFFFFFFFFFFFFFF00FF07777777
        7770FFFFFFFFFFFFFFF00FF077777777000070FFFFFFFFFFFFFFFFFFF0777777
        77770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF077777777
        000070FFFFFFFFFFFFFFFFFFF000000007770FFFFFFFFFFFFFFFFFFF00000000
        7770FFFFFFFFFFFFFFFFFFF000000007000070FFFFFFFFFFFFFFFFFFF0FFFFFF
        07770FFFFFFFFFFFFFFFFFFF0FFFFFF07770FFFFFFFFFFFFFFFFFFF0FFFFFF07
        000070FFFFFFFFFFFFFFFFFFF0FFFFFF07770FFFFFFFFFFFFFFFFFFF0FFFFFF0
        7770FFFFFFFFFFFFFFFFFFF0FFFFFF07000070FFFFFFFFFFFFFFFFFFF0FFFFFF
        07770FFFFFFFFFFFFFFFFFFF0FFFFFF07770FFFFFFFFFFFFFFFFFFF0FFFFFF07
        000070FFFFFFFFFFFFFFFFFFF0FFFFFF07770FFFFFFFFFFFFFFFFFFF0FFFFFF0
        7770FFFFFFFFFFFFFFFFFFF0FFFFFF07000070FFFFFFFFFFFFFFFFFFF0FF0FFF
        07770FFFFFFFFFFFFFFFFFFF0F0000F07770FFFFFFFFFFFFFFFFFFF0FF00FF07
        000070FFFFFFFFFFFFFFFFFFF0FF0FFF07770FFFFFFFFFFFFFFFFFFF0F0FFFF0
        7770FFFFFFFFFFFFFFFFFFF0F0FF0F07000070FFFFFFFFFFFFFFFFFFF0FF0FFF
        07770FFFFFFFFFFFFFFFFFFF0FF0FFF07770FFFFFFFFFFFFFFFFFFF0FFFF0F07
        000070FFFFFFFFFFFFFFFFFFF0FF0FFF07770FFFFFFFFFFFFFFFFFFF0FFF0FF0
        7770FFFFFFFFFFFFFFFFFFF0FFF0FF07000070FFFFFFFFFFFFFFFFFFF0FF0FFF
        07770FFFFFFFFFFFFFFFFFFF0FFFF0F07770FFFFFFFFFFFFFFFFFFF0FFFF0F07
        000070FFFFFFFFFFFFFFFFFFF0F00FFF07770FFFFFFFFFFFFFFFFFFF0F0FF0F0
        7770FFFFFFFFFFFFFFFFFFF0F0FF0F07000070FFFFFFFFFFFFFFFFFFF0FF0FFF
        07770FFFFFFFFFFFFFFFFFFF0FF00FF07770FFFFFFFFFFFFFFFFFFF0FF00FF07
        00007000000000000000000000FFFFFF0777000000000000000000000FFFFFF0
        777000000000000000000000FFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF070000777777770FFFFFFFFFFFFFFFFFFF
        077777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF07
        0000777777770FFFFFFFFFFFFFFFFFFF077777777770FFFFFFFFFFFFFFFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF0700007777777700000000000000000000
        0777777777700000000000000000000077777777770000000000000000000007
        0000}
      Visible = False
    end
    object imgCollate: TImage
      Left = 20
      Top = 48
      Width = 74
      Height = 53
      AutoSize = True
      Picture.Data = {
        07544269746D6170BE080000424DBE0800000000000076000000280000004A00
        0000350000000100040000000000480800000000000000000000100000000000
        000000000000000080000080000000808000800000008000800080800000C0C0
        C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
        FF00777777777777777777777777777777777777777777777777777777777777
        7777777777777700000000000000000000000000077777777777777777000000
        0000000000000007777777777777770000000FFFFFFFFFFFFFFFFFFF07777777
        77777777770FFFFFFFFFFFFFFFFFFF07777777777777770000000FFFFFFFFFFF
        FFFFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF077777777777777700
        00000FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF07
        777777777777770000000FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFF
        FFFFFFFFFFFFFF07777777777777770000000FFFFFFFFFFFFFFF0FFF07777777
        77777777770FFFFFFFFFFFFFFF0FFF07777777777777770000000FFFFFFFFFFF
        FFFF0FFF0777777777777777770FFFFFFFFFFFFFFF0FFF077777777777777700
        00000FFFFFFFFFFFFFFF0FFF0777777777777777770FFFFFFFFFFFFFFF0FFF07
        777777777777770000000FFFFFFFFFFFFFFF0FFF0777777777777777770FFFFF
        FFFFFFFFFF0FFF07777777777777770000000FFFFFFFFFFFFFFF0FFF07777777
        77777777770FFFFFFFFFFFFFFF0FFF07777777777777770000000FFFFFFFFFFF
        FFF00FFF0777777777777777770FFFFFFFFFFFFFF00FFF077777777777777700
        00000FFFFFFFFFFFFFFF0FFF0777777777777777770FFFFFFFFFFFFFFF0FFF07
        777777777777770000000FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFF
        FFFFFFFFFFFFFF07777777777777770000000FFFFFFFFFFFFFFFFFFF00000000
        77777777770FFFFFFFFFFFFFFFFFFF00000000777777770000000FFFFFFFFFFF
        FFFFFFFF0FFFFFF077777777770FFFFFFFFFFFFFFFFFFF0FFFFFF07777777700
        00000FFFFFFFFFFFFFFFFFFF0FFFFFF077777777770FFFFFFFFFFFFFFFFFFF0F
        FFFFF0777777770000000FFFFFFFFFFFFFFFFFFF0FFFFFF077777777770FFFFF
        FFFFFFFFFFFFFF0FFFFFF0777777770000000FFFFFFFFFFFFFFFFFFF0FFFFFF0
        77777777770FFFFFFFFFFFFFFFFFFF0FFFFFF0777777770000000FFFFFFFFFFF
        FFFFFFFF0F0000F077777777770FFFFFFFFFFFFFFFFFFF0F0000F07777777700
        00000FFFFFFFFFFFFFFFFFFF0F0FFFF077777777770FFFFFFFFFFFFFFFFFFF0F
        0FFFF0777777770000000FFFFFFFFFFFFFFFFFFF0FF0FFF077777777770FFFFF
        FFFFFFFFFFFFFF0FF0FFF0777777770000000FFFFFFFFFFFFFFFFFFF0FFF0FF0
        77777777770FFFFFFFFFFFFFFFFFFF0FFF0FF0777777770000000FFFFFFFFFFF
        FFFFFFFF0FFFF0F077777777770FFFFFFFFFFFFFFFFFFF0FFFF0F07777777700
        00000FFFFFFFFFFFFFFFFFFF0F0FF0F077777777770FFFFFFFFFFFFFFFFFFF0F
        0FF0F0777777770000000FFFFFFFFFFFFFFFFFFF0FF00FF077777777770FFFFF
        FFFFFFFFFFFFFF0FF00FF077777777000000000000000000000000000FFFFFF0
        7777777777000000000000000000000FFFFFF07777777700000077777770FFFF
        FFFFFFFFFFFFFFF0000000077777777770FFFFFFFFFFFFFFFFFFF00000000700
        000077777770FFFFFFFFFFFFFFFFFFF0FFFFFF077777777770FFFFFFFFFFFFFF
        FFFFF0FFFFFF0700000077777770FFFFFFFFFFFFFFFFFFF0FFFFFF0777777777
        70FFFFFFFFFFFFFFFFFFF0FFFFFF0700000077777770FFFFFFFFFFFFFFFFFFF0
        FFFFFF077777777770FFFFFFFFFFFFFFFFFFF0FFFFFF0700000077777770FFFF
        FFFFFFFFFFFFFFF0FFFFFF077777777770FFFFFFFFFFFFFFFFFFF0FFFFFF0700
        000077777770FFFFFFFFFFFFFFFFFFF0FF00FF077777777770FFFFFFFFFFFFFF
        FFFFF0FF00FF0700000077777770FFFFFFFFFFFFFFFFFFF0F0FF0F0777777777
        70FFFFFFFFFFFFFFFFFFF0F0FF0F0700000077777770FFFFFFFFFFFFFFFFFFF0
        FFFF0F077777777770FFFFFFFFFFFFFFFFFFF0FFFF0F0700000077777770FFFF
        FFFFFFFFFFFFFFF0FFF0FF077777777770FFFFFFFFFFFFFFFFFFF0FFF0FF0700
        000077777770FFFFFFFFFFFFFFFFFFF0FFFF0F077777777770FFFFFFFFFFFFFF
        FFFFF0FFFF0F0700000077777770FFFFFFFFFFFFFFFFFFF0F0FF0F0777777777
        70FFFFFFFFFFFFFFFFFFF0F0FF0F0700000077777770FFFFFFFFFFFFFFFFFFF0
        FF00FF077777777770FFFFFFFFFFFFFFFFFFF0FF00FF07000000777777700000
        0000000000000000FFFFFF07777777777000000000000000000000FFFFFF0700
        0000777777777777770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFF
        FFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFFFFFFFF0777777777
        777777770FFFFFFFFFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFF
        FFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF07000000777777777777
        770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF0700
        0000777777777777770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFF
        FFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFFFFFFFF0777777777
        777777770FFFFFFFFFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFF
        FFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF07000000777777777777
        770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF0700
        0000777777777777770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFF
        FFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFFFFFFFF0777777777
        777777770FFFFFFFFFFFFFFFFFFF07000000777777777777770FFFFFFFFFFFFF
        FFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF07000000777777777777
        770FFFFFFFFFFFFFFFFFFF0777777777777777770FFFFFFFFFFFFFFFFFFF0700
        0000777777777777770000000000000000000007777777777777777700000000
        00000000000007000000}
      Visible = False
    end
    object edCopies: TEdit
      Left = 132
      Top = 16
      Width = 37
      Height = 20
      HelpContext = 99
      TabOrder = 0
      Text = '1'
    end
    object cbCollate: TCheckBox
      Left = 116
      Top = 58
      Width = 69
      Height = 17
      HelpContext = 188
      Caption = #36880#20221#25171#21360
      Checked = True
      State = cbChecked
      TabOrder = 1
      Visible = False
    end
    object UpDown1: TUpDown
      Left = 169
      Top = 16
      Width = 16
      Height = 21
      Associate = edCopies
      Min = 1
      Position = 1
      TabOrder = 2
    end
  end
end
