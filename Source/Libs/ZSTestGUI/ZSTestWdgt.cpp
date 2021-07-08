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

#include "ZSTestGUI/ZSTestWdgt.h"
#include "ZSTestGUI/ZSTestStepAdminObjPoolModel.h"
#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysRequest.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;
using namespace ZS::Test;
using namespace ZS::Test::GUI;


/*******************************************************************************
class CDelegateModelTest : public QAbstractItemDelegate
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CDelegateModelTest::CDelegateModelTest( QObject* i_pObjParent ) :
//------------------------------------------------------------------------------
    QItemDelegate(i_pObjParent),
    m_rectChkBoxBreakpoint(0,0,10,10),
    m_rectChkBoxEnabled(0,0,10,10)
{
} // ctor

//------------------------------------------------------------------------------
CDelegateModelTest::~CDelegateModelTest()
//------------------------------------------------------------------------------
{
    //m_rectChkBoxBreakpoiont;
    //m_rectChkBoxEnabled;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDelegateModelTest::isCheckBoxBreakpointHit(
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
bool CDelegateModelTest::isCheckBoxEnabledHit(
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
void CDelegateModelTest::paint(
    QPainter*                   i_pPainter,
    const QStyleOptionViewItem& i_styleOption,
    const QModelIndex&          i_modelIdx ) const
//------------------------------------------------------------------------------
{
    bool bPainted = false;

    const CObjPoolTreeEntry* pCTreeEntry  = static_cast<const CObjPoolTreeEntry*>(i_modelIdx.internalPointer());
    CObjPoolTreeEntry*       pVTreeEntry  = const_cast<CObjPoolTreeEntry*>(pCTreeEntry);
    CTestStepAdminObj*       pTSAdmObj    = nullptr;
    CTestStep*               pTestStep    = nullptr;
    QStyleOptionViewItem     styleOption  = i_styleOption;

    if( pVTreeEntry != nullptr )
    {
        pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pVTreeEntry->getObj());
    }

    if( pTSAdmObj != nullptr )
    {
        if( pTSAdmObj->isGroup() )
        {
            //pTSGrp = dynamic_cast<CTestStepGroup*>(pTSAdmObj);
        }
        else
        {
            pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
        }
    } // if( pTSAdmObj != nullptr )

    switch( i_modelIdx.column() )
    {
        case CTestStepAdminObjPoolModel::EColumnName:
        {
            if( pVTreeEntry != nullptr )
            {
                QRect   rctDecoration(styleOption.rect.left(),styleOption.rect.top(),22,styleOption.rect.height());
                QRect   rctText(rctDecoration.right()+1,styleOption.rect.top(),styleOption.rect.width()-rctDecoration.width(),styleOption.rect.height());
                QPixmap pxm;
                QString strNodeName;

                if( pVTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    pxm = CModelObjPool::GetIcon(pVTreeEntry->getEntryType()).pixmap(rctDecoration.size());
                }
                else
                {
                    pxm = CModelObjPool::GetIcon(pVTreeEntry->getEntryType()).pixmap(rctDecoration.size());
                }
                strNodeName = pVTreeEntry->getNodeName(false);

                QItemDelegate::drawDecoration(i_pPainter, styleOption, rctDecoration, pxm);
                QItemDelegate::drawDisplay(i_pPainter, styleOption, rctText, strNodeName);

                if( styleOption.state & QStyle::State_HasFocus )
                {
                    QItemDelegate::drawFocus(i_pPainter,styleOption,rctText);
                }
                bPainted = true;
            }
            break;
        }
        case CTestStepAdminObjPoolModel::EColumnObjId:
        {
            break;
        }

        case CTestStepAdminObjPoolModel::EColumnBreakpoint:
        {
            if( pTestStep != nullptr )
            {
                QRect          rectCheckBox = m_rectChkBoxBreakpoint;
                Qt::CheckState checkState   = Qt::Unchecked;

                rectCheckBox.moveLeft(styleOption.rect.left());
                rectCheckBox.moveTop(styleOption.rect.top());

                if( pVTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    styleOption.state |= QStyle::State_Selected;
                }
                if( styleOption.rect.width() > m_rectChkBoxBreakpoint.width() )
                {
                    rectCheckBox.moveLeft(styleOption.rect.left()+(styleOption.rect.width()-m_rectChkBoxBreakpoint.width())/2);
                }
                if( styleOption.rect.height() > m_rectChkBoxBreakpoint.height() )
                {
                    rectCheckBox.moveTop(styleOption.rect.top()+(styleOption.rect.height()-m_rectChkBoxBreakpoint.height())/2);
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
        } // case CTestStepAdminObjPoolModel::EColumnBreakpoint

        case CTestStepAdminObjPoolModel::EColumnEnabled:
        {
            if( pTSAdmObj != nullptr )
            {
                QRect          rectCheckBox = m_rectChkBoxEnabled;
                Qt::CheckState checkState   = Qt::Unchecked;

                rectCheckBox.moveLeft(styleOption.rect.left());
                rectCheckBox.moveTop(styleOption.rect.top());

                if( pVTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    styleOption.state |= QStyle::State_Selected;
                }
                if( styleOption.rect.width() > m_rectChkBoxEnabled.width() )
                {
                    rectCheckBox.moveLeft(styleOption.rect.left()+(styleOption.rect.width()-m_rectChkBoxEnabled.width())/2);
                }
                if( styleOption.rect.height() > m_rectChkBoxEnabled.height() )
                {
                    rectCheckBox.moveTop(styleOption.rect.top()+(styleOption.rect.height()-m_rectChkBoxEnabled.height())/2);
                }
                if( pTSAdmObj->getEnabled() == EEnabled::Yes )
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

            } // if( pTSAdmObj != nullptr )
            break;
        } // case CTestStepAdminObjPoolModel::EColumnEnabled

        case CTestStepAdminObjPoolModel::EColumnResult:
        case CTestStepAdminObjPoolModel::EColumnDuration:
        case CTestStepAdminObjPoolModel::EColumnOperation:
        case CTestStepAdminObjPoolModel::EColumnDescription:
        case CTestStepAdminObjPoolModel::EColumnDesiredValues:
        case CTestStepAdminObjPoolModel::EColumnActualValues:
        default:
        {
            break;
        }
    }

    if( !bPainted )
    {
        QItemDelegate::paint(i_pPainter,styleOption,i_modelIdx);
    }

} // paint


/*******************************************************************************
class CTreeViewModelTest : public QTreeView
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTreeViewModelTest::CTreeViewModelTest( CTestStepAdminObjPoolModel* i_pAdminObjPoolModel, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTreeView(i_pWdgtParent),
    m_pTestStepsModel(i_pAdminObjPoolModel),
    m_pDelegate(nullptr),
    m_pMenuGroupContext(nullptr),
    m_pActionTitle(nullptr),
    m_pActionEnableTestGroups(nullptr),
    m_pActionDisableTestGroups(nullptr),
    m_pTreeEntryGroupContextAction(nullptr)
{
    m_pDelegate = new CDelegateModelTest(this);

    setModel(m_pTestStepsModel);
    setItemDelegate(m_pDelegate);

    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAllColumnsShowFocus(true);

    setColumnHidden(CTestStepAdminObjPoolModel::EColumnName, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnObjId, true);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnBreakpoint, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnEnabled, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnResult, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnDuration, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnOperation, true);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnDescription, true);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnDesiredValues, false);
    setColumnHidden(CTestStepAdminObjPoolModel::EColumnActualValues, false);

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
CTreeViewModelTest::~CTreeViewModelTest()
//------------------------------------------------------------------------------
{
    try
    {
        delete m_pDelegate;
    }
    catch(...)
    {
    }

    m_pTestStepsModel = nullptr;
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
void CTreeViewModelTest::keyPressEvent( QKeyEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->key() == Qt::Key_Return
     || i_pEv->key() == Qt::Key_Enter
     || i_pEv->key() == Qt::Key_Space
     || i_pEv->key() == Qt::Key_F5 )
    {
        QModelIndex                 modelIdxSelected = selectionModel()->currentIndex();
        CObjPoolTreeEntry*          pTreeEntry = nullptr;
        CTestStepAdminObj*          pTSAdmObj = nullptr;
        CTestStep*                  pTestStep = nullptr;
        QAbstractItemModel*         pModelAbstract = nullptr;
        CTestStepAdminObjPoolModel* pModel = nullptr;

        pModelAbstract = const_cast<QAbstractItemModel*>(modelIdxSelected.model());

        if( pModelAbstract != nullptr )
        {
            pModel = dynamic_cast<CTestStepAdminObjPoolModel*>(pModelAbstract);
        }

        if( i_pEv->key() == Qt::Key_F5 )
        {
            if( pModel != nullptr )
            {
                pModel->reset();
            }
        }
        else // if( i_pEv->key() != Qt::Key_F5 )
        {
            if( modelIdxSelected.isValid() )
            {
                pTreeEntry = static_cast<CObjPoolTreeEntry*>(modelIdxSelected.internalPointer());

                if( pTreeEntry != nullptr )
                {
                    pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());

                    if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
                    {
                        pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
                    }
                }
            } // if( modelIdxSelected.isValid() )

            if( pTSAdmObj != nullptr )
            {
                switch( modelIdxSelected.column() )
                {
                    case CTestStepAdminObjPoolModel::EColumnName:
                    case CTestStepAdminObjPoolModel::EColumnObjId:
                    {
                        break;
                    }
                    case CTestStepAdminObjPoolModel::EColumnBreakpoint:
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
                            pTestStep->update();
                            bEventHandled = true;
                        }
                        break;
                    }
                    case CTestStepAdminObjPoolModel::EColumnEnabled:
                    {
                        if( pTSAdmObj->getEnabled() == EEnabled::Yes )
                        {
                            pTSAdmObj->setEnabled(EEnabled::No);
                        }
                        else
                        {
                            pTSAdmObj->setEnabled(EEnabled::Yes);
                        }
                        pTSAdmObj->update();
                        bEventHandled = true;
                        break;
                    }
                    case CTestStepAdminObjPoolModel::EColumnResult:
                    case CTestStepAdminObjPoolModel::EColumnDuration:
                    case CTestStepAdminObjPoolModel::EColumnOperation:
                    case CTestStepAdminObjPoolModel::EColumnDescription:
                    case CTestStepAdminObjPoolModel::EColumnDesiredValues:
                    case CTestStepAdminObjPoolModel::EColumnActualValues:
                    default:
                    {
                        break;
                    }
                } // switch( modelIdxSelected.column() )
            } // if( pTSAdmObj != nullptr )
        } // if( i_pEv->key() != Qt::Key_F5 )
    } // if( i_pEv->key() == Qt::Key_Return ...

    if( !bEventHandled )
    {
        QTreeView::keyPressEvent(i_pEv);
    }

} // keyPressEvent

//------------------------------------------------------------------------------
void CTreeViewModelTest::mousePressEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    if( i_pEv->buttons() & Qt::RightButton )
    {
        QModelIndex                 modelIdxSelected = indexAt(i_pEv->pos());
        CObjPoolTreeEntry*          pTreeEntry = nullptr;
        CTestStepAdminObj*          pTSAdmObj = nullptr;

        if( modelIdxSelected.isValid() )
        {
            pTreeEntry = static_cast<CObjPoolTreeEntry*>(modelIdxSelected.internalPointer());
        }
        if( pTreeEntry != nullptr )
        {
            pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());
        }

        if( pTSAdmObj != nullptr && pTSAdmObj->isGroup() )
        {
            switch( modelIdxSelected.column() )
            {
                case CTestStepAdminObjPoolModel::EColumnName:
                {
                    if( m_pMenuGroupContext != nullptr )
                    {
                        m_pActionTitle->setText( "Name Space: " + pTreeEntry->getNodeName(false) );
                        m_pTreeEntryGroupContextAction = pTreeEntry;
                        m_pMenuGroupContext->popup(QWidget::mapToGlobal(i_pEv->pos()));
                    }
                    bEventHandled = true;
                    break;
                }
                case CTestStepAdminObjPoolModel::EColumnObjId:
                case CTestStepAdminObjPoolModel::EColumnBreakpoint:
                case CTestStepAdminObjPoolModel::EColumnEnabled:
                case CTestStepAdminObjPoolModel::EColumnResult:
                case CTestStepAdminObjPoolModel::EColumnDuration:
                case CTestStepAdminObjPoolModel::EColumnOperation:
                case CTestStepAdminObjPoolModel::EColumnDescription:
                case CTestStepAdminObjPoolModel::EColumnDesiredValues:
                case CTestStepAdminObjPoolModel::EColumnActualValues:
                default:
                {
                    break;
                }
            } // switch( modelIdxSelected.column() )
        } // if( pTSAdmObj != nullptr && pTSAdmObj->isGroup() )
    } // if( i_pEv->buttons() & Qt::RightButton )

    if( !bEventHandled )
    {
        QTreeView::mousePressEvent(i_pEv);
    }

} // mousePressEvent

//------------------------------------------------------------------------------
void CTreeViewModelTest::mouseReleaseEvent( QMouseEvent* i_pEv )
//------------------------------------------------------------------------------
{
    bool bEventHandled = false;

    QModelIndex                 modelIdxSelected = indexAt(i_pEv->pos());
    CObjPoolTreeEntry*          pTreeEntry = nullptr;
    CTestStepAdminObj*          pTSAdmObj = nullptr;
    CTestStep*                  pTestStep = nullptr;
    CDelegateModelTest*         pDelegate = nullptr;

    pDelegate = dynamic_cast<CDelegateModelTest*>(itemDelegate());

    if( modelIdxSelected.isValid() )
    {
        pTreeEntry = static_cast<CObjPoolTreeEntry*>(modelIdxSelected.internalPointer());

        if( pTreeEntry != nullptr )
        {
            pTSAdmObj = reinterpret_cast<CTestStepAdminObj*>(pTreeEntry->getObj());

            if( pTreeEntry->getEntryType() == EObjPoolEntryTypeObject )
            {
                pTestStep = dynamic_cast<CTestStep*>(pTSAdmObj);
            }
        }
    } // if( modelIdxSelected.isValid() )

    if( pTSAdmObj != nullptr )
    {
        switch( modelIdxSelected.column() )
        {
            case CTestStepAdminObjPoolModel::EColumnName:
            {
                if( pTreeEntry->getStyleState() & QStyle::State_Selected )
                {
                    pTreeEntry->resetStyleStateFlag(QStyle::State_Selected);
                }
                break;
            }
            case CTestStepAdminObjPoolModel::EColumnObjId:
            {
                break;
            }
            case CTestStepAdminObjPoolModel::EColumnBreakpoint:
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
                        pTSAdmObj->update();
                    }
                    bEventHandled = true;
                }
                break;
            }
            case CTestStepAdminObjPoolModel::EColumnEnabled:
            {
                QRect rectVisual = visualRect(modelIdxSelected);

                if( pDelegate != nullptr && pDelegate->isCheckBoxEnabledHit(rectVisual,i_pEv->pos(),modelIdxSelected) )
                {
                    if( pTSAdmObj->getEnabled() == EEnabled::Yes )
                    {
                        pTSAdmObj->setEnabled(EEnabled::No);
                    }
                    else
                    {
                        pTSAdmObj->setEnabled(EEnabled::Yes);
                    }
                    pTSAdmObj->update();
                    bEventHandled = true;
                }
                break;
            }
            case CTestStepAdminObjPoolModel::EColumnOperation:
            case CTestStepAdminObjPoolModel::EColumnDescription:
            case CTestStepAdminObjPoolModel::EColumnDesiredValues:
            case CTestStepAdminObjPoolModel::EColumnActualValues:
            case CTestStepAdminObjPoolModel::EColumnDuration:
            case CTestStepAdminObjPoolModel::EColumnResult:
            default:
            {
                break;
            }
        } // switch( modelIdxSelected.column() )
    } // if( pTSAdmObj != nullptr )

    if( !bEventHandled )
    {
        QTreeView::mouseReleaseEvent(i_pEv);
    }

} // mouseReleaseEvent

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewModelTest::onHeaderCustomContextMenuRequested( const QPoint& i_pt )
//------------------------------------------------------------------------------
{
    QPoint ptGlobal = mapToGlobal(i_pt);

    QMenu*   pMenuHeaderView = new QMenu("", this);
    QAction* pAction;
    QString  strClmName;
    bool     bClmVisible;
    int      idxClm;

    for( idxClm = 0; idxClm < CTestStepAdminObjPoolModel::EColumnCount; idxClm++ )
    {
        bClmVisible = !isColumnHidden(idxClm);

        if( idxClm != CTestStepAdminObjPoolModel::EColumnName )
        {
            strClmName = CTestStepAdminObjPoolModel::column2Str(idxClm);
            pAction = new QAction(strClmName, this);
            pAction->setCheckable(true);
            pAction->setChecked(bClmVisible);
            pAction->setData(idxClm);
            pMenuHeaderView->addAction(pAction);
        }
    } // for( idxClm = 0; idxClm < CTestStepAdminObjPoolModel::EColumnCount; idxClm++ )

    if( !isColumnHidden(CTestStepAdminObjPoolModel::EColumnDesiredValues)
     || !isColumnHidden(CTestStepAdminObjPoolModel::EColumnActualValues) )
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

        if( m_pTestStepsModel->isSetShowDesiredAndActualValuesOnlyIfTestStepFailed() )
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

        if( idxClm >= 0 && idxClm < CTestStepAdminObjPoolModel::EColumnCount )
        {
            setColumnHidden(idxClm, !pActSelected->isChecked());
        }
        else if( idxClm == -1 )
        {
            m_pTestStepsModel->setShowDesiredAndActualValuesOnlyIfTestStepFailed(false);
        }
        else if( idxClm == -2 )
        {
            m_pTestStepsModel->setShowDesiredAndActualValuesOnlyIfTestStepFailed(true);
        }
    }

} // onHeaderCustomContextMenuRequested

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTreeViewModelTest::onActionEnableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeEntryGroupContextAction != nullptr && m_pTestStepsModel != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::Yes );
    }

} // onActionEnableTestGroupsTriggered

//------------------------------------------------------------------------------
void CTreeViewModelTest::onActionDisableTestGroupsTriggered( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeEntryGroupContextAction != nullptr && m_pTestStepsModel != nullptr )
    {
        //m_pTestStepsModel->updateGroup(
        //    /* pTreeEntry */ m_pTreeEntryGroupContextAction,
        //    /* enabled    */ EEnabled::No );
    }

} // onActionDisableTestGroupsTriggered


