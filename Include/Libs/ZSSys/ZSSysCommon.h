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

#ifndef ZSSys_Common_h
#define ZSSys_Common_h

#include <QtCore/qstring.h>

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumTemplate.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

#ifndef INT8_MIN
#define INT8_MIN     (-127 - 1)                  /* minimum signed 8 bit value */
#endif
#ifndef INT8_MAX
#define INT8_MAX       127                       /* maximum signed 8 bit value */
#endif
#ifndef UINT8_MAX
#define UINT8_MAX      0xff                      /* maximum unsigned 8 bit value */
#endif

#ifndef INT16_MIN
#define INT16_MIN    (-32767 - 1)                /* minimum signed 16 bit value */
#endif
#ifndef INT16_MAX
#define INT16_MAX      32767                     /* maximum signed 16 bit value */
#endif
#ifndef UINT16_MAX
#define UINT16_MAX     0xffff                    /* maximum unsigned 16 bit value */
#endif

#ifndef INT32_MAX
#define INT32_MAX    (-2147483647 - 1)           /* minimum signed 32 bit value */
#endif
#ifndef INT32_MAX
#define INT32_MAX      2147483647                /* maximum signed 32 bit value */
#endif
#ifndef UINT32_MAX
#define UINT32_MAX     0xffffffff                /* maximum unsigned 32 bit value */
#endif

#ifndef INT64_MIN
#define INT64_MIN    (-9223372036854775807 - 1)  /* maximum signed 64 bit value */
#endif
#ifndef INT64_MAX
#define INT64_MAX      9223372036854775807       /* maximum unsigned 64 bit value */
#endif
#ifndef UINT64_MAX
#define UINT64_MAX     0xffffffffffffffff
#endif


namespace ZS
{
namespace System
{
// The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 )
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic pop
#endif

//==============================================================================
/*! A mode is often used to distinguish between different settings.

    This enumerations defines some possible mode pairs.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EMode
//==============================================================================
{
    EModeEdit       = 0, /*!< Indicates that the control is in Edit mode. */
    EModeDesign     = 0, /*!< Indicates that the control is in Design mode (alias for Edit mode). */
    EModeSimulation = 1, /*!< Indicates that the control is in Simulation mode. */
    EModeRun        = 1, /*!< Indicates that the control is in Run mode (alias for Simulation mode). */
    EModeView       = 1, /*!< Indicates that the control is in View mode (alias for Simulation mode). */
    EModeUndefined  = 2, /*!< Indicates that the control is in Undefined mode. */
    EModeIgnore     = 2  /*!< Indicates that the Mode parameter should be ignored (alias for Undefined mode). */
};

template class ZSSYSDLL_API CEnum<EMode>;
typedef CEnum<EMode> CEnumMode;


//==============================================================================
/*! The run mode enumeration is used to distinguish between two different run
    modes of a sequence.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ERunMode
//==============================================================================
{
    ERunModeContinuous = 0, /*! The sequence should run continuosly. */
    ERunModeSingleStep = 1, /*! The sequence should perform just one step. */
    ERunModeUndefined  = 2  /*! The run mode is not set and unknown. */
};

template class ZSSYSDLL_API CEnum<ERunMode>;
typedef CEnum<ERunMode> CEnumRunMode;


//==============================================================================
/*! "Yes" and "No" might be more readable to reflect a boolean value instead
    of "true" and "false".

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EYesNo
//==============================================================================
{
    EYesNoNo        = 0,  /*! Indicates that the option has not been chosen (alias for false). */
    EYesNoYes       = 1,  /*! Indicates that the option has been chosen (alias for true). */
    EYesNoUndefined = 2   /*! The variable is not yet initialised and its value is unknown. */
};

template class ZSSYSDLL_API CEnum<EYesNo>;
typedef CEnum<EYesNo> CEnumYesNo;

//==============================================================================
/*! "On" and "Off" might be more readable to reflect a boolean value instead
    of "true" and "false".

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EStateOnOff
//==============================================================================
{
    EStateOnOffOff       = 0,  /*! Indicates that the state is switched off (alias for false). */
    EStateOnOffOn        = 1,  /*! Indicates that the state is switched on (alias for true). */
    EStateOnOffUndefined = 2   /*! The variable is not yet initialised and its value is unknown. */
};

template class ZSSYSDLL_API CEnum<EStateOnOff>;
typedef CEnum<EStateOnOff> CEnumStateOnOff;


//==============================================================================
/*! Indicates whether an optional value is enabled or disabled.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EEnabled
//==============================================================================
{
    EEnabledNo        = 0, /*! Indicates that the option is disabled (alias for false). */
    EEnabledYes       = 1, /*! Indicates that the option is enabled (alias for true). */
    EEnabledUndefined = 2  /*! The variable is not yet initialised and its value is unknown. */
};

