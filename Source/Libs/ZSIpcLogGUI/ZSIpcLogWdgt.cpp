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
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSIpcLogGUI/ZSIpcLogWdgt.h"
#include "ZSIpcLog/ZSIpcLogClient.h"
#include "ZSSysGUI/ZSSysEditIntValueDlg.h"
#include "ZSSysGUI/ZSSysFindTextDlg.h"
#include "ZSSysGUI/ZSSysProgressBar.h"
#include "ZSSysGUI/ZSSysLoggerIdxTreeDlg.h"
#include "ZSSys/ZSSysApp.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysLogger.h"
#include "ZSSys/ZSSysLoggerIdxTree.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcMthFile.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Ipc;
using namespace ZS::Log;
using namespace ZS::Log::GUI;


/*******************************************************************************
class CWdgtLog : public QWidget
*******************************************************************************/

static const QString c_strBtnConnect    = "Connect";
static const QString c_strBtnDisconnect = "Disconnect";
static const QString c_strBtnAbort      = "Abort";

static const QString s_arstrThreadHtmlClrCodes[] = {
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
CWdgtLog::CWdgtLog(
    CIpcLogClient* i_pLogClient,
    int            i_iItemsCountMax,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pLogClient(i_pLogClient),
    m_pReqInProgress(nullptr),
    m_strThreadClrFileAbsFilePath(),
    m_iEdtItemsCountMax(i_iItemsCountMax),
    m_iEdtItems(0),
    m_bEdtFull(false),
    m_pEdt(nullptr),
    m_hashThreadColors(),
    m_pBtnClear(nullptr),
    m_pLblServerLoggingEnabled(nullptr),
    m_pChkServerLoggingEnabled(nullptr),
    m_pLblServerUseIpcServer(nullptr),
    m_pChkServerUseIpcServer(nullptr),
    m_pBtnLoggerIdxTree(nullptr),
    m_pBtnConnect(nullptr),
    m_pProgressBarCnct(nullptr),
    m_pLblFindText(nullptr),
    m_pCmbFindText(nullptr),
    m_pBtnFindTextNext(nullptr),
    m_pBtnFindTextPrev(nullptr)
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

    QObject::connect(
        m_pBtnClear, &QPushButton::clicked,
        this, &CWdgtLog::onBtnClearClicked);

    // <CheckBox> Logging Enabled (at Server)
    //---------------------------------------

    m_pLblServerLoggingEnabled = new QLabel("Logging Enabled:");
    pLytBtnListWidget->addWidget(m_pLblServerLoggingEnabled);
    m_pChkServerLoggingEnabled = new QCheckBox();
    m_pChkServerLoggingEnabled->setChecked(false);
    m_pChkServerLoggingEnabled->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pChkServerLoggingEnabled);
    pLytBtnListWidget->addSpacing(10);

    QObject::connect(
        m_pChkServerLoggingEnabled, &QCheckBox::toggled,
        this, &CWdgtLog::onChkServerLoggingEnabledToggled);

    // <CheckBox> Use Ipc Server (at Server)
    //---------------------------------------

    m_pLblServerUseIpcServer = new QLabel("Remote Tracing:");
    pLytBtnListWidget->addWidget(m_pLblServerUseIpcServer);
    m_pChkServerUseIpcServer = new QCheckBox();
    m_pChkServerUseIpcServer->setChecked(false);
    m_pChkServerUseIpcServer->setEnabled(false);
    pLytBtnListWidget->addWidget(m_pChkServerUseIpcServer);
    pLytBtnListWidget->addSpacing(20);

    QObject::connect(
        m_pChkServerUseIpcServer, &QCheckBox::toggled,
        this, &CWdgtLog::onChkServerUseIpcServerToggled);

    // <Button> Loggers Index Tree
    //----------------------------

    m_pBtnLoggerIdxTree = new QPushButton();
    m_pBtnLoggerIdxTree->setText("Loggers");
    pLytBtnListWidget->addWidget(m_pBtnLoggerIdxTree);
    pLytBtnListWidget->addSpacing(10);

    QObject::connect(
        m_pBtnLoggerIdxTree, &QPushButton::clicked,
        this, &CWdgtLog::onBtnLoggerIdxTreeClicked);

    // <Button> Connect/Disconnect
    //----------------------------

    m_pBtnConnect = new QPushButton();
    pLytBtnListWidget->addWidget(m_pBtnConnect);

    if (m_pLogClient->isConnected()) {
        m_pBtnConnect->setText(c_strBtnDisconnect);
    }
    else {
        m_pBtnConnect->setText(c_strBtnConnect);
    }

    QObject::connect(
        m_pBtnConnect, &QPushButton::clicked,
        this, &CWdgtLog::onBtnConnectClicked);

    m_pProgressBarCnct = new CProgressBar();
    m_pProgressBarCnct->setFixedWidth(240);
    m_pProgressBarCnct->setTextVisible(false);
    m_pProgressBarCnct->installEventFilter(this);
    pLytBtnListWidget->addWidget(m_pProgressBarCnct);
    pLytBtnListWidget->addSpacing(20);

    // <FindText>
    //-----------

    m_pLblFindText = new QLabel("Find:");
    pLytBtnListWidget->addWidget(m_pLblFindText);
    m_pCmbFindText = new QComboBox();
    m_pCmbFindText->setEditable(true);
    m_pCmbFindText->setInsertPolicy(QComboBox::NoInsert);
    m_pCmbFindText->setMaxCount(10);
    pLytBtnListWidget->addWidget(m_pCmbFindText, 1);

    QPixmap pxmBtnDown(":/ZS/Button/ButtonMoveDown24x24.png");
    m_pBtnFindTextNext = new QPushButton();
    m_pBtnFindTextNext->setIcon(pxmBtnDown);
    m_pBtnFindTextNext->setToolTip("Find Next");
    pLytBtnListWidget->addWidget(m_pBtnFindTextNext);
    QObject::connect(
        m_pBtnFindTextNext, &QPushButton::clicked,
        this, &CWdgtLog::onBtnFindTextNextClicked);

    QPixmap pxmBtnUp(":/ZS/Button/ButtonMoveUp24x24.png");
    m_pBtnFindTextPrev = new QPushButton();
    m_pBtnFindTextPrev->setIcon(pxmBtnUp);
    m_pBtnFindTextPrev->setToolTip("Find Previous");
    pLytBtnListWidget->addWidget(m_pBtnFindTextPrev);
    QObject::connect(
        m_pBtnFindTextPrev, &QPushButton::clicked,
        this, &CWdgtLog::onBtnFindTextPrevClicked);

    pLytBtnListWidget->addStretch();

    // Keeping indicated parameters up to date
    //========================================

    onIpcClientSettingsChanged(m_pLogClient);

    QObject::connect(
        m_pLogClient, &CIpcLogClient::connected,
        this, &CWdgtLog::onIpcClientConnected);
    QObject::connect(
        m_pLogClient, &CIpcLogClient::disconnected,
        this, &CWdgtLog::onIpcClientDisconnected);
    QObject::connect(
        m_pLogClient, &CIpcLogClient::settingsChanged,
        this, &CWdgtLog::onIpcClientSettingsChanged);
    QObject::connect(
        m_pLogClient, &CIpcLogClient::logSettingsChanged,
        this, &CWdgtLog::onLogSettingsChanged);
    QObject::connect(
        m_pLogClient, &CIpcLogClient::logDataReceived,
        this, &CWdgtLog::onLogDataReceived);

} // ctor

