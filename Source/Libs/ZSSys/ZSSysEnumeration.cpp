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

#include "ZSSys/ZSSysEnumeration.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;


/*******************************************************************************
class CEnumeration
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CEnumeration::CEnumeration( const QString& /*i_strNameSpace*/, const QString& i_strEnumName ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EEntryType::Leave, i_strEnumName),
    m_arpEntries(),
    m_bMaxEnumEntryIndexDirty(false),
    m_iMaxEnumEntryIndex(-1),
    m_bMaxEnumEntryStrAliasCountDirty(false),
    m_iMaxEnumEntryStrAliasCount(0)
{
} // ctor

//------------------------------------------------------------------------------
CEnumeration::CEnumeration(
    const QString&    /*i_strNameSpace*/,
    const QString&    i_strEnumName,
    const SEnumEntry* i_pEnumArr,
    int               i_iArrLen ) :
//------------------------------------------------------------------------------
    CIdxTreeEntry(EEntryType::Leave, i_strEnumName),
    m_arpEntries(),
    m_bMaxEnumEntryIndexDirty(false),
    m_iMaxEnumEntryIndex(-1),
    m_bMaxEnumEntryStrAliasCountDirty(false),
    m_iMaxEnumEntryStrAliasCount(0)
{
    const SEnumEntry* pEntry;
    int               idx;

    for( idx = 0; idx < i_iArrLen; idx++ )
    {
        pEntry = &i_pEnumArr[idx];

        setEntry(
            /* idx            */ pEntry->m_iEnumerator,
            /* strlstValNames */ pEntry->m_strlstNames,
            /* val            */ pEntry->m_val,
            /* validity       */ pEntry->m_validity );

        if( pEntry->m_iEnumerator > m_iMaxEnumEntryIndex )
        {
            m_iMaxEnumEntryIndex = pEntry->m_iEnumerator;
        }
        if( pEntry->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
        {
            m_iMaxEnumEntryStrAliasCount = pEntry->m_strlstNames.size();
        }
    }

} // ctor

////------------------------------------------------------------------------------
//CEnumeration::CEnumeration( const CEnumeration& i_other ) :
////------------------------------------------------------------------------------
//    CIdxTreeEntry(i_other),
//    m_strNameSpace(i_other.m_strNameSpace),
//    m_arpEntries(),
//    m_bMaxEnumEntryIndexDirty(i_other.m_bMaxEnumEntryIndexDirty),
//    m_iMaxEnumEntryIndex(i_other.m_iMaxEnumEntryIndex),
//    m_bMaxEnumEntryStrAliasCountDirty(i_other.m_bMaxEnumEntryStrAliasCountDirty),
//    m_iMaxEnumEntryStrAliasCount(i_other.m_iMaxEnumEntryStrAliasCount)
//{
//    const SEnumEntry* pEntry;
//    int               idx;
//
//    for( idx = 0; idx < i_other.m_arpEntries.size(); idx++ )
//    {
//        pEntry = i_other.m_arpEntries[idx];
//
//        setEntry(
//            /* idx            */ pEntry->m_iEnumerator,
//            /* strlstValNames */ pEntry->m_strlstNames,
//            /* validity       */ pEntry->m_validity,
//            /* val            */ pEntry->m_val );
//    }
//
//} // ctor

//------------------------------------------------------------------------------
CEnumeration::~CEnumeration()
//------------------------------------------------------------------------------
{
    if( m_arpEntries.size() > 0 )
    {
        SEnumEntry* pEntry;
        int         idx;

        for( idx = m_arpEntries.size()-1; idx >= 0; idx-- )
        {
            pEntry = m_arpEntries[idx];

            try
            {
                delete pEntry;
            }
            catch(...)
            {
            }
            pEntry = nullptr;
            m_arpEntries[idx] = nullptr;
        }
    }

    //m_strNameSpace;
    //m_arpEntries;
    m_bMaxEnumEntryIndexDirty = false;
    m_iMaxEnumEntryIndex = 0;
    m_bMaxEnumEntryStrAliasCountDirty = false;
    m_iMaxEnumEntryStrAliasCount = 0;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

////------------------------------------------------------------------------------
//CEnumeration& CEnumeration::operator = ( const CEnumeration& i_other )
////------------------------------------------------------------------------------
//{
//    m_strNameSpace = i_other.m_strNameSpace;
//
//    throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
//
//    return *this;
//
//} // operator =

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CEnumeration::operator == ( const CEnumeration& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_arpEntries.size() != i_other.m_arpEntries.size() )
    {
        bEqual = false;
    }
    else
    {
        throw CException(__FILE__, __LINE__, EResultMethodNotYetImplemented);
    }

    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CEnumeration::operator != ( const CEnumeration& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CEnumeration* CEnumeration::clone() const
//------------------------------------------------------------------------------
{
    // The clone will also have the same object identifier.
    CEnumeration* pSchema = new CEnumeration(*this);

    return pSchema;

} // clone

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CEnumeration::getNameSpace() const
//------------------------------------------------------------------------------
{
    return CIdxTreeEntry::path();
}

//------------------------------------------------------------------------------
QString CEnumeration::getName() const
//------------------------------------------------------------------------------
{
    return CIdxTreeEntry::name();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CEnumeration::count() const
//------------------------------------------------------------------------------
{
    return m_arpEntries.count();
}

//------------------------------------------------------------------------------
SEnumEntry* CEnumeration::getEntry( int i_idxRow ) const
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = nullptr;

    if( i_idxRow >= 0 && i_idxRow < m_arpEntries.size() )
    {
        pEntry = m_arpEntries[i_idxRow];
    }
    return pEntry;

} // getEntry

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumeration::getEnumEntryValidity( int i_idxRow ) const
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityUndefined;

    if( i_idxRow >= 0 && i_idxRow < m_arpEntries.size() )
    {
        validity = m_arpEntries[i_idxRow]->m_validity;
    }
    return validity;

} // getEnumEntryValidity

//------------------------------------------------------------------------------
void CEnumeration::removeEntry( int i_idxRow )
//------------------------------------------------------------------------------
{
    if( i_idxRow >= 0 && i_idxRow < m_arpEntries.count() )
    {
        SEnumEntry* pEntry = m_arpEntries[i_idxRow];
        int         idxRow;

        m_arpEntries.remove(i_idxRow);

        delete pEntry;
        pEntry = nullptr;

        for( idxRow = i_idxRow; idxRow < m_arpEntries.count(); idxRow++ )
        {
            pEntry = m_arpEntries[idxRow];
            pEntry->m_iEnumerator = idxRow;
        }

        m_bMaxEnumEntryIndexDirty = true;
        m_bMaxEnumEntryStrAliasCountDirty = true;

    } // if( i_idxRow >= 0 && i_idxRow < m_arpEntries.count() )

} // removeEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntries( const ZS::System::SEnumEntry* i_pEnumArr, int i_iArrLen )
//------------------------------------------------------------------------------
{
    const SEnumEntry* pEntry;
    int               idx;

    for( idx = 0; idx < i_iArrLen; idx++ )
    {
        pEntry = &i_pEnumArr[idx];

        setEntry(
            /* idx            */ pEntry->m_iEnumerator,
            /* strlstValNames */ pEntry->m_strlstNames,
            /* val            */ pEntry->m_val,
            /* validity       */ pEntry->m_validity );

        if( pEntry->m_iEnumerator > m_iMaxEnumEntryIndex )
        {
            m_iMaxEnumEntryIndex = pEntry->m_iEnumerator;
        }
        if( pEntry->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
        {
            m_iMaxEnumEntryStrAliasCount = pEntry->m_strlstNames.size();
        }
    }

} // setEntries

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
int CEnumeration::getMaxEnumEntryIndex() const
//------------------------------------------------------------------------------
{
    if( m_bMaxEnumEntryIndexDirty )
    {
        const SEnumEntry* pEntryTmp;
        int               idxTmp;

        m_iMaxEnumEntryIndex = -1;

        for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )
        {
            pEntryTmp = m_arpEntries[idxTmp];

            if( pEntryTmp->m_iEnumerator > m_iMaxEnumEntryIndex )
            {
                m_iMaxEnumEntryIndex = pEntryTmp->m_iEnumerator;
            }
        }

        m_bMaxEnumEntryIndexDirty = false;

    } // if( m_bMaxEnumEntryIndexDirty )

    return m_iMaxEnumEntryIndex;

} // getMaxEnumEntryIndex

//------------------------------------------------------------------------------
int CEnumeration::getMaxEnumEntryStrAliasCount() const
//------------------------------------------------------------------------------
{
    if( m_bMaxEnumEntryStrAliasCountDirty )
    {
        const SEnumEntry* pEntryTmp;
        int               idxTmp;

        m_iMaxEnumEntryStrAliasCount = 0;

        for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )
        {
            pEntryTmp = m_arpEntries[idxTmp];

            if( pEntryTmp->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
            {
                m_iMaxEnumEntryStrAliasCount = pEntryTmp->m_strlstNames.size();
            }
        }

        m_bMaxEnumEntryStrAliasCountDirty = false;

    } // if( m_bMaxEnumEntryStrAliasCountDirty )

    return m_iMaxEnumEntryStrAliasCount;

} // getMaxEnumEntryStrAliasCount

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SEnumEntry* CEnumeration::index2EnumEntry( int i_idx ) const
//------------------------------------------------------------------------------
{
    QString strMth = "index2EnumEntry";
    QString strAddErrInfo;

    SEnumEntry* pEntry = nullptr;

    if( i_idx >= 0 && i_idx < m_arpEntries.size() )
    {
        SEnumEntry* pEntryTmp = m_arpEntries[i_idx];
        bool        bErrLogEntryAdded = false;

        if( pEntryTmp->m_iEnumerator == i_idx )
        {
            pEntry = pEntryTmp;
        }
        else if( !bErrLogEntryAdded )
        {
            if( CErrLog::GetInstance() != nullptr )
            {
                strAddErrInfo  = "Enum entry index " + QString::number(pEntry->m_iEnumerator);
                strAddErrInfo += " of " + pEntryTmp->getName();
                strAddErrInfo += " does not correspond to array index " + QString::number(i_idx);
                SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                    NameSpace(), ClassName(), keyInTree(), strMth,
                    EResultListCorrupted, ZS::System::EResultSeverityCritical, strAddErrInfo);
                CErrLog::GetInstance()->addEntry(errResultInfo);
            }
            bErrLogEntryAdded = true;
        }
    } // if( i_idx >= 0 && i_idx < m_arpEntries.size() )

    if( pEntry == nullptr && i_idx >= 0 && i_idx < m_arpEntries.size() ) // !! Should never ever happen !!
    {
        SEnumEntry* pEntryTmp;
        int         idxVal;

        for( idxVal = 0; idxVal < m_arpEntries.size(); idxVal++ )
        {
            pEntryTmp = m_arpEntries[idxVal];

            if( pEntryTmp->m_iEnumerator == i_idx )
            {
                pEntry = pEntryTmp;
                break;
            }
            else // if( pEntryTmp->m_iEnumerator != i_idx )
            {
                if( CErrLog::GetInstance() != nullptr )
                {
                    strAddErrInfo  = "Enum entry index " + QString::number(pEntryTmp->m_iEnumerator);
                    strAddErrInfo += " of " + pEntryTmp->getName();
                    strAddErrInfo += " does not correspond to array index " + QString::number(idxVal);
                    SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                        NameSpace(), ClassName(), keyInTree(), strMth,
                        EResultListCorrupted, ZS::System::EResultSeverityCritical, strAddErrInfo);
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        } // for( idxVal = 0; idxVal < i_iArrLen; idxVal++ )
    } // if( pEntry == nullptr ) // !! Should never ever happen !!

    return pEntry;

} // index2EnumEntry

//------------------------------------------------------------------------------
QString CEnumeration::index2Name( int i_idx, int i_alias ) const
//------------------------------------------------------------------------------
{
    QString strName;

    const SEnumEntry* pEntry = index2EnumEntry(i_idx);

    if( pEntry != nullptr )
    {
        strName = pEntry->getName(i_alias);
    }
    return strName;

} // index2Name

//------------------------------------------------------------------------------
QVariant CEnumeration::index2Val( int i_idx ) const
//------------------------------------------------------------------------------
{
    QVariant val;

    const SEnumEntry* pEntry = index2EnumEntry(i_idx);

    if( pEntry != nullptr )
    {
        val = pEntry->m_val;
    }
    return val;

} // index2Val

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SEnumEntry* CEnumeration::name2EnumEntry(
    const QString&      i_strName,
    Qt::CaseSensitivity i_caseSensitivity,
    int                 i_alias ) const
//------------------------------------------------------------------------------
{
    QString strMth = "name2EnumEntry";
    QString strAddErrInfo;

    SEnumEntry* pEntry = nullptr;

    bool bConverted = false;

    int idxVal = i_strName.toInt(&bConverted);

    if( !bConverted )
    {
        idxVal = -1;
    }

    // If the name is not the index value itself ..
    if( idxVal >= 0 && idxVal < m_arpEntries.count() )
    {
        pEntry = m_arpEntries[idxVal];
    }
    else // if( idxVal < 0 || idxVal >= i_iArrLen )
    {
        SEnumEntry* pEntryTmp;
        int         idxTmp;
        int         idxStrLstMin;
        int         idxStrLstMax;
        int         idxStrLst;

        for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )
        {
            pEntryTmp = m_arpEntries[idxTmp];

            if( i_alias == EEnumEntryAliasStrUndefined )
            {
                idxStrLstMin = 0;
                idxStrLstMax = pEntryTmp->m_strlstNames.size()-1;
            }
            else
            {
                idxStrLstMin = i_alias;
                idxStrLstMax = i_alias;
            }

            for( idxStrLst = idxStrLstMin; idxStrLst <= idxStrLstMax; idxStrLst++ )
            {
                if( idxStrLst >= 0 && idxStrLst < pEntryTmp->m_strlstNames.size() )
                {
                    if( pEntryTmp->m_strlstNames[idxStrLst].compare(i_strName,i_caseSensitivity) == 0 )
                    {
                        pEntry = pEntryTmp;
                        break;
                    }
                }
                else
                {
                    if( CErrLog::GetInstance() != nullptr )
                    {
                        strAddErrInfo  = "Enum entry " + pEntryTmp->getName();
                        strAddErrInfo += " does not have an alias string at index " + QString::number(idxStrLst);
                        SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                            NameSpace(), ClassName(), keyInTree(), strMth,
                            EResultListCorrupted, ZS::System::EResultSeverityCritical, strAddErrInfo);
                        CErrLog::GetInstance()->addEntry(errResultInfo);
                    }
                }
            }

            if( pEntry != nullptr )
            {
                break;
            }
        } // for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )
    } // if( idxVal < 0 || idxVal >= i_iArrLen )

    return pEntry;

} // name2EnumEntry

