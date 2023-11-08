/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtimer.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#endif
#else
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#endif
#include <QtXml/qdom.h>

#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSysGUI/ZSSysEditIntValueDlg.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcAdminObjIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMthFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Trace;
using namespace ZS::Trace::GUI;


/*******************************************************************************
class CWdgtTrcMthList : public QWidget
*******************************************************************************/

static const QString c_strBtnConnect    = "Connect";
static const QString c_strBtnDisconnect = "Disconnect";
static const QString c_strBtnAbort      = "Abort";

static const QString s_arstrThreadHtmlColorCodes[] = {
    /* Red   */ "#C82020",
    /* Green */ "#20C820",
    /* Blue  */ "#2020C8",
    /* Red   */ "#A42020",
    /* Green */ "#20A420",
    /* Blue  */ "#2020A4",
    /* Red   */ "#802020",
    /* Green */ "#208020",
    /* Blue  */ "#202080",
    /* Red   */ "#C84040",
    /* Green */ "#40C840",
    /* Blue  */ "#4040C8",
    /* Red   */ "#A44040",
    /* Green */ "#40A440",
    /* Blue  */ "#4040A4",
    /* Red   */ "#804040",
    /* Green */ "#408040",
    /* Blue  */ "#404080"
};

static int s_idxThreadHtmlColorCodes = 0;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTrcMthList::CWdgtTrcMthList(
    CIpcTrcClient* i_pTrcClient,
    int            i_iItemsCountMax,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pTrcClient(i_pTrcClient),
    m_dataRateCalculatorBytes(1000),
    m_dataRateCalculatorLines(1000),
    m_arfDataRateDiffsProcTime_s(),
    m_ariDataRateDiffs_linesPerSec(),
    m_pReqInProgress(nullptr),
    m_strThreadClrFileAbsFilePath(),
    m_bShowTimeInfo(true),
    m_iEdtItemsCountMax(i_iItemsCountMax),
    m_iEdtItems(0),
    m_bEdtFull(false),
    m_pEdt(nullptr),
    m_hashThreads(),
    m_pBtnClear(nullptr),
    m_pLblServerTracingEnabled(nullptr),
    m_pChkServerTracingEnabled(nullptr),
    m_pLblServerUseIpcServer(nullptr),
    m_pChkServerUseIpcServer(nullptr),
    m_pBtnTrcAdminObjIdxTree(nullptr),
    m_pBtnConnect(nullptr),
    m_pProgressBarCnct(nullptr),
    m_pTmrDataRateRefresh(nullptr),
    m_iTimeSpanTooMuchData_s(10),
    m_pLblTimeSpanTooMuchData(nullptr),
    m_pEdtTimeSpanTooMuchData(nullptr),
    m_pDlgEditTimeSpanTooMuchData(nullptr),
    m_pLblCurrentDataRatesClient(nullptr),
    m_pEdtCurrentDataRatesClient(nullptr),
    m_pLblCurrentDataRatesServer(nullptr),
    m_pEdtCurrentDataRatesServer(nullptr)
{
    setObjectName("TrcMthList");

    QSettings settings;

    // <Widget> Trace Outputs
    //=======================

    QFont fntListWidget(
        /* strFamily  */ "Courier New",
        /* iPointSize */ 8,
        /* iWeight    */ -1,
        /* bItalic    */ false );

    QVBoxLayout* pLytMain = new QVBoxLayout;
    setLayout(pLytMain);

    // <TextEdit> Trace Outputs
    //==========================

    m_pEdt = new QTextEdit(this);
    m_pEdt->setFont(fntListWidget);
    m_pEdt->setLineWrapMode(QTextEdit::NoWrap);
    m_pEdt->setStyleSheet( "QTextEdit { selection-color: white; selection-background-color: darkblue }" );
    m_pEdt->installEventFilter(this);
    pLytMain->addWidget(m_pEdt);

    // <ButtonRow>
    //=============

    QHBoxLayout* pLytBtnListWidget = new QHBoxLayout;
    pLytMain->addLayout(pLytBtnListWidget);

    // <Button> Clear
    //-------------------

    m_pBtnClear = new QPushButton(tr("Clear List"));
    pLytBtnListWidget->addWidget(m_pBtnClear);
    pLytBtnListWidget->addSpacing(10);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnClear,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnClearClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <CheckBox> Tracing Enabled (at Server)
    //---------------------------------------

    m_pLblServerTracingEnabled = new QLabel("Tracing Enabled:");
    pLytBtnListWidget->addWidget(m_pLblServerTracingEnabled);
    m_pChkServerTracingEnabled = new QCheckBox();
    m_pChkServerTracingEnabled->setChecked(false);
    m_pChkServerTracingEnabled->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pChkServerTracingEnabled);
    pLytBtnListWidget->addSpacing(10);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkServerTracingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkServerTracingEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <CheckBox> Use Ipc Server (at Server)
    //---------------------------------------

    m_pLblServerUseIpcServer = new QLabel("Remote Tracing:");
    pLytBtnListWidget->addWidget(m_pLblServerUseIpcServer);
    m_pChkServerUseIpcServer = new QCheckBox();
    m_pChkServerUseIpcServer->setChecked(false);
    m_pChkServerUseIpcServer->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pChkServerUseIpcServer);
    pLytBtnListWidget->addSpacing(20);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkServerUseIpcServer,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkServerUseIpcServerToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Trace Admin Objects Index Tree
    //----------------------------------------

    m_pBtnTrcAdminObjIdxTree = new QPushButton();
    m_pBtnTrcAdminObjIdxTree->setText("Trace Admin Objects");
    pLytBtnListWidget->addWidget(m_pBtnTrcAdminObjIdxTree);
    pLytBtnListWidget->addSpacing(10);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTrcAdminObjIdxTree,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTrcAdminObjIdxTreeClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // <Button> Connect/Disconnect
    //----------------------------

    m_pBtnConnect = new QPushButton();
    pLytBtnListWidget->addWidget(m_pBtnConnect);

    if( m_pTrcClient->isConnected() )
    {
        m_pBtnConnect->setText(c_strBtnDisconnect);
    }
    else
    {
        m_pBtnConnect->setText(c_strBtnConnect);
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnConnect,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnConnectClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pProgressBarCnct = new CProgressBar();
    m_pProgressBarCnct->setFixedWidth(240);
    m_pProgressBarCnct->setTextVisible(false);
    m_pProgressBarCnct->installEventFilter(this);
    pLytBtnListWidget->addWidget(m_pProgressBarCnct);

    // <Stretch> after progress bar
    //-----------------------------

    pLytBtnListWidget->addStretch();

    // <Label> Data Rate
    //------------------

    settings.beginGroup(objectName());
    m_iTimeSpanTooMuchData_s = settings.value("TimeSpanDetectTooMuchData_s", m_iTimeSpanTooMuchData_s).toInt();
    m_dataRateCalculatorBytes.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorBytes.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    settings.endGroup();

    m_pTmrDataRateRefresh = new QTimer(this);
    m_pTmrDataRateRefresh->start(1000);

    if( !QObject::connect(
        /* pObjSender   */ m_pTmrDataRateRefresh,
        /* szSignal     */ SIGNAL(timeout()),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTmrDataRateRefreshTimeout()) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLblTimeSpanTooMuchData = new QLabel("Time Span:");
    m_pLblTimeSpanTooMuchData->setToolTip("Double click to edit the time span used to detect too much data");
    pLytBtnListWidget->addWidget(m_pLblTimeSpanTooMuchData);
    m_pEdtTimeSpanTooMuchData = new QLineEdit(QString::number(m_iTimeSpanTooMuchData_s) + " s");
    m_pEdtTimeSpanTooMuchData->setEnabled(false);
    m_pEdtTimeSpanTooMuchData->setFixedWidth(60);
    m_pEdtTimeSpanTooMuchData->installEventFilter(this);
    pLytBtnListWidget->addWidget(m_pEdtTimeSpanTooMuchData);

    m_pLblCurrentDataRatesClient = new QLabel("Client:");
    pLytBtnListWidget->addWidget(m_pLblCurrentDataRatesClient);
    m_pEdtCurrentDataRatesClient = new QLineEdit("0 Lines/s - 0.0 KB/s");
    m_pEdtCurrentDataRatesClient->setFixedWidth(140);
    m_pEdtCurrentDataRatesClient->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pEdtCurrentDataRatesClient);

    m_pLblCurrentDataRatesServer = new QLabel("Server:");
    pLytBtnListWidget->addWidget(m_pLblCurrentDataRatesServer);
    m_pEdtCurrentDataRatesServer = new QLineEdit("0 Lines/s - 0.0 KB/s");
    m_pEdtCurrentDataRatesServer->setFixedWidth(140);
    m_pEdtCurrentDataRatesServer->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pEdtCurrentDataRatesServer);

    // Keeping indicated parameters up to date
    //========================================

    onIpcClientSettingsChanged(m_pTrcClient);

    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(connected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIpcClientConnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(disconnected(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIpcClientDisconnected(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(settingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onIpcClientSettingsChanged(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL(traceSettingsChanged(QObject*)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onTraceSettingsChanged(QObject*)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }
    if( !QObject::connect(
        /* pObjSender   */ m_pTrcClient,
        /* szSignal     */ SIGNAL( traceDataReceived(QObject*, const QString&) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTraceDataReceived(QObject*, const QString&) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtTrcMthList::~CWdgtTrcMthList()
//------------------------------------------------------------------------------
{
    QSettings settings;

    settings.beginGroup(objectName());
    settings.setValue("TimeSpanDetectTooMuchData_s", m_iTimeSpanTooMuchData_s);
    settings.endGroup();

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    if( !m_strThreadClrFileAbsFilePath.isEmpty() )
    {
        saveThreadColors();
    }
    #endif

    m_pTrcClient = nullptr;
    //m_dataRateCalculatorBytes;
    //m_dataRateCalculatorLines;
    //m_arfDataRateDiffsProcTime_s;
    //m_ariDataRateDiffs_linesPerSec;
    m_pReqInProgress = nullptr;
    //m_strThreadClrFileAbsFilePath;
    m_bShowTimeInfo = false;
    m_iEdtItemsCountMax = 0;
    m_iEdtItems = 0;
    m_bEdtFull = false;
    m_pEdt = nullptr;
    //m_hashThreads;
    m_pBtnClear = nullptr;
    m_pLblServerTracingEnabled = nullptr;
    m_pChkServerTracingEnabled = nullptr;
    m_pLblServerUseIpcServer = nullptr;
    m_pChkServerUseIpcServer = nullptr;
    m_pBtnTrcAdminObjIdxTree = nullptr;
    m_pBtnConnect = nullptr;
    m_pProgressBarCnct = nullptr;
    m_pTmrDataRateRefresh = nullptr;
    m_iTimeSpanTooMuchData_s = 0;
    m_pLblTimeSpanTooMuchData = nullptr;
    m_pEdtTimeSpanTooMuchData = nullptr;
    m_pDlgEditTimeSpanTooMuchData = nullptr;
    m_pLblCurrentDataRatesClient = nullptr;
    m_pEdtCurrentDataRatesClient = nullptr;
    m_pLblCurrentDataRatesServer = nullptr;
    m_pEdtCurrentDataRatesServer = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtTrcMthList::getDefaultThreadColorsFilePath() const
//------------------------------------------------------------------------------
{
    // Calculate default file path for thread colors definition
    //--------------------------------------------------------

    QString strAppName = QString(m_pTrcClient != nullptr ? m_pTrcClient->getRemoteApplicationName() : "");

    if( strAppName.isEmpty() )
    {
        strAppName = QCoreApplication::applicationName();
    }

    // The application name may contain characters which are invalid in file names:
    strAppName.remove(":");
    strAppName.remove(" ");
    strAppName.remove("\\");
    strAppName.remove("/");
    strAppName.remove("<");
    strAppName.remove(">");

    QString strServerName = QString(m_pTrcClient != nullptr ? m_pTrcClient->getRemoteServerName() : "");

    QString strThreadClrFileBaseName = strAppName;

    if( !strServerName.isEmpty() )
    {
        strThreadClrFileBaseName += "-" + strServerName;
    }
    strThreadClrFileBaseName += "-ThreadColors";

    QString strAppConfigDir = ZS::System::getAppConfigDir("System");
    QString strThreadClrFileSuffix = "xml";

    return strAppConfigDir + "/" + strThreadClrFileBaseName + "." + strThreadClrFileSuffix;
}

#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
//------------------------------------------------------------------------------
void CWdgtTrcMthList::loadThreadColors( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strFileName = i_strAbsFilePath;

    if( strFileName.isEmpty() )
    {
        strFileName = m_strThreadClrFileAbsFilePath;
    }

    QFile fileXML(strFileName);

    if( fileXML.open(QIODevice::ReadOnly) )
    {
        QXmlStreamReader xmlStreamReader(&fileXML);

        if( xmlStreamReader.readNext() != QXmlStreamReader::StartDocument )
        {
            xmlStreamReader.raiseError("Invalid XML document");
        }
        if( xmlStreamReader.readNext() != QXmlStreamReader::StartElement && xmlStreamReader.name() != QString("TrcMthClient") )
        {
            xmlStreamReader.raiseError("File does not seem to be an valid XML file for the trace method client");
        }

        bool    bIsStartElement = false;
        bool    bIsEndElement = false;
        bool    bIsElement = false;
        QString strElemName;
        QString strThreadName;
        QString strThreadHtmlClrCode;

        while( !xmlStreamReader.atEnd() )
        {
            xmlStreamReader.readNext();

            bIsStartElement = xmlStreamReader.isStartElement();
            bIsEndElement   = xmlStreamReader.isEndElement();
            bIsElement      = bIsStartElement || bIsEndElement;

            if( bIsElement )
            {
                strElemName = xmlStreamReader.name().toString();

                //---------------------------------------------
                if( strElemName == "Thread" )
                //---------------------------------------------
                {
                    if( bIsStartElement )
                    {
                        strThreadName = xmlStreamReader.attributes().value("Name").toString();
                        strThreadHtmlClrCode = xmlStreamReader.attributes().value("HtmlColorCode").toString();

                        if( strThreadName.isEmpty() )
                        {
                            xmlStreamReader.raiseError("Attribute \"Name\" for element \"" + strElemName + "\" not defined");
                        }
                        else if( strThreadHtmlClrCode.isEmpty() )
                        {
                            xmlStreamReader.raiseError("Attribute \"HtmlColorCode\" for element \"" + strElemName + "\" not defined");
                        }
                        else
                        {
                            STrcMthThreadEntry threadEntry(0,strThreadHtmlClrCode);

                            if( m_hashThreads.contains(strThreadName) )
                            {
                                threadEntry = m_hashThreads.value(strThreadName);
                            }
                            else
                            {
                                m_hashThreads.insert(strThreadName,threadEntry);
                            }
                        }
                    } // if( bIsStartElement )

                    else if( bIsEndElement )
                    {
                    } // if( bIsEndElement )
                } // if( strElemName == "ThreadColor" )
            } // if( bIsElement )
        } // while( !xmlStreamReader.atEnd() )

        if( xmlStreamReader.hasError() )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                QString strAddInfo;
                strAddInfo += xmlStreamReader.errorString() + " on reading " + strFileName + " ";
                strAddInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
                strAddInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());
                SErrResultInfo errResultInfo = ErrResultInfoError("loadThreadColors", EResultFileReadContent, strAddInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
    } // if( fileXML.open(QIODevice::ReadOnly) )

} // loadThreadColors
#endif // #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)

#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
//------------------------------------------------------------------------------
void CWdgtTrcMthList::saveThreadColors( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    QString strFileName = i_strAbsFilePath;

    if( strFileName.isEmpty() )
    {
        strFileName = m_strThreadClrFileAbsFilePath;
    }

    QFile fileXML(strFileName);

    if( !fileXML.open(QIODevice::WriteOnly) )
    {
        if( CErrLog::GetInstance() != nullptr )
        {
            SErrResultInfo errResultInfo = ErrResultInfoError("saveThreadColors", EResultFileOpenForWrite, strFileName);
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }
    else // if( fileXML.open(QIODevice::WriteOnly) )
    {
        QXmlStreamWriter xmlStreamWriter(&fileXML);

        xmlStreamWriter.setAutoFormatting(true);

        xmlStreamWriter.writeStartDocument();

        xmlStreamWriter.writeStartElement("TrcMthClient");

        QHashIterator<QString,STrcMthThreadEntry> it(m_hashThreads);

        while( it.hasNext() )
        {
            it.next();
            xmlStreamWriter.writeStartElement("Thread");
            xmlStreamWriter.writeAttribute("Name",it.key());
            xmlStreamWriter.writeAttribute("HtmlColorCode",it.value().m_strHtmlClrCode);
            xmlStreamWriter.writeEndElement();
        }

        //xmlStreamWriter.writeStartElement("ExampleColorCodes");
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Red");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FF0000");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","White");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FFFFFF");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Turquoise");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#00FFFF");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","LightGrey");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#C0C0C0");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","LightBlue");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#0000FF");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","DarkGrey");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#808080");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","DarkBlue");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#0000A0");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Black");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#000000");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","LightPurple");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FF0080");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Orange");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FF8040");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","DarkPurple");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#800080");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Brown");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#804000");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Yellow");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FFFF00");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Burgundy");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#800000");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","PastelGreen");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#00FF00");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","ForestGreen");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#808000");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","Pink");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#FF00FF");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeStartElement("Thread");
        //xmlStreamWriter.writeAttribute("Name","GrassGreen");
        //xmlStreamWriter.writeAttribute("HtmlColorCode","#408080");
        //xmlStreamWriter.writeEndElement();
        //xmlStreamWriter.writeEndElement(); // ExampleColorCodes

        xmlStreamWriter.writeEndElement(); // TrcMthClient

        xmlStreamWriter.writeEndDocument();

    } // if( fileXML.open(QIODevice::WriteOnly) )

} // saveThreadColors
#endif // #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CWdgtTrcMthList::readTraceMethodFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "readTraceMethodFile");

    QFile fileTrcMthFile;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        fileTrcMthFile.setFileName(i_strAbsFilePath);

        if( !fileTrcMthFile.open(QIODevice::ReadOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QTextStream txtstrmTrcMthFile(&fileTrcMthFile);

        QString strLine;

        while( txtstrmTrcMthFile.readLineInto(&strLine) )
        {
            QString            strTrace;
            int                idxPos = 0;
            int                idxBeg;
            int                idxEnd;
            int                iStrLen;
            int                idx;
            QString            strMthThreadName;
            QString            strThread;
            STrcMthThreadEntry threadEntry;
            QString            strDateTime;
            QString            strSysTime;
            QString            strMthCall;

            // Thread Name
            idxBeg = strLine.indexOf("<", idxPos);
            idxEnd = strLine.indexOf("> ", idxPos);
            if(idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strMthThreadName = strLine.mid(idxBeg+1, idxEnd-idxBeg-1);
                strMthThreadName = strMthThreadName.trimmed();
            }
            idxPos += idxEnd+2;

            if( !strMthThreadName.isEmpty() )
            {
                if( m_hashThreads.contains(strMthThreadName) )
                {
                    threadEntry = m_hashThreads.value(strMthThreadName);
                }
                else
                {
                    if( !strMthThreadName.contains("GUIMain",Qt::CaseInsensitive) )
                    {
                        threadEntry.m_strHtmlClrCode = s_arstrThreadHtmlColorCodes[s_idxThreadHtmlColorCodes];

                        s_idxThreadHtmlColorCodes++;

                        if( s_idxThreadHtmlColorCodes >= _ZSArrLen(s_arstrThreadHtmlColorCodes) )
                        {
                            s_idxThreadHtmlColorCodes = 0;
                        }
                    }
                    m_hashThreads.insert(strMthThreadName,threadEntry);
                }
            }

            strThread = "&lt;";

            if( strMthThreadName.length() > CTrcMthFile::c_iStrLenThreadMax )
            {
                strThread += strMthThreadName.left(CTrcMthFile::c_iStrLenThreadMax);
            }
            else // if( strMthThreadName.length() <= CTrcMthFile::c_iStrLenThreadMax )
            {
                strThread += strMthThreadName;

                for( int idx = strMthThreadName.length(); idx < CTrcMthFile::c_iStrLenThreadMax; idx++ )
                {
                    strThread.append("&nbsp;");
                }
            }

            strThread.append("&gt; ");
            strTrace += strThread;

            // Date Time
            if( m_bShowTimeInfo )
            {
                idxBeg = idxPos;
                idxEnd = strLine.indexOf(" (", idxPos);
                if(idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                    strDateTime = strLine.mid(idxBeg, idxEnd-idxBeg);
                }
                idxPos = idxEnd+2;
                strTrace += strDateTime;
            }

            // System Time
            if( m_bShowTimeInfo )
            {
                strTrace += " (";
                idxBeg = idxPos;
                idxEnd = strLine.indexOf("): ", idxPos);
                if( idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                    strSysTime = strLine.mid(idxBeg, idxEnd-idxBeg);
                    strSysTime = strSysTime.trimmed();
                }
                idxPos = idxEnd+3;
                iStrLen = strSysTime.length();
                for( idx = 0; idx < CTrcMthFile::c_iStrLenSysTimeMax-iStrLen; idx++ )
                {
                    strSysTime.insert(0,"&nbsp;");
                }
                strTrace += strSysTime;
                strTrace += "):&nbsp;";
            }

            idxBeg = idxPos;
            idxEnd = strLine.size();
            if( idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strMthCall = strLine.mid(idxBeg, idxEnd-idxBeg);
                strMthCall = encodeForHtml(strMthCall);
            }
            strTrace += strMthCall;

            addEdtItem(strTrace, threadEntry.m_strHtmlClrCode);

        } // while( txtstrmTrcMthFile.readLineInto(&strLine) )

        fileTrcMthFile.close();

    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // readTraceMethodFile

//------------------------------------------------------------------------------
SErrResultInfo CWdgtTrcMthList::writeTraceMethodFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(nameSpace(), className(), objectName(), "writeTraceMethodFile");

    QFile fileTrcMthFile;

    if( i_strAbsFilePath.isEmpty() )
    {
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidFileName);
    }
    else
    {
        fileTrcMthFile.setFileName(i_strAbsFilePath);

        if( !fileTrcMthFile.open(QIODevice::WriteOnly) )
        {
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileOpenForRead);
            errResultInfo.setAddErrInfoDscr(i_strAbsFilePath);
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        QTextStream txtstrmTrcMthFile(&fileTrcMthFile);
        txtstrmTrcMthFile << m_pEdt->toPlainText();
        fileTrcMthFile.close();
    }

    return errResultInfo;

} // writeTraceMethodFile

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::findText()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Find Text";

    CDlgFindText* pDlg = dynamic_cast<CDlgFindText*>(CDlgFindText::GetInstance("FindText"));

    if( pDlg == nullptr )
    {
        pDlg = CDlgFindText::CreateInstance(strDlgTitle, "FindText");
        pDlg->setTextEdit(m_pEdt);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }

} // findText

