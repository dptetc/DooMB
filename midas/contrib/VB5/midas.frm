VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "MIDAS VB Sample by Unlimited Realities"
   ClientHeight    =   4785
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6675
   LinkTopic       =   "Form1"
   ScaleHeight     =   4785
   ScaleWidth      =   6675
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox Picture2 
      Height          =   1215
      Left            =   120
      ScaleHeight     =   1155
      ScaleWidth      =   3075
      TabIndex        =   11
      Top             =   2880
      Width           =   3135
   End
   Begin VB.PictureBox Picture1 
      Height          =   1575
      Left            =   3480
      Picture         =   "midas.frx":0000
      ScaleHeight     =   1515
      ScaleWidth      =   3075
      TabIndex        =   9
      Top             =   2880
      Width           =   3135
   End
   Begin VB.DirListBox Dir2 
      Height          =   1665
      Left            =   3480
      TabIndex        =   8
      Top             =   1080
      Width           =   1695
   End
   Begin VB.DriveListBox Drive2 
      Height          =   315
      Left            =   3480
      TabIndex        =   7
      Top             =   720
      Width           =   1695
   End
   Begin VB.FileListBox File2 
      Height          =   2040
      Left            =   5160
      Pattern         =   "*.wav"
      TabIndex        =   6
      Top             =   720
      Width           =   1455
   End
   Begin VB.DirListBox Dir1 
      Height          =   1665
      Left            =   120
      TabIndex        =   3
      Top             =   1080
      Width           =   1695
   End
   Begin VB.DriveListBox Drive1 
      Height          =   315
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   1695
   End
   Begin VB.FileListBox File1 
      Height          =   2040
      Left            =   1800
      Pattern         =   "*.mod;*.xm"
      TabIndex        =   1
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Stop MOD"
      Height          =   465
      Left            =   1920
      TabIndex        =   0
      Top             =   4200
      Width           =   1350
   End
   Begin VB.Label Label2 
      Caption         =   "Click above to play the selected sample"
      Height          =   255
      Left            =   3480
      TabIndex        =   10
      Top             =   4560
      Width           =   3255
   End
   Begin VB.Line Line3 
      X1              =   3480
      X2              =   6600
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Line Line2 
      X1              =   120
      X2              =   3240
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Line Line1 
      X1              =   3360
      X2              =   3360
      Y1              =   0
      Y2              =   4680
   End
   Begin VB.Label Label1 
      Caption         =   "Sample Player"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   1
      Left            =   3480
      TabIndex        =   5
      Top             =   120
      Width           =   2535
   End
   Begin VB.Label Label1 
      Caption         =   "MOD Player"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   14.25
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   2535
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Dim module As Long
Dim playHandle As Long
Dim res As Boolean

Dim sample_handle As Long, tmp As Long


Private Sub Command2_Click()
 'stop the mod file
 If playHandle > 0 Then res = MIDASstopModule(playHandle)
 playHandle = 0
 If module > 0 Then res = MIDASfreeModule(module)
 module = 0
End Sub

Private Sub Dir1_Change()
File1.Path = Dir1.Path
End Sub

Private Sub Dir2_Change()
File2.Path = Dir2.Path
End Sub

Private Sub Drive1_Change()
Dir1.Path = Drive1.Drive
End Sub

Private Sub Drive2_Change()
Dir2.Path = Drive2.Drive
End Sub

Private Sub File1_Click()
'work out the filename
fn$ = File1.Path
If Right$(fn$, 1) <> "\" Then fn$ = fn$ & "\"
fn$ = fn$ & File1.fileName

'load the mod file
Dim tmp As Boolean
module = MIDASloadModule(fn$)
Dim mmi As MIDASmoduleInfo
tmp = MIDASgetModuleInfo(module, mmi)

'display module information
Picture2.Cls
Picture2.FontName = "courier new"
Picture2.FontBold = True
Picture2.Print till0(mmi.songName)
Picture2.FontBold = False
Picture2.Print "Length         : " & mmi.songLength
Picture2.Print "Patterns       : " & mmi.numPatterns
Picture2.Print "NumChannels    : " & mmi.numChannels
Picture2.Print "NumInstruments : " & mmi.numInstruments

u% = DoEvents
'play the module
If module > 0 Then playHandle = MIDASplayModule(module, True)
End Sub
Private Function till0(ins$) As String
    'return the string until a null char
    Dim tmp$
    Dim a%
    For a% = 1 To Len(ins$)
        If Asc(Mid$(ins$, a%, 1)) = 0 Then Exit For
        tmp$ = tmp$ & Mid$(ins$, a%, 1)
    Next
    till0 = tmp$
End Function
Private Sub File2_Click()
'work out the filename
fn$ = File2.Path
If Right$(fn$, 1) <> "\" Then fn$ = fn$ & "\"
fn$ = fn$ & File2.fileName

'if appropriate free old sample
If sample_handle > 0 Then tmp = MIDASfreeSample(sample_handle)
'load the sample
sample_handle = MIDASloadWaveSample(fn$, MIDAS_LOOP_NO)
End Sub

Private Sub Form_Load()
 'initialise MIDAS
 res = MIDASstartup()
 res = MIDASinit()
 res = MIDASstartBackgroundPlay(100)
 res = MIDASopenChannels(50)
  
End Sub

Private Sub Form_Unload(Cancel As Integer)
'free the samples
If sample_handle > 0 Then tmp = MIDASfreeSample(sample_handle)
'check if the mod file is stopped
If module > 0 Then Command2_Click
'free MIDAS
res = MIDAScloseChannels()
res = MIDASstopBackgroundPlay
res = MIDASclose
End Sub


Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
If sample_handle > 0 Then
    'play the sample, adjust the frequncy and pan based on position
    'tmp = MIDASplaySample(sample_handle, MIDAS_CHANNEL_AUTO, 0, 22050, 64, MIDAS_PAN_MIDDLE)
    Dim pan%, freq!
    pan% = (X - (Picture1.ScaleWidth / 2)) / Picture2.ScaleWidth * 128
    freq! = (Y / Picture1.ScaleHeight) * 33000 + 11000
    tmp = MIDASplaySample(sample_handle, 1, 0, freq!, 64, pan%)
End If
End Sub
