
#ifndef ZSPhysVal_PhysUnitsFctConvert_h
#define ZSPhysVal_PhysUnitsFctConvert_h

#include "ZSPhysVal/ZSPhysVal.h"

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

namespace ZS
{
namespace PhysVal
{
class CUnitsTreeEntryPhysUnit;

typedef enum
{
    EFctConvert_Undefined         =  0,
    EFctConvert_mMULxADDt         =  1, // y = m*x + t
    EFctConvert_mLOGxADDt         =  2, // y = m*log10(x) + t
    EFctConvert_EXP__xADDt_DIVm_  =  3, // y = 10exp((x+t)/m)
    //EFctConvert_xMULr             =  4, // y = x*r
    //EFctConvert_xDIVr             =  5, // y = x/r
    //EFctConvert_SQRxDIVr          =  6, // y = (x*x)/r
    //EFctConvert_SQRT_xMULr_       =  7, // y = sqrt(x*r)
    //EFctConvert_SQRxMULr          =  8, // y = (x*x)*r
    //EFctConvert_SQRT_xDIVr_       =  9, // y = sqrt(x/r)
    //EFctConvert_mMULxADDtADDkLOGr = 10, // y = m*x + t + k*log10(r)
    EFctConvert_Count
}   EFctConvert;

//******************************************************************************
struct ZSPHYSVALDLL_API SFctConvert
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::PhysVal"; }
    static QString ClassName() { return "SFctConvert"; }
public: // class methods
    static QString FormatM( double i_fM );
    static QString FormatT( double i_fT );
    static QString FormatOperand( double i_fOp );
public: // ctors and dtor
    SFctConvert();
    SFctConvert(
        EFctConvert i_fctConvertType,
        CUnitsTreeEntryPhysUnit* i_pPhysUnitSrc,
        CUnitsTreeEntryPhysUnit* i_pPhysUnitDst,
        const CPhysVal& i_physValM,
        const CPhysVal& i_physValT = CPhysVal(),
        const CPhysVal& i_physValK = CPhysVal());
    SFctConvert(SFctConvert& i_other);
    SFctConvert(const SFctConvert& i_other);
    SFctConvert(SFctConvert&& i_other);
    virtual ~SFctConvert();
public: // operators
    SFctConvert& operator=(SFctConvert& i_other);
    SFctConvert& operator=(const SFctConvert& i_other);
    SFctConvert& operator=(SFctConvert&& i_other);
public: // instance methods
    void buildFctConvertName();
    bool isValid() const;
public: // instance members
    CUnitsTreeEntryPhysUnit* m_pPhysUnitSrc;
    CUnitsTreeEntryPhysUnit* m_pPhysUnitDst;
    EFctConvert m_fctConvertType;
    QString m_strFctConvertName;
    /*!< Gradient m in conversion routine y = mx + t. */
    CPhysVal m_physValM;
    /*!< Offset t in conversion routine y = mx + t. */
    CPhysVal m_physValT;
    /*!< Operand k in logarithmic conversion routine y = mx + t k*log10(r). */
    CPhysVal m_physValK;

}; // struc SFctConvert

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysVal_PhysUnitsFctConvert_h
