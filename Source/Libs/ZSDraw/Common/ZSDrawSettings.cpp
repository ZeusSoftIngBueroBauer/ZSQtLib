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

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/Common/ZSDrawSettings.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

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
    /*  2 */ SEnumEntry( EDrawAttributeLineStyle,                    "LineStyle"                    ),
    /*  3 */ SEnumEntry( EDrawAttributeFillColor,                    "FillColor"                    ),
    /*  4 */ SEnumEntry( EDrawAttributeFillStyle,                    "FillStyle"                    ),
    /*  5 */ SEnumEntry( EDrawAttributeLineRecordType,               "LineRecordType"               ),
    /*  6 */ SEnumEntry( EDrawAttributeLineExtent,                   "LineExtent"                   ),
    /*  7 */ SEnumEntry( EDrawAttributeLineStartLineEndStyle,        "LineStartLineEndStyle"        ),
    /*  8 */ SEnumEntry( EDrawAttributeLineStartLineEndBaseLineType, "LineStartLineEndBaseLineType" ),
    /*  9 */ SEnumEntry( EDrawAttributeLineStartLineEndFillStyle,    "LineStartLineEndFillStyle"    ),
    /* 10 */ SEnumEntry( EDrawAttributeLineStartLineEndWidth,        "LineStartLineEndWidth"        ),
    /* 11 */ SEnumEntry( EDrawAttributeLineStartLineEndLength,       "LineStartLineEndLength"       ),
    /* 12 */ SEnumEntry( EDrawAttributeLineEndLineEndStyle,          "LineEndLineEndStyle"          ),
    /* 13 */ SEnumEntry( EDrawAttributeLineEndLineEndBaseLineType,   "LineEndLineEndBaseLineType"   ),
    /* 14 */ SEnumEntry( EDrawAttributeLineEndLineEndFillStyle,      "LineEndLineEndFillStyle"      ),
    /* 15 */ SEnumEntry( EDrawAttributeLineEndLineEndWidth,          "LineEndLineEndWidth"          ),
    /* 16 */ SEnumEntry( EDrawAttributeLineEndLineEndLength,         "LineEndLineEndLength"         ),
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
    return SEnumEntry::enumerator2Str(s_arEnumStrDrawAttributes,_ZSArrLen(s_arEnumStrDrawAttributes),i_iAttribute);
}

//------------------------------------------------------------------------------
EDrawAttribute ZS::Draw::str2DrawAttribute( const QString& i_str )
//------------------------------------------------------------------------------
{
    EDrawAttribute drawAttribute = EDrawAttributeUndefined;
    int iDrawAttribute = SEnumEntry::str2Enumerator(s_arEnumStrDrawAttributes,_ZSArrLen(s_arEnumStrDrawAttributes),i_str);
    if( iDrawAttribute >= 0 && iDrawAttribute < _ZSArrLen(s_arEnumStrDrawAttributes) )
    {
        drawAttribute = static_cast<EDrawAttribute>(iDrawAttribute);
    }
    else
    {
        bool bConverted;
        iDrawAttribute = i_str.toInt(&bConverted);
        if( bConverted && iDrawAttribute >= 0 && iDrawAttribute < _ZSArrLen(s_arEnumStrDrawAttributes) )
        {
            drawAttribute = static_cast<EDrawAttribute>(iDrawAttribute);
        }
    }
    return drawAttribute;

} // str2DrawAttribute


/*******************************************************************************
class CDrawSettings
*******************************************************************************/

