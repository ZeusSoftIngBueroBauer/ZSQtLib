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

#include "ZSSys/ZSSysErrCode.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum ResultSeverity
==============================================================================*/

const SEnumEntry s_arEnumStrErrResultSeverity[EResultSeverityCount] =
{
                                        // IdxName, Symbol,  Text, SCPIShort, SCPILong
    SEnumEntry( EResultSeveritySuccess,  "Success",  "S", "Success",  "SUCC", "SUCCess"  ),
    SEnumEntry( EResultSeverityInfo,     "Info",     "I", "Info",     "INFO", "INFO"     ),
    SEnumEntry( EResultSeverityWarning,  "Warning",  "W", "Warning",  "WARN", "WARNing"  ),
    SEnumEntry( EResultSeverityError,    "Error",    "E", "Error",    "ERR",  "ERRor"    ),
    SEnumEntry( EResultSeverityCritical, "Critical", "C", "Critical", "CRIT", "CRITical" )
};

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @param i_severity [in] Enumerator to be converted.
    @param i_alias [in] Range [IdxName, Symbol, Text, SCPIShort, SCPILong].
                        Selects one of the available strings.

    @return String representation of the given enumerator.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString ZS::System::resultSeverity2Str( EResultSeverity i_severity, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrErrResultSeverity, _ZSArrLen(s_arEnumStrErrResultSeverity), i_severity, i_alias);
}

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @param i_str [in] String to be converted into the enumerator.
    @param i_alias [in] Range [IdxName, Symbol, Text, SCPIShort, SCPILong, Undefined].
                        Selects one of the available strings.
                        If Undefined is used the string will be compared with all possible strings.
                        If a valid index is used the passed string will only be compared with this alias string.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.
*/
EResultSeverity ZS::System::str2ResultSeverity( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EResultSeverity severity = EResultSeverityUndefined;
    bool bOk = false;
    int iSeverity = SEnumEntry::str2Enumerator(s_arEnumStrErrResultSeverity, EResultSeverityCount, i_str, i_alias, Qt::CaseInsensitive, &bOk);
    if( bOk && iSeverity >= 0 && iSeverity < EResultSeverityCount )
    {
        severity = static_cast<EResultSeverity>(iSeverity);
    }
    return severity;
}

/*==============================================================================
enum Result
==============================================================================*/

