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

#ifndef ZSDiagTrace_h
#define ZSDiagTrace_h

#include "ZSDiagram/ZSDiagramAux.h"
#include "ZSPhysVal/ZSPhysValArr.h"

#include <QtCore/qobject.h>

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Diagram
{
class CDataDiagram;
class CPixmapDiagram;
class CWdgtDiagram;
class CDiagScale;

//******************************************************************************
class ZSDIAGRAMDLL_API CDiagTrace : public QObject
//******************************************************************************
{
friend class CDataDiagram;
friend class CPixmapDiagram;
friend class CWdgtDiagram;
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Diagram"; }
    static QString ClassName() { return "CDiagTrace"; }
public: // ctors and dtor
    CDiagTrace(
        const QString& i_strObjName,
        CDiagScale*    i_pDiagScaleX,
        CDiagScale*    i_pDiagScaleY );
    virtual ~CDiagTrace();
signals:
    void traceChanged( ZS::Diagram::CDiagTrace* i_pDiagTrace );
public: // instance methods
    QString getObjName() const;
    CDataDiagram* getDiagram();
    CDiagScale* getDiagScale( EScaleDir i_scaleDir );
    ESpacing getSpacing( EScaleDir i_scaleDir ) const;
    bool isScaleValid( EScaleDir i_scaleDir ) const;
    SScale getScale( EScaleDir i_scaleDir ) const;
    int getScaleMinValPix( EScaleDir i_scaleDir ) const;
    int getScaleMaxValPix( EScaleDir i_scaleDir ) const;
    int getScaleRangePix( EScaleDir i_scaleDir ) const;
    double getScaleRes( EScaleDir i_scaleDir, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getScaleRes( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double round2ScaleRes( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getValRes( EScaleDir i_scaleDir, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double getValRes( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    double round2ValRes( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    void setValues( EScaleDir i_scaleDir, const PhysVal::CPhysValArr& i_physValArr );
    void setValues(
        EScaleDir              i_scaleDir,
        const QVector<double>& i_arfValues,
        PhysVal::CUnit*        i_pUnitVal = nullptr,
        double                 i_fRes = 0.0,
        PhysVal::CUnit*        i_pUnitRes = nullptr );
    bool areValuesValid( EScaleDir i_scaleDir ) const;
    int getValCount( EScaleDir i_scaleDir ) const;
    QVector<double> getValues( EScaleDir i_scaleDir ) const;
    int getValPix( EScaleDir i_scaleDir, double i_fVal, PhysVal::CUnit* i_pUnit = nullptr ) const;
    QString getValString(
        EScaleDir       i_scaleDir,
        double          i_fVal,
        PhysVal::CUnit* i_pUnit = nullptr,
        int             i_iDigitsCountMax = 0,
        bool            i_bUseEngineeringFormat = false ) const;
    ZS::System::EValueValidity getVal(
        EScaleDir       i_scaleDirSrc,
        double          i_fValSrc,
        PhysVal::CUnit* i_pUnitSrc,
        EScaleDir       i_scaleDirDst,
        double*         o_pfValDst,
        PhysVal::CUnit* i_pUnitDst = nullptr,
        bool            i_bRound2Res = true ) const;
    double getVal( EScaleDir i_scaleDir, double i_fPix, PhysVal::CUnit* i_pUnit = nullptr ) const;
    void invalidate( unsigned int i_uUpdateFlags );
    void validate( unsigned int i_uUpdateFlags );
    void update( unsigned int i_uUpdateFlags );
protected: // overridables
    virtual CDiagTrace* clone( CDataDiagram* i_pDiagramTrg ) const;
private: // copy ctor not allowed
    CDiagTrace( const CDiagTrace& );
private: // assignment operator not allowed
    void operator=( const CDiagTrace& );
protected:  // class members
    static bool s_bClassInitialised;
protected:  // instance members
    QString                   m_strObjName;
    CDataDiagram*             m_pDiagram;
    CDiagScale*               m_arpDiagScale[EScaleDirCount];
    PhysVal::CPhysVal         m_arphysValRes[EScaleDirCount];
    PhysVal::CPhysValArr      m_arphysValArr[EScaleDirCount];
    unsigned int              m_uUpdateFlags;
    CDiagTrace*               m_pDiagTraceNext;
    CDiagTrace*               m_pDiagTracePrev;
    ZS::System::CTrcAdminObj* m_pTrcAdminObj;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjUpdate;
    ZS::System::CTrcAdminObj* m_pTrcAdminObjValidate;

}; // class CDiagTrace

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagTrace_h
