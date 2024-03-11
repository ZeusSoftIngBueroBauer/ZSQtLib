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
#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "ZSDraw/Common/ZSDrawPhysValSize.h"

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
/*! The class combines the properties to define the size of a drawing scene.

    This includes the unit (either metric or pixels) as well as the scale factor.
    Unit conversion routines between pixels and metric units (and vice versa)
    must take the screen resolution (defined in pixels per inch or pixels per mm)
    and the scale factor into account. This means that those unit conversion routines
    must be able to access the drawing scene and its drawing size to use the currently
    used scale factor.
*/
class ZSDRAWDLL_API CDrawingSize
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CDrawingSize"; }
public: // ctors and dtor
    CDrawingSize(const QString& i_strName);
    CDrawingSize(const CDrawingSize& i_other);
    ~CDrawingSize();
public: // operators
    CDrawingSize& operator = (const CDrawingSize& i_other);
public: // operators
    bool operator == (const CDrawingSize& i_other) const;
    bool operator != (const CDrawingSize& i_other) const;
public: // instance methods
    bool isValid() const;
public: // instance methods
    void setDimensionUnit( const CEnumScaleDimensionUnit& i_eDimensionUnit );
    CEnumScaleDimensionUnit dimensionUnit() const;
    ZS::PhysVal::CUnit unit() const;
public: // instance methods
    void setScreenResolutionInPxPerMM(double i_fRes_px_mm);
    double screenResolutionInPxPerMM() const;
    ZS::PhysVal::CPhysVal screenPixelWidth() const;
    ZS::PhysVal::CPhysVal screenPixelWidth(const ZS::PhysVal::CUnit& i_unit) const;
    void setMetricImageCoorsDecimals(int i_iDecimals);
    int metricImageCoorsDecimals() const;
    double imageCoorsResolutionInPx() const;
    ZS::PhysVal::CPhysValRes imageCoorsResolution() const;
    ZS::PhysVal::CPhysValRes imageCoorsResolution(const ZS::PhysVal::CUnit& i_unit) const;
public: // instance methods
    void setMetricUnit(const ZS::PhysVal::CUnit& i_unit);
    ZS::PhysVal::CUnit metricUnit() const;
    void setNormedPaperSize(const CEnumNormedPaperSize& i_ePaperSize);
    CEnumNormedPaperSize normedPaperSize() const;
    void setNormedPaperOrientation(const ZS::System::CEnumOrientation& i_eOrientation);
    ZS::System::CEnumOrientation normedPaperOrientation() const;
    void setScaleFactor(int i_iDividend, int i_iDivisor);
    int scaleFactorDividend() const;
    int scaleFactorDivisor() const;
    void setYScaleAxisOrientation(const ZS::System::CEnumYScaleAxisOrientation& i_eOrientation);
    ZS::System::CEnumYScaleAxisOrientation yScaleAxisOrientation() const;
    void setImageSize(const CPhysValSize& i_physValSize);
    void setImageSize(const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight);
    QSize imageSizeInPixels() const;
    int imageWidthInPixels() const;
    int imageHeightInPixels() const;
    CPhysValSize metricImageSize() const;
    CPhysValSize metricImageSize(const ZS::PhysVal::CUnit& i_unit) const;
    ZS::PhysVal::CPhysVal metricImageWidth() const;
    ZS::PhysVal::CPhysVal metricImageWidth(const ZS::PhysVal::CUnit& i_unit) const;
    ZS::PhysVal::CPhysVal metricImageHeight() const;
    ZS::PhysVal::CPhysVal metricImageHeight(const ZS::PhysVal::CUnit& i_unit) const;
public: // instance methods
    void save( QXmlStreamWriter& i_xmlStreamWriter );
    void load( QXmlStreamReader& i_xmlStreamReader );
protected: // instance methods
    void updateImageSizeInPixels();
    void updateImageSizeMetrics();
    void updatePaperFormat();
public: // instance methods
    QString toString() const;
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    /*!< Name of the instance. Mainly introduced for debugging purposes. */
    QString m_strName;
    /*!< Either Pixels or Metric System (e.g. mm, cm, m, etc.). */
    CEnumScaleDimensionUnit m_eDimensionUnit;
    /*!< If m_eDimensionUnit is Metric the metric unit (mm, cm, m, etc.) is set here. */
    ZS::PhysVal::CUnit m_metricUnit;
    /*!< Resolution of a pixel on the screen in pixels/mm.
         The screen resolution in "pixels/mm" is automatically detected by a system call via
         "QPaintDevice::logicalDpi". This automatically detected value is just an approximate
         value and may be overwritten. */
    double m_fScreenResolution_px_mm;
    /*!< Number of valid decimals for editing and indicating coordinates in metric system.
         The number of decimals defines the resolution of the coordinates. */
    int m_iImageMetricCoorsDecimals;
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
    /*!< Y scale axis orientation. Defaults to "TopDown". */
    ZS::System::CEnumYScaleAxisOrientation m_eYScaleAxisOrientation;
    /*!< Resolution of the image in pixels. As default the resolution is 1 pixel. */
    double m_fImageSizeRes_px;
    /*!< Width of the image in pixels. Either set for Dimension unit Pixels or calculated
         by the metric width, the scale factor and the screen resolution in pixels per mm. */
    double m_fImageSizeWidth_px;
    /*!< Height of the image in pixels. Either set for Dimension unit Pixels or calculated
         by the metric height, the scale factor and the screen resolution in pixels per mm. */
    double m_fImageSizeHeight_px;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDrawingSize

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingSize_h
