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

#include "ZSDraw/Drawing/ZSDrawingSize.h"
#include "ZSDraw/Drawing/ZSDrawUnits.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

//#if QT_VERSION < 0x050000
//#include <QtXml/qxmlstream.h>
//#else
//#include <QtCore/qxmlstream.h>
//#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates an instance of the class.
*/
CDrawingSize::CDrawingSize(const QString& i_strName) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_eDimensionUnit(EDrawingDimensionUnit::Pixels),
    m_metricUnit(Units.Length.mm),
    m_fImageMetricWidth(0.0),
    m_fImageMetricHeight(0.0),
    m_eNormedPaperSize(ENormedPaperSize::Undefined),
    m_eNormedPaperOrientation(EDirection::Undefined),
    m_iMetricScaleFactorDividend(1),
    m_iMetricScaleFactorDivisor(1),
    m_fImageSizeWidth_px(0.0),
    m_fImageSizeHeight_px(0.0),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), m_strName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
/*! Creates an instance of the class.
*/
CDrawingSize::CDrawingSize(const QString& i_strName, const QSize& i_size) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_eDimensionUnit(EDrawingDimensionUnit::Pixels),
    m_metricUnit(Units.Length.mm),
    m_fImageMetricWidth(0.0),
    m_fImageMetricHeight(0.0),
    m_eNormedPaperSize(ENormedPaperSize::Undefined),
    m_eNormedPaperOrientation(EDirection::Undefined),
    m_iMetricScaleFactorDividend(1),
    m_iMetricScaleFactorDivisor(1),
    m_fImageSizeWidth_px(i_size.width()),
    m_fImageSizeHeight_px(i_size.height()),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), m_strName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    updateImageSizeMetrics();
    updatePaperFormat();

} // ctor

//------------------------------------------------------------------------------
/*! Creates an instance of the class.
*/
CDrawingSize::CDrawingSize(const CDrawingSize& i_other) :
//------------------------------------------------------------------------------
    m_strName(i_other.m_strName),
    m_eDimensionUnit(i_other.m_eDimensionUnit),
    m_metricUnit(i_other.m_metricUnit),
    m_fImageMetricWidth(i_other.m_fImageMetricWidth),
    m_fImageMetricHeight(i_other.m_fImageMetricHeight),
    m_eNormedPaperSize(i_other.m_eNormedPaperSize),
    m_eNormedPaperOrientation(i_other.m_eNormedPaperOrientation),
    m_iMetricScaleFactorDividend(i_other.m_iMetricScaleFactorDividend),
    m_iMetricScaleFactorDivisor(i_other.m_iMetricScaleFactorDivisor),
    m_fImageSizeWidth_px(i_other.m_fImageSizeWidth_px),
    m_fImageSizeHeight_px(i_other.m_fImageSizeHeight_px),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(NameSpace(), ClassName(), m_strName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // copy ctor

//------------------------------------------------------------------------------
CDrawingSize::~CDrawingSize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    //m_eDimensionUnit = static_cast<EDrawingDimensionUnit>(0);
    //m_metricUnit;
    m_fImageMetricWidth = 0.0;
    m_fImageMetricHeight = 0.0;
    //m_eNormedPaperSize = static_cast<ENormedPaperSize>(0);
    //m_eNormedPaperOrientation = static_cast<EDirection>(0);
    m_iMetricScaleFactorDividend = 0;
    m_iMetricScaleFactorDivisor = 0;
    m_fImageSizeWidth_px = 0.0;
    m_fImageSizeHeight_px = 0.0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingSize& CDrawingSize::operator = (const CDrawingSize& i_other)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_other.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "=Other",
        /* strAddInfo   */ strMthInArgs );

    m_strName = i_other.m_strName;
    m_eDimensionUnit = i_other.m_eDimensionUnit;
    m_metricUnit = i_other.m_metricUnit;
    m_fImageMetricWidth = i_other.m_fImageMetricWidth;
    m_fImageMetricHeight = i_other.m_fImageMetricHeight;
    m_eNormedPaperSize = i_other.m_eNormedPaperSize;
    m_eNormedPaperOrientation = i_other.m_eNormedPaperOrientation;
    m_iMetricScaleFactorDividend = i_other.m_iMetricScaleFactorDividend;
    m_iMetricScaleFactorDivisor = i_other.m_iMetricScaleFactorDivisor;
    m_fImageSizeWidth_px = i_other.m_fImageSizeWidth_px;
    m_fImageSizeHeight_px = i_other.m_fImageSizeHeight_px;

    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingSize::operator == ( const CDrawingSize& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if( m_eDimensionUnit != i_other.m_eDimensionUnit ) {
        bEqual = false;
    }
    else if( m_metricUnit != i_other.m_metricUnit ) {
        bEqual = false;
    }
    else if( m_fImageMetricWidth != i_other.m_fImageMetricWidth ) {
        bEqual = false;
    }
    else if( m_fImageMetricHeight != i_other.m_fImageMetricHeight ) {
        bEqual = false;
    }
    else if( m_eNormedPaperSize != i_other.m_eNormedPaperSize ) {
        bEqual = false;
    }
    else if( m_eNormedPaperOrientation != i_other.m_eNormedPaperOrientation ) {
        bEqual = false;
    }
    else if( m_iMetricScaleFactorDividend != i_other.m_iMetricScaleFactorDividend ) {
        bEqual = false;
    }
    else if( m_iMetricScaleFactorDivisor != i_other.m_iMetricScaleFactorDivisor ) {
        bEqual = false;
    }
    else if( m_fImageSizeWidth_px != i_other.m_fImageSizeWidth_px ) {
        bEqual = false;
    }
    else if( m_fImageSizeHeight_px != i_other.m_fImageSizeHeight_px ) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CDrawingSize::operator != ( const CDrawingSize& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
#if 0
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
#endif
} // save

//------------------------------------------------------------------------------
void CDrawingSize::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
#if 0
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
#endif
} // load

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_eDimensionUnit.toString() + " (Prev: " + m_eDimensionUnit.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDimensionUnit",
        /* strAddInfo   */ strMthInArgs );

    m_eDimensionUnit = i_eDimensionUnit;
}

