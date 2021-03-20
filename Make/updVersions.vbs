Include "const.vbs"
Include "getVersionString.vbs"
Include "setVersion.vbs"

WScript.Quit(updVersions)

'===============================================================================
Function updVersions
'
' Reads version strings from header files and updates the corresponding version
' strings in resource file and setup projekts if the version string has been
' changed. If the version changed also the product and package codes within the
' setup projekts will be updated with new GUID's.
' The result of calling the "getVersion" and "setVersion" calls will be reported
' in the "error.log" file.
'
' Return Values:
'   SET_VERSION_RESULT_OK                        = 0
'   SET_VERSION_RESULT_FILE_NOT_FOUND            = 1
'   SET_VERSION_RESULT_FILE_IS_READ_ONLY         = 2
'   SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED = 3
'
'===============================================================================

    Dim strVersionFile
    Dim strZSQtLibVersion
    Dim iZSQtLibGetVersionResult
    Dim iSetVersionResultTmp
    Dim iSetVersionResult

    iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK
    iSetVersionResult = SET_VERSION_RESULT_OK

    Set wshShell = CreateObject("WScript.Shell")
    Set fso = CreateObject("Scripting.FileSystemObject")

    If fso.FileExists("UpdVersion.log") Then
        Set tsErrLog = fso.OpenTextFile("UpdVersion.log",FILE_IOMODE_WRITE)
        tsErrLog.Close
    End If

    strVersionDef = "#define ZSSysVersionNr"
    strVersionFile = "..\ZSSys\Include\ZSSysVersion.h"
    strZSQtLibVersion = GetVersionString( fso, strVersionFile, strVersionDef, 1 )
    If Len(strZSQtLibVersion) = 0 Then
        iZSQtLibGetVersionResult = SET_VERSION_RESULT_FILE_NOT_FOUND
        iSetVersionResult = SET_VERSION_RESULT_FILE_NOT_FOUND
        Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing or version definition string '" & strVersionDef & "' not found" )
    End If

    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        Call WriteErrLog( fso, "Setting environment variable ZSQTLIBVERSION to '" & strZSQtLibVersion & "'" )
        wshShell.Environment.item("ZSQTLIBVERSION") = strZSQtLibVersion
    End If

    'ZSQtLib - Libraries
    '===========================================================================

    'ZSSys
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSSys\Make\Windows\ZSSys.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'QMake Project File
    strVersionFile = "..\ZSSys\Make\Linux\ZSSys.pro"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSIpc
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSIpc\Make\Windows\ZSIpc.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'QMake Project File
    strVersionFile = "..\ZSIpc\Make\Linux\ZSIpc.pro"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSTrace
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSTrace\Make\Windows\ZSTrace.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'QMake Project File
    strVersionFile = "..\ZSTrace\Make\Linux\ZSTrace.pro"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSPhysVal
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSPhysVal\Make\Windows\ZSPhysVal.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'QMake Project File
    strVersionFile = "..\ZSPhysVal\Make\Linux\ZSPhysVal.pro"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSPhysSizes\Electricity
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSPhysSizes\Electricity\Make\Windows\ZSPhysSizesElectricity.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSPhysSizes\Geometry
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSPhysSizes\Geometry\Make\Windows\ZSPhysSizesGeometry.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSPhysSizes\Kinematics
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSPhysSizes\Kinematics\Make\Windows\ZSPhysSizesKinematics.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSDiagram
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSDiagram\Make\Windows\ZSDiagram.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSDataSet
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSDataSet\Make\Windows\ZSDataSet.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSSNMP
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSSNMP\Make\Windows\ZSSNMP.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'QMake Project File
    strVersionFile = "..\ZSSNMP\Make\Linux\ZSSNMP.pro"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSQtLib - Applications
    '===========================================================================

    'ZSAppExamples\ZSAppDataSet
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppExamples\ZSAppDataSet\Make\Windows\ZSAppDataSet.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppExamples\ZSAppDiagram
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppExamples\ZSAppDiagram\Make\Windows\ZSAppDiagram.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppExamples\ZSAppPhysVal
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppExamples\ZSAppPhysVal\Make\Windows\ZSAppPhysVal.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppExamples\ZSAppTrcServer
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppExamples\ZSAppTrcServer\Make\Windows\ZSAppTrcServer.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppCmdClient
    '---------------------------------------------------------------------------

    If 0 Then 'Not yet implemented. Should be renamed to RemCtrl. Missing basic functionality.

        'Resource File
        strVersionFile = "..\ZSAppCmdClient\Make\Windows\ZSAppCmdClient.rc"
        If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
            Select Case iSetVersionResultTmp
                Case SET_VERSION_RESULT_FILE_NOT_FOUND
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
                Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
                Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
                Case Else
                    Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
            End Select
            If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
                iSetVersionResult = iSetVersionResultTmp
            End If
        End If

        'Windows Setup File
        strVersionFile = "..\ZSAppCmdClient\Make\Windows\ZSAppCmdClientSetup_8.0.vdproj"
        If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
            Select Case iSetVersionResultTmp
                Case SET_VERSION_RESULT_FILE_NOT_FOUND
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
                Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
                Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                    Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
                Case Else
                    Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
            End Select
            If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
                iSetVersionResult = iSetVersionResultTmp
            End If
        End If

    End If

    'ZSAppTrcCtxClient
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTrcCtxClient\Make\Windows\ZSAppTrcCtxClient.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Windows Setup File
    strVersionFile = "..\ZSAppTrcCtxClient\Make\Windows\ZSAppTrcCtxClientSetup_8.0.vdproj"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppTrcMsgClient
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTrcMsgClient\Make\Windows\ZSAppTrcMsgClient.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Windows Setup File
    strVersionFile = "..\ZSAppTrcMsgClient\Make\Windows\ZSAppTrcMsgClientSetup_8.0.vdproj"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppTrcMthClient
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTrcMthClient\Make\Windows\ZSAppTrcMthClient.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Windows Setup File
    strVersionFile = "..\ZSAppTrcMthClient\Make\Windows\ZSAppTrcMthClientSetup_8.0.vdproj"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppSNMPExplorer
    '===========================================================================

    'Resource File
    strVersionFile = "..\ZSAppSNMPExplorer\Make\Windows\ZSAppSNMPExplorer.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Windows Setup File
    strVersionFile = "..\ZSAppSNMPExplorer\Make\Windows\ZSAppSNMPExplorerSetup_8.0.vdproj"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'ZSAppTMI
    '===========================================================================

    'Common
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTMI\Common\Make\Windows\ZSTMICommon.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Database
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTMI\Database\Make\Windows\ZSTMIDatabase.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'FSWCapture
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTMI\FSWCapture\Make\Windows\ZSTMIFSWCapture.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'FSWSigGen
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTMI\FSWSigGen\Make\Windows\ZSTMIFSWSigGen.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Application (GUI)
    '---------------------------------------------------------------------------

    'Resource File
    strVersionFile = "..\ZSAppTMI\GUI\Make\Windows\ZSAppTMI.rc"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Windows Setup File
    strVersionFile = "..\ZSAppTMI\GUI\Make\Windows\ZSAppTMISetup_8.0.vdproj"
    If iZSQtLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, strZSQtLibVersion, "" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    'Linux build and install/uninstall scripts
    '===========================================================================

    'build_linux_all.cmd: ZSQtLibVersion
    strVersionFile = "build_linux_all.cmd"
    If iSMLibGetVersionResult = SET_VERSION_RESULT_OK Then
        iSetVersionResultTmp = SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_BUILD, strVersionFile, strZSQtLibVersion, "ZSQtLibVersion" )
        Select Case iSetVersionResultTmp
            Case SET_VERSION_RESULT_FILE_NOT_FOUND
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is not existing" )
            Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' is read only" )
            Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
                Call WriteErrLog( fso, "Error: File '" & strVersionFile & "' with unknown content" )
            Case Else
                Call WriteErrLog( fso, "ZSQtLibVersion '" & strZSQtLibVersion & "' successfully set in File '" & strVersionFile & "'" )
        End Select
        If Not iSetVersionResultTmp = SET_VERSION_RESULT_OK And iSetVersionResult = SET_VERSION_RESULT_OK Then
            iSetVersionResult = iSetVersionResultTmp
        End If
    End If

    updVersions = iSetVersionResult