/*******************************************************************************
class CWdgtTest : public QWidget
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CWdgtTest::CWdgtTest(
    CTest*          i_pTest,
    //bool            i_bUseContentWidget,
    QWidget*        i_pWdgtParent,
    Qt::WindowFlags i_wflags ) :
//------------------------------------------------------------------------------
    QWidget(i_pWdgtParent,i_wflags),
    m_pTest(i_pTest),
    m_pReqInProgress(nullptr),
    m_pLytMain(nullptr),
    m_pLytHeadLine(nullptr),
    m_pBtnStart(nullptr),
    m_pBtnStep(nullptr),
    m_pBtnPause(nullptr),
    m_pBtnStop(nullptr),
    m_pBtnTreeViewResizeRowsAndColumnsToContents(nullptr),
    m_pBtnTreeViewExpandAll(nullptr),
    m_pBtnTreeViewCollapseAll(nullptr),
    m_pLblTestStepCurr(nullptr),
    m_pEdtTestStepCurr(nullptr),
    m_pTestStepsModel(nullptr),
    m_pTreeViewTestSteps(nullptr),
    m_pTreeEntrySelected(nullptr)
{
    setObjectName( "WdgtTest" + m_pTest->objectName() );

    m_pLytMain = new QVBoxLayout;
    setLayout(m_pLytMain);

    // <Headline> with start/stop/pause buttons
    //=========================================

    QSize szBtn(24, 24);

    m_pLytHeadLine = new QHBoxLayout();
    m_pLytMain->addLayout(m_pLytHeadLine);

    // <Button> Start
    //---------------

    QPixmap pxmStart(":/ZS/Button/ButtonCmdStart16x16.bmp");
    pxmStart.setMask(pxmStart.createHeuristicMask());

    m_pBtnStart = new QPushButton();
    m_pBtnStart->setIcon(pxmStart);
    m_pBtnStart->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnStart);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStart,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStartClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Step
    //---------------

    QPixmap pxmStep(":/ZS/Button/ButtonCmdStepForward16x16.bmp");
    pxmStep.setMask(pxmStep.createHeuristicMask());

    m_pBtnStep = new QPushButton();
    m_pBtnStep->setIcon(pxmStep);
    m_pBtnStep->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnStep);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStep,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStepClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Pause
    //---------------

    QPixmap pxmPause(":/ZS/Button/ButtonCmdPause16x16.bmp");
    pxmPause.setMask(pxmPause.createHeuristicMask());

    m_pBtnPause = new QPushButton();
    m_pBtnPause->setIcon(pxmPause);
    m_pBtnPause->setFixedSize(szBtn);
    m_pBtnPause->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnPause);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnPause,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnPauseClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Stop
    //---------------

    QPixmap pxmStop(":/ZS/Button/ButtonCmdStop16x16.bmp");
    pxmStop.setMask(pxmStop.createHeuristicMask());

    m_pBtnStop = new QPushButton();
    m_pBtnStop->setIcon(pxmStop);
    m_pBtnStop->setFixedSize(szBtn);
    m_pBtnStop->setEnabled(false);
    m_pLytHeadLine->addWidget(m_pBtnStop);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnStop,
        /* szSignal     */ SIGNAL( clicked(bool) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onBtnStopClicked(bool) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(30);

    // <Button> Resize Columns To Contents
    //------------------------------------

    QPixmap pxmResizeToContents(":/ZS/TreeView/TreeViewResizeToContents.bmp");
    pxmResizeToContents.setMask(pxmResizeToContents.createHeuristicMask());

    m_pBtnTreeViewResizeRowsAndColumnsToContents = new QPushButton();
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setIcon(pxmResizeToContents);
    m_pBtnTreeViewResizeRowsAndColumnsToContents->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewResizeRowsAndColumnsToContents);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewResizeRowsAndColumnsToContents,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewResizeRowsAndColumnsToContentsClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Expand All
    //--------------------

    QPixmap pxmExpandAll(":/ZS/TreeView/TreeViewExpandAll.bmp");
    pxmExpandAll.setMask(pxmExpandAll.createHeuristicMask());

    m_pBtnTreeViewExpandAll = new QPushButton();
    m_pBtnTreeViewExpandAll->setIcon(pxmExpandAll);
    m_pBtnTreeViewExpandAll->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewExpandAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewExpandAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewExpandAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <Button> Collapse All
    //----------------------

    QPixmap pxmCollapseAll(":/ZS/TreeView/TreeViewCollapseAll.bmp");
    pxmCollapseAll.setMask(pxmCollapseAll.createHeuristicMask());

    m_pBtnTreeViewCollapseAll = new QPushButton();
    m_pBtnTreeViewCollapseAll->setIcon(pxmCollapseAll);
    m_pBtnTreeViewCollapseAll->setFixedSize(szBtn);
    m_pLytHeadLine->addWidget(m_pBtnTreeViewCollapseAll);

    if( !QObject::connect(
        /* pObjSender   */ m_pBtnTreeViewCollapseAll,
        /* szSignal     */ SIGNAL(clicked(bool)),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT(onBtnTreeViewCollapseAllClicked(bool)) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    m_pLytHeadLine->addSpacing(10);

    // <EditField> Current Test Step
    //------------------------------------

    m_pLblTestStepCurr = new QLabel("Current Test Step:");
    m_pEdtTestStepCurr = new QLineEdit("---",this);
    m_pEdtTestStepCurr->setEnabled(false);
    //m_pEdtTestStepCurr->setMaximumWidth(60);
    m_pLytHeadLine->addWidget(m_pLblTestStepCurr);
    m_pLytHeadLine->addWidget(m_pEdtTestStepCurr);

    // <Stretch> at EOL
    //-----------------

    //m_pLytHeadLine->addStretch();

    //// <Line> Test Steps Summary
    ////==========================

    //m_pLytLineTestStepsSummary = new QHBoxLayout();
    //m_pLytMain->addLayout(m_pLytLineTestStepsSummary);

    //// Test step interval
    ////-------------------

    //m_pLblTestStepIntervalInMs = new QLabel("Test Step Interval / ms:");
    //m_pEdtTestStepIntervalInMs = new QLineEdit( QString::number(m_pTest->getTestStepIntervalInMs()) );
    //m_pEdtTestStepIntervalInMs->setValidator( new QDoubleValidator(0.0,60000.0,3,m_pEdtTestStepIntervalInMs) );
    //m_pEdtTestStepIntervalInMs->setEnabled(false);
    //m_pEdtTestStepIntervalInMs->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepIntervalInMs);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepIntervalInMs);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //if( !QObject::connect(
    //    /* pObjSender   */ m_pEdtTestStepIntervalInMs,
    //    /* szSignal     */ SIGNAL( textChanged(const QString&) ),
    //    /* pObjReceiver */ this,
    //    /* szSlot       */ SLOT( onEdtTestStepIntervalTextChanged(const QString&) ) ) )
    //{
    //    throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    //}

    //// Number of tests
    ////-----------------------

    //m_pLblTestStepsCount = new QLabel("Number Of Tests:");
    //m_pEdtTestStepsCount = new QLineEdit("0");
    //m_pEdtTestStepsCount->setEnabled(false);
    //m_pEdtTestStepsCount->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsCount);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsCount);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of executed tests
    ////-------------------------

    //m_pLblTestStepsExecuted = new QLabel("Executed:");
    //m_pEdtTestStepsExecuted = new QLineEdit("0");
    //m_pEdtTestStepsExecuted->setEnabled(false);
    //m_pEdtTestStepsExecuted->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsExecuted);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsExecuted);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of passed tests
    ////-----------------------

    //m_pLblTestStepsPassed = new QLabel("Passed:");
    //m_pEdtTestStepsPassed = new QLineEdit("0");
    //m_pEdtTestStepsPassed->setEnabled(false);
    //m_pEdtTestStepsPassed->setMaximumWidth(60);
    //m_pEdtTestStepsPassed->setStyleSheet("QLineEdit { color: green; }");
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsPassed);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsPassed);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of failed tests
    ////-----------------------

    //m_pLblTestStepsFailed = new QLabel("Failed:");
    //m_pEdtTestStepsFailed = new QLineEdit("0",this);
    //m_pEdtTestStepsFailed->setEnabled(false);
    //m_pEdtTestStepsFailed->setMaximumWidth(60);
    //m_pEdtTestStepsFailed->setStyleSheet("QLineEdit { color: green; }");
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsFailed);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsFailed);

    //m_pLytLineTestStepsSummary->addSpacing(10);

    //// Number of tests with ignored result
    ////------------------------------------

    //m_pLblTestStepsIgnored = new QLabel("Ignored:");
    //m_pEdtTestStepsIgnored = new QLineEdit("0",this);
    //m_pEdtTestStepsIgnored->setEnabled(false);
    //m_pEdtTestStepsIgnored->setMaximumWidth(60);
    //m_pLytLineTestStepsSummary->addWidget(m_pLblTestStepsIgnored);
    //m_pLytLineTestStepsSummary->addWidget(m_pEdtTestStepsIgnored);

    //// <Stretch> at EOL
    ////-----------------

    //m_pLytLineTestStepsSummary->addStretch();

    // <TreeView> Test Steps
    //======================

    m_pTestStepsModel = new CTestStepAdminObjPoolModel( m_pTest->getAdminObjIdxTree() );

    m_pTreeViewTestSteps = new CTreeViewModelTest(m_pTestStepsModel,this);
    m_pTreeViewTestSteps->setAlternatingRowColors(true);
    m_pLytMain->addWidget(m_pTreeViewTestSteps);

    // Connect to state changed signal of test
    //----------------------------------------

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( stateChanged(int,int) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onTestStateChanged(int,int) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    if( !QObject::connect(
        /* pObjSender   */ m_pTest,
        /* szSignal     */ SIGNAL( currentTestStepChanged(ZS::Test::CTestStep*) ),
        /* pObjReceiver */ this,
        /* szSlot       */ SLOT( onCurrentTestStepChanged(ZS::Test::CTestStep*) ) ) )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
    }

    // Connect to signals of the test steps object pool model
    //-------------------------------------------------------

    if( m_pTestStepsModel != nullptr )
    {
    }

    // Connect to the signals of the tree view
    //----------------------------------------

    if( m_pTreeViewTestSteps != nullptr )
    {
        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewTestSteps,
            /* szSignal     */ SIGNAL( expanded(const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewExpanded(const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
        if( !QObject::connect(
            /* pObjSender   */ m_pTreeViewTestSteps->selectionModel(),
            /* szSignal     */ SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&) ),
            /* pObjReceiver */ this,
            /* szSlot       */ SLOT( onTreeViewSelectedNodeChanged(const QModelIndex&, const QModelIndex&) ) ) )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultSignalSlotConnectionFailed );
        }
    }

    // <Geometry> Restore splitter position
    //=====================================

    // <TreeView> Resize
    //------------------

    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->resizeColumnToContents(CTestStepAdminObjPoolModel::EColumnName);
    }

} // ctor

