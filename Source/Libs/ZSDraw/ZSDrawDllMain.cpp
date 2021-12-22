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

#include <QtCore/qglobal.h>
#include <QtGui/QBitmap>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawGraphObjRect.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysVersion.h"

#ifdef _WINDOWS
// As "min" will be defined as a macro with two arguments and qdatetime uses "min"
// as a function with no arguments "windows.h" must be included after qdatetime
// (which is included by ZSSysTime (which again is included by ZSSysAux)).
#include <windows.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

/*==============================================================================
EPaperFormat
==============================================================================*/

static const SEnumEntry s_arEnumStrPaperFormat[EPaperFormatCount] =
{
    /*  0 */ SEnumEntry( EPaperFormatDinA5,       "Din A5"       ),
    /*  1 */ SEnumEntry( EPaperFormatDinA4,       "Din A4"       ),
    /*  2 */ SEnumEntry( EPaperFormatDinA3,       "Din A3"       ),
    /*  3 */ SEnumEntry( EPaperFormatUserDefined, "User Defined" )
};


//------------------------------------------------------------------------------
QString ZS::Draw::paperFormat2Str( int i_iVal )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPaperFormat, _ZSArrLen(s_arEnumStrPaperFormat), i_iVal );
}

//------------------------------------------------------------------------------
EPaperFormat ZS::Draw::str2PaperFormat( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    EPaperFormat format = EPaperFormatUndefined;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrPaperFormat, _ZSArrLen(s_arEnumStrPaperFormat), i_strVal );

    if( iVal >= 0 && iVal < EPaperFormatCount )
    {
        format = static_cast<EPaperFormat>(iVal);
    }
    return format;

} // str2PaperFormat


/*==============================================================================
EPaperOrientation
==============================================================================*/

static const SEnumEntry s_arEnumStrPaperOrientation[EPaperOrientationCount] =
{
    /*  0 */ SEnumEntry( EPaperOrientationUpright,   "Upright"   ),
    /*  1 */ SEnumEntry( EPaperOrientationLandscape, "Landscape" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::paperOrientation2Str( int i_iVal )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPaperOrientation, _ZSArrLen(s_arEnumStrPaperOrientation), i_iVal );
}

//------------------------------------------------------------------------------
EPaperOrientation ZS::Draw::str2PaperOrientation( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    EPaperOrientation orientation = EPaperOrientationUndefined;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrPaperOrientation, _ZSArrLen(s_arEnumStrPaperOrientation), i_strVal );

    if( iVal >= 0 && iVal < EPaperOrientationCount )
    {
        orientation = static_cast<EPaperOrientation>(iVal);
    }
    return orientation;

} // str2PaperOrientation


/*==============================================================================
EPrinterPaperSource
==============================================================================*/

static const SEnumEntry s_arEnumStrPrinterPaperSource[EPrinterPaperSourceCount] =
{
    /*  0 */ SEnumEntry( EPrinterPaperSourceAutoSelect, "Auto Select" ),
    /*  1 */ SEnumEntry( EPrinterPaperSourceManualFeed, "Manual Feed" ),
    /*  2 */ SEnumEntry( EPrinterPaperSourceTray1,      "Tray 1"      ),
    /*  3 */ SEnumEntry( EPrinterPaperSourceTray2,      "Tray 2"      )
};

//------------------------------------------------------------------------------
QString ZS::Draw::printerPaperSource2Str( int i_iVal )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPrinterPaperSource, _ZSArrLen(s_arEnumStrPrinterPaperSource), i_iVal );
}

//------------------------------------------------------------------------------
EPrinterPaperSource ZS::Draw::str2PrinterPaperSource( const QString& i_strVal )
//------------------------------------------------------------------------------
{
    EPrinterPaperSource source = EPrinterPaperSourceUndefined;

    int iVal = SEnumEntry::str2Enumerator( s_arEnumStrPrinterPaperSource, _ZSArrLen(s_arEnumStrPrinterPaperSource), i_strVal );

    if( iVal >= 0 && iVal < EPrinterPaperSourceCount )
    {
        source = static_cast<EPrinterPaperSource>(iVal);
    }
    return source;

} // str2PrinterPaperSource


/*==============================================================================
class SDrawArea
==============================================================================*/

//------------------------------------------------------------------------------
SDrawArea::SDrawArea() :
//------------------------------------------------------------------------------
    m_physValWidth( Geometry::GraphDevice()->Pixel() ),
    m_physValHeight( Geometry::GraphDevice()->Pixel() ),
    m_fXScaleFac(1.0),
    m_fYScaleFac(1.0)
{
} // default ctor

//------------------------------------------------------------------------------
SDrawArea::SDrawArea(
    const CPhysVal& i_physValWidth,
    const CPhysVal& i_physValHeight,
    double          i_fXScaleFactor,
    double          i_fYScaleFactor ) :
//------------------------------------------------------------------------------
    m_physValWidth(i_physValWidth),
    m_physValHeight(i_physValHeight),
    m_fXScaleFac(i_fXScaleFactor),
    m_fYScaleFac(i_fYScaleFactor)
{
} // ctor

//------------------------------------------------------------------------------
bool SDrawArea::isValid() const
//------------------------------------------------------------------------------
{
    bool bIsValid = false;

    if( m_physValWidth.isValid() && m_physValHeight.isValid() )
    {
        if( m_fXScaleFac >= 0.0 && m_fYScaleFac >= 0.0 )
        {
            bIsValid = true;
        }
    }
    return bIsValid;

} // isValid


/*==============================================================================
EGraphObjType
==============================================================================*/

const SEnumEntry s_arEnumStrGraphObjType[EGraphObjTypeCount] =
{
    /*  0 */ SEnumEntry( EGraphObjTypePoint,           "Point",           "Point"            ),
    /*  1 */ SEnumEntry( EGraphObjTypeLine,            "Line",            "Line"             ),
    /*  2 */ SEnumEntry( EGraphObjTypeRect,            "Rect",            "Rectangle"        ),
    /*  3 */ SEnumEntry( EGraphObjTypeEllipse,         "Ellipse",         "Ellipse"          ),
    /*  4 */ SEnumEntry( EGraphObjTypePolygon,         "Polygon",         "Polygon"          ),
    /*  5 */ SEnumEntry( EGraphObjTypePolyline,        "Polyline",        "Polyline"         ),
    /*  6 */ SEnumEntry( EGraphObjTypeText,            "Text",            "Text"             ),
    /*  7 */ SEnumEntry( EGraphObjTypeImage,           "Image",           "Image"            ),
    /*  8 */ SEnumEntry( EGraphObjTypeConnectionPoint, "ConnectionPoint", "Connection Point" ),
    /*  9 */ SEnumEntry( EGraphObjTypeConnectionLine,  "ConnectionLine",  "Connection Line"  ),
    /* 10 */ SEnumEntry( EGraphObjTypeGroup,           "Group",           "Group"            )
};

const ZS::System::SEnumEntry s_arEnumStrGraphObjClassName[EGraphObjTypeCount] =
{
    /*  0 */ SEnumEntry( EGraphObjTypePoint,           "CGraphObjPoint"           ),
    /*  1 */ SEnumEntry( EGraphObjTypeLine,            "CGraphObjLine"            ),
    /*  2 */ SEnumEntry( EGraphObjTypeRect,            "CGraphObjRect"            ),
    /*  3 */ SEnumEntry( EGraphObjTypeEllipse,         "CGraphObjEllipse"         ),
    /*  4 */ SEnumEntry( EGraphObjTypePolygon,         "CGraphObjPolygon"         ),
    /*  5 */ SEnumEntry( EGraphObjTypePolyline,        "CGraphObjPolyline"        ),
    /*  6 */ SEnumEntry( EGraphObjTypeText,            "CGraphObjText"            ),
    /*  7 */ SEnumEntry( EGraphObjTypeImage,           "CGraphObjImage"           ),
    /*  8 */ SEnumEntry( EGraphObjTypeConnectionPoint, "CGraphObjConnectionPoint" ),
    /*  9 */ SEnumEntry( EGraphObjTypeConnectionLine,  "CGraphObjConnectionLine"  ),
    /* 10 */ SEnumEntry( EGraphObjTypeGroup,           "CGraphObjGroup"           )
};

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjType2Str( int i_graphObjType )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_graphObjType == EGraphObjTypeSelectionPoint )
    {
        str = "SelectionPoint";
    }
    else if( i_graphObjType == EGraphObjTypeLabel )
    {
        str = "Label";
    }
    else if ( i_graphObjType >= EGraphObjTypeMin && i_graphObjType <= EGraphObjTypeMax )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_graphObjType-EGraphObjTypeMin);
    }
    else
    {
        str = "? (" + QString::number(i_graphObjType) + ")";
    }
    return str;

} // graphObjType2Str

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::str2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    
    int iGraphObjType = SEnumEntry::str2Enumerator(s_arEnumStrGraphObjType, _ZSArrLen(s_arEnumStrGraphObjType), i_str);

    if( iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjType) )
    {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    else
    {
        bool bConverted;
        iGraphObjType = i_str.toInt(&bConverted);
        if( bConverted && iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjType) )
        {
            graphObjType = static_cast<EGraphObjType>(iGraphObjType);
        }
    }
    return graphObjType;

} // str2GraphObjType

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjType2ClassName( int i_graphObjType )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_graphObjType == EGraphObjTypeSelectionPoint )
    {
        str = "CGraphObjSelectionPoint";
    }
    else if( i_graphObjType == EGraphObjTypeLabel )
    {
        str = "CGraphObjLabel";
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrGraphObjClassName,_ZSArrLen(s_arEnumStrGraphObjClassName),i_graphObjType);
    }
    return str;

} // graphObjType2ClassName

//------------------------------------------------------------------------------
EGraphObjType ZS::Draw::className2GraphObjType( const QString& i_str )
//------------------------------------------------------------------------------
{
    EGraphObjType graphObjType = EGraphObjTypeUndefined;
    int iGraphObjType = SEnumEntry::str2Enumerator(s_arEnumStrGraphObjClassName,_ZSArrLen(s_arEnumStrGraphObjClassName),i_str);
    if( iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjClassName) )
    {
        graphObjType = static_cast<EGraphObjType>(iGraphObjType);
    }
    else
    {
        bool bConverted;
        iGraphObjType = i_str.toInt(&bConverted);
        if( bConverted && iGraphObjType >= 0 && iGraphObjType < _ZSArrLen(s_arEnumStrGraphObjClassName) )
        {
            graphObjType = static_cast<EGraphObjType>(iGraphObjType);
        }
    }
    return graphObjType;

} // str2GraphObjClassName


/*==============================================================================
QPainterPath::ElementType
==============================================================================*/

const SEnumEntry s_arEnumStrPainterPathElemType[] =
{
    /*  0 */ SEnumEntry( QPainterPath::MoveToElement,      "MoveTo"      ),
    /*  1 */ SEnumEntry( QPainterPath::LineToElement,      "LineTo"      ),
    /*  2 */ SEnumEntry( QPainterPath::CurveToElement,     "CurveTo"     ),
    /*  3 */ SEnumEntry( QPainterPath::CurveToDataElement, "CurveToData" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::painterPathElemType2Str( int i_type )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrPainterPathElemType, _ZSArrLen(s_arEnumStrPainterPathElemType), i_type );
}

//------------------------------------------------------------------------------
QString ZS::Draw::painterPath2Str( QGraphicsItem* i_pGraphObj, const QPainterPath& i_painterPath )
//------------------------------------------------------------------------------
{
    QString str;

    QPainterPath painterPatherSimplified = i_painterPath.simplified();

    if( painterPatherSimplified.elementCount() > 0 )
    {
        QPainterPath::Element elem;
        QPointF               ptElem;
        QString               strElem;
        QString               strElemPrev;
        int                   idx;

        str = "ElemCount:" + QString::number(painterPatherSimplified.elementCount());

        for( idx = 0; idx < painterPatherSimplified.elementCount(); idx++ )
        {
            elem = painterPatherSimplified.elementAt(idx);

            ptElem = QPointF(elem.x,elem.y);

            ptElem = i_pGraphObj->mapToScene(ptElem);

            strElem = painterPathElemType2Str(elem.type) + ":" + point2Str(ptElem);

            if( strElem != strElemPrev )
            {
                str += ", " +QString::number(idx) + ":" + strElem;
            }
            strElemPrev = strElem;
        }
    }

    return str;

} // painterPath2Str


/*==============================================================================
QTransform
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::transformationType2Str( int i_type )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_type == QTransform::TxNone )
    {
        str = "None";
    }
    else // if( i_type != QTransform::TxNone )
    {
        if( i_type & QTransform::TxTranslate )
        {
            str = "Translate";
        }
        if( i_type & QTransform::TxScale )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Scale";
        }
        if( i_type & QTransform::TxRotate )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Rotate";
        }
        if( i_type & QTransform::TxShear )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Shear";
        }
        if( i_type & QTransform::TxProject )
        {
            if( !str.isEmpty() )
            {
                str += "|";
            }
            str += "Project";
        }
    } // if( i_type != QTransform::TxNone )

    return str;

} // transformationType2Str

//------------------------------------------------------------------------------
QStringList ZS::Draw::transformation2StrList( const QTransform& i_transform )
//------------------------------------------------------------------------------
{
    QStringList strlst;

    QString strType = transformationType2Str(i_transform.type());
    QString strFormula;

    /*
        m11 m12 m13
        m21 m22 m23
        m31 m32 m33

    Affine:
        x' = m11*x + m21*y + m31;
        y' = m22*y + m12*x + m32;

    Not Affine:
        w' = m32*x + m23*y + m33
        x' = x' / w'
        y' = y' / w'
    */

    QString strm11 = QString::number(fabs(i_transform.m11())); // = scale X (without rotation)
    QString strm12 = QString::number(fabs(i_transform.m12()));
    QString strm13 = QString::number(fabs(i_transform.m13()));
    QString strm21 = QString::number(fabs(i_transform.m21()));
    QString strm22 = QString::number(fabs(i_transform.m22())); // = scale Y (without rotation)
    QString strm23 = QString::number(fabs(i_transform.m23()));
    QString strm31 = QString::number(fabs(i_transform.m31())); // = dx
    QString strm32 = QString::number(fabs(i_transform.m32())); // = dy
    QString strm33 = QString::number(fabs(i_transform.m33()));

    strlst << strType; // Idx 0

    strFormula = "x' = ";

    if( i_transform.m11() > 0.0 )
    {
        strFormula += strm11 + "*x";
    }
    else if( i_transform.m11() < 0.0 )
    {
        strFormula += "-" + strm11 + "*x";
    }
    if( i_transform.m21() > 0.0 )
    {
        strFormula += " + " + strm21 + "*y";
    }
    else if( i_transform.m21() < 0.0 )
    {
        strFormula += " - " + strm21 + "*y";
    }
    if( i_transform.m31() > 0.0 )
    {
        strFormula += " + " + strm31;
    }
    else if( i_transform.m31() < 0.0 )
    {
        strFormula += " - " + strm31;
    }
    strlst << strFormula; // Idx 1

    strFormula = "y' = ";

    if( i_transform.m22() > 0.0 )
    {
        strFormula += strm22 + "*y";
    }
    else if( i_transform.m22() < 0.0 )
    {
        strFormula += "-" + strm22 + "*y";
    }
    if( i_transform.m12() > 0.0 )
    {
        strFormula += " + " + strm12 + "*x";
    }
    else if( i_transform.m12() < 0.0 )
    {
        strFormula += " - " + strm12 + "*x";
    }
    if( i_transform.m32() > 0.0 )
    {
        strFormula += " + " + strm32;
    }
    else if( i_transform.m32() < 0.0 )
    {
        strFormula += " - " + strm32;
    }

    strlst << strFormula; // Idx 2

    if( !i_transform.isAffine() )
    {
        strFormula = "w' = ";

        if( i_transform.m32() > 0.0 )
        {
            strFormula += strm32 + "*x";
        }
        else if( i_transform.m32() < 0.0 )
        {
            strFormula += "-" + strm32 + "*x";
        }
        if( i_transform.m23() > 0.0 )
        {
            strFormula += " + " + strm23 + "*y";
        }
        else if( i_transform.m23() < 0.0 )
        {
            strFormula += " - " + strm23 + "*y";
        }
        if( i_transform.m33() > 0.0 )
        {
            strFormula += " + " + strm33;
        }
        else if( i_transform.m33() < 0.0 )
        {
            strFormula += " - " + strm33;
        }
        strlst << strFormula; // Idx 3

        strlst << "x' /= w'"; // Idx 4
        strlst << "y' /= w'"; // Idx 5
    }

    return strlst;

} // transformation2StrList


