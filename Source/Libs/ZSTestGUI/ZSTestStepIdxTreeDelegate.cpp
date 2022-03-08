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
#include <QtGui/qpainter.h>

#include "ZSTestGUI/ZSTestStepIdxTreeDelegate.h"
#include "ZSTestGUI/ZSTestStepIdxTreeModel.h"
#include "ZSSysGUI/ZSSysIdxTreeModelEntry.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CDelegateIdxTreeTestSteps : public QItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateIdxTreeTestSteps::CDelegateIdxTreeTestSteps( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QItemDelegate(i_pObjParent),
    m_rectChkBoxBreakpoint(0,0,10,10),
    m_rectChkBoxEnabled(0,0,10,10)
{
} // ctor

//------------------------------------------------------------------------------
CDelegateIdxTreeTestSteps::~CDelegateIdxTreeTestSteps()
//------------------------------------------------------------------------------
{
    //m_rectChkBoxBreakpoiont;
    //m_rectChkBoxEnabled;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateIdxTreeTestSteps::isCheckBoxBreakpointHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& /*i_modelIdx*/ )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxBreakpoint.width()
     && i_rectVisual.height() <= m_rectChkBoxBreakpoint.height() )
    {
        bIsHit = true;
    }

    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxBreakpoint;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxBreakpoint.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxBreakpoint.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxBreakpoint.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxBreakpoint.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;

} // isCheckBoxBreakpointHit

//------------------------------------------------------------------------------
bool CDelegateIdxTreeTestSteps::isCheckBoxEnabledHit(
    const QRect&       i_rectVisual,
    const QPoint&      i_ptPos,
    const QModelIndex& /*i_modelIdx*/ )
//------------------------------------------------------------------------------
{
    bool bIsHit = false;

    // If the check box is bigger than the visual rect ..
    if( i_rectVisual.width() <= m_rectChkBoxEnabled.width()
     && i_rectVisual.height() <= m_rectChkBoxEnabled.height() )
    {
        bIsHit = true;
    }

    // If the check box is smaller than the visual rect ..
    else
    {
        QRect rectCheckBox = m_rectChkBoxEnabled;

        rectCheckBox.moveLeft(i_rectVisual.left());
        rectCheckBox.moveTop(i_rectVisual.top());

        if( i_rectVisual.width() > m_rectChkBoxEnabled.width() )
        {
            rectCheckBox.moveLeft(i_rectVisual.left()+(i_rectVisual.width()-m_rectChkBoxEnabled.width())/2);
        }
        if( i_rectVisual.height() > m_rectChkBoxEnabled.height() )
        {
            rectCheckBox.moveTop(i_rectVisual.top()+(i_rectVisual.height()-m_rectChkBoxEnabled.height())/2);
        }
        if( rectCheckBox.contains(i_ptPos) )
        {
            bIsHit = true;
        }
    }
    return bIsHit;

} // isCheckBoxEnabledHit

/*==============================================================================
public: // overridables of base class QItemDelegate
==============================================================================*/