//------------------------------------------------------------------------------
static const SDrawAttribute s_arDrawAttributesDefaultValsGraphObjTypeUndefined[EDrawAttributeCount] =
//------------------------------------------------------------------------------
{   // For Undefined GraphObjTypes (used by the drawing scene) each of the attributes is used as default:
    /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
    /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
    /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
    /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
    /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
    /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
    /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
    /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
    /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
    /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
    /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
    /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
    /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
    /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
    /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
    /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
    /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
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
    /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
    /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
    /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
    /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
    /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
    /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
    /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
    /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
    /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
    /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
    /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
    /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
    /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
    /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
    /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
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
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  1 EGraphObjTypeLine            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  2 EGraphObjTypeRect            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  3 EGraphObjTypeEllipse         */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  4 EGraphObjTypePolygon         */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), true ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  5 EGraphObjTypePolyline        */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  6 EGraphObjTypeText            */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), true ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), true ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), true ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, true ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), true )
    /*                                 */ },
    /*  7 EGraphObjTypeImage           */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  8 EGraphObjTypeConnectionPoint */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::black), true ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::SolidPattern), true ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /*  9 EGraphObjTypeConnectionLine  */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), true ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, true),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), true ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), true ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, true ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), true ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), true ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), true ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), true ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), true ),
        /* 17 TextColor                    */ SDrawAttribute( "TextColor", QColor(Qt::black), false ),
        /* 18 TextFont                     */ SDrawAttribute( "TextFont", QFont("Arial"), false ),
        /* 19 TextStyle                    */ SDrawAttribute( "TextStyle", static_cast<int>(ETextStyle::Normal), false ),
        /* 20 TextSize                     */ SDrawAttribute( "TextSize", ETextSize11, false ),
        /* 21 TextEffect                   */ SDrawAttribute( "TextEffect", static_cast<int>(ETextEffect::None), false )
    /*                                 */ },
    /* 10 EGraphObjTypeGroup           */ {
        /*  0 PenColor                     */ SDrawAttribute( "PenColor", QColor(Qt::black), false ),
        /*  1 PenWidth                     */ SDrawAttribute( "PenWidth", 1, false),
        /*  2 LineStyle                    */ SDrawAttribute( "LineStyle", static_cast<int>(ELineStyle::SolidLine), false ),
        /*  3 FillColor                    */ SDrawAttribute( "FillColor", QColor(Qt::white), false ),
        /*  4 FillStyle                    */ SDrawAttribute( "FillStyle", static_cast<int>(EFillStyle::NoFill), false ),
        /*  5 LineRecordType               */ SDrawAttribute( "LineRecordType", static_cast<int>(ELineRecordType::Normal), false ),
        /*  6 LineExtent                   */ SDrawAttribute( "LineExtent", 1, false ),
        /*  7 LineStartLineEndStyle        */ SDrawAttribute( "LineStartLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /*  8 LineStartLineEndBaseLineType */ SDrawAttribute( "LineStartLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /*  9 LineStartLineEndFillStyle    */ SDrawAttribute( "LineStartLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 10 LineStartLineEndWidth        */ SDrawAttribute( "LineStartLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 11 LineStartLineEndLength       */ SDrawAttribute( "LineStartLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
        /* 12 LineEndLineEndStyle          */ SDrawAttribute( "LineEndLineEndStyle", static_cast<int>(ELineEndStyle::Normal), false ),
        /* 13 LineEndLineEndBaseLineType   */ SDrawAttribute( "LineEndLineEndBaseLineType", static_cast<int>(ELineEndBaseLineType::NoLine), false ),
        /* 14 LineEndLineEndFillStyle      */ SDrawAttribute( "LineEndLineEndFillStyle", static_cast<int>(ELineEndFillStyle::NoFill), false ),
        /* 15 LineEndLineEndWidth          */ SDrawAttribute( "LineEndLineEndWidth", static_cast<int>(ELineEndWidth::Medium), false ),
        /* 16 LineEndLineEndLength         */ SDrawAttribute( "LineEndLineEndLength", static_cast<int>(ELineEndLength::Medium), false ),
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

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        int idxGraphObjType = i_graphObjType - EGraphObjTypeMin;

        if( idxGraphObjType >= 0 && idxGraphObjType < EGraphObjTypeCount )
        {
            bIsUsed = s_ararDrawAttributesDefaultVals[idxGraphObjType][i_iAttribute].m_bIsUsed;
        }
        else if( i_graphObjType == EGraphObjTypeUndefined )
        {
            bIsUsed = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute].m_bIsUsed;
        }
        else // if( i_graphObjType >= EGraphObjTypeUserDefined )
        {
            bIsUsed = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute].m_bIsUsed;
        }
    }
    return bIsUsed;

} // IsAttributeUsed