/*==============================================================================
EEditTool
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrEditTool[EEditToolCount] =
{
    /* 0 */ SEnumEntry( EEditToolSelect,        "Select"        ),
    /* 1 */ SEnumEntry( EEditToolCreateObjects, "CreateObjects" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::editTool2Str( int i_iTool )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iTool == EEditToolUndefined )
    {
        str = "Undefined";
    }
    else if( i_iTool == EEditToolIgnore )
    {
        str = "Ignore";
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrEditTool,_ZSArrLen(s_arEnumStrEditTool),i_iTool);
    }
    return str;

} // editTool2Str


/*==============================================================================
ECoordinatesVersion
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrCoordinatesVersion[ECoordinatesVersionCount] =
{
    /* 0 */ SEnumEntry( ECoordinatesVersionOriginal, "Original" ),
    /* 1 */ SEnumEntry( ECoordinatesVersionCurrent,  "Current"  )
};

//------------------------------------------------------------------------------
QString ZS::Draw::coordinatesVersion2Str( int i_iVal )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrCoordinatesVersion,_ZSArrLen(s_arEnumStrCoordinatesVersion),i_iVal);
}


/*==============================================================================
EEditMode
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrEditMode[EEditModeCount] =
{
    /* 0 */ SEnumEntry( EEditModeCreating,       "Creating"       ),
    /* 1 */ SEnumEntry( EEditModeMove,           "Move"           ),
    /* 2 */ SEnumEntry( EEditModeResize,         "Resize"         ),
    /* 3 */ SEnumEntry( EEditModeRotate,         "Rotate"         ),
    /* 4 */ SEnumEntry( EEditModeMoveShapePoint, "MoveShapePoint" ),
    /* 5 */ SEnumEntry( EEditModeEditText,       "EditText"       )
};

//------------------------------------------------------------------------------
QString ZS::Draw::editMode2Str( int i_iMode )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iMode == EEditModeUndefined )
    {
        str = "Undefined";
    }
    else if( i_iMode == EEditModeIgnore )
    {
        str = "Ignore";
    }
    else if( i_iMode == EEditModeSelect )
    {
        str = "Select";
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrEditMode,_ZSArrLen(s_arEnumStrEditMode),i_iMode);
    }
    return str;

} // editMode2Str


/*==============================================================================
EEditResizeMode
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrEditResizeMode[EEditResizeModeCount] =
{
    /* 0 */ SEnumEntry( EEditResizeModeResizeAll, "ResizeAll" ),
    /* 1 */ SEnumEntry( EEditResizeModeResizeHor, "ResizeHor" ),
    /* 2 */ SEnumEntry( EEditResizeModeResizeVer, "ResizeVer" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::editResizeMode2Str( int i_iMode )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_iMode == EEditResizeModeUndefined )
    {
        str = "Undefined";
    }
    else if( i_iMode == EEditResizeModeIgnore )
    {
        str = "Ignore";
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrEditResizeMode,_ZSArrLen(s_arEnumStrEditResizeMode),i_iMode);
    }
    return str;

} // editResizeMode2Str


/*==============================================================================
ESelectionPointType
==============================================================================*/

const SEnumEntry s_arEnumStrSelectionPointType[ESelectionPointTypeCount] =
{
    /*  0 */ SEnumEntry( ESelectionPointTypeBoundingRectangle, "BoundingRect"      ),
    /*  1 */ SEnumEntry( ESelectionPointTypePolygonShapePoint, "PolygonShapePoint" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::selectionPointType2Str( int i_selectionPointType )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrSelectionPointType, _ZSArrLen(s_arEnumStrSelectionPointType), i_selectionPointType );
}

//------------------------------------------------------------------------------
ESelectionPointType ZS::Draw::str2SelectionPointType( const QString& i_str )
//------------------------------------------------------------------------------
{
    ESelectionPointType selectionPointType = ESelectionPointTypeUndefined;
    int iSelectionPointType = SEnumEntry::str2Enumerator(s_arEnumStrSelectionPointType,_ZSArrLen(s_arEnumStrSelectionPointType),i_str);

    if( iSelectionPointType >= 0 && iSelectionPointType < _ZSArrLen(s_arEnumStrSelectionPointType) )
    {
        selectionPointType = static_cast<ESelectionPointType>(iSelectionPointType);
    }
    else
    {
        bool bConverted;
        iSelectionPointType = i_str.toInt(&bConverted);
        if( bConverted && iSelectionPointType >= 0 && iSelectionPointType < _ZSArrLen(s_arEnumStrSelectionPointType) )
        {
            selectionPointType = static_cast<ESelectionPointType>(iSelectionPointType);
        }
    }
    return selectionPointType;

} // str2SelectionPointType


/*==============================================================================
ESelectionPoints
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::selectionPoints2Str( int i_selPts )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_selPts & ESelectionPointsBoundingRectCorner )
    {
        str += "BoundingRectCorners";
    }
    if( i_selPts & ESelectionPointsBoundingRectLineCenter )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectLineCenter";
    }
    if( i_selPts & ESelectionPointsBoundingRectCenter )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectCenter";
    }
    if( i_selPts & ESelectionPointsBoundingRectRotate )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "BoundingRectRotation";
    }
    if( i_selPts & ESelectionPointsPolygonShapePoints )
    {
        if( !str.isEmpty() )
        {
            str += "|";
        }
        str += "PolygonShapePoints";
    }
    return str;

} // selectionPoints2Str


/*==============================================================================
ESelectionPoint
==============================================================================*/

static double s_fSelectionPointRotateDistance_px = 20.0;

const ZS::System::SEnumEntry s_arEnumStrBoundingRectSelectionPoint[ESelectionPointCount] =
{
    /*  0 */ SEnumEntry( ESelectionPointTopLeft,      "TL", "TopLeft"      ),
    /*  1 */ SEnumEntry( ESelectionPointTopRight,     "TR", "TopRight"     ),
    /*  2 */ SEnumEntry( ESelectionPointBottomRight,  "BR", "BottomRight"  ),
    /*  3 */ SEnumEntry( ESelectionPointBottomLeft,   "BL", "BottomLeft"   ),
    /*  4 */ SEnumEntry( ESelectionPointTopCenter,    "TC", "TopCenter"    ),
    /*  5 */ SEnumEntry( ESelectionPointRightCenter,  "RC", "RightCenter"  ),
    /*  6 */ SEnumEntry( ESelectionPointBottomCenter, "BC", "BottomCenter" ),
    /*  7 */ SEnumEntry( ESelectionPointLeftCenter,   "LC", "LeftCenter"   ),
    /*  8 */ SEnumEntry( ESelectionPointCenter,       "C",  "Center"       ),
    /*  9 */ SEnumEntry( ESelectionPointRotateTop,    "OT", "RotateTop"    ),
    /* 10 */ SEnumEntry( ESelectionPointRotateBottom, "OB", "RotateBottom" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::selectionPoint2Str( int i_selPt, bool i_bShort )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bShort )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrBoundingRectSelectionPoint,_ZSArrLen(s_arEnumStrBoundingRectSelectionPoint),i_selPt);
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrBoundingRectSelectionPoint,_ZSArrLen(s_arEnumStrBoundingRectSelectionPoint),i_selPt);
    }
    return str;

} // selectionPoint2Str

//------------------------------------------------------------------------------
ESelectionPoint ZS::Draw::str2SelectionPoint( const QString& i_str )
//------------------------------------------------------------------------------
{
    ESelectionPoint graphObjSelectionPoint = ESelectionPointUndefined;
    int iGraphObjSelectionPoint = SEnumEntry::str2Enumerator(s_arEnumStrBoundingRectSelectionPoint,_ZSArrLen(s_arEnumStrBoundingRectSelectionPoint),i_str);
    if( iGraphObjSelectionPoint >= 0 && iGraphObjSelectionPoint < _ZSArrLen(s_arEnumStrBoundingRectSelectionPoint) )
    {
        graphObjSelectionPoint = static_cast<ESelectionPoint>(iGraphObjSelectionPoint);
    }
    else
    {
        bool bConverted;
        iGraphObjSelectionPoint = i_str.toInt(&bConverted);
        if( bConverted && iGraphObjSelectionPoint >= 0 && iGraphObjSelectionPoint < _ZSArrLen(s_arEnumStrBoundingRectSelectionPoint) )
        {
            graphObjSelectionPoint = static_cast<ESelectionPoint>(iGraphObjSelectionPoint);
        }
    }
    return graphObjSelectionPoint;

} // str2SelectionPoint

//------------------------------------------------------------------------------
void ZS::Draw::setSelectionPointRotateDistance( double i_fDist_px )
//------------------------------------------------------------------------------
{
    s_fSelectionPointRotateDistance_px = i_fDist_px;
}

//------------------------------------------------------------------------------
double ZS::Draw::getSelectionPointRotateDistance()
//------------------------------------------------------------------------------
{
    return s_fSelectionPointRotateDistance_px;
}

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPoint( const QLineF& i_lin, ESelectionPoint /*i_selPt*/ )
//------------------------------------------------------------------------------
{
    double fx = i_lin.p1().x() + i_lin.dx()/2.0;
    double fy = i_lin.p1().y() + i_lin.dy()/2.0;

    QPointF pt(fx,fy);

    return pt;

} // getSelectionPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPoint( const QRectF& i_rct, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF pt = i_rct.center();

    switch( i_selPt )
    {
        case ESelectionPointTopLeft:
        {
            pt = i_rct.topLeft();
            break;
        }
        case ESelectionPointTopRight:
        {
            pt = i_rct.topRight();
            break;
        }
        case ESelectionPointBottomRight:
        {
            pt = i_rct.bottomRight();
            break;
        }
        case ESelectionPointBottomLeft:
        {
            pt = i_rct.bottomLeft();
            break;
        }
        case ESelectionPointTopCenter:
        {
            pt = QPointF( i_rct.center().x(), i_rct.top() );
            break;
        }
        case ESelectionPointRightCenter:
        {
            pt = QPointF( i_rct.right(), i_rct.center().y() );
            break;
        }
        case ESelectionPointBottomCenter:
        {
            pt = QPointF( i_rct.center().x(), i_rct.bottom() );
            break;
        }
        case ESelectionPointLeftCenter:
        {
            pt = QPointF( i_rct.left(), i_rct.center().y() );
            break;
        }
        case ESelectionPointCenter:
        {
            pt = i_rct.center();
            break;
        }
        case ESelectionPointRotateTop:
        {
            pt = QPointF( i_rct.center().x(), i_rct.top()-s_fSelectionPointRotateDistance_px );
            break;
        }
        case ESelectionPointRotateBottom:
        {
            pt = QPointF( i_rct.center().x(), i_rct.bottom()+s_fSelectionPointRotateDistance_px );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_selPt )

    return pt;

} // getSelectionPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getSelectionPointOpposite( const QRectF& i_rct, ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    QPointF ptOpp = i_rct.center();

    switch( i_selPt )
    {
        case ESelectionPointTopLeft:
        {
            ptOpp = i_rct.bottomRight();
            break;
        }
        case ESelectionPointTopRight:
        {
            ptOpp = i_rct.bottomLeft();
            break;
        }
        case ESelectionPointBottomRight:
        {
            ptOpp = i_rct.topLeft();
            break;
        }
        case ESelectionPointBottomLeft:
        {
            ptOpp = i_rct.topRight();
            break;
        }
        case ESelectionPointTopCenter:
        {
            ptOpp = QPointF( i_rct.bottom(), i_rct.center().x() );
            break;
        }
        case ESelectionPointRightCenter:
        {
            ptOpp = QPointF( i_rct.left(), i_rct.center().y() );
            break;
        }
        case ESelectionPointBottomCenter:
        {
            ptOpp = QPointF( i_rct.top(), i_rct.center().x() );
            break;
        }
        case ESelectionPointLeftCenter:
        {
            ptOpp = QPointF( i_rct.right(), i_rct.center().y() );
            break;
        }
        case ESelectionPointCenter:
        {
            ptOpp = i_rct.center();
            break;
        }
        case ESelectionPointRotateTop:
        {
            ptOpp = QPointF( i_rct.center().x(), i_rct.bottom()+s_fSelectionPointRotateDistance_px );
            break;
        }
        case ESelectionPointRotateBottom:
        {
            ptOpp = QPointF( i_rct.center().x(), i_rct.top()-s_fSelectionPointRotateDistance_px );
            break;
        }
        default:
        {
            break;
        }
    } // switch( i_selPt )

    return ptOpp;

} // getSelectionPointOpposite

//------------------------------------------------------------------------------
ESelectionPoint ZS::Draw::getSelectionPointOpposite( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelectionPointsOpposite[ESelectionPointCount] =
    {
        /*  0: ESelectionPointTopLeft      */ ESelectionPointBottomRight,
        /*  1: ESelectionPointTopRight     */ ESelectionPointBottomLeft,
        /*  2: ESelectionPointBottomRight  */ ESelectionPointTopLeft,
        /*  3: ESelectionPointBottomLeft   */ ESelectionPointTopRight,
        /*  4: ESelectionPointTopCenter    */ ESelectionPointBottomCenter,
        /*  5: ESelectionPointRightCenter  */ ESelectionPointLeftCenter,
        /*  6: ESelectionPointBottomCenter */ ESelectionPointTopCenter,
        /*  7: ESelectionPointLeftCenter   */ ESelectionPointRightCenter,
        /*  8: ESelectionPointCenter       */ ESelectionPointCenter,
        /*  9: ESelectionPointRotateTop    */ ESelectionPointRotateBottom,
        /* 10: ESelectionPointRotateBottom */ ESelectionPointRotateTop
    };

    ESelectionPoint selPtOpp = ESelectionPointUndefined;

    if( i_selPt >= 0 && i_selPt < _ZSArrLen(s_arSelectionPointsOpposite) )
    {
        selPtOpp = s_arSelectionPointsOpposite[i_selPt];
    }
    return selPtOpp;

} // getSelectionPointOpposite

//------------------------------------------------------------------------------
EEditMode ZS::Draw::selectionPoint2EditMode( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    const EEditMode s_arEditModes[ESelectionPointCount] =
    {
        /*  0: ESelectionPointTopLeft      */ EEditModeResize,
        /*  1: ESelectionPointTopRight     */ EEditModeResize,
        /*  2: ESelectionPointBottomRight  */ EEditModeResize,
        /*  3: ESelectionPointBottomLeft   */ EEditModeResize,
        /*  4: ESelectionPointTopCenter    */ EEditModeResize,
        /*  5: ESelectionPointRightCenter  */ EEditModeResize,
        /*  6: ESelectionPointBottomCenter */ EEditModeResize,
        /*  7: ESelectionPointLeftCenter   */ EEditModeResize,
        /*  8: ESelectionPointCenter       */ EEditModeMove,
        /*  9: ESelectionPointRotateTop    */ EEditModeRotate,
        /* 10: ESelectionPointRotateBottom */ EEditModeRotate
    };

    EEditMode editMode = EEditModeUndefined;

    if( i_selPt >= 0 && i_selPt < _ZSArrLen(s_arEditModes) )
    {
        editMode = s_arEditModes[i_selPt];
    }
    return editMode;

} // selectionPoint2EditMode

//------------------------------------------------------------------------------
EEditResizeMode ZS::Draw::selectionPoint2EditResizeMode( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const EEditResizeMode s_arEditResizeModes[ESelectionPointCount] =
    {
        /*  0: ESelectionPointTopLeft      */ EEditResizeModeResizeAll,
        /*  1: ESelectionPointTopRight     */ EEditResizeModeResizeAll,
        /*  2: ESelectionPointBottomRight  */ EEditResizeModeResizeAll,
        /*  3: ESelectionPointBottomLeft   */ EEditResizeModeResizeAll,
        /*  4: ESelectionPointTopCenter    */ EEditResizeModeResizeVer,
        /*  5: ESelectionPointRightCenter  */ EEditResizeModeResizeHor,
        /*  6: ESelectionPointBottomCenter */ EEditResizeModeResizeVer,
        /*  7: ESelectionPointLeftCenter   */ EEditResizeModeResizeHor,
        /*  8: ESelectionPointCenter       */ EEditResizeModeUndefined,
        /*  9: ESelectionPointRotateTop    */ EEditResizeModeUndefined,
        /* 10: ESelectionPointRotateBottom */ EEditResizeModeUndefined
    };

    EEditResizeMode editResizeMode = EEditResizeModeUndefined;

    if( i_selPt >= 0 && i_selPt < _ZSArrLen(s_arEditResizeModes) )
    {
        editResizeMode = s_arEditResizeModes[i_selPt];
    }
    return editResizeMode;

} // selectionPoint2EditResizeMode

