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

--------------------------------------------------------------------------------

$Date: $

$Revision: $

$Author: $

$History: $

*******************************************************************************/

#ifndef ZSSNPMCltStdPCMsg_h
#define ZSSNPMCltStdPCMsg_h

#include "ZSSNMP/Include/ZSSNMPDllMain.h"
#include "ZSSNMP/Include/ZSSNMPCltStdPC.h"
#include "ZSSNMP/Include/ZSSNMPMsg.h"


/*******************************************************************************
global type definitions and constants
*******************************************************************************/

namespace ZS
{
namespace SNMP
{
typedef enum
{
    EMsgTypeCltStdPCMin             = (SNMP::EMsgTypeMax + 1),
    // Message for communication between threads:
    EMsgTypeCltStdPCReqStartSession = (EMsgTypeCltStdPCMin  +  1),
    EMsgTypeCltStdPCMax             = (EMsgTypeCltStdPCMin  +  1),
    EMsgTypeCltStdPCCount           = (EMsgTypeCltStdPCMax-EMsgTypeCltStdPCMin+1),
    EMsgTypeCltStdPCUndefined
}   EMsgTypeCltStdPC;

//******************************************************************************
class ZSSNMPDLL_API CMsgReqCltStdPCStartSession : public CMsgReq
//******************************************************************************
{
public: // ctors and dtor
    CMsgReqCltStdPCStartSession(
        QObject*          i_pObjSender,
        QObject*          i_pObjReceiver,
        const SOidsStdPC& i_oids,
        bool              i_bMustBeConfirmed = true,
        int               i_iMsgId = -1 );
    virtual ~CMsgReqCltStdPCStartSession();
public: // instance methods
    SOidsStdPC getOids() const;
public: // overridables of base class CMsg
    virtual ZS::System::CMsg* clone();
    virtual QString msgTypeToStr() const;
    virtual QString contentToStr( EContentToStrFormat i_format = EContentToStrFormatPlainText );
protected: // instance members
    SOidsStdPC m_oids;

}; // CMsgReqCltStdPCStartSession

} // namespace SNMP

} // namespace ZS

#endif // #ifndef ZSSNPMCltStdPCMsg_h
