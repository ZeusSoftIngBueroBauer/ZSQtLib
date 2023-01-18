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

#ifndef ZSDraw_WgdtPageSetupDrawingScene_h
#define ZSDraw_WgdtPageSetupDrawingScene_h

#include <QtCore/qglobal.h>

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSDraw/DrawingPageSetup/ZSDrawWdgtPageSetup.h"
#include "ZSPhysVal/ZSPhysVal.h"

class QFrame;
class QLabel;
class QLineEdit;

class QLayout;
class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CWdgtPageSetupDrawingScene : public CWdgtPageSetup
//******************************************************************************
{
    Q_OBJECT
public: // instance methods
    CWdgtPageSetupDrawingScene( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent = nullptr );
    ~CWdgtPageSetupDrawingScene();
public: // overridables of base class CWdgtFormatGraphObjs
    void setSettings( const CPageSetup& i_pageSetup );
protected slots: // Size
    void onEdtSizeWidthTextChanged( const QString& i_strText );
    void onEdtSizeHeightTextChanged( const QString& i_strText );
private: // instance members
    QVBoxLayout*             m_pLyt;
    QLabel*                  m_pLblHeadLine;
    QHBoxLayout*             m_pLytResolutionDpi;
    QLabel*                  m_pLblResolutionDpi;
    QLabel*                  m_pLblResolutionDpiVal;
    QLabel*                  m_pLblResolutionDpiUnit;
    QHBoxLayout*             m_pLytResolutionDpmm;
    QLabel*                  m_pLblResolutionDpmm;
    QLabel*                  m_pLblResolutionDpmmVal;
    QLabel*                  m_pLblResolutionDpmmUnit;
    QHBoxLayout*             m_pLytSepLineSize;
    QLabel*                  m_pLblSepLineSize;
    QFrame*                  m_pSepLineSize;
    QHBoxLayout*             m_pLytSizeWidth;
    QLabel*                  m_pLblSizeWidth;
    QLineEdit*               m_pEdtSizeWidth;
    QHBoxLayout*             m_pLytSizeHeight;
    QLabel*                  m_pLblSizeHeight;
    QLineEdit*               m_pEdtSizeHeight;
    // Trace
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;

}; // CWdgtPageSetupDrawingScene

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_WgdtPageSetupDrawingScene_h
