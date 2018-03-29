{*      midasdll.pas
 *
 * MIDAS DLL programming interface Delphi interface unit
 *
 * $Id: midasdll.pas,v 1.5 1997/07/31 14:30:58 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc
 *
 * This file is part of MIDAS Digital Audio System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Digital Audio System license, "license.txt". By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*}

unit midasdll;


interface


uses wintypes;


const
    { enum MIDASoptions }
    MIDAS_OPTION_NONE = 0;
    MIDAS_OPTION_MIXRATE = 1;
    MIDAS_OPTION_OUTPUTMODE = 2;
    MIDAS_OPTION_MIXBUFLEN = 3;
    MIDAS_OPTION_MIXBUFBLOCKS = 4;
    MIDAS_OPTION_DSOUND_MODE = 5;
    MIDAS_OPTION_DSOUND_HWND = 6;
    MIDAS_OPTION_DSOUND_OBJECT = 7;
    MIDAS_OPTION_DSOUND_BUFLEN = 8;
    MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT = 9;
    MIDAS_OPTION_FILTER_MODE = 10;
    MIDAS_OPTION_MIXING_MODE = 11;
    MIDAS_OPTION_DEFAULT_STEREO_SEPARATION = 12;
    MIDAS_OPTION_FORCE_NO_SOUND = 13;

   

    { enum MIDASmodes }
    MIDAS_MODE_NONE = 0;
    MIDAS_MODE_MONO = 1;
    MIDAS_MODE_STEREO = 2;
    MIDAS_MODE_8BIT = 4;
    MIDAS_MODE_16BIT = 8;
    MIDAS_MODE_8BIT_MONO = MIDAS_MODE_8BIT or MIDAS_MODE_MONO;
    MIDAS_MODE_8BIT_STEREO = MIDAS_MODE_8BIT or MIDAS_MODE_STEREO;
    MIDAS_MODE_16BIT_MONO = MIDAS_MODE_16BIT or MIDAS_MODE_MONO;
    MIDAS_MODE_16BIT_STEREO = MIDAS_MODE_16BIT or MIDAS_MODE_STEREO;

    { enum MIDASsampleTypes }
    MIDAS_SAMPLE_NONE = 0;
    MIDAS_SAMPLE_8BIT_MONO = 1;
    MIDAS_SAMPLE_16BIT_MONO = 2;
    MIDAS_SAMPLE_8BIT_STEREO = 3;
    MIDAS_SAMPLE_16BIT_STEREO = 4;
    MIDAS_SAMPLE_ADPCM_MONO = 5;
    MIDAS_SAMPLE_ADPCM_STEREO = 6;
    MIDAS_SAMPLE_ULAW_MONO = 7;
    MIDAS_SAMPLE_ULAW_STEREO = 8;
    
    { enum MIDASloop }
    MIDAS_LOOP_NO = 0;
    MIDAS_LOOP_YES = 1;

    { enum MIDASpanning }
    MIDAS_PAN_LEFT = -64;
    MIDAS_PAN_MIDDLE = 0;
    MIDAS_PAN_RIGHT = 64;
    MIDAS_PAN_SURROUND = $80;

    { enum MIDASchannels }
    MIDAS_CHANNEL_AUTO = $FFFF;
    MIDAS_ILLEGAL_CHANNEL = $FFFF;

    { enum MIDASdsoundModes }
    MIDAS_DSOUND_DISABLED = 0;
    MIDAS_DSOUND_STREAM = 1;
    MIDAS_DSOUND_PRIMARY = 2;
    MIDAS_DSOUND_FORCE_STREAM = 3;

    { enum MIDASpositions }
    MIDAS_POSITION_DEFAULT = $FFFFFFFF;

    { enum MIDASfilterModes }
    MIDAS_FILTER_NONE = 0;
    MIDAS_FILTER_LESS = 1;
    MIDAS_FILTER_MORE = 2;
    MIDAS_FILTER_AUTO = 3;

    { enum MIDASechoFilterTypes }
    MIDAS_ECHO_FILTER_NONE = 0;
    MIDAS_ECHO_FILTER_LOWPASS = 1;
    MIDAS_ECHO_FILTER_HIGHPASS = 2;

    { enum MIDASmixingModes }
    MIDAS_MIX_NORMAL_QUALITY = 0;
    MIDAS_MIX_HIGH_QUALITY = 1;

    { enum MIDASsamplePlayStatus }
    MIDAS_SAMPLE_STOPPED = 0;
    MIDAS_SAMPLE_PLAYING = 1;



