/*******************************************************************************

Copyright 2004 - 2009 by    ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDiagramMsg_h
#define ZSDiagramMsg_h

#include <QtCore/qstring.h>

#include "ZSDiagram/ZSDiagramDllMain.h"
#include "ZSDiagram/ZSDiagramAux.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "ZSSys/ZSSysMsg.h"
#include "ZSSys/ZSSysErrResult.h"


namespace ZS
{
namespace Diagram
{
typedef enum
{
    EMsgTypeMin         = (ZS::System::EBaseMsgTypeMax+1),
    EMsgTypeReqShow     = (EMsgTypeMin+0),
    EMsgTypeConShow     = (EMsgTypeMin+1),
    EMsgTypeReqHide     = (EMsgTypeMin+2),
    EMsgTypeConHide     = (EMsgTypeMin+3),
    EMsgTypeReqSetValue = (EMsgTypeMin+4),
    EMsgTypeConSetValue = (EMsgTypeMin+5),
    EMsgTypeMax         = (EMsgTypeMin+5),
    EMsgTypeCount       = (EMsgTypeMax-EMsgTypeMin+1)
}   EMsgType;

typedef enum
{
    EMsgValTypeNone     = 0x00,
    EMsgValTypeInt      = 0x01,
    EMsgValTypeDouble   = 0x02,
    EMsgValTypeString   = 0x04,
    EMsgValTypePhysVal  = 0x08,
    EMsgValTypeCount,
    EMsgValTypeUndefined
}   EMsgValType;

typedef quint8 TMsgValType;

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqShow : public ZS::System::CMsgReq
/* Requests an object to become visible.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgReqShow(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqShow();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );

}; // class CMsgReqShow

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgConShow : public ZS::System::CMsgCon
/* Confirmation message for the show request message.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgConShow(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ErrResultSuccess );
    virtual ~CMsgConShow();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );

}; // class CMsgConShow

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqHide : public ZS::System::CMsgReq
/* Requests an object to become visible.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgReqHide(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    virtual ~CMsgReqHide();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );

}; // class CMsgReqHide

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgConHide : public ZS::System::CMsgCon
/* Confirmation message for the show request message.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgConHide(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ErrResultSuccess );
    virtual ~CMsgConHide();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );

}; // class CMsgConHide

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgReqSetValue : public ZS::System::CMsgReq
/* Requests an object to become visible.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgReqSetValue(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSetValue(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        int      i_iVal,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSetValue(
        QObject* i_pObjSender,
        QObject* i_pObjReceiver,
        double   i_fVal,
        bool     i_bMustBeConfirmed = false,
        qint64   i_iParentReqId = -1,
        qint64   i_iMsgId = -1 );
    CMsgReqSetValue(
        QObject*       i_pObjSender,
        QObject*       i_pObjReceiver,
        const QString& i_strVal,
        bool           i_bMustBeConfirmed = false,
        qint64         i_iParentReqId = -1,
        qint64         i_iMsgId = -1 );
    CMsgReqSetValue(
        QObject*                 i_pObjSender,
        QObject*                 i_pObjReceiver,
        const PhysVal::CPhysVal& i_physVal,
        bool                     i_bMustBeConfirmed = false,
        qint64                   i_iParentReqId = -1,
        qint64                   i_iMsgId = -1 );
    virtual ~CMsgReqSetValue();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
    virtual ZS::System::CMsgCon* createConfirmationMessage( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );
public: // instance methods
    bool isValid( EMsgValType i_valType ) const;
    void setVal( int i_iVal );
    int getValInt() const;
    void setVal( double i_fVal );
    double getValDouble() const;
    void setVal( const QString& i_strVal );
    QString getValStr() const;
    void setVal( const PhysVal::CPhysVal& i_physVal );
    PhysVal::CPhysVal getValPhys() const;
protected: // instance members
    TMsgValType       m_msgValType; // Ored bit field. If the bit is set the value is valid.
    int               m_iVal;
    double            m_fVal;
    QString           m_strVal;
    PhysVal::CPhysVal m_physVal;

}; // class CMsgReqSetValue

//******************************************************************************
class ZSDIAGRAMDLL_API CMsgConSetValue : public ZS::System::CMsgCon
/* Confirmation message for the show request message.
*******************************************************************************/
{
public: // ctors and dtor
    CMsgConSetValue(
        QObject*                          i_pObjSender,
        QObject*                          i_pObjReceiver,
        qint64                            i_iParentReqId,
        qint64                            i_iMsgIdReq,
        const ZS::System::SErrResultInfo& i_errResultInfo = ErrResultSuccess );
    virtual ~CMsgConSetValue();
public: // must overridables of base class ZS::System::CMsg
    virtual ZS::System::CMsg* clone( ZS::System::ECopyDepth i_copyDepth = ZS::System::ECopyDepth::FlatKeepOwnership );

}; // class CMsgConSetValue

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagramMsg_h
