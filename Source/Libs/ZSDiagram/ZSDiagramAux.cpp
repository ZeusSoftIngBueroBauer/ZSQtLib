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

#include "ZSDiagram/ZSDiagramAux.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysMath.h"

#include <math.h>

#include <QtCore/qrect.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Diagram;


/*******************************************************************************
Exported auxiliary methods
*******************************************************************************/

//------------------------------------------------------------------------------
double ZS::Diagram::getValRes(
    CUnit*   i_pUnitScale,
    double   i_fScaleMinVal,
    double   i_fScaleMaxVal,
    int      i_iScaleRangePix, // zero is allowed
    ESpacing i_spacing,
    CUnit*   i_pUnitVal,       // use scale unit if nullptr
    double   i_fVal,
    CUnit*   i_pUnitRes )      // use unit of value if nullptr
//------------------------------------------------------------------------------
{
    double fRes = 1.0;
    CUnit* pUnitVal = i_pUnitVal;
    CUnit* pUnitRes = i_pUnitRes;
    double fScaleRange;
    double fVal = i_fVal;

    if( pUnitVal == nullptr )
    {
        pUnitVal = i_pUnitScale;
    }
    if( pUnitRes == nullptr )
    {
        pUnitRes = pUnitVal;
    }

    if( i_spacing == ESpacing::Linear )
    {
        fScaleRange = fabs(i_fScaleMaxVal-i_fScaleMinVal);

        if( fScaleRange > 0.0 )
        {
            if( i_iScaleRangePix > 1 )
            {
                fRes = fScaleRange / (i_iScaleRangePix-1);
            }
            else
            {
                fRes = fScaleRange / 1000.0;
            }
        }
    } // if( spacing == Linear )

    else // if( i_spacing == ESpacingLogarithmic )
    {
        if( i_fScaleMaxVal > 0.0 && i_fScaleMinVal > 0.0 )
        {
            fScaleRange = log10(i_fScaleMaxVal) - log10(i_fScaleMinVal);

            if( fScaleRange > 0.0 )
            {
                if( i_iScaleRangePix > 1.0 )
                {
                    fRes = fScaleRange / (i_iScaleRangePix-1.0);
                }
                else
                {
                    fRes = fScaleRange / 1000.0;
                }
            }
        } // if( fScaleMaxVal > 0.0 && fScaleMinVal > 0.0 )

        if( pUnitVal != nullptr && i_pUnitScale != nullptr && pUnitVal != i_pUnitScale )
        {
            fVal = pUnitVal->convertValue( fVal, i_pUnitScale );
        }
        fRes = Math::logRes2LinRes(fRes,fVal);

    } // if( spacing == Logarithmic )

    if( pUnitRes != nullptr && i_pUnitScale != nullptr && pUnitRes != i_pUnitScale )
    {
        fRes = i_pUnitScale->convertValue( fRes, pUnitRes );
    }
    fRes = Math::round2LowerDecade(fRes);

    return fRes;

} // getValRes

