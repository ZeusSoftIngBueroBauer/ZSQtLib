/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qdatetime.h>
#include <QtCore/qsettings.h>
#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qvalidator.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtextedit.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#endif

#include "ZSDataGUI/ZSDatabaseTrcMsgLogWdgt.h"
#include "ZSData/ZSDatabase.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysTime.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
//#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::Data;
using namespace ZS::Data::GUI;


/*******************************************************************************
class CWdgtDbTrcMsgLog : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDbTrcMsgLog::CWdgtDbTrcMsgLog(
    const ZS::Ipc::GUI::STrcMsgLogSettings& i_settings,
    QWidget*                                /*i_pWdgtParent*/ ) :
//------------------------------------------------------------------------------
    Ipc::GUI::CWdgtTrcMsgLog(i_settings),
    m_pDb(nullptr)
{
    setObjectName("DbTrcMsgLogWdgt");

} // ctor

//------------------------------------------------------------------------------
CWdgtDbTrcMsgLog::~CWdgtDbTrcMsgLog()
//------------------------------------------------------------------------------
{
    setDb(nullptr);

    m_pDb = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbTrcMsgLog::setDb( CDb* i_pDb )
//------------------------------------------------------------------------------
{
    if( m_pDb != i_pDb )
    {
        if( m_pDb != nullptr )
        {
            QObject::disconnect(
                /* pObjSender     */ m_pDb,
                /* szSignal       */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver   */ this,
                /* szSlot         */ SLOT(onDbDestroyed(QObject*)) );

            m_pDb->removeTrcMsgLogObject(this);
        }

        m_pDb = i_pDb;

        if( m_pDb != nullptr )
        {
            m_pDb->addTrcMsgLogObject(this);

            // Use "direct connection" to avoid that the "onDestroyed" slot is called after
            // the object has already been completely destroyed.
            if( !QObject::connect(
                /* pObjSender     */ m_pDb,
                /* szSignal       */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver   */ this,
                /* szSlot         */ SLOT(onDbDestroyed(QObject*)),
                /* connectionType */ Qt::DirectConnection ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    } // if( m_pDb != i_pDb )

} // setDb

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbTrcMsgLog::onDbDestroyed( QObject* /*i_pDb*/ )
//------------------------------------------------------------------------------
{
    setDb(nullptr);
}