//------------------------------------------------------------------------------
bool CDrawSettings::IsAttributeUsed( EGraphObjType i_graphObjType, const QString& i_strName )
//------------------------------------------------------------------------------
{
    int iAttribute = str2DrawAttribute(i_strName);

    return IsAttributeUsed(i_graphObjType,iAttribute);

} // IsAttributeUsed

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawSettings::CDrawSettings( EGraphObjType i_graphObjType ) :
//------------------------------------------------------------------------------
    m_graphObjType(i_graphObjType),
    m_idxGraphObjType(i_graphObjType-EGraphObjTypeMin)
    //m_arAttributes[EDrawAttributeCount]
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

    int idxAttr;

    for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        if( m_arAttributes[idxAttr].m_bIsUsed )
        {
            if( m_arAttributes[idxAttr].m_val != i_other.m_arAttributes[idxAttr].m_val )
            {
                bEqual = false;
                break;
            }
        }
    }
    return bEqual;

} // operator ==

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
    QString strAttrName;
    QString strAttrValue;
    int     idxAttr;

    for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        strAttrName = getAttributeName(idxAttr);
        strAttrValue = attributeValue2Str(idxAttr); // current value

        i_settings.setValue( i_strSettingsKey + "/" + strAttrName, strAttrValue );
    }

} // save

