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

#include "ZSSys/ZSSysEnumValArr.h"
#include "ZSSys/ZSSysEnumeration.h"
#include "ZSSys/ZSSysException.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CEnumValArr
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr() :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(),
    m_ariEnumEntryIdx(),
    m_arValidities(),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( int i_iValCount ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_iValCount),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(),
    m_arValidities(),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, int i_iValCount ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_iValCount),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, int i_iEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_iValCount,i_iEnumEntryIdx),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = 0; idxVal < m_ariEnumEntryIdx.size(); idxVal++ )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry != nullptr )
            {
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_iValCount,-1),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = 0; idxVal < m_ariEnumEntryIdx.size(); idxVal++ )
        {
            pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_alias);

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, int i_iValCount, const QVariant& i_valEnumEntry ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_iValCount,-1),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = 0; idxVal < m_ariEnumEntryIdx.size(); idxVal++ )
        {
            pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, const QVector<int>& i_ariEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_ariEnumEntryIdx),
    m_arValidities(i_ariEnumEntryIdx.size(), static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = 0; idxVal < m_ariEnumEntryIdx.size(); idxVal++ )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry != nullptr )
            {
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, const QStringList& i_strlstIdxNames, Qt::CaseSensitivity i_caseSensitivity, int i_alias ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_strlstIdxNames.size()),
    m_arValidities(i_strlstIdxNames.size(), static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    SEnumEntry* pEntry;
    int         idxVal;

    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        for( idxVal = 0; idxVal < i_strlstIdxNames.size(); idxVal++ )
        {
            pEntry = m_pEnum->name2EnumEntry( i_strlstIdxNames[idxVal], i_caseSensitivity, i_alias );

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( CEnumeration* i_pEnum, const QVector<QVariant>& i_arValsEnumEntries ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_arValsEnumEntries.size()),
    m_arValidities(i_arValsEnumEntries.size(), static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
    SEnumEntry* pEntry;
    int         idxVal;

    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        for( idxVal = 0; idxVal < i_arValsEnumEntries.size(); idxVal++ )
        {
            pEntry = m_pEnum->val2EnumEntry(i_arValsEnumEntries[idxVal]);

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( int i_iValCount, const CEnumVal& i_enumVal ) :
//------------------------------------------------------------------------------
    m_pEnum(i_enumVal.getEnumeration()),
    m_strEnumUId(i_enumVal.getEnumerationUId()),
    m_ariEnumEntryIdx(i_iValCount,i_enumVal.getEnumEntryIndex()),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Valid)),
    m_ararEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(i_enumVal.getEnumEntryIndex());

        if( pEntry == nullptr )
        {
            m_arValidities = QVector<quint8>(i_iValCount, static_cast<int>(EValueValidity::Invalid));
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const QVector<CEnumVal>& i_arEnumVals ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(),
    m_ariEnumEntryIdx(i_arEnumVals.size(),-1),
    m_arValidities(i_arEnumVals.size(), static_cast<int>(EValueValidity::Valid)),
    m_ararEnumEntryValidities()
{
    if( i_arEnumVals.size() > 0 )
    {
        m_pEnum = i_arEnumVals[0].getEnumeration();
        m_strEnumUId = i_arEnumVals[0].getEnumerationUId();
    }

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = 0; idxVal < i_arEnumVals.size(); idxVal++ )
        {
            pEntry = m_pEnum->val2EnumEntry( i_arEnumVals[idxVal].getEnumEntryIndex() );

            if( pEntry == nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);
            }
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const QString& i_strEnumUId ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_ariEnumEntryIdx(),
    m_arValidities(),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const QString& i_strEnumUId, int i_iValCount ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_ariEnumEntryIdx(i_iValCount),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Invalid)),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const QString& i_strEnumUId, int i_iValCount, int i_iEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_ariEnumEntryIdx(i_iValCount,i_iEnumEntryIdx),
    m_arValidities(i_iValCount, static_cast<int>(EValueValidity::Valid)),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const QString& i_strEnumUId, const QVector<int>& i_ariEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_ariEnumEntryIdx(i_ariEnumEntryIdx),
    m_arValidities(i_ariEnumEntryIdx.size(), static_cast<int>(EValueValidity::Valid)),
    m_ararEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumValArr::CEnumValArr( const CEnumValArr& i_other ) :
//------------------------------------------------------------------------------
    m_pEnum(i_other.m_pEnum),
    m_strEnumUId(i_other.m_strEnumUId),
    m_ariEnumEntryIdx(i_other.m_ariEnumEntryIdx),
    m_arValidities(i_other.m_arValidities),
    m_ararEnumEntryValidities(i_other.m_ararEnumEntryValidities)
{
} // copy ctor

//------------------------------------------------------------------------------
CEnumValArr::~CEnumValArr()
//------------------------------------------------------------------------------
{
    m_pEnum = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CEnumValArr::operator == ( const CEnumValArr& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_strEnumUId != i_other.m_strEnumUId )
    {
        bEqual = false;
    }
    else if( m_ariEnumEntryIdx.size() != i_other.m_ariEnumEntryIdx.size() )
    {
        bEqual = false;
    }
    else if( m_arValidities.size() != i_other.m_arValidities.size() )
    {
        bEqual = false;
    }
    else if( m_ararEnumEntryValidities.size() != i_other.m_ararEnumEntryValidities.size() )
    {
        bEqual = false;
    }
    else if( memcmp( m_ariEnumEntryIdx.data(), i_other.m_ariEnumEntryIdx.data(), m_ariEnumEntryIdx.size()*sizeof(m_ariEnumEntryIdx[0]) ) != 0 )
    {
        bEqual = false;
    }
    else if( memcmp( m_arValidities.data(), i_other.m_arValidities.data(), m_arValidities.size()*sizeof(m_arValidities.data()[0]) ) != 0 )
    {
        bEqual = false;
    }
    else
    {
        for( int idxVal = 0; idxVal < m_ararEnumEntryValidities.size(); idxVal++ )
        {
            const quint8* puThis  = m_ararEnumEntryValidities[idxVal].data();
            const quint8* puOther = i_other.m_ararEnumEntryValidities[idxVal].data();
            size_t        iSize   = m_ararEnumEntryValidities[idxVal].size()*sizeof(m_ararEnumEntryValidities[idxVal].data()[0]);

            if( memcmp(puThis,puOther,iSize) != 0)
            {
                bEqual = false;
                break;
            }
        }
    }

    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CEnumValArr::operator != ( const CEnumValArr& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CEnumValArr& CEnumValArr::operator = ( const CEnumValArr& i_other )
//------------------------------------------------------------------------------
{
    if( size() != i_other.size() )
    {
        resize( i_other.size() );
    }

    setValues( 0, i_other );

    return *this;

} // operator =

//------------------------------------------------------------------------------
CEnumValArr& CEnumValArr::operator = ( const QVector<CEnumVal>& i_arEnumVals )
//------------------------------------------------------------------------------
{
    if( size() != i_arEnumVals.size() )
    {
        resize( i_arEnumVals.size() );
    }

    setValues( 0, i_arEnumVals );

    return *this;

} // operator =

//------------------------------------------------------------------------------
CEnumValArr& CEnumValArr::operator = ( const QVector<int>& i_ariEnumEntryIdx )
//------------------------------------------------------------------------------
{
    if( size() != i_ariEnumEntryIdx.size() )
    {
        resize( i_ariEnumEntryIdx.size() );
    }

    setValuesByEnumEntryIndices( 0, i_ariEnumEntryIdx );

    return *this;

} // operator =

//------------------------------------------------------------------------------
CEnumValArr& CEnumValArr::operator = ( const QStringList& i_strlstIdxNames )
//------------------------------------------------------------------------------
{
    if( size() != i_strlstIdxNames.size() )
    {
        resize( i_strlstIdxNames.size() );
    }

    setValuesByEnumEntryNames( 0, i_strlstIdxNames );

    return *this;

} // operator =

//------------------------------------------------------------------------------
CEnumValArr& CEnumValArr::operator = ( const QVector<QVariant>& i_arValsEnumEntries )
//------------------------------------------------------------------------------
{
    if( size() != i_arValsEnumEntries.size() )
    {
        resize( i_arValsEnumEntries.size() );
    }

    setValuesByEnumEntryVals( 0, i_arValsEnumEntries );

    return *this;

} // operator =

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setEnumeration( CEnumeration* i_pEnum )
//------------------------------------------------------------------------------
{
    if( m_pEnum != i_pEnum )
    {
        m_pEnum = i_pEnum;
        m_strEnumUId = "";

        m_arValidities.fill(static_cast<int>(EValueValidity::Invalid));
        m_ararEnumEntryValidities.clear();

        if( m_pEnum != nullptr )
        {
            m_strEnumUId = m_pEnum->keyInTree();

            SEnumEntry* pEntry;
            int         idxVal;

            for( idxVal = 0; idxVal < size(); idxVal++ )
            {
                pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

                if( pEntry != nullptr )
                {
                    m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
                }
            }
        }
    } // if( m_pEnum != i_pEnum )

} // setEnumeration

//------------------------------------------------------------------------------
void CEnumValArr::setEnumerationUId( const QString& i_strEnumUId )
//------------------------------------------------------------------------------
{
    if( m_pEnum != nullptr && m_pEnum->keyInTree() != i_strEnumUId )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CEnumValArr::" + getEnumerationUId() + ".setEnumerationUId";
        strArgs   = "EnumKey: " + i_strEnumUId;
        strErr    = "EnumKey is different from key of already set enumeration " + m_pEnum->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strEnumUId = i_strEnumUId;

} // setEnumerationUId

//------------------------------------------------------------------------------
QString CEnumValArr::getEnumerationName() const
//------------------------------------------------------------------------------
{
    QString strName;

    if( m_pEnum != nullptr )
    {
        strName = m_pEnum->getName();
    }
    return strName;

} // getEnumerationName

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_pEnum = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::resize( int i_iSize )
//------------------------------------------------------------------------------
{
    int iSizePrev = size();

    m_ariEnumEntryIdx.resize(i_iSize);
    m_arValidities.resize(i_iSize);

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.resize(i_iSize);
    }

    if( i_iSize > iSizePrev )
    {
        memset( &m_arValidities.data()[iSizePrev], static_cast<int>(EValueValidity::Invalid), (i_iSize-iSizePrev)*sizeof(m_arValidities.data()[0]) );
    }

} // resize

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::clear()
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx.clear();
    m_arValidities.clear();
    m_ararEnumEntryValidities.clear();

} // clear

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CEnumValArr::isValid( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    return (m_arValidities[i_idxVal] == static_cast<int>(EValueValidity::Valid));
}

//------------------------------------------------------------------------------
bool CEnumValArr::isNull( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    return (m_arValidities[i_idxVal] == static_cast<int>(EValueValidity::Null));
}

//------------------------------------------------------------------------------
void CEnumValArr::setValidity( int i_idxVal, EValueValidity i_validity )
//------------------------------------------------------------------------------
{
    m_arValidities[i_idxVal] = static_cast<int>(i_validity);
}

//------------------------------------------------------------------------------
EValueValidity CEnumValArr::getValidity( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    return static_cast<EValueValidity>(m_arValidities[i_idxVal]);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setValidities( const QVector<quint8>& i_arValidities )
//------------------------------------------------------------------------------
{
    m_arValidities = i_arValidities;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setEnumEntryValidityByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx, EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    if( i_iEnumEntryIdx >= 0 && i_iEnumEntryIdx <= m_pEnum->count() )
    {
        if( m_ararEnumEntryValidities.size() < size() )
        {
            m_ararEnumEntryValidities.resize( size() );
        }
        if( m_ararEnumEntryValidities[i_idxVal].size() != m_pEnum->count() )
        {
            m_ararEnumEntryValidities[i_idxVal] = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
        }

        m_ararEnumEntryValidities[i_idxVal][i_iEnumEntryIdx] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumValArr::setEnumEntryValidityByEnumEntryName( int i_idxVal, const QString& i_strName, EEnumEntryValidity i_validity, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    int idxEnumEntry = m_pEnum->name2Index(i_strName,i_caseSensitivity,i_alias);

    if( idxEnumEntry >= 0 && idxEnumEntry < m_pEnum->count() )
    {
        if( m_ararEnumEntryValidities.size() < size() )
        {
            m_ararEnumEntryValidities.resize( size() );
        }
        if( m_ararEnumEntryValidities[i_idxVal].size() != m_pEnum->count() )
        {
            m_ararEnumEntryValidities[i_idxVal] = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
        }

        m_ararEnumEntryValidities[i_idxVal][idxEnumEntry] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryName

//------------------------------------------------------------------------------
void CEnumValArr::setEnumEntryValidityByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry, EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    int idxEnumEntry = m_pEnum->val2Index(i_valEnumEntry);

    if( idxEnumEntry >= 0 && idxEnumEntry < m_pEnum->count() )
    {
        if( m_ararEnumEntryValidities.size() < size() )
        {
            m_ararEnumEntryValidities.resize( size() );
        }
        if( m_ararEnumEntryValidities[i_idxVal].size() != m_pEnum->count() )
        {
            m_ararEnumEntryValidities[i_idxVal] = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
        }

        m_ararEnumEntryValidities[i_idxVal][idxEnumEntry] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumValArr::setEnumEntryValidities( int i_idxVal, const QVector<quint8> i_arValidities )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }
    if( m_pEnum->count() != i_arValidities.size() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Number of enum validites must be equal to number of enum entries" );
    }

    if( m_ararEnumEntryValidities.size() < size() )
    {
        m_ararEnumEntryValidities.resize( size() );
    }
    m_ararEnumEntryValidities[i_idxVal] = i_arValidities;

} // setEnumEntryValidities

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumValArr::getEnumEntryValidityByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx ) const
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityNormal;

    if( i_idxVal >= 0 && i_idxVal < m_ararEnumEntryValidities.size() )
    {
        if( i_iEnumEntryIdx >= 0 && i_iEnumEntryIdx < m_ararEnumEntryValidities[i_idxVal].size() )
        {
            validity = static_cast<EEnumEntryValidity>(m_ararEnumEntryValidities[i_idxVal][i_iEnumEntryIdx]);
        }
    }
    return validity;

} // getEnumEntryValidityByEnumEntryIndex

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumValArr::getEnumEntryValidityByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias ) const
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only retrieve validities of enum entries if enum is known" );
    }

    EEnumEntryValidity validity = EEnumEntryValidityNormal;

    if( i_idxVal >= 0 && i_idxVal < m_ararEnumEntryValidities.size() )
    {
        int idxEnumEntry = m_pEnum->name2Index(i_strName,i_caseSensitivity,i_alias);

        if( idxEnumEntry >= 0 && idxEnumEntry < m_ararEnumEntryValidities[i_idxVal].size() )
        {
            validity = static_cast<EEnumEntryValidity>(m_ararEnumEntryValidities[i_idxVal][idxEnumEntry]);
        }
    }
    return validity;

} // getEnumEntryValidityByEnumEntryName

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumValArr::getEnumEntryValidityByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry ) const
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only retrieve validities of enum entries if enum is known" );
    }

    EEnumEntryValidity validity = EEnumEntryValidityNormal;

    if( i_idxVal >= 0 && i_idxVal < m_ararEnumEntryValidities.size() )
    {
        int idxEnumEntry = m_pEnum->val2Index(i_valEnumEntry);

        if( idxEnumEntry >= 0 && idxEnumEntry < m_ararEnumEntryValidities[i_idxVal].size() )
        {
            validity = static_cast<EEnumEntryValidity>(m_ararEnumEntryValidities[i_idxVal][idxEnumEntry]);
        }
    }
    return validity;

} // getEnumEntryValidityByEnumEntryVal