type MIDASmoduleInfo =
    record
        songName : array[0..31] of char;
        songLength : integer;
        numPatterns : integer;
        numInstruments : integer;
        numChannels : integer;
    end;
    PMIDASmoduleInfo = ^MIDASmoduleInfo;

type MIDASinstrumentInfo =
    record
        instName : array[0..31] of char;
    end;
    PMIDASinstrumentInfo = ^MIDASinstrumentInfo;

type MIDASplayStatus =
    record
        position : dword;
        pattern : dword;
        row : dword;
        syncInfo : integer;
        songLoopCount : dword
    end;                                
    PMIDASplayStatus = ^MIDASplayStatus;

type MIDASecho =
    record
        delay : dword;
        gain : integer;
        reverseChannels : integer;
        filterType : dword;
    end;
    PMIDASecho = ^MIDASecho;

type MIDASechoSet =
    record
        feedback : integer;
        gain : integer;
        numEchoes : integer;
        echoes : PMIDASecho;
    end;
    PMIDASechoSet = ^MIDASechoSet;


    MIDASmodule = pointer;
    MIDASmodulePlayHandle = DWORD;
    MIDASsample = DWORD;
    MIDASsamplePlayHandle = DWORD;
    MIDASstreamHandle = pointer;
    Pbyte = ^byte;
    MIDASechoHandle = pointer;



function MIDASgetLastError : integer; stdcall;
function MIDASgetErrorMessage(errorCode : integer) : PChar; stdcall;

function MIDASstartup : boolean; stdcall;
function MIDASconfig : boolean; stdcall;
function MIDASloadConfig(fileName : PChar) : boolean; stdcall;
function MIDASsaveConfig(fileName : PChar) : boolean; stdcall;
function MIDASreadConfigRegistry(key : dword; subKey : PChar) : boolean; stdcall;
function MIDASwriteConfigRegistry(key : dword; subKey : PChar) : boolean; stdcall;

function MIDASinit : boolean; stdcall;
function MIDASsetOption(option : integer; value : dword) : boolean; stdcall;
function MIDASgetOption(option : integer) : dword; stdcall;
function MIDASclose : boolean; stdcall;
function MIDASsuspend : boolean; stdcall;
function MIDASresume : boolean; stdcall;
function MIDASopenChannels(numChannels : integer) : boolean; stdcall;
function MIDAScloseChannels : boolean; stdcall;
function MIDASsetAmplification(amplification : dword) : boolean; stdcall;
function MIDASstartBackgroundPlay(pollRate : dword) : boolean; stdcall;
function MIDASstopBackgroundPlay : boolean; stdcall;
function MIDASpoll : boolean; stdcall;
function MIDASlock : boolean; stdcall;
function MIDASunlock : boolean; stdcall;
function MIDASgetVersionString : PChar; stdcall;

function MIDASallocateChannel : dword; stdcall;
function MIDASfreeChannel(channel : dword) : boolean; stdcall;

function MIDASloadModule(fileName : PChar) : MIDASmodule; stdcall;
function MIDASplayModule(module : MIDASmodule; loopSong : boolean) :
    MIDASmodulePlayHandle; stdcall;
function MIDASplayModuleSection(module : MIDASmodule; startPos, endPos,
    restartPos : dword; loopSong : boolean) : MIDASmodulePlayHandle; stdcall;
function MIDASstopModule(playHandle : MIDASmodulePlayHandle) : boolean;
    stdcall;
function MIDASfreeModule(module : MIDASmodule) : boolean; stdcall;

function MIDASgetPlayStatus(playHandle : MIDASmodulePlayHandle;
    status : PMIDASplayStatus) : boolean; stdcall;
function MIDASsetPosition(playHandle : MIDASmodulePlayHandle;
    newPosition : integer) : boolean; stdcall;
function MIDASsetMusicVolume(playHandle : MIDASmodulePlayHandle;
    volume : dword) : boolean; stdcall;
