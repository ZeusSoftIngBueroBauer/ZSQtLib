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

#ifndef ZSSys_ErrCode_h
#define ZSSys_ErrCode_h

#include <QtCore/qstring.h>

#include "ZSSys/ZSSysEnumEntry.h"

namespace ZS
{
namespace System
{
//==============================================================================
/*! Enumeration for setting the severity of an error code.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    Error numbers are defined by the enum EResult. Whether an error number actually
    corresponds to an error depends on the method called.

    The result code "ObjAlreadyInList" can be used in different ways.

    - When calling "canAdd" and found that the object already exists in the list,
      the ResultCode has an informative character.
    - If "add" is called and it is found that the object already exists in the list,
      this is usually treated as a real error.
    - If the "add" method was triggered by the user, this should be interpreted as an
      error and the "add" request may be rejected with a suitable error message.
    - If the "add" method was called internally and it should not happen that one and
      the same object already appears in the list, this would have to be treated as a
      critical error and an exception might have to be thrown.

    @see ZS::System::resultSeverity2Str
    @see ZS::System::str2ResultSeverity
*/
enum EResultSeverity {
//==============================================================================
    EResultSeveritySuccess  = 0,    /*!< ![Success](Libs\ZSSys\ResultSeveritySuccess.png) Used e.g. to indicate no error condition. */
    EResultSeverityInfo     = 1,    /*!< ![Info](Libs\ZSSys\ResultSeverityInfo.png) The result code has informative character. */
    EResultSeverityWarning  = 2,    /*!< ![Warning](Libs\ZSSys\ResultSeverityWarning.png) The result code is used to create a warning. */
    EResultSeverityError    = 3,    /*!< ![Error](Libs\ZSSys\ResultSeverityError.png) The result code indicates an error. */
    EResultSeverityCritical = 4,    /*!< ![Criticial](Libs\ZSSys\ResultSeverityCritical.png) The result code indicates a critical error. */
    EResultSeverityCount,           /*!< Number of valid result severities. */
    EResultSeverityUndefined        /*!< Used to indicate an unitialized severity value. */
};

ZSSYSDLL_API QString resultSeverity2Str( EResultSeverity i_severity, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API EResultSeverity str2ResultSeverity( const QString& i_strSeverity, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrUndefined );

//==============================================================================
/*! Result codes.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @see ZS::System::result2Str
    @see ZS::System::str2Result
*/
enum EResult {
//==============================================================================
    EResultSuccess                           =   0, /*!< No Error. */
    EResultNoErrorMIN                        =   0, /*!< Lowest result code indicating No Error. Used in method ZS::System::isErrorResult(int). */
    EResultSuccessIntermediateResults        =   1, /*!< Indicates no error. But the request has not yet finished. */
    EResultInvalid                           =   2, /*!< The result code is invalid. Will be set later. */
    EResultUndefined                         =   3, /*!< Indicates an not yet initialized result code variable. */
    EResultNoErrorMAX                        =   3, /*!< Highest result code indicating No Error. Used in method ZS::System::isErrorResult(int). */
    EResultError                             =   4, /*!< Indicates any error. */
    EResultUnknownError                      =   5, /*!< An unknown error has occurred. */
    EResultUnknownException                  =   6, /*!< An unknown exception was raised. */
    EResultUnexpected                        =   7, /*!< An unexpected result was delivered. */
    EResultInternalProgramError              =   8, /*!< There is an error within the program. */
    EResultInternalStateMachineError         =   9, /*!< The state machine is in an error state and cannot process the current requests. */
    EResultSignalSlotConnectionFailed        =  10, /*!< The QObjekt::connect call was unsuccessful because of probably wrong method signatures. */
    EResultMemLeak                           =  11, /*!< A Speicker leak was discovered. */
    EResultOutOfMemory                       =  12, /*!< The program no longer gets the required memory. */
    EResultArgOutOfRange                     =  13, /*!< An argument with an invalid value was passed to a method. */
    EResultIdxOutOfRange                     =  14, /*!< The index value is outside the permitted limits. */
    EResultTimeout                           =  15, /*!< A timeout has occurred. */
    EResultListIsEmpty                       =  16, /*!< The list has no elements. */
    EResultListNotEmpty                      =  17, /*!< The list is not empty. */
    EResultListIsFull                        =  18, /*!< The list is full. */
    EResultListCorrupted                     =  19, /*!< The content of the list is incorrect. */
    EResultEndOfList                         =  20, /*!< The end of the list has been reached. */
    EResultObjNotInList                      =  21, /*!< The item is not in the list. */
    EResultObjAlreadyInList                  =  22, /*!< The element is already in the list. */
    EResultObjRefCounterIsZero               =  23, /*!< The reference counter is 0. */
    EResultObjRefCounterIsNotZero            =  24, /*!< The reference counter is not 0. */
    EResultInvalidObjId                      =  25, /*!< The object identified is invalid. */
    EResultInvalidObjName                    =  26, /*!< The name of the object is not valid. */
    EResultInvalidObjGUID                    =  27, /*!< The object's GUID is not valid. */
    EResultInvalidObjType                    =  28, /*!< The type of the object is not valid. */
    EResultInvalidObjAddress                 =  29, /*!< The address of the object is invalid. */
    EResultObjNameNotUnique                  =  30, /*!< The name of the object is not unique. */
    EResultObjCreateFailed                   =  31, /*!< The object could be created. */
    EResultObjDestroyFailed                  =  32, /*!< The object could not be destroyed as expected. */
    EResultObjActivateFailed                 =  33, /*!< The object could not be activated. */
    EResultObjDeactivateFailed               =  34, /*!< The object could not be deactivated. */
    EResultObjRegisterFailed                 =  35, /*!< The object could not be registered. */
    EResultObjUnregisterFailed               =  36, /*!< The object could not be unregistered. */
    EResultObjNotFound                       =  37, /*!< The object was not found. */
    EResultObjNotDefined                     =  38, /*!< The object is not defined. */
    EResultObjNotRegistered                  =  39, /*!< The object is not registered. */
    EResultObjAlreadyRegistered              =  40, /*!< The object is already registered. */
    EResultObjFactoryNotDefined              =  41, /*!< The factory object is not defined. */
    EResultObjFactoryNotFound                =  42, /*!< The factory object was not found. */
    EResultNumberOverflow                    =  43, /*!< A range overflow has occurred. */
    EResultNumberUnderflow                   =  44, /*!< A number underflow has occurred. */
    EResultDivisionByZero                    =  45, /*!< It was divided by 0. */
    EResultInvalidDynamicTypeCast            =  46, /*!< The object could not be converted to the target type. */
    EResultOptionNotSupported                =  47, /*!< The option is not supported. */
    EResultOptionNotYetSupported             =  48, /*!< The option is not currently supported. */
    EResultMethodNotYetImplemented           =  49, /*!< The method has not yet been implemented. */
    EResultInvalidMethodCall                 =  50, /*!< The method should not have been called. */
    EResultInitModuleFailed                  =  51, /*!< The module could not be initialized. */
    EResultFinalizeModuleFailed              =  52, /*!< The module could not be completed as expected. */
    EResultModuleNotInitialised              =  53, /*!< The module is not initialized. */
    EResultModuleAlreadyInitialised          =  54, /*!< The module has already been initialized. */
    EResultInitClassFailed                   =  55, /*!< The class could not be initialized. */
    EResultFinalizeClassFailed               =  56, /*!< The class could not be completed as expected. */
    EResultClassNotInitialised               =  57, /*!< The class is not initialized. */
    EResultClassAlreadyInitialised           =  58, /*!< The class has already been initialized. */
    EResultSingletonClassNotInstantiated     =  59, /*!< No instance of the singleton class was created. */
    EResultSingletonClassAlreadyInstantiated =  60, /*!< An instance of the Singleton class has already been created. */
    EResultUnexpectedMessage                 =  61, /*!< The message was not expected. */
    EResultMessageTypeMismatch               =  62, /*!< The type of the message is not as expected. */
    EResultMessageStateMismatch              =  63, /*!< The message was not expected in its current state. */
    EResultInvalidMessage                    =  64, /*!< The message is invalid. */
    EResultInvalidMessageContent             =  65, /*!< The message contains invalid content. */
    EResultInvalidSenderObjInMessage         =  66, /*!< The message contains an invalid object ID for the sender. */
    EResultInvalidReceiverObjInMessage       =  67, /*!< The message contains an invalid object ID for the recipient. */
    EResultMethodStateMismatch               =  68, /*!< The method was not expected in its current state. */
    EResultRequestRefused                    =  69, /*!< Processing of the order was rejected. */
    EResultRequestAborted                    =  70, /*!< The processing of the order was canceled. */
    EResultRequestExecutionError             =  71, /*!< An error occurred while processing the order. */
    EResultUnfinishedOperation               =  72, /*!< The operation could not be completed successfully. */
    EResultInvalidFileName                   =  73, /*!< The file name is invalid. */
    EResultFileNotFound                      =  74, /*!< The file was not found. */
    EResultFileOpenError                     =  75, /*!< An error occurred while opening the file. */
    EResultFileOpenForWrite                  =  76, /*!< The file could not be opened for writing. */
    EResultFileOpenForRead                   =  77, /*!< The file could not be opened for reading. */
    EResultFileNotOpen                       =  78, /*!< The file is not open. */
    EResultFileReadContent                   =  79, /*!< An error occurred while reading from the file. */
    EResultFileWriteContent                  =  80, /*!< An error occurred while writing to the file. */
    EResultSessionOpenError                  =  81, /*!< The session could not be opened. */
    EResultSessionCloseError                 =  82, /*!< The session could not be closed as expected. */
    EResultSessionNotOpen                    =  83, /*!< The meeting is not open. */
    EResultServerNotFound                    =  84, /*!< The server was not found. */
    EResultMaxConnectionsExceeded            =  85, /*!< The maximum number of connections has been reached. */
    EResultSocketIdOutOfRange                =  86, /*!< The socket ID is outside the valid range. */
    EResultSocketIdAlreadyUsed               =  87, /*!< The socket ID is already in use. */
    EResultConnectionRefused                 =  88, /*!< The connection establishment was rejected. */
    EResultConnectionError                   =  89, /*!< It was not possible to make a connection. */
    EResultNoConnection                      =  90, /*!< There was no connection. */
    EResultRemoteHostClosed                  =  91, /*!< The remote host has closed the connection. */
    EResultHostNotFound                      =  92, /*!< The remote host was not found. */
    EResultSocketAccessError                 =  93, /*!< An error occurred while accessing the socket. */
    EResultSocketResourceError               =  94, /*!< The socket has a resource problem. */
    EResultSocketTimeout                     =  95, /*!< The socket operation could not be completed within the expected period. */
    EResultSocketProtocolError               =  96, /*!< There is an error in the Socket Protocol. */
    EResultDatagramTooLarge                  =  97, /*!< The datagram is too big. */
    EResultNetworkError                      =  98, /*!< A network error has occurred. */
    EResultAddressAlreadyInUse               =  99, /*!< The address is already in use. */
    EResultSocketAddressNotAvailable         = 100, /*!< The socket address is not available. */
    EResultUnsupportedSocketOperation        = 101, /*!< The socket operation is not supported. */
    EResultUnfinishedSocketOperation         = 102, /*!< The socket operation was not completed as expected. */
    EResultProxyAuthenticationRequired       = 103, /*!< Authentication error with the proxy. */
    EResultSslHandshakeFailed                = 104, /*!< SSL handshake failed. */
    EResultProxyConnectionRefused            = 105, /*!< The proxy has rejected the connection request. */
    EResultProxyConnectionClosed             = 106, /*!< The proxy connection is closed. */
    EResultProxyTimeout                      = 107, /*!< The proxy operation could not be completed within the expected period. */
    EResultProxyNotFound                     = 108, /*!< The proxy was not found. */
    EResultProxyProtocol                     = 109, /*!< There is an error in the proxy protocol. */
    EResultReadyReadTimeout                  = 110, /*!< The reading of the data could not be completed within the expected period. */
    EResultTooMuchData                       = 111, /*!< There is too much data. */
    EResultStringParseError                  = 112, /*!< An error occurred while parsing the string. */
    EResultPermissionDenied                  = 113, /*!< Access was denied. */
    EResultSIUnitNotDefined                  = 114, /*!< SI unit is not defined. */
    EResultPhysSizeNotInitialized            = 115, /*!< The physical size has not been initialized. */
    EResultDifferentPhysSizes                = 116, /*!< The values ​​do not belong to the same physical size. */
    EResultInvalidPhysSize                   = 117, /*!< The physical size is not valid. */
    EResultInvalidPhysUnit                   = 118, /*!< The physical unit is not valid. */
    EResultInvalidRefVal                     = 119, /*!< The reference value is invalid. */
    EResultFctConvertInvalid                 = 120, /*!< There was an error converting the value. */
    EResultFctConvertNotDefined              = 121, /*!< The conversion function is not defined. */
    EResultConversionFailed                  = 122, /*!< The value could not be converted. */
    EResultMixedLinLogInValAndRes            = 123, /*!< Linear and logarithmic representation of value and resolution do not match. */
    EResultMixedLinLogInMathOp               = 124, /*!< Linear and logarithmic representation of value and resolution when working the method do not match. */
    EResultInvalidValue                      = 125, /*!< An invalid value was recognized. */
    EResultNotConvertible                    = 126, /*!< The value could not be converted. */
    EResultInvalidSIUnit                     = 127, /*!< The SI base unit is invalid. */
    EResultPhysSizeNotChangeable             = 128, /*!< The physical size cannot be changed. */
    EResultRefValMissing                     = 129, /*!< The reference value is missing. */
    EResultInvalidRefValUnit                 = 130, /*!< The reference value has an invalid unit. */
    EResultLogarithmicUnitExpected           = 131, /*!< A logarithmic unit was not expected. */
    EResultDimensionOutOfRange               = 132, /*!< The dimension is outside the valid range. */
    EResultValIdxOutOfRange                  = 133, /*!< The value has an invalid index. */
    EResultValueOutOfRange                   = 134, /*!< The value is outside the valid range. */
    EResultValueExceedsMinLimit              = 135, /*!< The value falls below the permissible minimum. */
    EResultValueExceedsMaxLimit              = 136, /*!< The value exceeds the permissible maximum. */
    EResultNoValueAssigned                   = 137, /*!< No value has been assigned. */
    EResultNullValueNotAllowed               = 138, /*!< A (zero) value is not allowed. */
    EResultValueCorrected                    = 139, /*!< The value has been corrected. */
    EResultInvalidDataType                   = 140, /*!< The data type is not valid. */
    EResultUnknownDataType                   = 141, /*!< The data type is not known. */
    EResultUnsupportedDataType               = 142, /*!< The data type is not supported. */
    EResultPrimaryKeyConstraintViolation     = 143, /*!< The primary key is not valid. */
    EResultUniqueConstraintViolation         = 144, /*!< The key is not clear. */
    EResultForeignKeyConstraintViolation     = 145, /*!< The key to accessing another resource is not valid. */
    EResultStatementError                    = 146, /*!< Errors within a statement. */
    EResultTransactionError                  = 147, /*!< Error within a transfer. */
    EResultCount                                    /*!< Total number of result codes. */
};

ZSSYSDLL_API QString result2Str( EResult i_result, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API EResult str2Result( const QString& i_strResult, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrUndefined );
ZSSYSDLL_API bool isErrorResult( EResult i_result );

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_ErrCode_h
