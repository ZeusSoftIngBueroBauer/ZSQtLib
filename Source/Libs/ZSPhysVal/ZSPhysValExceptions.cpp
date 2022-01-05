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

#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysValException : public CException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValException::CPhysValException(
    const QString&  i_strFile,
    int             i_iLine,
    EResult         i_result,
    const QString&  i_strAddErrInfo,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CException(
        /* strFile       */ i_strFile,
        /* iLine         */ i_iLine,
        /* result        */ i_result,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* severity      */ i_severity,
        /* bAdd2ErrLog   */ false )
{
} // ctor

//------------------------------------------------------------------------------
CPhysValException::CPhysValException( const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CException(i_errResultInfo, false)
{
} // ctor

//------------------------------------------------------------------------------
CPhysValException::~CPhysValException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CUnitConversionException : public CPhysValException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitConversionException::CUnitConversionException(
    const QString&  i_strFile,
    int             i_iLine,
    EResult         i_result,
    const QString&  i_strAddErrInfo,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CPhysValException(
        /* strFile       */ i_strFile,
        /* iLine         */ i_iLine,
        /* result        */ i_result,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CUnitConversionException::CUnitConversionException( const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CPhysValException(i_errResultInfo)
{
} // ctor

//------------------------------------------------------------------------------
CUnitConversionException::~CUnitConversionException()
//------------------------------------------------------------------------------
{
} // dtor
