object ServerControlModule: TServerControlModule
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Height = 271
  Width = 415
  object DSServer1: TDSServer
    OnConnect = DSServer1Connect
    AutoStart = False
    Left = 40
    Top = 19
  end
  object DSTCPServerTransport1: TDSTCPServerTransport
    Port = 411
    Server = DSServer1
    Filters = <>
    AuthenticationManager = DSAuthenticationManager1
    KeepAliveEnablement = kaEnabled
    Left = 40
    Top = 81
  end
  object DSHTTPService1: TDSHTTPService
    HttpPort = 4040
    Server = DSServer1
    Filters = <>
    AuthenticationManager = DSAuthenticationManager1
    Left = 32
    Top = 151
  end
  object DSAuthenticationManager1: TDSAuthenticationManager
    OnUserAuthenticate = DSAuthenticationManager1UserAuthenticate
    Roles = <>
    Left = 32
    Top = 205
  end
  object DSServerClass1: TDSServerClass
    OnGetClass = DSServerClass1GetClass
    Server = DSServer1
    Left = 200
    Top = 11
  end
  object DSHTTPServiceProxyDispatcher1: TDSHTTPServiceProxyDispatcher
    Service = DSHTTPService1
    WebFileExtensions = <
      item
        MimeType = 'application/x-zip-compressed'
        Extensions = 'zip'
      end>
    WebDirectories = <
      item
        DirectoryAction = dirInclude
        DirectoryMask = '\proxy\*'
      end
      item
        DirectoryAction = dirExclude
        DirectoryMask = '\proxy\*\*'
      end>
    DSProxyGenerator = DSProxyGenerator1
    Left = 304
    Top = 128
  end
  object DSProxyGenerator1: TDSProxyGenerator
    MetaDataProvider = DSServerMetaDataProvider1
    TargetUnitName = 'ServerFunctions.js'
    TargetDirectory = 'js'
    Writer = 'Java Script REST'
    Left = 304
    Top = 72
  end
  object DSServerMetaDataProvider1: TDSServerMetaDataProvider
    Server = DSServer1
    Left = 304
    Top = 16
  end
  object DSHTTPServiceFileDispatcher1: TDSHTTPServiceFileDispatcher
    Service = DSHTTPService1
    WebFileExtensions = <
      item
        MimeType = 'text/css'
        Extensions = 'css'
      end
      item
        MimeType = 'text/html'
        Extensions = 'html;htm'
      end
      item
        MimeType = 'text/javascript'
        Extensions = 'js'
      end
      item
        MimeType = 'image/jpeg'
        Extensions = 'jpeg;jpg'
      end
      item
        MimeType = 'image/x-png'
        Extensions = 'png'
      end>
    WebDirectories = <
      item
        DirectoryAction = dirInclude
        DirectoryMask = '*'
      end
      item
        DirectoryAction = dirExclude
        DirectoryMask = '\templates\*'
      end>
    Left = 304
    Top = 192
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 200
    Top = 88
  end
end
