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

from zsipctrace import *
from server import *

#import tracemalloc

if __name__ == "__main__":

    trcServer = None

    testOk = True

    # Test Step: Trace Server Create Instance
    #----------------------------------------

    testStepOk = False
    try:
        orgName = "ZeusSoft"
        appName = "ZSAppTestIpcTraceDllIfPythonApp"
        CTrcServer.SetOrganizationName(orgName)
        CTrcServer.SetApplicationName(appName)
        orgName = CTrcServer.GetOrganizationName();
        appName = CTrcServer.GetApplicationName();
        trcServer = CIpcTrcServer.CreateInstance("ZSTrcServer", ETraceDetailLevelVerbose)
        if trcServer is not None:
            testStepOk = True
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk

    # Test Step: Trace Server Attributes
    #-----------------------------------

    #testStepOk = False
    #closeFileAfterEachWrite = True
    #try:
    #    if( trcServer is not None):
    #        testStepOk = trcServer.setLocalTrcFileCloseFileAfterEachWrite(closeFileAfterEachWrite)
    #except Exception as exc:
    #    testStepOk = False
    #    print(exc)
    #if testOk:
    #   testOk = testStepOk

    #testStepOk = False
    #try:
    #    if( trcServer is not None):
    #        closeFileAfterEachWriteTmp = trcServer.getLocalTrcFileCloseFileAfterEachWrite()
    #        if closeFileAfterEachWriteTmp == closeFileAfterEachWrite:
    #            testStepOk = True
    #except Exception as exc:
    #    testStepOk = False
    #    print(exc)
    #if testOk:
    #   testOk = testStepOk

    # Test Step: Trace Server Startup
    #--------------------------------

    testStepOk = False
    try:
        if( trcServer is not None):
            testStepOk = trcServer.startup()
    except Exception as exc:
        testStepOk = False
        print(exc)
    if testOk:
       testOk = testStepOk

    # Test Step: Trace Server Recall Admin Objects
    #---------------------------------------------

    #testStepOk = False
    #try:
    #    if( trcServer is not None):
    #        testStepOk = trcServer.recallAdminObjs()
    #except Exception as exc:
    #    testStepOk = False
    #    print(exc)
    #if testOk:
    #   testOk = testStepOk

    # Test Step: Trace Client Connect
    #--------------------------------

    #testStepOk = False
    #print("Please start and connect the trace method client.")
    #result = input("Is the trace client connected? (n/y)")
    #if result == "y":
    #    testStepOk = True
    #if testOk:
    #   testOk = testStepOk

    # Test Step: Create Server
    #-------------------------

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

    # Test Step: Create Gateways
    #---------------------------

    # tracemalloc.start()

    testStepOk = False
    serverGateway1 = None
    serverGateway2 = None
    if server is not None:
        try:
            serverGateway1 = server.createGateway("Gateway1")
            serverGateway2 = server.createGateway("Gateway2")
            if serverGateway1 is not None and serverGateway2 is not None:
                testStepOk = True
        except Exception as exc:
            testStepOk = False
            print(exc)
    if testOk:
       testOk = testStepOk

    # Test Step: Wait for GatewayThreads to terminate
    #------------------------------------------------

    serverGatewayThread1 = server.getGatewayThread("Gateway1")
    serverGatewayThread2 = server.getGatewayThread("Gateway2")

    if not serverGatewayThread1 is None:
        serverGatewayThread1.join(20.0)
    if not serverGatewayThread2 is None:
        serverGatewayThread2.join(20.0)

    # Test Step: Delete Gateways
    #---------------------------

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

    # Test Step: Delete Server
    #---------------------------

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

    # Test Step: Trace Client Disconnect
    #-----------------------------------

    #testStepOk = False
    #print("Please disconnect the trace method client.")
    #result = input("Is the trace client disconnected? (n/y)")
    #if result == "y":
    #    testStepOk = True
    #if testOk:
    #   testOk = testStepOk

    # Test Step: Trace Server Save Admin Objects
    #---------------------------------------------

    #testStepOk = False
    #if trcServer is not None:
    #    try:
    #        testStepOk = trcServer.saveAdminObjs()
    #    except Exception as exc:
    #        testStepOk = False
    #        print(exc)
    #if testOk:
    #   testOk = testStepOk

    # Test Step: Trace Server Shutdown
    #---------------------------------

    testStepOk = False
    if trcServer is not None:
        try:
            testStepOk = trcServer.shutdown()
        except Exception as exc:
            testStepOk = False
            print(exc)
    if testOk:
       testOk = testStepOk

    # Test Step: Trace Server Destroy Instance
    #----------------------------------------

    #if trcServer is not None:
    #    CIpcTrcServer.DestroyInstance(trcServer)
    #    trcServer = None
    #if testOk:
    #   testOk = testStepOk

    print("Test Result: ", testOk)
    #input("Press any key to continue ...")