//------------------------------------------------------------------------------
void CDrawSettings::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    QString strAttrName;
    QString strAttrValue;
    int     idxAttr;

    for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        strAttrName = getAttributeName(idxAttr);
        strAttrValue = attributeValue2Str(idxAttr); // current value used as default for "settings.value"

        strAttrValue = i_settings.value( i_strSettingsKey + "/" + strAttrName, strAttrValue ).toString();

        setAttribute(strAttrName,strAttrValue);
    }

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr;
    int            idxAttr;
    bool           bSave;
    QString        strName;
    QString        strValue;

    for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        attr = static_cast<EDrawAttribute>(idxAttr);

        bSave = true;

        if( isAttributeUsed(attr) && !isDefault(attr) )
        {
            switch( attr )
            {
                case EDrawAttributeTextColor:
                case EDrawAttributeTextFont:
                case EDrawAttributeTextStyle:
                case EDrawAttributeTextSize:
                case EDrawAttributeTextEffect:
                {
                    if( !isTextUsed() )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeFillColor:
                case EDrawAttributeFillStyle:
                {
                    if( !isFillUsed() )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributePenColor:
                case EDrawAttributePenWidth:
                {
                    if( !isPenUsed() )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineStyle:
                case EDrawAttributeLineRecordType:
                {
                    if( !isPenUsed() )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineExtent:
                {
                    if( !isPenUsed() || getLineRecordType() == ELineRecordType::Normal )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineStartLineEndStyle:
                case EDrawAttributeLineStartLineEndBaseLineType:
                case EDrawAttributeLineStartLineEndFillStyle:
                case EDrawAttributeLineStartLineEndWidth:
                case EDrawAttributeLineStartLineEndLength:
                {
                    if( !isPenUsed() || getLineEndStyle(ELinePoint::Start) == ELineEndStyle::Normal )
                    {
                        bSave = false;
                    }
                    break;
                }
                case EDrawAttributeLineEndLineEndStyle:
                case EDrawAttributeLineEndLineEndBaseLineType:
                case EDrawAttributeLineEndLineEndFillStyle:
                case EDrawAttributeLineEndLineEndWidth:
                case EDrawAttributeLineEndLineEndLength:
                {
                    if( !isPenUsed() || getLineEndStyle(ELinePoint::End) == ELineEndStyle::Normal )
                    {
                        bSave = false;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            if( bSave )
            {
                strName = getAttributeName(idxAttr);
                strValue = attributeValue2Str(idxAttr);

                i_xmlStreamWriter.writeTextElement(strName,strValue);
            }
        }
    }

} // save

//------------------------------------------------------------------------------
void CDrawSettings::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QXmlStreamReader::TokenType xmlStreamTokenType;

    QString strElemName;
    QString strElemText;
    QString strName;
    QString strVal;
    int     iAttribute;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )
        {
            strElemName = i_xmlStreamReader.name().toString();

            if( i_xmlStreamReader.isStartElement() )
            {
                if( strElemName == "DrawSettings" )
                {
                    for( iAttribute = 0; iAttribute < EDrawAttributeCount; ++iAttribute)
                    {
                        strName = drawAttribute2Str(iAttribute);

                        if( i_xmlStreamReader.attributes().hasAttribute(strName) )
                        {
                            strVal = i_xmlStreamReader.attributes().value(strName).toString();
                            setAttribute(iAttribute, strVal);
                        }
                    }
                }
            } // if( i_xmlStreamReader.isStartElement() )

            else if( i_xmlStreamReader.isEndElement() )
            {
                if( strElemName == "DrawSettings" )
                {
                    break;
                }
            } // if( i_xmlStreamReader.isEndElement() )
        } // if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )

        xmlStreamTokenType = i_xmlStreamReader.readNext();

    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::set2Default()
//------------------------------------------------------------------------------
{
    int idxAttr;

    if( m_idxGraphObjType >= 0 && m_idxGraphObjType < EGraphObjTypeCount )
    {
        for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
        {
            m_arAttributes[idxAttr] = s_ararDrawAttributesDefaultVals[m_idxGraphObjType][idxAttr];
        }
    }
    else if( m_graphObjType == EGraphObjTypeUndefined )
    {
        for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
        {
            m_arAttributes[idxAttr] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[idxAttr];
        }
    }
    else // if( m_graphObjType >= EGraphObjTypeUserDefined )
    {
        for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
        {
            m_arAttributes[idxAttr] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[idxAttr];
        }
    }

} // set2Default

//------------------------------------------------------------------------------
void CDrawSettings::set2Default( int i_iAttribute )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        if( m_idxGraphObjType >= 0 && m_idxGraphObjType < EGraphObjTypeCount )
        {
            m_arAttributes[i_iAttribute] = s_ararDrawAttributesDefaultVals[m_idxGraphObjType][i_iAttribute];
        }
        else if( m_graphObjType == EGraphObjTypeUndefined )
        {
            m_arAttributes[i_iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute];
        }
        else // if( m_graphObjType >= EGraphObjTypeUserDefined )
        {
            m_arAttributes[i_iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute];
        }
    }

} // set2Default

//------------------------------------------------------------------------------
void CDrawSettings::set2Default( const QString& i_strName )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        if( m_idxGraphObjType >= 0 && m_idxGraphObjType < EGraphObjTypeCount )
        {
            m_arAttributes[iAttribute] = s_ararDrawAttributesDefaultVals[m_idxGraphObjType][iAttribute];
        }
        else if( m_graphObjType == EGraphObjTypeUndefined )
        {
            m_arAttributes[iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUndefined[iAttribute];
        }
        else // if( m_graphObjType >= EGraphObjTypeUserDefined )
        {
            m_arAttributes[iAttribute] = s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[iAttribute];
        }
    }

} // set2Default

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault() const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    int idxAttr;

    for( idxAttr = 0; idxAttr < EDrawAttributeCount; idxAttr++ )
    {
        if( isAttributeUsed(idxAttr) )
        {
            if( m_idxGraphObjType >= 0 && m_idxGraphObjType < EGraphObjTypeCount )
            {
                if( m_arAttributes[idxAttr] != s_ararDrawAttributesDefaultVals[m_idxGraphObjType][idxAttr] )
                {
                    bIsDefault = false;
                    break;
                }
            }
            else if( m_graphObjType == EGraphObjTypeUndefined )
            {
                if( m_arAttributes[idxAttr] != s_arDrawAttributesDefaultValsGraphObjTypeUndefined[idxAttr] )
                {
                    bIsDefault = false;
                    break;
                }
            }
            else // if( m_graphObjType >= EGraphObjTypeUserDefined )
            {
                if( m_arAttributes[idxAttr] != s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[idxAttr] )
                {
                    bIsDefault = false;
                    break;
                }
            }
        }
    }
    return bIsDefault;

} // isDefault

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        if( isAttributeUsed(i_iAttribute) )
        {
            if( m_idxGraphObjType >= 0 && m_idxGraphObjType < EGraphObjTypeCount )
            {
                if( m_arAttributes[i_iAttribute] != s_ararDrawAttributesDefaultVals[m_idxGraphObjType][i_iAttribute] )
                {
                    bIsDefault = false;
                }
            }
            else if( m_graphObjType == EGraphObjTypeUndefined )
            {
                if( m_arAttributes[i_iAttribute] != s_arDrawAttributesDefaultValsGraphObjTypeUndefined[i_iAttribute] )
                {
                    bIsDefault = false;
                }
            }
            else // if( m_graphObjType >= EGraphObjTypeUserDefined )
            {
                if( m_arAttributes[i_iAttribute] != s_arDrawAttributesDefaultValsGraphObjTypeUserDefined[i_iAttribute] )
                {
                    bIsDefault = false;
                }
            }
        }
    }
    return bIsDefault;

} // isDefault

