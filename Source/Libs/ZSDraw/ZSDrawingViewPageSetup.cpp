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

#include "ZSDraw/ZSDrawingViewPageSetup.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingViewPageSetup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingViewPageSetup::CDrawingViewPageSetup() :
//------------------------------------------------------------------------------
    m_sizeDrawing(),
    m_marginsViewport()
{
} // ctor

//------------------------------------------------------------------------------
CDrawingViewPageSetup::CDrawingViewPageSetup(
    const QSize&    i_size,
    const QMargins& i_marginsViewport) :
//------------------------------------------------------------------------------
    m_sizeDrawing(i_size),
    m_marginsViewport(i_marginsViewport)
{
} // ctor

//------------------------------------------------------------------------------
CDrawingViewPageSetup::~CDrawingViewPageSetup()
//------------------------------------------------------------------------------
{
    //m_sizeDrawing;
    //m_marginsViewport;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingViewPageSetup::operator == ( const CDrawingViewPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_sizeDrawing != i_other.m_sizeDrawing )
    {
        bEqual = false;
    }
    else if( m_marginsViewport != i_other.m_marginsViewport )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CDrawingViewPageSetup::operator != ( const CDrawingViewPageSetup& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::save( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    i_settings.setValue( i_strSettingsKey + "/Drawing/Size/Width", m_sizeDrawing.width() );
    i_settings.setValue( i_strSettingsKey + "/Drawing/Size/Height", m_sizeDrawing.height() );

    i_settings.setValue( i_strSettingsKey + "/Viewport/Margins/Left", m_marginsViewport.left() );
    i_settings.setValue( i_strSettingsKey + "/Viewport/Margins/Right", m_marginsViewport.right() );
    i_settings.setValue( i_strSettingsKey + "/Viewport/Margins/Top", m_marginsViewport.top() );
    i_settings.setValue( i_strSettingsKey + "/Viewport/Margins/Bottom", m_marginsViewport.bottom() );

} // save

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::load( QSettings& i_settings, const QString& i_strSettingsKey )
//------------------------------------------------------------------------------
{
    m_sizeDrawing.setWidth( i_settings.value( i_strSettingsKey + "/Drawing/Size/Width", m_sizeDrawing.width() ).toInt() );
    m_sizeDrawing.setHeight( i_settings.value( i_strSettingsKey + "/Drawing/Size/Height", m_sizeDrawing.height() ).toInt() );

    m_marginsViewport.setLeft( i_settings.value( i_strSettingsKey + "/Viewport/Margins/Left", m_marginsViewport.left() ).toInt() );
    m_marginsViewport.setRight( i_settings.value( i_strSettingsKey + "/Viewport/Margins/Right", m_marginsViewport.right() ).toInt() );
    m_marginsViewport.setTop( i_settings.value( i_strSettingsKey + "/Viewport/Margins/Top", m_marginsViewport.top() ).toInt() );
    m_marginsViewport.setBottom( i_settings.value( i_strSettingsKey + "/Viewport/Margins/Bottom", m_marginsViewport.bottom() ).toInt() );

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    i_xmlStreamWriter.writeStartElement("Drawing");
    i_xmlStreamWriter.writeStartElement("Size");

    i_xmlStreamWriter.writeAttribute("Width", QString::number(m_sizeDrawing.width()));
    i_xmlStreamWriter.writeAttribute("Height", QString::number(m_sizeDrawing.height()));

    i_xmlStreamWriter.writeEndElement(); // "Size"
    i_xmlStreamWriter.writeEndElement(); // "Drawing"

    i_xmlStreamWriter.writeStartElement("Viewport");
    i_xmlStreamWriter.writeStartElement("Margins");

    i_xmlStreamWriter.writeAttribute("Left", QString::number(m_marginsViewport.left()));
    i_xmlStreamWriter.writeAttribute("Right", QString::number(m_marginsViewport.right()));
    i_xmlStreamWriter.writeAttribute("Top", QString::number(m_marginsViewport.top()));
    i_xmlStreamWriter.writeAttribute("Bottom", QString::number(m_marginsViewport.bottom()));

    i_xmlStreamWriter.writeEndElement(); // "Margins"
    i_xmlStreamWriter.writeEndElement(); // "Viewport"

} // save

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QXmlStreamReader::TokenType xmlStreamTokenType;

    QString  strElemName;
    QString  strElemPath;
    bool     bConverted;
    QString  strVal;
    int      iVal;

    QMargins marginsViewport;
    QSize    sizeDrawing;

    while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )
    {
        xmlStreamTokenType = i_xmlStreamReader.readNext();

        if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )
        {
            strElemName = i_xmlStreamReader.name().toString();

            if( i_xmlStreamReader.isStartElement() )
            {
                strElemPath += strElemName;

                if( strElemName == "Drawing" )
                {
                }
                else if( strElemName == "Size" )
                {
                    if( strElemPath.startsWith("Drawing") )
                    {
                        strVal = i_xmlStreamReader.attributes().value("Width").toString();
                        iVal = strVal.toInt(&bConverted);
                        if( bConverted ) sizeDrawing.setWidth(iVal);
                        else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Width\" (" + strVal + ") is out of range");

                        if( !i_xmlStreamReader.hasError() )
                        {
                            strVal = i_xmlStreamReader.attributes().value("Height").toString();
                            iVal = strVal.toInt(&bConverted);
                            if( bConverted ) sizeDrawing.setHeight(iVal);
                            else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Height\" (" + strVal + ") is out of range");
                        }
                    }
                }
                else if( strElemName == "Viewport" )
                {
                }
                else if( strElemName == "Margins" )
                {
                    if( strElemPath.startsWith("Viewport") )
                    {
                        strVal = i_xmlStreamReader.attributes().value("Left").toString();
                        iVal = strVal.toInt(&bConverted);
                        if( bConverted ) marginsViewport.setLeft(iVal);
                        else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Left\" (" + strVal + ") is out of range");

                        if( !i_xmlStreamReader.hasError() )
                        {
                            strVal = i_xmlStreamReader.attributes().value("Right").toString();
                            iVal = strVal.toInt(&bConverted);
                            if( bConverted ) marginsViewport.setLeft(iVal);
                            else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Right\" (" + strVal + ") is out of range");
                        }

                        if( !i_xmlStreamReader.hasError() )
                        {
                            strVal = i_xmlStreamReader.attributes().value("Top").toString();
                            iVal = strVal.toInt(&bConverted);
                            if( bConverted ) marginsViewport.setLeft(iVal);
                            else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Top\" (" + strVal + ") is out of range");
                        }

                        if( !i_xmlStreamReader.hasError() )
                        {
                            strVal = i_xmlStreamReader.attributes().value("Bottom").toString();
                            iVal = strVal.toInt(&bConverted);
                            if( bConverted ) marginsViewport.setLeft(iVal);
                            else i_xmlStreamReader.raiseError("Attribute \"" + strElemPath + "Bottom\" (" + strVal + ") is out of range");
                        }
                    }
                }
            } // if( i_xmlStreamReader.isStartElement() )

            else if( i_xmlStreamReader.isEndElement() )
            {
                if( strElemPath.endsWith(strElemName) ) strElemPath.remove(strElemPath.length() - strElemName.length(), strElemName.length());
                else i_xmlStreamReader.raiseError("EndElement for " + strElemName + " not expected");

                if( strElemName == "Drawing" )
                {
                }
                else if( strElemName == "Size" )
                {
                    m_sizeDrawing = sizeDrawing;
                }
                else if( strElemName == "Viewport" )
                {
                }
                else if( strElemName == "Margins" )
                {
                    m_marginsViewport = marginsViewport;
                }
                else if( strElemName == "PageSetup" )
                {
                    break;
                }
            }
        } // if( i_xmlStreamReader.isStartElement() || i_xmlStreamReader.isEndElement() )
    } // while( !i_xmlStreamReader.hasError() && !i_xmlStreamReader.atEnd() )

} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingViewPageSetup::setZoomFactorInPerCent( double i_fZoomFactor_percent )
////------------------------------------------------------------------------------
//{
//    m_fZoomFactor_percent = i_fZoomFactor_percent;
//}

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setDrawingSize( const QSize& i_size )
//------------------------------------------------------------------------------
{
    m_sizeDrawing = i_size;
}

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setDrawingSizeInPixels( int i_iWidth_px, int i_iHeight_px )
//------------------------------------------------------------------------------
{
    m_sizeDrawing.setWidth(i_iWidth_px);
    m_sizeDrawing.setHeight(i_iHeight_px);
}

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setDrawingWidthInPixels( int i_iWidth_px )
//------------------------------------------------------------------------------
{
    m_sizeDrawing.setWidth(i_iWidth_px);
}

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setDrawingHeightInPixels( int i_iHeight_px )
//------------------------------------------------------------------------------
{
    m_sizeDrawing.setHeight(i_iHeight_px);
}

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