//------------------------------------------------------------------------------
QVector<quint8> CEnumValArr::getEnumEntryValidities( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    QVector<quint8> arValidities;

    if( i_idxVal >= 0 && i_idxVal < m_ararEnumEntryValidities.size() )
    {
        arValidities = m_ararEnumEntryValidities[i_idxVal];
    }
    return arValidities;

} // getEnumEntryValidities

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setValByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx )
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx[i_idxVal] = i_iEnumEntryIdx;

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(i_iEnumEntryIdx);

        if( pEntry == nullptr )
        {
            m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Invalid);
        }
        else
        {
            m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Valid);
        }
    }
    else
    {
        m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Valid);
    }

} // setValByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumValArr::setValByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set index value by name of enum entries if enum is known" );
    }

    SEnumEntry* pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_alias);

    if( pEntry != nullptr )
    {
        m_ariEnumEntryIdx[i_idxVal] = pEntry->m_iEnumerator;
        m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Valid);
    }
    else
    {
        m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Invalid);
    }

} // setValByEnumEntryName

//------------------------------------------------------------------------------
void CEnumValArr::setValByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set index value by value of enum entries if enum is known" );
    }

    m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Invalid);

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

        if( pEntry != nullptr )
        {
            m_ariEnumEntryIdx[i_idxVal] = pEntry->m_iEnumerator;
            m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Valid);
        }
    }

} // setValByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumValArr::setVal( int i_idxVal, const CEnumVal& i_enumVal )
//------------------------------------------------------------------------------
{
    if( m_strEnumUId != i_enumVal.getEnumerationUId() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
    }

    if( m_pEnum == nullptr )
    {
        m_pEnum = i_enumVal.getEnumeration();
    }

    int iEnumEntryIdx = i_enumVal.getEnumEntryIndex();

    m_ariEnumEntryIdx[i_idxVal] = iEnumEntryIdx;

    if( m_pEnum != nullptr && iEnumEntryIdx >= 0 )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[i_idxVal]);

        if( pEntry == nullptr )
        {
            m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Invalid);
        }
        else
        {
            m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Valid);
        }
    }
    else
    {
        m_arValidities[i_idxVal] = static_cast<int>(EValueValidity::Invalid);
    }

} // setVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::insertValByEnumEntryIndex( int i_idxVal, int i_iEnumEntryIdx )
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx.insert( i_idxVal, i_iEnumEntryIdx );

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(i_iEnumEntryIdx);

        if( pEntry == nullptr )
        {
            m_arValidities.insert(i_idxVal, static_cast<int>(EValueValidity::Invalid) );
        }
        else
        {
            m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Valid) );
        }
    }
    else
    {
        m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Valid) );
    }

} // insertValByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumValArr::insertValByEnumEntryName( int i_idxVal, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set index value by name of enum entries if enum is known" );
    }

    SEnumEntry* pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_alias);

    if( pEntry != nullptr )
    {
        m_ariEnumEntryIdx.insert( i_idxVal, pEntry->m_iEnumerator );
        m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Valid) );
    }
    else
    {
        m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Invalid) );
    }

} // insertValByEnumEntryName