//------------------------------------------------------------------------------
int CEnumeration::name2Index(
    const QString&      i_strName,
    Qt::CaseSensitivity i_caseSensitivity,
    int                 i_alias ) const
//------------------------------------------------------------------------------
{
    int idx = -1;

    const SEnumEntry* pEntry = name2EnumEntry(i_strName, i_caseSensitivity, i_alias);

    if( pEntry != nullptr )
    {
        idx = pEntry->m_iEnumerator;
    }
    return idx;

} // name2Index

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
SEnumEntry* CEnumeration::val2EnumEntry( const QVariant& i_val ) const
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = nullptr;
    SEnumEntry* pEntryTmp;
    int         idxTmp;

    for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )
    {
        pEntryTmp = m_arpEntries[idxTmp];

        if( pEntryTmp->m_val == i_val )
        {
            pEntry = pEntryTmp;
        }

        if( pEntry != nullptr )
        {
            break;
        }
    } // for( idxTmp = 0; idxTmp < m_arpEntries.count(); idxTmp++ )

    return pEntry;

} // val2EnumEntry

//------------------------------------------------------------------------------
int CEnumeration::val2Index( const QVariant& i_val ) const
//------------------------------------------------------------------------------
{
    int idx = -1;

    const SEnumEntry* pEntry = val2EnumEntry(i_val);

    if( pEntry != nullptr )
    {
        idx = pEntry->m_iEnumerator;
    }
    return idx;

} // val2Index

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CEnumeration::val2Name( const QVariant& i_val, int i_alias ) const
//------------------------------------------------------------------------------
{
    QString strMth = "val2Name";
    QString strAddErrInfo;

    QString strVal;

    const SEnumEntry* pEntry = val2EnumEntry(i_val);

    if( pEntry != nullptr )
    {
        int idxStrLstMin;
        int idxStrLstMax;
        int idxStrLst;

        if( i_alias == EEnumEntryAliasStrUndefined )
        {
            idxStrLstMin = 0;
            idxStrLstMax = pEntry->m_strlstNames.size()-1;
        }
        else
        {
            idxStrLstMin = i_alias;
            idxStrLstMax = i_alias;
        }

        for( idxStrLst = idxStrLstMin; idxStrLst <= idxStrLstMax; idxStrLst++ )
        {
            if( idxStrLst >= 0 && idxStrLst < pEntry->m_strlstNames.size() )
            {
                strVal = pEntry->m_strlstNames[idxStrLst];
            }
            else
            {
                strVal = i_val.toString();

                if( CErrLog::GetInstance() != nullptr )
                {
                    strAddErrInfo  = "Enum entry " + pEntry->getName();
                    strAddErrInfo += " does not have an alias string at index " + QString::number(idxStrLst);
                    SErrResultInfo errResultInfo = ZS::System::SErrResultInfo(
                        NameSpace(), ClassName(), keyInTree(), strMth,
                        EResultListCorrupted, ZS::System::EResultSeverityCritical, strAddErrInfo);
                    CErrLog::GetInstance()->addEntry(errResultInfo);
                }
            }
        } // for( idxStrLst = idxStrLstMin; idxStrLst <= idxStrLstMax; idxStrLst++ )
    }
    else // if( pEntry == nullptr )
    {
        strVal = i_val.toString();
    }
    return strVal;

} // val2Name

