#*********************************************************************************
#
# Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

def myPrint(msg):
    if True:
        print(msg)

try:
    import ZSIpcTracePy
except:
    myPrint("Importing ZSIpcTracePy failed")


#===============================================================================
# Predefined trace detail levels.
#
# @ingroup _GRP_Libs_ZSSys_MethodTracing
#
# Higher detail levels include lower detail levels.
# Please note that this enum is only a suggestion for trace detail levels
# which can be used. The detail level of the trace admin object is an integer
# type and user defined values can be used if necessary.
# To avoid type casts requested by the compiler this enum is not a class enum definition.
#
# @see ZS::System::SEnumEntry
# @see ZS::System::CEnum
# @see _GRP_BasicConcepts_Enumerations
#===============================================================================

ETraceDetailLevelUndefined      = -1    #!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used.
ETraceDetailLevelNone           =  0    #!< Trace output is disabled.
ETraceDetailLevelMethodCalls    =  1    #!< Tracing method entries and leaves without tracing input and output arguments and without method return value.
ETraceDetailLevelMethodArgs     =  2    #!< Tracing method entries and leaves together with input and output arguemnts and method return value.
ETraceDetailLevelInternalStates =  3    #!< Tracing everything above and internal states (e.g. current state, current request in progress etc..
ETraceDetailLevelRuntimeInfo    =  4    #!< Tracing everything above and additional runtime info (e.g. what is going to be done, what has been executed and how).
ETraceDetailLevelVerbose        =  5    #!< Tracing everything above and even more.