//------------------------------------------------------------------------------
void CDelegateIdxTreeTestSteps::paint(
    QPainter*                   i_pPainter,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bPainted = false;

    const CModelIdxTreeEntry* pCModelTreeEntry = static_cast<const CModelIdxTreeEntry*>(i_modelIdx.internalPointer());
    CModelIdxTreeEntry*       pModelTreeEntry = const_cast<CModelIdxTreeEntry*>(pCModelTreeEntry);

    QStyleOptionViewItem styleOption = i_styleOption;

    CIdxTreeEntry* pIdxTreeEntry = nullptr;

    if( pModelTreeEntry != nullptr )
    {
        pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
    }

    if( pIdxTreeEntry != nullptr )
    {
        i_pPainter->save();

        CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
        CTestStepGroup*                pTestStepGroup = nullptr;
        CTestStep*                     pTestStep = nullptr;

        if( !pIdxTreeEntry->isRoot() )
        {
            pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

            if( pTestStepEntry->isGroup() )
            {
                pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
            }
            else
            {
                pTestStep = dynamic_cast<CTestStep*>(pTestStepEntry);
            }
        }

        switch(i_modelIdx.column())
        {
            case CModeldxTreeTestSteps::EColumnName:
            {
                //QRect   rctDecoration(styleOption.rect.left(), styleOption.rect.top(), 22, styleOption.rect.height());
                //QRect   rctText(rctDecoration.right() + 1, styleOption.rect.top(), styleOption.rect.width() - rctDecoration.width(), styleOption.rect.height());
                //QPixmap pxm;
                //QString strNodeName;

                //if( pModelTreeEntry->isSelected() )
                //{
                //    pxm = CModelIdxTree::GetIcon(pIdxTreeEntry->entryType()).pixmap(rctDecoration.size());
                //}
                //else
                //{
                //    pxm = CModelIdxTree::GetIcon(pIdxTreeEntry->entryType()).pixmap(rctDecoration.size());
                //}
                //strNodeName = pIdxTreeEntry->name();

                //QItemDelegate::drawDecoration(i_pPainter, styleOption, rctDecoration, pxm);
                //QItemDelegate::drawDisplay(i_pPainter, styleOption, rctText, strNodeName);

                //if( styleOption.state & QStyle::State_HasFocus )
                //{
                //    QItemDelegate::drawFocus(i_pPainter, styleOption, rctText);
                //}
                //bPainted = true;
                break;
            }

            case CModeldxTreeTestSteps::EColumnBreakpoint:
            {
                if( pTestStep != nullptr )
                {
                    QRect          rectCheckBox = m_rectChkBoxBreakpoint;
                    Qt::CheckState checkState = Qt::Unchecked;

                    rectCheckBox.moveLeft(styleOption.rect.left());
                    rectCheckBox.moveTop(styleOption.rect.top());

                    if( pModelTreeEntry->isSelected() )
                    {
                        styleOption.state |= QStyle::State_Selected;
                    }
                    if( styleOption.rect.width() > m_rectChkBoxBreakpoint.width() )
                    {
                        rectCheckBox.moveLeft(styleOption.rect.left() + (styleOption.rect.width() - m_rectChkBoxBreakpoint.width()) / 2);
                    }
                    if( styleOption.rect.height() > m_rectChkBoxBreakpoint.height() )
                    {
                        rectCheckBox.moveTop(styleOption.rect.top() + (styleOption.rect.height() - m_rectChkBoxBreakpoint.height()) / 2);
                    }

                    if( pTestStep->isBreakpointSet() )
                    {
                        checkState = Qt::Checked;

                        i_pPainter->setPen(Qt::red);
                        i_pPainter->setBrush(Qt::red);
                        i_pPainter->drawEllipse(rectCheckBox);
                    }
                    else // if( !pTestStep->isBreakpointSet() )
                    {
                        checkState = Qt::Unchecked;

                        QItemDelegate::drawBackground(i_pPainter, styleOption, i_modelIdx);
                        QItemDelegate::drawCheck(i_pPainter, styleOption, rectCheckBox, checkState);
                    }

                    if( styleOption.state & QStyle::State_HasFocus )
                    {
                        QItemDelegate::drawFocus(i_pPainter, styleOption, styleOption.rect);
                    }

                    bPainted = true;

                } // if( pTestStep != nullptr )
                break;
            } // case CModeldxTreeTestSteps::EColumnBreakpoint

            case CModeldxTreeTestSteps::EColumnEnabled:
            {
                if( pTestStepEntry != nullptr )
                {
                    QRect          rectCheckBox = m_rectChkBoxEnabled;
                    Qt::CheckState checkState = Qt::Unchecked;

                    rectCheckBox.moveLeft(styleOption.rect.left());
                    rectCheckBox.moveTop(styleOption.rect.top());

                    if( pModelTreeEntry->isSelected() )
                    {
                        styleOption.state |= QStyle::State_Selected;
                    }
                    if( styleOption.rect.width() > m_rectChkBoxEnabled.width() )
                    {
                        rectCheckBox.moveLeft(styleOption.rect.left() + (styleOption.rect.width() - m_rectChkBoxEnabled.width()) / 2);
                    }
                    if( styleOption.rect.height() > m_rectChkBoxEnabled.height() )
                    {
                        rectCheckBox.moveTop(styleOption.rect.top() + (styleOption.rect.height() - m_rectChkBoxEnabled.height()) / 2);
                    }
                    if( pTestStepEntry->getEnabled() == EEnabled::Yes )
                    {
                        checkState = Qt::Checked;
                    }
                    else
                    {
                        checkState = Qt::Unchecked;
                    }

                    QItemDelegate::drawBackground(i_pPainter, styleOption, i_modelIdx);
                    QItemDelegate::drawCheck(i_pPainter, styleOption, rectCheckBox, checkState);

                    if( styleOption.state & QStyle::State_HasFocus )
                    {
                        QItemDelegate::drawFocus(i_pPainter, styleOption, styleOption.rect);
                    }

                    bPainted = true;

                } // if( pTestStepIdxTreeEntry != nullptr )
                break;
            } // case CModeldxTreeTestSteps::EColumnEnabled

            case CModeldxTreeTestSteps::EColumnResult:
            case CModeldxTreeTestSteps::EColumnDuration:
            case CModeldxTreeTestSteps::EColumnOperation:
            case CModeldxTreeTestSteps::EColumnDescription:
            case CModeldxTreeTestSteps::EColumnExpectedValues:
            case CModeldxTreeTestSteps::EColumnResultValues:
            default:
            {
                break;
            }
        } // switch(i_modelIdx.column())

        i_pPainter->restore();

    } // if( pIdxTreeEntry != nullptr )

    if( !bPainted )
    {
        QItemDelegate::paint(i_pPainter, styleOption, i_modelIdx);
    }

} // paint