//------------------------------------------------------------------------------
bool CDrawSettings::isDefault( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        bIsDefault = isDefault(iAttribute);
    }
    return bIsDefault;

} // isDefault

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const SDrawAttribute& i_drawAttribute )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        m_arAttributes[i_iAttribute] = i_drawAttribute;
    }

} // setAttribute

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        if( i_val.type() == QVariant::String )
        {
            setAttribute( i_iAttribute, i_val.toString() );
        }
        else
        {
            m_arAttributes[i_iAttribute] = i_val;
        }
    }

} // setAttribute

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( int i_iAttribute, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
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
            case EDrawAttributeLineStartLineEndStyle:
            case EDrawAttributeLineEndLineEndStyle:
            {
                bool bConverted = false;
                CEnumLineEndStyle lineEndStyle = CEnumLineEndStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeLineStartLineEndBaseLineType:
            case EDrawAttributeLineEndLineEndBaseLineType:
            {
                bool bConverted = false;
                CEnumLineEndBaseLineType lineEndBaseLineType = CEnumLineEndBaseLineType::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndBaseLineType.enumerator());
                }
                break;
            }
            case EDrawAttributeLineStartLineEndFillStyle:
            case EDrawAttributeLineEndLineEndFillStyle:
            {
                bool bConverted = false;
                CEnumLineEndFillStyle lineEndFillStyle = CEnumLineEndFillStyle::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndFillStyle.enumerator());
                }
                break;
            }
            case EDrawAttributeLineStartLineEndWidth:
            case EDrawAttributeLineEndLineEndWidth:
            {
                bool bConverted = false;
                CEnumLineEndWidth lineEndWidth = CEnumLineEndWidth::fromString(i_strVal, &bConverted);
                if( bConverted )
                {
                    m_arAttributes[i_iAttribute].m_val = static_cast<int>(lineEndWidth.enumerator());
                }
                break;
            }
            case EDrawAttributeLineStartLineEndLength:
            case EDrawAttributeLineEndLineEndLength:
            {
                bool bConverted = false;
                CEnumLineEndLength lineEndLength = CEnumLineEndLength::fromString(i_strVal, &bConverted);
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

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        m_arAttributes[iAttribute] = i_val;
    }

} // setAttribute

//------------------------------------------------------------------------------
void CDrawSettings::setAttribute( const QString& i_strName, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        setAttribute(iAttribute,i_strVal);
    }

} // setAttribute

//------------------------------------------------------------------------------
SDrawAttribute CDrawSettings::getAttribute( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    SDrawAttribute drawAttribute;

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        drawAttribute = m_arAttributes[i_iAttribute];
    }
    return drawAttribute;

} // getAttribute

