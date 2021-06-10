VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmReceiptPrinter 
   Caption         =   "Receipt Printer Emulator"
   ClientHeight    =   4500
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8190
   LinkTopic       =   "Form1"
   ScaleHeight     =   4500
   ScaleWidth      =   8190
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Caption         =   "SetBitmap Command"
      Height          =   4215
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7935
      Begin VB.Frame Frame3 
         Height          =   2655
         Left            =   3000
         TabIndex        =   20
         Top             =   1320
         Width           =   3615
         Begin VB.TextBox txtMask 
            Alignment       =   2  'Center
            Height          =   285
            Left            =   2880
            TabIndex        =   27
            Text            =   "XX"
            Top             =   1200
            Width           =   495
         End
         Begin VB.CommandButton cmdQuickSetFileMask 
            Caption         =   "Quick Set with File Mask"
            Height          =   375
            Left            =   240
            TabIndex        =   23
            Top             =   2040
            Width           =   3135
         End
         Begin VB.TextBox txtFromIndex2 
            Alignment       =   2  'Center
            Height          =   285
            Left            =   1080
            TabIndex        =   22
            Text            =   "1"
            Top             =   1200
            Width           =   495
         End
         Begin VB.TextBox txtToIndex2 
            Alignment       =   2  'Center
            Height          =   285
            Left            =   1080
            TabIndex        =   21
            Text            =   "20"
            Top             =   1560
            Width           =   495
         End
         Begin VB.Label Label1 
            Alignment       =   1  'Right Justify
            Caption         =   "Replace Mask:"
            Height          =   255
            Index           =   9
            Left            =   1680
            TabIndex        =   28
            Top             =   1215
            Width           =   1095
         End
         Begin VB.Label Label3 
            Caption         =   $"frmReceiptPrinter.frx":0000
            Height          =   855
            Left            =   120
            TabIndex        =   26
            Top             =   240
            Width           =   3375
         End
         Begin VB.Label Label1 
            Alignment       =   1  'Right Justify
            Caption         =   "From Index:"
            Height          =   255
            Index           =   8
            Left            =   120
            TabIndex        =   25
            Top             =   1215
            Width           =   855
         End
         Begin VB.Label Label1 
            Alignment       =   1  'Right Justify
            Caption         =   "To Index:"
            Height          =   255
            Index           =   7
            Left            =   120
            TabIndex        =   24
            Top             =   1575
            Width           =   855
         End
      End
      Begin VB.Frame Frame2 
         Height          =   2655
         Left            =   120
         TabIndex        =   13
         Top             =   1320
         Width           =   2535
         Begin VB.TextBox txtToIndex 
            Alignment       =   2  'Center
            Height          =   285
            Left            =   1080
            TabIndex        =   18
            Text            =   "20"
            Top             =   1560
            Width           =   495
         End
         Begin VB.TextBox txtFromIndex 
            Alignment       =   2  'Center
            Height          =   285
            Left            =   1080
            TabIndex        =   16
            Text            =   "1"
            Top             =   1200
            Width           =   495
         End
         Begin VB.CommandButton cmdQuickSetSameFile 
            Caption         =   "Quick Set with Same File"
            Height          =   375
            Left            =   240
            TabIndex        =   14
            Top             =   2040
            Width           =   2055
         End
         Begin VB.Label Label1 
            Alignment       =   1  'Right Justify
            Caption         =   "To Index:"
            Height          =   255
            Index           =   6
            Left            =   120
            TabIndex        =   19
            Top             =   1575
            Width           =   855
         End
         Begin VB.Label Label1 
            Alignment       =   1  'Right Justify
            Caption         =   "From Index:"
            Height          =   255
            Index           =   5
            Left            =   120
            TabIndex        =   17
            Top             =   1215
            Width           =   855
         End
         Begin VB.Label Label2 
            Caption         =   "Use Quick Set with Same File to make mutliple SetBitmap calls with the values above for the specified range:"
            Height          =   855
            Left            =   120
            TabIndex        =   15
            Top             =   240
            Width           =   2295
         End
      End
      Begin VB.TextBox txtAlign 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   6360
         TabIndex        =   11
         Text            =   "-2"
         Top             =   525
         Width           =   615
      End
      Begin VB.TextBox txtStation 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   840
         TabIndex        =   9
         Text            =   "2"
         Top             =   525
         Width           =   495
      End
      Begin VB.TextBox txtWidth 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   5520
         TabIndex        =   7
         Text            =   "-11"
         Top             =   525
         Width           =   615
      End
      Begin VB.CommandButton cmdSetBitmap 
         Caption         =   "Go!"
         Height          =   375
         Left            =   7080
         TabIndex        =   6
         Top             =   480
         Width           =   735
      End
      Begin VB.TextBox txtIndex 
         Alignment       =   2  'Center
         Height          =   285
         Left            =   120
         TabIndex        =   3
         Text            =   "1"
         Top             =   525
         Width           =   495
      End
      Begin MSComDlg.CommonDialog cdBitmapFile 
         Left            =   3000
         Top             =   240
         _ExtentX        =   847
         _ExtentY        =   847
         _Version        =   393216
         DialogTitle     =   "Bitmap File"
         Filter          =   "*.*|*.*"
         InitDir         =   "C:\"
      End
      Begin VB.CommandButton cmdBrowse 
         Caption         =   "Browse..."
         Height          =   255
         Left            =   4080
         TabIndex        =   2
         Top             =   960
         Width           =   1215
      End
      Begin VB.TextBox txtBitmapFile 
         Height          =   285
         Left            =   1560
         TabIndex        =   1
         Top             =   525
         Width           =   3735
      End
      Begin VB.Label Label1 
         Caption         =   "Alignment:"
         Height          =   255
         Index           =   4
         Left            =   6360
         TabIndex        =   12
         Top             =   240
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "Station:"
         Height          =   255
         Index           =   3
         Left            =   720
         TabIndex        =   10
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label1 
         Caption         =   "Width:"
         Height          =   255
         Index           =   2
         Left            =   5520
         TabIndex        =   8
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label1 
         Caption         =   "Index:"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   5
         Top             =   240
         Width           =   975
      End
      Begin VB.Label Label1 
         Caption         =   "Bitmap File:"
         Height          =   255
         Index           =   0
         Left            =   1560
         TabIndex        =   4
         Top             =   240
         Width           =   975
      End
   End