//------------------------------------------------------------------------------
void CEnumValArr::insertValByEnumEntryVal( int i_idxVal, const QVariant& i_valEnumEntry )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set index value by value of enum entries if enum is known" );
    }

    m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Invalid) );

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

        if( pEntry != nullptr )
        {
            m_ariEnumEntryIdx.insert( i_idxVal, pEntry->m_iEnumerator );
            m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Valid) );
        }
    }

} // insertValByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumValArr::insertVal( int i_idxVal, const CEnumVal& i_enumVal )
//------------------------------------------------------------------------------
{
    if( m_strEnumUId != i_enumVal.getEnumerationUId() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
    }

    if( m_pEnum == nullptr )
    {
        m_pEnum = i_enumVal.getEnumeration();
    }

    int iEnumEntryIdx = i_enumVal.getEnumEntryIndex();

    m_ariEnumEntryIdx.insert( i_idxVal, iEnumEntryIdx );

    if( m_pEnum != nullptr && iEnumEntryIdx >= 0 )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[i_idxVal]);

        if( pEntry == nullptr )
        {
            m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Invalid) );
        }
        else
        {
            m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Valid) );
        }
    }
    else
    {
        m_arValidities.insert( i_idxVal, static_cast<int>(EValueValidity::Invalid) );
    }

} // insertVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::appendValByEnumEntryIndex( int i_iEnumEntryIdx )
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx.append(i_iEnumEntryIdx);

    SEnumEntry* pEntry = nullptr;

    if( m_pEnum != nullptr )
    {
        pEntry = m_pEnum->index2EnumEntry(i_iEnumEntryIdx);

        if( pEntry != nullptr )
        {
            m_arValidities.append(static_cast<int>(EValueValidity::Valid));
        }
        else
        {
            m_arValidities.append(static_cast<int>(EValueValidity::Invalid));
        }
    }
    else
    {
        m_arValidities.append(static_cast<int>(EValueValidity::Valid));
    }

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.append( QVector<quint8>() );
    }

} // appendValByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumValArr::appendValByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by name of enum entries if enum is known" );
    }

    SEnumEntry* pEntry = nullptr;

    if( m_pEnum != nullptr )
    {
        pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_alias);
    }

    if( pEntry != nullptr )
    {
        m_ariEnumEntryIdx.append( pEntry->m_iEnumerator );
        m_arValidities.append(static_cast<int>(EValueValidity::Valid));
    }
    else
    {
        m_ariEnumEntryIdx.append(-1);
        m_arValidities.append(static_cast<int>(EValueValidity::Invalid));
    }

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.append( QVector<quint8>() );
    }

} // appendValByEnumEntryName