//------------------------------------------------------------------------------
Qt::CursorShape ZS::Draw::selectionPoint2CursorShape( ESelectionPoint i_selPt )
//------------------------------------------------------------------------------
{
    static const Qt::CursorShape s_arCursorShapes[ESelectionPointCount] =
    {
        /*  0: ESelectionPointTopLeft      */ Qt::SizeFDiagCursor, /*  \  */
        /*  1: ESelectionPointTopRight     */ Qt::SizeBDiagCursor, /*  /  */
        /*  2: ESelectionPointBottomRight  */ Qt::SizeFDiagCursor, /*  \  */
        /*  3: ESelectionPointBottomLeft   */ Qt::SizeBDiagCursor, /*  /  */
        /*  4: ESelectionPointTopCenter    */ Qt::SizeVerCursor,
        /*  5: ESelectionPointRightCenter  */ Qt::SizeHorCursor,
        /*  6: ESelectionPointBottomCenter */ Qt::SizeVerCursor,
        /*  7: ESelectionPointLeftCenter   */ Qt::SizeHorCursor,
        /*  8: ESelectionPointCenter       */ Qt::SizeAllCursor,
        /*  9: ESelectionPointRotateTop    */ Qt::BitmapCursor,
        /* 10: ESelectionPointRotateBottom */ Qt::BitmapCursor
    };

    Qt::CursorShape cursorShape = Qt::ArrowCursor;

    if( i_selPt >= 0 && i_selPt < _ZSArrLen(s_arCursorShapes) )
    {
        cursorShape = s_arCursorShapes[i_selPt];
    }
    return cursorShape;

} // selectionPoint2CursorShape

/*==============================================================================
QGraphicsItem::GraphicsItemChange
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrGraphicsItemChange[] =
{
    /*  0 */ SEnumEntry( QGraphicsItem::ItemPositionChange, "PositionChange" ),
    /*  1 */ SEnumEntry( QGraphicsItem::ItemMatrixChange, "MatrixChange" ),
    /*  2 */ SEnumEntry( QGraphicsItem::ItemVisibleChange, "VisibleChange" ),
    /*  3 */ SEnumEntry( QGraphicsItem::ItemEnabledChange, "EnabledChange" ),
    /*  4 */ SEnumEntry( QGraphicsItem::ItemSelectedChange, "SelectedChange" ),
    /*  5 */ SEnumEntry( QGraphicsItem::ItemParentChange, "ParentChange" ),
    /*  6 */ SEnumEntry( QGraphicsItem::ItemChildAddedChange, "ChildAddedChange" ),
    /*  7 */ SEnumEntry( QGraphicsItem::ItemChildRemovedChange, "ChildRemovedChange" ),
    /*  8 */ SEnumEntry( QGraphicsItem::ItemTransformChange, "TransformChange" ),
    /*  9 */ SEnumEntry( QGraphicsItem::ItemPositionHasChanged, "PositionHasChanged" ),
    /* 10 */ SEnumEntry( QGraphicsItem::ItemTransformHasChanged, "TransformHasChanged" ),
    /* 11 */ SEnumEntry( QGraphicsItem::ItemSceneChange, "SceneChange" ),
    /* 12 */ SEnumEntry( QGraphicsItem::ItemVisibleHasChanged, "VisibleHasChanged" ),
    /* 13 */ SEnumEntry( QGraphicsItem::ItemEnabledHasChanged, "EnabledHasChanged" ),
    /* 14 */ SEnumEntry( QGraphicsItem::ItemSelectedHasChanged, "SelectedHasChanged" ),
    /* 15 */ SEnumEntry( QGraphicsItem::ItemParentHasChanged, "ParentHasChanged" ),
    /* 16 */ SEnumEntry( QGraphicsItem::ItemSceneHasChanged, "SceneHasChanged" ),
    /* 17 */ SEnumEntry( QGraphicsItem::ItemCursorChange, "CursorChange" ),
    /* 18 */ SEnumEntry( QGraphicsItem::ItemCursorHasChanged, "CursorHasChanged" ),
    /* 19 */ SEnumEntry( QGraphicsItem::ItemToolTipChange, "ToolTipChange" ),
    /* 20 */ SEnumEntry( QGraphicsItem::ItemToolTipHasChanged, "ToolTipHasChanged" ),
    /* 21 */ SEnumEntry( QGraphicsItem::ItemFlagsChange, "FlagsChange" ),
    /* 22 */ SEnumEntry( QGraphicsItem::ItemFlagsHaveChanged, "FlagsHaveChanged" ),
    /* 23 */ SEnumEntry( QGraphicsItem::ItemZValueChange, "ZValueChange" ),
    /* 24 */ SEnumEntry( QGraphicsItem::ItemZValueHasChanged, "ZValueHasChanged" ),
    /* 25 */ SEnumEntry( QGraphicsItem::ItemOpacityChange, "OpacityChange" ),
    /* 26 */ SEnumEntry( QGraphicsItem::ItemOpacityHasChanged, "OpacityHasChanged" ),
    /* 27 */ SEnumEntry( QGraphicsItem::ItemScenePositionHasChanged, "ScenePositionHasChanged" ),
    #if QT_VERSION >= 0x040700
    /* 28 */ SEnumEntry( QGraphicsItem::ItemRotationChange, "RotationChange" ),
    /* 29 */ SEnumEntry( QGraphicsItem::ItemRotationHasChanged, "RotationHasChanged" ),
    /* 30 */ SEnumEntry( QGraphicsItem::ItemScaleChange, "ScaleChange" ),
    /* 31 */ SEnumEntry( QGraphicsItem::ItemScaleHasChanged, "ScaleHasChanged" ),
    /* 32 */ SEnumEntry( QGraphicsItem::ItemTransformOriginPointChange, "TransformOriginPointChange" ),
    /* 33 */ SEnumEntry( QGraphicsItem::ItemTransformOriginPointHasChanged, "TransformOriginPointHasChanged" )
    #else
    /* 28 */ SEnumEntry( 28, "28" )
    #endif
};

//------------------------------------------------------------------------------
QString ZS::Draw::graphicsItemChange2Str( int i_change )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str( s_arEnumStrGraphicsItemChange, _ZSArrLen(s_arEnumStrGraphicsItemChange), i_change );
}


/*==============================================================================
ELinePoint
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrLinePoint[ELinePointCount] =
{
    SEnumEntry( ELinePointStart, "Start", "Start" ),
    SEnumEntry( ELinePointEnd,   "End",   "End"   )
};

//------------------------------------------------------------------------------
QString ZS::Draw::linePoint2Str( int i_linePoint )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLinePoint,_ZSArrLen(s_arEnumStrLinePoint),i_linePoint);
}

//------------------------------------------------------------------------------
ELinePoint ZS::Draw::str2LinePoint( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELinePoint linePoint = ELinePointUndefined;
    int iLinePoint = SEnumEntry::str2Enumerator(s_arEnumStrLinePoint,_ZSArrLen(s_arEnumStrLinePoint),i_str);
    if( iLinePoint >= 0 && iLinePoint < _ZSArrLen(s_arEnumStrLinePoint) )
    {
        linePoint = static_cast<ELinePoint>(iLinePoint);
    }
    else
    {
        bool bConverted;
        iLinePoint = i_str.toInt(&bConverted);
        if( bConverted && iLinePoint >= 0 && iLinePoint < _ZSArrLen(s_arEnumStrLinePoint) )
        {
            linePoint = static_cast<ELinePoint>(iLinePoint);
        }
    }
    return linePoint;

} // str2LinePoint


/*==============================================================================
ETextStyle
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrTextStyle[] =
{
    /* 0x00 */ SEnumEntry( ETextStyleNormal,     "Normal",     "Normal"      ),
    /* 0x01 */ SEnumEntry( ETextStyleItalic,     "Italic",     "Italic"      ),
    /* 0x02 */ SEnumEntry( ETextStyleBold,       "Bold",       "Bold"        ),
    /* 0x03 */ SEnumEntry( ETextStyleBoldItalic, "BoldItalic", "Bold Italic" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::textStyle2Str( int i_textStyle )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTextStyle,_ZSArrLen(s_arEnumStrTextStyle),i_textStyle);
}

//------------------------------------------------------------------------------
ETextStyle ZS::Draw::str2TextStyle( const QString& i_str )
//------------------------------------------------------------------------------
{
    ETextStyle textStyle = ETextStyleNormal;
    int        iTextStyle = SEnumEntry::str2Enumerator(s_arEnumStrTextStyle,_ZSArrLen(s_arEnumStrTextStyle),i_str);

    if( iTextStyle >= 0 && iTextStyle < _ZSArrLen(s_arEnumStrTextStyle) )
    {
        textStyle = static_cast<ETextStyle>(iTextStyle);
    }
    else
    {
        bool bConverted;
        iTextStyle = i_str.toInt(&bConverted);
        if( bConverted && iTextStyle >= 0 && iTextStyle < _ZSArrLen(s_arEnumStrTextStyle) )
        {
            textStyle = static_cast<ETextStyle>(iTextStyle);
        }
    }
    return textStyle;

} // str2TextStyle


/*==============================================================================
ETextSize
==============================================================================*/

const int s_ariTextSizes_px[] =
{
    /*  0: ETextSize6  */  6,
    /*  1: ETextSize7  */  7,
    /*  2: ETextSize8  */  8,
    /*  3: ETextSize9  */  9,
    /*  4: ETextSize10 */ 10,
    /*  5: ETextSize11 */ 11,
    /*  6: ETextSize12 */ 12,
    /*  7: ETextSize14 */ 14,
    /*  8: ETextSize16 */ 16,
    /*  9: ETextSize18 */ 18,
    /* 10: ETextSize20 */ 20,
    /* 11: ETextSize22 */ 22,
    /* 12: ETextSize24 */ 24,
    /* 13: ETextSize26 */ 26,
    /* 14: ETextSize28 */ 28,
    /* 15: ETextSize36 */ 36,
    /* 16: ETextSize48 */ 48,
    /* 17: ETextSize72 */ 72
};

const ZS::System::SEnumEntry s_arEnumStrTextSize[] =
{
    /*  0 */ SEnumEntry( ETextSize6,   "6" ),
    /*  1 */ SEnumEntry( ETextSize7,   "7" ),
    /*  2 */ SEnumEntry( ETextSize8,   "8" ),
    /*  3 */ SEnumEntry( ETextSize9,   "9" ),
    /*  4 */ SEnumEntry( ETextSize10, "10" ),
    /*  5 */ SEnumEntry( ETextSize11, "11" ),
    /*  6 */ SEnumEntry( ETextSize12, "12" ),
    /*  7 */ SEnumEntry( ETextSize14, "14" ),
    /*  8 */ SEnumEntry( ETextSize16, "16" ),
    /*  9 */ SEnumEntry( ETextSize18, "18" ),
    /* 10 */ SEnumEntry( ETextSize20, "20" ),
    /* 11 */ SEnumEntry( ETextSize22, "22" ),
    /* 12 */ SEnumEntry( ETextSize24, "24" ),
    /* 13 */ SEnumEntry( ETextSize26, "26" ),
    /* 14 */ SEnumEntry( ETextSize28, "28" ),
    /* 15 */ SEnumEntry( ETextSize36, "36" ),
    /* 16 */ SEnumEntry( ETextSize48, "48" ),
    /* 17 */ SEnumEntry( ETextSize72, "72" )
};

//------------------------------------------------------------------------------
int ZS::Draw::textSize2SizeInPixels( int i_textSize )
//------------------------------------------------------------------------------
{
    int iSize_px = 6;

    if( i_textSize >= 0 && i_textSize < ETextSizeCount )
    {
        return iSize_px = s_ariTextSizes_px[i_textSize];
    }
    return iSize_px;

} // textSize2SizeInPixels

//------------------------------------------------------------------------------
QString ZS::Draw::textSize2Str( int i_textSize )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_textSize);
}

//------------------------------------------------------------------------------
ETextSize ZS::Draw::str2TextSize( const QString& i_str )
//------------------------------------------------------------------------------
{
    ETextSize textSize = ETextSize11;
    int       iTextSize = SEnumEntry::str2Enumerator(s_arEnumStrTextSize,_ZSArrLen(s_arEnumStrTextSize),i_str);

    if( iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
    {
        textSize = static_cast<ETextSize>(iTextSize);
    }
    else
    {
        bool bConverted;
        iTextSize = i_str.toInt(&bConverted);
        if( bConverted && iTextSize >= 0 && iTextSize < _ZSArrLen(s_arEnumStrTextSize) )
        {
            textSize = static_cast<ETextSize>(iTextSize);
        }
    }
    return textSize;

} // str2TextSize


/*==============================================================================
ETextEffect
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrTextEffect[] =
{
    /* 0x00 */ SEnumEntry( ETextEffectNone,               "None",               "None"               ),
    /* 0x01 */ SEnumEntry( ETextEffectStrikeout,          "Strikeout",          "Strikeout"          ),
    /* 0x02 */ SEnumEntry( ETextEffectUnderline,          "Underline",          "Underline"          ),
    /* 0x03 */ SEnumEntry( ETextEffectStrikeoutUnderline, "StrikeoutUnderline", "StrikeoutUnderline" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::textEffect2Str( int i_textEffect )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrTextEffect,_ZSArrLen(s_arEnumStrTextEffect),i_textEffect);
}

//------------------------------------------------------------------------------
ETextEffect ZS::Draw::str2TextEffect( const QString& i_str )
//------------------------------------------------------------------------------
{
    ETextEffect textEffect = ETextEffectNone;
    int        iTextEffect = SEnumEntry::str2Enumerator(s_arEnumStrTextEffect,_ZSArrLen(s_arEnumStrTextEffect),i_str);

    if( iTextEffect >= 0 && iTextEffect < _ZSArrLen(s_arEnumStrTextEffect) )
    {
        textEffect = static_cast<ETextEffect>(iTextEffect);
    }
    else
    {
        bool bConverted;
        iTextEffect = i_str.toInt(&bConverted);
        if( bConverted && iTextEffect >= 0 && iTextEffect < _ZSArrLen(s_arEnumStrTextEffect) )
        {
            textEffect = static_cast<ETextEffect>(iTextEffect);
        }
    }
    return textEffect;

} // str2TextEffect


/*==============================================================================
EFillStyle
==============================================================================*/

const Qt::BrushStyle s_arFillStyles2QtBrushStyles[] =
{
    /*  0: EFillStyleNone                     */ Qt::NoBrush,
    /*  1: EFillStyleSolidPattern             */ Qt::SolidPattern,
    /*  2: EFillStyleDense1Pattern            */ Qt::Dense1Pattern,
    /*  3: EFillStyleDense2Pattern            */ Qt::Dense2Pattern,
    /*  4: EFillStyleDense3Pattern            */ Qt::Dense3Pattern,
    /*  5: EFillStyleDense4Pattern            */ Qt::Dense4Pattern,
    /*  6: EFillStyleDense5Pattern            */ Qt::Dense5Pattern,
    /*  7: EFillStyleDense6Pattern            */ Qt::Dense6Pattern,
    /*  8: EFillStyleDense7Pattern            */ Qt::Dense7Pattern,
    /*  9: EFillStyleHorPattern               */ Qt::HorPattern,
    /* 10: EFillStyleVerPattern               */ Qt::VerPattern,
    /* 11: EFillStyleCrossPattern             */ Qt::CrossPattern,
    /* 12: EFillStyleBDiagPattern             */ Qt::BDiagPattern,
    /* 13: EFillStyleFDiagPattern             */ Qt::FDiagPattern,
    /* 14: EFillStyleDiagCrossPattern         */ Qt::DiagCrossPattern,
    /* 15: EFillStyleLinearGradientPattern    */ Qt::LinearGradientPattern,
    /* 16: EFillStyleRadialGradientPattern    */ Qt::RadialGradientPattern,
    /* 17: EFillStyleConicalGradientPattern   */ Qt::ConicalGradientPattern,
    /* 18: EFillStyleTexturePattern           */ Qt::TexturePattern
};