//------------------------------------------------------------------------------
CEnumDrawingDimensionUnit CDrawingSize::dimensionUnit() const
//------------------------------------------------------------------------------
{
    return m_eDimensionUnit;
}

//------------------------------------------------------------------------------
void CDrawingSize::setMetricUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_unit.symbol() + " (Prev: " + m_metricUnit.symbol() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_metricUnit != i_unit ) {
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.
        CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);   // Value in previous unit
        CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit); // Value in previous unit
        m_metricUnit = i_unit;
        physValWidth.convertValue(m_metricUnit);
        physValHeight.convertValue(m_metricUnit);
        m_fImageMetricWidth = physValWidth.getVal();
        m_fImageMetricHeight = physValHeight.getVal();
    }
}

//------------------------------------------------------------------------------
CUnit CDrawingSize::metricUnit() const
//------------------------------------------------------------------------------
{
    return m_metricUnit;
}

//------------------------------------------------------------------------------
void CDrawingSize::setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_ePaperSize.toString() + " (Prev: " + m_eNormedPaperSize.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_eNormedPaperSize != i_ePaperSize ) {
        m_eNormedPaperSize = i_ePaperSize;
        if( m_eNormedPaperSize == ENormedPaperSize::Undefined ) {
        }
        else {
            if( m_eNormedPaperOrientation == EDirection::Undefined ) {
                m_eNormedPaperOrientation = EDirection::Horizontal;
            }
            QVariant varPaperSize = m_eNormedPaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( m_eNormedPaperOrientation == EDirection::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_metricUnit);
                physValHeight.convertValue(m_metricUnit);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
CEnumNormedPaperSize CDrawingSize::normedPaperSize() const
//------------------------------------------------------------------------------
{
    return m_eNormedPaperSize;
}

//------------------------------------------------------------------------------
void CDrawingSize::setNormedPaperOrientation( const CEnumDirection& i_direction )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_direction.toString() + " (Prev: " + m_eNormedPaperOrientation.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_eNormedPaperOrientation != i_direction ) {
        m_eNormedPaperOrientation = i_direction;
        // Setting values according to orientation makes only sense for normed paper sizes.
        // For user defined paper sizes there is no orientation. The width might be
        // greater than the height or vice versa. The orientation will be stored if later
        // on a normed paper size is set but for now the values will not be adjusted.
        if( m_eNormedPaperSize != ENormedPaperSize::Undefined ) {
            QVariant varPaperSize = m_eNormedPaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( m_eNormedPaperOrientation == EDirection::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_metricUnit);
                physValHeight.convertValue(m_metricUnit);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
CEnumDirection CDrawingSize::normedPaperOrientation() const
//------------------------------------------------------------------------------
{
    return m_eNormedPaperOrientation;
}

//------------------------------------------------------------------------------
void CDrawingSize::setScaleFactor( int i_iDividend, int i_iDivisor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = QString::number(i_iDividend) + "/" + QString::number(i_iDivisor)
                     + " (Prev: " + QString::number(m_iMetricScaleFactorDividend)
                     + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleFactor",
        /* strAddInfo   */ strMthInArgs );

    if( i_iDividend <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Dividend <= 0!");
    }
    if( i_iDivisor <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Divisor <= 0!");
    }
    if( m_iMetricScaleFactorDividend != i_iDividend || m_iMetricScaleFactorDivisor != i_iDivisor ) {
        m_iMetricScaleFactorDividend = i_iDividend;
        m_iMetricScaleFactorDivisor = i_iDivisor;
        updateImageSizeInPixels();
    }
}

//------------------------------------------------------------------------------
int CDrawingSize::scaleFactorDividend() const
//------------------------------------------------------------------------------
{
    return m_iMetricScaleFactorDividend;
}

//------------------------------------------------------------------------------
int CDrawingSize::scaleFactorDivisor() const
//------------------------------------------------------------------------------
{
    return m_iMetricScaleFactorDivisor;
}

//------------------------------------------------------------------------------
void CDrawingSize::setImageSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) ) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString()
                    + " (Prev: " + CPhysVal(m_fImageSizeWidth_px, Units.Length.pxX).toString()
                    + ", " + CPhysVal(m_fImageSizeHeight_px, Units.Length.pxY).toString() + ")"
                    + ", (Prev: " + CPhysVal(m_fImageMetricWidth, m_metricUnit).toString()
                    + ", " + CPhysVal(m_fImageMetricHeight, m_metricUnit).toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setImageSize",
        /* strAddInfo   */ strMthInArgs );

    if( !i_physValWidth.isValid() || i_physValWidth.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValWidth <= 0.0!");
    }
    if( !i_physValHeight.isValid() || i_physValHeight.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValHeight <= 0.0!");
    }
    if( (i_physValWidth.unit() == Units.Length.pxX && i_physValHeight.unit() != Units.Length.pxY)
     || (i_physValWidth.unit() != Units.Length.pxX && i_physValHeight.unit() == Units.Length.pxY) ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Either set size in pixels or in metrics");
    }

    if( i_physValWidth.unit() == Units.Length.pxX ) {
        double fImageSizeWidth_px = i_physValWidth.getVal();
        double fImageSizeHeight_px = i_physValHeight.getVal();
        if( m_fImageSizeWidth_px != fImageSizeWidth_px || m_fImageSizeHeight_px != fImageSizeHeight_px ) {
            m_fImageSizeWidth_px = fImageSizeWidth_px;
            m_fImageSizeHeight_px = fImageSizeHeight_px;
            updateImageSizeMetrics();
            updatePaperFormat();
        }
    }
    else {
        double fImageMetricWidth = i_physValWidth.getVal(m_metricUnit);
        double fImageMetricHeight = i_physValHeight.getVal(m_metricUnit);
        if( m_fImageMetricWidth != fImageMetricWidth || m_fImageMetricHeight != fImageMetricHeight ) {
            m_fImageMetricWidth = fImageMetricWidth;
            m_fImageMetricHeight = fImageMetricHeight;
            updateImageSizeInPixels();
            updatePaperFormat();
        }
    }
}

