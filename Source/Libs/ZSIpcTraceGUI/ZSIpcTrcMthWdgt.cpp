/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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
#include <QtGui/qlistwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcTraceGUI/ZSIpcTrcMthWdgt.h"
#include "ZSIpcTrace/ZSIpcTrcClient.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSys/ZSSysApp.h"
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
    m_pReqInProgress(nullptr),
    m_strThreadClrFileAbsFilePath(),
    m_iEdtItemsCountMax(i_iItemsCountMax),
    m_iEdtItems(0),
    m_bEdtFull(false),
    m_pEdt(nullptr),
    m_hashThreads(),
    m_pBtnClear(nullptr),
    m_pLblServerTracingEnabled(nullptr),
    m_pChkServerTracingEnabled(nullptr),
    m_pBtnConnect(nullptr),
    m_pProgressBarCnct(nullptr)
{
    // Calculate default file path for thread colors definition
    //--------------------------------------------------------

    QString strAppNameNormalized = QCoreApplication::applicationName();

    // The application name may contain characters which are invalid in file names:
    strAppNameNormalized.remove(":");
    strAppNameNormalized.remove(" ");
    strAppNameNormalized.remove("\\");
    strAppNameNormalized.remove("/");
    strAppNameNormalized.remove("<");
    strAppNameNormalized.remove(">");

    QString strAppConfigDir = ZS::System::getAppConfigDir("System");

    QString strThreadClrFileBaseName = strAppNameNormalized + "-ThreadColors";
    QString strThreadClrFileSuffix = "xml";

    m_strThreadClrFileAbsFilePath = strAppConfigDir + "/" + strThreadClrFileBaseName + "." + strThreadClrFileSuffix;

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

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnClear,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnClearClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Add space to button line
    pLytBtnListWidget->addSpacing(10);

    // <CheckBox> Tracing Enabled (at Server)
    //---------------------------------------

    m_pLblServerTracingEnabled = new QLabel("Tracing Enabled:");
    pLytBtnListWidget->addWidget(m_pLblServerTracingEnabled);
    m_pChkServerTracingEnabled = new QCheckBox();
    m_pChkServerTracingEnabled->setChecked(false);
    m_pChkServerTracingEnabled->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pChkServerTracingEnabled);
    pLytBtnListWidget->addSpacing(20);

    if( !QObject::connect(
        /* pObjSender   */ m_pChkServerTracingEnabled,
        /* szSignal     */ SIGNAL(toggled(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onChkServerTracingEnabledToggled(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Add space to button line
    pLytBtnListWidget->addSpacing(10);

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

    // <Stretch> at end of button line
    //--------------------------------

    pLytBtnListWidget->addStretch();

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
    m_pTrcClient = nullptr;
    m_pReqInProgress = nullptr;
    m_pEdt = nullptr;
    m_pBtnClear = nullptr;
    m_pLblServerTracingEnabled = nullptr;
    m_pChkServerTracingEnabled = nullptr;
    m_pBtnConnect = nullptr;
    m_pProgressBarCnct = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

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
        if( xmlStreamReader.readNext() != QXmlStreamReader::StartElement && xmlStreamReader.name() != "TrcMthClient" )
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
void CWdgtTrcMthList::findText()
//------------------------------------------------------------------------------
{
    QString strDlgTitle = getMainWindowTitle() + ": Find";

    CDlgFindText* pDlg = dynamic_cast<CDlgFindText*>(CDlgFindText::GetInstance(strDlgTitle));

    if( pDlg == nullptr )
    {
        pDlg = CDlgFindText::CreateInstance(strDlgTitle, strDlgTitle);
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
    bool bExpFound = false;

    if( m_pEdt != nullptr )
    {
        bExpFound = m_pEdt->find(i_strExp, i_findFlags);
    }
    return bExpFound;

} // find

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
    if( m_pEdt != nullptr )
    {
        m_pEdt->clear();
        m_iEdtItems = 0;
        m_bEdtFull = false;
    }

} // onBtnClearClicked

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onChkServerTracingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    if( i_bChecked )
    {
        m_pTrcClient->setEnabled(true);
    }
    else
    {
        m_pTrcClient->setEnabled(false);
    }

} // onChkServerTracingEnabledToggled

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

                if( m_pProgressBarCnct != nullptr )
                {
                    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                    QString strText = cnctSettings.getConnectionString() + " Connecting ...";
                    m_pProgressBarCnct->setLabelText(strText);
                    m_pProgressBarCnct->reset();
                    m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                    m_pProgressBarCnct->setIncrementInMs(200);
                    m_pProgressBarCnct->start();
                }

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

                if( m_pProgressBarCnct != nullptr )
                {
                    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                    QString strText = cnctSettings.getConnectionString() + " Disconnecting ...";
                    m_pProgressBarCnct->setLabelText(strText);
                    m_pProgressBarCnct->reset();
                    m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                    m_pProgressBarCnct->setIncrementInMs(200);
                    m_pProgressBarCnct->start();
                }

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

        if( m_pProgressBarCnct != nullptr )
        {
            SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
            QString strText = cnctSettings.getConnectionString();
            m_pProgressBarCnct->setLabelText(strText);
            m_pProgressBarCnct->reset();
            m_pProgressBarCnct->stop();
        }
    } // if( m_pBtnConnect->text() == c_strBtnAbort )

} // onBtnConnectClicked

/*==============================================================================
protected slots: // connected to the signals of the IPC client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerTracingEnabled->setEnabled(true);

    m_hashThreads.clear();

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    loadThreadColors();
    #endif

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(c_strBtnDisconnect);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();
    }

} // onIpcClientConnected

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerTracingEnabled->setEnabled(false);
    m_pChkServerTracingEnabled->setChecked(false);

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    saveThreadColors();
    #endif

    m_hashThreads.clear();

    if( m_pBtnConnect != nullptr )
    {
        m_pBtnConnect->setText(c_strBtnConnect);
    }
    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();
    }

} // onIpcClientDisconnected

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onIpcClientSettingsChanged( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    if( m_pProgressBarCnct != nullptr )
    {
        SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
    }

} // onIpcClientSettingsChanged

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
                if( m_pBtnConnect != nullptr )
                {
                    if( m_pTrcClient->isConnected() )
                    {
                        m_pBtnConnect->setText(c_strBtnDisconnect);
                    }
                    else
                    {
                        m_pBtnConnect->setText(c_strBtnConnect);
                    }
                }
                if( m_pProgressBarCnct != nullptr )
                {
                    SClientHostSettings cnctSettings = m_pTrcClient->getHostSettings();
                    QString strText = cnctSettings.getConnectionString();
                    m_pProgressBarCnct->setLabelText(strText);
                    m_pProgressBarCnct->stop();
                    m_pProgressBarCnct->reset();
                }
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
    if( m_pChkServerTracingEnabled != nullptr )
    {
        m_pChkServerTracingEnabled->setChecked( m_pTrcClient->isEnabled() );
    }

} // onTraceSettingsChanged

//------------------------------------------------------------------------------
void CWdgtTrcMthList::onTraceDataReceived( QObject* /*i_pObjSender*/, const QString& i_str )
//------------------------------------------------------------------------------
{
    QXmlStreamReader xmlStreamReader(i_str);

    QString strAddErrInfo;

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onTraceDataReceived");

    QXmlStreamReader::TokenType xmlStreamTokenType;

    QString strElemName;
    QString strAttr;
    int     iVal;
    double  fVal;
    bool    bOk;

    CIdxTreeTrcAdminObjs* pIdxTree = m_pTrcClient->getTraceAdminObjIdxTree();

    CTrcAdminObj*      pTrcAdminObj = nullptr;
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

                        QString strObjPath;

                        if( pTrcAdminObj != nullptr )
                        {
                            strObjPath = pIdxTree->buildPathStr( pTrcAdminObj->getNameSpace(), pTrcAdminObj->getClassName() );
                        }
                        if( !strObjPath.isEmpty() )
                        {
                            strTrace += "&lt;" + strObjPath + "&gt;&nbsp;";
                        }

                        if( pTrcAdminObj != nullptr )
                        {
                            if( !pTrcAdminObj->getObjectName().isEmpty() )
                            {
                                strTrace += pTrcAdminObj->getObjectName();
                                strTrace += ".";
                            }
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
                                pTrcAdminObj = pIdxTree->getTraceAdminObj(iVal);

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

} // onTraceDataReceived

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode )
//------------------------------------------------------------------------------
{
    if( m_pEdt != nullptr )
    {
        if( m_iEdtItemsCountMax > 0 && m_iEdtItems >= m_iEdtItemsCountMax )
        {
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
    }

} // addEdtItem

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTrcMthList::normalize( QString& i_str ) const
//------------------------------------------------------------------------------
{
    if( i_str.contains('<') )
    {
        i_str.replace("<","&lt;");
    }
    if( i_str.contains('>') )
    {
        i_str.replace(">","&gt;");
    }
    if( i_str.contains('"') )
    {
        i_str.replace("\"","&quot;");
    }
    if( i_str.contains("'") )
    {
        i_str.replace("'","&apos;");
    }
} // normalize
