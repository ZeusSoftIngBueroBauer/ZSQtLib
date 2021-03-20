Const SET_VERSION_FILE_TYPE_WIN_RC            = 1
Const SET_VERSION_FILE_TYPE_WIN_VDPROJ        = 2
Const SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO    = 3
Const SET_VERSION_FILE_TYPE_LINUX_BUILD       = 4
Const SET_VERSION_FILE_TYPE_LINUX_INSTALL     = 5
Const SET_VERSION_FILE_TYPE_LINUX_INITSERVICE = 6

Const SET_VERSION_RESULT_OK                         = 0
Const SET_VERSION_RESULT_FILE_NOT_FOUND             = 1
Const SET_VERSION_RESULT_FILE_IS_READ_ONLY          = 2
Const SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED  = 3


'-------------------------------------------------------------------------------
'For testing the function:
'
'Const FILE_IOMODE_READ     =    1 '0000 0000 0000 0001
'Const FILE_IOMODE_WRITE    =    2 '0000 0000 0000 0020
'
'Const FILE_ATTR_READONLY   =    1 '0000 0000 0000 0001
'
'Set fso = CreateObject("Scripting.FileSystemObject")
'
'strVersionFile = "..\ZSQtLib\ZSSys\ZSSys.rc", "1.2.3"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, strVersionFile, "" )
'
'strVersionFile = "..\ZSQtLib\ZSAppTrcMthClient\Make\Windows\ZSAppTrcMthClientSetup_8.0.vdproj"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_VDPROJ, strVersionFile, "1.2.3", "" )
'
'strVersionFile = "..\ZSQtLib\ZSSys\Make\Linux\ZSSys.pro"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, "4.2.2", "" )
'
'strVersionFile = "..\SMLibBase\Make\Linux\SMLibBase.pro"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO, strVersionFile, "1.7.1", "" )
'
'strVersionFile = "build_linux_all.cmd"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_BUILD, strVersionFile, "4.2.2", "ZSQtLibVersion" )
'
'strVersionFile = "build_linux_all.cmd"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_BUILD, strVersionFile, "1.7.1", "SMVersion" )
'
'strVersionFile = "..\SMAppAgent\Make\Linux\Setup\install_agent"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_INSTALL, strVersionFile, "4.2.2", "ZSQtLibVersion" )
'
'strVersionFile = "..\SMAppAgent\Make\Linux\Setup\install_agent"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_INSTALL, strVersionFile, "1.7.1", "SMVersion" )
'
'strVersionFile = "..\SMAppAgent\Make\Linux\Setup\smagentinit"
'iResult =  SetVersion( fso, SET_VERSION_FILE_TYPE_LINUX_INITSERVICE, strVersionFile, "1.7.1", "" )
'
'Select Case iResult
'    Case SET_VERSION_RESULT_FILE_NOT_FOUND
'        WScript.Echo "File '" & strVersionFile & "' is not existing"
'    Case SET_VERSION_RESULT_FILE_IS_READ_ONLY
'        WScript.Echo "File '" & strVersionFile & "' is read only"
'    Case SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
'        WScript.Echo "File '" & strVersionFile & "' with unknown content"
'    Case Else
'End Select
'-------------------------------------------------------------------------------


