program gplay;

uses
  midasdll, windows,
  Forms,
  gplmain in 'gplmain.pas' {Form1};

{$R *.RES}

procedure MIDASerror;
begin
  Application.MessageBox(MIDASgetErrorMessage(MIDASgetLastError), 'MIDAS Error', MB_OK);
  MIDASclose;
  halt;
end;


begin
  MIDASstartup;
  if not MIDASconfig then MIDASerror;
  if not MIDASinit then MIDASerror;
  if not MIDASstartBackgroundPlay(0) then MIDASerror;
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
  if not MIDASstopBackgroundPlay then MIDASerror;
  MIDASclose;
end.