//------------------------------------------------------------------------------
void CEnumValArr::appendValByEnumEntryVal( const QVariant& i_valEnumEntry )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by name of enum entries if enum is known" );
    }

    SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

    if( pEntry != nullptr )
    {
        m_ariEnumEntryIdx.append(pEntry->m_iEnumerator);
        m_arValidities.append(static_cast<int>(EValueValidity::Valid));
    }
    else
    {
        m_ariEnumEntryIdx.append(-1);
        m_arValidities.append(static_cast<int>(EValueValidity::Invalid));
    }

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.append( QVector<quint8>() );
    }

} // appendValByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumValArr::appendVal( const CEnumVal& i_enumVal )
//------------------------------------------------------------------------------
{
    if( m_strEnumUId != i_enumVal.getEnumerationUId() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
    }

    if( m_pEnum == nullptr )
    {
        m_pEnum = i_enumVal.getEnumeration();
    }

    m_ariEnumEntryIdx.append(i_enumVal.getEnumEntryIndex());

    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(i_enumVal.getEnumEntryIndex());

        if( pEntry != nullptr )
        {
            m_arValidities.append(static_cast<int>(EValueValidity::Valid));
        }
        else
        {
            m_arValidities.append(static_cast<int>(EValueValidity::Invalid));
        }
    }
    else
    {
        m_arValidities.append(static_cast<int>(EValueValidity::Valid));
    }

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.append( QVector<quint8>() );
    }

} // appendVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::removeVal( int i_idxVal )
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx.remove(i_idxVal);
    m_arValidities.remove(i_idxVal);

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.remove(i_idxVal);
    }

} // removeVal

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryIndex( int i_idxStart, int i_iValCount, int i_iEnumEntryIdx )
//------------------------------------------------------------------------------
{
    int idxStart  = toArrayIndex( i_idxStart, size() );
    int iValCount = toArrayLen( i_iValCount, size() );

    if( idxStart >= 0 && iValCount > 0 )
    {
        if( idxStart + iValCount > size() )
        {
            resize(idxStart+iValCount);
        }

        QVector<int> ariIdxs(iValCount,i_iEnumEntryIdx);
        memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );
        memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Valid), iValCount );

        if( m_pEnum != nullptr )
        {
            SEnumEntry* pEntry;
            int         idxVal;

            for( idxVal = idxStart; idxVal < iValCount; idxVal++ )
            {
                pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

                if( pEntry == nullptr )
                {
                    m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);
                }
            }
        }
    }

} // setValuesByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryName( int i_idxStart, int i_iValCount, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by name of enum entries if enum is known" );
    }

    int idxStart  = toArrayIndex( i_idxStart, size() );
    int iValCount = toArrayLen( i_iValCount, size() );

    if( idxStart >= 0 && iValCount > 0 )
    {
        if( idxStart + iValCount > size() )
        {
            resize(idxStart+iValCount);
        }

        SEnumEntry* pEntry = m_pEnum->name2EnumEntry( i_strName, i_caseSensitivity, i_alias );

        if( pEntry == nullptr )
        {
            QVector<int> ariIdxs(iValCount,-1);
            memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );
            memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Invalid), iValCount );
        }
        else // if( pEntry != nullptr )
        {
            QVector<int> ariIdxs(iValCount,pEntry->m_iEnumerator);
            memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );
            memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Valid), iValCount );
        }
    }

} // setValuesByEnumEntryName

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryVal( int i_idxStart, int i_iValCount, const QVariant& i_valEnumEntry )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by values of enum entries if enum is known" );
    }

    int idxStart  = toArrayIndex( i_idxStart, size() );
    int iValCount = toArrayLen( i_iValCount, size() );

    if( idxStart >= 0 && iValCount > 0 )
    {
        if( idxStart + iValCount > size() )
        {
            resize(idxStart+iValCount);
        }

        SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

        if( pEntry == nullptr )
        {
            QVector<int> ariIdxs(iValCount,-1);
            memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );
            memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Invalid), iValCount );
        }
        else // if( pEntry != nullptr )
        {
            QVector<int> ariIdxs(iValCount,pEntry->m_iEnumerator);
            memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );
            memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Valid), iValCount );
        }
    }

} // setValuesByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumValArr::setValues( int i_idxStart, int i_iValCount, const CEnumVal& i_enumVal )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        m_pEnum = i_enumVal.getEnumeration();

        if( m_pEnum != nullptr )
        {
            m_strEnumUId = m_pEnum->keyInTree();
        }
    }
    if( m_strEnumUId != i_enumVal.getEnumerationUId() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
    }

    int idxStart  = toArrayIndex( i_idxStart, size() );
    int iValCount = toArrayLen( i_iValCount, size() );

    if( idxStart >= 0 && iValCount > 0 )
    {
        if( idxStart + iValCount > size() )
        {
            resize(idxStart+iValCount);
        }

        QVector<int> ariIdxs( iValCount, i_enumVal.getEnumEntryIndex() );
        memcpy( &m_ariEnumEntryIdx.data()[idxStart], ariIdxs.data(), iValCount*sizeof(m_ariEnumEntryIdx.data()[0]) );

        SEnumEntry* pEntry = nullptr;

        if( m_pEnum != nullptr )
        {
            pEntry = m_pEnum->index2EnumEntry(i_enumVal.getEnumEntryIndex());

            if( pEntry == nullptr )
            {
                memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Invalid), iValCount );
            }
            else
            {
                memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Valid), iValCount );
            }
        }
        else
        {
            memset( &m_arValidities.data()[idxStart], static_cast<int>(EValueValidity::Valid), iValCount );
        }
    }

} // setValues

