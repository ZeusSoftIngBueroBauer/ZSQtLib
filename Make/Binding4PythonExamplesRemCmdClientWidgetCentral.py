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

import traceback

from PySide import QtGui, QtCore

from ZSQtLib import ZSSys
from ZSQtLib import ZSIpc
from ZSQtLib import ZSTrace
from ZSQtLib import ZSRemCmd
from ZSQtLib import ZSSysGUI
from ZSQtLib import ZSRemCmdGUI

from htol_util.ErrResult import EResultSeverity, EResult, CErrResult, ErrResultException


class CWidgetCentral(QtGui.QWidget):
    '''
    '''
    #===========================================================================
    # class members
    #===========================================================================

    #===========================================================================
    # signals
    #===========================================================================

    # pylint: disable=no-member

    editRemoteSettingsTriggered = QtCore.Signal()

    #===========================================================================
    # ctor
    #===========================================================================

    def __init__( self, remCmdClient, theApp, parent=None ):
        QtGui.QWidget.__init__(self,parent)

        # Instance members
        #=============================

        self._name = "theCentralWidget"
        self._theApp = theApp
        self._remCmdClient = remCmdClient
        self._remCmdObjPool = None
        self._reqInProgress = None
        self._lyt = None
        self._lytLineCnct = None
        self._btnConnect = None
        self._progressBarCnct = None
        self._splitter = None
        self._splitterRemCmd = None
        self._grpRemCmdObjPool = None
        self._lytRemCmdObjPool = None
        self._wdgtRemCmdObjPool = None
        self._remCmdTreeEntrySelected = None
        self._wdgtRemCmd = None
        self._lytRemCmd = None
        self._lytLineRemCmd = None
        self._lblRemCmd = None
        self._edtRemCmd = None
        self._btnRemCmdExecute = None
        self._edtRemCmdDscr = None
        self._grpLog = None
        self._lytLog = None
        self._edtLog = None
        self._lytLineLogBtns = None
        self._btnLogClear = None
        self._trcAdminObj = None

        if self._remCmdClient is not None:
            self._remCmdObjPool = self._remCmdClient.getObjPool()

        self.setObjectName(self._name)

        strMethod = self._name + ".__init__"

        self._trcAdminObj = ZSTrace.CTrcServer.getTrcAdminObj(
            ZSSys.Trace.ETraceTypeMth,
            "Dialog::LTT::RemCmdClient",
            "CWidgetCentral",
            "",
            self.objectName() )

        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'ctor', '' )

        # Layout
        #=============================

        cxWidthClm1 = 80
        cxBtnWidth = 120

        self._lyt = QtGui.QVBoxLayout()
        self.setLayout(self._lyt)

        # <Line> Remote Host
        #=============================

        self._lytLineCnct = QtGui.QHBoxLayout()
        self._lyt.addLayout(self._lytLineCnct)

        self._btnConnect = QtGui.QPushButton()
        self._btnConnect.setFixedWidth(cxBtnWidth)
        self._lytLineCnct.addWidget(self._btnConnect)

        if self._remCmdClient is None:
            self._btnConnect.setText("Connect")
        else:
            if self._remCmdClient.isConnected():
                self._btnConnect.setText("Disconnect")
            else:
                self._btnConnect.setText("Connect")

        self._btnConnect.clicked.connect(self.onBtnConnectClicked)

        self._progressBarCnct = ZSSysGUI.CProgressBar()
        self._progressBarCnct.setTextVisible(False)
        self._lytLineCnct.addWidget(self._progressBarCnct,1)

        # <Splitter> Main
        #==================

        # Dividing remote command group and log widget

        self._splitter = QtGui.QSplitter(QtCore.Qt.Vertical)
        self._lyt.addWidget(self._splitter)

        # <GroupBox> Remote Commands
        #===========================

        self._grpRemCmdObjPool = QtGui.QGroupBox("Remote Commands")
        self._lytRemCmdObjPool = QtGui.QVBoxLayout()
        self._lytRemCmdObjPool.setContentsMargins(0,0,0,0)
        self._grpRemCmdObjPool.setLayout(self._lytRemCmdObjPool)
        self._splitter.addWidget(self._grpRemCmdObjPool)

        # Dividing remote command tree view and selected command with command description
        self._splitterRemCmd = QtGui.QSplitter(QtCore.Qt.Vertical)
        self._lytRemCmdObjPool.addWidget(self._splitterRemCmd)

        self._wdgtRemCmdObjPool = ZSRemCmdGUI.CWdgtRemCmdObjPool(self._remCmdObjPool)
        self._splitterRemCmd.addWidget(self._wdgtRemCmdObjPool)

        self._wdgtRemCmdObjPool.onBtnObjPoolExpandAllClicked(True)
        self._wdgtRemCmdObjPool.onBtnObjPoolResizeRowsAndColumnsToContentsClicked(True)

        self._wdgtRemCmdObjPool.remCmdObjSelected.connect(self.onRemCmdObjSelected)

        treeView = self._wdgtRemCmdObjPool.getTreeView()
        if treeView is not None:
            treeView.expanded.connect(self.onTreeViewRemCmdObjPoolExpanded)

        # <Widget> Selected Command, Syntax, Description
        #-----------------------------------------------

        self._wdgtRemCmd = QtGui.QWidget()
        self._lytRemCmd = QtGui.QVBoxLayout()
        self._wdgtRemCmd.setLayout(self._lytRemCmd)
        self._splitterRemCmd.addWidget(self._wdgtRemCmd)

        # <Line> Selected Command and Execute Button
        #-------------------------------------------

        self._lytLineRemCmd = QtGui.QHBoxLayout()
        self._lytRemCmd.addLayout(self._lytLineRemCmd)

        self._lblRemCmd = QtGui.QLabel("Command:")
        self._lblRemCmd.setFixedWidth(cxWidthClm1)
        self._lytLineRemCmd.addWidget(self._lblRemCmd)

        self._edtRemCmd = QtGui.QLineEdit()
        self._lytLineRemCmd.addWidget(self._edtRemCmd,1)

        self._btnRemCmdExecute = QtGui.QPushButton("Execute")
        self._btnRemCmdExecute.setEnabled(True)
        self._lytLineRemCmd.addWidget(self._btnRemCmdExecute)

        self._btnRemCmdExecute.clicked.connect(self.onBtnRemCmdExecuteClicked)

        # <Line> Selected Command Syntax
        #-------------------------------------------

        self._lytLineRemCmdSyntax = QtGui.QHBoxLayout()
        self._lytRemCmd.addLayout(self._lytLineRemCmdSyntax)

        self._lblRemCmdSyntax = QtGui.QLabel("Syntax:")
        self._lblRemCmdSyntax.setFixedWidth(cxWidthClm1)
        self._lytLineRemCmdSyntax.addWidget(self._lblRemCmdSyntax)

        self._edtRemCmdSyntax = QtGui.QLineEdit()
        self._lytLineRemCmdSyntax.addWidget(self._edtRemCmdSyntax,1)

        # <TextEdit> Description of selected Command
        #-------------------------------------------

        self._edtRemCmdDscr = QtGui.QTextEdit()
        self._lytRemCmd.addWidget(self._edtRemCmdDscr,1)

        # <GroupBox> Command Log
        #===========================

        fntEdtLog = QtGui.QFont( "Courier New", 8, -1, False )

        self._grpLog = QtGui.QGroupBox("Log")
        self._lytLog = QtGui.QVBoxLayout()
        self._grpLog.setLayout(self._lytLog)
        self._splitter.addWidget(self._grpLog)

        self._edtLog = QtGui.QTextEdit()
        self._edtLog.setFont(fntEdtLog)
        self._lytLog.addWidget(self._edtLog,1)

        self._lytLineLogBtns = QtGui.QHBoxLayout()
        self._lytLog.addLayout(self._lytLineLogBtns)

        self._btnLogClear = QtGui.QPushButton("Clear")
        self._btnLogClear.setFixedWidth(cxBtnWidth)
        self._lytLineLogBtns.addWidget(self._btnLogClear)

        self._btnLogClear.clicked.connect(self.onBtnLogClearClicked)

        self._lytLineLogBtns.addStretch()

        # Geometry (restore position and size)
        #=====================================

        settings = QtCore.QSettings()

        self.restoreGeometry( settings.value(self.objectName()+"/Geometry") )

        if self._splitterRemCmd is not None:
            listSizes = self._splitterRemCmd.sizes()
            for idx in range(len(listSizes)):
                listSizes[idx] = int( settings.value( self.objectName() + "/RemCmdSplitterHeight" + str(idx), 50 ) )
            self._splitterRemCmd.setSizes(listSizes)
        if self._splitter is not None:
            listSizes = self._splitter.sizes()
            for idx in range(len(listSizes)):
                listSizes[idx] = int( settings.value( self.objectName() + "/SplitterHeight" + str(idx), 50 ) )
            self._splitter.setSizes(listSizes)

        # Keeping indicated parameters up to date ..
        #===========================================

        if self._remCmdClient is not None:
            self.onRemCmdClientSettingsChanged(self._remCmdClient)

            self._remCmdClient.connected.connect(self.onRemCmdClientConnected)
            self._remCmdClient.disconnected.connect(self.onRemCmdClientDisconnected)
            self._remCmdClient.settingsChanged.connect(self.onRemCmdClientSettingsChanged)
            self._remCmdClient.receivedData.connect(self.onRemCmdClientReceivedData)
            self._remCmdClient.commandSent.connect(self.onRemCmdClientCommandSent)

        # <Stretch> at bottom of main layout
        #===================================

        #self._lyt.addStretch()

    #==============================================================================
    # public: // instance methods
    #==============================================================================

    def addLogItem( self, transmitDir, bBold, strMsg ):
        # This method may be called from any but not just the GUIMainThread.
        ev = ZSSys.CMsgReqAddLogItem( self, self, transmitDir, bBold, strMsg )
        QtCore.QCoreApplication.postEvent(self,ev)

    #===========================================================================
    # overridables of base class QWidget
    #===========================================================================

    def hideEvent( self, ev ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'hideEvent', '' )
        settings = QtCore.QSettings()
        settings.setValue( self.objectName()+"/Geometry", self.saveGeometry() )
        if self._splitterRemCmd is not None:
            listSizes = self._splitterRemCmd.sizes()
            for idx in range(len(listSizes)):
                settings.setValue( self.objectName() + "/RemCmdSplitterHeight" + str(idx), listSizes[idx] )
        if self._splitter is not None:
            listSizes = self._splitter.sizes()
            for idx in range(len(listSizes)):
                settings.setValue( self.objectName() + "/SplitterHeight" + str(idx), listSizes[idx] )

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onBtnConnectClicked(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onBtnConnectClicked', '' )
        if self._btnConnect.text() == 'Connect':
            if self._reqInProgress is None:
                self.applySettings()
                self._reqInProgress = self._remCmdClient.connect_()
                if not ZSSys.is_asynchronous_request(self._reqInProgress):
                    self._reqInProgress = None  # deleted later by request queue of client
                else:
                    self._btnConnect.setText('Abort')
                    if self._progressBarCnct is not None:
                        cnctSettings = self._remCmdClient.getHostSettings()
                        strText = cnctSettings.getConnectionString() + " Connecting ..."
                        self._progressBarCnct.setLabelText(strText)
                        self._progressBarCnct.reset()
                        self._progressBarCnct.setDurationInMs(cnctSettings.m_iConnectTimeout_ms)
                        self._progressBarCnct.setIncrementInMs(200)
                        self._progressBarCnct.start()
                    self._reqInProgress.changed.connect(self.onRemCmdClientPendingRequestChanged)
        elif self._btnConnect.text() == 'Disconnect':
            if self._reqInProgress is None:
                self._reqInProgress = self._remCmdClient.disconnect_()
                if not ZSSys.is_asynchronous_request(self._reqInProgress):
                    self._reqInProgress = None  # deleted later by request queue of client
                else:
                    self._btnConnect.setText('Abort')
                    if self._progressBarCnct is not None:
                        cnctSettings = self._remCmdClient.getHostSettings()
                        strText = cnctSettings.getConnectionString() + " Disconnecting ..."
                        self._progressBarCnct.setLabelText(strText)
                        self._progressBarCnct.reset()
                        self._progressBarCnct.setDurationInMs(cnctSettings.m_iConnectTimeout_ms)
                        self._progressBarCnct.setIncrementInMs(200)
                        self._progressBarCnct.start()
                    self._reqInProgress.changed.connect(self.onRemCmdClientPendingRequestChanged)
        elif self._btnConnect.text() == 'Abort':
            if self._reqInProgress is not None:
                self._remCmdClient.abortRequest(self._reqInProgress.getId())
                self._reqInProgress = None
            if self._remCmdClient.isConnected():
                self._btnConnect.setText('Disconnect')
            else:
                self._btnConnect.setText('Connect')
            if self._progressBarCnct is not None:
                cnctSettings = self._remCmdClient.getHostSettings()
                strText = cnctSettings.getConnectionString()
                self._progressBarCnct.setLabelText(strText)
                self._progressBarCnct.reset()
                self._progressBarCnct.stop()

    #==============================================================================
    # protected slots: // connected to the signals of the tree view
    #==============================================================================

    def onTreeViewRemCmdObjPoolExpanded( self, modelIdx ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onTreeViewRemCmdObjPoolExpanded', '' )
        if modelIdx.isValid():
            self._wdgtRemCmdObjPool.getTreeView().resizeColumnToContents(modelIdx.column())

    def onRemCmdObjSelected( self, strCmd, strSyntax, strDscr ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdObjSelected', '' )
        self._edtRemCmd.setText(strCmd)
        self._edtRemCmdSyntax.setText(strSyntax)
        self._edtRemCmdDscr.setHtml(strDscr)

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onBtnLogClearClicked(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onBtnLogClearClicked', '' )
        if self._edtLog is not None:
            self._edtLog.clear()

    #==============================================================================
    # protected slots:
    #==============================================================================

    def onBtnRemCmdExecuteClicked(self):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onBtnRemCmdExecuteClicked', '' )
        if self._edtRemCmd is not None:
            strTxt = self._edtRemCmd.text()
            if isinstance(strTxt,unicode):
                strTxt = str(strTxt)
            if not len(strTxt) == 0:
                errResultInfo, strCmd, bIsQuery, strlstArgs = self._remCmdClient.parseData(strTxt)
                if errResultInfo.isErrorResult():
                    if ZSSys.CErrLog.instance() is not None:
                        ZSSys.CErrLog.instance().addEntry(errResultInfo)
                else:
                    if isinstance(strCmd,unicode):
                        strCmd = str(strCmd)
                    if bIsQuery:
                        strCmd += "?"
                    self._remCmdClient.sendCommand( strCmd, strlstArgs )

    #==============================================================================
    # protected: // instance methods
    #==============================================================================

    def getCnctSettings(self):
        # The edit field "RemCmd" is read only. The remote settings will be changed
        # and applied through the IpcClient dialog.
        return self._remCmdClient.getHostSettings()

    #==============================================================================
    # protected: // instance methods
    #==============================================================================

    def applySettings( self, iReqIdParent=-1 ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'applySettings', '' )
        cnctSettingsWdgt   = self.getCnctSettings()
        cnctSettingsClient = self._remCmdClient.getHostSettings()
        if cnctSettingsWdgt != cnctSettingsClient:
            self._remCmdClient.settingsChanged.disconnect(self.onRemCmdClientSettingsChanged)
            self._remCmdClient.setHostSettings(cnctSettingsWdgt)
            self._remCmdClient.settingsChanged.connect(self.onRemCmdClientSettingsChanged)
        # In order for the changed settings to take affect:
        return self._remCmdClient.changeSettings(iReqIdParent)

    #==============================================================================
    # protected: // overridables of inherited class QWidget
    #==============================================================================

    def mouseDoubleClickEvent( self, ev ):
        if self._progressBarCnct is not None:
            rct = self._progressBarCnct.geometry()
            ptMousePos = ev.pos()
            if rct.contains(ptMousePos):
                editRemoteSettingsTriggered.emit()
        QtGui.QWidget.mouseDoubleClickEvent(self,ev)

    #==============================================================================
    # protected: // overridables of inherited class QObject
    #==============================================================================

    def event( self, ev ):
        bEventHandled = False
        if type(ev) is ZSSys.CMsgReqAddLogItem:
            if ev.getReceiver() is self and ev.getSender() is self \
            and ev.isBaseMsgType() and ev.getMsgType() == ZSSys.EBaseMsgTypeReqAddLogItem:
                if self._edtLog is not None:
                    strItem = ""
                    strMsg = ev.getMsg()
                    strTime = QtCore.QTime.currentTime().toString("hh:mm:ss:zzz")
                    #strSysTimeFill = "            "
                    #strSysTime = str(ZS.Time.getProcTimeInSec(),'f',6)
                    strMsg.replace("<","&lt;")
                    strMsg.replace(">","&gt;")
                    #if strSysTime.length() < 12:
                    #    strSysTime.insert(0,strSysTimeFill.data(),12-strSysTime.length())
                    #    strSysTime.replace(" ","&nbsp;")
                    strItem = strTime + ": " + strMsg
                    #strItem = ZSSys.transmitDir2Str(ev.getTransmitDir(),True) + ": " + strTime + " (" + strSysTime + "): " + strMsg
                    if ev.isBold():
                        self._edtLog.append( "<FONT color=black><b>" + strItem + "</b></FONT>" )
                    else:
                        self._edtLog.append( "<FONT color=black>" + strItem + "</FONT>" )
        if not bEventHandled:
            bEventHandled = QtGui.QWidget.event(self,ev)
        return bEventHandled

    #==============================================================================
    # protected slots: // connected to the signals of the remote command client
    #==============================================================================

    def onRemCmdClientConnected( self, remCmdClient ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientConnected', '' )
        if self._btnConnect is not None:
            self._btnConnect.setText('Disconnect')
        if self._btnRemCmdExecute is not None:
            self._btnRemCmdExecute.setEnabled(True)
        if self._progressBarCnct is not None:
            cnctSettings = self._remCmdClient.getHostSettings()
            strText = cnctSettings.getConnectionString()
            self._progressBarCnct.setLabelText(strText)
            self._progressBarCnct.reset()
            self._progressBarCnct.stop()

    def onRemCmdClientDisconnected( self, remCmdClient ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientDisconnected', '' )
        if self._btnConnect is not None:
            self._btnConnect.setText('Connect')
        if self._btnRemCmdExecute is not None:
            self._btnRemCmdExecute.setEnabled(False)
        if self._progressBarCnct is not None:
            cnctSettings = self._remCmdClient.getHostSettings()
            strText = cnctSettings.getConnectionString()
            self._progressBarCnct.setLabelText(strText)
            self._progressBarCnct.reset()
            self._progressBarCnct.stop()

    def onRemCmdClientSettingsChanged( self, remCmdClient ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientSettingsChanged', '' )
        cnctSettings = self._remCmdClient.getHostSettings()
        if self._progressBarCnct is not None:
            strTxt = cnctSettings.m_strRemoteHostName + ":" + str(cnctSettings.m_uRemotePort)
            self._progressBarCnct.setLabelText(strTxt)

    def onRemCmdClientReceivedData( self, remCmdClient, byteArr ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientReceivedData', '' )
        strData = str(byteArr)
        self.addLogItem( ZSSys.ETransmitDirReceive, False, strData )

    def onRemCmdClientCommandSent( self, remCmdClient, strCmd, strlstArgs ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientCommandSent', '' )
        strTmp = strCmd
        if len(strlstArgs) > 0:
            strTmp += " " + str(strlstArgs)
        self.addLogItem( ZSSys.ETransmitDirSend, True, strTmp )

    def onRemCmdClientPendingRequestChanged( self, reqDscr ):
        methodTracer = ZSSys.Trace.CMethodTracer( self._trcAdminObj, 0, 'onRemCmdClientPendingRequestChanged', '' )
        if self._reqInProgress is not None:
            if self._reqInProgress.getId() == reqDscr.m_iId:
                if self._reqInProgress.getRequest() == ZSRemCmd.CRemCmdClient.ERequestChangeSettings:
                    if reqDscr.m_iProgress_perCent >= 100:
                        if reqDscr.m_request == ZSRemCmd.CRemCmdClient.ERequestChangeSettings:
                            self._reqInProgress = None
                elif self._reqInProgress.getRequest() == ZSRemCmd.CRemCmdClient.ERequestConnect:
                        if reqDscr.m_iProgress_perCent >= 100:
                            if reqDscr.m_request == ZSRemCmd.CRemCmdClient.ERequestConnect:
                                self._reqInProgress = None
                elif self._reqInProgress.getRequest() == ZSRemCmd.CRemCmdClient.ERequestDisconnect:
                        if reqDscr.m_iProgress_perCent >= 100:
                            if reqDscr.m_request == ZSRemCmd.CRemCmdClient.ERequestDisconnect:
                                self._reqInProgress = None
                if self._reqInProgress is None:
                    if self._btnConnect is not None:
                        if self._remCmdClient.isConnected():
                            self._btnConnect.setText('Disconnect')
                        else:
                            self._btnConnect.setText('Connect')
                    if self._progressBarCnct is not None:
                        cnctSettings = self._remCmdClient.getHostSettings()
                        strText = cnctSettings.getConnectionString()
                        self._progressBarCnct.setLabelText(strText)
                        self._progressBarCnct.stop()
                        self._progressBarCnct.reset()
