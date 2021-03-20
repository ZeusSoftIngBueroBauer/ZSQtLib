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

#include <QtCore/qcoreapplication.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qthread.h>

#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysAux.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


//******************************************************************************
/*! Class to initialise the module.

    By creating a static instance within this module the ctor of the class
    is executed doing some initialization work for the module.
    This class registers the data types EResultSeverity, EResult and the
    structures SErrResult and SErrResultInfo as meta types for signal slot
    handling within by Qt. This way those data types can be passed as
    arguments when connecting signals and slots.
*/
class CInitModuleSysErrResult
//******************************************************************************
{
public: // ctor
    CInitModuleSysErrResult()
    {
        qRegisterMetaType<EResultSeverity>("EResultSeverity");
        qRegisterMetaType<ZS::System::EResultSeverity>("ZS::System::EResultSeverity");
        qRegisterMetaType<EResult>("EResult");
        qRegisterMetaType<ZS::System::EResult>("ZS::System::EResult");
        qRegisterMetaType<SErrResult>("SErrResult");
        qRegisterMetaType<ZS::System::SErrResult>("ZS::System::SErrResult");
        qRegisterMetaType<SErrResultInfo>("SErrResultInfo");
        qRegisterMetaType<ZS::System::SErrResultInfo>("ZS::System::SErrResultInfo");
    }
};

static CInitModuleSysErrResult s_initModule;



/*******************************************************************************
struct SErrResult
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an error result combining the result code Success with the
    severity Success.
*/
SErrResult::SErrResult() :
//------------------------------------------------------------------------------
    m_severity(EResultSeveritySuccess),
    m_result(EResultSuccess)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result combining the given result code with the given severity.

    @param i_severity [in] Severity of the result code.
    @param i_result [in] Error result code.
*/
SErrResult::SErrResult( EResultSeverity i_severity, EResult i_result ) :
//------------------------------------------------------------------------------
    m_severity(i_severity),
    m_result(i_result)
{
} // ctor

//------------------------------------------------------------------------------
/*! Copy constructor.

    @param i_other [in] Error result to be copied.
*/
SErrResult::SErrResult( const SErrResult& i_other ) :
//------------------------------------------------------------------------------
    m_severity(i_other.m_severity),
    m_result(i_other.m_result)
{
} // copy ctor

