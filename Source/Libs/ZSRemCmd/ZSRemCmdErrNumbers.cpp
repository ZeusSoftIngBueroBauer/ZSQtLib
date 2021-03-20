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

#include "ZSRemCmd/ZSRemCmdErrNumbers.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::RemCmd;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
enum Result
==============================================================================*/

const SEnumEntry s_arErrNrEnumEntries[] =
{
    // Please note that the error number is not equal to the error number
    // (SEnumEntry.m_iEnumerator != SEnumEntry.m_idxRow).
    // According to the recommendation of defining enumerations the error number
    // should have become the value of the enum entry.

                                                // EErrNrAliasStrEnumName, EErrNrAliasStrText, EErrNrAliasStrDescription
    /*   0 */ SEnumEntry( EErrNrNoError,                        "NoError",         "No error",                "No error" ),


    /*
    Command Error
    -------------

    An <error/event number> in the range [ -199 , -100 ] indicates that an
    IEEE 488.2 syntax error has been detected by the instrument’s parser.
    The occurrence of any error in this class shall cause the command error bit
    (bit 5) in the event status register) to be set. One of the following events
    has occurred:

    - An IEEE 488.2 syntax error has been detected by the parser. That is, a
      controller-to-device message was received which is in violation of the
      IEEE 488.2 standard. Possible violations include a data element which
      violates the device listening formats or whose type is unacceptable to the
      device.
    - An unrecognized header was received. Unrecognized headers include incorrect
      device-specific headers and incorrect or unimplemented IEEE 488.2 common
      commands.
    - A Group Execute Trigger (GET) was entered into the input buffer inside of an
      IEEE 488.2 <PROGRAM MESSAGE>. Events that generate command errors shall not
      generate execution errors, device-specific errors, or query errors.
    */
                                                             // EErrNrAliasStrEnumName,          EErrNrAliasStrText,                   EErrNrAliasStrDescription
    /* -100 */ SEnumEntry( EErrNrCommandError,                  "CommandError",                  "Command error",                      "This is the generic syntax error for devices that cannot detect more specific errors. This code indicates only that a Command Error as defined in IEEE 488.2, 11.5.1.1.4 has occurred." ),
    /* -101 */ SEnumEntry( EErrNrInvalidCharacter,              "InvalidCharacter",              "Invalid character",                  "A syntactic element contains a character which is invalid for that type; for example, a header containing an ampersand, SETUP&. This error might be used in place of errors -114, -121, -141, and perhaps some others." ),
    /* -102 */ SEnumEntry( EErrNrSyntaxError,                   "SyntaxError",                   "Syntax error",                       "An unrecognized command or data type was encountered; for example, a string was received when the device does not accept strings." ),
    /* -103 */ SEnumEntry( EErrNrInvalidSeparator,              "InvalidSeparator",              "Invalid separator",                  "The parser was expecting a separator and encountered an illegal character; for example, the semicolon was omitted after a program message unit, *EMC 1:CH1:VOLTS 5." ),
    /* -104 */ SEnumEntry( EErrNrDataTypeError,                 "DataTypeError",                 "Data type error",                    "The parser recognized a data element different than one allowed; for example, numeric or string data was expected but block data was encountered." ),
    /* -105 */ SEnumEntry( EErrNrGETNotAllowed,                 "GETNotAllowed",                 "GET not allowed",                    "A Group Execute Trigger was received within a program message (see IEEE 488.2, 7.7)." ),
    /* -108 */ SEnumEntry( EErrNrParameterNotAllowed,           "ParameterNotAllowed",           "Parameter not allowed",              "More parameters were received than expected for the header; for example, the *EMC common command only accepts one parameter, so receiving *EMC 0,1 is not allowed." ),
    /* -109 */ SEnumEntry( EErrNrMissingParameter,              "MissingParameter",              "Missing parameter",                  "Fewer parameters were recieved than required for the header; for example, the *EMC common command requires one parameter, so receiving *EMC is not allowed." ),
    /* -110 */ SEnumEntry( EErrNrCommandHeaderError,            "CommandHeaderError",            "Command header error",               "An error was detected in the header. This error message should be used when the device cannot detect the more specific errors described for errors -111 through -119." ),
    /* -111 */ SEnumEntry( EErrNrHeaderSeparatorError,          "HeaderSeparatorError",          "Header separator error",             "A character which is not a legal header separator was encountered while parsing the header; for example, no white shace followed the header, thus *GMC\"MACRO\" is an error." ),
    /* -112 */ SEnumEntry( EErrNrProgramMnemonicTooLong,        "ProgramMnemonicTooLong",        "Program mnemonic too long",          "The header contains more that twelve characters (see IEEE 488.2, 7.6.1.4.1)." ),
    /* -113 */ SEnumEntry( EErrNrUndefinedHeader,               "UndefinedHeader",               "Undefined header",                   "The header is syntactically correct, but it is undefined for this specific device; for example, *XYZ is not defined for any device." ),
    /* -114 */ SEnumEntry( EErrNrHeaderSuffixOutOfFange,        "HeaderSuffixOutOfFange",        "Header suffix out of range",         "The value of a numeric suffix attached to a program mnemonic, see Syntax and Style section 6.2.5.2, makes the header invalid." ),
    /* -115 */ SEnumEntry( EErrNrUnexpectedNumberOfParameters,  "UnexpectedNumberOfParameters",  "Unexpected number of parameters",    "The number of parameters received does not correspond to the number of parameters expected. This is typically due an inconsistency with the number of instruments in the selected group (see section on INSTrument:DEFine:GROup)." ),
    /* -120 */ SEnumEntry( EErrNrNumericDataError,              "NumericDataError",              "Numeric data error",                 "This error, as well as errors -121 through -129, are generated when parsing a data element which appears to be numeric, including the nondecimal numeric types. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -121 */ SEnumEntry( EErrNrInvalidCharacterInNumber,      "InvalidCharacterInNumber",      "Invalid character in number",        "An invalid character for the data type being parsed was encountered; for example, an alpha in a decimal numeric or a \"9\" in octal data." ),
    /* -123 */ SEnumEntry( EErrNrExponentTooLarge,              "ExponentTooLarge",              "Exponent too large",                 "The magnitude of the exponent was larger than 32000 (see IEEE 488.2, 7.7.2.4.1)." ),
    /* -124 */ SEnumEntry( EErrNrTooManyDigits,                 "TooManyDigits",                 "Too many digits",                    "The mantissa of a decimal numeric data element contained more than 255 digits excluding leading zeros (see IEEE 488.2, 7.7.2.4.1)." ),
    /* -128 */ SEnumEntry( EErrNrNumericDataNotAllowed,         "NumericDataNotAllowed",         "Numeric data not allowed",           "A legal numeric data element was received, but the device does not accept one in this position for the header." ),
    /* -130 */ SEnumEntry( EErrNrSuffixError,                   "SuffixError",                   "Suffix error",                       "This error, as well as errors -131 through -139, are generated when parsing a suffix. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -131 */ SEnumEntry( EErrNrInvalidSuffix,                 "InvalidSuffix",                 "Invalid suffix",                     "The suffix does not follow the syntax described in IEEE 488.2, 7.7.3.2, or the suffix is inappropriate for this device." ),
    /* -134 */ SEnumEntry( EErrNrSuffixTooLong,                 "SuffixTooLong",                 "Suffix too long",                    "The suffix contained more than 12 characters (see IEEE 488.2, 7.7.3.4)." ),
    /* -138 */ SEnumEntry( EErrNrSuffixNotAllowed,              "SuffixNotAllowed",              "Suffix not allowed",                 "A suffix was encountered after a numeric element which does not allow suffixes." ),
    /* -140 */ SEnumEntry( EErrNrCharacterDataError,            "CharacterDataError",            "Character data ",                    "This error, as well as errors -141 through -149, are generated when parsing a character data element. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -141 */ SEnumEntry( EErrNrInvalidCharacterData,          "InvalidCharacterData",          "Invalid character data",             "Either the character data element contains an invalid character or the particular element received is not valid for the header." ),
    /* -144 */ SEnumEntry( EErrNrCharacterDataTooLong,          "CharacterDataTooLong",          "Character data too long",            "The character data element contains more than twelve characters (see IEEE 488.2, 7.7.1.4)." ),
    /* -148 */ SEnumEntry( EErrNrCharacterDataNotAllowed,       "CharacterDataNotAllowed",       "Character data not allowed",         "A legal character data element was encountered where prohibited by the device." ),
    /* -150 */ SEnumEntry( EErrNrStringDataError,               "StringDataError",               "String data error",                  "This error, as well as errors -151 through -159, are generated when parsing a string data element. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -151 */ SEnumEntry( EErrNrInvalidStringData,             "InvalidStringData",             "Invalid string data",                "A string data element was expected, but was invalid for some reason (see IEEE 488.2, 7.7.5.2); for example, an END message was received before the terminal quote character." ),
    /* -158 */ SEnumEntry( EErrNrStringDataNotAllowed,          "StringDataNotAllowed",          "String data not allowed",            "A string data element was encountered but was not allowed by the device at this point in parsing." ),
    /* -160 */ SEnumEntry( EErrNrBlockDataDrror,                "BlockDataDrror",                "Block data error",                   "This error, as well as errors -161 through -169, are generated when parsing a block data element. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -161 */ SEnumEntry( EErrNrInvalidBlockData,              "InvalidBlockData",              "Invalid block data",                 "A block data element was expected, but was invalid for some reason (see IEEE 488.2, 7.7.6.2); for example, an END message was received before the length was satisfied." ),
    /* -168 */ SEnumEntry( EErrNrBlockDataNotAllowed,           "BlockDataNotAllowed",           "Block data not allowed",             "A legal block data element was encountered but was not allowed by the device at this point in parsing." ),
    /* -170 */ SEnumEntry( EErrNrCommandExpressionError,        "CommandExpressionError",        "Expression error",                   "This error, as well as errors -171 through -179, are generated when parsing an expression data element. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -171 */ SEnumEntry( EErrNrInvalidExpression,             "InvalidExpression",             "Invalid expression",                 "The expression data element was invalid (see IEEE 488.2, 7.7.7.2); for example, unmatched parentheses or an illegal character." ),
    /* -178 */ SEnumEntry( EErrNrExpressionDataNotAllowed,      "ExpressionDataNotAllowed",      "Expression data not allowed",        "A legal expression data was encountered but was not allowed by the device at this point in parsing." ),
    /* -180 */ SEnumEntry( EErrNrCommandMacroError,             "CommandMacroError",             "Macro error",                        "This error, as well as errors -181 through -189, are generated when defining a macro or executing a macro. This particular error message should be used if the device cannot detect a more specific error." ),
    /* -181 */ SEnumEntry( EErrNrInvalidOutsideMacroDefinition, "InvalidOutsideMacroDefinition", "Invalid outside macro definition",   "Indicates that a macro parameter placeholder ($<number) was encountered outside of a macro definition." ),
    /* -183 */ SEnumEntry( EErrNrInvalidInsideMacroDefinition,  "InvalidInsideMacroDefinition",  "Invalid inside macro definition",    "Indicates that the program message unit sequence, sent with a *DDT or *DMC command, is syntactically invalid (see IEEE 488.2, 10.7.6.3)." ),
    /* -184 */ SEnumEntry( EErrNrCommandMacroParameterError,    "CommandMacroParameterError",    "Macro parameter error",              "Indicates that a command inside the macro definition had the wrong number or type of parameters." ),

    /*
    Execution Error
    ---------------

    An <error/event number> in the range [ -299 , -200 ] indicates that an error
    has been detected by the instrument’s execution control block. The occurrence
    of any error in this class shall cause the execution error bit (bit 4) in the
    event status register) to be set. One of the following events has occurred:

    - A <PROGRAM DATA> element following a header was evaluated by the device
      as outside of its legal input range or is otherwise inconsistent with the device’s
      capabilities.
    - A valid program message could not be properly executed due to some device
      condition.

    Execution errors shall be reported by the device after rounding and expression evaluation
    operations have taken place. Rounding a numeric data element, for example, shall not be
    reported as an execution error. Events that generate execution errors shall not generate
    Command Errors, device-specific errors, or Query Errors; see the other error definitions in
    this section.
    */

                                                             // EErrNrAliasStrEnumName,          EErrNrAliasStrText,                   EErrNrAliasStrDescription
    /* -200 */ SEnumEntry( EErrNrExecutionError,               "ExecutionError",              "Execution error",                "This is the generic syntax error for devices that cannot detect more specific errors. This code indicates only that an Execution Error as defined in IEEE 488.2, 11.5.1.1.5 has occurred." ),
    /* -201 */ SEnumEntry( EErrNrInvalidWhileInLocal,          "InvalidWhileInLocal",         "Invalid while in local",         "Indicates that a command is not executable while the device is in local due to a hard local control (see IEEE 488.2, 5.6.1.5); for example, a device with a rotary switch receives a message which would change the switches state, but the device is in local so the message can not be executed." ),
    /* -202 */ SEnumEntry( EErrNrSettingsLostDueToRtl,         "SettingsLostDueToRtl",        "Settings lost due to rtl",       "Indicates that a setting associated with a hard local control (see IEEE 488.2, 5.6.1.5) was lost when the device changed to LOCS from REMS or to LWLS from RWLS." ),
    /* -203 */ SEnumEntry( EErrNrCommandProtected,             "CommandProtected",            "Command protected",              "Indicates that a legal password-protected program command or query could not be executed because the command was disabled." ),
    /* -210 */ SEnumEntry( EErrNrTriggerError,                 "TriggerError",                "Trigger error",                  "" ),
    /* -211 */ SEnumEntry( EErrNrTriggerIgnored,               "TriggerIgnored",              "Trigger ignored",                "Indicates that a GET, *TRG, or triggering signal was received and recognized by the device but was ignored because of device timing considerations; for example, the device was not ready to respond. Note: a DT0 device always ignores GET and treats *TRG as a Command Error." ),
    /* -212 */ SEnumEntry( EErrNrArmIgnored,                   "ArmIgnored",                  "Arm ignored",                    "Indicates that an arming signal was received and recognized by the device but was ignored." ),
    /* -213 */ SEnumEntry( EErrNrInitIgnored,                  "InitIgnored",                 "Init ignored",                   "Indicates that a request for a measurement initiation was ignored as another measurement was already in progress." ),
    /* -214 */ SEnumEntry( EErrNrTriggerDeadlock,              "TriggerDeadlock",             "Trigger deadlock",               "Indicates that the trigger source for the initiation of a measurement is set to GET and subsequent measurement query is received. The measurement cannot be started until a GET is received, but the GET would cause an INTERRUPTED error." ),
    /* -215 */ SEnumEntry( EErrNrArmDeadlock,                  "ArmDeadlock",                 "Arm deadlock",                   "Indicates that the arm source for the initiation of a measurement is set to GET and subsequent measurement query is received. The measurement cannot bestarted until a GET is received, but the GET would cause an INTERRUPTED error." ),
    /* -220 */ SEnumEntry( EErrNrParameterError,               "ParameterError",              "Parameter error",                "Indicates that a program data element related error occurred. This error message should be used when the device cannot detect the more specific errors described for errors -221 through -229." ),
    /* -221 */ SEnumEntry( EErrNrSettingsConflict,             "SettingsConflict",            "Settings conflict",              "Indicates that a legal program data element was parsed but could not be executed due to the current device state (see IEEE 488.2, 6.4.5.3 and 11.5.1.1.5.)" ),
    /* -222 */ SEnumEntry( EErrNrDataOutOfRange,               "DataOutOfRange",              "Data out of range",              "Indicates that a legal program data element was parsed but could not be executed because the interpreted value was outside the legal range as defined by the device (see IEEE 488.2, 11.5.1.1.5.)" ),
    /* -223 */ SEnumEntry( EErrNrTooMuchData,                  "TooMuchData",                 "Too much data",                  "Indicates that a legal program data element of block, expression, or string type was received that contained more data than the device could handle due to memory or related device-specific requirements." ),
    /* -224 */ SEnumEntry( EErrNrIllegalParameterValue,        "IllegalParameterValue",       "Illegal parameter value",        "Used where exact value, from a list of possibles, was expected." ),
    /* -225 */ SEnumEntry( EErrNrOutOfMemory,                  "OutOfMemory",                 "Out of memory",                  "The device has insufficent memory to perform the requested operation." ),
    /* -226 */ SEnumEntry( EErrNrListsNotSameLength,           "ListsNotSameLength",          "Lists not same length",          "Attempted to use LIST structure having individual LISTs of unequal lengths." ),
    /* -230 */ SEnumEntry( EErrNrDataCorruptOrStale,           "DataCorruptOrStale",          "Data corrupt or stale",          "Possibly invalid data; new reading started but not completed since last access." ),
    /* -231 */ SEnumEntry( EErrNrDataQuestionable,             "DataQuestionable",            "Data questionable",              "Indicates that measurement accuracy is suspect." ),
    /* -232 */ SEnumEntry( EErrNrInvalidFormat,                "InvalidFormat",               "Invalid format",                 "Indicates that a legal program data element was parsed but could not be executed because the data format or structure is inappropriate. For example when loading memory tables or when sending a SYSTem:SET parameter from an unknown instrument." ),
    /* -233 */ SEnumEntry( EErrNrInvalidVersion,               "InvalidVersion",              "Invalid version",                "Indicates that a legal program data element was parsed but could not be executed because the version of the data is incorrect to the device. This particular error should be used when file or block data formats are recognized by the instrument but cannot be executed for reasons of version incompatibility. For example, a not supported file version, a not supported instrument version." ),
    /* -240 */ SEnumEntry( EErrNrHardwareError,                "HardwareError",               "Hardware error",                 "Indicates that a legal program command or query could not be executed because of a hardware problem in the device. Definition of what constitutes a hardware problem is completely device-specific. This error message should be used when the device cannot detect the more specific errors described for errors -241 through -249." ),
    /* -241 */ SEnumEntry( EErrNrHardwareMissing,              "HardwareMissing",             "Hardware missing",               "Indicates that a legal program command or query could not be executed because of missing device hardware; for example, an option was not installed. Definition of what constitutes missing hardware is completely device-specific." ),
    /* -250 */ SEnumEntry( EErrNrMassStorageError,             "MassStorageError",            "Mass storage error",             "Indicates that a mass storage error occurred. This error message should be used when the device cannot detect the more specific errors described for errors -251 through -259." ),
    /* -251 */ SEnumEntry( EErrNrMissingMassStorage,           "MissingMassStorage",          "Missing mass storage",           "Indicates that a legal program command or query could not be executed because of missing mass storage; for example, an option that was not installed. Definition of what constitutes missing mass storage is device-specific." ),
    /* -252 */ SEnumEntry( EErrNrMissingMedia,                 "MissingMedia",                "Missing media",                  "Indicates that a legal program command or query could not be executed because of a missing media; for example, no disk. The definition of what constitutes missing media is device-specific." ),
    /* -253 */ SEnumEntry( EErrNrCorruptMedia,                 "CorruptMedia",                "Corrupt media",                  "Indicates that a legal program command or query could not be executed because of corrupt media; for example, bad disk or wrong format. The definition of what constitutes corrupt media is device-specific." ),
    /* -254 */ SEnumEntry( EErrNrMediaFull,                    "MediaFull",                   "Media full",                     "Indicates that a legal program command or query could not be executed because the media was full; for example, there is no room on the disk. The definition of what constitutes a full media is device-specific." ),
    /* -255 */ SEnumEntry( EErrNrDirectoryFull,                "DirectoryFull",               "Directory full",                 "Indicates that a legal program command or query could not be executed because the media directory was full. The definition of what constitutes a full media directory is device-specific." ),
    /* -256 */ SEnumEntry( EErrNrFileNameNotFound,             "FileNameNotFound",            "File name not found",            "Indicates that a legal program command or query could not be executed because the file name on the device media was not found; for example, an attempt was made to read or copy a nonexistent file. The definition of what constitutes a file not being found is device-specific." ),
    /* -257 */ SEnumEntry( EErrNrFileNameError,                "FileNameError",               "File name error",                "Indicates that a legal program command or query could not be executed because the file name on the device media was in error; for example, an attempt was made to copy to a duplicate file name. The definition of what constitutes a file name error is device-specific." ),
    /* -258 */ SEnumEntry( EErrNrMediaProtected,               "MediaProtected",              "Media protected",                "Indicates that a legal program command or query could not be executed because the media was protected; for example, the write-protect tab on a disk was present. The definition of what constitutes protected media is device-specific." ),
    /* -260 */ SEnumEntry( EErrNrExecutionExpressionError,     "ExecutionExpressionError",    "Expression error",               "Indicates that a expression program data element related error occurred. This error message should be used when the device cannot detect the more specific errors described for errors -261 through -269." ),
    /* -261 */ SEnumEntry( EErrNrMathErrorInExpression,        "MathErrorInExpression",       "Math error in expression",       "Indicates that a syntactically legal expression program data element could not be executed due to a math error; for example, a divide-by-zero was attempted. The definition of math error is device-specific." ),
    /* -270 */ SEnumEntry( EErrNrExecutionMacroError,          "ExecutionMacroError",         "Macro error",                    "Indicates that a macro-related execution error occurred. This error message should be used when the device cannot detect the more specific errors described for errors -271 through -279." ),
    /* -271 */ SEnumEntry( EErrNrMacroSyntaxError,             "MacroSyntaxError",            "Macro syntax error",             "Indicates that that a syntactically legal macro program data sequence, according to IEEE 488.2, 10.7.2, could not be executed due to a syntax error within the macro definition (see IEEE 488.2, 10.7.6.3.)" ),
    /* -272 */ SEnumEntry( EErrNrMacroExecutionError,          "MacroExecutionError",         "Macro execution error",          "Indicates that a syntactically legal macro program data sequence could not be executed due to some error in the macro definition (see IEEE 488.2, 10.7.6.3.)" ),
    /* -273 */ SEnumEntry( EErrNrIllegalMacroLabel,            "IllegalMacroLabel",           "Illegal macro label",            "Indicates that the macro label defined in the *DMC command was a legal string syntax, but could not be accepted by the device (see IEEE 488.2, 10.7.3 and 10.7.6.2); for example, the label was too long, the same as a common command header, or contained invalid header syntax." ),
    /* -274 */ SEnumEntry( EErrNrExecutionMacroParameterError, "ExecutionMacroParameterError","Macro parameter error",          "Indicates that the macro definition improperly used a macro parameter placeholder (see IEEE 488.2, 10.7.3)." ),
    /* -275 */ SEnumEntry( EErrNrMacroDefinitionTooLong,       "MacroDefinitionTooLong",      "Macro definition too long",      "Indicates that a syntactically legal macro program data sequence could not be executed because the string or block contents were too long for the device to handle (see IEEE 488.2, 10.7.6.1)." ),
    /* -276 */ SEnumEntry( EErrNrMacroRecursionArror,          "MacroRecursionArror",         "Macro recursion error",          "Indicates that a syntactically legal macro program data sequence could not be executed because the device found it to be recursive (see IEEE 488.2, 10.7.6.6)." ),
    /* -277 */ SEnumEntry( EErrNrMacroRedefinitionNotAllowed,  "MacroRedefinitionNotAllowed", "Macro redefinition not allowed", "Indicates that a syntactically legal macro label in the *DMC command could not be executed because the macro label was already defined (see IEEE 488.2, 10.7.6.4)." ),
    /* -278 */ SEnumEntry( EErrNrMacroHeaderNotFound,          "MacroHeaderNotFound",         "Macro header not found",         "Indicates that a syntactically legal macro label in the *GMC? query could not be executed because the header was not previously defined." ),
    /* -280 */ SEnumEntry( EErrNrProgramError,                 "ProgramError",                "Program error",                  "Indicates that a downloaded program-related execution error occurred. This error message should be used when the device cannot detect the more specific errors described for errors -281 through -289. A downloaded program is used to add algorithmic capability to a device. The syntax used in the program and the mechanism for downloading a program is device-specific." ),
    /* -281 */ SEnumEntry( EErrNrCannotCreateProgram,          "CannotCreateProgram",         "Cannot create program",          "Indicates that an attempt to create a program was unsuccessful. A reason for the failure might include not enough memory." ),
    /* -282 */ SEnumEntry( EErrNrIllegalProgramName,           "IllegalProgramName",          "Illegal program name",           "The name used to reference a program was invalid; for example, redefining an existing program, deleting a nonexistent program, or in general, referencing a nonexistent program." ),
    /* -283 */ SEnumEntry( EErrNrIllegalVariableName,          "IllegalVariableName",         "Illegal variable name",          "An attempt was made to reference a nonexistent variable in a program." ),
    /* -284 */ SEnumEntry( EErrNrProgramCurrentlyRunning,      "ProgramCurrentlyRunning",     "Program currently running",      "Certain operations dealing with programs may be illegal while the program is running; for example, deleting a running program might not be possible." ),
    /* -285 */ SEnumEntry( EErrNrProgramSyntaxError,           "ProgramSyntaxError",          "Program syntax error",           "Indicates that a syntax error appears in a downloaded program. The syntax used when parsing the downloaded program is device-specific." ),
    /* -286 */ SEnumEntry( EErrNrProgramRuntimeError,          "ProgramRuntimeError",         "Program runtime error",          "" ),
    /* -290 */ SEnumEntry( EErrNrMemoryUseError,               "MemoryUseError",              "Memory use error",               "Indicates that a user request has directly or indirectly caused an error related to  memory or <data_handle>s, this is not the same as bad memory." ),
    // -291 ** SEnumEntry( EErrNrOutOfMemory,                  "OutOfMemory",                 "Out of memory",                  "" ),
    /* -292 */ SEnumEntry( EErrNrReferencedNameDoesNotExist,   "ReferencedNameDoesNotExist",  "Referenced name does not exist", "" ),
    /* -293 */ SEnumEntry( EErrNrReferencedNameAlreadyExists,  "ReferencedNameAlreadyExists", "Referenced name already exists", "" ),
    /* -294 */ SEnumEntry( EErrNrIncompatibleType,             "IncompatibleType",            "Incompatible type",              "Indicates that the type or structure of a memory item is inadequate" ),

    /*
    Device-Specific Error
    ---------------------

    An <error/event number> in the range [ -399 , -300 ] or [ 1 , 32767 ] indicates
    that the instrument has detected an error which is not a command error, a query
    error, or an execution error; some device operations did not properly complete,
    possibly due to an abnormal hardware or firmware condition. These codes are also
    used for self-test response errors. The occurrence of any error in this class
    should cause the device-specific error bit (bit 3) in the event status register
    to be set. The meaning of positive error codes is device-dependent and may be
    enumerated or bit mapped; the <error message> string for positive error codes is
    not defined by SCPI and available to the device designer.
    Note that the string is not optional; if the designer does not wish to implement
    a string for a particular error, the null string should be sent (for example, 42,"").
    The occurrence of any error in this class should cause the device-specific error bit
    (bit 3) in the event status register to be set.
    Events that generate device-specific errors shall not generate command errors,
    execution errors, or query errors; see the other error definitions in this section.
    */

                                                            // EErrNrAliasStrEnumName,         EErrNrAliasStrText,                 EErrNrAliasStrDescription
    /* -300 */ SEnumEntry( EErrNrDeviceSpecificError,          "DeviceSpecificError",          "Device-specific error",            "This is the generic device-dependent error for devices that cannot detect more specific errors. This code indicates only that a Device-Dependent Error as defined in IEEE 488.2, 11.5.1.1.6 has occurred." ),
    /* -310 */ SEnumEntry( EErrNrSystemError,                  "SystemError",                  "System error",                     "Indicates that some error, termed system error by the device, has occurred. This code is device-dependent." ),
    /* -311 */ SEnumEntry( EErrNrMemoryError,                  "MemoryError",                  "Memory error",                     "Indicates some physical fault in the devices memory, such as parity error." ),
    /* -312 */ SEnumEntry( EErrNrPUDMemoryLost,                "PUDMemoryLost",                "PUD memory lost",                  "Indicates that the protected user data saved by the *PUD command has been lost." ),
    /* -313 */ SEnumEntry( EErrNrCalibrationMemoryLost,        "CalibrationMemoryLost",        "Calibration memory lost",          "Indicates that nonvolatile calibration data used by the *CAL? command has been lost." ),
    /* -314 */ SEnumEntry( EErrNrSaveRecallMemoryLost,         "SaveRecallMemoryLost",         "Save/recall memory lost",          "Indicates that the nonvolatile data saved by the *SAV? command has been lost." ),
    /* -315 */ SEnumEntry( EErrNrConfigurationMemoryLost,      "ConfigurationMemoryLost",      "Configuration memory lost",        "Indicates that nonvolatile configuration data saved by the device has been lost. The meaning of this error is device-specific." ),
    /* -320 */ SEnumEntry( EErrNrStorageFault,                 "StorageFault",                 "Storage fault",                    "Indicates that the firmware detected a fault when using data storage. This error is not an indication of physical damage or failure of any mass storage element." ),
    // -321 ** SEnumEntry( EErrNrOutOfMemory,                  "OutOfMemory",                  "Out of memory",                    "An internal operation needed more memory than was available." ),
    /* -330 */ SEnumEntry( EErrNrSelfTestFailed,               "SelfTestFailed",               "Self-test failed",                 "" ),
    /* -340 */ SEnumEntry( EErrNrCalibrationFailed,            "CalibrationFailed",            "Calibration failed",               "" ),
    /* -350 */ SEnumEntry( EErrNrQueueOverflow,                "QueueOverflow",                "Queue overflow",                   "A specific code entered into the queue in lieu of the code that caused the error. This code indicates that there is no room in the queue and an error occurred but was not recorded." ),
    /* -360 */ SEnumEntry( EErrNrCommunicationError,           "CommunicationError",           "Communication error",              "This is the generic communication error for devices that cannot detect the more specific errors described for errors -361 through -363." ),
    /* -361 */ SEnumEntry( EErrNrParityErrorInProgramMessage,  "ParityErrorInProgramMessage",  "Parity error in program message",  "Parity bit not correct when data received for example, on a serial port." ),
    /* -362 */ SEnumEntry( EErrNrFramingErrorInProgramMessage, "FramingErrorInProgramMessage", "Framing error in program message", "A stop bit was not detected when data was received for example, on a serial port (for example, a baud rate mismatch)." ),
    /* -363 */ SEnumEntry( EErrNrInputBufferOverrun,           "InputBufferOverrun",           "Input buffer overrun",             "Software or hardware input buffer on serial port overflows with data caused by improper or nonexistent pacing." ),
    /* -365 */ SEnumEntry( EErrNrTimeOutError,                 "TimeOutError",                 "Time out error",                   "This is a generic device-dependent error." ),

    /*
    Query Error
    -----------

    An <error/event number> in the range [ -499 , -400 ] indicates that the
    output queue control of the instrument has detected a problem with the
    message exchange protocol described in IEEE 488.2, chapter 6. The
    occurrence of any error in this class shall cause the query error bit (bit 2)
    in the event status register to be set.
    These errors correspond to message exchange protocol errors described in
    IEEE 488.2, section 6.5. One of the following is true:

    - An attempt is being made to read data from the output queue when no output is
      either present or pending;
    - Data in the output queue has been lost.

    Events that generate query errors shall not generate command errors, execution
    errors, or device-specific errors; see the other error definitions in this section.
    */

                                                                        // EErrNrAliasStrEnumName,                     EErrNrAliasStrText,                             EErrNrAliasStrDescription
    /* -400 */ SEnumEntry( EErrNrQueryError,                               "QueryError",                               "Query error",                                  "This is the generic query error for devices that cannot detect more specific errors. This code indicates only that a Query Error as defined in IEEE 488.2, 11.5.1.1.7 and 6.3 has occurred." ),
    /* -410 */ SEnumEntry( EErrNrQueryInterrupted,                         "QueryInterrupted",                         "Query INTERRUPTED",                            "Indicates that a condition causing an INTERRUPTED Query error occurred (see IEEE 488.2, 6.3.2.3); for example, a query followed by DAB or GET before a response was completely sent." ),
    /* -420 */ SEnumEntry( EErrNrQueryUnterminated,                        "QueryUnterminated",                        "Query UNTERMINATED",                           "Indicates that a condition causing an UNTERMINATED Query error occurred (see IEEE 488.2, 6.3.2.2); for example, the device was addressed to talk and an incomplete program message was received." ),
    /* -430 */ SEnumEntry( EErrNrQueryDeadlocked,                          "QueryDeadlocked",                          "Query DEADLOCKED",                             "Indicates that a condition causing an DEADLOCKED Query error occurred (see IEEE 488.2, 6.3.1.7); for example, both input buffer and output buffer are full and the device cannot continue." ),
    /* -440 */ SEnumEntry( EErrNrQueryUnterminatedAfterIndefiniteResponse, "QueryUnterminatedAfterIndefiniteResponse", "Query UNTERMINATED after indefinite response", "Indicates that a query was received in the same program message after an query requesting an indefinite response was executed (see IEEE 488.2, 6.5.7.5)." ),

    /*
    Power On Event
    --------------

    An <error/event number> in the range [-599:-500] is used when the instrument wishes to TK047
    report a 488.2 power on event to the event/error queue. This event occurs when the
    instrument detects an off to on transition in its power supply. This event also sets the power
    on bit, (bit 7) of the Standard Event Status Register. See IEEE 488.2, section 11.5.1.
    */

    /* -500 */ SEnumEntry( EErrNrPowerOn, "PowerOn", "Power on", "The instrument has detected an off to on transition in its power supply." ),

    /*
    User Request Event
    ------------------

    An <error/event number> in the range [-699:-600] is used when the instrument wishes to TK047
    report a 488.2 user request event. This event occurs when the instrument detects the
    activation of a user request local control. This event also sets the user request bit (bit 6) of
    the Standard Event Status Register. See IEEE 488.2, section 11.5.1.
    */

    /* -600 */ SEnumEntry( EErrNrUserRequest, "UserRequest", "User request", "The instrument has detected the activation of a user request local control" ),

    /*
    Request Control Event
    ---------------------

    An <error/event number> in the range [-799:-700] is used when the instrument wishes to TK047
    report a 488.2 request control event to the error/event queue. This event occurs when the
    instrument requests to become the active IEEE 488.1 controller-in-charge. This event also
    sets request control bit (bit 1) of the Standard Event Status Register. See IEEE 488.2,
    section 11.5.1.
    */

    /* -700 */ SEnumEntry( EErrNrRequestControl, "RequestControl", "Request control", "The instrument requested to become the active IEEE 488.1 controller-in-charge" ),

    /*
    Operation Complete Event
    ------------------------

    An <error/event number> in the range [-899:-800] is used when the instrument wishes to TK047
    report a 488.2 operation complete event to the error/event queue. This event occurs when
    instrument’s synchronization protocol, having been enabled by an *OPC command,
    completes all selected pending operations. This protocol is described in IEEE 488.2, section
    12.5.2. This event also sets the operation complete bit (bit 0) of the Standard Event Status
    Register. See IEEE 488.2, section 11.5.1. Note: *OPC? does not set bit 0 nor does it enter
    any event in the error/event queue.
    */

    /* -800 */ SEnumEntry( EErrNrOperationComplete, "OperationComplete", "Operation complete", "The instrument has completed all selected pending operations in accordance with the IEEE 488.2, 12.5.2 synchronization protocol" )
};

static bool s_bErrNrMapsInitialised = false;

static QHash<EErrNr, int>        s_hshErrNr2EnumEntryIdx;
static QHash<QString, EErrNr>    s_hshStrEnumName2ErrNr;
static QHash<QString, EErrNr>    s_hshStrEnumText2ErrNr;
static QHash<QByteArray, EErrNr> s_hshByteArrEnumName2ErrNr;
static QHash<QByteArray, EErrNr> s_hshByteArrEnumText2ErrNr;
static QHash<EErrNr, EResult>    s_hshErrNr2SysResultCode;
static QHash<EResult, EErrNr>    s_hshSysResultCode2ErrNr;

//------------------------------------------------------------------------------
static void initErrNrMaps()
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        int idxEntry;

        for( idxEntry = 0; idxEntry < _ZSArrLen(s_arErrNrEnumEntries); ++idxEntry )
        {
            const SEnumEntry& enumEntry = s_arErrNrEnumEntries[idxEntry];

            // Please note that the error number is not equal to the error number
            // (SEnumEntry.m_iEnumerator != SEnumEntry.m_idxRow).
            // According to the recommendation of defining enumerations the error number
            // should have become the value of the enum entry.
            s_hshErrNr2EnumEntryIdx[static_cast<EErrNr>(enumEntry.m_iEnumerator)] = idxEntry;

            s_hshStrEnumName2ErrNr[enumEntry.m_strlstNames[EErrNrAliasStrEnumName]] = static_cast<EErrNr>(enumEntry.m_iEnumerator);
            s_hshStrEnumText2ErrNr[enumEntry.m_strlstNames[EErrNrAliasStrText]] = static_cast<EErrNr>(enumEntry.m_iEnumerator);
            s_hshByteArrEnumName2ErrNr[enumEntry.m_strlstNames[EErrNrAliasStrEnumName].toStdString().c_str()] = static_cast<EErrNr>(enumEntry.m_iEnumerator);
            s_hshByteArrEnumText2ErrNr[enumEntry.m_strlstNames[EErrNrAliasStrText].toStdString().c_str()] = static_cast<EErrNr>(enumEntry.m_iEnumerator);
        }

        /*   -1 */ s_hshErrNr2SysResultCode[EErrNrUndefined]                                 = EResultUndefined;
        /*    0 */ s_hshErrNr2SysResultCode[EErrNrNoError]                                   = EResultUndefined;
        /* -100 */ s_hshErrNr2SysResultCode[EErrNrCommandError]                              = EResultUndefined;
        /* -101 */ s_hshErrNr2SysResultCode[EErrNrInvalidCharacter]                          = EResultUndefined;
        /* -102 */ s_hshErrNr2SysResultCode[EErrNrSyntaxError]                               = EResultUndefined;
        /* -103 */ s_hshErrNr2SysResultCode[EErrNrInvalidSeparator]                          = EResultUndefined;
        /* -104 */ s_hshErrNr2SysResultCode[EErrNrDataTypeError]                             = EResultUndefined;
        /* -105 */ s_hshErrNr2SysResultCode[EErrNrGETNotAllowed]                             = EResultUndefined;
        /* -108 */ s_hshErrNr2SysResultCode[EErrNrParameterNotAllowed]                       = EResultUndefined;
        /* -109 */ s_hshErrNr2SysResultCode[EErrNrMissingParameter]                          = EResultUndefined;
        /* -110 */ s_hshErrNr2SysResultCode[EErrNrCommandHeaderError]                        = EResultUndefined;
        /* -111 */ s_hshErrNr2SysResultCode[EErrNrHeaderSeparatorError]                      = EResultUndefined;
        /* -112 */ s_hshErrNr2SysResultCode[EErrNrProgramMnemonicTooLong]                    = EResultUndefined;
        /* -113 */ s_hshErrNr2SysResultCode[EErrNrUndefinedHeader]                           = EResultUndefined;
        /* -114 */ s_hshErrNr2SysResultCode[EErrNrHeaderSuffixOutOfFange]                    = EResultUndefined;
        /* -115 */ s_hshErrNr2SysResultCode[EErrNrUnexpectedNumberOfParameters]              = EResultUndefined;
        /* -120 */ s_hshErrNr2SysResultCode[EErrNrNumericDataError]                          = EResultUndefined;
        /* -121 */ s_hshErrNr2SysResultCode[EErrNrInvalidCharacterInNumber]                  = EResultUndefined;
        /* -123 */ s_hshErrNr2SysResultCode[EErrNrExponentTooLarge]                          = EResultUndefined;
        /* -124 */ s_hshErrNr2SysResultCode[EErrNrTooManyDigits]                             = EResultUndefined;
        /* -128 */ s_hshErrNr2SysResultCode[EErrNrNumericDataNotAllowed]                     = EResultUndefined;
        /* -130 */ s_hshErrNr2SysResultCode[EErrNrSuffixError]                               = EResultUndefined;
        /* -131 */ s_hshErrNr2SysResultCode[EErrNrInvalidSuffix]                             = EResultUndefined;
        /* -134 */ s_hshErrNr2SysResultCode[EErrNrSuffixTooLong]                             = EResultUndefined;
        /* -138 */ s_hshErrNr2SysResultCode[EErrNrSuffixNotAllowed]                          = EResultUndefined;
        /* -140 */ s_hshErrNr2SysResultCode[EErrNrCharacterDataError]                        = EResultUndefined;
        /* -141 */ s_hshErrNr2SysResultCode[EErrNrInvalidCharacterData]                      = EResultUndefined;
        /* -144 */ s_hshErrNr2SysResultCode[EErrNrCharacterDataTooLong]                      = EResultUndefined;
        /* -148 */ s_hshErrNr2SysResultCode[EErrNrCharacterDataNotAllowed]                   = EResultUndefined;
        /* -150 */ s_hshErrNr2SysResultCode[EErrNrStringDataError]                           = EResultUndefined;
        /* -151 */ s_hshErrNr2SysResultCode[EErrNrInvalidStringData]                         = EResultUndefined;
        /* -158 */ s_hshErrNr2SysResultCode[EErrNrStringDataNotAllowed]                      = EResultUndefined;
        /* -160 */ s_hshErrNr2SysResultCode[EErrNrBlockDataDrror]                            = EResultUndefined;
        /* -161 */ s_hshErrNr2SysResultCode[EErrNrInvalidBlockData]                          = EResultUndefined;
        /* -168 */ s_hshErrNr2SysResultCode[EErrNrBlockDataNotAllowed]                       = EResultUndefined;
        /* -170 */ s_hshErrNr2SysResultCode[EErrNrCommandExpressionError]                    = EResultUndefined;
        /* -171 */ s_hshErrNr2SysResultCode[EErrNrInvalidExpression]                         = EResultUndefined;
        /* -178 */ s_hshErrNr2SysResultCode[EErrNrExpressionDataNotAllowed]                  = EResultUndefined;
        /* -180 */ s_hshErrNr2SysResultCode[EErrNrCommandMacroError]                         = EResultUndefined;
        /* -181 */ s_hshErrNr2SysResultCode[EErrNrInvalidOutsideMacroDefinition]             = EResultUndefined;
        /* -183 */ s_hshErrNr2SysResultCode[EErrNrInvalidInsideMacroDefinition]              = EResultUndefined;
        /* -184 */ s_hshErrNr2SysResultCode[EErrNrCommandMacroParameterError]                = EResultUndefined;
        /* -200 */ s_hshErrNr2SysResultCode[EErrNrExecutionError]                            = EResultUndefined;
        /* -201 */ s_hshErrNr2SysResultCode[EErrNrInvalidWhileInLocal]                       = EResultUndefined;
        /* -202 */ s_hshErrNr2SysResultCode[EErrNrSettingsLostDueToRtl]                      = EResultUndefined;
        /* -203 */ s_hshErrNr2SysResultCode[EErrNrCommandProtected]                          = EResultUndefined;
        /* -210 */ s_hshErrNr2SysResultCode[EErrNrTriggerError]                              = EResultUndefined;
        /* -211 */ s_hshErrNr2SysResultCode[EErrNrTriggerIgnored]                            = EResultUndefined;
        /* -212 */ s_hshErrNr2SysResultCode[EErrNrArmIgnored]                                = EResultUndefined;
        /* -213 */ s_hshErrNr2SysResultCode[EErrNrInitIgnored]                               = EResultUndefined;
        /* -214 */ s_hshErrNr2SysResultCode[EErrNrTriggerDeadlock]                           = EResultUndefined;
        /* -215 */ s_hshErrNr2SysResultCode[EErrNrArmDeadlock]                               = EResultUndefined;
        /* -220 */ s_hshErrNr2SysResultCode[EErrNrParameterError]                            = EResultUndefined;
        /* -221 */ s_hshErrNr2SysResultCode[EErrNrSettingsConflict]                          = EResultUndefined;
        /* -222 */ s_hshErrNr2SysResultCode[EErrNrDataOutOfRange]                            = EResultUndefined;
        /* -223 */ s_hshErrNr2SysResultCode[EErrNrTooMuchData]                               = EResultUndefined;
        /* -224 */ s_hshErrNr2SysResultCode[EErrNrIllegalParameterValue]                     = EResultUndefined;
        /* -225 */ s_hshErrNr2SysResultCode[EErrNrOutOfMemory]                               = EResultUndefined;
        /* -226 */ s_hshErrNr2SysResultCode[EErrNrListsNotSameLength]                        = EResultUndefined;
        /* -230 */ s_hshErrNr2SysResultCode[EErrNrDataCorruptOrStale]                        = EResultUndefined;
        /* -231 */ s_hshErrNr2SysResultCode[EErrNrDataQuestionable]                          = EResultUndefined;
        /* -232 */ s_hshErrNr2SysResultCode[EErrNrInvalidFormat]                             = EResultUndefined;
        /* -233 */ s_hshErrNr2SysResultCode[EErrNrInvalidVersion]                            = EResultUndefined;
        /* -240 */ s_hshErrNr2SysResultCode[EErrNrHardwareError]                             = EResultUndefined;
        /* -241 */ s_hshErrNr2SysResultCode[EErrNrHardwareMissing]                           = EResultUndefined;
        /* -250 */ s_hshErrNr2SysResultCode[EErrNrMassStorageError]                          = EResultUndefined;
        /* -251 */ s_hshErrNr2SysResultCode[EErrNrMissingMassStorage]                        = EResultUndefined;
        /* -252 */ s_hshErrNr2SysResultCode[EErrNrMissingMedia]                              = EResultUndefined;
        /* -253 */ s_hshErrNr2SysResultCode[EErrNrCorruptMedia]                              = EResultUndefined;
        /* -254 */ s_hshErrNr2SysResultCode[EErrNrMediaFull]                                 = EResultUndefined;
        /* -255 */ s_hshErrNr2SysResultCode[EErrNrDirectoryFull]                             = EResultUndefined;
        /* -256 */ s_hshErrNr2SysResultCode[EErrNrFileNameNotFound]                          = EResultUndefined;
        /* -257 */ s_hshErrNr2SysResultCode[EErrNrFileNameError]                             = EResultUndefined;
        /* -258 */ s_hshErrNr2SysResultCode[EErrNrMediaProtected]                            = EResultUndefined;
        /* -260 */ s_hshErrNr2SysResultCode[EErrNrExecutionExpressionError]                  = EResultUndefined;
        /* -261 */ s_hshErrNr2SysResultCode[EErrNrMathErrorInExpression]                     = EResultUndefined;
        /* -270 */ s_hshErrNr2SysResultCode[EErrNrExecutionMacroError]                       = EResultUndefined;
        /* -271 */ s_hshErrNr2SysResultCode[EErrNrMacroSyntaxError]                          = EResultUndefined;
        /* -272 */ s_hshErrNr2SysResultCode[EErrNrMacroExecutionError]                       = EResultUndefined;
        /* -273 */ s_hshErrNr2SysResultCode[EErrNrIllegalMacroLabel]                         = EResultUndefined;
        /* -274 */ s_hshErrNr2SysResultCode[EErrNrExecutionMacroParameterError]              = EResultUndefined;
        /* -275 */ s_hshErrNr2SysResultCode[EErrNrMacroDefinitionTooLong]                    = EResultUndefined;
        /* -276 */ s_hshErrNr2SysResultCode[EErrNrMacroRecursionArror]                       = EResultUndefined;
        /* -277 */ s_hshErrNr2SysResultCode[EErrNrMacroRedefinitionNotAllowed]               = EResultUndefined;
        /* -278 */ s_hshErrNr2SysResultCode[EErrNrMacroHeaderNotFound]                       = EResultUndefined;
        /* -280 */ s_hshErrNr2SysResultCode[EErrNrProgramError]                              = EResultUndefined;
        /* -281 */ s_hshErrNr2SysResultCode[EErrNrCannotCreateProgram]                       = EResultUndefined;
        /* -282 */ s_hshErrNr2SysResultCode[EErrNrIllegalProgramName]                        = EResultUndefined;
        /* -283 */ s_hshErrNr2SysResultCode[EErrNrIllegalVariableName]                       = EResultUndefined;
        /* -284 */ s_hshErrNr2SysResultCode[EErrNrProgramCurrentlyRunning]                   = EResultUndefined;
        /* -285 */ s_hshErrNr2SysResultCode[EErrNrProgramSyntaxError]                        = EResultUndefined;
        /* -286 */ s_hshErrNr2SysResultCode[EErrNrProgramRuntimeError]                       = EResultUndefined;
        /* -290 */ s_hshErrNr2SysResultCode[EErrNrMemoryUseError]                            = EResultUndefined;
        /* -292 */ s_hshErrNr2SysResultCode[EErrNrReferencedNameDoesNotExist]                = EResultUndefined;
        /* -293 */ s_hshErrNr2SysResultCode[EErrNrReferencedNameAlreadyExists]               = EResultUndefined;
        /* -294 */ s_hshErrNr2SysResultCode[EErrNrIncompatibleType]                          = EResultUndefined;
        /* -300 */ s_hshErrNr2SysResultCode[EErrNrDeviceSpecificError]                       = EResultUndefined;
        /* -310 */ s_hshErrNr2SysResultCode[EErrNrSystemError]                               = EResultUndefined;
        /* -311 */ s_hshErrNr2SysResultCode[EErrNrMemoryError]                               = EResultUndefined;
        /* -312 */ s_hshErrNr2SysResultCode[EErrNrPUDMemoryLost]                             = EResultUndefined;
        /* -313 */ s_hshErrNr2SysResultCode[EErrNrCalibrationMemoryLost]                     = EResultUndefined;
        /* -314 */ s_hshErrNr2SysResultCode[EErrNrSaveRecallMemoryLost]                      = EResultUndefined;
        /* -315 */ s_hshErrNr2SysResultCode[EErrNrConfigurationMemoryLost]                   = EResultUndefined;
        /* -320 */ s_hshErrNr2SysResultCode[EErrNrStorageFault]                              = EResultUndefined;
        /* -330 */ s_hshErrNr2SysResultCode[EErrNrSelfTestFailed]                            = EResultUndefined;
        /* -340 */ s_hshErrNr2SysResultCode[EErrNrCalibrationFailed]                         = EResultUndefined;
        /* -350 */ s_hshErrNr2SysResultCode[EErrNrQueueOverflow]                             = EResultUndefined;
        /* -360 */ s_hshErrNr2SysResultCode[EErrNrCommunicationError]                        = EResultUndefined;
        /* -361 */ s_hshErrNr2SysResultCode[EErrNrParityErrorInProgramMessage]               = EResultUndefined;
        /* -362 */ s_hshErrNr2SysResultCode[EErrNrFramingErrorInProgramMessage]              = EResultUndefined;
        /* -363 */ s_hshErrNr2SysResultCode[EErrNrInputBufferOverrun]                        = EResultUndefined;
        /* -365 */ s_hshErrNr2SysResultCode[EErrNrTimeOutError]                              = EResultUndefined;
        /* -400 */ s_hshErrNr2SysResultCode[EErrNrQueryError]                                = EResultUndefined;
        /* -410 */ s_hshErrNr2SysResultCode[EErrNrQueryInterrupted]                          = EResultUndefined;
        /* -420 */ s_hshErrNr2SysResultCode[EErrNrQueryUnterminated]                         = EResultUndefined;
        /* -430 */ s_hshErrNr2SysResultCode[EErrNrQueryDeadlocked]                           = EResultUndefined;
        /* -440 */ s_hshErrNr2SysResultCode[EErrNrQueryUnterminatedAfterIndefiniteResponse]  = EResultUndefined;
        /* -500 */ s_hshErrNr2SysResultCode[EErrNrPowerOn]                                   = EResultUndefined;
        /* -600 */ s_hshErrNr2SysResultCode[EErrNrUserRequest]                               = EResultUndefined;
        /* -700 */ s_hshErrNr2SysResultCode[EErrNrRequestControl]                            = EResultUndefined;
        /* -800 */ s_hshErrNr2SysResultCode[EErrNrOperationComplete]                         = EResultUndefined;

        /*    0 */ s_hshSysResultCode2ErrNr[EResultSuccess]                             = EErrNrUndefined;
        /*    1 */ s_hshSysResultCode2ErrNr[EResultSuccessIntermediateResults]          = EErrNrUndefined;
        /*    2 */ s_hshSysResultCode2ErrNr[EResultInvalid]                             = EErrNrUndefined;
        /*    3 */ s_hshSysResultCode2ErrNr[EResultUndefined]                           = EErrNrUndefined;
        /*    4 */ s_hshSysResultCode2ErrNr[EResultError]                               = EErrNrUndefined;
        /*    5 */ s_hshSysResultCode2ErrNr[EResultUnknownError]                        = EErrNrUndefined;
        /*    6 */ s_hshSysResultCode2ErrNr[EResultUnknownException]                    = EErrNrUndefined;
        /*    7 */ s_hshSysResultCode2ErrNr[EResultUnexpected]                          = EErrNrUndefined;
        /*    8 */ s_hshSysResultCode2ErrNr[EResultInternalProgramError]                = EErrNrUndefined;
        /*    9 */ s_hshSysResultCode2ErrNr[EResultInternalStateMachineError]           = EErrNrUndefined;
        /*   10 */ s_hshSysResultCode2ErrNr[EResultSignalSlotConnectionFailed]          = EErrNrUndefined;
        /*   11 */ s_hshSysResultCode2ErrNr[EResultMemLeak]                             = EErrNrUndefined;
        /*   12 */ s_hshSysResultCode2ErrNr[EResultOutOfMemory]                         = EErrNrUndefined;
        /*   13 */ s_hshSysResultCode2ErrNr[EResultArgOutOfRange]                       = EErrNrUndefined;
        /*   14 */ s_hshSysResultCode2ErrNr[EResultIdxOutOfRange]                       = EErrNrUndefined;
        /*   15 */ s_hshSysResultCode2ErrNr[EResultTimeout]                             = EErrNrUndefined;
        /*   16 */ s_hshSysResultCode2ErrNr[EResultListIsEmpty]                         = EErrNrUndefined;
        /*   17 */ s_hshSysResultCode2ErrNr[EResultListNotEmpty]                        = EErrNrUndefined;
        /*   18 */ s_hshSysResultCode2ErrNr[EResultListIsFull]                          = EErrNrUndefined;
        /*   19 */ s_hshSysResultCode2ErrNr[EResultListCorrupted]                       = EErrNrUndefined;
        /*   20 */ s_hshSysResultCode2ErrNr[EResultEndOfList]                           = EErrNrUndefined;
        /*   21 */ s_hshSysResultCode2ErrNr[EResultObjNotInList]                        = EErrNrUndefined;
        /*   22 */ s_hshSysResultCode2ErrNr[EResultObjAlreadyInList]                    = EErrNrUndefined;
        /*   23 */ s_hshSysResultCode2ErrNr[EResultObjRefCounterIsZero]                 = EErrNrUndefined;
        /*   24 */ s_hshSysResultCode2ErrNr[EResultObjRefCounterIsNotZero]              = EErrNrUndefined;
        /*   25 */ s_hshSysResultCode2ErrNr[EResultInvalidObjId]                        = EErrNrUndefined;
        /*   26 */ s_hshSysResultCode2ErrNr[EResultInvalidObjName]                      = EErrNrUndefined;
        /*   27 */ s_hshSysResultCode2ErrNr[EResultInvalidObjGUID]                      = EErrNrUndefined;
        /*   28 */ s_hshSysResultCode2ErrNr[EResultInvalidObjType]                      = EErrNrUndefined;
        /*   29 */ s_hshSysResultCode2ErrNr[EResultInvalidObjAddress]                   = EErrNrUndefined;
        /*   30 */ s_hshSysResultCode2ErrNr[EResultObjNameNotUnique]                    = EErrNrUndefined;
        /*   31 */ s_hshSysResultCode2ErrNr[EResultObjCreateFailed]                     = EErrNrUndefined;
        /*   32 */ s_hshSysResultCode2ErrNr[EResultObjDestroyFailed]                    = EErrNrUndefined;
        /*   33 */ s_hshSysResultCode2ErrNr[EResultObjActivateFailed]                   = EErrNrUndefined;
        /*   34 */ s_hshSysResultCode2ErrNr[EResultObjDeactivateFailed]                 = EErrNrUndefined;
        /*   35 */ s_hshSysResultCode2ErrNr[EResultObjRegisterFailed]                   = EErrNrUndefined;
        /*   36 */ s_hshSysResultCode2ErrNr[EResultObjUnregisterFailed]                 = EErrNrUndefined;
        /*   37 */ s_hshSysResultCode2ErrNr[EResultObjNotFound]                         = EErrNrUndefined;
        /*   38 */ s_hshSysResultCode2ErrNr[EResultObjNotDefined]                       = EErrNrUndefined;
        /*   39 */ s_hshSysResultCode2ErrNr[EResultObjNotRegistered]                    = EErrNrUndefined;
        /*   40 */ s_hshSysResultCode2ErrNr[EResultObjAlreadyRegistered]                = EErrNrUndefined;
        /*   41 */ s_hshSysResultCode2ErrNr[EResultObjPoolNotDefined]                   = EErrNrUndefined;
        /*   42 */ s_hshSysResultCode2ErrNr[EResultObjPoolNotFound]                     = EErrNrUndefined;
        /*   43 */ s_hshSysResultCode2ErrNr[EResultObjFactoryNotDefined]                = EErrNrUndefined;
        /*   44 */ s_hshSysResultCode2ErrNr[EResultObjFactoryNotFound]                  = EErrNrUndefined;
        /*   45 */ s_hshSysResultCode2ErrNr[EResultNumberOverflow]                      = EErrNrUndefined;
        /*   46 */ s_hshSysResultCode2ErrNr[EResultDivisionByZero]                      = EErrNrUndefined;
        /*   47 */ s_hshSysResultCode2ErrNr[EResultInvalidDynamicTypeCast]              = EErrNrUndefined;
        /*   48 */ s_hshSysResultCode2ErrNr[EResultOptionNotSupported]                  = EErrNrUndefined;
        /*   49 */ s_hshSysResultCode2ErrNr[EResultOptionNotYetSupported]               = EErrNrUndefined;
        /*   50 */ s_hshSysResultCode2ErrNr[EResultMethodNotYetImplemented]             = EErrNrUndefined;
        /*   51 */ s_hshSysResultCode2ErrNr[EResultInvalidMethodCall]                   = EErrNrUndefined;
        /*   52 */ s_hshSysResultCode2ErrNr[EResultInitModuleFailed]                    = EErrNrUndefined;
        /*   53 */ s_hshSysResultCode2ErrNr[EResultFinalizeModuleFailed]                = EErrNrUndefined;
        /*   54 */ s_hshSysResultCode2ErrNr[EResultModuleNotInitialised]                = EErrNrUndefined;
        /*   55 */ s_hshSysResultCode2ErrNr[EResultModuleAlreadyInitialised]            = EErrNrUndefined;
        /*   56 */ s_hshSysResultCode2ErrNr[EResultInitClassFailed]                     = EErrNrUndefined;
        /*   57 */ s_hshSysResultCode2ErrNr[EResultFinalizeClassFailed]                 = EErrNrUndefined;
        /*   58 */ s_hshSysResultCode2ErrNr[EResultClassNotInitialised]                 = EErrNrUndefined;
        /*   59 */ s_hshSysResultCode2ErrNr[EResultClassAlreadyInitialised]             = EErrNrUndefined;
        /*   60 */ s_hshSysResultCode2ErrNr[EResultSingletonClassNotInstantiated]       = EErrNrUndefined;
        /*   61 */ s_hshSysResultCode2ErrNr[EResultSingletonClassAlreadyInstantiated]   = EErrNrUndefined;
        /*   62 */ s_hshSysResultCode2ErrNr[EResultUnexpectedMessage]                   = EErrNrUndefined;
        /*   63 */ s_hshSysResultCode2ErrNr[EResultMessageTypeMismatch]                 = EErrNrUndefined;
        /*   64 */ s_hshSysResultCode2ErrNr[EResultMessageStateMismatch]                = EErrNrUndefined;
        /*   65 */ s_hshSysResultCode2ErrNr[EResultInvalidMessage]                      = EErrNrUndefined;
        /*   66 */ s_hshSysResultCode2ErrNr[EResultInvalidMessageContent]               = EErrNrUndefined;
        /*   67 */ s_hshSysResultCode2ErrNr[EResultInvalidSenderObjInMessage]           = EErrNrUndefined;
        /*   68 */ s_hshSysResultCode2ErrNr[EResultInvalidReceiverObjInMessage]         = EErrNrUndefined;
        /*   69 */ s_hshSysResultCode2ErrNr[EResultMethodStateMismatch]                 = EErrNrUndefined;
        /*   70 */ s_hshSysResultCode2ErrNr[EResultRequestRefused]                      = EErrNrUndefined;
        /*   71 */ s_hshSysResultCode2ErrNr[EResultRequestAborted]                      = EErrNrUndefined;
        /*   72 */ s_hshSysResultCode2ErrNr[EResultRequestExecutionError]               = EErrNrUndefined;
        /*   73 */ s_hshSysResultCode2ErrNr[EResultUnfinishedOperation]                 = EErrNrUndefined;
        /*   74 */ s_hshSysResultCode2ErrNr[EResultInvalidFileName]                     = EErrNrUndefined;
        /*   75 */ s_hshSysResultCode2ErrNr[EResultFileNotFound]                        = EErrNrUndefined;
        /*   76 */ s_hshSysResultCode2ErrNr[EResultFileOpenError]                       = EErrNrUndefined;
        /*   77 */ s_hshSysResultCode2ErrNr[EResultFileOpenForWrite]                    = EErrNrUndefined;
        /*   78 */ s_hshSysResultCode2ErrNr[EResultFileOpenForRead]                     = EErrNrUndefined;
        /*   79 */ s_hshSysResultCode2ErrNr[EResultFileNotOpen]                         = EErrNrUndefined;
        /*   80 */ s_hshSysResultCode2ErrNr[EResultFileReadContent]                     = EErrNrUndefined;
        /*   81 */ s_hshSysResultCode2ErrNr[EResultFileWriteContent]                    = EErrNrUndefined;
        /*   82 */ s_hshSysResultCode2ErrNr[EResultSessionOpenError]                    = EErrNrUndefined;
        /*   83 */ s_hshSysResultCode2ErrNr[EResultSessionCloseError]                   = EErrNrUndefined;
        /*   84 */ s_hshSysResultCode2ErrNr[EResultSessionNotOpen]                      = EErrNrUndefined;
        /*   85 */ s_hshSysResultCode2ErrNr[EResultServerNotFound]                      = EErrNrUndefined;
        /*   86 */ s_hshSysResultCode2ErrNr[EResultMaxConnectionsExceeded]              = EErrNrUndefined;
        /*   87 */ s_hshSysResultCode2ErrNr[EResultSocketIdOutOfRange]                  = EErrNrUndefined;
        /*   88 */ s_hshSysResultCode2ErrNr[EResultSocketIdAlreadyUsed]                 = EErrNrUndefined;
        /*   89 */ s_hshSysResultCode2ErrNr[EResultConnectionRefused]                   = EErrNrUndefined;
        /*   90 */ s_hshSysResultCode2ErrNr[EResultConnectionError]                     = EErrNrUndefined;
        /*   91 */ s_hshSysResultCode2ErrNr[EResultNoConnection]                        = EErrNrUndefined;
        /*   92 */ s_hshSysResultCode2ErrNr[EResultRemoteHostClosed]                    = EErrNrUndefined;
        /*   93 */ s_hshSysResultCode2ErrNr[EResultHostNotFound]                        = EErrNrUndefined;
        /*   94 */ s_hshSysResultCode2ErrNr[EResultSocketAccessError]                   = EErrNrUndefined;
        /*   95 */ s_hshSysResultCode2ErrNr[EResultSocketResourceError]                 = EErrNrUndefined;
        /*   96 */ s_hshSysResultCode2ErrNr[EResultSocketTimeout]                       = EErrNrUndefined;
        /*   97 */ s_hshSysResultCode2ErrNr[EResultSocketProtocolError]                 = EErrNrUndefined;
        /*   98 */ s_hshSysResultCode2ErrNr[EResultDatagramTooLarge]                    = EErrNrUndefined;
        /*   99 */ s_hshSysResultCode2ErrNr[EResultNetworkError]                        = EErrNrUndefined;
        /*  100 */ s_hshSysResultCode2ErrNr[EResultAddressAlreadyInUse]                 = EErrNrUndefined;
        /*  101 */ s_hshSysResultCode2ErrNr[EResultSocketAddressNotAvailable]           = EErrNrUndefined;
        /*  102 */ s_hshSysResultCode2ErrNr[EResultUnsupportedSocketOperation]          = EErrNrUndefined;
        /*  103 */ s_hshSysResultCode2ErrNr[EResultUnfinishedSocketOperation]           = EErrNrUndefined;
        /*  104 */ s_hshSysResultCode2ErrNr[EResultProxyAuthenticationRequired]         = EErrNrUndefined;
        /*  105 */ s_hshSysResultCode2ErrNr[EResultSslHandshakeFailed]                  = EErrNrUndefined;
        /*  106 */ s_hshSysResultCode2ErrNr[EResultProxyConnectionRefused]              = EErrNrUndefined;
        /*  107 */ s_hshSysResultCode2ErrNr[EResultProxyConnectionClosed]               = EErrNrUndefined;
        /*  108 */ s_hshSysResultCode2ErrNr[EResultProxyTimeout]                        = EErrNrUndefined;
        /*  109 */ s_hshSysResultCode2ErrNr[EResultProxyNotFound]                       = EErrNrUndefined;
        /*  110 */ s_hshSysResultCode2ErrNr[EResultProxyProtocol]                       = EErrNrUndefined;
        /*  111 */ s_hshSysResultCode2ErrNr[EResultReadyReadTimeout]                    = EErrNrUndefined;
        /*  112 */ s_hshSysResultCode2ErrNr[EResultTooMuchData]                         = EErrNrUndefined;
        /*  113 */ s_hshSysResultCode2ErrNr[EResultStringParseError]                    = EErrNrUndefined;
        /*  114 */ s_hshSysResultCode2ErrNr[EResultPermissionDenied]                    = EErrNrUndefined;
        /*  115 */ s_hshSysResultCode2ErrNr[EResultSIUnitNotDefined]                    = EErrNrUndefined;
        /*  116 */ s_hshSysResultCode2ErrNr[EResultPhysSizeNotInitialized]              = EErrNrUndefined;
        /*  117 */ s_hshSysResultCode2ErrNr[EResultDifferentPhysSizes]                  = EErrNrUndefined;
        /*  118 */ s_hshSysResultCode2ErrNr[EResultInvalidPhysSize]                     = EErrNrUndefined;
        /*  119 */ s_hshSysResultCode2ErrNr[EResultInvalidPhysUnit]                     = EErrNrUndefined;
        /*  120 */ s_hshSysResultCode2ErrNr[EResultInvalidRefVal]                       = EErrNrUndefined;
        /*  121 */ s_hshSysResultCode2ErrNr[EResultFctConvertInvalid]                   = EErrNrUndefined;
        /*  122 */ s_hshSysResultCode2ErrNr[EResultFctConvertNotDefined]                = EErrNrUndefined;
        /*  123 */ s_hshSysResultCode2ErrNr[EResultConversionFailed]                    = EErrNrUndefined;
        /*  124 */ s_hshSysResultCode2ErrNr[EResultMixedLinLogInValAndRes]              = EErrNrUndefined;
        /*  125 */ s_hshSysResultCode2ErrNr[EResultMixedLinLogInMathOp]                 = EErrNrUndefined;
        /*  126 */ s_hshSysResultCode2ErrNr[EResultInvalidValue]                        = EErrNrUndefined;
        /*  127 */ s_hshSysResultCode2ErrNr[EResultNotConvertible]                      = EErrNrUndefined;
        /*  128 */ s_hshSysResultCode2ErrNr[EResultInvalidSIUnit]                       = EErrNrUndefined;
        /*  129 */ s_hshSysResultCode2ErrNr[EResultPhysSizeNotChangeable]               = EErrNrUndefined;
        /*  130 */ s_hshSysResultCode2ErrNr[EResultRefValMissing]                       = EErrNrUndefined;
        /*  131 */ s_hshSysResultCode2ErrNr[EResultInvalidRefValUnit]                   = EErrNrUndefined;
        /*  132 */ s_hshSysResultCode2ErrNr[EResultLogarithmicUnitExpected]             = EErrNrUndefined;
        /*  133 */ s_hshSysResultCode2ErrNr[EResultDimensionOutOfRange]                 = EErrNrUndefined;
        /*  134 */ s_hshSysResultCode2ErrNr[EResultValIdxOutOfRange]                    = EErrNrUndefined;
        /*  135 */ s_hshSysResultCode2ErrNr[EResultValueOutOfRange]                     = EErrNrUndefined;
        /*  136 */ s_hshSysResultCode2ErrNr[EResultValueExceedsMinLimit]                = EErrNrUndefined;
        /*  137 */ s_hshSysResultCode2ErrNr[EResultValueExceedsMaxLimit]                = EErrNrUndefined;
        /*  138 */ s_hshSysResultCode2ErrNr[EResultNoValueAssigned]                     = EErrNrUndefined;
        /*  139 */ s_hshSysResultCode2ErrNr[EResultNullValueNotAllowed]                 = EErrNrUndefined;
        /*  140 */ s_hshSysResultCode2ErrNr[EResultValueCorrected]                      = EErrNrUndefined;
        /*  141 */ s_hshSysResultCode2ErrNr[EResultInvalidDataType]                     = EErrNrUndefined;
        /*  142 */ s_hshSysResultCode2ErrNr[EResultUnknownDataType]                     = EErrNrUndefined;
        /*  143 */ s_hshSysResultCode2ErrNr[EResultUnsupportedDataType]                 = EErrNrUndefined;
        /*  144 */ s_hshSysResultCode2ErrNr[EResultPrimaryKeyConstraintViolation]       = EErrNrUndefined;
        /*  145 */ s_hshSysResultCode2ErrNr[EResultUniqueConstraintViolation]           = EErrNrUndefined;
        /*  146 */ s_hshSysResultCode2ErrNr[EResultForeignKeyConstraintViolation]       = EErrNrUndefined;
        /*  147 */ s_hshSysResultCode2ErrNr[EResultStatementError]                      = EErrNrUndefined;
        /*  148 */ s_hshSysResultCode2ErrNr[EResultTransactionError]                    = EErrNrUndefined;

        s_bErrNrMapsInitialised = true;

    } // if( !s_bErrNrMapsInitialised )

} // initErrNrMaps

//------------------------------------------------------------------------------
EResult ZS::RemCmd::errNr2SysResultCode( int i_iErrNr )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    EResult result = EResultUndefined;

    return s_hshErrNr2SysResultCode.value(static_cast<EErrNr>(i_iErrNr), EResultUndefined);

    return result;

} // errNr2SysResultCode

//------------------------------------------------------------------------------
SErrResult ZS::RemCmd::errNr2SysErrResult( int i_iErrNr )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    EResult result = errNr2SysResultCode(i_iErrNr);

    return SErrResult(EResultSeverityError, result);

} // errNr2SysErrResult

//------------------------------------------------------------------------------
SErrResultInfo ZS::RemCmd::errNr2SysErrResultInfo( int i_iErrNr )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    SErrResult errResult = errNr2SysErrResult(i_iErrNr);

    SErrResultInfo errResultInfo(errResult);

    int idxEnumEntry = s_hshErrNr2EnumEntryIdx.value(static_cast<EErrNr>(i_iErrNr), -1);

    if( idxEnumEntry >= 0 )
    {
        const SEnumEntry& enumEntry = s_arErrNrEnumEntries[idxEnumEntry];
        QString strAddErrInfo = QString::number(i_iErrNr) + ": " + enumEntry.m_strlstNames[EErrNrAliasStrText];
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }

    return errResultInfo;

} // errNr2SysErrResultInfo

