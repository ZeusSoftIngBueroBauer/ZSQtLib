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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qthread.h>
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysTime.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an exception instance.

    @param i_strFile [in] Name of file throwing the exception.
    @param i_iLine [in] Line number in the file throwing the exception.
    @param i_result [in] Error result code.
    @param i_strAddErrInfo [in] Addition info about the error.
    @param i_severity [in] Severity of the error result (defaults to Critical).
    @param i_bAdd2ErrLog [in] Flag specifying whether an entry to the error log
                              should be added (defaults to true).
*/
CException::CException(
    const QString&  i_strFile,
    int             i_iLine,
    EResult         i_result,
    const QString&  i_strAddErrInfo,
    EResultSeverity i_severity,
    bool            i_bAdd2ErrLog ) :
//------------------------------------------------------------------------------
    m_errResultInfo(
        /* errSource     */ i_strFile, i_iLine,
        /* result        */ i_result,
        /* severity      */ i_severity,
        /* strAddErrInfo */ i_strAddErrInfo )
{
    if( i_bAdd2ErrLog )
    {
        CErrLog* pErrLog = CErrLog::GetInstance();

        if( pErrLog != nullptr )
        {
            addErrorLogFileEntry();
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Constructs an exception instance.

    @param i_errResultInfo [in] Error result info containing information about
                                the error that occurred.
    @param i_bAdd2ErrLog [in] Flag specifying whether an entry to the error log
                              should be added (defaults to true).
*/
CException::CException( const SErrResultInfo& i_errResultInfo, bool i_bAdd2ErrLog ) :
//------------------------------------------------------------------------------
    m_errResultInfo(i_errResultInfo)
{
    if( i_bAdd2ErrLog )
    {
        CErrLog* pErrLog = CErrLog::GetInstance();

        if( pErrLog != nullptr )
        {
            addErrorLogFileEntry();
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Constructs an exception instance.

    @param i_strFile [in] Name of file throwing the exception.
    @param i_iLine [in] Line number in the file throwing the exception.
    @param i_errResultInfo [in] Error result info containing information about
                                the error that occurred.
    @param i_bAdd2ErrLog [in] Flag specifying whether an entry to the error log
                              should be added (defaults to true).
*/
CException::CException(
    const QString&        i_strFile,
    int                   i_iLine,
    const SErrResultInfo& i_errResultInfo,
    bool                  i_bAdd2ErrLog ) :
//------------------------------------------------------------------------------
    m_errResultInfo(i_errResultInfo)
{
    m_errResultInfo.setErrSource(i_strFile, i_iLine);

    if( i_bAdd2ErrLog )
    {
        CErrLog* pErrLog = CErrLog::GetInstance();

        if( pErrLog != nullptr )
        {
            addErrorLogFileEntry();
        }
    }
} // ctor

//------------------------------------------------------------------------------
/*! Destroys the exception instance.
*/
CException::~CException()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! Converts the content of the exception to a string.

    Assuming that the default separaotr "\n" is given the string may look like:

    @code
    "ZS::System::CException( Critical, IdxOutOfRange )\n
     ZS::Test::Database::CDataTable::Customers.addRow\n"
    @endcode
*/
QString CException::toString( const QString& i_strSeparator ) const
//------------------------------------------------------------------------------
{
    QString str;

    str  = type() + "( " + getSeverityStr() + ", " + getResultStr() + " )" + i_strSeparator;
    str += "Source: " + getErrSource().toString() + i_strSeparator;

    return str;

} // toString

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Internal method called by the ctors to add the error to the error log.
*/
void CException::addErrorLogFileEntry()
//------------------------------------------------------------------------------
{
    CErrLog* pErrLog = CErrLog::GetInstance();

    if( pErrLog != nullptr )
    {
        pErrLog->addEntry(m_errResultInfo);

        m_errResultInfo.setAddedToErrLogModel();
    }
} // addErrorLogFileEntry