//------------------------------------------------------------------------------
/*! Destroys an error result structure by "nulling" its members.
*/
SErrResult::~SErrResult()
//------------------------------------------------------------------------------
{
    m_severity = static_cast<EResultSeverity>(0);
    m_result   = static_cast<EResult>(0);

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Assignment operator.

    @param i_other [in] Error result to be copied.
*/
SErrResult& SErrResult::operator = ( const SErrResult& i_other )
//------------------------------------------------------------------------------
{
    m_severity = i_other.m_severity;
    m_result   = i_other.m_result;

    return *this;

} // assignment operator

//------------------------------------------------------------------------------
/*! Equality comparison operator.

    @param i_other [in] Error result to be compared with.
*/
bool SErrResult::operator == ( const SErrResult& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_severity != i_other.m_severity )
    {
        bEqual = false;
    }
    else if( m_result != i_other.m_result )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
/*! Inequality comparison operator.

    @param i_other [in] Error result to be compared with.
*/
bool SErrResult::operator != ( const SErrResult& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the given severity.

    @param i_severity [in] Severity to be set.
*/
void SErrResult::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    m_severity = i_severity;
}

//------------------------------------------------------------------------------
/*! Returns the string representation of the severity.

    @return String representation of severity.
*/
QString SErrResult::getSeverityStr() const
//------------------------------------------------------------------------------
{
    return ZS::System::resultSeverity2Str(m_severity);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the given result.

    @param i_result [in] Result code to be set.
*/
void SErrResult::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    m_result = i_result;
}

//------------------------------------------------------------------------------
/*! Returns the string representation of the result code.

    @return String representation of result code.
*/
QString SErrResult::getResultStr() const
//------------------------------------------------------------------------------
{
    return ZS::System::result2Str(m_result);
}


/*******************************************************************************
struct SErrSource
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an empty error source structure.

    @note The struct member m_strThreadName is initialized with the name of the current thread.
*/
SErrSource::SErrSource() :
//------------------------------------------------------------------------------
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(),
    m_strMthName(),
    m_strFilePath(),
    m_iLineNr(-1),
    m_strThreadName()
{
    m_strThreadName = getCurrentThreadName();

} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error source structure with the given intialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.

    @note The struct member m_strThreadName is initialized with the name of the current thread.
*/
SErrSource::SErrSource(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMthName ) :
//------------------------------------------------------------------------------
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMthName(i_strMthName),
    m_strFilePath(),
    m_iLineNr(-1),
    m_strThreadName()
{
    m_strThreadName = getCurrentThreadName();

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an error source structure with the given intialisation values.

    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.

    @note The struct member m_strThreadName is initialized with the name of the current thread.
*/
SErrSource::SErrSource( const QString& i_strFilePath, int i_iLineNr ) :
//------------------------------------------------------------------------------
    m_strNameSpace(),
    m_strClassName(),
    m_strObjName(),
    m_strMthName(),
    m_strFilePath(i_strFilePath),
    m_iLineNr(i_iLineNr),
    m_strThreadName()
{
    m_strThreadName = getCurrentThreadName();

} // ctor

//------------------------------------------------------------------------------
/*! Constructs an error source structure with the given intialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.

    @note The struct member m_strThreadName is initialized with the name of the current thread.
*/
SErrSource::SErrSource(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMthName,
    const QString& i_strFilePath,
    int            i_iLineNr ) :
//------------------------------------------------------------------------------
    m_strNameSpace(i_strNameSpace),
    m_strClassName(i_strClassName),
    m_strObjName(i_strObjName),
    m_strMthName(i_strMthName),
    m_strFilePath(i_strFilePath),
    m_iLineNr(i_iLineNr),
    m_strThreadName()
{
    m_strThreadName = getCurrentThreadName();

} // ctor

//------------------------------------------------------------------------------
/*! Copy constructor.

    @param i_other [in] Error source structure to be copied.
*/
SErrSource::SErrSource( const SErrSource& i_other ) :
//------------------------------------------------------------------------------
    m_strNameSpace(i_other.m_strNameSpace),
    m_strClassName(i_other.m_strClassName),
    m_strObjName(i_other.m_strObjName),
    m_strMthName(i_other.m_strMthName),
    m_strFilePath(i_other.m_strFilePath),
    m_iLineNr(i_other.m_iLineNr),
    m_strThreadName(i_other.m_strThreadName)
{
} // copy ctor

//------------------------------------------------------------------------------
/*! Destroys the error source structure by "nulling" its members.
*/
SErrSource::~SErrSource()
//------------------------------------------------------------------------------
{
    //m_strNameSpace;
    //m_strClassName;
    //m_strObjName;
    //m_strMth;
    //m_strFilePath;
    m_iLineNr = 0;
    //m_strThreadName;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Assignment operator.

    @param i_other [in] Error source structure to be copied.
*/
SErrSource& SErrSource::operator = ( const SErrSource& i_other )
//------------------------------------------------------------------------------
{
    m_strNameSpace  = i_other.m_strNameSpace;
    m_strClassName  = i_other.m_strClassName;
    m_strObjName    = i_other.m_strObjName;
    m_strMthName    = i_other.m_strMthName;
    m_strFilePath   = i_other.m_strFilePath;
    m_iLineNr       = i_other.m_iLineNr;
    m_strThreadName = i_other.m_strThreadName;

    return *this;

} // assignment operator

//------------------------------------------------------------------------------
/*! Equality comparison operator.

    @param i_other [in] Error source structure to be compared with.
*/
bool SErrSource::operator == ( const SErrSource& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_strNameSpace != i_other.m_strNameSpace )
    {
        bEqual = false;
    }
    else if( m_strClassName != i_other.m_strClassName )
    {
        bEqual = false;
    }
    else if( m_strObjName != i_other.m_strObjName )
    {
        bEqual = false;
    }
    else if( m_strMthName != i_other.m_strMthName )
    {
        bEqual = false;
    }
    else if( m_strFilePath != i_other.m_strFilePath )
    {
        bEqual = false;
    }
    else if( m_iLineNr != i_other.m_iLineNr )
    {
        bEqual = false;
    }
    else if( m_strThreadName != i_other.m_strThreadName )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
/*! Inequality comparison operator.

    @param i_other [in] Error source structure to be compared with.
*/
bool SErrSource::operator != ( const SErrSource& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the name space of the object generating the error.

    @param i_strNameSpace [in] Name space of the object generating the error.
*/
void SErrSource::setNameSpace( const QString& i_strNameSpace )
//------------------------------------------------------------------------------
{
    m_strNameSpace = i_strNameSpace;
}

//------------------------------------------------------------------------------
/*! Sets the class name of the object generating the error.

    @param i_strClassName [in] Class name of the object generating the error.
*/
void SErrSource::setClassName( const QString& i_strClassName )
//------------------------------------------------------------------------------
{
    m_strClassName = i_strClassName;
}

//------------------------------------------------------------------------------
/*! Sets the name of the object generating the error.

    @param i_strObjName [in] Name space of the object generating the error.
*/
void SErrSource::setObjectName( const QString& i_strObjName )
//------------------------------------------------------------------------------
{
    m_strObjName = i_strObjName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the object path by joining the name space, class name and object
    name using "::" as seperator.

    @return The joined string (e.g. "ZS::System::CErrLog::ErrLog").
*/
QString SErrSource::getObjectPath() const
//------------------------------------------------------------------------------
{
    return buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);
}

//------------------------------------------------------------------------------
/*! Returns whether name space, class name or object name has been set.

    @return Returns true if neither name space nor class name nor object name has been set.
            Otherwise returns false.
*/
bool SErrSource::isObjectPathEmpty() const
//------------------------------------------------------------------------------
{
    return m_strNameSpace.isEmpty() && m_strClassName.isEmpty() && m_strObjName.isEmpty();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the name of the method generating the error.

    @param i_strMthName [in] Name of the method generating the error.
*/
void SErrSource::setMethodName( const QString& i_strMthName )
//------------------------------------------------------------------------------
{
    m_strMthName = i_strMthName;
}

//------------------------------------------------------------------------------
/*! Sets the modules file name generating the error.

    @param i_strFilePath [in] File name generating the error.
*/
void SErrSource::setFilePath( const QString& i_strFilePath )
//------------------------------------------------------------------------------
{
    m_strFilePath = i_strFilePath;
}

//------------------------------------------------------------------------------
/*! Sets the line number of the modules file name generating the error.

    @param i_iLineNr [in] Line number.
*/
void SErrSource::setLineNr( int i_iLineNr )
//------------------------------------------------------------------------------
{
    m_iLineNr = i_iLineNr;
}

//------------------------------------------------------------------------------
/*! Sets the name of the thread generating the error.

    @param i_strThreadName [in] Name of the thread generating the error.

    @note When creating a structur the thread name is initialised with the
          name of the current thread.
*/
void SErrSource::setThreadName( const QString& i_strThreadName )
//------------------------------------------------------------------------------
{
    m_strThreadName = i_strThreadName;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the string representation of the error source.

    @return String indicating the error source.

    Examples:

    @code
    "<MainThread>ZS::System::CErrLog::ErrLog.addEntry"
    "<MainThread>ZSSysErrLog.cpp(122)"
    "<MainThread>ZS::System::CErrLog::ErrLog.addEntry {ZSSysErrLog.cpp(122)}"
    @endcode
*/
QString SErrSource::toString() const
//------------------------------------------------------------------------------
{
    QString str;

    if( !m_strThreadName.isEmpty() )
    {
        str += "<" + m_strThreadName + ">";
    }

    str += buildPathStr("::", m_strNameSpace, m_strClassName, m_strObjName);

    if( !m_strMthName.isEmpty() )
    {
        if( !str.isEmpty() ) str += ".";
        str += m_strMthName;
    }

    if( !m_strFilePath.isEmpty() )
    {
        bool bBracket = !str.isEmpty();
        if( bBracket ) str += " {";
        str += m_strFilePath;
        if( m_iLineNr >= 0 ) str += "(" + QString::number(m_iLineNr) + ")";
        if( bBracket ) str += "}";
    }

    return str;

} // toString


/*******************************************************************************
struct SErrResultInfo
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with default values indicating Success.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo() :
//------------------------------------------------------------------------------
    m_errResult(EResultSeveritySuccess, EResultSuccess),
    m_errSource(),
    m_strAddErrInfoDscr(),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_result [in] Error result code.
    @param i_severity [in] Severity of the result code.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&  i_strNameSpace,
    const QString&  i_strClassName,
    const QString&  i_strObjName,
    const QString&  i_strMthName,
    EResult         i_result,
    EResultSeverity i_severity,
    const QString&  i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_severity, i_result),
    m_errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_errResult [in] Structure combining error result code with severity.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&    i_strNameSpace,
    const QString&    i_strClassName,
    const QString&    i_strObjName,
    const QString&    i_strMthName,
    const SErrResult& i_errResult,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_errResult),
    m_errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
    @param i_result [in] Error result code.
    @param i_severity [in] Severity of the result code.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&  i_strFilePath,
    int             i_iLineNr,
    EResult         i_result,
    EResultSeverity i_severity,
    const QString&  i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_severity, i_result),
    m_errSource(i_strFilePath, i_iLineNr),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
    @param i_errResult [in] Structure combining error result code with severity.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&    i_strFilePath,
    int               i_iLineNr,
    const SErrResult& i_errResult,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_errResult),
    m_errSource(i_strFilePath, i_iLineNr),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
    @param i_result [in] Error result code.
    @param i_severity [in] Severity of the result code.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&  i_strNameSpace,
    const QString&  i_strClassName,
    const QString&  i_strObjName,
    const QString&  i_strMthName,
    const QString&  i_strFilePath,
    int             i_iLineNr,
    EResult         i_result,
    EResultSeverity i_severity,
    const QString&  i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_severity, i_result),
    m_errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName, i_strFilePath, i_iLineNr),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
    @param i_errResult [in] Structure combining error result code with severity.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const QString&    i_strNameSpace,
    const QString&    i_strClassName,
    const QString&    i_strObjName,
    const QString&    i_strMthName,
    const QString&    i_strFilePath,
    int               i_iLineNr,
    const SErrResult& i_errResult,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_errResult),
    m_errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName, i_strFilePath, i_iLineNr),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_errSource [in] Structure containing information about the error source.
    @param i_result [in] Error result code.
    @param i_severity [in] Severity of the result code.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const SErrSource& i_errSource,
    EResult           i_result,
    EResultSeverity   i_severity,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_severity, i_result),
    m_errSource(i_errSource),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_errSource [in] Structure containing information about the error source.
    @param i_errResult [in] Structure combining error result code with severity.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const SErrSource& i_errSource,
    const SErrResult& i_errResult,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_errResult),
    m_errSource(i_errSource),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // default ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_result [in] Error result code.
    @param i_severity [in] Severity of the result code.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    EResult         i_result,
    EResultSeverity i_severity,
    const QString&  i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_severity,i_result),
    m_errSource(),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! Constructs an error result info structure with the given initialisation values.

    @param i_errResult [in] Structure combining error result code with severity.
    @param i_strAddErrInfoDscr [in] Additional info about the error.

    @note The member m_strThreadName of the error source is initialized with the
          name of the current thread.
*/
SErrResultInfo::SErrResultInfo(
    const SErrResult& i_errResult,
    const QString&    i_strAddErrInfoDscr ) :
//------------------------------------------------------------------------------
    m_errResult(i_errResult),
    m_errSource(),
    m_strAddErrInfoDscr(i_strAddErrInfoDscr),
    m_bAddedToErrLogModel(false)
{
} // ctor

//------------------------------------------------------------------------------
/*! Copy constructor.

    @param i_other [in] Error result info structure to be copied.
*/
SErrResultInfo::SErrResultInfo( const SErrResultInfo& i_other ) :
//------------------------------------------------------------------------------
    m_errResult(i_other.m_errResult),
    m_errSource(i_other.m_errSource),
    m_strAddErrInfoDscr(i_other.m_strAddErrInfoDscr),
    m_bAddedToErrLogModel(i_other.m_bAddedToErrLogModel)
{
} // copy ctor

//------------------------------------------------------------------------------
/*! Destroys the error source structure by "nulling" its members.
*/
SErrResultInfo::~SErrResultInfo()
//------------------------------------------------------------------------------
{
    //m_errResult;
    //m_errSource;
    //m_strAddErrInfoDscr;
    m_bAddedToErrLogModel = false;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! Assignment operator.

    @param i_other [in] Error result info structure to be copied.
*/
SErrResultInfo& SErrResultInfo::operator = ( const SErrResultInfo& i_other )
//------------------------------------------------------------------------------
{
    m_errResult = i_other.m_errResult;
    m_errSource = i_other.m_errSource;
    m_strAddErrInfoDscr = i_other.m_strAddErrInfoDscr;
    m_bAddedToErrLogModel = i_other.m_bAddedToErrLogModel;

    return *this;

} // assignment operator

//------------------------------------------------------------------------------
/*! Equality comparison operator.

    @param i_other [in] Error result info structure to be compared with.
*/
bool SErrResultInfo::operator == ( const SErrResultInfo& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_errResult != i_other.m_errResult )
    {
        bEqual = false;
    }
    else if( m_errSource != i_other.m_errSource )
    {
        bEqual = false;
    }
    else if( m_strAddErrInfoDscr != i_other.m_strAddErrInfoDscr )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
/*! Inequality comparison operator.

    @param i_other [in] Error result info structure to be compared with.
*/
bool SErrResultInfo::operator != ( const SErrResultInfo& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the result code and severity.

    @param i_errResult [in] Structure combining error result code with severity.
*/
void SErrResultInfo::setErrResult( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    if( m_errResult != i_errResult )
    {
        m_errResult = i_errResult;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setErrResult

//------------------------------------------------------------------------------
/*! Sets the severity.

    @param i_severity [in] Severity of the error.
*/
void SErrResultInfo::setSeverity( EResultSeverity i_severity )
//------------------------------------------------------------------------------
{
    if( m_errResult.getSeverity() != i_severity )
    {
        m_errResult.setSeverity(i_severity);

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setSeverity

//------------------------------------------------------------------------------
/*! Sets the result code.

    @param i_result [in] Error result code.
*/
void SErrResultInfo::setResult( EResult i_result )
//------------------------------------------------------------------------------
{
    if( m_errResult.getResult() != i_result )
    {
        m_errResult.setResult(i_result);

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setResult

//------------------------------------------------------------------------------
/*! Sets the error source.

    @param i_errSource [in] Structure defining the error source.
*/
void SErrResultInfo::setErrSource( const SErrSource& i_errSource )
//------------------------------------------------------------------------------
{
    if( m_errSource != i_errSource )
    {
        m_errSource = i_errSource;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setErrSource

//------------------------------------------------------------------------------
/*! Sets the error source to the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
*/
void SErrResultInfo::setErrSource(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMthName )
//------------------------------------------------------------------------------
{
    SErrSource errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName);

    errSource.setFilePath(m_errSource.getFilePath());
    errSource.setLineNr(m_errSource.getLineNr());

    if( m_errSource != errSource )
    {
        m_errSource = errSource;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setErrSource

//------------------------------------------------------------------------------
/*! Sets the error source to the given initialisation values.

    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
*/
void SErrResultInfo::setErrSource( const QString& i_strFilePath, int i_iLineNr )
//------------------------------------------------------------------------------
{
    SErrSource errSource(i_strFilePath, i_iLineNr);

    errSource.setNameSpace(m_errSource.getNameSpace());
    errSource.setClassName(m_errSource.getClassName());
    errSource.setObjectName(m_errSource.getObjectName());
    errSource.setMethodName(m_errSource.getMethodName());

    if( m_errSource != errSource )
    {
        m_errSource = errSource;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setErrSource

//------------------------------------------------------------------------------
/*! Sets the error source to the given initialisation values.

    @param i_strNameSpace [in] Name space of the object generating the error.
    @param i_strClassName [in] Class name of the object generating the error.
    @param i_strObjName [in] Name of the object generating the error.
    @param i_strMthName [in] Name of the method generating the error.
    @param i_strFilePath [in] The modules file name generating the error.
    @param i_iLineNr [in] The line number in the modules file name generating the error.
*/
void SErrResultInfo::setErrSource(
    const QString& i_strNameSpace,
    const QString& i_strClassName,
    const QString& i_strObjName,
    const QString& i_strMthName,
    const QString& i_strFilePath,
    int            i_iLineNr )
//------------------------------------------------------------------------------
{
    SErrSource errSource(i_strNameSpace, i_strClassName, i_strObjName, i_strMthName, i_strFilePath, i_iLineNr);

    if( m_errSource != errSource )
    {
        m_errSource = errSource;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setErrSource

//------------------------------------------------------------------------------
/*! Sets the additional error info.

    @param i_strAddInfo [in] Additional info about the error.
*/
void SErrResultInfo::setAddErrInfoDscr( const QString& i_strAddInfo )
//------------------------------------------------------------------------------
{
    if( m_strAddErrInfoDscr != i_strAddInfo )
    {
        m_strAddErrInfoDscr = i_strAddInfo;

        // On changing the result code, severity, error source or additional
        // description (see operator ==) a new error result info object is
        // created which has not yet been added to err log model.
        m_bAddedToErrLogModel = false;
    }
} // setAddErrInfoDscr

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the flag whether the error result info has been added to the error log.

    If the error result info is investigating several times this flag helps to
    avoid that the error result is added more than once to the error log.

    @param i_bAdded [in] true if the error result info has been added to the error log.
*/
void SErrResultInfo::setAddedToErrLogModel( bool i_bAdded )
//------------------------------------------------------------------------------
{
    m_bAddedToErrLogModel = i_bAdded;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Returns the string representation of the error result info.

    @return String indicating the error result info.

    Examples:

    @code
    "Critical.IdxOutOfRange"
    "Critical.IdxOutOfRange (<MainThread>ZS::System::CIdxTree::PhysUnits.add: Voltage already in index tree)"
    "Critical.IdxOutOfRange (<MainThread>ZSSysErrLog.cpp(122))"
    "Critical.IdxOutOfRange (<MainThread>ZS::System::CIdxTree::PhysUnits.add {ZSSysErrLog.cpp(122)})"
    @endcode
*/
QString SErrResultInfo::toString( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString str;

    QString strSeverity = getSeverityStr();
    QString strResult = getResultStr();
    QString strSource;
    QString strAddInfo;

    if( i_iDetailLevel >= 1 )
    {
        strSource = m_errSource.toString();
    }
    if( i_iDetailLevel >= 2 )
    {
        strAddInfo = getAddErrInfoDscr();
    }

    if( strSeverity == strResult )
    {
        str = strResult;
    }
    else
    {
        str = strSeverity + "." + strResult;
    }

    if( !strSource.isEmpty() && !strAddInfo.isEmpty() )
    {
        str += " (" + strSource + ": " + strAddInfo + ")";
    }
    else if( !strSource.isEmpty() )
    {
        str += " (" + strSource + ")";
    }
    else if( !strAddInfo.isEmpty() )
    {
        str += " (" + strAddInfo + ")";
    }

    return str;

} // toString
