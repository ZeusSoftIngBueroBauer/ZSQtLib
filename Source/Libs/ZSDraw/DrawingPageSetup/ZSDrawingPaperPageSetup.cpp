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

#error Module not used anymore

#include <QtCore/qsettings.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawingPaperPageSetup.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingPaperPageSetup
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

////------------------------------------------------------------------------------
//double CDrawingPaperPageSetup::getFormatWidthInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation )
////------------------------------------------------------------------------------
//{
//    double fFormatWidth_mm = c_fPaperFormatWidthDinA4_mm;
//
//    switch( i_orientation )
//    {
//        case EPaperOrientationUpright:
//        {
//            switch( i_format )
//            {
//                case EPaperFormatDinA5:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatWidthDinA4_mm/2.0;
//                    break;
//                }
//                case EPaperFormatDinA4:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatWidthDinA4_mm;
//                    break;
//                }
//                case EPaperFormatDinA3:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatWidthDinA4_mm*2.0;
//                    break;
//                }
//                case EPaperFormatUserDefined:
//                {
//                    break;
//                }
//            }
//            break;
//        }
//        case EPaperOrientationLandscape:
//        {
//            switch( i_format )
//            {
//                case EPaperFormatDinA5:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatHeightDinA4_mm/2.0;
//                    break;
//                }
//                case EPaperFormatDinA4:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatHeightDinA4_mm;
//                    break;
//                }
//                case EPaperFormatDinA3:
//                {
//                    fFormatWidth_mm  = c_fPaperFormatHeightDinA4_mm*2.0;
//                    break;
//                }
//                case EPaperFormatUserDefined:
//                {
//                    break;
//                }
//            }
//            break;
//        }
//    }
//    return fFormatWidth_mm;
//
//} // getFormatWidthInMilliMeter

////------------------------------------------------------------------------------
//double CDrawingPaperPageSetup::getFormatHeightInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation )
////------------------------------------------------------------------------------
//{
//    double fFormatHeight_mm = c_fPaperFormatHeightDinA4_mm;
//
//    switch( i_orientation )
//    {
//        case EPaperOrientationUpright:
//        {
//            switch( i_format )
//            {
//                case EPaperFormatDinA5:
//                {
//                    fFormatHeight_mm = c_fPaperFormatHeightDinA4_mm/2.0;
//                    break;
//                }
//                case EPaperFormatDinA4:
//                {
//                    fFormatHeight_mm = c_fPaperFormatHeightDinA4_mm;
//                    break;
//                }
//                case EPaperFormatDinA3:
//                {
//                    fFormatHeight_mm = c_fPaperFormatHeightDinA4_mm*2.0;
//                    break;
//                }
//                case EPaperFormatUserDefined:
//                {
//                    break;
//                }
//            }
//            break;
//        }
//        case EPaperOrientationLandscape:
//        {
//            switch( i_format )
//            {
//                case EPaperFormatDinA5:
//                {
//                    fFormatHeight_mm = c_fPaperFormatWidthDinA4_mm/2.0;
//                    break;
//                }
//                case EPaperFormatDinA4:
//                {
//                    fFormatHeight_mm = c_fPaperFormatWidthDinA4_mm;
//                    break;
//                }
//                case EPaperFormatDinA3:
//                {
//                    fFormatHeight_mm = c_fPaperFormatWidthDinA4_mm*2.0;
//                    break;
//                }
//                case EPaperFormatUserDefined:
//                {
//                    break;
//                }
//            }
//            break;
//        }
//    }
//    return fFormatHeight_mm;
//
//} // getFormatHeightInMilliMeter

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingPaperPageSetup::CDrawingPaperPageSetup()
//------------------------------------------------------------------------------
{
} // ctor

