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

const SEnumEntry s_arEnumStrErrResultSeverity[] =
{
                                       // IdxName,  Symbol,   Text,     SCPIShort, SCPILong
    SEnumEntry( EResultSeveritySuccess,   "Success",   "S",   "Success",   "SUCC", "SUCCess"   ),
    SEnumEntry( EResultSeverityInfo,      "Info",      "I",   "Info",      "INFO", "INFO"      ),
    SEnumEntry( EResultSeverityWarning,   "Warning",   "W",   "Warning",   "WARN", "WARNing"   ),
    SEnumEntry( EResultSeverityError,     "Error",     "E",   "Error",     "ERR",  "ERRor"     ),
    SEnumEntry( EResultSeverityCritical,  "Critical",  "C",   "Critical",  "CRIT", "CRITical"  ),
    SEnumEntry( EResultSeverityCount,     "Count",     "All", "Count",     "COUN", "COUNt"     ),
    SEnumEntry( EResultSeverityUndefined, "Undefined", "?",   "Undefined", "UND",  "UNDefined" )
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
    /*  41 */ SEnumEntry( EResultObjFactoryNotDefined, "ObjFactoryNotDefined", "Object Factory Not Defined" ),
    /*  42 */ SEnumEntry( EResultObjFactoryNotFound, "ObjFactoryNotFound", "Object Factory Not Found" ),
    /*  43 */ SEnumEntry( EResultNumberOverflow, "NumberOverflow", "Number Overflow" ),
    /*  44 */ SEnumEntry( EResultNumberUnderflow, "NumberUnderflow", "Number Underflow" ),
    /*  45 */ SEnumEntry( EResultDivisionByZero, "DivisionByZero", "Division By Zero" ),
    /*  46 */ SEnumEntry( EResultInvalidDynamicTypeCast, "InvalidDynamicTypeCast", "Invalid Dynamic Type Cast" ),
    /*  47 */ SEnumEntry( EResultOptionNotSupported, "OptionNotSupported", "Option Yet Supported" ),
    /*  48 */ SEnumEntry( EResultOptionNotYetSupported, "OptionNotYetSupported", "Option Not Yet Supported" ),
    /*  49 */ SEnumEntry( EResultMethodNotYetImplemented, "MethodNotYetImplemented", "Method Not Yet Implemented" ),
    /*  50 */ SEnumEntry( EResultInvalidMethodCall, "InvalidMethodCall", "Invalid Method Called" ),
    /*  51 */ SEnumEntry( EResultInitModuleFailed, "InitModuleFailed", "Init Module Failed" ),
    /*  52 */ SEnumEntry( EResultFinalizeModuleFailed, "FinalizeModuleFailed", "Fainalize Module Failed" ),
    /*  53 */ SEnumEntry( EResultModuleNotInitialised, "ModuleNotInitialised", "Module Not Initialised" ),
    /*  54 */ SEnumEntry( EResultModuleAlreadyInitialised, "ModuleAlreadyInitialised", "Module Already Initialised" ),
    /*  55 */ SEnumEntry( EResultInitClassFailed, "InitClassFailed", "Init Class Failed" ),
    /*  56 */ SEnumEntry( EResultFinalizeClassFailed, "FinalizeClassFailed", "Finalize Class Failed" ),
    /*  57 */ SEnumEntry( EResultClassNotInitialised, "ClassNotInitialised", "Class Not Initialised" ),
    /*  58 */ SEnumEntry( EResultClassAlreadyInitialised, "ClassAlreadyInitialised", "Class Already Initialised" ),
    /*  59 */ SEnumEntry( EResultSingletonClassNotInstantiated, "SingletonClassNotInstantiated", "Singleton Class Not Instantiated" ),
    /*  60 */ SEnumEntry( EResultSingletonClassAlreadyInstantiated, "SingletonClassAlreadyInstantiated", "Singleton Class Already Instantiated" ),
    /*  61 */ SEnumEntry( EResultUnexpectedMessage, "UnexpectedMessage", "Unexpected message" ),
    /*  62 */ SEnumEntry( EResultMessageTypeMismatch, "MessageTypeMismatch", "Message type identifier does not match message type class" ),
    /*  63 */ SEnumEntry( EResultMessageStateMismatch, "MessageStateMismatch", "Did not expect this message in current state" ),
    /*  64 */ SEnumEntry( EResultInvalidMessage, "InvalidMessage", "Invalid Message" ),
    /*  65 */ SEnumEntry( EResultInvalidMessageContent, "InvalidMessageContent", "Invalid content in message" ),
    /*  66 */ SEnumEntry( EResultInvalidSenderObjInMessage, "InvalidSenderObjInMessage", "Invalid sender object in message" ),
    /*  67 */ SEnumEntry( EResultInvalidReceiverObjInMessage, "InvalidReceiverObjInMessage", "Invalid receiver object in message" ),
    /*  68 */ SEnumEntry( EResultMethodStateMismatch, "MethodStateMismatch", "Method call not allowed in current state" ),
    /*  69 */ SEnumEntry( EResultRequestRefused, "RequestRefused", "Request Refused" ),
    /*  70 */ SEnumEntry( EResultRequestAborted, "RequestAborted", "Request Aborted" ),
    /*  71 */ SEnumEntry( EResultRequestExecutionError, "RequestExecutionError", "Request Execution Error" ),
    /*  72 */ SEnumEntry( EResultUnfinishedOperation, "UnfinishedOperation", "Unfinished Operation" ),
    /*  73 */ SEnumEntry( EResultInvalidFileName, "InvalidFileName", "Invalid File Name" ),
    /*  74 */ SEnumEntry( EResultFileNotFound, "FileNotFound", "File does not exist" ),
    /*  75 */ SEnumEntry( EResultFileOpenError, "FileOpenError", "Could not open file" ),
    /*  76 */ SEnumEntry( EResultFileOpenForWrite, "FileOpenForWrite", "Could not open file for writing" ),
    /*  77 */ SEnumEntry( EResultFileOpenForRead, "FileOpenForRead", "Could not open file for reading" ),
    /*  78 */ SEnumEntry( EResultFileNotOpen, "FileNotOpen", "File is not open" ),
    /*  79 */ SEnumEntry( EResultFileReadContent, "FileReadContent", "Error on reading content of file" ),
    /*  80 */ SEnumEntry( EResultFileWriteContent, "FileWriteContent", "Error on writing content of file" ),
    /*  81 */ SEnumEntry( EResultSessionOpenError, "SessionOpenError", "Could not open session" ),
    /*  82 */ SEnumEntry( EResultSessionCloseError, "SessionCloseError", "Could not close session" ),
    /*  83 */ SEnumEntry( EResultSessionNotOpen, "SessionNotOpen", "Session is not open" ),
    /*  84 */ SEnumEntry( EResultServerNotFound, "ServerNotFound", "Server Not Found" ),
    /*  85 */ SEnumEntry( EResultMaxConnectionsExceeded, "MaxConnectionsExceeded", "Exceeded Maximum Number Of Connections" ),
    /*  86 */ SEnumEntry( EResultSocketIdOutOfRange, "SocketIdOutOfRange", "Socket Id Out Of Range" ),
    /*  87 */ SEnumEntry( EResultSocketIdAlreadyUsed, "SocketIdAlreadyUsed", "Socket Id Already Used" ),
    /*  88 */ SEnumEntry( EResultConnectionRefused, "ConnectionRefused", "The Remote Host Refused The Connection" ),
    /*  89 */ SEnumEntry( EResultConnectionError, "ConnectionError", "Unknown connection error" ),
    /*  90 */ SEnumEntry( EResultNoConnection, "NoConnection", "No Connection" ),
    /*  91 */ SEnumEntry( EResultRemoteHostClosed, "RemoteHostClosed", "The Remote Host Closed The Connection" ),
    /*  92 */ SEnumEntry( EResultHostNotFound, "HostNotFound", "The Remote Host Has Not Been Found" ),
    /*  93 */ SEnumEntry( EResultSocketAccessError, "SocketAccessError", "Socket Access Error" ),
    /*  94 */ SEnumEntry( EResultSocketResourceError, "SocketResourceError", "Socket Resource Error" ),
    /*  95 */ SEnumEntry( EResultSocketTimeout, "SocketTimeoutError", "Socket Operation Timed Out" ),
    /*  96 */ SEnumEntry( EResultSocketProtocolError, "SocketProtocolError", "Socket Protocol Error" ),
    /*  97 */ SEnumEntry( EResultDatagramTooLarge, "DatagramTooLarge", "Datagram Too Large" ),
    /*  98 */ SEnumEntry( EResultNetworkError, "NetworkError", "Network Error" ),
    /*  99 */ SEnumEntry( EResultAddressAlreadyInUse, "AddressInUse", "Address Already In Use" ),
    /* 100 */ SEnumEntry( EResultSocketAddressNotAvailable, "SocketAddressNotAvailable", "Socket Address Is Not Available" ),
    /* 101 */ SEnumEntry( EResultUnsupportedSocketOperation, "UnsupportedSocketOperation", "Socket Operation Not Supported" ),
    /* 102 */ SEnumEntry( EResultUnfinishedSocketOperation, "UnfinishedSocketOperation", "Unfinished Socket Operation" ),
    /* 103 */ SEnumEntry( EResultProxyAuthenticationRequired, "ProxyAuthenticationRequired", "Proxy Authentication Required" ),
    /* 104 */ SEnumEntry( EResultSslHandshakeFailed, "SslHandshakeFailed", "SSL Handshake Failed" ),
    /* 105 */ SEnumEntry( EResultProxyConnectionRefused, "ProxyConnectionRefused", "The Proxy Refused The Connection" ),
    /* 106 */ SEnumEntry( EResultProxyConnectionClosed, "ProxyConnectionClosed", "The Proxy Closed The Connection" ),
    /* 107 */ SEnumEntry( EResultProxyTimeout, "ProxyTimeout", "Proxy Timeout" ),
    /* 108 */ SEnumEntry( EResultProxyNotFound, "ProxyNotFound", "Proxy Not Found" ),
    /* 109 */ SEnumEntry( EResultProxyProtocol, "ProxyProtocolError", "Proxy Protocol Error" ),
    /* 110 */ SEnumEntry( EResultReadyReadTimeout, "ReadyReadTimeout", "Timeout On Reading Data" ),
    /* 111 */ SEnumEntry( EResultTooMuchData, "TooMuchData", "Too Much Data" ),
    /* 112 */ SEnumEntry( EResultStringParseError, "StringParseError", "String Parse Error" ),
    /* 113 */ SEnumEntry( EResultPermissionDenied, "PermisionDenied", "Permission Denied" ),
    /* 114 */ SEnumEntry( EResultSIUnitNotDefined, "SIUnitNotDefined", "SI Unit Not Defined" ),
    /* 115 */ SEnumEntry( EResultPhysSizeNotInitialized, "PhysSizeNotInitialized", "Physical Size Not Initialized" ),
    /* 116 */ SEnumEntry( EResultDifferentPhysSizes, "DifferentPhysSizes", "Different Physical Sizes" ),
    /* 117 */ SEnumEntry( EResultInvalidPhysSize, "InvalidPhysSize", "Invalid Physical Size Or Physical Size Not Defined" ),
    /* 118 */ SEnumEntry( EResultInvalidPhysUnit, "InvalidPhysUnit", "Invalid Physical Unit Or Physical Unit Not Defined" ),
    /* 119 */ SEnumEntry( EResultInvalidRefVal, "InvalidRefVal", "Invalid Reference Value" ),
    /* 120 */ SEnumEntry( EResultFctConvertInvalid, "FctConvertInvalid", "Invalid Unit Conversion Routine" ),
    /* 121 */ SEnumEntry( EResultFctConvertNotDefined, "FctConvertNotDefined", "Unit Conversion Routine Not Defined" ),
    /* 122 */ SEnumEntry( EResultConversionFailed, "ConversionFailed", "Unit Conversion Failed" ),
    /* 123 */ SEnumEntry( EResultMixedLinLogInValAndRes, "MixedLinLogInValAndRes", "Mixed Linear And Logarithmic Unit For Value And Resolution" ),
    /* 124 */ SEnumEntry( EResultMixedLinLogInMathOp, "MixedLinLogInMathOp", "Linear And Logarithmic Value For This Mathematical Operation Not Supported" ),
    /* 125 */ SEnumEntry( EResultInvalidValue, "InvalidValue", "Invalid Value" ),
    /* 126 */ SEnumEntry( EResultNotConvertible, "NotConvertible", "Unit Not Convertible Into Destination Unit" ),
    /* 127 */ SEnumEntry( EResultInvalidSIUnit, "InvalidSIUnit", "Invalid SI Unit" ),
    /* 128 */ SEnumEntry( EResultPhysSizeNotChangeable, "PhysicalSizeNotChangeable", "Physical Size Of Physical Values Are Not Changeable" ),
    /* 129 */ SEnumEntry( EResultRefValMissing, "RefValMissing", "Reference Value(s) Missing" ),
    /* 130 */ SEnumEntry( EResultInvalidRefValUnit, "InvalidRefValUnit", "Invalid Unit Of Reference Value" ),
    /* 131 */ SEnumEntry( EResultLogarithmicUnitExpected, "LogarithmicUnitExpected", "Expected A Logarithmic Unit" ),
    /* 132 */ SEnumEntry( EResultDimensionOutOfRange, "DimensionOutOfRange", "Dimension Out Of Range" ),
    /* 133 */ SEnumEntry( EResultValIdxOutOfRange, "ValIdxOutOfRange", "Value Index Out Of Range" ),
    /* 134 */ SEnumEntry( EResultValueOutOfRange, "ValueOutOfRange", "Value Out Of Range" ),
    /* 135 */ SEnumEntry( EResultValueExceedsMinLimit, "ValueExceedsMinLimit", "Value Is Below Minimal Limit" ),
    /* 136 */ SEnumEntry( EResultValueExceedsMaxLimit, "ValueExceedsMaxLimit", "Value Is Above Maximum Limite" ),
    /* 137 */ SEnumEntry( EResultNoValueAssigned, "NoValueAssigned", "No Value Assigned" ),
    /* 138 */ SEnumEntry( EResultNullValueNotAllowed, "NullValueNotAllowed", "Null Value Not Allowed" ),
    /* 139 */ SEnumEntry( EResultValueCorrected,  "ValueCorrected", "Value Corrected" ),
    /* 140 */ SEnumEntry( EResultInvalidDataType, "InvalidDataType", "Invalid Or Unknown Data Type" ),
    /* 141 */ SEnumEntry( EResultUnknownDataType, "UnknownDataType", "Unknown Data Type" ),
    /* 142 */ SEnumEntry( EResultUnsupportedDataType, "UnsupportedDataType", "Data Type Not Supported" ),
    /* 143 */ SEnumEntry( EResultPrimaryKeyConstraintViolation, "PrimaryKeyConstraintViolation", "Primary Key Constraint Violation" ),
    /* 144 */ SEnumEntry( EResultUniqueConstraintViolation, "UniqueConstraintViolation", "Unique Constraint Violation" ),
    /* 145 */ SEnumEntry( EResultForeignKeyConstraintViolation, "ForeignKeyConstraintViolation", "Foreign Key Constraint Violation" ),
    /* 146 */ SEnumEntry( EResultStatementError, "StatementError", "Statement Error" ),
    /* 147 */ SEnumEntry( EResultTransactionError, "TransactionError", "TransactionError" )
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

