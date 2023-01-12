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

#ifndef ZSPhysVal_PhysUnitsFctConvert_h
#define ZSPhysVal_PhysUnitsFctConvert_h

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

#include "ZSPhysVal/ZSPhysValDllMain.h"

namespace ZS
{
namespace PhysVal
{
class CPhysUnitTreeEntry;

typedef enum
{
    EFctConvert_Undefined         =  0,
    EFctConvert_mMULxADDt         =  1, // y = m*x + t
    EFctConvert_mLOGxADDt         =  2, // y = m*log10(x) + t
    EFctConvert_EXP__xADDt_DIVm_  =  3, // y = 10exp((x+t)/m)
    EFctConvert_xMULr             =  4, // y = x*r
    EFctConvert_xDIVr             =  5, // y = x/r
    EFctConvert_SQRxDIVr          =  6, // y = x²/r
    EFctConvert_SQRT_xMULr_       =  7, // y = sqrt(x*r)
    EFctConvert_SQRxMULr          =  8, // y = x²*r
    EFctConvert_SQRT_xDIVr_       =  9, // y = sqrt(x/r)
    EFctConvert_mMULxADDtADDkLOGr = 10, // y = m*x + t + k*log10(r)
    EFctConvert_Count
}   EFctConvert;

//******************************************************************************
class ZSPHYSVALDLL_API CFctConvert
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "CFctConvert"; }
public: // class methods
    static QString FormatM( double i_fM );
    static QString FormatT( double i_fT );
    static QString FormatOperand( double i_fOp );
public: // ctors and dtor
    CFctConvert();
    CFctConvert(CFctConvert& i_other);
    CFctConvert(const CFctConvert& i_other);
    CFctConvert(CFctConvert&& i_other);
    virtual ~CFctConvert();
public: // operators
    CFctConvert& operator=(CFctConvert& i_other);
    CFctConvert& operator=(const CFctConvert& i_other);
    CFctConvert& operator=(CFctConvert&& i_other);
public: // instance methods
    void buildFctConvertName();
    bool isValid() const;
public: // instance members
    CPhysUnitTreeEntry* m_pPhysUnitSrc;
    CPhysUnitTreeEntry* m_pPhysUnitDst;
    CPhysUnitTreeEntry* m_pPhysUnitRef;
    EFctConvert m_fctConvertType;
    QString m_strFctConvertName;
    double m_fM;
    double m_fT;
    double m_fK;

}; // class CFctConvert

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsFctConvert_h
