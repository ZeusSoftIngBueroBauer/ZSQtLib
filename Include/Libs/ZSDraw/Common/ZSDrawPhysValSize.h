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

#ifndef ZSDraw_PhysValSize_h
#define ZSDraw_PhysValSize_h

#include "ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysVal.h"
#include "QtCore/qsize.h"

namespace ZS
{
namespace PhysVal
{
class CPhysVal;
}
namespace Draw
{
//******************************************************************************
/*! @brief Corresponds to QSizeF but providing a physical unit and a resolution.

    This class is not part of ZS::PhysVal as conversion into pixel values is
    necessary and for this the unit "Pixel" must be available.
    ZS::PhysVal is abstract concerning units and does not know about the unit "px".
*/
class ZSDRAWDLL_API CPhysValSize
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValSize"; }
public: // ctors and dtor
    CPhysValSize( ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValSize( const ZS::PhysVal::CUnit& i_unit, double i_fRes = 0.0, ZS::PhysVal::EResType i_resType = ZS::PhysVal::EResType::Resolution );
    CPhysValSize( const ZS::PhysVal::CPhysVal& i_physValWidth, const ZS::PhysVal::CPhysVal& i_physValHeight );
    CPhysValSize( const CPhysValSize& i_physValSizeOther );
    CPhysValSize( const QSizeF& i_rect );
    virtual ~CPhysValSize();
public: // operators
    bool operator == ( const CPhysValSize& i_physValSizeOther ) const;
    bool operator != ( const CPhysValSize& i_physValSizeOther ) const;
public: // operators
    CPhysValSize& operator = ( const CPhysValSize& i_physValSizeOther );
    CPhysValSize& operator = ( const QSizeF& i_size );
public: // instance methods
    ZS::PhysVal::CUnit unit() const;
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
    bool isValid() const;
public: // instance methods
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
public: // instance methods (to convert the values into another unit)
    QSizeF toQSizeF() const;
    void convertValues( const ZS::PhysVal::CUnit& i_unitDst );
public: // instance methods
    QString toString() const;
protected: // instance members
    ZS::PhysVal::CUnit m_unit;
    ZS::PhysVal::CPhysValRes m_physValRes;
    QSizeF m_size;

}; // class CPhysValSize

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValSize_h
