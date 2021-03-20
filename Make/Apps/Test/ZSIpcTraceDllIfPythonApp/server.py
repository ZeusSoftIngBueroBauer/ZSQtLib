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

from zsipctrace import *
from threading import *
from time import *

def myPrint(msg):
    if True:
        print(msg)

class CServer:

    def NameSpace():
        return "ZS::Apps::Test::IpcTraceDllIfPythonApp"

    NameSpace = staticmethod(NameSpace)

    def ClassName():
        return "CServer"

    ClassName = staticmethod(ClassName)

    def __init__(self, objName):
        myPrint("-> <CServer> " + str(objName) + ".__init__()")
        self.__objName = objName
        self.__gatewayThreads = {}
        self.__gateways = {}
        self.__trcAdminObj = CIpcTrcServer.GetTraceAdminObj(CServer.NameSpace(), CServer.ClassName(), self.objectName)
        self.__trcAdminObj.setTraceDetailLevel(ETraceDetailLevelRuntimeInfo)
        mthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            mthInArgs = "ObjName: " + objName
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "__init__", mthInArgs)
        myPrint("<- <CServer> " + str(objName) + ".__init__()")

    def __del__(self):
        myPrint("-> <CServer> " + str(self.__objName) + ".__del__()")
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "__del__","")
        for gatewayName in self.__gatewayThreads.keys():
            if self.__gatewayThreads[gatewayName] is not None and self.__gatewayThreads[gatewayName].isRunning():
                self.__gatewayThreads[gatewayName].quit()
                self.__gatewayThreads[gatewayName].wait()
                del self.__gatewayThreads[gatewayName]
                self.__gatewayThreads[gatewayName] = None
                self.__gateways[gatewayName] = None
        self.__gatewayThreads.clear()
        self.__gateways.clear()
        mthTracer.onAdminObjAboutToBeReleased()
        CIpcTrcServer.ReleaseTraceAdminObj(self.__trcAdminObj)
        objName = self.__objName
        self.__objName = None
        self.__gatewayObjName = None
        self.__gatewayThread = None
        self.__gateway = None
        self.__trcAdminObj = None
        myPrint("<- <CServer> " + str(objName) + ".__del__()")

    def __str__(self):
        return "<CServer> " + str(self.__objName)

    def getNameSpace(self):
        return CServer.NameSpace()

    nameSpace = property(getNameSpace)

    def getClassName(self):
        return CServer.ClassName()

    className = property(getClassName)

    def getObjectName(self):
        return self.__objName

    objectName = property(getObjectName)

    def getGatewayThread(self, gatewayName):
        gatewayThread = None
        if gatewayName in self.__gatewayThreads:
            gatewayThread = self.__gatewayThreads[gatewayName]
        return gatewayThread

    def getGateway(self, gatewayName):
        gateway = None
        if gatewayName in self.__gateways:
            gateway = self.__gateways[gatewayName]
        return gateway

    def createGateway(self, gatewayName, timeout_s = 10.0):
        myPrint("-> <CServer> " + str(self.__objName) + ".createGateway(" + gatewayName + ")")
        mthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            mthInArgs = "Name: " + str(gatewayName) + ", Timeout: " + str(timeout_s) + " s"
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "createGateway", mthInArgs)
        if gatewayName not in self.__gatewayThreads:
            self.__gatewayThreads[gatewayName] = CServerGatewayThread(gatewayName, self)
            self.__gateways[gatewayName] = None
        if not self.__gatewayThreads[gatewayName].isRunning():
            self.__gatewayThreads[gatewayName].start()
            timeStart_s = time()
            while not self.__gatewayThreads[gatewayName].isRunning() or self.__gatewayThreads[gatewayName].getGateway() is None:
                sleep(0.1)
                timeCurr_s = time()
                timeDuration_s = timeCurr_s - timeStart_s
                if timeDuration_s > timeout_s:
                    myPrint("   <CServer> " + str(self.__objName) + ".createGateway(" + gatewayName + ") !! FAILED !! TimeDuration (" + str(timeDuration_s) + " s) > Timeout (" + str(timeout_s) + " s)")
                    break
            timeCurr_s = time()
            timeDuration_s = timeCurr_s - timeStart_s
            myPrint("   <CServer> " + str(self.__objName) + ".createGateway(" + gatewayName + ") TimeDuration (" + str(timeDuration_s) + " s)")
        self.__gateways[gatewayName] = self.__gatewayThreads[gatewayName].getGateway()
        mthTracer.setMethodReturn(str(self.__gateways[gatewayName]))
        myPrint("<- <CServer> " + str(self.__objName) + ".createGateway(): " + str(self.__gateways[gatewayName]))
        return self.__gateways[gatewayName]

    def deleteGateway(self, gatewayName, timeout_s = 5.0):
        myPrint("-> <CServer> " + str(self.__objName) + ".deleteGateway(" + gatewayName + ")")
        mthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            mthInArgs = "Name: " + str(gatewayName) + ", Timeout: " + str(timeout_s) + " s"
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "deleteGateway", mthInArgs)
        if gatewayName in self.__gatewayThreads:
            if self.__gatewayThreads[gatewayName] is not None and self.__gatewayThreads[gatewayName].isRunning():
                self.__gatewayThreads[gatewayName].quit()
                self.__gatewayThreads[gatewayName].wait(timeout_s)
                del self.__gatewayThreads[gatewayName]
                self.__gatewayThreads[gatewayName] = None
                self.__gateways[gatewayName] = None
            del self.__gatewayThreads[gatewayName]
            del self.__gateways[gatewayName]
        myPrint("<- <CServer> " + str(self.__objName) + ".deleteGateway()")


