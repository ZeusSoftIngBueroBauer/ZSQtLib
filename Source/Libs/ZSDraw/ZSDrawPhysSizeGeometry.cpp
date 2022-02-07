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

#include "ZSDraw/ZSDrawPhysSizeGeometry.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::Draw;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
class CPhysSizeGeometry : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates the physical size with units for the drawing's page setup.

    Allows conversion from pixel to metrics units and vice versa.

    As the resolution in X and Y direction may be different two instances of
    the physical size will be created by the page setup.

    @param [in] i_fDpmm
        Pixel resolution in dots per milli meter (= px/mm).
*/
CPhysSizeGeometry::CPhysSizeGeometry( double i_fDpmm ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldGeometry,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "l",
        /* bIsPowerRelated  */ false ),
    m_physUnitPixel(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Pixel",
        /* strSymbol      */ "px",
        /* fMFromSI       */ 1000.0 * i_fDpmm ),
    m_physUnitMicroMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_physUnitMilliMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_physUnitCentiMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixCenti ),
    m_physUnitDeziMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixDezi ),
    m_physUnitMeter(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKiloMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    m_physUnitMicroMeter.setNextLowerHigherUnits( nullptr,               &m_physUnitMilliMeter );
    m_physUnitMilliMeter.setNextLowerHigherUnits( &m_physUnitMicroMeter, &m_physUnitMeter      );
    m_physUnitMeter.     setNextLowerHigherUnits( &m_physUnitMilliMeter, &m_physUnitKiloMeter  );
    m_physUnitKiloMeter. setNextLowerHigherUnits( &m_physUnitMeter,      nullptr               );

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the physical size.
*/
CPhysSizeGeometry::~CPhysSizeGeometry()
//------------------------------------------------------------------------------
{
    //m_physUnitPixel;
    //m_physUnitMicroMeter;
    //m_physUnitMilliMeter;
    //m_physUnitCentiMeter;
    //m_physUnitDeziMeter;
    //m_physUnitMeter;
    //m_physUnitKiloMeter;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the resolution of a pixel on the screen in dots per milli meter.

    @param [in] i_fDpmm
        Pixel resolution in dots per milli meter (= px/mm).
*/
//------------------------------------------------------------------------------
void CPhysSizeGeometry::setDpmm( double i_fDpmm )
{
    m_physUnitPixel.setFactorConvertFromSIUnit(1000.0 * i_fDpmm);
}

//------------------------------------------------------------------------------
/*! Returns the resolution of a pixel on the screen in dots per milli meter.

    @return
        Pixel resolution in dots per milli meter (= px/mm).
*/
//------------------------------------------------------------------------------
double CPhysSizeGeometry::getDpmm() const
{
    return m_physUnitPixel.getFactorConvertFromSIUnit() / 1000.0;
}

