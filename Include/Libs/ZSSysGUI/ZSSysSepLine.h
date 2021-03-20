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

#ifndef ZSSysGUI_SepLine_h
#define ZSSysGUI_SepLine_h

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qframe.h>
#else
#include <QtWidgets/qframe.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QHBoxLayout;
class QVBoxLayout;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CSepLine : public QFrame
//******************************************************************************
{
public: // ctors and dtor
    CSepLine( int i_cxSpacing = 5, QWidget* i_pWdgtParent = nullptr );
    virtual ~CSepLine();
protected: // instance members
    QVBoxLayout* m_pLyt;

}; // class CSepLine

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_SepLine_h