class CTrcAdminObj:

    @classmethod
    def buildKeyInTree(cls, nameSpace, className, objName):
        myPrint("-> <CTrcAdminObj>.buildKeyInTree(" + nameSpace + ", " + className + ", " + str(objName) + ")")
        keyInTree = "L:" + nameSpace + "::" + className + "::" + objName
        myPrint("<- <CTrcAdminObj>.buildKeyInTree()" + keyInTree)
        return keyInTree

    def __init__(self, trcServer, nameSpace, className, objName, enabledAsDefault = True, defaultDetailLevel = ETraceDetailLevelNone):
        keyInTree = self.buildKeyInTree(nameSpace, className, objName)
        myPrint("-> <CTrcAdminObj> " + str(keyInTree) + ".__init__(" + trcServer.objectName + ", " + nameSpace + ", " + className + ", " + str(objName) + ")")
        self.__trcServer = trcServer
        self.__trcServerName = trcServer.objectName
        self.__nameSpace = nameSpace
        self.__className = className
        self.__objName = objName
        self.__keyInTree = keyInTree
        #self.__indexInTree = -1
        self.__objThreadName = ""
        self.__refCount = 1
        self.__enabled = enabledAsDefault
        self.__trcDetailLevel = defaultDetailLevel
        #self.__indexInTree, self.__keyInTree = ZSIpcTracePy.IpcTrcServer_getTraceAdminObj(nameSpace, className, objName, enabledAsDefault, defaultDetailLevel, trcServer.objectName)
        #self.__indexInTree = ZSIpcTracePy.IpcTrcServer_getTraceAdminObj(nameSpace, className, objName, enabledAsDefault, defaultDetailLevel, trcServer.objectName)
        self.__keyInTree = ZSIpcTracePy.IpcTrcServer_getTraceAdminObj(nameSpace, className, objName, enabledAsDefault, defaultDetailLevel, trcServer.objectName)
        myPrint("<- <CTrcAdminObj> " + str(keyInTree) + ".__init__()")

    def __del__(self):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".__del__()")
        #ZSIpcTracePy.IpcTrcServer_releaseTraceAdminObj(int(self.__indexInTree), self.__trcServerName)
        ZSIpcTracePy.IpcTrcServer_releaseTraceAdminObj(self.__keyInTree, self.__trcServerName)
        self.__trcServer = None
        self.__trcServerName = ""
        self.__nameSpace = ""
        self.__className = ""
        self.__objName = ""
        keyInTree = self.__keyInTree
        self.__keyInTree = ""
        #self.__indexInTree = 0
        self.__objThreadName = ""
        self.__refCount = 0
        self.__enabled = False
        self.__trcDetailLevel = ETraceDetailLevelNone
        myPrint("<- <CTrcAdminObj> " + str(keyInTree) + ".__del__()")

    def __str__(self):
        #return "<CTrcAdminObj> " + str(self.__trcServerName) + "::" + str(self.__keyInTree) + " {IdxInTree: " + str(self.__indexInTree) + ", RefCount: " + str(self.__refCount) + "}"
        return "<CTrcAdminObj> " + str(self.__trcServerName) + "::" + str(self.__keyInTree) + " {RefCount: " + str(self.__refCount) + "}"

    def getServerName(self):
        return self.__trcServerName

    serverName = property(getServerName)

    def getNameSpace(self):
        return self.__nameSpace

    nameSpace = property(getNameSpace)

    def getClassName(self):
        return self.__className

    className = property(getClassName)

    def getObjectName(self):
        return self.__objName

    objectName = property(getObjectName)

    def getKeyInTree(self):
        return self.__keyInTree

    keyInTree = property(getKeyInTree)

    #def getIndexInTree(self):
    #    return int(self.__indexInTree)

    #indexInTree = property(getIndexInTree)

    def setObjectThreadName(self, threadName):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".setObjectThreadName(" + threadName + ")")
        self.__objThreadName = threadName
        #ok = ZSIpcTracePy.TrcAdminObj_setObjectThreadName(int(self.__indexInTree), threadName, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_setObjectThreadName(self.__keyInTree, threadName, self.__trcServerName)
        myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".setObjectThreadName()")
        #return ok

    def getObjectThreadName(self):
        objThreadName = self.__objThreadName
        #objThreadName = ZSIpcTracePy.TrcAdminObj_getObjectThreadName(int(self.__indexInTree), self.__trcServerName)
        objThreadName = ZSIpcTracePy.TrcAdminObj_getObjectThreadName(self.__keyInTree, self.__trcServerName)
        return objThreadName

    objectThreadName = property(getObjectThreadName, setObjectThreadName)

    def incrementRefCount(self):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".incrementRefCount()")
        self.__refCount = self.__refCount + 1
        myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".incrementRefCount(): " + str(self.__refCount))
        return self.__refCount

    def decrementRefCount(self):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".decrementRefCount()")
        self.__refCount = self.__refCount - 1
        if self.__refCount < 0:
            raise RuntimeError("Reference counter of trace admin object " + str(self.__keyInTree) + " out of range (< 0)")
        myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".decrementRefCount(): " + str(self.__refCount))
        return self.__refCount

    def getRefCount(self):
        refCount = self.__refCount
        return refCount

    refCount = property(getRefCount)

    def setEnabled(self, enabled):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".setEnabled(" + str(enabled) + ")")
        self.__enabled = enabled
        #ok = ZSIpcTracePy.TrcAdminObj_setEnabled(int(self.__indexInTree), enabled, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_setEnabled(self.__keyInTree, enabled, self.__trcServerName)
        myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".setEnabled()")
        #return ok

    def isEnabled(self):
        enabled = self.__enabled
        #enabled = ZSIpcTracePy.TrcAdminObj_isEnabled(int(self.__indexInTree), self.__trcServerName)
        enabled = ZSIpcTracePy.TrcAdminObj_isEnabled(self.__keyInTree, self.__trcServerName)
        return enabled

    enabled = property(isEnabled, setEnabled)

    def setTraceDetailLevel(self, trcDetailLevel):
        myPrint("-> <CTrcAdminObj> " + str(self.__keyInTree) + ".setTraceDetailLevel(" + str(trcDetailLevel) + ")")
        self.__trcDetailLevel = trcDetailLevel
        #ok = ZSIpcTracePy.TrcAdminObj_setTraceDetailLevel(int(self.__indexInTree), trcDetailLevel, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_setTraceDetailLevel(self.__keyInTree, trcDetailLevel, self.__trcServerName)
        #myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".setTraceDetailLevel(): " + str(ok))
        myPrint("<- <CTrcAdminObj> " + str(self.__keyInTree) + ".setTraceDetailLevel()")
        #return ok

    def getTraceDetailLevel(self):
        trcDetailLevel = self.__trcDetailLevel
        #trcDetailLevel = ZSIpcTracePy.TrcAdminObj_getTraceDetailLevel(int(self.__indexInTree), self.__trcServerName)
        trcDetailLevel = ZSIpcTracePy.TrcAdminObj_getTraceDetailLevel(self.__keyInTree, self.__trcServerName)
        return trcDetailLevel

    traceDetailLevel = property(getTraceDetailLevel, setTraceDetailLevel)

    def isActive(self, detailLevel):
        isActive = self.__enabled and self.__trcDetailLevel >= detailLevel
        #isActive = ZSIpcTracePy.TrcAdminObj_isActive(int(self.__indexInTree), detailLevel, self.__trcServerName)
        isActive = ZSIpcTracePy.TrcAdminObj_isActive(self.__keyInTree, detailLevel, self.__trcServerName)
        return isActive

    def traceMethodEnter(self, methodName, methodInArgs = "", objectName = ""):
        myPrint("-> <CTrcAdminObj> " + self.__keyInTree + ".traceMethodEnter(" + methodName + ", " + str(methodInArgs) + ", " + str(objectName) + ")")
        #ZSIpcTracePy.TrcAdminObj_traceMethodEnter(int(self.__indexInTree), methodName, str(methodInArgs), objectName, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_traceMethodEnter(self.__keyInTree, methodName, str(methodInArgs), objectName, self.__trcServerName)
        myPrint("<- <CTrcAdminObj> " + self.__keyInTree + ".traceMethodEnter()")

    def traceMethodLeave(self, methodName, methodReturn = "", methodOutArgs = "", objectName = ""):
        myPrint("-> <CTrcAdminObj> " + self.__keyInTree + ".traceMethodLeave(" + methodName + ", " + str(methodReturn) + ", " + str(methodOutArgs) + ", " + str(objectName) + ")")
        #ZSIpcTracePy.TrcAdminObj_traceMethodLeave(int(self.__indexInTree), methodName, str(methodReturn), str(methodOutArgs), objectName, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_traceMethodLeave(self.__keyInTree, methodName, str(methodReturn), str(methodOutArgs), str(objectName), self.__trcServerName)
        myPrint("<- <CTrcAdminObj> " + self.__keyInTree + ".traceMethodLeave()")

    def traceMethod(self, methodName, methodAddInfo, objectName = ""):
        myPrint("-> <CTrcAdminObj> " + self.__keyInTree + ".traceMethod(" + methodName + ", " + methodAddInfo + ", " + objectName + ")")
        #ZSIpcTracePy.TrcAdminObj_traceMethod(int(self.__indexInTree), methodName, str(methodAddInfo), objectName, self.__trcServerName)
        ZSIpcTracePy.TrcAdminObj_traceMethod(self.__keyInTree, methodName, str(methodAddInfo), objectName, self.__trcServerName)
        myPrint("<- <CTrcAdminObj> " + self.__keyInTree + ".traceMethod()")