function MIDASgetModuleInfo(module : MIDASmodule; info : PMIDASmoduleInfo) :
    boolean; stdcall;
function MIDASgetInstrumentInfo(module : MIDASmodule; instNum : integer;
    info : PMIDASinstrumentInfo) : boolean; stdcall;
function MIDASfadeMusicChannel(playHandle : MIDASmodulePlayHandle; channel,
    fade : dword) : boolean; stdcall;

function MIDASloadRawSample(fileName : PChar; sampleType,
    loopSample : integer) : MIDASsample; stdcall;
function MIDASloadWaveSample(fileName : PChar; loopSample : integer) :
    MIDASsample; stdcall;
function MIDASfreeSample(sample : MIDASsample) : boolean; stdcall;
function MIDASallocAutoEffectChannels(numChannels : dword) : boolean; stdcall;
function MIDASfreeAutoEffectChannels : boolean; stdcall;
function MIDASplaySample(sample : MIDASsample; channel : dword;
    priority : integer; rate, volume : dword; panning : integer) :
    MIDASsamplePlayHandle;
    stdcall;
function MIDASstopSample(sample : MIDASsamplePlayHandle) : boolean; stdcall;
function MIDASsetSampleRate(sample : MIDASsamplePlayHandle; rate : dword) :
    boolean; stdcall;
function MIDASsetSampleVolume(sample : MIDASsamplePlayHandle; volume : dword)
    : boolean; stdcall;
function MIDASsetSamplePanning(sample : MIDASsamplePlayHandle;
    panning : integer) : boolean; stdcall;
function MIDASsetSamplePriority(sample : MIDASsamplePlayHandle;
    priority : integer) : boolean; stdcall;
function MIDASgetSamplePlayStatus(sample : MIDASsamplePlayHandle) : dword;
    stdcall;

function MIDASplayStreamFile(fileName : PChar;
    sampleType : dword; sampleRate : dword; bufferLength : dword;
    loopStream : integer) : MIDASstreamHandle; stdcall;
function MIDASplayStreamWaveFile(fileName : PChar; bufferLength : dword;
    loopStream : integer) : MIDASstreamHandle; stdcall;
function MIDASstopStream(stream : MIDASstreamHandle) : boolean; stdcall;

function MIDASplayStreamPolling(sampleType : dword;
    sampleRate : dword; bufferLength : dword) : MIDASstreamHandle; stdcall;
function MIDASfeedStreamData(stream : MIDASstreamHandle; data : Pbyte;
    numBytes : dword; feedAll : boolean) : dword; stdcall;

function MIDASsetStreamRate(stream : MIDASstreamHandle; rate : dword)
    : boolean; stdcall;
function MIDASsetStreamVolume(stream : MIDASstreamHandle; volume : dword)
    : boolean; stdcall;
function MIDASsetStreamPanning(stream : MIDASstreamHandle; panning : integer)
     : boolean; stdcall;

function MIDASgetStreamBytesBuffered(stream : MIDASstreamHandle) : boolean;
    stdcall;
function MIDASpauseStream(stream : MIDASstreamHandle) : boolean; stdcall;
function MIDASresumeStream(stream : MIDASstreamHandle ) : boolean; stdcall;

function MIDASaddEchoEffect(echoSet : PMIDASechoSet) : MIDASechoHandle;
    stdcall;
function MIDASremoveEchoEffect(echoHandle : MIDASechoHandle) : boolean;
    stdcall;


implementation


function MIDASgetLastError : integer;
    stdcall; external 'midas11.dll' name '_MIDASgetLastError@0';
function MIDASgetErrorMessage(errorCode : integer) : PChar;
    stdcall; external 'midas11.dll' name '_MIDASgetErrorMessage@4';

function MIDASstartup : boolean;
    stdcall; external 'midas11.dll' name '_MIDASstartup@0';
function MIDASconfig : boolean;
    stdcall; external 'midas11.dll' name '_MIDASconfig@0';
function MIDASloadConfig(fileName : PChar) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASloadConfig@4';
function MIDASsaveConfig(fileName : PChar) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsaveConfig@4';
function MIDASreadConfigRegistry(key : dword; subKey : PChar) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASreadConfigRegistry@8';
function MIDASwriteConfigRegistry(key : dword; subKey : PChar) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASwriteConfigRegistry@8';