//------------------------------------------------------------------------------
QVariant CEnumeration::name2Val(
    const QString&      i_strName,
    Qt::CaseSensitivity i_caseSensitivity,
    int                 i_alias ) const
//------------------------------------------------------------------------------
{
    QVariant val;

    const SEnumEntry* pEntry = name2EnumEntry(i_strName, i_caseSensitivity, i_alias);

    if( pEntry != nullptr )
    {
        val = pEntry->m_val;
    }
    return val;

} // name2Val

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumeration::setEntry( SEnumEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    if( i_pEntry->m_iEnumerator >= m_arpEntries.size() )
    {
        m_arpEntries.resize(i_pEntry->m_iEnumerator+1);
    }

    m_arpEntries[i_pEntry->m_iEnumerator] = i_pEntry;

    i_pEntry->m_iEnumerator = i_pEntry->m_iEnumerator;

    if( i_pEntry->m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_pEntry->m_iEnumerator;
    }
    if( i_pEntry->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_pEntry->m_strlstNames.size();
    }

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry( const SEnumEntry& i_entry )
//------------------------------------------------------------------------------
{
    if( i_entry.m_iEnumerator >= m_arpEntries.size() )
    {
        m_arpEntries.resize(i_entry.m_iEnumerator+1);
    }

    SEnumEntry* pEntry = m_arpEntries[i_entry.m_iEnumerator];

    *pEntry = i_entry;

    pEntry->m_iEnumerator = i_entry.m_iEnumerator;

    if( i_entry.m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_entry.m_iEnumerator;
    }
    if( i_entry.m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_entry.m_strlstNames.size();
    }

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( i_idx >= m_arpEntries.size() )
    {
        m_arpEntries.resize(i_idx+1);
    }

    SEnumEntry* pEntry = m_arpEntries[i_idx];

    pEntry->m_iEnumerator = i_idx;
    pEntry->m_validity    = i_validity;
    pEntry->m_val         = i_val;

    pEntry->m_strlstNames.append(QString::number(i_idx));

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( i_idx >= m_arpEntries.size() )
    {
        m_arpEntries.resize(i_idx+1);
    }

    SEnumEntry* pEntry = m_arpEntries[i_idx];

    pEntry->m_iEnumerator = i_idx;
    pEntry->m_validity    = i_validity;
    pEntry->m_val         = i_val;

    pEntry->m_strlstNames.append(i_strName);

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QString&     i_strAlias10,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);
    strlstVals.append(i_strAlias10);

    setEntry(i_idx, strlstVals, i_val, i_validity);

} // setEntry

