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
class CRequest;
class CTrcAdminObj;
class CTrcMthFile;
class CTrcServer;

//******************************************************************************
/*! @brief The CMethodTracer class is a convenience class that simplifies tracing
           entering and leaving methods and should be created within a function
           when entering and leaving the function should be traced.

    Tracing leaving methods in complex functions and statements or in exception
    handling code may be error-prone. CMethodTracer can be used in such situations
    to ensure that leaving the method is always traced as the destructor of the
    CMethodTracer is used to trace leaving the function.

    @note If the CMethodTracer is used in the destructor of a class and a trace
          admin object is used the trace admin object is usually released in this
          destructor. In this case leaving the destructor cannot be traced if the
          CMethodTracer class as the trace admin object is no longer accessible
          if the destructor of the CMethodTracer class is called.
          So when using CMethodTracer in destructors the CMethodTracer got to be
          informed that the trace adming object will be released to trace leaving
          the method before the trace admin object is released. For this the method
          "onAdminObjAboutToBeReleased" has to be applied at the method tracer instance
          as this method will trace leaving the current method.

        @code

        CIdxTree::~CIdxTree()
        {
            QString strMthInArgs;

            CMethodTracer mthTracer(
                ** pTrcAdminObj       ** m_pTrcAdminObj,
                ** pTrcServer         ** CTrcServer::GetInstance(),
                ** iTrcDetailLevel    ** m_eTrcDetailLevel,
                ** iFilterDetailLevel ** EMethodTraceDetailLevel,
                ** strNameSpace       ** NameSpace(),
                ** strClassName       ** ClassName(),
                ** strObjName         ** objectName(),
                ** strMethod          ** "dtor",
                ** strMethodInArgs    ** strMthInArgs );

            ...

            if( m_pTrcAdminObj != nullptr )
            {
                mthTracer.onAdminObjAboutToBeReleased();

                CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
            }
        }
        @endcode
*/
class ZSSYSDLL_API CMethodTracer : public QObject
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CMethodTracer(
        CTrcAdminObj* i_pTrcAdminObj,
        EMethodTraceDetailLevel i_eFilterDetailLevel,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer(
        CTrcAdminObj* i_pTrcAdminObj,
        EMethodTraceDetailLevel i_eFilterDetailLevel,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer(
        CTrcMthFile* i_pTrcMthFile,
        EMethodTraceDetailLevel i_eTrcDetailLevel,
        EMethodTraceDetailLevel i_eFilterDetailLevel,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer(
        CTrcAdminObj* i_pTrcAdminObj,
        CTrcMthFile*  i_pTrcMthFile,
        EMethodTraceDetailLevel i_eTrcDetailLevel,
        EMethodTraceDetailLevel i_eFilterDetailLevel,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    ~CMethodTracer();
public: // instance methods
    void onAdminObjAboutToBeReleased();
public: // instance methods
    EMethodTraceDetailLevel getMethodCallsTraceDetailLevel() const;
    bool areMethodCallsActive( EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    bool areMethodCallsActive( const QString& i_strObjName, EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    bool areMethodCallsActive( const QString& i_strObjName, const QString& i_strMethodName, EMethodTraceDetailLevel i_eFilterDetailLevel ) const;
    EMethodTraceDetailLevel getEnterLeaveFilterDetailLevel() const;
    ELogDetailLevel getRuntimeInfoTraceDetailLevel() const;
    bool isRuntimeInfoActive( ELogDetailLevel i_eFilterDetailLevel ) const;
    bool isRuntimeInfoActive( const QString& i_strObjName, ELogDetailLevel i_eFilterDetailLevel ) const;
    bool isRuntimeInfoActive( const QString& i_strObjName, const QString& i_strMethodName, ELogDetailLevel i_eFilterDetailLevel ) const;
    bool isObjectNameSuppressedByFilter( const QString& i_strObjName ) const;
    bool isMethodNameSuppressedByFilter( const QString& i_strMethodName ) const;
    bool isTraceDataSuppressedByFilter( const QString& i_strTraceData ) const;
public: // instance methods
    QString getNameSpace() const;
    QString getClassName() const;
    QString getObjectName() const;
    QString getMethod() const;
public: // instance methods
    void setMethodReturn( bool i_bResult );
    void setMethodReturn( int i_iResult );
    void setMethodReturn( EResult i_result );
    void setMethodReturn( const SErrResultInfo& i_errResultInfo );
    void setMethodReturn( CRequest* i_pReq );
    void setMethodReturn( const QString& i_str );
    QString getMethodReturn() const;
public: // instance methods
    void setMethodOutArgs( const QString& i_str );
    QString getMethodOutArgs() const;
public: // instance methods
    void trace(
        const QString&  i_strAddInfo,
        ELogDetailLevel i_eFilterDetailLevel = ELogDetailLevel::Fatal ) const;
    void trace(
        const QString&  i_strAddInfo,
        ELogDetailLevel i_eTrcDetailLevel,
        ELogDetailLevel i_eFilterDetailLevel ) const;
protected slots:
    void onAdminObjAboutToBeDestroyed( QObject* i_pTrcAdminObj );
protected: // instance members
    /*!< = nullptr, if pTrcMthFile != nullptr */
    CTrcAdminObj* m_pTrcAdminObj;
    /*!< = nullptr, if pTrcAdminObj != nullptr */
    CTrcMthFile* m_pTrcMthFile;
    /*!< = Undefined, if pTrcAdminObj != nullptr. */
    EMethodTraceDetailLevel m_eMethodCallsTrcDetailLevel;
    /*!< Filter setting for the detail level of the module or class creating the method tracer.
         Entering and leaving the method is traced if this trace level is greater or equal
         the admin objects detail level. */
    EMethodTraceDetailLevel m_eEnterLeaveFilterDetailLevel;
    /*!< After entering a method the detail level of the trace admin object may be changed
         (multi threaded access). If entering the method has been traced leaving must also be traced
         even if the detail level has been set to None after entering the method.*/
    bool m_bEnterTraced;
    /*!< Without a trace admin object the name space (if there is any) should be set at the method tracer. */
    QString m_strNameSpace;
    /*!< Without a trace admin object the class name (if there is any) should be set at the method tracer. */
    QString m_strClassName;
    /*!< If the object name is not set at the trace admin object. */
    QString m_strObjName;
    /*!< Name of method to be traced. */
    QString m_strMethod;
    /*!< The dtor will trace the method return value which has to be set therefore before the method tracer is destroyed. */
    QString m_strMethodReturn;
    /*!< The dtor will trace the method output arguments which have to be set therefore before the method tracer is destroyed. */
    QString m_strMethodOutArgs;

}; // class CMethodTracer

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_TrcMethod_h