//------------------------------------------------------------------------------
EErrNr ZS::RemCmd::sysResultCode2ErrNr( EResult i_result )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }
    return s_hshSysResultCode2ErrNr.value(i_result, EErrNrUndefined);
}

//------------------------------------------------------------------------------
EErrNr ZS::RemCmd::sysErrResult2ErrNr( const SErrResult& i_errResult )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }
    return s_hshSysResultCode2ErrNr.value(i_errResult.getResult(), EErrNrUndefined);
}

//------------------------------------------------------------------------------
EErrNr ZS::RemCmd::sysErrResultInfo2ErrNr( const SErrResultInfo& i_errResultInfo )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }
    return s_hshSysResultCode2ErrNr.value(i_errResultInfo.getResult(), EErrNrUndefined);
}

//------------------------------------------------------------------------------
QString ZS::RemCmd::errNr2Str( int i_iErrNr, EErrNrAliasStr i_alias )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    QString str;

    int idxEnumEntry = s_hshErrNr2EnumEntryIdx.value(static_cast<EErrNr>(i_iErrNr), -1);

    if( idxEnumEntry >= 0 )
    {
        const SEnumEntry& enumEntry = s_arErrNrEnumEntries[idxEnumEntry];
        str = QString::number(i_iErrNr) + ": " + enumEntry.getName(i_alias);
    }
    else
    {
        str = "? (" + QString::number(i_iErrNr) + ")";
    }

    return str;

} // errNr2Str