const ZS::System::SEnumEntry s_arEnumStrFillStyle[] =
{
    SEnumEntry( EFillStyleNoFill,                 "NoFill",                  "No Fill"                ),
    SEnumEntry( EFillStyleSolidPattern,           "SolidPattern",            "Solid Pattern"          ),
    SEnumEntry( EFillStyleDense1Pattern,          "Dense1Pattern",           "Dense1Pattern"          ),
    SEnumEntry( EFillStyleDense2Pattern,          "Dense2Pattern",           "Dense2Pattern"          ),
    SEnumEntry( EFillStyleDense3Pattern,          "Dense3Pattern",           "Dense3Pattern"          ),
    SEnumEntry( EFillStyleDense4Pattern,          "Dense4Pattern",           "Dense4Pattern"          ),
    SEnumEntry( EFillStyleDense5Pattern,          "Dense5Pattern",           "Dense5Pattern"          ),
    SEnumEntry( EFillStyleDense6Pattern,          "Dense6Pattern",           "Dense6Pattern"          ),
    SEnumEntry( EFillStyleDense7Pattern,          "Dense7Pattern",           "Dense7Pattern"          ),
    SEnumEntry( EFillStyleHorPattern,             "HorPattern",              "HorPattern"             ),
    SEnumEntry( EFillStyleVerPattern,             "VerPattern",              "VerPattern"             ),
    SEnumEntry( EFillStyleCrossPattern,           "CrossPattern",            "CrossPattern"           ),
    SEnumEntry( EFillStyleBDiagPattern,           "BDiagPattern",            "BDiagPattern"           ),
    SEnumEntry( EFillStyleFDiagPattern,           "FDiagPattern",            "FDiagPattern"           ),
    SEnumEntry( EFillStyleDiagCrossPattern,       "DiagCrossPattern",        "DiagCrossPattern"       ),
    SEnumEntry( EFillStyleLinearGradientPattern,  "LinearGradientPattern",   "LinearGradientPattern"  ),
    SEnumEntry( EFillStyleRadialGradientPattern,  "RadialGradientPattern",   "RadialGradientPattern"  ),
    SEnumEntry( EFillStyleConicalGradientPattern, "ConicalGradientPattern",  "ConicalGradientPattern" ),
    SEnumEntry( EFillStyleTexturePattern,         "TexturePattern",          "TexturePattern"         )
};

//------------------------------------------------------------------------------
Qt::BrushStyle ZS::Draw::fillStyle2QtBrushStyle( int i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;

    if( i_fillStyle >= 0 && i_fillStyle < _ZSArrLen(s_arFillStyles2QtBrushStyles) )
    {
        brushStyle = s_arFillStyles2QtBrushStyles[i_fillStyle];
    }
    return brushStyle;

} // fillStyle2QtBrushStyle

//------------------------------------------------------------------------------
bool ZS::Draw::isFillStyleGradientPattern( int i_fillStyle )
//------------------------------------------------------------------------------
{
    bool bIsGradientPattern = false;

    if( i_fillStyle == EFillStyleLinearGradientPattern
     || i_fillStyle == EFillStyleRadialGradientPattern
     || i_fillStyle == EFillStyleConicalGradientPattern )
    {
        bIsGradientPattern = true;
    }
    return bIsGradientPattern;

} // isFillStyleGradientPattern

//------------------------------------------------------------------------------
QString ZS::Draw::fillStyle2Str( int i_fillStyle )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrFillStyle,_ZSArrLen(s_arEnumStrFillStyle),i_fillStyle);
}

//------------------------------------------------------------------------------
EFillStyle ZS::Draw::str2FillStyle( const QString& i_str )
//------------------------------------------------------------------------------
{
    EFillStyle fillStyle = EFillStyleUndefined;
    int        iFillStyle = SEnumEntry::str2Enumerator(s_arEnumStrFillStyle,_ZSArrLen(s_arEnumStrFillStyle),i_str);

    if( iFillStyle >= 0 && iFillStyle < _ZSArrLen(s_arEnumStrFillStyle) )
    {
        fillStyle = static_cast<EFillStyle>(iFillStyle);
    }
    else
    {
        bool bConverted;
        iFillStyle = i_str.toInt(&bConverted);
        if( bConverted && iFillStyle >= 0 && iFillStyle < _ZSArrLen(s_arEnumStrFillStyle) )
        {
            fillStyle = static_cast<EFillStyle>(iFillStyle);
        }
    }
    return fillStyle;

} // str2FillStyle


/*==============================================================================
ELineStyle
==============================================================================*/

const Qt::PenStyle s_arLineStyles2QtPenStyles[] =
{
    /*  0: ELineStyleNoLine         */ Qt::NoPen,
    /*  1: ELineStyleSolidLine      */ Qt::SolidLine,
    /*  2: ELineStyleDashLine       */ Qt::DashLine,
    /*  3: ELineStyleDotLine        */ Qt::DotLine,
    /*  4: ELineStyleDashDotLine    */ Qt::DashDotLine,
    /*  5: ELineStyleDashDotDotLine */ Qt::DashDotDotLine
};

const ZS::System::SEnumEntry s_arEnumStrLineStyle[] =
{
    SEnumEntry( ELineStyleNoLine,         "NoLine",         "No Line"                   ),
    SEnumEntry( ELineStyleSolidLine,      "SolidLine",      "Solid Line"                ),
    SEnumEntry( ELineStyleDashLine,       "DashLine",       "Dashed Line"               ),
    SEnumEntry( ELineStyleDotLine,        "DotLine",        "Dotted Line"               ),
    SEnumEntry( ELineStyleDashDotLine,    "DashDotLine",    "Dashed Dotted Line"        ),
    SEnumEntry( ELineStyleDashDotDotLine, "DashDotDotLine", "Dashed Dotted Dotted Line" )
};

//------------------------------------------------------------------------------
Qt::PenStyle ZS::Draw::lineStyle2QtPenStyle( int i_lineStyle )
//------------------------------------------------------------------------------
{
    Qt::PenStyle penStyle = Qt::NoPen;

    if( i_lineStyle >= 0 && i_lineStyle < _ZSArrLen(s_arLineStyles2QtPenStyles) )
    {
        penStyle = s_arLineStyles2QtPenStyles[i_lineStyle];
    }
    return penStyle;

} // lineStyle2QtPenStyle

//------------------------------------------------------------------------------
QString ZS::Draw::lineStyle2Str( int i_lineStyle )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineStyle,_ZSArrLen(s_arEnumStrLineStyle),i_lineStyle);
}

//------------------------------------------------------------------------------
ELineStyle ZS::Draw::str2LineStyle( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineStyle lineStyle = ELineStyleUndefined;
    int iLineStyle = SEnumEntry::str2Enumerator(s_arEnumStrLineStyle,_ZSArrLen(s_arEnumStrLineStyle),i_str);
    if( iLineStyle >= 0 && iLineStyle < _ZSArrLen(s_arEnumStrLineStyle) )
    {
        lineStyle = static_cast<ELineStyle>(iLineStyle);
    }
    else
    {
        bool bConverted;
        iLineStyle = i_str.toInt(&bConverted);
        if( bConverted && iLineStyle >= 0 && iLineStyle < _ZSArrLen(s_arEnumStrLineStyle) )
        {
            lineStyle = static_cast<ELineStyle>(iLineStyle);
        }
    }
    return lineStyle;

} // str2LineStyle


/*==============================================================================
ELineRecordType
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrLineRecordType[ELineRecordTypeCount] =
{
    SEnumEntry( ELineRecordTypeNormal, "Normal", "Normal" ),
    SEnumEntry( ELineRecordTypeDouble, "Double", "Double" ),
    SEnumEntry( ELineRecordTypeTriple, "Triple", "Triple" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::lineRecordType2Str( int i_lineRecordType )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineRecordType,_ZSArrLen(s_arEnumStrLineRecordType),i_lineRecordType);
}

//------------------------------------------------------------------------------
ELineRecordType ZS::Draw::str2LineRecordType( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineRecordType lineRecordType = ELineRecordTypeUndefined;
    int iLineRecordType = SEnumEntry::str2Enumerator(s_arEnumStrLineRecordType,_ZSArrLen(s_arEnumStrLineRecordType),i_str);

    if( iLineRecordType >= 0 && iLineRecordType < _ZSArrLen(s_arEnumStrLineRecordType) )
    {
        lineRecordType = static_cast<ELineRecordType>(iLineRecordType);
    }
    else
    {
        bool bConverted;
        iLineRecordType = i_str.toInt(&bConverted);
        if( bConverted && iLineRecordType >= 0 && iLineRecordType < _ZSArrLen(s_arEnumStrLineRecordType) )
        {
            lineRecordType = static_cast<ELineRecordType>(iLineRecordType);
        }
    }
    return lineRecordType;

} // str2LineRecordType


/*==============================================================================
ELineEndStyle
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrLineEndStyle[ELineEndStyleCount] =
{
    SEnumEntry( ELineEndStyleNormal,    "Normal",    "Normal"     ),
    SEnumEntry( ELineEndStyleArrowHead, "ArrowHead", "Arrow Head" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::lineEndStyle2Str( int i_lineEndStyle )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineEndStyle,_ZSArrLen(s_arEnumStrLineEndStyle),i_lineEndStyle);
}

//------------------------------------------------------------------------------
ELineEndStyle ZS::Draw::str2LineEndStyle( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineEndStyle lineEndStyle = ELineEndStyleUndefined;
    int iLineEndStyle = SEnumEntry::str2Enumerator(s_arEnumStrLineEndStyle,_ZSArrLen(s_arEnumStrLineEndStyle),i_str);

    if( iLineEndStyle >= 0 && iLineEndStyle < _ZSArrLen(s_arEnumStrLineEndStyle) )
    {
        lineEndStyle = static_cast<ELineEndStyle>(iLineEndStyle);
    }
    else
    {
        bool bConverted;
        iLineEndStyle = i_str.toInt(&bConverted);
        if( bConverted && iLineEndStyle >= 0 && iLineEndStyle < _ZSArrLen(s_arEnumStrLineEndStyle) )
        {
            lineEndStyle = static_cast<ELineEndStyle>(iLineEndStyle);
        }
    }
    return lineEndStyle;

} // str2LineEndStyle


/*==============================================================================
ELineEndFillStyle
==============================================================================*/

const Qt::BrushStyle s_arLineEndFillStyles2QtBrushStyles[] =
{
    /*  0: ELineEndFillStyleNoFill       */ Qt::NoBrush,
    /*  1: ELineEndFillStyleSolidPattern */ Qt::SolidPattern
};

const ZS::System::SEnumEntry s_arEnumStrLineEndFillStyle[ELineEndFillStyleCount] =
{
    SEnumEntry( ELineEndFillStyleNoFill,       "NoFill",       "No Fill"       ),
    SEnumEntry( ELineEndFillStyleSolidPattern, "SolidPattern", "Solid Pattern" )
};

//------------------------------------------------------------------------------
Qt::BrushStyle ZS::Draw::lineEndFillStyle2QtBrushStyle( int i_fillStyle )
//------------------------------------------------------------------------------
{
    Qt::BrushStyle brushStyle = Qt::NoBrush;

    if( i_fillStyle >= 0 && i_fillStyle < _ZSArrLen(s_arLineEndFillStyles2QtBrushStyles) )
    {
        brushStyle = s_arLineEndFillStyles2QtBrushStyles[i_fillStyle];
    }
    return brushStyle;

} // lineEndFillStyle2QtBrushStyle

//------------------------------------------------------------------------------
QString ZS::Draw::lineEndFillStyle2Str( int i_lineEndFillStyle )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineEndFillStyle,_ZSArrLen(s_arEnumStrLineEndFillStyle),i_lineEndFillStyle);
}

//------------------------------------------------------------------------------
ELineEndFillStyle ZS::Draw::str2LineEndFillStyle( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineEndFillStyle lineEndFillStyle = ELineEndFillStyleUndefined;
    int iLineEndFillStyle = SEnumEntry::str2Enumerator(s_arEnumStrLineEndFillStyle,_ZSArrLen(s_arEnumStrLineEndFillStyle),i_str);

    if( iLineEndFillStyle >= 0 && iLineEndFillStyle < _ZSArrLen(s_arEnumStrLineEndFillStyle) )
    {
        lineEndFillStyle = static_cast<ELineEndFillStyle>(iLineEndFillStyle);
    }
    else
    {
        bool bConverted;
        iLineEndFillStyle = i_str.toInt(&bConverted);
        if( bConverted && iLineEndFillStyle >= 0 && iLineEndFillStyle < _ZSArrLen(s_arEnumStrLineEndFillStyle) )
        {
            lineEndFillStyle = static_cast<ELineEndFillStyle>(iLineEndFillStyle);
        }
    }
    return lineEndFillStyle;

} // str2LineEndFillStyle


/*==============================================================================
ELineEndStyleBaseLineType
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrLineEndBaseLineType[ELineEndBaseLineTypeCount] =
{
    SEnumEntry( ELineEndBaseLineTypeNoLine,   "NoLine",   "No Line"  ),
    SEnumEntry( ELineEndBaseLineTypeNormal,   "Normal",   "Normal"   ),
    SEnumEntry( ELineEndBaseLineTypeIndented, "Indented", "Indented" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::lineEndBaseLineType2Str( int i_lineEndBaseLineType )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineEndBaseLineType,_ZSArrLen(s_arEnumStrLineEndBaseLineType),i_lineEndBaseLineType);
}

//------------------------------------------------------------------------------
ELineEndBaseLineType ZS::Draw::str2LineEndBaseLineType( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineEndBaseLineType lineEndBaseLineType = ELineEndBaseLineTypeUndefined;
    int iLineEndBaseLineType = SEnumEntry::str2Enumerator(s_arEnumStrLineEndBaseLineType,_ZSArrLen(s_arEnumStrLineEndBaseLineType),i_str);

    if( iLineEndBaseLineType >= 0 && iLineEndBaseLineType < _ZSArrLen(s_arEnumStrLineEndBaseLineType) )
    {
        lineEndBaseLineType = static_cast<ELineEndBaseLineType>(iLineEndBaseLineType);
    }
    else
    {
        bool bConverted;
        iLineEndBaseLineType = i_str.toInt(&bConverted);
        if( bConverted && iLineEndBaseLineType >= 0 && iLineEndBaseLineType < _ZSArrLen(s_arEnumStrLineEndBaseLineType) )
        {
            lineEndBaseLineType = static_cast<ELineEndBaseLineType>(iLineEndBaseLineType);
        }
    }
    return lineEndBaseLineType;

} // str2LineEndBaseLineType


/*==============================================================================
ELineEndWidth
==============================================================================*/

const double s_arfLineEndWidths[ELineEndWidthCount] =
{
    /* Thin   */  7.0,
    /* Medium */ 13.0,
    /* Wide   */ 19.0
};

const ZS::System::SEnumEntry s_arEnumStrLineEndWidth[ELineEndWidthCount] =
{
    SEnumEntry( ELineEndWidthThin,   "Thin",   "Thin"   ),
    SEnumEntry( ELineEndWidthMedium, "Medium", "Medium" ),
    SEnumEntry( ELineEndWidthWide,   "Wide",   "Wide"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::lineEndWidth2dy( int i_lineEndWidth )
//------------------------------------------------------------------------------
{
    double fdy = 0.0;

    if( i_lineEndWidth >= 0 && i_lineEndWidth < ELineEndWidthCount )
    {
        fdy = s_arfLineEndWidths[i_lineEndWidth];
    }
    return fdy;

} // lineEndWidth2dy

//------------------------------------------------------------------------------
QString ZS::Draw::lineEndWidth2Str( int i_lineEndWidth )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineEndWidth,_ZSArrLen(s_arEnumStrLineEndWidth),i_lineEndWidth);
}

//------------------------------------------------------------------------------
ELineEndWidth ZS::Draw::str2LineEndWidth( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineEndWidth lineEndWidth = ELineEndWidthUndefined;
    int iLineEndWidth = SEnumEntry::str2Enumerator(s_arEnumStrLineEndWidth,_ZSArrLen(s_arEnumStrLineEndWidth),i_str);

    if( iLineEndWidth >= 0 && iLineEndWidth < _ZSArrLen(s_arEnumStrLineEndWidth) )
    {
        lineEndWidth = static_cast<ELineEndWidth>(iLineEndWidth);
    }
    else
    {
        bool bConverted;
        iLineEndWidth = i_str.toInt(&bConverted);
        if( bConverted && iLineEndWidth >= 0 && iLineEndWidth < _ZSArrLen(s_arEnumStrLineEndWidth) )
        {
            lineEndWidth = static_cast<ELineEndWidth>(iLineEndWidth);
        }
    }
    return lineEndWidth;

} // str2LineEndWidth


/*==============================================================================
ELineEndLength
==============================================================================*/

const double s_arfLineEndLengths[ELineEndWidthCount] =
{
    /* Short  */  7.0,
    /* Medium */ 10.0,
    /* Long   */ 13.0
};

const ZS::System::SEnumEntry s_arEnumStrLineEndLength[ELineEndLengthCount] =
{
    SEnumEntry( ELineEndLengthShort,  "Short",  "Short"  ),
    SEnumEntry( ELineEndLengthMedium, "Medium", "Medium" ),
    SEnumEntry( ELineEndLengthLong,   "Long",   "Long"   )
};

//------------------------------------------------------------------------------
double ZS::Draw::lineEndLength2dx( int i_lineEndLength )
//------------------------------------------------------------------------------
{
    double fdx = 0.0;

    if( i_lineEndLength >= 0 && i_lineEndLength < ELineEndLengthCount )
    {
        fdx = s_arfLineEndLengths[i_lineEndLength];
    }
    return fdx;

} // lineEndLength2dx

