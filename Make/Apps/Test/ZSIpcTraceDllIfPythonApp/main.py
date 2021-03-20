#*********************************************************************************
#
# Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
#                          Gewerbepark 28
#                          D-83670 Bad Heilbrunn
#                          Tel: 0049 8046 9488
#                          www.zeussoft.de
#                          E-Mail: mailbox@zeussoft.de
#
#---------------------------------------------------------------------------------
#
# Content: This file is part of the ZSQtLib.
#
# This file may be used with no license restrictions for your needs. But it is not
# allowed to resell any modules of the ZSQtLib veiling the original developer of
# the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
# be removed from the header of the source code modules.
#
# ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
# that the code is written without faults.
#
# ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
# may result in using the software modules.
#
#*********************************************************************************

import sys
sys.path.append('C:/Projekte/ZeusSoft/ZSQtLib/Make/Libs/ZSIpcTracePy')
sys.path.append('C:/Projekte/ZeusSoft/ZSQtLib/Bin/msvc2015_x64')
sys.path.append('C:/Projekte/ZeusSoft/ZSQtLib/Apps/Test/ZSIpcTraceDllIfPythonApp')
sys.path.append('C:/Projekte/ZeusSoft/ZSQtLib/Tmp/Out/Libs/ZSIpcTracePy/Release_msvc2015/x64')
import re

from zsipctrace import *
from server import *

