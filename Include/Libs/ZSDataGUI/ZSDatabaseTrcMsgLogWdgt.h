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

#ifndef ZSDataGUI_DatabaseTrcMsgLogWdgt_h
#define ZSDataGUI_DatabaseTrcMsgLogWdgt_h

#include "ZSDataGUI/ZSDataGUIDllMain.h"
#include "ZSIpcGUI/ZSIpcTrcMsgLogWdgt.h"

class QHBoxLayout;
class QVBoxLayout;

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QTimer;

namespace ZS
{
namespace Trace
{
class CTrcAdminObj;
}

namespace Data
{
class CDb;

namespace GUI
{
//******************************************************************************
class ZSDATAGUIDLL_API CWdgtDbTrcMsgLog : public ZS::Ipc::GUI::CWdgtTrcMsgLog
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtDbTrcMsgLog( const ZS::Ipc::GUI::STrcMsgLogSettings& i_settings, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDbTrcMsgLog();
public: // instance methods
    void setDb( CDb* i_pDb );
protected slots:
    virtual void onDbDestroyed( QObject* i_pDb );
protected:
    CDb* m_pDb;

}; // class CWdgtDbTrcMsgLog

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseTrcMsgLogWdgt_h
