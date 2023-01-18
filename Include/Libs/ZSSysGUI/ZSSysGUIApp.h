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

#ifndef ZSSys_GUIApp_h
#define ZSSys_GUIApp_h

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/qapplication.h>
#else
#include <QtWidgets/qapplication.h>
#endif

#include "ZSSysGUI/ZSSysGUIDllMain.h"

namespace ZS
{
namespace System
{
class CErrLog;

namespace GUI
{
/*******************************************************************************
Application
*******************************************************************************/

// TODO: Define template class CZSGUIApp with inline method "qapplication_notify"

//******************************************************************************
class ZSSYSGUIDLL_API CGUIApp : public QApplication
//******************************************************************************
{
public: // ctors and dtor
    CGUIApp( int& i_argc, char* i_argv[] );
    ~CGUIApp();
public: // overridables of base class QCoreApplication
    virtual bool notify( QObject* i_pObjRcv, QEvent* i_pEv ) override;

}; // class CGUIApp

} // namespace GUI

} // namespace System

} // namespace ZS

#endif // #ifndef ZSSys_GUIApp_h
