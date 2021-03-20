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

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSData/ZSDataCommon.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysEnumEntry.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Data;


/*******************************************************************************
public type definitions and constants
*******************************************************************************/

/*==============================================================================
enum ECloneCopyDepth
==============================================================================*/

const SEnumEntry s_arEnumStrCloneCopyDepth[] =
{
    /* 0 */ SEnumEntry( ECloneCopyDepthSchemaObjIdentity, "SchemaObjIdentity", "SchObjId"  ),
    /* 1 */ SEnumEntry( ECloneCopyDepthSchemaPrepareData, "SchemaPreperaData", "SchPrepDt" ),
    /* 2 */ SEnumEntry( ECloneCopyDepthSchemaAll,         "SchemaAll",         "SchAll"    ),
    /* 3 */ SEnumEntry( ECloneCopyDepthDataAll,           "DataAll",           "DatAll"    ),
    /* 4 */ SEnumEntry( ECloneCopyDepthAll,               "All",               "All"       ),
    /* 5 */ SEnumEntry( ECloneCopyDepthCount,             "Count",             "All"       ),
    /* 6 */ SEnumEntry( ECloneCopyDepthUndefined,         "Undefined",         "?"         )
};

//------------------------------------------------------------------------------
QString ZS::Data::cloneCopyDepth2Str( int i_iDepth, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrCloneCopyDepth,_ZSArrLen(s_arEnumStrCloneCopyDepth),i_iDepth,i_alias);
}

//------------------------------------------------------------------------------
ECloneCopyDepth ZS::Data::str2CloneCopyDepth( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    ECloneCopyDepth copyDepth = ECloneCopyDepthUndefined;
    int iDepth = SEnumEntry::str2Enumerator(s_arEnumStrCloneCopyDepth,_ZSArrLen(s_arEnumStrCloneCopyDepth),i_str,i_alias,Qt::CaseInsensitive);
    if( iDepth >= 0 && iDepth < ECloneCopyDepthCount )
    {
        copyDepth = static_cast<ECloneCopyDepth>(iDepth);
    }
    return copyDepth;

} // str2DataSetObjType


/*==============================================================================
enum EDataSetObjType
==============================================================================*/

const SEnumEntry s_arEnumStrDataSetObjType[] =
{   //                                               IdxName,       Symbol, Text,              SCPIShort, SCPILong, Alias6
    //-------------------------------------------------------------------------------------------------------------------
    /*  0 */ SEnumEntry( EDataSetObjTypeDataSet,     "DataSet",     "DS",   "CDataSet"         ),
    /*  1 */ SEnumEntry( EDataSetObjTypeGroup,       "Group",       "Grp",  "CDataSetObjGroup" ),
    /*  2 */ SEnumEntry( EDataSetObjTypeEnumeration, "Enumeration", "Enum", "CDataEnumeration" ),
    /*  3 */ SEnumEntry( EDataSetObjTypeTable,       "Table",       "DT",   "CDataTable"       ),
    /*  4 */ SEnumEntry( EDataSetObjTypeParameter,   "Parameter",   "DP",   "CDataParameter"   ),
    /*  5 */ SEnumEntry( EDataSetObjTypeCount,       "Count",       "All",  "Count"            ),
    /*  6 */ SEnumEntry( EDataSetObjTypeUndefined,   "Undefined",   "?",    "Undefined"        )
};

//------------------------------------------------------------------------------
QString ZS::Data::dataSetObjType2Str( int i_iType, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrDataSetObjType,_ZSArrLen(s_arEnumStrDataSetObjType),i_iType,i_alias);
}

//------------------------------------------------------------------------------
EDataSetObjType ZS::Data::str2DataSetObjType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EDataSetObjType type = EDataSetObjTypeUndefined;
    int iType = SEnumEntry::str2Enumerator(s_arEnumStrDataSetObjType,_ZSArrLen(s_arEnumStrDataSetObjType),i_str,i_alias,Qt::CaseInsensitive);
    if( iType >= 0 && iType < EDataSetObjTypeCount )
    {
        type = static_cast<EDataSetObjType>(iType);
    }
    return type;

} // str2DataSetObjType


/*==============================================================================
enum EProtocolType
==============================================================================*/

const SEnumEntry s_arEnumStrProtocolTypes[] =
{
    /*  0 */ SEnumEntry( EProtocolTypeZSXML,     "ZSXML",     "ZSXML" ),
    /*  1 */ SEnumEntry( EProtocolTypeZSMsg,     "ZSMsg",     "ZSMsg" ),
    /*  2 */ SEnumEntry( EProtocolTypeSQL,       "SQL",       "SQL"   ),
    /*  3 */ SEnumEntry( EProtocolTypeCount,     "Count",     "All"   ),
    /*  4 */ SEnumEntry( EProtocolTypeUndefined, "Undefined", "?"     )
};

