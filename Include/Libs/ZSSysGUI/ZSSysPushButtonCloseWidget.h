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

#ifndef ZSSysGUI_PushButtonCloseWidget_h
#define ZSSysGUI_PushButtonCloseWidget_h

#include <QtCore/qstring.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qpushbutton.h>
#else
#include <QtWidgets/qpushbutton.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

class QLabel;

namespace ZS
{
namespace System
{
namespace GUI
{
//******************************************************************************
class ZSSYSGUIDLL_API CPushButtonCloseWidget : public QPushButton
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
    enum ESize {
        ESize16x16,
        ESize24x24,
        ESizeCount,
        ESizeUndefined
    };
    static QString size2Str( int i_size );
public: // ctors and dtor
    CPushButtonCloseWidget( ESize i_size = ESize16x16, QWidget* i_pWdgtParent = nullptr );
    virtual ~CPushButtonCloseWidget();

}; // class CPushButtonCloseWidget

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSysGUI_PushButtonCloseWidget_h
