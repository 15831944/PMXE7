object XZSmsServerXE: TXZSmsServerXE
  OldCreateOrder = False
  OnCreate = ServiceCreate
  OnDestroy = ServiceDestroy
  AllowPause = False
  DisplayName = 'Foresight SMS Server XE'
  BeforeInstall = ServiceBeforeInstall
  AfterInstall = ServiceAfterInstall
  BeforeUninstall = ServiceBeforeUninstall
  OnStart = ServiceStart
  OnStop = ServiceStop
  Height = 114
  Width = 140
  object Timer1: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = Timer1Timer
    Left = 18
    Top = 16
  end
end