//------------------------------------------------------------------------------
CWdgtTest::~CWdgtTest()
//------------------------------------------------------------------------------
{
    QSettings settings;

    try
    {
        delete m_pTestStepsModel;
    }
    catch(...)
    {
    }

    m_pTest = nullptr;
    m_pReqInProgress = nullptr;
    m_pLytMain = nullptr;
    m_pLytHeadLine = nullptr;
    m_pBtnStart = nullptr;
    m_pBtnStep = nullptr;
    m_pBtnPause = nullptr;
    m_pBtnStop = nullptr;
    m_pBtnTreeViewResizeRowsAndColumnsToContents = nullptr;
    m_pBtnTreeViewExpandAll = nullptr;
    m_pBtnTreeViewCollapseAll = nullptr;
    m_pLblTestStepCurr = nullptr;
    m_pEdtTestStepCurr = nullptr;
    m_pTestStepsModel = nullptr;
    m_pTreeViewTestSteps = nullptr;
    m_pTreeEntrySelected = nullptr;

} // dtor

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onBtnStartClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->start();
}

//------------------------------------------------------------------------------
void CWdgtTest::onBtnStepClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->step();
}

//------------------------------------------------------------------------------
void CWdgtTest::onBtnPauseClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->pause();
}

//------------------------------------------------------------------------------
void CWdgtTest::onBtnStopClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    m_pTest->stop();
}

