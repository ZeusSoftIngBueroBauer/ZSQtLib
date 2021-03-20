/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#ifndef ZSRemCmd_ErrCode_h
#define ZSRemCmd_ErrCode_h

#include "ZSRemCmd/ZSRemCmdDllMain.h"
#include "ZSSys/ZSSysErrResult.h"

namespace ZS
{
namespace RemCmd
{
//------------------------------------------------------------------------------
enum EErrNr {
//------------------------------------------------------------------------------
    /*
    The <Error/event_number> is a unique integer in the range [-32768, 32767].
    All positive numbers are instrument-dependent. All negative numbers are
    reserved by the SCPI standard with certain standard error/event codes
    described in document "scpi-99.pdf". The value, zero, is also reserved to
    indicate that no error or event has occurred.

    The system-defined error/event numbers are chosen on an enumerated (“1 of N”)
    basis. The SCPI-defined error/event numbers and the <error/event_description>
    portions of the full queue item are listed here. The first error/event described
    in each class (for example, -100, -200, -300, -400) is a “generic” error. In
    selecting the proper Error/event number to report, more specific error/event
    codes are preferred, and the generic error/event is used only if the others
    are inappropriate.
    */

    EErrNrUndefined        =   -1,
    EErrNrNoError          =    0,

    // Command Error
    //--------------

    EErrNrCommandError                  = -100,
    EErrNrInvalidCharacter              = -101,
    EErrNrSyntaxError                   = -102,
    EErrNrInvalidSeparator              = -103,
    EErrNrDataTypeError                 = -104,
    EErrNrGETNotAllowed                 = -105,
    EErrNrParameterNotAllowed           = -108,
    EErrNrMissingParameter              = -109,
    EErrNrCommandHeaderError            = -110,
    EErrNrHeaderSeparatorError          = -111,
    EErrNrProgramMnemonicTooLong        = -112,
    EErrNrUndefinedHeader               = -113,
    EErrNrHeaderSuffixOutOfFange        = -114,
    EErrNrUnexpectedNumberOfParameters  = -115,
    EErrNrNumericDataError              = -120,
    EErrNrInvalidCharacterInNumber      = -121,
    EErrNrExponentTooLarge              = -123,
    EErrNrTooManyDigits                 = -124,
    EErrNrNumericDataNotAllowed         = -128,
    EErrNrSuffixError                   = -130,
    EErrNrInvalidSuffix                 = -131,
    EErrNrSuffixTooLong                 = -134,
    EErrNrSuffixNotAllowed              = -138,
    EErrNrCharacterDataError            = -140,
    EErrNrInvalidCharacterData          = -141,
    EErrNrCharacterDataTooLong          = -144,
    EErrNrCharacterDataNotAllowed       = -148,
    EErrNrStringDataError               = -150,
    EErrNrInvalidStringData             = -151,
    EErrNrStringDataNotAllowed          = -158,
    EErrNrBlockDataDrror                = -160,
    EErrNrInvalidBlockData              = -161,
    EErrNrBlockDataNotAllowed           = -168,
    EErrNrCommandExpressionError        = -170,
    EErrNrInvalidExpression             = -171,
    EErrNrExpressionDataNotAllowed      = -178,
    EErrNrCommandMacroError             = -180,
    EErrNrInvalidOutsideMacroDefinition = -181,
    EErrNrInvalidInsideMacroDefinition  = -183,
    EErrNrCommandMacroParameterError    = -184,

    // Execution Error
    //----------------

    EErrNrExecutionError               = -200,
    EErrNrInvalidWhileInLocal          = -201,
    EErrNrSettingsLostDueToRtl         = -202,
    EErrNrCommandProtected             = -203,
    EErrNrTriggerError                 = -210,
    EErrNrTriggerIgnored               = -211,
    EErrNrArmIgnored                   = -212,
    EErrNrInitIgnored                  = -213,
    EErrNrTriggerDeadlock              = -214,
    EErrNrArmDeadlock                  = -215,
    EErrNrParameterError               = -220,
    EErrNrSettingsConflict             = -221,
    EErrNrDataOutOfRange               = -222,
    EErrNrTooMuchData                  = -223,
    EErrNrIllegalParameterValue        = -224,
    EErrNrOutOfMemory                  = -225,
    EErrNrListsNotSameLength           = -226,
    EErrNrDataCorruptOrStale           = -230,
    EErrNrDataQuestionable             = -231,
    EErrNrInvalidFormat                = -232,
    EErrNrInvalidVersion               = -233,
    EErrNrHardwareError                = -240,
    EErrNrHardwareMissing              = -241,
    EErrNrMassStorageError             = -250,
    EErrNrMissingMassStorage           = -251,
    EErrNrMissingMedia                 = -252,
    EErrNrCorruptMedia                 = -253,
    EErrNrMediaFull                    = -254,
    EErrNrDirectoryFull                = -255,
    EErrNrFileNameNotFound             = -256,
    EErrNrFileNameError                = -257,
    EErrNrMediaProtected               = -258,
    EErrNrExecutionExpressionError     = -260,
    EErrNrMathErrorInExpression        = -261,
    EErrNrExecutionMacroError          = -270,
    EErrNrMacroSyntaxError             = -271,
    EErrNrMacroExecutionError          = -272,
    EErrNrIllegalMacroLabel            = -273,
    EErrNrExecutionMacroParameterError = -274,
    EErrNrMacroDefinitionTooLong       = -275,
    EErrNrMacroRecursionArror          = -276,
    EErrNrMacroRedefinitionNotAllowed  = -277,
    EErrNrMacroHeaderNotFound          = -278,
    EErrNrProgramError                 = -280,
    EErrNrCannotCreateProgram          = -281,
    EErrNrIllegalProgramName           = -282,
    EErrNrIllegalVariableName          = -283,
    EErrNrProgramCurrentlyRunning      = -284,
    EErrNrProgramSyntaxError           = -285,
    EErrNrProgramRuntimeError          = -286,
    EErrNrMemoryUseError               = -290,
    //EErrNrOutOfMemory                = -291,  = -225
    EErrNrReferencedNameDoesNotExist   = -292,
    EErrNrReferencedNameAlreadyExists  = -293,
    EErrNrIncompatibleType             = -294,