'===============================================================================
Function SetVersion( ByRef i_fso, ByVal i_fileType, ByRef i_strFileName, ByRef i_strVersion, ByRef i_strVersionSpec )
'-------------------------------------------------------------------------------
'
' Sets the version number within the file by replacing the corresponding
' version definitions.
'
' Example:
'   Set fso = CreateObject("Scripting.FileSystemObject")
'   SetVersion( fso, SET_VERSION_FILE_TYPE_WIN_RC, "ZSSys.rc", "4.2.2" )
'
' Parameters:
'   i_fso (IN)
'     File system object that must have been created before with the call
'     Set fso = CreateObject("Scripting.FileSystemObject")
'   i_fileType (IN)
'     Range [SET_VERSION_FILE_TYPE_WIN_RC=1, SET_VERSION_FILE_TYPE_WIN_VDPROJ=2, SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO=3]
'     Which version definitions need to be replaced depend on the file type.
'   i_strFileName (IN)
'     Name of the file in which the version numbers should be replaced.
'   i_strVersion (IN)
'     Version number to be set. Only three digits are supported. The digits
'     must be delimited with a ".".
'   i_strVersionSpec (IN)
'     If the file contains multiple version strings the version specifier is used
'     as an additional search and compare string to decide which version number
'     should be set. If the file contains multiple version strings the VersionSpec
'     parameter is not optional. E.g. if the file contains both "ZSQtLibVersion = 4.2.1"
'     and "SMVersion = 1.6.1" and you want to set the version number of "SMVersion" you
'     need to pass "SMVersion" as the version specifier.
'     The version specifier is needed for following file types:
'     - SET_VERSION_FILE_TYPE_LINUX_BUILD
'     - SET_VERSION_FILE_TYPE_LINUX_INSTALL
'     For all other file types an empty string need to be passed to the function.
'
' Return Values:
'   SET_VERSION_RESULT_OK
'   SET_VERSION_RESULT_FILE_NOT_FOUND
'   SET_VERSION_RESULT_FILE_IS_READ_ONLY
'   SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
'
'===============================================================================

    iResult = SET_VERSION_RESULT_OK

    arstrVersionNumbers = Split( i_strVersion, "." )

    If UBound(arstrVersionNumbers) < 2 Then
        iArrLenOld = UBound(arstrVersionNumbers)
        Redim Preserve arstrVersionNumbers(2)
        For i = iArrLenOld+1 To UBound(arstrVersionNumbers)
            arstrVersionNumbers(i) = "0"
        Next
    ElseIf UBound(arstrVersionNumbers) > 2 Then
        Redim Preserve arstrVersionNumbers(2)
    End If

    If Len(i_strFileName) = 0 Then
        iResult = SET_VERSION_RESULT_FILE_NOT_FOUND
    Else
        If Not (i_fso.FileExists(i_strFileName)) Then
            iResult = SET_VERSION_RESULT_FILE_NOT_FOUND
        Else
            Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_READ)
            iLineCount = 0
            Do While Not ts.AtEndOfStream
                strLine = ts.ReadLine
                iLineCount = iLineCount + 1
            Loop
            ts.Close

            ReDim arstrLines(iLineCount)
            Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_READ)
            For iLineNr = 1 to iLineCount
                arstrLines(iLineNr) = ts.ReadLine
            Next
            ts.Close

            bCorrectFormat = 0

            Select Case i_fileType

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_WIN_RC
                '---------------------------------------------------------------

                    Const FILE_TYPE_WIN_RC_SUB_VERSION_INFO_DEF      = "VS_VERSION_INFO VERSIONINFO"
                    Const FILE_TYPE_WIN_RC_FILE_VERSION_DEF          = "FILEVERSION"
                    Const FILE_TYPE_WIN_RC_PRODUCT_VERSION_DEF       = "PRODUCTVERSION"
                    Const FILE_TYPE_WIN_RC_VALUE_FILE_VERSION_DEF    = "VALUE ""FileVersion"","
                    Const FILE_TYPE_WIN_RC_VALUE_PRODUCT_VERSION_DEF = "VALUE ""ProductVersion"","

                    iLineNrSubVersionInfo = 0
                    iLineNrFileVersionDef = 0
                    iLineNrProductVersionDef = 0
                    iLineNrValueFileVersionDef = 0
                    iLineNrValueProductVersionDef = 0

                    'Read line by line and store line numbers in which the version numbers are defined.
                    '----------------------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, FILE_TYPE_WIN_RC_SUB_VERSION_INFO_DEF ) > 0 Then
                            iLineNrSubVersionInfo = iLineNr
                        End If
                        If iLineNrSubVersionInfo > 0 Then
                            If InStr( 1, strLine, FILE_TYPE_WIN_RC_FILE_VERSION_DEF ) > 0 Then
                                iLineNrFileVersionDef = iLineNr
                            End If
                        End If
                        If iLineNrFileVersionDef > 0 Then
                            If InStr( 1, strLine, FILE_TYPE_WIN_RC_PRODUCT_VERSION_DEF ) > 0 Then
                                iLineNrProductVersionDef = iLineNr
                            End If
                        End If
                        If iLineNrProductVersionDef > 0 Then
                            If InStr( 1, strLine, FILE_TYPE_WIN_RC_VALUE_FILE_VERSION_DEF ) > 0 Then
                                iLineNrValueFileVersionDef = iLineNr
                            End If
                        End If
                        If iLineNrValueFileVersionDef > 0 Then
                            If InStr( 1, strLine, FILE_TYPE_WIN_RC_VALUE_PRODUCT_VERSION_DEF ) > 0 Then
                                iLineNrValueProductVersionDef = iLineNr
                            End If
                        End If
                        If iLineNrSubVersionInfo > 0 _
                        And iLineNrFileVersionDef > 0 _
                        And iLineNrProductVersionDef > 0 _
                        And iLineNrValueFileVersionDef > 0 _
                        And iLineNrValueProductVersionDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version numbers are defined in the expected format ..
                    If bCorrectFormat = 1 Then

                        'Replace version definition string
                        '---------------------------------

                        iStrLenLine = Len(arstrLines(iLineNrFileVersionDef))

                        iStrLenVersionDef = Len(FILE_TYPE_WIN_RC_FILE_VERSION_DEF)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrFileVersionDef), FILE_TYPE_WIN_RC_FILE_VERSION_DEF )
                        iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                        strVersionOld = Mid( arstrLines(iLineNrFileVersionDef), iStrPosVersionDefEnd+1, iStrLenLine )
                        strVersionOld = Replace( strVersionOld, " ", "" )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        For i = 0 to UBound(arstrVersionNumbers)
                            strVersionNew = strVersionNew & arstrVersionNumbers(i) & ","
                        Next
                        strVersionNew = strVersionNew & "0"

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrFileVersionDef), 1, iStrPosVersionDefStart+iStrLenVersionDef )
                            strLineNew = strLineNew & strVersionNew
                            arstrLines(iLineNrFileVersionDef) = strLineNew

                            'Replace product version definition string
                            '-----------------------------------------

                            iStrLenLine = Len(arstrLines(iLineNrProductVersionDef))
                            iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrProductVersionDef), FILE_TYPE_WIN_RC_PRODUCT_VERSION_DEF )
                            iStrLenVersionDef = Len(FILE_TYPE_WIN_RC_PRODUCT_VERSION_DEF)
                            strLineNew = Mid( arstrLines(iLineNrProductVersionDef), 1, iStrPosVersionDefStart+iStrLenVersionDef )
                            'Create new version number in the desired format:
                            strVersionNew = ""
                            For i = 0 to UBound(arstrVersionNumbers)
                                strVersionNew = strVersionNew & arstrVersionNumbers(i) & ","
                            Next
                            strVersionNew = strVersionNew & "0"
                            strLineNew = strLineNew & strVersionNew
                            arstrLines(iLineNrProductVersionDef) = strLineNew

                            'Replace value for version definition
                            '------------------------------------

                            iStrLenLine = Len(arstrLines(iLineNrValueFileVersionDef))
                            iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrValueFileVersionDef), FILE_TYPE_WIN_RC_VALUE_FILE_VERSION_DEF )
                            iStrLenVersionDef = Len(FILE_TYPE_WIN_RC_VALUE_FILE_VERSION_DEF)
                            strLineNew = Mid( arstrLines(iLineNrValueFileVersionDef), 1, iStrPosVersionDefStart+iStrLenVersionDef )
                            'Create new version number in the desired format:
                            strVersionNew = """"
                            For i = 0 to UBound(arstrVersionNumbers)
                                strVersionNew = strVersionNew & arstrVersionNumbers(i) & ", "
                            Next
                            strVersionNew = strVersionNew & "0"""
                            strLineNew = strLineNew & strVersionNew
                            arstrLines(iLineNrValueFileVersionDef) = strLineNew

                            'Replace value for product version
                            '---------------------------------

                            iStrLenLine = Len(arstrLines(iLineNrValueProductVersionDef))
                            iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrValueProductVersionDef), FILE_TYPE_WIN_RC_VALUE_PRODUCT_VERSION_DEF )
                            iStrLenVersionDef = Len(FILE_TYPE_WIN_RC_VALUE_PRODUCT_VERSION_DEF)
                            strLineNew = Mid( arstrLines(iLineNrValueProductVersionDef), 1, iStrPosVersionDefStart+iStrLenVersionDef )

                            'Create new version number in the desired format:
                            strVersionNew = """"
                            For i = 0 to UBound(arstrVersionNumbers)
                                strVersionNew = strVersionNew & arstrVersionNumbers(i) & ", "
                            Next
                            strVersionNew = strVersionNew & "0"""
                            strLineNew = strLineNew & strVersionNew
                            arstrLines(iLineNrValueProductVersionDef) = strLineNew

                            'Write updated resource file
                            '---------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_WIN_VDPROJ
                '---------------------------------------------------------------

                    Const FILE_TYPE_WIN_VDPROJ_PRODUCT_VERSION_DEF = "ProductVersion"" = "
                    Const FILE_TYPE_WIN_VDPROJ_PRODUCT_CODE_DEF    = "ProductCode"" = "
                    Const FILE_TYPE_WIN_VDPROJ_PACKAGE_CODE_DEF    = "PackageCode"" = "

                    iLineNrProductVersionDef = 0
                    iLineNrProductCodeDef = 0
                    iLineNrPackageCodeDef = 0

                    'Read line by line and store line numbers in which the version number,
                    'the product and the package codes are defined.
                    '---------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, FILE_TYPE_WIN_VDPROJ_PRODUCT_VERSION_DEF ) > 0 Then
                            iLineNrProductVersionDef = iLineNr
                        End If
                        If InStr( 1, strLine, FILE_TYPE_WIN_VDPROJ_PRODUCT_CODE_DEF ) > 0 Then
                            iLineNrProductCodeDef = iLineNr
                        End If
                        If InStr( 1, strLine, FILE_TYPE_WIN_VDPROJ_PACKAGE_CODE_DEF ) > 0 Then
                            iLineNrPackageCodeDef = iLineNr
                        End If
                        If iLineNrProductVersionDef > 0 _
                        And iLineNrProductCodeDef > 0 _
                        And iLineNrPackageCodeDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version number, the product and the package codes are defined
                    'in the expected format ..
                    If bCorrectFormat = 1 Then

                        '.. read the old version number.
                        iStrLenLine = Len(arstrLines(iLineNrProductVersionDef))

                        iStrLenVersionDef = Len(FILE_TYPE_WIN_VDPROJ_PRODUCT_VERSION_DEF)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrProductVersionDef), FILE_TYPE_WIN_VDPROJ_PRODUCT_VERSION_DEF )
                        iStrPosVersionDefEnd = InStr( iStrPosVersionDefStart+iStrLenVersionDef, arstrLines(iLineNrProductVersionDef), ":" )

                        strVersionOld = Mid( arstrLines(iLineNrProductVersionDef), iStrPosVersionDefEnd+1, iStrLenLine-iStrPosVersionDefEnd-1 )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        For i = 0 to UBound(arstrVersionNumbers)-1
                            strVersionNew = strVersionNew & arstrVersionNumbers(i) & "."
                        Next
                        strVersionNew = strVersionNew & arstrVersionNumbers(i)

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrProductVersionDef), 1, iStrPosVersionDefEnd )
                            strLineNew = strLineNew & strVersionNew & """"

                            arstrLines(iLineNrProductVersionDef) = strLineNew

                            'Create and set new product code
                            '-------------------------------

                            iStrLenProductCodeDef = Len(FILE_TYPE_WIN_VDPROJ_PRODUCT_CODE_DEF)
                            iStrPosProductCodeDefStart = InStr( 1, arstrLines(iLineNrProductCodeDef), FILE_TYPE_WIN_VDPROJ_PRODUCT_CODE_DEF )
                            iStrPosProductCodeDefEnd = InStr( iStrPosProductCodeDefStart+iStrLenProductCodeDef, arstrLines(iLineNrProductCodeDef), ":" )

                            strLineNew = Mid( arstrLines(iLineNrProductCodeDef), 1, iStrPosProductCodeDefEnd )
                            strLineNew = strLineNew & CreateGUID & """"

                            arstrLines(iLineNrProductCodeDef) = strLineNew

                            'Create and set new package code
                            '-------------------------------

                            iStrLenPackageCodeDef = Len(FILE_TYPE_WIN_VDPROJ_PACKAGE_CODE_DEF)
                            iStrPosPackageCodeDefStart = InStr( 1, arstrLines(iLineNrPackageCodeDef), FILE_TYPE_WIN_VDPROJ_PACKAGE_CODE_DEF )
                            iStrPosPackageCodeDefEnd = InStr( iStrPosPackageCodeDefStart+iStrLenPackageCodeDef, arstrLines(iLineNrPackageCodeDef), ":" )

                            strLineNew = Mid( arstrLines(iLineNrPackageCodeDef), 1, iStrPosPackageCodeDefEnd )

                            strLineNew = strLineNew & CreateGUID & """"

                            arstrLines(iLineNrPackageCodeDef) = strLineNew

                            'Write updated project file
                            '--------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_LINUX_QMAKEPRO
                '---------------------------------------------------------------

                    Const FILE_TYPE_LINUX_QMAKEPRO_VERSION_DEF = "VERSION = "

                    iLineNrVersionDef = 0

                    'Read line by line and store line numbers in which the version numbers are defined.
                    '----------------------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, FILE_TYPE_LINUX_QMAKEPRO_VERSION_DEF ) > 0 Then
                            iLineNrVersionDef = iLineNr
                        End If
                        If iLineNrVersionDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version numbers are defined in the expected format ..
                    If bCorrectFormat = 1 Then

                        'Replace version definition string
                        '---------------------------------

                        iStrLenLine = Len(arstrLines(iLineNrVersionDef))

                        iStrLenVersionDef = Len(FILE_TYPE_LINUX_QMAKEPRO_VERSION_DEF)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionDef), FILE_TYPE_LINUX_QMAKEPRO_VERSION_DEF )
                        iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                        strVersionOld = Mid( arstrLines(iLineNrVersionDef), iStrPosVersionDefEnd, iStrLenLine )
                        strVersionOld = Replace( strVersionOld, " ", "" )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        strVersionNew = strVersionNew & arstrVersionNumbers(0) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(1) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(2)

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrVersionDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & strVersionNew

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionDef) = strLineNew

                            'Write updated resource file
                            '---------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_LINUX_BUILD
                '---------------------------------------------------------------

                    Const FILE_TYPE_LINUX_BUILD_ZSQTLIBVERSION_DEF = "ZSQtLibVersion="
                    Const FILE_TYPE_LINUX_BUILD_SMVERSION_DEF = "SMVersion="

                    If i_strVersionSpec = "ZSQtLibVersion" Then
                        strVersionDef = FILE_TYPE_LINUX_BUILD_ZSQTLIBVERSION_DEF
                    Else
                        strVersionDef = FILE_TYPE_LINUX_BUILD_SMVERSION_DEF
                    End If

                    iLineNrVersionDef = 0

                    'Read line by line and store line numbers in which the version numbers are defined.
                    '----------------------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, strVersionDef ) > 0 Then
                            iLineNrVersionDef = iLineNr
                        End If
                        If iLineNrVersionDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version numbers are defined in the expected format ..
                    If bCorrectFormat = 1 Then

                        'Replace version definition string
                        '---------------------------------

                        iStrLenLine = Len(arstrLines(iLineNrVersionDef))

                        iStrLenVersionDef = Len(strVersionDef)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionDef), strVersionDef )
                        iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                        strVersionOld = Mid( arstrLines(iLineNrVersionDef), iStrPosVersionDefEnd, iStrLenLine )
                        strVersionOld = Replace( strVersionOld, " ", "" )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        strVersionNew = strVersionNew & arstrVersionNumbers(0) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(1) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(2)

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & ", " & i_strVersionSpec & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrVersionDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & strVersionNew

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionDef) = strLineNew

                            'Write updated resource file
                            '---------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_LINUX_INSTALL
                '---------------------------------------------------------------

                    Const FILE_TYPE_LINUX_INSTALL_ZSQTLIBVERSION_DEF = "ZSQtLibVersion"
                    Const FILE_TYPE_LINUX_INSTALL_SMVERSION_DEF = "SMVersion"

                    If i_strVersionSpec = "ZSQtLibVersion" Then
                        strVersionDef      = FILE_TYPE_LINUX_INSTALL_ZSQTLIBVERSION_DEF & "="
                        strVersionMinorDef = FILE_TYPE_LINUX_INSTALL_ZSQTLIBVERSION_DEF & "Minor="
                        strVersionMajorDef = FILE_TYPE_LINUX_INSTALL_ZSQTLIBVERSION_DEF & "Major="
                    Else
                        strVersionDef      = FILE_TYPE_LINUX_INSTALL_SMVERSION_DEF & "="
                        strVersionMinorDef = FILE_TYPE_LINUX_INSTALL_SMVERSION_DEF & "Minor="
                        strVersionMajorDef = FILE_TYPE_LINUX_INSTALL_SMVERSION_DEF & "Major="
                    End If

                    iLineNrVersionDef = 0
                    iLineNrVersionMinorDef = 0
                    iLineNrVersionMajorDef = 0

                    'Read line by line and store line numbers in which the version numbers are defined.
                    '----------------------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, strVersionDef ) > 0 Then
                            iLineNrVersionDef = iLineNr
                        End If
                        If iLineNrVersionDef > 0 Then
                            If InStr( 1, strLine, strVersionMinorDef ) > 0 Then
                                iLineNrVersionMinorDef = iLineNr
                            End If
                        End If
                        If iLineNrVersionMinorDef > 0 Then
                            If InStr( 1, strLine, strVersionMajorDef ) > 0 Then
                                iLineNrVersionMajorDef = iLineNr
                            End If
                        End If
                        If iLineNrVersionDef > 0 _
                        And iLineNrVersionMinorDef > 0 _
                        And iLineNrVersionMajorDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version numbers are defined in the expected format ..
                    If bCorrectFormat = 1 Then

                        'Replace version definition string
                        '---------------------------------

                        iStrLenLine = Len(arstrLines(iLineNrVersionDef))

                        iStrLenVersionDef = Len(strVersionDef)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionDef), strVersionDef )
                        iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                        strVersionOld = Mid( arstrLines(iLineNrVersionDef), iStrPosVersionDefEnd, iStrLenLine )
                        strVersionOld = Replace( strVersionOld, " ", "" )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        strVersionNew = strVersionNew & arstrVersionNumbers(0) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(1) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(2)

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & ", " & i_strVersionSpec & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrVersionDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & strVersionNew

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionDef) = strLineNew

                            'Replace minor version definition string
                            '---------------------------------------

                            iStrLenLine = Len(arstrLines(iLineNrVersionMinorDef))

                            iStrLenVersionDef = Len(strVersionMinorDef)
                            iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionMinorDef), strVersionMinorDef )
                            iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                            'Create new version number in the desired format:
                            strVersionNew = ""
                            strVersionNew = strVersionNew & arstrVersionNumbers(0) & "."
                            strVersionNew = strVersionNew & arstrVersionNumbers(1)

                            strLineNew = Mid( arstrLines(iLineNrVersionMinorDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & strVersionNew

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionMinorDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionMinorDef) = strLineNew

                            'Replace major version definition string
                            '---------------------------------------

                            iStrLenLine = Len(arstrLines(iLineNrVersionMajorDef))

                            iStrLenVersionDef = Len(strVersionMajorDef)
                            iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionMajorDef), strVersionMajorDef )
                            iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                            'Create new version number in the desired format:
                            strVersionNew = ""
                            strVersionNew = strVersionNew & arstrVersionNumbers(0)

                            strLineNew = Mid( arstrLines(iLineNrVersionMajorDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & strVersionNew

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionMajorDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionMajorDef) = strLineNew

                            'Write updated resource file
                            '---------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                '---------------------------------------------------------------
                Case SET_VERSION_FILE_TYPE_LINUX_INITSERVICE
                '---------------------------------------------------------------

                    Const FILE_TYPE_LINUX_INITSERVICE_DEF = "DESC=""R&S System Management Agent "

                    strVersionDef = FILE_TYPE_LINUX_INITSERVICE_DEF

                    iLineNrVersionDef = 0

                    'Read line by line and store line numbers in which the version numbers are defined.
                    '----------------------------------------------------------------------------------

                    For iLineNr = 1 to iLineCount
                        strLine = arstrLines(iLineNr)
                        iStrLenLine = Len(strLine)
                        If InStr( 1, strLine, strVersionDef ) > 0 Then
                            iLineNrVersionDef = iLineNr
                        End If
                        If iLineNrVersionDef > 0 Then
                            bCorrectFormat = 1
                            Exit For
                        End If
                    Next

                    'If the version numbers are defined in the expected format ..
                    If bCorrectFormat = 1 Then

                        'Replace version definition string
                        '---------------------------------

                        iStrLenLine = Len(arstrLines(iLineNrVersionDef))

                        iStrLenVersionDef = Len(strVersionDef)
                        iStrPosVersionDefStart = InStr( 1, arstrLines(iLineNrVersionDef), strVersionDef )
                        iStrPosVersionDefEnd = iStrPosVersionDefStart+iStrLenVersionDef

                        strVersionOld = Mid( arstrLines(iLineNrVersionDef), iStrPosVersionDefEnd, iStrLenLine )
                        strVersionOld = Replace( strVersionOld, " ", "" )
                        strVersionOld = Replace( strVersionOld, """", "" )
                        strVersionOld = Replace( strVersionOld, "(", "" )
                        strVersionOld = Replace( strVersionOld, ")", "" )

                        'Create new version number in the desired format:
                        strVersionNew = ""
                        strVersionNew = strVersionNew & arstrVersionNumbers(0) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(1) & "."
                        strVersionNew = strVersionNew & arstrVersionNumbers(2)

                        'WScript.Echo "SetVersion( " & i_strFileName & ", " & i_strVersion & ", " & i_strVersionSpec & " )"
                        'WScript.Echo "VersionOld = '" & strVersionOld & "'"
                        'WScript.Echo "VersionNew = '" & strVersionNew & "'"

                        If strVersionNew <> strVersionOld Then

                            strLineNew = Mid( arstrLines(iLineNrVersionDef), 1, iStrPosVersionDefEnd-1 )
                            strLineNew = strLineNew & "(" & strVersionNew & ")"""

                            'WScript.Echo "LineOld = '" & arstrLines(iLineNrVersionDef) & "'"
                            'WScript.Echo "LineNew = '" & strLineNew & "'"

                            arstrLines(iLineNrVersionDef) = strLineNew

                            'Write updated resource file
                            '---------------------------

                            If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                                iResult = SET_VERSION_RESULT_FILE_IS_READ_ONLY
                            Else
                                Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                                For iLineNr = 1 to iLineCount
                                    ts.WriteLine( arstrLines(iLineNr) )
                                Next
                                ts.Close
                            End If
                        End If 'If strVersionNew <> strVersionOld Then
                    End If 'If bCorrectFormat = 1 Then

                Case Else

            End Select

            If bCorrectFormat = 0 Then
                iResult = SET_VERSION_RESULT_FILE_FORMAT_NOT_SUPPORTED
            End If

        End If 'If (i_fso.FileExists(i_strFileName)) Then
    End If 'If Len(i_strFileName) = 0 Then

    SetVersion = iResult

End Function 'SetVersion

'===============================================================================
Function CreateGUID
'
' Creates a GUID in string format.
'
' Example:
'   strGUI = CreateGUID
'
' Return Values:
'   GUID in string format
'
'===============================================================================

    Set TypeLib = CreateObject("Scriptlet.TypeLib")
    strGUID = Replace(TypeLib.Guid, Chr(0), "")
    iPosEnd = InStr( 1, strGUID, "}" )
    strGUID = Left( strGUID, iPosEnd )
    CreateGUID = strGUID

End Function
