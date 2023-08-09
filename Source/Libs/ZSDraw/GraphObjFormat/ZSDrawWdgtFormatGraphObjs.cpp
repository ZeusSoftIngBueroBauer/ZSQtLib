/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h"
#include "ZSDraw/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
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
    const QString& i_strClassName,
    const QString& i_strObjName,
    CGraphObj* i_pGraphObj,
    QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent),
    m_pDrawingScene(i_pDrawingScene),
    m_pGraphObj(i_pGraphObj),
    m_pGraphicsItem(nullptr),
    m_drawSettings(),
    // List of popup widgets which have to be closed on activating other widgets
    m_arpWdgtsPopup(),
    m_bApplyingChanges(false)
{
    if( m_pGraphObj != nullptr )
    {
        QObject::connect(
            /* pObjSender   */ m_pDrawingScene, &CDrawingScene::graphObjChanged,
            /* pObjReceiver */ this, &CWdgtFormatGraphObjs::onDrawingSceneGraphObjChanged );

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
    m_pGraphicsItem = nullptr;
    //m_drawSettings;
    m_arpWdgtsPopup.clear();
    m_bApplyingChanges = false;

} // dtor

/*==============================================================================
public: // must overridables
==============================================================================*/

#if 0
//------------------------------------------------------------------------------
/*! On changing a setting of the graphical object "onGraphObjChanged" will be called as reentry.
    "onGraphObjChanged" will modify the states of the GUI controls. E.g. if the "NameLabelVisible" and
    "NameLabelAnchorLineVisible" CheckBoxes are checked and you call "showLabel" "onGraphObjChanged"
    is immediately called resetting the check state of "NameLabelAnchorLineVisible".
    To avoid this undesired correction the "onGraphObjChanged" method must be disabled while applying
    changes but will be called after all changes have been applied. For this the flag "m_bApplyingChanges"
    of the base class "CWdgtFormatGraphObjs" is set at the beginning, reset at the end of "applyChanges"
    and afterwards "onGraphObjChanged" is explicitly called.
*/
void CWdgtFormatGraphObjs::applyChanges()
//------------------------------------------------------------------------------
{
    m_bApplyingChanges = true;

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

    m_bApplyingChanges = false;

    onGraphObjChanged();

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

/*==============================================================================
private slots:
==============================================================================*/

//------------------------------------------------------------------------------
/*! Slot connected to the graphObjChanged signal of the drawing scene.

    The signal is emitted for any changed object of the drawing scene.
    The slot checks whether the changed object is the object shown by the widget
    and invokes the pure virtual method onGraphObjChanged.
    For all other objects the slot will be ignored.

    @param i_strKeyInTree [in]
        Unique key of the changed object.
*/
void CWdgtFormatGraphObjs::onDrawingSceneGraphObjChanged(const QString& i_strKeyInTree)
//------------------------------------------------------------------------------
{
    if (m_pGraphObj != nullptr && m_pGraphObj->keyInTree() == i_strKeyInTree)
    {
        /*! On changing a setting of the graphical object "onGraphObjChanged" will be called as reentry.
            "onGraphObjChanged" will modify the states of the GUI controls. E.g. if the "NameLabelVisible" and
            "NameLabelAnchorLineVisible" CheckBoxes are checked and you call "showLabel" "onGraphObjChanged"
            is immediately called resetting the check state of "NameLabelAnchorLineVisible".
            To avoid this undesired correction the "onGraphObjChanged" method must be disabled while applying
            changes but will be called after all changes have been applied. For this the flag "m_bApplyingChanges"
            of the base class "CWdgtFormatGraphObjs" is set at the beginning, reset at the end of "applyChanges"
            and afterwards "onGraphObjChanged" is explicitly called.
        */
        if( !m_bApplyingChanges )
        {
            onGraphObjChanged();
        }
    }
}
