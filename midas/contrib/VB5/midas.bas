Attribute VB_Name = "Module1"

'    { enum MIDASoptions }
Public Const MIDAS_OPTION_NONE = 0
Public Const MIDAS_OPTION_MIXRATE = 1
Public Const MIDAS_OPTION_OUTPUTMODE = 2
Public Const MIDAS_OPTION_MIXBUFLEN = 3
Public Const MIDAS_OPTION_MIXBUFBLOCKS = 4
Public Const MIDAS_OPTION_DSOUND_MODE = 5
Public Const MIDAS_OPTION_DSOUND_HWND = 6
Public Const MIDAS_OPTION_DSOUND_OBJECT = 7
Public Const MIDAS_OPTION_DSOUND_BUFLEN = 8
Public Const MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT = 9
Public Const MIDAS_OPTION_FILTER_MODE = 10
Public Const MIDAS_OPTION_MIXING_MODE = 11
Public Const MIDAS_OPTION_DEFAULT_STEREO_SEPARATION = 12
Public Const MIDAS_OPTION_FORCE_NO_SOUND = 13
  
'    { enum MIDASmodes }
Public Const MIDAS_MODE_NONE = 0
Public Const MIDAS_MODE_MONO = 1
Public Const MIDAS_MODE_STEREO = 2
Public Const MIDAS_MODE_8BIT = 4
Public Const MIDAS_MODE_16BIT = 8
Public Const MIDAS_MODE_8BIT_MONO = MIDAS_MODE_8BIT Or MIDAS_MODE_MONO
Public Const MIDAS_MODE_8BIT_STEREO = MIDAS_MODE_8BIT Or MIDAS_MODE_STEREO
Public Const MIDAS_MODE_16BIT_MONO = MIDAS_MODE_16BIT Or MIDAS_MODE_MONO
Public Const MIDAS_MODE_16BIT_STEREO = MIDAS_MODE_16BIT Or MIDAS_MODE_STEREO

'    { enum MIDASsampleTypes }
Public Const MIDAS_SAMPLE_NONE = 0
Public Const MIDAS_SAMPLE_8BIT_MONO = 1
Public Const MIDAS_SAMPLE_16BIT_MONO = 2
Public Const MIDAS_SAMPLE_8BIT_STEREO = 3
Public Const MIDAS_SAMPLE_16BIT_STEREO = 4
Public Const MIDAS_SAMPLE_ADPCM_MONO = 5
Public Const MIDAS_SAMPLE_ADPCM_STEREO = 6
Public Const MIDAS_SAMPLE_ULAW_MONO = 7
Public Const MIDAS_SAMPLE_ULAW_STEREO = 8
    
'    { enum MIDASloop }
Public Const MIDAS_LOOP_NO = 0
Public Const MIDAS_LOOP_YES = 1

'    { enum MIDASpanning }
Public Const MIDAS_PAN_LEFT = -64
Public Const MIDAS_PAN_MIDDLE = 0
Public Const MIDAS_PAN_RIGHT = 64
Public Const MIDAS_PAN_SURROUND = &H80

'   { enum MIDASchannels }
Public Const MIDAS_CHANNEL_AUTO = &HFFFF
Public Const MIDAS_ILLEGAL_CHANNEL = &HFFFF

'    { enum MIDASdsoundModes }
Public Const MIDAS_DSOUND_DISABLED = 0
Public Const MIDAS_DSOUND_STREAM = 1
Public Const MIDAS_DSOUND_PRIMARY = 2
Public Const MIDAS_DSOUND_FORCE_STREAM = 3

'    { enum MIDASpositions }
Public Const MIDAS_POSITION_DEFAULT = &HFFFFFFFF

'    { enum MIDASfilterModes }
Public Const MIDAS_FILTER_NONE = 0
Public Const MIDAS_FILTER_LESS = 1
Public Const MIDAS_FILTER_MORE = 2
Public Const MIDAS_FILTER_AUTO = 3

