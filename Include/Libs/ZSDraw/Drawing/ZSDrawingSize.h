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

#ifndef ZSDraw_DrawingSize_h
#define ZSDraw_DrawingSize_h

#include "ZSDraw/Common/ZSDrawCommon.h"

class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
class CMethodTracer;
}
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CDrawingSize
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CDrawingSize"; }
public: // ctors and dtor
    CDrawingSize(const QString& i_strName);
    CDrawingSize(const QString& i_strName, const QSize& i_size);
    CDrawingSize(const CDrawingSize& i_other);
    ~CDrawingSize();
public: // operators
    CDrawingSize& operator = (const CDrawingSize& i_other);
public: // operators
    bool operator == (const CDrawingSize& i_other) const;
    bool operator != (const CDrawingSize& i_other) const;
public: // instance methods
    void setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit );
    CEnumDrawingDimensionUnit dimensionUnit() const;
    void setMetricUnit( const ZS::PhysVal::CUnit& i_unit );
    ZS::PhysVal::CUnit metricUnit() const;
    void setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize );
    CEnumNormedPaperSize normedPaperSize() const;
    void setNormedPaperOrientation( const ZS::System::CEnumOrientation& i_eOrientation );
    ZS::System::CEnumOrientation normedPaperOrientation() const;
    void setScaleFactor( int i_iDividend, int i_iDivisor );
    int scaleFactorDividend() const;
    int scaleFactorDivisor() const;
    void setImageSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight );
    QSize imageSizeInPixels() const;
    int imageWidthInPixels() const;
    int imageHeightInPixels() const;
    ZS::PhysVal::CPhysVal metricImageWidth() const;
    ZS::PhysVal::CPhysVal metricImageHeight() const;
protected: // instance methods
    void updateImageSizeInPixels();
    void updateImageSizeMetrics();
    void updatePaperFormat();
public: // instance methods
    QString toString() const;
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Name of the instance. */
    QString m_strName;
    /*!< Either Pixels or Metric System (e.g. mm, cm, m, etc.). */
    CEnumDrawingDimensionUnit m_eDimensionUnit;
    /*!< If m_eDimensionUnit is Metric the unit (mm, cm, m, etc.) is defined here. */
    ZS::PhysVal::CUnit m_metricUnit;
    /*!< Width of the image in metric unit. */
    double m_fImageMetricWidth;
    /*!< Height of the image in metric unit. */
    double m_fImageMetricHeight;
    /*!< One of the normed paper sizes (DINA4 etc.) or invalid. */
    CEnumNormedPaperSize m_eNormedPaperSize;
    /*!< If a normed paper size is used the paper may be orientated vertical or horizontal.
         If no normed paper size is used the orientation is an invalid value. */
    ZS::System::CEnumOrientation m_eNormedPaperOrientation;
    /*!< For metric system the scale factor is defined by Dividend/Divisor. */
    int m_iMetricScaleFactorDividend;
    /*!< For metric system the scale factor is defined by Dividend/Divisor. */
    int m_iMetricScaleFactorDivisor;
    /*!< Width of the image in pixels. Either set for Dimension unit Pixels or calculated
         by the metric width, the scale factor and the screen resolution in dots per inch. */
    double m_fImageSizeWidth_px;
    /*!< Height of the image in pixels. Either set for Dimension unit Pixels or calculated
         by the metric height, the scale factor and the screen resolution in dots per inch. */
    double m_fImageSizeHeight_px;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDrawingSize

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingSize_h
