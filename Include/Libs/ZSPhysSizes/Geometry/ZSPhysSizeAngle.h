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

#ifndef ZSPhysSizes_Geometry_Angle_h
#define ZSPhysSizes_Geometry_Angle_h

#include "ZSPhysSizes/Geometry/ZSPhysSizesGeometryDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Geometry
{
const QString c_strSymbolPhi(QChar(0x03C6));

//******************************************************************************
class ZSPHYSSIZESGEOMETRYDLL_API CPhysSizeAngle : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeAngle();
    ~CPhysSizeAngle();
public: // instance methods
    CPhysUnit* Rad() { return &m_physUnitRad; }
    CPhysUnit* Degree() { return &m_physUnitDegree; }
protected: // instance members
    CPhysUnit m_physUnitRad;
    CPhysUnit m_physUnitDegree;

}; // CPhysSizeAngle

} // namespace Geometry

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Geometry_Angle_h