//------------------------------------------------------------------------------
QString ZS::Draw::lineEndLength2Str( int i_lineEndLength )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrLineEndLength,_ZSArrLen(s_arEnumStrLineEndLength),i_lineEndLength);
}

//------------------------------------------------------------------------------
ELineEndLength ZS::Draw::str2LineEndLength( const QString& i_str )
//------------------------------------------------------------------------------
{
    ELineEndLength lineEndLength = ELineEndLengthUndefined;
    int iLineEndLength = SEnumEntry::str2Enumerator(s_arEnumStrLineEndLength,_ZSArrLen(s_arEnumStrLineEndLength),i_str);

    if( iLineEndLength >= 0 && iLineEndLength < _ZSArrLen(s_arEnumStrLineEndLength) )
    {
        lineEndLength = static_cast<ELineEndLength>(iLineEndLength);
    }
    else
    {
        bool bConverted;
        iLineEndLength = i_str.toInt(&bConverted);
        if( bConverted && iLineEndLength >= 0 && iLineEndLength < _ZSArrLen(s_arEnumStrLineEndLength) )
        {
            lineEndLength = static_cast<ELineEndLength>(iLineEndLength);
        }
    }
    return lineEndLength;

} // str2LineEndLength


/*==============================================================================
EAlignmentRefLine
==============================================================================*/

const ZS::System::SEnumEntry s_arEnumStrAlignmentRef[EAlignmentRefCount] =
{
    SEnumEntry( EAlignmentRefLeft,    "Left",    "L"  ),
    SEnumEntry( EAlignmentRefHCenter, "HCenter", "HC" ),
    SEnumEntry( EAlignmentRefRight,   "Right",   "R"  ),
    SEnumEntry( EAlignmentRefTop,     "Top",     "T"  ),
    SEnumEntry( EAlignmentRefVCenter, "VCenter", "VC" ),
    SEnumEntry( EAlignmentRefBottom,  "Bottom",  "B"  ),
    SEnumEntry( EAlignmentRefWidth,   "Width",   "W"  ),
    SEnumEntry( EAlignmentRefHeight,  "Height",  "H"  )
};

//------------------------------------------------------------------------------
QString ZS::Draw::alignmentRef2Str( int i_ref, bool i_bShort )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bShort )
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrAlignmentRef,_ZSArrLen(s_arEnumStrAlignmentRef),i_ref,1);
    }
    else
    {
        str = SEnumEntry::enumerator2Str(s_arEnumStrAlignmentRef,_ZSArrLen(s_arEnumStrAlignmentRef),i_ref,0);
    }
    return str;

} // alignmentRef2Str

//------------------------------------------------------------------------------
EAlignmentRef ZS::Draw::str2AlignmentRef( const QString& i_str )
//------------------------------------------------------------------------------
{
    EAlignmentRef alignmentRef = EAlignmentRefUndefined;
    int iAlignmentRef = SEnumEntry::str2Enumerator(s_arEnumStrAlignmentRef,_ZSArrLen(s_arEnumStrAlignmentRef),i_str);

    if( iAlignmentRef >= 0 && iAlignmentRef < _ZSArrLen(s_arEnumStrAlignmentRef) )
    {
        alignmentRef = static_cast<EAlignmentRef>(iAlignmentRef);
    }
    else
    {
        bool bConverted;
        iAlignmentRef = i_str.toInt(&bConverted);
        if( bConverted && iAlignmentRef >= 0 && iAlignmentRef < _ZSArrLen(s_arEnumStrAlignmentRef) )
        {
            alignmentRef = static_cast<EAlignmentRef>(iAlignmentRef);
        }
    }
    return alignmentRef;

} // str2AlignmentRef


/*==============================================================================
struct SGraphObjAlignment
==============================================================================*/

//------------------------------------------------------------------------------
QString ZS::Draw::graphObjAlignment2Str( const SGraphObjAlignment& i_alignment )
//------------------------------------------------------------------------------
{
    QString str;

    str  = "RefChild:" + alignmentRef2Str(i_alignment.m_alignmentRefChild);
    str += ", RefParent:" + alignmentRef2Str(i_alignment.m_alignmentRefParent);
    str += ", Absolute:" + bool2Str(i_alignment.m_bAlignAbsolute);
    str += ", Val:" + QString::number(i_alignment.m_fVal);

    return str;

} // graphObjAlignment2Str


/*==============================================================================
struct SGraphObjHitInfo
==============================================================================*/

/* public: // ctor
==============================================================================*/

//------------------------------------------------------------------------------
SGraphObjHitInfo::SGraphObjHitInfo() :
//------------------------------------------------------------------------------
    m_editMode(EEditModeUndefined),
    m_editResizeMode(EEditResizeModeUndefined),
    m_selPtBoundingRect(ESelectionPointUndefined),
    m_idxPolygonShapePoint(-1),
    m_idxLineSegment(-1),
    m_ptSelected(),
    m_cursor()
{
} // ctor

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isBoundingRectSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode == EEditModeResize && m_editResizeMode != EEditResizeModeUndefined )
    {
        bIsHit = (m_selPtBoundingRect >= ESelectionPointRectMin) && (m_selPtBoundingRect <= ESelectionPointRectMax);
    }
    return bIsHit;

} // isBoundingRectSelectionPointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isSelectionPointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode != EEditModeUndefined )
    {
        bIsHit = (m_selPtBoundingRect != ESelectionPointUndefined);
    }
    return bIsHit;

} // isSelectionPointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isPolygonShapePointHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode == EEditModeMoveShapePoint )
    {
        bIsHit = (m_idxPolygonShapePoint >= 0);
    }
    return bIsHit;

} // isPolygonShapePointHit

//------------------------------------------------------------------------------
bool SGraphObjHitInfo::isLineSegmentHit() const
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    if( m_editMode != EEditModeUndefined )
    {
        bIsHit = (m_idxLineSegment >= 0);
    }
    return bIsHit;

} // isLineSegmentHit

/* public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SGraphObjHitInfo::setCursor( double i_fGraphObjRotAngle_rad )
//------------------------------------------------------------------------------
{
    double fCursorAngle_rad = 0.0;
    bool   bSetResizeCursor = false;

    switch( m_editMode )
    {
        case EEditModeMove:
        {
            m_cursor = Qt::SizeAllCursor;
            break;
        }
        case EEditModeResize:
        {
            switch( m_editResizeMode )
            {
                case EEditResizeModeResizeAll:
                {
                    switch( m_selPtBoundingRect )
                    {
                        case ESelectionPointTopLeft:
                        case ESelectionPointBottomRight:
                        {
                            fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f135Degrees_rad; // 2nd quadrant: arrow from right/bottom -> top/left
                            m_cursor = Qt::SizeFDiagCursor; /*  \  */
                            bSetResizeCursor = true;
                            break;
                        }
                        case ESelectionPointTopRight:
                        case ESelectionPointBottomLeft:
                        {
                            fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f45Degrees_rad; // 1st quadrant: arrow from bottom/left -> top/right
                            m_cursor = Qt::SizeBDiagCursor; /*  /  */
                            bSetResizeCursor = true;
                            break;
                        }
                        case ESelectionPointRotateTop:
                        case ESelectionPointRotateBottom:
                        {
                            QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
                            QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
                            m_cursor = QCursor(bmpCursor,bmpCursorMask);
                            break;
                        }
                        case ESelectionPointLeftCenter:
                        case ESelectionPointRightCenter:
                        case ESelectionPointTopCenter:
                        case ESelectionPointBottomCenter:
                        case ESelectionPointCenter:
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case EEditResizeModeResizeHor:
                {
                    fCursorAngle_rad = i_fGraphObjRotAngle_rad;
                    m_cursor = Qt::SizeHorCursor;
                    bSetResizeCursor = true;
                    break;
                }
                case EEditResizeModeResizeVer:
                {
                    fCursorAngle_rad = i_fGraphObjRotAngle_rad + Math::c_f90Degrees_rad;
                    m_cursor = Qt::SizeVerCursor;
                    bSetResizeCursor = true;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case EEditModeRotate:
        {
            QBitmap bmpCursor(":/ZS/Draw/CursorRotateFree16x16.bmp");
            QBitmap bmpCursorMask = bmpCursor.createHeuristicMask();
            m_cursor = QCursor(bmpCursor,bmpCursorMask);
            break;
        }
        case EEditModeMoveShapePoint:
        {
            m_cursor = Qt::CrossCursor;
            break;
        }
        case EEditModeEditText:
        {
            m_cursor = Qt::IBeamCursor;
            break;
        }
        default:
        {
            break;
        }
    } // switch( editMode )

    if( bSetResizeCursor )
    {
        // Force resulting cursor rotation angle to 1st or 2nd quadrant (0..180°)
        while( fCursorAngle_rad >= Math::c_f180Degrees_rad )
        {
            fCursorAngle_rad -= Math::c_f180Degrees_rad;
        }
        while( fCursorAngle_rad < 0.0 )
        {
            fCursorAngle_rad += Math::c_f180Degrees_rad;
        }
        if( fCursorAngle_rad >= 0.0 && fCursorAngle_rad < Math::c_f45Degrees_rad/2.0 ) // 0.0 .. 22.5°
        {
            m_cursor = Qt::SizeHorCursor;
        }
        else if( fCursorAngle_rad >= Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 3.0*Math::c_f45Degrees_rad/2.0 ) // 22.5° .. 67.5°
        {
            m_cursor = Qt::SizeBDiagCursor; // 1st quadrant: arrow from bottom/left -> top/right
        }
        else if( fCursorAngle_rad >= 3.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 5.0*Math::c_f45Degrees_rad/2.0 ) // 67.5° .. 112.5°
        {
            m_cursor = Qt::SizeVerCursor;
        }
        else if( fCursorAngle_rad >= 5.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < 7.0*Math::c_f45Degrees_rad/2.0 ) // 112.5° .. 157.5°
        {
            m_cursor = Qt::SizeFDiagCursor; // 2nd quadrant: arrow from top/left -> bottom/right
        }
        else if( fCursorAngle_rad >= 7.0*Math::c_f45Degrees_rad/2.0 && fCursorAngle_rad < Math::c_f180Degrees_rad ) // 157.5° .. 180.0°
        {
            m_cursor = Qt::SizeHorCursor;
        }

    } // if( bSetResizeCursor )

} // setCursor


/*******************************************************************************
exported methods implementation
*******************************************************************************/

//------------------------------------------------------------------------------
void ZS::Draw::getLineEndPolygons(
    const QLineF&        i_line,
    const CDrawSettings& i_drawSettings,
    QPolygonF*           i_pplgLineStart,
    QPolygonF*           i_pplgLineEnd )
