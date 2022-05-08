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
}

namespace Trace
{
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
                ** iTrcDetailLevel    ** m_iTrcDetailLevel,
                ** iFilterDetailLevel ** ETraceDetailLevelMethodCalls,
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
        CTrcAdminObj*  i_pTrcAdminObj,
        int            i_iFilterDetailLevel,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer(
        CTrcAdminObj*  i_pTrcAdminObj,
        int            i_iFilterDetailLevel,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs );
    CMethodTracer(
        CTrcAdminObj*  i_pTrcAdminObj,
        CTrcServer*    i_pTrcServer,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer(
        CTrcServer*    i_pTrcServer,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer(
        CTrcMthFile*   i_pTrcMthFile,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel,
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMethod,
        const QString& i_strMethodInArgs = "" );
    CMethodTracer(
        CTrcAdminObj*  i_pTrcAdminObj,
        CTrcMthFile*   i_pTrcMthFile,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel,
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
    int getEnterLeaveFilterDetailLevel() const;
public: // instance methods
    QString getNameSpace() const;
    QString getClassName() const;
    QString getObjectName() const;
    QString getMethod() const;
public: // instance methods
    void setMethodReturn( bool i_bResult );
    void setMethodReturn( int i_iResult );
    void setMethodReturn( ZS::System::EResult i_result );
    void setMethodReturn( const ZS::System::SErrResultInfo& i_errResultInfo );
    void setMethodReturn( ZS::System::CRequest* i_pReq );
    void setMethodReturn( const QString& i_str );
    QString getMethodReturn() const;
public: // instance methods
    void setMethodOutArgs( const QString& i_str );
    QString getMethodOutArgs() const;
public: // instance methods
    bool isActive( int i_iFilterDetailLevel ) const;
public: // instance methods
    void trace( // Trace detail level defined by trace admin object or by ctor of method tracer
        const QString& i_strAddInfo,
        int            i_iFilterDetailLevel = ETraceDetailLevelMethodCalls ) const;
    void trace( // Trace detail level defined by calling module, class or class instance
        const QString& i_strAddInfo,
        int            i_iTrcDetailLevel,
        int            i_iFilterDetailLevel ) const;
protected slots:
    void onAdminObjAboutToBeDestroyed( ZS::Trace::CTrcAdminObj* i_pTrcAdminObj );
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
