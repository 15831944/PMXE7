object Form1: TForm1
  Left = 315
  Top = 292
  Caption = 'Form1'
  ClientHeight = 248
  ClientWidth = 458
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 248
    Top = 56
    Width = 36
    Height = 13
    Caption = #21495#30721#65306
  end
  object Label2: TLabel
    Left = 248
    Top = 96
    Width = 36
    Height = 13
    Caption = #20869#23481#65306
  end
  object Button1: TButton
    Left = 48
    Top = 16
    Width = 75
    Height = 25
    Caption = #35013#36733#20989#25968
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 48
    Top = 47
    Width = 75
    Height = 25
    Caption = #21021#22987#21270
    TabOrder = 1
    OnClick = Button2Click
  end
  object Edit1: TEdit
    Left = 296
    Top = 48
    Width = 121
    Height = 21
    TabOrder = 2
    Text = '13728060684'
  end
  object Edit2: TEdit
    Left = 304
    Top = 88
    Width = 121
    Height = 21
    TabOrder = 3
    Text = #24685#21916#21457#36130#20808#26234#27979#35797
  end
  object Button3: TButton
    Left = 48
    Top = 78
    Width = 88
    Height = 25
    Caption = #21457#36865#65306
    TabOrder = 4
    OnClick = Button3Click
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 229
    Width = 458
    Height = 19
    Panels = <>
  end
  object Button4: TButton
    Left = 48
    Top = 144
    Width = 75
    Height = 25
    Caption = 'Button4'
    TabOrder = 6
    OnClick = Button4Click
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 304
    Top = 152
  end
end
