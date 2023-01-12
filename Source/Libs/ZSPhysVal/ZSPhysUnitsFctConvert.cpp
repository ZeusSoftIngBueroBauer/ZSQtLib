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

#include "ZSPhysVal/ZSPhysUnitsFctConvert.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysErrResult.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::System::Math;
using namespace ZS::PhysVal;


/*******************************************************************************
class CFctConvert
*******************************************************************************/

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CFctConvert::FormatM( double i_fM )
//------------------------------------------------------------------------------
{
    QString strM;
    int     iExponent;

    if( i_fM != 1.0 )
    {
        if( getExponentFromFactor(i_fM,&iExponent) )
        {
            if( iExponent != 0 )
            {
                strM = "1e" + QString::number(iExponent);
            }
        }
        else
        {
            strM = FormatOperand(i_fM);
        }
    }
    return strM;

} // FormatM

//------------------------------------------------------------------------------
QString CFctConvert::FormatT( double i_fT )
//------------------------------------------------------------------------------
{
    QString strT;

    if( i_fT != 0.0 )
    {
        int iExponent;

        if( getExponentFromFactor(i_fT,&iExponent) )
        {
            if( iExponent != 0 )
            {
                strT = "1e" + QString::number(iExponent);
            }
        }
        else
        {
            strT = FormatOperand(i_fT);
        }
    }
    return strT;

} // FormatT

//------------------------------------------------------------------------------
QString CFctConvert::FormatOperand( double i_fOp )
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
CFctConvert::CFctConvert() :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(nullptr),
    m_pPhysUnitDst(nullptr),
    m_pPhysUnitRef(nullptr),
    m_fctConvertType(EFctConvert_Undefined),
    m_strFctConvertName("---"),
    m_fM(1.0),
    m_fT(0.0),
    m_fK(0.0)
{
} // default ctor

//------------------------------------------------------------------------------
CFctConvert::CFctConvert(CFctConvert& i_other) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_other.m_pPhysUnitSrc),
    m_pPhysUnitDst(i_other.m_pPhysUnitDst),
    m_pPhysUnitRef(i_other.m_pPhysUnitRef),
    m_fctConvertType(i_other.m_fctConvertType),
    m_strFctConvertName(i_other.m_strFctConvertName),
    m_fM(i_other.m_fM),
    m_fT(i_other.m_fT),
    m_fK(i_other.m_fK)
{
} // copy ctor

//------------------------------------------------------------------------------
CFctConvert::CFctConvert(const CFctConvert& i_other) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_other.m_pPhysUnitSrc),
    m_pPhysUnitDst(i_other.m_pPhysUnitDst),
    m_pPhysUnitRef(i_other.m_pPhysUnitRef),
    m_fctConvertType(i_other.m_fctConvertType),
    m_strFctConvertName(i_other.m_strFctConvertName),
    m_fM(i_other.m_fM),
    m_fT(i_other.m_fT),
    m_fK(i_other.m_fK)
{
} // copy ctor

//------------------------------------------------------------------------------
CFctConvert::CFctConvert(CFctConvert&& i_other) :
//------------------------------------------------------------------------------
    m_pPhysUnitSrc(i_other.m_pPhysUnitSrc),
    m_pPhysUnitDst(i_other.m_pPhysUnitDst),
    m_pPhysUnitRef(i_other.m_pPhysUnitRef),
    m_fctConvertType(i_other.m_fctConvertType),
    m_strFctConvertName(i_other.m_strFctConvertName),
    m_fM(i_other.m_fM),
    m_fT(i_other.m_fT),
    m_fK(i_other.m_fK)
{
    i_other.m_pPhysUnitSrc = nullptr;
    i_other.m_pPhysUnitDst = nullptr;
    i_other.m_pPhysUnitRef = nullptr;
    i_other.m_fctConvertType = static_cast<EFctConvert>(0);
    //i_other.m_strFctConvertName;
    i_other.m_fM = 0.0;
    i_other.m_fT = 0.0;
    i_other.m_fK = 0.0;

} // move ctor

