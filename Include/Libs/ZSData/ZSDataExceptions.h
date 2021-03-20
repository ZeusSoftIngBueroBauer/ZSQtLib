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

#ifndef ZSData_Exceptions_h
#define ZSData_Exceptions_h

#include "ZSData/ZSDataDllMain.h"
#include "ZSSys/ZSSysException.h"

namespace ZS
{
namespace Data
{
//******************************************************************************
class ZSDATADLL_API CDataException : public ZS::System::CException
//******************************************************************************
{
public: // ctors and dtor
    CDataException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CDataException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CDataException();
public: // overridables of base class CException
    virtual QString type() const { return "UnspecificDataException"; }

}; // class CDataException

//******************************************************************************
class ZSDATADLL_API CConstraintException : public CDataException
/* Represents the exception that is thrown when attempting an action that
   violates a constraint.
*******************************************************************************/
{
public: // ctors and dtor
    CConstraintException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CConstraintException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CConstraintException();
public: // overridables of base class CException
    virtual QString type() const { return "ConstraintViolation"; }

}; // class CConstraintException

//******************************************************************************
class ZSDATADLL_API CInvalidTypeException : public CDataException
/* Represents the exception that is thrown when an invalid database object type
   is encountered during an add or a remove operation in a DataSet related object.
*******************************************************************************/
{
public: // ctors and dtor
    CInvalidTypeException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultInvalidObjType,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CInvalidTypeException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CInvalidTypeException();
public: // overridables of base class CException
    virtual QString type() const { return "InvalidType"; }

}; // class CInvalidTypeException

//******************************************************************************
class ZSDATADLL_API CInvalidNameException : public CDataException
/* Represents the exception that is thrown when an invalid database object name
   is encountered during a remove operation in a DataSet related object.
*******************************************************************************/
{
public: // ctors and dtor
    CInvalidNameException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultInvalidObjName,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CInvalidNameException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CInvalidNameException();
public: // overridables of base class CException
    virtual QString type() const { return "InvalidName"; }

}; // class CInvalidNameException

//******************************************************************************
class ZSDATADLL_API CUnknownNameException : public CDataException
/* Represents the exception that is thrown when an unknown database object name
   is encountered during a remove operation in a DataSet related object.
*******************************************************************************/
{
public: // ctors and dtor
    CUnknownNameException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultInvalidObjName,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CUnknownNameException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CUnknownNameException();
public: // overridables of base class CException
    virtual QString type() const { return "UnkonwnName"; }

}; // class CUnknownNameException

//******************************************************************************
class ZSDATADLL_API CDuplicateNameException : public CDataException
/* Represents the exception that is thrown when a duplicate database object name
   is encountered during an add operation in a DataSet related object.
*******************************************************************************/
{
public: // ctors and dtor
    CDuplicateNameException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultInvalidObjName,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CDuplicateNameException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CDuplicateNameException();
public: // overridables of base class CException
    virtual QString type() const { return "DuplicateName"; }

}; // class CDuplicateNameException

//******************************************************************************
class ZSDATADLL_API CInRowChangingEventException : public CDataException
/* Represents the exception that is thrown when you call the EndEdit method
   within the RowChanging event.
*******************************************************************************/
{
public: // ctors and dtor
    CInRowChangingEventException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CInRowChangingEventException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CInRowChangingEventException();
public: // overridables of base class CException
    virtual QString type() const { return "InRowChangingEvent"; }

}; // class CInRowChangingEventException

//******************************************************************************
class ZSDATADLL_API CInvalidConstraintException : public CDataException
/* Represents the exception that is thrown when incorrectly trying to create or
   access a relation.
*******************************************************************************/
{
public: // ctors and dtor
    CInvalidConstraintException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CInvalidConstraintException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CInvalidConstraintException();
public: // overridables of base class CException
    virtual QString type() const { return "InvalidConstraint"; }

}; // class CInvalidConstraintException

//******************************************************************************
class ZSDATADLL_API CInvalidExpressionException : public CDataException
/* Represents the exception that is thrown when you try to add a DataColumn that
   contains an invalid Expression to a DataColumnCollection
*******************************************************************************/
{
public: // ctors and dtor
    CInvalidExpressionException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CInvalidExpressionException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CInvalidExpressionException();
public: // overridables of base class CException
    virtual QString type() const { return "InvalidExpression"; }

}; // class CInvalidExpressionException

//******************************************************************************
class ZSDATADLL_API CMissingPrimaryKeyException : public CDataException
/* Represents the exception that is thrown when you try to access a row in a
   table that has no primary key
*******************************************************************************/
{
public: // ctors and dtor
    CMissingPrimaryKeyException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CMissingPrimaryKeyException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CMissingPrimaryKeyException();
public: // overridables of base class CException
    virtual QString type() const { return "MissingPrimaryKey"; }

}; // class CMissingPrimaryKeyException

//******************************************************************************
class ZSDATADLL_API CNoNullAllowedException : public CDataException
/* Represents the exception that is thrown when you try to insert a null value
   into a column where AllowDBNull is set to false
*******************************************************************************/
{
public: // ctors and dtor
    CNoNullAllowedException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultNullValueNotAllowed,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CNoNullAllowedException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CNoNullAllowedException();
public: // overridables of base class CException
    virtual QString type() const { return "NoNullValueAllowed"; }

}; // class CNoNullAllowedException

//******************************************************************************
class ZSDATADLL_API CReadOnlyException : public CDataException
/* Represents the exception that is thrown when you try to change the value of
   a read-only column
*******************************************************************************/
{
public: // ctors and dtor
    CReadOnlyException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CReadOnlyException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CReadOnlyException();
public: // overridables of base class CException
    virtual QString type() const { return "ColumnIsReadOnly"; }

}; // class CReadOnlyException

//******************************************************************************
class ZSDATADLL_API CRowNotInTableException : public CDataException
/* Represents the exception that is thrown when you try to perform an operation
   on a DataRow that is not in a DataTable
*******************************************************************************/
{
public: // ctors and dtor
    CRowNotInTableException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CRowNotInTableException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CRowNotInTableException();
public: // overridables of base class CException
    virtual QString type() const { return "RowNotInTable"; }

}; // class CRowNotInTableException

//******************************************************************************
class ZSDATADLL_API CStrongTypingException : public CDataException
/* The exception that is thrown by a strongly typed DataSet when the user
   accesses a DBNull value
*******************************************************************************/
{
public: // ctors and dtor
    CStrongTypingException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CStrongTypingException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CStrongTypingException();
public: // overridables of base class CException
    virtual QString type() const { return "DBNullValueAccessed"; }

}; // class CStrongTypingException

//******************************************************************************
class ZSDATADLL_API CUpdateException : public CDataException
/* The exception that is thrown when modifications to object instances cannot
   be persisted to the data source
*******************************************************************************/
{
public: // ctors and dtor
    CUpdateException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CUpdateException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CUpdateException();
public: // overridables of base class CException
    virtual QString type() const { return "UpdateError"; }

}; // class CUpdateException

//******************************************************************************
class ZSDATADLL_API COptimisticConcurrencyException : public CUpdateException
/* The exception that is thrown when an optimistic concurrency violation occurs.
*******************************************************************************/
{
public: // ctors and dtor
    COptimisticConcurrencyException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    COptimisticConcurrencyException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~COptimisticConcurrencyException();
public: // overridables of base class CException
    virtual QString type() const { return "OptimisticConcurrencyViolation"; }

}; // class COptimisticConcurrencyException

//******************************************************************************
class ZSDATADLL_API CVersionNotFoundException : public CDataException
/* Represents the exception that is thrown when you try to return a version of
   a DataRow that has been deleted
*******************************************************************************/
{
public: // ctors and dtor
    CVersionNotFoundException(
        const QString&              i_strFile,
        unsigned int                i_uLine,
        const QString&              i_strAddErrInfo = "",
        ZS::System::EResult         i_result = ZS::System::EResultError,
        ZS::System::EResultSeverity i_severity = ZS::System::EResultSeverityError );
    CVersionNotFoundException(
        const QString&                    i_strFile,
        unsigned int                      i_uLine,
        const ZS::System::SErrResultInfo& i_errResultInfo );
    virtual ~CVersionNotFoundException();
public: // overridables of base class CException
    virtual QString type() const { return "VersionNotFound"; }

}; // class CVersionNotFoundException

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSData_Exceptions_old_h
