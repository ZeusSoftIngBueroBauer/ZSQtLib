/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qglobal.h>

#include "ZSDraw/ZSDrawWdgtFormatGraphObjs.h"
#include "ZSDraw/ZSDrawGraphObj.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Draw;


/*******************************************************************************
class CWdgtFormatGraphObjs : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtFormatGraphObjs::CWdgtFormatGraphObjs(
    CDrawingScene* i_pDrawingScene,
    CGraphObj*     i_pGraphObj,
    QWidget*       i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pGraphObj(i_pGraphObj),
    m_pGraphicsItem(nullptr),
    m_strObjName(),
    m_strObjId(),
    m_drawSettings(),
    // List of popup widgets which have to be closed on activating other widgets
    m_arpWdgtsPopup()
{
    if( m_pGraphObj != nullptr )
    {
        m_pGraphicsItem = dynamic_cast<QGraphicsItem*>(m_pGraphObj);

        if( m_pGraphicsItem == nullptr )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidDynamicTypeCast, "m_pGraphicsItem" );
        }
        m_drawSettings = m_pGraphObj->getDrawSettings();
    }
    else
    {
        m_drawSettings = m_pDrawingScene->getDrawSettings();
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtFormatGraphObjs::~CWdgtFormatGraphObjs()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    m_pGraphObj = nullptr;

} // dtor

/*==============================================================================
public: // must overridables
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
void CWdgtFormatGraphObjs::applyChanges()
//------------------------------------------------------------------------------
{
    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->setDrawSettings(m_drawSettingsNew);
    }
    if( m_pGraphObj != nullptr )
    {
        m_pGraphObj->setObjName(m_strObjNameNew);
        m_pGraphObj->setDrawSettings(m_drawSettingsNew);
    }

    m_drawSettingsOld = m_drawSettingsNew;

} // applyChanges
#endif

#if 0
//------------------------------------------------------------------------------
void CWdgtFormatGraphObjs::resetChanges()
//------------------------------------------------------------------------------
{
    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->setDrawSettings(m_drawSettingsNew);
    }
    if( m_pGraphObj != nullptr )
    {
        m_pGraphObj->setObjName(m_strObjNameNew);
        m_pGraphObj->setDrawSettings(m_drawSettingsNew);
    }

    m_drawSettingsOld = m_drawSettingsNew;

} // resetChanges
#endif

#if 0
//------------------------------------------------------------------------------
bool CWdgtFormatGraphObjs::hasChanges() const
//------------------------------------------------------------------------------
{
    bool bHasChanges = false;

    if( m_pDrawingScene != nullptr )
    {
        m_pDrawingScene->setDrawSettings(m_drawSettingsNew);
    }
    if( m_pGraphObj != nullptr )
    {
        m_pGraphObj->setObjName(m_strObjNameNew);
        m_pGraphObj->setDrawSettings(m_drawSettingsNew);
    }

    m_drawSettingsOld = m_drawSettingsNew;

    return bHasChanges;

} // hasChanges
#endif

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtFormatGraphObjs::hidePopups( QWidget* i_pWdgtKeepVisible )
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