/*==============================================================================
public slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTreeViewResizeRowsAndColumnsToContentsClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        int idxClm;

        for( idxClm = 0; idxClm < CTestStepAdminObjPoolModel::EColumnCount; idxClm++ )
        {
            m_pTreeViewTestSteps->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewResizeRowsAndColumnsToContentsClicked

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTreeViewExpandAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->expandAll();

        for( int idxClm = 0; idxClm < m_pTestStepsModel->columnCount(); idxClm++ )
        {
            m_pTreeViewTestSteps->resizeColumnToContents(idxClm);
        }
    }

} // onBtnTreeViewExpandAllClicked

//------------------------------------------------------------------------------
void CWdgtTest::onBtnTreeViewCollapseAllClicked( bool /*i_bChecked*/ )
//------------------------------------------------------------------------------
{
    if( m_pTreeViewTestSteps != nullptr )
    {
        m_pTreeViewTestSteps->collapseAll();
    }

} // onBtnTreeViewCollapseAllClicked

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onTestStateChanged( int /*i_iStateNew*/, int /*i_iStatePrev*/ )
//------------------------------------------------------------------------------
{
    if( m_pTest->getState() == CTest::EStateIdle )
    {
        m_pBtnStart->setEnabled(true);
        m_pBtnStep->setEnabled(true);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(false);
    }
    else if( m_pTest->getState() == CTest::EStateInit )
    {
        m_pBtnStart->setEnabled(false);
        m_pBtnStep->setEnabled(false);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(false);
    }
    else if( m_pTest->getState() == CTest::EStateRunning )
    {
        m_pBtnStart->setEnabled(false);
        m_pBtnStep->setEnabled(false);
        m_pBtnPause->setEnabled(true);
        m_pBtnStop->setEnabled(true);
    }
    else if( m_pTest->getState() == CTest::EStatePaused )
    {
        m_pBtnStart->setEnabled(true);
        m_pBtnStep->setEnabled(true);
        m_pBtnPause->setEnabled(false);
        m_pBtnStop->setEnabled(true);
    }

} // onTestStateChanged

