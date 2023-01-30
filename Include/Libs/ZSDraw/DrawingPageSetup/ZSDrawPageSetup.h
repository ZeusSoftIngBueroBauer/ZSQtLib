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

#if 0

#ifndef ZSDraw_PageSetup_h
#define ZSDraw_PageSetup_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/Drawing/ZSDrawUnits.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CPageSetup
//******************************************************************************
{
public: // class methods
    //static double GetFormatWidthInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
    //static double GetFormatHeightInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
public: // ctors and dtor
    CPageSetup();
    ~CPageSetup();
public: // operators
    bool operator == (const CPageSetup& i_other) const;
    bool operator != (const CPageSetup& i_other) const;
public: // instance methods
    void save(QSettings& i_settings, const QString& i_strSettingsKey);
    void load(QSettings& i_settings, const QString& i_strSettingsKey);
public: // instance methods
    void save(QXmlStreamWriter& i_xmlStreamWriter);
    void load(QXmlStreamReader& i_xmlStreamReader);
public: // instance methods
    //void setPaperFormat( EPaperFormat i_format );
    //EPaperFormat getPaperFormat() const { return m_paperFormat; }
    //void setPaperOrientation( EPaperOrientation i_orientation );
    //EPaperOrientation getPaperOrientation() const { return m_paperOrientation; }
    //void setPrinterPaperSource( EPrinterPaperSource i_source );
    //EPrinterPaperSource getPrinterPaperSource() const { return m_printerPaperSource; }
public: // instance methods
    //void setZoomFactor( double i_fZoomFactor ); // 0.0 .. 1.0
    //void setZoomFactorInPerCent( double i_fZoomFactor_percent );
    //double getZoomFactor() const;
    //double getZoomFactorInPerCent() const { return m_fZoomFactor_percent; }
    //void setFormatWidthInMilliMeter( double i_fWidth_mm );
    //void setFormatWidthInPx( double i_fWidth_px );
    //double getFormatWidthInMilliMeter() const { return m_fFormatWidth_mm; }
    //double getFormatWidthInPx() const;
    //void setFormatHeightInMilliMeter( double i_fHeight_mm );
    //void setFormatHeightInPx( double i_fHeight_px );
    //double getFormatHeightInMilliMeter() const { return m_fFormatHeight_mm; };
    //double getFormatHeightInPx() const;
public: // instance methods (drawing area)
    //void setMarginTopInMilliMeter( double i_fTop_mm );
    //void setMarginTopInPx( double i_fTop_px );
    //double getMarginTopInMilliMeter() const { return m_fMarginTop_mm; }
    //double getMarginTopInPx() const;
    //void setMarginBottomInMilliMeter( double i_fBottom_mm );
    //void setMarginBottomInPx( double i_fBottom_px );
    //double getMarginBottomInMilliMeter() const { return m_fMarginBottom_mm; }
    //double getMarginBottomInPx() const;
    //void setMarginLeftInMilliMeter( double i_fLeft_mm );
    //void setMarginLeftInPx( double i_fLeft_px );
    //double getMarginLeftInMilliMeter() const { return m_fMarginLeft_mm; }
    //double getMarginLeftInPx() const;
    //void setMarginRightInMilliMeter( double i_fRight_mm );
    //void setMarginRightInPx( double i_fRight_px );
    //double getMarginRightInMilliMeter() const { return m_fMarginRight_mm; }
    //double getMarginRightInPx() const;
public: // instance methods (drawing area)
    //double getDrawingWidthInMilliMeter() const;  // area between left and right margin
    //double getDrawingHeightInMilliMeter() const; // area between top and bottom margin
public: // instance methods (drawing area)
    //void setDrawArea( const SDrawArea& i_drawArea );
    //SDrawArea getDrawArea() const;
public: // instance methods
    ZS::PhysVal::CUnit unit( ZS::System::EDirection i_direction ) const;
public: // instance methods
    void setDrawingSize( const ZS::PhysVal::CPhysVal& i_width, const ZS::PhysVal::CPhysVal& i_height );
    void setDrawingWidth( const ZS::PhysVal::CPhysVal& i_width );
    ZS::PhysVal::CPhysVal drawingWidth() const;
    void setDrawingHeight( const ZS::PhysVal::CPhysVal& i_height );
    ZS::PhysVal::CPhysVal drawingHeight() const;
public: // instance methods
    void setDrawingScale( double i_fScale );
    double drawingScale() const;
    void setDrawingScales( double i_fScaleWidth, double i_fScaleHeight );
    void setDrawingScale( ZS::System::EDirection i_direction, double i_fScaleFactor );
    double drawingScale( ZS::System::EDirection i_direction ) const;
protected: // instance members
    QVector<ZS::PhysVal::CPhysVal> m_arPhysValsSize;
    QVector<double> m_arfLengthScaleFactors;

}; // class CPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PageSetup_h

#endif