'    { enum MIDASechoFilterTypes }
Public Const MIDAS_ECHO_FILTER_NONE = 0
Public Const MIDAS_ECHO_FILTER_LOWPASS = 1
Public Const MIDAS_ECHO_FILTER_HIGHPASS = 2

'    { enum MIDASmixingModes }
Public Const MIDAS_MIX_NORMAL_QUALITY = 0
Public Const MIDAS_MIX_HIGH_QUALITY = 1

'    { enum MIDASsamplePlayStatus }
Public Const MIDAS_SAMPLE_STOPPED = 0
Public Const MIDAS_SAMPLE_PLAYING = 1

Type MIDASmoduleInfo
    songName As String * 32
    songLength As Long
    numPatterns As Long
    numInstruments As Long
    numChannels As Long
End Type
Public PMIDASmoduleInfo As MIDASmoduleInfo

Type MIDASinstrumentInfo
    instName As String * 32
End Type
Public PMIDASinstrumentInfo As MIDASinstrumentInfo

Type MIDASplayStatus
    position As Long
    pattern As Long
    row As Long
    syncInfo As Long
    songLoopCount As Long
End Type
Public PMIDASplayStatus As MIDASplayStatus

Type MIDASecho
    delay As Long
    gain As Long
    reverseChannels As Long
    filterType As Long
End Type
Public PMIDASecho As MIDASecho

Type MIDASechoSet
        feedback As Long
        gain As Long
        numEchoes As Long
        echoes As MIDASecho
End Type
Public PMIDASechoSet As MIDASechoSet

Declare Function MIDASgetLastError Lib "midas11.dll" Alias "_MIDASgetLastError@0" () As Long
Declare Function MIDASgetErrorMessage Lib "midas11.dll" Alias "_MIDASgetErrorMessage@4" (ByVal errorCode As Long) As String

Declare Function MIDASstartup Lib "midas11.dll" Alias "_MIDASstartup@0" () As Boolean
Declare Function MIDASconfig Lib "midas11.dll" Alias "_MIDASconfig@0" () As Boolean

Declare Function MIDASloadConfig Lib "midas11.dll" Alias "_MIDASloadConfig@4" (ByVal fileName As String) As Boolean
Declare Function MIDASsaveConfig Lib "midas11.dll" Alias "_MIDASsaveConfig@4" (ByVal fileName As String) As Boolean
Declare Function MIDASreadConfigRegistry Lib "midas11.dll" Alias "_MIDASreadConfigRegistry@8" (ByVal key As Long, ByVal subKey As String) As Boolean
Declare Function MIDASwriteConfigRegistry Lib "midas11.dll" Alias "_MIDASwriteConfigRegistry@8" (ByVal key As Long, ByVal subKey As String) As Boolean

Declare Function MIDASinit Lib "midas11.dll" Alias "_MIDASinit@0" () As Boolean
Declare Function MIDASsetOption Lib "midas11.dll" Alias "_MIDASsetOption@8" (ByVal soption As Long, ByVal value As Long) As Boolean
Declare Function MIDASgetOption Lib "midas11.dll" Alias "_MIDASgetOption@4" (ByVal soption As Long) As Long
Declare Function MIDASclose Lib "midas11.dll" Alias "_MIDASclose@0" () As Boolean
Declare Function MIDASsuspend Lib "midas11.dll" Alias "_MIDASclose@0" () As Boolean
Declare Function MIDASresume Lib "midas11.dll" Alias "_MIDASresume@0" () As Boolean
Declare Function MIDASopenChannels Lib "midas11.dll" Alias "_MIDASopenChannels@4" (ByVal numChannels As Long) As Boolean
Declare Function MIDAScloseChannels Lib "midas11.dll" Alias "_MIDAScloseChannels@0" () As Boolean
Declare Function MIDASsetAmplification Lib "midas11.dll" Alias "_MIDASsetAmplification@4" (ByVal amplification As Long) As Boolean
Declare Function MIDASstartBackgroundPlay Lib "midas11.dll" Alias "_MIDASstartBackgroundPlay@4" (ByVal pollRate As Long) As Boolean
Declare Function MIDASstopBackgroundPlay Lib "midas11.dll" Alias "_MIDASstopBackgroundPlay@0" () As Boolean
Declare Function MIDASpoll Lib "midas11.dll" Alias "_MIDASpoll@0" () As Boolean
Declare Function MIDASlock Lib "midas11.dll" Alias "_MIDASlock@0" () As Boolean
Declare Function MIDASunlock Lib "midas11.dll" Alias "_MIDASunlock@0" () As Boolean
Declare Function MIDASgetVersionString Lib "midas11.dll" Alias "_MIDASgetVersionString@0" () As String