/*==============================================================================
public: // instance methods (to set values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryIndices( int i_idxStart, const QVector<int>& i_ariEnumEntryIdx )
//------------------------------------------------------------------------------
{
    int idxStart = toArrayIndex( i_idxStart, size() );

    if( idxStart + i_ariEnumEntryIdx.size() > size() )
    {
        resize( idxStart + i_ariEnumEntryIdx.size() );
    }

    SEnumEntry* pEntry;
    int         idxVal;

    for( idxVal = i_idxStart; idxVal < (i_idxStart + i_ariEnumEntryIdx.size()); idxVal++ )
    {
        m_ariEnumEntryIdx[idxVal] = i_ariEnumEntryIdx[idxVal-i_idxStart];
        m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);

        if( m_pEnum != nullptr )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry == nullptr )
            {
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);
            }
        }
    }

} // setValuesByEnumEntryIndices

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryNames( int i_idxStart, const QStringList& i_strlstIdxNames, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by name of enum entries if enum is known" );
    }

    int idxStart = toArrayIndex( i_idxStart, size() );

    if( idxStart + i_strlstIdxNames.size() > size() )
    {
        resize( idxStart + i_strlstIdxNames.size() );
    }

    SEnumEntry* pEntry;
    int         idxVal;

    for( idxVal = i_idxStart; idxVal < (i_idxStart + i_strlstIdxNames.size()); idxVal++ )
    {
        m_ariEnumEntryIdx[idxVal] = -1;
        m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);

        if( m_pEnum != nullptr )
        {
            pEntry = m_pEnum->name2EnumEntry( i_strlstIdxNames[idxVal-i_idxStart], i_caseSensitivity, i_alias );

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // setValuesByEnumEntryNames

//------------------------------------------------------------------------------
void CEnumValArr::setValuesByEnumEntryVals( int i_idxStart, const QVector<QVariant>& i_arValsEnumEntries )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set value by values of enum entries if enum is known" );
    }

    int idxStart = toArrayIndex( i_idxStart, size() );

    if( idxStart + i_arValsEnumEntries.size() > size() )
    {
        resize( idxStart + i_arValsEnumEntries.size() );
    }

    SEnumEntry* pEntry;
    int         idxVal;

    for( idxVal = i_idxStart; idxVal < (i_idxStart + i_arValsEnumEntries.size()); idxVal++ )
    {
        m_ariEnumEntryIdx[idxVal] = -1;
        m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);

        if( m_pEnum != nullptr )
        {
            pEntry = m_pEnum->val2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry != nullptr )
            {
                m_ariEnumEntryIdx[idxVal] = pEntry->m_iEnumerator;
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
    }

} // setValuesByEnumEntryVals

//------------------------------------------------------------------------------
void CEnumValArr::setValues( int i_idxStart, const QVector<CEnumVal>& i_arEnumVals )
//------------------------------------------------------------------------------
{
    int idxStart = toArrayIndex( i_idxStart, size() );

    if( idxStart + i_arEnumVals.size() > size() )
    {
        resize( idxStart + i_arEnumVals.size() );
    }

    SEnumEntry* pEntry;
    int         idxVal;

    for( idxVal = i_idxStart; idxVal < (i_idxStart + i_arEnumVals.size()); idxVal++ )
    {
        m_ariEnumEntryIdx[idxVal] = i_arEnumVals[idxVal-i_idxStart].getEnumEntryIndex();
        m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);

        if( m_pEnum == nullptr )
        {
            m_pEnum = i_arEnumVals[idxVal-i_idxStart].getEnumeration();

            if( m_pEnum != nullptr )
            {
                m_strEnumUId = m_pEnum->keyInTree();
            }
        }
        if( m_strEnumUId != i_arEnumVals[idxVal-i_idxStart].getEnumerationUId() )
        {
            throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
        }

        if( m_pEnum != nullptr )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry == nullptr )
            {
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Invalid);
            }
            else
            {
                m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
            }
        }
        else
        {
            m_arValidities[idxVal] = static_cast<int>(EValueValidity::Valid);
        }
    }

} // setValues

//------------------------------------------------------------------------------
void CEnumValArr::setValues( int i_idxStart, const CEnumValArr& i_enumValArr )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        m_pEnum = i_enumValArr.getEnumeration();

        if( m_pEnum != nullptr )
        {
            m_strEnumUId = i_enumValArr.getEnumerationUId();
        }
    }
    if( m_strEnumUId != i_enumValArr.getEnumerationUId() )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultArgOutOfRange, "Different enumerations" );
    }

    int idxStart = toArrayIndex( i_idxStart, size() );

    if( idxStart + i_enumValArr.size() > size() )
    {
        resize( idxStart + i_enumValArr.size() );
    }

    int idxVal;

    for( idxVal = i_idxStart; idxVal < (i_idxStart + i_enumValArr.size()); idxVal++ )
    {
        m_ariEnumEntryIdx[idxVal] = i_enumValArr.m_ariEnumEntryIdx[idxVal-i_idxStart];
        m_arValidities[idxVal] = i_enumValArr.m_arValidities[idxVal-i_idxStart];

        if( m_ararEnumEntryValidities.size() > 0 )
        {
            m_ararEnumEntryValidities[idxVal] = i_enumValArr.m_ararEnumEntryValidities[idxVal-i_idxStart];
        }
    }

} // setValues

/*==============================================================================
public: // instance methods (to remove values)
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumValArr::removeValues( int i_idxStart, int i_iValCount )
//------------------------------------------------------------------------------
{
    m_ariEnumEntryIdx.remove(i_idxStart,i_iValCount);
    m_arValidities.remove(i_idxStart,i_iValCount);

    if( m_ararEnumEntryValidities.size() > 0 )
    {
        m_ararEnumEntryValidities.remove(i_idxStart,i_iValCount);
    }

} // removeValues

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
CEnumVal CEnumValArr::toEnumVal( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    CEnumVal enumVal(m_pEnum);

    if( m_pEnum == nullptr )
    {
        enumVal.setEnumerationUId(m_strEnumUId);
    }

    if( i_idxVal >= 0 && i_idxVal < size() )
    {
        enumVal.setValByEnumEntryIndex(m_ariEnumEntryIdx[i_idxVal]);
        enumVal.setValidity(static_cast<EValueValidity>(m_arValidities[i_idxVal]));
    }
    return enumVal;

} // toEnumVal

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
int CEnumValArr::getEnumEntryIndex( int i_idxVal ) const
//------------------------------------------------------------------------------
{
    int iVal = -1;

    if( i_idxVal >= 0 && i_idxVal < size() )
    {
        iVal = m_ariEnumEntryIdx[i_idxVal];
    }
    return iVal;

} // getEnumEntryIndex

/*==============================================================================
public: // instance methods (methods can only convert the index into the name or assigned value if the enumeration is known)
==============================================================================*/

