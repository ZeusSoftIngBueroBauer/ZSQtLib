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

#include <QtCore/qsettings.h>
#include <QtGui/qevent.h>
#include <QtGui/qpainter.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmenu.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qtableview.h>
#else
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmenu.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qtableview.h>
#endif

#include "ZSTestGUI/ZSTestStepIdxTreeView.h"
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
class CDelegateIdxTreeTestSteps : public QAbstractItemDelegate
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
                QRect   rctDecoration(styleOption.rect.left(), styleOption.rect.top(), 22, styleOption.rect.height());
                QRect   rctText(rctDecoration.right() + 1, styleOption.rect.top(), styleOption.rect.width() - rctDecoration.width(), styleOption.rect.height());
                QPixmap pxm;
                QString strNodeName;

                if( pModelTreeEntry->isSelected() )
                {
                    pxm = CModelIdxTree::GetIcon(pIdxTreeEntry->entryType()).pixmap(rctDecoration.size());
                }
                else
                {
                    pxm = CModelIdxTree::GetIcon(pIdxTreeEntry->entryType()).pixmap(rctDecoration.size());
                }
                strNodeName = pIdxTreeEntry->name();

                QItemDelegate::drawDecoration(i_pPainter, styleOption, rctDecoration, pxm);
                QItemDelegate::drawDisplay(i_pPainter, styleOption, rctText, strNodeName);

                if(styleOption.state & QStyle::State_HasFocus)
                {
                    QItemDelegate::drawFocus(i_pPainter, styleOption, rctText);
                }
                bPainted = true;
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
            case CModeldxTreeTestSteps::EColumnDesiredValues:
            case CModeldxTreeTestSteps::EColumnActualValues:
            default:
            {
                break;
            }
        } // switch(i_modelIdx.column())
    } // if( pIdxTreeEntry != nullptr )

    if( !bPainted )
    {
        QItemDelegate::paint(i_pPainter, styleOption, i_modelIdx);
    }

} // paint


