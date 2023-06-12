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

#ifndef ZSDraw_Units_h
#define ZSDraw_Units_h

#include "ZSDraw/Common/ZSDrawDllMain.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSPhysVal/ZSPhysUnit.h"
#include "ZSSys/ZSSysCommon.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CUnitsLength : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CUnitsLength(ZS::System::CIdxTreeEntry* i_pParentBranch);
    ~CUnitsLength();
public: // instance methods (resolution of monitor, pixels per inches)
    void setPxpis( double i_fPxpiX, double i_fPxpiY );
    double pxpi( ZS::System::EOrientation i_orientation ) const;
public: // instance methods (resolution of printer, dots per inches)
    void setDpis( double i_fDpiX, double i_fDpiY );
    double dpi( ZS::System::EOrientation i_orientation ) const;
public: // instance methods (distinguish between metric system and pixels or dots)
    bool isMetricUnit(const QString& i_strSymbolOrName) const;
private: // instance methods
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryCentiMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDeziMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloMeter;
    /*!< Inches (1 Inch = 25.4 mm = 2.54cm = 0.0254m)
         MToSI: 0.0254 -> MFromSI: 1.0/MToSI */
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryInch;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPixelX;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPixelY;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDotsX;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDotsY;
public: // instance members
    ZS::PhysVal::CUnit um;
    ZS::PhysVal::CUnit mm;
    ZS::PhysVal::CUnit cm;
    ZS::PhysVal::CUnit dm;
    ZS::PhysVal::CUnit m;
    ZS::PhysVal::CUnit km;
    ZS::PhysVal::CUnit in;
    ZS::PhysVal::CUnit pxX;
    ZS::PhysVal::CUnit pxY;
    ZS::PhysVal::CUnit dotsX;
    ZS::PhysVal::CUnit dotsY;

}; // CUnitsLength

//******************************************************************************
class ZSDRAWDLL_API CUnits : public ZS::PhysVal::CIdxTreeUnits
//******************************************************************************
{
public: // ctors and dtor
    CUnits();
    ~CUnits();
public: // instance members
    CUnitsLength Length;

}; // CUnits

extern ZSDRAWDLL_API CUnits Units;

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_Units_h
