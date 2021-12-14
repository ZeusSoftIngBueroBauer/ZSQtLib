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

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSDraw/ZSDrawPageSetup.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
enum EPageSetupAttribute
*******************************************************************************/

const SEnumEntry s_arEnumStrPageSetupAttributes[] =
{
    /*  0 */ SEnumEntry( EPageSetupAttributeDrawingWidthInPixels,  "DrawingWidthInPixels"  ),
    /*  1 */ SEnumEntry( EPageSetupAttributeDrawingHeightInPixels, "DrawingHeightInPixels" )
};

//------------------------------------------------------------------------------
QString ZS::Draw::pageSetupAttribute2Str( int i_iAttribute )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrPageSetupAttributes,_ZSArrLen(s_arEnumStrPageSetupAttributes),i_iAttribute);
}

//------------------------------------------------------------------------------
EPageSetupAttribute ZS::Draw::str2PageSetupAttribute( const QString& i_str )
//------------------------------------------------------------------------------
{
    EPageSetupAttribute pageSetupAttribute = EPageSetupAttributeUndefined;
    int iPageSetupAttribute = SEnumEntry::str2Enumerator(s_arEnumStrPageSetupAttributes,_ZSArrLen(s_arEnumStrPageSetupAttributes),i_str);
    if( iPageSetupAttribute >= 0 && iPageSetupAttribute < _ZSArrLen(s_arEnumStrPageSetupAttributes) )
    {
        pageSetupAttribute = static_cast<EPageSetupAttribute>(iPageSetupAttribute);
    }
    else
    {
        bool bConverted;
        iPageSetupAttribute = i_str.toInt(&bConverted);
        if( bConverted && iPageSetupAttribute >= 0 && iPageSetupAttribute < _ZSArrLen(s_arEnumStrPageSetupAttributes) )
        {
            pageSetupAttribute = static_cast<EPageSetupAttribute>(iPageSetupAttribute);
        }
    }
    return pageSetupAttribute;

} // str2PageSetupAttribute


/*******************************************************************************
class CPageSetup
*******************************************************************************/

//------------------------------------------------------------------------------
static const SAttribute s_arPageSetupAttributesDefaultVals[EPageSetupAttributeCount] =
//------------------------------------------------------------------------------
{
    /*  0 DrawingWidthInPixels  */ SAttribute( "DrawingWidthInPixels",  800, true ),
    /*  1 DrawingHeightInPixels */ SAttribute( "DrawingHeightInPixels", 600, true )
};

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
CPageSetup::CPageSetup()
//------------------------------------------------------------------------------
    //m_arAttributes[EPageSetupAttributeCount]
{
    set2Default();

} // ctor

//------------------------------------------------------------------------------
CPageSetup::~CPageSetup()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPageSetup::operator == ( const CPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    int idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
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
bool CPageSetup::operator != ( const CPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::save( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    QString strAttrName;
    QString strAttrValue;
    int     idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
    {
        strAttrName = getAttributeName(idxAttr);
        strAttrValue = attributeValue2Str(idxAttr); // current value

        i_settings.setValue( i_strSettingsKey + "/" + strAttrName, strAttrValue );
    }

} // save

//------------------------------------------------------------------------------
void CPageSetup::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    QString strAttrName;
    QString strAttrValue;
    int     idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
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
void CPageSetup::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    QString strName;
    QString strValue;
    int     idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
    {
        if( isAttributeUsed(idxAttr) && !isDefault(idxAttr) )
        {
            strName = getAttributeName(idxAttr);
            strValue = attributeValue2Str(idxAttr);

            i_xmlStreamWriter.writeTextElement(strName,strValue);
        }
    }

} // save

//------------------------------------------------------------------------------
void CPageSetup::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QString strElemName;
    QString strElemText;
    int     iAttribute;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        strElemName = i_xmlStreamReader.name().toString();

        if( i_xmlStreamReader.isStartElement() )
        {
            iAttribute = findAttribute(strElemName);

            if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
            {
                strElemText = i_xmlStreamReader.readElementText();

                setAttribute(iAttribute,strElemText);
            }

        } // if( i_xmlStreamReader.isStartElement() )

        else if( i_xmlStreamReader.isEndElement() )
        {
            if( strElemName == "PageSetup" )
            {
                break;
            }

        } // if( i_xmlStreamReader.isEndElement() )

    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::set2Default()
//------------------------------------------------------------------------------
{
    int idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
    {
        m_arAttributes[idxAttr] = s_arPageSetupAttributesDefaultVals[idxAttr];
    }

} // set2Default

//------------------------------------------------------------------------------
void CPageSetup::set2Default( int i_iAttribute )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[i_iAttribute] = s_arPageSetupAttributesDefaultVals[i_iAttribute];
    }

} // set2Default

