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

#ifndef ZSDiagramAux_h
#define ZSDiagramAux_h

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"

namespace ZS
{
namespace Diagram
{
/*******************************************************************************
Type definitions and constants
*******************************************************************************/

typedef enum {
    EOnOffStateMin = 0,
    EOnOffStateOff = 0,
    EOnOffStateOn  = 1,
    EOnOffStateMax = 1,
    EOnOffStateCount,
    EOnOffStateUndefined
}   EOnOffState;
ZSDIAGRAMDLL_API QString onOffState2Str( int i_iState );
ZSDIAGRAMDLL_API EOnOffState str2OnOffState( const QString& i_str );

typedef enum {
    EMeasStateMin    = 0,
    EMeasStateOff    = 0,
    EMeasStateOn     = 1,
    EMeasStateHalted = 2,
    EMeasStateMax    = 2,
    EMeasStateCount,
    EMeasStateUndefined
}   EMeasState;
ZSDIAGRAMDLL_API QString measState2Str( int i_iMeasState );

typedef enum {
    EMeasModeMin        = 0,
    EMeasModeSingle     = 0,
    EMeasModeContinuous = 1,
    EMeasModeMax        = 1,
    EMeasModeCount,
    EMeasModeUndefined
}   EMeasMode;
ZSDIAGRAMDLL_API QString measMode2Str( int i_iMeasMode );

typedef enum {
    EDiagramUpdateTypeMin    = 0,
    EDiagramUpdateTypeData   = 0,
    EDiagramUpdateTypePixmap = 1,
    EDiagramUpdateTypeWidget = 2,
    EDiagramUpdateTypeMax    = 2,
    EDiagramUpdateTypeCount,
    EDiagramUpdateTypeUndefined
}   EDiagramUpdateType;
ZSDIAGRAMDLL_API QString diagramUpdateType2Str( int i_iUpdateType );

enum {
    EUpdateNone                   = static_cast<unsigned int>(0x00), // 0000 0000
    EUpdateLayout                 = static_cast<unsigned int>(0x01), // 0000 0001
    EUpdateData                   = static_cast<unsigned int>(0x02), // 0000 0010
    EUpdateLayoutData             = static_cast<unsigned int>(0x03), // 0000 0011
    EUpdatePixmap                 = static_cast<unsigned int>(0x04), // 0000 0100
    EUpdateLayoutPixmap           = static_cast<unsigned int>(0x05), // 0000 0101
    EUpdateDataPixmap             = static_cast<unsigned int>(0x06), // 0000 0110
    EUpdateLayoutDataPixmap       = static_cast<unsigned int>(0x07), // 0000 0111
    EUpdateWidget                 = static_cast<unsigned int>(0x08), // 0000 1000
    EUpdateLayoutWidget           = static_cast<unsigned int>(0x09), // 0000 1001
    EUpdateDataWidget             = static_cast<unsigned int>(0x0A), // 0000 1010
    EUpdateLayoutDataWidget       = static_cast<unsigned int>(0x0B), // 0000 1011
    EUpdatePixmapWidget           = static_cast<unsigned int>(0x0C), // 0000 1100
    EUpdateLayoutPixmapWidget     = static_cast<unsigned int>(0x0D), // 0000 1101
    EUpdateDataPixmapWidget       = static_cast<unsigned int>(0x0E), // 0000 1110
    EUpdateLayoutDataPixmapWidget = static_cast<unsigned int>(0x0F), // 0000 1111
    EUpdateAll                    = static_cast<unsigned int>(0x0F)  // 0000 1111
};
ZSDIAGRAMDLL_API QString updateFlags2Str( unsigned int i_uUpdateFlags );

typedef enum {
    EScaleDirMin = 0,
    EScaleDirX   = 0,
    EScaleDirY   = 1,
    EScaleDirMax = 1,
    EScaleDirCount,
    EScaleDirUndefined
}   EScaleDir;
ZSDIAGRAMDLL_API QString scaleDir2Str( int i_iScaleDir );

typedef enum {
    EOrientationMin        = 0,
    EOrientationHorizontal = 0,
    EOrientationVertical   = 1,
    EOrientationMax        = 1,
    EOrientationCount,
    EOrientationUndefined
}   EOrientation;
ZSDIAGRAMDLL_API QString orientation2Str( int i_iOrientation );

typedef enum {
    ESpacingMin         = 0,
    ESpacingLinear      = 0,
    ESpacingLogarithmic = 1,
    ESpacingMax         = 1,
    ESpacingCount,
    ESpacingUndefined
}   ESpacing;
ZSDIAGRAMDLL_API QString spacing2Str( int i_iSpacing );

typedef enum {
    ELayoutPosMin       = 0,
    ELayoutPosTop       = 0, // Top, Left, and Right overlap each other at the top corners of the diagram
    ELayoutPosBottom    = 1, // Bottom, Left, and Right overlap each other at the bottom corners of the diagram
    ELayoutPosLeft      = 2,
    ELayoutPosRight     = 3,
    ELayoutPosCenter    = 4, // The Center area does not overlap with any other part and is expandable
    ELayoutPosUndefined = 5, // For invisible objects (e.g. value providers)
    ELayoutPosMax       = 5,
    ELayoutPosCount
}   ELayoutPos;
ZSDIAGRAMDLL_API QString layoutPos2Str( int i_iLayoutPos );

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
template class ZSDIAGRAMDLL_API ZS::System::CEnum<EDivLineLayer>;
typedef ZS::System::CEnum<EDivLineLayer> CEnumDivLineLayer;

//==============================================================================
/*! Labeling axis lines is done in three different parts.

    -|-----|-----|-----|-----|    <- Lines
    0.0   1.0   2.0   3.0   Freq/MHz
    Labels                  AxisLabel

    - The Lines are used to indicate at which position the axis is divided.
      If also grid is shown the Lines of the axis may be superfluous.
    - Labels are shown at the Lines to indicate the corresponding value.
    - The AxisLabel is used to indicate the physical size (Frequency)
      together with the physical unit.

    @see ZS::System::SEnumEntry
    @see ZS::System::CEnum
    @see _GRP_BasicConcepts_Enumerations
*/
enum class EDivLineLabelsPart {
//==============================================================================
    Lines,      /*!< Lines to divide the axis into sections. */
    Labels,     /*!< Labels at the lines to indicate their values. */
    AxisLabel   /*!< Indicates the physical size and unit. */
};
template class ZSDIAGRAMDLL_API ZS::System::CEnum<EDivLineLabelsPart>;
typedef ZS::System::CEnum<EDivLineLabelsPart> CEnumDivLineLabelsPart;

typedef enum {
    ETextDirectionMin        = 0,
    ETextDirectionLeft2Right = 0,
    ETextDirectionRight2Left = 1,
    ETextDirectionBottom2Top = 2,
    ETextDirectionTop2Bottom = 3,
    ETextDirectionMax        = 3,
    ETextDirectionCount
}   ETextDirection;
ZSDIAGRAMDLL_API QString textDirection2Str( int i_iTextDirection );

typedef enum {
    EDiagObjStateMin     = 0,
    EDiagObjStateNormal  = 0,
    EDiagObjStateFocused = 1,
    EDiagObjStateEditing = 2,
    EDiagObjStateMax     = 2,
    EDiagObjStateCount
}   EDiagObjState;
ZSDIAGRAMDLL_API QString diagObjState2Str( int i_iDiagObjStateIdx );

typedef enum {
    EDiagObjMoveModeMin         = 0,
    EDiagObjMoveModeToTop       = 0, // to the end of the list (to be treated at the last object)
    EDiagObjMoveModeToBottom    = 1, // not yet supported: to the beginning of the list (to be treated at the first object)
    EDiagObjMoveModeOneStepUp   = 2, // not yet supported: to the end of the list
    EDiagObjMoveModeOneStepDown = 3, // not yet supported: to the beginning of the list
    EDiagObjMoveModeRelative    = 4, // not yet supported
    EDiagObjMoveModeAbsolute    = 5, // not yet supported
    EDiagObjMoveModeMax         = 5,
    EDiagObjMoveModeCount
}   EDiagObjMoveMode;
ZSDIAGRAMDLL_API QString moveMode2Str( int i_iMoveMode );

typedef enum {
    EImageFormatMin = 0,
    EImageFormatBMP = 0,
    EImageFormatJPG = 1,
    EImageFormatXPM = 2,
    EImageFormatPNG = 3,
    EImageFormatMax = 3,
    EImageFormatCount,
    EImageFormatUndefined = -1
}   EImageFormat;


/*******************************************************************************
Exported auxiliary methods
*******************************************************************************/

ZSDIAGRAMDLL_API double getValRes(
    PhysVal::CUnit* i_pUnitScale,
    double          i_fScaleMinVal,
    double          i_fScaleMaxVal,
    int             i_iScaleRangePix, // zero is allowed
    ESpacing        i_spacing,
    PhysVal::CUnit* i_pUnitVal,    // use unit of scale if nullptr
    double          i_fVal,
    PhysVal::CUnit* i_pUnitRes );  // use unit of value if nullptr

//******************************************************************************
struct ZSDIAGRAMDLL_API SScale
//******************************************************************************
{
public: // ctors
    SScale();
    SScale( const SScale& i_scaleOther );
    SScale( double i_fMin, double i_fMax, const PhysVal::CUnit& i_unit );
public: // instance methods
    PhysVal::CPhysVal physValMin() const;
    PhysVal::CPhysVal physValMax() const;
    PhysVal::CPhysVal physValRange() const;
public: // operators
    bool operator == ( const SScale& i_scaleOther ) const;
    bool operator != ( const SScale& i_scaleOther ) const;
public: // instance methods
    QString toString() const;
public: // instance members
    double         m_fMin;
    double         m_fMax;
    PhysVal::CUnit m_unit;

}; // struct SScale

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramAux_h