End Function

'===============================================================================
Sub WriteErrLog( ByRef i_fso, ByRef i_strErrMsg )
'-------------------------------------------------------------------------------
'
' Appends the specified error message to the error log file "error.log".
'
' Example:
'   WriteErrLog( "File is not existing" )
'
' Parameters:
'   i_strErrMsg (IN)
'     Error message to be appended to the error log file.
'
'===============================================================================

    If i_fso.FileExists("UpdVersion.log") Then
        Set tsErrLog = i_fso.OpenTextFile("UpdVersion.log",FILE_IOMODE_APPEND)
    Else
        Set tsErrLog = i_fso.CreateTextFile("UpdVersion.log")
    End If
    tsErrLog.WriteLine(i_strErrMsg)
    tsErrLog.Close

End Sub

'===============================================================================
Sub Include( ByRef i_strVBSFileName )
'-------------------------------------------------------------------------------
'
' Call this method to include the definitions, functions and sub routines of
' other vb-script files.
'
' Example:
'   Include( "const.vbs" )
'   Include( "getVersionString.vbs" )
'
' Parameters:
'   i_strVBSFileName (IN)
'     Name of the VB script file to be included.
'
'===============================================================================

    Set fso = CreateObject("Scripting.FileSystemObject")
    Set ts = fso.OpenTextFile(i_strVBSFileName)
    ExecuteGlobal ts.ReadAll()
    ts.Close
End Sub
