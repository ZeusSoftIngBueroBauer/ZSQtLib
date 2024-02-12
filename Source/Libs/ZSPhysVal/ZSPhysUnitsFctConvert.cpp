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

#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
struct SFctConvert
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString SFctConvert::FormatM( double i_fM )
//------------------------------------------------------------------------------
{
    QString strM;
    if( i_fM != 1.0 ) {
        bool bOk = false;
        int iExponent = getExponentFromFactor(i_fM,&bOk);
        if( bOk ) {
            if( iExponent != 0 ) {
                strM = "1e" + QString::number(iExponent);
            }
        } else {
            strM = FormatOperand(i_fM);
        }
    }
    return strM;
}

//------------------------------------------------------------------------------
QString SFctConvert::FormatT( double i_fT )
//------------------------------------------------------------------------------
{
    QString strT;
    if( i_fT != 0.0 ) {
        bool bOk = false;
        int iExponent = getExponentFromFactor(i_fT,&bOk);
        if( bOk ) {
            if( iExponent != 0 ) {
                strT = "1e" + QString::number(iExponent);
            }
        }
        else {
            strT = FormatOperand(i_fT);
        }
    }
    return strT;
}

//------------------------------------------------------------------------------
QString SFctConvert::FormatOperand( double i_fOp )
//------------------------------------------------------------------------------
{
    QString strOp;
    double  fOpAbs = fabs(i_fOp);
    int     iDigitsLeading = 0;
    int     iDigitsTrailing = 0;

    if( fOpAbs == 1.0 )
    {
        iDigitsLeading = 1;
    }
    else if( fOpAbs > 1.0 )
    {
        iDigitsLeading = static_cast<int>(log10(fOpAbs))+1;
    }
    else if( fOpAbs < 1.0 && fOpAbs > 0.0 )
    {
        double fFract, fInt;
        double fDigitsTrailing = log10(fOpAbs);
        iDigitsTrailing = -static_cast<int>(fDigitsTrailing);

        fFract = modf(fDigitsTrailing,&fInt);
        if( fFract != 0.0 )
        {
            iDigitsTrailing += 1;
        }
        if( iDigitsTrailing < 3 )
        {
            iDigitsTrailing = 3;
        }
    }
    if( (iDigitsTrailing == 0 && iDigitsLeading <= 4) || (iDigitsLeading == 0 && iDigitsTrailing <= 3) )
    {
        if( iDigitsTrailing == 0 )
        {
            iDigitsTrailing = 1;
        }
        strOp = QString::number(i_fOp,'f',iDigitsTrailing);
    }
    else
    {
        strOp = QString::number(i_fOp,'e',1);
    }
    return strOp;

} // FormatOperand

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
SFctConvert::SFctConvert() :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(nullptr),
    m_pPhysUnitDst(nullptr),
    m_fctConvertType(EFctConvert_Undefined),
    m_strFctConvertName("---"),
    m_physValM(),
    m_physValT(),
    m_physValK()
{
} // default ctor

//------------------------------------------------------------------------------
SFctConvert::SFctConvert(
    EFctConvert i_fctConvertType,
    CUnitsTreeEntryPhysUnit* i_pPhysUnitSrc,
    CUnitsTreeEntryPhysUnit* i_pPhysUnitDst,
    const CPhysVal& i_physValM,
    const CPhysVal& i_physValT,
    const CPhysVal& i_physValK ) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_pPhysUnitSrc),
    m_pPhysUnitDst(i_pPhysUnitDst),
    m_fctConvertType(i_fctConvertType),
    m_strFctConvertName(),
    m_physValM(i_physValM),
    m_physValT(i_physValT),
    m_physValK(i_physValK)
{
}

//------------------------------------------------------------------------------
SFctConvert::SFctConvert(const SFctConvert& i_other) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_other.m_pPhysUnitSrc),
    m_pPhysUnitDst(i_other.m_pPhysUnitDst),
    m_fctConvertType(i_other.m_fctConvertType),
    m_strFctConvertName(i_other.m_strFctConvertName),
    m_physValM(i_other.m_physValM),
    m_physValT(i_other.m_physValT),
    m_physValK(i_other.m_physValK)
{
} // copy ctor

//------------------------------------------------------------------------------
SFctConvert::SFctConvert(SFctConvert&& i_other) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_other.m_pPhysUnitSrc),
    m_pPhysUnitDst(i_other.m_pPhysUnitDst),
    m_fctConvertType(i_other.m_fctConvertType),
    m_strFctConvertName(i_other.m_strFctConvertName),
    m_physValM(i_other.m_physValM),
    m_physValT(i_other.m_physValT),
    m_physValK(i_other.m_physValK)
{
    i_other.m_pPhysUnitSrc = nullptr;
    i_other.m_pPhysUnitDst = nullptr;
    i_other.m_fctConvertType = static_cast<EFctConvert>(0);
    //i_other.m_strFctConvertName;

} // move ctor