//------------------------------------------------------------------------------
CWdgtLog::~CWdgtLog()
//------------------------------------------------------------------------------
{
    QSettings settings;

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    if( !m_strThreadClrFileAbsFilePath.isEmpty() )
    {
        saveThreadColors();
    }
    #endif

    m_pLogClient = nullptr;
    m_pReqInProgress = nullptr;
    //m_strThreadClrFileAbsFilePath;
    m_iEdtItemsCountMax = 0;
    m_iEdtItems = 0;
    m_bEdtFull = false;
    m_pEdt = nullptr;
    //m_hashThreadColors;
    m_pBtnClear = nullptr;
    m_pLblServerLoggingEnabled = nullptr;
    m_pChkServerLoggingEnabled = nullptr;
    m_pLblServerUseIpcServer = nullptr;
    m_pChkServerUseIpcServer = nullptr;
    m_pBtnLoggerIdxTree = nullptr;
    m_pBtnConnect = nullptr;
    m_pProgressBarCnct = nullptr;
    m_pLblFindText = nullptr;
    m_pCmbFindText = nullptr;
    m_pBtnFindTextNext = nullptr;
    m_pBtnFindTextPrev = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CWdgtLog::getDefaultThreadColorsFilePath() const
//------------------------------------------------------------------------------
{
    // Calculate default file path for thread colors definition
    //--------------------------------------------------------

    QString strAppName = QString(m_pLogClient != nullptr ? m_pLogClient->getRemoteApplicationName() : "");

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

    QString strServerName = QString(m_pLogClient != nullptr ? m_pLogClient->getRemoteServerName() : "");

    QString strThreadClrFileBaseName = strAppName;

    if( !strServerName.isEmpty() )
    {
        strThreadClrFileBaseName += "-" + strServerName;
    }
    strThreadClrFileBaseName += "-ThreadColors";

    QString strAppConfigDir = ZS::System::getAppConfigDir();
    QString strThreadClrFileSuffix = "xml";

    return strAppConfigDir + "/" + strThreadClrFileBaseName + "." + strThreadClrFileSuffix;
}

#if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
//------------------------------------------------------------------------------
void CWdgtLog::loadThreadColors( const QString& i_strAbsFilePath )
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
        if( xmlStreamReader.readNext() != QXmlStreamReader::StartElement && xmlStreamReader.name() != QString("LogClient") )
        {
            xmlStreamReader.raiseError("File does not seem to be an valid XML file for the log client");
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
                            if( m_hashThreadColors.contains(strThreadName) )
                            {
                                m_hashThreadColors[strThreadName] = strThreadHtmlClrCode;
                            }
                            else
                            {
                                m_hashThreadColors.insert(strThreadName, strThreadHtmlClrCode);
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
void CWdgtLog::saveThreadColors( const QString& i_strAbsFilePath )
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

        xmlStreamWriter.writeStartElement("LogClient");

        QHashIterator<QString, QString> it(m_hashThreadColors);

        while( it.hasNext() )
        {
            it.next();
            xmlStreamWriter.writeStartElement("Thread");
            xmlStreamWriter.writeAttribute("Name",it.key());
            xmlStreamWriter.writeAttribute("HtmlColorCode", it.value());
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

        xmlStreamWriter.writeEndElement(); // LogClient

        xmlStreamWriter.writeEndDocument();

    } // if( fileXML.open(QIODevice::WriteOnly) )

} // saveThreadColors
#endif // #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CWdgtLog::readLogFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "readLogFile");

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
            QString strLog;
            int     idxPos = 0;
            int     idxBeg;
            int     idxEnd;
            int     iStrLen;
            int     idx;
            QString strThreadName;
            QString strThread;
            QString strThreadHtmlClrCode;
            QString strDateTime;
            QString strSysTime;
            QString strMthCall;

            // Thread Name
            idxBeg = strLine.indexOf("<", idxPos);
            idxEnd = strLine.indexOf("> ", idxPos);
            if(idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strThreadName = strLine.mid(idxBeg+1, idxEnd-idxBeg-1);
                strThreadName = strThreadName.trimmed();
            }
            idxPos += idxEnd+2;

            if( !strThreadName.isEmpty() )
            {
                if( m_hashThreadColors.contains(strThreadName) )
                {
                    strThreadHtmlClrCode = m_hashThreadColors.value(strThreadName);
                }
                else
                {
                    if( !strThreadName.contains("GUIMain",Qt::CaseInsensitive) )
                    {
                        strThreadHtmlClrCode = s_arstrThreadHtmlClrCodes[s_idxThreadHtmlColorCodes];

                        s_idxThreadHtmlColorCodes++;

                        if( s_idxThreadHtmlColorCodes >= _ZSArrLen(s_arstrThreadHtmlClrCodes) )
                        {
                            s_idxThreadHtmlColorCodes = 0;
                        }
                    }
                    m_hashThreadColors.insert(strThreadName, strThreadHtmlClrCode);
                }
            }

            strThread = "&lt;";

            if( strThreadName.length() > CLogServer::c_iStrLenThreadMax )
            {
                strThread += strThreadName.left(CLogServer::c_iStrLenThreadMax);
            }
            else
            {
                strThread += strThreadName;

                for( int idx = strThreadName.length(); idx < CLogServer::c_iStrLenThreadMax; idx++ )
                {
                    strThread.append("&nbsp;");
                }
            }

            strThread.append("&gt; ");
            strLog += strThread;

            // Date Time
            idxBeg = idxPos;
            idxEnd = strLine.indexOf(" (", idxPos);
            if(idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strDateTime = strLine.mid(idxBeg, idxEnd-idxBeg);
            }
            idxPos = idxEnd+2;
            strLog += strDateTime;

            // System Time
            strLog += " (";
            idxBeg = idxPos;
            idxEnd = strLine.indexOf("): ", idxPos);
            if( idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strSysTime = strLine.mid(idxBeg, idxEnd-idxBeg);
                strSysTime = strSysTime.trimmed();
            }
            idxPos = idxEnd+3;
            iStrLen = strSysTime.length();
            for( idx = 0; idx < CLogServer::c_iStrLenSysTimeMax-iStrLen; idx++ )
            {
                strSysTime.insert(0,"&nbsp;");
            }
            strLog += strSysTime;
            strLog += "):&nbsp;";

            idxBeg = idxPos;
            idxEnd = strLine.size();
            if( idxBeg >= 0 && idxEnd >= 0 && idxEnd > idxBeg) {
                strMthCall = strLine.mid(idxBeg, idxEnd-idxBeg);
                strMthCall = encodeForXml(strMthCall);
            }
            strLog += strMthCall;

            addEdtItem(strLog, strThreadHtmlClrCode);

        } // while( txtstrmTrcMthFile.readLineInto(&strLine) )

        fileTrcMthFile.close();

    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // readLogFile

//------------------------------------------------------------------------------
SErrResultInfo CWdgtLog::writeLogFile( const QString& i_strAbsFilePath )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(NameSpace(), ClassName(), objectName(), "writeLogFile");

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

} // writeLogFile

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtLog::findText( const QString& i_strExp, QTextDocument::FindFlags i_findFlags )
//------------------------------------------------------------------------------
{
    return m_pEdt->find(i_strExp, i_findFlags);
}