//------------------------------------------------------------------------------
void CWdgtTest::onCurrentTestStepChanged( CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    if( i_pTestStep == nullptr )
    {
        m_pEdtTestStepCurr->setText("---");
    }
    else // if( i_pTestStep != nullptr )
    {
        m_pEdtTestStepCurr->setText( i_pTestStep->getName(true) );
    }
} // onCurrentTestStepChanged

/*==============================================================================
public slots:
==============================================================================*/

////------------------------------------------------------------------------------
//void CWdgtTest::onTestStepInserted( QObject* /*i_pAdminObjPoolModel*/, QObject* /*i_pTestStep*/ )
////------------------------------------------------------------------------------
//{
//    m_pEdtTestStepsCount->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsCount()) );
//}

////------------------------------------------------------------------------------
//void CWdgtTest::onTestStepChanged( QObject* /*i_pAdminObjPoolModel*/, QObject* /*i_pTestStep*/ )
////------------------------------------------------------------------------------
//{
//    m_pEdtTestStepsExecuted->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsExecutedCount()) );
//    m_pEdtTestStepsPassed->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsPassedCount()) );
//    m_pEdtTestStepsFailed->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsFailedCount()) );
//    m_pEdtTestStepsIgnored->setText( QString::number(m_pTest->getAdminObjIdxTree()->getTestStepsIgnoredCount()) );
//
//} // onTestStepChanged

