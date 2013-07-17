# Microsoft Developer Studio Project File - Name="ClothScroller" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ClothScroller - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ClothScroller.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ClothScroller.mak" CFG="ClothScroller - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ClothScroller - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ClothScroller - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ClothScroller - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /GR /GX /O2 /I "D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\GLUT\Include" /I "D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\Bass\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glut32.lib bass.lib /nologo /subsystem:windows /machine:I386 /libpath:"D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\GLUT\LIB" /libpath:"D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\Bass\LIB"

!ELSEIF  "$(CFG)" == "ClothScroller - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\GLUT\Include" /I "D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\Bass\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib glut32.lib bass.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\GLUT\LIB" /libpath:"D:\Application Data\Work\Development Projects\Visual Studio Additional Libraries\Bass\LIB"

!ENDIF 

# Begin Target

# Name "ClothScroller - Win32 Release"
# Name "ClothScroller - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "ClothScroller"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClothNodeNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerMain.cpp
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerModels.cpp
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerScenesCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollerManager.cpp
# End Source File
# End Group
# Begin Group "ClothScroller Support/General"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackgroundAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStreamServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLUTOpenGLInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelGeometryOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\PrimitiveSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneManagerMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneManagerOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneRepOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemSupport.cpp
# End Source File
# End Group
# Begin Group "ClothScroller Scenes (Source)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SceneScrollerFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerRisingCloth.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTapestry.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTeatheredCloth.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTrampoline.cpp
# End Source File
# End Group
# Begin Group "ClothScroller Nodes/Forces (Source)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExternalNodeForces.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysicalNode.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "ClothScroller Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClothNodeNetwork.h
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerDefines.h
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerModels.h
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerResources.h
# End Source File
# Begin Source File

SOURCE=.\ClothScrollerScenesCommon.h
# End Source File
# Begin Source File

SOURCE=.\ScrollerManager.h
# End Source File
# End Group
# Begin Group "ClothScroller Supp./Gen. Hdrs."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackgroundAudio.h
# End Source File
# Begin Source File

SOURCE=.\DataStreamServer.h
# End Source File
# Begin Source File

SOURCE=.\GLUTInstanceMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\GLUTOpenGLInstance.h
# End Source File
# Begin Source File

SOURCE=.\MemoryUtility.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeometry.h
# End Source File
# Begin Source File

SOURCE=.\ModelGeometryOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\PrimaryHeaders.h
# End Source File
# Begin Source File

SOURCE=.\PrimitiveSupport.h
# End Source File
# Begin Source File

SOURCE=.\RenderSupport.h
# End Source File
# Begin Source File

SOURCE=.\SceneManagerMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\SceneManagerOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\SceneRepOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\SystemSupport.h
# End Source File
# End Group
# Begin Group "ClothScroller Scenes (Headers)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClothScrollerScenes.h
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerFlag.h
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerRisingCloth.h
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTapestry.h
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTeatheredCloth.h
# End Source File
# Begin Source File

SOURCE=.\SceneScrollerTrampoline.h
# End Source File
# End Group
# Begin Group "ClothScroller Nodes/Forces (Hdrs.)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExternalNodeForces.h
# End Source File
# Begin Source File

SOURCE=.\PhysicalNode.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