//------------------------------------------------------------------------------
EErrNr ZS::RemCmd::str2ErrNr( const QString& i_strErrNr )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    EErrNr errNr = EErrNrUndefined;

    if( s_hshStrEnumName2ErrNr.contains(i_strErrNr) )
    {
        errNr = s_hshStrEnumName2ErrNr[i_strErrNr];
    }
    else if( s_hshStrEnumText2ErrNr.contains(i_strErrNr) )
    {
        errNr = s_hshStrEnumText2ErrNr[i_strErrNr];
    }

    return errNr;

} // str2ErrNr

//------------------------------------------------------------------------------
QByteArray ZS::RemCmd::errNr2ByteArr( int i_iErrNr, EErrNrAliasStr i_alias )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    QByteArray byteArr;

    int idxEnumEntry = s_hshErrNr2EnumEntryIdx.value(static_cast<EErrNr>(i_iErrNr), -1);

    if( idxEnumEntry >= 0 )
    {
        const SEnumEntry& enumEntry = s_arErrNrEnumEntries[idxEnumEntry];
        byteArr = QByteArray::number(i_iErrNr) + ": " + enumEntry.getName(i_alias).toStdString().c_str();
    }
    else
    {
        byteArr = "? (" + QByteArray::number(i_iErrNr) + ")";
    }

    return byteArr;

} // errNr2ByteArr