//------------------------------------------------------------------------------
CDrawingPaperPageSetup::~CDrawingPaperPageSetup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingPaperPageSetup::operator == ( const CDrawingPaperPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CDrawingPaperPageSetup::operator != ( const CDrawingPaperPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingPaperPageSetup::save( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
} // save

//------------------------------------------------------------------------------
void CDrawingPaperPageSetup::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingPaperPageSetup::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
} // save

//------------------------------------------------------------------------------
void CDrawingPaperPageSetup::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setPaperFormat( EPaperFormat i_format )
////------------------------------------------------------------------------------
//{
//    m_paperFormat      = i_format;
//    m_fFormatWidth_mm  = getFormatWidthInMilliMeter(m_paperFormat,m_paperOrientation);
//    m_fFormatHeight_mm = getFormatHeightInMilliMeter(m_paperFormat,m_paperOrientation);
//
//} // setPaperFormat

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setPaperOrientation( EPaperOrientation i_orientation )
////------------------------------------------------------------------------------
//{
//    m_paperOrientation = i_orientation;
//    m_fFormatWidth_mm  = getFormatWidthInMilliMeter(m_paperFormat,m_paperOrientation);
//    m_fFormatHeight_mm = getFormatHeightInMilliMeter(m_paperFormat,m_paperOrientation);
//
//} // setPaperOrientation

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setPrinterPaperSource( EPrinterPaperSource i_source )
////------------------------------------------------------------------------------
//{
//    m_printerPaperSource = i_source;
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setZoomFactorInPerCent( double i_fZoomFactor_percent )
////------------------------------------------------------------------------------
//{
//    m_fZoomFactor_percent = i_fZoomFactor_percent;
//}

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setFormatWidthInMilliMeter( double i_fWidth_mm )
////------------------------------------------------------------------------------
//{
//    m_fFormatWidth_mm = i_fWidth_mm;
//
//    if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm/2.0
//     && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA5;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm/2.0
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA5;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm
//          && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm )
//    {
//        m_paperFormat      = EPaperFormatDinA4;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm )
//    {
//        m_paperFormat      = EPaperFormatDinA4;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm*2.0
//          && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm*2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA3;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm/2.0
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA3;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else
//    {
//        m_paperFormat = EPaperFormatUserDefined;
//    }
//
//}  // setFormatWidthInMilliMeter

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setFormatHeightInMilliMeter( double i_fHeight_mm )
////------------------------------------------------------------------------------
//{
//    m_fFormatHeight_mm = i_fHeight_mm;
//
//    if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm/2.0
//     && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA5;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm/2.0
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA5;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm
//          && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm )
//    {
//        m_paperFormat      = EPaperFormatDinA4;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm )
//    {
//        m_paperFormat      = EPaperFormatDinA4;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatWidthDinA4_mm*2.0
//          && m_fFormatHeight_mm == c_fPaperFormatHeightDinA4_mm*2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA3;
//        m_paperOrientation = EPaperOrientationUpright;
//    }
//    else if( m_fFormatWidth_mm == c_fPaperFormatHeightDinA4_mm/2.0
//          && m_fFormatHeight_mm == c_fPaperFormatWidthDinA4_mm/2.0 )
//    {
//        m_paperFormat      = EPaperFormatDinA3;
//        m_paperOrientation = EPaperOrientationLandscape;
//    }
//    else
//    {
//        m_paperFormat = EPaperFormatUserDefined;
//    }
//
//} // setFormatHeightInMilliMeter

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setMarginTopInMilliMeter( double i_fTop_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginTop_mm = i_fTop_mm;
//}
//
////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setMarginBottomInMilliMeter( double i_fBottom_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginBottom_mm = i_fBottom_mm;
//}
//
////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setMarginLeftInMilliMeter( double i_fLeft_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginLeft_mm = i_fLeft_mm;
//}
//
////------------------------------------------------------------------------------
//void CDrawingPaperPageSetup::setMarginRightInMilliMeter( double i_fRight_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginRight_mm = i_fRight_mm;
//}

///*==============================================================================
//public: // instance methods
//==============================================================================*/

////------------------------------------------------------------------------------
//double CDrawingPaperPageSetup::getDrawingWidthInMilliMeter() const  // area between left and right margin
////------------------------------------------------------------------------------
//{
//    return (m_fFormatWidth_mm - m_fMarginLeft_mm - m_fMarginRight_mm);
//}
//
////------------------------------------------------------------------------------
//double CDrawingPaperPageSetup::getDrawingHeightInMilliMeter() const // area between top and bottom margin
////------------------------------------------------------------------------------
//{
//    return (m_fFormatHeight_mm - m_fMarginTop_mm - m_fMarginBottom_mm);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingPaperPageSetup::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    return str;
}