//------------------------------------------------------------------------------
QString CEnumValArr::enumEntryIndex2Name( int i_idxVal, int i_alias, EValueValidity* o_pValidity ) const
//------------------------------------------------------------------------------
{
    QString        strName;
    EValueValidity validity = EValueValidity::Invalid;

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[i_idxVal]);

        if( pEntry != nullptr )
        {
            strName = pEntry->getName(i_alias);
        }
        else
        {
            validity = EValueValidity::Invalid;
        }
    }
    if( o_pValidity != nullptr )
    {
        *o_pValidity = validity;
    }
    if( strName.isEmpty() )
    {
        if( validity != EValueValidity::Valid )
        {
            strName = CEnumValueValidity::toString(validity);
        }
    }
    return strName;

} // enumEntryIndex2Name

//------------------------------------------------------------------------------
QVariant CEnumValArr::enumEntryIndex2Val( int i_idxVal, EValueValidity* o_pValidity ) const
//------------------------------------------------------------------------------
{
    QVariant       valEnumEntry;
    EValueValidity validity = EValueValidity::Invalid;

    if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[i_idxVal]);

        if( pEntry != nullptr )
        {
            valEnumEntry = pEntry->m_val;
        }
        else
        {
            validity = EValueValidity::Invalid;
        }
    }
    if( o_pValidity != nullptr )
    {
        *o_pValidity = validity;
    }
    return valEnumEntry;

} // enumEntryIndex2Val

