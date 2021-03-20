strTargetDir = Session.Property("CustomActionData")
'strTargetDir = "C:\Program Files\Rohde-Schwarz\Test\"

Set wshShell = CreateObject("WScript.Shell")

'Remove installation directory of the agent from the path environment variable.
Call RemovePath( wshShell, strTargetDir )

'-------------------------------------------------------------------------------
Sub RemovePath( i_wshShell, i_strPath )
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

    'MsgBox "TargetDir = " & strPath
    'MsgBox "PrcPath = " & strPrcPath

    If Mid(strPath,iStrLenPath,1) = "\" Then
        strPath = Left( strPath, iStrLenPath-1 )
        iStrLenPath = Len(strPath)
    End If
    If Mid(strPath,iStrLenPath,1) = "/" Then
        strPath = Left( strPath, iStrLenPath-1 )
        iStrLenPath = Len(strPath)
    End If
    If Mid(strPath,iStrLenPath,1) = ";" Then
        strPath = Left( strPath, iStrLenPath-1 )
        iStrLenPath = Len(strPath)
    End If

    iStrPosPath = InStr(1,strPrcPath,strPath,1)

    'MsgBox "TargetDir = " & strPath
    'MsgBox "PrcPath = " & strPrcPath

    If iStrPosPath > 0 Then
        strPrcPath = Replace(strPrcPath,strPath,"")
        iStrLenPrcPath = Len(strPrcPath)
        If Mid(strPrcPath,iStrLenPrcPath,1) = ";" Then
            strPrcPath = Left( strPrcPath, iStrLenPrcPath-1 )
            iStrLenPrcPath = Len(strPrcPath)
        End If
        'MsgBox "New PrcPath = " & strPrcPath
        env.item("PATH") = strPrcPath
        'MsgBox "PrcPath = " & env.item("PATH")
    End If

End Sub
