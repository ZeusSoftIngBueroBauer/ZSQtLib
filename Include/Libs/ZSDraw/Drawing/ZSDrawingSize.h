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
    ~CDrawingSize();
public: // operators
    CDrawingSize& operator = (const QSize& i_size_px);
public: // operators
    bool operator == (const CDrawingSize& i_other) const;
    bool operator != (const CDrawingSize& i_other) const;
public: // instance methods
    void save(QXmlStreamWriter& i_xmlStreamWriter);
    void load(QXmlStreamReader& i_xmlStreamReader);
public: // instance methods
    void setDimensionUnit( const CEnumDrawingDimensionUnit& i_eDimensionUnit );
    CEnumDrawingDimensionUnit dimensionUnit() const;
    void setMetricUnit( const ZS::PhysVal::CUnit& i_unit );
    ZS::PhysVal::CUnit metricUnit() const;
    void setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize );
    CEnumNormedPaperSize normedPaperSize() const;
    void setNormedPaperOrientation( const ZS::System::CEnumDirection& i_eDirection );
    ZS::System::CEnumDirection normedPaperOrientation() const;
    void setScaleFactor( int i_iDividend, int i_iDivisor );
    int scaleFactorDividend() const;
    int scaleFactorDivisor() const;
    void setImageSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight );
    QSize imageSizeInPixels() const;
    ZS::PhysVal::CPhysVal metricImageWidth() const;
    ZS::PhysVal::CPhysVal metricImageHeight() const;
protected: // instance methods
    void updateImageSizeInPixels();
    void updateImageSizeMetrics();
    void updatePaperFormat();
protected: // instance methods (method tracing)
    void traceValues(ZS::System::CMethodTracer& i_mthTracer, ZS::System::EMethodDir i_methodDir);
private: // instance members
    QString m_strName;
    CEnumDrawingDimensionUnit m_eDimensionUnit;
    ZS::PhysVal::CUnit m_metricUnit;
    double m_fImageMetricWidth;
    double m_fImageMetricHeight;
    CEnumNormedPaperSize m_eNormedPaperSize;
    ZS::System::CEnumDirection m_eNormedPaperOrientation;
    int m_iMetricScaleFactorDividend;
    int m_iMetricScaleFactorDivisor;
    double m_fImageSizeWidth_px;
    double m_fImageSizeHeight_px;
    /*!< Trace admin object for method tracing. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // class CDrawingSize

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingSize_h