/*==============================================================================
protected slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CWdgtTest::onModelClearing( QObject* i_pAdminObjPoolModel )
//------------------------------------------------------------------------------
{
    if( i_pAdminObjPoolModel == m_pTestStepsModel )
    {
        m_pTreeEntrySelected = nullptr;
    }

} // onModelClearing

//------------------------------------------------------------------------------
void CWdgtTest::onTreeViewExpanded( const QModelIndex& i_iModelIdx )
//------------------------------------------------------------------------------
{
    if( i_iModelIdx.isValid() )
    {
        #if QT_VERSION >= 0x040000 && QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        m_pTreeViewTestSteps->resizeColumnToContents(i_iModelIdx.column());
        #elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        // Cannot invoke resizeColumnToContents as this leads to an exception.
        #endif
    }

} // onTreeViewExpanded

//------------------------------------------------------------------------------
void CWdgtTest::onTreeViewSelectedNodeChanged(
    const QModelIndex& /*i_modelIdxCurr*/,
    const QModelIndex& /*i_modelIdxPrev*/ )
//------------------------------------------------------------------------------
{
    //CWdgtGroupContent* pWdgtGroupContent = dynamic_cast<CWdgtGroupContent*>(m_pTblViewTestSteps);

    //if( pWdgtGroupContent != nullptr )
    //{
    //    CObjPoolTreeEntry* pTreeEntryCurr = nullptr;
    //    CObjPoolTreeEntry* pTreeEntryPrev = nullptr;

    //    if( i_modelIdxCurr.isValid() )
    //    {
    //        pTreeEntryCurr = static_cast<CObjPoolTreeEntry*>(i_modelIdxCurr.internalPointer());
    //    }
    //    if( i_modelIdxPrev.isValid() )
    //    {
    //        pTreeEntryPrev = static_cast<CObjPoolTreeEntry*>(i_modelIdxPrev.internalPointer());
    //    }

    //    if( pTreeEntryCurr != pTreeEntryPrev )
    //    {
    //        m_pTreeEntrySelected = nullptr;

    //        if( pTreeEntryCurr != nullptr && pTreeEntryCurr->getStyleState() & QStyle::State_Selected )
    //        {
    //            m_pTreeEntrySelected = pTreeEntryCurr;
    //        }
    //        pWdgtGroupContent->setSelectedTreeEntry(m_pTreeEntrySelected);
    //    }
    //}

} // onTreeViewSelectedNodeChanged
