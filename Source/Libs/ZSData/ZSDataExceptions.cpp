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

#include "ZSData/ZSDataExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Data;


/*******************************************************************************
class CDataException : public CException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDataException::CDataException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* result        */ i_result,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* severity      */ i_severity,
        /* bAdd2ErrLog   */ false )
{
} // ctor

//------------------------------------------------------------------------------
CDataException::CDataException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo,
        /* bAdd2ErrLog   */ false )
{
} // ctor

//------------------------------------------------------------------------------
CDataException::~CDataException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CConstraintException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CConstraintException::CConstraintException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CConstraintException::CConstraintException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CConstraintException::~CConstraintException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CInvalidTypeException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInvalidTypeException::CInvalidTypeException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidTypeException::CInvalidTypeException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidTypeException::~CInvalidTypeException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CInvalidNameException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInvalidNameException::CInvalidNameException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidNameException::CInvalidNameException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidNameException::~CInvalidNameException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CUnknownNameException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnknownNameException::CUnknownNameException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CUnknownNameException::CUnknownNameException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CUnknownNameException::~CUnknownNameException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CDuplicateNameException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDuplicateNameException::CDuplicateNameException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CDuplicateNameException::CDuplicateNameException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CDuplicateNameException::~CDuplicateNameException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CInRowChangingEventException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInRowChangingEventException::CInRowChangingEventException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CInRowChangingEventException::CInRowChangingEventException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CInRowChangingEventException::~CInRowChangingEventException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CInvalidConstraintException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInvalidConstraintException::CInvalidConstraintException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidConstraintException::CInvalidConstraintException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidConstraintException::~CInvalidConstraintException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CInvalidExpressionException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CInvalidExpressionException::CInvalidExpressionException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidExpressionException::CInvalidExpressionException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CInvalidExpressionException::~CInvalidExpressionException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CMissingPrimaryKeyException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CMissingPrimaryKeyException::CMissingPrimaryKeyException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CMissingPrimaryKeyException::CMissingPrimaryKeyException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CMissingPrimaryKeyException::~CMissingPrimaryKeyException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CNoNullAllowedException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CNoNullAllowedException::CNoNullAllowedException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CNoNullAllowedException::CNoNullAllowedException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CNoNullAllowedException::~CNoNullAllowedException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CReadOnlyException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CReadOnlyException::CReadOnlyException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CReadOnlyException::CReadOnlyException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CReadOnlyException::~CReadOnlyException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CRowNotInTableException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRowNotInTableException::CRowNotInTableException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CRowNotInTableException::CRowNotInTableException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CRowNotInTableException::~CRowNotInTableException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CStrongTypingException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CStrongTypingException::CStrongTypingException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CStrongTypingException::CStrongTypingException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CStrongTypingException::~CStrongTypingException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CUpdateException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUpdateException::CUpdateException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CUpdateException::CUpdateException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CUpdateException::~CUpdateException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class COptimisticConcurrencyException : public CUpdateException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
COptimisticConcurrencyException::COptimisticConcurrencyException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CUpdateException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
COptimisticConcurrencyException::COptimisticConcurrencyException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CUpdateException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
COptimisticConcurrencyException::~COptimisticConcurrencyException()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CVersionNotFoundException : public CDataException
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CVersionNotFoundException::CVersionNotFoundException(
    const QString&  i_strFile,
    unsigned int    i_uLine,
    const QString&  i_strAddErrInfo,
    EResult         i_result,
    EResultSeverity i_severity ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* strAddErrInfo */ i_strAddErrInfo,
        /* result        */ i_result,
        /* severity      */ i_severity )
{
} // ctor

//------------------------------------------------------------------------------
CVersionNotFoundException::CVersionNotFoundException(
    const QString&        i_strFile,
    unsigned int          i_uLine,
    const SErrResultInfo& i_errResultInfo ) :
//------------------------------------------------------------------------------
    CDataException(
        /* strFile       */ i_strFile,
        /* uLine         */ i_uLine,
        /* errResultInfo */ i_errResultInfo )
{
} // ctor

//------------------------------------------------------------------------------
CVersionNotFoundException::~CVersionNotFoundException()
//------------------------------------------------------------------------------
{
} // dtor
