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

#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
enum EDrawAttribute
*******************************************************************************/

const SEnumEntry s_arEnumStrDrawAttributes[] =
{
    /*  0 */ SEnumEntry( EDrawAttributePenColor,                     "PenColor"                     ),
    /*  1 */ SEnumEntry( EDrawAttributePenWidth,                     "PenWidth"                     ),
    /*  2 */ SEnumEntry( EDrawAttributeFillColor,                    "FillColor"                    ),
    /*  3 */ SEnumEntry( EDrawAttributeFillStyle,                    "FillStyle"                    ),
    /*  4 */ SEnumEntry( EDrawAttributeLineStyle,                    "LineStyle"                    ),
    /*  5 */ SEnumEntry( EDrawAttributeLineRecordType,               "LineRecordType"               ),
    /*  6 */ SEnumEntry( EDrawAttributeLineExtent,                   "LineExtent"                   ),
    /*  7 */ SEnumEntry( EDrawAttributeLineP1EndStyle,               "LineP1EndStyle"               ),
    /* 13 */ SEnumEntry( EDrawAttributeLineP1ArrowHeadBaseLineType,  "LineP1ArrowHeadBaseLineType"  ),
    /* 14 */ SEnumEntry( EDrawAttributeLineP1ArrowHeadFillStyle,     "LineP1ArrowHeadFillStyle"     ),
    /* 15 */ SEnumEntry( EDrawAttributeLineP1ArrowHeadWidth,         "LineP1ArrowHeadWidth"         ),
    /* 16 */ SEnumEntry( EDrawAttributeLineP1ArrowHeadLength,        "LineP1ArrowHeadLength"        ),
    /* 12 */ SEnumEntry( EDrawAttributeLineP2EndStyle,               "LineP2EndStyle"               ),
    /* 13 */ SEnumEntry( EDrawAttributeLineP2ArrowHeadBaseLineType,  "LineP2ArrowHeadBaseLineType"  ),
    /* 14 */ SEnumEntry( EDrawAttributeLineP2ArrowHeadFillStyle,     "LineP2ArrowHeadFillStyle"     ),
    /* 15 */ SEnumEntry( EDrawAttributeLineP2ArrowHeadWidth,         "LineP2ArrowHeadWidth"         ),
    /* 16 */ SEnumEntry( EDrawAttributeLineP2ArrowHeadLength,        "LineP2ArrowHeadLength"        ),
    /* 17 */ SEnumEntry( EDrawAttributeTextColor,                    "TextColor"                    ),
    /* 18 */ SEnumEntry( EDrawAttributeTextFont,                     "TextFont"                     ),
    /* 19 */ SEnumEntry( EDrawAttributeTextStyle,                    "TextStyle"                    ),
    /* 20 */ SEnumEntry( EDrawAttributeTextSize,                     "TextSize"                     ),
    /* 21 */ SEnumEntry( EDrawAttributeTextEffect,                   "TextEffect"                   )
};

//------------------------------------------------------------------------------
QString ZS::Draw::drawAttribute2Str( int i_iAttribute )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(
        s_arEnumStrDrawAttributes, _ZSArrLen(s_arEnumStrDrawAttributes), i_iAttribute);
}

//------------------------------------------------------------------------------
EDrawAttribute ZS::Draw::str2DrawAttribute( const QString& i_str )
//------------------------------------------------------------------------------
{
    EDrawAttribute drawAttribute = EDrawAttributeUndefined;
    int iDrawAttribute = SEnumEntry::str2Enumerator(
        s_arEnumStrDrawAttributes, _ZSArrLen(s_arEnumStrDrawAttributes), i_str);
    if (iDrawAttribute >= 0 && iDrawAttribute < _ZSArrLen(s_arEnumStrDrawAttributes)) {
        drawAttribute = static_cast<EDrawAttribute>(iDrawAttribute);
    }
    else {
        bool bConverted;
        iDrawAttribute = i_str.toInt(&bConverted);
        if (bConverted && iDrawAttribute >= 0 && iDrawAttribute < _ZSArrLen(s_arEnumStrDrawAttributes)) {
            drawAttribute = static_cast<EDrawAttribute>(iDrawAttribute);
        }
    }
    return drawAttribute;
}


/*******************************************************************************
class CDrawSettings
*******************************************************************************/