//------------------------------------------------------------------------------
void CEnumeration::setEntry(
    int                i_idx,
    const QStringList& i_strlstValNames,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( i_idx >= m_arpEntries.size() )
    {
        m_arpEntries.resize(i_idx+1);
    }

    SEnumEntry* pEntry = m_arpEntries[i_idx];

    pEntry->m_iEnumerator = i_idx;
    pEntry->m_strlstNames = i_strlstValNames;
    pEntry->m_validity    = i_validity;
    pEntry->m_val         = i_val;

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( i_strlstValNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_strlstValNames.size();
    }

} // setEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumeration::appendEntry( SEnumEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    i_pEntry->m_iEnumerator = m_arpEntries.size();

    if( i_pEntry->m_iEnumerator < 0 )
    {
        i_pEntry->m_iEnumerator = i_pEntry->m_iEnumerator;
    }

    m_arpEntries.append(i_pEntry);

    if( i_pEntry->m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_pEntry->m_iEnumerator;
    }
    if( i_pEntry->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_pEntry->m_strlstNames.size();
    }

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry( const SEnumEntry& i_entry )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_entry);

    pEntry->m_iEnumerator = m_arpEntries.size();

    m_arpEntries.append(pEntry);

    if( i_entry.m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_entry.m_iEnumerator;
    }
    if( i_entry.m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_entry.m_strlstNames.size();
    }

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_val, i_validity);

    pEntry->m_iEnumerator = m_arpEntries.size();

    m_arpEntries.append(pEntry);

    pEntry->m_strlstNames.append(QString::number(i_idx));

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_strName, i_val, i_validity);

    pEntry->m_iEnumerator = m_arpEntries.size();

    m_arpEntries.append(pEntry);

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QString&     i_strAlias10,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);
    strlstVals.append(i_strAlias10);

    appendEntry(i_idx, strlstVals, i_val, i_validity);

} // appendEntry

