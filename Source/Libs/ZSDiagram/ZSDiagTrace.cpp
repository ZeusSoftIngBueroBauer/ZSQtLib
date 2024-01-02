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

#include "ZSDiagram/ZSDiagTrace.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDiagTrace
*******************************************************************************/

/*==============================================================================
protected: // class members
==============================================================================*/

bool CDiagTrace::s_bClassInitialised = false;

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strObjName [in]
        Name of the object.
        Each trace object belonging to the same diagram must have a unique name.
    @param i_pDiagScaleX [in]
        Reference to X scale object the trace is linked to.
        If the scale changes the trace values must be updated.
    @param i_pDiagScaleY [in]
        Reference to Y scale object the trace is linked to.
        If the scale changes the trace values must be updated.
    @param i_physValResX [in]
         Resolution of the X values.
         The resolution defined the number of significant digits to be output
         if the values should be indicated e.g. by a marker object.
    @param i_physValResY [in]
         Resolution of the Y values.
         The resolution defined the number of significant digits to be output
         if the values should be indicated e.g. by a marker object.
*/
CDiagTrace::CDiagTrace(
    const QString&     i_strObjName,
    CDiagScale*        i_pDiagScaleX,
    CDiagScale*        i_pDiagScaleY,
    const CPhysValRes& i_physValResX,
    const CPhysValRes& i_physValResY ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    m_arpDiagScale(CEnumScaleDir::count(), nullptr),
    //m_arphysValRes(CEnumScaleDir::count()),
    m_arphysValArr(CEnumScaleDir::count()),
    m_uUpdateFlags(EUpdateData),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjValidate(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", objectName());
    m_pTrcAdminObjValidate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Validate", objectName());

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strObjName +
            ", ScaleX: " + QString(i_pDiagScaleX == nullptr ? "null" : i_pDiagScaleX->objectName()) +
            ", ScaleY: " + QString(i_pDiagScaleY == nullptr ? "null" : i_pDiagScaleY->objectName()) +
            ", ResX: " + i_physValResX.toString() +
            ", ResY: " + i_physValResY.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

    if( !s_bClassInitialised )
    {
        s_bClassInitialised = true;
    }

    m_arpDiagScale[EScaleDirX] = i_pDiagScaleX;
    m_arpDiagScale[EScaleDirY] = i_pDiagScaleY;

    //m_arphysValRes[EScaleDirX].setValidity(EValueValidity::Invalid);
    //m_arphysValRes[EScaleDirY].setValidity(EValueValidity::Invalid);

    //m_arphysValArr[EScaleDirX].setValidity(EValueValidity::Invalid);
    //m_arphysValArr[EScaleDirY].setValidity(EValueValidity::Invalid);

    if( i_pDiagScaleX != nullptr )
    {
        //m_arphysValRes[EScaleDirX].setUnit(i_pDiagScaleX->getScale().m_unit);
        m_arphysValArr[EScaleDirX].setUnit(i_pDiagScaleX->getScale().unit());
        m_arphysValArr[EScaleDirX].setRes(i_physValResX);
    }
    if( i_pDiagScaleY != nullptr )
    {
        //m_arphysValRes[EScaleDirY].setUnit(i_pDiagScaleY->getScale().m_unit);
        m_arphysValArr[EScaleDirY].setUnit(i_pDiagScaleY->getScale().unit());
        m_arphysValArr[EScaleDirY].setRes(i_physValResY);
    }

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destructor.
*/
CDiagTrace::~CDiagTrace()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjValidate);
    m_pTrcAdminObjValidate = nullptr;

    m_pDiagram = nullptr;
    //m_arpDiagScale.clear();
    //m_arphysValRes.clear();
    //m_arphysValArr.clear();
    m_uUpdateFlags = EUpdateNone;
    m_pTrcAdminObj = nullptr;
    m_pTrcAdminObjValidate = nullptr;

} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the object.

    @return Name of the object.
