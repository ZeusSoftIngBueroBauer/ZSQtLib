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

#include "ZSData/ZSDataProtocol.h"
#include "ZSData/ZSDataMsg.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Data::MsgProtocol;


/*******************************************************************************
Auxiliary Methods
*******************************************************************************/

//------------------------------------------------------------------------------
SErrResultInfo ZS::Data::MsgProtocol::checkMsgHeader( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = ZS::System::MsgProtocol::checkMsgHeader(i_pMsgHdr);

    if( !errResultInfo.isErrorResult() )
    {
        QString strAddErrInfo;

        if( i_pMsgHdr->m_msgType < ZS::Data::EMsgTypeMin || i_pMsgHdr->m_msgType >= ZS::Data::EMsgTypeMax )
        {
            strAddErrInfo = "Invalid message type '0x" + QString::number(i_pMsgHdr->m_msgType,16) + "' received.";
            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInvalidMessageContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }

        if( !errResultInfo.isErrorResult() )
        {
            int iSysMsgHeaderSize = sizeof(ZS::System::MsgProtocol::SMsgHeader) + i_pMsgHdr->m_blockLenBuffer;

            const char* pcSysDataBlock = reinterpret_cast<const char*>(i_pMsgHdr) + iSysMsgHeaderSize;

            const ZS::Data::MsgProtocol::SMsgHeader* pDataMsgHdr = reinterpret_cast<const ZS::Data::MsgProtocol::SMsgHeader*>(pcSysDataBlock);

            if( pDataMsgHdr->m_protocolVersion != ZS::Data::MsgProtocol::VERSION )
            {
                strAddErrInfo += "Data protocol version received is '0x" + QString::number(i_pMsgHdr->m_protocolVersion,16) + "'";
                strAddErrInfo += " but current version is '0x" + QString::number(MsgProtocol::VERSION,16) + "'";
                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInvalidMessageContent);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }

            if( !errResultInfo.isErrorResult() )
            {
                if( /*pDataMsgHdr->m_protocolType < 0 ||*/ pDataMsgHdr->m_protocolType >= EProtocolTypeCount )
                {
                    strAddErrInfo = "Invalid data protocol type '0x" + QString::number(pDataMsgHdr->m_protocolType,16) + "' received.";
                    errResultInfo.setSeverity(EResultSeverityError);
                    errResultInfo.setResult(EResultInvalidMessageContent);
                    errResultInfo.setAddErrInfoDscr(strAddErrInfo);
                }
            }

        } // if( !errResultInfo.isErrorResult() )
    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // checkMsgHeader

//------------------------------------------------------------------------------
SErrResultInfo ZS::Data::MsgProtocol::checkMsgContent( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = ZS::Data::MsgProtocol::checkMsgHeader(i_pMsgHdr);

    if( !errResultInfo.isErrorResult() )
    {
        QString strAddErrInfo;

        //const SCmdHeader* pCmdHdr = nullptr;

        //if( !errResultInfo.isErrorResult() )
        //{
        //    if( i_pMsgHdr->m_blockLenData <= 0 || i_pMsgHdr->m_blockLenData < sizeof(SCmdHeader) )
        //    {
        //        strAddErrInfo = "Invalid message data length '" + QString::number(i_pMsgHdr->m_blockLenData) + "' received.";

        //        errResultInfo.setSeverity(EResultSeverityError);
        //        errResultInfo.setResult(EResultInvalidMessageContent);
        //        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        //    }
        //    else
        //    {
        //        const char* pc = reinterpret_cast<const char*>(i_pMsgHdr) + sizeof(SMsgHeader);
        //        pCmdHdr = reinterpret_cast<const SCmdHeader*>(pc);
        //    }
        //}

        //if( !errResultInfo.isErrorResult() && pCmdHdr != nullptr )
        //{
        //    if( pCmdHdr->m_cmdClass < ECmdClass_MIN || pCmdHdr->m_cmdClass > ECmdClass_MAX )
        //    {
        //        strAddErrInfo = "Invalid command class '0x" + QString::number(pCmdHdr->m_cmdClass,16) + "' received.";

        //        errResultInfo.setSeverity(EResultSeverityError);
        //        errResultInfo.setResult(EResultInvalidMessageContent);
        //        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        //    }
        //}

        //if( !errResultInfo.isErrorResult() && pCmdHdr != nullptr )
        //{
        //    if( pCmdHdr->m_cmdType < ECmdType_MIN || pCmdHdr->m_cmdType > ECmdType_MAX )
        //    {
        //        strAddErrInfo = "Invalid command type '0x" + QString::number(pCmdHdr->m_cmdType,16) + "' received.";

        //        errResultInfo.setSeverity(EResultSeverityError);
        //        errResultInfo.setResult(EResultInvalidMessageContent);
        //        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        //    }
        //}
    } // if( !errResultInfo.isErrorResult() )

    return errResultInfo;

} // checkMsgContent