template class ZSSYSDLL_API CEnum<EEnabled>;
typedef CEnum<EEnabled> CEnumEnabled;


//==============================================================================
/*! When passing pointers as arguments to methods this enumeration defines how
    to handle the addressed memory. It may be possible just to reuse the pointer,
    to take the ownership of the pointer or to make a deep copy of the addressed space.

    There are essentially two different use cases for handling pointers.

    - When passing a pointer as an argument to a method (e.g. "target.setMessage(pMsg)")
      the enumeration is used to define whether

      - FlatKeepOwnerShip .. Just the pointer should be taken over. The source object
                     invoking setMessage is responsible to create and releasing the
                     addressed memory. The target object must rely on that the pointer
                     is accessible as long as needed.
      - ReleaseOwnership .. The pointer will be taken over. The source object
                     invoking setMessage is no longer responsible for releasing the
                     addressed memory. After calling setMessage the source object
                     should no longer access the pointer. The target object must
                     release the pointer it if no longer needed.
                     This option corresponds to the move semantic in C++11 standard.
      - Deep ....... The addresses memory will be cloned and both the source object and
                     target object use different pointers.

    - When querying a pointer from a source object (e.g. "source.getMessage()")
      the enumeration is used to define whether

      - FlatKeepOwnerShip .. The source object keeps ownership of the pointer.
                     The target object must rely on that the pointer is accessible
                     as long as needed.
      - ReleaseOwnership .. The pointer will be taken over. The source object
                     for which getMessage was called is no longer responsible for
                     releasing the addressed memory. After calling getMessage the
                     source object should no longer access the pointer.
                     The target object must release the pointer it if no longer needed.
                     This option corresponds to the move semantic in C++11 standard.
      - Deep ....... The addresses memory will be cloned and both the source object and
                     target object use different pointers.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ECopyDepth
//==============================================================================
{
    ECopyDepthFlatKeepOwnership     = 0, /*! The source object keeps the ownership of the pointer. */
    ECopyDepthFlatReleaseOwnwership = 1, /*! The source object releases the ownership of the pointer. */
    ECopyDepthDeep                  = 2  /*! The addressed memory will be cloned and both the source and target use their own and different pointers. */
};

template class ZSSYSDLL_API CEnum<ECopyDepth>;
typedef CEnum<ECopyDepth> CEnumCopyDepth;


//==============================================================================
/*! Defines the different life cycle states of an object.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EObjState
//==============================================================================
{
    EObjStateDetached   = 0, /*!< A created object may be set to this state to indicate that it has not been added to or has been removed from the target collection or has not yet been created. */
    EObjStateCreating   = 1, /*!< The object is being created. */
    EObjStateCreated    = 2, /*!< The object has been created. */
    EObjStateDestroying = 3, /*!< The object is beeing destroyed. */
    EObjStateDestroyed  = 4, /*!< The object has been destroyed. */
    EObjStateUndefined  = 5  /*!< The object state is not defined. */
};

template class ZSSYSDLL_API CEnum<EObjState>;
typedef CEnum<EObjState> CEnumObjState;


//==============================================================================
/*! Defines different row versions.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ERowVersion
//==============================================================================
{
    ERowVersionOriginal  = 0,  /*!< Original version before changing the value. */
    ERowVersionCurrent   = 1,  /*!< After editing a value the current version might be differet from the original value. */
    ERowVersionProposed  = 2,  /*!< When editing a value the proposed value may be differnt from the current version. */
    ERowVersionDefault   = 3,  /*!< Use either Original, Current or Proposed version depending on current state. */
    ERowVersionUndefined = 4   /*!< The row version is not defined. */
};

template class ZSSYSDLL_API CEnum<ERowVersion>;
typedef CEnum<ERowVersion> CEnumRowVersion;


//==============================================================================
/* This enumeration is used to indicate the state of values in lists.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ERowState
//==============================================================================
{
    ERowStateUnchanged = 0, /*!< After merging changes between different versions and accepting
                                 the changes the state is set to Unchanged. For Removed objects
                                 and DataRows accepting the changes means destroying the object. */
    ERowStateDetached  = 1, /*!< After initially creating the object the row state is set to
                                 Detached. The object is not added to its parent, rows don't
                                  belong to tables yet. */
    ERowStateAdded     = 2, /*!< After adding the object to its parent the row state is changed
                                 from Detached to Added. */
    ERowStateRemoved   = 3, /*!< After removing the object from its parent the row state is set
                                 to Removed. The object is still alive. On accepting the change
                                 the object or the data row got to be deleted. */
    ERowStateModified  = 4, /*!< On changing any property or value of an Unchanged object the
                                 state may be set to Modified. */
    ERowStateUndefined = 5  /*!< The row state is not defined. */
};

