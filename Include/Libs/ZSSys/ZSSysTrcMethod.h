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

#ifndef ZSSys_TrcMethod_h
#define ZSSys_TrcMethod_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysTrcAdminObj.h"

#include <QtCore/qobject.h>


/*******************************************************************************
public type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace System
{
class CMsg;
}

namespace Trace
{
class CTrcAdminObj;
class CTrcMthFile;
class CTrcServer;

//******************************************************************************
class ZSSYSDLL_API CMethodTracer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CMethodTracer(  // instance tracer (name space, class and object name set at trace admin object)
        CTrcAdminObj*  i_pTrcAdminObj,
        int            i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer(  // class tracer (name space and class name (but not object name) set at trace admin object)
        CTrcAdminObj*  i_pTrcAdminObj,
        int            i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer( // method trace without trace admin object but with trace server
        CTrcServer*    i_pTrcServer,
        int            i_iTrcDetailLevel,      // Entering and leaving the method is traced if the method trace detail level is greater or equal than the filter setting than the detail level.
        int            i_iFilterDetailLevel,   // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer( // method trace without trace admin object but with trace method file
        CTrcMthFile*   i_pTrcMthFile,
        int            i_iTrcDetailLevel,       // Entering and leaving the method is traced if the method trace detail level is greater or equal than the filter setting than the detail level.
        int            i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer( // method trace either with trace admin object or with trace method file
        CTrcAdminObj*  i_pTrcAdminObj,          // if != nullptr TrcMthFile is expected to be nullptr
        CTrcMthFile*   i_pTrcMthFile,           // if != nullptr TrcAdminObj is expected to be nullptr
        int            i_iTrcDetailLevel,       // Entering and leaving the method is traced if the method trace detail level is greater or equal than the filter setting than the detail level (only used if TrcMthFiel is != nullptr).
        int            i_iFilterDetailLevel,    // Entering and leaving the method is traced if the admin objects detail level is greater or equal than the filter setting than the detail level.
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased();
public: // instance methods
    int getTraceDetailLevel() const;
    int getEnterLeaveFilterDetailLevel() const { return m_iEnterLeaveFilterDetailLevel; }
public: // instance methods
    QString getNameSpace() const;
    QString getClassName() const;
    QString getObjectName() const;
    QString getMethod() const { return m_strMethod; }
public: // instance methods
    void setMethodReturn( bool i_bResult );
    void setMethodReturn( int i_iResult );
    void setMethodReturn( ZS::System::EResult i_result );
    void setMethodReturn( const ZS::System::SErrResultInfo& i_errResultInfo );
    void setMethodReturn( const QString& i_str );
    QString getMethodReturn() const { return m_strMethodReturn; }
public: // instance methods
    void setMethodOutArgs( const QString& i_str );
    QString getMethodOutArgs() const { return m_strMethodOutArgs; }
public: // instance methods
    bool isActive( int i_iFilterDetailLevel ) const;
public: // instance methods
    void trace( // Trace detail level defined by trace admin object or by ctor of method tracer
        const QString& i_strAddInfo,
        int            i_iFilterDetailLevel = ETraceDetailLevelInternalStates ) const;
    void trace( // Trace detail level defined by calling module, class or class instance
        const QString& i_strAddInfo,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel ) const;
protected slots:
    void onAdminObjDestroyed( QObject* i_pObj );
protected: // instance members
    CTrcAdminObj* m_pTrcAdminObj;                 /*!< = nullptr, if pTrcMthFile != nullptr or pTrcServer != nullptr. */
    CTrcServer*   m_pTrcServer;                   /*!< = nullptr, if pTrcAdminObj != nullptr or pTrcMthFile != nullptr. */
    CTrcMthFile*  m_pTrcMthFile;                  /*!< = nullptr, if pTrcAdminObj != nullptr or pTrcServer != nullptr. */
    int           m_iTrcDetailLevel;              /*!< = Undefined, if pTrcAdminObj != nullptr. */
    int           m_iEnterLeaveFilterDetailLevel; /*!< Filter setting for the detail level of the module or class creating the method tracer.
                                                       Entering and leaving the method is traced if this DbgOut level is greater or equal
                                                       the admin objects detail level. */
    QString       m_strNameSpace;                 /*!< Without a trace admin object the name space (if there is any) should be set at the method tracer. */
    QString       m_strClassName;                 /*!< Without a trace admin object the class name (if there is any) should be set at the method tracer. */
    QString       m_strObjName;                   /*!< If the object name is not set at the trace admin object. */
    QString       m_strMethod;                    /*!< Name of method to be traced. */
    QString       m_strMethodReturn;              /*!< The dtor will trace the method return value which has to be set therefore before the method tracer is destroyed. */
    QString       m_strMethodOutArgs;             /*!< The dtor will trace the method output arguments which have to be set therefore before the method tracer is destroyed. */

}; // class CMethodTracer

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_TrcMethod_h
