; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Algorithms.h"
LastPage=0

ClassCount=7
Class1=CAlgorithmsApp
Class2=CAlgorithmsDoc
Class3=CAlgorithmsView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_ALGORITYPE
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDD_ALGORITHMS_FORM
Class7=CShorthestPath
Resource5=IDD_DIALOG_SELECT_NODES

[CLS:CAlgorithmsApp]
Type=0
HeaderFile=Algorithms.h
ImplementationFile=Algorithms.cpp
Filter=N

[CLS:CAlgorithmsDoc]
Type=0
HeaderFile=AlgorithmsDoc.h
ImplementationFile=AlgorithmsDoc.cpp
Filter=N

[CLS:CAlgorithmsView]
Type=0
HeaderFile=AlgorithmsView.h
ImplementationFile=AlgorithmsView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=CAlgorithmsView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=ID_ADD_NODE


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=Algorithms.cpp
ImplementationFile=Algorithms.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_ADD_NODE
Command5=ID_ADD_EDGE
Command6=ID_SHORTEST_PATH
CommandCount=6

[MNU:IDR_ALGORITYPE]
Type=1
Class=CAlgorithmsView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

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

[DLG:IDD_ALGORITHMS_FORM]
Type=1
Class=CAlgorithmsView
ControlCount=1
Control1=IDC_DIJKSTRA1,{6F02AF52-EC0B-4188-A452-3F1E39F1B347},1342242816

[DLG:IDD_DIALOG_SELECT_NODES]
Type=1
Class=CShorthestPath
ControlCount=6
Control1=IDC_EDIT_NODE1,edit,1350631552
Control2=IDC_EDIT_NODE2,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CShorthestPath]
Type=0
HeaderFile=ShorthestPath.h
ImplementationFile=ShorthestPath.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