function MIDASinit : boolean;
    stdcall; external 'midas11.dll' name '_MIDASinit@0';
function MIDASsetOption(option : integer; value : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetOption@8';
function MIDASgetOption(option : integer) : dword;
    stdcall; external 'midas11.dll' name '_MIDASgetOption@4';
function MIDASclose : boolean;
    stdcall; external 'midas11.dll' name '_MIDASclose@0';
function MIDASsuspend : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsuspend@0';
function MIDASresume : boolean; stdcall;
    stdcall; external 'midas11.dll' name '_MIDASresume@0';
function MIDASopenChannels(numChannels : integer) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASopenChannels@4';
function MIDAScloseChannels : boolean;
    stdcall; external 'midas11.dll' name '_MIDAScloseChannels@0';
function MIDASsetAmplification(amplification : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetAmplification@4';
function MIDASstartBackgroundPlay(pollRate : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASstartBackgroundPlay@4';
function MIDASstopBackgroundPlay : boolean;
    stdcall; external 'midas11.dll' name '_MIDASstopBackgroundPlay@0';
function MIDASpoll : boolean;
    stdcall; external 'midas11.dll' name '_MIDASpoll@0';
function MIDASlock : boolean;
    stdcall; external 'midas11.dll' name '_MIDASlock@0';
function MIDASunlock : boolean;
    stdcall; external 'midas11.dll' name '_MIDASunlock@0';
function MIDASgetVersionString : PChar;
    stdcall; external 'midas11.dll' name '_MIDASgetVersionString@0';

function MIDASallocateChannel : dword;
    stdcall; external 'midas11.dll' name '_MIDASallocateChannel@0';
function MIDASfreeChannel(channel : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASfreeChannel@4';

function MIDASloadModule(fileName : PChar) : MIDASmodule;
    stdcall; external 'midas11.dll' name '_MIDASloadModule@4';
function MIDASplayModule(module : MIDASmodule; loopSong : boolean) :
    MIDASmodulePlayHandle;
    stdcall; external 'midas11.dll' name '_MIDASplayModule@8';
function MIDASplayModuleSection(module : MIDASmodule; startPos, endPos,
    restartPos : dword; loopSong : boolean) : MIDASmodulePlayHandle;
    stdcall; external 'midas11.dll' name '_MIDASplayModuleSection@20';
function MIDASstopModule(playHandle : MIDASmodulePlayHandle) : boolean ;
    stdcall; external 'midas11.dll' name '_MIDASstopModule@4';
function MIDASfreeModule(module : MIDASmodule) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASfreeModule@4';

function MIDASgetPlayStatus(playHandle : MIDASmodulePlayHandle;
    status : PMIDASplayStatus) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASgetPlayStatus@8';
function MIDASsetPosition(playHandle : MIDASmodulePlayHandle;
    newPosition : integer) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetPosition@8';
function MIDASsetMusicVolume(playHandle : MIDASmodulePlayHandle;
    volume : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetMusicVolume@8';
function MIDASgetModuleInfo(module : MIDASmodule; info : PMIDASmoduleInfo) :
    boolean;
    stdcall; external 'midas11.dll' name '_MIDASgetModuleInfo@8';
function MIDASgetInstrumentInfo(module : MIDASmodule; instNum : integer;
    info : PMIDASinstrumentInfo) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASgetInstrumentInfo@12';
function MIDASfadeMusicChannel(playHandle : MIDASmodulePlayHandle; channel,
    fade : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASfadeMusicChannel@12';

function MIDASloadRawSample(fileName : PChar; sampleType,
    loopSample : integer) : MIDASsample;
    stdcall; external 'midas11.dll' name '_MIDASloadRawSample@12';
function MIDASloadWaveSample(fileName : PChar; loopSample : integer) :
    MIDASsample;
    stdcall; external 'midas11.dll' name '_MIDASloadWaveSample@8';
function MIDASfreeSample(sample : MIDASsample) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASfreeSample@4';
function MIDASallocAutoEffectChannels(numChannels : dword) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASallocAutoEffectChannels@4';
function MIDASfreeAutoEffectChannels : boolean;
    stdcall; external 'midas11.dll' name '_MIDASfreeAutoEffectChannels@0';
function MIDASplaySample(sample : midasSample; channel : dword;
    priority : integer; rate, volume : dword; panning : integer) :
    MIDASsamplePlayHandle;
    stdcall; external 'midas11.dll' name '_MIDASplaySample@24';
function MIDASstopSample(sample : MIDASsamplePlayHandle) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASstopSample@4';
function MIDASsetSampleRate(sample : MIDASsamplePlayHandle; rate : dword) :
    boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetSampleRate@8';
function MIDASsetSampleVolume(sample : MIDASsamplePlayHandle; volume : dword)
    : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetSampleVolume@8';
function MIDASsetSamplePanning(sample : MIDASsamplePlayHandle;
    panning : integer) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetSamplePanning@8';
function MIDASsetSamplePriority(sample : MIDASsamplePlayHandle;
    priority : integer) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetSamplePriority@8';
function MIDASgetSamplePlayStatus(sample : MIDASsamplePlayHandle) : dword;
    stdcall; external 'midas11.dll' name '_MIDASgetSamplePlayStatus@4';

function MIDASplayStreamFile(fileName : PChar;
    sampleType : dword; sampleRate : dword; bufferLength : dword;
    loopStream : integer) : MIDASstreamHandle;
    stdcall; external 'midas11.dll' name '_MIDASplayStreamFile@20';
function MIDASplayStreamWaveFile(fileName : PChar; bufferLength : dword;
    loopStream : integer) : MIDASstreamHandle;
    stdcall; external 'midas11.dll' name '_MIDASplayStreamWaveFile@12';
function MIDASstopStream(stream : MIDASstreamHandle) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASstopStream@4';

function MIDASplayStreamPolling(sampleType : dword;
    sampleRate : dword; bufferLength : dword) : MIDASstreamHandle;
    stdcall; external 'midas11.dll' name '_MIDASplayStreamPolling@12';
function MIDASfeedStreamData(stream : MIDASstreamHandle; data : Pbyte;
    numBytes : dword; feedAll : boolean) : dword;
    stdcall; external 'midas11.dll' name '_MIDASfeedStreamData@16';

function MIDASsetStreamRate(stream : MIDASstreamHandle; rate : dword)
    : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetStreamRate@8';
function MIDASsetStreamVolume(stream : MIDASstreamHandle; volume : dword)
    : boolean;
    stdcall; external 'midas11.dll' name '_MIDASsetStreamVolume@8';
function MIDASsetStreamPanning(stream : MIDASstreamHandle; panning : integer)
     : boolean;
     stdcall; external 'midas11.dll' name '_MIDASsetStreamPanning@8';

function MIDASgetStreamBytesBuffered(stream : MIDASstreamHandle) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASgetStreamBytesBuffered@4';
function MIDASpauseStream(stream : MIDASstreamHandle) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASpauseStream@4';
function MIDASresumeStream(stream : MIDASstreamHandle ) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASresumeStream@4';

function MIDASaddEchoEffect(echoSet : PMIDASechoSet) : MIDASechoHandle;
    stdcall; external 'midas11.dll' name '_MIDASaddEchoEffect@4';
function MIDASremoveEchoEffect(echoHandle : MIDASechoHandle) : boolean;
    stdcall; external 'midas11.dll' name '_MIDASremoveEchoEffect@4';



BEGIN
END.

{*
 * $Log: midasdll.pas,v $
 * Revision 1.5  1997/07/31 14:30:58  pekangas
 * Added option MIDAS_OPTION_FORCE_NO_SOUND
 *
 * Revision 1.4  1997/07/31 10:56:48  pekangas
 * Renamed from MIDAS Sound System to MIDAS Digital Audio System
 *
 * Revision 1.3  1997/07/29 16:57:12  pekangas
 * Added sample playing status query
 *
 * Revision 1.2  1997/07/29 16:51:09  pekangas
 * Updated for 1.1 beta 1
 *
 * Revision 1.1  1997/05/23 14:47:44  pekangas
 * Initial revision
 *
*}