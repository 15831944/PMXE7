object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'zxing-test'
  ClientHeight = 420
  ClientWidth = 509
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 35
    Width = 31
    Height = 13
    Caption = 'picfile:'
  end
  object Label2: TLabel
    Left = 16
    Top = 68
    Width = 36
    Height = 13
    Caption = 'format:'
  end
  object Label3: TLabel
    Left = 16
    Top = 104
    Width = 31
    Height = 13
    Caption = 'result:'
  end
  object spbOpen: TSpeedButton
    Left = 360
    Top = 32
    Width = 23
    Height = 22
    Caption = '...'
    OnClick = spbOpenClick
  end
  object Image1: TImage
    Left = 120
    Top = 192
    Width = 209
    Height = 189
  end
  object Label4: TLabel
    Left = 16
    Top = 141
    Width = 28
    Height = 13
    Caption = 'error:'
  end
  object edFileName: TEdit
    Left = 72
    Top = 32
    Width = 289
    Height = 21
    ReadOnly = True
    TabOrder = 0
  end
  object edBarcodeFormat: TEdit
    Left = 72
    Top = 66
    Width = 273
    Height = 21
    TabOrder = 1
  end
  object edBarcodeValue: TEdit
    Left = 72
    Top = 101
    Width = 345
    Height = 21
    TabOrder = 2
  end
  object btDecode: TButton
    Left = 8
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Decode'
    TabOrder = 3
    OnClick = btDecodeClick
  end
  object btEncode: TButton
    Left = 376
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Encode'
    TabOrder = 4
  end
  object edError: TEdit
    Left = 72
    Top = 136
    Width = 345
    Height = 21
    TabOrder = 5
  end
  object OpenPictureDialog1: TOpenPictureDialog
    DefaultExt = '*.jpg'
    Filter = 
      'All (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png|JPEG Image File (*.j' +
      'pg)|*.jpg|JPEG Image File (*.jpeg)|*.jpeg|Portable Network Graph' +
      'ics (*.png)|*.png'
    Options = [ofReadOnly, ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Barcode picture file'
    Left = 440
    Top = 40
  end
end
