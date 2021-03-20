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

#include "ZSDataGUI/ZSDatabaseClientTrcMsgLogWdgt.h"
#include "ZSData/ZSDatabaseClient.h"
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
class CWdgtDbClientTrcMsgLog : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtDbClientTrcMsgLog::CWdgtDbClientTrcMsgLog(
    const ZS::Ipc::GUI::STrcMsgLogSettings& i_settings,
    QWidget*                                /*i_pWdgtParent*/ ) :
//------------------------------------------------------------------------------
    ZS::Ipc::GUI::CWdgtTrcMsgLog(i_settings),
    m_pDbClt(nullptr)
{
    setObjectName("DbClientTrsMsgLogWdgt");

} // ctor

//------------------------------------------------------------------------------
CWdgtDbClientTrcMsgLog::~CWdgtDbClientTrcMsgLog()
//------------------------------------------------------------------------------
{
    setDbClient(nullptr);

    m_pDbClt = nullptr;

} // dtor

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientTrcMsgLog::setDbClient( CDbClient* i_pDbClt )
//------------------------------------------------------------------------------
{
    if( m_pDbClt != i_pDbClt )
    {
        if( m_pDbClt != nullptr )
        {
            QObject::disconnect(
                /* pObjSender     */ m_pDbClt,
                /* szSignal       */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver   */ this,
                /* szSlot         */ SLOT(onDbClientDestroyed(QObject*)) );

            m_pDbClt->removeTrcMsgLogObject(this);
        }

        m_pDbClt = i_pDbClt;

        if( m_pDbClt != nullptr )
        {
            m_pDbClt->addTrcMsgLogObject(this);

            // Use "direct connection" to avoid that the "onDestroyed" slot is called after
            // the object has already been completely destroyed.
            if( !QObject::connect(
                /* pObjSender     */ m_pDbClt,
                /* szSignal       */ SIGNAL(destroyed(QObject*)),
                /* pObjReceiver   */ this,
                /* szSlot         */ SLOT(onDbClientDestroyed(QObject*)),
                /* connectionType */ Qt::DirectConnection ) )
            {
                throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
            }
        }
    } // if( m_pDbClt != i_pDbClt )

} // setDbClient

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtDbClientTrcMsgLog::onDbClientDestroyed( QObject* /*i_pDbClt*/ )
//------------------------------------------------------------------------------
{
    setDbClient(nullptr);
}
