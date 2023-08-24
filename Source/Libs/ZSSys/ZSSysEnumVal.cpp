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

#include "ZSSys/ZSSysEnumVal.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysEnumeration.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;


/*******************************************************************************
class CEnumVal
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CEnumVal::CEnumVal() :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(),
    m_iEnumEntryIdx(-1),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( CEnumeration* i_pEnum ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_iEnumEntryIdx(-1),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();
    }

} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( CEnumeration* i_pEnum, int i_iEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_iEnumEntryIdx(i_iEnumEntryIdx),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(i_iEnumEntryIdx);

        if( pEntry != nullptr )
        {
            m_validity = EValueValidity::Valid;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( CEnumeration* i_pEnum, const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_iEnumEntryIdx(-1),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_alias);

        if( pEntry != nullptr )
        {
            m_iEnumEntryIdx = pEntry->m_iEnumerator;
            m_validity = EValueValidity::Valid;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( CEnumeration* i_pEnum, const QVariant& i_valEnumEntry ) :
//------------------------------------------------------------------------------
    m_pEnum(i_pEnum),
    m_strEnumUId(),
    m_iEnumEntryIdx(-1),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

        if( pEntry != nullptr )
        {
            m_iEnumEntryIdx = pEntry->m_iEnumerator;
            m_validity = EValueValidity::Valid;
        }
    }

} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( const QString& i_strEnumUId ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_iEnumEntryIdx(-1),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( const QString& i_strEnumUId, int i_iEnumEntryIdx ) :
//------------------------------------------------------------------------------
    m_pEnum(nullptr),
    m_strEnumUId(i_strEnumUId),
    m_iEnumEntryIdx(i_iEnumEntryIdx),
    m_validity(EValueValidity::Invalid),
    m_arEnumEntryValidities()
{
} // ctor

//------------------------------------------------------------------------------
CEnumVal::CEnumVal( const CEnumVal& i_other ) :
//------------------------------------------------------------------------------
    m_pEnum(i_other.m_pEnum),
    m_strEnumUId(i_other.m_strEnumUId),
    m_iEnumEntryIdx(i_other.m_iEnumEntryIdx),
    m_validity(i_other.m_validity),
    m_arEnumEntryValidities()
{
} // copy ctor

//------------------------------------------------------------------------------
CEnumVal::~CEnumVal()
//------------------------------------------------------------------------------
{
    m_pEnum = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CEnumVal::operator == ( const CEnumVal& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;

    if( m_strEnumUId != i_other.m_strEnumUId )
    {
        bEqual = false;
    }
    else if( m_iEnumEntryIdx != i_other.m_iEnumEntryIdx )
    {
        bEqual = false;
    }
    else if( m_validity != i_other.m_validity )
    {
        bEqual = false;
    }
    else if( m_arEnumEntryValidities.size() != i_other.m_arEnumEntryValidities.size() )
    {
        bEqual = false;
    }
    else if( memcmp( m_arEnumEntryValidities.data(), i_other.m_arEnumEntryValidities.data(), m_arEnumEntryValidities.size()*sizeof(m_arEnumEntryValidities[0]) ) != 0 )
    {
        bEqual = false;
    }
    return bEqual;

} // operator ==

//------------------------------------------------------------------------------
bool CEnumVal::operator != ( const CEnumVal& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CEnumVal& CEnumVal::operator = ( const CEnumVal& i_other )
//------------------------------------------------------------------------------
{
    m_pEnum = i_other.m_pEnum;
    m_strEnumUId = i_other.m_strEnumUId;
    m_iEnumEntryIdx = i_other.m_iEnumEntryIdx;
    m_validity = i_other.m_validity;
    m_arEnumEntryValidities = i_other.m_arEnumEntryValidities;

    return *this;

} // operator =

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EValueValidity CEnumVal::setEnumeration( CEnumeration* i_pEnum )
//------------------------------------------------------------------------------
{
    m_pEnum = i_pEnum;

    m_validity = EValueValidity::Invalid;
    m_arEnumEntryValidities.clear();

    if( m_pEnum != nullptr )
    {
        m_strEnumUId = m_pEnum->keyInTree();

        if( m_iEnumEntryIdx >= 0 )
        {
            SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_iEnumEntryIdx);

            if( pEntry != nullptr )
            {
                m_validity = EValueValidity::Valid;
            }
        }
    }
    return m_validity;

} // setEnumeration

//------------------------------------------------------------------------------
void CEnumVal::setEnumerationUId( const QString& i_strEnumUId )
//------------------------------------------------------------------------------
{
    if( m_pEnum != nullptr && m_pEnum->keyInTree() != i_strEnumUId )
    {
        QString strMethod, strArgs, strErr;
        strMethod = "CEnumVal::setEnumerationUId";
        strArgs   = "EnumKey: " + i_strEnumUId;
        strErr    = "EnumKey is different from key of already set enumeration " + m_pEnum->keyInTree();
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, strMethod + "( " + strArgs + " ): " + strErr );
    }

    m_strEnumUId = i_strEnumUId;

} // setEnumerationUId

//------------------------------------------------------------------------------
QString CEnumVal::getEnumerationName() const
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
void CEnumVal::invalidateObjectReferences()
//------------------------------------------------------------------------------
{
    m_pEnum = nullptr;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EValueValidity CEnumVal::setValByEnumEntryIndex( int i_iEnumEntryIdx )
//------------------------------------------------------------------------------
{
    m_iEnumEntryIdx = i_iEnumEntryIdx;

    // Check whether index is in valid range.
    if( m_iEnumEntryIdx >= 0 )
    {
        if( m_pEnum == nullptr )
        {
            // Cannot check whether index is in valid range.
            m_validity = EValueValidity::Valid;
        }
        else // if( m_pEnum != nullptr )
        {
            SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_iEnumEntryIdx);

            if( pEntry == nullptr )
            {
                m_validity = EValueValidity::Invalid;
            }
            else if( pEntry != nullptr )
            {
                m_validity = EValueValidity::Valid;
            }
        } // if( m_pEnum != nullptr )
    }
    else // if( m_iEnumEntryIdx < 0 )
    {
        m_validity = EValueValidity::Invalid;
    }

    return m_validity;

} // setValByEnumEntryIndex

//------------------------------------------------------------------------------
EValueValidity CEnumVal::setValByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_idxValNames )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        m_validity = EValueValidity::Invalid;
    }
    else
    {
        SEnumEntry* pEntry = m_pEnum->name2EnumEntry(i_strName,i_caseSensitivity,i_idxValNames);

        if( pEntry == nullptr )
        {
            m_validity = EValueValidity::Invalid;
        }
        else
        {
            m_iEnumEntryIdx = pEntry->m_iEnumerator;
            m_validity = EValueValidity::Valid;
        }
    }
    return m_validity;

} // setValByEnumEntryName

//------------------------------------------------------------------------------
EValueValidity CEnumVal::setValByEnumEntryVal( const QVariant& i_valEnumEntry )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        m_validity = EValueValidity::Invalid;
    }
    else
    {
        SEnumEntry* pEntry = m_pEnum->val2EnumEntry(i_valEnumEntry);

        if( pEntry == nullptr )
        {
            m_validity = EValueValidity::Invalid;
        }
        else if( pEntry != nullptr )
        {
            m_iEnumEntryIdx = pEntry->m_iEnumerator;
            m_validity = EValueValidity::Valid;
        }
    }
    return m_validity;

} // setValByEnumEntryVal

/*==============================================================================
public: // instance methods (methods can only convert the index into the name or assigned value if the enumeration is known)
==============================================================================*/