*/
QString CDiagTrace::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
/*! @brief Returns the diagram the scale object belongs to.

    @return Diagram the scale belongs to.
*/
CDataDiagram* CDiagTrace::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDiagram;
}

//lint -e1762
//------------------------------------------------------------------------------
/*! @brief Returns the scale object for the given scale direction the trace is linked to.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Linked scale object.
*/
CDiagScale* CDiagTrace::getDiagScale( const CEnumScaleDir& i_scaleDir )
//------------------------------------------------------------------------------
{
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()];
}
//lint +e1762

//------------------------------------------------------------------------------
/*! @brief Returns the spacing used by scale object for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Spacing used by the scale object.
*/
ESpacing CDiagTrace::getSpacing( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw ZS::System::CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getSpacing();
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the scale object for the given scale direction is valid.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
        Also an invalid enum value may be passed to check all scale direction.

    @return true if scale is valid, false otherwise.
*/
bool CDiagTrace::isScaleValid( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bScaleValid = true;
    int  idxScaleMin = 0;
    int  idxScaleMax = CEnumScaleDir::count()-1;

    if( i_scaleDir.isValid() )
    {
        idxScaleMin = i_scaleDir.enumeratorAsInt();
        idxScaleMax = i_scaleDir.enumeratorAsInt();
    }

    for (int idxScale = idxScaleMin; idxScale <= idxScaleMax; idxScale++)
    {
        const CDiagScale*  pDiagScale = m_arpDiagScale[idxScale];

        if( pDiagScale == nullptr )
        {
            bScaleValid = false;
        }
        else
        {
            bScaleValid = pDiagScale->isScaleValid();
        }
        if( !bScaleValid )
        {
            break;
        }
    }
    return bScaleValid;
}

//------------------------------------------------------------------------------
/*! @brief Returns the scale values used by scale object for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Scale values.
*/
CScale CDiagTrace::getScale( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getScale();
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum scale value in pixels used by scale object for
           the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Minimum scale value in pixels.
*/
int CDiagTrace::getScaleMinValPix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getMinValPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum scale value in pixels used by scale object for
           the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Maximum scale value in pixels.
*/
int CDiagTrace::getScaleMaxValPix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getMaxValPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the range in pixels used by scale object for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Scale range in pixels.
*/
int CDiagTrace::getScaleRangePix( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getRangePix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the range in pixels used by scale object for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Scale range in pixels.
*/
CPhysValRes CDiagTrace::getScaleRes( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getScaleRes();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the unit of the values for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Unit of the values.
*/
CUnit CDiagTrace::getValuesUnit( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of the values for the given scale direction.

    The resolution of the values defines the number of significant digits to be
    output if the values should be indicated e.g. by a marker object.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
    @param i_physValRes [in]
        Resolution of the values to be set.
*/
void CDiagTrace::setValuesRes( const CEnumScaleDir& i_scaleDir, const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].setRes(i_physValRes);
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the values for the given scale direction.

    The resolution of the values defines the number of significant digits to be
    output if the values should be indicated e.g. by a marker object.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Resolution of the values.
*/
CPhysValRes CDiagTrace::getValuesRes( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].getRes();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets new values for the given scale direction.

    If the passed value array contains a valid resolution this resolution will
    be taken over. Otherwise the current resolution is kept.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
    @param i_physValArr [in]
        New value array to be set.
*/
void CDiagTrace::setValues( const CEnumScaleDir& i_scaleDir, const CPhysValArr& i_physValArr )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scaleDir.toString() +
            ", ValArrRes: " + i_physValArr.getRes().toString() +
            ", ValArr [" + QString::number(i_physValArr.size()) + "]";
        if (i_physValArr.size() > 0) {
            strMthInArgs += "(";
            if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsVerbose)) {
                QStringList strlst = i_physValArr.toStringList();
                strMthInArgs += strlst.join(", ");
            }
            else if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsDetailed)) {
                if (i_physValArr.size() <= 50) {
                    QStringList strlst = i_physValArr.toStringList();
                    strMthInArgs += strlst.join(", ");
                }
                else {
                    QStringList strlst = i_physValArr.toStringList(0, 25);
                    strMthInArgs += strlst.join(", ");
                    strlst = i_physValArr.toStringList(i_physValArr.size()-25, 25);
                    strMthInArgs += strlst.join(", ");
                }
            }
            else {
                if (i_physValArr.size() <= 10) {
                    QStringList strlst = i_physValArr.toStringList();
                    strMthInArgs += strlst.join(", ");
                }
                else {
                    QStringList strlst = i_physValArr.toStringList(0, 5);
                    strMthInArgs += strlst.join(", ");
                    strlst = i_physValArr.toStringList(i_physValArr.size()-5, 5);
                    strMthInArgs += strlst.join(", ");
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setValues",
        /* strAddInfo   */ strMthInArgs );

    int iScaleDir = i_scaleDir.enumeratorAsInt();

    if( m_arpDiagScale[iScaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[iScaleDir];

    CPhysValRes physValResPrev = m_arphysValArr[iScaleDir].getRes();

    m_arphysValArr[iScaleDir] = i_physValArr;

    if( i_physValArr.unit().isValid() && pDiagScale->getScale().unit() != i_physValArr.unit() )
    {
        m_arphysValArr[iScaleDir].convertValues(pDiagScale->getScale().unit());
    }
    if( !i_physValArr.getRes().isValid() )
    {
        m_arphysValArr[iScaleDir].setRes(physValResPrev);
    }

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets new values for the given scale direction.

    If the passed value array contains a valid resolution this resolution will
    be taken over. Otherwise the current resolution is kept.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
    @param i_arfValues [in]
        New value array to be set.
    @param i_pUnitVal [in]
        Unit of the passed values or nullptr, if the values are provided in
        the unit already set at the trace object.
    @param i_physValRes [in]
        Resolution of the values. If invalid resolution is passed the
        current resolution will not be changed.
*/
void CDiagTrace::setValues(
    const CEnumScaleDir&   i_scaleDir,
    const QVector<double>& i_arfValues,
    const CUnit*           i_pUnitVals,
    const CPhysValRes&     i_physValRes )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scaleDir.toString() +
            ", ValArrRes: " + i_physValRes.toString() +
            ", ValArr [" + QString::number(i_arfValues.size()) + "] " +
            QString(i_pUnitVals == nullptr ? "" : i_pUnitVals->symbol());
        if (i_arfValues.size() > 0) {
            strMthInArgs += "(";
            if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsVerbose)) {
                for (int idxVal = 0; idxVal < i_arfValues.size(); ++idxVal) {
                    if (idxVal > 0) strMthInArgs += ", ";
                    strMthInArgs += QString::number(i_arfValues[idxVal]);
                }
            }
            else if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsDetailed)) {
                if (i_arfValues.size() <= 50) {
                    for (int idxVal = 0; idxVal < i_arfValues.size(); ++idxVal) {
                        if (idxVal > 0) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                }
                else {
                    for (int idxVal = 0; idxVal < 25; ++idxVal) {
                        if (idxVal > 0) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                    for (int idxVal = i_arfValues.size()-25; idxVal < i_arfValues.size(); ++idxVal) {
                        if (idxVal > i_arfValues.size()-25) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                }
            }
            else {
                if (i_arfValues.size() <= 10) {
                    for (int idxVal = 0; idxVal < i_arfValues.size(); ++idxVal) {
                        if (idxVal > 0) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                }
                else {
                    for (int idxVal = 0; idxVal < 5; ++idxVal) {
                        if (idxVal > 0) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                    for (int idxVal = i_arfValues.size()-5; idxVal < i_arfValues.size(); ++idxVal) {
                        if (idxVal > i_arfValues.size()-5) strMthInArgs += ", ";
                        strMthInArgs += QString::number(i_arfValues[idxVal]);
                    }
                }
            }
            strMthInArgs += ")";
        }
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setValues",
        /* strAddInfo   */ strMthInArgs );

    int iScaleDir = i_scaleDir.enumeratorAsInt();

    if( m_arpDiagScale[iScaleDir] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    const CDiagScale* pDiagScale = m_arpDiagScale[iScaleDir];
    CUnit  unitVal = i_pUnitVals == nullptr ? m_arphysValArr[iScaleDir].unit() : *i_pUnitVals;

    if( !areOfSameUnitGroup(m_arphysValArr[iScaleDir].unit(),unitVal) )
    {
        m_arphysValArr[iScaleDir].setUnit(unitVal);
    }

    m_arphysValArr[iScaleDir].setValues(0, i_arfValues, unitVal);

    if( pDiagScale->getScale().unit() != unitVal )
    {
        m_arphysValArr[iScaleDir].convertValues(pDiagScale->getScale().unit());
    }
    if( i_physValRes.isValid() )
    {
        m_arphysValArr[iScaleDir].setRes(i_physValRes);
    }
    m_arphysValArr[iScaleDir].setValidity(EValueValidity::Valid);

    invalidate(EUpdateData);

    if( m_pDiagram != nullptr )
    {
        m_pDiagram->traceChanged(this);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns whether valid values have been set for the given scale direction.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return true if there are valid values, false otherwise.
*/
bool CDiagTrace::areValuesValid( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    bool bValuesValid = true;

    int idxScaleMin = 0;
    int idxScaleMax = CEnumScaleDir::count()-1;

    if( i_scaleDir.isValid() )
    {
        idxScaleMin = i_scaleDir.enumeratorAsInt();
        idxScaleMax = i_scaleDir.enumeratorAsInt();
    }

    for( int idxScale = idxScaleMin; idxScale <= idxScaleMax; idxScale++ )
    {
        if( !m_arphysValArr[idxScale].isValid() )
        {
            bValuesValid = false;
            break;
        }
    }
    return bValuesValid;
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of values for the given scale direction.

    Usually the number of values is the same for both scale directions.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Number of values.
*/
int CDiagTrace::getValCount( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].size();
}

//------------------------------------------------------------------------------
/*! @brief Returns a vector with values for the given scale direction.

    Usually the number of values is the same for both scale directions.

    @param i_scaleDir [in]
        Scale direction (either X or Y).

    @return Array with values.
*/
QVector<double> CDiagTrace::getValues( const CEnumScaleDir& i_scaleDir ) const
//------------------------------------------------------------------------------
{
    return m_arphysValArr[i_scaleDir.enumeratorAsInt()].toDoubleVec(0,EArrayIndexCountAllElements);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the pixel value of the given value.

    The method call will be forwarded to the corresponding scale object which
    contains the pixel range for the axis.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
    @param i_fVal [in]
        Value to be converted into the corresponding pixel value.
    @param i_pUnit [in]
        Pass a reference to a valid unit if the value is not given in the unit of the trace.

    @Return Value converted into pixel.
*/
int CDiagTrace::getValPix( const CEnumScaleDir& i_scaleDir, double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getValPix(i_fVal,i_pUnit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the string representation of the given value.

    The method call will be forwarded to the corresponding scale object which
    contains the pixel range for the axis.

    @param i_scaleDir [in]
        Scale direction (either X or Y).
    @param i_fVal [in]
        Value to be converted into the corresponding pixel value.
    @param i_pUnit [in]
        Pass a reference to a valid unit if the value is not given in the unit of the trace.
    @param i_iDigitsCountMax [in]
        Maximum number of digits used for the output string.
    @param i_bUseEngineeringFormat [in]
        true if the value should be converted using engineering format,
        false otherwise.

    @return Value converted to string.
*/
QString CDiagTrace::getValString(
    const CEnumScaleDir& i_scaleDir,
    double i_fVal,
    const CUnit* i_pUnit,
    int i_iDigitsCountMax,
    bool i_bUseEngineeringFormat ) const
//------------------------------------------------------------------------------
{
    int iScaleDir = i_scaleDir.enumeratorAsInt();

    CUnit unit = m_arphysValArr[iScaleDir].unit();

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_arphysValArr[iScaleDir].unit(), unit) )
    {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    QString strVal = "---";

    double fVal = i_fVal;
    int    iLeadingDigits = 1;
    int    iTrailingDigits = 1;
    int    iExponentDigits = 0;
    double fRes = getValuesRes(i_scaleDir).getVal(unit);

    if( fVal >= 10.0 )
    {
        iLeadingDigits = static_cast<int>(log10(fabs(fVal)))+1;
    }
    if( fRes < 0.1 )
    {
        double fResLog = log10(fabs(fRes));
        double fResLogInt;
        double fResLogFrac = modf(fResLog,&fResLogInt);
        iTrailingDigits = static_cast<int>(-fResLog);
        if( fResLogFrac > 0.0 )
        {
            iTrailingDigits += 1;
        }
    }
    if( i_iDigitsCountMax > 0 || i_bUseEngineeringFormat )
    {
        if( ( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
            || ( i_bUseEngineeringFormat && (iLeadingDigits > 1) ) )
        {
            if( fVal > 1.0 )
            {
                iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
            }
            else if( fVal < 1.0 )
            {
                iExponentDigits = static_cast<int>(log10(static_cast<double>(iTrailingDigits)))+1;
            }
            iTrailingDigits += iLeadingDigits-1;
            iLeadingDigits = 1;
            if( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
            {
                iTrailingDigits = i_iDigitsCountMax - iLeadingDigits;
            }
        }
    }
    if( iExponentDigits > 0 )
    {
        strVal = QString::number(
            /* fVal       */ fVal,
            /* chFormat   */ 'e',
            /* iPrecision */ iTrailingDigits );
    }
    else
    {
        strVal = QString::number(
            /* fVal       */ fVal,
            /* chFormat   */ 'f',
            /* iPrecision */ iTrailingDigits );
    }
    return strVal;

} // getValString

//------------------------------------------------------------------------------
/*! @brief Returns the value at the given source value.

    If the source value is out of range the method returns Invalid.

    @param i_scaleDirSrc [in]
        Scale direction (either X or Y) of the source value.
    @param i_fValSrc [in]
        Source value for which the destination value should be calculated.
    @param i_pUnitSrc [in]
        Pass a reference to a valid unit if the value is not given in the unit of the trace.
    @param i_scaleDirDst [in]
        Scale direction (either X or Y) of the destination value.
        If the source value is on the X-axis the destination value should
        be on the Y-axis and vice versa.
    @param o_pfValDst [out]
        If not nullptr the destination value is returned here.
    @param i_pUnitDst
        If not nullptr the destination value is converted into this unit.
    @param i_bRound2Res
        true if the value should be converted into the resolution of the trace,
        false otherwise.
*/
EValueValidity CDiagTrace::getVal(
    const CEnumScaleDir& i_scaleDirSrc,
    double               i_fValSrc,
    const CUnit*         i_pUnitSrc,
    const CEnumScaleDir& i_scaleDirDst,
    double*              o_pfValDst,
    CUnit*               i_pUnitDst,
    bool                 i_bRound2Res ) const
//------------------------------------------------------------------------------
{
    int iScaleDirSrc = i_scaleDirSrc.enumeratorAsInt();
    int iScaleDirDst = i_scaleDirDst.enumeratorAsInt();

    if( m_arpDiagScale[iScaleDirSrc] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    if( m_arpDiagScale[iScaleDirDst] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }

    for( int idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
    {
        if( !m_arphysValArr[idxScale].isValid() )
        {
            return EValueValidity::Invalid;
        }
    }

    QVector<double> arfValuesSrc = m_arphysValArr[iScaleDirSrc].toDoubleVec(0, EArrayIndexCountAllElements);
    QVector<double> arfValuesDst = m_arphysValArr[iScaleDirDst].toDoubleVec(0, EArrayIndexCountAllElements);

    if( arfValuesSrc.size() == 0 || arfValuesDst.size() == 0 )
    {
        return EValueValidity::Invalid;
    }

    double fValSrc = i_fValSrc;
    CUnit unitValuesSrc = m_arphysValArr[iScaleDirSrc].unit();
    if( i_pUnitSrc != nullptr && *i_pUnitSrc != unitValuesSrc )
    {
        fValSrc = i_pUnitSrc->convertValue(fValSrc, unitValuesSrc);
    }

    int iValCount = arfValuesSrc.size();
    if( arfValuesSrc.size() > arfValuesDst.size() )
    {
        iValCount = arfValuesDst.size();
    }

    EValueValidity validDst = EValueValidity::Invalid;
    double fValSrcPrev = 0.0;
    double fValSrcNext = 0.0;
    double fValDstPrev = 0.0;
    double fValDstNext = 0.0;
    double fResSrc = getValuesRes(i_scaleDirSrc).getVal(unitValuesSrc);

    if( iValCount == 1 )
    {
        fValSrcPrev = arfValuesSrc[0];
        fValDstPrev = arfValuesDst[0];
        fValSrcNext = arfValuesSrc[0];
        fValDstNext = arfValuesDst[0];

        if ((fValSrc >= (arfValuesSrc[0]-fResSrc)) && (fValSrc <= (arfValuesSrc[0]+fResSrc)))
        {
            validDst = EValueValidity::Valid;
        }
    }
    else if( iValCount > 1 )
    {
        if( fValSrc >= (arfValuesSrc[0]-fResSrc/2.0) && fValSrc <= (arfValuesSrc[iValCount-1]+fResSrc/2.0) )
        {
            validDst = EValueValidity::Valid;

            const CDiagScale* pDiagScaleSrc = m_arpDiagScale[iScaleDirSrc];

            double fScaleMinSrc = arfValuesSrc[0];
            double fScaleMaxSrc = arfValuesSrc[iValCount-1];
            double fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
            double fValSrcTmp = fValSrc;

            if( pDiagScaleSrc->getSpacing() == ESpacing::Logarithmic )
            {
                if( fScaleMinSrc > 0.0 && fScaleMaxSrc > 0.0 && fValSrc > 0.0 )
                {
                    fValSrcTmp     = log10(fValSrc);
                    fScaleMinSrc   = log10(arfValuesSrc[0]);
                    fScaleMaxSrc   = log10(arfValuesSrc[iValCount-1]);
                    fScaleRangeSrc = fScaleMaxSrc - fScaleMinSrc;
                }
            }

            int idxVal = static_cast<int>( static_cast<double>(iValCount-1) * (fValSrcTmp - fScaleMinSrc) / fScaleRangeSrc );

            if( fValSrc > arfValuesSrc[idxVal] && idxVal >= 0 && idxVal < (iValCount-1) )
            {
                const double* pfValSrcPrev = &arfValuesSrc[idxVal];
                const double* pfValDstPrev = &arfValuesDst[idxVal];
                const double* pfValSrcNext = &arfValuesSrc[idxVal+1];
                const double* pfValDstNext = &arfValuesDst[idxVal+1];

                for( ; idxVal < iValCount-1; idxVal++, pfValSrcPrev++, pfValSrcNext++, pfValDstPrev++, pfValDstNext++ )
                {
                    fValSrcPrev = *pfValSrcPrev;
                    fValSrcNext = *pfValSrcNext;
                    fValDstPrev = *pfValDstPrev;
                    fValDstNext = *pfValDstNext;

                    if( fValSrc >= fValSrcPrev && fValSrc <= fValSrcNext )
                    {
                        break;
                    }
                }
            }
            else if( fValSrc < arfValuesSrc[idxVal] && idxVal > 0 && idxVal < iValCount )
            {
                const double* pfValSrcPrev = &arfValuesSrc[idxVal-1];
                const double* pfValDstPrev = &arfValuesDst[idxVal-1];
                const double* pfValSrcNext = &arfValuesSrc[idxVal];
                const double* pfValDstNext = &arfValuesDst[idxVal];

                for( ; idxVal > 0; idxVal--, pfValSrcPrev--, pfValSrcNext--, pfValDstPrev--, pfValDstNext-- )
                {
                    fValSrcPrev = *pfValSrcPrev;
                    fValSrcNext = *pfValSrcNext;
                    fValDstPrev = *pfValDstPrev;
                    fValDstNext = *pfValDstNext;

                    if( fValSrc >= fValSrcPrev && fValSrc <= fValSrcNext )
                    {
                        break;
                    }
                }
            }
            else if( fValSrc >= (arfValuesSrc[idxVal]-fResSrc/2.0) && fValSrc <= (arfValuesSrc[idxVal]+fResSrc/2.0) && idxVal >= 0 && idxVal < iValCount )
            {
                fValSrcPrev = arfValuesSrc[idxVal];
                fValDstPrev = arfValuesDst[idxVal];
                fValSrcNext = arfValuesSrc[idxVal];
                fValDstNext = arfValuesDst[idxVal];
            }
        }
    }

    if( fValSrcNext < fValSrcPrev || validDst != EValueValidity::Valid )
    {
        return EValueValidity::Invalid;
    }

    if( o_pfValDst != nullptr )
    {
        double fValDst = 0.0;

        if( fValSrcNext > fValSrcPrev )
        {
            double fDistSrc = fValSrcNext - fValSrcPrev;
            double fDistDst = fValDstNext - fValDstPrev;
            double fM = fDistDst / fDistSrc;
            double fT = fValDstPrev - fM * fValSrcPrev;
            fValDst = fM * fValSrc + fT;
        }
        else // if( fValSrcNext == fValSrcPrev )
        {
            fValDst = fValDstPrev;
        }

        CUnit unitDst = m_arphysValArr[iScaleDirDst].unit();
        if( i_pUnitDst != nullptr && *i_pUnitDst != unitDst )
        {
            fValDst = m_arphysValArr[iScaleDirDst].unit().convertValue(fValDst, unitDst);
        }

        if (i_bRound2Res)
        {
            double fResDst = getValuesRes(i_scaleDirDst).getVal(unitDst);
            if (fResDst != 0.0)
            {
                fValDst = Math::round2Resolution(fValDst, fabs(fResDst));
            }
        }
        *o_pfValDst = fValDst;
    }
    return EValueValidity::Valid;

} // getVal

//------------------------------------------------------------------------------
/*! @brief Returns the value in world coordinates (physical value) for the
           given position in pixels.

    @param i_scaleDirSrc [in]
        Scale direction (either X or Y) for which the pixel coordinate should be returned.
    @param i_fPix [in]
        Value in pixel coordinates.
    @param i_pUnit [in]
        You may pass a pointer to a unit if the returned value should be
        converted into another unit than the current scale unit.

    @return Value in world coordinates.
*/
double CDiagTrace::getVal( const CEnumScaleDir& i_scaleDir, double i_fPix, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    if( m_arpDiagScale[i_scaleDir.enumeratorAsInt()] == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultObjNotInList);
    }
    return m_arpDiagScale[i_scaleDir.enumeratorAsInt()]->getVal(i_fPix, i_pUnit);
}

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the process depth as defined by the given update flags.

    @param i_uUpdateFlags [in]
        Bitmap with flags defining the process depth which need to be recalculated.
*/
void CDiagTrace::invalidate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;
    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal)) {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "invalidate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // A trace object does not draw themselves into a pixmap. Only data
        // processing is needed. Pixmap and widget processing is not needed.
        m_uUpdateFlags |= (i_uUpdateFlags & ~(EUpdatePixmap|EUpdateWidget));
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
}

//------------------------------------------------------------------------------
/*! @brief Validates the process depth as defined by the given update flags.

    @param i_uUpdateFlags [in]
        Bitmap with flags defining the process depth which has been processed.
*/
void CDiagTrace::validate( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;
    if (areMethodCallsActive(m_pTrcAdminObjValidate, EMethodTraceDetailLevel::ArgsNormal)) {
        strTrcMsg = updateFlags2Str(i_uUpdateFlags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjValidate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "validate",
        /* strAddInfo   */ strTrcMsg );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "OldUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }

    if( i_uUpdateFlags != EUpdateNone )
    {
        // If a bit is set the corresponding update process need to be executed.
        // To validate the process depth the bit need to be reset.
        m_uUpdateFlags &= ~i_uUpdateFlags;
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        strTrcMsg  = "NewUpdFlags=";
        strTrcMsg += updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strTrcMsg);
    }
}

//------------------------------------------------------------------------------
/*! @brief Updates the internal data of the process depth as defined by the
           given update flags.

    @param i_uUpdateFlags [in]
        Bitmap with flags defining the process depth which has been processed.
*/
void CDiagTrace::update( unsigned int i_uUpdateFlags )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjUpdate, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = updateFlags2Str(i_uUpdateFlags);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ strMthInArgs );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strRuntimeInfo = "OldUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strRuntimeInfo);
    }

    // Layout processing is the task of the diagram:
    if( i_uUpdateFlags & EUpdateLayout && m_uUpdateFlags & EUpdateLayout )
    {
        validate(EUpdateLayout);
    }
    else if( i_uUpdateFlags & EUpdateData && m_uUpdateFlags & EUpdateData )
    {
        for( int idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
        {
            CDiagScale* pDiagScale = m_arpDiagScale[idxScale];

            if( pDiagScale != nullptr )
            {
                if (pDiagScale->getScale().unit() != m_arphysValArr[idxScale].unit())
                {
                    m_arphysValArr[idxScale].convertValues(pDiagScale->getScale().unit());
                }
            }
        }
        validate(EUpdateData);
        emit traceChanged(this);
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strRuntimeInfo = "NewUpdFlags: " + updateFlags2Str(m_uUpdateFlags);
        mthTracer.trace(strRuntimeInfo);
    }

} // update

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Clones the object and adds the clone to the target diagram.

    @param i_pDiagramTrg [in]
        Pointer to diagram the cloned object should be added.

    @return Cloned object.
*/
CDiagTrace* CDiagTrace::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    const CDiagScale* pDiagScaleXSrc = m_arpDiagScale[EScaleDirX];
    const CDiagScale* pDiagScaleYSrc = m_arpDiagScale[EScaleDirY];

    if( pDiagScaleXSrc == nullptr || pDiagScaleYSrc == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleX = i_pDiagramTrg->findDiagScale(pDiagScaleXSrc->getObjName());
    CDiagScale* pDiagScaleY = i_pDiagramTrg->findDiagScale(pDiagScaleYSrc->getObjName());

    if( pDiagScaleX == nullptr || pDiagScaleY == nullptr )
    {
        return nullptr;
    }

    CDiagTrace* pDiagTraceCloned = new CDiagTrace(
        /* strObjName  */ m_strObjName,
        /* pDiagScaleX */ pDiagScaleX,
        /* pDiagScaleY */ pDiagScaleY,
        /* physValResX */ m_arphysValArr[EScaleDirX].getRes(),
        /* physValResY */ m_arphysValArr[EScaleDirY].getRes() );

    int idxScale;

    for( idxScale = 0; idxScale < CEnumScaleDir::count(); idxScale++ )
    {
        pDiagTraceCloned->m_arphysValArr[idxScale] = m_arphysValArr[idxScale];
    }
    i_pDiagramTrg->addDiagTrace(pDiagTraceCloned);

    return pDiagTraceCloned;

} // clone