//------------------------------------------------------------------------------
SDrawAttribute CDrawSettings::getAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    SDrawAttribute drawAttribute;
    int        iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        drawAttribute = m_arAttributes[iAttribute];
    }
    return drawAttribute;

} // getAttribute

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
            case EDrawAttributeLineStartLineEndStyle:
            {
                strVal = CEnumLineEndStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineStartLineEndBaseLineType:
            {
                strVal = CEnumLineEndBaseLineType(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineStartLineEndFillStyle:
            {
                strVal = CEnumLineEndFillStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineStartLineEndWidth:
            {
                strVal = CEnumLineEndWidth(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineStartLineEndLength:
            {
                strVal = CEnumLineEndLength(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineEndLineEndStyle:
            {
                strVal = CEnumLineEndStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineEndLineEndBaseLineType:
            {
                strVal = CEnumLineEndBaseLineType(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineEndLineEndFillStyle:
            {
                strVal = CEnumLineEndFillStyle(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineEndLineEndWidth:
            {
                strVal = CEnumLineEndWidth(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
                break;
            }
            case EDrawAttributeLineEndLineEndLength:
            {
                strVal = CEnumLineEndLength(m_arAttributes[i_iAttribute].m_val.toInt()).toString();
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
    int     iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        strVal = attributeValue2Str(iAttribute);
    }
    return strVal;

} // attributeValue2Str

//------------------------------------------------------------------------------
int CDrawSettings::findAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    int iAttribute = -1;
    int idx;

    for( idx = 0; idx < EDrawAttributeCount; idx++ )
    {
        if( i_strName.compare(m_arAttributes[idx].m_strName,Qt::CaseInsensitive) == 0 )
        {
            iAttribute = idx;
            break;
        }
    }
    return iAttribute;

} // findAttribute

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

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        strName = m_arAttributes[i_iAttribute].m_strName;
    }
    return strName;

} // getAttributeName

//------------------------------------------------------------------------------
QVariant CDrawSettings::getAttributeValue( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QVariant val;

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        val = m_arAttributes[i_iAttribute].m_val;
    }
    return val;

} // getAttributeValue

//------------------------------------------------------------------------------
QVariant CDrawSettings::getAttributeValue( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QVariant val;
    int      iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        val = m_arAttributes[iAttribute].m_val;
    }
    return val;

} // getAttributeValue

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setAttributesAreUsed( bool i_bIsUsed, int i_iAttributeMin, int i_iAttributesCount )
//------------------------------------------------------------------------------
{
    int iAttributeMin = i_iAttributeMin;
    int iAttributeMax = iAttributeMin + i_iAttributesCount -1;
    int iAttribute;

    if( iAttributeMax >= EDrawAttributeCount )
    {
        iAttributeMax = EDrawAttributeCount-1;
    }
    if( iAttributeMax >= iAttributeMin )
    {
        for( iAttribute = iAttributeMin; iAttribute <= iAttributeMax; iAttribute++ )
        {
            m_arAttributes[iAttribute].m_bIsUsed = i_bIsUsed;
        }
    }

} // setAttributesAreUsed

//------------------------------------------------------------------------------
void CDrawSettings::setAttributeIsUsed( int i_iAttribute, bool i_bIsUsed )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        m_arAttributes[i_iAttribute].m_bIsUsed = i_bIsUsed;
    }

} // setAttributeIsUsed

//------------------------------------------------------------------------------
void CDrawSettings::setAttributeIsUsed( const QString& i_strName, bool i_bIsUsed )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        m_arAttributes[iAttribute].m_bIsUsed = i_bIsUsed;
    }

} // setAttributeIsUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isAttributeUsed( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;

    if( i_iAttribute >= 0 && i_iAttribute < EDrawAttributeCount )
    {
        bIsUsed = m_arAttributes[i_iAttribute].m_bIsUsed;
    }
    return bIsUsed;

} // isAttributeUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isAttributeUsed( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;
    int  iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EDrawAttributeCount )
    {
        bIsUsed = isAttributeUsed(iAttribute);
    }
    return bIsUsed;

} // isAttributeUsed

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawSettings::isPenUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributePenMin; idxAttr <= EDrawAttributePenMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isPenUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isLineStyleUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributeLineStyleMin; idxAttr <= EDrawAttributeLineStyleMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isLineStyleUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isFillUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributeFillMin; idxAttr <= EDrawAttributeFillMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isFillUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isLineRecordTypeUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributeLineRecordTypeMin; idxAttr <= EDrawAttributeLineRecordTypeMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isLineRecordTypeUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isLineEndStyleUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributeLineEndStyleMin; idxAttr <= EDrawAttributeLineEndStyleMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isLineEndStyleUsed

//------------------------------------------------------------------------------
bool CDrawSettings::isTextUsed() const
//------------------------------------------------------------------------------
{
    bool bIsUsed = true;
    int  idxAttr;

    for( idxAttr = EDrawAttributeTextMin; idxAttr <= EDrawAttributeTextMax; idxAttr++ )
    {
        if( !m_arAttributes[idxAttr].m_bIsUsed )
        {
            bIsUsed = false;
            break;
        }
    }
    return bIsUsed;

} // isTextUsed

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
void CDrawSettings::setLineStyle( ELineStyle i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineStyle] = static_cast<int>(i_val);
}

//------------------------------------------------------------------------------
ELineStyle CDrawSettings::getLineStyle() const
//------------------------------------------------------------------------------
{
    return static_cast<ELineStyle>(m_arAttributes[EDrawAttributeLineStyle].m_val.toInt());
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
void CDrawSettings::setFillStyle( EFillStyle i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeFillStyle] = static_cast<int>(i_val);
}

//------------------------------------------------------------------------------
EFillStyle CDrawSettings::getFillStyle() const
//------------------------------------------------------------------------------
{
    return static_cast<EFillStyle>(m_arAttributes[EDrawAttributeFillStyle].m_val.toInt());
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawSettings::setLineRecordType( ELineRecordType i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineRecordType] = static_cast<int>(i_val);
}

//------------------------------------------------------------------------------
ELineRecordType CDrawSettings::getLineRecordType() const
//------------------------------------------------------------------------------
{
    return static_cast<ELineRecordType>(m_arAttributes[EDrawAttributeLineRecordType].m_val.toInt());
}