//------------------------------------------------------------------------------
void CEnumeration::appendEntry(
    int                i_idx,
    const QStringList& i_strlstValNames,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_strlstValNames, i_val, i_validity);

    pEntry->m_iEnumerator = m_arpEntries.size();

    m_arpEntries.append(pEntry);

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( i_strlstValNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_strlstValNames.size();
    }

} // appendEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumeration::insertEntry( int i_idxRow, SEnumEntry* i_pEntry )
//------------------------------------------------------------------------------
{
    m_arpEntries.insert(i_idxRow,i_pEntry);

    i_pEntry->m_iEnumerator = i_idxRow;

    if( i_pEntry->m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_pEntry->m_iEnumerator;
    }
    if( i_pEntry->m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_pEntry->m_strlstNames.size();
    }

    SEnumEntry* pEntry;
    int         idxRow;

    for( idxRow = i_idxRow+1; idxRow < m_arpEntries.count(); idxRow++ )
    {
        pEntry = m_arpEntries[idxRow];
        pEntry->m_iEnumerator = idxRow;
    }

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry( int i_idxRow, const SEnumEntry& i_entry )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_entry);

    m_arpEntries.insert(i_idxRow,pEntry);

    pEntry->m_iEnumerator = i_idxRow;

    if( i_entry.m_iEnumerator > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_entry.m_iEnumerator;
    }
    if( i_entry.m_strlstNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_entry.m_strlstNames.size();
    }

    for( int idxRow = i_idxRow+1; idxRow < m_arpEntries.count(); idxRow++ )
    {
        pEntry = m_arpEntries[idxRow];
        pEntry->m_iEnumerator = idxRow;
    }

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_val, i_validity);

    m_arpEntries.insert(i_idxRow,pEntry);

    pEntry->m_iEnumerator = i_idxRow;

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

    for( int idxRow = i_idxRow+1; idxRow < m_arpEntries.count(); idxRow++ )
    {
        pEntry = m_arpEntries[idxRow];
        pEntry->m_iEnumerator = idxRow;
    }

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_strName, i_val, i_validity);

    m_arpEntries.insert(i_idxRow,pEntry);

    pEntry->m_iEnumerator = i_idxRow;

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( m_iMaxEnumEntryStrAliasCount < 1 )
    {
        m_iMaxEnumEntryStrAliasCount = 1;
    }

    for( int idxRow = i_idxRow+1; idxRow < m_arpEntries.count(); idxRow++ )
    {
        pEntry = m_arpEntries[idxRow];
        pEntry->m_iEnumerator = idxRow;
    }

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QString&     i_strName,
    const QString&     i_strAliasSymbol,
    const QString&     i_strAliasText,
    const QString&     i_strAliasSCPIShort,
    const QString&     i_strAliasSCPILong,
    const QString&     i_strAlias6,
    const QString&     i_strAlias7,
    const QString&     i_strAlias8,
    const QString&     i_strAlias9,
    const QString&     i_strAlias10,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    QStringList strlstVals;

    strlstVals.append(i_strName);
    strlstVals.append(i_strAliasSymbol);
    strlstVals.append(i_strAliasText);
    strlstVals.append(i_strAliasSCPIShort);
    strlstVals.append(i_strAliasSCPILong);
    strlstVals.append(i_strAlias6);
    strlstVals.append(i_strAlias7);
    strlstVals.append(i_strAlias8);
    strlstVals.append(i_strAlias9);
    strlstVals.append(i_strAlias10);

    insertEntry(i_idxRow, i_idx, strlstVals, i_val, i_validity);

} // insertEntry

