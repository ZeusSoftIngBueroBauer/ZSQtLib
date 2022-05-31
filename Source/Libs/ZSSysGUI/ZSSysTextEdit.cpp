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

#include "ZSSysGUI/ZSSysTextEdit.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CTextEdit : public QTextEdit
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTextEdit::CTextEdit( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTextEdit(i_pWdgtParent),
    m_cxPreferredWidth(100),
    m_cyPreferredHeight(50)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//------------------------------------------------------------------------------
CTextEdit::CTextEdit( const QString& i_strText, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QTextEdit(i_strText, i_pWdgtParent),
    m_cxPreferredWidth(100),
    m_cyPreferredHeight(50)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//------------------------------------------------------------------------------
CTextEdit::~CTextEdit()
//------------------------------------------------------------------------------
{
    m_cxPreferredWidth = 0;
    m_cyPreferredHeight = 0;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTextEdit::setPreferredWidth(int i_cxWidth)
//------------------------------------------------------------------------------
{
    m_cxPreferredWidth = i_cxWidth;
}

//------------------------------------------------------------------------------
int CTextEdit::preferredWidth() const
//------------------------------------------------------------------------------
{
    return m_cxPreferredWidth;
}

//------------------------------------------------------------------------------
void CTextEdit::setPreferredHeight(int i_cyHeight)
//------------------------------------------------------------------------------
{
    m_cyPreferredHeight = i_cyHeight;
}

//------------------------------------------------------------------------------
int CTextEdit::preferredHeight() const
//------------------------------------------------------------------------------
{
    return m_cyPreferredHeight;
}

/*==============================================================================
public: // overridables of base class QTextEdit
==============================================================================*/

//------------------------------------------------------------------------------
QSize CTextEdit::sizeHint () const
//------------------------------------------------------------------------------
{
    QSize size(document()->size().toSize());

    /*
    * Make sure width and height have 'usable' values.
    */
    size.rwidth() = std::max(m_cxPreferredWidth, size.width());
    size.rheight() = std::max(m_cyPreferredHeight, size.height());
    return(size);
}

/*==============================================================================
protected: // overridables of base class QTextEdit
==============================================================================*/

//------------------------------------------------------------------------------
void CTextEdit::resizeEvent (QResizeEvent* i_pEv)
//------------------------------------------------------------------------------
{
    /*
    * If the widget has been resized then the size hint will
    * also have changed. Call updateGeometry to make sure
    * any layouts are notified of the change.
    */
    QTextEdit::updateGeometry();

    QTextEdit::resizeEvent(i_pEv);
}

