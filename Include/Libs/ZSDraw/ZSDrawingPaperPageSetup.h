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

#ifndef ZSDraw_DrawingPaperPageSetup_h
#define ZSDraw_DrawingPaperPageSetup_h

#include "ZSDraw/ZSDrawDllMain.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CDrawingPaperPageSetup
//******************************************************************************
{
public: // class methods
    //static double GetFormatWidthInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
    //static double GetFormatHeightInMilliMeter( EPaperFormat i_format, EPaperOrientation i_orientation );
public: // ctors and dtor
    CDrawingPaperPageSetup();
    ~CDrawingPaperPageSetup();
public: // operators
    bool operator == ( const CDrawingPaperPageSetup& i_other ) const;
    bool operator != ( const CDrawingPaperPageSetup& i_other ) const;
public: // instance methods
    void save( QSettings& i_settings, const QString& i_strSettingsKey );
    void load( QSettings& i_settings, const QString& i_strSettingsKey );
public: // instance methods
    void save( QXmlStreamWriter& i_xmlStreamWriter );
    void load( QXmlStreamReader& i_xmlStreamReader );
public: // instance methods
    QString toString() const;
protected: // instance members

}; // class CDrawingPaperPageSetup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_DrawingPaperPageSetup_h