//------------------------------------------------------------------------------
QSize CDrawingSize::imageSizeInPixels() const
//------------------------------------------------------------------------------
{
    return QSize(m_fImageSizeWidth_px, m_fImageSizeHeight_px);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageWidth() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fImageMetricWidth, m_metricUnit);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageHeight() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fImageMetricHeight, m_metricUnit);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::updateImageSizeInPixels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeInPixels",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                    / static_cast<double>(m_iMetricScaleFactorDivisor);
    double fScaledWidth = fFactor * m_fImageMetricWidth;
    double fScaledHeight = fFactor * m_fImageMetricHeight;
    CPhysVal physValWidth(fScaledWidth, m_metricUnit);
    CPhysVal physValHeight(fScaledHeight, m_metricUnit);
    physValWidth.convertValue(Units.Length.pxX);
    physValHeight.convertValue(Units.Length.pxY);
    m_fImageSizeWidth_px = physValWidth.getVal();
    m_fImageSizeHeight_px = physValHeight.getVal();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeInPixels

//------------------------------------------------------------------------------
void CDrawingSize::updateImageSizeMetrics()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeMetrics",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    CPhysVal physValWidth(m_fImageSizeWidth_px, Units.Length.pxX);
    CPhysVal physValHeight(m_fImageSizeHeight_px, Units.Length.pxY);
    physValWidth.convertValue(m_metricUnit);
    physValHeight.convertValue(m_metricUnit);
    m_fImageMetricWidth = physValWidth.getVal();
    m_fImageMetricHeight = physValHeight.getVal();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeMetrics

