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

#include "ZSSys/ZSSysMsgProtocol.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::MsgProtocol;


/*******************************************************************************
Auxiliary Methods
*******************************************************************************/

//------------------------------------------------------------------------------
SErrResultInfo ZS::System::MsgProtocol::checkMsgHeader( const ZS::System::MsgProtocol::SMsgHeader* i_pMsgHdr )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo("ZS::System::MsgProtocol", "", "", "checkMsgHeader");

    QString strAddErrInfo;

    if( i_pMsgHdr->m_protocolVersion != ZS::System::MsgProtocol::VERSION )
    {
        strAddErrInfo += "System protocol version received is '0x" + QString::number(i_pMsgHdr->m_protocolVersion,16) + "'";
        strAddErrInfo += " but current version is '0x" + QString::number(MsgProtocol::VERSION,16) + "'";
        errResultInfo.setSeverity(EResultSeverityError);
        errResultInfo.setResult(EResultInvalidMessageContent);
        errResultInfo.setAddErrInfoDscr(strAddErrInfo);
    }

    if( !errResultInfo.isErrorResult() )
    {
        if( i_pMsgHdr->m_systemMsgType == ZS::System::MsgProtocol::ESystemMsgTypeReq || i_pMsgHdr->m_systemMsgType == ZS::System::MsgProtocol::ESystemMsgTypeInd )
        {
            if( /*i_pMsgHdr->m_resultSeverity < 0 ||*/ i_pMsgHdr->m_resultSeverity >= EResultSeverityCount )
            {
                strAddErrInfo  = "Invalid result severity '0x" + QString::number(i_pMsgHdr->m_resultSeverity,16) + "' received.";
                strAddErrInfo += " Expected severity is 0x00.";

                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInvalidMessageContent);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
            else if( i_pMsgHdr->m_result < 0 || i_pMsgHdr->m_result >= EResultCount )
            {
                strAddErrInfo = "Invalid result '0x" + QString::number(i_pMsgHdr->m_result,16) + "' received.";
                strAddErrInfo += " Expected severity is 0x00.";

                errResultInfo.setSeverity(EResultSeverityError);
                errResultInfo.setResult(EResultInvalidMessageContent);
                errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            }
        }
    }

    if( !errResultInfo.isErrorResult() )
    {
        if( /*i_pMsgHdr->m_iProgress_perCent < 0 ||*/ i_pMsgHdr->m_iProgress_perCent > 100 )
        {
            strAddErrInfo = "Invalid progress '" + QString::number(i_pMsgHdr->m_iProgress_perCent) + "' received.";

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultInvalidMessageContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
        }
    }

    return errResultInfo;

} // checkMsgHeader


/*******************************************************************************
struct STimeStamp
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
STimeStamp::STimeStamp() :
//------------------------------------------------------------------------------
    m_iYear(0),
    m_iMonth(0),
    m_iDay(0),
    m_iHour(0),
    m_iMinute(0),
    m_iSecond(0),
    m_iMilliSecond(0)
{
    QDateTime dateTime = QDateTime::currentDateTime().toUTC();

    m_iYear = dateTime.date().year();
    m_iMonth = dateTime.date().month();
    m_iDay = dateTime.date().day();

    m_iHour = dateTime.time().hour();
    m_iMinute = dateTime.time().minute();
    m_iSecond = dateTime.time().second();
    m_iMilliSecond = dateTime.time().msec();

} // default ctor

//------------------------------------------------------------------------------
STimeStamp::STimeStamp(
    int i_iYear,
    int i_iMonth,
    int i_iDay,
    int i_iHour,
    int i_iMinute,
    int i_iSecond,
    int i_iMilliSecond ) :
//------------------------------------------------------------------------------
    m_iYear(i_iYear),
    m_iMonth(i_iMonth),
    m_iDay(i_iDay),
    m_iHour(i_iHour),
    m_iMinute(i_iMinute),
    m_iSecond(i_iSecond),
    m_iMilliSecond(i_iMilliSecond)
{
} // ctor

//------------------------------------------------------------------------------
STimeStamp::STimeStamp( TTimeStampDate i_timeStampDate, TTimeStampTime i_timeStampTime ) :
//------------------------------------------------------------------------------
    m_iYear( (i_timeStampDate & ETimeStampYearBitMask) >> ETimeStampYearBitShift ),
    m_iMonth( (i_timeStampDate & ETimeStampMonthBitMask) >> ETimeStampMonthBitShift ),
    m_iDay( (i_timeStampDate & ETimeStampDayBitMask) >> ETimeStampDayBitShift ),
    m_iHour( (i_timeStampTime & ETimeStampHourBitMask) >> ETimeStampHourBitShift ),
    m_iMinute( (i_timeStampTime & ETimeStampMinuteBitMask) >> ETimeStampMinuteBitShift ),
    m_iSecond( (i_timeStampTime & ETimeStampSecondBitMask) >> ETimeStampSecondBitShift ),
    m_iMilliSecond( (i_timeStampTime & ETimeStampMSecBitMask) >> ETimeStampMSecBitShift )
{
} // ctor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
TTimeStampDate STimeStamp::toTimeStampDate() const
//------------------------------------------------------------------------------
{
    TTimeStampDate timeStampDate = 0;

    timeStampDate |= (m_iYear  << ETimeStampYearBitShift);
    timeStampDate |= (m_iMonth << ETimeStampMonthBitShift);
    timeStampDate |= (m_iDay   << ETimeStampDayBitShift);

    return timeStampDate;
}

//------------------------------------------------------------------------------
TTimeStampTime STimeStamp::toTimeStampTime() const
//------------------------------------------------------------------------------
{
    TTimeStampTime timeStampTime = 0;

    timeStampTime |= (m_iHour        << ETimeStampHourBitShift);
    timeStampTime |= (m_iMinute      << ETimeStampMinuteBitShift);
    timeStampTime |= (m_iSecond      << ETimeStampSecondBitShift);
    timeStampTime |= (m_iMilliSecond << ETimeStampMSecBitShift);

    return timeStampTime;
}

//------------------------------------------------------------------------------
QDateTime STimeStamp::toDateTime() const
//------------------------------------------------------------------------------
{
    return QDateTime(
        /* date     */ QDate(m_iYear,m_iMonth,m_iDay),
        /* time     */ QTime(m_iHour,m_iMinute,m_iSecond,m_iMilliSecond),
        /* timeSpec */ Qt::UTC );
}
