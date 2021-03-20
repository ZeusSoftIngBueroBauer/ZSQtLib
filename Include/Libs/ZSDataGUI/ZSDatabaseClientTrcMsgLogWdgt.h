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

#ifndef ZSDataGUI_DatabaseClientTrcMsgLogWdgt_h
#define ZSDataGUI_DatabaseClientTrcMsgLogWdgt_h

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
class CDbClient;

namespace GUI
{
//******************************************************************************
class ZSDATAGUIDLL_API CWdgtDbClientTrcMsgLog : public ZS::Ipc::GUI::CWdgtTrcMsgLog
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CWdgtDbClientTrcMsgLog( const ZS::Ipc::GUI::STrcMsgLogSettings& i_settings, QWidget* i_pWdgtParent = nullptr );
    virtual ~CWdgtDbClientTrcMsgLog();
public: // instance methods
    void setDbClient( CDbClient* i_pDbClt );
protected slots:
    virtual void onDbClientDestroyed( QObject* i_pDbClt );
protected:
    CDbClient* m_pDbClt;

}; // class CWdgtDbClientTrcMsgLog

} // namespace GUI

} // namespace Data

} // namespace ZS

#endif // #ifndef ZSDataGUI_DatabaseClientTrcMsgLogWdgt_h
