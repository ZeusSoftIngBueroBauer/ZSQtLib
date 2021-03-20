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

#ifndef ZSDraw_PageSetup_h
#define ZSDraw_PageSetup_h

#include "ZSDraw/ZSDrawDllMain.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
/*******************************************************************************
enum EPageSetupAttribute
*******************************************************************************/

typedef enum
{
    EPageSetupAttributeDrawingWidthInPixels  = 0,
    EPageSetupAttributeDrawingHeightInPixels = 1,
    EPageSetupAttributeCount,
    EPageSetupAttributeUndefined
}   EPageSetupAttribute;

QString ZSDRAWDLL_API pageSetupAttribute2Str( int i_iAttribute );
EPageSetupAttribute ZSDRAWDLL_API str2PageSetupAttribute( const QString& i_str );

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
    bool operator == ( const CPageSetup& i_other ) const;
    bool operator != ( const CPageSetup& i_other ) const;
public: // instance methods
    void save( QSettings& i_settings, const QString& i_strSettingsKey );
    void load( QSettings& i_settings, const QString& i_strSettingsKey );
public: // instance methods
    void save( QXmlStreamWriter& i_xmlStreamWriter );
    void load( QXmlStreamReader& i_xmlStreamReader );
public: // instance methods
    void set2Default();
    bool isDefault() const;
    void set2Default( int i_iAttribute );
    void set2Default( const QString& i_strName );
    bool isDefault( int i_iAttribute ) const;
    bool isDefault( const QString& i_strName ) const;
public: // instance methods
    void setAttribute( int i_iAttribute, const SAttribute& i_drawAttribute );
    void setAttribute( int i_iAttribute, const QVariant& i_val );
    void setAttribute( int i_iAttribute, const QString& i_strVal );
    void setAttribute( const QString& i_strName, const QVariant& i_val );
    void setAttribute( const QString& i_strName, const QString& i_strVal );
    SAttribute getAttribute( int i_iAttribute ) const;
    SAttribute getAttribute( const QString& i_strName ) const;
    QString attributeValue2Str( int i_iAttribute ) const;
    QString attributeValue2Str( const QString& i_strName ) const;
    int findAttribute( const QString& i_strName ) const;
    int getAttributeCount() const;
    QString getAttributeName( int i_iAttribute ) const;
    QVariant getAttributeValue( int i_iAttribute ) const;
    QVariant getAttributeValue( const QString& i_strName ) const;
public: // instance methods
    void setAttributesAreUsed( bool i_bIsUsed, int i_iAttributeMin = 0, int i_iAttributesCount = EPageSetupAttributeCount );
    void setAttributeIsUsed( int i_iAttribute, bool i_bIsUsed );
    void setAttributeIsUsed( const QString& i_strName, bool i_bIsUsed );
    bool isAttributeUsed( int i_iAttribute ) const;
    bool isAttributeUsed( const QString& i_strName ) const;
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
    void setDrawingWidthInPixels( int i_iWidth_px );
    int getDrawingWidthInPixels() const;
    void setDrawingHeightInPixels( int i_iHeight_px );
    int getDrawingHeightInPixels() const;
    //void setDrawingXScaleFactor( double i_fScaleFactor );
    //double getDrawingXScaleFactor() const { return m_drawArea.m_fXScaleFac; }
    //void setDrawingYScaleFactor( double i_fScaleFactor );
    //double getDrawingYScaleFactor() const { return m_drawArea.m_fYScaleFac; }
protected: // instance members
    SAttribute m_arAttributes[EPageSetupAttributeCount];

}; // class CPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PageSetup_h
