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

#ifndef ZSDraw_WdgtPageSetupDrawingPaper_h
#define ZSDraw_WdgtPageSetupDrawingPaper_h

#include <QtCore/qglobal.h>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawWdgtPageSetup.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QLabel;

class QLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtPageSetupDrawingPaper : public CWdgtPageSetup
//******************************************************************************
{
    Q_OBJECT
public: // instance methods
    CWdgtPageSetupDrawingPaper( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr );
    ~CWdgtPageSetupDrawingPaper();
public: // overridables of base class CWdgtFormatGraphObjs
    void setSettings( const CPageSetup& i_pageSetup );
protected: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    // Trace
    ZS::Trace::CTrcAdminObj* m_pTrcAdminObj;

}; // CWdgtPageSetupDrawingPaper

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WdgtPageSetupDrawingPaper_h