/*******************************************************************************
class CTreeViewIdxTreeTestSteps : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewIdxTreeTestSteps::CTreeViewIdxTreeTestSteps(
    CModeldxTreeTestSteps* i_pModel,
    QWidget*               i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pDelegate(nullptr),
    m_pMenuGroupContext(nullptr),
    m_pActionTitle(nullptr),
    m_pActionEnableTestGroups(nullptr),
    m_pActionDisableTestGroups(nullptr),
    m_pTreeEntryGroupContextAction(nullptr)
{
    m_pDelegate = new CDelegateIdxTreeTestSteps(this);

    setModel(i_pModel);
    setItemDelegate(m_pDelegate);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);

    setColumnHidden(CModeldxTreeTestSteps::EColumnName, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnBreakpoint, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnEnabled, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnResult, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnDuration, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnOperation, true);
    setColumnHidden(CModeldxTreeTestSteps::EColumnDescription, true);
    setColumnHidden(CModeldxTreeTestSteps::EColumnDesiredValues, false);
    setColumnHidden(CModeldxTreeTestSteps::EColumnActualValues, false);

    m_pMenuGroupContext = new QMenu("Test Group",this);

    m_pActionTitle = new QAction("Test Group:",this);

    QFont fntActionTitle = m_pActionTitle->font();
    fntActionTitle.setBold(true);
    m_pActionTitle->setFont(fntActionTitle);
    m_pMenuGroupContext->addAction(m_pActionTitle);

    m_pMenuGroupContext->addSeparator();

    m_pActionEnableTestGroups = new QAction("Recursively Enable Test Groups",this);
    m_pMenuGroupContext->addAction(m_pActionEnableTestGroups);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionEnableTestGroups,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionEnableTestGroupsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pActionDisableTestGroups = new QAction("Recursively Disable Test Groups",this);
    m_pMenuGroupContext->addAction(m_pActionDisableTestGroups);

    if( !QObject::connect(
        /* pObjSender   */ m_pActionDisableTestGroups,
        /* szSignal     */ SIGNAL( triggered(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onActionDisableTestGroupsTriggered(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    QHeaderView* pHdrView = header();

    #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if( !pHdrView->isClickable() )
    {
        pHdrView->setClickable(true);
    }
    #else
    if( !pHdrView->sectionsClickable() )
    {
        pHdrView->setSectionsClickable(true);
    }
    #endif

    pHdrView->setContextMenuPolicy(Qt::CustomContextMenu);

    #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if( pHdrView->isClickable() )
    #else
    if( pHdrView->sectionsClickable() )
    #endif
    {
        if( !QObject::connect(
            /* pObjSender   */ header(),
            /* szSignal     */ SIGNAL( customContextMenuRequested(const QPoint&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onHeaderCustomContextMenuRequested(const QPoint&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    } // if( pHdrView->sectionsClickable() )

} // ctor

//------------------------------------------------------------------------------
CTreeViewIdxTreeTestSteps::~CTreeViewIdxTreeTestSteps()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pDelegate;
    }
    catch(...)
    {
    }

    m_pDelegate = nullptr;
    m_pMenuGroupContext = nullptr;
    m_pActionTitle = nullptr;
    m_pActionEnableTestGroups = nullptr;
    m_pActionDisableTestGroups = nullptr;
    m_pTreeEntryGroupContextAction = nullptr;

} // dtor

/*==============================================================================
protected: // overridables of base class QTreeView
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::keyPressEvent(QKeyEvent* i_pEv)
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_F5 )
    {
        QAbstractItemModel* pModelAbstract = const_cast<QAbstractItemModel*>(model());

        if( pModelAbstract != nullptr )
        {
            CModeldxTreeTestSteps* pModel = dynamic_cast<CModeldxTreeTestSteps*>(pModelAbstract);

            if( pModel != nullptr )
            {
                //pModel->reset();
            }
        }
    }
    else if( i_pEv->key() == Qt::Key_Return || i_pEv->key() == Qt::Key_Enter || i_pEv->key() == Qt::Key_Space )
    {
        QModelIndex modelIdxSelected = selectionModel()->currentIndex();

        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( modelIdxSelected.isValid() )
        {
            pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
        }

        CIdxTreeEntry* pIdxTreeEntry = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
        }

        if( pIdxTreeEntry != nullptr )
        {
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

            switch( modelIdxSelected.column() )
            {
                case CModeldxTreeTestSteps::EColumnName:
                {
                    break;
                }
                case CModeldxTreeTestSteps::EColumnBreakpoint:
                {
                    if( pTestStep != nullptr )
                    {
                        if( pTestStep->isBreakpointSet() )
                        {
                            pTestStep->removeBreakpoint();
                        }
                        else
                        {
                            pTestStep->setBreakpoint();
                        }
                        //pTestStep->update();
                        bEventHandled = true;
                    }
                    break;
                }
                case CModeldxTreeTestSteps::EColumnEnabled:
                {
                    if( pTestStepEntry->getEnabled() == EEnabled::Yes )
                    {
                        pTestStepEntry->setEnabled(EEnabled::No);
                    }
                    else
                    {
                        pTestStepEntry->setEnabled(EEnabled::Yes);
                    }
                    //pTestStepIdxTreeEntry->update();
                    bEventHandled = true;
                    break;
                }
                case CModeldxTreeTestSteps::EColumnResult:
                case CModeldxTreeTestSteps::EColumnDuration:
                case CModeldxTreeTestSteps::EColumnOperation:
                case CModeldxTreeTestSteps::EColumnDescription:
                case CModeldxTreeTestSteps::EColumnDesiredValues:
                case CModeldxTreeTestSteps::EColumnActualValues:
                default:
                {
                    break;
                }
            } // switch( modelIdxSelected.column() )
        } // if( pIdxTreeEntry != nullptr )
    } // if( i_pEv->key() == Qt::Key_Return || i_pEv->key() == Qt::Key_Enter || i_pEv->key() == Qt::Key_Space )

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->buttons() & Qt::RightButton )
    {
        QModelIndex modelIdxSelected = indexAt(i_pEv->pos());

        CModelIdxTreeEntry* pModelTreeEntry = nullptr;

        if( modelIdxSelected.isValid() )
        {
            pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
        }

        CIdxTreeEntry* pIdxTreeEntry = nullptr;

        if( pModelTreeEntry != nullptr )
        {
            pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
        }

        if( pIdxTreeEntry != nullptr )
        {
            CAbstractTestStepIdxTreeEntry* pTestStepEntry = nullptr;
            CTestStepGroup*                pTestStepGroup = nullptr;

            if( !pIdxTreeEntry->isRoot() )
            {
                pTestStepEntry = dynamic_cast<CAbstractTestStepIdxTreeEntry*>(pIdxTreeEntry);

                if( pTestStepEntry->isGroup() )
                {
                    pTestStepGroup = dynamic_cast<CTestStepGroup*>(pTestStepEntry);
                }
            }

            if( pTestStepGroup != nullptr )
            {
                switch( modelIdxSelected.column() )
                {
                    case CModeldxTreeTestSteps::EColumnName:
                    {
                        if( m_pMenuGroupContext != nullptr )
                        {
                            m_pActionTitle->setText( "Name Space: " + pTestStepGroup->name() );
                            m_pTreeEntryGroupContextAction = pTestStepGroup;
                            m_pMenuGroupContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                        }
                        bEventHandled = true;
                        break;
                    }
                    case CModeldxTreeTestSteps::EColumnBreakpoint:
                    case CModeldxTreeTestSteps::EColumnEnabled:
                    case CModeldxTreeTestSteps::EColumnResult:
                    case CModeldxTreeTestSteps::EColumnDuration:
                    case CModeldxTreeTestSteps::EColumnOperation:
                    case CModeldxTreeTestSteps::EColumnDescription:
                    case CModeldxTreeTestSteps::EColumnDesiredValues:
                    case CModeldxTreeTestSteps::EColumnActualValues:
                    default:
                    {
                        break;
                    }
                } // switch( modelIdxSelected.column() )
            } // if( pTestStepGroup != nullptr )
        } // if( pIdxTreeEntry != nullptr )
    } // if( i_pEv->buttons() & Qt::RightButton )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    QModelIndex modelIdxSelected = indexAt(i_pEv->pos());

    CDelegateIdxTreeTestSteps* pDelegate = dynamic_cast<CDelegateIdxTreeTestSteps*>(itemDelegate());

    CModelIdxTreeEntry* pModelTreeEntry = nullptr;

    if( modelIdxSelected.isValid() )
    {
        pModelTreeEntry = static_cast<CModelIdxTreeEntry*>(modelIdxSelected.internalPointer());
    }

    CIdxTreeEntry* pIdxTreeEntry = nullptr;

    if( pModelTreeEntry != nullptr )
    {
        pIdxTreeEntry = dynamic_cast<CIdxTreeEntry*>(pModelTreeEntry->treeEntry());
    }

    if( pIdxTreeEntry != nullptr )
    {
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

        switch( modelIdxSelected.column() )
        {
            case CModeldxTreeTestSteps::EColumnName:
            {
                //if( pTestStepIdxTreeEntry->isSelected() )
                //{
                //    pTestStepIdxTreeEntry->resetStyleStateFlag(QStyle::State_Selected);
                //}
                break;
            }
            case CModeldxTreeTestSteps::EColumnBreakpoint:
            {
                QRect rectVisual = visualRect(modelIdxSelected);

                if( pDelegate != nullptr && pDelegate->isCheckBoxBreakpointHit(rectVisual,i_pEv->pos(),modelIdxSelected) )
                {
                    if( pTestStep != nullptr )
                    {
                        if( pTestStep->isBreakpointSet() )
                        {
                            pTestStep->removeBreakpoint();
                        }
                        else
                        {
                            pTestStep->setBreakpoint();
                        }
                        //pTestStepIdxTreeEntry->update();
                    }
                    bEventHandled = true;
                }
                break;
            }
            case CModeldxTreeTestSteps::EColumnEnabled:
            {
                QRect rectVisual = visualRect(modelIdxSelected);

                if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),modelIdxSelected) )
                {
                    if( pTestStepEntry->getEnabled() == EEnabled::Yes )
                    {
                        pTestStepEntry->setEnabled(EEnabled::No);
                    }
                    else
                    {
                        pTestStepEntry->setEnabled(EEnabled::Yes);
                    }
                    //pTestStepIdxTreeEntry->update();
                    bEventHandled = true;
                }
                break;
            }
            case CModeldxTreeTestSteps::EColumnOperation:
            case CModeldxTreeTestSteps::EColumnDescription:
            case CModeldxTreeTestSteps::EColumnDesiredValues:
            case CModeldxTreeTestSteps::EColumnActualValues:
            case CModeldxTreeTestSteps::EColumnDuration:
            case CModeldxTreeTestSteps::EColumnResult:
            default:
            {
                break;
            }
        } // switch( modelIdxSelected.column() )
    } // if( pIdxTreeEntry != nullptr )

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onHeaderCustomContextMenuRequested( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    QPoint ptGlobal = mapToGlobal(i_pt);

    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    QMenu*   pMenuHeaderView = new QMenu("", this);
    QAction* pAction;
    QString  strClmName;
    bool     bClmVisible;
    int      idxClm;

    for( idxClm = 0; idxClm < CModeldxTreeTestSteps::EColumnCount; idxClm++ )
    {
        bClmVisible = !isColumnHidden(idxClm);

        if( idxClm != CModeldxTreeTestSteps::EColumnName )
        {
            strClmName = CModeldxTreeTestSteps::column2Str(idxClm);
            pAction = new QAction(strClmName, this);
            pAction->setCheckable(true);
            pAction->setChecked(bClmVisible);
            pAction->setData(idxClm);
            pMenuHeaderView->addAction(pAction);
        }
    } // for( idxClm = 0; idxClm < CModeldxTreeTestSteps::EColumnCount; idxClm++ )

    if( !isColumnHidden(CModeldxTreeTestSteps::EColumnDesiredValues)
     || !isColumnHidden(CModeldxTreeTestSteps::EColumnActualValues) )
    {
        pMenuHeaderView->addSeparator();

        QActionGroup* pActGrpShowValueClms = new QActionGroup(this);

        QAction* pActionShowAlways = new QAction("Show Always", this);
        pActionShowAlways->setCheckable(true);
        pActionShowAlways->setData(-1);
        pMenuHeaderView->addAction(pActionShowAlways);
        pActGrpShowValueClms->addAction(pActionShowAlways);

        QAction* pActionOnlyErrors = new QAction("Only Errors", this);
        pActionOnlyErrors->setCheckable(true);
        pActionOnlyErrors->setData(-2);
        pMenuHeaderView->addAction(pActionOnlyErrors);
        pActGrpShowValueClms->addAction(pActionOnlyErrors);

        if( pModelIdxTree->isSetShowDesiredAndActualValuesOnlyIfTestStepFailed() )
        {
            pActionOnlyErrors->setChecked(true);
        }
        else
        {
            pActionShowAlways->setChecked(true);
        }
    }

    QAction* pActSelected = pMenuHeaderView->exec(ptGlobal);

    if( pActSelected != nullptr )
    {
        idxClm = pActSelected->data().toInt();

        if( idxClm >= 0 && idxClm < CModeldxTreeTestSteps::EColumnCount )
        {
            setColumnHidden(idxClm, !pActSelected->isChecked());
        }
        else if( idxClm == -1 )
        {
            pModelIdxTree->setShowDesiredAndActualValuesOnlyIfTestStepFailed(false);
        }
        else if( idxClm == -2 )
        {
            pModelIdxTree->setShowDesiredAndActualValuesOnlyIfTestStepFailed(true);
        }
    }

} // onHeaderCustomContextMenuRequested

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onActionEnableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    if( m_pTreeEntryGroupContextAction != nullptr && pModelIdxTree != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::Yes );
    }

} // onActionEnableTestGroupsTriggered

//------------------------------------------------------------------------------
void CTreeViewIdxTreeTestSteps::onActionDisableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    CModeldxTreeTestSteps* pModelIdxTree = dynamic_cast<CModeldxTreeTestSteps*>(model());

    if( m_pTreeEntryGroupContextAction != nullptr && pModelIdxTree != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::No );
    }

} // onActionDisableTestGroupsTriggered
