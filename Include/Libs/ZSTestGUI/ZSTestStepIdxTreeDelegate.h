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

#ifndef ZSTestGUI_TestStepIdxTreeDelegate_h
#define ZSTestGUI_TestStepIdxTreeDelegate_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qitemdelegate.h>
#else
#include <QtWidgets/qitemdelegate.h>
#endif

#include "ZSTestGUI/ZSTestGUIDllMain.h"

namespace ZS
{
namespace Test
{
namespace GUI
{
//******************************************************************************
class ZSTESTGUIDLL_API CDelegateIdxTreeTestSteps : public QItemDelegate
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Test::GUI"; }
    static QString ClassName() { return "CDelegateIdxTreeTestSteps"; }
public: // ctors and dtor
    CDelegateIdxTreeTestSteps( QObject* i_pObjParent = nullptr );
    virtual ~CDelegateIdxTreeTestSteps();
public: // overridables
    virtual QString nameSpace() const { return NameSpace(); }
    virtual QString className() const { return ClassName(); }
public: // instance methods
    bool isCheckBoxBreakpointHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
    bool isCheckBoxEnabledHit(
        const QRect&       i_rectVisual,
        const QPoint&      i_ptPos,
        const QModelIndex& i_modelIdx );
public: // overridables of base class QItemDelegate
    virtual void paint(
        QPainter*                   i_pPainter,
        const QStyleOptionViewItem& i_styleOption,
        const QModelIndex&          i_modelIdx ) const;
private: // copy ctor not implemented
    CDelegateIdxTreeTestSteps( const CDelegateIdxTreeTestSteps& );
private: // assignment operator not implemented
    CDelegateIdxTreeTestSteps& operator = ( const CDelegateIdxTreeTestSteps& );
protected: // instance members
    mutable QRect m_rectChkBoxBreakpoint;
    mutable QRect m_rectChkBoxEnabled;

}; // class CDelegateIdxTreeTestSteps

} // namespace GUI

} // namespace Test

} // namespace ZS

#endif // #ifndef ZSTestGUI_TestStepIdxTreeDelegate_h