//------------------------------------------------------------------------------
void CDrawSettings::setLineExtent( int i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeLineExtent] = i_val;
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
void CDrawSettings::setLineEndStyle( ELinePoint i_linePoint, ELineEndStyle i_val )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndStyle;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndStyle;
    }
    m_arAttributes[attr] = static_cast<int>(i_val);

} // setLineEndStyle

//------------------------------------------------------------------------------
ELineEndStyle CDrawSettings::getLineEndStyle( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndStyle;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndStyle;
    }
    return static_cast<ELineEndStyle>(m_arAttributes[attr].m_val.toInt());

} // getLineEndStyle

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndBaseLineType( ELinePoint i_linePoint, ELineEndBaseLineType i_val )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndBaseLineType;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndBaseLineType;
    }
    m_arAttributes[attr] = static_cast<int>(i_val);

} // setLineEndBaseLineType

//------------------------------------------------------------------------------
ELineEndBaseLineType CDrawSettings::getLineEndBaseLineType( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndBaseLineType;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndBaseLineType;
    }
    return static_cast<ELineEndBaseLineType>(m_arAttributes[attr].m_val.toInt());

} // getLineEndBaseLineType

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndFillStyle( ELinePoint i_linePoint, ELineEndFillStyle i_val )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndFillStyle;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndFillStyle;
    }
    m_arAttributes[attr] = static_cast<int>(i_val);

} // setLineEndFillStyle

//------------------------------------------------------------------------------
ELineEndFillStyle CDrawSettings::getLineEndFillStyle( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndFillStyle;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndFillStyle;
    }
    return static_cast<ELineEndFillStyle>(m_arAttributes[attr].m_val.toInt());

} // getLineEndFillStyle

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndWidth( ELinePoint i_linePoint, ELineEndWidth i_val )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndWidth;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndWidth;
    }
    m_arAttributes[attr] = static_cast<int>(i_val);

} // setLineEndWidth

//------------------------------------------------------------------------------
ELineEndWidth CDrawSettings::getLineEndWidth( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndWidth;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndWidth;
    }
    return static_cast<ELineEndWidth>(m_arAttributes[attr].m_val.toInt());

} // getLineEndWidth

//------------------------------------------------------------------------------
void CDrawSettings::setLineEndLength( ELinePoint i_linePoint, ELineEndLength i_val )
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndLength;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndLength;
    }
    m_arAttributes[attr] = static_cast<int>(i_val);

} // setLineEndLength

//------------------------------------------------------------------------------
ELineEndLength CDrawSettings::getLineEndLength( ELinePoint i_linePoint ) const
//------------------------------------------------------------------------------
{
    EDrawAttribute attr = EDrawAttributeLineStartLineEndLength;

    if( i_linePoint != ELinePoint::Start )
    {
        attr = EDrawAttributeLineEndLineEndLength;
    }
    return static_cast<ELineEndLength>(m_arAttributes[attr].m_val.toInt());

} // getLineEndLength

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
void CDrawSettings::setTextStyle( ETextStyle i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextStyle] = static_cast<int>(i_val);
}

//------------------------------------------------------------------------------
ETextStyle CDrawSettings::getTextStyle() const
//------------------------------------------------------------------------------
{
    return static_cast<ETextStyle>(m_arAttributes[EDrawAttributeTextStyle].m_val.toInt());
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
void CDrawSettings::setTextEffect( ETextEffect i_val )
//------------------------------------------------------------------------------
{
    m_arAttributes[EDrawAttributeTextEffect] = static_cast<int>(i_val);
}

//------------------------------------------------------------------------------
ETextEffect CDrawSettings::getTextEffect() const
//------------------------------------------------------------------------------
{
    return static_cast<ETextEffect>(m_arAttributes[EDrawAttributeTextEffect].m_val.toInt());
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawSettings::toString(EGraphObjType i_graphObjType) const
//------------------------------------------------------------------------------
{
    QString str;
    int iSectionsPrint = 0;
    for (int iAttribute = 0; iAttribute < EDrawAttributeCount; ++iAttribute) {
        if (iSectionsPrint > 0) str += ", ";
        if (IsAttributeUsed(i_graphObjType, iAttribute)) {
            str += drawAttribute2Str(iAttribute) + ": " + attributeValue2Str(iAttribute);
            iSectionsPrint++;
        }
    }
    return str;
}