//------------------------------------------------------------------------------
QString ZS::Data::protocolType2Str( int i_iType, EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    return SEnumEntry::enumerator2Str(s_arEnumStrProtocolTypes,_ZSArrLen(s_arEnumStrProtocolTypes),i_iType,i_alias);
}

//------------------------------------------------------------------------------
EProtocolType ZS::Data::str2ProtocolType( const QString& i_str, ZS::System::EEnumEntryAliasStr i_alias )
//------------------------------------------------------------------------------
{
    EProtocolType type = EProtocolTypeUndefined;
    int iType = SEnumEntry::str2Enumerator(s_arEnumStrProtocolTypes,_ZSArrLen(s_arEnumStrProtocolTypes),i_str,i_alias,Qt::CaseInsensitive);
    if( iType >= 0 && iType < EProtocolTypeCount )
    {
        type = static_cast<EProtocolType>(iType);
    }
    return type;

} // str2ProtocolType


/*******************************************************************************
struct SCnctId
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SCnctId::SCnctId( EProtocolType i_protocolType ) :
//------------------------------------------------------------------------------
    m_protocolType(i_protocolType),
    m_socketType(ZS::Ipc::ESocketTypeUndefined),
    m_iSocketId(-1),
    m_pObjDbClt(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
SCnctId::SCnctId( EProtocolType i_protocolType, ZS::Ipc::ESocketType i_socketType ) :
//------------------------------------------------------------------------------
    m_protocolType(i_protocolType),
    m_socketType(i_socketType),
    m_iSocketId(-1),
    m_pObjDbClt(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
SCnctId::SCnctId(
    EProtocolType        i_protocolType,
    ZS::Ipc::ESocketType i_socketType,
    int                  i_iSocketId ) :
//------------------------------------------------------------------------------
    m_protocolType(i_protocolType),
    m_socketType(i_socketType),
    m_iSocketId(i_iSocketId),
    m_pObjDbClt(nullptr)
{
} // ctor

//------------------------------------------------------------------------------
SCnctId::SCnctId(
    EProtocolType        i_protocolType,
    ZS::Ipc::ESocketType i_socketType,
    QObject*             i_pObjDbClt ) :
//------------------------------------------------------------------------------
    m_protocolType(i_protocolType),
    m_socketType(i_socketType),
    m_iSocketId(-1),
    m_pObjDbClt(i_pObjDbClt)
{
} // ctor

////------------------------------------------------------------------------------
//SCnctId::SCnctId( const QString& i_strKeyCnct ) :
////------------------------------------------------------------------------------
//    m_protocolType(EProtocolTypeUndefined),
//    m_socketType(ZS::Ipc::ESocketTypeUndefined),
//    m_iSocketId(-1),
//    m_pObjDbClt(nullptr)
//{
//    QStringList strlstKeyCnct = i_strKeyCnct.split(":");
//
//    if( strlstKeyCnct.size() >= 1 )
//    {
//        m_protocolType = str2ProtocolType(strlstKeyCnct[0]);
//    }
//
//    if( strlstKeyCnct.size() >= 2 )
//    {
//        m_socketType = ZS::Ipc::str2SocketType(strlstKeyCnct[1]);
//    }
//
//    if( strlstKeyCnct.size() >= 3 )
//    {
//        bool bConverted = false;
//        int iSocketId = strlstKeyCnct[2].toInt(&bConverted);
//        if( bConverted )
//        {
//            m_iSocketId = iSocketId;
//        }
//    }
//
//} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SCnctId::toString() const
//------------------------------------------------------------------------------
{
    QString str;

    str += protocolType2Str(m_protocolType);
    str += ":" + ZS::Ipc::socketType2Str(m_socketType);

    if( m_socketType == ZS::Ipc::ESocketTypeTcp )
    {
        str += ":" + QString::number(m_iSocketId);
    }
    else if( m_socketType == ZS::Ipc::ESocketTypeShm )
    {
        str += ":" + QString::number(m_iSocketId);
    }
    else if( m_socketType == ZS::Ipc::ESocketTypeInProcMsg )
    {
        str += ":" + QString( m_pObjDbClt == nullptr ? "nullptr" : m_pObjDbClt->objectName() );
    }
    return str;

} // toString


/*******************************************************************************
struct SCnctType
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SCnctType::SCnctType() :
//------------------------------------------------------------------------------
    m_protocolType(EProtocolTypeUndefined),
    m_socketType(ZS::Ipc::ESocketTypeUndefined)
{
} // ctor

//------------------------------------------------------------------------------
SCnctType::SCnctType(
    EProtocolType        i_protocolType,
    ZS::Ipc::ESocketType i_socketType ) :
//------------------------------------------------------------------------------
    m_protocolType(i_protocolType),
    m_socketType(i_socketType)
{
} // ctor

//------------------------------------------------------------------------------
SCnctType::SCnctType( const SCnctId& i_cnctId ) :
//------------------------------------------------------------------------------
    m_protocolType(i_cnctId.m_protocolType),
    m_socketType(i_cnctId.m_socketType)
{
} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SCnctType::toString() const
//------------------------------------------------------------------------------
{
    QString strKeyCnct;

    strKeyCnct += protocolType2Str(m_protocolType);
    strKeyCnct += ":" + ZS::Ipc::socketType2Str(m_socketType);

    return strKeyCnct;

} // toString


/*******************************************************************************
struct SDSNodeSpec
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SDSNodeSpec::SDSNodeSpec() :
//------------------------------------------------------------------------------
    m_dsObjType(EDataSetObjTypeUndefined),
    m_strObjKey(),
    m_strlstChildObjNames(),
    //m_arRangeInfos(),
    m_strWhere(),
    m_bRecursive(false)
{
} // ctor

//------------------------------------------------------------------------------
SDSNodeSpec::SDSNodeSpec(
    EDataSetObjType i_dsObjType,
    const QString&  i_strObjKey,
    bool            i_bRecursive ) :
//------------------------------------------------------------------------------
    m_dsObjType(i_dsObjType),
    m_strObjKey(i_strObjKey),
    m_strlstChildObjNames(),
    //m_arRangeInfos(),
    m_strWhere(),
    m_bRecursive(i_bRecursive)
{
} // ctor

//------------------------------------------------------------------------------
SDSNodeSpec::SDSNodeSpec( const SDSNodeSpec& i_other ) :
//------------------------------------------------------------------------------
    m_dsObjType(i_other.m_dsObjType),
    m_strObjKey(i_other.m_strObjKey),
    m_strlstChildObjNames(i_other.m_strlstChildObjNames),
    //m_arRangeInfos(i_other.m_arRangeInfos),
    m_strWhere(i_other.m_strWhere),
    m_bRecursive(i_other.m_bRecursive)
{
} // ctor

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SDSNodeSpec::clear()
//------------------------------------------------------------------------------
{
    m_dsObjType = EDataSetObjTypeUndefined;
    m_strObjKey = "";
    m_strlstChildObjNames.clear();
    //m_arRangeInfos.clear();
    m_strWhere = "";
    m_bRecursive = false;

} // clear

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SDSNodeSpec::isValid() const
//------------------------------------------------------------------------------
{
    bool bIsValid = true;

    if( m_dsObjType == EDataSetObjTypeUndefined )
    {
        bIsValid = false;
    }
    else if( m_strObjKey.isEmpty() )
    {
        bIsValid = false;
    }
    return bIsValid;

} // isValid

//------------------------------------------------------------------------------
QString SDSNodeSpec::toString() const
//------------------------------------------------------------------------------
{
    return m_strObjKey;
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SDSNodeSpec::getAddTrcInfoStr( int /*i_iDetailLevel*/ ) const
//------------------------------------------------------------------------------
{
    QString strAddTrcInfo;

    if( m_strlstChildObjNames.size() == 0 )
    {
        strAddTrcInfo += "*";
    }
    else
    {
        QString strName;
        int     idx;

        for( idx = 0; idx < m_strlstChildObjNames.size(); idx++ )
        {
            strName = m_strlstChildObjNames[idx];

            strAddTrcInfo += strName;

            if( idx < m_strlstChildObjNames.size()-1 )
            {
                strAddTrcInfo += ", ";
            }
        }
    }

    strAddTrcInfo += " FROM " + m_strObjKey;

    if( !m_strWhere.isEmpty() )
    {
        strAddTrcInfo += " WHERE " + m_strWhere;
    }

    if( m_bRecursive )
    {
        strAddTrcInfo += " Recursive";
    }

    return strAddTrcInfo;

} // getAddTrcInfoStr