/*==============================================================================
public: // instance methods (if the enumeration is not known the index value is converted into the string)
==============================================================================*/

//------------------------------------------------------------------------------
QString CEnumValArr::toString( int i_idxVal, int i_alias ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( m_pEnum == nullptr )
    {
        str = QString::number(m_ariEnumEntryIdx[i_idxVal]);
    }
    else // if( m_pEnum != nullptr )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[i_idxVal]);

        if( pEntry != nullptr )
        {
            str = pEntry->getName(i_alias);
        }
        else
        {
            str = QString::number(m_ariEnumEntryIdx[i_idxVal]);
        }
    }
    return str;

} // toString

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
CEnumValArr CEnumValArr::mid( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    CEnumValArr enumValArr(m_pEnum);

    if( m_pEnum == nullptr )
    {
        enumValArr.setEnumerationUId(m_strEnumUId);
    }

    QVector<int>    ariEnumEntryIdx = m_ariEnumEntryIdx.mid(i_idxStart,i_iValCount);
    QVector<quint8> arValidities = m_arValidities.mid(i_idxStart,i_iValCount);

    // Please note:
    // When compiling with gcc to avoid compiliation error:
    //   '>>' should be '> >' within a nested argument list.
    QVector< QVector<quint8> > ararEnumEntryValidities = m_ararEnumEntryValidities.mid(i_idxStart,i_iValCount);

    enumValArr.m_ariEnumEntryIdx = ariEnumEntryIdx;
    enumValArr.m_arValidities = arValidities;
    enumValArr.m_ararEnumEntryValidities = ararEnumEntryValidities;

    return enumValArr;

} // mid

