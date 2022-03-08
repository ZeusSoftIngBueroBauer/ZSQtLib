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

#include <QtCore/qthread.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#include <QtGui/qmessagebox.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#endif

#include "ZSSysGUI/ZSSysGUIApp.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CGUIApp : public QApplication
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CGUIApp::CGUIApp( int& i_argc, char* i_argv[] ) :
//------------------------------------------------------------------------------
    QApplication(i_argc,i_argv)
{
} // ctor

//------------------------------------------------------------------------------
CGUIApp::~CGUIApp()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
protected: // overridables of base class QCoreApplication
==============================================================================*/

//------------------------------------------------------------------------------
bool CGUIApp::notify( QObject* i_pObjRcv, QEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool     bResult = false;
    bool     bDeleteMsg = false;
    CMsgErr* pMsgErr = nullptr;
    CMsgErr* pMsgExc = nullptr;

    if( i_pEv->type() != QEvent::Timer )
    {
        pMsgErr = dynamic_cast<CMsgErr*>(i_pEv);
    }

    if( pMsgErr != nullptr )
    {
        if( pMsgErr->wasException() )
        {
            // The error (exception) message will be deleted by Qt.
            pMsgExc = pMsgErr;
        }
        else
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                // To avoid wrong gcc error message "invalid initialization of non-const reference ...".
                SErrResultInfo errResultInfo = pMsgErr->getErrResultInfo();
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
        }
        pMsgErr = nullptr;

    } // if( pMsgErr != nullptr )

    if( pMsgExc == nullptr )
    {
        QString strExcType;

        try
        {
            bResult = QApplication::notify(i_pObjRcv,i_pEv);
        }
        catch( ZS::System::CException* pExc )
        {
            // This type of exception has already been logged on throwing (creating) the exception.
            pMsgExc = new CMsgErr(
                /* pObjSender   */ this,
                /* pObjReceiver */ QCoreApplication::instance(),
                /* exc          */ *pExc,
                /* strExcType   */ "ZS::System::CException" );

            // Dialogs may only be opened from within the GUI main thread.
            if( QThread::currentThread() != thread() )
            {
                QApplication::postEvent(this,pMsgExc);
                pMsgExc = nullptr;
                throw;
            }
            else
            {
                // The error dialog may be opened immediately, the exception
                // message don't need to be posted and therefore hast to be
                // destroyed within the notify method.
                bDeleteMsg = true;
            }
        }
        catch( ZS::System::CException& exc )
        {
            // This type of exception has already been logged on throwing (creating) the exception.
            pMsgExc = new CMsgErr(
                /* pObjSender   */ this,
                /* pObjReceiver */ QCoreApplication::instance(),
                /* exc          */ exc,
                /* strExcType   */ "ZS::System::CException" );

            // Dialogs may only be opened from within the GUI main thread.
            if( QThread::currentThread() != thread() )
            {
                QApplication::postEvent(this,pMsgExc);
                pMsgExc = nullptr;
                throw;
            }
            else
            {
                // The error dialog may be opened immediately, the exception
                // message don't need to be posted and therefore hast to be
                // destroyed within the notify method.
                bDeleteMsg = true;
            }
        }
        catch(...)
        {
            // An ZS::System::CException instance will be temporarily created.
            // This exception instance is used to create an exception error message.
            // The ctor of this exception adds an entry of this event to the error log model.
            ZS::System::CException exc(
                /* strFile    */ "",
                /* iLineNr    */ -1,
                /* result     */ EResultUnknownException,
                /* strAddInfo */ "" );
            pMsgExc = new CMsgErr(
                /* pObjSender   */ this,
                /* pObjReceiver */ QCoreApplication::instance(),
                /* exc          */ exc,
                /* strExcType   */ "Unknown exception" );

            // Dialogs may only be opened from within the GUI main thread.
            if( QThread::currentThread() != thread() )
            {
                QApplication::postEvent(this,pMsgExc);
                pMsgExc = nullptr;
                throw;
            }
            else
            {
                // The error dialog may be opened immediately, the exception
                // message don't need to be posted and therefore hast to be
                // destroyed within the notify method.
                bDeleteMsg = true;
            }
        } // catch(...)
    } // if( pMsgExc == nullptr )

    if( pMsgExc != nullptr )
    {
        // Exceptions have already been logged either on throwing (creating) a "known" exception
        // of type ZS::System::CException or if an unknown exception has been caught above.

        QDateTime         dateTime = QDateTime::currentDateTime().toUTC();
        SErrResultInfo    errResultInfo = pMsgExc->getErrResultInfo();
        QMessageBox::Icon icon = QMessageBox::NoIcon;

        QString strBtnAbort;
        QString strBtnIgnore;

        switch( pMsgExc->getSeverity() )
        {
            case EResultSeveritySuccess:
            {
                icon = QMessageBox::Information;
                break;
            }
            case EResultSeverityInfo:
            {
                icon = QMessageBox::Information;
                break;
            }
            case EResultSeverityWarning:
            {
                icon = QMessageBox::Warning;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            case EResultSeverityError:
            {
                icon = QMessageBox::Warning;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            case EResultSeverityCritical:
            {
                icon = QMessageBox::Critical;
                strBtnAbort  = "[Abort] \t to terminate the application";
                strBtnIgnore = "[Ignore]\t to continue execution but with unpredictable behavior of the program.";
                break;
            }
            default:
            {
                break;
            }
        }

        QString strErrMsg;

        strErrMsg += "Severity: \t" + errResultInfo.getSeverityStr() + "\n";
        strErrMsg += "Error:    \t" + errResultInfo.getResultStr() + "\n";
        strErrMsg += "Source:   \t" + errResultInfo.getErrSource().toString() + "\n";

        if( !errResultInfo.getAddErrInfoDscr().isEmpty() )
        {
            strErrMsg += "AddInfo: \t" + errResultInfo.getAddErrInfoDscr() + "\n";
        }
        strErrMsg += "\n";

        strErrMsg += "Exception was thrown from:\n\n";
        strErrMsg += "File:   \t" + pMsgExc->getErrSource().getFilePath() + "\n";
        strErrMsg += "Line:   \t" + QString::number(pMsgExc->getErrSource().getLineNr()) + "\n";
        strErrMsg += "Thread: \t" + pMsgExc->getErrSource().getThreadName() + "\n\n";

        QString strDateTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        QString strSysTime = QString::number(ZS::System::Time::getProcTimeInMicroSec(),'f',6);

        strErrMsg.insert( 0, strDateTime + "\n" + strSysTime + "\n\n" );

        if( !strBtnAbort.isEmpty() )
        {
            strErrMsg += strBtnAbort + "\n";
        }
        if( !strBtnIgnore.isEmpty() )
        {
            strErrMsg += strBtnIgnore + "\n";
        }

        QMessageBox msgBox(
            /* icon     */ icon,
            /* strTitle */ QCoreApplication::applicationName(),
            /* strText  */ strErrMsg );
        msgBox.setModal(true);

        switch( pMsgExc->getSeverity() )
        {
            case EResultSeveritySuccess:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setEscapeButton(QMessageBox::Ok);
                #endif
                break;
            }
            case EResultSeverityInfo:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setEscapeButton(QMessageBox::Ok);
                #endif
                break;
            }
            case EResultSeverityWarning:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            case EResultSeverityError:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            case EResultSeverityCritical:
            {
                msgBox.addButton("Ignore",QMessageBox::RejectRole);
                msgBox.addButton("Abort",QMessageBox::AcceptRole);
                #if QT_VERSION >= QT_VERSION_CHECK(4, 5, 1)
                msgBox.setDefaultButton(QMessageBox::Abort);
                msgBox.setEscapeButton(QMessageBox::Abort);
                #endif
                break;
            }
            default:
            {
                msgBox.addButton("Ok",QMessageBox::AcceptRole);
                break;
            }
        }

        int iDlgCode = msgBox.exec();

        if( bDeleteMsg )
        {
            delete pMsgExc;
            pMsgExc = nullptr;
        }
        if( iDlgCode == QDialog::Accepted )
        {
            // Quit the application (terminate main event loop)
            quit();
        }
    } // if( pMsgExc != nullptr )

    return bResult;

} // notify