End
Attribute VB_Name = "frmReceiptPrinter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------------------------------------------------------
' Printer Emulator for ACS
'
' Author: Steve Antonakakis
'         20070713
'
' History:
'          07/13/2007 - Initial Release (SetPrinter command)
'
'----------------------------------------------------------------
Option Explicit

Public Enum PrinterFunction
    PTR_FUNCTION_CUTPAPER = 1
    PTR_FUNCTION_PRINT = 2
    PTR_FUNCTION_BARCODE = 3
    PTR_FUNCTION_BITMAP = 4
    PTR_FUNCTION_SETBITMAP = 5
    PTR_FUNCTION_SETLOGO = 6
    PTR_FUNCTION_SLIPSTATUS = 7
End Enum


Private oPrinter As CPrinterObj

Private Sub cmdBrowse_Click()
    cdBitmapFile.ShowOpen
    txtBitmapFile.Text = cdBitmapFile.FileName
End Sub

Private Sub cmdQuickSetFileMask_Click()
On Error GoTo E_cmdQuickSetFileMask_Click
    'Build and send the printer message - allow empty text and bad values
    'for unit testing applications
    Dim iStart As Long: iStart = CLng(txtFromIndex2.Text)
    Dim iEnd As Long: iEnd = CLng(txtToIndex2.Text)
    Dim i As Long
    Dim sFile As String
    
    For i = iStart To iEnd
        oPrinter.Height = 0
        oPrinter.Width = CInt(txtWidth.Text)
        oPrinter.TextPosition = 0
        oPrinter.Alignment = SignedValToByte(txtAlign.Text)
        oPrinter.StationId = CInt(txtStation.Text)
        oPrinter.BarcodeType = 1
        oPrinter.BarcodeCharacterPrint = 0
        oPrinter.BitmapLogoNumber = i
        sFile = Replace(txtBitmapFile.Text, txtMask.Text, CStr(i), , , vbTextCompare)
        oPrinter.Data = sFile
        oPrinter.Arguments = ""
        oPrinter.Function = PrinterFunction.PTR_FUNCTION_SETBITMAP
        frmKeyBoard.otbc1.Send oPrinter
    Next
   
