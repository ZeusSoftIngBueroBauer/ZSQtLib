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

#ifndef ZSDiagObjValueProvider_h
#define ZSDiagObjValueProvider_h

#include <QtCore/qobject.h>

#include "ZSDiagram/ZSDiagObj.h"
#include "ZSPhysVal/ZSPhysVal.h"

namespace ZS
{
namespace Diagram
{
//******************************************************************************
class ZSDIAGRAMDLL_API CDiagObjValueProvider : public CDiagObj
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString ClassName() { return "CDiagObjValueProvider"; }
public: // type definitions and constants
    typedef enum {
        EMathOpMin            = 0,
        EMathOpNone           = 0, // Value = Op1
        EMathOpAddition       = 1, // Value = Op1 + Op2
        EMathOpSubtraction    = 2, // Value = Op1 - Op2
        EMathOpMax            = 2,
        EMathOpCount
    }   EMathOp;
public: // ctors and dtor
    CDiagObjValueProvider(
        const QString& i_strObjName,
        EMathOp        i_mathOp,
        CDiagObj*      i_pDiagObjOp1,
        EScaleDir      i_scaleDirOp1,
        CDiagObj*      i_pDiagObjOp2 = nullptr,
        EScaleDir      i_scaleDirOp2 = EScaleDirUndefined );
    virtual ~CDiagObjValueProvider();
public: // must overridables of base class CDiagObj
    virtual QString className() { return ClassName(); }
public: // instance methods
    void setMathOp( EMathOp i_mathOp );
    EMathOp getMathOp() const;
    void setOp1( CDiagObj* i_pDiagObj, EScaleDir i_scaleDir );
    CDiagObj* getDiagObjOp1();
    EScaleDir getScaleDirOp1() const;
    void setOp2( CDiagObj* i_pDiagObj, EScaleDir i_scaleDir );
    CDiagObj* getDiagObjOp2();
    EScaleDir getScaleDirOp2() const;
    void setValueFormatProvider( const PhysVal::SValueFormatProvider& i_valueFormat );
    PhysVal::SValueFormatProvider getValueFormatProvider() const;
public: // overridables
    virtual PhysVal::CPhysVal getVal() const;
    virtual QString getValString() const;
public: // must overridables of base class CDiagObj
    virtual CDiagObj* clone( CDataDiagram* i_pDiagramTrg ) const;
    virtual void update( unsigned int i_uUpdateFlags, QPaintDevice* i_pPaintDevice = nullptr );
protected slots:
    void op1ValueXChanged( ZS::Diagram::CDiagObj* );
    void op1ValueYChanged( ZS::Diagram::CDiagObj* );
    void op2ValueXChanged( ZS::Diagram::CDiagObj* );
    void op2ValueYChanged( ZS::Diagram::CDiagObj* );
    void op1VisibilityChanged( ZS::Diagram::CDiagObj* );
    void op2VisibilityChanged( ZS::Diagram::CDiagObj* );
private: // copy ctor not allowed
    CDiagObjValueProvider( const CDiagObjValueProvider& );
private: // assignment operator not allowed
    void operator=( const CDiagObjValueProvider& );
protected:  // instance members
    EMathOp                       m_mathOp;
    CDiagObj*                     m_pDiagObjOp1;
    EScaleDir                     m_scaleDirOp1;
    CDiagObj*                     m_pDiagObjOp2;
    EScaleDir                     m_scaleDirOp2;
    PhysVal::SValueFormatProvider m_valueFormat;
    PhysVal::CPhysVal             m_physVal;
    QString                       m_strVal;

}; // class CDiagObjValueProvider

} // namespace Diagram

} // namespace ZS

#endif // #ifndef ZSDiagObjValueProvider_h
