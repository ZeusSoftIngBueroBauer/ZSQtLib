#-------------------------------------------------------------------------------
# Copyright (c) 2015 Dialog Semiconductor GmbH, All rights reserved.
# 
# Created on 20.07.2015
#
# Author: Christian Bauer
#
#-------------------------------------------------------------------------------
'''
'''

import argparse
import sys
import traceback

from PySide import QtGui, QtCore

from ZSQtLib import ZSSys
from ZSQtLib import ZSIpc
from ZSQtLib import ZSTrace
from ZSQtLib import ZSRemCmd

from htol_util import Images

from main_window import CMainWindow

#*******************************************************************************
class CApp(QtGui.QApplication):     # pylint: disable=no-member
#*******************************************************************************
    '''
    '''

    #===========================================================================
    # class members
    #===========================================================================

    _instance = None

    #===========================================================================
    # class methods
    #===========================================================================

    @staticmethod
    #---------------------------------------------------------------------------
    def instance():
    #---------------------------------------------------------------------------
        '''
        '''
        return CApp._instance

    #===========================================================================
    # ctor
    #===========================================================================

    #---------------------------------------------------------------------------
    def __init__( self, args, strOrgName, strOrgDomain, strAppName, strMainWindowTitle, strConfigDir, strLogDir ):    # pylint: disable=too-many-arguments
    #---------------------------------------------------------------------------
        '''
        '''
        QtGui.QApplication.__init__( self, sys.argv )   # pylint: disable=no-member

        #: Object name of the instance.
        self._name = "theApp"

        self.setObjectName("theApp")

        if len(self.thread().objectName()) == 0:
            self.thread().setObjectName("GUIMain")

        strMethod = self._name + ".__init__"

        # The application is a singleton class. Store reference to the created instance in class variable.
        CApp._instance = self

        # Set organization name, internet domain and name of application at the base class.
        QtCore.QCoreApplication.setOrganizationName(strOrgName)         # pylint: disable=no-member
        QtCore.QCoreApplication.setOrganizationDomain(strOrgDomain)     # pylint: disable=no-member
        QtCore.QCoreApplication.setApplicationName(strAppName)          # pylint: disable=no-member

        # Instance members
        #------------------

        self._strConfigDir = strConfigDir

        if self._strConfigDir is None or len(self._strConfigDir) == 0:
            self._strConfigDir = QtCore.QCoreApplication.applicationDirPath()
            if self._strConfigDir.lower().endswith("bin"):
                self._strConfigDir = self._strConfigDir[:-3]
                self._strConfigDir += "Config"
        if not self._strConfigDir.endswith("/") and not self._strConfigDir.endswith("\\"):
            self._strConfigDir += "/"

        self._strLogDir = strLogDir

        if self._strLogDir is None or len(self._strLogDir) == 0:
            self._strLogDir = QtCore.QCoreApplication.applicationDirPath()
            if self._strLogDir.lower().endswith("bin"):
                self._strLogDir = self._strLogDir[:-3]
                self._strLogDir += "Log"
        if not self._strLogDir.endswith("/") and not self._strLogDir.endswith("\\"):
            self._strLogDir += "/"

        self._settingsFile = None

        #: Name of the error log file. This allows that the error log file
        #: might be indicated e.g. by the central widget.
        self._strErrLogFileName = self._strLogDir + strAppName + "-Error" + ".xml" 

        #: Error log instance. Used to store runtime errors allowing to indicate all
        #: logged errors in a table view within the GUI.
        self._errLog = None

        self._trcServerHostSettings = ZSIpc.SServerHostSettings()
        self._trcServerHostSettings.m_socketType = ZSIpc.ESocketTypeTcp
        self._trcServerHostSettings.m_strLocalHostName = "127.0.0.1"
        #self._trcServerHostSettings.m_hostAddrLocal = QHostAddress()
        self._trcServerHostSettings.m_uLocalPort = 24763
        self._trcServerHostSettings.m_uMaxPendingConnections = 30
        self._trcServerHostSettings.m_uBufferSize = 4096

        self._trcServerSettings = ZSSys.Trace.STrcServerSettings()
        self._trcServerSettings.m_bEnabled = True
        self._trcServerSettings.m_bCacheDataIfNotConnected = False
        self._trcServerSettings.m_iCacheDataMaxArrLen = 1000
        self._trcServerSettings.m_strAdminObjFileName = self._strConfigDir + strAppName + "-TrcAdminObj" + ".xml"
        self._trcServerSettings.m_bUseLocalTrcFile = True
        self._trcServerSettings.m_strLocalTrcFileName = self._strLogDir + strAppName + ".log"
        self._trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms = 1000
        self._trcServerSettings.m_iLocalTrcFileCountMax = 5
        self._trcServerSettings.m_iLocalTrcLineCountMax = 2000
        self._trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = False

        self._trcServer = None

        self._remCmdClientHostSettings = ZSIpc.SClientHostSettings()
        self._remCmdClientHostSettings.m_socketType = ZSIpc.ESocketTypeTcp
        self._remCmdClientHostSettings.m_strRemoteHostName = "127.0.0.1"
        self._remCmdClientHostSettings.m_uRemotePort = 26297
        self._remCmdClientHostSettings.m_iConnectTimeout_ms = 5000
        self._remCmdClientHostSettings.m_uBufferSize = 4096

        self._remCmdClientWatchDogSettings = ZSIpc.STimerSettings()
        self._remCmdClientWatchDogSettings.m_bEnabled = False
        self._remCmdClientWatchDogSettings.m_iInterval_ms = 5000
        self._remCmdClientWatchDogSettings.m_iTimeout_ms = 11000

        self._remCmdClient = None

        self._bReqExecTreeGarbageCollectorEnabled = True
        self._fReqExecTreeGarbageCollectorInterval_s = 5.0
        self._fReqExecTreeGarbageCollectorElapsed_s = 60.0
        self._reqExecTree = None

        self._mainWindow = None

        self._trcAdminObj = None

        # Command arguments
        #------------------

        parser = argparse.ArgumentParser(
            description='Remote Command Client Example Command Arguments',
            formatter_class=argparse.RawTextHelpFormatter)

        #strHelp = 'Specifies the directory where the ini file is located:\n' \
        #          'User:\t user-specific (<Users>/<UserName>/AppData/Roaming/<OrgName>)\n' \
        #          'System:\t shared by all users of the same system\n' \
        #          'AppDir:\t relative to the applications program path.'

        #parser.add_argument(
        #    '--ini_file_scope',
        #    default='User',
        #    choices=['User', 'System', 'AppDir'],
        #    help=strHelp)

        args = parser.parse_args()

        # Evaluate command arguments (first part, IniFile)
        #-------------------------------------------------

        strIniFileName = self._strConfigDir + strAppName + ".ini"
    
        # Read settings file
        #---------------------

        self._settingsFile = QtCore.QSettings( strIniFileName, QtCore.QSettings.IniFormat )

        self.read_settings()  # Trace Settings, Remote Command Client Settings, etc.

        # Evaluate command arguments (second part, overwriting IniFile settings)
        #-----------------------------------------------------------------------

        # Error Manager
        #---------------

        #: Error log instance. Used to store runtime errors allowing to indicate all
        #: logged errors in a table view within the GUI.
        self._errLog = ZSSys.CErrLog( True, self._strErrLogFileName )

        # Request Execution Tree
        #------------------------

        self._reqExecTree = ZSSys.CRequestExecTree.createInstance()

        self._reqExecTree.setGarbageCollectorEnabled(self._bReqExecTreeGarbageCollectorEnabled)
        self._reqExecTree.setGarbageCollectorIntervalInSec(self._fReqExecTreeGarbageCollectorInterval_s)
        self._reqExecTree.setGarbageCollectorElapsedInSec(self._fReqExecTreeGarbageCollectorElapsed_s)

        # Create trace server
        #------------------------

        trcServerHostSettingsDefault = ZSIpc.SServerHostSettings()
        trcServerHostSettingsDefault.m_socketType = ZSIpc.ESocketTypeTcp
        trcServerHostSettingsDefault.m_strLocalHostName = "127.0.0.1"
        #trcServerHostSettingsDefault.m_hostAddrLocal = QHostAddress()
        trcServerHostSettingsDefault.m_uLocalPort = 24763
        trcServerHostSettingsDefault.m_uMaxPendingConnections = 30
        trcServerHostSettingsDefault.m_uBufferSize = 4096

        trcServerSettingsDefault = ZSSys.Trace.STrcServerSettings()
        trcServerSettingsDefault.m_bEnabled = True
        trcServerSettingsDefault.m_bCacheDataIfNotConnected = False
        trcServerSettingsDefault.m_iCacheDataMaxArrLen = 1000
        trcServerSettingsDefault.m_strAdminObjFileName = self._strConfigDir + strAppName + "-TrcAdminObj" + ".xml"
        trcServerSettingsDefault.m_bUseLocalTrcFile = True
        trcServerSettingsDefault.m_strLocalTrcFileName = self._strLogDir + strAppName + ".log"
        trcServerSettingsDefault.m_iLocalTrcFileAutoSaveInterval_ms = 1000
        trcServerSettingsDefault.m_iLocalTrcFileCountMax = 5
        trcServerSettingsDefault.m_iLocalTrcLineCountMax = 2000
        trcServerSettingsDefault.m_bLocalTrcFileCloseFileAfterEachWrite = False

        self._trcServer = ZSTrace.CTrcServer(
            ZSSys.Trace.ETraceTypeMth,
            "TrcMthServer",
            trcServerHostSettingsDefault,
            trcServerSettingsDefault )

        self._trcServer.setHostSettings(self._trcServerHostSettings)
        self._trcServer.setTraceSettings(self._trcServerSettings)
        self._trcServer.recallAdminObjs()
        self._trcServer.startup()

        self._trcAdminObj = ZSTrace.CTrcServer.getTrcAdminObj(
            ZSSys.Trace.ETraceTypeMth,
            "Dialog::LTT::RemCmdClient",
            "CApp",
            "",
            self.objectName() )

        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'ctor', '' )

        # Remote command client
        #----------------------

        remCmdClientHostSettingsDefault = ZSIpc.SClientHostSettings()
        remCmdClientHostSettingsDefault.m_socketType = ZSIpc.ESocketTypeTcp
        remCmdClientHostSettingsDefault.m_strRemoteHostName = "127.0.0.1"
        remCmdClientHostSettingsDefault.m_uRemotePort = 26297
        remCmdClientHostSettingsDefault.m_iConnectTimeout_ms = 5000
        remCmdClientHostSettingsDefault.m_uBufferSize = 4096

        remCmdClientWatchDogSettingsDefault = ZSIpc.STimerSettings()
        remCmdClientWatchDogSettingsDefault.m_bEnabled = False
        remCmdClientWatchDogSettingsDefault.m_iInterval_ms = 5000
        remCmdClientWatchDogSettingsDefault.m_iTimeout_ms = 11000

        #: Remote command client
        self._remCmdClient = ZSRemCmd.CRemCmdClient(
            "RemCmdClient", ":", " ", ",",
            remCmdClientHostSettingsDefault,
            remCmdClientWatchDogSettingsDefault )

        self._remCmdClient.setHostSettings(self._remCmdClientHostSettings)
        self._remCmdClient.setWatchDogTimerSettings(self._remCmdClientWatchDogSettings)
        self._remCmdClient.changeSettings()

        # <MainWindow>
        #-------------

        Images.loadImages()
        pngDat = '''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'''
        img = QtGui.QImage.fromData(QtCore.QByteArray.fromBase64(pngDat),'PNG')
        pxm = QtGui.QPixmap.fromImage(img)
        iconApp = QtGui.QIcon(pxm)
        self.setWindowIcon(iconApp)
        self.setQuitOnLastWindowClosed(False)
        self.lastWindowClosed.connect(self.onLastWindowClosed)

        #: Main window of the client application.
        self._mainWindow = CMainWindow( strMainWindowTitle, self._remCmdClient, self )
        self._mainWindow.setMinimumWidth(400)
        self._mainWindow.setMinimumHeight(120)
        self._mainWindow.show()

    #==============================================================================
    # public: // instance methods
    #==============================================================================

    def read_settings(self):

        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'read_settings', '' )

        if self._settingsFile is not None:

            # Request Execution Tree
            #=======================================================

            strSettingsKey = "ReqExecTree"
            bSyncSettings  = False

            if self._settingsFile.contains(strSettingsKey+"/GarbageCollectorEnabled"):
                self._bReqExecTreeGarbageCollectorEnabled = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/GarbageCollectorEnabled",str(self._bReqExecTreeGarbageCollectorEnabled)) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/GarbageCollectorEnabled", str(self._bReqExecTreeGarbageCollectorEnabled) )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/GarbageCollectorIntervalInSec"):
                self._fReqExecTreeGarbageCollectorInterval_s = float( self._settingsFile.value(strSettingsKey+"/GarbageCollectorIntervalInSec",self._fReqExecTreeGarbageCollectorInterval_s) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/GarbageCollectorIntervalInSec",self._fReqExecTreeGarbageCollectorInterval_s)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/GarbageCollectorElapsedInSec"):
                self._fReqExecTreeGarbageCollectorElapsed_s = float( self._settingsFile.value(strSettingsKey+"/GarbageCollectorElapsedInSec",self._fReqExecTreeGarbageCollectorElapsed_s) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/GarbageCollectorElapsedInSec",self._fReqExecTreeGarbageCollectorElapsed_s)
                bSyncSettings = True

            # Update settings file
            #---------------------

            if bSyncSettings:
                self._settingsFile.sync()

            # Trace Server
            #=======================================================
    
            strSettingsKey = "TrcMthServer"
            bSyncSettings = False

            # Host Settings
            #--------------

            if self._settingsFile.contains(strSettingsKey+"/SocketType"):
                self._trcServerHostSettings.m_socketType = ZSIpc.str_to_socket_type( self._settingsFile.value(strSettingsKey+"/SocketType",ZSIpc.socket_type_to_str(self._trcServerHostSettings.m_socketType)) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/SocketType", ZSIpc.socket_type_to_str(self._trcServerHostSettings.m_socketType) )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalHostName"):
                self._trcServerHostSettings.m_strLocalHostName = self._settingsFile.value(strSettingsKey+"/LocalHostName",self._trcServerHostSettings.m_strLocalHostName)
            else:
                self._settingsFile.setValue( strSettingsKey+"/LocalHostName", self._trcServerHostSettings.m_strLocalHostName )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalPort"):
                self._trcServerHostSettings.m_uLocalPort = int( self._settingsFile.value(strSettingsKey+"/LocalPort",self._trcServerHostSettings.m_uLocalPort) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/LocalPort", self._trcServerHostSettings.m_uLocalPort )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/MaxPendingConnections"):
                self._trcServerHostSettings.m_uMaxPendingConnections = int( self._settingsFile.value(strSettingsKey+"/MaxPendingConnections",self._trcServerHostSettings.m_uMaxPendingConnections) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/MaxPendingConnections", self._trcServerHostSettings.m_uMaxPendingConnections )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/BufferSize"):
                self._trcServerHostSettings.m_uBufferSize = int( self._settingsFile.value(strSettingsKey+"/BufferSize",self._trcServerHostSettings.m_uBufferSize) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/BufferSize", self._trcServerHostSettings.m_uBufferSize )
                bSyncSettings = True

            # Trace Settings
            #---------------

            if self._settingsFile.contains(strSettingsKey+"/Enabled"):
                self._trcServerSettings.m_bEnabled = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/Enabled",str(self._trcServerSettings.m_bEnabled)) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/Enabled",str(self._trcServerSettings.m_bEnabled))
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/CacheDataIfNotConnected"):
                self._trcServerSettings.m_bCacheDataIfNotConnected = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/CacheDataIfNotConnected",str(self._trcServerSettings.m_bCacheDataIfNotConnected)) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/CacheDataIfNotConnected",str(self._trcServerSettings.m_bCacheDataIfNotConnected))
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/CacheDataMaxArrLen"):
                self._trcServerSettings.m_iCacheDataMaxArrLen = int( self._settingsFile.value(strSettingsKey+"/CacheDataMaxArrLen",self._trcServerSettings.m_iCacheDataMaxArrLen) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/CacheDataMaxArrLen",self._trcServerSettings.m_iCacheDataMaxArrLen)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/AdminObjFileName"):
                self._trcServerSettings.m_strAdminObjFileName = self._settingsFile.value(strSettingsKey+"/AdminObjFileName",self._trcServerSettings.m_strAdminObjFileName)
            else:
                self._settingsFile.setValue(strSettingsKey+"/AdminObjFileName",self._trcServerSettings.m_strAdminObjFileName)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/UseLocalTrcFile"):
                self._trcServerSettings.m_bUseLocalTrcFile = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/UseLocalTrcFile",str(self._trcServerSettings.m_bUseLocalTrcFile)) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/UseLocalTrcFile",str(self._trcServerSettings.m_bUseLocalTrcFile))
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalTrcFileName"):
                self._trcServerSettings.m_strLocalTrcFileName = self._settingsFile.value(strSettingsKey+"/LocalTrcFileName",self._trcServerSettings.m_strLocalTrcFileName)
            else:
                self._settingsFile.setValue(strSettingsKey+"/LocalTrcFileName",self._trcServerSettings.m_strLocalTrcFileName)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs"):
                self._trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms = int( self._settingsFile.value(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs",self._trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs",self._trcServerSettings.m_iLocalTrcFileAutoSaveInterval_ms)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalTrcFileCountMax"):
                self._trcServerSettings.m_iLocalTrcFileCountMax = int( self._settingsFile.value(strSettingsKey+"/LocalTrcFileCountMax",self._trcServerSettings.m_iLocalTrcFileCountMax) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/LocalTrcFileCountMax",self._trcServerSettings.m_iLocalTrcFileCountMax)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalTrcLineCountMax"):
                self._trcServerSettings.m_iLocalTrcLineCountMax = int( self._settingsFile.value(strSettingsKey+"/LocalTrcLineCountMax",self._trcServerSettings.m_iLocalTrcLineCountMax) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/LocalTrcLineCountMax",self._trcServerSettings.m_iLocalTrcLineCountMax)
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite"):
                self._trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite = ZSSys.str_to_bool( self._settingsFile.value( strSettingsKey + "/LocalTrcFileCloseFileAfterEachWrite", str(self._trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite) ) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite",str(self._trcServerSettings.m_bLocalTrcFileCloseFileAfterEachWrite))
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/Enabled"):
                self._trcServerSettings.m_bEnabled = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/Enabled",str(self._trcServerSettings.m_bEnabled)) )
            else:
                self._settingsFile.setValue(strSettingsKey+"/Enabled",str(self._trcServerSettings.m_bEnabled))
                bSyncSettings = True

            # Update settings file
            #---------------------

            if bSyncSettings:
                self._settingsFile.sync()

            # Remote Command Client
            #=======================================================

            strSettingsKey = "RemCmdClient"

            # Host Settings
            #--------------

            if self._settingsFile.contains(strSettingsKey+"/SocketType"):
                self._remCmdClientHostSettings.m_socketType = ZSIpc.str_to_socket_type( self._settingsFile.value(strSettingsKey+"/SocketType",ZSIpc.socket_type_to_str(self._remCmdClientHostSettings.m_socketType)) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/SocketType", ZSIpc.socket_type_to_str(self._remCmdClientHostSettings.m_socketType) )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/RemoteHostName"):
                self._remCmdClientHostSettings.m_strRemoteHostName = self._settingsFile.value(strSettingsKey+"/RemoteHostName",self._remCmdClientHostSettings.m_strRemoteHostName)
            else:
                self._settingsFile.setValue( strSettingsKey+"/RemoteHostName", self._remCmdClientHostSettings.m_strRemoteHostName )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/RemotePort"):
                self._remCmdClientHostSettings.m_uRemotePort = int( self._settingsFile.value(strSettingsKey+"/RemotePort",self._remCmdClientHostSettings.m_uRemotePort) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/RemotePort", self._remCmdClientHostSettings.m_uRemotePort )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/ConnectTimeoutInMs"):
                self._remCmdClientHostSettings.m_iConnectTimeout_ms = int( self._settingsFile.value(strSettingsKey+"/ConnectTimeoutInMs",self._remCmdClientHostSettings.m_iConnectTimeout_ms) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/ConnectTimeoutInMs", self._remCmdClientHostSettings.m_iConnectTimeout_ms )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/BufferSize"):
                self._remCmdClientHostSettings.m_uBufferSize = int( self._settingsFile.value(strSettingsKey+"/BufferSize",self._remCmdClientHostSettings.m_uBufferSize) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/BufferSize", self._remCmdClientHostSettings.m_uBufferSize )
                bSyncSettings = True

            # Watch Dog Timer Settings
            #-------------------------

            if self._settingsFile.contains(strSettingsKey+"/WatchDogTimerEnabled"):
                self._remCmdClientWatchDogSettings.m_bEnabled = ZSSys.str_to_bool( self._settingsFile.value(strSettingsKey+"/WatchDogTimerEnabled",str(self._remCmdClientWatchDogSettings.m_bEnabled)) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerEnabled", str(self._remCmdClientWatchDogSettings.m_bEnabled) )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/WatchDogTimerInterval_ms"):
                self._remCmdClientWatchDogSettings.m_iInterval_ms = int( self._settingsFile.value(strSettingsKey+"/WatchDogTimerInterval_ms",self._remCmdClientWatchDogSettings.m_iInterval_ms) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerInterval_ms", self._remCmdClientWatchDogSettings.m_iInterval_ms )
                bSyncSettings = True
            if self._settingsFile.contains(strSettingsKey+"/WatchDogTimerTimeout_ms"):
                self._remCmdClientWatchDogSettings.m_iTimeout_ms = int( self._settingsFile.value(strSettingsKey+"/WatchDogTimerTimeout_ms",self._remCmdClientWatchDogSettings.m_iTimeout_ms) )
            else:
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerTimeout_ms", self._remCmdClientWatchDogSettings.m_iTimeout_ms )
                bSyncSettings = True

            # Update settings file
            #---------------------

            if bSyncSettings:
                self._settingsFile.sync()

    def save_settings(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'save_settings', '' )
        if self._settingsFile is not None:
            if self._reqExecTree is not None:
                strSettingsKey = "ReqExecTree"
                bGarbageCollectorEnabled    = self._reqExecTree.isGarbageCollectorEnabled()
                fGarbageCollectorInterval_s = self._reqExecTree.getGarbageCollectorIntervalInSec()
                fGarbageCollectorElapsed_s  = self._reqExecTree.getGarbageCollectorElapsedInSec()
                self._settingsFile.setValue( strSettingsKey+"/GarbageCollectorEnabled", str(bGarbageCollectorEnabled) )
                self._settingsFile.setValue(strSettingsKey+"/GarbageCollectorIntervalInSec",fGarbageCollectorInterval_s)
                self._settingsFile.setValue(strSettingsKey+"/GarbageCollectorElapsedInSec",fGarbageCollectorElapsed_s)
            if self._trcServer is not None:
                strSettingsKey = "TrcMthServer"
                hostSettings = self._trcServer.getHostSettings()
                self._settingsFile.setValue( strSettingsKey+"/SocketType", ZSIpc.socket_type_to_str(hostSettings.m_socketType) )
                self._settingsFile.setValue( strSettingsKey+"/LocalHostName", hostSettings.m_strLocalHostName )
                self._settingsFile.setValue( strSettingsKey+"/LocalPort", hostSettings.m_uLocalPort )
                self._settingsFile.setValue( strSettingsKey+"/MaxPendingConnections", hostSettings.m_uMaxPendingConnections )
                self._settingsFile.setValue( strSettingsKey+"/BufferSize", hostSettings.m_uBufferSize )
                trcSettings  = self._trcServer.getTraceSettings()
                self._settingsFile.setValue( strSettingsKey+"/Enabled", str(trcSettings.m_bEnabled) )
                self._settingsFile.setValue( strSettingsKey+"/CacheDataIfNotConnected", str(trcSettings.m_bCacheDataIfNotConnected) )
                self._settingsFile.setValue( strSettingsKey+"/CacheDataMaxArrLen", str(trcSettings.m_iCacheDataMaxArrLen) )
                self._settingsFile.setValue( strSettingsKey+"/AdminObjFileName", trcSettings.m_strAdminObjFileName )
                self._settingsFile.setValue( strSettingsKey+"/UseLocalTrcFile", str(trcSettings.m_bUseLocalTrcFile) )
                self._settingsFile.setValue( strSettingsKey+"/LocalTrcFileName", trcSettings.m_strLocalTrcFileName )
                self._settingsFile.setValue( strSettingsKey+"/LocalTrcFileAutoSaveIntervalInMs", trcSettings.m_iLocalTrcFileAutoSaveInterval_ms )
                self._settingsFile.setValue( strSettingsKey+"/LocalTrcFileCountMax", trcSettings.m_iLocalTrcFileCountMax )
                self._settingsFile.setValue( strSettingsKey+"/LocalTrcLineCountMax", trcSettings.m_iLocalTrcLineCountMax )
                self._settingsFile.setValue( strSettingsKey+"/LocalTrcFileCloseFileAfterEachWrite", str(trcSettings.m_bLocalTrcFileCloseFileAfterEachWrite) )
                self._settingsFile.sync()
            if self._remCmdClient is not None:
                strSettingsKey = "RemCmdClient"
                hostSettings = self._remCmdClient.getHostSettings()
                self._settingsFile.setValue( strSettingsKey+"/SocketType", ZSIpc.socket_type_to_str(hostSettings.m_socketType) )
                self._settingsFile.setValue( strSettingsKey+"/RemoteHostName", hostSettings.m_strRemoteHostName )
                self._settingsFile.setValue( strSettingsKey+"/RemotePort", hostSettings.m_uRemotePort )
                self._settingsFile.setValue( strSettingsKey+"/ConnectTimeoutInMs", hostSettings.m_iConnectTimeout_ms )
                self._settingsFile.setValue( strSettingsKey+"/BufferSize", hostSettings.m_uBufferSize )
                watchDogSettings = self._remCmdClient.getWatchDogTimerSettings()
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerEnabled", str(watchDogSettings.m_bEnabled) )
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerInterval_ms", watchDogSettings.m_iInterval_ms )
                self._settingsFile.setValue( strSettingsKey+"/WatchDogTimerTimeout_ms", watchDogSettings.m_iTimeout_ms )
                self._settingsFile.sync()

    #===========================================================================
    # overridables of base class QtGui.QApplication
    #===========================================================================

    #---------------------------------------------------------------------------
    def onLastWindowClosed(self):
    #---------------------------------------------------------------------------
        '''
        Slot method called if the application quits. Invokes the shutdown method
        of the application.
        '''
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onLastWindowClosed', '' )
        if self._trcServer is not None:
            self._trcServer.saveAdminObjs()
        self.save_settings()
        self.quit() 

    #---------------------------------------------------------------------------
    def notify( self, objReceiver, ev ):
    #---------------------------------------------------------------------------
        '''
        Overwrites method of QCoreApplication base class to catch and print
        exceptions occurred while processing events.
        '''
        bResult = False
        try:
            bResult = QtGui.QApplication.notify(self,objReceiver,ev)    # pylint: disable=no-member
        except:     # pylint: disable=bare-except
            traceback.print_exc()
        return bResult

