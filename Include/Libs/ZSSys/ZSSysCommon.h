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

#ifndef ZSSys_Common_h
#define ZSSys_Common_h

#include "ZSSys/ZSSysDllMain.h"
#include "ZSSys/ZSSysEnumTemplate.h"

#include <QtCore/qstring.h>


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


// The static arrays "CEnum<>::s_arEnumEntries" are defined in the cpp file.
#ifdef _WINDOWS
#pragma warning( push )
#pragma warning( disable : 4661 )
#elif defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic pop
#endif

// Note about CEnum template instantiation:
// gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope"
// if the for example the template definition is encapsulated in namespace ZS::System.
// To work around this gcc compile error message the enum classes and the typedef for the
// enum classes are encapsulated in namespace ZS::System but the temlate instantiation is
// done in global namespace.

namespace ZS {
namespace System {
//==============================================================================
/*! A mode is often used to distinguish between different settings.

    This enumerations defines some possible mode pairs.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EMode
//==============================================================================
{
    Edit      = 0, /*!< Indicates that the control is in Edit mode. */
    View      = 1, /*!< Indicates that the control is in View mode. */
    Undefined = 2 /*!< Indicates that the control is in Undefined mode. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EMode>;

namespace ZS {
namespace System {
typedef CEnum<ZS::System::EMode> CEnumMode;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! The run mode enumeration is used to distinguish between two different run
    modes of a sequence.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ERunMode
//==============================================================================
{
    Continuous = 0, /*! The sequence should run continuosly. */
    SingleStep = 1, /*! The sequence should perform just one step. */
    Undefined  = 2  /*! The run mode is not set and unknown. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ERunMode>;
namespace ZS {
namespace System {
typedef CEnum<ERunMode> CEnumRunMode;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! "Yes" and "No" might be more readable to reflect a boolean value instead
    of "true" and "false".

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EYesNo
//==============================================================================
{
    No        = 0,  /*! Indicates that the option has not been chosen (alias for false). */
    Yes       = 1,  /*! Indicates that the option has been chosen (alias for true). */
    Undefined = 2   /*! The variable is not yet initialised and its value is unknown. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EYesNo>;
namespace ZS {
namespace System {
typedef CEnum<EYesNo> CEnumYesNo;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! "On" and "Off" might be more readable to reflect a boolean value instead
    of "true" and "false".

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EStateOnOff
//==============================================================================
{
    Off       = 0,  /*! Indicates that the state is switched off (alias for false). */
    On        = 1,  /*! Indicates that the state is switched on (alias for true). */
    Undefined = 2   /*! The variable is not yet initialised and its value is unknown. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EStateOnOff>;
namespace ZS {
namespace System {
typedef CEnum<EStateOnOff> CEnumStateOnOff;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Indicates whether an optional value is enabled or disabled.

    @ingroup _GRP_Namespace_ZS_System_Enumerations_Enumerations

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EEnabled
//==============================================================================
{
    No        = 0, /*! Indicates that the option is disabled (alias for false). */
    Yes       = 1, /*! Indicates that the option is enabled (alias for true). */
    Undefined = 2  /*! The variable is not yet initialised and its value is unknown. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EEnabled>;
namespace ZS {
namespace System {
typedef CEnum<EEnabled> CEnumEnabled;
} }

namespace ZS {
namespace System {
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
enum class ECopyDepth
//==============================================================================
{
    FlatKeepOwnership     = 0, /*! The source object keeps the ownership of the pointer. */
    FlatReleaseOwnwership = 1, /*! The source object releases the ownership of the pointer. */
    Deep                  = 2  /*! The addressed memory will be cloned and both the source and target use their own and different pointers. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ECopyDepth>;
namespace ZS {
namespace System {
typedef CEnum<ECopyDepth> CEnumCopyDepth;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines the different life cycle states of an object.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EObjState
//==============================================================================
{
    Detached   = 0, /*!< A created object may be set to this state to indicate that it has not been added to or has been removed from the target collection or has not yet been created. */
    Creating   = 1, /*!< The object is being created. */
    Created    = 2, /*!< The object has been created. */
    Destroying = 3, /*!< The object is beeing destroyed. */
    Destroyed  = 4, /*!< The object has been destroyed. */
    Undefined  = 5  /*!< The object state is not defined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EObjState>;
namespace ZS {
namespace System {
typedef CEnum<EObjState> CEnumObjState;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines different row versions.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ERowVersion
//==============================================================================
{
    Original  = 0,  /*!< Original version before changing the value. */
    Current   = 1   /*!< After editing a value the current version might be differet from the original value. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ERowVersion>;
namespace ZS {
namespace System {
typedef CEnum<ERowVersion> CEnumRowVersion;
} }

namespace ZS {
namespace System {
//==============================================================================
/* This enumeration is used to indicate the state of values in lists.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ERowState
//==============================================================================
{
    Unchanged = 0, /*!< After merging changes between different versions and accepting
                        the changes the state is set to Unchanged. For Removed objects
                        and DataRows accepting the changes means destroying the object. */
    Detached  = 1, /*!< After initially creating the object the row state is set to
                        Detached. The object is not added to its parent, rows don't
                        belong to tables yet. */
    Added     = 2, /*!< After adding the object to its parent the row state is changed
                        from Detached to Added. */
    Removed   = 3, /*!< After removing the object from its parent the row state is set
                        to Removed. The object is still alive. On accepting the change
                        the object or the data row got to be deleted. */
    Modified  = 4, /*!< On changing any property or value of an Unchanged object the
                        state may be set to Modified. */
    Undefined = 5  /*!< The row state is not defined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ERowState>;
namespace ZS {
namespace System {
typedef CEnum<ERowState> CEnumRowState;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines the desired format of an string.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EContentToStrFormat
//==============================================================================
{
    PlainText = 0,  /*!< Plain text without formatting characters. */
    HtmlText  = 1,  /*!< Text containing HTML format strings. */
    Undefined = 2   /*!< The desired format is not defined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EContentToStrFormat>;
namespace ZS {
namespace System {
typedef CEnum<EContentToStrFormat> CEnumContentToStrFormat;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! This enum defines two different search directions.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ESearchDirection
//==============================================================================
{
    Ascending  = 0,  /*!< Search upwards. */
    Descending = 1   /*!< Search downwards. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ESearchDirection>;
namespace ZS {
namespace System {
typedef CEnum<ESearchDirection> CEnumSearchDirection;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! This enum is mainly used to define for what purpose trace outputs should
    be generated.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EMethodDir
//==============================================================================
{
    Enter     = 0,  /*!< The method has been entered (indicating usually with "->" in text output). */
    Leave     = 1,  /*!< The method will be left (indicating usually with "<-" in text output). */
    None      = 2,  /*!< The method in somewhere in the middle (indicating usually with "  " in text output). */
    Undefined = 3   /*!< The direction is not defined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EMethodDir>;
namespace ZS {
namespace System {
typedef CEnum<EMethodDir> CEnumMethodDir;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines whether a data stream is input or output.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ETransmitDir
//==============================================================================
{
    Receive   = 0,  /*!< The data (message) is received. */
    Send      = 1,  /*!< The data (message) is sent. */
    Undefined = 2   /*!< The transmission direction is undefined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ETransmitDir>;
namespace ZS {
namespace System {
typedef CEnum<ETransmitDir> CEnumTransmitDir;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines whether a data stream is input or output.

    May be used if IODir better reflects the usage than TransmitDir.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EIODir
//==============================================================================
{
    In        = 0,  /*!< The data (message) is input. */
    Out       = 1,  /*!< The data (message) is output. */
    Undefined = 2   /*!< The IO direction is undefined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EIODir>;
namespace ZS {
namespace System {
typedef CEnum<EIODir> CEnumIODir;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Orientation which could either be horizontal or vertical.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EOrientation
//==============================================================================
{
    Horizontal = 0, /*!< Horizontally aligned. */
    Vertical   = 1  /*!< Vertically aligned. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EOrientation>;
namespace ZS {
namespace System {
typedef CEnum<EOrientation> CEnumOrientation;
} }


namespace ZS {
namespace System {
//==============================================================================
/*! Layout positions
*/
enum class ELayoutPos {
//==============================================================================
    Top    = 0,
    Bottom = 1,
    Left   = 2,
    Right  = 3,
    Center = 4,
};
} } // ZS::System