//------------------------------------------------------------------------------
SFctConvert::~SFctConvert()
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = nullptr;
    m_pPhysUnitDst = nullptr;
    m_fctConvertType = static_cast<EFctConvert>(0);
    //m_strFctConvertName;
    //m_physValM;
    //m_physValT;
    //m_physValK;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
SFctConvert& SFctConvert::operator=(const SFctConvert& i_other)
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = i_other.m_pPhysUnitSrc;
    m_pPhysUnitDst = i_other.m_pPhysUnitDst;
    m_fctConvertType = i_other.m_fctConvertType;
    m_strFctConvertName = i_other.m_strFctConvertName;
    m_physValM = i_other.m_physValM;
    m_physValT = i_other.m_physValT;
    m_physValK = i_other.m_physValK;

    return *this;
}

//------------------------------------------------------------------------------
SFctConvert& SFctConvert::operator=(SFctConvert&& i_other)
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = i_other.m_pPhysUnitSrc;
    m_pPhysUnitDst = i_other.m_pPhysUnitDst;
    m_fctConvertType = i_other.m_fctConvertType;
    m_strFctConvertName = i_other.m_strFctConvertName;
    m_physValM = i_other.m_physValM;
    m_physValT = i_other.m_physValT;
    m_physValK = i_other.m_physValK;

    i_other.m_pPhysUnitSrc = nullptr;
    i_other.m_pPhysUnitDst = nullptr;
    i_other.m_fctConvertType = static_cast<EFctConvert>(0);
    //i_other.m_strFctConvertName;

    return *this;

} // move operator

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void SFctConvert::buildFctConvertName()
//------------------------------------------------------------------------------
{
    QString strM;
    QString strT;
    QString strK;
    QString strFctConvertName;

    CUnitsTreeEntryGrpPhysUnits* pPhysSizeDst = nullptr;
    QString strFormulaSymbolDst = "?";
    QString strUnitSymbolDst = "?";

    if( m_pPhysUnitDst != nullptr ) {
        strUnitSymbolDst = m_pPhysUnitDst->symbol();
        pPhysSizeDst = m_pPhysUnitDst->physSize();
        if( pPhysSizeDst != nullptr ) {
            strFormulaSymbolDst = pPhysSizeDst->getFormulaSymbol();
        }
    }

    strFctConvertName = strFormulaSymbolDst + "/" + strUnitSymbolDst + " = ";

    CUnitsTreeEntryGrpPhysUnits* pPhysSizeSrc = nullptr;
    QString strFormulaSymbolSrc = "?";
    QString strUnitSymbolSrc = "?";

    if( m_pPhysUnitSrc != nullptr )
    {
        strUnitSymbolSrc = m_pPhysUnitSrc->symbol();
        pPhysSizeSrc = m_pPhysUnitSrc->physSize();
        if( pPhysSizeSrc != nullptr ) {
            strFormulaSymbolSrc = pPhysSizeSrc->getFormulaSymbol();
        }
    }

    double fM = m_physValM.getVal();
    double fT = m_physValT.getVal();
    double fK = m_physValK.getVal();

    switch( m_fctConvertType )
    {
        case EFctConvert_mMULxADDt:
        {
            strM = FormatM(fM);
            strT = FormatT(fT);

            if( !strM.isEmpty() ) { // if( fM != 1.0 )
                strFctConvertName += strM;
                if( m_physValM.unit().isValid() ) {
                    strFctConvertName += " " + m_physValM.unit().symbol();
                }
                strFctConvertName += " * ";
            }
            strFctConvertName += strFormulaSymbolSrc + "/" + strUnitSymbolSrc;

            if( !strT.isEmpty() ) { // if( fT != 0.0 )
                if( fT < 0.0 ) {
                    strFctConvertName += " - ";
                }
                else {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT;
                if( m_physValT.unit().isValid() ) {
                    strFctConvertName += " " + m_physValT.unit().symbol();
                }
            }
            break;
        }
        case EFctConvert_mLOGxADDt:
        {
            strM = FormatOperand(fM);
            strT = FormatT(fabs(fT));

            if( !strM.isEmpty() ) { // if( fM != 1.0 )
                strFctConvertName += strM;
                if( m_physValM.unit().isValid() ) {
                    strFctConvertName += " " + m_physValM.unit().symbol();
                }
                strFctConvertName += " * ";
            }
            strFctConvertName += "log10(" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ")";

            if( !strT.isEmpty() ) { // if( fT != 0.0 )
                if( fT < 0.0 ) {
                    strFctConvertName += " - ";
                }
                else {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT;
                if( m_physValT.unit().isValid() ) {
                    strFctConvertName += " " + m_physValT.unit().symbol();
                }
            }
            break;
        }
        case EFctConvert_EXP__xADDt_DIVm_:
        {
            strM = FormatOperand(fM);
            strT = FormatT(fabs(fT));

            strFctConvertName += "10exp(";

            if( !strT.isEmpty() ) { // if( fT != 0.0 )
                strFctConvertName += "(";
            }
            strFctConvertName += strFormulaSymbolSrc + "/" + strUnitSymbolSrc;

            if( !strT.isEmpty() ) { // if( fT != 0.0 )
                if( fT < 0.0 ) {
                    strFctConvertName += " - ";
                }
                else {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT;
                if( m_physValT.unit().isValid() ) {
                    strFctConvertName += " " + m_physValT.unit().symbol();
                }
                strFctConvertName += ")";
            }
            if( !strM.isEmpty() ) { // if( fM != 1.0 )
                strFctConvertName += " / " + strM;
                if( m_physValM.unit().isValid() ) {
                    strFctConvertName += " " + m_physValM.unit().symbol();
                }
            }
            strFctConvertName += ")";
            break;
        }
        //case EFctConvert_xMULr:
        //{
        //    strFctConvertName +=
        //        "(" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "* (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ")";
        //    break;
        //}
        //case EFctConvert_xDIVr:
        //{
        //    strFctConvertName +=
        //        "(" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "/ (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ")";
        //    break;
        //}
        //case EFctConvert_SQRxDIVr:
        //{
        //    strFctConvertName +=
        //        "sqr(" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "/ (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ")";
        //    break;
        //}
        //case EFctConvert_SQRT_xMULr_:
        //{
        //    strFctConvertName +=
        //        c_strSymbolSquareRoot + "( (" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "* (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ") )";
        //    break;
        //}
        //case EFctConvert_SQRxMULr:
        //{
        //    strFctConvertName +=
        //        "sqr(" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "* (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ")";
        //    break;
        //}
        //case EFctConvert_SQRT_xDIVr_:
        //{
        //    strFctConvertName +=
        //        c_strSymbolSquareRoot + "( (" + strFormulaSymbolSrc + "/" + strUnitSymbolSrc + ") "
        //        "/ (" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ") )";
        //    break;
        //}
        //case EFctConvert_mMULxADDtADDkLOGr:
        //{
        //    strM = FormatM(m_fM);
        //    strT = FormatT(fabs(m_fT));
        //    strK = FormatOperand(fabs(m_fK));

        //    if( !strM.isEmpty() ) // if( m_fM != 1.0 )
        //    {
        //        strFctConvertName += strM + " * ";
        //    }
        //    strFctConvertName += strFormulaSymbolSrc + "/" + strUnitSymbolSrc;

        //    if( !strT.isEmpty() ) // if( m_fT != 0.0 )
        //    {
        //        if( m_fT < 0.0 )
        //        {
        //            strFctConvertName += " - ";
        //        }
        //        else // if( m_fT > 0.0 )
        //        {
        //            strFctConvertName += " + ";
        //        }
        //        strFctConvertName += strT;
        //    }
        //    if( !strK.isEmpty() ) // if( m_fK != 0.0 )
        //    {
        //        if( m_fK < 0.0 )
        //        {
        //            strFctConvertName += " - ";
        //        }
        //        else // if( m_fK > 0.0 )
        //        {
        //            strFctConvertName += " + ";
        //        }
        //        strFctConvertName += strK + " * log10(" + strFormulaSymbolRef + "/" + strUnitSymbolRef + ")";
        //    }
        //    break;
        //}
        case EFctConvert_Undefined:
        default:
        {
            strFctConvertName += "? " + strFormulaSymbolSrc + "/" + strUnitSymbolSrc;
            break;
        }
    }
    m_strFctConvertName = strFctConvertName;

} // buildFctConvertName

//------------------------------------------------------------------------------
bool SFctConvert::isValid() const
//------------------------------------------------------------------------------
{
    bool bValid = false;

    if( m_pPhysUnitSrc != nullptr && m_pPhysUnitDst != nullptr )
    {
        if( m_fctConvertType != EFctConvert_Undefined )
        {
            if( m_fctConvertType >= 0 && m_fctConvertType < EFctConvert_Count )
            {
                bValid = true;
            }
        }
    }
    return bValid;

} // isValid
