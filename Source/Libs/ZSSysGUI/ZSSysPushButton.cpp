/*******************************************************************************

Copyright 2004 - 2012 by    ZeusSoft, Ing. Buero Bauer
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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#else
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#endif

#include "ZSSysGUI/ZSSysPushButton.h"
#include "ZSSys/ZSSysErrResult.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CPushButton : public QPushButton
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPushButton::CPushButton( QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QPushButton(i_pWdgtParent),
    m_pLabel(nullptr)
{
    QVBoxLayout* pLytMain = new QVBoxLayout;

    #if QT_VERSION >= 0x040500
    pLytMain->setContentsMargins(4,4,4,4);
    #endif

    setLayout(pLytMain);

    m_pLabel = new QLabel();
    pLytMain->addWidget(m_pLabel);

} // ctor

//------------------------------------------------------------------------------
CPushButton::CPushButton( const QString& i_strText, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QPushButton(i_pWdgtParent),
    m_pLabel(nullptr)
{
    QVBoxLayout* pLytMain = new QVBoxLayout;

    #if QT_VERSION >= 0x040500
    pLytMain->setContentsMargins(4,4,4,4);
    #endif

    setLayout(pLytMain);

    m_pLabel = new QLabel(i_strText);
    pLytMain->addWidget(m_pLabel);

} // ctor

//------------------------------------------------------------------------------
CPushButton::~CPushButton()
//------------------------------------------------------------------------------
{
    m_pLabel = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPushButton::setText( const QString& i_strText )
//------------------------------------------------------------------------------
{
    m_pLabel->setText(i_strText);

} // setText

//------------------------------------------------------------------------------
QString CPushButton::text() const
//------------------------------------------------------------------------------
{
    return m_pLabel->text();
}

//------------------------------------------------------------------------------
void CPushButton::setPixmap( const QPixmap& i_pxm )
//------------------------------------------------------------------------------
{
    m_pLabel->setPixmap(i_pxm);

} // setPixmap

//------------------------------------------------------------------------------
const QPixmap* CPushButton::pixmap() const
//------------------------------------------------------------------------------
{
    return m_pLabel->pixmap();
}

//------------------------------------------------------------------------------
void CPushButton::setScaledContents ( bool i_bScaled )
//------------------------------------------------------------------------------
{
    m_pLabel->setScaledContents(i_bScaled);
}

//------------------------------------------------------------------------------
bool CPushButton::hasScaledContents () const
//------------------------------------------------------------------------------
{
    return m_pLabel->hasScaledContents();
}

/*==============================================================================
public: // overridables of base class QWidget
==============================================================================*/

//------------------------------------------------------------------------------
QSize CPushButton::sizeHint() const
//------------------------------------------------------------------------------
{
    QSize size;
    int   iLeft;
    int   iTop;
    int   iRight;
    int   iBottom;

    size = m_pLabel->sizeHint();

    #if QT_VERSION >= 0x040600
    QMargins margins = contentsMargins();
    iLeft = margins.left();
    iTop = margins.top();
    iRight = margins.right();
    iBottom = margins.bottom();
    #else
    layout()->getContentsMargins(&iLeft,&iTop,&iRight,&iBottom);
    #endif

    size.setWidth( size.width() + iLeft + iRight );
    size.setHeight( size.height() + iTop + iBottom );

    return size;

} // sizeHint