const SEnumEntry s_arEnumStrErrResult[EResultCount] =
{
                                        // IdxName, Symbol
    /*   0 */ SEnumEntry( EResultSuccess, "Success", "Success" ),
    /*   1 */ SEnumEntry( EResultSuccessIntermediateResults, "IntermediateResults", "Intermediate results" ),
    /*   2 */ SEnumEntry( EResultInvalid, "Invalid", "Invalid" ),
    /*   3 */ SEnumEntry( EResultUndefined, "Undefined", "Undefined" ),
    /*   4 */ SEnumEntry( EResultError, "Error", "Error" ),
    /*   5 */ SEnumEntry( EResultUnknownError, "UnknownError", "Unknown Error" ),
    /*   6 */ SEnumEntry( EResultUnknownException, "UnknownException", "Unnknown exception has been thrown" ),
    /*   7 */ SEnumEntry( EResultUnexpected, "UnexpectedResult", "Unexpected result" ),
    /*   8 */ SEnumEntry( EResultInternalProgramError, "InternalProgramError", "Internal program error" ),
    /*   9 */ SEnumEntry( EResultInternalStateMachineError, "InternalStateMachineError", "Internal state machine error" ),
    /*  10 */ SEnumEntry( EResultSignalSlotConnectionFailed, "SignalSlotConnectionFailed", "Connecting signal with slot failed" ),
    /*  11 */ SEnumEntry( EResultMemLeak, "MemLeak", "Memory leak" ),
    /*  12 */ SEnumEntry( EResultOutOfMemory, "OutOfMemory", "Out of memory" ),
    /*  13 */ SEnumEntry( EResultArgOutOfRange, "ArgOutOfRange", "Argument out of range" ),
    /*  14 */ SEnumEntry( EResultIdxOutOfRange, "IdxOutOfRange", "Index out of range" ),
    /*  15 */ SEnumEntry( EResultTimeout, "Timeout", "Timeout" ),
    /*  16 */ SEnumEntry( EResultListIsEmpty, "ListIsEmpty", "List is empty" ),
    /*  17 */ SEnumEntry( EResultListNotEmpty, "ListNotEmpty", "List is not empty" ),
    /*  18 */ SEnumEntry( EResultListIsFull, "ListIsFull", "List is full" ),
    /*  19 */ SEnumEntry( EResultListCorrupted, "ListCorrupted", "List ss corrupted" ),
    /*  20 */ SEnumEntry( EResultEndOfList, "EndOfList", "End of list" ),
    /*  21 */ SEnumEntry( EResultObjNotInList, "ObjNotInList", "Object not part of the list" ),
    /*  22 */ SEnumEntry( EResultObjAlreadyInList, "ObjAlreadyInList", "Object is already part of the list" ),
    /*  23 */ SEnumEntry( EResultObjRefCounterIsZero, "ObjRefCounterIsZero", "The objects reference counter is 0" ),
    /*  24 */ SEnumEntry( EResultObjRefCounterIsNotZero, "ObjRefCounterIsNotZero", "The objects reference counter is not 0" ),
    /*  25 */ SEnumEntry( EResultInvalidObjId, "InvalidObjId", "Invalid Object Id" ),
    /*  26 */ SEnumEntry( EResultInvalidObjName, "InvalidObjName", "Invalid Object Name" ),
    /*  27 */ SEnumEntry( EResultInvalidObjGUID, "InvalidObjGUID", "Invalid Object GUID" ),
    /*  28 */ SEnumEntry( EResultInvalidObjType, "InvalidObjType", "Invalid Object Type" ),
    /*  29 */ SEnumEntry( EResultInvalidObjAddress, "InvalidObjAddress", "Invalid Object Address" ),
    /*  30 */ SEnumEntry( EResultObjNameNotUnique, "ObjectNameNotUnique", "Object Name Not Unique" ),
    /*  31 */ SEnumEntry( EResultObjCreateFailed, "ObjectCreateFailed", "Creating Object Failed" ),
    /*  32 */ SEnumEntry( EResultObjDestroyFailed, "ObjectDestroyFailed", "Destroying Object Failed" ),
    /*  33 */ SEnumEntry( EResultObjActivateFailed, "ObjectActivateFailed", "Activating Object Failed" ),
    /*  34 */ SEnumEntry( EResultObjDeactivateFailed, "ObjectDeactivateFailed", "Deactivating Object Failed" ),
    /*  35 */ SEnumEntry( EResultObjRegisterFailed, "ObjectRegisterFailed", "Register Object Failed" ),
    /*  36 */ SEnumEntry( EResultObjUnregisterFailed, "ObjectUnregisterFailed", "Unregister Object Failed" ),
    /*  37 */ SEnumEntry( EResultObjNotFound, "ObjectNotFound", "Object Not Found" ),
    /*  38 */ SEnumEntry( EResultObjNotDefined, "ObjectNotDefined", "Object Not Defined" ),
    /*  39 */ SEnumEntry( EResultObjNotRegistered, "ObjectNotRegistered", "Object Not Registered" ),
    /*  40 */ SEnumEntry( EResultObjAlreadyRegistered, "ObjectAlreadyRegistered", "Object Already Registered" ),
    /*  41 */ SEnumEntry( EResultObjPoolNotDefined, "ObjPoolNotDefined", "Object Pool Not Defined" ),
    /*  42 */ SEnumEntry( EResultObjPoolNotFound, "ObjPoolNotFound", "Object Pool Not Found" ),
    /*  43 */ SEnumEntry( EResultObjFactoryNotDefined, "ObjFactoryNotDefined", "Object Factory Not Defined" ),
    /*  44 */ SEnumEntry( EResultObjFactoryNotFound, "ObjFactoryNotFound", "Object Factory Not Found" ),
    /*  45 */ SEnumEntry( EResultNumberOverflow, "NumberOverflow", "Number Overflow" ),
    /*  46 */ SEnumEntry( EResultNumberUnderflow, "NumberUnderflow", "Number Underflow" ),
    /*  47 */ SEnumEntry( EResultDivisionByZero, "DivisionByZero", "Division By Zero" ),
    /*  48 */ SEnumEntry( EResultInvalidDynamicTypeCast, "InvalidDynamicTypeCast", "Invalid Dynamic Type Cast" ),
    /*  49 */ SEnumEntry( EResultOptionNotSupported, "OptionNotSupported", "Option Yet Supported" ),
    /*  50 */ SEnumEntry( EResultOptionNotYetSupported, "OptionNotYetSupported", "Option Not Yet Supported" ),
    /*  51 */ SEnumEntry( EResultMethodNotYetImplemented, "MethodNotYetImplemented", "Method Not Yet Implemented" ),
    /*  52 */ SEnumEntry( EResultInvalidMethodCall, "InvalidMethodCall", "Invalid Method Called" ),
    /*  53 */ SEnumEntry( EResultInitModuleFailed, "InitModuleFailed", "Init Module Failed" ),
    /*  54 */ SEnumEntry( EResultFinalizeModuleFailed, "FinalizeModuleFailed", "Fainalize Module Failed" ),
    /*  55 */ SEnumEntry( EResultModuleNotInitialised, "ModuleNotInitialised", "Module Not Initialised" ),
    /*  56 */ SEnumEntry( EResultModuleAlreadyInitialised, "ModuleAlreadyInitialised", "Module Already Initialised" ),
    /*  57 */ SEnumEntry( EResultInitClassFailed, "InitClassFailed", "Init Class Failed" ),
    /*  58 */ SEnumEntry( EResultFinalizeClassFailed, "FinalizeClassFailed", "Finalize Class Failed" ),
    /*  59 */ SEnumEntry( EResultClassNotInitialised, "ClassNotInitialised", "Class Not Initialised" ),
    /*  60 */ SEnumEntry( EResultClassAlreadyInitialised, "ClassAlreadyInitialised", "Class Already Initialised" ),
    /*  61 */ SEnumEntry( EResultSingletonClassNotInstantiated, "SingletonClassNotInstantiated", "Singleton Class Not Instantiated" ),
    /*  62 */ SEnumEntry( EResultSingletonClassAlreadyInstantiated, "SingletonClassAlreadyInstantiated", "Singleton Class Already Instantiated" ),
    /*  63 */ SEnumEntry( EResultUnexpectedMessage, "UnexpectedMessage", "Unexpected message" ),
    /*  64 */ SEnumEntry( EResultMessageTypeMismatch, "MessageTypeMismatch", "Message type identifier does not match message type class" ),
    /*  65 */ SEnumEntry( EResultMessageStateMismatch, "MessageStateMismatch", "Did not expect this message in current state" ),
    /*  66 */ SEnumEntry( EResultInvalidMessage, "InvalidMessage", "Invalid Message" ),
    /*  67 */ SEnumEntry( EResultInvalidMessageContent, "InvalidMessageContent", "Invalid content in message" ),
    /*  68 */ SEnumEntry( EResultInvalidSenderObjInMessage, "InvalidSenderObjInMessage", "Invalid sender object in message" ),
    /*  69 */ SEnumEntry( EResultInvalidReceiverObjInMessage, "InvalidReceiverObjInMessage", "Invalid receiver object in message" ),
    /*  70 */ SEnumEntry( EResultMethodStateMismatch, "MethodStateMismatch", "Method call not allowed in current state" ),
    /*  71 */ SEnumEntry( EResultRequestRefused, "RequestRefused", "Request Refused" ),
    /*  72 */ SEnumEntry( EResultRequestAborted, "RequestAborted", "Request Aborted" ),
    /*  73 */ SEnumEntry( EResultRequestExecutionError, "RequestExecutionError", "Request Execution Error" ),
    /*  74 */ SEnumEntry( EResultUnfinishedOperation, "UnfinishedOperation", "Unfinished Operation" ),
    /*  75 */ SEnumEntry( EResultInvalidFileName, "InvalidFileName", "Invalid File Name" ),
    /*  76 */ SEnumEntry( EResultFileNotFound, "FileNotFound", "File does not exist" ),
    /*  77 */ SEnumEntry( EResultFileOpenError, "FileOpenError", "Could not open file" ),
    /*  78 */ SEnumEntry( EResultFileOpenForWrite, "FileOpenForWrite", "Could not open file for writing" ),
    /*  79 */ SEnumEntry( EResultFileOpenForRead, "FileOpenForRead", "Could not open file for reading" ),
    /*  80 */ SEnumEntry( EResultFileNotOpen, "FileNotOpen", "File is not open" ),
    /*  81 */ SEnumEntry( EResultFileReadContent, "FileReadContent", "Error on reading content of file" ),
    /*  82 */ SEnumEntry( EResultFileWriteContent, "FileWriteContent", "Error on writing content of file" ),
    /*  83 */ SEnumEntry( EResultSessionOpenError, "SessionOpenError", "Could not open session" ),
    /*  84 */ SEnumEntry( EResultSessionCloseError, "SessionCloseError", "Could not close session" ),
    /*  85 */ SEnumEntry( EResultSessionNotOpen, "SessionNotOpen", "Session is not open" ),
    /*  86 */ SEnumEntry( EResultServerNotFound, "ServerNotFound", "Server Not Found" ),
    /*  87 */ SEnumEntry( EResultMaxConnectionsExceeded, "MaxConnectionsExceeded", "Exceeded Maximum Number Of Connections" ),
    /*  88 */ SEnumEntry( EResultSocketIdOutOfRange, "SocketIdOutOfRange", "Socket Id Out Of Range" ),
    /*  89 */ SEnumEntry( EResultSocketIdAlreadyUsed, "SocketIdAlreadyUsed", "Socket Id Already Used" ),
    /*  90 */ SEnumEntry( EResultConnectionRefused, "ConnectionRefused", "The Remote Host Refused The Connection" ),
    /*  91 */ SEnumEntry( EResultConnectionError, "ConnectionError", "Unknown connection error" ),
    /*  92 */ SEnumEntry( EResultNoConnection, "NoConnection", "No Connection" ),
    /*  93 */ SEnumEntry( EResultRemoteHostClosed, "RemoteHostClosed", "The Remote Host Closed The Connection" ),
    /*  94 */ SEnumEntry( EResultHostNotFound, "HostNotFound", "The Remote Host Has Not Been Found" ),
    /*  95 */ SEnumEntry( EResultSocketAccessError, "SocketAccessError", "Socket Access Error" ),
    /*  96 */ SEnumEntry( EResultSocketResourceError, "SocketResourceError", "Socket Resource Error" ),
    /*  97 */ SEnumEntry( EResultSocketTimeout, "SocketTimeoutError", "Socket Operation Timed Out" ),
    /*  98 */ SEnumEntry( EResultSocketProtocolError, "SocketProtocolError", "Socket Protocol Error" ),
    /*  99 */ SEnumEntry( EResultDatagramTooLarge, "DatagramTooLarge", "Datagram Too Large" ),
    /* 100 */ SEnumEntry( EResultNetworkError, "NetworkError", "Network Error" ),
    /* 101 */ SEnumEntry( EResultAddressAlreadyInUse, "AddressInUse", "Address Already In Use" ),
    /* 102 */ SEnumEntry( EResultSocketAddressNotAvailable, "SocketAddressNotAvailable", "Socket Address Is Not Available" ),
    /* 103 */ SEnumEntry( EResultUnsupportedSocketOperation, "UnsupportedSocketOperation", "Socket Operation Not Supported" ),
    /* 104 */ SEnumEntry( EResultUnfinishedSocketOperation, "UnfinishedSocketOperation", "Unfinished Socket Operation" ),
    /* 105 */ SEnumEntry( EResultProxyAuthenticationRequired, "ProxyAuthenticationRequired", "Proxy Authentication Required" ),
    /* 106 */ SEnumEntry( EResultSslHandshakeFailed, "SslHandshakeFailed", "SSL Handshake Failed" ),
    /* 107 */ SEnumEntry( EResultProxyConnectionRefused, "ProxyConnectionRefused", "The Proxy Refused The Connection" ),
    /* 108 */ SEnumEntry( EResultProxyConnectionClosed, "ProxyConnectionClosed", "The Proxy Closed The Connection" ),
    /* 109 */ SEnumEntry( EResultProxyTimeout, "ProxyTimeout", "Proxy Timeout" ),
    /* 110 */ SEnumEntry( EResultProxyNotFound, "ProxyNotFound", "Proxy Not Found" ),
    /* 111 */ SEnumEntry( EResultProxyProtocol, "ProxyProtocolError", "Proxy Protocol Error" ),
    /* 112 */ SEnumEntry( EResultReadyReadTimeout, "ReadyReadTimeout", "Timeout On Reading Data" ),
    /* 113 */ SEnumEntry( EResultTooMuchData, "TooMuchData", "Too Much Data" ),
    /* 114 */ SEnumEntry( EResultStringParseError, "StringParseError", "String Parse Error" ),
    /* 115 */ SEnumEntry( EResultPermissionDenied, "PermisionDenied", "Permission Denied" ),
    /* 116 */ SEnumEntry( EResultSIUnitNotDefined, "SIUnitNotDefined", "SI Unit Not Defined" ),
    /* 117 */ SEnumEntry( EResultPhysSizeNotInitialized, "PhysSizeNotInitialized", "Physical Size Not Initialized" ),
    /* 118 */ SEnumEntry( EResultDifferentPhysSizes, "DifferentPhysSizes", "Different Physical Sizes" ),
    /* 119 */ SEnumEntry( EResultInvalidPhysSize, "InvalidPhysSize", "Invalid Physical Size Or Physical Size Not Defined" ),
    /* 120 */ SEnumEntry( EResultInvalidPhysUnit, "InvalidPhysUnit", "Invalid Physical Unit Or Physical Unit Not Defined" ),
    /* 121 */ SEnumEntry( EResultInvalidRefVal, "InvalidRefVal", "Invalid Reference Value" ),
    /* 122 */ SEnumEntry( EResultFctConvertInvalid, "FctConvertInvalid", "Invalid Unit Conversion Routine" ),
    /* 123 */ SEnumEntry( EResultFctConvertNotDefined, "FctConvertNotDefined", "Unit Conversion Routine Not Defined" ),
    /* 124 */ SEnumEntry( EResultConversionFailed, "ConversionFailed", "Unit Conversion Failed" ),
    /* 125 */ SEnumEntry( EResultMixedLinLogInValAndRes, "MixedLinLogInValAndRes", "Mixed Linear And Logarithmic Unit For Value And Resolution" ),
    /* 126 */ SEnumEntry( EResultMixedLinLogInMathOp, "MixedLinLogInMathOp", "Linear And Logarithmic Value For This Mathematical Operation Not Supported" ),
    /* 127 */ SEnumEntry( EResultInvalidValue, "InvalidValue", "Invalid Value" ),
    /* 128 */ SEnumEntry( EResultNotConvertible, "NotConvertible", "Unit Not Convertible Into Destination Unit" ),
    /* 129 */ SEnumEntry( EResultInvalidSIUnit, "InvalidSIUnit", "Invalid SI Unit" ),
    /* 130 */ SEnumEntry( EResultPhysSizeNotChangeable, "PhysicalSizeNotChangeable", "Physical Size Of Physical Values Are Not Changeable" ),
    /* 131 */ SEnumEntry( EResultRefValMissing, "RefValMissing", "Reference Value(s) Missing" ),
    /* 132 */ SEnumEntry( EResultInvalidRefValUnit, "InvalidRefValUnit", "Invalid Unit Of Reference Value" ),
    /* 133 */ SEnumEntry( EResultLogarithmicUnitExpected, "LogarithmicUnitExpected", "Expected A Logarithmic Unit" ),
    /* 134 */ SEnumEntry( EResultDimensionOutOfRange, "DimensionOutOfRange", "Dimension Out Of Range" ),
    /* 135 */ SEnumEntry( EResultValIdxOutOfRange, "ValIdxOutOfRange", "Value Index Out Of Range" ),
    /* 136 */ SEnumEntry( EResultValueOutOfRange, "ValueOutOfRange", "Value Out Of Range" ),
    /* 137 */ SEnumEntry( EResultValueExceedsMinLimit, "ValueExceedsMinLimit", "Value Is Below Minimal Limit" ),
    /* 138 */ SEnumEntry( EResultValueExceedsMaxLimit, "ValueExceedsMaxLimit", "Value Is Above Maximum Limite" ),
    /* 139 */ SEnumEntry( EResultNoValueAssigned, "NoValueAssigned", "No Value Assigned" ),
    /* 140 */ SEnumEntry( EResultNullValueNotAllowed, "NullValueNotAllowed", "Null Value Not Allowed" ),
    /* 141 */ SEnumEntry( EResultValueCorrected,  "ValueCorrected", "Value Corrected" ),
    /* 142 */ SEnumEntry( EResultInvalidDataType, "InvalidDataType", "Invalid Or Unknown Data Type" ),
    /* 143 */ SEnumEntry( EResultUnknownDataType, "UnknownDataType", "Unknown Data Type" ),
    /* 144 */ SEnumEntry( EResultUnsupportedDataType, "UnsupportedDataType", "Data Type Not Supported" ),
    /* 145 */ SEnumEntry( EResultPrimaryKeyConstraintViolation, "PrimaryKeyConstraintViolation", "Primary Key Constraint Violation" ),
    /* 146 */ SEnumEntry( EResultUniqueConstraintViolation, "UniqueConstraintViolation", "Unique Constraint Violation" ),
    /* 147 */ SEnumEntry( EResultForeignKeyConstraintViolation, "ForeignKeyConstraintViolation", "Foreign Key Constraint Violation" ),
    /* 148 */ SEnumEntry( EResultStatementError, "StatementError", "Statement Error" ),
    /* 149 */ SEnumEntry( EResultTransactionError, "TransactionError", "TransactionError" )
};