Exit Sub
E_cmdQuickSetFileMask_Click:
    MsgBox Err.Description, vbCritical
End Sub

Private Function SignedValToByte(sVal As String)
    'calculate the alignment in unsigned format in
    'case it's negative to prevent an overflow
    Const SIZEOFBYTE = 256
    Dim iVal As Integer: iVal = CInt(sVal)
    iVal = iVal Mod SIZEOFBYTE
    If iVal < 0 Then
        iVal = iVal + SIZEOFBYTE  'sizeof a byte
    End If
    SignedValToByte = CByte(iVal)
End Function


Private Sub cmdQuickSetSameFile_Click()
On Error GoTo E_cmdQuickSetSameFile_Click
    'Build and send the printer message - allow empty text and bad values
    'for unit testing applications
    Dim iStart As Long: iStart = CLng(txtFromIndex.Text)
    Dim iEnd As Long: iEnd = CLng(txtToIndex.Text)
    Dim i As Long
    
    For i = iStart To iEnd
        oPrinter.Height = 0
        oPrinter.Width = CInt(txtWidth.Text)
        oPrinter.TextPosition = 0
        oPrinter.Alignment = SignedValToByte(txtAlign.Text)
        oPrinter.StationId = CInt(txtStation.Text)
        oPrinter.BarcodeType = 1
        oPrinter.BarcodeCharacterPrint = 0
        oPrinter.BitmapLogoNumber = i
        oPrinter.Data = txtBitmapFile.Text
        oPrinter.Arguments = ""
        oPrinter.Function = PrinterFunction.PTR_FUNCTION_SETBITMAP
        frmKeyBoard.otbc1.Send oPrinter
    Next
   
Exit Sub
E_cmdQuickSetSameFile_Click:
    MsgBox Err.Description, vbCritical
End Sub

Private Sub cmdSetBitmap_Click()
On Error GoTo E_cmdSetBitmap_Click
    'Build and send the printer message - allow empty text and bad values
    'for unit testing applications
    oPrinter.Height = 0
    oPrinter.TextPosition = 0
    oPrinter.BarcodeType = 1
    oPrinter.BarcodeCharacterPrint = 0
    oPrinter.Width = CInt(txtWidth.Text)
    oPrinter.Alignment = SignedValToByte(txtAlign.Text)
    oPrinter.StationId = CInt(txtStation.Text)
    oPrinter.BitmapLogoNumber = CInt(txtIndex.Text)
    oPrinter.Data = txtBitmapFile.Text
    oPrinter.Arguments = ""
    oPrinter.Function = PrinterFunction.PTR_FUNCTION_SETBITMAP
    
    frmKeyBoard.otbc1.Send oPrinter
Exit Sub
E_cmdSetBitmap_Click:
    MsgBox Err.Description, vbCritical
End Sub

Private Sub Form_Load()
    Set oPrinter = New CPrinterObj
End Sub

Private Sub Form_Unload(Cancel As Integer)
    frmKeyBoard.miReceiptPrinter.Checked = False
    Set oPrinter = Nothing
End Sub