/*==============================================================================
public: // instance methods (to get values)
==============================================================================*/

//------------------------------------------------------------------------------
QVector<int> CEnumValArr::getEnumEntryIndices( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    QVector<int> ariEnumEntryIdx;

    int idxValMin = 0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxStart, i_iValCount, size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        ariEnumEntryIdx = m_ariEnumEntryIdx.mid( idxValMin, idxValMax-idxValMin+1 );
    }
    return ariEnumEntryIdx;

} // getEnumEntryIndices

//------------------------------------------------------------------------------
QStringList CEnumValArr::enumEntryIndices2Names( int i_idxStart, int i_iValCount, int i_alias ) const
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only get value by name of enum entries if enum is known" );
    }

    QStringList strlstNames;

    int idxValMin = 0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxStart, i_iValCount, size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        #if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
        strlstNames.reserve(idxValMax-idxValMin+1);
        #endif

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = idxValMin; idxVal <= idxValMax; idxVal++ )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry != nullptr )
            {
                strlstNames.append( pEntry->getName(i_alias) );
            }
        }
    } // if( i_idxStart >= 0 && i_idxStart < size() )

    return strlstNames;

} // enumEntryIndices2Names

//------------------------------------------------------------------------------
QVector<QVariant> CEnumValArr::enumEntryIndices2ValsVec( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only get value by name of enum entries if enum is known" );
    }

    QVector<QVariant> arVals;

    int idxValMin = 0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxStart, i_iValCount, size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        arVals.resize(i_iValCount);

        SEnumEntry* pEntry;
        int         idxVal;

        for( idxVal = idxValMin; idxVal <= idxValMax; idxVal++ )
        {
            pEntry = m_pEnum->index2EnumEntry(m_ariEnumEntryIdx[idxVal]);

            if( pEntry != nullptr )
            {
                arVals[idxVal-idxValMin] = pEntry->m_val;
            }
        }
    }
    return arVals;

} // enumEntryIndices2ValsVec

//------------------------------------------------------------------------------
QVector<CEnumVal> CEnumValArr::toEnumValsVec( int i_idxStart, int i_iValCount ) const
//------------------------------------------------------------------------------
{
    QVector<CEnumVal> arEnumVals;

    int idxValMin = 0;
    int idxValMax = -1;

    getMinMaxArrayRange( i_idxStart, i_iValCount, size(), &idxValMin, &idxValMax );

    if( idxValMax >= idxValMin )
    {
        arEnumVals.reserve(idxValMax-idxValMin+1);

        CEnumVal enumVal(m_pEnum);

        if( m_pEnum == nullptr )
        {
            enumVal.setEnumerationUId(m_strEnumUId);
        }

        int idxVal;

        for( idxVal = idxValMin; idxVal <= idxValMax; idxVal++ )
        {
            enumVal.setValByEnumEntryIndex(m_ariEnumEntryIdx[idxVal]);
            enumVal.setValidity(static_cast<EValueValidity>(m_arValidities[idxVal]));

            arEnumVals[idxVal-idxValMin] = enumVal;
        }
    } // if( i_idxStart >= 0 && i_idxStart < size() )

    return arEnumVals;

} // toEnumValsVec