    // Device-Specific Error
    //----------------------

    EErrNrDeviceSpecificError          = -300,
    EErrNrSystemError                  = -310,
    EErrNrMemoryError                  = -311,
    EErrNrPUDMemoryLost                = -312,
    EErrNrCalibrationMemoryLost        = -313,
    EErrNrSaveRecallMemoryLost         = -314,
    EErrNrConfigurationMemoryLost      = -315,
    EErrNrStorageFault                 = -320,
    //EErrNrOutOfMemory                = -321,  = -225
    EErrNrSelfTestFailed               = -330,
    EErrNrCalibrationFailed            = -340,
    EErrNrQueueOverflow                = -350,
    EErrNrCommunicationError           = -360,
    EErrNrParityErrorInProgramMessage  = -361,
    EErrNrFramingErrorInProgramMessage = -362,
    EErrNrInputBufferOverrun           = -363,
    EErrNrTimeOutError                 = -365,

    // Query Error
    //------------

    EErrNrQueryError                               = -400,
    EErrNrQueryInterrupted                         = -410,
    EErrNrQueryUnterminated                        = -420,
    EErrNrQueryDeadlocked                          = -430,
    EErrNrQueryUnterminatedAfterIndefiniteResponse = -440,

    // Power On Event
    //---------------

    EErrNrPowerOn               = -500,

    // User Request Event
    //-------------------

    EErrNrUserRequest           = -600,

    // Request Control Event
    //-----------------------

    EErrNrRequestControl        = -700,

    // Operation Complete Event
    //-------------------------

    EErrNrOperationComplete     = -800
};

enum EErrNrAliasStr {
    EErrNrAliasStrEnumName    = 0,  // e.g. "InvalidCharacterData"
    EErrNrAliasStrText        = 1,  // e.g. "Invalid character data"
    EErrNrAliasStrDescription = 2   // e.g. "Either the character data element contains an invalid character or the particular element received is not valid for the header."
};

ZSREMCMDDLL_API ZS::System::EResult errNr2SysResultCode( int i_iErrNr );
ZSREMCMDDLL_API ZS::System::SErrResult errNr2SysErrResult( int i_iErrNr );
ZSREMCMDDLL_API ZS::System::SErrResultInfo errNr2SysErrResultInfo( int i_iErrNr );

ZSREMCMDDLL_API EErrNr sysResultCode2ErrNr( ZS::System::EResult i_result );
ZSREMCMDDLL_API EErrNr sysErrResult2ErrNr( const ZS::System::SErrResult& i_errResult );
ZSREMCMDDLL_API EErrNr sysErrResultInfo2ErrNr( const ZS::System::SErrResultInfo& i_errResultInfo );

ZSREMCMDDLL_API QString errNr2Str( int i_iErrNr, EErrNrAliasStr i_alias = ZS::RemCmd::EErrNrAliasStrText );
ZSREMCMDDLL_API EErrNr str2ErrNr( const QString& i_strErrNr );

ZSREMCMDDLL_API QByteArray errNr2ByteArr( int i_iErrNr, EErrNrAliasStr i_alias = ZS::RemCmd::EErrNrAliasStrText );
ZSREMCMDDLL_API EErrNr byteArr2ErrNr( const QByteArray& i_strErrNr );

ZSREMCMDDLL_API QString errResultInfo2Str( const ZS::System::SErrResultInfo& i_errResultInfo, EErrNrAliasStr i_alias = ZS::RemCmd::EErrNrAliasStrText );
ZSREMCMDDLL_API ZS::System::SErrResultInfo str2ErrResultInfo( const QString& i_str );

ZSREMCMDDLL_API QByteArray errResultInfo2ByteArr( const ZS::System::SErrResultInfo& i_errResultInfo, EErrNrAliasStr i_alias = ZS::RemCmd::EErrNrAliasStrText );
ZSREMCMDDLL_API ZS::System::SErrResultInfo byteArr2ErrResultInfo( const QByteArray& i_byteArr );

} // namespace RemCmd

} // namespace ZS

#endif // #ifndef ZSRemCmd_ErrCode_h