//------------------------------------------------------------------------------
void CPageSetup::set2Default( const QString& i_strName )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[iAttribute] = s_arPageSetupAttributesDefaultVals[iAttribute];
    }

} // set2Default

//------------------------------------------------------------------------------
bool CPageSetup::isDefault() const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    int idxAttr;

    for( idxAttr = 0; idxAttr < EPageSetupAttributeCount; idxAttr++ )
    {
        if( isAttributeUsed(idxAttr) )
        {
            if( m_arAttributes[idxAttr] != s_arPageSetupAttributesDefaultVals[idxAttr] )
            {
                bIsDefault = false;
                break;
            }
        }
    }
    return bIsDefault;

} // isDefault

//------------------------------------------------------------------------------
bool CPageSetup::isDefault( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        if( isAttributeUsed(i_iAttribute) )
        {
            if( m_arAttributes[i_iAttribute] != s_arPageSetupAttributesDefaultVals[i_iAttribute] )
            {
                bIsDefault = false;
            }
        }
    }
    return bIsDefault;

} // isDefault

//------------------------------------------------------------------------------
bool CPageSetup::isDefault( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsDefault = true;

    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        bIsDefault = isDefault(iAttribute);
    }
    return bIsDefault;

} // isDefault

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::setAttribute( int i_iAttribute, const SAttribute& i_drawAttribute )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[i_iAttribute] = i_drawAttribute;
    }

} // setAttribute

//------------------------------------------------------------------------------
void CPageSetup::setAttribute( int i_iAttribute, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
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
void CPageSetup::setAttribute( int i_iAttribute, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        switch( i_iAttribute )
        {
            case EPageSetupAttributeDrawingWidthInPixels:
            {
                m_arAttributes[i_iAttribute].m_val = i_strVal.toInt();
                break;
            }
            case EPageSetupAttributeDrawingHeightInPixels:
            {
                m_arAttributes[i_iAttribute].m_val = i_strVal.toInt();
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
void CPageSetup::setAttribute( const QString& i_strName, const QVariant& i_val )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[iAttribute] = i_val;
    }

} // setAttribute

//------------------------------------------------------------------------------
void CPageSetup::setAttribute( const QString& i_strName, const QString& i_strVal )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        setAttribute(iAttribute,i_strVal);
    }

} // setAttribute

//------------------------------------------------------------------------------
SAttribute CPageSetup::getAttribute( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    SAttribute drawAttribute;

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        drawAttribute = m_arAttributes[i_iAttribute];
    }
    return drawAttribute;

} // getAttribute

//------------------------------------------------------------------------------
SAttribute CPageSetup::getAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    SAttribute drawAttribute;
    int        iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        drawAttribute = m_arAttributes[iAttribute];
    }
    return drawAttribute;

} // getAttribute

