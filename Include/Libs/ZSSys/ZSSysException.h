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

#ifndef ZSSys_Exception_h
#define ZSSys_Exception_h

#include <QtCore/qstring.h>
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief Base class for exceptions.

    This exception class summarises an error result info data structure.

    You can use this class like this:

    @code
    try
    {
        SErrResultInfo errResultInfo(
            ** errSource  ** "ZS::System", "CEnum", "", "toEnumerator",
            ** result     ** EResultIdxOutOfRange,
            ** severity   ** EResultSeverityCritical,
            ** strAddInfo ** "The enumerator value " + QString::number(enumerator) + " is out of range" );
        throw CException(__FILE__, __LINE__, errResultInfo);
    }
    catch( CException& exc )
    {
        errDialog.show(exc.toString());
    }
    @endcode
*/
class ZSSYSDLL_API CException
//******************************************************************************
{
public: // ctors and dtor
    CException(
        const QString&  i_strFile,
        int             i_iLine,
        EResult         i_result,
        const QString&  i_strAddErrInfo = "",
        EResultSeverity i_severity = EResultSeverityCritical,
        bool            i_bAdd2ErrLog = true );
    CException( const SErrResultInfo& i_errResultInfo, bool i_bAdd2ErrLog = true );
    CException(
        const QString&        i_strFile,
        int                   i_iLine,
        const SErrResultInfo& i_errResultInfo,
        bool                  i_bAdd2ErrLog = true );
    virtual ~CException();
public: // overridables
    /*! Returns the type of exception which is the namespace followed by the class name.
        May be overridden by derived classes. */
    virtual QString type() const { return "ZS::System::CException"; }
public: // overridables
    /*! Returns the error result info. */
    virtual SErrResultInfo getErrResultInfo() const { return m_errResultInfo; }
    /*! Returns the error result (without info, just result and severity). */
    SErrResult getErrResult() const { return m_errResultInfo.getErrResult(); }
    /*! Returns the severity of the error result. */
    virtual EResultSeverity getSeverity() const { return m_errResultInfo.getSeverity(); }
    /*! Returns the string representation of the severity. */
    virtual QString getSeverityStr() const { return m_errResultInfo.getSeverityStr(); }
    /*! Returns the error result code. */
    virtual EResult getResult() const { return m_errResultInfo.getResult(); }
    /*! Returns the string representation of the severity. */
    virtual QString getResultStr() const { return m_errResultInfo.getResultStr(); }
    /*! Returns the source of the error. */
    virtual SErrSource getErrSource() const { return m_errResultInfo.getErrSource(); }
    /*! Returns addition error info. */
    virtual QString getAddErrInfo() const { return m_errResultInfo.getAddErrInfoDscr(); }
    /*! Returns the flag indicating whether the error has been added to the error log or not. */
    bool isErrResultAddedToErrLogModel() const { return m_errResultInfo.isAddedToErrLogModel(); }
public: // overridables
    virtual QString toString( const QString& i_strSeparator = "\n" ) const;
protected: // instance methods
    void addErrorLogFileEntry();
protected: // instance elements
    SErrResultInfo  m_errResultInfo;    /*!< Error result info indicating the cause of the exception. */

}; // class CException

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Exception_h