/*******************************************************************************
struct SDbClientRegSpec
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SDbClientRegSpec::SDbClientRegSpec() :
//------------------------------------------------------------------------------
    m_cnctId(),
    m_strClientName(),
    m_arDSNodeSpecs()
{
} // ctor

//------------------------------------------------------------------------------
SDbClientRegSpec::SDbClientRegSpec( const SCnctId& i_cnctId ) :
//------------------------------------------------------------------------------
    m_cnctId(i_cnctId),
    m_strClientName(),
    m_arDSNodeSpecs()
{
} // ctor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SDbClientRegSpec& SDbClientRegSpec::operator = ( const SCnctId& i_cnctId )
//------------------------------------------------------------------------------
{
    m_cnctId = i_cnctId;
    return *this;
}

//------------------------------------------------------------------------------
SDbClientRegSpec& SDbClientRegSpec::operator = ( const QVector<SDSNodeSpec>& i_arDSNodeSpecs )
//------------------------------------------------------------------------------
{
    m_arDSNodeSpecs = i_arDSNodeSpecs;
    return *this;
}

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
void SDbClientRegSpec::clear()
//------------------------------------------------------------------------------
{
    m_cnctId.m_protocolType = EProtocolTypeUndefined;
    m_cnctId.m_socketType = ZS::Ipc::ESocketTypeUndefined;
    m_cnctId.m_iSocketId = -1;
    m_cnctId.m_pObjDbClt = nullptr;

    m_strClientName.clear();

    m_arDSNodeSpecs.clear();

} // clear

/*==============================================================================
public: // struct methods
==============================================================================*/

