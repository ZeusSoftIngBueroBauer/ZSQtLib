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

#error Module not used anymore

#ifndef ZSDraw_DrawingViewPageSetup_h
#define ZSDraw_DrawingViewPageSetup_h

#include "ZSDraw/Common/ZSDrawDllMain.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CDrawingViewPageSetup
//******************************************************************************
{
public: // class methods
    //static double GetFormatWidthInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
    //static double GetFormatHeightInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
public: // ctors and dtor
    CDrawingViewPageSetup();
    CDrawingViewPageSetup( const QSize& i_size, const QMargins& i_marginsViewport );
    ~CDrawingViewPageSetup();
public: // operators
    bool operator == ( const CDrawingViewPageSetup& i_other ) const;
    bool operator != ( const CDrawingViewPageSetup& i_other ) const;
public: // instance methods
    void save( QSettings& i_settings, const QString& i_strSettingsKey );
    void load( QSettings& i_settings, const QString& i_strSettingsKey );
public: // instance methods
    void save( QXmlStreamWriter& i_xmlStreamWriter );
    void load( QXmlStreamReader& i_xmlStreamReader );
public: // instance methods (drawing area)
    void setDrawingSize( const QSize& i_size );
    void setDrawingSizeInPixels( int i_iWidth_px, int i_iHeight_px );
    void setDrawingWidthInPixels( int i_iWidth_px );
    void setDrawingHeightInPixels( int i_iHeight_px );
    QSize getDrawingSize() const { return m_sizeDrawing; }
    int getDrawingWidthInPixels() const { return m_sizeDrawing.width(); }
    int getDrawingHeightInPixels() const { return m_sizeDrawing.height(); }
public: // instance methods (drawing area)
    void setViewportMargins( int i_iLeft, int i_iTop, int i_iRight, int i_iBottom );
    void setViewportMargins( const QMargins& i_margins );
    QMargins getViewportMargins() const { return m_marginsViewport; };
public: // instance methods
    QString toString() const;
protected: // instance members
    QSize    m_sizeDrawing;
    QMargins m_marginsViewport;

}; // class CDrawingViewPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingViewPageSetup_h

#endif