/*==============================================================================
protected: // overridables of base class QObject
==============================================================================*/

//------------------------------------------------------------------------------
bool CWdgtLog::eventFilter( QObject* i_pObjWatched, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bHandled = false;
    if (i_pObjWatched == m_pProgressBarCnct) {
        if (i_pEv->type() == QEvent::MouseButtonDblClick) {
            emit progressBarConnectDblClicked();
            bHandled = true;
        }
    }
    else if (i_pObjWatched == m_pEdt) {
        if (i_pEv->type() == QEvent::KeyPress) {
            QKeyEvent* pKeyEv = dynamic_cast<QKeyEvent*>(i_pEv);
            if (pKeyEv != nullptr) {
                if (pKeyEv->modifiers() & Qt::ControlModifier && pKeyEv->key() == Qt::Key_F) {
                    QString strText = m_pEdt->textCursor().selectedText();
                    m_pCmbFindText->setFocus();
                    m_pCmbFindText->setCurrentText(strText);
                }
            }
        }
    }
    else {
        // pass the event on to the parent class
        bHandled = CWdgtLog::eventFilter(i_pObjWatched,i_pEv);
    }
    return bHandled;
}

/*==============================================================================
protected slots: // connected to the signals of my user controls
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLog::onBtnClearClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pEdt->clear();
    m_iEdtItems = 0;
    m_bEdtFull = false;
}

//------------------------------------------------------------------------------
void CWdgtLog::onChkServerLoggingEnabledToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    SLogServerSettings trcServerSettings = m_pLogClient->getLogSettings();
    if (trcServerSettings.m_bEnabled != i_bChecked) {
        trcServerSettings.m_bEnabled = i_bChecked;
        m_pLogClient->setLogSettings(trcServerSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtLog::onChkServerUseIpcServerToggled( bool i_bChecked )
//------------------------------------------------------------------------------
{
    SLogServerSettings trcServerSettings = m_pLogClient->getLogSettings();
    if (trcServerSettings.m_bUseIpcServer != i_bChecked) {
        trcServerSettings.m_bUseIpcServer = i_bChecked;
        m_pLogClient->setLogSettings(trcServerSettings);
    }
}

//------------------------------------------------------------------------------
void CWdgtLog::onBtnLoggerIdxTreeClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strDlgTitle = ZS::System::GUI::getMainWindowTitle() + ": Loggers";
    CDlgIdxTreeLoggers* pDlg = CDlgIdxTreeLoggers::GetInstance(m_pLogClient->getLoggersIdxTree()->objectName());
    if (pDlg == nullptr) {
        pDlg = CDlgIdxTreeLoggers::CreateInstance(strDlgTitle, m_pLogClient->getLoggersIdxTree());
        pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
        pDlg->adjustSize();
        pDlg->show();
    }
    else {
        if (pDlg->isHidden()) {
            pDlg->show();
        }
        pDlg->raise();
        pDlg->activateWindow();
    }
}

//------------------------------------------------------------------------------
void CWdgtLog::onBtnConnectClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if (m_pBtnConnect->text() == c_strBtnConnect) {
        if (m_pReqInProgress == nullptr) {
            m_pReqInProgress = m_pLogClient->connect_();
            if (!isAsynchronousRequest(m_pReqInProgress)) {
                m_pReqInProgress = nullptr; // deleted later by request queue of client
            }
            else {
                m_pBtnConnect->setText(c_strBtnAbort);
                SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString() + " Connecting ...";
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->reset();
                m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                m_pProgressBarCnct->setIncrementInMs(200);
                m_pProgressBarCnct->start();
                QObject::connect(
                    m_pReqInProgress, &CRequest::changed,
                    this, &CWdgtLog::onIpcClientPendingRequestChanged);
            }
        }
    }
    else if (m_pBtnConnect->text() == c_strBtnDisconnect) {
        if (m_pReqInProgress == nullptr) {
            m_pReqInProgress = m_pLogClient->disconnect_();
            if (!isAsynchronousRequest(m_pReqInProgress)) {
                m_pReqInProgress = nullptr; // deleted later by request queue of client
            }
            else {
                m_pBtnConnect->setText(c_strBtnAbort);
                SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString() + " Disconnecting ...";
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->reset();
                m_pProgressBarCnct->setDurationInMs(cnctSettings.m_iConnectTimeout_ms);
                m_pProgressBarCnct->setIncrementInMs(200);
                m_pProgressBarCnct->start();
                QObject::connect(
                    m_pReqInProgress, &CRequest::changed,
                    this, &CWdgtLog::onIpcClientPendingRequestChanged);
            }
        }
    }
    else if (m_pBtnConnect->text() == c_strBtnAbort) {
        if (m_pReqInProgress != nullptr) {
            m_pLogClient->abortRequest(m_pReqInProgress->getId());
            m_pReqInProgress = nullptr;
        }
        if (m_pLogClient->isConnected()) {
            m_pBtnConnect->setText(c_strBtnDisconnect);
        }
        else {
            m_pBtnConnect->setText(c_strBtnConnect);
        }
        SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
        QString strText = cnctSettings.getConnectionString();
        m_pProgressBarCnct->setLabelText(strText);
        m_pProgressBarCnct->reset();
        m_pProgressBarCnct->stop();
    }
} // onBtnConnectClicked

//------------------------------------------------------------------------------
void CWdgtLog::onBtnFindTextNextClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strExp = m_pCmbFindText->currentText();
    if (m_pCmbFindText->findText(strExp, Qt::MatchFixedString) < 0) {
        m_pCmbFindText->insertItem(0, strExp);
    }
    QTextDocument::FindFlags findFlags;
    bool bExpFound = m_pEdt->find(strExp, findFlags);
    if (!bExpFound) {
        m_pCmbFindText->setToolTip("Text not found");
    }
    else {
        m_pCmbFindText->setToolTip("");
    }
}

//------------------------------------------------------------------------------
void CWdgtLog::onBtnFindTextPrevClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    QString strExp = m_pCmbFindText->currentText();
    if (m_pCmbFindText->findText(strExp, Qt::MatchFixedString) < 0) {
        m_pCmbFindText->insertItem(0, strExp);
    }
    QTextDocument::FindFlags findFlags = QTextDocument::FindBackward;
    bool bExpFound = m_pEdt->find(strExp, findFlags);
    if (!bExpFound) {
        m_pCmbFindText->setToolTip("Text not found");
    }
    else {
        m_pCmbFindText->setToolTip("");
    }
}

/*==============================================================================
protected slots: // connected to the signals of the IPC client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLog::onIpcClientConnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerLoggingEnabled->setEnabled(true);
    m_pChkServerUseIpcServer->setEnabled(true);

    m_hashThreadColors.clear();

    m_pBtnConnect->setText(c_strBtnDisconnect);

    SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
    m_pProgressBarCnct->reset();
    m_pProgressBarCnct->stop();
}

//------------------------------------------------------------------------------
void CWdgtLog::onIpcClientDisconnected( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    m_pChkServerLoggingEnabled->setEnabled(false);
    m_pChkServerLoggingEnabled->setChecked(false);
    m_pChkServerUseIpcServer->setEnabled(false);
    m_pChkServerUseIpcServer->setChecked(false);

    #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
    saveThreadColors();
    #endif

    m_strThreadClrFileAbsFilePath = "";

    m_hashThreadColors.clear();

    m_pBtnConnect->setText(c_strBtnConnect);

    SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
    m_pProgressBarCnct->reset();
    m_pProgressBarCnct->stop();
}

//------------------------------------------------------------------------------
void CWdgtLog::onIpcClientSettingsChanged( QObject* /*i_pClient*/ )
//------------------------------------------------------------------------------
{
    SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
    QString strText = cnctSettings.getConnectionString();
    m_pProgressBarCnct->setLabelText(strText);
}

