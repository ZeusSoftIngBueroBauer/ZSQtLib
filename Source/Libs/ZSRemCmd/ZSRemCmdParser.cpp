/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer, Germany
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

#include "ZSRemCmd/ZSRemCmdParser.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Trace;
using namespace ZS::RemCmd;


/*******************************************************************************
class CRemCmdParser
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CRemCmdParser::Parse(
    const QByteArray& i_byteArrData,
    QString*          o_pstrCmd,
    bool*             o_pbIsQuery,
    QByteArray*       o_pByteArrArgs,
    char              i_chCmdSeparator,
    char              /*i_chArgsSeparator*/ )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo(NameSpace(), ClassName(), "", "Parse");

    QString    strCmd;
    bool       bIsQuery = false;
    QByteArray byteArrArgs;
    int        idxCmdEnd = 0;

    /* Examples of strings to be parsed:
     *
     *   0123456789012345678901234 | byteArr.len() | strCmd       | strCmd.len() | byteArrArgs
     *   --------------------------+---------------+--------------+--------------+------------------
     *   SYSTem:ERRor?             |     13        | SYSTem:ERRor |      12      |                  
     *   FREQuency 2000000.0       |     19        | FREQuency    |       9      | 2000000.0        
     *   POW:CORR 2000000.0,   2.4 |     25        | POW:CORR     |       8      | 2000000.0,   2.4 
     *   COMMent "Hello World"     |     21        | COMMent      |       7      | "Hello World"    
    */

    if( !i_byteArrData.isEmpty() )
    {
        idxCmdEnd = i_byteArrData.indexOf(i_chCmdSeparator,0);

        if( idxCmdEnd >= 0 )
        {
            strCmd = i_byteArrData.left(idxCmdEnd);
        }
        else
        {
            strCmd = i_byteArrData;
        }

        if( strCmd.endsWith('?') )
        {
            bIsQuery = true;
            strCmd.remove( strCmd.length()-1, 1 );
        }

        if( idxCmdEnd >= 0 )
        {
            byteArrArgs = i_byteArrData.right(i_byteArrData.size()-(idxCmdEnd+1));
            while( byteArrArgs.startsWith(QByteArray(1,i_chCmdSeparator)) )
            {
                byteArrArgs.remove(0,1);
            }
        }

        //if( !byteArrArgs.isEmpty() )
        //{
        //    if( byteArrArgs.size() >= 2 && byteArrArgs.startsWith('"') && byteArrArgs.endsWith('"') )
        //    {
        //        strArgs.remove(0,1);
        //        strArgs.remove(strArgs.length()-1,1);
        //        strlstArgs.append(strArgs);
        //    }
        //    else if( !strArgs.isEmpty() )
        //    {
        //        strlstArgs = strArgs.split(m_chArgsSeparator);

        //        if( strlstArgs.size() > 0 )
        //        {
        //            for( idxArg = strlstArgs.size()-1; idxArg >= 0; idxArg-- )
        //            {
        //                strArg = strlstArgs[idxArg];

        //                while( strArg.startsWith(' ') )
        //                {
        //                    strArg.remove(0,1);
        //                }
        //                if( strArg.isEmpty() )
        //                {
        //                    strlstArgs.removeAt(idxArg);
        //                }
        //                else if( strArg.size() >= 2 && strArg.startsWith('"') && strArg.endsWith('"') )
        //                {
        //                    strArg.remove(0,1);
        //                    strArg.remove(strArg.length()-1,1);
        //                    strlstArgs[idxArg] = strArg;
        //                }
        //            }
        //        } // if( strlstArgs.size() > 0 )
        //    } // if( !strArgs.isEmpty() )
        //} // if( !strArgs.isEmpty() )

    } // if( !i_byteArrData.isEmpty() )

    if( o_pstrCmd != nullptr )
    {
        *o_pstrCmd = strCmd;
    }
    if( o_pbIsQuery != nullptr )
    {
        *o_pbIsQuery = bIsQuery;
    }
    if( o_pByteArrArgs != nullptr )
    {
        *o_pByteArrArgs = byteArrArgs;
    }

    return errResultInfo;

} // Parse

//------------------------------------------------------------------------------
QByteArray CRemCmdParser::RemoveLeadingAndTrailingWhitespaces( const QByteArray& i_byteArrArg )
//------------------------------------------------------------------------------
{
    return i_byteArrArg.trimmed();
}

//------------------------------------------------------------------------------
QList<QByteArray> CRemCmdParser::RemoveLeadingAndTrailingWhitespaces( const QList<QByteArray>& i_arByteArrArgs )
//------------------------------------------------------------------------------
{
    QList<QByteArray> arByteArrArgs = i_arByteArrArgs;

    for( int idxArg = 0; idxArg < arByteArrArgs.size(); idxArg++ )
    {
        arByteArrArgs[idxArg] = RemoveLeadingAndTrailingWhitespaces(arByteArrArgs[idxArg]);
    }
    return arByteArrArgs;

} // RemoveLeadingAndTrailingWhitespaces

