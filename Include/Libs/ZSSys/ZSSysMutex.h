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

#ifndef ZSSys_Mutex_h
#define ZSSys_Mutex_h

#include <QtCore/qmutex.h>
#include <QtCore/qstring.h>

#include "ZSSys/ZSSysDllMain.h"

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
class CTrcMthFile;
}
namespace System
{
//******************************************************************************
/*! @brief Wrapper around the QMutex class with the possibility to trace
           the method class using the ZSTrcServer.
*/
//******************************************************************************
class ZSSYSDLL_API CMutex : public QMutex
{
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMutex"; }
public: // ctors and dtor
    CMutex(QMutex::RecursionMode i_mode, const QString& i_strObjName);
    CMutex(const QString& i_strObjName);
    CMutex(QMutex::RecursionMode i_mode, const QString& i_strObjName, int i_iTrcMthFileDetailLevel);
    CMutex(const QString& i_strObjName, int i_iTrcMthFileDetailLevel);
    virtual ~CMutex();
public: // instance methods
    QString objectName() const;
    ZS::Trace::CTrcMthFile* traceMethodFile();
    int traceMethodFileDetailLevel() const;
public: // instance methods
    void lock();
    bool tryLock(int i_timeout_ms = 0);
    bool try_lock();
    template <typename Rep, typename Period>
    bool try_lock_for(std::chrono::duration<Rep, Period> i_duration);
    template <typename Clock, typename Duration>
    bool try_lock_until(std::chrono::time_point<Clock, Duration> i_timePoint);
    void unlock();
protected: // auxiliary methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
private: // instance members
    /*!< Descriptive name of the mutex. */
    QString m_strObjName;
    /*<! Trace detail level used if the method trace of the client got to be output
         directly to a trace method file and not through the trace server. */
    int m_iTrcMthFileDetailLevel;
    /*<! Reference to local trace method file. Used if the Mutex belongs to
         the trace server itself and tracing through trace server cannot be used. */
    ZS::Trace::CTrcMthFile* m_pTrcMthFile;
     /*!< Trace admin object to control the method trace if used by trace server. */
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
};

//******************************************************************************
/*! @brief Wrapper around the QMutex class with the possibility to trace
           the method class using the ZSTrcServer.
*/
//******************************************************************************
class ZSSYSDLL_API CRecursiveMutex : public QRecursiveMutex
{
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CRecursiveMutex"; }
public: // ctors and dtor
    CRecursiveMutex(const QString& i_strObjName);
    CRecursiveMutex( const QString& i_strObjName, int i_iTrcMthFileDetailLevel);
    virtual ~CRecursiveMutex();
public: // instance methods
    QString objectName() const;
    ZS::Trace::CTrcMthFile* traceMethodFile();
    int traceMethodFileDetailLevel() const;
public: // instance methods
    void lock();
    bool tryLock(int i_timeout_ms = 0);
    bool try_lock();
    template <typename Rep, typename Period>
    bool try_lock_for(std::chrono::duration<Rep, Period> i_duration);
    template <typename Clock, typename Duration>
    bool try_lock_until(std::chrono::time_point<Clock, Duration> i_timePoint);
    void unlock();
protected: // auxiliary methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
private: // instance members
    /*!< Descriptive name of the mutex. */
    QString m_strObjName;
    /*<! Trace detail level used if the method trace of the client got to be output
         directly to a trace method file and not through the trace server. */
    int m_iTrcMthFileDetailLevel;
    /*<! Reference to local trace method file. Used if the Mutex belongs to
         the trace server itself and tracing through trace server cannot be used. */
    ZS::Trace::CTrcMthFile* m_pTrcMthFile;
     /*!< Trace admin object to control the method trace if used by trace server. */
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
};

//******************************************************************************
/*! @brief Wrapper around the QMutexLocker class with the possibility to trace
           the method class using the ZSTrcServer.
*/
//******************************************************************************
class ZSSYSDLL_API CMutexLocker
{
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CMutexLocker"; }
public: // ctors and dtor
    CMutexLocker(CMutex* i_pMutex);
    CMutexLocker(CRecursiveMutex* i_pMutex);
    virtual ~CMutexLocker();
public: // instance methods
    QString objectName() const;
public: // instance methods
    void relock();
    void unlock();
protected: // auxiliary methods
    bool isMethodTraceActive( int i_iFilterDetailLevel ) const;
private: // instance members
    /*!< Mutex to be locked and unlocked. */
    CMutex* m_pMtx;
    /*!< Recursive mutex to be locked and unlocked. */
    CRecursiveMutex* m_pRecursiveMtx;
    /*<! Trace detail level used if the method trace of the client got to be output
         directly to a trace method file and not through the trace server. */
    int m_iTrcMthFileDetailLevel;
    /*<! Reference to local trace method file. Used if the Mutex belongs to
         the trace server itself and tracing through trace server cannot be used. */
    ZS::Trace::CTrcMthFile* m_pTrcMthFile;
     /*!< Trace admin object to control the method trace if used by trace server. */
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_Mutex_h