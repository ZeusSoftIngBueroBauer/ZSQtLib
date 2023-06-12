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
        const QString& i_strObjName,
        EScaleDir      i_scaleDir,
        const SScale&  i_scale );
    virtual ~CDiagScale();
public: // overridables
    virtual QString className() { return ClassName(); }
signals:
    void spacingChanged( ESpacing i_spacing );
    void scaleChanged( const SScale& i_scale );
    void scaleResChanged( double i_fRes );
    void geometryChanged( int i_iMinVal_px, int i_iMaxVal_px );
    void divLineDistMinPixChanged( EDivLineLayer i_layer, int i_iDivLineDistMin_px );
public: // instance methods
    QString getObjName() const;
    EScaleDir getScaleDir() const;
    CDataDiagram* getDiagram();
    void setSpacing( const CEnumSpacing& i_spacing );
    ESpacing getSpacing() const;
    void setScale( const SScale& i_scale );
    SScale getScale() const;
    bool isScaleValid() const;
    void setGeometry( int i_iMinVal_px, int i_iMaxVal_px );
    int getMinValPix() const;
    int getMaxValPix() const;
    int getRangePix() const;
    void setDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer, int i_iDistMinPix );
    int getDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer ) const;
    bool areDivLinesCalculated() const;
    double getScaleResPerPx( bool i_bRoundedToLowerDecade = true ) const;
    double getScaleResPerPx( const PhysVal::CUnit* i_pUnit, bool i_bRoundedToLowerDecade = true ) const;
    double getScaleResPerPx( double i_fVal, bool i_bRoundedToLowerDecade = true ) const;
    double getScaleResPerPx( double i_fVal, const PhysVal::CUnit* i_pUnit, bool i_bRoundedToLowerDecade = true ) const;
    double round2ScaleRes( double i_fVal, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    int getDivLineCount( const CEnumDivLineLayer& i_eLayer ) const;
    double getDivLineDistMin( const CEnumDivLineLayer& i_eLayer, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLineVal( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLinePix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1 ) const;
    double getDivLineDist( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLineDistPix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2 ) const;
    int getValPix( double i_fVal, const PhysVal::CUnit* i_pUnit = nullptr ) const;
    QString getValString( double i_fVal, const PhysVal::CUnit* i_pUnit = nullptr, int i_iDigitsCountMax = 0, bool i_bUseEngineeringFormat = false ) const;
    double getVal( double i_fPix, const PhysVal::CUnit* i_pUnit = nullptr ) const;
public: // instance methods
    void zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix );
    void zoomIn( double i_fMinVal, double i_fMaxVal, PhysVal::CUnit* i_pUnit = nullptr );
    void zoomOut( bool i_bComplete = false );
public: // overridables
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
        SZoomStackEntry( const SScale& i_scale );
        SScale           m_scale;
        SZoomStackEntry* m_pStackEntryNext;
        SZoomStackEntry* m_pStackEntryPrev;
    };
protected:  // instance memthods
    void pushScale( const SScale& i_scale );
    SScale popScale();
    SScale clearScaleStack();
protected: // instance methods (method tracing)
    void emit_spacingChanged( ESpacing i_spacing );
    void emit_scaleChanged( const SScale& i_scale );
    void emit_scaleResChanged( double i_fRes );
    void emit_geometryChanged( int i_iMinVal_px, int i_iMaxVal_px );
    void emit_divLineDistMinPixChanged( EDivLineLayer i_layer, int i_iDivLineDistMin_px );
protected:  // class members
    static bool s_bClassInitialised;
    static double s_arfScaleRangeFacPixDivValLog[9];
protected:  // instance members
    QString m_strObjName;
    CDataDiagram* m_pDiagram;
    EScaleDir m_scaleDir;
    ESpacing m_spacing;
    SScale m_scale;
    double m_fScaleRes_perPx;
    bool m_bResCalculated;
    int m_iMinVal_px;
    int m_iMaxVal_px;
    QVector<int> m_ariDivLineDistMin_px; //[EDivLineLayerCount];
    QVector<int> m_ariDivLineCount; //[EDivLineLayerCount];
    double m_fDivDistMinVal;
    QVector<QVector<double>> m_ararfDivLineVal; //[EDivLineLayerCount];
    QVector<QVector<double>> m_ararfDivLine_px; //[EDivLineLayerCount];
    bool m_bDivLinesCalculated;
    int m_iZoomCount;
    SZoomStackEntry* m_pZoomStackFirst;
    SZoomStackEntry* m_pZoomStackLast;
protected:  // instance members
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;

}; // class CDiagScale

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagScale_h