//------------------------------------------------------------------------------
QByteArray CRemCmdParser::NormalizeStringArg( const QByteArray& i_byteArrArg )
//------------------------------------------------------------------------------
{
    QByteArray byteArr = i_byteArrArg;

    if( byteArr.startsWith("\"") && byteArr.endsWith("\"") )
    {
        byteArr.remove(0,1);
        byteArr.remove(byteArr.size()-1,1);
    }
    else if( byteArr.startsWith("'") && byteArr.endsWith("'") )
    {
        byteArr.remove(0,1);
        byteArr.remove(byteArr.size()-1,1);
    }
    return byteArr;

} // NormalizeStringArg

//------------------------------------------------------------------------------
QList<QByteArray> CRemCmdParser::NormalizeStringArg( const QList<QByteArray>& i_arByteArrArgs )
//------------------------------------------------------------------------------
{
    QList<QByteArray> arByteArrArgs = i_arByteArrArgs;

    for( int idxArg = 0; idxArg < arByteArrArgs.size(); idxArg++ )
    {
        arByteArrArgs[idxArg] = NormalizeStringArg(arByteArrArgs[idxArg]);
    }
    return arByteArrArgs;

} // NormalizeStringArg

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CRemCmdParser::CRemCmdParser(
    const QString& i_strObjName,
    char           i_chCmdSeparator,
    char           i_chArgsSeparator ) :
//------------------------------------------------------------------------------
    QObject(),
    m_chCmdSeparator(i_chCmdSeparator),
    m_chArgsSeparator(i_chArgsSeparator),
    m_pTrcAdminObj(nullptr)
{
    QString strObjName = i_strObjName;

    if( strObjName.isEmpty() )
    {
        strObjName = "RemCmdParser";
    }
    setObjectName(strObjName);

    // Create trace admin object
    //--------------------------

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj("ZS::RemCmd", "CRemCmdParser", i_strObjName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
CRemCmdParser::~CRemCmdParser()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    // Destroy instance members
    //-------------------------

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;

    // Invalidate instance members
    //----------------------------

    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CRemCmdParser::setCmdSeparator( char i_chCmdSeparator )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_chCmdSeparator;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setCmdSeparator",
        /* strAddInfo   */ strAddTrcInfo );

    m_chCmdSeparator = i_chCmdSeparator;

} // setCmdSeparator

//------------------------------------------------------------------------------
void CRemCmdParser::setArgsSeparator( char i_chArgsSeparator )
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_chArgsSeparator;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "setArgsSeparator",
        /* strAddInfo   */ strAddTrcInfo );

    m_chArgsSeparator = i_chArgsSeparator;

} // setArgsSeparator

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SErrResultInfo CRemCmdParser::parse(
    const QByteArray& i_byteArrData,
    QString*          o_pstrCmd,
    bool*             o_pbIsQuery,
    QByteArray*       o_pByteArrArgs ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_byteArrData;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "parse",
        /* strAddInfo   */ strAddTrcInfo );

    SErrResultInfo errResultInfo = Parse(
        /* byteArrData     */ i_byteArrData,
        /* pstrCmd         */ o_pstrCmd,
        /* pbIsQuery       */ o_pbIsQuery,
        /* pstrlstArgs     */ o_pByteArrArgs,
        /* chCmdSeparator  */ m_chCmdSeparator,
        /* chArgsSeparator */ m_chArgsSeparator );

    if( mthTracer.isActive(ETraceDetailLevelMethodCalls) )
    {
        if( errResultInfo.isErrorResult() )
        {
            strAddTrcInfo = errResultInfo.getSeverityStr() + " " + errResultInfo.getResultStr() + " " + errResultInfo.getAddErrInfoDscr();
        }
        else
        {
            strAddTrcInfo = "Ok";
        }

        if( o_pstrCmd != nullptr )
        {
            strAddTrcInfo += ", Cmd: " + *o_pstrCmd;
        }
        if( o_pbIsQuery != nullptr )
        {
            strAddTrcInfo += ", IsQuery: " + bool2Str(*o_pbIsQuery);
        }
        if( o_pByteArrArgs != nullptr )
        {
            strAddTrcInfo += ", Args: " + *o_pByteArrArgs;
        }
        mthTracer.setMethodReturn(strAddTrcInfo);
    }

    return errResultInfo;

} // parse

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QList<QByteArray> CRemCmdParser::splitArgs(
    const QByteArray& i_byteArrArgs,
    bool              i_bRemoveLeadingAndTrailingWhiteSpaces,
    bool              i_bNormalizeStringArgs ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strAddTrcInfo = i_byteArrArgs;
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ 1,
        /* strMethod    */ "splitArgs",
        /* strAddInfo   */ strAddTrcInfo );

    QList<QByteArray> arByteArrs = i_byteArrArgs.split(m_chArgsSeparator);
    QByteArray        byteArr;
    int               idxArg;

    for( idxArg = 0; idxArg < arByteArrs.size(); idxArg++ )
    {
        byteArr = arByteArrs[idxArg];

        if( i_bRemoveLeadingAndTrailingWhiteSpaces )
        {
            byteArr = RemoveLeadingAndTrailingWhitespaces(byteArr);
        }
        if( i_bNormalizeStringArgs )
        {
            byteArr = NormalizeStringArg(byteArr);
        }
        arByteArrs[idxArg] = byteArr;
    }

    return arByteArrs;

} // splitArgs