//------------------------------------------------------------------------------
{
    /*--------------------------------------------------------------------------

    Line Start (line.p1(())    Line End (line.p2())

             Pt3                      Pt3
             /                         \
            /                           \
        Pt2/__Pt4____           ____Pt4__\Pt2
           \                             /
            \                           /
             \                         /
             Pt1                      Pt1

    --------------------------------------------------------------------------*/

    if( i_drawSettings.getLineEndStyle(ELinePointStart) != ELineEndStyleNormal
     || i_drawSettings.getLineEndStyle(ELinePointEnd) != ELineEndStyleNormal )
    {
        double fAngle_rad = 0.0;

        QPolygonF            plgLineStart;
        QPolygonF            plgLineEnd;
        QPointF              pt;
        int                  iLinePoint;
        ELinePoint           linePoint;
        ELineEndWidth        width;
        ELineEndLength       length;
        ELineEndBaseLineType baseLineType;

        double fdx = i_line.dx();
        double fdy = i_line.dy();
        double flen = i_line.length();

        if( fabs(flen) > 0.0 )
        {
            fAngle_rad = acos( fdx / flen );
        }

        // Right of y axis ..
        if( fdx >= 0.0 )
        {
            // "Above" x axis ( 1. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if( fdy <= 0.0 )
            {
            }
            // "Below" x axis ( 2. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else
            {
                fAngle_rad *= -1.0;
            }
        }
        // Left of y axis ..
        else
        {
            // "Above" x axis ( 3. quadrant, angle returned by acos: 0° <= f <= 90°) ..
            if( fdy <= 0.0 )
            {
            }
            // "Below" x axis ( 4. quadrant, angle returned by acos: 90° <= f <= 180°) ..
            else
            {
                fAngle_rad *= -1.0;
            }
        }

        #ifdef _DEBUG
        //double fAngle_deg = 360.0*fAngle_rad/Math::c_f2PI;
        #endif

        for( iLinePoint = 0; iLinePoint < ELinePointCount; iLinePoint++ )
        {
            linePoint = static_cast<ELinePoint>(iLinePoint);

            width        = i_drawSettings.getLineEndWidth(linePoint);
            length       = i_drawSettings.getLineEndLength(linePoint);
            baseLineType = i_drawSettings.getLineEndBaseLineType(linePoint);

            if( iLinePoint == ELinePointStart )
            {
                if( i_pplgLineStart != nullptr )
                {
                    pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                    pt.setY( i_line.p1().y() + lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                    plgLineStart.append(pt); // Pt1

                    plgLineStart.append( i_line.p1() ); // Pt2

                    pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                    pt.setY( i_line.p1().y() - lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                    plgLineStart.append(pt); // Pt3

                    if( baseLineType == ELineEndBaseLineTypeNormal )
                    {
                        pt.setX( i_line.p1().x() + lineEndLength2dx(length) );
                        pt.setY( i_line.p1().y() );
                        pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                        plgLineStart.append(pt); // Pt4
                    }
                    else if( baseLineType == ELineEndBaseLineTypeIndented )
                    {
                        pt.setX( i_line.p1().x() + lineEndLength2dx(length)/2.0 );
                        pt.setY( i_line.p1().y() );
                        pt = rotatePoint( i_line.p1(), pt, fAngle_rad );
                        plgLineStart.append(pt); // Pt4
                    }

                    *i_pplgLineStart = plgLineStart;

                } // if( i_pplgLineStart != nullptr )

            } // if( iLinePoint == ELinePointStart )

            else // if( iLinePoint == ELinePointEnd )
            {
                if( i_pplgLineEnd != nullptr )
                {
                    pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                    pt.setY( i_line.p2().y() + lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                    plgLineEnd.append(pt); // Pt1

                    plgLineEnd.append( i_line.p2() ); // Pt2

                    pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                    pt.setY( i_line.p2().y() - lineEndWidth2dy(width)/2.0 );
                    pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                    plgLineEnd.append(pt); // Pt3

                    if( baseLineType == ELineEndBaseLineTypeNormal )
                    {
                        pt.setX( i_line.p2().x() - lineEndLength2dx(length) );
                        pt.setY( i_line.p2().y() );
                        pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                        plgLineEnd.append(pt); // Pt4
                    }
                    else if( baseLineType == ELineEndBaseLineTypeIndented )
                    {
                        pt.setX( i_line.p2().x() - lineEndLength2dx(length)/2.0 );
                        pt.setY( i_line.p2().y() );
                        pt = rotatePoint( i_line.p2(), pt, fAngle_rad );
                        plgLineEnd.append(pt); // Pt4
                    }

                    *i_pplgLineEnd = plgLineEnd;

                } // if( i_pplgLineEnd != nullptr )

            } // if( iLinePoint == ELinePointEnd )

        } // for( iLinePoint = 0; iLinePoint < ELinePointCount; iLinePoint++ )

    } // if( i_drawSettings.getLineEndStyle(ELinePointStart) != ELineEndStyleNormal ..

} // getLineEndPolygons

//------------------------------------------------------------------------------
void ZS::Draw::getMaxRectArea(
    double  i_fXLeft1,    double  i_fYTop1,
    double  i_fXRight1,   double  i_fYBottom1,
    double  i_fXLeft2,    double  i_fYTop2,
    double  i_fXRight2,   double  i_fYBottom2,
    double& o_fXLeftMin,  double& o_fYTopMin,
    double& o_fXRightMax, double& o_fYBottomMax )
//------------------------------------------------------------------------------
{
    o_fXLeftMin   = i_fXLeft1;
    o_fYTopMin    = i_fYTop1;
    o_fXRightMax  = i_fXRight1;
    o_fYBottomMax = i_fYBottom1;

    if( o_fXLeftMin > i_fXLeft2 )
    {
        o_fXLeftMin = i_fXLeft2;
    }
    if( o_fYTopMin > i_fYTop2 )
    {
        o_fYTopMin = i_fYTop2;
    }
    if( o_fXRightMax < i_fXRight2 )
    {
        o_fXRightMax = i_fXRight2;
    }
    if( o_fYBottomMax < i_fYBottom2 )
    {
        o_fYBottomMax = i_fYBottom2;
    }

} // getMaxRectArea

//------------------------------------------------------------------------------
void ZS::Draw::getMaxRectArea(
    const CPhysVal& i_physValLeft1,    const CPhysVal& i_physValTop1,
    const CPhysVal& i_physValRight1,   const CPhysVal& i_physValBottom1,
    const CPhysVal& i_physValLeft2,    const CPhysVal& i_physValTop2,
    const CPhysVal& i_physValRight2,   const CPhysVal& i_physValBottom2,
    CPhysVal&       o_physValLeftMax,  CPhysVal&       o_physValTopMax,
    CPhysVal&       o_physValRightMax, CPhysVal&       o_physValBottomMax )
//------------------------------------------------------------------------------
{
    o_physValLeftMax   = i_physValLeft1;
    o_physValTopMax    = i_physValTop1;
    o_physValRightMax  = i_physValRight1;
    o_physValBottomMax = i_physValBottom1;

    if( o_physValLeftMax > i_physValLeft2 )
    {
        o_physValLeftMax = i_physValLeft2;
    }
    if( o_physValTopMax > i_physValTop2 )
    {
        o_physValTopMax = i_physValTop2;
    }
    if( o_physValRightMax < i_physValRight2 )
    {
        o_physValRightMax = i_physValRight2;
    }
    if( o_physValBottomMax < i_physValBottom2 )
    {
        o_physValBottomMax = i_physValBottom2;
    }

} // getMaxRectArea

//------------------------------------------------------------------------------
QRectF ZS::Draw::resizeRect(
    const QRectF&    i_rect,
    ESelectionPoint  i_selPt,
    const QPointF&   i_ptSel,
    ESelectionPoint* o_pSelPt )
//------------------------------------------------------------------------------
{
    ESelectionPoint selPt = i_selPt;

    QRectF rctResult = i_rect;

    double fXPos = i_ptSel.x();
    double fYPos = i_ptSel.y();

    switch( i_selPt )
    {
        case ESelectionPointTopLeft:
        {
            // Moved right and below right bottom corner ..
            if( fXPos > rctResult.right() && fYPos > rctResult.bottom() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomRight;
            }
            // Moved right of right border ..
            else if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopRight;
            }
            // Moved below bottom border ..
            else if( fYPos > rctResult.bottom() )
            {
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPointBottomLeft:
        {
            // Moved right and above right top corner ..
            if( fXPos > rctResult.right() && fYPos < rctResult.top() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopRight;
            }
            // Moved right of right border ..
            else if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomRight;
            }
            // Moved above top border ..
            else if( fYPos < rctResult.top() )
            {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopLeft;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }

        case ESelectionPointTopRight:
        {
            // Moved left and below left bottom corner ..
            if( fXPos < rctResult.left() && fYPos > rctResult.bottom() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomLeft;
            }
            // Moved left of left border ..
            else if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopLeft;
            }
            // Moved below bottom border ..
            else if( fYPos > rctResult.bottom() )
            {
                rctResult.setRight(fXPos);
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPointBottomRight:
        {
            // Moved left and above left top corner ..
            if( fXPos < rctResult.left() && fYPos < rctResult.top() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopLeft;
            }
            // Moved left of left border ..
            else if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomLeft;
            }
            // Moved above top border ..
            else if( fYPos < rctResult.top() )
            {
                rctResult.setRight(fXPos);
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopRight;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
                rctResult.setBottom(fYPos);
            }
            break;
        }

        case ESelectionPointLeftCenter:
        {
            // Moved right of right border ..
            if( fXPos > rctResult.right() )
            {
                rctResult.setLeft(rctResult.right());
                rctResult.setRight(fXPos);
                selPt = ESelectionPointRightCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setLeft(fXPos);
            }
            break;
        }

        case ESelectionPointRightCenter:
        {
            // Moved left of left border ..
            if( fXPos < rctResult.left() )
            {
                rctResult.setRight(rctResult.left());
                rctResult.setLeft(fXPos);
                selPt = ESelectionPointLeftCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setRight(fXPos);
            }
            break;
        }

        case ESelectionPointTopCenter:
        {
            // Moved below bottom border ..
            if( fYPos > rctResult.bottom() )
            {
                rctResult.setTop(rctResult.bottom());
                rctResult.setBottom(fYPos);
                selPt = ESelectionPointBottomCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setTop(fYPos);
            }
            break;
        }

        case ESelectionPointBottomCenter:
        {
            // Moved above top border ..
            if( fYPos < rctResult.top() )
            {
                rctResult.setBottom(rctResult.top());
                rctResult.setTop(fYPos);
                selPt = ESelectionPointTopCenter;
            }
            // If the bounding rectangle don't need to be twisted ...
            else
            {
                rctResult.setBottom(fYPos);
            }
            break;
        }

        case ESelectionPointCenter:
        case ESelectionPointRotateTop:
        case ESelectionPointRotateBottom:
        default:
        {
            break;
        }
    }

    if( o_pSelPt != nullptr )
    {
        *o_pSelPt = selPt;
    }

    return rctResult;

} // resizeRect

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::resizePolygon(
    const QPolygonF& i_plg,
    const QRectF&    i_rctBoundingNew,
    const QRectF&    i_rctBoundingOld )
//------------------------------------------------------------------------------
{
    QPolygonF plgResult = i_plg;

    QRectF rctBoundingOld = i_rctBoundingOld;

    if( !rctBoundingOld.isValid() )
    {
        rctBoundingOld = plgResult.boundingRect();
    }

    double fx0Old = rctBoundingOld.left();
    double fy0Old = rctBoundingOld.top();
    double fWOld  = fabs(rctBoundingOld.width());
    double fHOld  = fabs(rctBoundingOld.height());

    double fx0New = i_rctBoundingNew.left();
    double fy0New = i_rctBoundingNew.top();
    double fWNew  = fabs(i_rctBoundingNew.width());
    double fHNew  = fabs(i_rctBoundingNew.height());

    double fScaleX = 1.0;
    double fScaleY = 1.0;

    if( fWOld > 0.0 )
    {
        fScaleX = fWNew / fWOld;
    }
    if( fHOld > 0.0 )
    {
        fScaleY = fHNew / fHOld;
    }

    QPointF pt;
    int     idxPt;
    double  fxdOld, fydOld;
    double  fxdNew, fydNew;

    for( idxPt = 0; idxPt < plgResult.size(); idxPt++ )
    {
        pt = plgResult[idxPt];

        fxdOld = pt.x() - fx0Old;
        fydOld = pt.y() - fy0Old;

        fxdNew = fScaleX * fxdOld;
        fydNew = fScaleY * fydOld;

        pt.setX( fx0New + fxdNew );
        pt.setY( fy0New + fydNew );

        plgResult[idxPt] = pt;
    }

    return plgResult;

} // resizePolygon

//------------------------------------------------------------------------------
QRectF ZS::Draw::boundingRect( const QPointF& i_pt, double i_fRadius )
//------------------------------------------------------------------------------
{
    return QRectF( i_pt.x()-i_fRadius,  i_pt.y()-i_fRadius, 2.0*i_fRadius, 2.0*i_fRadius );
}

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rect2Polygon( const QRectF& i_rct, int i_iSelPtsCount, const ESelectionPoint* i_pSelPts )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPts[] = {
        ESelectionPointTopLeft,
        ESelectionPointTopRight,
        ESelectionPointBottomRight,
        ESelectionPointBottomLeft
    };

    const ESelectionPoint*  pSelPts = i_pSelPts;
    int                     iSelPtsCount = i_iSelPtsCount;

    if( pSelPts == nullptr )
    {
        iSelPtsCount = _ZSArrLen(s_arSelPts);
        pSelPts = s_arSelPts;
    }

    QPolygonF plg(iSelPtsCount);

    ESelectionPoint selPt;
    int             idxSelPt;

    for( idxSelPt = 0; idxSelPt < 4 && idxSelPt < plg.size(); idxSelPt++ )
    {
        selPt = pSelPts[idxSelPt];

        switch( selPt )
        {
            case ESelectionPointTopLeft:
            {
                plg[idxSelPt] = i_rct.topLeft();
                break;
            }
            case ESelectionPointTopRight:
            {
                plg[idxSelPt] = i_rct.topRight();
                break;
            }
            case ESelectionPointBottomRight:
            {
                plg[idxSelPt] = i_rct.bottomRight();
                break;
            }
            case ESelectionPointBottomLeft:
            {
                plg[idxSelPt] = i_rct.bottomLeft();
                break;
            }
            case ESelectionPointBottomCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.bottomLeft(),i_rct.bottomRight()) );
                break;
            }
            case ESelectionPointTopCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topLeft(),i_rct.topRight()) );
                break;
            }
            case ESelectionPointRightCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topRight(),i_rct.bottomRight()) );
                break;
            }
            case ESelectionPointLeftCenter:
            {
                plg[idxSelPt] = getCenterPoint( QLineF(i_rct.topLeft(),i_rct.bottomLeft()) );
                break;
            }
            case ESelectionPointCenter:
            {
                plg[idxSelPt] = i_rct.center();
                break;
            }
            case ESelectionPointRotateTop:
            case ESelectionPointRotateBottom:
            default:
            {
                break;
            }
        }
    }

    return plg;

} // rect2Polygon

//------------------------------------------------------------------------------
bool ZS::Draw::isLineHit( const QLineF& i_lineF, const QPointF& i_point, double i_fTolerance )
//------------------------------------------------------------------------------
{
    // We create a square with the SideLength = 2 * fTolerance.
    // For each side we are going to check whether the border line inteferes
    // with the given input line.

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    double fXLeft   = i_point.x() - fTolerance;
    double fYTop    = i_point.y() - fTolerance;
    double fXRight  = i_point.x() + fTolerance;
    double fYBottom = i_point.y() + fTolerance;

    QRectF rctPt( QPointF(fXLeft,fYTop), QPointF(fXRight,fYBottom) );
    QLineF lineRctPt;
    int    idx;

    for( idx = 0; idx < 4; idx++ )
    {
        switch( idx )
        {
            // Rectangle line numbers surrounding the point to be tested:
            //        +-- 0 --+
            //        |       |
            //        3   X   1
            //        |       |
            //        +-- 2 --+
            case 0:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.top()) );
                break;
            }
            case 1:
            {
                lineRctPt.setP1( QPointF(rctPt.right(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.bottom()) );
                break;
            }
            case 2:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.bottom()) );
                lineRctPt.setP2( QPointF(rctPt.right(),rctPt.bottom()) );
                break;
            }
            case 3:
            {
                lineRctPt.setP1( QPointF(rctPt.left(),rctPt.top()) );
                lineRctPt.setP2( QPointF(rctPt.left(),rctPt.bottom()) );
                break;
            }
            default:
            {
                break;
            }
        }
        if (lineRctPt.intersects(i_lineF, nullptr) == QLineF::BoundedIntersection)
        {
            bIsHit = true;
            break;
        }
    }

    return bIsHit;

} // isLineHit

//------------------------------------------------------------------------------
bool ZS::Draw::isRectHit(
    const QRectF&     i_rct,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPtsCorners[] = {
        ESelectionPointBottomRight,
        ESelectionPointTopRight,
        ESelectionPointBottomLeft,
        ESelectionPointTopLeft
    };

    static const ESelectionPoint s_arSelPtsLineCenters[] = {
        ESelectionPointBottomCenter,
        ESelectionPointRightCenter,
        ESelectionPointLeftCenter,
        ESelectionPointTopCenter
    };

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditModeUndefined;
        o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_rct;

    rctBndTmp.setLeft( i_rct.left() - fTolerance );
    rctBndTmp.setTop( i_rct.top() - fTolerance );
    rctBndTmp.setRight( i_rct.right() + fTolerance );
    rctBndTmp.setBottom( i_rct.bottom() + fTolerance );

    if( rctBndTmp.contains(i_pt) )
    {
        QPolygonF       plg;
        QRectF          rct;
        QLineF          lin;
        QPointF         pt;
        ESelectionPoint selPt;
        int             idxPt;

        plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsCorners), s_arSelPtsCorners );

        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
        {
            selPt = s_arSelPtsCorners[idxPt];

            pt = plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                }
                break;
            }
        }

        if( !bIsHit )
        {
            plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters );

            for( idxPt = 0; idxPt < plg.size(); idxPt++ )
            {
                selPt = s_arSelPtsLineCenters[idxPt];

                pt = plg[idxPt];

                rct = boundingRect(pt,fTolerance);

                if( rct.contains(i_pt) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                        o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                    break;
                }
            }
        }

        if( !bIsHit )
        {
            plg = i_rct; // this cast return 5 points (left top corner twice)

            for( idxPt = 0; idxPt < plg.size()-1; idxPt++ )
            {
                lin.setP1(plg[idxPt]);
                lin.setP2(plg[idxPt+1]);

                if( isLineHit(lin,i_pt,fTolerance) )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        o_pHitInfo->m_editMode = EEditModeMove;
                        o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                        o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = idxPt;
                        o_pHitInfo->m_ptSelected = i_pt;
                        o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                    }
                    break;
                }
            }
        }

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyleSolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isRectHit

