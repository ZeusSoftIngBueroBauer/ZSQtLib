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

#include <QtCore/qglobal.h>

#include "ZSDraw/DrawingPageSetup/ZSDrawWdgtPageSetup.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::Draw;


/*******************************************************************************
class CWdgtPageSetup : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtPageSetup::CWdgtPageSetup( CDrawingView* i_pDrawingView, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingView(i_pDrawingView),
    m_pageSetup(),
    // List of popup widgets which have to be closed on activating other widgets
    m_arpWdgtsPopup()
{
} // ctor

//------------------------------------------------------------------------------
CWdgtPageSetup::~CWdgtPageSetup()
//------------------------------------------------------------------------------
{
    m_pDrawingView = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

/*==============================================================================
public: // overridables of base class CWdgtPageSetup
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtPageSetup::setSettings( const CPageSetup& i_pageSetup )
//------------------------------------------------------------------------------
{
    m_pageSetup = i_pageSetup;
}

//------------------------------------------------------------------------------
void CWdgtPageSetup::hidePopups( QWidget* i_pWdgtKeepVisible )
//------------------------------------------------------------------------------
{
    QWidget* pWdgt;
    int      idxWdgt;

    for( idxWdgt = 0; idxWdgt < m_arpWdgtsPopup.size(); idxWdgt++ )
    {
        pWdgt = m_arpWdgtsPopup[idxWdgt];

        if( pWdgt != i_pWdgtKeepVisible )
        {
            pWdgt->hide();
        }
    }

} // hidePopups

#endif
