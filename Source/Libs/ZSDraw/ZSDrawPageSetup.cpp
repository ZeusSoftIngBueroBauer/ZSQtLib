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

#include <QtCore/qsettings.h>
#include <QtGui/qpaintdevice.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawPageSetup.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
//using namespace ZS::PhysVal;


/*******************************************************************************
class CPageSetup
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

////------------------------------------------------------------------------------
//double CPageSetup::getFormatWidthInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation )
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
//
////------------------------------------------------------------------------------
//double CPageSetup::getFormatHeightInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation )
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
/*! Creates an instance of the class.

    @param i_fXResolution_dpmm [in]
        Resolution of one pixel per milli meter on the screen in X (horizontal) direction.
        Defaults to 96.0 dpi (=3.78 dpmm).
    @param i_fXResolution_dpmm [in]
        Resolution of one pixel per milli meter on the screen in Y (vertical) direction.
        Defaults to 96.0 dpi (=3.78 dpmm).
*/
CPageSetup::CPageSetup( double i_fXResolution_dpmm, double i_fYResolution_dpmm ) :
//------------------------------------------------------------------------------
    m_fXResolution_dpmm(i_fXResolution_dpmm),
    m_fYResolution_dpmm(i_fYResolution_dpmm),
    m_cxImageWidth_px(800),
    m_cyImageHeight_px(600),
    m_physSizeWidth(i_fXResolution_dpmm),
    m_physSizeHeight(i_fYResolution_dpmm)
{
} // ctor