//------------------------------------------------------------------------------
bool ZS::Draw::isEllipseHit(
    const QRectF&     i_rct,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    static const ESelectionPoint s_arSelPtsLineCenters[] = {
        ESelectionPointBottomCenter,
        ESelectionPointRightCenter,
        ESelectionPointLeftCenter,
        ESelectionPointTopCenter
    };

    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditModeUndefined;
        o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_rct;

    rctBndTmp.setLeft( i_rct.left() - fTolerance );
    rctBndTmp.setTop( i_rct.top() - fTolerance );
    rctBndTmp.setRight( i_rct.right() + fTolerance );
    rctBndTmp.setBottom( i_rct.bottom() + fTolerance );

    if( rctBndTmp.contains(i_pt) )
    {
        QPointF ptC = i_rct.center();

        QPolygonF       plg;
        QRectF          rct;
        QPointF         pt;
        ESelectionPoint selPt;
        int             idxPt;

        plg = rect2Polygon( i_rct, _ZSArrLen(s_arSelPtsLineCenters), s_arSelPtsLineCenters );

        for( idxPt = 0; idxPt < plg.size(); idxPt++ )
        {
            selPt = s_arSelPtsLineCenters[idxPt];

            pt = plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = selectionPoint2EditMode(selPt);
                    o_pHitInfo->m_editResizeMode = selectionPoint2EditResizeMode(selPt);
                    o_pHitInfo->m_selPtBoundingRect = selPt;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                }
                break;
            }
        }

        if( !bIsHit )
        {
            double fa = i_rct.width() / 2.0;
            double fb = i_rct.height() / 2.0;
            double fx = i_pt.x() - ptC.x();
            double fy = i_pt.y() - ptC.y();

            // Ellipse as vertical line ..
            if( fa == 0.0 )
            {
                if( fabs(fx) <= fTolerance )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        selPt = ESelectionPointRightCenter;
                        o_pHitInfo->m_editMode = EEditModeResize;
                        o_pHitInfo->m_editResizeMode = EEditResizeModeResizeHor;
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                }
            } // if( fa == 0.0 )

            // Ellipse as horizontal line ..
            else if( fb == 0.0 )
            {
                if( fabs(fy) <= fTolerance )
                {
                    bIsHit = true;

                    if( o_pHitInfo != nullptr )
                    {
                        selPt = ESelectionPointBottomCenter;
                        o_pHitInfo->m_editMode = EEditModeResize;
                        o_pHitInfo->m_editResizeMode = EEditResizeModeResizeVer;
                        o_pHitInfo->m_selPtBoundingRect = selPt;
                        o_pHitInfo->m_idxPolygonShapePoint = -1;
                        o_pHitInfo->m_idxLineSegment = -1;
                        o_pHitInfo->m_ptSelected = pt;
                        o_pHitInfo->m_cursor = selectionPoint2CursorShape(selPt);
                    }
                }
            } // if( fb == 0.0 )

            // Ellipse is a circle ..
            else if( fa == fb )
            {
                // Circle equation:
                //   x² + y² = r²
                //   y = sqrt(r² - x²)

                double fyTmp = sqrt( Math::sqr(fa) - Math::sqr(fx) );

                if( (fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp) )
                {
                    bIsHit = true;
                }
                else if( (fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp) )
                {
                    bIsHit = true;
                }
                if( bIsHit && o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            } // if( fa == fb )

            else // if( fa != 0.0 && fb != 0.0 && fa != fb )
            {
                // Ellipse equation:
                //   x²/a² + y²/b² = 1
                //   y²/b² = 1 - x²/a²
                //   y² = b² * (1 - x²/a²)
                //   y = b * sqrt(1 - x²/a²)

                if( fx == fa )
                {
                    if( fabs(fy) <= fTolerance )
                    {
                        bIsHit = true;
                    }
                }
                else if( fy == fb )
                {
                    if( fabs(fx) <= fTolerance )
                    {
                        bIsHit = true;
                    }
                }
                else
                {
                    double fyTmp = fb * sqrt( 1.0 - Math::sqr(fx/fa) );

                    if( (fy + fTolerance >= fyTmp) && (fy - fTolerance <= fyTmp) )
                    {
                        bIsHit = true;
                    }
                    else if( (fy + fTolerance >= -fyTmp) && (fy - fTolerance <= -fyTmp) )
                    {
                        bIsHit = true;
                    }
                }

                if( bIsHit && o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            } // if( fa != 0.0 && fb != 0.0 && fa != fb )

        } // if( !bIsHit )

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyleSolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isEllipseHit

//------------------------------------------------------------------------------
bool ZS::Draw::isPolygonHit(
    const QPolygonF&  i_plg,
    EFillStyle        i_fillStyle,
    const QPointF&    i_pt,
    double            i_fTolerance,
    SGraphObjHitInfo* o_pHitInfo )
//------------------------------------------------------------------------------
{
    bool   bIsHit = false;
    double fTolerance = i_fTolerance;

    if( fTolerance <= 0.0 )
    {
        fTolerance = 2.0;
    }

    if( o_pHitInfo != nullptr )
    {
        o_pHitInfo->m_editMode = EEditModeUndefined;
        o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
        o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
        o_pHitInfo->m_idxPolygonShapePoint = -1;
        o_pHitInfo->m_idxLineSegment = -1;
        o_pHitInfo->m_ptSelected = QPointF();
        o_pHitInfo->m_cursor = Qt::ArrowCursor;
    }

    QRectF rctBndTmp = i_plg.boundingRect();

    rctBndTmp.setLeft( rctBndTmp.left() - fTolerance );
    rctBndTmp.setTop( rctBndTmp.top() - fTolerance );
    rctBndTmp.setRight( rctBndTmp.right() + fTolerance );
    rctBndTmp.setBottom( rctBndTmp.bottom() + fTolerance );

    if( rctBndTmp.contains(i_pt) )
    {
        QRectF  rct;
        QLineF  lin;
        QPointF pt;
        int     idxPt;

        for( idxPt = 0; idxPt < i_plg.size(); idxPt++ )
        {
            pt = i_plg[idxPt];

            rct = boundingRect(pt,fTolerance);

            if( rct.contains(i_pt) )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMoveShapePoint;
                    o_pHitInfo->m_idxPolygonShapePoint = idxPt;
                    o_pHitInfo->m_ptSelected = pt;
                    o_pHitInfo->m_cursor = Qt::CrossCursor;
                }
                break;
            }
        }

        if( !bIsHit )
        {
            if( i_plg.size() > 1 )
            {
                for( idxPt = 0; idxPt < i_plg.size()-1; idxPt++ )
                {
                    lin.setP1(i_plg[idxPt]);
                    lin.setP2(i_plg[idxPt+1]);

                    if( isLineHit(lin,i_pt,fTolerance) )
                    {
                        bIsHit = true;

                        if( o_pHitInfo != nullptr )
                        {
                            o_pHitInfo->m_editMode = EEditModeMove;
                            o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                            o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                            o_pHitInfo->m_idxPolygonShapePoint = -1;
                            o_pHitInfo->m_idxLineSegment = idxPt;
                            o_pHitInfo->m_ptSelected = i_pt;
                            o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                        }
                        break;
                    }
                }
            }
        }

        if( !bIsHit )
        {
            if( i_fillStyle == EFillStyleSolidPattern )
            {
                bIsHit = true;

                if( o_pHitInfo != nullptr )
                {
                    o_pHitInfo->m_editMode = EEditModeMove;
                    o_pHitInfo->m_editResizeMode = EEditResizeModeUndefined;
                    o_pHitInfo->m_selPtBoundingRect = ESelectionPointUndefined;
                    o_pHitInfo->m_idxPolygonShapePoint = -1;
                    o_pHitInfo->m_idxLineSegment = -1;
                    o_pHitInfo->m_ptSelected = i_pt;
                    o_pHitInfo->m_cursor = Qt::SizeAllCursor;
                }
            }
        }

    } // if( rctBndTmp.contains(i_pt) )

    return bIsHit;

} // isPolygonHit

//------------------------------------------------------------------------------
double ZS::Draw::getDist( const QPointF& i_pt1, const QPointF& i_pt2 )
//------------------------------------------------------------------------------
{
    double fXDistPow_px = pow( i_pt2.x() - i_pt1.x(), 2.0 );
    double fYDistPow_px = pow( i_pt2.y() - i_pt1.y(), 2.0 );
    double fRadius_px   = sqrt( fXDistPow_px + fYDistPow_px );
    return fRadius_px;

} // getDist

//------------------------------------------------------------------------------
double ZS::Draw::getAngleRad( const QPointF& i_pt1, const QPointF& i_pt2 )
//------------------------------------------------------------------------------
{
    // Calculates the angle between point 2 and point 1.
    // Point 1 is considered to be the origin of the coordinate system.

    /*
            Q2        |      Q1        
      (PI/2 .. PI)    | (0.0 .. PI/2)  
      (90° .. 180°)   | (0 .. 90°)     
                      |                
    -----------------Pt1---------------
                      |                
      (-180° .. -90°) | (-90 .. 0°)    
      (-PI .. -PI/2)  | (-PI/2 .. 0.0) 
            Q3        |      Q4        
    */

    double fAngle_rad = 0.0;

    // If both points are at the same y position (horizontal line) ...
    if( i_pt1.y() == i_pt2.y() )
    {
        // If its a line from right to left ..
        if( i_pt2.x() < i_pt1.x() )
        {
            fAngle_rad = ZS::System::Math::c_fPI;
        }
    }
    else
    {
        double fWidth_px  = i_pt2.x() - i_pt1.x();
        double fRadius_px = getDist(i_pt1,i_pt2);

        fAngle_rad = acos(fWidth_px/fRadius_px);

        if( i_pt2.y() > i_pt1.y() )
        {
            fAngle_rad *= -1.0;
        }
    }

    return fAngle_rad;

} // getAngleRad

//------------------------------------------------------------------------------
QPointF ZS::Draw::rotatePoint( const QPointF& i_ptCenter, const QPointF& i_pt, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPointF ptRes = i_pt;

    if( i_fAngle_rad != 0.0 )
    {
        double fRadius_px     = getDist(i_ptCenter,i_pt);
        double fAnglePt_rad   = getAngleRad(i_ptCenter,i_pt);
        double fAngleRes_rad  = fAnglePt_rad + i_fAngle_rad;

        ptRes.setX( i_ptCenter.x() + fRadius_px * cos(fAngleRes_rad) );
        ptRes.setY( i_ptCenter.y() - fRadius_px * sin(fAngleRes_rad) );
    }

    return ptRes;

} // rotatePoint

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotateRect( const QPointF& i_ptCenter, const QRectF& i_rct, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygonRect;

    polygonRect.resize(4);

    polygonRect[0] = rotatePoint( i_ptCenter, i_rct.topLeft(), i_fAngle_rad );
    polygonRect[1] = rotatePoint( i_ptCenter, i_rct.topRight(), i_fAngle_rad );
    polygonRect[2] = rotatePoint( i_ptCenter, i_rct.bottomRight(), i_fAngle_rad );
    polygonRect[3] = rotatePoint( i_ptCenter, i_rct.bottomLeft(), i_fAngle_rad );

    return polygonRect;

} // rotateRect

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::rotatePolygon( const QPointF& i_ptCenter, const QPolygonF& i_polygon, double i_fAngle_rad )
//------------------------------------------------------------------------------
{
    QPolygonF polygon;

    if( i_polygon.size() > 0 )
    {
        polygon.resize(i_polygon.size());

        int idxPt;

        for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
        {
            if( i_fAngle_rad != 0.0 )
            {
                polygon[idxPt] = rotatePoint( i_ptCenter, i_polygon[idxPt], i_fAngle_rad );
            }
            else
            {
                polygon[idxPt] = i_polygon[idxPt];
            }
        }
    }
    return polygon;

} // rotatePolygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::normalizePolygon( const QPolygonF& i_plg, int i_iPrecision )
//------------------------------------------------------------------------------
{
    QPolygonF plg = i_plg;

    int idxPt;

    // Remove "identical" points:
    if( plg.size() > 2 )
    {
        QPointF pt1;
        QPointF pt2;
        QString strPt1;
        QString strPt2;

        for( idxPt = plg.size()-1; idxPt >= 2; idxPt-- )
        {
            pt1 = plg[idxPt];
            pt2 = plg[idxPt-1];

            strPt1 = point2Str(pt1,'f',0);
            strPt2 = point2Str(pt2,'f',0);

            if( strPt1 == strPt2 )
            {
                plg.remove(idxPt);
            }
        }
    }

    // Remove points on direct lines between two neighbor points:
    if( plg.size() > 3 )
    {
        bool    bPtRemoved = true;
        QLineF  ln;
        QPointF ptCheck;

        while( bPtRemoved )
        {
            bPtRemoved = false;

            for( idxPt = plg.size()-1; idxPt >= 2; idxPt-- )
            {
                ln = QLineF( plg[idxPt], plg[idxPt-2] );
                ptCheck = plg[idxPt-1];

                if( isLineHit(ln,ptCheck,i_iPrecision) )
                {
                    plg.remove(idxPt-1);

                    bPtRemoved = true;
                    break;

                } // if( isLineHit() )

            } // for( idxPt = plg.size()-1; idxPt >= 3; idxPt-- )

        } // while( bNormalizedTmp )

    } // if( plg.size() > 3 )

    return plg;

} // normalizePolygon

////------------------------------------------------------------------------------
//QRectF ZS::Draw::rotatePolygonRect2HorRect( const QPointF& i_ptCenter, const QPolygonF& i_polygonRect )
////------------------------------------------------------------------------------
//{
//    QRectF rctHor;
//
//    if( i_polygonRect.size() == 4 )
//    {
//        double fAngle_rad = getAngleRad( i_polygonRect[0], i_polygonRect[1] );
//
//        if( fAngle_rad != 0.0 )
//        {
//            QPointF ptTopLeft     = rotatePoint( i_ptCenter, i_polygonRect[0], -fAngle_rad );
//            QPointF ptBottomRight = rotatePoint( i_ptCenter, i_polygonRect[2], -fAngle_rad );
//
//            rctHor = QRectF( ptTopLeft, ptBottomRight );
//        }
//        else
//        {
//            rctHor = QRectF( i_polygonRect[0], i_polygonRect[2] );
//        }
//    }
//
//    return rctHor;
//
//} // rotatePolygonRect2HorRect

////------------------------------------------------------------------------------
//QPolygonF ZS::Draw::rotatePolygonRectSelectionPoints2HorRectSelectionPoints( const QPointF& i_ptCenter, const QPolygonF& i_polygonRectSelectionPoints )
////------------------------------------------------------------------------------
//{
//    QPolygonF polygonRectSelectionPoints;
//
//    polygonRectSelectionPoints.resize(ESelectionPointCount);
//
//    if( i_polygonRectSelectionPoints.size() == ESelectionPointCount )
//    {
//        double fAngle_rad = getAngleRad( i_polygonRectSelectionPoints[ESelectionPointTopLeft], i_polygonRectSelectionPoints[ESelectionPointTopRight] );
//
//        double fDistPtRotate2TopCenter = getDist(i_polygonRectSelectionPoints[ESelectionPointRotate],i_polygonRectSelectionPoints[ESelectionPointTopCenter]);
//
//        polygonRectSelectionPoints[ESelectionPointTopLeft] = rotatePoint( i_ptCenter, i_polygonRectSelectionPoints[ESelectionPointTopLeft], -fAngle_rad );
//        polygonRectSelectionPoints[ESelectionPointBottomRight] = rotatePoint( i_ptCenter, i_polygonRectSelectionPoints[ESelectionPointBottomRight], -fAngle_rad );
//
//        QRectF rctTmp( polygonRectSelectionPoints[ESelectionPointTopLeft], polygonRectSelectionPoints[ESelectionPointBottomRight] );
//
//        polygonRectSelectionPoints[ESelectionPointTopRight] = rctTmp.topRight();
//        polygonRectSelectionPoints[ESelectionPointBottomLeft] = rctTmp.bottomLeft();
//
//        QPointF ptCenterTmp = rctTmp.center();
//
//        polygonRectSelectionPoints[ESelectionPointLeftCenter].setX( rctTmp.left() );
//        polygonRectSelectionPoints[ESelectionPointLeftCenter].setY( ptCenterTmp.y() );
//        polygonRectSelectionPoints[ESelectionPointRightCenter].setX( rctTmp.right() );
//        polygonRectSelectionPoints[ESelectionPointRightCenter].setY( ptCenterTmp.y() );
//        polygonRectSelectionPoints[ESelectionPointTopCenter].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPointTopCenter].setY( rctTmp.top() );
//        polygonRectSelectionPoints[ESelectionPointBottomCenter].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPointBottomCenter].setY( rctTmp.bottom() );
//
//        polygonRectSelectionPoints[ESelectionPointRotate].setX( ptCenterTmp.x() );
//        polygonRectSelectionPoints[ESelectionPointRotate].setY( rctTmp.top() - fDistPtRotate2TopCenter );
//
//        polygonRectSelectionPoints[ESelectionPointCenter] = ptCenterTmp;
//        polygonRectSelectionPoints[ESelectionPointRotate] = ;
//    }
//
//    return polygonRectSelectionPoints;
//
//} // rotatePolygonRectSelectionPoints2HorRectSelectionPoints

//------------------------------------------------------------------------------
QPointF ZS::Draw::getCenterPoint( const QLineF& i_line )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    double fXLeft   = i_line.p1().x();
    double fYTop    = i_line.p1().y();
    double fXRight  = i_line.p2().x();
    double fYBottom = i_line.p2().y();

    if( fXLeft > fXRight )
    {
        double fXLeftTmp = fXLeft;
        fXLeft = fXRight;
        fXRight = fXLeftTmp;
    }
    if( fYTop > fYBottom )
    {
        double fYTopTmp = fYTop;
        fYTop = fYBottom;
        fYBottom = fYTopTmp;
    }

    ptCenter.setX( fXLeft + (fXRight-fXLeft)/2.0 );
    ptCenter.setY( fYTop + (fYBottom-fYTop)/2.0 );

    return ptCenter;

} // getCenterPoint

//------------------------------------------------------------------------------
QPointF ZS::Draw::getCenterPointOfPolygonRect( const QPolygonF& i_polygonRect )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    if( i_polygonRect.size() == 4 )
    {
        double fXLeft   = i_polygonRect[0].x();
        double fYTop    = i_polygonRect[0].y();
        double fXRight  = i_polygonRect[0].x();
        double fYBottom = i_polygonRect[0].y();
        int    idxPt;

        for( idxPt = 1; idxPt < i_polygonRect.size(); idxPt++ )
        {
            if( i_polygonRect[idxPt].x() < fXLeft )
            {
                fXLeft = i_polygonRect[idxPt].x();
            }
            if( i_polygonRect[idxPt].y() < fYTop )
            {
                fYTop = i_polygonRect[idxPt].y();
            }
            if( i_polygonRect[idxPt].x() > fXRight )
            {
                fXRight = i_polygonRect[idxPt].x();
            }
            if( i_polygonRect[idxPt].y() > fYBottom )
            {
                fYBottom = i_polygonRect[idxPt].y();
            }
        }

        if( fXLeft > fXRight )
        {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if( fYTop > fYBottom )
        {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }

        ptCenter.setX( fXLeft + (fXRight-fXLeft)/2.0 );
        ptCenter.setY( fYTop + (fYBottom-fYTop)/2.0 );

    } // if( i_polygonRect.size() == 4 )

    return ptCenter;

} // getCenterPointOfPolygonRect

