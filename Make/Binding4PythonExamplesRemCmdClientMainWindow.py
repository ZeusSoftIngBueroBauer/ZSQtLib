#-------------------------------------------------------------------------------
# Copyright (c) 2015 Dialog Semiconductor GmbH, All rights reserved.
# 
# Created on 25.05.2015
#
# Author: Christian Bauer
#
#-------------------------------------------------------------------------------
'''
'''

import sys

from PySide import QtGui, QtCore

from ZSQtLib import ZSSys
from ZSQtLib import ZSIpc
from ZSQtLib import ZSTrace
from ZSQtLib import ZSRemCmd
from ZSQtLib import ZSSysGUI
from ZSQtLib import ZSIpcGUI
from ZSQtLib import ZSTraceGUI

from htol_util.ErrResult import EResultSeverity
from htol_util import Images

from widget_central import CWidgetCentral


#*******************************************************************************
class CMainWindow(QtGui.QMainWindow):
#*******************************************************************************
    '''
    '''

    #===========================================================================
    # class members
    #===========================================================================

    _instance = None

    #===========================================================================
    # ctor
    #===========================================================================

    #---------------------------------------------------------------------------
    def __init__( self, strWindowTitle, remCmdClient, theApp, parent = None ):
    #---------------------------------------------------------------------------
        QtGui.QMainWindow.__init__(self,parent)

        CMainWindow._instance = self

        self.setWindowTitle(strWindowTitle)

        # Instance members
        #=============================

        self._name = "theMainWindow"
        self._theApp = theApp
        self._errLog = self._theApp._errLog
        self._settingsFile = None
        self._remCmdClient = remCmdClient
        self._mnuFile = None
        self._actFileQuit = None
        self._mnuSettings = None
        self._actSettingsRemCmdClient = None
        self._actSettingsFileInfo = None
        self._mnuDebug = None
        self._actDebugTrcServer = None
        self._actDebugTrcAdminObjPool = None
        self._actDebugErrLog = None
        self._actDebugRequestExecTree = None
        self._mnuInfo = None
        self._actInfoVersion = None
        self._statusBar = None
        self._lblReqInProgress = None
        self._barReqInProgress = None
        self._lblErrors = None
        self._wdgtRemCmdClientCnctStatus = None
        self._lblRemCmdClientCnctStatusIcon = None
        self._wdgtCentral = None
        self._dlgErrLog = None
        self._dlgRequestExecTree = None
        self._dlgRemCmdClient = None
        self._dlgTrcServer = None
        self._dlgTrcAdminObjPool = None
        self._trcAdminObj = None

        self.setObjectName(self._name)

        strMethod = self._name + ".__init__"

        self._trcAdminObj = ZSTrace.CTrcServer.getTrcAdminObj(
            ZSSys.Trace.ETraceTypeMth,
            "Dialog::LTT::RemCmdClient",
            "CMainWindow",
            "",
            self.objectName() )

        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'ctor', '' )

        if self._remCmdClient is not None:
            self._remCmdClient.destroyed.connect(self.onRemCmdClientDestroyed)
            self._remCmdClient.stateChanged.connect(self.onRemCmdClientStateChanged)

        # <Menu> File
        #=============================

        self._mnuFile = self.menuBar().addMenu("&File")

        # <MenuItem> File::Quit
        #----------------------

        self._actFileQuit = QtGui.QAction( "&Quit", self, shortcut="Ctrl+Q", statusTip="Quit Application", triggered=self._theApp.onLastWindowClosed )
        self._mnuFile.addAction(self._actFileQuit)

        # <Menu> Settings
        #=============================

        self._mnuSettings = self.menuBar().addMenu("&Settings")

        # <MenuItem> Settings::RemCmdClient
        #----------------------------------

        if self._remCmdClient is not None:
            self._actSettingsRemCmdClient = QtGui.QAction("&Remote Command Client",self)
            self._mnuSettings.addAction(self._actSettingsRemCmdClient)
            self._actSettingsRemCmdClient.triggered.connect(self.onActSettingsRemCmdClientTriggered)

        # <MenuItem> Settings::Settings File Info
        #----------------------------------------

        self._settingsFile = self._theApp._settingsFile
        if self._settingsFile is not None:
            strActionSettingsFileInfo = "Settings File: " + self._settingsFile.fileName()
            self._actSettingsFileInfo = QtGui.QAction(strActionSettingsFileInfo,self)
            self._mnuSettings.addAction(self._actSettingsFileInfo)

        # <Menu> Debug
        #=============================

        self._mnuDebug = self.menuBar().addMenu("&Debug")

        # <MenuItem> Debug::TraceServer
        #----------------------------------

        if self._theApp._trcServer is not None:

            self._actDebugTrcServer = QtGui.QAction("&Trace Server",self)
            self._mnuDebug.addAction(self._actDebugTrcServer)
            self._actDebugTrcServer.triggered.connect(self.onActDebugTrcServerTriggered)

        # <MenuItem> Debug::TraceAdminObjPool
        #---------------------------------------

        if self._theApp._trcServer is not None:

            self._actDebugTrcAdminObjPool = QtGui.QAction("&Trace Admin Objects",self)
            self._mnuDebug.addAction(self._actDebugTrcAdminObjPool)
            self._actDebugTrcAdminObjPool.triggered.connect(self.onActDebugTrcAdminObjPoolTriggered)

        # <MenuItem> Debug::Error Log
        #----------------------------

        if self._errLog is not None:
            iconErrorLog = QtGui.QIcon()
            pxmErrorLog16x16 = QtGui.QPixmap(":/ZS/App/Zeus16x16.bmp")
            pxmErrorLog16x16.setMask(pxmErrorLog16x16.createHeuristicMask())
            iconErrorLog.addPixmap(pxmErrorLog16x16)
            self._actDebugErrLog = QtGui.QAction( iconErrorLog, "Error Log", self )
            self._actDebugErrLog.setToolTip("Open error log dialog")
            self._actDebugErrLog.setEnabled(True)
            self._mnuDebug.addAction(self._actDebugErrLog)
            self._actDebugErrLog.triggered.connect(self.onActDebugErrLogTriggered)

        # <MenuItem> Debug::Request Execution Tree
        #-----------------------------------------

        if self._theApp._reqExecTree is not None:
            iconReqExecTree = QtGui.QIcon()
            pxmReqExecTree16x16 = QtGui.QPixmap(":/ZS/App/Zeus16x16.bmp")
            pxmReqExecTree16x16.setMask(pxmReqExecTree16x16.createHeuristicMask())
            iconReqExecTree.addPixmap(pxmReqExecTree16x16)
            self._actDebugRequestExecTree = QtGui.QAction( iconReqExecTree, "Request Tree", self )
            self._actDebugRequestExecTree.setToolTip("Open request execution tree dialog")
            self._actDebugRequestExecTree.setEnabled(True)
            self._mnuDebug.addAction(self._actDebugRequestExecTree)
            self._actDebugRequestExecTree.triggered.connect(self.onActDebugRequestExecTreeTriggered)

        # <Menu> Info
        #=============================

        self._mnuInfo = self.menuBar().addMenu("&Info")

        # <MenuItem> Info::Version
        #--------------------------

        strActionInfoVersion = "Version: " + "?" # ZS::c_strSysVersionNr

        self._actInfoVersion = QtGui.QAction(strActionInfoVersion,self)
        self._mnuInfo.addAction(self._actInfoVersion)

        # <StatusBar>
        #=============================

        theApp = QtGui.QApplication.instance()
        theApp.setStyleSheet( "QStatusBar::item {border: none;} " )

        self._statusBar = QtGui.QStatusBar()
        strStyleSheet = \
            "QStatusBar { " \
                "background: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3); } "
        self._statusBar.setStyleSheet(strStyleSheet)
        self.setStatusBar(self._statusBar)

        # <Label> Python Version
        #------------------------

        self._lblVersionPython = QtGui.QLabel( "Python Version: " + sys.version )
        self._statusBar.addWidget(self._lblVersionPython)

        # <Label> RequestInProgress with ProgressBar
        #-------------------------------------------

        self._barReqInProgress = QtGui.QProgressBar()
        self._barReqInProgress.setMaximumHeight(12)
        self._barReqInProgress.setMaximumWidth(150)
        self._barReqInProgress.setRange(0,100)
        self._barReqInProgress.setTextVisible(False)
        self._barReqInProgress.installEventFilter(self)
        self._statusBar.addPermanentWidget(self._barReqInProgress)
        self._barReqInProgress.hide()

        self._lblReqInProgress = QtGui.QLabel("Ready")
        self._lblReqInProgress.installEventFilter(self)
        self._statusBar.addPermanentWidget(self._lblReqInProgress)

        self.updateReqInProgressStatus( "Ready", 100 )

        # <Label> Errors
        #---------------

        if self._errLog is not None:
            self._lblErrors = QtGui.QLabel("Errors")
            self._lblErrors.installEventFilter(self)
            self._statusBar.addPermanentWidget(self._lblErrors)

            self.updateErrorsStatus()

            self._errLog.entryAdded.connect(self.onErrLogEntryAdded)
            self._errLog.entryChanged.connect(self.onErrLogEntryChanged)
            self._errLog.entryRemoved.connect(self.onErrLogEntryRemoved)

        # <Label> Remote Command Client Status
        #-------------------------------------

        if self._remCmdClient is not None:
            self._wdgtRemCmdClientCnctStatus = QtGui.QWidget(self)
            self._wdgtRemCmdClientCnctStatus.installEventFilter(self)

            QtGui.QHBoxLayout(self._wdgtRemCmdClientCnctStatus)

            self._wdgtRemCmdClientCnctStatus.setContentsMargins(0,0,0,0)
            self._wdgtRemCmdClientCnctStatus.layout().setContentsMargins(1,1,1,1)

            self._lblRemCmdClientCnctStatusIcon = QtGui.QLabel("")
            self._lblRemCmdClientCnctStatusIcon.setScaledContents(True)
            self._lblRemCmdClientCnctStatusIcon.setMaximumSize( QtCore.QSize(16,16) )
            self._wdgtRemCmdClientCnctStatus.layout().addWidget(self._lblRemCmdClientCnctStatusIcon)
    
            self._statusBar.addPermanentWidget(self._wdgtRemCmdClientCnctStatus)

            self.onRemCmdClientStateChanged( self._remCmdClient, self._remCmdClient.getState() )

        # <CentralWidget>
        #=============================

        self._wdgtCentral = CWidgetCentral( self._remCmdClient, self._theApp )
        self.setCentralWidget(self._wdgtCentral)

        self._wdgtCentral.editRemoteSettingsTriggered.connect(self.onActSettingsRemCmdClientTriggered)

        # <Geometry>
        #=============================

        self._settings = QtCore.QSettings()
        self.restoreGeometry( self._settings.value("MainWindow/Geometry") )

        self.setMinimumWidth(480)
        self.setMinimumHeight(400)

    #===========================================================================
    # overridables of base class QtGui.QWidget
    #===========================================================================

    def closeEvent( self, ev ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'closeEvent', '' )
        if self._dlgRemCmdClient is not None:
            self._dlgRemCmdClient.hide()
        if self._dlgTrcServer is not None:
            self._dlgTrcServer.hide()
        if self._dlgTrcAdminObjPool is not None:
            self._dlgTrcAdminObjPool.hide()
        if self._dlgErrLog  is not None:
            self._dlgErrLog.hide()
        if self._dlgRequestExecTree is not None:
            self._dlgRequestExecTree.hide()
        self._settings.setValue( "MainWindow/Geometry", self.saveGeometry() )
        QtGui.QMainWindow.closeEvent(self,ev)

    #===========================================================================
    # overridables of base class QObject
    #===========================================================================

    def eventFilter( self, objWatched, ev ):
        # pass the event on to the parent class
        bHandled = False
        if objWatched == self._lblReqInProgress or objWatched == self._barReqInProgress:
            if ev.type() == QtCore.QEvent.MouseButtonDblClick:
                self.onActDebugRequestExecTreeTriggered()
                bHandled = True
        elif objWatched == self._lblErrors:
            if ev.type() == QtCore.QEvent.MouseButtonDblClick:
                self.onActDebugErrLogTriggered()
                bHandled = True
        elif objWatched == self._wdgtRemCmdClientCnctStatus:
            if ev.type() == QtCore.QEvent.MouseButtonDblClick:
                self.onActSettingsRemCmdClientTriggered()
                bHandled = True
        else:
            bHandled = QtGui.QMainWindow.eventFilter(self,objWatched,ev)
        return bHandled

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onActSettingsRemCmdClientTriggered(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onActSettingsRemCmdClientTriggered', '' )
        if self._dlgRemCmdClient is None:
            self._dlgRemCmdClient = ZSIpcGUI.CDlgIpcClient(
                self._remCmdClient,
                self.windowTitle() + ": Remote Command Client" )
            self._dlgRemCmdClient.adjustSize()
        self._dlgRemCmdClient.show()
        self._dlgRemCmdClient.raise_()
        self._dlgRemCmdClient.activateWindow()

    def onActDebugTrcServerTriggered(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onActDebugTrcServerTriggered', '' )
        if self._dlgTrcServer is None:
            self._dlgTrcServer = ZSTraceGUI.CDlgTrcServer(
                ZSSys.Trace.ETraceTypeMth,
                self.windowTitle() + ": " + "Method Trace Server" )
            self._dlgTrcServer.adjustSize()
        self._dlgTrcServer.show()
        self._dlgTrcServer.raise_()
        self._dlgTrcServer.activateWindow()

    def onActDebugTrcAdminObjPoolTriggered(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onActDebugTrcAdminObjPoolTriggered', '' )
        if self._dlgTrcAdminObjPool is None:
            self._dlgTrcAdminObjPool = ZSTraceGUI.CDlgTrcAdminObjPool(
                ZSTrace.CTrcServer.getTrcServer(ZSSys.Trace.ETraceTypeMth).getTrcAdminObjPool(),
                self.windowTitle() + ": " + "Method Trace Admin Objects" )
        self._dlgTrcAdminObjPool.show()
        self._dlgTrcAdminObjPool.raise_()
        self._dlgTrcAdminObjPool.activateWindow()

    def onActDebugErrLogTriggered(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onActDebugErrLogTriggered', '' )
        if self._dlgErrLog is None:
            self._dlgErrLog = ZSSysGUI.CDlgErrLog(
                self.windowTitle() + ": " + "Error Log" )
        self._dlgErrLog.show()
        self._dlgErrLog.raise_()
        self._dlgErrLog.activateWindow()

    def onActDebugRequestExecTreeTriggered(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onActDebugRequestExecTreeTriggered', '' )
        if self._dlgRequestExecTree is None:
            self._dlgRequestExecTree = ZSSysGUI.CDlgRequestExecTree(
                self.windowTitle() + ": " + "Requests Execution Tree" )
        self._dlgRequestExecTree.show()
        self._dlgRequestExecTree.raise_()
        self._dlgRequestExecTree.activateWindow()

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onErrLogEntryAdded( self, errResultInfo ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onErrLogEntryAdded', '' )
        self.updateErrorsStatus()

    def onErrLogEntryChanged( self, errResultInfo ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onErrLogEntryChanged', '' )
        self.updateErrorsStatus()

    def onErrLogEntryRemoved( self, errResultInfo ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onErrLogEntryRemoved', '' )
        self.updateErrorsStatus()

    #==============================================================================
    # protected: instance methods
    #==============================================================================

    def updateErrorsStatus(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'updateErrorsStatus', '' )
        severityMax = EResultSeverity.Success
        strToolTip = ""
        ariErrorsCount = []
        for severity in range(ZSSys.EResultSeverityCount):
            ariErrorsCount.append(ZSSys.EResultSeveritySuccess)
        iErrorsCount = 0
        if self._errLog is not None:
            self._errLog.lock()
            for iRowIdx in range(self._errLog.getEntryCount()):
                errLogEntry = self._errLog.getEntry(iRowIdx)
                if errLogEntry is not None:
                    if errLogEntry.m_errResultInfo.getSeverity() > severityMax:
                        severityMax = errLogEntry.m_errResultInfo.getSeverity()
                    if errLogEntry.m_errResultInfo.getSeverity() >= ZSSys.EResultSeverityInfo \
                    and errLogEntry.m_errResultInfo.getSeverity() < ZSSys.EResultSeverityCount:
                        ariErrorsCount[errLogEntry.m_errResultInfo.getSeverity()] += 1
                        iErrorsCount += 1
            self._errLog.unlock()
            if iErrorsCount == 0:
                strToolTip = "There is no Info, no Warning, no Error and no Critical Error message pending"
            else:
                iErrorsCountTmp = 0
                if iErrorsCount == 1:
                    strToolTip = "There is "
                else:
                    strToolTip = "There are "
                for severity in range(ZSSys.EResultSeverityCount):
                    if ariErrorsCount[severity] > 0:
                        strToolTip += str(ariErrorsCount[severity]) + " " + self.resultSeverity2Str(severity)
                        if severity in [ZSSys.EResultSeverityInfo, ZSSys.EResultSeverityWarning, ZSSys.EResultSeverityError]:
                            if ariErrorsCount[severity] > 1:
                                strToolTip += "s"
                        elif severity == ZSSys.EResultSeverityCritical:
                            if ariErrorsCount[severity] == 1:
                                strToolTip += " Error"
                            else:
                                strToolTip += " Errors"
                        iErrorsCountTmp += ariErrorsCount[severity]
                        if iErrorsCountTmp < iErrorsCount:
                            iSeveritiesWithErrorsFollow = 0
                            for severityTmp in range(severity+1,ZSSys.EResultSeverityCount):
                                if ariErrorsCount[severityTmp] > 0:
                                    iSeveritiesWithErrorsFollow += 1
                            if iSeveritiesWithErrorsFollow >= 2:
                                strToolTip += ", "
                            elif iSeveritiesWithErrorsFollow >= 1:
                                strToolTip += " and "
            strToolTip += "."
        if self._lblErrors is not None:
            self._lblErrors.setPixmap( self.getErrPixmap(severityMax) )
            self._lblErrors.setToolTip(strToolTip)

    def getErrPixmap( self, severity ):
        pxm = Images.pxmResultSeverityCritical
        if severity == ZSSys.EResultSeveritySuccess:
            pxm = Images.pxmResultSeveritySuccess
        elif severity == ZSSys.EResultSeverityInfo:
            pxm = Images.pxmResultSeverityInfo
        elif severity == ZSSys.EResultSeverityWarning:
            pxm = Images.pxmResultSeverityWarning
        elif severity == ZSSys.EResultSeverityError:
            pxm = Images.pxmResultSeverityError
        return pxm

    def resultSeverity2Str( self, severity ):
        str = "Critical"
        if severity == ZSSys.EResultSeveritySuccess:
            str = "Success"
        elif severity == ZSSys.EResultSeverityInfo:
            str = "Info"
        elif severity == ZSSys.EResultSeverityWarning:
            str = "Warning"
        elif severity == ZSSys.EResultSeverityError:
            str = "Error"
        return str

    #==============================================================================
    # protected: // instance methods
    #==============================================================================

    def updateReqInProgressStatus( self, strRequest, iProgress_perCent ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'updateReqInProgressStatus', '' )
        if self._lblReqInProgress is not None:
            self._lblReqInProgress.setText(strRequest)
        if self._barReqInProgress is not None:
            if iProgress_perCent < 100:
                if not self._barReqInProgress.isVisible():
                    self.m_barReqInProgress.show()
            else:
                if self._barReqInProgress.isVisible():
                    self._barReqInProgress.hide()
            self._barReqInProgress.setValue(iProgress_perCent)

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onRemCmdClientStateChanged( self, remCmdClient, iState ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientStateChanged', '' )
        strAddTrcInfo = ""
        strStatus = ""
        pxmStatus = QtGui.QPixmap()
        strConnection = ""
        strToolTip = ""
        socketDscr = self._remCmdClient.getSocketDscr();
        strConnection = socketDscr.m_strRemoteHostName
        strConnection += ":" + str(socketDscr.m_uServerListenPort)
        strConnection += ":" + str(socketDscr.m_uRemotePort)
        strToolTip = strConnection
        if self._remCmdClient.isConnected():
            strStatus = "Client: Connected"
            pxmStatus = Images.dctLedCircle2Pixmap[Images.ELedCircleGreen]
        elif self._remCmdClient.getRequestInProgress() == ZSRemCmd.CRemCmdClient.ERequestConnect:
            strStatus = "Client: Connecting ..."
            pxmStatus = Images.dctLedCircle2Pixmap[Images.ELedCircleYellow]
        else:
            strStatus = "Client: Disconnected"
            pxmStatus = Images.dctLedCircle2Pixmap[Images.ELedCircleRed]
        if self._lblRemCmdClientCnctStatusIcon is not None:
            self._lblRemCmdClientCnctStatusIcon.setPixmap(pxmStatus)
            self._lblRemCmdClientCnctStatusIcon.setToolTip(strToolTip)

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onRemCmdClientDestroyed(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientDestroyed', '' )
        self._remCmdClient = None