//------------------------------------------------------------------------------
void CEnumeration::insertEntry(
    int                i_idxRow,
    int                i_idx,
    const QStringList& i_strlstValNames,
    const QVariant&    i_val,
    EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = new SEnumEntry(i_idx, i_strlstValNames, i_val, i_validity);

    m_arpEntries.insert(i_idxRow,pEntry);

    pEntry->m_iEnumerator = i_idxRow;

    if( i_idx > m_iMaxEnumEntryIndex )
    {
        m_iMaxEnumEntryIndex = i_idx;
    }
    if( i_strlstValNames.size() > m_iMaxEnumEntryStrAliasCount )
    {
        m_iMaxEnumEntryStrAliasCount = i_strlstValNames.size();
    }

    for( int idxRow = i_idxRow+1; idxRow < m_arpEntries.count(); idxRow++ )
    {
        pEntry = m_arpEntries[idxRow];
        pEntry->m_iEnumerator = idxRow;
    }

} // insertEntry

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumeration::removeEntryByIdx( int i_idx )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = index2EnumEntry(i_idx);

    if( pEntry != nullptr )
    {
        removeEntry(pEntry->m_iEnumerator);
    }

} // removeEntryByIdx

//------------------------------------------------------------------------------
void CEnumeration::removeEntryByVal( const QVariant& i_val )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = val2EnumEntry(i_val);

    if( pEntry != nullptr )
    {
        removeEntry(pEntry->m_iEnumerator);
    }

} // removeEntryByVal

//------------------------------------------------------------------------------
void CEnumeration::removeEntryByName(
    const QString&      i_strName,
    Qt::CaseSensitivity i_caseSensitive,
    int                 i_alias )
//------------------------------------------------------------------------------
{
    SEnumEntry* pEntry = name2EnumEntry(i_strName,i_caseSensitive,i_alias);

    if( pEntry != nullptr )
    {
        removeEntry(pEntry->m_iEnumerator);
    }

} // removeEntryByName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumeration::clear() // keeps Name, Id and UseValueNames
//------------------------------------------------------------------------------
{
    if( m_arpEntries.size() > 0 )
    {
        SEnumEntry* pEntry;
        int         idx;

        for( idx = m_arpEntries.size()-1; idx >= 0; idx-- )
        {
            pEntry = m_arpEntries[idx];

            try
            {
                delete pEntry;
            }
            catch(...)
            {
            }
            pEntry = nullptr;
            m_arpEntries[idx] = nullptr;
        }
    }

    m_arpEntries.clear();

    m_bMaxEnumEntryIndexDirty = false;
    m_iMaxEnumEntryIndex = -1;
    m_bMaxEnumEntryStrAliasCountDirty = false;
    m_iMaxEnumEntryStrAliasCount = 0;

} // clear