//------------------------------------------------------------------------------
CPageSetup::~CPageSetup()
//------------------------------------------------------------------------------
{
    m_fXResolution_dpmm = 0.0;
    m_fYResolution_dpmm = 0.0;
    m_cxImageWidth_px = 0;
    m_cyImageHeight_px = 0;
    //m_physSizeWidth;
    //m_physSizeHeight;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPageSetup::operator == ( const CPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_fXResolution_dpmm != i_other.m_fXResolution_dpmm )
    {
        bEqual = false;
    }
    else if( m_fYResolution_dpmm != i_other.m_fYResolution_dpmm )
    {
        bEqual = false;
    }
    else if( m_cxImageWidth_px != i_other.m_cxImageWidth_px )
    {
        bEqual = false;
    }
    else if( m_cyImageHeight_px != i_other.m_cyImageHeight_px )
    {
        bEqual = false;
    }

    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CPageSetup::operator != ( const CPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::setXResolutionInDpmm( double i_fRes_dpmm )
//------------------------------------------------------------------------------
{
    if( m_fXResolution_dpmm != i_fRes_dpmm )
    {
        m_fXResolution_dpmm = i_fRes_dpmm;

        m_physSizeWidth.setDpmm(i_fRes_dpmm);
    }
}

//------------------------------------------------------------------------------
double CPageSetup::getXResolutionInDpmm() const
//------------------------------------------------------------------------------
{
    return m_fXResolution_dpmm;
}

//------------------------------------------------------------------------------
void CPageSetup::setYResolutionInDpmm( double i_fRes_dpmm )
//------------------------------------------------------------------------------
{
    if( m_fYResolution_dpmm != i_fRes_dpmm )
    {
        m_fYResolution_dpmm = i_fRes_dpmm;

        m_physSizeHeight.setDpmm(i_fRes_dpmm);
    }
}

//------------------------------------------------------------------------------
double CPageSetup::getYResolutionInDpmm() const
//------------------------------------------------------------------------------
{
    return m_fYResolution_dpmm;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::save( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    //i_settings.setValue( i_strSettingsKey + "/" + strAttrName, strAttrValue );

} // save

//------------------------------------------------------------------------------
void CPageSetup::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    //strAttrValue = i_settings.value( i_strSettingsKey + "/" + strAttrName, strAttrValue ).toString();

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    //QString strName;
    //QString strValue;

    //strName = getAttributeName(idxAttr);
    //strValue = attributeValue2Str(idxAttr);

    //i_xmlStreamWriter.writeTextElement(strName,strValue);

} // save

//------------------------------------------------------------------------------
void CPageSetup::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QString strElemName;
    QString strElemText;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
        } // if( i_xmlStreamReader.isStartElement() )

        else if( i_xmlStreamReader.isEndElement() )
        {
        } // if( i_xmlStreamReader.isEndElement() )
    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CPageSetup::setPaperFormat( EPaperFormat i_format )
////------------------------------------------------------------------------------
//{
//    m_paperFormat      = i_format;
//    m_fFormatWidth_mm  = getFormatWidthInMilliMeter(m_paperFormat,m_paperOrientation);
//    m_fFormatHeight_mm = getFormatHeightInMilliMeter(m_paperFormat,m_paperOrientation);
//
//} // setPaperFormat

////------------------------------------------------------------------------------
//void CPageSetup::setPaperOrientation( EPaperOrientation i_orientation )
////------------------------------------------------------------------------------
//{
//    m_paperOrientation = i_orientation;
//    m_fFormatWidth_mm  = getFormatWidthInMilliMeter(m_paperFormat,m_paperOrientation);
//    m_fFormatHeight_mm = getFormatHeightInMilliMeter(m_paperFormat,m_paperOrientation);
//
//} // setPaperOrientation

////------------------------------------------------------------------------------
//void CPageSetup::setPrinterPaperSource( EPrinterPaperSource i_source )
////------------------------------------------------------------------------------
//{
//    m_printerPaperSource = i_source;
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CPageSetup::setZoomFactorInPerCent( double i_fZoomFactor_percent )
////------------------------------------------------------------------------------
//{
//    m_fZoomFactor_percent = i_fZoomFactor_percent;
//}

////------------------------------------------------------------------------------
//void CPageSetup::setFormatWidthInMilliMeter( double i_fWidth_mm )
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
//void CPageSetup::setFormatHeightInMilliMeter( double i_fHeight_mm )
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
//void CPageSetup::setMarginTopInMilliMeter( double i_fTop_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginTop_mm = i_fTop_mm;
//}
//
////------------------------------------------------------------------------------
//void CPageSetup::setMarginBottomInMilliMeter( double i_fBottom_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginBottom_mm = i_fBottom_mm;
//}
//
////------------------------------------------------------------------------------
//void CPageSetup::setMarginLeftInMilliMeter( double i_fLeft_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginLeft_mm = i_fLeft_mm;
//}
//
////------------------------------------------------------------------------------
//void CPageSetup::setMarginRightInMilliMeter( double i_fRight_mm )
////------------------------------------------------------------------------------
//{
//    m_fMarginRight_mm = i_fRight_mm;
//}
//
///*==============================================================================
//public: // instance methods
//==============================================================================*/
//
////------------------------------------------------------------------------------
//double CPageSetup::getDrawingWidthInMilliMeter() const  // area between left and right margin
////------------------------------------------------------------------------------
//{
//    return (m_fFormatWidth_mm - m_fMarginLeft_mm - m_fMarginRight_mm);
//}
//
////------------------------------------------------------------------------------
//double CPageSetup::getDrawingHeightInMilliMeter() const // area between top and bottom margin
////------------------------------------------------------------------------------
//{
//    return (m_fFormatHeight_mm - m_fMarginTop_mm - m_fMarginBottom_mm);
//}

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CPageSetup::setDrawArea( const SDrawArea& i_drawArea )
////------------------------------------------------------------------------------
//{
//    m_drawArea = i_drawArea;
//}

//------------------------------------------------------------------------------
void CPageSetup::setDrawingWidthInPixels( int i_iWidth_px )
//------------------------------------------------------------------------------
{
    m_cxImageWidth_px = i_iWidth_px;
}

//------------------------------------------------------------------------------
int CPageSetup::getDrawingWidthInPixels() const
//------------------------------------------------------------------------------
{
    return m_cxImageWidth_px;
}

////------------------------------------------------------------------------------
//void CPageSetup::setDrawingWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_physValWidth = i_physValWidth;
//}

//------------------------------------------------------------------------------
void CPageSetup::setDrawingHeightInPixels( int i_iHeight_px )
//------------------------------------------------------------------------------
{
    m_cyImageHeight_px = i_iHeight_px;
}

//------------------------------------------------------------------------------
int CPageSetup::getDrawingHeightInPixels() const
//------------------------------------------------------------------------------
{
    return m_cyImageHeight_px;
}

////------------------------------------------------------------------------------
//void CPageSetup::setDrawingHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_physValHeight = i_physValHeight;
//}

////------------------------------------------------------------------------------
//void CPageSetup::setDrawingXScaleFactor( double i_fScaleFactor )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_fXScaleFac = i_fScaleFactor;
//}

////------------------------------------------------------------------------------
//void CPageSetup::setDrawingYScaleFactor( double i_fScaleFactor )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_fYScaleFac = i_fScaleFactor;
//}
