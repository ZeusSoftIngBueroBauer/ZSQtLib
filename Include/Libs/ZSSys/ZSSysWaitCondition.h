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

#ifndef ZSSys_WaitCondition_h
#define ZSSys_WaitCondition_h

#include <QtCore/qdeadlinetimer.h>
#include <QtCore/qstring.h>
#include <QtCore/qwaitcondition.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace System
{
class CMutex;
class CTrcAdminObj;
class CTrcMthFile;

//******************************************************************************
/*! @brief Wrapper around the QWaitCondition class with the possibility to trace
           the method class using the ZSTrcServer.
*/
//******************************************************************************
class ZSSYSDLL_API CWaitCondition : public QWaitCondition
{
    static QString NameSpace() { return "ZS::System"; }
    static QString ClassName() { return "CWaitCondition"; }
public: // ctors and dtor
    CWaitCondition(const QString& i_strObjName);
    CWaitCondition(const QString& i_strObjName, EMethodTraceDetailLevel i_eTrcMthFileDetailLevel);
    virtual ~CWaitCondition();
public: // instance methods
    void setObjectName(const QString& i_strObjName);
    QString objectName() const;
public: // instance methods
    void notify_all();
    void notify_one();
    #if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    bool wait(CMutex* i_pMutexLocked, QDeadlineTimer i_deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    bool wait(QReadWriteLock* i_pReadWriteLock, QDeadlineTimer i_deadline = QDeadlineTimer(QDeadlineTimer::Forever));
    #endif
    bool wait(CMutex* i_pMutexLocked, unsigned long i_uTime_ms);
    bool wait(QReadWriteLock* i_pReadWriteLock, unsigned long i_uTime_ms);
    void wakeAll();
    void wakeOne();
public: // auxiliary methods
    bool isMethodTraceActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    EMethodTraceDetailLevel getMethodTraceDetailLevel() const;
private: // instance members
    /*!< Descriptive name of the wait condition object. */
    QString m_strObjName;
    /*<! Trace detail level used if the method trace of the client got to be output
         directly to a trace method file and not through the trace server. */
    EMethodTraceDetailLevel m_eTrcMthFileDetailLevel;
    /*<! Reference to local trace method file. Used if the Mutex belongs to
         the trace server itself and tracing through trace server cannot be used. */
    CTrcMthFile* m_pTrcMthFile;
     /*!< Trace admin object to control the method trace if used by trace server. */
    CTrcAdminObj* m_pTrcAdminObj;
};

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_WaitCondition_h