template class ZSSYSDLL_API CEnum<ERowState>;
typedef CEnum<ERowState> CEnumRowState;


//==============================================================================
/*! Defines the desired format of an string.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EContentToStrFormat
//==============================================================================
{
    EContentToStrFormatPlainText = 0,  /*!< Plain text without formatting characters. */
    EContentToStrFormatHtmlText  = 1,  /*!< Text containing HTML format strings. */
    EContentToStrFormatUndefined = 2   /*!< The desired format is not defined. */
};

template class ZSSYSDLL_API CEnum<EContentToStrFormat>;
typedef CEnum<EContentToStrFormat> CEnumContentToStrFormat;


//==============================================================================
/*! This enum is mainly used to define for what purpose trace outputs should
    be generated.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EMethodDir
//==============================================================================
{
    EMethodDirEnter     = 0,  /*!< The method has been entered (indicating usually with "->" in text output). */
    EMethodDirLeave     = 1,  /*!< The method will be left (indicating usually with "<-" in text output). */
    EMethodDirNone      = 2,  /*!< The method in somewhere in the middle (indicating usually with "  " in text output). */
    EMethodDirUndefined = 3   /*!< The direction is not defined. */
};

template class ZSSYSDLL_API CEnum<EMethodDir>;
typedef CEnum<EMethodDir> CEnumMethodDir;


//==============================================================================
/*! Defines whether a data stream is input or output.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ETransmitDir
//==============================================================================
{
    ETransmitDirReceive   = 0,  /*!< The data (message) is received. */
    ETransmitDirIn        = 0,  /*!< The data (message) is input (alias for Receive). */
    ETransmitDirRead      = 0,  /*!< The data (message) is read (alias for Receive). */
    ETransmitDirSend      = 1,  /*!< The data (message) is sent. */
    ETransmitDirOut       = 1,  /*!< The data (message) is output (alias for Send). */
    ETransmitDirWrite     = 1,  /*!< The data (message) is written (alias for Send). */
    ETransmitDirUndefined = 2   /*!< The transmission direction is undefined. */
};

template class ZSSYSDLL_API CEnum<ETransmitDir>;
typedef CEnum<ETransmitDir> CEnumTransmitDir;


//==============================================================================
/*! Defines whether a data stream is input or output.

    May be used if IODir better reflects the usage than TransmitDir.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EIODir
//==============================================================================
{
    EIODirIn        = 0,  /*!< The data (message) is input. */
    EIODirReceive   = 0,  /*!< The data (message) is received (alias for In). */
    EIODirGet       = 0,  /*!< The data (message) is get (alias for In). */
    EIODirRead      = 0,  /*!< The data (message) is read (alias for In). */
    EIODirOut       = 1,  /*!< The data (message) is output. */
    EIODirSend      = 1,  /*!< The data (message) is send (alias for Out). */
    EIODirSet       = 1,  /*!< The data (message) is set (alias for Out). */
    EIODirWrite     = 1,  /*!< The data (message) is written (alias for Out). */
    EIODirUndefined = 2   /*!< The IO direction is undefined. */
};

template class ZSSYSDLL_API CEnum<EIODir>;
typedef CEnum<EIODir> CEnumIODir;


//==============================================================================
/*! Defines the alignment of a direction.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EDirection
//==============================================================================
{
    EDirectionVertical   = 0, /*!< Vertically aligned. */
    EDirectionHorizontal = 1, /*!< Horizontally aligned. */
    EDirectionUndefined  = 2  /*!< The direction is undefined. */
};

template class ZSSYSDLL_API CEnum<EDirection>;
typedef CEnum<EDirection> CEnumDirection;


//==============================================================================
/*! Defines the comparison operator to be used.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EComparisonOperator
//==============================================================================
{
    EComparisonOperatorEqual     = 0,  /*!< To be compared for equality (==). */
    EComparisonOperatorUnequal   = 1,  /*!< To be compared for inequality (!=). */
    EComparisonOperatorUndefined = 2   /*!< Comparison operator not defined. */
};

template class ZSSYSDLL_API CEnum<EComparisonOperator>;
typedef CEnum<EComparisonOperator> CEnumComparisonOperator;


//==============================================================================
/*! Defines the validity of a value.

    @note A nullptr (null, DbNull, ..) value is a valid value.
          Considering QVariant an invalid value is created with the default ctor
          (creating a QVariant with type "QVariant::Invalid").
          A null QVariant is created by providing the type but not the value.
          E.g. QVariant(QVariant::Int) creates a valid null value of type int.
          The validity enumeration is used e.g. by class CPhysVal.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EValueValidity
//==============================================================================
{
    EValueValidityInvalid   = 0,  /*!< The value is invalid. */
    EValueValidityNull      = 1,  /*!< The value is valid but set to null. */
    EValueValidityValid     = 2,  /*!< The value is valid. */
    EValueValidityUndefined = 3   /*!< The validity is undefined. */
};