class CMethodTracer:

    def __init__(self, trcAdminObj, filterDetailLevel, methodName, methodInArgs, objectName = "" ):
        myPrint("-> <CMethodTracer> " + str(trcAdminObj) + ".__init__(" + str(methodName) + ")")
        self.__trcAdminObj = trcAdminObj
        self.__trcAdminObjKeyInTree = ""
        self.__enterLeaveFilterDetailLevel = filterDetailLevel
        self.__objName = objectName
        self.__methodName = methodName
        self.__methodInArgs = str(methodInArgs)
        self.__methodOutArgs = ""
        self.__methodReturn = ""
        if self.__trcAdminObj is not None:
            self.__trcAdminObjKeyInTree = self.__trcAdminObj.keyInTree
            if self.__trcAdminObj.isActive(self.__enterLeaveFilterDetailLevel):
                self.__trcAdminObj.traceMethodEnter(self.__methodName, self.__methodInArgs, self.__objName)
        myPrint("<- <CMethodTracer> " + str(trcAdminObj) + ".__init__(" + str(methodName) + ")")

    def __del__(self):
        myPrint("-> <CMethodTracer> " + str(self.__trcAdminObjKeyInTree) + ".__del__(" + str(self.__methodName) + ")")
        if self.__trcAdminObj is not None:
            if self.__trcAdminObj.isActive(self.__enterLeaveFilterDetailLevel):
                self.__trcAdminObj.traceMethodLeave(self.__methodName, self.__methodReturn, self.__methodOutArgs, self.__objName)
        myPrint("<- <CMethodTracer> " + str(self.__trcAdminObjKeyInTree) + ".__del__(" + str(self.__methodName) + ")")
        self.__trcAdminObj = None
        self.__trcAdminObjKeyInTree = ""
        self.__enterLeaveFilterDetailLevel = ETraceDetailLevelNone
        self.__objName = ""
        self.__methodName = ""
        self.__methodInArgs = ""
        self.__methodOutArgs = ""
        self.__methodReturn = ""

    def onAdminObjAboutToBeReleased(self):
        myPrint("-> <CMethodTracer> " + str(self.__trcAdminObj) + ".onAdminObjAboutToBeReleased(" + str(self.__methodName) + ")")
        if self.__trcAdminObj is not None:
            if self.__trcAdminObj.isActive(self.__enterLeaveFilterDetailLevel):
                self.__trcAdminObj.traceMethodLeave(self.__methodName, self.__methodReturn, self.__methodOutArgs, self.__objName)
        myPrint("<- <CMethodTracer> " + str(self.__trcAdminObj) + ".onAdminObjAboutToBeReleased(" + str(self.__methodName) + ")")
        self.__trcAdminObj = None

    def getTraceDetailLevel(self):
        detailLevel = ETraceDetailLevelNone
        if self.__trcAdminObj is not None:
            detailLevel = self.__trcAdminObj.getTraceDetailLevel()
        return detailLevel

    traceDetailLevel = property(getTraceDetailLevel)

    def getNameSpace(self):
        name = ""
        if self.__trcAdminObj is not None:
            name = self.__trcAdminObj.getNameSpace()
        return name;

    nameSpace = property(getNameSpace)

    def getClassName(self):
        name = ""
        if self.__trcAdminObj is not None:
            name = self.__trcAdminObj.getClassName()
        return name;

    className = property(getClassName)

    def getObjectName(self):
        name = ""
        if len(self.__objName) == 0 and self.__trcAdminObj is not None:
            name = self.__trcAdminObj.getObjectName()
        return name;

    objectName = property(getObjectName)

    def setMethodReturn(self, methodReturn):
        self.__methodReturn = str(methodReturn)

    def isActive(self, filterDetailLevel):
        isActive = False
        if self.__trcAdminObj is not None:
            isActive = self.__trcAdminObj.isActive(filterDetailLevel);
        return isActive

    def trace(self, methodAddInfo, filterDetailLevel):
        if self.__trcAdminObj is not None:
            if self.__trcAdminObj.isActive(filterDetailLevel):
                self.__trcAdminObj.traceMethod(self.__methodName, str(methodAddInfo), self.__objName)


