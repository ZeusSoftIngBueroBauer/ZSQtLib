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

#ifndef ZSDiagScale_h
#define ZSDiagScale_h

#include "ZSDiagram/ZSDiagramAux.h"
#include "ZSSys/ZSSysMathScaleDivLines.h"

#include <QtCore/qobject.h>

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
//******************************************************************************
/*! @brief Class defining the scale values.

    A scale is defined by the minimum and maximum value.
    The scale values have a physical unit and a resolution.

    The resolution defines the number of significant digits when converting
    the minimum and maximum values to their string representations.
*/
class ZSDIAGRAMDLL_API CScale
//******************************************************************************
{
public: // ctors
    CScale();
    CScale( const CScale& i_scaleOther );
    CScale(
        double i_fMin, double i_fMax, const PhysVal::CUnit& i_unit,
        const PhysVal::CPhysValRes& i_physValRes);
public: // instance methods
    void setMinVal(const PhysVal::CPhysVal& i_physVal);
    void setMinVal(double i_fVal, const PhysVal::CUnit& i_unit);
    PhysVal::CPhysVal minVal() const;
    void setMaxVal(const PhysVal::CPhysVal& i_physVal);
    void setMaxVal(double i_fVal, const PhysVal::CUnit& i_unit);
    PhysVal::CPhysVal maxVal() const;
    void setRangeVal(const PhysVal::CPhysVal& i_physVal);
    void setRangeVal(double i_fVal, const PhysVal::CUnit& i_unit);
    PhysVal::CPhysVal rangeVal() const;
    void setUnit(const PhysVal::CUnit& i_unit);
    PhysVal::CUnit unit() const;
    void setRes(const PhysVal::CPhysValRes& i_physValRes);
    PhysVal::CPhysValRes res() const;
public: // operators
    bool operator == ( const CScale& i_scaleOther ) const;
    bool operator != ( const CScale& i_scaleOther ) const;
public: // instance methods
    QString toString() const;
private: // instance members
    /*!< Minimum scale value. */
    double m_fMin;
    /*!< Maximum scale value. */
    double m_fMax;
    /*!< Unit of minimum and maximum scale values. */
    PhysVal::CUnit m_unit;
    /*!< Resolution of minimum and maximum scale values. */
    PhysVal::CPhysValRes m_physValRes;

}; // class CScale