//------------------------------------------------------------------------------
QString CEnumVal::enumEntryIndex2Name( int i_alias, EValueValidity* o_pValidity ) const
//------------------------------------------------------------------------------
{
    QString        strName;
    EValueValidity validity = m_validity;

    if( m_pEnum == nullptr || m_iEnumEntryIdx < 0 )
    {
        validity = EValueValidity::Invalid;
    }
    else // if( m_pEnum != nullptr && m_iEnumEntryIdx >= 0 )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_iEnumEntryIdx);

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
QVariant CEnumVal::enumEntryIndex2Val( EValueValidity* o_pValidity ) const
//------------------------------------------------------------------------------
{
    QVariant       valEnumEntry;
    EValueValidity validity = m_validity;

    if( m_pEnum == nullptr || m_iEnumEntryIdx < 0 )
    {
        validity = EValueValidity::Invalid;
    }
    else // if( m_pEnum != nullptr && m_iEnumEntryIdx >= 0 )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_iEnumEntryIdx);

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
QString CEnumVal::toString( int i_alias ) const
//------------------------------------------------------------------------------
{
    QString str;

    if( m_pEnum == nullptr || m_iEnumEntryIdx < 0 )
    {
        str = QString::number(m_iEnumEntryIdx);
    }
    else // if( m_pEnum != nullptr && m_iEnumEntryIdx >= 0 )
    {
        SEnumEntry* pEntry = m_pEnum->index2EnumEntry(m_iEnumEntryIdx);

        if( pEntry != nullptr )
        {
            str = pEntry->getName(i_alias);
        }
        else
        {
            str = QString::number(m_iEnumEntryIdx);
        }
    }
    return str;

} // toString

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumVal::setValidity( EValueValidity i_validity )
//------------------------------------------------------------------------------
{
    m_validity = i_validity;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CEnumVal::setEnumEntryValidityByEnumEntryIndex( int i_iEnumEntryIdx, EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    if( m_arEnumEntryValidities.size() != m_pEnum->count() )
    {
        m_arEnumEntryValidities = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
    }
    if( i_iEnumEntryIdx >= 0 && i_iEnumEntryIdx < m_arEnumEntryValidities.size() )
    {
        m_arEnumEntryValidities[i_iEnumEntryIdx] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryIndex

//------------------------------------------------------------------------------
void CEnumVal::setEnumEntryValidityByEnumEntryName( const QString& i_strName, EEnumEntryValidity i_validity, Qt::CaseSensitivity i_caseSensitivity, int i_alias )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    if( m_arEnumEntryValidities.size() != m_pEnum->count() )
    {
        m_arEnumEntryValidities = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
    }

    int idxEnumEntry = m_pEnum->name2Index(i_strName,i_caseSensitivity,i_alias);

    if( idxEnumEntry >= 0 && idxEnumEntry < m_arEnumEntryValidities.size() )
    {
        m_arEnumEntryValidities[idxEnumEntry] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryName

//------------------------------------------------------------------------------
void CEnumVal::setEnumEntryValidityByEnumEntryVal( const QVariant& i_valEnumEntry, EEnumEntryValidity i_validity )
//------------------------------------------------------------------------------
{
    if( m_pEnum == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultInvalidMethodCall, "Can only set validities of enum entries if enum is known" );
    }

    if( m_arEnumEntryValidities.size() != m_pEnum->count() )
    {
        m_arEnumEntryValidities = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
    }

    int idxEnumEntry = m_pEnum->val2Index(i_valEnumEntry);

    if( idxEnumEntry >= 0 && idxEnumEntry < m_arEnumEntryValidities.size() )
    {
        m_arEnumEntryValidities[idxEnumEntry] = i_validity;
    }

} // setEnumEntryValidityByEnumEntryVal

//------------------------------------------------------------------------------
void CEnumVal::setEnumEntryValidities( const QVector<quint8> i_arValidities )
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

    if( m_arEnumEntryValidities.size() != m_pEnum->count() )
    {
        m_arEnumEntryValidities = QVector<quint8>( m_pEnum->count(), EEnumEntryValidityNormal );
    }
    if( i_arValidities.size() == m_arEnumEntryValidities.size() )
    {
        m_arEnumEntryValidities = i_arValidities;
    }

} // setEnumEntryValidities

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumVal::getEnumEntryValidityByEnumEntryIndex( int i_iEnumEntryIdx ) const
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityInvalid;

    if( i_iEnumEntryIdx >= 0 && i_iEnumEntryIdx < m_arEnumEntryValidities.size() )
    {
        validity = static_cast<EEnumEntryValidity>(m_arEnumEntryValidities[i_iEnumEntryIdx]);
    }
    return validity;

} // getEnumEntryValidityByEnumEntryIndex

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumVal::getEnumEntryValidityByEnumEntryName( const QString& i_strName, Qt::CaseSensitivity i_caseSensitivity, int i_alias ) const
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityInvalid;

    if( m_pEnum != nullptr )
    {
        int idxEnumEntry = m_pEnum->name2Index(i_strName,i_caseSensitivity,i_alias);

        if( idxEnumEntry >= 0 && idxEnumEntry < m_arEnumEntryValidities.size() )
        {
            validity = static_cast<EEnumEntryValidity>(m_arEnumEntryValidities[idxEnumEntry]);
        }
    }
    return validity;

} // getEnumEntryValidityByEnumEntryName

//------------------------------------------------------------------------------
EEnumEntryValidity CEnumVal::getEnumEntryValidityByEnumEntryVal( const QVariant& i_valEnumEntry ) const
//------------------------------------------------------------------------------
{
    EEnumEntryValidity validity = EEnumEntryValidityInvalid;

    if( m_pEnum != nullptr )
    {
        int idxEnumEntry = m_pEnum->val2Index(i_valEnumEntry);

        if( idxEnumEntry >= 0 && idxEnumEntry < m_arEnumEntryValidities.size() )
        {
            validity = static_cast<EEnumEntryValidity>(m_arEnumEntryValidities[idxEnumEntry]);
        }
    }
    return validity;

} // getEnumEntryValidityByEnumEntryVal