//------------------------------------------------------------------------------
QString CPageSetup::attributeValue2Str( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QString strVal;

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        switch( i_iAttribute )
        {
            case EPageSetupAttributeDrawingWidthInPixels:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.toString();
                break;
            }
            case EPageSetupAttributeDrawingHeightInPixels:
            {
                strVal = m_arAttributes[i_iAttribute].m_val.toString();
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
QString CPageSetup::attributeValue2Str( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QString strVal;
    int     iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        strVal = attributeValue2Str(iAttribute);
    }
    return strVal;

} // attributeValue2Str

//------------------------------------------------------------------------------
int CPageSetup::findAttribute( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    int iAttribute = -1;
    int idx;

    for( idx = 0; idx < EPageSetupAttributeCount; idx++ )
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
int CPageSetup::getAttributeCount() const
//------------------------------------------------------------------------------
{
    return EPageSetupAttributeCount;
}

//------------------------------------------------------------------------------
QString CPageSetup::getAttributeName( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QString strName = "Undefined";

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        strName = m_arAttributes[i_iAttribute].m_strName;
    }
    return strName;

} // getAttributeName

//------------------------------------------------------------------------------
QVariant CPageSetup::getAttributeValue( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    QVariant val;

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        val = m_arAttributes[i_iAttribute].m_val;
    }
    return val;

} // getAttributeValue

//------------------------------------------------------------------------------
QVariant CPageSetup::getAttributeValue( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    QVariant val;
    int      iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        val = m_arAttributes[iAttribute].m_val;
    }
    return val;

} // getAttributeValue

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPageSetup::setAttributesAreUsed( bool i_bIsUsed, int i_iAttributeMin, int i_iAttributesCount )
//------------------------------------------------------------------------------
{
    int iAttributeMin = i_iAttributeMin;
    int iAttributeMax = iAttributeMin + i_iAttributesCount -1;
    int iAttribute;

    if( iAttributeMax >= EPageSetupAttributeCount )
    {
        iAttributeMax = EPageSetupAttributeCount-1;
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
void CPageSetup::setAttributeIsUsed( int i_iAttribute, bool i_bIsUsed )
//------------------------------------------------------------------------------
{
    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[i_iAttribute].m_bIsUsed = i_bIsUsed;
    }

} // setAttributeIsUsed

//------------------------------------------------------------------------------
void CPageSetup::setAttributeIsUsed( const QString& i_strName, bool i_bIsUsed )
//------------------------------------------------------------------------------
{
    int iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        m_arAttributes[iAttribute].m_bIsUsed = i_bIsUsed;
    }

} // setAttributeIsUsed

//------------------------------------------------------------------------------
bool CPageSetup::isAttributeUsed( int i_iAttribute ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;

    if( i_iAttribute >= 0 && i_iAttribute < EPageSetupAttributeCount )
    {
        bIsUsed = m_arAttributes[i_iAttribute].m_bIsUsed;
    }
    return bIsUsed;

} // isAttributeUsed

//------------------------------------------------------------------------------
bool CPageSetup::isAttributeUsed( const QString& i_strName ) const
//------------------------------------------------------------------------------
{
    bool bIsUsed = false;
    int  iAttribute = findAttribute(i_strName);

    if( iAttribute >= 0 && iAttribute < EPageSetupAttributeCount )
    {
        bIsUsed = isAttributeUsed(iAttribute);
    }
    return bIsUsed;

} // isAttributeUsed

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
    m_arAttributes[EPageSetupAttributeDrawingWidthInPixels] = i_iWidth_px;
}

//------------------------------------------------------------------------------
int CPageSetup::getDrawingWidthInPixels() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EPageSetupAttributeDrawingWidthInPixels].m_val.toInt();
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
    m_arAttributes[EPageSetupAttributeDrawingHeightInPixels] = i_iHeight_px;
}

//------------------------------------------------------------------------------
int CPageSetup::getDrawingHeightInPixels() const
//------------------------------------------------------------------------------
{
    return m_arAttributes[EPageSetupAttributeDrawingHeightInPixels].m_val.toInt();
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