//------------------------------------------------------------------------------
QString ZS::Diagram::measState2Str( int i_iMeasState )
//------------------------------------------------------------------------------
{
    static const char* s_arszMeasState[EMeasStateCount] = {
        "Off",
        "On",
        "Halted"
    };
    if( i_iMeasState >= 0 && i_iMeasState < EMeasStateCount )
    {
        return s_arszMeasState[i_iMeasState];
    }
    if( i_iMeasState == EMeasStateCount )
    {
        return "Count";
    }
    if( i_iMeasState == EMeasStateUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
QString ZS::Diagram::measMode2Str( int i_iMeasMode )
//------------------------------------------------------------------------------
{
    static const char* s_arszMeasMode[EMeasModeCount] = {
        "Single",
        "Cont."
    };
    if( i_iMeasMode >= 0 && i_iMeasMode < EMeasModeCount )
    {
        return s_arszMeasMode[i_iMeasMode];
    }
    if( i_iMeasMode == EMeasModeCount )
    {
        return "Count";
    }
    if( i_iMeasMode == EMeasModeUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
QString ZS::Diagram::diagramUpdateType2Str( int i_iUpdateType )
//------------------------------------------------------------------------------
{
    static const char* s_arszUpdateType[EDiagramUpdateTypeCount] = {
        "Data",
        "Pixmap",
        "Widget"
    };
    if( i_iUpdateType >= 0 && i_iUpdateType < EDiagramUpdateTypeCount )
    {
        return s_arszUpdateType[i_iUpdateType];
    }
    if( i_iUpdateType == EDiagramUpdateTypeCount )
    {
        return "Count";
    }
    if( i_iUpdateType == EDiagramUpdateTypeUndefined )
    {
        return "Undefined";
    }
    return "Out Of Range";
}

//------------------------------------------------------------------------------
QString ZS::Diagram::updateFlags2Str( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    static const char* s_arszUpdateFlags[EUpdateAll+1] = {
        "None",                      // = 0x00, 0000 0000
        "Layout",                    // = 0x01, 0000 0001
        "Data",                      // = 0x02, 0000 0010
        "Layout|Data",               // = 0x03, 0000 0011
        "Pixmap",                    // = 0x04, 0000 0100
        "Layout|Pixmap",             // = 0x05, 0000 0101
        "Data|Pixmap",               // = 0x06, 0000 0110
        "Layout|Data|Pixmap",        // = 0x07, 0000 0111
        "Widget",                    // = 0x08, 0000 1000
        "Layout|Widget",             // = 0x09, 0000 1001
        "Data|Widget",               // = 0x0A, 0000 1010
        "Layout|Data|Widget",        // = 0x0B, 0000 1011
        "Pixmap|Widget",             // = 0x0C, 0000 1100
        "Layout|Pixmap|Widget",      // = 0x0D, 0000 1101
        "Data|Pixmap|Widget",        // = 0x0E, 0000 1110
        "Layout|Data|Pixmap|Widget"  // = 0x0F  0000 1111
    };

    unsigned int uUpdateFlags = EUpdateAll & i_uUpdateFlags;
    if( uUpdateFlags <= EUpdateAll )
    {
        return s_arszUpdateFlags[uUpdateFlags];
    }
    return "?";

} // updateFlags2Str


/*==============================================================================
Enum ETextOrientation
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Diagram::textOrientation2Str( int i_iTextOrientation )
//------------------------------------------------------------------------------
{
    static const char* s_arszTextOrientation[ETextOrientationCount] = {
        "Left2Right",
        "Right2Left",
        "Bottom2Top",
        "Top2Bottom"
    };

    if( i_iTextOrientation >= 0 && i_iTextOrientation < ETextOrientationCount )
    {
        return s_arszTextOrientation[i_iTextOrientation];
    }
    if( i_iTextOrientation == ETextOrientationCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // textOrientation2Str

/*==============================================================================
Enum EDiagObjMoveMode
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Diagram::moveMode2Str( int i_iMoveMode )
//------------------------------------------------------------------------------
{
    static const char* s_arszMoveMode[EDiagObjMoveModeCount] = {
        "ToTop",
        "ToBottom",
        "OneStepUp",
        "OneStepDown",
        "Relative",
        "Absolute"
    };

    if( i_iMoveMode >= 0 && i_iMoveMode < EDiagObjMoveModeCount )
    {
        return s_arszMoveMode[i_iMoveMode];
    }
    if( i_iMoveMode == EDiagObjMoveModeCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // moveMode2Str

/*==============================================================================
Enum EDiagObjState
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Diagram::diagObjState2Str( int i_iDiagObjState )
//------------------------------------------------------------------------------
{
    static const char* s_arszDiagObjState[] = {
        "Normal",
        "Focused",
        "Editing"
    };

    if( i_iDiagObjState >= 0 && i_iDiagObjState < _ZSArrLen(s_arszDiagObjState) )
    {
        return s_arszDiagObjState[i_iDiagObjState];
    }
    if( i_iDiagObjState == EDiagObjStateCount )
    {
        return "Count";
    }
    return "Out Of Range";

} // diagObjState2Str
