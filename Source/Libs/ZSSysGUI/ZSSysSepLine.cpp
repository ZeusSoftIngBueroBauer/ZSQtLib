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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qlayout.h>
#else
#include <QtWidgets/qlayout.h>
#endif

#include "ZSSysGUI/ZSSysSepLine.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::GUI;


/*******************************************************************************
class CSepLine : public QFrame
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CSepLine::CSepLine( int i_cxSpacing, QWidget* i_pWdgtParent ) :
//------------------------------------------------------------------------------
    QFrame(i_pWdgtParent),
    m_pLyt(nullptr)
{
    m_pLyt = new QVBoxLayout;
    m_pLyt->setContentsMargins(0,i_cxSpacing,0,i_cxSpacing);
    setLayout(m_pLyt);

    setFrameShape(QFrame::HLine);
    setFrameShadow(QFrame::Sunken);

} // ctor

//------------------------------------------------------------------------------
CSepLine::~CSepLine()
//------------------------------------------------------------------------------
{
    m_pLyt = nullptr;

} // dtor
