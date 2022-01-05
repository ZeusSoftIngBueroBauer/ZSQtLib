/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSDraw_Settings_h
#define ZSDraw_Settings_h

#include <QtCore/qvariant.h>
#include <QtCore/qvector.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

#include "ZSDraw/ZSDrawDllMain.h"
#include "ZSDraw/ZSDrawCommon.h"

class QSettings;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace ZS
{
namespace Draw
{
/*******************************************************************************
enum EDrawAttribute
*******************************************************************************/

// Enum with predefined attributes. Some graphical objects may extent
// the list of attributes in CDrawSettings starting with "UserDefinedMin".
typedef enum
{
    EDrawAttributePenColor                     =  0,
    EDrawAttributePenWidth                     =  1,
    EDrawAttributeLineStyle                    =  2,
    EDrawAttributeFillColor                    =  3,
    EDrawAttributeFillStyle                    =  4,
    EDrawAttributeLineRecordType               =  5,
    EDrawAttributeLineExtent                   =  6,
    EDrawAttributeLineStartLineEndStyle        =  7,
    EDrawAttributeLineStartLineEndBaseLineType =  8,
    EDrawAttributeLineStartLineEndFillStyle    =  9,
    EDrawAttributeLineStartLineEndWidth        = 10,
    EDrawAttributeLineStartLineEndLength       = 11,
    EDrawAttributeLineEndLineEndStyle          = 12,
    EDrawAttributeLineEndLineEndBaseLineType   = 13,
    EDrawAttributeLineEndLineEndFillStyle      = 14,
    EDrawAttributeLineEndLineEndWidth          = 15,
    EDrawAttributeLineEndLineEndLength         = 16,
    EDrawAttributeTextColor                    = 17,
    EDrawAttributeTextFont                     = 18,
    EDrawAttributeTextStyle                    = 19,
    EDrawAttributeTextSize                     = 20,
    EDrawAttributeTextEffect                   = 21,
    EDrawAttributeCount,
    EDrawAttributeUserDefinedMin        = EDrawAttributeCount,
    EDrawAttributePenMin                = EDrawAttributePenColor,
    EDrawAttributePenMax                = EDrawAttributePenWidth,
    EDrawAttributePenCount              = (EDrawAttributePenMax - EDrawAttributePenMin + 1),
    EDrawAttributeLineStyleMin          = EDrawAttributeLineStyle,
    EDrawAttributeLineStyleMax          = EDrawAttributeLineStyle,
    EDrawAttributeLineStyleCount        = (EDrawAttributeLineStyleMax - EDrawAttributeLineStyleMin + 1),
    EDrawAttributeFillMin               = EDrawAttributeFillColor,
    EDrawAttributeFillMax               = EDrawAttributeFillStyle,
    EDrawAttributeFillCount             = (EDrawAttributeFillMax - EDrawAttributeFillMin + 1),
    EDrawAttributeLineRecordTypeMin     = EDrawAttributeLineRecordType,
    EDrawAttributeLineRecordTypeMax     = EDrawAttributeLineExtent,
    EDrawAttributeLineRecordTypeCount   = (EDrawAttributeLineRecordTypeMax - EDrawAttributeLineRecordTypeMin + 1),
    EDrawAttributeLineEndStyleMin       = EDrawAttributeLineStartLineEndStyle,
    EDrawAttributeLineEndStyleMax       = EDrawAttributeLineEndLineEndLength,
    EDrawAttributeLineEndStyleCount     = (EDrawAttributeLineEndStyleMax - EDrawAttributeLineEndStyleMin + 1),
    EDrawAttributeTextMin               = EDrawAttributeTextColor,
    EDrawAttributeTextMax               = EDrawAttributeTextEffect,
    EDrawAttributeTextCount             = (EDrawAttributeTextMax - EDrawAttributeTextMin + 1),
    EDrawAttributeUndefined             = -1
}   EDrawAttribute;

QString ZSDRAWDLL_API drawAttribute2Str( int i_iAttribute );
EDrawAttribute ZSDRAWDLL_API str2DrawAttribute( const QString& i_str );


//******************************************************************************
struct ZSDRAWDLL_API SDrawAttribute
//******************************************************************************
{
public: // ctors
    SDrawAttribute() :
        m_strName(),
        m_val(),
        m_bIsUsed(true)
    {
    }
    SDrawAttribute( const QString& i_strName, QVariant::Type i_type ) :
        m_strName(i_strName),
        m_val(i_type),
        m_bIsUsed(true)
    {
    }
    SDrawAttribute( const QString& i_strName, const QVariant& i_val, bool i_bIsUsed ) :
        m_strName(i_strName),
        m_val(i_val),
        m_bIsUsed(i_bIsUsed)
    {
    }
public: // operators
    SDrawAttribute& operator = ( const QVariant& i_val )
    {
        m_val = i_val;
        return *this;
    }
    bool operator == ( const SDrawAttribute& i_attrOther ) const
    {
        bool bEqual = true;
        if( m_strName != i_attrOther.m_strName || m_val != i_attrOther.m_val )
        {
            bEqual = false;
        }
        return bEqual;
    }
    bool operator != ( const SDrawAttribute& i_attrOther ) const
    {
        return !(*this == i_attrOther);
    }
public: // struct members
    QString  m_strName;
    QVariant m_val;
    bool     m_bIsUsed;

}; // struct SDrawAttribute


//******************************************************************************
class ZSDRAWDLL_API CDrawSettings
//******************************************************************************
{
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; } // Please note that the static class functions name must be different from the non static virtual member function "nameSpace"
    /*! Returns the class name. */
    static QString ClassName() { return "CDrawSettings"; } // Please note that the static class functions name must be different from the non static virtual member function "className"
public: // class methods
    static bool IsAttributeUsed( EGraphObjType i_graphObjType, int i_iAttribute );
    static bool IsAttributeUsed( EGraphObjType i_graphObjType, const QString& i_strName );
public: // ctors and dtor
    CDrawSettings( EGraphObjType i_graphObjType = EGraphObjTypeUndefined );
    ~CDrawSettings();
public: // overridables
    /*! This virtual method returns the name space of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name space of the derived class. */
    virtual QString nameSpace() const { return NameSpace(); }
    /*! This virtual method returns the class name of the object's class.
        This method can be reimplemented in derived classes so when invoked for the
        polymorphic base type the method returns the name of the derived class. */
    virtual QString className() const { return ClassName(); }
public: // operators
    bool operator == ( const CDrawSettings& i_other ) const;
    bool operator != ( const CDrawSettings& i_other ) const;
public: // instance methods
    EGraphObjType getGraphObjType() const { return m_graphObjType; }
public: // instance methods
    void save( QSettings& i_settings, const QString& i_strSettingsKey );
    void load( QSettings& i_settings, const QString& i_strSettingsKey );
public: // instance methods
    void save( QXmlStreamWriter& i_xmlStreamWriter );
    void load( QXmlStreamReader& i_xmlStreamReader );
public: // instance methods
    void set2Default();
    bool isDefault() const;
    void set2Default( int i_iAttribute );
    void set2Default( const QString& i_strName );
    bool isDefault( int i_iAttribute ) const;
    bool isDefault( const QString& i_strName ) const;
public: // instance methods
    void setAttribute( int i_iAttribute, const SDrawAttribute& i_drawAttribute );
    void setAttribute( int i_iAttribute, const QVariant& i_val );
    void setAttribute( int i_iAttribute, const QString& i_strVal );
    void setAttribute( const QString& i_strName, const QVariant& i_val );
    void setAttribute( const QString& i_strName, const QString& i_strVal );
    SDrawAttribute getAttribute( int i_iAttribute ) const;
    SDrawAttribute getAttribute( const QString& i_strName ) const;
    QString attributeValue2Str( int i_iAttribute ) const;
    QString attributeValue2Str( const QString& i_strName ) const;
    int findAttribute( const QString& i_strName ) const;
    int getAttributeCount() const;
    QString getAttributeName( int i_iAttribute ) const;
    QVariant getAttributeValue( int i_iAttribute ) const;
    QVariant getAttributeValue( const QString& i_strName ) const;
public: // instance methods
    void setAttributesAreUsed( bool i_bIsUsed, int i_iAttributeMin = 0, int i_iAttributesCount = EDrawAttributeCount );
    void setAttributeIsUsed( int i_iAttribute, bool i_bIsUsed );
    void setAttributeIsUsed( const QString& i_strName, bool i_bIsUsed );
    bool isAttributeUsed( int i_iAttribute ) const;
    bool isAttributeUsed( const QString& i_strName ) const;
public: // instance methods
    bool isPenUsed() const;
    bool isLineStyleUsed() const;
    bool isFillUsed() const;
    bool isLineRecordTypeUsed() const;
    bool isLineEndStyleUsed() const;
    bool isTextUsed() const;
public: // instance methods
    void setPenColor( const QColor& );
    QColor getPenColor() const;
    void setPenWidth( int );
    int getPenWidth() const;
public: // instance methods
    void setLineStyle( ELineStyle );
    ELineStyle getLineStyle() const;
public: // instance methods
    void setFillColor( const QColor& );
    QColor getFillColor() const;
    void setFillStyle( EFillStyle );
    EFillStyle getFillStyle() const;
public: // instance methods
    void setLineRecordType( ELineRecordType );
    ELineRecordType getLineRecordType() const;
    void setLineExtent( int );
    int getLineExtent() const;
public: // instance methods
    void setLineEndStyle( ELinePoint, ELineEndStyle );
    ELineEndStyle getLineEndStyle( ELinePoint ) const;
    void setLineEndBaseLineType( ELinePoint, ELineEndBaseLineType );
    ELineEndBaseLineType getLineEndBaseLineType( ELinePoint ) const;
    void setLineEndFillStyle( ELinePoint, ELineEndFillStyle );
    ELineEndFillStyle getLineEndFillStyle( ELinePoint ) const;
    void setLineEndWidth( ELinePoint, ELineEndWidth );
    ELineEndWidth getLineEndWidth( ELinePoint ) const;
    void setLineEndLength( ELinePoint, ELineEndLength );
    ELineEndLength getLineEndLength( ELinePoint ) const;
public: // instance methods
    void setTextColor( const QColor& );
    QColor getTextColor() const;
    void setTextFont( const QFont& );
    QFont getTextFont() const;
    void setTextStyle( ETextStyle );
    ETextStyle getTextStyle() const;
    void setTextSize( ETextSize );
    ETextSize getTextSize() const;
    void setTextEffect( const ETextEffect );
    ETextEffect getTextEffect() const;
protected: // instance members
    EGraphObjType  m_graphObjType;
    int            m_idxGraphObjType;
    SDrawAttribute m_arAttributes[EDrawAttributeCount];

}; // class CDrawSettings

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Settings_h