Declare Function MIDASallocateChannel Lib "midas11.dll" Alias "_MIDASallocateChannel@0" () As Long
Declare Function MIDASfreeChannel Lib "midas11.dll" Alias "_MIDASfreeChannel@4" (ByVal channel As Long) As Boolean

Declare Function MIDASloadModule Lib "midas11.dll" Alias "_MIDASloadModule@4" (ByVal fileName As String) As Long
Declare Function MIDASplayModule Lib "midas11.dll" Alias "_MIDASplayModule@8" (ByVal module As Long, ByVal loopSong As Boolean) As Long
    
Declare Function MIDASplayModuleSection Lib "midas11.dll" Alias "_MIDASplayModuleSection@20" (ByVal module As Long, ByVal startPos As Long, ByVal endPos As Long, ByVal restartPos As Long, ByVal loopSong As Boolean) As Long
Declare Function MIDASstopModule Lib "midas11.dll" Alias "_MIDASstopModule@4" (ByVal playHandle As Long) As Boolean
    
Declare Function MIDASfreeModule Lib "midas11.dll" Alias "_MIDASfreeModule@4" (ByVal module As Long) As Boolean

Declare Function MIDASgetPlayStatus Lib "midas11.dll" Alias "_MIDASgetPlayStatus@8" (ByVal playHandle As Long, status As MIDASplayStatus) As Boolean
Declare Function MIDASsetPosition Lib "midas11.dll" Alias "_MIDASsetPosition@8" (ByVal playHandle As Long, ByVal newPosition As Long) As Boolean
Declare Function MIDASsetMusicVolume Lib "midas11.dll" Alias "_MIDASsetMusicVolume@8" (ByVal playHandle As Long, ByVal volume As Long) As Boolean
Declare Function MIDASgetModuleInfo Lib "midas11.dll" Alias "_MIDASgetModuleInfo@8" (ByVal module As Long, info As MIDASmoduleInfo) As Boolean
Declare Function MIDASgetInstrumentInfo Lib "midas11.dll" Alias "_MIDASgetInstrumentInfo@12" (ByVal module As Long, ByVal instNum As Long, info As MIDASinstrumentInfo) As Boolean
Declare Function MIDASfadeMusicChannel Lib "midas11.dll" Alias "_MIDASfadeMusicChannel@12" (ByVal playHandle As Long, ByVal channel As Long, ByVal fade As Long) As Boolean

Declare Function MIDASloadRawSample Lib "midas11.dll" Alias "_MIDASloadRawSample@12" (ByVal fileName As String, sampleType As Long, loopSample As Long) As Long
Declare Function MIDASloadWaveSample Lib "midas11.dll" Alias "_MIDASloadWaveSample@8" (ByVal fileName As String, ByVal loopSample As Long) As Long
Declare Function MIDASfreeSample Lib "midas11.dll" Alias "_MIDASfreeSample@4" (ByVal sample As Long) As Boolean
Declare Function MIDASallocAutoEffectChannels Lib "midas11.dll" Alias "_MIDASallocAutoEffectChannels@4" (ByVal numChannels As Long) As Boolean
Declare Function MIDASfreeAutoEffectChannels Lib "midas11.dll" Alias "_MIDASfreeAutoEffectChannels@0" () As Boolean
Declare Function MIDASplaySample Lib "midas11.dll" Alias "_MIDASplaySample@24" (ByVal sample As Long, ByVal channel As Long, ByVal priority As Long, ByVal rate As Long, ByVal volume As Long, ByVal panning As Long) As Long