//******************************************************************************
/*! @brief Scale object within a diagram.

    Diagram objects may be linked to the scale object to calculate their
    internal data. E.g. the grid uses both the X and Y scale objects.
    A diagram axis label is either linked to a X or Y scale object.
    Before the internal data of those diagram objects can be calculated
    the diagram will trigger the scale object for updating its internal data
    (position of division lines).

    Scale objects have a geometry defined by the number of pixels used to
    indicate the scale in the diagrams pixmap.

    Scale objects also have a physical minimum and a maximum value and a unit.
    A scale also has a resolution defining the number of significant digits
    when converting the minimum and maximum values to their string representations.

    A scale object either defines the X or Y axis of the diagram.
    The scale object also supports logarithmic spacing.

    The scale object is responsible to calculate division lines.
    The values of the calculated division lines are rounded to a whole number
    of a decimal power.

    To output the number of digits for the minimum and maximum scale values the
    scale resolution is important. If the user enters the value 3.625362 it
    would not be good to limit the output to less than the entered digits.

    The scale object also supports zooming in and zooming out by maintaing
    an internal zoom stack.

    Please note that changing properties of the scale object don't trigger
    recalculation of the internal scale data (e.g. new division line values).
    To update the internal data you need to call the update method of the
    diagram the scale belongs to as when changing the scale also all other
    diagram linked to the scale object must be updated.
    By explictitly triggering the recalculation of internal data via the
    diagrams update method several properties may be set before triggering
    the complex calculation of the diagrams internal data.
*/
class ZSDIAGRAMDLL_API CDiagScale : public QObject
//******************************************************************************
{
friend class CDataDiagram;
friend class CPixmapDiagram;
friend class CWdgtDiagram;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDiagScale"; }
public: // ctors and dtor
    CDiagScale(
        const QString&        i_strObjName,
        ZS::System::EScaleDir i_scaleDir,
        const CScale&         i_scale);
    virtual ~CDiagScale();
public: // overridables
    virtual QString className() { return ClassName(); }
signals:
    void spacingChanged( ZS::System::ESpacing i_spacing );
    void scaleChanged( const CScale& i_scale );
    void geometryChanged( int i_iMinVal_px, int i_iMaxVal_px );
    void divLineDistMinPixChanged( ZS::System::EDivLineLayer i_layer, int i_iDivLineDistMin_px );
public: // instance methods (common properties)
    QString getObjName() const;
    ZS::System::EScaleDir getScaleDir() const;
    CDataDiagram* getDiagram();
    void setSpacing( const ZS::System::CEnumSpacing& i_spacing );
    ZS::System::ESpacing getSpacing() const;
public: // instance methods (scale values, unit and resolution)
    bool isScaleValid() const;
    void setScale( const CScale& i_scale );
    CScale getScale() const;
    void setScaleMinVal( const PhysVal::CPhysVal& i_physVal );
    void setScaleMinVal( double i_fMin, const PhysVal::CUnit& i_unit );
    PhysVal::CPhysVal getScaleMinVal() const;
    void setScaleMaxVal( const PhysVal::CPhysVal& i_physVal );
    void setScaleMaxVal( double i_fMax, const PhysVal::CUnit& i_unit );
    PhysVal::CPhysVal getScaleMaxVal() const;
    void setScaleRangeVal( const PhysVal::CPhysVal& i_physVal );
    void setScaleRangeVal( double i_fMin, const PhysVal::CUnit& i_unit );
    PhysVal::CPhysVal getScaleRangeVal() const;
    void setScaleUnit(const PhysVal::CUnit& i_unit);
    PhysVal::CUnit getScaleUnit() const;
    void setScaleRes( const PhysVal::CPhysValRes& i_physValRes );
    PhysVal::CPhysValRes getScaleRes() const;
public: // instance methods (geometry in pixels)
    void setGeometry( int i_iMinVal_px, int i_iMaxVal_px );
    int getMinValPix() const;
    int getMaxValPix() const;
    int getRangePix() const;
public: // instance methods (calculation of division lines)
    void setDivLineDistMinPix( const ZS::System::CEnumDivLineLayer& i_eLayer, int i_iDistMinPix );
    int getDivLineDistMinPix( const ZS::System::CEnumDivLineLayer& i_eLayer ) const;
    bool areDivLinesCalculated() const;
    int getDivLineCount( const ZS::System::CEnumDivLineLayer& i_eLayer ) const;
    double getDivLineDistMin( const ZS::System::CEnumDivLineLayer& i_eLayer, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLineVal( const ZS::System::CEnumDivLineLayer& i_eLayer, int i_idxDivLine, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLinePix( const ZS::System::CEnumDivLineLayer& i_eLayer, int i_idxDivLine ) const;
    double getDivLineDistVal( const ZS::System::CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLineDistPix( const ZS::System::CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2 ) const;
public: // instance mehods (calculation of division lines)
    const ZS::System::Math::CScaleDivLines& getDivLinesScaler() const;
public: // instance methods (converting values)
    int getValPix( double i_fVal, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getVal( double i_fPix, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    QString getValString( double i_fVal, const PhysVal::CUnit* i_pUnit = nullptr, int i_iDigitsCountMax = 0, bool i_bUseEngineeringFormat = false ) const;
public: // instance methods (zooming)
    void zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix );
    void zoomIn( double i_fMinVal, double i_fMaxVal, PhysVal::CUnit* i_pUnit = nullptr );
    void zoomOut( bool i_bComplete = false );
protected: // overridables
    virtual void update();
protected: // overridables
    virtual CDiagScale* clone( CDataDiagram* i_pDiagramTrg ) const;
private: // copy ctor not allowed
    CDiagScale( const CDiagScale& );
private: // assignment operator not allowed
    void operator=( const CDiagScale& );
protected: // type definitions and constants
    struct SZoomStackEntry
    {
        SZoomStackEntry( const CScale& i_scale );
        CScale           m_scale;
        SZoomStackEntry* m_pStackEntryNext;
        SZoomStackEntry* m_pStackEntryPrev;
    };
protected:  // instance memthods
    void pushScale( const CScale& i_scale );
    CScale popScale();
    CScale clearScaleStack();
protected: // instance methods (method tracing)
    void emit_spacingChanged( ZS::System::ESpacing i_spacing );
    void emit_scaleChanged( const CScale& i_scale );
    void emit_geometryChanged( int i_iMinVal_px, int i_iMaxVal_px );
    void emit_divLineDistMinPixChanged( ZS::System::EDivLineLayer i_layer, int i_iDivLineDistMin_px );
protected:  // instance members
    /*!< Name of the scale (same as QObject::objectName).
         The name will also be saved here for easier visualizing the name in debug sessions. */
    QString m_strObjName;
    /*!< Reference to diagram the scale belongs to. */
    CDataDiagram* m_pDiagram;
    /*!< Scale values (minimum, maximum, unit and resolution). */
    CScale m_scale;
    /*!< Calculates the division lines of the scale. */
    ZS::System::Math::CScaleDivLines m_divLines;
    /*!< Current number of scales pushed to the zoom stack. */
    int m_iZoomCount;
    /*!< Pointer to first zoom stack entry. */
    SZoomStackEntry* m_pZoomStackFirst;
    /*!< Pointer to last zoom stack entry. */
    SZoomStackEntry* m_pZoomStackLast;
protected:  // instance members
    /*!< Trace admin object for the most common (but not too often) called methods. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    /*!< Trace admin object to control trace output of the update method. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    /*!< Trace admin object to control trace output of the layout methods. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;

}; // class CDiagScale

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagScale_h
