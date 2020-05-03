object fmApprovalEditor: TfmApprovalEditor
  Left = 384
  Top = 172
  Caption = 'Approval'
  ClientHeight = 470
  ClientWidth = 476
  Color = clBtnFace
  Constraints.MinHeight = 340
  Constraints.MinWidth = 300
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnActivate = FormActivate
  OnCreate = FormCreate
  DesignSize = (
    476
    470)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 5
    Top = 5
    Width = 456
    Height = 420
    ActivePage = tsGeneral
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object tsGeneral: TTabSheet
      Caption = 'General'
      DesignSize = (
        448
        392)
      object lbSubject: TLabel
        Left = 5
        Top = 5
        Width = 36
        Height = 13
        Caption = 'Subject'
      end
      object lbDescription: TLabel
        Left = 5
        Top = 45
        Width = 53
        Height = 13
        Caption = 'Description'
      end
      object edSubject: TEdit
        Left = 5
        Top = 20
        Width = 438
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = PropertyChanged
      end
      object mmComments: TMemo
        Left = 5
        Top = 60
        Width = 438
        Height = 85
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 1
        OnChange = PropertyChanged
      end
      object cbMailNotification: TCheckBox
        Left = 5
        Top = 152
        Width = 250
        Height = 17
        Caption = 'Send mail notification'
        TabOrder = 2
        OnClick = PropertyChanged
      end
      inline frAssignedUser: TfrAssignedUserFrame
        Left = 0
        Top = 172
        Width = 448
        Height = 220
        Align = alBottom
        TabOrder = 3
        ExplicitTop = 172
        ExplicitWidth = 448
        inherited Splitter1: TSplitter
          Width = 448
          ExplicitWidth = 474
        end
        inherited Panel1: TPanel
          Width = 448
          ExplicitWidth = 448
          inherited lvAssignedUsers: TListView
            Width = 438
            ExplicitWidth = 438
          end
        end
        inherited Panel2: TPanel
          Width = 448
          ExplicitWidth = 448
        end
        inherited Panel3: TPanel
          Width = 448
          ExplicitWidth = 448
        end
      end
    end
    object tsStatus: TTabSheet
      Caption = 'Status'
      ImageIndex = 1
      inline frStatus: TfrStatusFrame
        Left = 0
        Top = 0
        Width = 448
        Height = 392
        Align = alClient
        TabOrder = 0
        ExplicitWidth = 448
        ExplicitHeight = 392
        inherited Splitter1: TSplitter
          Width = 448
          ExplicitWidth = 372
        end
        inherited Panel1: TPanel
          Width = 448
          ExplicitWidth = 448
          inherited lvStatus: TListView
            Width = 438
            ExplicitWidth = 438
          end
        end
        inherited Panel2: TPanel
          Width = 448
          Height = 234
          ExplicitWidth = 448
          ExplicitHeight = 234
          inherited edStatusName: TEdit
            Width = 353
            ExplicitWidth = 353
          end
        end
        inherited Panel3: TPanel
          Width = 448
          ExplicitWidth = 448
        end
      end
    end
    object tsAttachments: TTabSheet
      Caption = 'Attachments'
      ImageIndex = 2
      inline frPermissions: TfrAttachPermissions
        Left = 0
        Top = 0
        Width = 448
        Height = 392
        Align = alClient
        TabOrder = 0
        ExplicitWidth = 448
        ExplicitHeight = 392
        inherited cbShowAttachments: TCheckBox
          Width = 220
          ExplicitWidth = 220
        end
        inherited cbDelAttachment: TCheckBox
          Width = 220
          ExplicitWidth = 220
        end
        inherited cbInsAttachment: TCheckBox
          Width = 220
          ExplicitWidth = 220
        end
        inherited cbEditAttachment: TCheckBox
          Width = 220
          ExplicitWidth = 220
        end
      end
    end
    object tsFields: TTabSheet
      Caption = 'Fields'
      ImageIndex = 3
      inline frFields: TfrFieldFrame
        Left = 0
        Top = 0
        Width = 448
        Height = 392
        Align = alClient
        TabOrder = 0
        ExplicitWidth = 448
        ExplicitHeight = 392
        inherited Splitter1: TSplitter
          Top = 151
          Width = 448
          ExplicitTop = 151
          ExplicitWidth = 372
        end
        inherited Panel1: TPanel
          Top = 30
          Width = 448
          ExplicitTop = 30
          ExplicitWidth = 448
          inherited lvVars: TListView
            Width = 438
            ExplicitWidth = 438
          end
        end
        inherited Panel2: TPanel
          Top = 154
          Width = 448
          Height = 238
          ExplicitTop = 154
          ExplicitWidth = 448
          ExplicitHeight = 238
          inherited edCaption: TEdit
            Width = 343
            ExplicitWidth = 343
          end
          inherited cbReadOnly: TCheckBox
            Width = 85
            ExplicitWidth = 85
          end
          inherited cbRequired: TCheckBox
            Width = 85
            ExplicitWidth = 85
          end
        end
        inherited Panel3: TPanel
          Width = 448
          Height = 30
          ExplicitWidth = 448
          ExplicitHeight = 30
        end
      end
    end
    object tsExpiration: TTabSheet
      Caption = 'Expiration'
      ImageIndex = 4
      inline frExpiration: TfrExpirationFrame
        Left = 0
        Top = 0
        Width = 448
        Height = 392
        Align = alClient
        TabOrder = 0
        ExplicitWidth = 448
        ExplicitHeight = 392
        inherited edExpression: TEdit
          Width = 418
          ExplicitWidth = 418
        end
        inherited cbStatus: TComboBox
          Width = 438
          ExplicitWidth = 438
        end
      end
    end
  end
  object btCancel: TButton
    Left = 386
    Top = 430
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = btCancelClick
  end
  object btOk: TButton
    Left = 306
    Top = 430
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Ok'
    TabOrder = 1
    OnClick = btOkClick
  end
end
