object DMmodule: TDMmodule
  OldCreateOrder = False
  Height = 150
  Width = 215
  object FSDSConnection: TSQLConnection
    DriverName = 'Datasnap'
    LoginPrompt = False
    Params.Strings = (
      'DriverUnit=Data.DBXDataSnap'
      'HostName=localhost'
      'Port=211'
      'CommunicationProtocol=tcp/ip'
      'DatasnapContext=datasnap/'
      
        'DriverAssemblyLoader=Borland.Data.TDBXClientDriverLoader,Borland' +
        '.Data.DbxClientDriver,Version=16.0.0.0,Culture=neutral,PublicKey' +
        'Token=91d62ebb5b0d1b1b'
      'Filters={}'
      'DSAuthenticationUser=FSAdmin'
      'DSAuthenticationPassword=fst123!@#')
    Left = 128
    Top = 16
    UniqueId = '{F1016E56-39E7-4E75-AFBD-0E7B04AED31B}'
  end
  object DSClientCallbackChannelManager1: TDSClientCallbackChannelManager
    ManagerId = '399121.96532.559919'
    Left = 48
    Top = 40
  end
end
