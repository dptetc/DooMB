unit gplmain;

interface

uses
  midasdll, Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ComCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    OpenDialog: TOpenDialog;
    OpenButton: TButton;
    CloseButton: TButton;
    PlayButton: TButton;
    StopButton: TButton;
    PosTrackBar: TTrackBar;
    TrackTimer: TTimer;
    procedure OpenButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure CloseButtonClick(Sender: TObject);
    procedure PlayButtonClick(Sender: TObject);
    procedure StopButtonClick(Sender: TObject);
    procedure PosTrackBarChange(Sender: TObject);
    procedure TrackTimerTimer(Sender: TObject);
  private
    { Private declarations }
    module : MIDASmodule;
    playHandle : MIDASmodulePlayHandle;
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure MIDASerror;
begin
  Application.MessageBox(MIDASgetErrorMessage(MIDASgetLastError), 'MIDAS Error', MB_OK);
  MIDASclose;
  halt;
end;


procedure TForm1.OpenButtonClick(Sender: TObject);
var
  s : array[0..512] of Char;
  i : MIDASmoduleInfo;
begin
  if OpenDialog.Execute then
  begin
    if playHandle <> 0 then
      if not MIDASstopModule(playHandle) then MIDASerror;
    playHandle := 0;
    if module <> NIL then
      if not MIDASfreeModule(module) then MIDASerror;
    module := NIL;
    module := MIDASloadModule(StrPCopy(@s, OpenDialog.FileName));
    if module = NIL then MIDASerror;
    if not MIDASgetModuleInfo(module, @i) then MIDASerror;
    Caption := i.songName;
    PosTrackBar.Max := i.songLength - 1;
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  module := NIL;
  playHandle := 0;
end;

procedure TForm1.CloseButtonClick(Sender: TObject);
begin
  if playHandle <> 0 then
    if not MIDASstopModule(playHandle) then MIDASerror;
  playHandle := 0;
  if module <> NIL then
    if not MIDASfreeModule(module) then MIDASerror;
  module := NIL;
  Caption := 'Example MIDAS player';
  PosTrackBar.Max := 0;
end;

procedure TForm1.PlayButtonClick(Sender: TObject);
begin
  if module = NIL then exit;
  if playHandle <> 0 then
    if not MIDASstopModule(playHandle) then MIDASerror;
  playHandle := MIDASplayModule(module, True);
  if playHandle = 0 then MIDASerror;
end;

procedure TForm1.StopButtonClick(Sender: TObject);
begin
  if playHandle <> 0 then
    if not MIDASstopModule(playHandle) then MIDASerror;
  playHandle := 0;
end;

procedure TForm1.PosTrackBarChange(Sender: TObject);
begin
  if playHandle = 0 then exit;
  MIDASsetPosition(playHandle, PosTrackBar.Position);
end;

procedure TForm1.TrackTimerTimer(Sender: TObject);
var
  s : MIDASplayStatus;
begin
  if playHandle = 0 then
  begin
    PosTrackBar.Position := 0;
  end
  else begin
    if not MIDASgetPlayStatus(playHandle, @s) then MIDASerror;
    PosTrackBar.Position := s.position;
  end
end;

end.
