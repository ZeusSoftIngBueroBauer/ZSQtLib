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

#ifndef ZSSys_ErrCode_h
#define ZSSys_ErrCode_h

#include <QtCore/qstring.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumEntry.h"

namespace ZS
{
namespace System
{
//==============================================================================
/*! Severities zum Festlegen der Gewichtung eines Fehlercodes.

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
    EResultSeveritySuccess  = 0,    /*!< ![Success](Libs\ZSSys\ResultSeveritySuccess.bmp) Used e.g. to indicate no error condition. */
    EResultSeverityInfo     = 1,    /*!< ![Info](Libs\ZSSys\ResultSeverityInfo.bmp) The result code has informative character. */
    EResultSeverityWarning  = 2,    /*!< ![Warning](Libs\ZSSys\ResultSeverityWarning.bmp) The result code is used to create a warning. */
    EResultSeverityError    = 3,    /*!< ![Error](Libs\ZSSys\ResultSeverityError.bmp) The result code indicates an error. */
    EResultSeverityCritical = 4,    /*!< ![Criticial](Libs\ZSSys\ResultSeverityCritical.bmp) The result code indicates a critical error. */
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
    EResultObjPoolNotDefined                 =  41, /*!< The pool object is not defined. */
    EResultObjPoolNotFound                   =  42, /*!< The object pool was not found. */
    EResultObjFactoryNotDefined              =  43, /*!< The factory object is not defined. */
    EResultObjFactoryNotFound                =  44, /*!< The factory object was not found. */
    EResultNumberOverflow                    =  45, /*!< A range overflow has occurred. */
    EResultNumberUnderflow                   =  46, /*!< A number underflow has occurred. */
    EResultDivisionByZero                    =  47, /*!< It was divided by 0. */
    EResultInvalidDynamicTypeCast            =  48, /*!< The object could not be converted to the target type. */
    EResultOptionNotSupported                =  49, /*!< The option is not supported. */
    EResultOptionNotYetSupported             =  50, /*!< The option is not currently supported. */
    EResultMethodNotYetImplemented           =  51, /*!< The method has not yet been implemented. */
    EResultInvalidMethodCall                 =  52, /*!< The method should not have been called. */
    EResultInitModuleFailed                  =  53, /*!< The module could not be initialized. */
    EResultFinalizeModuleFailed              =  54, /*!< The module could not be completed as expected. */
    EResultModuleNotInitialised              =  55, /*!< The module is not initialized. */
    EResultModuleAlreadyInitialised          =  56, /*!< The module has already been initialized. */
    EResultInitClassFailed                   =  57, /*!< The class could not be initialized. */
    EResultFinalizeClassFailed               =  58, /*!< The class could not be completed as expected. */
    EResultClassNotInitialised               =  59, /*!< The class is not initialized. */
    EResultClassAlreadyInitialised           =  60, /*!< The class has already been initialized. */
    EResultSingletonClassNotInstantiated     =  61, /*!< No instance of the singleton class was created. */
    EResultSingletonClassAlreadyInstantiated =  62, /*!< An instance of the Singleton class has already been created. */
    EResultUnexpectedMessage                 =  63, /*!< The message was not expected. */
    EResultMessageTypeMismatch               =  64, /*!< The type of the message is not as expected. */
    EResultMessageStateMismatch              =  65, /*!< The message was not expected in its current state. */
    EResultInvalidMessage                    =  66, /*!< The message is invalid. */
    EResultInvalidMessageContent             =  67, /*!< The message contains invalid content. */
    EResultInvalidSenderObjInMessage         =  68, /*!< The message contains an invalid object ID for the sender. */
    EResultInvalidReceiverObjInMessage       =  69, /*!< The message contains an invalid object ID for the recipient. */
    EResultMethodStateMismatch               =  70, /*!< The method was not expected in its current state. */
    EResultRequestRefused                    =  71, /*!< Processing of the order was rejected. */
    EResultRequestAborted                    =  72, /*!< The processing of the order was canceled. */
    EResultRequestExecutionError             =  73, /*!< An error occurred while processing the order. */
    EResultUnfinishedOperation               =  74, /*!< The operation could not be completed successfully. */
    EResultInvalidFileName                   =  75, /*!< The file name is invalid. */
    EResultFileNotFound                      =  76, /*!< The file was not found. */
    EResultFileOpenError                     =  77, /*!< An error occurred while opening the file. */
    EResultFileOpenForWrite                  =  78, /*!< The file could not be opened for writing. */
    EResultFileOpenForRead                   =  79, /*!< The file could not be opened for reading. */
    EResultFileNotOpen                       =  80, /*!< The file is not open. */
    EResultFileReadContent                   =  81, /*!< An error occurred while reading from the file. */
    EResultFileWriteContent                  =  82, /*!< An error occurred while writing to the file. */
    EResultSessionOpenError                  =  83, /*!< The session could not be opened. */
    EResultSessionCloseError                 =  84, /*!< The session could not be closed as expected. */
    EResultSessionNotOpen                    =  85, /*!< The meeting is not open. */
    EResultServerNotFound                    =  86, /*!< The server was not found. */
    EResultMaxConnectionsExceeded            =  87, /*!< The maximum number of connections has been reached. */
    EResultSocketIdOutOfRange                =  88, /*!< The socket ID is outside the valid range. */
    EResultSocketIdAlreadyUsed               =  89, /*!< The socket ID is already in use. */
    EResultConnectionRefused                 =  90, /*!< The connection establishment was rejected. */
    EResultConnectionError                   =  91, /*!< It was not possible to make a connection. */
    EResultNoConnection                      =  92, /*!< There was no connection. */
    EResultRemoteHostClosed                  =  93, /*!< The remote host has closed the connection. */
    EResultHostNotFound                      =  94, /*!< The remote host was not found. */
    EResultSocketAccessError                 =  95, /*!< An error occurred while accessing the socket. */
    EResultSocketResourceError               =  96, /*!< The socket has a resource problem. */
    EResultSocketTimeout                     =  97, /*!< The socket operation could not be completed within the expected period. */
    EResultSocketProtocolError               =  98, /*!< There is an error in the Socket Protocol. */
    EResultDatagramTooLarge                  =  99, /*!< The datagram is too big. */
    EResultNetworkError                      = 100, /*!< A network error has occurred. */
    EResultAddressAlreadyInUse               = 101, /*!< The address is already in use. */
    EResultSocketAddressNotAvailable         = 102, /*!< The socket address is not available. */
    EResultUnsupportedSocketOperation        = 103, /*!< The socket operation is not supported. */
    EResultUnfinishedSocketOperation         = 104, /*!< The socket operation was not completed as expected. */
    EResultProxyAuthenticationRequired       = 105, /*!< Authentication error with the proxy. */
    EResultSslHandshakeFailed                = 106, /*!< SSL handshake failed. */
    EResultProxyConnectionRefused            = 107, /*!< The proxy has rejected the connection request. */
    EResultProxyConnectionClosed             = 108, /*!< The proxy connection is closed. */
    EResultProxyTimeout                      = 109, /*!< The proxy operation could not be completed within the expected period. */
    EResultProxyNotFound                     = 110, /*!< The proxy was not found. */
    EResultProxyProtocol                     = 111, /*!< There is an error in the proxy protocol. */
    EResultReadyReadTimeout                  = 112, /*!< The reading of the data could not be completed within the expected period. */
    EResultTooMuchData                       = 113, /*!< There is too much data. */
    EResultStringParseError                  = 114, /*!< An error occurred while parsing the string. */
    EResultPermissionDenied                  = 115, /*!< Access was denied. */
    EResultSIUnitNotDefined                  = 116, /*!< SI unit is not defined. */
    EResultPhysSizeNotInitialized            = 117, /*!< The physical size has not been initialized. */
    EResultDifferentPhysSizes                = 118, /*!< The values ​​do not belong to the same physical size. */
    EResultInvalidPhysSize                   = 119, /*!< The physical size is not valid. */
    EResultInvalidPhysUnit                   = 120, /*!< The physical unit is not valid. */
    EResultInvalidRefVal                     = 121, /*!< The reference value is invalid. */
    EResultFctConvertInvalid                 = 122, /*!< There was an error converting the value. */
    EResultFctConvertNotDefined              = 123, /*!< The conversion function is not defined. */
    EResultConversionFailed                  = 124, /*!< The value could not be converted. */
    EResultMixedLinLogInValAndRes            = 125, /*!< Linear and logarithmic representation of value and resolution do not match. */
    EResultMixedLinLogInMathOp               = 126, /*!< Linear and logarithmic representation of value and resolution when working the method do not match. */
    EResultInvalidValue                      = 127, /*!< An invalid value was recognized. */
    EResultNotConvertible                    = 128, /*!< The value could not be converted. */
    EResultInvalidSIUnit                     = 129, /*!< The SI base unit is invalid. */
    EResultPhysSizeNotChangeable             = 130, /*!< The physical size cannot be changed. */
    EResultRefValMissing                     = 131, /*!< The reference value is missing. */
    EResultInvalidRefValUnit                 = 132, /*!< The reference value has an invalid unit. */
    EResultLogarithmicUnitExpected           = 133, /*!< A logarithmic unit was not expected. */
    EResultDimensionOutOfRange               = 134, /*!< The dimension is outside the valid range. */
    EResultValIdxOutOfRange                  = 135, /*!< The value has an invalid index. */
    EResultValueOutOfRange                   = 136, /*!< The value is outside the valid range. */
    EResultValueExceedsMinLimit              = 137, /*!< The value falls below the permissible minimum. */
    EResultValueExceedsMaxLimit              = 138, /*!< The value exceeds the permissible maximum. */
    EResultNoValueAssigned                   = 139, /*!< No value has been assigned. */
    EResultNullValueNotAllowed               = 140, /*!< A (zero) value is not allowed. */
    EResultValueCorrected                    = 141, /*!< The value has been corrected. */
    EResultInvalidDataType                   = 142, /*!< The data type is not valid. */
    EResultUnknownDataType                   = 143, /*!< The data type is not known. */
    EResultUnsupportedDataType               = 144, /*!< The data type is not supported. */
    EResultPrimaryKeyConstraintViolation     = 145, /*!< The primary key is not valid. */
    EResultUniqueConstraintViolation         = 146, /*!< The key is not clear. */
    EResultForeignKeyConstraintViolation     = 147, /*!< The key to accessing another resource is not valid. */
    EResultStatementError                    = 148, /*!< Errors within a statement. */
    EResultTransactionError                  = 149, /*!< Error within a transfer. */
    EResultCount                                    /*!< Total number of result codes. */
};

ZSSYSDLL_API QString result2Str( EResult i_result, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );
ZSSYSDLL_API EResult str2Result( const QString& i_strResult, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrUndefined );

bool ZSSYSDLL_API isErrorResult( EResult i_result );

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_ErrCode_h