//------------------------------------------------------------------------------
bool CWdgtTrcMthList::find( const QString& i_strExp, QTextDocument::FindFlags i_findFlags )
//------------------------------------------------------------------------------
{
    return m_pEdt->find(i_strExp, i_findFlags);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtTrcMthList::getShowTimeInfo() const
//------------------------------------------------------------------------------
{
    return m_bShowTimeInfo;
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::setShowTimeInfo( bool i_bShow )
//------------------------------------------------------------------------------
{
    m_bShowTimeInfo = i_bShow;
}

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtTrcMthList::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;

    if( i_pObjWatched == m_pProgressBarCnct )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            emit progressBarConnectDblClicked();
            bHandled = true;
        }
    }
    else if( i_pObjWatched == m_pEdt )
    {
        if( i_pEv->type() == QEvent::KeyPress )
        {
            QKeyEvent* pKeyEv = dynamic_cast<QKeyEvent*>(i_pEv);

            if( pKeyEv != nullptr )
            {
                if( pKeyEv->modifiers() & Qt::ControlModifier )
                {
                    if( pKeyEv->key() == Qt::Key_F )
                    {
                        findText();
                    }
                }
            }
        }
    }
    else if( i_pObjWatched == m_pEdtTimeSpanTooMuchData )
    {
        if( i_pEv->type() == QEvent::MouseButtonDblClick )
        {
            bHandled = true;
            showEditMaxDataRateDialog();
        }
    }
    else
    {
        // pass the event on to the parent class
        bHandled = CWdgtTrcMthList::eventFilter(i_pObjWatched,i_pEv);
    }
    return bHandled;

} // eventFilter

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onBtnClearClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pEdt->clear();
    m_iEdtItems = 0;
    m_bEdtFull = false;
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onChkServerTracingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    STrcServerSettings trcServerSettings = m_pTrcClient->getTraceSettings();
    if( trcServerSettings.m_bEnabled != i_bChecked )
    {
        trcServerSettings.m_bEnabled = i_bChecked;
        m_pTrcClient->setTraceSettings(trcServerSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onChkServerUseIpcServerToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    STrcServerSettings trcServerSettings = m_pTrcClient->getTraceSettings();
    if( trcServerSettings.m_bUseIpcServer != i_bChecked )
    {
        trcServerSettings.m_bUseIpcServer = i_bChecked;
        m_pTrcClient->setTraceSettings(trcServerSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onBtnTrcAdminObjIdxTreeClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Trace Admin Objects";

    CDlgIdxTreeTrcAdminObjs* pDlg = CDlgIdxTreeTrcAdminObjs::GetInstance(m_pTrcClient->getTraceAdminObjIdxTree()->objectName());

    if( pDlg == nullptr )
    {
        pDlg = CDlgIdxTreeTrcAdminObjs::CreateInstance(strDlgTitle, m_pTrcClient->getTraceAdminObjIdxTree());
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else
    {
        if( pDlg->isHidden() )
        {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
} // onBtnTrcAdminObjIdxTreeClicked

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onBtnConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pBtnConnect->text() == c_strBtnConnect )
    {
        if( m_pReqInProgress == nullptr )
        {
            m_pReqInProgress = m_pTrcClient->connect_();

            if( !isAsynchronousRequest(m_pReqInProgress) )
            {
                m_pReqInProgress = nullptr; // deleted later by request queue of client
            }
            else
            {
                m_pBtnConnect->setText(c_strBtnAbort);

                SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString() + " Connecting ...";
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->reset();
                m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                m_pProgressBarCnct->setIncrementInMs(200);
                m_pProgressBarCnct->start();

                if( !QObject::connect(
                    /* pObjSender   */ m_pReqInProgress,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onIpcClientPendingRequestChanged(ZS::System::SRequestDscr)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        } // if( m_requestQueue.isIdle() )
    } // if( m_pBtnConnect->text() == c_strBtnConnect )

    else if( m_pBtnConnect->text() == c_strBtnDisconnect )
    {
        if( m_pReqInProgress == nullptr )
        {
            m_pReqInProgress = m_pTrcClient->disconnect_();

            if( !isAsynchronousRequest(m_pReqInProgress) )
            {
                m_pReqInProgress = nullptr; // deleted later by request queue of client
            }
            else
            {
                m_pBtnConnect->setText(c_strBtnAbort);

                SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString() + " Disconnecting ...";
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->reset();
                m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                m_pProgressBarCnct->setIncrementInMs(200);
                m_pProgressBarCnct->start();

                if( !QObject::connect(
                    /* pObjSender   */ m_pReqInProgress,
                    /* szSignal     */ SIGNAL(changed(ZS::System::SRequestDscr)),
                    /* pObjReceiver */ this,
                    /* szSlot       */ SLOT(onIpcClientPendingRequestChanged(ZS::System::SRequestDscr)) ) )
                {
                    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
                }
            }
        } // if( m_requestQueue.isIdle() )
    } // if( m_pBtnConnect->text() == c_strBtnDisconnect )

    else if( m_pBtnConnect->text() == c_strBtnAbort )
    {
        if( m_pReqInProgress != nullptr )
        {
            m_pTrcClient->abortRequest(m_pReqInProgress->getId());
            m_pReqInProgress = nullptr;
        }

        if( m_pTrcClient->isConnected() )
        {
            m_pBtnConnect->setText(c_strBtnDisconnect);
        }
        else
        {
            m_pBtnConnect->setText(c_strBtnConnect);
        }

        SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();

    } // if( m_pBtnConnect->text() == c_strBtnAbort )

} // onBtnConnectClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onDlgEditTimeSpanTooMuchDataApplied()
//------------------------------------------------------------------------------
{
    m_iTimeSpanTooMuchData_s = m_pDlgEditTimeSpanTooMuchData->getValue();
    m_dataRateCalculatorBytes.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorBytes.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_pEdtTimeSpanTooMuchData->setText(QString::number(m_iTimeSpanTooMuchData_s) + " s");
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onDlgEditTimeSpanTooMuchDataAccepted()
//------------------------------------------------------------------------------
{
    m_iTimeSpanTooMuchData_s = m_pDlgEditTimeSpanTooMuchData->getValue();
    m_dataRateCalculatorBytes.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorBytes.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setRecordTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_dataRateCalculatorLines.setMeasurePointsTimeSpanInSec(m_iTimeSpanTooMuchData_s);
    m_pEdtTimeSpanTooMuchData->setText(QString::number(m_iTimeSpanTooMuchData_s) + " s");
    m_pDlgEditTimeSpanTooMuchData->hide();
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onDlgEditTimeSpanTooMuchDataRejected()
//------------------------------------------------------------------------------
{
    m_pDlgEditTimeSpanTooMuchData->hide();
}

/*==============================================================================
protected slots: // connected to the signals of the IPC client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerTracingEnabled->setEnabled(true);
    m_pChkServerUseIpcServer->setEnabled(true);

    m_hashThreads.clear();

    m_pBtnConnect->setText(c_strBtnDisconnect);

    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
    m_pProgressBarCnct->reset();
    m_pProgressBarCnct->stop();

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerTracingEnabled->setEnabled(false);
    m_pChkServerTracingEnabled->setChecked(false);
    m_pChkServerUseIpcServer->setEnabled(false);
    m_pChkServerUseIpcServer->setChecked(false);

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    saveThreadColors();
    #endif

    m_strThreadClrFileAbsFilePath = "";

    m_hashThreads.clear();

    m_pBtnConnect->setText(c_strBtnConnect);

    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
    m_pProgressBarCnct->reset();
    m_pProgressBarCnct->stop();

} // onIpcClientDisconnected

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientSettingsChanged( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if( m_pReqInProgress != nullptr )
    {
        if( m_pReqInProgress->getId() == i_reqDscr.m_iId )
        {
            switch( m_pReqInProgress->getRequest() )
            {
                case CIpcTrcClient::ERequestChangeSettings:
                {
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( i_reqDscr.m_request == CIpcTrcClient::ERequestChangeSettings )
                        {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                case CIpcTrcClient::ERequestConnect:
                {
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( i_reqDscr.m_request == CIpcTrcClient::ERequestConnect )
                        {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                case CIpcTrcClient::ERequestDisconnect:
                {
                    if( i_reqDscr.m_iProgress_perCent >= 100 )
                    {
                        if( i_reqDscr.m_request == CIpcTrcClient::ERequestDisconnect )
                        {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            } // switch( pReqInProgress->getRequest() )

            if( m_pReqInProgress == nullptr )
            {
                if( m_pTrcClient->isConnected() )
                {
                    m_pBtnConnect->setText(c_strBtnDisconnect);
                }
                else
                {
                    m_pBtnConnect->setText(c_strBtnConnect);
                }
                SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString();
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->stop();
                m_pProgressBarCnct->reset();

            } // if( m_pReqInProgress == nullptr )
        } // if( m_requestQueue.isPendingRequest(pReqInProgress,i_reqDscr.m_iId) )
    } // if( pReqInProgress != nullptr )

} // onIpcClientPendingRequestChanged

/*==============================================================================
protected slots: // connected to the signals of the trace client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onTraceSettingsChanged( QObject* /*i_pTrcClient*/ )
//------------------------------------------------------------------------------
{
    // If the remote application name or the remote server name has been
    // initially received the default file path of the thread colors file
    // will be corrected and the thread colors got to be newly read.
    // If the client has been newly connected thread colors have been read
    // from "MyAppName-ThreadColors.xml" (e.g. "ZSAppTrcMthClient-ThreadColors.xml")
    // to have some colors. But as the number of threads and their names depend
    // on the remote application they will be read for each remote application
    // and remote server within this application separately.

    QString strThreadClrFileAbsFilePath = getDefaultThreadColorsFilePath();

    if( strThreadClrFileAbsFilePath != m_strThreadClrFileAbsFilePath )
    {
        m_strThreadClrFileAbsFilePath = strThreadClrFileAbsFilePath;

        #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
        loadThreadColors();
        #endif
    }

    m_pChkServerTracingEnabled->setChecked( m_pTrcClient->getTraceSettings().m_bEnabled );
    m_pChkServerUseIpcServer->setChecked( m_pTrcClient->getTraceSettings().m_bUseIpcServer );
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onTraceDataReceived( QObject* /*i_pObjSender*/, const QString& i_str )
//------------------------------------------------------------------------------
{
    QXmlStreamReader xmlStreamReader(i_str);

    QString strAddErrInfo;

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onTraceDataReceived");

    STrcServerSettings trcServerSettings = m_pTrcClient->getTraceSettings();

    // If too much data Use Ipc Server has been set to false. Ignore following data.
    if( trcServerSettings.m_bUseIpcServer )
    {
        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString strElemName;
        QString strAttr;
        int     iVal;
        double  fVal;
        bool    bOk;

        CIdxTreeTrcAdminObjs* pIdxTree = m_pTrcClient->getTraceAdminObjIdxTree();

        CTrcAdminObj*      pTrcAdminObj = nullptr;
        QString            strNameSpace;
        QString            strClassName;
        QString            strObjName;
        QString            strMthName;
        QString            strMthThreadName;
        EMethodDir         mthDir = EMethodDir::Undefined;
        STrcMthThreadEntry threadEntry;
        QString            strDateTime;
        double             fSysTime_s = -1.0;
        QString            strMthInArgs;
        QString            strMthOutArgs;
        QString            strMthRet;
        QString            strAddInfo;
        QString            strTrace;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
        {
            while( !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    if( strElemName == "TrcData" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            pTrcAdminObj = nullptr;
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";
                            strMthName = "";
                            strMthThreadName = "";
                            mthDir = EMethodDir::Undefined;
                            strDateTime = "";
                            fSysTime_s = -1.0;
                            threadEntry = STrcMthThreadEntry();
                            strMthInArgs = "";
                            strMthOutArgs = "";
                            strMthRet = "";
                            strAddInfo = "";
                            strTrace = "";

                        } // if( xmlStreamReader.isStartElement() )

                        else if( xmlStreamReader.isEndElement() )
                        {
                            if( m_hashThreads.contains(strMthThreadName) )
                            {
                                threadEntry = m_hashThreads.value(strMthThreadName);
                            }
                            else
                            {
                                if( !strMthThreadName.contains("GUIMain",Qt::CaseInsensitive) )
                                {
                                    threadEntry.m_strHtmlClrCode = s_arstrThreadHtmlColorCodes[s_idxThreadHtmlColorCodes];

                                    s_idxThreadHtmlColorCodes++;

                                    if( s_idxThreadHtmlColorCodes >= _ZSArrLen(s_arstrThreadHtmlColorCodes) )
                                    {
                                        s_idxThreadHtmlColorCodes = 0;
                                    }
                                }
                                m_hashThreads.insert(strMthThreadName,threadEntry);
                            }

                            QString strThread = "&lt;";

                            if( strMthThreadName.length() > CTrcMthFile::c_iStrLenThreadMax )
                            {
                                strThread += strMthThreadName.left(CTrcMthFile::c_iStrLenThreadMax);
                            }
                            else // if( strMthThreadName.length() <= CTrcMthFile::c_iStrLenThreadMax )
                            {
                                strThread += strMthThreadName;

                                for( int idx = strMthThreadName.length(); idx < CTrcMthFile::c_iStrLenThreadMax; idx++ )
                                {
                                    strThread.append("&nbsp;");
                                }
                            }

                            strThread.append("&gt; ");
                            strTrace += strThread;

                            if( m_bShowTimeInfo )
                            {
                                strTrace += strDateTime;
                                strTrace += " (";

                                QString strSysTime = QString::number(fSysTime_s, 'f', 6);
                                int iStrLen = strSysTime.length();
                                for( int idx = 0; idx < CTrcMthFile::c_iStrLenSysTimeMax-iStrLen; idx++ )
                                {
                                    strSysTime.insert(0,"&nbsp;");
                                }
                                strTrace += strSysTime;
                                strTrace += "):&nbsp;";
                            }

                            if( mthDir == EMethodDir::Enter )
                            {
                                for( int idx = 0; idx < threadEntry.m_iCallDepth; idx++ )
                                {
                                    strTrace += "&nbsp;&nbsp;&nbsp;";
                                }
                                threadEntry.m_iCallDepth++;

                                strTrace += "-&gt;&nbsp;";
                            }
                            else if( mthDir == EMethodDir::Leave )
                            {
                                for( int idx = 0; idx < threadEntry.m_iCallDepth-1; idx++ )
                                {
                                    strTrace += "&nbsp;&nbsp;&nbsp;";
                                }
                                if( threadEntry.m_iCallDepth > 0 )
                                {
                                    threadEntry.m_iCallDepth--;
                                }
                                strTrace += "&lt;-&nbsp;";
                            }
                            else
                            {
                                for( int idx = 0; idx < threadEntry.m_iCallDepth; idx++ )
                                {
                                    strTrace += "&nbsp;&nbsp;&nbsp;";
                                }
                            }

                            m_hashThreads[strMthThreadName].m_iCallDepth = threadEntry.m_iCallDepth;

                            if( pTrcAdminObj != nullptr )
                            {
                                strNameSpace = pTrcAdminObj->getNameSpace();
                                strClassName = pTrcAdminObj->getClassName();

                                // When using class (module) trace admin objects the object name must be
                                // passed to the method tracer when tracing instance methods.
                                // When using instance trace admin objects the object name should not be
                                // provided to the method tracer when tracing instance methods.
                                // But if the object name is passed to the method tracer use this object name.
                                if( strObjName.isEmpty() )
                                {
                                    strObjName = pTrcAdminObj->getObjectName();
                                }
                            }

                            QString strObjPath = pIdxTree->buildPathStr(strNameSpace, strClassName);

                            if( !strObjPath.isEmpty() )
                            {
                                strTrace += "&lt;" + strObjPath + "&gt;&nbsp;";
                            }

                            if( !strObjName.isEmpty() )
                            {
                                strTrace += strObjName;
                                strTrace += ".";
                            }

                            strTrace += strMthName;

                            normalize(strMthInArgs);
                            normalize(strMthOutArgs);
                            normalize(strMthRet);
                            normalize(strAddInfo);

                            if( mthDir == EMethodDir::Enter )
                            {
                                strTrace += "(";
                                strTrace += strMthInArgs;
                                strTrace += ")";
                            }
                            else if( mthDir == EMethodDir::Leave )
                            {
                                strTrace += "(";
                                strTrace += strMthOutArgs;
                                strTrace += ")";
                                if( !strMthRet.isEmpty() )
                                {
                                    strTrace += ": " + strMthRet;
                                }
                            }
                            else
                            {
                                strTrace += ":&nbsp;";
                                strTrace += strAddInfo;
                            }

                            addEdtItem(strTrace, threadEntry.m_strHtmlClrCode);

                        } // if( xmlStreamReader.isEndElement() )
                    } // if( strElemName == "TrcData" )

                    else if( strElemName == "TrcAdminObj" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            if( !xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                xmlStreamReader.raiseError("Attribute \"IdxInTree\" of Element \"" + strElemName + "\" is missing");
                            }
                            else // if( xmlStreamReader.attributes().hasAttribute("IdxInTree") )
                            {
                                strAttr = xmlStreamReader.attributes().value("IdxInTree").toString();
                                iVal = strAttr.toInt(&bOk);
                                if( bOk && iVal >= 0 )
                                {
                                    pTrcAdminObj = pIdxTree->getTraceAdminObj(iVal, false);

                                    if( pTrcAdminObj == nullptr )
                                    {
                                        xmlStreamReader.raiseError("There is no trace admin object with \"IdxInTree\"=" + strAttr);
                                    }
                                }
                                else
                                {
                                    xmlStreamReader.raiseError("Attribute \"IdxInTree\" (" + strAttr + ") of Element \"" + strElemName + "\" is out of range");
                                }
                            }
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "TrcAdminObj" )

                    else if( strElemName == "Method" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            if( xmlStreamReader.attributes().hasAttribute("ObjName") )
                            {
                                strObjName = xmlStreamReader.attributes().value("ObjName").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("Name") )
                            {
                                strMthName = xmlStreamReader.attributes().value("Name").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("Name") )
                            {
                                strMthThreadName = xmlStreamReader.attributes().value("Thread").toString();
                            }
                            if( !xmlStreamReader.attributes().hasAttribute("Dir") )
                            {
                                xmlStreamReader.raiseError("Attribute \"Dir\" of Element \"" + strElemName + "\" is missing");
                            }
                            else
                            {
                                strAttr = xmlStreamReader.attributes().value("Dir").toString();
                                mthDir = CEnumMethodDir::toEnumerator(strAttr);
                                if( mthDir == EMethodDir::Undefined ) xmlStreamReader.raiseError("Attribute \"Dir\" (" + strAttr + ") of Element \"" + strElemName + "\" is out of range");
                            }
                            if( xmlStreamReader.attributes().hasAttribute("DateTime") )
                            {
                                strDateTime = xmlStreamReader.attributes().value("DateTime").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("SysTime") )
                            {
                                strAttr = xmlStreamReader.attributes().value("SysTime").toString();
                                fVal = strAttr.toDouble(&bOk);
                                if( bOk && fVal >= 0.0 ) fSysTime_s = fVal;

                                if( m_iTimeSpanTooMuchData_s > 0 )
                                {
                                    m_dataRateCalculatorBytes.addMeasurement(fSysTime_s, 2 * static_cast<qint64>(i_str.size()));
                                    m_dataRateCalculatorLines.addMeasurement(fSysTime_s, 1);

                                    showAndCheckDataRates();
                                }
                            }
                            if( xmlStreamReader.attributes().hasAttribute("InArgs") )
                            {
                                strMthInArgs = xmlStreamReader.attributes().value("InArgs").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("OutArgs") )
                            {
                                strMthOutArgs = xmlStreamReader.attributes().value("OutArgs").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("Return") )
                            {
                                strMthRet = xmlStreamReader.attributes().value("Return").toString();
                            }
                            if( xmlStreamReader.attributes().hasAttribute("AddInfo") )
                            {
                                strAddInfo = xmlStreamReader.attributes().value("AddInfo").toString();
                            }
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Method" )
                } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
            } // while( !xmlStreamReader.atEnd() )

            if( xmlStreamReader.hasError() )
            {
                strAddErrInfo  = xmlStreamReader.errorString();
                strAddErrInfo += " on receiving: " + i_str;

                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultFileReadContent);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);

                if( CErrLog::GetInstance() != nullptr )
                {
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        } // if( xmlStreamTokenType == QXmlStreamReader::StartElement )
    } // if( trcServerSettings.m_bUseIpcServer )

} // onTraceDataReceived

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onTmrDataRateRefreshTimeout()
//------------------------------------------------------------------------------
{
    m_dataRateCalculatorBytes.updateMeasurements();
    m_dataRateCalculatorLines.updateMeasurements();

    showAndCheckDataRates();
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode )
//------------------------------------------------------------------------------
{
    if( m_iEdtItemsCountMax > 0 && m_iEdtItems >= m_iEdtItemsCountMax )
    {
        // !!!! To slow !!!!
        // GUI becomes unresponsive when removing lines.
        //QTextCursor cursor = m_pEdt->textCursor();

        //for( int i = 0; i < 10; ++i )
        //{
        //    cursor.movePosition(QTextCursor::Start);
        //    cursor.select(QTextCursor::LineUnderCursor);
        //    cursor.removeSelectedText();
        //    cursor.deleteChar(); // clean up new line
        //    --m_iEdtItems;
        //}
        //m_pEdt->setTextCursor(cursor);

        if( !m_bEdtFull )
        {
            QString strText = "---------- MAXIMUM NUMBER OF ENTRIES REACHED -----------";
            m_bEdtFull = true;
            m_pEdt->append(strText);
            emit textItemAdded(strText);
        }
    }
    else
    {
        m_pEdt->append("<FONT color=" + i_strHtmlClrCode + ">" + i_strText + "</FONT>");
        m_iEdtItems++;
        emit textItemAdded(i_strText);
    }
} // addEdtItem

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::normalize( QString& i_str ) const
//------------------------------------------------------------------------------
{
    i_str = encodeForHtml(i_str);
}

//------------------------------------------------------------------------------
void CWdgtTrcMthList::showAndCheckDataRates()
//------------------------------------------------------------------------------
{
    if( m_iTimeSpanTooMuchData_s > 0 )
    {
        double fTimeSpanServer_LinesPerSec = 0.0;
        double fTimeSpanServer_KBs = 0.0;
        double fTimeSpanClient_LinesPerSec = 0.0;
        double fTimeSpanClient_KBs = 0.0;

        int iDataRateServer_LinesPerSec =
            m_dataRateCalculatorLines.getMeasurePointsDataRatePerSec(-1, &fTimeSpanServer_LinesPerSec);
        double fDataRateServer_KBs =
            static_cast<double>(m_dataRateCalculatorBytes.getMeasurePointsDataRatePerSec(-1, &fTimeSpanServer_KBs)) / 1000.0;

        int iDataRateClient_LinesPerSec =
            m_dataRateCalculatorLines.getRecordingDataRatePerSec(-1, &fTimeSpanClient_LinesPerSec);
        double fDataRateClient_KBs =
            static_cast<double>(m_dataRateCalculatorBytes.getRecordingDataRatePerSec(-1, &fTimeSpanClient_KBs)) / 1000.0;

        QString strDataRatesServer =
            QString::number(iDataRateServer_LinesPerSec) + " Lines/s - " +
            QString::number(fDataRateServer_KBs, 'f', 3) + " KB/s";

        QString strDataRatesClient =
            QString::number(iDataRateClient_LinesPerSec) + " Lines/s - " +
            QString::number(fDataRateClient_KBs, 'f', 3) + " KB/s";

        m_pEdtCurrentDataRatesServer->setText(strDataRatesServer);
        m_pEdtCurrentDataRatesClient->setText(strDataRatesClient);

        double fProcTime_s = Time::getProcTimeInSec();
        int iDataRateDiff_LinesPerSec = iDataRateServer_LinesPerSec - iDataRateClient_LinesPerSec;

        if( m_arfDataRateDiffsProcTime_s.isEmpty() )
        {
            m_arfDataRateDiffsProcTime_s << fProcTime_s;
            m_ariDataRateDiffs_linesPerSec << fProcTime_s;
        }

        if( fProcTime_s - m_arfDataRateDiffsProcTime_s.last() >= 1.0 )
        {
            m_arfDataRateDiffsProcTime_s << fProcTime_s;
            m_ariDataRateDiffs_linesPerSec << iDataRateDiff_LinesPerSec;

            if( m_arfDataRateDiffsProcTime_s.size() > 10 )
            {
                m_arfDataRateDiffsProcTime_s.removeFirst();
                m_ariDataRateDiffs_linesPerSec.removeFirst();
            }
        }

        if( fTimeSpanClient_LinesPerSec > m_iTimeSpanTooMuchData_s || m_dataRateCalculatorLines.getMeasurePointsCount() >= m_dataRateCalculatorLines.getMaxMeasurePoints() )
        {
            bool bClientCanProcessData = false;

            for( int iDataRateDiff : m_ariDataRateDiffs_linesPerSec )
            {
                // If for at least one second the client was able to process the data ..
                if( iDataRateDiff <= 0 )
                {
                    bClientCanProcessData = true;
                    break;
                }
            }

            if( !bClientCanProcessData )
            {
                STrcServerSettings trcServerSettings = m_pTrcClient->getTraceSettings();

                if( trcServerSettings.m_bUseIpcServer )
                {
                    trcServerSettings.m_bUseIpcServer = false;
                    m_pTrcClient->setTraceSettings(trcServerSettings);

                    QString strText = "The lines received per second have exceeded the configured maximum value.\n\n";
                    strText += "To avoid that the GUI gets unresponsive remote tracing has been disabled.\n";
                    strText += "Reduce the amount of trace data by disabling some trace admin objects.\n";

                    QMessageBox* msgBox = new QMessageBox();
                    msgBox->setIcon(QMessageBox::Critical);
                    msgBox->setWindowTitle(ZS::System::GUI::getMainWindowTitle() + ": Too much data");
                    msgBox->setText(strText);
                    msgBox->setAttribute(Qt::WA_DeleteOnClose);
                    msgBox->setModal(false);
                    msgBox->show();
                }
            }
        }
    } // if( m_iTimeSpanTooMuchData_s > 0 )
} // showAndCheckDataRates

//------------------------------------------------------------------------------
void CWdgtTrcMthList::showEditMaxDataRateDialog()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = QCoreApplication::applicationName() + ": Edit Max Data Rate";

    if( m_pDlgEditTimeSpanTooMuchData == nullptr )
    {
        m_pDlgEditTimeSpanTooMuchData = dynamic_cast<CDlgEditIntValue*>(CDlgEditIntValue::GetInstance("EditMaxDataRate"));
    }
    if( m_pDlgEditTimeSpanTooMuchData == nullptr )
    {
        m_pDlgEditTimeSpanTooMuchData = CDlgEditIntValue::CreateInstance(strDlgTitle, "EditMaxDataRate");
        m_pDlgEditTimeSpanTooMuchData->setAttribute(Qt::WA_DeleteOnClose, true);
        m_pDlgEditTimeSpanTooMuchData->adjustSize();
        m_pDlgEditTimeSpanTooMuchData->show();

        if( !QObject::connect(
            /* pObjSender   */ m_pDlgEditTimeSpanTooMuchData,
            /* szSignal     */ SIGNAL(applied()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDlgEditTimeSpanTooMuchDataApplied()) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pDlgEditTimeSpanTooMuchData,
            /* szSignal     */ SIGNAL(accepted()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDlgEditTimeSpanTooMuchDataAccepted()) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pDlgEditTimeSpanTooMuchData,
            /* szSignal     */ SIGNAL(rejected()),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT(onDlgEditTimeSpanTooMuchDataRejected()) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }
    else
    {
        if( m_pDlgEditTimeSpanTooMuchData->isHidden() )
        {
            m_pDlgEditTimeSpanTooMuchData->show();
        }
        m_pDlgEditTimeSpanTooMuchData->raise();
        m_pDlgEditTimeSpanTooMuchData->activateWindow();
    }

    m_pDlgEditTimeSpanTooMuchData->setMinimum(0);
    m_pDlgEditTimeSpanTooMuchData->setMaximum(60);
    m_pDlgEditTimeSpanTooMuchData->setValueName("Time Span");
    m_pDlgEditTimeSpanTooMuchData->setUnit("s");
    m_pDlgEditTimeSpanTooMuchData->setValue(m_iTimeSpanTooMuchData_s);
    m_pDlgEditTimeSpanTooMuchData->setDescription(
        "The value determines the length of time in seconds the client uses to check "
        "whether the server's data can be processed and indicated.\n"
        "If the server sends the data faster than the client can display it, "
        "the input buffer may overflow over time.\n"
        "Furthermore, so much time would be spent processing the data that the GUI"
        "is no longer able to display the data or respond to user inputs.\n"
        "The client must be able to process and display all incoming data at least "
        "once for one second within the time span.\n"
        "If this is not the case, remote tracing is disabled.");

} // showEditMaxDataRateDialog