//------------------------------------------------------------------------------
/*! Converts the given enumerator into a string.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @param i_result [in] Enumerator to be converted.
    @param i_alias [in] Range [IdxName, Symbol].
                        Selects one of the available strings.

    @return String representation of the given enumerator.

    @note Throws an Exception
          - with Result = IdxOutOutOfRange if the enumerator is out of range or
          - with Result = ArgOutOfRange if the alias is out of range.
*/
QString ZS::System::result2Str( EResult i_result, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    QString str;
    // To avoid endless recursion as "index2Name" may fail with an error result
    // whereupon again "result2Str" will be called which again may fail whereupon ..
    static bool s_bResult2StrInProgress = false;
    if( !s_bResult2StrInProgress )
    {
        s_bResult2StrInProgress = true;

        // enumerator2Str throws an exception if i_iResult does not belong to the enumeration.
        str = SEnumEntry::enumerator2Str(s_arEnumStrErrResult, _ZSArrLen(s_arEnumStrErrResult), i_result, i_alias);
        s_bResult2StrInProgress = false;
    }

    return str;

} // result2Str

//------------------------------------------------------------------------------
/*! Converts the given string into the corresponding enumerator.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @param i_str [in] String to be converted into the enumerator.
    @param i_alias [in] Range [IdxName, Symbol]
                        Selects one of the available strings.
                        If Undefined is used the string will be compared with all possible strings.
                        If a valid index is used the passed string will only be compared with this alias string.

    @return Enumerator corresponding to the given string. Undefined if the conversion failed.
*/
EResult ZS::System::str2Result( const QString& i_str, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EResult result = EResultSuccess;
    int iResult = SEnumEntry::str2Enumerator(s_arEnumStrErrResult, EResultCount, i_str, i_alias, Qt::CaseInsensitive);
    if( iResult >= 0 && iResult < EResultCount )
    {
        result = static_cast<EResult>(iResult);
    }
    return result;

} // str2Result

//------------------------------------------------------------------------------
/*! Checks whether the result code represents an error condition.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_ErrorHandling

    @param i_result [in] Result code to be checked.

    @return true, if the result indicates no error, false otherwise.
*/
bool ZS::System::isErrorResult( EResult i_result )
//------------------------------------------------------------------------------
{
    bool bIsError = true;

    if( i_result >= EResultNoErrorMIN && i_result <= EResultNoErrorMAX )
    {
        bIsError = false;
    }
    return bIsError;

} // isErrorResult