//------------------------------------------------------------------------------
bool SDbClientRegSpec::isValid() const
//------------------------------------------------------------------------------
{
    bool bIsValid = true;

    if( m_cnctId.m_protocolType == EProtocolTypeUndefined )
    {
        bIsValid = false;
    }
    else if( m_cnctId.m_socketType == ZS::Ipc::ESocketTypeUndefined )
    {
        bIsValid = false;
    }
    return bIsValid;

} // isValid

//------------------------------------------------------------------------------
QString SDbClientRegSpec::toString( int i_iDetailLevel ) const
//------------------------------------------------------------------------------
{
    QString str;

    str += protocolType2Str(m_cnctId.m_protocolType);
    str += ":" + ZS::Ipc::socketType2Str(m_cnctId.m_socketType);

    if( m_cnctId.m_socketType == ZS::Ipc::ESocketTypeTcp )
    {
        str += ":" + QString::number(m_cnctId.m_iSocketId);

        if( !m_strClientName.isEmpty() )
        {
            str += ":" + m_strClientName;
        }
    }
    else if( m_cnctId.m_socketType == ZS::Ipc::ESocketTypeShm )
    {
        str += ":" + QString::number(m_cnctId.m_iSocketId);

        if( !m_strClientName.isEmpty() )
        {
            str += ":" + m_strClientName;
        }
    }
    else if( m_cnctId.m_socketType == ZS::Ipc::ESocketTypeInProcMsg )
    {
        if( m_strClientName.isEmpty() )
        {
            str += ":" + QString( m_cnctId.m_pObjDbClt == nullptr ? "nullptr" : m_cnctId.m_pObjDbClt->objectName() );
        }
        else
        {
            str += ":" + m_strClientName;
        }
    }

    if( i_iDetailLevel >= 1 )
    {
        str += ", DSNodeSpecs [" + QString::number(m_arDSNodeSpecs.size()) + "]";

        if( m_arDSNodeSpecs.size() > 0 )
        {
            str += "(";

            for( int idxSpec = 0; idxSpec < m_arDSNodeSpecs.size(); idxSpec++ )
            {
                if( idxSpec > 0 )
                {
                    str += ", ";
                }
                str += QString::number(idxSpec) + ": ";
                str += "{ " + m_arDSNodeSpecs[idxSpec].getAddTrcInfoStr(i_iDetailLevel) + " }";
            }
            str += ")";
        }
    } // if( i_iDetailLevel >= 1 )

    return str;

} // toString

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo SDbClientRegSpec::serialize( QByteArray& io_byteArr ) const
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo("ZS::Data", "SDbClientRegSpec", "", "serialize");

    QXmlStreamWriter xmlStreamWriter(&io_byteArr);

    // DataSetObject Attributes
    //-------------------------

    xmlStreamWriter.writeStartElement("RegSpec");

    if( !m_strClientName.isEmpty() )
    {
        xmlStreamWriter.writeAttribute("ClientName", m_strClientName);
    }

    if( m_arDSNodeSpecs.size() > 0 )
    {
        xmlStreamWriter.writeStartElement("DSNodeSpecs");

        //SDSNodeSpec dsNodeSpec;
        //SRangeInfo  rangeInfo;
        //int         idxSpec;
        //int         idxSub;

        //for( idxSpec = 0; idxSpec < m_arDSNodeSpecs.size(); idxSpec++ )
        //{
        //    dsNodeSpec = m_arDSNodeSpecs[idxSpec];

        //    xmlStreamWriter.writeStartElement("DSNodeSpec");

        //    /*
        //    struct SDSNodeSpec {
        //        EDataSetObjType     m_dsObjType;            // Type of the data set object (table, parameter).
        //        QString             m_strObjKey;            // Key of the data set object (table, parameter).
        //        QStringList         m_strlstChildObjNames;  // Usually used to specify columns of a table.
        //        QVector<SRangeInfo> m_arRangeInfos;         // Usually used to specify part (sub array) of an array parameter.
        //        QString             m_strWhere;             // Filter.
        //        bool                m_bRecursive;           // Specifies whether children of a group should be included.
        //    }; */

        //    xmlStreamWriter.writeAttribute("DSObjType", dataSetObjType2Str(dsNodeSpec.m_dsObjType));
        //    xmlStreamWriter.writeAttribute("DSObjKey", dsNodeSpec.m_strObjKey);

        //    if( !dsNodeSpec.m_strWhere.isEmpty() )
        //    {
        //        xmlStreamWriter.writeAttribute("Where", dsNodeSpec.m_strWhere);
        //    }
        //    xmlStreamWriter.writeAttribute("Recursive", bool2Str(dsNodeSpec.m_bRecursive));

        //    if( dsNodeSpec.m_strlstChildObjNames.size() > 0 )
        //    {
        //        xmlStreamWriter.writeStartElement("Childs");

        //        for( idxSub = 0; idxSub < dsNodeSpec.m_strlstChildObjNames.size(); idxSub++ )
        //        {
        //            xmlStreamWriter.writeStartElement("Child");
        //            xmlStreamWriter.writeAttribute("Name", dsNodeSpec.m_strlstChildObjNames[idxSub]);
        //            xmlStreamWriter.writeEndElement(); // "Child"
        //        }
        //        xmlStreamWriter.writeEndElement(); // "Childs"

        //    } // if( dsNodeSpec.m_strlstChildObjNames.size() > 0 )

        //    if( dsNodeSpec.m_arRangeInfos.size() > 0 )
        //    {
        //        xmlStreamWriter.writeStartElement("RangeInfos");

        //        for( idxSub = 0; idxSub < dsNodeSpec.m_arRangeInfos.size(); idxSub++ )
        //        {
        //            rangeInfo = dsNodeSpec.m_arRangeInfos[idxSub];

        //            /*
        //            struct SRangeInfo {
        //                bool                       m_bValid;
        //                ZS::System::EDimensionType m_dimType;
        //                int                        m_iDimension;     // [EArrayIndexUndefined,FirstElement,LastElement,CountAllElements,0..N]
        //                SDataType                  m_dataType;
        //                int                        m_idxStart;       // [EArrayIndexUndefined,FirstElement,LastElement,CountAllElements,0..N]
        //                int                        m_idxCount;       // [EArrayIndexUndefined,CountAllElements,0..N]
        //            }; */

        //            if( rangeInfo.m_bValid )
        //            {
        //                xmlStreamWriter.writeStartElement("RangeInfo");
        //                xmlStreamWriter.writeAttribute("DimType", CEnumDimensionType::toString(rangeInfo.m_dimType));

        //                if( rangeInfo.m_dimType == EDimensionType::MoreDimensional )
        //                {
        //                    xmlStreamWriter.writeAttribute("Dimension", arrayIndex2Str(rangeInfo.m_iDimension));
        //                }
        //                if( rangeInfo.m_dimType != EDimensionType::SingleValue )
        //                {
        //                    xmlStreamWriter.writeAttribute("IdxStart", arrayIndex2Str(rangeInfo.m_idxStart));
        //                    xmlStreamWriter.writeAttribute("IdxCount", arrayIndex2Str(rangeInfo.m_idxCount));
        //                }

        //                xmlStreamWriter.writeStartElement("DataType");
        //                xmlStreamWriter.writeAttribute("DataType", dataType2Str(rangeInfo.m_dataType.m_dataType));
        //                if( rangeInfo.m_dataType == EDataTypeEnum )
        //                {
        //                    if( !rangeInfo.m_dataType.m_strEnumUId.isEmpty() )
        //                    {
        //                        xmlStreamWriter.writeAttribute("EnumUId", rangeInfo.m_dataType.m_strEnumUId);
        //                    }
        //                    else if( rangeInfo.m_dataType.m_pEnum != nullptr )
        //                    {
        //                        xmlStreamWriter.writeAttribute("EnumUId", rangeInfo.m_dataType.m_pEnum->keyInTree());
        //                    }
        //                }
        //                else if( rangeInfo.m_dataType == EDataTypePhysVal )
        //                {
        //                    if( !rangeInfo.m_dataType.m_strUnitGrpKey.isEmpty() )
        //                    {
        //                        xmlStreamWriter.writeAttribute("UnitGrpUId", rangeInfo.m_dataType.m_strUnitGrpKey);
        //                    }
        //                    else if( rangeInfo.m_dataType.m_pUnitGrp != nullptr )
        //                    {
        //                        xmlStreamWriter.writeAttribute("UnitGrpUId", rangeInfo.m_dataType.m_pUnitGrp->getDSObjKey());
        //                    }
        //                    if( !rangeInfo.m_dataType.m_strUnitKey.isEmpty() )
        //                    {
        //                        xmlStreamWriter.writeAttribute("UnitUId", rangeInfo.m_dataType.m_strUnitKey);
        //                    }
        //                    else if( rangeInfo.m_dataType.m_pUnit != nullptr )
        //                    {
        //                        xmlStreamWriter.writeAttribute("UnitUId", rangeInfo.m_dataType.m_pUnit->getDSObjKey());
        //                    }
        //                }
        //                xmlStreamWriter.writeEndElement(); // "DataType"
        //                xmlStreamWriter.writeEndElement(); // "RangeInfo"
        //            }
        //        } // for( idxSub = 0; idxSub < dsNodeSpec.m_arRangeInfos.size(); idxSub++ )

        //        xmlStreamWriter.writeEndElement(); // "RangeInfos"

        //    } // if( dsNodeSpec.m_arRangeInfos.size() > 0 )

        //    xmlStreamWriter.writeEndElement(); // "DSNodeSpec"

        //} // for( idxSpec = 0; idxSpec < m_arDSNodeSpecs.size(); idxSpec++ )

        xmlStreamWriter.writeEndElement(); // "DSNodeSpecs"

    } // if( m_arDSNodeSpecs.size() > 0 )

    xmlStreamWriter.writeEndElement(); // RegSpec

    return errResultInfo;

} // serialize