class CIpcTrcServer:

    _instances = {}

    @staticmethod
    def SetOrganizationName(name):
        myPrint("-> <CIpcTrcServer> SetOrganizationName(" + str(name) + ")")
        ZSIpcTracePy.IpcTrcServer_setOrganizationName(name)
        myPrint("<- <CIpcTrcServer> SetOrganizationName()")

    @staticmethod
    def GetOrganizationName():
        myPrint("-> <CIpcTrcServer> GetOrganizationName()")
        name = ZSIpcTracePy.IpcTrcServer_getOrganizationName()
        myPrint("<- <CIpcTrcServer> GetOrganizationName(): " + str(name))
        return name

    @staticmethod
    def SetApplicationName(name):
        myPrint("-> <CIpcTrcServer> SetApplicationName(" + str(name) + ")")
        ZSIpcTracePy.IpcTrcServer_setApplicationName(name)
        myPrint("<- <CIpcTrcServer> SetApplicationName()")

    @staticmethod
    def GetApplicationName():
        myPrint("-> <CIpcTrcServer> GetApplicationName()")
        name = ZSIpcTracePy.IpcTrcServer_getApplicationName()
        myPrint("<- <CIpcTrcServer> GetApplicationName(): " + str(name))
        return name

    @staticmethod
    def GetDefaultFilePaths(iniFileScope = "System"):
        myPrint("-> <CIpcTrcServer> GetDefaultFilePaths(" + str(iniFileScope) + ")")
        adminObjFileAbsFilePath, localTrcFileAbsFilePath = ZSIpcTracePy.IpcTrcServer_getDefaultFilePaths(iniFileScope)
        myPrint("<- <CIpcTrcServer> GetDefaultFilePaths(): " + str(adminObjFileAbsFilePath) + ", " + str(localTrcFileAbsFilePath))
        return adminObjFileAbsFilePath, localTrcFileAbsFilePath

    @classmethod
    def GetInstance(cls, serverName = "ZSTrcServer"):
        myPrint("-> <CIpcTrcServer> GetInstance(" + serverName + ")")
        trcServer = None
        if serverName in cls._instances:
            trcServer = cls._instances[serverName]
        myPrint("<- <CIpcTrcServer> GetInstance(): " + str(trcServer))
        return trcServer

    @classmethod
    def CreateInstance(cls, serverName = "ZSTrcServer", trcDetailLevel = ETraceDetailLevelNone):
        myPrint("-> <CIpcTrcServer> CreateInstance(" + serverName + ")")
        if serverName in cls._instances:
            raise KeyError("A trace server with name " + str(serverName) + " is already existing")
        trcServer = CIpcTrcServer(serverName, trcDetailLevel)
        myPrint("<- <CIpcTrcServer> CreateInstance(): " + str(trcServer))
        return trcServer

    @classmethod
    def DestroyInstance(cls, serverName = "ZSTrcServer"):
        myPrint("-> <CIpcTrcServer> DestroyInstance(" + serverName + ")")
        if serverName not in cls._instances:
            raise KeyError("A trace server with name " + str(serverName) + " is not existing")
        trcServer = cls._instances[serverName]
        del trcServer
        myPrint("<- <CIpcTrcServer> DestroyInstance()")

    @classmethod
    def RegisterThreadName(cls, threadId, threadName):
        myPrint("-> <CIpcTrcServer> RegisterThreadName(" + str(threadId) + ", " + threadName + ")")
        ZSIpcTracePy.IpcTrcServer_registerThreadName(threadId, threadName)
        myPrint("<- <CIpcTrcServer> RegisterThreadName()")

    @classmethod
    def UnregisterThreadName(cls, threadId):
        myPrint("-> <CIpcTrcServer> UnregisterThreadName(" + str(threadId) + ")")
        ZSIpcTracePy.IpcTrcServer_unregisterThreadName(threadId)
        myPrint("<- <CIpcTrcServer> UnregisterThreadName()")

    @classmethod
    def GetTraceAdminObj(cls, nameSpace, className, objName, enabledAsDefault = True, defaultDetailLevel = ETraceDetailLevelNone, serverName = "ZSTrcServer"):
        myPrint("-> <CIpcTrcServer> " + str(serverName) + ".GetTraceAdminObj(" + nameSpace + ", " + className + ", " + str(objName) + ")")
        trcServer = cls.GetInstance(serverName)
        if trcServer is None:
            raise KeyError("A trace server with name " + str(serverName) + " is not existing")
        trcAdminObj = None
        keyInTree = CTrcAdminObj.buildKeyInTree(nameSpace, className, objName)
        if keyInTree in trcServer.__trcAdminObjs:
            trcAdminObj = trcServer.__trcAdminObjs[keyInTree]
        if trcAdminObj is None:
            trcAdminObj = CTrcAdminObj(trcServer, nameSpace, className, objName, enabledAsDefault, defaultDetailLevel)
            trcServer.__trcAdminObjs[keyInTree] = trcAdminObj
        else:
            trcAdminObj.incrementRefCount()
        myPrint("<- <CIpcTrcServer> " + str(serverName) + ".GetTraceAdminObj(): " + str(trcAdminObj))
        return trcAdminObj

    @classmethod
    def ReleaseTraceAdminObj(cls, trcAdminObj):
        serverName = trcAdminObj.serverName
        myPrint("-> <CIpcTrcServer> " + str(serverName) + ".ReleaseTraceAdminObj(" + str(trcAdminObj) + ")")
        keyInTree = trcAdminObj.keyInTree
        trcServer = cls.GetInstance(serverName)
        trcAdminObj.decrementRefCount()
        if trcAdminObj.refCount == 0:
            del trcAdminObj
            trcAdminObj = None
            if trcServer is not None: # The trace server may be deleted by python before the trace admin object when finishing the script
                if keyInTree not in trcServer.__trcAdminObjs:
                    raise KeyError("A trace admin object with key " + str(self.__keyInTree) + " is not existing")
                del trcServer.__trcAdminObjs[keyInTree]
        myPrint("<- <CIpcTrcServer> " + str(serverName) + ".ReleaseTraceAdminObj(): " + str(trcAdminObj))
        return trcAdminObj

    def __init__(self, objName, trcDetailLevel):
        myPrint("-> <CIpcTrcServer> " + str(objName) + ".__init__()")
        self.__objName = objName
        self.__trcAdminObjs = {}
        if objName in self._instances:
            raise KeyError("A trace server with name " + str(objName) + " is already existing")
        ZSIpcTracePy.IpcTrcServer_createInstance(objName, trcDetailLevel)
        self._instances[objName] = self
        myPrint("<- <CIpcTrcServer> " + str(self.objectName) + ".__init__()")

    def __del__(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".__del__()")
        ZSIpcTracePy.IpcTrcServer_destroyInstance(self.__objName)
        objName = self.__objName
        del self._instances[objName]
        # Please note that the trace server may be deleted before the last trace admin object is deleted.
        # Even if the trace admin objects got a reference to the trace server.
        # For me it looks like python doesn't care about reference counting when terminating the script.
        for keyInTree in self.__trcAdminObjs.keys():
            if self.__trcAdminObjs[keyInTree] is not None:
                trcAdminObj = self.__trcAdminObjs[keyInTree]
                # Please note that the del call does not necessarily delete the object.
                # There might still be a reference to the trace admin object somewhere else.
                del trcAdminObj
                self.__trcAdminObjs[keyInTree] = None
        self.__objName = ""
        self.__trcAdminObjs.clear()
        myPrint("<- <CIpcTrcServer> " + str(objName) + ".__del__()")

    def __str__(self):
        return "<CIpcTrcServer> " + str(self.objectName)

    def getObjectName(self):
        return self.__objName

    objectName = property(getObjectName)

    def isActive(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isActive()")
        isActive = ZSIpcTracePy.IpcTrcServer_isActive(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isActive(): " + str(isActive))
        return isActive

    def setEnabled(self, enabled):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setEnabled(" + str(enabled) + ")")
        ZSIpcTracePy.IpcTrcServer_setEnabled(enabled, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setEnabled()")

    def isEnabled(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isEnabled()")
        enabled = ZSIpcTracePy.IpcTrcServer_isEnabled(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isEnabled(): " + str(enabled))
        return enabled

    def setNewTrcAdminObjsEnabledAsDefault(self, enabled):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setNewTrcAdminObjsEnabledAsDefault(" + str(enabled) + ")")
        ZSIpcTracePy.IpcTrcServer_setNewTrcAdminObjsEnabledAsDefault(enabled, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setNewTrcAdminObjsEnabledAsDefault()")

    def areNewTrcAdminObjsEnabledAsDefault(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".areNewTrcAdminObjsEnabledAsDefault()")
        enabled = ZSIpcTracePy.IpcTrcServer_areNewTrcAdminObjsEnabledAsDefault(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".areNewTrcAdminObjsEnabledAsDefault(): " + str(enabled))
        return enabled

    def setNewTrcAdminObjsDefaultDetailLevel(self, detailLevel):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setNewTrcAdminObjsDefaultDetailLevel(" + str(detailLevel) + ")")
        ZSIpcTracePy.IpcTrcServer_setNewTrcAdminObjsDefaultDetailLevel(detailLevel, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setNewTrcAdminObjsDefaultDetailLevel()")

    def getNewTrcAdminObjsDefaultDetailLevel(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getNewTrcAdminObjsDefaultDetailLevel()")
        detailLevel = ZSIpcTracePy.IpcTrcServer_getNewTrcAdminObjsDefaultDetailLevel(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getNewTrcAdminObjsDefaultDetailLevel(): " + str(detailLevel))
        return detailLevel

    def setAdminObjFileAbsoluteFilePath(self, absFilePath):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setAdminObjFileAbsoluteFilePath(" + str(absFilePath) + ")")
        ZSIpcTracePy.IpcTrcServer_setAdminObjFileAbsoluteFilePath(absFilePath, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setAdminObjFileAbsoluteFilePath()")

    def getAdminObjFileAbsoluteFilePath(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getAdminObjFileAbsoluteFilePath()")
        absFilePath = ZSIpcTracePy.IpcTrcServer_getAdminObjFileAbsoluteFilePath(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getAdminObjFileAbsoluteFilePath(): " + str(absFilePath))
        return absFilePath

    def recallAdminObjs(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".recallAdminObjs()")
        ok = ZSIpcTracePy.IpcTrcServer_recallAdminObjs(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".recallAdminObjs(): " + str(ok))
        return ok

    def saveAdminObjs(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".saveAdminObjs()")
        ok = ZSIpcTracePy.IpcTrcServer_saveAdminObjs(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".saveAdminObjs(): " + str(ok))
        return ok

    def setUseLocalTrcFile(self, used):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setUseLocalTrcFile(" + str(used) + ")")
        ZSIpcTracePy.IpcTrcServer_setUseLocalTrcFile(used, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setUseLocalTrcFile()")

    def isLocalTrcFileUsed(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isLocalTrcFileUsed()")
        used = ZSIpcTracePy.IpcTrcServer_isLocalTrcFileUsed(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isLocalTrcFileUsed(): " + str(used))
        return used

    def setLocalTrcFileAbsoluteFilePath(self, absFilePath):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileAbsoluteFilePath(" + str(absFilePath) + ")")
        ZSIpcTracePy.IpcTrcServer_setLocalTrcFileAbsoluteFilePath(absFilePath, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileAbsoluteFilePath()")

    def getLocalTrcFileAbsoluteFilePath(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAbsoluteFilePath()")
        absFilePath = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileAbsoluteFilePath(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAbsoluteFilePath(): " + str(absFilePath))
        return absFilePath

    def getLocalTrcFileCompleteBaseName(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileCompleteBaseName()")
        baseName = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileCompleteBaseName(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileCompleteBaseName(): " + str(baseName))
        return baseName

    def getLocalTrcFileAbsolutePath(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAbsolutePath()")
        absFilePath = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileAbsolutePath(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAbsolutePath(): " + str(absFilePath))
        return absFilePath

    def isLocalTrcFileActive(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isLocalTrcFileActive()")
        active = ZSIpcTracePy.IpcTrcServer_isLocalTrcFileActive(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isLocalTrcFileActive(): " + str(active))
        return active

    def setLocalTrcFileAutoSaveInterval(self, autoSaveInterval_ms):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileAutoSaveInterval(" + str(autoSaveInterval_ms) + " ms)")
        ZSIpcTracePy.IpcTrcServer_setLocalTrcFileAutoSaveInterval(autoSaveInterval_ms, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileAutoSaveInterval()")

    def getLocalTrcFileAutoSaveInterval(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAutoSaveInterval()")
        autoSaveInterval_ms = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileAutoSaveInterval(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileAutoSaveInterval(): " + str(autoSaveInterval_ms) + " ms") 
        return autoSaveInterval_ms

    def setLocalTrcFileCloseFileAfterEachWrite(self, closeFile):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileCloseFileAfterEachWrite(" + str(closeFile) + ")")
        ZSIpcTracePy.IpcTrcServer_setLocalTrcFileCloseFileAfterEachWrite(closeFile, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileCloseFileAfterEachWrite()")

    def getLocalTrcFileCloseFileAfterEachWrite(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileCloseFileAfterEachWrite()")
        closeFile = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileCloseFileAfterEachWrite(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileCloseFileAfterEachWrite(): " + str(closeFile))
        return closeFile

    def setLocalTrcFileSubFileCountMax(self, countMax):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileSubFileCountMax(" + str(countMax) + ")")
        ZSIpcTracePy.IpcTrcServer_setLocalTrcFileSubFileCountMax(countMax, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileSubFileCountMax()")

    def getLocalTrcFileSubFileCountMax(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileSubFileCountMax()")
        countMax = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileSubFileCountMax(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileSubFileCountMax(): " + str(countMax))
        return countMax

    def setLocalTrcFileSubFileLineCountMax(self, countMax):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileSubFileLineCountMax(" + str(countMax) + ")")
        ZSIpcTracePy.IpcTrcServer_setLocalTrcFileSubFileLineCountMax(countMax, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setLocalTrcFileSubFileLineCountMax()")

    def getLocalTrcFileSubFileLineCountMax(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileSubFileLineCountMax()")
        countMax = ZSIpcTracePy.IpcTrcServer_getLocalTrcFileSubFileLineCountMax(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getLocalTrcFileSubFileLineCountMax(): " + str(countMax))
        return countMax

    def setCacheTrcDataIfNotConnected(self, cacheData):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setCacheTrcDataIfNotConnected(" + str(cacheData) + ")")
        ZSIpcTracePy.IpcTrcServer_setCacheTrcDataIfNotConnected(cacheData, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setCacheTrcDataIfNotConnected()")

    def getCacheTrcDataIfNotConnected(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getCacheTrcDataIfNotConnected()")
        cacheData = ZSIpcTracePy.IpcTrcServer_getCacheTrcDataIfNotConnected(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getCacheTrcDataIfNotConnected(): " + str(cacheData))
        return cacheData

    def setCacheTrcDataMaxArrLen(self, maxArrLen):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setCacheTrcDataMaxArrLen(" + str(maxArrLen) + ")")
        ZSIpcTracePy.IpcTrcServer_setCacheTrcDataMaxArrLen(maxArrLen, self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setCacheTrcDataMaxArrLen()")

    def getCacheTrcDataMaxArrLen(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getCacheTrcDataMaxArrLen()")
        maxArrLen = ZSIpcTracePy.IpcTrcServer_getCacheTrcDataMaxArrLen(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getCacheTrcDataMaxArrLen(): " + str(maxArrLen))
        return maxArrLen

    def clearLocalTrcFile(self):
        myPrint("-> <CIpcTrcServer> " + str(self.objectName) + ".clearLocalTrcFile()")
        ZSIpcTracePy.IpcTrcServer_clearLocalTrcFile(self.objectName, timeout_ms, wait)
        myPrint("<- <CIpcTrcServer> " + str(self.objectName) + ".clearLocalTrcFile()")

    def startup(self, timeout_ms = 5000, wait = True):
        myPrint("-> <CIpcTrcServer> " + str(self.objectName) + ".startup(Timeout: " + str(timeout_ms) + " ms, Wait: " + str(wait) + ")")
        bOk = ZSIpcTracePy.IpcTrcServer_startup(self.objectName, timeout_ms, wait)
        myPrint("<- <CIpcTrcServer> " + str(self.objectName) + ".startup(): " + str(bOk))
        return bOk

    def shutdown(self, timeout_ms = 5000, wait = True):
        myPrint("-> <CIpcTrcServer> " + str(self.objectName) + ".shutdown(Timeout: " + str(timeout_ms) + " ms, Wait: " + str(wait) + ")")
        bOk = ZSIpcTracePy.IpcTrcServer_shutdown(self.objectName, timeout_ms, wait)
        myPrint("<- <CIpcTrcServer> " + str(self.objectName) + ".shutdown(): " + str(bOk))
        return bOk

    def isListening(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isListening()")
        listening = ZSIpcTracePy.IpcTrcServer_isListening(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isListening(): " + str(listening))
        return listening

    def isConnected(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".isConnected()")
        connected = ZSIpcTracePy.IpcTrcServer_isConnected(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".isConnected(): " + str(connected))
        return connected

    def setPort(self, port, timeout_ms = 5000, wait = True):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".setPort(Port: " + str(port) + ", Timeout: " + str(timeout_ms) + " ms, Wait: " + str(wait) + ")")
        ok = ZSIpcTracePy.IpcTrcServer_setPort(port, self.__objName, timeout_ms, wait)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".setPort()")
        return ok

    def getPort(self):
        myPrint("-> <CIpcTrcServer> " + str(self.__objName) + ".getPort()")
        port = ZSIpcTracePy.IpcTrcServer_getPort(self.__objName)
        myPrint("<- <CIpcTrcServer> " + str(self.__objName) + ".getPort(): " + str(port))
        return port