class CServerGatewayThread(Thread):

    def NameSpace():
        return "ZS::Apps::Test::IpcTraceDllIfPythonApp"

    NameSpace = staticmethod(NameSpace)

    def ClassName():
        return "CServerGatewayThread"

    ClassName = staticmethod(ClassName)

    def __init__(self, objName, server) :
        Thread.__init__(self, name=objName)
        self.setName(objName)
        myPrint("-> <CServerGatewayThread> " + str(objName) + ".__init__()")
        self.__objName = objName
        self.__server = server
        self.__isRunning = False
        self.__quit = False
        self.__gateway = None
        self.__trcAdminObj = CIpcTrcServer.GetTraceAdminObj(CServerGatewayThread.NameSpace(), CServerGatewayThread.ClassName(), self.objectName)
        self.__trcAdminObj.setTraceDetailLevel(ETraceDetailLevelRuntimeInfo)
        strMthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            strMthInArgs  = "ObjName: " + objName
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "__init__", strMthInArgs)
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".__init__()")

    def __del__(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".__del__()")
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "__del__", "" )
        if self.isRunning():
            self.quit()
            self.wait(5000)
        mthTracer.onAdminObjAboutToBeReleased()
        CIpcTrcServer.ReleaseTraceAdminObj(self.__trcAdminObj)
        objName = self.__objName
        self.__objName = None
        self.__server = None
        self.__isRunning = False
        self.__quit = False
        if self.__gateway is not None:
            del self.__gateway
        self.__gateway = None
        self.__trcAdminObj = None
        myPrint("<- <CServerGatewayThread> " + str(objName) + ".__del__()")

    def __str__(self):
        return "<CServerGatewayThread> " + str(self.__objName)

    def getNameSpace(self):
        return CServer.NameSpace()

    nameSpace = property(getNameSpace)

    def getClassName(self):
        return CServer.ClassName()

    className = property(getClassName)

    def getObjectName(self):
        return self.__objName

    objectName = property(getObjectName)

    def getGateway(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".getGateway()")
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".getGateway(): " + str(self.__gateway))
        return self.__gateway

    def isRunning(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".isRunning()")
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".isRunning(): " + str(self.__isRunning))
        return self.__isRunning

    def quit(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".quit()")
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "quit", "")
        self.__quit = True
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".quit()")

    def wait(self, timeout_s):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".wait()")
        strMthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            strMthInArgs = "Timeout: " + str(timeout_s) + " s"
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "wait", strMthInArgs)
        timeStart_s = time()
        while self.__isRunning:
            sleep(0.1)
            timeCurr_s = time()
            timeDuration_s = timeCurr_s - timeStart_s
            if timeDuration_s > timeout_s:
                break
        if mthTracer.isActive(ETraceDetailLevelMethodArgs):
            mthTracer.setMethodReturn(not self.__isRunning)
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".wait(): " + str(not self.__isRunning))
        return not self.__isRunning

    def exec(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".exec()")
        result = 0
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "exec", "")
        timeStart_s = time()
        timeCurr_s = time()
        timeDuration_s = timeCurr_s - timeStart_s
        while not self.__quit and timeDuration_s < 10.0:
            sleep(0.5)
            timeCurr_s = time()
            timeDuration_s = timeCurr_s - timeStart_s
            if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo:
                strMthAddInfo = "Thread running for " + str(timeDuration_s) + " s"
                mthTracer.trace(strMthAddInfo, ETraceDetailLevelRuntimeInfo)
            myPrint("   <CServerGatewayThread> " + str(self.__objName) + ".exec(): Thread running for {} s".format(timeDuration_s))
        if mthTracer.isActive(ETraceDetailLevelMethodArgs):
            mthTracer.setMethodReturn(result)
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".exec()")
        return result

    def run(self):
        myPrint("-> <CServerGatewayThread> " + str(self.__objName) + ".run()")
        CIpcTrcServer.RegisterThreadName(self.ident, self.__objName)
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "run", "")
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelRuntimeInfo:
            strMthAddInfo = "ThreadName: " + self.name + ", ThreadId: " + "0x{0:08x}".format(self.ident)
            mthTracer.trace(strMthAddInfo, ETraceDetailLevelRuntimeInfo)
        self.__isRunning = True
        self.__gateway = CServerGateway(self.__objName, self)
        self.__gateway.recursiveTraceMethod()
        self.exec()
        self.__isRunning = False
        self.__gateway = None
        mthTracer.__del__()
        CIpcTrcServer.UnregisterThreadName(self.ident)
        myPrint("<- <CServerGatewayThread> " + str(self.__objName) + ".run()")