//------------------------------------------------------------------------------
ZS::System::SErrResultInfo SDbClientRegSpec::deserialize( const QByteArray& i_byteArr )
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo("ZS::Data", "SDbClientRegSpec", "", "deserialize");

    QString strAddErrInfo;

    QXmlStreamReader xmlStreamReader(i_byteArr);

    QString          strElemName;
    QString          strAttr;
    EDataSetObjType  dsObjType;
    SDSNodeSpec      dsNodeSpec;
    //SRangeInfo     rangeInfo;
    //EDimensionType dimType;
    //EDataType      dataType;
    //int            iVal;
    bool             bVal;
    bool             bConverted;

    while( !xmlStreamReader.atEnd() )
    {
        if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )
        {
            strElemName = xmlStreamReader.name().toString();

            if( strElemName.compare("RegSpec",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                    if( xmlStreamReader.attributes().hasAttribute("ClientName") )
                    {
                        m_strClientName = xmlStreamReader.attributes().value("ClientName").toString();
                    }
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("RegSpec",Qt::CaseInsensitive) == 0 )

            else if( strElemName.compare("DSNodeSpecs",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("DSNodeSpecs",Qt::CaseInsensitive) == 0 )

            else if( strElemName.compare("DSNodeSpec",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                    if( !xmlStreamReader.attributes().hasAttribute("DSObjType") )
                    {
                        xmlStreamReader.raiseError("Attribute \"DSObjType\" for DSNodeSpec not defined");
                    }
                    else // if( xmlStreamReader.attributes().hasAttribute("DSObjType") )
                    {
                        strAttr = xmlStreamReader.attributes().value("DSObjType").toString();

                        dsObjType = str2DataSetObjType(strAttr);

                        if( dsObjType == EDataSetObjTypeUndefined )
                        {
                            xmlStreamReader.raiseError("Attribute \"DSObjType\" " + strAttr + " of DSNodeSpec out of range");
                        }
                        else
                        {
                            dsNodeSpec.m_dsObjType = dsObjType;
                        }
                    } // if( xmlStreamReader.attributes().hasAttribute("DSObjType") )

                    if( !xmlStreamReader.hasError() )
                    {
                        if( !xmlStreamReader.attributes().hasAttribute("DSObjKey") )
                        {
                            xmlStreamReader.raiseError("Attribute \"DSObjKey\" for DSNodeSpec not defined");
                        }
                        else // if( xmlStreamReader.attributes().hasAttribute("DSObjKey") )
                        {
                            dsNodeSpec.m_strObjKey = xmlStreamReader.attributes().value("DSObjKey").toString();
                        }
                    } // if( !xmlStreamReader.hasError() )

                    if( !xmlStreamReader.hasError() )
                    {
                        if( xmlStreamReader.attributes().hasAttribute("Where") )
                        {
                            dsNodeSpec.m_strWhere = xmlStreamReader.attributes().value("Where").toString();
                        }
                    } // if( !xmlStreamReader.hasError() )

                    if( !xmlStreamReader.hasError() )
                    {
                        if( xmlStreamReader.attributes().hasAttribute("Recursive") )
                        {
                            strAttr = xmlStreamReader.attributes().value("Recursive").toString();

                            bVal = str2Bool(strAttr, &bConverted);

                            if( !bConverted )
                            {
                                xmlStreamReader.raiseError("Attribute \"Recursive\" " + strAttr + " of DSNodeSpec out of range");
                            }
                            else
                            {
                                dsNodeSpec.m_bRecursive = bVal;
                            }
                        }
                    } // if( !xmlStreamReader.hasError() )
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                    m_arDSNodeSpecs.append(dsNodeSpec);
                    dsNodeSpec.clear();

                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("DSNodeSpec",Qt::CaseInsensitive) == 0 )

            else if( strElemName.compare("Childs",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("Childs",Qt::CaseInsensitive) == 0 )

            else if( strElemName.compare("Child",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                    if( xmlStreamReader.attributes().hasAttribute("Name") )
                    {
                        dsNodeSpec.m_strlstChildObjNames.append( xmlStreamReader.attributes().value("Name").toString() );
                    }
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("Child",Qt::CaseInsensitive) == 0 )

            else if( strElemName.compare("RangeInfos",Qt::CaseInsensitive) == 0 )
            {
                if( xmlStreamReader.isStartElement() )
                {
                } // if( xmlStreamReader.isStartElement() )

                else if( xmlStreamReader.isEndElement() )
                {
                } // if( xmlStreamReader.isEndElement() )
            } // if( strElemName.compare("RangeInfos",Qt::CaseInsensitive) == 0 )

            //else if( strElemName.compare("RangeInfo",Qt::CaseInsensitive) == 0 )
            //{
            //    if( xmlStreamReader.isStartElement() )
            //    {
            //        if( !xmlStreamReader.attributes().hasAttribute("DimType") )
            //        {
            //            xmlStreamReader.raiseError("Attribute \"DimType\" for DSNodeSpec.RangeInfo not defined");
            //        }
            //        else // if( xmlStreamReader.attributes().hasAttribute("DimType") )
            //        {
            //            strAttr = xmlStreamReader.attributes().value("DimType").toString();

            //            dimType = CEnumDimensionType::toEnumerator(strAttr);

            //            if( dimType == EDimensionType::Undefined )
            //            {
            //                xmlStreamReader.raiseError("Attribute \"DimType\" " + strAttr + " of DSNodeSpec.RangeInfo out of range");
            //            }
            //            else
            //            {
            //                rangeInfo.m_dimType = dimType;
            //            }
            //        } // if( xmlStreamReader.attributes().hasAttribute("DimType") )

            //        if( !xmlStreamReader.hasError() )
            //        {
            //            if( rangeInfo.m_dimType == EDimensionType::MoreDimensional )
            //            {
            //                if( !xmlStreamReader.attributes().hasAttribute("Dimension") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"Dimension\" for DSNodeSpec.RangeInfo not defined");
            //                }
            //                else // if( xmlStreamReader.attributes().hasAttribute("DimType") )
            //                {
            //                    strAttr = xmlStreamReader.attributes().value("Dimension").toString();

            //                    iVal = strAttr.toInt(&bConverted);

            //                    if( !bConverted )
            //                    {
            //                        xmlStreamReader.raiseError("Attribute \"Dimension\" " + strAttr + " of DSNodeSpec.RangeInfo out of range");
            //                    }
            //                    else
            //                    {
            //                        rangeInfo.m_iDimension = iVal;
            //                    }
            //                }
            //            }
            //            else // if( rangeInfo.m_dimType != EDimensionType::MoreDimensional )
            //            {
            //                if( xmlStreamReader.attributes().hasAttribute("Dimension") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"Dimension\" for DSNodeSpec.RangeInfo not expected");
            //                }
            //            }
            //        } // if( !xmlStreamReader.hasError() )

            //        if( !xmlStreamReader.hasError() )
            //        {
            //            if( rangeInfo.m_dimType != EDimensionType::SingleValue )
            //            {
            //                if( !xmlStreamReader.attributes().hasAttribute("IdxStart") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"IdxStart\" for DSNodeSpec.RangeInfo not defined");
            //                }
            //                else // if( xmlStreamReader.attributes().hasAttribute("IdxStart") )
            //                {
            //                    strAttr = xmlStreamReader.attributes().value("IdxStart").toString();

            //                    iVal = strAttr.toInt(&bConverted);

            //                    if( !bConverted )
            //                    {
            //                        xmlStreamReader.raiseError("Attribute \"IdxStart\" " + strAttr + " of DSNodeSpec.RangeInfo out of range");
            //                    }
            //                    else
            //                    {
            //                        rangeInfo.m_idxStart = iVal;
            //                    }
            //                }
            //            }
            //            else // if( rangeInfo.m_dimType == EDimensionType::SingleValue )
            //            {
            //                if( xmlStreamReader.attributes().hasAttribute("IdxStart") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"IdxStart\" for DSNodeSpec.RangeInfo not expected");
            //                }
            //            }
            //        } // if( !xmlStreamReader.hasError() )

            //        if( !xmlStreamReader.hasError() )
            //        {
            //            if( rangeInfo.m_dimType != EDimensionType::SingleValue )
            //            {
            //                if( !xmlStreamReader.attributes().hasAttribute("IdxCount") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"IdxCount\" for DSNodeSpec.RangeInfo not defined");
            //                }
            //                else // if( xmlStreamReader.attributes().hasAttribute("IdxCount") )
            //                {
            //                    strAttr = xmlStreamReader.attributes().value("IdxCount").toString();

            //                    iVal = strAttr.toInt(&bConverted);

            //                    if( !bConverted )
            //                    {
            //                        xmlStreamReader.raiseError("Attribute \"IdxCount\" " + strAttr + " of DSNodeSpec.RangeInfo out of range");
            //                    }
            //                    else
            //                    {
            //                        rangeInfo.m_idxCount = iVal;
            //                    }
            //                }
            //            }
            //            else // if( rangeInfo.m_dimType == EDimensionType::SingleValue )
            //            {
            //                if( xmlStreamReader.attributes().hasAttribute("IdxCount") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"IdxCount\" for DSNodeSpec.RangeInfo not expected");
            //                }
            //            }
            //        } // if( !xmlStreamReader.hasError() )
            //    } // if( xmlStreamReader.isStartElement() )

            //    else if( xmlStreamReader.isEndElement() )
            //    {
            //        dsNodeSpec.m_arRangeInfos.append(rangeInfo);
            //        rangeInfo = SRangeInfo();

            //    } // if( xmlStreamReader.isEndElement() )
            //} // if( strElemName.compare("RangeInfo",Qt::CaseInsensitive) == 0 )

            //else if( strElemName.compare("DataType",Qt::CaseInsensitive) == 0 )
            //{
            //    if( xmlStreamReader.isStartElement() )
            //    {
            //        if( !xmlStreamReader.attributes().hasAttribute("DataType") )
            //        {
            //            xmlStreamReader.raiseError("Attribute \"DataType\" for DSNodeSpec.RangeInfo not defined");
            //        }
            //        else // if( xmlStreamReader.attributes().hasAttribute("IdxCount") )
            //        {
            //            strAttr = xmlStreamReader.attributes().value("DataType").toString();

            //            dataType = str2DataType(strAttr);

            //            if( dataType == EDataTypeUndefined )
            //            {
            //                xmlStreamReader.raiseError("Attribute \"DataType\" " + strAttr + " of DSNodeSpec.RangeInfo out of range");
            //            }
            //            else
            //            {
            //                rangeInfo.m_dataType.m_dataType = dataType;
            //            }
            //        }

            //        if( !xmlStreamReader.hasError() )
            //        {
            //            if( rangeInfo.m_dataType.m_dataType == EDataTypeEnum )
            //            {
            //                if( !xmlStreamReader.attributes().hasAttribute("EnumUId") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"EnumUId\" for DSNodeSpec.RangeInfo not defined");
            //                }
            //                else // if( xmlStreamReader.attributes().hasAttribute("EnumUId") )
            //                {
            //                    rangeInfo.m_dataType.m_strEnumUId = xmlStreamReader.attributes().value("EnumUId").toString();
            //                }
            //            } // if( rangeInfo.m_dataType.m_dataType == EDataTypeEnum )

            //            else if( rangeInfo.m_dataType.m_dataType == EDataTypePhysVal )
            //            {
            //                if( !xmlStreamReader.attributes().hasAttribute("UnitUId") )
            //                {
            //                    xmlStreamReader.raiseError("Attribute \"UnitUId\" for DSNodeSpec.RangeInfo not defined");
            //                }
            //                else // if( xmlStreamReader.attributes().hasAttribute("UnitUId") )
            //                {
            //                    rangeInfo.m_dataType.m_strUnitKey = xmlStreamReader.attributes().value("UnitUId").toString();
            //                }
            //                if( xmlStreamReader.attributes().hasAttribute("UnitGrpUId") )
            //                {
            //                    rangeInfo.m_dataType.m_strUnitGrpKey = xmlStreamReader.attributes().value("UnitGrpUId").toString();
            //                }
            //            } // if( rangeInfo.m_dataType.m_dataType == EDataTypeEnum )
            //        } // if( !xmlStreamReader.hasError() )
            //    } // if( xmlStreamReader.isStartElement() )

            //    else if( xmlStreamReader.isEndElement() )
            //    {
            //    } // if( xmlStreamReader.isEndElement() )
            //} // if( strElemName.compare("DataType",Qt::CaseInsensitive) == 0 )

            else
            {
                xmlStreamReader.raiseError("Invalid element \"" + strElemName + "\"");
            }
        } // if( xmlStreamReader.isStartElement() || xmlStreamReader.isEndElement() )

        if( xmlStreamReader.hasError() )
        {
            strAddErrInfo  = xmlStreamReader.errorString() + " on deserializing byte array with XML data.\n";
            strAddErrInfo += "Line:" + QString::number(xmlStreamReader.lineNumber()) + ", ";
            strAddErrInfo += "Column:" + QString::number(xmlStreamReader.columnNumber());

            errResultInfo.setSeverity(EResultSeverityError);
            errResultInfo.setResult(EResultFileReadContent);
            errResultInfo.setAddErrInfoDscr(strAddErrInfo);
            break;
        }
    } // while( !xmlStreamReader.atEnd() )

    return errResultInfo;

} // deserialize
