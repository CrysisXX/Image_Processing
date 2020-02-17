; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNewchh375View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "newchh375.h"
LastPage=0

ClassCount=7
Class1=CNewchh375App
Class2=CNewchh375Doc
Class3=CNewchh375View
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDD_HISTOGRAM
Class6=CDlgHistogram
Resource3=IDR_MAINFRAME
Class7=CDlg24Histogram
Resource4=IDD_24_HISTOGRAM

[CLS:CNewchh375App]
Type=0
HeaderFile=newchh375.h
ImplementationFile=newchh375.cpp
Filter=N
LastObject=CNewchh375App

[CLS:CNewchh375Doc]
Type=0
HeaderFile=newchh375Doc.h
ImplementationFile=newchh375Doc.cpp
Filter=N
LastObject=CNewchh375Doc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CNewchh375View]
Type=0
HeaderFile=newchh375View.h
ImplementationFile=newchh375View.cpp
Filter=C
LastObject=ID_24BUTTERWORTH_FILTER_HIGH
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=newchh375.cpp
ImplementationFile=newchh375.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=1
Control1=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_CLEAN
Command10=ID_GRAY
Command11=ID_GRAY_TRUE
Command12=ID_HISTOGRAM
Command13=ID_LINETRANS
Command14=ID_EQUALIZE
Command15=ID_FOURIER
Command16=ID_IFOURIER
Command17=ID_FFT
Command18=ID_IFFT
Command19=ID_AVERAGE_FILTER
Command20=ID_MEDIUM_FILTER
Command21=ID_GAUSS_FILTER
Command22=ID_LAPLACE_SHARPENING
Command23=ID_GRAD_SHARPENING
Command24=ID_PRIWITTSHARPENING
Command25=ID_IDEAL_LOW_FILTER
Command26=ID_IDEAL_HIGH_FILTER
Command27=ID_BUTTERWORTH_LOW_FILTER
Command28=ID_BUTTERWORTH_HIGH_FILTER
Command29=ID_24HISTOGRAM
Command30=ID_24LINETRANS
Command31=ID_24EQUALIZE
Command32=ID_24FFT
Command33=ID_24IFFT
Command34=ID_24AVERAGE_FILTER
Command35=ID_24MEDIUM_FILTER
Command36=ID_24GAUSS_FILTER
Command37=ID_24LAPLACE_SHARPENING
Command38=ID_24ROBERT_SHARPENING
Command39=ID_24PRIWITT_SHARPENING
Command40=ID_24IDEAL_FILTER_LOW
Command41=ID_24IDEAL_FILTER_HIGH
Command42=ID_24BUTTERWORTH_FILTER_LOW
Command43=ID_24BUTTERWORTH_FILTER_HIGH
CommandCount=43

[DLG:IDD_HISTOGRAM]
Type=1
Class=CDlgHistogram
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CDlgHistogram]
Type=0
HeaderFile=DlgHistogram.h
ImplementationFile=DlgHistogram.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHistogram
VirtualFilter=dWC

[DLG:IDD_24_HISTOGRAM]
Type=1
Class=CDlg24Histogram
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BLUE,static,1342312961
Control4=IDC_GREEN,static,1342312961
Control5=IDC_RED,static,1342312961

[CLS:CDlg24Histogram]
Type=0
HeaderFile=Dlg24Histogram.h
ImplementationFile=Dlg24Histogram.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlg24Histogram
VirtualFilter=dWC