if __name__ == "__main__":

    trcServer = None

    testOk = True

    testStep = "Test Step: Get Default File Paths"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    orgName = "ZeusSoft"
    appName = "ZSAppTestIpcTraceDllIfPythonApp"
    adminObjFileAbsFilePath = ""
    localTrcFileAbsFilePath = ""
    try:
        CIpcTrcServer.SetOrganizationName(orgName)
        CIpcTrcServer.SetApplicationName(appName)
        orgNameTmp = CIpcTrcServer.GetOrganizationName();
        if orgNameTmp != orgName:
            testStepOk = False
        appNameTmp = CIpcTrcServer.GetApplicationName();
        if appNameTmp != appName:
            testStepOk = False
        adminObjFileAbsFilePath, localTrcFileAbsFilePath = CIpcTrcServer.GetDefaultFilePaths()
        if re.search(orgName, adminObjFileAbsFilePath) is None:
            testStepOk = False
        if re.search(appName, adminObjFileAbsFilePath) is None:
            testStepOk = False
        if re.search("TrcMthAdmObj.xml", adminObjFileAbsFilePath) is None:
            testStepOk = False
        if re.search(orgName, localTrcFileAbsFilePath) is None:
            testStepOk = False
        if re.search(appName, localTrcFileAbsFilePath) is None:
            testStepOk = False
        if re.search("TrcMth.log", localTrcFileAbsFilePath) is None:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Create Instance"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    trcServer = None
    try:
        trcServer = CIpcTrcServer.CreateInstance("ZSTrcServer") #, ETraceDetailLevelVerbose)
        if trcServer is None:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    try:
        trcServerNone = CIpcTrcServer.GetInstance("ZSTrcServer")
        if trcServerNone is not trcServer:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileCloseFileAfterEachWrite"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    closeFileAfterEachWrite = False
    try:
        if( trcServer is not None):
            closeFileAfterEachWrite = trcServer.getLocalTrcFileCloseFileAfterEachWrite()
            if closeFileAfterEachWrite: # Initial value should be False
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setLocalTrcFileCloseFileAfterEachWrite(True)
            closeFileAfterEachWrite = trcServer.getLocalTrcFileCloseFileAfterEachWrite()
            if not closeFileAfterEachWrite:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setLocalTrcFileCloseFileAfterEachWrite(False)
            closeFileAfterEachWrite = trcServer.getLocalTrcFileCloseFileAfterEachWrite()
            if closeFileAfterEachWrite:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: CacheTrcDataIfNotConnected"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    cacheTrcDataIfNotConnected = False
    try:
        if( trcServer is not None):
            cacheTrcDataIfNotConnected = trcServer.getCacheTrcDataIfNotConnected()
            if cacheTrcDataIfNotConnected: # Initial value should be False
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setCacheTrcDataIfNotConnected(True)
            cacheTrcDataIfNotConnected = trcServer.getCacheTrcDataIfNotConnected()
            if not cacheTrcDataIfNotConnected:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setCacheTrcDataIfNotConnected(False)
            cacheTrcDataIfNotConnected = trcServer.getCacheTrcDataIfNotConnected()
            if cacheTrcDataIfNotConnected:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: CacheTrcDataMaxArrLen"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    cacheTrcDataMaxArrLen = 1000
    try:
        if( trcServer is not None):
            cacheTrcDataMaxArrLen = trcServer.getCacheTrcDataMaxArrLen()
            if cacheTrcDataMaxArrLen != 1000: # Initial value should be 1000
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setCacheTrcDataMaxArrLen(100)
            cacheTrcDataMaxArrLen = trcServer.getCacheTrcDataMaxArrLen()
            if cacheTrcDataMaxArrLen != 100:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    try:
        if( trcServer is not None):
            trcServer.setCacheTrcDataMaxArrLen(1000)
            cacheTrcDataMaxArrLen = trcServer.getCacheTrcDataMaxArrLen()
            if cacheTrcDataMaxArrLen != 1000:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Enabled"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    enabled = True
    try:
        if( trcServer is not None):
            enabled = trcServer.isEnabled()
            if not enabled: # Initial value should be True
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setEnabled(False)
        enabled = trcServer.isEnabled()
        if enabled:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setEnabled(True)
        enabled = trcServer.isEnabled()
        if not enabled:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Active"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    active = True
    try:
        active = trcServer.isActive()
        if not active: # As the local trace file is used the server should be active even if not started and therefore not connected
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: NewTrcAdminObjsEnabledAsDefault"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    enabled = True
    try:
        if( trcServer is not None):
            enabled = trcServer.areNewTrcAdminObjsEnabledAsDefault()
            if not enabled: # Initial value should be True
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setNewTrcAdminObjsEnabledAsDefault(False)
        enabled = trcServer.areNewTrcAdminObjsEnabledAsDefault()
        if enabled:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setNewTrcAdminObjsEnabledAsDefault(True)
        enabled = trcServer.areNewTrcAdminObjsEnabledAsDefault()
        if not enabled:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: NewTrcAdminObjsDefaultDetailLevel"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    detailLevel = ETraceDetailLevelNone
    try:
        if( trcServer is not None):
            detailLevel = trcServer.getNewTrcAdminObjsDefaultDetailLevel()
            if detailLevel != ETraceDetailLevelNone: # Initial value should be None
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelRuntimeInfo)
        detailLevel = trcServer.getNewTrcAdminObjsDefaultDetailLevel()
        if detailLevel != ETraceDetailLevelRuntimeInfo:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setNewTrcAdminObjsDefaultDetailLevel(ETraceDetailLevelNone)
        detailLevel = trcServer.getNewTrcAdminObjsDefaultDetailLevel()
        if detailLevel != ETraceDetailLevelNone:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: AdminObjFileAbsoluteFilePath"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    adminObjFileAbsFilePathTmp1 = adminObjFileAbsFilePath
    adminObjFileAbsFilePathTmp2 = adminObjFileAbsFilePath
    try:
        if( trcServer is not None):
            adminObjFileAbsFilePathTmp1 = trcServer.getAdminObjFileAbsoluteFilePath()
            if adminObjFileAbsFilePathTmp1 != adminObjFileAbsFilePath: # Initial value should be same as returned by GetDefaultFilePaths
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        adminObjFileAbsFilePathTmp2 = adminObjFileAbsFilePathTmp1.replace("TrcMthAdmObj.xml", "TraceMethodAdminObjects.xml")
        trcServer.setAdminObjFileAbsoluteFilePath(adminObjFileAbsFilePathTmp2)
        adminObjFileAbsFilePathTmp1 = trcServer.getAdminObjFileAbsoluteFilePath()
        if adminObjFileAbsFilePathTmp1 != adminObjFileAbsFilePathTmp2:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setAdminObjFileAbsoluteFilePath(adminObjFileAbsFilePath)
        adminObjFileAbsFilePathTmp1 = trcServer.getAdminObjFileAbsoluteFilePath()
        if adminObjFileAbsFilePathTmp1 != adminObjFileAbsFilePath:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: UseLocalTrcFile"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    used = True
    try:
        if( trcServer is not None):
            used = trcServer.isLocalTrcFileUsed()
            if not used: # Initial value should be True
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setUseLocalTrcFile(False)
        used = trcServer.isLocalTrcFileUsed()
        if used:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setUseLocalTrcFile(True)
        used = trcServer.isLocalTrcFileUsed()
        if not used:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileAbsoluteFilePath"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    localTrcFileAbsFilePathTmp1 = localTrcFileAbsFilePath
    localTrcFileAbsFilePathTmp2 = localTrcFileAbsFilePath
    try:
        if( trcServer is not None):
            localTrcFileAbsFilePathTmp1 = trcServer.getLocalTrcFileAbsoluteFilePath()
            if localTrcFileAbsFilePathTmp1 != localTrcFileAbsFilePath: # Initial value should be same as returned by GetDefaultFilePaths
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        localTrcFileAbsFilePathTmp2 = localTrcFileAbsFilePathTmp1.replace("TrcMth.log", "TraceMethod.log")
        trcServer.setLocalTrcFileAbsoluteFilePath(localTrcFileAbsFilePathTmp2)
        localTrcFileAbsFilePathTmp1 = trcServer.getLocalTrcFileAbsoluteFilePath()
        if localTrcFileAbsFilePathTmp1 != localTrcFileAbsFilePathTmp2:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileAbsoluteFilePath(localTrcFileAbsFilePath)
        localTrcFileAbsFilePathTmp1 = trcServer.getLocalTrcFileAbsoluteFilePath()
        if localTrcFileAbsFilePathTmp1 != localTrcFileAbsFilePath:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileCompleteBaseName"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    localTrcFileCompleteBaseName = appName + "-TrcMth"
    try:
        if( trcServer is not None):
            localTrcFileCompleteBaseNameTmp1 = trcServer.getLocalTrcFileCompleteBaseName()
            if localTrcFileCompleteBaseNameTmp1 != localTrcFileCompleteBaseName:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileAbsolutePath"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    localTrcFileAbsPathTmp1 = ""
    try:
        if( trcServer is not None):
            localTrcFileAbsPathTmp1 = trcServer.getLocalTrcFileAbsolutePath()
            localTrcFileAbsPathTmp2 = localTrcFileAbsFilePath.replace(localTrcFileCompleteBaseName+".log", "")
            localTrcFileAbsPathTmp1 = localTrcFileAbsPathTmp1 + "/"
            if localTrcFileAbsPathTmp1 != localTrcFileAbsPathTmp2:
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileAutoSaveInterval"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    autoSaveInterval_ms = 1000
    try:
        if( trcServer is not None):
            autoSaveInterval_ms = trcServer.getLocalTrcFileAutoSaveInterval()
            if autoSaveInterval_ms != 1000: # Initial value should be 1000 ms
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileAutoSaveInterval(30000)
        autoSaveInterval_ms = trcServer.getLocalTrcFileAutoSaveInterval()
        if autoSaveInterval_ms != 30000:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileAutoSaveInterval(1000)
        autoSaveInterval_ms = trcServer.getLocalTrcFileAutoSaveInterval()
        if autoSaveInterval_ms != 1000:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileSubFileCountMax"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    countMax = 5
    try:
        if( trcServer is not None):
            countMax = trcServer.getLocalTrcFileSubFileCountMax()
            if countMax != 5: # Initial value should be 5
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileSubFileCountMax(10)
        countMax = trcServer.getLocalTrcFileSubFileCountMax()
        if countMax != 10:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileSubFileCountMax(5)
        countMax = trcServer.getLocalTrcFileSubFileCountMax()
        if countMax != 5:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: LocalTrcFileSubFileLineCountMax"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    countMax = 2000
    try:
        if( trcServer is not None):
            countMax = trcServer.getLocalTrcFileSubFileLineCountMax()
            if countMax != 2000: # Initial value should be 2000
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileSubFileLineCountMax(10000)
        countMax = trcServer.getLocalTrcFileSubFileLineCountMax()
        if countMax != 10000:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setLocalTrcFileSubFileLineCountMax(2000)
        countMax = trcServer.getLocalTrcFileSubFileLineCountMax()
        if countMax != 2000:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Listening"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    listening = False
    try:
        if( trcServer is not None):
            listening = trcServer.isListening()
            if listening: # Trace server not started and therefore not listening for incoming connections
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Startup"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = False
    try:
        if( trcServer is not None):
            testStepOk = trcServer.startup()
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Recall Admin Objects"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = False
    try:
        if( trcServer is not None):
            testStepOk = trcServer.recallAdminObjs()
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Listening"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    listening = True
    try:
        if( trcServer is not None):
            listening = trcServer.isListening()
            if not listening: # Trace server started and therefore listening for incoming connections
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Connected"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    connected = False
    try:
        if( trcServer is not None):
            connected = trcServer.isConnected()
            if connected: # Trace client not yet connected
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: HostSetting.Port"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    port = 24763
    try:
        if( trcServer is not None):
            port = trcServer.getPort()
            if port != 24763: # Initial value should be 24763
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setPort(36742)
        port = trcServer.getPort()
        if port != 36742:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    try:
        trcServer.setPort(24763)
        port = trcServer.getPort()
        if port != 24763:
            testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Client Connect"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    connected = True
    print("Please start and connect the trace method client.")
    result = input("Is the trace client connected? (n/y)")
    if result != "n" and result != "N":
        if( trcServer is not None):
            connected = trcServer.isConnected()
            if not connected: # Trace client should be connected
                testStepOk = False
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Create Server"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = False
    server = None
    try:
        server = CServer("Server")
        testStepOk = True
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Create Gateways"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    serverGateways = {}
    if server is not None:
        testStepOk = True
        for i in range(11):
            try:
                gatewayName = "Gateway" + str(i)
                serverGateway = server.createGateway(gatewayName)
                if serverGateway is None:
                    testStepOk = False
                    break
                else:
                    serverGateways[gatewayName] = serverGateway
            except Exception as exc:
                testStepOk = False
                print(exc)
                break
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Wait for GatewayThreads to terminate"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    try:
        for gatewayName in serverGateways.keys():
            serverGatewayThread = server.getGatewayThread(gatewayName)
            if not serverGatewayThread is None:
                serverGatewayThread.join(20.0)
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    #testStep = "Test Step: Delete Gateways"
    #print(testStep)
    #print("---------------------------------------------")

    #testStepOk = False
    #if testModule1 is not None: # and testModule2 is not None:
    #    try:
    #        testModule1.deleteModule2()
    #        testStepOk = True
    #    except Exception as exc:
    #        testStepOk = False
    #        print(exc)
    #if testOk:
    #   testOk = testStepOk
    #testModule2 = None
    #if testStepOk:
    #    print(testStep + ": Ok")
    #else:
    #    print(testStep + ": !!! FAILED !!!")
    #print("")

    #testStep = "Test Step: Delete Server"
    #print(testStep)
    #print("---------------------------------------------")

    #testStepOk = False
    #if testModule1 is not None:
    #    try:
    #        del testModule1
    #        testStepOk = True
    #    except Exception as exc:
    #        testStepOk = False
    #        print(exc)
    #if testOk:
    #   testOk = testStepOk
    #testModule1 = None
    #if testStepOk:
    #    print(testStep + ": Ok")
    #else:
    #    print(testStep + ": !!! FAILED !!!")
    #print("")

    testStep = "Test Step: Trace Server Attributes: Connected"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    connected = True
    try:
        if( trcServer is not None):
            connected = trcServer.isConnected()
            if not connected: # Trace client still connected
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Client Disconnect"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    connected = False
    print("Please disconnect the trace method client.")
    result = input("Is the trace client disconnected? (n/y)")
    if result != "n" and result != "N":
        if( trcServer is not None):
            connected = trcServer.isConnected()
            if connected: # Trace client no longer connected
                testStepOk = False
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Save Admin Objects"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    if trcServer is not None:
        try:
            testStepOk = trcServer.saveAdminObjs()
        except Exception as exc:
            testStepOk = False
            print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Listening"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    listening = True
    try:
        if( trcServer is not None):
            listening = trcServer.isListening()
            if not listening: # Trace server started and therefore listening for incoming connections
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Shutdown"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = False
    if trcServer is not None:
        try:
            testStepOk = trcServer.shutdown()
        except Exception as exc:
            testStepOk = False
            print(exc)
    if testOk:
       testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    testStep = "Test Step: Trace Server Attributes: Listening"
    print(testStep)
    print("---------------------------------------------")

    testStepOk = True
    listening = False
    try:
        if( trcServer is not None):
            listening = trcServer.isListening()
            if listening: # Trace server not started and therefore not listening for incoming connections
                testStepOk = False
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
        testOk = testStepOk
    if testStepOk:
        print(testStep + ": Ok")
    else:
        print(testStep + ": !!! FAILED !!!")
    print("")

    #testStep = "Test Step: Trace Server Destroy Instance"
    #print(testStep)
    #print("---------------------------------------------")

    #if trcServer is not None:
    #    CIpcTrcServer.DestroyInstance(trcServer)
    #    trcServer = None
    #if testOk:
    #   testOk = testStepOk
    #print(testStep + ": " + str(testStepOk))
    #if testStepOk:
    #    print(testStep + ": Ok")
    #else:
    #    print(testStep + ": !!! FAILED !!!")
    #print("")

    print("")
    if testOk:
        print("+++++++++++++++++++++++++++++++++++++++++++++")
        print("Overall Test Result: OK")
        print("+++++++++++++++++++++++++++++++++++++++++++++")
    else:
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("Overall Test Result: FAILED")
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    print("")
    input("Press any key to continue ...")

