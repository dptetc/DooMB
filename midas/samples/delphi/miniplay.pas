{*      miniplay.pas
 *
 * A minimal Delphi console module player example
 *
 * Compile to a console application and link with midasdll unit
 *
 * Copyright 1996,1997 Housemarque Inc
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*}

uses
    midasdll, sysutils;


procedure MIDASerror;
begin
    WriteLn('MIDAS error: ', MIDASgetErrorMessage(MIDASgetLastError));
    MIDASclose;
    halt;
end;


var
    module : MIDASmodule;
    playHandle : MIDASmodulePlayHandle;

BEGIN
    MIDASstartup;
    if not MIDASinit then
        MIDASerror;
    if not MIDASstartBackgroundPlay(0) then
        MIDASerror;

    module := MIDASloadModule('..\data\templsun.xm');
    if module = NIL then
        MIDASerror;

    playHandle := MIDASplayModule(module, true);
    if playHandle = 0 then
        MIDASerror;

    WriteLn('Playing - press enter');
    ReadLn;

    if not MIDASstopModule(playHandle) then
        MIDASerror;
    if not MIDASfreeModule(module) then
        MIDASerror;
    if not MIDASstopBackgroundPlay then
        MIDASerror;
    if not MIDASclose then
        MIDASerror;
END.