//------------------------------------------------------------------------------
CFctConvert::~CFctConvert()
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = nullptr;
    m_pPhysUnitDst = nullptr;
    m_pPhysUnitRef = nullptr;
    m_fctConvertType = static_cast<EFctConvert>(0);
    //m_strFctConvertName;
    m_fM = 0.0;
    m_fT = 0.0;
    m_fK = 0.0;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CFctConvert& CFctConvert::operator=(CFctConvert& i_other)
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = i_other.m_pPhysUnitSrc;
    m_pPhysUnitDst = i_other.m_pPhysUnitDst;
    m_pPhysUnitRef = i_other.m_pPhysUnitRef;
    m_fctConvertType = i_other.m_fctConvertType;
    m_strFctConvertName = i_other.m_strFctConvertName;
    m_fM = i_other.m_fM;
    m_fT = i_other.m_fT;
    m_fK = i_other.m_fK;

    return *this;
}

//------------------------------------------------------------------------------
CFctConvert& CFctConvert::operator=(const CFctConvert& i_other)
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = i_other.m_pPhysUnitSrc;
    m_pPhysUnitDst = i_other.m_pPhysUnitDst;
    m_pPhysUnitRef = i_other.m_pPhysUnitRef;
    m_fctConvertType = i_other.m_fctConvertType;
    m_strFctConvertName = i_other.m_strFctConvertName;
    m_fM = i_other.m_fM;
    m_fT = i_other.m_fT;
    m_fK = i_other.m_fK;

    return *this;
}