template class ZSSYSDLL_API CEnum<ZS::System::ELayoutPos>;
namespace ZS {
namespace System {
typedef CEnum<ELayoutPos> CEnumLayoutPos;
} } // ZS::System


namespace ZS {
namespace System {
//==============================================================================
/*! Scale axis.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EScaleAxis {
//==============================================================================
    X,  /*!< X (horizontal) scale orientation. */
    Y   /*!< Y (vertical ) scale orientation. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EScaleAxis>;
namespace ZS {
namespace System {
typedef CEnum<EScaleAxis> CEnumScaleAxis;

// To make the code more readable:
const int EScaleAxisX = static_cast<int>(EScaleAxis::X);
const int EScaleAxisY = static_cast<int>(EScaleAxis::Y);
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Orientation of the Y scale axis.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EYScaleAxisOrientation {
//==============================================================================
    TopDown, /*!< Y scale orientation is from top to bottom. */
    BottomUp /*!< Y scale orientation is from bottom to top. */
};
} }

template class ZSSYSDLL_API CEnum<ZS::System::EYScaleAxisOrientation>;
namespace ZS {
namespace System {
typedef CEnum<EYScaleAxisOrientation> CEnumYScaleAxisOrientation;
} }


namespace ZS {
namespace System {
//==============================================================================
/*! Spacing of scales which could be either linear or logarithmic.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ESpacing {
//==============================================================================
    Linear,     /*!< Linear spacing (distance between division lines is equal). */
    Logarithmic /*!< Loagrithmic spacing (distance between division lines is logarithimic). */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ESpacing>;
namespace ZS {
namespace System {
typedef CEnum<ESpacing> CEnumSpacing;
} }


namespace ZS {
namespace System {
//==============================================================================
/*! For grids in diagrams main and sub division lines may be drawn.

    Main division lines are usually more emphasized than sub division lines.
    Some of the sub division lines don't need to be drawn as they overlay
    main division lines.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EDivLineLayer {
//==============================================================================
    Main,       /*!< Main division lines. */
    Sub,        /*!< Sub division lines which are less emphasized than the main lines. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EDivLineLayer>;
namespace ZS {
namespace System {
typedef CEnum<EDivLineLayer> CEnumDivLineLayer;

// To make the code more readable:
const int EDivLineLayerMain = static_cast<int>(EDivLineLayer::Main);
const int EDivLineLayerSub = static_cast<int>(EDivLineLayer::Sub);
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines the comparison operator to be used.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum EComparisonOperator
//==============================================================================
{
    Equal     = 0,  /*!< To be compared for equality (==). */
    Unequal   = 1,  /*!< To be compared for inequality (!=). */
    Undefined = 2   /*!< Comparison operator not defined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EComparisonOperator>;
namespace ZS {
namespace System {
typedef CEnum<EComparisonOperator> CEnumComparisonOperator;
} }

namespace ZS {
namespace System {
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
enum class EValueValidity
//==============================================================================
{
    Invalid   = 0,  /*!< The value is invalid. */
    Null      = 1,  /*!< The value is valid but set to null. */
    Valid     = 2,  /*!< The value is valid. */
    Undefined = 3   /*!< The validity is undefined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EValueValidity>;
namespace ZS {
namespace System {
typedef CEnum<EValueValidity> CEnumValueValidity;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Defines the dimensin type of parameter arrays.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EDimensionType
//==============================================================================
{
    SingleValue     = 0,    /*!< No array just a single value. */
    OneDimensional  = 1,    /*!< An one dimensional array. */
    MoreDimensional = 2,    /*!< More dimensional arrays (an array of arrrays). */
    Undefined       = 3     /*!< The dimension type is undefined. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EDimensionType>;
namespace ZS {
namespace System {
typedef CEnum<EDimensionType> CEnumDimensionType;
} }

namespace ZS {
namespace System {
// Negative values for array indices are invalid. Some of those negative values
// are used to indicate special ranges or special elements within the array.
const int EArrayIndexUndefined        = -1;
const int EArrayIndexFirstElement     = -2;
const int EArrayIndexLastElement      = -3;
const int EArrayIndexCountAllElements = -4;

ZSSYSDLL_API QString arrayIndex2Str( int i_idx, EEnumEntryAliasStr i_alias = EEnumEntryAliasStrName );

ZSSYSDLL_API bool isValidIndex( int i_idx, int i_iArrLen, bool i_bAllowSpecialArrayIndices = false ); // ok if idxVal >= 0 AND idxVal < ArrLen or idxVal = First|Last|All (if AllowSpecial=true)
ZSSYSDLL_API bool isValidRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, bool i_bAllowSpecialArrayIndices = false );
ZSSYSDLL_API int toArrayLen( int i_idxCount, int i_iArrLen, bool i_bLimitToArrLen = false );
ZSSYSDLL_API int toArrayIndex( int i_idx, int i_iArrLen, bool i_bLimitToArrLen = false );
ZSSYSDLL_API void getMinMaxArrayRange( int i_iArrIdx, int i_iArrLen, int* o_pIdxMin, int* o_pIdxMax );
ZSSYSDLL_API void getMinMaxArrayRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, int* o_pIdxMin, int* o_pIdxMax );
ZSSYSDLL_API void getMinCountArrayRange( int i_idxStart, int i_iArrLen, int* o_pIdxMin, int* o_piIdxCount );
ZSSYSDLL_API void getMinCountArrayRange( int i_idxStart, int i_iIdxCount, int i_iArrLen, int* o_pIdxMin, int* o_pIdxCount );
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Predefined trace detail levels for method calls.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EMethodTraceDetailLevel
//==============================================================================
{
    None         = 0, /*!< Method trace output is disabled. */
    EnterLeave   = 1, /*!< Tracing entering and leaving methods only
                           (without tracing input and output arguments and
                           without method return value). */
    ArgsNormal   = 2, /*!< Tracing entering and leaving methods together with
                           input and output arguemnts and method return value. */
    ArgsDetailed = 3, /*!< Tracing entering and leaving methods together with
                           input and output arguements and method return value
                           with more details than normal (content of arrays). */
    ArgsVerbose  = 4, /*!< Tracing entering and leaving methods together with
                           input and output arguemnts and method return value
                           in a very detailed level (content of arrays). */
    Undefined    = 5  /*!< Used e.g. to indicate that the trace level should not be used
                           but the predefined detail level should be used. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EMethodTraceDetailLevel>;
namespace ZS {
namespace System {
typedef CEnum<EMethodTraceDetailLevel> CEnumMethodTraceDetailLevel;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Predefined detail levels to trace runtime info.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    Higher detail levels include lower detail levels.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class ELogDetailLevel
//==============================================================================
{
    None          = 0, /*!< Log outputs are disabled. */
    Fatal         = 1, /*!< Log fatal errors (critical errors, critical exceptions). */
    Error         = 2, /*!< At this level all error conditions should be logged. */
    Warning       = 3, /*!< At the Warning level all events that could potentially
                            become an error should be logged. */
    Notice        = 4, /*!< At this level all events should be logged which are
                            considered to be notable but are not an error. */
    Info          = 5, /*!< The Info level is usually used to log all actions that
                            are user-driven or system specific. */
    Debug         = 6, /*!< The Debug level is usually used to log anything that
                            happens in the program for debugging purposes. */
    DebugDetailed = 7, /*!< Output detailed debug info. */
    DebugVerbose  = 8, /*!< Output very detailed debug info. */
    Undefined     = 9  /*!< Used e.g. to indicate that the log level should not be used
                            but the predefined detail level should be used. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::ELogDetailLevel>;
namespace ZS {
namespace System {
typedef CEnum<ELogDetailLevel> CEnumLogDetailLevel;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Filter properties which can be changed for trace admin objects.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EMethodTraceFilterProperty {
//==============================================================================
    Undefined   = 0,    /*!< Used to indicate that no filter property has been selected. */
    ObjectName  = 1,    /*!< Filter expression valid for object names. */
    MethodName  = 2,    /*!< Filter expression valid for method names. */
    TraceData   = 3     /*!< Filter expression for trace data. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EMethodTraceFilterProperty>;
namespace ZS {
namespace System {
typedef CEnum<EMethodTraceFilterProperty> CEnumMethodTraceFilterProperty;
} }

namespace ZS {
namespace System {
//==============================================================================
/*! Filter expressions are used to either include or exclude patterns.

    @ingroup _GRP_Libs_ZSSys_MethodTracing

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EMethodTraceFilterExpressionType {
//==============================================================================
    Undefined = 0,  /*!< Used to indicate that the filter expression has not been set. */
    Include   = 1,  /*!< Filter expression used to include patterns. */
    Exclude   = 2   /*!< Filter expression used to exclude patterns. */
};
} }

// template is out of namespace as gcc complains with error: "Explicit instantiation of 'CEnum" must occur at global scope."
template class ZSSYSDLL_API CEnum<ZS::System::EMethodTraceFilterExpressionType>;
namespace ZS {
namespace System {
typedef CEnum<EMethodTraceFilterExpressionType> CEnumMethodTraceFilterExpressionType;
} }

#ifdef _WINDOWS
#pragma warning( pop )
#endif

#endif // #ifndef ZSSys_Common_h