////------------------------------------------------------------------------------
//void CDrawingViewPageSetup::setDrawingWidth( const CPhysVal& i_physValWidth )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_physValWidth = i_physValWidth;
//}

////------------------------------------------------------------------------------
//void CDrawingViewPageSetup::setDrawingHeight( const CPhysVal& i_physValHeight )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_physValHeight = i_physValHeight;
//}

////------------------------------------------------------------------------------
//void CDrawingViewPageSetup::setDrawingXScaleFactor( double i_fScaleFactor )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_fXScaleFac = i_fScaleFactor;
//}

////------------------------------------------------------------------------------
//void CDrawingViewPageSetup::setDrawingYScaleFactor( double i_fScaleFactor )
////------------------------------------------------------------------------------
//{
//    m_drawArea.m_fYScaleFac = i_fScaleFactor;
//}

/*==============================================================================
public: // instance methods (drawing area)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setViewportMargins(int i_iLeft, int i_iTop, int i_iRight, int i_iBottom)
//------------------------------------------------------------------------------
{
    m_marginsViewport.setLeft(i_iLeft);
    m_marginsViewport.setTop(i_iTop);
    m_marginsViewport.setRight(i_iRight);
    m_marginsViewport.setBottom(i_iBottom);
}

//------------------------------------------------------------------------------
void CDrawingViewPageSetup::setViewportMargins(const QMargins& i_margins)
//------------------------------------------------------------------------------
{
    m_marginsViewport = i_margins;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingViewPageSetup::toString() const
//------------------------------------------------------------------------------
{
    QString str;
    str  = "Size {" + size2Str(m_sizeDrawing) + "}";
    str += ", ViewportMargins {" + qMargins2Str(m_marginsViewport) + "}";
    return str;
}
