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
public: // class methods
    static int CalculateDivLines4LinSpacing(
        double  i_fScaleMinVal,
        double  i_fScaleMaxVal,
        int     i_iScaleRangePix,
        double  i_fDivLineDistMinVal,
        int     i_iDivLineDistMinPix,
        bool    i_bUseDivLineDistValDecimalFactor25,
        double* o_pfDivLineFirstVal,
        double* o_pfDivLineDistFirstPix,
        double* o_pfDivLineDistVal,
        double* o_pfDivLineDistPix );
public: // ctors and dtor
    CDiagScale(
        const QString& i_strObjName,
        EScaleDir      i_scaleDir,
        const SScale&  i_scale );
    virtual ~CDiagScale();
signals:
    void scaleChanged( ZS::Diagram::CDiagScale* i_pDiagScale );
public: // instance methods
    QString getObjName() const;
    EScaleDir getScaleDir() const;
    CDataDiagram* getDiagram();
    void setSpacing( ESpacing i_spacing );
    ESpacing getSpacing() const;
    void setScale( const SScale& i_scale );
    SScale getScale() const;
    bool isScaleValid() const;
    void setDivLineDistMinPix( int i_iLayer, int i_iDistMinPix );
    int getDivLineDistMinPix( int i_iLayer ) const;
    void zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix );
    void zoomIn( double i_fMinVal, double i_fMaxVal, PhysVal::CUnit* i_pUnit = nullptr );
    void zoomOut( bool i_bComplete = false );
    void setGeometry( int i_iScaleMinValPix, int i_iScaleMaxValPix );
    bool areDivLinesCalculated() const;
    double getPixRes( PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getScaleRes( PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getScaleRes( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double round2ScaleRes( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    int getScaleMinValPix() const;
    int getScaleMaxValPix() const;
    int getScaleRangePix() const;
    int getDivLineCount( int i_iLayer ) const;
    double getDivLineVal( int i_iLayer, int i_idxDivLine, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getDivLinePix( int i_iLayer, int i_idxDivLine ) const;
    double getDivLineDistPix( int i_iLayer, int i_idxDivLine1, int i_idxDivLine2 ) const;
    int getValPix( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    QString getValString( double i_fVal, PhysVal::CUnit* i_pUnit = nullptr, int i_iDigitsCountMax = 0, bool i_bUseEngineeringFormat = false ) const;
    double getVal( double i_fPix, PhysVal::CUnit* i_pUnit = nullptr ) const;
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
protected:  // class members
    static bool   s_bClassInitialised;
    static double s_arfScaleRangeFacPixDivValLog[9];
protected:  // instance members
    QString                   m_strObjName;
    CDataDiagram*             m_pDiagram;
    EScaleDir                 m_scaleDir;
    ESpacing                  m_spacing;
    SScale                    m_scale;
    double                    m_fScaleRes;
    bool                      m_bScaleResCalculated;
    int                       m_iScaleMinValPix;
    int                       m_iScaleMaxValPix;
    double                    m_fPixRes;
    int                       m_ariDivLineDistMinPix[EDivLineLayerCount];
    int                       m_ariDivLineCount[EDivLineLayerCount];
    double*                   m_ararfDivLineVal[EDivLineLayerCount];
    double*                   m_ararfDivLinePix[EDivLineLayerCount];
    bool                      m_bDivLinesCalculated;
    CDiagScale*               m_pDiagScaleNext;
    CDiagScale*               m_pDiagScalePrev;
    int                       m_iZoomCount;
    SZoomStackEntry*          m_pZoomStackFirst;
    SZoomStackEntry*          m_pZoomStackLast;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjLayout;

}; // class CDiagScale

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagScale_h