//------------------------------------------------------------------------------
static const SDrawAttribute s_arDrawAttributesDefaultValsGraphObjTypeUndefined[EDrawAttributeCount] =
//------------------------------------------------------------------------------
{   // For Undefined GraphObjTypes (used by the drawing scene) each of the attributes is used as default:
    /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
    /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
    /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
    /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
    /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
    /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
    /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
    /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
    /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
    /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
    /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
    /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
    /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
    /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
    /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
    /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
    /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
    /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), true ),
    /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), true ),
    /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), true ),
    /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, true ),
    /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), true )
};

//------------------------------------------------------------------------------
static const SDrawAttribute s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[EDrawAttributeCount] =
//------------------------------------------------------------------------------
{   // For UserDefined GraphObjTypes none of the attributes is used as default:
    /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), false ),
    /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, false),
    /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
    /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
    /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
    /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
    /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
    /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
    /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
    /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
    /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
    /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
    /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
    /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
    /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
    /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
    /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
    /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
    /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
    /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
    /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
    /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
};

//------------------------------------------------------------------------------
static const SDrawAttribute s_ararDrawAttributesDefaultVals[EGraphObjTypeCount][EDrawAttributeCount] =
//------------------------------------------------------------------------------
{
    /*  0 EGraphObjTypePoint           */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  1 EGraphObjTypeLine            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  2 EGraphObjTypeRect            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  3 EGraphObjTypeEllipse         */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  4 EGraphObjTypePolygon         */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  5 EGraphObjTypePolyline        */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  6 EGraphObjTypeText            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), true ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), true ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), true ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, true ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), true )
    /*                                 */ },
    /*  7 EGraphObjTypeImage           */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  8 EGraphObjTypeConnectionPoint */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::black), true ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::SolidPattern), true ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  9 EGraphObjTypeConnectionLine  */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /* 10 EGraphObjTypeGroup           */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), false ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, false),
        /*  2 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  3 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  4 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(EArrowHeadBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(EArrowHeadFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(EArrowHeadWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(EArrowHeadLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ }
};

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawSettings::IsAttributeUsed( EGraphObjType i_graphObjType, int i_iAttribute )
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        int idxGraphObjType = i_graphObjType - EGraphObjTypeMin;
        if (idxGraphObjType >= 0 && idxGraphObjType < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
            bIsUsed = s_ararDrawAttributesDefaultVals[idxGraphObjType][i_iAttribute].m_bIsUsed;
        }
        else if (i_graphObjType == EGraphObjTypeUndefined) {
            bIsUsed = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute].m_bIsUsed;
        }
        else {
            bIsUsed = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute].m_bIsUsed;
        }
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::IsAttributeUsed( EGraphObjType i_graphObjType, const QString& i_strName )
//------------------------------------------------------------------------------
{
    int iAttribute = str2DrawAttribute(i_strName);
    return IsAttributeUsed(i_graphObjType,iAttribute);
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawSettings::CDrawSettings( EGraphObjType i_graphObjType ) :
//------------------------------------------------------------------------------
    m_graphObjType(i_graphObjType),
    m_idxGraphObjTypeDrawAttributesDefaultVals(i_graphObjType-EGraphObjTypeMin),
    m_arAttributes(EDrawAttributeCount)
{
    // On recalling graphical objects the attributes will be read from XML file
    // by the base class CGraphObj before the ctor of the inherited classes
    // (e.g. CGraphObjLine) is called. To force the base class to read all
    // attributes defined in the XML file all attributes are set to used as
    // default. Only those attributes should appear in the XML file which are
    // used as on saving the object only the used attributes are stored.
    // The ctor of the inherited CGraphObj class needs to set all unused
    // attributes to false after the base class has been created.
    // The drawing class uses all attributes.

    set2Default();

} // default ctor

//------------------------------------------------------------------------------
CDrawSettings::~CDrawSettings()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawSettings::operator == ( const CDrawSettings& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        if (m_arAttributes[idxAttr].m_bIsUsed) {
            if (m_arAttributes[idxAttr].m_val != i_other.m_arAttributes[idxAttr].m_val) {
                bEqual = false;
                break;
            }
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CDrawSettings::operator != ( const CDrawSettings& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::save( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        QString strAttrName = getAttributeName(idxAttr);
        QString strAttrValue = attributeValue2Str(idxAttr); // current value
        i_settings.setValue( i_strSettingsKey + "/" + strAttrName, strAttrValue );
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        QString strAttrName = getAttributeName(idxAttr);
        QString strAttrValue = attributeValue2Str(idxAttr); // current value used as default for "settings.value"
        strAttrValue = i_settings.value(i_strSettingsKey + "/" + strAttrName, strAttrValue).toString();
        setAttribute(strAttrName,strAttrValue);
    }
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        EDrawAttribute attr = static_cast<EDrawAttribute>(idxAttr);
        bool bSave = true;
        if (isAttributeUsed(attr) && !isDefault(attr)) {
            switch (attr)
            {
                case EDrawAttributeTextColor:
                case EDrawAttributeTextFont:
                case EDrawAttributeTextStyle:
                case EDrawAttributeTextSize:
                case EDrawAttributeTextEffect:
                {
                    if (!isTextUsed()) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeFillColor:
                case EDrawAttributeFillStyle:
                {
                    if (!isFillUsed()) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributePenColor:
                case EDrawAttributePenWidth:
                {
                    if (!isPenUsed()) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineStyle:
                case EDrawAttributeLineRecordType:
                {
                    if (!isPenUsed()) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineExtent:
                {
                    if (!isPenUsed() || getLineRecordType() == ELineRecordType::Normal) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineP1EndStyle:
                case EDrawAttributeLineP1ArrowHeadBaseLineType:
                case EDrawAttributeLineP1ArrowHeadFillStyle:
                case EDrawAttributeLineP1ArrowHeadWidth:
                case EDrawAttributeLineP1ArrowHeadLength:
                {
                    if (!isPenUsed() || getLineEndStyle(ELinePoint::Start) == ELineEndStyle::Normal) {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineP2EndStyle:
                case EDrawAttributeLineP2ArrowHeadBaseLineType:
                case EDrawAttributeLineP2ArrowHeadFillStyle:
                case EDrawAttributeLineP2ArrowHeadWidth:
                case EDrawAttributeLineP2ArrowHeadLength:
                {
                    if (!isPenUsed() || getLineEndStyle(ELinePoint::End) == ELineEndStyle::Normal) {
                        bSave = false;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (bSave) {
                QString strName = getAttributeName(idxAttr);
                QString strValue = attributeValue2Str(idxAttr);
                i_xmlStreamWriter.writeTextElement(strName,strValue);
            }
        }
    }
} // save

//------------------------------------------------------------------------------
void CDrawSettings::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    while (!i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd()) {
        if (i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement()) {
            QString strElemName = i_xmlStreamReader.name().toString();
            if (i_xmlStreamReader.isStartElement()) {
                if (strElemName == "DrawSettings") {
                    for (int iAttribute = 0; iAttribute < EDrawAttributeCount; ++iAttribute) {
                        QString strName = drawAttribute2Str(iAttribute);
                        if (i_xmlStreamReader.attributes().hasAttribute(strName)) {
                            QString strVal = i_xmlStreamReader.attributes().value(strName).toString();
                            setAttribute(iAttribute, strVal);
                        }
                    }
                }
            }
            else if (i_xmlStreamReader.isEndElement()) {
                if (strElemName == "DrawSettings") {
                    break;
                }
            }
        }
        i_xmlStreamReader.readNext();
    }
} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::set2Default()
//------------------------------------------------------------------------------
{
    if (m_idxGraphObjTypeDrawAttributesDefaultVals >= 0 && m_idxGraphObjTypeDrawAttributesDefaultVals < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
        for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
            m_arAttributes[idxAttr] = s_ararDrawAttributesDefaultVals[m_idxGraphObjTypeDrawAttributesDefaultVals][idxAttr];
        }
    }
    else if (m_graphObjType == EGraphObjTypeUndefined) {
        for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
            m_arAttributes[idxAttr] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[idxAttr];
        }
    }
    else {
        for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
            m_arAttributes[idxAttr] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[idxAttr];
        }
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::set2Default( int i_iAttribute )
//------------------------------------------------------------------------------
{
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        if (m_idxGraphObjTypeDrawAttributesDefaultVals >= 0 && m_idxGraphObjTypeDrawAttributesDefaultVals < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
            m_arAttributes[i_iAttribute] = s_ararDrawAttributesDefaultVals[m_idxGraphObjTypeDrawAttributesDefaultVals][i_iAttribute];
        }
        else if (m_graphObjType == EGraphObjTypeUndefined) {
            m_arAttributes[i_iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute];
        }
        else {
            m_arAttributes[i_iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute];
        }
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::set2Default( const QString& i_strName )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        if (m_idxGraphObjTypeDrawAttributesDefaultVals >= 0 && m_idxGraphObjTypeDrawAttributesDefaultVals < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
            m_arAttributes[iAttribute] = s_ararDrawAttributesDefaultVals[m_idxGraphObjTypeDrawAttributesDefaultVals][iAttribute];
        }
        else if (m_graphObjType == EGraphObjTypeUndefined) {
            m_arAttributes[iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[iAttribute];
        }
        else {
            m_arAttributes[iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[iAttribute];
        }
    }
}

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault() const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;
    for (int idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++) {
        if (isAttributeUsed(idxAttr)) {
            if (m_idxGraphObjTypeDrawAttributesDefaultVals >= 0 && m_idxGraphObjTypeDrawAttributesDefaultVals < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
                if (m_arAttributes[idxAttr] != s_ararDrawAttributesDefaultVals[m_idxGraphObjTypeDrawAttributesDefaultVals][idxAttr]) {
                    bIsDefault = false;
                    break;
                }
            }
            else if (m_graphObjType == EGraphObjTypeUndefined) {
                if (m_arAttributes[idxAttr] != s_arDrawAttributesDefaultValsGraphObjTypeUndefined[idxAttr]) {
                    bIsDefault = false;
                    break;
                }
            }
            else {
                if (m_arAttributes[idxAttr] != s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[idxAttr]) {
                    bIsDefault = false;
                    break;
                }
            }
        }
    }
    return bIsDefault;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        if (isAttributeUsed(i_iAttribute)) {
            if (m_idxGraphObjTypeDrawAttributesDefaultVals >= 0 && m_idxGraphObjTypeDrawAttributesDefaultVals < _ZSArrLen(s_ararDrawAttributesDefaultVals)) {
                if (m_arAttributes[i_iAttribute] != s_ararDrawAttributesDefaultVals[m_idxGraphObjTypeDrawAttributesDefaultVals][i_iAttribute]) {
                    bIsDefault = false;
                }
            }
            else if (m_graphObjType == EGraphObjTypeUndefined) {
                if (m_arAttributes[i_iAttribute] != s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute]) {
                    bIsDefault = false;
                }
            }
            else {
                if (m_arAttributes[i_iAttribute] != s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute]) {
                    bIsDefault = false;
                }
            }
        }
    }
    return bIsDefault;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        bIsDefault = isDefault(iAttribute);
    }
    return bIsDefault;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const SDrawAttribute& i_drawAttribute )
//------------------------------------------------------------------------------
{
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        m_arAttributes[i_iAttribute] = i_drawAttribute;
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        if (i_val.type() == QVariant::String) {
            setAttribute(i_iAttribute, i_val.toString());
        }
        else {
            m_arAttributes[i_iAttribute] = i_val;
        }
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount)
    {
        switch( i_iAttribute )
        {
            case EDrawAttributePenColor:
            {
                m_arAttributes[i_iAttribute].m_val = QColor(i_strVal);
                break;
            }
            case EDrawAttributePenWidth:
            {
                m_arAttributes[i_iAttribute].m_val = i_strVal.toInt();
                break;
            }
            case EDrawAttributeLineStyle:
            {
                bool bConverted = false;
                CEnumLineStyle lineStyle = CEnumLineStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeFillColor:
            {
                m_arAttributes[i_iAttribute].m_val = QColor(i_strVal);
                break;
            }
            case EDrawAttributeFillStyle:
            {
                bool bConverted = false;
                CEnumFillStyle fillStyle = CEnumFillStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(fillStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeLineRecordType:
            {
                bool bConverted = false;
                CEnumLineRecordType lineRecordType = CEnumLineRecordType::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineRecordType.enumerator());
                }
                break;
            }
            case EDrawAttributeLineExtent:
            {
                m_arAttributes[i_iAttribute].m_val = i_strVal.toInt();
                break;
            }
            case EDrawAttributeLineP1EndStyle:
            case EDrawAttributeLineP2EndStyle:
            {
                bool bConverted = false;
                CEnumLineEndStyle lineEndStyle = CEnumLineEndStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeLineP1ArrowHeadBaseLineType:
            case EDrawAttributeLineP2ArrowHeadBaseLineType:
            {
                bool bConverted = false;
                CEnumArrowHeadBaseLineType lineEndBaseLineType = CEnumArrowHeadBaseLineType::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndBaseLineType.enumerator());
                }
                break;
            }
            case EDrawAttributeLineP1ArrowHeadFillStyle:
            case EDrawAttributeLineP2ArrowHeadFillStyle:
            {
                bool bConverted = false;
                CEnumArrowHeadFillStyle lineEndFillStyle = CEnumArrowHeadFillStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndFillStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeLineP1ArrowHeadWidth:
            case EDrawAttributeLineP2ArrowHeadWidth:
            {
                bool bConverted = false;
                CEnumArrowHeadWidth lineEndWidth = CEnumArrowHeadWidth::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndWidth.enumerator());
                }
                break;
            }
            case EDrawAttributeLineP1ArrowHeadLength:
            case EDrawAttributeLineP2ArrowHeadLength:
            {
                bool bConverted = false;
                CEnumArrowHeadLength lineEndLength = CEnumArrowHeadLength::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndLength.enumerator());
                }
                break;
            }
            case EDrawAttributeTextColor:
            {
                m_arAttributes[i_iAttribute].m_val = QColor(i_strVal);
                break;
            }
            case EDrawAttributeTextFont:
            {
                QFont fnt;
                fnt.fromString(i_strVal);
                m_arAttributes[i_iAttribute].m_val = fnt;
                break;
            }
            case EDrawAttributeTextStyle:
            {
                bool bConverted = false;
                CEnumTextStyle textStyle = CEnumTextStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(textStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeTextSize:
            {
                ETextSize textSize = str2TextSize(i_strVal);
                if( textSize >= 0 && textSize < ETextSizeCount )
                {
                    m_arAttributes[i_iAttribute].m_val = textSize;
                }
                break;
            }
            case EDrawAttributeTextEffect:
            {
                bool bConverted = false;
                CEnumTextEffect textEffect = CEnumTextEffect::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(textEffect.enumerator());
                }
                break;
            }
            default:
            {
                QVariant valTmp(i_strVal);
                if( valTmp.canConvert(m_arAttributes[i_iAttribute].m_val.type()) )
                {
                    m_arAttributes[i_iAttribute].m_val = valTmp.convert(m_arAttributes[i_iAttribute].m_val.type());
                }
                break;
            }
        }
    }
} // setAttribute

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( const QString& i_strName, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        m_arAttributes[iAttribute] = i_val;
    }
}

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( const QString& i_strName, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        setAttribute(iAttribute, i_strVal);
    }
}

//------------------------------------------------------------------------------
SDrawAttribute CDrawSettings::getAttribute( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    SDrawAttribute drawAttribute;
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        drawAttribute = m_arAttributes[i_iAttribute];
    }
    return drawAttribute;
}

//------------------------------------------------------------------------------
SDrawAttribute CDrawSettings::getAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    SDrawAttribute drawAttribute;
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        drawAttribute = m_arAttributes[iAttribute];
    }
    return drawAttribute;
}

//------------------------------------------------------------------------------
QString CDrawSettings::attributeValue2Str( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QString strVal;

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        switch( i_iAttribute )
        {
            case EDrawAttributePenColor:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.value<QColor>().name();
                break;
            }
            case EDrawAttributePenWidth:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.toString();
                break;
            }
            case EDrawAttributeLineStyle:
            {
                strVal = CEnumLineStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeFillColor:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.value<QColor>().name();
                break;
            }
            case EDrawAttributeFillStyle:
            {
                strVal = CEnumFillStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineRecordType:
            {
                strVal = CEnumLineRecordType(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineExtent:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.toString();
                break;
            }
            case EDrawAttributeLineP1EndStyle:
            {
                strVal = CEnumLineEndStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP1ArrowHeadBaseLineType:
            {
                strVal = CEnumArrowHeadBaseLineType(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP1ArrowHeadFillStyle:
            {
                strVal = CEnumArrowHeadFillStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP1ArrowHeadWidth:
            {
                strVal = CEnumArrowHeadWidth(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP1ArrowHeadLength:
            {
                strVal = CEnumArrowHeadLength(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP2EndStyle:
            {
                strVal = CEnumLineEndStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP2ArrowHeadBaseLineType:
            {
                strVal = CEnumArrowHeadBaseLineType(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP2ArrowHeadFillStyle:
            {
                strVal = CEnumArrowHeadFillStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP2ArrowHeadWidth:
            {
                strVal = CEnumArrowHeadWidth(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineP2ArrowHeadLength:
            {
                strVal = CEnumArrowHeadLength(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeTextColor:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.value<QColor>().name();
                break;
            }
            case EDrawAttributeTextFont:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.value<QFont>().toString();
                break;
            }
            case EDrawAttributeTextStyle:
            {
                strVal = CEnumTextStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeTextSize:
            {
                strVal = textSize2Str(static_cast<ETextSize>(m_arAttributes[i_iAttribute].m_val.toInt()));
                break;
            }
            case EDrawAttributeTextEffect:
            {
                strVal = CEnumTextEffect(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            default:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.toString();
                break;
            }
        }
    }
    return strVal;

} // attributeValue2Str

//------------------------------------------------------------------------------
QString CDrawSettings::attributeValue2Str( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QString strVal;
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        strVal = attributeValue2Str(iAttribute);
    }
    return strVal;
}

//------------------------------------------------------------------------------
int CDrawSettings::findAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    int iAttribute = -1;
    for (int idx = 0; idx < EDrawAttributeCount; idx++) {
        if (i_strName.compare(m_arAttributes[idx].m_strName,Qt::CaseInsensitive) == 0) {
            iAttribute = idx;
            break;
        }
    }
    return iAttribute;
}

//------------------------------------------------------------------------------
int CDrawSettings::getAttributeCount() const
//------------------------------------------------------------------------------
{
    return EDrawAttributeCount;
}

//------------------------------------------------------------------------------
QString CDrawSettings::getAttributeName( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QString strName = "Undefined";
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        strName = m_arAttributes[i_iAttribute].m_strName;
    }
    return strName;
}

//------------------------------------------------------------------------------
QVariant CDrawSettings::getAttributeValue( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QVariant val;
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        val = m_arAttributes[i_iAttribute].m_val;
    }
    return val;
}

//------------------------------------------------------------------------------
QVariant CDrawSettings::getAttributeValue( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QVariant val;
    int iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        val = m_arAttributes[iAttribute].m_val;
    }
    return val;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawSettings::setAttributesAreUsed( bool i_bIsUsed, int i_iAttributeMin, int i_iAttributesCount )
////------------------------------------------------------------------------------
//{
//    int iAttributeMin = i_iAttributeMin;
//    int iAttributeMax = iAttributeMin + i_iAttributesCount -1;
//    if (iAttributeMax >= EDrawAttributeCount) {
//        iAttributeMax = EDrawAttributeCount-1;
//    }
//    if (iAttributeMax >= iAttributeMin)
//    {
//        for (int iAttribute = iAttributeMin; iAttribute <= iAttributeMax; iAttribute++) {
//            m_arAttributes[iAttribute].m_bIsUsed = i_bIsUsed;
//        }
//    }
//}

////------------------------------------------------------------------------------
//void CDrawSettings::setAttributeIsUsed( int i_iAttribute, bool i_bIsUsed )
////------------------------------------------------------------------------------
//{
//    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
//        m_arAttributes[i_iAttribute].m_bIsUsed = i_bIsUsed;
//    }
//}

////------------------------------------------------------------------------------
//void CDrawSettings::setAttributeIsUsed( const QString& i_strName, bool i_bIsUsed )
////------------------------------------------------------------------------------
//{
//    int iAttribute = findAttribute(i_strName);
//    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
//        m_arAttributes[iAttribute].m_bIsUsed = i_bIsUsed;
//    }
//}

//------------------------------------------------------------------------------
bool CDrawSettings::isAttributeUsed( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;
    if (i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount) {
        bIsUsed = m_arAttributes[i_iAttribute].m_bIsUsed;
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isAttributeUsed( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;
    int  iAttribute = findAttribute(i_strName);
    if (iAttribute >= 0 && iAttribute < EDrawAttributeCount) {
        bIsUsed = isAttributeUsed(iAttribute);
    }
    return bIsUsed;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawSettings::isPenUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    for (int idxAttr = EDrawAttributePenMin; idxAttr <= EDrawAttributePenMax; idxAttr++) {
        if (!m_arAttributes[idxAttr].m_bIsUsed) {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isFillUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    for (int idxAttr = EDrawAttributeFillMin; idxAttr <= EDrawAttributeFillMax; idxAttr++) {
        if (!m_arAttributes[idxAttr].m_bIsUsed) {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isLineStyleUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    if (!m_arAttributes[EDrawAttributeLineStyle].m_bIsUsed) {
        bIsUsed = false;
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isLineRecordTypeUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    for (int idxAttr = EDrawAttributeLineRecordTypeMin; idxAttr <= EDrawAttributeLineRecordTypeMax; idxAttr++) {
        if (!m_arAttributes[idxAttr].m_bIsUsed) {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isLineEndStyleUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    if (!m_arAttributes[EDrawAttributeLineP1EndStyle].m_bIsUsed && !m_arAttributes[EDrawAttributeLineP2EndStyle].m_bIsUsed) {
        bIsUsed = false;
    }
    return bIsUsed;
}

//------------------------------------------------------------------------------
bool CDrawSettings::isTextUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    for (int idxAttr = EDrawAttributeTextMin; idxAttr <= EDrawAttributeTextMax; idxAttr++) {
        if (!m_arAttributes[idxAttr].m_bIsUsed) {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setPenColor( const QColor& i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributePenColor] = i_val;
}

//------------------------------------------------------------------------------
QColor CDrawSettings::getPenColor() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributePenColor].m_val.value<QColor>();
}

//------------------------------------------------------------------------------
void CDrawSettings::setPenWidth( int i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributePenWidth] = i_val;
}

//------------------------------------------------------------------------------
int CDrawSettings::getPenWidth() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributePenWidth].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setLineStyle( const CEnumLineStyle& i_lineStyle )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineStyle] = i_lineStyle.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumLineStyle CDrawSettings::getLineStyle() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeLineStyle].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setFillColor( const QColor& i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeFillColor] = i_val;
}

//------------------------------------------------------------------------------
QColor CDrawSettings::getFillColor() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeFillColor].m_val.value<QColor>();
}

//------------------------------------------------------------------------------
void CDrawSettings::setFillStyle( const CEnumFillStyle& i_fillStyle )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeFillStyle] = i_fillStyle.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumFillStyle CDrawSettings::getFillStyle() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeFillStyle].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setLineRecordType( const CEnumLineRecordType& i_recordType )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineRecordType] = i_recordType.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumLineRecordType CDrawSettings::getLineRecordType() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeLineRecordType].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineExtent( int i_iExtent )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineExtent] = i_iExtent;
}

//------------------------------------------------------------------------------
int CDrawSettings::getLineExtent() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeLineExtent].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndStyle( const CEnumLinePoint& i_linePoint, const CEnumLineEndStyle& i_endStyle )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1EndStyle;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2EndStyle;
    }
    m_arAttributes[attr] = i_endStyle.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumLineEndStyle CDrawSettings::getLineEndStyle( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1EndStyle;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2EndStyle;
    }
    return m_arAttributes[attr].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndBaseLineType(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadBaseLineType& i_baseLineType )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadBaseLineType;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadBaseLineType;
    }
    m_arAttributes[attr] = i_baseLineType.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumArrowHeadBaseLineType CDrawSettings::getLineEndBaseLineType( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadBaseLineType;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadBaseLineType;
    }
    return m_arAttributes[attr].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndFillStyle(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadFillStyle& i_fillStyle )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadFillStyle;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadFillStyle;
    }
    m_arAttributes[attr] = i_fillStyle.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumArrowHeadFillStyle CDrawSettings::getLineEndFillStyle( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadFillStyle;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadFillStyle;
    }
    return m_arAttributes[attr].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndWidth(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadWidth& i_width )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadWidth;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadWidth;
    }
    m_arAttributes[attr] = i_width.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumArrowHeadWidth CDrawSettings::getLineEndWidth( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadWidth;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadWidth;
    }
    return m_arAttributes[attr].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndLength(
    const CEnumLinePoint& i_linePoint, const CEnumArrowHeadLength& i_length )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadLength;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadLength;
    }
    m_arAttributes[attr] = i_length.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumArrowHeadLength CDrawSettings::getLineEndLength( const CEnumLinePoint& i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineP1ArrowHeadLength;
    if (i_linePoint != ELinePoint::Start) {
        attr = EDrawAttributeLineP2ArrowHeadLength;
    }
    return m_arAttributes[attr].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setTextColor( const QColor& i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextColor] = i_val;
}

//------------------------------------------------------------------------------
QColor CDrawSettings::getTextColor() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeTextColor].m_val.value<QColor>();
}

//------------------------------------------------------------------------------
void CDrawSettings::setTextFont( const QFont& i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextFont] = i_val;
}

//------------------------------------------------------------------------------
QFont CDrawSettings::getTextFont() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeTextFont].m_val.value<QFont>();
}

//------------------------------------------------------------------------------
void CDrawSettings::setTextStyle( const CEnumTextStyle& i_textStyle )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextStyle] = i_textStyle.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumTextStyle CDrawSettings::getTextStyle() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeTextStyle].m_val.toInt();
}

//------------------------------------------------------------------------------
void CDrawSettings::setTextSize( ETextSize i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextSize] = i_val;
}

//------------------------------------------------------------------------------
ETextSize CDrawSettings::getTextSize() const
//------------------------------------------------------------------------------
{
    return static_cast<ETextSize>(m_arAttributes[EDrawAttributeTextSize].m_val.toInt());
}

//------------------------------------------------------------------------------
void CDrawSettings::setTextEffect( const CEnumTextEffect& i_textEffect )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextEffect] = i_textEffect.enumeratorAsInt();
}

//------------------------------------------------------------------------------
CEnumTextEffect CDrawSettings::getTextEffect() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EDrawAttributeTextEffect].m_val.toInt();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawSettings::toString() const
//------------------------------------------------------------------------------
{
    QString str = "GraphObjType: " + graphObjType2Str(m_graphObjType);
    int iSectionsPrint = 0;
    if (isPenUsed()) {
        str += ", Pen {";
        for (int iAttribute = EDrawAttributePenMin; iAttribute <= EDrawAttributePenMax; ++iAttribute) {
            if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                QString strAttr = drawAttribute2Str(iAttribute);
                strAttr = strAttr.remove("Pen");
                if (!str.endsWith("{")) str += ", ";
                str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                iSectionsPrint++;
            }
        }
        str += "}";
    }
    if (isFillUsed()) {
        str += ", Fill {";
        for (int iAttribute = EDrawAttributeFillMin; iAttribute <= EDrawAttributeFillMax; ++iAttribute) {
            if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                QString strAttr = drawAttribute2Str(iAttribute);
                strAttr = strAttr.remove("Fill");
                if (!str.endsWith("{")) str += ", ";
                str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                iSectionsPrint++;
            }
        }
        str += "}";
    }
    if (isLineStyleUsed()) {
        str += ", Line {";
        int iAttribute = EDrawAttributeLineStyle;
        if (IsAttributeUsed(m_graphObjType, iAttribute)) {
            QString strAttr = drawAttribute2Str(iAttribute);
            strAttr = strAttr.remove("Line");
            if (!str.endsWith("{")) str += ", ";
            str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
            iSectionsPrint++;
        }
        if (isLineEndStyleUsed()) {
            str += ", EndStyle P1 {";
            iAttribute = EDrawAttributeLineP1EndStyle;
            if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                QString strAttr = drawAttribute2Str(iAttribute);
                strAttr = strAttr.remove("LineP1EndStyle");
                if (!str.endsWith("{")) str += ", ";
                str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                iSectionsPrint++;
            }
            str += ", ArrowHead {";
            for (int iAttribute = EDrawAttributeLineP1ArrowHeadMin; iAttribute <= EDrawAttributeLineP1ArrowHeadMax; ++iAttribute) {
                if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                    QString strAttr = drawAttribute2Str(iAttribute);
                    strAttr = strAttr.remove("LineP1ArrowHead");
                    if (!str.endsWith("{")) str += ", ";
                    str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                    iSectionsPrint++;
                }
            }
            str += "}} P2 {";
            iAttribute = EDrawAttributeLineP2EndStyle;
            if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                QString strAttr = drawAttribute2Str(iAttribute);
                strAttr = strAttr.remove("LineP2EndStyle");
                if (!str.endsWith("{")) str += ", ";
                str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                iSectionsPrint++;
            }
            str += ", ArrowHead {";
            for (int iAttribute = EDrawAttributeLineP2ArrowHeadMin; iAttribute <= EDrawAttributeLineP2ArrowHeadMax; ++iAttribute) {
                if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                    QString strAttr = drawAttribute2Str(iAttribute);
                    strAttr = strAttr.remove("LineP2ArrowHead");
                    if (!str.endsWith("{")) str += ", ";
                    str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                    iSectionsPrint++;
                }
            }
            str += "}}";
        }
        if (isLineRecordTypeUsed()) {
            str += "RecordType {";
            for (int iAttribute = EDrawAttributeLineRecordTypeMin; iAttribute <= EDrawAttributeLineRecordTypeMax; ++iAttribute) {
                if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                    QString strAttr = drawAttribute2Str(iAttribute);
                    strAttr = strAttr.remove("Line");
                    strAttr = strAttr.remove("RecordType");
                    if (!str.endsWith("{")) str += ", ";
                    str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                    iSectionsPrint++;
                }
            }
            str += "}";
        }
        str += "}";
    }
    if (isTextUsed()) {
        str += ", Text {";
        for (int iAttribute = EDrawAttributeTextMin; iAttribute <= EDrawAttributeTextMax; ++iAttribute) {
            if (IsAttributeUsed(m_graphObjType, iAttribute)) {
                QString strAttr = drawAttribute2Str(iAttribute);
                strAttr = strAttr.remove("Text");
                if (!str.endsWith("{")) str += ", ";
                str += ", " + strAttr + ": " + attributeValue2Str(iAttribute);
                iSectionsPrint++;
            }
        }
        str += "}";
    }
    return str;
}