//------------------------------------------------------------------------------
void CWdgtLog::onIpcClientPendingRequestChanged( ZS::System::SRequestDscr i_reqDscr )
//------------------------------------------------------------------------------
{
    if (m_pReqInProgress != nullptr) {
        if (m_pReqInProgress->getId() == i_reqDscr.m_iId) {
            switch (m_pReqInProgress->getRequest()) {
                case CIpcLogClient::ERequestChangeSettings: {
                    if (i_reqDscr.m_iProgress_perCent >= 100) {
                        if (i_reqDscr.m_request == CIpcLogClient::ERequestChangeSettings) {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                case CIpcLogClient::ERequestConnect: {
                    if (i_reqDscr.m_iProgress_perCent >= 100) {
                        if (i_reqDscr.m_request == CIpcLogClient::ERequestConnect) {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                case CIpcLogClient::ERequestDisconnect: {
                    if (i_reqDscr.m_iProgress_perCent >= 100) {
                        if (i_reqDscr.m_request == CIpcLogClient::ERequestDisconnect) {
                            m_pReqInProgress = nullptr;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            if (m_pReqInProgress == nullptr) {
                if (m_pLogClient->isConnected()) {
                    m_pBtnConnect->setText(c_strBtnDisconnect);
                }
                else {
                    m_pBtnConnect->setText(c_strBtnConnect);
                }
                SClientHostSettings cnctSettings = m_pLogClient->getHostSettings();
                QString strText = cnctSettings.getConnectionString();
                m_pProgressBarCnct->setLabelText(strText);
                m_pProgressBarCnct->stop();
                m_pProgressBarCnct->reset();
            }
        }
    }
} // onIpcClientPendingRequestChanged

/*==============================================================================
protected slots: // connected to the signals of the trace client
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLog::onLogSettingsChanged( QObject* /*i_pLogClient*/ )
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
    if (strThreadClrFileAbsFilePath != m_strThreadClrFileAbsFilePath) {
        m_strThreadClrFileAbsFilePath = strThreadClrFileAbsFilePath;
        #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
        loadThreadColors();
        #endif
    }
    m_pChkServerLoggingEnabled->setChecked( m_pLogClient->getLogSettings().m_bEnabled );
    m_pChkServerUseIpcServer->setChecked( m_pLogClient->getLogSettings().m_bUseIpcServer );
}

//------------------------------------------------------------------------------
void CWdgtLog::onLogDataReceived( QObject* /*i_pObjSender*/, const QString& i_str )
//------------------------------------------------------------------------------
{
    QXmlStreamReader xmlStreamReader(i_str);

    QString strAddErrInfo;

    SErrResultInfo errResultInfo = ErrResultInfoSuccess("onLogDataReceived");

    SLogServerSettings trcServerSettings = m_pLogClient->getLogSettings();

    // If too much data Use Ipc Server has been set to false. Ignore following data.
    if( trcServerSettings.m_bUseIpcServer )
    {
        QXmlStreamReader::TokenType xmlStreamTokenType;

        QString strElemName;
        QString strAttr;
        int     iVal;
        double  fVal;
        bool    bOk;

        CIdxTreeLoggers* pIdxTree = m_pLogClient->getLoggersIdxTree();

        CLogger* pLogger = nullptr;
        QString  strThreadName;
        QString  strThreadHtmlClrCode;
        QString  strDateTime;
        double   fSysTime_s = -1.0;
        QString  strNameSpace;
        QString  strClassName;
        QString  strObjName;
        QString  strEntry;
        QString  strLogData;

        xmlStreamTokenType = xmlStreamReader.readNext();

        if( xmlStreamTokenType == QXmlStreamReader::StartDocument )
        {
            while( !xmlStreamReader.atEnd() )
            {
                xmlStreamTokenType = xmlStreamReader.readNext();

                if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
                {
                    strElemName = xmlStreamReader.name().toString();

                    if( strElemName == "LogData" )
                    {
                        if( xmlStreamReader.isStartElement() )
                        {
                            pLogger = nullptr;
                            strThreadName = "";
                            strThreadHtmlClrCode = "#000000";
                            strDateTime = "";
                            fSysTime_s = -1.0;
                            strNameSpace = "";
                            strClassName = "";
                            strObjName = "";
                            strEntry = "";
                            strLogData = "";

                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Thread") )
                            {
                                strThreadName = xmlStreamReader.attributes().value("Thread").toString();

                                if( !strThreadName.isEmpty() )
                                {
                                    if( m_hashThreadColors.contains(strThreadName) )
                                    {
                                        strThreadHtmlClrCode = m_hashThreadColors.value(strThreadName);
                                    }
                                    else
                                    {
                                        if( !strThreadName.contains("GUIMain",Qt::CaseInsensitive) )
                                        {
                                            strThreadHtmlClrCode = s_arstrThreadHtmlClrCodes[s_idxThreadHtmlColorCodes];

                                            s_idxThreadHtmlColorCodes++;

                                            if( s_idxThreadHtmlColorCodes >= _ZSArrLen(s_arstrThreadHtmlClrCodes) )
                                            {
                                                s_idxThreadHtmlColorCodes = 0;
                                            }
                                        }
                                        m_hashThreadColors.insert(strThreadName, strThreadHtmlClrCode);
                                    }
                                }
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("DateTime") )
                            {
                                strDateTime = xmlStreamReader.attributes().value("DateTime").toString();
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("SysTime") )
                            {
                                strAttr = xmlStreamReader.attributes().value("SysTime").toString();
                                fVal = strAttr.toDouble(&bOk);
                                if( bOk && fVal >= 0.0 )
                                {
                                    fSysTime_s = fVal;
                                }
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("NameSpace") )
                            {
                                strNameSpace = xmlStreamReader.attributes().value("NameSpace").toString();
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ClassName") )
                            {
                                strClassName = xmlStreamReader.attributes().value("ClassName").toString();
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("ObjName") )
                            {
                                strObjName = xmlStreamReader.attributes().value("ObjName").toString();
                            }
                            if( !xmlStreamReader.hasError() && xmlStreamReader.attributes().hasAttribute("Entry") )
                            {
                                strEntry = xmlStreamReader.attributes().value("Entry").toString();
                            }

                            SLogData logData(
                                /* strThreadName */ strThreadName,
                                /* strDateTime   */ strDateTime,
                                /* fSysTimeInSec */ fSysTime_s,
                                /* strNameSpace  */ strNameSpace,
                                /* strClassName  */ strClassName,
                                /* strObjName    */ strObjName,
                                /* strEntry      */ strEntry );

                            addEdtItem(logData.toHtmlString(), strThreadHtmlClrCode);

                        } // if( xmlStreamReader.isEndElement() )
                    } // if( strElemName == "LogData" )

                    else if( strElemName == "Logger" )
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
                                    pLogger = pIdxTree->getLogger(iVal);

                                    if( pLogger == nullptr )
                                    {
                                        xmlStreamReader.raiseError("There is no logger with \"IdxInTree\"=" + strAttr);
                                    }
                                }
                                else
                                {
                                    xmlStreamReader.raiseError("Attribute \"IdxInTree\" (" + strAttr + ") of Element \"" + strElemName + "\" is out of range");
                                }
                            }
                        } // if( xmlStreamReader.isStartElement() )
                    } // if( strElemName == "Logger" )
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

} // onLogDataReceived

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLog::addEdtItem( const QString& i_strText, const QString& i_strHtmlClrCode )
//------------------------------------------------------------------------------
{
    if (m_iEdtItemsCountMax > 0 && m_iEdtItems >= m_iEdtItemsCountMax) {
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

        if (!m_bEdtFull) {
            QString strText = "---------- MAXIMUM NUMBER OF ENTRIES REACHED -----------";
            m_bEdtFull = true;
            m_pEdt->append(strText);
            emit textItemAdded(strText);
        }
    }
    else {
        m_pEdt->append("<FONT color=" + i_strHtmlClrCode + ">" + i_strText + "</FONT>");
        m_iEdtItems++;
        emit textItemAdded(i_strText);
    }
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtLog::normalize( QString& i_str ) const
//------------------------------------------------------------------------------
{
    i_str = encodeForHtml(i_str);
}