template class ZSSYSDLL_API CEnum<EValueValidity>;
typedef CEnum<EValueValidity> CEnumValueValidity;


//==============================================================================
/*! Defines the dimensin type of parameter arrays.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EDimensionType
//==============================================================================
{
    EDimensionTypeSingleValue     = 0,    /*!< No array just a single value. */
    EDimensionTypeOneDimensional  = 1,    /*!< An one dimensional array. */
    EDimensionTypeMoreDimensional = 2,    /*!< More dimensional arrays (an array of arrrays). */
    EDimensionTypeUndefined       = 3     /*!< The dimension type is undefined. */
};

template class ZSSYSDLL_API CEnum<EDimensionType>;
typedef CEnum<EDimensionType> CEnumDimensionType;

//#ifdef _WINDOWS
//#pragma warning( push )
//#pragma warning( disable : 4661 )
//#elif defined __linux__
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-result"
//#pragma GCC diagnostic pop
//#endif

// Negative values for array indices are invalid. Some of those negative values
// are used to indicate special ranges or special elements within the array.
const int EArrayIndexUndefined        = -1;
const int EArrayIndexFirstElement     = -2;
const int EArrayIndexLastElement      = -3;
const int EArrayIndexCountAllElements = -4;

ZSSYSDLL_API QString arrayIndex2Str( int i_idx, EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );

ZSSYSDLL_API bool isValidIndex( int i_idx, int i_iArrLen, bool i_bAllowSpecialArrayIndices = false ); // ok if idxVal >= 0 AND idxVal < ArrLen or idxVal = First|Last|All (if AllowSpecial=true)
ZSSYSDLL_API bool isValidRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, bool i_bAllowSpecialArrayIndices = false );
ZSSYSDLL_API int toArrayLen( int i_idxCount, int i_iArrLen, bool i_bLimitToArrLen = false );
ZSSYSDLL_API int toArrayIndex( int i_idx, int i_iArrLen, bool i_bLimitToArrLen = false );
ZSSYSDLL_API void getMinMaxArrayRange( int i_iArrIdx, int i_iArrLen, int* o_pIdxMin, int* o_pIdxMax );
ZSSYSDLL_API void getMinMaxArrayRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, int* o_pIdxMin, int* o_pIdxMax );
ZSSYSDLL_API void getMinCountArrayRange( int i_idxStart, int i_iArrLen, int* o_pIdxMin, int* o_piIdxCount );
ZSSYSDLL_API void getMinCountArrayRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, int* o_pIdxMin, int* o_pIdxCount );

} // namespace System

namespace Trace
{
#ifndef ETraceDetailLevelDefined
#define ETraceDetailLevelDefined
//==============================================================================
/*! Predefined trace detail levels.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.
    Please note that this enum is only a suggestion for trace detail levels
    which can be used. The detail level of the trace admin object is an integer
    type and user defined values can be used if necessary.
    To avoid type casts requested by the compiler this enum is not a class enum definition.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum ETraceDetailLevel
//==============================================================================
{
    ETraceDetailLevelUndefined      = -1,    /*!< Used e.g. to indicate that the trace level should not be used but the predefined detail level should be used. */
    ETraceDetailLevelNone           =  0,    /*!< Trace output is disabled. */
    ETraceDetailLevelMethodCalls    =  1,    /*!< Tracing method entries and leaves without tracing input and output arguments and without method return value. */
    ETraceDetailLevelMethodArgs     =  2,    /*!< Tracing method entries and leaves together with input and output arguemnts and method return value. */
    ETraceDetailLevelInternalStates =  3,    /*!< Tracing everything above and internal states (e.g. current state, current request in progress etc.. */
    ETraceDetailLevelRuntimeInfo    =  4,    /*!< Tracing everything above and additional runtime info (e.g. what is going to be done, what has been executed and how). */
    ETraceDetailLevelVerbose        =  5,    /*!< Tracing everything above and even more. */
    ETraceDetailLevelCount
};
#endif // #ifndef ETraceDetailLevelDefined

ZSSYSDLL_API QString traceDetailLevel2Str( int i_iDetailLevel, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );
ZSSYSDLL_API int str2TraceDetailLevel( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias = ZS::System::EEnumEntryAliasStrName );

#pragma warning( pop )

} // namespace Trace

} // namespace ZS

#endif // #ifndef ZSSys_Common_h
