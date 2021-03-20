'-------------------------------------------------------------------------------
'For testing the function:
'
'Const FILE_IOMODE_READ     =    1 '0000 0000 0000 0001
'
'Dim strZSQtLibVersion
'
'Set fso = CreateObject("Scripting.FileSystemObject")
'strZSQtLibVersion = GetVersionString( "..\ZSQtLib\ZSSys\Include\ZSSysVersion.h", "#define ZSQtLibVersion", 1 )
'MsgBox "ZSQtLibVersion = " & strZSQtLibVersion
'-------------------------------------------------------------------------------

'===============================================================================
Function GetVersionString( ByRef i_fso, ByRef i_strFileName, ByRef i_strVersionDef, ByVal i_bTouchVersionFile )
'-------------------------------------------------------------------------------
'
' Searches the specified file for the given version definition string and
' returns the version string.
'
' Example:
'   Dim strZSQtLibVersion
'   Set fso = CreateObject("Scripting.FileSystemObject")
'   strZSQtLibVersion = GetVersionString( fso, "ZSSysVersion.h", "#define ZSQtLibVersion", 1 )
'
' Parameters:
'   i_fso (IN)
'     File system object that must have been created before with the call
'     Set fso = CreateObject("Scripting.FileSystemObject")
'   i_strFileName (IN)
'     Name of the file that should be scanned for the version string.
'   i_strVersionDef (IN)
'     String the constant containing the version string has been defined with.
'   i_bTouchVersionFile (IN)
'     Set this parameter to 1 to change the date of the version file. For this
'     the file will be read and written. Useful if all files indicating the
'     version string should be recompiled to add e.g. the current compile time
'     to the version string.
'
' Return Value:
'   Found version string (e.g. "4.2.2").
'
'===============================================================================

    Dim ts
    Dim strVersion
    Dim iStrPos
    Dim iStrPosVersionStart
    Dim iStrPosVersionEnd

    If Len(i_strFileName) = 0 Then
        WScript.Echo "GetVersionString: ArgOutOfRange: File name missing"
    Else
        If Not (i_fso.FileExists(i_strFileName)) Then
            WScript.Echo "GetVersionString: ArgOutOfRange: The File '" & i_strFileName & "' Is Not Existing"
        Else
            Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_READ)
            iStrLenVersionDef = Len(i_strVersionDef)
            Do While Not ts.AtEndOfStream
                strLine = ts.ReadLine
                iStrLenLine = Len(strLine)
                If InStr( 1, strLine, i_strVersionDef ) > 0 Then
                    If iStrLenLine > iStrLenVersionDef + 5 Then
                        iStrPosVersionStart = InStr( iStrLenVersionDef+1, strLine, """" )
                        iStrPosVersionEnd = InStr( iStrPosVersionStart+1, strLine, """" )
                        If iStrPosVersionEnd > iStrPosVersionStart+1 Then
                            strVersion = Mid(strLine,iStrPosVersionStart+1,iStrPosVersionEnd-iStrPosVersionStart-1)
                            Exit Do
                        End If
                    End If
                End If
            Loop
            ts.Close

            If i_bTouchVersionFile = 1 Then
                If i_fso.GetFile(i_strFileName).Attributes And FILE_ATTR_READONLY Then
                    WScript.Echo "GetVersionString: ArgOutOfRange: The File '" & i_strFileName & "' Is Read Only"
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

                    Set ts = i_fso.OpenTextFile(i_strFileName,FILE_IOMODE_WRITE)
                    For iLineNr = 1 to iLineCount
                        ts.WriteLine( arstrLines(iLineNr) )
                    Next
                    ts.Close
                End If
            End If
        End If
    End If

    GetVersionString = strVersion

End Function 'GetVersionString
