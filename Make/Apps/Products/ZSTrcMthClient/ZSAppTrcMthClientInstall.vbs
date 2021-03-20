strTargetDir = Session.Property("CustomActionData")
'strTargetDir = "C:\Program Files\Rohde-Schwarz\Test\"

Set wshShell = CreateObject("WScript.Shell")

'Add installation directory of the application to the path environment variable so that
'software components are able to dynamically load the dlls of the trace subsystem.
Call AddPath( wshShell, strTargetDir )


'-------------------------------------------------------------------------------
Sub AddPath( i_wshShell, i_strPath )
'-------------------------------------------------------------------------------

    Dim strPath
    Dim iStrLenPath
    Dim strPrcPath
    Dim iStrLenPrcPath

    Set env = i_wshShell.Environment("SYSTEM")

    strPath = i_strPath
    iStrLenPath = Len(strPath)

    strPrcPath = env.item("PATH")
    iStrLenPrcPath = Len(strPrcPath)

    If iStrLenPath > 0 Then
        If Mid(strPath,iStrLenPath,1) = ";" Then
            strPath = Left( strPath, iStrLenPath-1 )
            iStrLenPath = Len(strPath)
        End If
        If Mid(strPath,iStrLenPath,1) = "\" Then
            strPath = Left( strPath, iStrLenPath-1 )
            iStrLenPath = Len(strPath)
        End If
        If Mid(strPath,iStrLenPath,1) = "/" Then
            strPath = Left( strPath, iStrLenPath-1 )
            iStrLenPath = Len(strPath)
        End If
    End If

    iStrPosAddPath = InStr(1,strPrcPath,strPath,1)
    bAppend = 0

    'If the additional path is not existing in the current path ..
    If iStrPosAddPath = 0 Then
        bAppend = 1

    'If the additional path is existing in the current path ..
    Else
        If iStrPosAddPath > 0 Then

            'Get position within current path after found path string
            iStrPosNext = iStrPosAddPath + iStrLenPath

            'If position is within current process path ..
            Do While iStrPosNext <= iStrLenPrcPath
                'If the next character a delimiter of paths ...
                If Mid(strPrcPath,iStrPosNext,1) = ";" Then
                    '.. the path is already existing and don't need to be added.
                    bAppend = 0
                    Exit Do
                Else 
                    '.. the found path sub string is longer than the path to be added.
                    'But the path may still appear again on in the current path ..
                    bAppend = 1
                End If 
                'Check whether the path to be added appears again in the current path ..
                iStrPosAddPath = InStr(iStrPosNext,strPrcPath,strPath,1)
                'If the path appears again ..
                If iStrPosAddPath > 0 Then 
                    'Get position within current path after found path string
                    iStrPosNext = iStrPosAddPath + iStrLenPath
                    'At end of current path ..
                    If iStrPosNext = iStrLenPrcPath + 1 Then
                        bAppend = 0
                        Exit Do
                    End If
                'If the path does not appear again ..
                Else
                    Exit Do
                End If
            Loop
        End If
    End If

    'If the new path need to be appended ..
    If bAppend = 1 Then
        If strPrcPath = "" Then
            strTmpPath = strPath
        Else
            strTmpPath = strPrcPath & ";" & strPath
        End If
        env.item("PATH") = strTmpPath
        'Takes no effect for HKEY_LOCAL_MACHINE parameters !!!
        'i_wshShell.Run "%windir%\System32\RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters", 1, True
    End If

End Sub
