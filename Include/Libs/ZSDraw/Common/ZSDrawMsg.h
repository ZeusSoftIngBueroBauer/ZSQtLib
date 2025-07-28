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

#ifndef ZSDraw_Msg_h
#define ZSDraw_Msg_h

#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSSys/ZSSysMsg.h"

#include <QtCore/qstring.h>


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace Draw
{
enum EMsgType {
    EMsgTypeMin               = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqDeleteGraphobj = (EMsgTypeMin +  0),
    EMsgTypeMax               = (EMsgTypeMin +  0),
    EMsgTypeCount             = (EMsgTypeMax-EMsgTypeMin+1),
    EMsgTypeUndefined
};

ZSDRAWDLL_API QString msgType2Str(int i_iMsgType);

//******************************************************************************
class ZSDRAWDLL_API CMsgReqDeleteGraphObj : public ZS::System::CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqDeleteGraphObj(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        const QString& i_strKeyInTree);
    virtual ~CMsgReqDeleteGraphObj();
public: // instance methods
    QString keyInTree() const;
public: // must overridables of base class CMsg
    ZS::System::CMsg* clone(ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership) override;
    ZS::System::CMsgCon* createConfirmationMessage(ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership);
public: // overridables of base class CMsg
    QString msgTypeToStr() const override;
    QString getAddTrcInfoStr(int i_iDetailLevel = 0, ZS::System::EContentToStrFormat i_format = ZS::System::EContentToStrFormat::PlainText) override;
protected: // instance members
    QString m_strKeyInTree;
};

} // namespace Draw

} // namespace ZS

#endif // ZSDraw_Msg_h
