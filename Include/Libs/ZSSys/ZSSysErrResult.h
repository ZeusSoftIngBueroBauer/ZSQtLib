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

#ifndef ZSSys_ErrResult_h
#define ZSSys_ErrResult_h

#include <QtCore/qstring.h>
#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysErrCode.h"

namespace ZS
{
namespace System
{
//******************************************************************************
/*! @brief This structure combines result severity and code into a tuple.

    Macros may be used to initialise an error result info structure to shorten the number of code lines.

    @code
    SErrResult errResult = ErrResultSuccess;
    SErrResult errResult = ErrResultUndefined;
    @endcode

    @ingroup _GRP_Libs_ZSSys_ErrorHandling
*/
struct ZSSYSDLL_API SErrResult
//******************************************************************************
{
    Q_GADGET
    Q_PROPERTY(QString severity READ getSeverityStr WRITE setSeverity)
    Q_PROPERTY(QString result READ getResultStr WRITE setResult)
public: // ctors and dtor
    SErrResult();
    SErrResult( EResultSeverity i_severity, EResult i_result );
    SErrResult( const SErrResult& i_other );
    ~SErrResult();
public: // operators
    SErrResult& operator = ( const SErrResult& i_other );
    bool operator == ( const SErrResult& i_other ) const;
    bool operator != ( const SErrResult& i_other ) const;
public: // instance methods
    /*! Returns true if the severity of the error is greator or equal the Error. */
    bool isErrorResult() const { return m_severity >= EResultSeverityError; }
public: // instance methods
    void setSeverity( EResultSeverity i_severity );
    void setSeverity( const QString& i_strSeverity );
    /*! Returns the severity of the error. */
    EResultSeverity getSeverity() const { return m_severity; }
    QString getSeverityStr() const;
public: // instance methods
    void setResult( EResult i_result );
    void setResult( const QString& i_strResult );
    /*! Returns the error result code. */
    EResult getResult() const { return m_result; }
    QString getResultStr() const;
public: // instance methods
    QString toString() const;
protected: // instance members
    EResultSeverity m_severity; /*!< Severity of the error. */
    EResult         m_result;   /*!< Result code. */

}; // struct SErrResult

//------------------------------------------------------------------------------
/*! Macro to initialise an error result tuple indicating no error.

    Example:

    @code
    SErrResult errResult = ErrResultSuccess;
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultSuccess ZS::System::SErrResult(ZS::System::EResultSeveritySuccess, ZS::System::EResultSuccess)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result tuple indicating an undefined (not yet set) error.

    Example:

    @code
    SErrResult errResult = ErrResultUndefined;
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultUndefined ZS::System::SErrResult(ZS::System::EResultSeveritySuccess, ZS::System::EResultUndefined)


//******************************************************************************
/*! @brief This structure defines the source of the error.

    @ingroup _GRP_Libs_ZSSys_ErrorHandling
*/
struct ZSSYSDLL_API SErrSource
//******************************************************************************
{
    Q_GADGET
    Q_PROPERTY(QString nameSpace READ getNameSpace WRITE setNameSpace)
    Q_PROPERTY(QString className READ getClassName WRITE setClassName)
    Q_PROPERTY(QString objectName READ getObjectName WRITE setObjectName)
    Q_PROPERTY(QString methodName READ getMethodName WRITE setMethodName)
    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath)
    Q_PROPERTY(int lineNr READ getLineNr WRITE setLineNr)
    Q_PROPERTY(QString threadName READ getThreadName WRITE setThreadName)
public: // ctors and dtor
    SErrSource();
    SErrSource(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMth );
    SErrSource( const QString& i_strFilePath, int i_iLineNr );
    SErrSource(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMth,
        const QString& i_strFilePath,
        int            i_iLineNr );
    SErrSource( const SErrSource& i_other );
    ~SErrSource();
public: // operators
    SErrSource& operator = ( const SErrSource& i_other );
    bool operator == ( const SErrSource& i_other ) const;
    bool operator != ( const SErrSource& i_other ) const;
public: // instance methods
    void setNameSpace( const QString& i_strNameSpace );
    /*! Returns the name space of the object generating the error. */
    QString getNameSpace() const { return m_strNameSpace; }
    void setClassName( const QString& i_strClassName );
    /*! Returns the class name of the object generating the error. */
    QString getClassName() const { return m_strClassName; }
    void setObjectName( const QString& i_strObjName );
    /*! Returns the name of the object generating the error. */
    QString getObjectName() const { return m_strObjName; }
public: // instance methods
    QString getObjectPath() const; // returns path string build with name space, class name and object name
    bool isObjectPathEmpty() const; // returns false if name space, class name and object name is not set
public: // instance methods
    void setMethodName( const QString& i_strMthName );
    /*! Returns the name of the method generating the error. */
    QString getMethodName() const { return m_strMthName; }
public: // instance methods
    void setFilePath( const QString& i_strFilePath );
    /*! Returns the modules file path generating the error. */
    QString getFilePath() const { return m_strFilePath; }
    void setLineNr( int i_iLineNr );
    /*! Returns the line number in the file generating the error. */
    int getLineNr() const { return m_iLineNr; }
public: // instance methods
    void setThreadName( const QString& i_strThreadName );
    /*! Returns the name of the thread generating the error. */
    QString getThreadName() const { return m_strThreadName; }
public: // instance methods
    QString toString() const;
protected: // instance members
    QString m_strNameSpace;     /*!< Namespace of the class producing the error. */
    QString m_strClassName;     /*!< Name of the class producing the error. */
    QString m_strObjName;       /*!< Name of the object (instance) producing the error. */
    QString m_strMthName;       /*!< Name of the method producing the error. */
    QString m_strFilePath;      /*!< Name of the file producing the error. */
    int     m_iLineNr;          /*!< Line number within the file producing the error. */
    QString m_strThreadName;    /*!< Name of the thread producing the error. */

}; // struct SErrSource


//******************************************************************************
/*! @brief This structure summarizes error code, severity, error source and additional
    Information about the error.

    A flag also indicates whether the error has already been stored in the error log.

    Macros may be used to initialise an error result info structure to shorten the number of code lines.

    @code
    SErrResultInfo errResultInfo = ErrResultInfoSuccess("add");
    SErrResultInfo errResultInfo = ErrResultInfoUndefined("add");
    SErrResultInfo errResultInfo = ErrResultInfoInfo("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    SErrResultInfo errResultInfo = ErrResultInfoWarning("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    SErrResultInfo errResultInfo = ErrResultInfoError("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    SErrResultInfo errResultInfo = ErrResultInfoCritical("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    @endcode

    @ingroup _GRP_Libs_ZSSys_ErrorHandling
*/
struct ZSSYSDLL_API SErrResultInfo
//******************************************************************************
{
    Q_GADGET
    Q_PROPERTY(SErrResult errResult READ getErrResult WRITE setErrResult)
    Q_PROPERTY(QString severity READ getSeverityStr WRITE setSeverity)
    Q_PROPERTY(QString result READ getResultStr WRITE setResult)
    Q_PROPERTY(SErrSource errSource READ getErrSource WRITE setErrSource)
    Q_PROPERTY(QString addErrInfoDscr READ getAddErrInfoDscr WRITE setAddErrInfoDscr)
public: // ctors and dtor
    SErrResultInfo();
    SErrResultInfo(
        const QString&  i_strNameSpace,
        const QString&  i_strClassName,
        const QString&  i_strObjName,
        const QString&  i_strMthName,
        EResult         i_result = EResultSuccess,
        EResultSeverity i_severity = EResultSeveritySuccess,
        const QString&  i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const QString&    i_strNameSpace,
        const QString&    i_strClassName,
        const QString&    i_strObjName,
        const QString&    i_strMthName,
        const SErrResult& i_errResult,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const QString&  i_strFilePath,
        int             i_iLineNr,
        EResult         i_result = EResultSuccess,
        EResultSeverity i_severity = EResultSeveritySuccess,
        const QString&  i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const QString&    i_strFilePath,
        int               i_iLineNr,
        const SErrResult& i_errResult,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const QString&  i_strNameSpace,
        const QString&  i_strClassName,
        const QString&  i_strObjName,
        const QString&  i_strMthName,
        const QString&  i_strFilePath,
        int             i_iLineNr,
        EResult         i_result = EResultSuccess,
        EResultSeverity i_severity = EResultSeveritySuccess,
        const QString&  i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const QString&    i_strNameSpace,
        const QString&    i_strClassName,
        const QString&    i_strObjName,
        const QString&    i_strMthName,
        const QString&    i_strFilePath,
        int               i_iLineNr,
        const SErrResult& i_errResult,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const SErrSource& i_errSource,
        EResult           i_result = EResultSuccess,
        EResultSeverity   i_severity = EResultSeveritySuccess,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const SErrSource& i_errSource,
        const SErrResult& i_errResult,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        EResult         i_result,
        EResultSeverity i_severity,
        const QString&  i_strAddErrInfoDscr = "" );
    SErrResultInfo(
        const SErrResult& i_errResult,
        const QString&    i_strAddErrInfoDscr = "" );
    SErrResultInfo( const SErrResultInfo& i_other );
    ~SErrResultInfo();
public: // operators
    SErrResultInfo& operator = ( const SErrResultInfo& i_other );
    bool operator == ( const SErrResultInfo& i_other ) const;
    bool operator != ( const SErrResultInfo& i_other ) const;
public: // instance methods
    /*! Returns true if the severity of the error is greater or equal to Error. */
    bool isErrorResult() const { return m_errResult.isErrorResult(); }
public: // instance methods
    void setErrResult( const SErrResult& i_errResult );
    /*! Returns the error result code together with the severity. */
    SErrResult getErrResult() const { return m_errResult; }
public: // instance methods
    void setSeverity( EResultSeverity i_severity );
    void setSeverity( const QString& i_strSeverity );
    /*! Returns the severity of the error. */
    EResultSeverity getSeverity() const { return m_errResult.getSeverity(); }
    /*! Returns the string representation of the errors severity. */
    QString getSeverityStr() const { return m_errResult.getSeverityStr(); }
public: // instance methods
    void setResult( EResult i_result );
    void setResult( const QString& i_strResult );
    /*! Returns the error result code. */
    EResult getResult() const { return m_errResult.getResult(); }
    /*! Returns the string representation of the errors result code. */
    QString getResultStr() const { return m_errResult.getResultStr(); }
public: // instance methods
    void setErrSource( const SErrSource& i_errSource );
    void setErrSource(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMthName );
    void setErrSource(
        const QString& i_strFilePath,
        int            i_iLineNr );
    void setErrSource(
        const QString& i_strNameSpace,
        const QString& i_strClassName,
        const QString& i_strObjName,
        const QString& i_strMthName,
        const QString& i_strFilePath,
        int            i_iLineNr );
    /*! Returns the error source. */
    SErrSource getErrSource() const { return m_errSource; }
public: // instance methods
    void setAddErrInfoDscr( const QString& i_strAddInfo );
    /*! Returns the additiona error info string. */
    QString getAddErrInfoDscr() const { return m_strAddErrInfoDscr; }
public: // instance methods
    void setAddedToErrLogModel( bool i_bAdded = true );
    /*! Returns the flag indicating whether the error has already been added to the error log. */
    bool isAddedToErrLogModel() const { return m_bAddedToErrLogModel; }
public: // instance methods
    QString toString( int i_iDetailLevel = 0 ) const;
protected: // instance members
    SErrResult m_errResult;             /*!< Error result code together with severity of error. */
    SErrSource m_errSource;             /*!< Source of the error. */
    QString    m_strAddErrInfoDscr;     /*!< Additional information about the error cause. */
    bool       m_bAddedToErrLogModel;   /*!< Flag defining whether the error has been added to the error log. */

}; // struct SErrResultInfo

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating no error.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoSuccess("add");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoSuccess(_strMth) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, ZS::System::EResultSuccess, ZS::System::EResultSeveritySuccess)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating and undefined (not yet set) error.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoUndefined("add");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoUndefined(_strMth) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, ZS::System::EResultUndefined, ZS::System::EResultSeveritySuccess)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating and error
    with severity Info.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.
    @param _result [in] Error result code.
    @param _strAddErrInfo [in] Additional info about the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoInfo("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoInfo(_strMth, _result, _strAddErrInfo) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, _result, ZS::System::EResultSeverityInfo, _strAddErrInfo)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating and error
    with severity Warning.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.
    @param _result [in] Error result code.
    @param _strAddErrInfo [in] Additional info about the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoWarning("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoWarning(_strMth, _result, _strAddErrInfo) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, _result, ZS::System::EResultSeverityWarning, _strAddErrInfo)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating and error
    with severity Error.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.
    @param _result [in] Error result code.
    @param _strAddErrInfo [in] Additional info about the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoError("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoError(_strMth, _result, _strAddErrInfo) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, _result, ZS::System::EResultSeverityError, _strAddErrInfo)

//------------------------------------------------------------------------------
/*! Macro to initialise an error result info structure indicating and error
    with severity Critical.

    Using this macro requires that the class provides the methods
    nameSpace, className and objectName.

    @param _strMth [in] Name of the method generating the error.
    @param _result [in] Error result code.
    @param _strAddErrInfo [in] Additional info about the error.

    Example:

    @code
    SErrResultInfo errResultInfo = ErrResultInfoCritical("add", EResultIdxOutOfRange, "RowIdx (=67) is out of range [0..10]");
    @endcode
*/
//------------------------------------------------------------------------------
#define ErrResultInfoCritical(_strMth, _result, _strAddErrInfo) ZS::System::SErrResultInfo(nameSpace(), className(), objectName(), _strMth, _result, ZS::System::EResultSeverityCritical, _strAddErrInfo)

} // namespace System

} // namespace ZS

#endif // ZSSys_ErrResult_h