class CServerGateway:

    def NameSpace():
        return "ZS::Apps::Test::IpcTraceDllIfPythonApp"

    NameSpace = staticmethod(NameSpace)

    def ClassName():
        return "CServerGateway"

    ClassName = staticmethod(ClassName)

    def __init__(self, objName, gatewayThread):
        myPrint("-> <CServerGateway> " + str(objName) + ".__init__()")
        self.__objName = objName
        self.__gatewayThread = gatewayThread
        self.__recursiveTraceMethodCounter = 0
        self.__recursiveTraceMethodFinished = False
        self.__trcAdminObj = CIpcTrcServer.GetTraceAdminObj(CServerGateway.NameSpace(), CServerGateway.ClassName(), self.objectName)
        self.__trcAdminObj.setTraceDetailLevel(ETraceDetailLevelRuntimeInfo)
        strMthInArgs = ""
        if self.__trcAdminObj is not None and self.__trcAdminObj.getTraceDetailLevel() >= ETraceDetailLevelMethodArgs:
            strMthInArgs += "ObjName: " + objName
            if gatewayThread is None:
                strMthInArgs = strMthInArgs + ", TestModule2Thread: None"
            else:
                strMthInArgs = strMthInArgs + ", TestModule2Thread: " + str(gatewayThread.objectName)
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "ctor", strMthInArgs)
        myPrint("<- <CServerGateway> " + str(self.__objName) + ".__init__()")

    def __del__(self):
        myPrint("-> <CServerGateway> " + str(self.__objName) + ".__del__()")
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "dtor", "")
        mthTracer.onAdminObjAboutToBeReleased()
        CIpcTrcServer.ReleaseTraceAdminObj(self.__trcAdminObj)
        objName = self.__objName
        self.__objName = None
        self.__gatewayThread = None
        self.__recursiveTraceMethodCounter = 0
        self.__recursiveTraceMethodFinished = False
        self.__trcAdminObj = None
        myPrint("<- <CServerGateway> " + str(objName) + ".__del__()")

    def __str__(self):
        return "<CServerGateway> " + str(self.__objName)

    def getNameSpace(self):
        return CServer.NameSpace()

    nameSpace = property(getNameSpace)

    def getClassName(self):
        return CServer.ClassName()

    className = property(getClassName)

    def getObjectName(self):
        return self.__objName

    objectName = property(getObjectName)

    def recursiveTraceMethod(self):
        myPrint("-> <CServerGateway> " + str(self.__objName) + ".recursiveTraceMethod()")
        mthTracer = CMethodTracer(self.__trcAdminObj, ETraceDetailLevelMethodCalls, "recursiveTraceMethod", "")
        self.__recursiveTraceMethodCounter = self.__recursiveTraceMethodCounter + 1
        while not self.__recursiveTraceMethodFinished:
            sleep(0.1)
            if self.__recursiveTraceMethodCounter >= 10:
                self.__recursiveTraceMethodFinished = True
            if not self.__recursiveTraceMethodFinished:
                self.recursiveTraceMethod()
        self.__recursiveTraceMethodCounter = self.__recursiveTraceMethodCounter - 1
        if mthTracer.isActive(ETraceDetailLevelMethodArgs):
            mthTracer.setMethodReturn(self.__recursiveTraceMethodCounter)
        myPrint("<- <CServerGateway> " + str(self.__objName) + ".recursiveTraceMethod(): " + str(self.__recursiveTraceMethodCounter))
        return self.__recursiveTraceMethodCounter