Declare Function MIDASstopSample Lib "midas11.dll" Alias "_MIDASstopSample@4" (ByVal sample As Long) As Boolean
Declare Function MIDASsetSampleRate Lib "midas11.dll" Alias "_MIDASsetSampleRate@8" (ByVal sample As Long, ByVal rate As Long) As Boolean
Declare Function MIDASsetSampleVolume Lib "midas11.dll" Alias "_MIDASsetSampleVolume@8" (ByVal sample As Long, ByVal volume As Long) As Boolean
Declare Function MIDASsetSamplePanning Lib "midas11.dll" Alias "_MIDASsetSamplePanning@8" (ByVal sample As Long, ByVal panning As Long) As Boolean
Declare Function MIDASsetSamplePriority Lib "midas11.dll" Alias "_MIDASsetSamplePriority@8" (ByVal sample As Long, ByVal priority As Long) As Boolean
Declare Function MIDASgetSamplePlayStatus Lib "midas11.dll" Alias "_MIDASgetSamplePlayStatus@4" (ByVal sample As Long) As Long

Declare Function MIDASplayStreamFile Lib "midas11.dll" Alias "_MIDASplayStreamFile@20" (ByVal fileName As String, ByVal sampleType As Long, ByVal sampleRate As Long, ByVal bufferLength As Long, ByVal loopStream As Long) As Long
Declare Function MIDASplayStreamWaveFile Lib "midas11.dll" Alias "_MIDASplayStreamWaveFile@12" (ByVal fileName As String, ByVal bufferLength As Long, ByVal loopStream As Long) As Long
Declare Function MIDASstopStream Lib "midas11.dll" Alias "_MIDASstopStream@4" (ByVal stream As Long) As Boolean

Declare Function MIDASplayStreamPolling Lib "midas11.dll" Alias "_MIDASplayStreamPolling@12" (ByVal sampleType As Long, ByVal sampleRate As Long, ByVal bufferLength As Long) As Long
'not to sure about this one...
Declare Function MIDASfeedStreamData Lib "midas11.dll" Alias "_MIDASfeedStreamData@16" (ByVal stream As Long, ByVal data As Long, ByVal numBytes As Long, ByVal feedAll As Boolean) As Long

Declare Function MIDASsetStreamRate Lib "midas11.dll" Alias "_MIDASsetStreamRate@8" (ByVal stream As Long, ByVal rate As Long) As Boolean
Declare Function MIDASsetStreamVolume Lib "midas11.dll" Alias "_MIDASsetStreamVolume@8" (ByVal stream As Long, ByVal volume As Long) As Boolean
Declare Function MIDASsetStreamPanning Lib "midas11.dll" Alias "_MIDASsetStreamPanning@8" (ByVal stream As Long, ByVal panning As Long) As Boolean

Declare Function MIDASgetStreamBytesBuffered Lib "midas11.dll" Alias "_MIDASgetStreamBytesBuffered@4" (ByVal stream As Long) As Boolean

Declare Function MIDASpauseStream Lib "midas11.dll" Alias "_MIDASpauseStream@4" (ByVal stream As Long) As Boolean
Declare Function MIDASresumeStream Lib "midas11.dll" Alias "_MIDASresumeStream@4" (ByVal stream As Long) As Boolean

Declare Function MIDASaddEchoEffect Lib "midas11.dll" Alias "_MIDASaddEchoEffect@4" (echoSet As MIDASechoSet) As Long
Declare Function MIDASremoveEchoEffect Lib "midas11.dll" Alias "_MIDASremoveEchoEffect@4" (echoHandle As Long) As Boolean