//------------------------------------------------------------------------------
QPointF ZS::Draw::getMassCenterPointOfPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPointF ptCenter;

    if( i_polygon.size() == 1 )
    {
        ptCenter = i_polygon[0];
    }
    else if( i_polygon.size() == 2 )
    {
        ptCenter.setX( (i_polygon[0].x() + i_polygon[1].x()) / 2.0 );
        ptCenter.setY( (i_polygon[0].y() + i_polygon[1].y()) / 2.0 );
    }
    else // if( i_polygon.size() > 2 )
    {
        int    idxPt, idxPt2;
        double ai, atmp = 0, xtmp = 0, ytmp = 0;

        for( idxPt = i_polygon.size()-1, idxPt2 = 0; idxPt2 < i_polygon.size(); idxPt = idxPt2, idxPt2++ )
        {
            ai = i_polygon[idxPt].x() * i_polygon[idxPt2].y() - i_polygon[idxPt2].x() * i_polygon[idxPt].y();
            atmp += ai;
            xtmp += (i_polygon[idxPt2].x() + i_polygon[idxPt].x()) * ai;
            ytmp += (i_polygon[idxPt2].y() + i_polygon[idxPt].y()) * ai;
        }

        if( atmp != 0.0 )
        {
            ptCenter.setX( xtmp / (3.0 * atmp) );
            ptCenter.setY( ytmp / (3.0 * atmp) );
        }
        else
        {
            ptCenter = i_polygon[0];
        }
    }

    return ptCenter;

} // getMassCenterPointOfPolygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getBoundingRectPolygon( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    QPolygonF polygonBoundingRect;

    polygonBoundingRect.resize(4);

    if( i_polygon.size() > 0 )
    {
        double fXLeft   = i_polygon[0].x();
        double fYTop    = i_polygon[0].y();
        double fXRight  = i_polygon[0].x();
        double fYBottom = i_polygon[0].y();
        int    idxPt;

        for( idxPt = 1; idxPt < i_polygon.size(); idxPt++ )
        {
            if( i_polygon[idxPt].x() < fXLeft )
            {
                fXLeft = i_polygon[idxPt].x();
            }
            if( i_polygon[idxPt].y() < fYTop )
            {
                fYTop = i_polygon[idxPt].y();
            }
            if( i_polygon[idxPt].x() > fXRight )
            {
                fXRight = i_polygon[idxPt].x();
            }
            if( i_polygon[idxPt].y() > fYBottom )
            {
                fYBottom = i_polygon[idxPt].y();
            }
        }

        if( fXLeft > fXRight )
        {
            double fXLeftTmp = fXLeft;
            fXLeft = fXRight;
            fXRight = fXLeftTmp;
        }
        if( fYTop > fYBottom )
        {
            double fYTopTmp = fYTop;
            fYTop = fYBottom;
            fYBottom = fYTopTmp;
        }

        polygonBoundingRect[0] = QPointF(fXLeft,fYTop);
        polygonBoundingRect[1] = QPointF(fXRight,fYTop);
        polygonBoundingRect[2] = QPointF(fXRight,fYBottom);
        polygonBoundingRect[3] = QPointF(fXLeft,fYBottom);

    } // if( i_polygon.size() > 0 )

    return polygonBoundingRect;

} // getBoundingRectPolygon

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    QString str;

    str = QString::number(i_pt.x()) + "/" + QString::number(i_pt.y());

    return str;

} // point2Str

//------------------------------------------------------------------------------
QString ZS::Draw::point2Str( const QPointF& i_pt, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bRound2Nearest && i_iPrecision > 0 )
    {
        str = QString::number(Math::round2nearest(i_pt.x(),1),i_cF,i_iPrecision) + "/" + QString::number(Math::round2nearest(i_pt.y(),1),i_cF,i_iPrecision);
    }
    else
    {
        str = QString::number(i_pt.x(),i_cF,i_iPrecision) + "/" + QString::number(i_pt.y(),i_cF,i_iPrecision);
    }

    return str;

} // point2Str

//------------------------------------------------------------------------------
QPoint ZS::Draw::str2Point( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QPoint pt;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        int     x = strX.toInt(&bXOk);
        int     y = strY.toInt(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            pt.setX(x);
            pt.setY(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return pt;

} // str2Point

//------------------------------------------------------------------------------
QPointF ZS::Draw::str2PointF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool    bConverted = false;
    QPointF pt;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        double  x = strX.toDouble(&bXOk);
        double  y = strY.toDouble(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            pt.setX(x);
            pt.setY(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return pt;

} // str2PointF

//------------------------------------------------------------------------------
QString ZS::Draw::size2Str( const QSize& i_size )
//------------------------------------------------------------------------------
{
    QString str;

    str = QString::number(i_size.width()) + "/" + QString::number(i_size.height());

    return str;

} // size2Str

//------------------------------------------------------------------------------
QString ZS::Draw::size2Str( const QSizeF& i_size, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_bRound2Nearest && i_iPrecision > 0 )
    {
        str = QString::number(Math::round2nearest(i_size.width(),1),i_cF,i_iPrecision) + "/" + QString::number(Math::round2nearest(i_size.height(),1),i_cF,i_iPrecision);
    }
    else
    {
        str = QString::number(i_size.width(),i_cF,i_iPrecision) + "/" + QString::number(i_size.height(),i_cF,i_iPrecision);
    }

    return str;

} // size2Str

//------------------------------------------------------------------------------
QSize ZS::Draw::str2Size( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QSize siz;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        int     x = strX.toInt(&bXOk);
        int     y = strY.toInt(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            siz.setWidth(x);
            siz.setHeight(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return siz;

} // str2Size

//------------------------------------------------------------------------------
QSizeF ZS::Draw::str2SizeF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QSizeF siz;

    QStringList strlst = i_str.split("/");

    if( strlst.size() == 2 )
    {
        QString strX = strlst[0];
        QString strY = strlst[1];
        bool    bXOk, bYOk;
        double  x = strX.toDouble(&bXOk);
        double  y = strY.toDouble(&bYOk);

        bConverted = bXOk && bYOk;

        if( bConverted )
        {
            siz.setWidth(x);
            siz.setHeight(y);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return siz;

} // str2SizeF

//------------------------------------------------------------------------------
QString ZS::Draw::line2Str( const QLine& i_line )
//------------------------------------------------------------------------------
{
    QString str;

    str = point2Str(i_line.p1()) + ", " + point2Str(i_line.p2());

    return str;

} // line2Str

//------------------------------------------------------------------------------
QString ZS::Draw::line2Str( const QLineF& i_line, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;

    str = point2Str(i_line.p1(),i_cF,i_iPrecision,i_bRound2Nearest) + ", " + point2Str(i_line.p2(),i_cF,i_iPrecision,i_bRound2Nearest);

    return str;

} // line2Str

//------------------------------------------------------------------------------
QLine ZS::Draw::str2Line( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QLine line;

    QStringList strlst = i_str.split(",");

    if( strlst.size() == 2 )
    {
        QString strP1 = strlst[0];
        QString strP2 = strlst[1];
        bool    bP1Ok, bP2Ok;
        QPoint  p1 = str2Point(strP1,&bP1Ok);
        QPoint  p2 = str2Point(strP2,&bP2Ok);

        bConverted = bP1Ok && bP2Ok;

        if( bConverted )
        {
            line.setP1(p1);
            line.setP2(p2);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return line;

} // str2Line

//------------------------------------------------------------------------------
QLineF ZS::Draw::str2LineF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QLineF line;

    QStringList strlst = i_str.split(",");

    if( strlst.size() == 2 )
    {
        QString strP1 = strlst[0];
        QString strP2 = strlst[1];
        bool    bP1Ok, bP2Ok;
        QPointF p1 = str2PointF(strP1,&bP1Ok);
        QPointF p2 = str2PointF(strP2,&bP2Ok);

        bConverted = bP1Ok && bP2Ok;

        if( bConverted )
        {
            line.setP1(p1);
            line.setP2(p2);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return line;

} // str2LineF

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QRect& i_rct, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    str  = point2Str(i_rct.topLeft()) + ", ";
    str += point2Str(i_rct.topRight()) + ", ";
    str += point2Str(i_rct.bottomRight()) + ", ";
    str += point2Str(i_rct.bottomLeft());

    if( i_bAddWidthAndHeight )
    {
        str += ", " + size2Str(i_rct.size());
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QRectF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    str  = point2Str(i_rct.topLeft(),i_cF,i_iPrecision,i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.topRight(),i_cF,i_iPrecision,i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.bottomRight(),i_cF,i_iPrecision,i_bRound2Nearest) + ", ";
    str += point2Str(i_rct.bottomLeft(),i_cF,i_iPrecision,i_bRound2Nearest);

    if( i_bAddWidthAndHeight )
    {
        str += ", " + size2Str(i_rct.size(),i_cF,i_iPrecision,i_bRound2Nearest);
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygon& i_rct, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_rct.size() >= 4 )
    {
        int idxPt;

        for( idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++ )
        {
            str += point2Str(i_rct[idxPt]);

            if( idxPt < 3 )
            {
                str += ", ";
            }
        }

        if( i_bAddWidthAndHeight )
        {
            double w = getDist(i_rct[0],i_rct[1]);
            double h = getDist(i_rct[1],i_rct[2]);
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );

            str += ", " + size2Str(siz);
        }
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QString ZS::Draw::rect2Str( const QPolygonF& i_rct, char i_cF, int i_iPrecision, bool i_bRound2Nearest, bool i_bAddWidthAndHeight )
//------------------------------------------------------------------------------
{
    QString str;

    if( i_rct.size() >= 4 )
    {
        int idxPt;

        for( idxPt = 0; idxPt < i_rct.size() && idxPt < 4; idxPt++ )
        {
            str += point2Str(i_rct[idxPt],i_cF,i_iPrecision,i_bRound2Nearest);

            if( idxPt < 3 )
            {
                str += ", ";
            }
        }

        if( i_bAddWidthAndHeight )
        {
            double w = getDist(i_rct[0],i_rct[1]);
            double h = getDist(i_rct[1],i_rct[2]);
            QSize  siz( static_cast<int>(w), static_cast<int>(h) );

            str += ", " + size2Str(siz);
        }
    }

    return str;

} // rect2Str

//------------------------------------------------------------------------------
QRect ZS::Draw::str2Rect( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool  bConverted = false;
    QRect rct;

    QStringList strlst = i_str.split(",");

    if( strlst.size() >= 4 )
    {
        QString strLT = strlst[0];
        QString strRT = strlst[1];
        QString strRB = strlst[2];
        QString strLB = strlst[3];

        bool    bLTOk, bRTOk, bRBOk, bLBOk;
        QPoint  ptLT = str2Point(strLT,&bLTOk);
        QPoint  ptRB = str2Point(strRB,&bRBOk);

        str2Point(strRT,&bRTOk);
        str2Point(strLB,&bLBOk);

        bConverted = bLTOk && bRTOk && bRBOk && bLBOk;

        if( bConverted )
        {
            rct = QRect(ptLT,ptRB);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return rct;

} // str2Rect

//------------------------------------------------------------------------------
QRectF ZS::Draw::str2RectF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool   bConverted = false;
    QRectF rct;

    QStringList strlst = i_str.split(",");

    if( strlst.size() >= 4 )
    {
        QString strLT = strlst[0];
        QString strRT = strlst[1];
        QString strRB = strlst[2];
        QString strLB = strlst[3];

        bool    bLTOk, bRTOk, bRBOk, bLBOk;
        QPointF ptLT = str2PointF(strLT,&bLTOk);
        QPointF ptRB = str2PointF(strRB,&bRBOk);

        str2PointF(strRT,&bRTOk);
        str2PointF(strLB,&bLBOk);

        bConverted = bLTOk && bRTOk && bRBOk && bLBOk;

        if( bConverted )
        {
            rct = QRectF(ptLT,ptRB);
        }
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return rct;

} // str2RectF

//------------------------------------------------------------------------------
QString ZS::Draw::polygon2Str( const QPolygon& i_polygon )
//------------------------------------------------------------------------------
{
    QString str;
    int     idxPt;

    for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
    {
        str += point2Str(i_polygon[idxPt]);

        if( idxPt < i_polygon.size()-1 )
        {
            str += ", ";
        }
    }

    return str;

} // polygon2Str

//------------------------------------------------------------------------------
QString ZS::Draw::polygon2Str( const QPolygonF& i_polygon, char i_cF, int i_iPrecision, bool i_bRound2Nearest )
//------------------------------------------------------------------------------
{
    QString str;
    int     idxPt;

    for( idxPt = 0; idxPt < i_polygon.size(); idxPt++ )
    {
        str += point2Str(i_polygon[idxPt],i_cF,i_iPrecision,i_bRound2Nearest);

        if( idxPt < i_polygon.size()-1 )
        {
            str += ", ";
        }
    }

    return str;

} // polygon2Str

//------------------------------------------------------------------------------
QPolygon ZS::Draw::str2Polygon( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool     bConverted = false;
    QPolygon plg;

    QStringList strlst = i_str.split(",");
    int         idxPt;

    for( idxPt = 0; idxPt < strlst.size(); idxPt++ )
    {
        QString strPt = strlst[idxPt];
        QPoint  pt    = str2Point(strPt,&bConverted);

        if( !bConverted )
        {
            break;
        }

        plg.append(pt);
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return plg;

} // str2Polygon

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::str2PolygonF( const QString& i_str, bool* i_pbConverted )
//------------------------------------------------------------------------------
{
    bool      bConverted = false;
    QPolygonF plg;

    QStringList strlst = i_str.split(",");
    int         idxPt;

    for( idxPt = 0; idxPt < strlst.size(); idxPt++ )
    {
        QString strPt = strlst[idxPt];
        QPointF pt    = str2PointF(strPt,&bConverted);

        if( !bConverted )
        {
            break;
        }

        plg.append(pt);
    }

    if( i_pbConverted != nullptr )
    {
        *i_pbConverted = bConverted;
    }

    return plg;

} // str2PolygonF

//------------------------------------------------------------------------------
QPolygonF ZS::Draw::getEllipseFocusPoints( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    QPolygonF plgFocusPoints(2);

    QPointF ptC = i_rct.center();

    if( i_rct.width() == i_rct.height() )
    {
        plgFocusPoints[0] = ptC;
        plgFocusPoints[1] = ptC;
    }
    else if( i_rct.width() > i_rct.height() )
    {
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x() - fe, ptC.y() );
        plgFocusPoints[1] = QPointF( ptC.x() + fe, ptC.y() );
    }
    else // if( i_rct.width() < i_rct.height() )
    {
        double fe = getEllipseCenterFocusDist(i_rct);
        plgFocusPoints[0] = QPointF( ptC.x(), ptC.y() - fe );
        plgFocusPoints[1] = QPointF( ptC.x(), ptC.y() + fe );
    }
    return plgFocusPoints;

} // getEllipseFocusPoints

//------------------------------------------------------------------------------
double ZS::Draw::getEllipseCenterFocusDist( const QRectF& i_rct )
//------------------------------------------------------------------------------
{
    double fa = i_rct.width();
    double fb = i_rct.height();

    if( fa < fb )
    {
        double fTmp = fb;
        fb = fa;
        fa = fTmp;
    }

    double fe = sqrt( Math::sqr(fa) - Math::sqr(fb) );

    return fe;

} // getEllipseCenterFocusDist


/*******************************************************************************
Libraries depending on build configuration and used Qt version
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma message(__FILE__ ": Linking against = " QTCORELIB)
#pragma comment(lib, QTCORELIB)
#pragma message(__FILE__ ": Linking against = " QTNETWORKLIB)
#pragma comment(lib, QTNETWORKLIB)
#pragma message(__FILE__ ": Linking against = " QTXMLLIB)
#pragma comment(lib, QTXMLLIB)
#pragma message(__FILE__ ": Linking against = " QTGUILIB)
#pragma comment( lib, QTGUILIB )
#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#pragma message(__FILE__ ": Linking against = " QTWIDGETSLIB)
#pragma comment( lib, QTWIDGETSLIB )
#endif

#pragma message(__FILE__ ": Linking against = " ZSSYSLIB)
#pragma comment(lib, ZSSYSLIB)
#pragma message(__FILE__ ": Linking against = " ZSSYSGUILIB)
#pragma comment(lib, ZSSYSGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCLIB)
#pragma comment(lib, ZSIPCLIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCGUILIB)
#pragma comment(lib, ZSIPCGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACELIB)
#pragma comment(lib, ZSIPCTRACELIB)
#pragma message(__FILE__ ": Linking against = " ZSIPCTRACEGUILIB)
#pragma comment(lib, ZSIPCTRACEGUILIB)
#pragma message(__FILE__ ": Linking against = " ZSPHYSVALLIB)
#pragma comment(lib, ZSPHYSVALLIB)
#pragma message(__FILE__ ": Linking against = " ZSPHYSSIZESGEOMETRYLIB)
#pragma comment(lib, ZSPHYSSIZESGEOMETRYLIB)

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the DLL application.
*******************************************************************************/

#ifdef _WINDOWS
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
    HANDLE /*i_hModule*/,
    DWORD  i_ul_reason_for_call,
    LPVOID /*i_lpReserved*/ )
//------------------------------------------------------------------------------
{
    switch( i_ul_reason_for_call )
    {
        case DLL_PROCESS_ATTACH:
        {
            break;
        }
        case DLL_THREAD_ATTACH:
        {
            break;
        }
        case DLL_THREAD_DETACH:
        {
            break;
        }
        case DLL_PROCESS_DETACH:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return TRUE;

} // DllMain
#endif
