unit OpenURL;

interface
uses Classes, SysUtils
{$IFDEF MACOS}
 ,Macapi.CoreFoundation,Macapi.Foundation, Macapi.AppKit
{$ENDIF MACOS}
;
procedure NSOpenURL(const AUrl: string);

implementation
procedure NSOpenURL(const AUrl: string);
{$IFDEF MACOS}
var
  CF1, CF2: CFStringRef;
  URLObj: NSURL;
  Workspace: NSWorkspace;
{$ENDIF MACOS}
begin
{$IFDEF MACOS}
  CF2 := nil;
  CF1 := CFStringCreateWithCharactersNoCopy(nil, PChar(AUrl),
    Length(AURL), kCFAllocatorNull);
  try
    CF2 := CFURLCreateStringByAddingPercentEscapes(
      kCFAllocatorDefault, CF1, nil, nil, kCFStringEncodingUTF8);
    Workspace := TNSWorkspace.Wrap(TNSWorkspace.OCClass.sharedWorkspace);
    URLObj :=
TNSURL.Wrap(TNSURL.OCClass.URLWithString(TNSString.Wrap(CF2)));
    Workspace.openURL(URLObj);
  finally
    if CF2 <> nil then CFRelease(CF2);
    if CF1 <> nil then CFRelease(CF1);
  end;
{$ENDIF MACOS}
end;

end.
