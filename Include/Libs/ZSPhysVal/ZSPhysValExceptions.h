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

#ifndef ZSPhysVal_Exceptions_h
#define ZSPhysVal_Exceptions_h

#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysException.h"

namespace ZS
{
namespace PhysVal
{
//******************************************************************************
class ZSPHYSVALDLL_API CPhysValException : public ZS::System::CException
//******************************************************************************
{
public: // ctors and dtor
    CPhysValException(
        const QString&  i_strFile,
        int             i_iLine,
        ZS::System::EResult         i_result,
        const QString&  i_strAddErrInfo = "",
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CPhysValException( const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CPhysValException();
public: // overridables of base class CException
    virtual QString type() const { return "UnspecificPhysValException"; }

}; // class CPhysValException

//******************************************************************************
class ZSPHYSVALDLL_API CUnitConversionException : public CPhysValException
/* Represents the exception that is thrown when attempting an action that
   violates a constraint.
*******************************************************************************/
{
public: // ctors and dtor
    CUnitConversionException(
        const QString&  i_strFile,
        int             i_iLine,
        ZS::System::EResult         i_result,
        const QString&  i_strAddErrInfo = "",
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CUnitConversionException( const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CUnitConversionException();
public: // overridables of base class CException
    virtual QString type() const { return "UnitConversionFailed"; }

}; // class CUnitConversionException

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_Exceptions_h