//------------------------------------------------------------------------------
EErrNr ZS::RemCmd::byteArr2ErrNr( const QByteArray& i_byteArrErrNr )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    EErrNr errNr = EErrNrUndefined;

    if( s_hshByteArrEnumName2ErrNr.contains(i_byteArrErrNr) )
    {
        errNr = s_hshByteArrEnumName2ErrNr[i_byteArrErrNr];
    }
    else if( s_hshByteArrEnumText2ErrNr.contains(i_byteArrErrNr) )
    {
        errNr = s_hshByteArrEnumText2ErrNr[i_byteArrErrNr];
    }

    return errNr;

} // byteArr2ErrNr

//------------------------------------------------------------------------------
QString ZS::RemCmd::errResultInfo2Str( const SErrResultInfo& i_errResultInfo, EErrNrAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return errResultInfo2ByteArr(i_errResultInfo, i_alias);
}

//------------------------------------------------------------------------------
SErrResultInfo ZS::RemCmd::str2ErrResultInfo( const QString& i_str )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    SErrResultInfo errResultInfo;

    int     iUserErrCode = EErrNrExecutionError;
    QString strUserErrCode;
    int     idxPos;
    bool    bConverted;

    QString strTmp = i_str;

    if( strTmp.startsWith("SYST:ERR",Qt::CaseInsensitive) )
    {
        strTmp.remove(0, 8);
        strTmp = strTmp.simplified();
    }

    //                 1         2         3         4         5
    //       012345678901234567890123456789012345678901234567890123456789
    // E.g. "-108, "Parameter not allowed; (Invalid number of arguments)""

    idxPos = strTmp.indexOf(',');                               // = 4

    if( idxPos > 0 )
    {
        strUserErrCode = strTmp.left(idxPos);                  // = "-108"
        iUserErrCode   = strUserErrCode.toInt(&bConverted);   // =  -108

        if( bConverted )
        {
            errResultInfo = errNr2SysErrResultInfo(iUserErrCode);
        }

        strTmp = strTmp.right(strTmp.size()-idxPos-1);          // " "Parameter not allowed; (Invalid number of arguments)""
        strTmp = strTmp.simplified();                           // ""Parameter not allowed; (Invalid number of arguments)""

        if( strTmp.startsWith('"') )
        {
            strTmp.remove(0, 1);                                // "Parameter not allowed; (Invalid number of arguments)""
        }
        if( strTmp.endsWith('"') )                              //            1         2         3         4         5
        {                                                       //  0123456789012345678901234567890123456789012345678901
            strTmp.remove(strTmp.size()-1, 1);                  // "Parameter not allowed; (Invalid number of arguments)"
        }

        errResultInfo.setAddErrInfoDscr(strTmp);

    } // if( idxPos > 0 )

    return errResultInfo;

} // str2ErrResultInfo

//------------------------------------------------------------------------------
QByteArray ZS::RemCmd::errResultInfo2ByteArr( const SErrResultInfo& i_errResultInfo, EErrNrAliasStr /*i_alias*/ )
//------------------------------------------------------------------------------
{
    if( !s_bErrNrMapsInitialised )
    {
        initErrNrMaps();
    }

    EErrNr errNr = sysErrResultInfo2ErrNr(i_errResultInfo);

    QByteArray byteArrErrResultInfo;

    byteArrErrResultInfo  = QByteArray::number(errNr);
    byteArrErrResultInfo += ", \"";
    byteArrErrResultInfo += errNr2ByteArr(errNr, EErrNrAliasStrText);
    byteArrErrResultInfo += "\"";

    return byteArrErrResultInfo;

} // errResultInfo2ByteArr

//------------------------------------------------------------------------------
SErrResultInfo ZS::RemCmd::byteArr2ErrResultInfo( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    return str2ErrResultInfo(i_byteArr);
}