//------------------------------------------------------------------------------
void CDrawingSize::updatePaperFormat()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePaperFormat",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    bool bDirectionFound = false;
    CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);
    CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit);
    physValWidth.convertValue(Units.Length.mm);
    physValHeight.convertValue(Units.Length.mm);
    QSizeF sizePaper(physValWidth.getVal(), physValHeight.getVal());
    m_eNormedPaperSize = CEnumNormedPaperSize::fromValue(sizePaper, &bDirectionFound);
    // If found the orientation is vertical.
    if( bDirectionFound ) {
        m_eNormedPaperOrientation = EDirection::Vertical;
    }
    // If not found try also the other orientation:
    else {
        physValWidth.setVal(m_fImageMetricHeight);
        physValHeight.setVal(m_fImageMetricWidth);
        sizePaper.setWidth(physValWidth.getVal());
        sizePaper.setHeight(physValHeight.getVal());
        m_eNormedPaperSize = CEnumNormedPaperSize::fromValue(sizePaper, &bDirectionFound);
        if( bDirectionFound ) {
            m_eNormedPaperOrientation = EDirection::Horizontal;
        }
        else {
            m_eNormedPaperSize = ENormedPaperSize::Undefined;
            m_eNormedPaperOrientation = EDirection::Undefined;
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updatePaperFormat

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingSize::toString() const
//------------------------------------------------------------------------------
{
    QString str = m_strName
        +  ", " + m_eDimensionUnit.toString()
        + ", Paper (" + m_eNormedPaperSize.toString()
            + ", " + m_eNormedPaperOrientation.toString() + ")"
        + ", Scale (" + QString::number(m_iMetricScaleFactorDividend)
            + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")"
        + ", Size (" + QString::number(m_fImageMetricWidth) + " " + m_metricUnit.symbol()
            + " * " + QString::number(m_fImageMetricHeight) + " " + m_metricUnit.symbol() + ")"
        + ", Size (" + QString::number(m_fImageSizeWidth_px)
            + " * " + QString::number(m_fImageSizeHeight_px) + " px)";
    return str;
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ")
        +  m_eDimensionUnit.toString()
        + ", Paper (" + m_eNormedPaperSize.toString()
            + ", " + m_eNormedPaperOrientation.toString() + ")"
        + ", Scale (" + QString::number(m_iMetricScaleFactorDividend)
            + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")"
        + ", Size (" + QString::number(m_fImageMetricWidth) + " " + m_metricUnit.symbol()
            + " * " + QString::number(m_fImageMetricHeight) + " " + m_metricUnit.symbol() + ")"
        + ", Size (" + QString::number(m_fImageSizeWidth_px)
            + " * " + QString::number(m_fImageSizeHeight_px) + " px)";
    mthTracer.trace(strMthLog);
}