//------------------------------------------------------------------------------
CFctConvert& CFctConvert::operator=(CFctConvert&& i_other)
//------------------------------------------------------------------------------
{
    m_pPhysUnitSrc = i_other.m_pPhysUnitSrc;
    m_pPhysUnitDst = i_other.m_pPhysUnitDst;
    m_pPhysUnitRef = i_other.m_pPhysUnitRef;
    m_fctConvertType = i_other.m_fctConvertType;
    m_strFctConvertName = i_other.m_strFctConvertName;
    m_fM = i_other.m_fM;
    m_fT = i_other.m_fT;
    m_fK = i_other.m_fK;

    i_other.m_pPhysUnitSrc = nullptr;
    i_other.m_pPhysUnitDst = nullptr;
    i_other.m_pPhysUnitRef = nullptr;
    i_other.m_fctConvertType = static_cast<EFctConvert>(0);
    //i_other.m_strFctConvertName;
    i_other.m_fM = 0.0;
    i_other.m_fT = 0.0;
    i_other.m_fK = 0.0;

    return *this;

} // move operator

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CFctConvert::buildFctConvertName()
//------------------------------------------------------------------------------
{
    QString strM;
    QString strT;
    QString strK;
    QString strFctConvertName;

    if( m_pPhysUnitDst == nullptr )
    {
        strFctConvertName = "nullptr = ...";
        return;
    }

    CPhysSizeTreeEntry* pPhysSizeDst = m_pPhysUnitDst->physSize();

    strFctConvertName = pPhysSizeDst->getFormulaSymbol() + "/" + m_pPhysUnitDst->symbol() + " = ";

    if( m_pPhysUnitSrc == nullptr )
    {
        strFctConvertName += "nullptr";
        return;
    }

    CPhysSizeTreeEntry* pPhysSizeSrc = m_pPhysUnitSrc->physSize();

    switch( m_fctConvertType )
    {
        case EFctConvert_mMULxADDt:
        {
            strM = FormatM(m_fM);
            strT = FormatT(fabs(m_fT));

            if( !strM.isEmpty() ) // if( m_fM != 1.0 )
            {
                strFctConvertName += strM + " * ";
            }
            strFctConvertName += pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol();

            if( !strT.isEmpty() ) // if( m_fT == 0.0 )
            {
                if( m_fT < 0.0 )
                {
                    strFctConvertName += " - ";
                }
                else // if( m_fT > 0.0 )
                {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT;
            }
            break;
        }
        case EFctConvert_mLOGxADDt:
        {
            strM = FormatOperand(m_fM);
            strT = FormatT(fabs(m_fT));

            if( !strM.isEmpty() ) // if( m_fM != 1.0 )
            {
                strFctConvertName += strM + "*";
            }
            strFctConvertName += "log10(" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ")";

            if( !strT.isEmpty() ) // if( m_fT != 0.0 )
            {
                if( m_fT < 0.0 )
                {
                    strFctConvertName += " - ";
                }
                else // if( m_fT > 0.0 )
                {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT;
            }
            break;
        }
        case EFctConvert_EXP__xADDt_DIVm_:
        {
            strM = FormatOperand(m_fM);
            strT = FormatT(fabs(m_fT));

            strFctConvertName += "10exp(";

            if( !strT.isEmpty() ) // if( m_fT != 0.0 )
            {
                strFctConvertName += "(";
            }
            strFctConvertName += pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol();

            if( !strT.isEmpty() ) // if( m_fT != 0.0 )
            {
                if( m_fT < 0.0 )
                {
                    strFctConvertName += " - ";
                }
                else // if( m_fT > 0.0 )
                {
                    strFctConvertName += " + ";
                }
                strFctConvertName += strT + ")";
            }
            if( !strM.isEmpty() ) // if( m_fM != 1.0 )
            {
                strFctConvertName += " / " + strM;
            }
            strFctConvertName += ")";
            break;
        }
        case EFctConvert_xMULr:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    "(" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ") "
                    "* (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ")";
            }
            break;
        }
        case EFctConvert_xDIVr:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    "(" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ") "
                    "/ (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ")";
            }
            break;
        }
        case EFctConvert_SQRxDIVr:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    "(" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ")² "
                    "/ (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ")";
            }
            break;
        }
        case EFctConvert_SQRT_xMULr_:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    c_strSymbolSquareRoot + "( (" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ") "
                    "* (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ") )";
            }
            break;
        }
        case EFctConvert_SQRxMULr:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    "(" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ")² "
                    "* (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ")";
            }
            break;
        }
        case EFctConvert_SQRT_xDIVr_:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();
                strFctConvertName +=
                    c_strSymbolSquareRoot + "( (" + pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol() + ") "
                    "/ (" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ") )";
            }
            break;
        }
        case EFctConvert_mMULxADDtADDkLOGr:
        {
            if( m_pPhysUnitRef == nullptr )
            {
                strFctConvertName = "(PhysUnitRef = nullptr)";
            }
            else
            {
                CPhysSizeTreeEntry* pPhysSizeRef = m_pPhysUnitRef->physSize();

                strM = FormatM(m_fM);
                strT = FormatT(fabs(m_fT));
                strK = FormatOperand(fabs(m_fK));

                if( !strM.isEmpty() ) // if( m_fM != 1.0 )
                {
                    strFctConvertName += strM + " * ";
                }
                strFctConvertName += pPhysSizeSrc->getFormulaSymbol() + "/" + m_pPhysUnitSrc->symbol();

                if( !strT.isEmpty() ) // if( m_fT != 0.0 )
                {
                    if( m_fT < 0.0 )
                    {
                        strFctConvertName += " - ";
                    }
                    else // if( m_fT > 0.0 )
                    {
                        strFctConvertName += " + ";
                    }
                    strFctConvertName += strT;
                }
                if( !strK.isEmpty() ) // if( m_fK != 0.0 )
                {
                    if( m_fK < 0.0 )
                    {
                        strFctConvertName += " - ";
                    }
                    else // if( m_fK > 0.0 )
                    {
                        strFctConvertName += " + ";
                    }
                    strFctConvertName += strK + " * log10(" + pPhysSizeRef->getFormulaSymbol() + "/" + m_pPhysUnitRef->symbol() + ")";
                }
            }
            break;
        }
        case EFctConvert_Undefined:
        default:
        {
            strFctConvertName = "Undefined";
            break;
        }
    }
    m_strFctConvertName = strFctConvertName;

} // buildFctConvertName

//------------------------------------------------------------------------------
bool CFctConvert::isValid() const
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
